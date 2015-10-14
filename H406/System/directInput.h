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
    DIRECT_INPUT_PAD_UP = -4, // �����L�[��(dijs.IY<0)
    DIRECT_INPUT_PAD_DOWN,    // �����L�[��(dijs.IY>0)
    DIRECT_INPUT_PAD_LEFT,    // �����L�[��(dijs.IX<0)
    DIRECT_INPUT_PAD_RIGHT,   // �����L�[�E(dijs.IX>0)
    DIRECT_INPUT_PAD_A,       // A�{�^��(dijs.rgbButtons[0]&0x80)
    DIRECT_INPUT_PAD_B,       // B�{�^��(dijs.rgbButtons[1]&0x80)
    DIRECT_INPUT_PAD_C,       // C�{�^��(dijs.rgbButtons[2]&0x80)
    DIRECT_INPUT_PAD_X,       // X�{�^��(dijs.rgbButtons[3]&0x80)
    DIRECT_INPUT_PAD_Y,       // Y�{�^��(dijs.rgbButtons[4]&0x80)
    DIRECT_INPUT_PAD_Z,       // Z�{�^��(dijs.rgbButtons[5]&0x80)
    DIRECT_INPUT_PAD_L,       // L�{�^��(dijs.rgbButtons[6]&0x80)
    DIRECT_INPUT_PAD_R,       // R�{�^��(dijs.rgbButtons[7]&0x80)
    DIRECT_INPUT_PAD_START,   // START�{�^��(dijs.rgbButtons[8]&0x80)
    DIRECT_INPUT_PAD_M,       // M�{�^��(dijs.rgbButtons[9]&0x80)
    DIRECT_INPUT_PAD_MAX,     // �ő吔
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

  // �f�o�C�X�����邩���ׂ�
  bool getEnableDevise(int id) { return _pDirectInputDevice[id] ? true : false; }

  // �p�b�h�ő吔
  static const int INPUT_MAX = 2;

private:

  // �₢���킹�p�R�[���o�b�N�֐�
  static bool CALLBACK EnumDevicesCallback(const DIDEVICEINSTANCE* didInstance, void* ref);
  static bool CALLBACK EnumAxesCallback(const DIDEVICEOBJECTINSTANCE* didoi, void* ref);

  // �\���L�[�p
  inline bool isCrossKey(int id, DIRECT_INPUT_PAD vk) const;

  // �f�o�C�X�I�u�W�F�N�g
  LPDIRECTINPUTDEVICE8 _pDirectInputDevice[INPUT_MAX];

  // �p�b�h��ID
  static GUID _padDiscrimination[INPUT_MAX];
  static int _padCount;

  // �R�[���o�b�N��Input�N���X�̃C���^�[�t�F�C�X�g����������
  static Input*  _input;

  // �p�b�h�f�[�^
  DIJOYSTATE2				_padData[INPUT_MAX];
  DIJOYSTATE2				_padDataLast[INPUT_MAX];

  // ���s�[�g�p
  BYTE _padRepeat[DIRECT_INPUT_PAD_MAX][INPUT_MAX];
  BYTE _padCrossRepeat[DIRECT_PP_MAX][INPUT_MAX];

  // ���z�L�[�{�[�h�z��
  DIRECT_INPUT_PAD _inputVTKey[int(VK_INPUT::MAX)];
};

#endif

//EOF