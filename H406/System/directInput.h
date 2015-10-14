//==============================================================================
//
// DirectInput[DirectInput.h]
// Author : Masato Masuda : 2015/10/10
//
//==============================================================================
#pragma once

#ifndef _INPUT_DIRECT_H_
#define _INPUT_DIRECT_H_

#include "iInput.h"
#include "vInput.h"

//==============================================================================
// 
//==============================================================================
class DirectInput : public iInput {
public :

  typedef enum{
    DIRECT_INPUT_PAD_UP = -4, // 方向キー上(dijs.IY<0)
    DIRECT_INPUT_PAD_DOWN,    // 方向キー下(dijs.IY>0)
    DIRECT_INPUT_PAD_LEFT,    // 方向キー左(dijs.IX<0)
    DIRECT_INPUT_PAD_RIGHT,   // 方向キー右(dijs.IX>0)
    DIRECT_INPUT_PAD_A,       // Aボタン(dijs.rgbButtons[0]&0x80)
    DIRECT_INPUT_PAD_B,       // Bボタン(dijs.rgbButtons[1]&0x80)
    DIRECT_INPUT_PAD_C,       // Cボタン(dijs.rgbButtons[2]&0x80)
    DIRECT_INPUT_PAD_X,       // Xボタン(dijs.rgbButtons[3]&0x80)
    DIRECT_INPUT_PAD_Y,       // Yボタン(dijs.rgbButtons[4]&0x80)
    DIRECT_INPUT_PAD_Z,       // Zボタン(dijs.rgbButtons[5]&0x80)
    DIRECT_INPUT_PAD_L,       // Lボタン(dijs.rgbButtons[6]&0x80)
    DIRECT_INPUT_PAD_R,       // Rボタン(dijs.rgbButtons[7]&0x80)
    DIRECT_INPUT_PAD_START,   // STARTボタン(dijs.rgbButtons[8]&0x80)
    DIRECT_INPUT_PAD_M,       // Mボタン(dijs.rgbButtons[9]&0x80)
    DIRECT_INPUT_PAD_MAX,     // 最大数
  } DIRECT_INPUT_PAD;

  typedef enum{
    DIRECT_PP_UP = (0x1),
    DIRECT_PP_DOWN = (0x4),
    DIRECT_PP_LEFT = (0x8),
    DIRECT_PP_RIGHT = (0x2),
    DIRECT_PP_MAX = 4
  } DIRECT_PP;

  DirectInput(void);

  void init(Input* input);
  void uninit(void);
  void update(void);

  bool isPress(int id, VK_INPUT vk) const;
  bool isTrigger(int id, VK_INPUT vk) const;
  bool isRelease(int id, VK_INPUT vk) const;
  bool isRepeat(int id, VK_INPUT vk) const;

  // デバイスがあるか調べる
  bool getEnableDevise(int id) { return _pDirectInputDevice[id] ? true : false; }

  // パッド最大数
  static const int INPUT_MAX = 2;

private:

  // 問い合わせ用コールバック関数
  static bool CALLBACK EnumDevicesCallback(const DIDEVICEINSTANCE* didInstance, void* ref);
  static bool CALLBACK EnumAxesCallback(const DIDEVICEOBJECTINSTANCE* didoi, void* ref);

  // 十字キー用
  inline bool isCrossKey(int id, DIRECT_INPUT_PAD vk) const;

  // デバイスオブジェクト
  LPDIRECTINPUTDEVICE8 _pDirectInputDevice[INPUT_MAX];

  // パッドのID
  static GUID _padDiscrimination[INPUT_MAX];
  static int _padCount;

  // コールバックでInputクラスのインターフェイス使いたかった
  static Input*  _input;

  // パッドデータ
  DIJOYSTATE2				_padData[INPUT_MAX];
  DIJOYSTATE2				_padDataLast[INPUT_MAX];

  // リピート用
  BYTE _padRepeat[DIRECT_INPUT_PAD_MAX][INPUT_MAX];
  BYTE _padCrossRepeat[DIRECT_PP_MAX][INPUT_MAX];

  // 仮想キーボード配列
  DIRECT_INPUT_PAD _inputVTKey[int(VK_INPUT::MAX)];
};

#endif

//EOF