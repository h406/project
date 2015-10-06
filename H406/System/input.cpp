//==============================================================================
//
// input manager[input.cpp]
// Author : Yasuaki Yamashita : 2015/10/04
//
//==============================================================================

//******************************************************************************
// include
//******************************************************************************
#include "input.h"
#include "iInput.h"
#include "vInput.h"
#include "app.h"
#include "keyboard.h"

//==============================================================================
// input
//------------------------------------------------------------------------------
Input::Input() :_pInputDevice (nullptr) {
  // Inputオブジェクト生成判定
  if(_pInputDevice == nullptr) {
    // Inputオブジェクトの生成
    if(FAILED(DirectInput8Create(App::instance().getHInst(),
      DIRECTINPUT_VERSION,
      IID_IDirectInput8,
      (void **)&_pInputDevice,
      nullptr))) {
      // 生成失敗
      MessageBox(NULL,"Inputオブジェクトの生成に失敗しました","エラー",MB_OK | MB_ICONHAND);
      return;
    }
  }

  auto key = new KeyBoard();
  key->init(this);
  _inputList.push_back(key);

  setRepeatStartTime(1);
  setRepeatSleepTime(10);
}

//==============================================================================
// input
//------------------------------------------------------------------------------
Input::~Input() {
  for(auto obj : _inputList) {
    obj->uninit();
    SafeDelete(obj);
  }
  _inputList.clear();
}

void Input::update() {
  for(auto& obj : _inputList) {
    obj->update();
  }
}

//==============================================================================
//
//------------------------------------------------------------------------------
bool Input::isPress(VK_INPUT vk) const {
  for(auto& obj : _inputList) {
    if(obj->isPress(vk)) return true;
  }
  return false;
}
bool Input::isTrigger(VK_INPUT vk) const {
  for(auto& obj : _inputList) {
    if(obj->isTrigger(vk)) return true;
  }
  return false;
}
bool Input::isRelease(VK_INPUT vk) const {
  for(auto& obj : _inputList) {
    if(obj->isRelease(vk)) return true;
  }
  return false;
}
bool Input::isRepeat(VK_INPUT vk)  const {
  for(auto& obj : _inputList) {
    if(obj->isRepeat(vk)) return true;
  }
  return false;
}

//EOF