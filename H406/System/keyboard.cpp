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
#include "app.h"

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
  memset(_InputVTKey,0,sizeof(_InputVTKey));

  // virtual�p�z��
  _InputVTKey[0][(int)VK_INPUT::LEFT] = DIK_LEFT;
  _InputVTKey[0][(int)VK_INPUT::RIGHT] = DIK_RIGHT;
  _InputVTKey[0][(int)VK_INPUT::UP] = DIK_UP;
  _InputVTKey[0][(int)VK_INPUT::DOWN] = DIK_DOWN;
  _InputVTKey[0][(int)VK_INPUT::_1] = DIK_RSHIFT;
  _InputVTKey[0][(int)VK_INPUT::_2] = DIK_SPACE;
  // _InputVTKey[0][(int)VK_INPUT::_3] = DIK_S;
  // _InputVTKey[0][(int)VK_INPUT::_4] = DIK_A;
  // _InputVTKey[0][(int)VK_INPUT::_5] = DIK_LSHIFT;
  // _InputVTKey[0][(int)VK_INPUT::_6] = DIK_F;
  // _InputVTKey[0][(int)VK_INPUT::_7] = DIK_Q;
  // _InputVTKey[0][(int)VK_INPUT::_8] = DIK_W;
  // _InputVTKey[0][(int)VK_INPUT::_9] = DIK_E;
  // _InputVTKey[0][(int)VK_INPUT::_10] = DIK_C;
  _InputVTKey[1][(int)VK_INPUT::LEFT] = DIK_A;
  _InputVTKey[1][(int)VK_INPUT::RIGHT] = DIK_D;
  _InputVTKey[1][(int)VK_INPUT::UP] = DIK_W;
  _InputVTKey[1][(int)VK_INPUT::DOWN] = DIK_S;
  _InputVTKey[1][(int)VK_INPUT::_1] = DIK_LSHIFT;

  memset(_aKeyState,0,sizeof(_aKeyState));
  memset(_aKeyTrigger,0,sizeof(_aKeyTrigger));
  memset(_aKeyRelease,0,sizeof(_aKeyRelease));
  memset(_aKeyRepeat,0,sizeof(_aKeyRepeat));
}

void KeyBoard::uninit() {
  // �A�N�Z�X���J��
  if(_pKeyboradDevice) _pKeyboradDevice->Unacquire();
  SafeRelease(_pKeyboradDevice);
}

void KeyBoard::update() {
  // ���͏��i�[���[�N
  BYTE aKeyState[256];
  // const int repeatStartTime = App::instance().getInput()->getRepeatStartTime();
  // const int repeatSleepTime = App::instance().getInput()->getRepeatSleepTime();

  // �f�o�C�X���̎擾
  if(SUCCEEDED(_pKeyboradDevice->GetDeviceState(sizeof(aKeyState),&aKeyState[0]))) {
    for(int nCntKey = 0; nCntKey < 256; nCntKey++) {
      // �g���K�[�쐬
      _aKeyTrigger[nCntKey] = (aKeyState[nCntKey] ^ _aKeyState[nCntKey]) & aKeyState[nCntKey];
      // �����[�X�쐬
      _aKeyRelease[nCntKey] = (aKeyState[nCntKey] ^ _aKeyState[nCntKey]) & ~aKeyState[nCntKey];
      // �v���X�쐬
      _aKeyState[nCntKey] = aKeyState[nCntKey];

      // ���s�[�g�쐬
      if(_aKeyState[nCntKey] & 0x80) {
        //0x80�Ŏ~�߂Ă���
        if(_aKeyRepeat[nCntKey] < 0x80) {
          _aKeyRepeat[nCntKey]++;
        }
      }
      else {
        _aKeyRepeat[nCntKey] = 0;
      }
    }
  }
  else {
    // �z��̃N���A
    memset(_aKeyState,0,sizeof(_aKeyState));

    // �L�[�{�[�h�ւ̃A�N�Z�X���̎擾
    _pKeyboradDevice->Acquire();
  }
}

bool KeyBoard::isPress(int id, VK_INPUT vk) const {
  return _aKeyState[_InputVTKey[id][(int)vk]] & 0x80 ? true : false;
}

bool KeyBoard::isTrigger(int id, VK_INPUT vk) const {
  return _aKeyTrigger[_InputVTKey[id][(int)vk]] & 0x80 ? true : false;
}

bool KeyBoard::isRelease(int id, VK_INPUT vk) const {
  return _aKeyRelease[_InputVTKey[id][(int)vk]] & 0x80 ? true : false;
}

bool KeyBoard::isRepeat(int id, VK_INPUT vk) const {
  const int repeatStartTime = App::instance().getInput()->getRepeatStartTime();
  return ((_aKeyRepeat[_InputVTKey[id][(int)vk]] > repeatStartTime) || (isTrigger(id,vk))) ? true : false;
}

//EOF