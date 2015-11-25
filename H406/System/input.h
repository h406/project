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

  bool isPress(int id, VK_INPUT vk) const;
  bool isTrigger(int id, VK_INPUT vk) const;
  bool isRelease(int id, VK_INPUT vk) const;
  bool isRepeat(int id, VK_INPUT vk) const;

  // インプットデバイス
  LPDIRECTINPUT8 getInputDevice(void) const { return _pInputDevice; }

  // リピート用タイム
  int getRepeatStartTime(void) const { return _nRepeatStartTime; }
  int getRepeatSleepTime(void) const { return _nRepeatSleepTime; }

  // リピート用セッタ
  void setRepeatStartTime(int time) { _nRepeatStartTime = time; }
  void setRepeatSleepTime(int time) { _nRepeatSleepTime = time; }

private:

  vector<iInput*> _inputList;

  // インプット用デバイス
  LPDIRECTINPUT8 _pInputDevice;

  // リピート入力が始まるまでの時間(フレーム数)
  int _nRepeatStartTime;
  // リピート入力が繰り返すまでの時間(フレーム数)
  int _nRepeatSleepTime;
};

#endif
//EOF