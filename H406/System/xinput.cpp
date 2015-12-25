//==============================================================================
//
// xInput[xuput.cpp]
// Author : Yoshiaki Nakajima : 2015/10/06
//
//==============================================================================

//******************************************************************************
// include
//******************************************************************************
#include "app.h"
#include "xinput.h"
#include "input.h"
#include "vInput.h"


xInput::xInput() 
:_pKeyboradDevice(nullptr){

}

void xInput::init(Input* input) {
	for (int nId = 0; nId < CONTROL_DEVICE_NUMBER_MAX; nId++)
	{
		//�R���g���[���[�̏����X�V
		UpdateControllerState(nId);
	}
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

	memset(_aButtons, 0, sizeof(_aButtons));
	memset(_aButtonsTrigger, 0, sizeof(_aButtonsTrigger));
	memset(_aButtonsRelease, 0, sizeof(_aButtonsRelease));
	memset(_aButtonsRepeat, 0, sizeof(_aButtonsRepeat));
}

void xInput::uninit() {
	// �A�N�Z�X���J��
	if (_pKeyboradDevice) _pKeyboradDevice->Unacquire();
	SafeRelease(_pKeyboradDevice);
}

void xInput::update() {
	//�R���g���[���[�̐������[�v
	for (int nId = 0; nId < CONTROL_DEVICE_NUMBER_MAX; nId++)
	{
		//�R���g���[���[�̏����X�V
		UpdateControllerState(nId);
		//�R���g���[���[���ڑ������ǂ��� 
		if (_controlDevice[nId].bConnected){
			// ���͏��i�[���[�N
			WORD wButtons = _controlDevice[nId].state.Gamepad.wButtons;
			SHORT sThumbLX = _controlDevice[nId].state.Gamepad.sThumbLX;
			SHORT sThumbLY = _controlDevice[nId].state.Gamepad.sThumbLY;

			// ���͏����X�e�[�g���ɕ������ĊǗ�
			BYTE aKeyState[CONTROL_DEVICE_KEY_MAX];
			int nCntKey = 0;
			//�����L�[�@�󂯎��
			aKeyState[0] = (wButtons & XINPUT_GAMEPAD_DPAD_LEFT) ? 1 : 0;
			aKeyState[1] = (wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) ? 1 : 0;
			aKeyState[2] = (wButtons & XINPUT_GAMEPAD_DPAD_UP) ? 1 : 0;
			aKeyState[3] = (wButtons & XINPUT_GAMEPAD_DPAD_DOWN) ? 1 : 0;

			//�A�i���O�X�e�B�b�N�@�󂯎��(�����L�[��OR����Ŏ��) �A�i���O�X�e�B�b�N���|���ȏ�Ŏ��
			aKeyState[0] = aKeyState[0] | ((sThumbLX < -CONTROL_STICK_RANGE_MAX / 2) ? 1 : 0);
			aKeyState[1] = aKeyState[1] | ((sThumbLX > CONTROL_STICK_RANGE_MAX / 2) ? 1 : 0);
			aKeyState[2] = aKeyState[2] | ((sThumbLY > CONTROL_STICK_RANGE_MAX / 2) ? 1 : 0);
			aKeyState[3] = aKeyState[3] | ((sThumbLY < -CONTROL_STICK_RANGE_MAX / 2) ? 1 : 0);

			//ABXY�{�^���󂯎�� ABXY�̏�
			aKeyState[4] = (wButtons & XINPUT_GAMEPAD_A) ? 1 : 0;
			aKeyState[5] = (wButtons & XINPUT_GAMEPAD_B) ? 1 : 0;
			aKeyState[6] = (wButtons & XINPUT_GAMEPAD_X) ? 1 : 0;
			aKeyState[7] = (wButtons & XINPUT_GAMEPAD_Y) ? 1 : 0;

			//L1 R1�󂯎��
			aKeyState[8] = (wButtons & XINPUT_GAMEPAD_LEFT_THUMB) ? 1 : 0;
			aKeyState[9] = (wButtons & XINPUT_GAMEPAD_RIGHT_THUMB) ? 1 : 0;

			//���E�X�e�B�b�N�{�^���s���Ƃ�
			aKeyState[10] = (wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) ? 1 : 0;
			aKeyState[11] = (wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) ? 1 : 0;

			//start back�{�^���󂯎��
			aKeyState[12] = (wButtons & XINPUT_GAMEPAD_START) ? 1 : 0;
			aKeyState[13] = (wButtons & XINPUT_GAMEPAD_BACK) ? 1 : 0;

			for (nCntKey = 0; nCntKey < CONTROL_DEVICE_KEY_MAX; nCntKey++) {
				// �g���K�[�쐬 ���݃g���K�[�������Ă���ԏ��ON
				_aButtonsTrigger[nId][nCntKey] = (aKeyState[nCntKey] ^ _aButtons[nId][nCntKey]) & aKeyState[nCntKey];

				// �����[�X�쐬�@���ݗ������㑼�̃L�[���������܂�ON
				_aButtonsRelease[nId][nCntKey] = (aKeyState[nCntKey] ^ _aButtons[nId][nCntKey]) & ~aKeyState[nCntKey];

				// �O���[�o���ֈڂ�
				_aButtons[nId][nCntKey] = aKeyState[nCntKey];

				// ���s�[�g�쐬
				if (_aButtons[nId][nCntKey] & 0x01) {
					//0x20�ɂȂ�����0�ɖ߂�
					(_aButtonsRepeat[nId][nCntKey] < 0x20) ? _aButtonsRepeat[nId][nCntKey]++ : _aButtonsRepeat[nId][nCntKey] = 0;
				}
				else {
					_aButtonsRepeat[nId][nCntKey] = 0;
				}
			}//for

		}//if�@�ڑ��m�F
		//�ڑ����؂�Ă�����
		else{
			// �z��̃N���A

		}//else
	}//for �R���g���[���[���̃��[�v
}

bool xInput::isPress(int id, VK_INPUT vk) const {
	return (_aButtons[id][(int)vk]) ? true : false;
}


bool xInput::isTrigger(int id,VK_INPUT vk) const {
	return (_aButtonsTrigger[id][(int)vk]) ? true : false;
}


bool xInput::isRelease(int id,VK_INPUT vk) const {
	return (_aButtonsRelease[id][(int)vk]) ? true : false;
}

bool xInput::isRepeat(int id,VK_INPUT vk) const {
	return (_aButtonsRepeat[id][(int)vk] >= 0x20 ) ? true : false;
}

//�R���g���[���[�̏�Ԃ��X�V����
HRESULT xInput::UpdateControllerState(int nId)
{
	//�ڑ���Ԃ��󂯎��ϐ����`
	DWORD dwResult;
	//�R���g���[���[�̏�Ԃ��󂯎��
	dwResult = XInputGetState(nId, &_controlDevice[nId].state);
	//�ڑ����Ȃ�΁@�ڑ���Ԃ�TRUE�� �ڑ�����:ERROR_SUCCESS �ڑ����s:ERROR_DEVICE_NOT_CONNECTED�������Ă���
	if (dwResult == ERROR_SUCCESS)
		_controlDevice[nId].bConnected = true;
	else
		_controlDevice[nId].bConnected = false;
	//�����������ǂ�����Ԃ�
	return S_OK;
}

//EOF