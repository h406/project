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

  bool isPress(VK_INPUT vk) const;
  bool isTrigger(VK_INPUT vk) const;
  bool isRelease(VK_INPUT vk) const;
  bool isRepeat(VK_INPUT vk) const;

  // デバイスがあるか調べる
  bool getEnableDevise(void) { return _pKeyboradDevice ? true : false; }

protected:
  // 仮想キーボード配列
  BYTE _InputVTKey[int(VK_INPUT::MAX)];
  // キー情報ワーク
  BYTE _aKeyState[256];
  // リピートカウンタ
  BYTE _aKeyRepeatCnt[256];

private:
  LPDIRECTINPUTDEVICE8 _pKeyboradDevice;

};

#endif
//EOF