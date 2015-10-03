//==============================================================================
//
// input[input.h]
// Author : Yasuaki Yamashita : 2015/10/04
//
//==============================================================================

#pragma once

#ifndef _INPUT_H_
#define _INPUT_H_

#include "vInput.h"

class iInput;

//==============================================================================
// input manager
//------------------------------------------------------------------------------
class Input {
public:
  Input();
  ~Input();

  void update();

  bool isPress(VK_INPUT vk) const;
  bool isTrigger(VK_INPUT vk) const;
  bool isRelease(VK_INPUT vk) const;
  bool isRepeat(VK_INPUT vk) const;

private:
  vector<iInput*> _inputList;

};

#endif
//EOF