//==============================================================================
//
// iInput[iInput.h]
// Author : Yasuaki Yamashita : 2015/10/04
//
//==============================================================================

#pragma once

#ifndef _IINPUT_H_
#define _IINPUT_H_

#include "vInput.h"

class Input;

//*****************************************************************************
// ÉNÉâÉXêÈåæ
//*****************************************************************************
class iInput {
public:
  virtual void init(Input *) = 0;
  virtual void uninit(void) = 0;
  virtual void update(void) = 0;

  virtual bool isPress(int id, VK_INPUT vk) const = 0;
  virtual bool isTrigger(int id, VK_INPUT vk) const = 0;
  virtual bool isRelease(int id, VK_INPUT vk) const = 0;
  virtual bool isRepeat(int id, VK_INPUT vk) const = 0;
};
#endif
//EOF