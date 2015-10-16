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
  memset(_InputVTKey,0,sizeof(_InputVTKey));

  // virtual用配列
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
  // アクセス権開放
  if(_pKeyboradDevice) _pKeyboradDevice->Unacquire();
  SafeRelease(_pKeyboradDevice);
}

void KeyBoard::update() {
  // 入力情報格納ワーク
  BYTE aKeyState[256];
  // const int repeatStartTime = App::instance().getInput()->getRepeatStartTime();
  // const int repeatSleepTime = App::instance().getInput()->getRepeatSleepTime();

  // デバイス情報の取得
  if(SUCCEEDED(_pKeyboradDevice->GetDeviceState(sizeof(aKeyState),&aKeyState[0]))) {
    for(int nCntKey = 0; nCntKey < 256; nCntKey++) {
      // トリガー作成
      _aKeyTrigger[nCntKey] = (aKeyState[nCntKey] ^ _aKeyState[nCntKey]) & aKeyState[nCntKey];
      // リリース作成
      _aKeyRelease[nCntKey] = (aKeyState[nCntKey] ^ _aKeyState[nCntKey]) & ~aKeyState[nCntKey];
      // プレス作成
      _aKeyState[nCntKey] = aKeyState[nCntKey];

      // リピート作成
      if(_aKeyState[nCntKey] & 0x80) {
        //0x80で止めておく
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
    // 配列のクリア
    memset(_aKeyState,0,sizeof(_aKeyState));

    // キーボードへのアクセス権の取得
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