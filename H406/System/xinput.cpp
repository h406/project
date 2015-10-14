//==============================================================================
//
// xInput[xuput.cpp]
// Author : Yoshiaki Nakajima : 2015/10/06
//
//==============================================================================

//******************************************************************************
// include
//******************************************************************************
#include "xinput.h"
#include "input.h"
#include "vInput.h"


namespace {
	// �L�[�{�[�h��񖢐ݒ�
	const BYTE KEY_NONE = (0x00);

	// �L�[�{�[�h�v���X���
	const BYTE KEY_PRESS = (0x01);

	// �L�[�{�[�h�g���K���
	const BYTE KEY_TRIGGER = (0x02);

	// �L�[�{�[�h�����[�X���
	const BYTE KEY_RELEASE = (0x04);

	// �L�[�{�[�h���s�[�g���
	const BYTE KEY_REPEAT = (0x08);
}

xInput::xInput() 
:_pKeyboradDevice(nullptr){

}

void xInput::init(Input* input) {
	_nControlDeviceNumber = 0;
	//�R���g���[���[�̏����X�V
	UpdateControllerState();

	// �L�[�{�[�h�f�o�C�X�I�u�W�F�N�g����
	if (FAILED(input->getInputDevice()->CreateDevice(
		GUID_SysKeyboard,
		&_pKeyboradDevice,
		NULL))) {
		// �������s
		MessageBox(NULL, "�L�[�{�[�h�f�o�C�X�I�u�W�F�N�g�̐����Ɏ��s���܂���", "�G���[", MB_OK | MB_ICONHAND);
		return;
	}

	// �f�[�^�t�H�[�}�b�g�̐ݒ�
	if (FAILED(_pKeyboradDevice->SetDataFormat(&c_dfDIKeyboard))) {
		// �ݒ莸�s
		MessageBox(NULL, "�L�[�{�[�h�f�[�^�t�H�[�}�b�g�̐ݒ�Ɏ��s���܂���", "�G���[", MB_OK | MB_ICONHAND);
		return;
	}

	// �������[�h�̐ݒ�
	if (FAILED(_pKeyboradDevice->SetCooperativeLevel(App::instance().getHWnd(), (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))) {
		// �������s
		MessageBox(NULL, "�L�[�{�[�h�������[�h�̐ݒ�Ɏ��s���܂���", "�G���[", MB_OK | MB_ICONHAND);
		return;
	}

	// �L�[�{�[�h�ւ̃A�N�Z�X���̎擾
	_pKeyboradDevice->Acquire();
}

void xInput::uninit() {
	// �A�N�Z�X���J��
	if (_pKeyboradDevice) _pKeyboradDevice->Unacquire();
	SafeRelease(_pKeyboradDevice);
}

void xInput::update() {
	//�R���g���[���[�f�o�C�X���������[�v
	for (int nDeviceNum = 0; nDeviceNum < CONTROL_DEVICE_MAX; nDeviceNum++)
	{
		//�R���g���[���[�̏����X�V
		UpdateControllerState();
		//�R���g���[���[���ڑ������ǂ��� 
		if (_controlDevice[nDeviceNum].bConnected){
			//�R���g���[���[�̓��͂ɕω������邩�ǂ���
			if (_controlDevice[nDeviceNum].state.dwPacketNumber != _oldControlDevice[nDeviceNum].dwPacketNumber){
				// ���͏��i�[���[�N
				WORD Buttons = _controlDevice[nDeviceNum].state.Gamepad.wButtons;

				const int repeatStartTime = App::instance().getInput()->getRepeatStartTime();
				const int repeatSleepTime = App::instance().getInput()->getRepeatSleepTime();

				// ���͏����X�e�[�g���ɕ������ĊǗ�
				BYTE aKeyState[CONTROL_DEVICE_KEY_MAX];
				int nCntKey = 0;
				//�����L�[�@�󂯎��
				aKeyState[0] = (Buttons & XINPUT_GAMEPAD_DPAD_LEFT) ? 1 : 0;
				aKeyState[1] = (Buttons & XINPUT_GAMEPAD_DPAD_RIGHT) ? 1 : 0;
				aKeyState[2] = (Buttons & XINPUT_GAMEPAD_DPAD_UP) ? 1 : 0;
				aKeyState[3] = (Buttons & XINPUT_GAMEPAD_DPAD_DOWN) ? 1 : 0;

				//ABXY�{�^���󂯎�� ABXY�̏�
				aKeyState[4] = (Buttons & XINPUT_GAMEPAD_A) ? 1 : 0;
				aKeyState[5] = (Buttons & XINPUT_GAMEPAD_B) ? 1 : 0;
				aKeyState[6] = (Buttons & XINPUT_GAMEPAD_X) ? 1 : 0;
				aKeyState[7] = (Buttons & XINPUT_GAMEPAD_Y) ? 1 : 0;

				//L1 R1�󂯎��
				aKeyState[8] = (Buttons & XINPUT_GAMEPAD_LEFT_THUMB) ? 1 : 0;
				aKeyState[9] = (Buttons & XINPUT_GAMEPAD_RIGHT_THUMB) ? 1 : 0;

				//���E�X�e�B�b�N�{�^���s���Ƃ�
				aKeyState[10] = (Buttons & XINPUT_GAMEPAD_LEFT_SHOULDER) ? 1 : 0;
				aKeyState[11] = (Buttons & XINPUT_GAMEPAD_RIGHT_SHOULDER) ? 1 : 0;

				//start back�{�^���󂯎��
				aKeyState[12] = (Buttons & XINPUT_GAMEPAD_START) ? 1 : 0;
				aKeyState[13] = (Buttons & XINPUT_GAMEPAD_BACK) ? 1 : 0;

				for (nCntKey = 0; nCntKey < CONTROL_DEVICE_KEY_MAX; nCntKey++) {
					// �O����̕ۑ�
					BYTE wButtonsOld = _wButtons[nDeviceNum][nCntKey];

					// �g���K���̍쐬
					aKeyState[nCntKey] += ((aKeyState[nCntKey] ^ wButtonsOld)& aKeyState[nCntKey]) << 1;

					// �����[�X���̍쐬
					aKeyState[nCntKey] += ((aKeyState[nCntKey] ^ wButtonsOld) & ~wButtonsOld) << 2;

					// ���s�[�g���̍쐬
					_wButtonsRepeatCnt[nDeviceNum][nCntKey] = (_wButtonsRepeatCnt[nDeviceNum][nCntKey] * (aKeyState[nCntKey] / KEY_PRESS)) + 1;

					aKeyState[nCntKey] += (BYTE)(((_wButtonsRepeatCnt[nDeviceNum][nCntKey] / repeatStartTime)) | ((aKeyState[nCntKey] & KEY_TRIGGER))) << 3;
					if (_wButtonsRepeatCnt[nDeviceNum][nCntKey] / repeatStartTime >= 1) {
						_wButtonsRepeatCnt[nDeviceNum][nCntKey] -= (BYTE)repeatSleepTime;
					}//if

					// �O���[�o���ֈڂ�
					_wButtons[nDeviceNum][nCntKey] = aKeyState[nCntKey];
				}//for

				//���݂̏�Ԃ�ۑ�
				_oldControlDevice[nDeviceNum].dwPacketNumber = _controlDevice[nDeviceNum].state.dwPacketNumber;
			}//if ��ԕω��m�F

		}//if�@�ڑ��m�F
		//�ڑ����؂�Ă�����
		else{
			// �z��̃N���A

		}//else
	}//for �R���g���[���[���̃��[�v
}

bool xInput::isPress(VK_INPUT vk) const {
	return (_wButtons[_nControlDeviceNumber][(int)vk] & KEY_PRESS) ? true : false;
}


bool xInput::isTrigger(VK_INPUT vk) const {
	return (_wButtons[_nControlDeviceNumber][(int)vk] & KEY_TRIGGER) ? true : false;
}


bool xInput::isRelease(VK_INPUT vk) const {
	return (_wButtons[_nControlDeviceNumber][(int)vk] & KEY_RELEASE) ? true : false;
}

bool xInput::isRepeat(VK_INPUT vk) const {
	return (_wButtons[_nControlDeviceNumber][(int)vk] & KEY_REPEAT) ? true : false;
}

//�R���g���[���[�̏�Ԃ��X�V����
HRESULT xInput::UpdateControllerState(void)
{
	//�ڑ���Ԃ��󂯎��ϐ����`
	DWORD dwResult;
	//�R���g���[���[�̏�Ԃ��󂯎��
	dwResult = XInputGetState(_nControlDeviceNumber, &_controlDevice[_nControlDeviceNumber].state);
	//�ڑ����Ȃ�΁@�ڑ���Ԃ�TRUE�� �ڑ�����:ERROR_SUCCESS �ڑ����s:ERROR_DEVICE_NOT_CONNECTED�������Ă���
	if (dwResult == ERROR_SUCCESS)
		_controlDevice[_nControlDeviceNumber].bConnected = true;
	else
		_controlDevice[_nControlDeviceNumber].bConnected = false;
	//�����������ǂ�����Ԃ�
	return S_OK;
}

//EOF