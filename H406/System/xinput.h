//==============================================================================
//
// xinput[xinput.h]
// Author : Yoshiaki Nakajima : 2015/10/07
//
//==============================================================================

#pragma once

#ifndef _X_INPUT_H_
#define _X_INPUT_H_

#include "iInput.h"
#include "vInput.h"

#pragma comment(lib,"Xinput.lib")
#include <XInput.h>

#define CONTROL_DEVICE_NUMBER_MAX	(4)	
#define CONTROL_DEVICE_KEY_MAX	(14)
#define CONTROL_STICK_RANGE_MAX (32768)


//コントローラーの状態
struct CONTROLER_STATE
{
	XINPUT_STATE state;
	bool bConnected;
};
//*****************************************************************************
// クラス宣言
//*****************************************************************************
class xInput : public iInput{
public:
	xInput();
	void init(Input *);
	void uninit(void);
	void update(void);

  bool isPress(int id,VK_INPUT vk) const;
  bool isTrigger(int id,VK_INPUT vk) const;
  bool isRelease(int id,VK_INPUT vk) const;
  bool isRepeat(int id,VK_INPUT vk) const;

	// デバイス接続中か調べる
  bool getConnectDevice(int nId) { return _controlDevice[nId].bConnected ? true : false; }

	// デバイスステートの更新
	HRESULT UpdateControllerState(int nId);

protected:
	// キー情報ワーク
	BYTE _aButtons[CONTROL_DEVICE_NUMBER_MAX][CONTROL_DEVICE_KEY_MAX];
	BYTE _aButtonsTrigger[CONTROL_DEVICE_NUMBER_MAX][CONTROL_DEVICE_KEY_MAX];
	BYTE _aButtonsRelease[CONTROL_DEVICE_NUMBER_MAX][CONTROL_DEVICE_KEY_MAX];
	BYTE _aButtonsRepeat[CONTROL_DEVICE_NUMBER_MAX][CONTROL_DEVICE_KEY_MAX];


private:
	CONTROLER_STATE _controlDevice[CONTROL_DEVICE_NUMBER_MAX];
	XINPUT_STATE _oldControlDevice[CONTROL_DEVICE_NUMBER_MAX];
	LPDIRECTINPUTDEVICE8 _pKeyboradDevice;
};
#endif
//EOF