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

//*****************************************************************************
// ÉNÉâÉXêÈåæ
//*****************************************************************************
class iInput {
public:
  virtual void init(void) = 0;
  virtual void uninit(void) = 0;
  virtual void update(void) = 0;

  virtual bool isPress(VK_INPUT vk) const = 0;
  virtual bool isTrigger(VK_INPUT vk) const = 0;
  virtual bool isRelease(VK_INPUT vk) const = 0;
  virtual bool isRepeat(VK_INPUT vk) const = 0;
};
#endif
//EOF