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

//==============================================================================
// input
//------------------------------------------------------------------------------
Input::Input() {

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