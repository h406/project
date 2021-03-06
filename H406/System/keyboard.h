//==============================================================================
//
// keyboard[keyboard.h]
// Author : Yasuaki Yamashita : 2015/10/04
//
//==============================================================================

#pragma once

#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include "iInput.h"
#include "vInput.h"

//==============================================================================
// 
//==============================================================================
class KeyBoard : public iInput {
public :
  KeyBoard();

  void init(Input* input);
  void uninit(void);
  void update(void);

  bool isPress(int id, VK_INPUT vk) const;
  bool isTrigger(int id, VK_INPUT vk) const;
  bool isRelease(int id, VK_INPUT vk) const;
  bool isRepeat(int id, VK_INPUT vk) const;

  // デバイスがあるか調べる
  bool getEnableDevise(void) { return _pKeyboradDevice ? true : false; }

protected:
  // 仮想キーボード配列
  BYTE _InputVTKey[4][int(VK_INPUT::MAX)];
  
  BYTE _aKeyState[256];
  BYTE _aKeyTrigger[256];
  BYTE _aKeyRelease[256];
  BYTE _aKeyRepeat[256];

private:
  LPDIRECTINPUTDEVICE8 _pKeyboradDevice;

};

#endif
//EOF