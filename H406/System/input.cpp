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
#include "xInput.h"
#include "wsInput.h"
#include "directInput.h"


//==============================================================================
// input
//------------------------------------------------------------------------------
Input::Input() :_pInputDevice(nullptr) {
  // Inputオブジェクト生成判定
  if (_pInputDevice == nullptr) {
    // Inputオブジェクトの生成
    if (FAILED(DirectInput8Create(App::instance().getHInst(),
      DIRECTINPUT_VERSION,
      IID_IDirectInput8,
      (void **)&_pInputDevice,
      nullptr))) {
      // 生成失敗
      MessageBox(NULL, "Inputオブジェクトの生成に失敗しました", "エラー", MB_OK | MB_ICONHAND);
      return;
    }
  }
  
  auto key = new KeyBoard();
  key->init(this);
  _inputList.push_back(key);
  
  auto pad = new xInput();
  pad->init(this);
  _inputList.push_back(pad);

  auto ws = new WsInput();
  ws->init(this);
  _inputList.push_back(ws);

  auto dInput = new DirectInput();
  dInput->init(this);
  if (dInput->getEnableDevise(0)){
    _inputList.push_back(dInput);
  }

  setRepeatStartTime(15);
  setRepeatSleepTime(5);
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
bool Input::isPress(int id, VK_INPUT vk) const {
  for(auto& obj : _inputList) {
    if(obj->isPress(id, vk)) return true;
  }
  return false;
}
bool Input::isTrigger(int id, VK_INPUT vk) const {
  for(auto& obj : _inputList) {
    if (obj->isTrigger(id, vk)) return true;
  }
  return false;
}
bool Input::isRelease(int id, VK_INPUT vk) const {
  for(auto& obj : _inputList) {
    if (obj->isRelease(id, vk)) return true;
  }
  return false;
}
bool Input::isRepeat(int id, VK_INPUT vk)  const {
  for(auto& obj : _inputList) {
    if (obj->isRepeat(id, vk)) return true;
  }
  return false;
}

//EOF