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


//==============================================================================
// input
//------------------------------------------------------------------------------
Input::Input() :_pInputDevice (nullptr) {
  // Input�I�u�W�F�N�g��������
  if(_pInputDevice == nullptr) {
    // Input�I�u�W�F�N�g�̐���
    if(FAILED(DirectInput8Create(App::instance().getHInst(),
      DIRECTINPUT_VERSION,
      IID_IDirectInput8,
      (void **)&_pInputDevice,
      nullptr))) {
      // �������s
      MessageBox(NULL,"Input�I�u�W�F�N�g�̐����Ɏ��s���܂���","�G���[",MB_OK | MB_ICONHAND);
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