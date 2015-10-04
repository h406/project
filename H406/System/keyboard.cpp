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
  // キーボード情報未設定
  const BYTE KEYBOARD_NONE = (0x00);

  // キーボードプレス情報
  const BYTE KEYBOARD_PRESS = (0x80);

  // キーボードトリガ情報
  const BYTE KEYBOARD_TRIGGER = (0x40);

  // キーボードリリース情報
  const BYTE KEYBOARD_RELEASE = (0x20);

  // キーボードリピート情報
  const BYTE KEYBOARD_REPEAT = (0x10);

  // 前回キーボードプレス情報
  const BYTE KEYBOARD_PRESS_OLD = (0x08);

  // 前回キーボードトリガ情報
  const BYTE KEYBOARD_TRIGGER_OLD = (0x04);

  // 前回キーボードリリース情報
  const BYTE KEYBOARD_RELEASE_OLD = (0x02);

  // 前回キーボードリピート情報
  const BYTE KEYBOARD_REPEAT_OLD = (0x01);
}

KeyBoard::KeyBoard()
  :_pKeyboradDevice(nullptr) {
}

void KeyBoard::init(Input* input) {
  // キーボードデバイスオブジェクト生成
  if(FAILED(input->getInputDevice()->CreateDevice(
    GUID_SysKeyboard,
    &_pKeyboradDevice,
    NULL))) {
    // 生成失敗
    MessageBox(NULL,"キーボードデバイスオブジェクトの生成に失敗しました","エラー",MB_OK | MB_ICONHAND);
    return;
  }

  // データフォーマットの設定
  if(FAILED(_pKeyboradDevice->SetDataFormat(&c_dfDIKeyboard))) {
    // 設定失敗
    MessageBox(NULL,"キーボードデータフォーマットの設定に失敗しました","エラー",MB_OK | MB_ICONHAND);
    return;
  }

  // 協調モードの設定
  if(FAILED(_pKeyboradDevice->SetCooperativeLevel(App::instance().getHWnd(),(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))) {
    // 生成失敗
    MessageBox(NULL,"キーボード協調モードの設定に失敗しました","エラー",MB_OK | MB_ICONHAND);
    return;
  }

  // キーボードへのアクセス権の取得
  _pKeyboradDevice->Acquire();

  // 配列のクリア
  memset(_aKeyState,0,sizeof(_aKeyState));

  // virtual用配列
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
  // アクセス権開放
  if(_pKeyboradDevice) _pKeyboradDevice->Unacquire();
  SafeRelease(_pKeyboradDevice);
}

void KeyBoard::update() {
  // 入力情報格納ワーク
  BYTE aKeyState[256];
  // ループカウンタ
  int nCntKey;

  const int repeatStartTime = App::instance().getInput()->getRepeatStartTime();
  const int repeatSleepTime = App::instance().getInput()->getRepeatSleepTime();

  // デバイス情報の取得
  if(SUCCEEDED(_pKeyboradDevice->GetDeviceState(sizeof(aKeyState),&aKeyState[0]))) {
    for(nCntKey = 0; nCntKey < 256; nCntKey++) {
      // 前回情報の保存
      _aKeyState[nCntKey] = (unsigned)(_aKeyState[nCntKey]) >> 4;

      // トリガ情報の作成
      aKeyState[nCntKey] += ((((_aKeyState[nCntKey] & KEYBOARD_PRESS_OLD) << 4) ^ (aKeyState[nCntKey] & KEYBOARD_PRESS)) & (aKeyState[nCntKey] & KEYBOARD_PRESS)) >> 1;

      // リリース情報の作成
      aKeyState[nCntKey] += ((((_aKeyState[nCntKey] & KEYBOARD_PRESS_OLD) << 4) ^ (aKeyState[nCntKey] & KEYBOARD_PRESS)) & ((_aKeyState[nCntKey] & KEYBOARD_PRESS_OLD) << 4)) >> 2;

      // リピート情報の作成
      _aKeyRepeatCnt[nCntKey] = (_aKeyRepeatCnt[nCntKey] * (aKeyState[nCntKey] / KEYBOARD_PRESS)) + 1;
      aKeyState[nCntKey] += (BYTE)(((_aKeyRepeatCnt[nCntKey] / repeatStartTime) << 4) | ((aKeyState[nCntKey] & KEYBOARD_TRIGGER) >> 2));
      if(_aKeyRepeatCnt[nCntKey] / repeatStartTime >= 1) {
        _aKeyRepeatCnt[nCntKey] -= (BYTE)repeatSleepTime;
      }

      // グローバルへ移す
      _aKeyState[nCntKey] += aKeyState[nCntKey];
    }
  }
  else {

    // 配列のクリア
    memset(_aKeyState,0,sizeof(_aKeyState));

    // キーボードへのアクセス権の取得
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