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

//�R���g���[���[�̏��
struct CONTROLER_STATE
{
	XINPUT_STATE state;
	bool bConnected;
};
//*****************************************************************************
// �N���X�錾
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

	// �f�o�C�X�ڑ��������ׂ� Number�̓R���g���[���ԍ�
	bool getConnectDevice() { return _controlDevice[_nControlDeviceNumber].bConnected ? true : false; }

	// �f�o�C�X�X�e�[�g�̍X�V
	HRESULT UpdateControllerState(void);

	// �L�[������󂯎��f�o�C�X�ԍ���ύX
	void setControlDevice(int nNumber){ _nControlDeviceNumber = nNumber; }

protected:
	// �L�[��񃏁[�N
	BYTE _wButtons[CONTROL_DEVICE_MAX][CONTROL_DEVICE_KEY_MAX];
	// ���s�[�g�J�E���^
	int _wButtonsRepeatCnt[CONTROL_DEVICE_MAX][CONTROL_DEVICE_KEY_MAX];


private:
	CONTROLER_STATE _controlDevice[CONTROL_DEVICE_MAX];
	XINPUT_STATE _oldControlDevice[CONTROL_DEVICE_MAX];
	LPDIRECTINPUTDEVICE8 _pKeyboradDevice;
	int _nControlDeviceNumber;
};
#endif
//EOF