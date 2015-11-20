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

  // �C���v�b�g�f�o�C�X
  LPDIRECTINPUT8 getInputDevice(void) const { return _pInputDevice; }

  // ���s�[�g�p�^�C��
  int getRepeatStartTime(void) const { return _nRepeatStartTime; }
  int getRepeatSleepTime(void) const { return _nRepeatSleepTime; }

  // ���s�[�g�p�Z�b�^
  void setRepeatStartTime(int time) { _nRepeatStartTime = time; }
  void setRepeatSleepTime(int time) { _nRepeatSleepTime = time; }

private:

  vector<iInput*> _inputList;

  // �C���v�b�g�p�f�o�C�X
  LPDIRECTINPUT8 _pInputDevice;

  // ���s�[�g���͂��n�܂�܂ł̎���(�t���[����)
  int _nRepeatStartTime;
  // ���s�[�g���͂��J��Ԃ��܂ł̎���(�t���[����)
  int _nRepeatSleepTime;
};

#endif
//EOF