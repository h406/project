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

#define CONTROL_DEVICE_MAX	(2)
#define CONTROL_DEVICE_KEY_MAX	(14)

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

	bool isPress(VK_INPUT vk)const;
	bool isTrigger(VK_INPUT vk)const;
	bool isRelease(VK_INPUT vk)const;
	bool isRepeat(VK_INPUT vk)const;

	// デバイス接続中か調べる Numberはコントローラ番号
	bool getConnectDevice() { return _controlDevice[_nControlDeviceNumber].bConnected ? true : false; }

	// デバイスステートの更新
	HRESULT UpdateControllerState(void);

	// キー操作を受け取るデバイス番号を変更
	void setControlDevice(int nNumber){ _nControlDeviceNumber = nNumber; }

protected:
	// キー情報ワーク
	BYTE _wButtons[CONTROL_DEVICE_MAX][CONTROL_DEVICE_KEY_MAX];
	// リピートカウンタ
	int _wButtonsRepeatCnt[CONTROL_DEVICE_MAX][CONTROL_DEVICE_KEY_MAX];


private:
	CONTROLER_STATE _controlDevice[CONTROL_DEVICE_MAX];
	XINPUT_STATE _oldControlDevice[CONTROL_DEVICE_MAX];
	LPDIRECTINPUTDEVICE8 _pKeyboradDevice;
	int _nControlDeviceNumber;
};
#endif
//EOF