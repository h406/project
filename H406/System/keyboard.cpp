//==============================================================================
//
// KeyBoard[KeyBoard.cpp]
// Author : Yasuaki Yamashita : 2015/10/04
//
//==============================================================================

//******************************************************************************
// include
//******************************************************************************
#include "KeyBoard.h"
#include "input.h"
#include "vInput.h"

namespace {
  // �L�[�{�[�h��񖢐ݒ�
  const BYTE KEYBOARD_NONE = (0x00);

  // �L�[�{�[�h�v���X���
  const BYTE KEYBOARD_PRESS = (0x80);

  // �L�[�{�[�h�g���K���
  const BYTE KEYBOARD_TRIGGER = (0x40);

  // �L�[�{�[�h�����[�X���
  const BYTE KEYBOARD_RELEASE = (0x20);

  // �L�[�{�[�h���s�[�g���
  const BYTE KEYBOARD_REPEAT = (0x10);

  // �O��L�[�{�[�h�v���X���
  const BYTE KEYBOARD_PRESS_OLD = (0x08);

  // �O��L�[�{�[�h�g���K���
  const BYTE KEYBOARD_TRIGGER_OLD = (0x04);

  // �O��L�[�{�[�h�����[�X���
  const BYTE KEYBOARD_RELEASE_OLD = (0x02);

  // �O��L�[�{�[�h���s�[�g���
  const BYTE KEYBOARD_REPEAT_OLD = (0x01);
}

KeyBoard::KeyBoard()
  :_pKeyboradDevice(nullptr) {
}

void KeyBoard::init(Input* input) {
  // �L�[�{�[�h�f�o�C�X�I�u�W�F�N�g����
  if(FAILED(input->getInputDevice()->CreateDevice(
    GUID_SysKeyboard,
    &_pKeyboradDevice,
    NULL))) {
    // �������s
    MessageBox(NULL,"�L�[�{�[�h�f�o�C�X�I�u�W�F�N�g�̐����Ɏ��s���܂���","�G���[",MB_OK | MB_ICONHAND);
    return;
  }

  // �f�[�^�t�H�[�}�b�g�̐ݒ�
  if(FAILED(_pKeyboradDevice->SetDataFormat(&c_dfDIKeyboard))) {
    // �ݒ莸�s
    MessageBox(NULL,"�L�[�{�[�h�f�[�^�t�H�[�}�b�g�̐ݒ�Ɏ��s���܂���","�G���[",MB_OK | MB_ICONHAND);
    return;
  }

  // �������[�h�̐ݒ�
  if(FAILED(_pKeyboradDevice->SetCooperativeLevel(App::instance().getHWnd(),(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))) {
    // �������s
    MessageBox(NULL,"�L�[�{�[�h�������[�h�̐ݒ�Ɏ��s���܂���","�G���[",MB_OK | MB_ICONHAND);
    return;
  }

  // �L�[�{�[�h�ւ̃A�N�Z�X���̎擾
  _pKeyboradDevice->Acquire();

  // �z��̃N���A
  memset(_aKeyState,0,sizeof(_aKeyState));

  // virtual�p�z��
  _InputVTKey[(int)VK_INPUT::LEFT] = DIK_LEFT;
  _InputVTKey[(int)VK_INPUT::RIGHT] = DIK_RIGHT;
  _InputVTKey[(int)VK_INPUT::UP] = DIK_UP;
  _InputVTKey[(int)VK_INPUT::DOWN] = DIK_DOWN;
  _InputVTKey[(int)VK_INPUT::_1] = DIK_SPACE;
  _InputVTKey[(int)VK_INPUT::_2] = DIK_D;
  _InputVTKey[(int)VK_INPUT::_3] = DIK_S;
  _InputVTKey[(int)VK_INPUT::_4] = DIK_A;
  _InputVTKey[(int)VK_INPUT::_5] = DIK_LSHIFT;
  _InputVTKey[(int)VK_INPUT::_6] = DIK_F;
  _InputVTKey[(int)VK_INPUT::_7] = DIK_Q;
  _InputVTKey[(int)VK_INPUT::_8] = DIK_W;
  _InputVTKey[(int)VK_INPUT::_9] = DIK_E;
  _InputVTKey[(int)VK_INPUT::_10] = DIK_C;
}

void KeyBoard::uninit() {
  // �A�N�Z�X���J��
  if(_pKeyboradDevice) _pKeyboradDevice->Unacquire();
  SafeRelease(_pKeyboradDevice);
}

void KeyBoard::update() {
  // ���͏��i�[���[�N
  BYTE aKeyState[256];
  // ���[�v�J�E���^
  int nCntKey;

  const int repeatStartTime = App::instance().getInput()->getRepeatStartTime();
  const int repeatSleepTime = App::instance().getInput()->getRepeatSleepTime();

  // �f�o�C�X���̎擾
  if(SUCCEEDED(_pKeyboradDevice->GetDeviceState(sizeof(aKeyState),&aKeyState[0]))) {
    for(nCntKey = 0; nCntKey < 256; nCntKey++) {
      // �O����̕ۑ�
      _aKeyState[nCntKey] = (unsigned)(_aKeyState[nCntKey]) >> 4;

      // �g���K���̍쐬
      aKeyState[nCntKey] += ((((_aKeyState[nCntKey] & KEYBOARD_PRESS_OLD) << 4) ^ (aKeyState[nCntKey] & KEYBOARD_PRESS)) & (aKeyState[nCntKey] & KEYBOARD_PRESS)) >> 1;

      // �����[�X���̍쐬
      aKeyState[nCntKey] += ((((_aKeyState[nCntKey] & KEYBOARD_PRESS_OLD) << 4) ^ (aKeyState[nCntKey] & KEYBOARD_PRESS)) & ((_aKeyState[nCntKey] & KEYBOARD_PRESS_OLD) << 4)) >> 2;

      // ���s�[�g���̍쐬
      _aKeyRepeatCnt[nCntKey] = (_aKeyRepeatCnt[nCntKey] * (aKeyState[nCntKey] / KEYBOARD_PRESS)) + 1;
      aKeyState[nCntKey] += (BYTE)(((_aKeyRepeatCnt[nCntKey] / repeatStartTime) << 4) | ((aKeyState[nCntKey] & KEYBOARD_TRIGGER) >> 2));
      if(_aKeyRepeatCnt[nCntKey] / repeatStartTime >= 1) {
        _aKeyRepeatCnt[nCntKey] -= (BYTE)repeatSleepTime;
      }

      // �O���[�o���ֈڂ�
      _aKeyState[nCntKey] += aKeyState[nCntKey];
    }
  }
  else {

    // �z��̃N���A
    memset(_aKeyState,0,sizeof(_aKeyState));

    // �L�[�{�[�h�ւ̃A�N�Z�X���̎擾
    _pKeyboradDevice->Acquire();
  }
}

bool KeyBoard::isPress(VK_INPUT vk) const {
  return (_aKeyState[_InputVTKey[(int)vk]] & KEYBOARD_PRESS) ? true : false;
}


bool KeyBoard::isTrigger(VK_INPUT vk) const {
  return (_aKeyState[_InputVTKey[(int)vk]] & KEYBOARD_TRIGGER) ? true : false;
}


bool KeyBoard::isRelease(VK_INPUT vk) const {
  return (_aKeyState[_InputVTKey[(int)vk]] & KEYBOARD_RELEASE) ? true : false;
}

bool KeyBoard::isRepeat(VK_INPUT vk) const {
  return (_aKeyState[_InputVTKey[(int)vk]] & KEYBOARD_REPEAT) ? true : false;
}

//EOF