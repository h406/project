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
	// キーボード情報未設定
	const BYTE KEY_NONE = (0x00);

	// キーボードプレス情報
	const BYTE KEY_PRESS = (0x01);

	// キーボードトリガ情報
	const BYTE KEY_TRIGGER = (0x02);

	// キーボードリリース情報
	const BYTE KEY_RELEASE = (0x04);

	// キーボードリピート情報
	const BYTE KEY_REPEAT = (0x08);
}

xInput::xInput() 
:_pKeyboradDevice(nullptr){

}

void xInput::init(Input* input) {
	_nControlDeviceNumber = 0;
	//コントローラーの情報を更新
	UpdateControllerState();

	// キーボードデバイスオブジェクト生成
	if (FAILED(input->getInputDevice()->CreateDevice(
		GUID_SysKeyboard,
		&_pKeyboradDevice,
		NULL))) {
		// 生成失敗
		MessageBox(NULL, "キーボードデバイスオブジェクトの生成に失敗しました", "エラー", MB_OK | MB_ICONHAND);
		return;
	}

	// データフォーマットの設定
	if (FAILED(_pKeyboradDevice->SetDataFormat(&c_dfDIKeyboard))) {
		// 設定失敗
		MessageBox(NULL, "キーボードデータフォーマットの設定に失敗しました", "エラー", MB_OK | MB_ICONHAND);
		return;
	}

	// 協調モードの設定
	if (FAILED(_pKeyboradDevice->SetCooperativeLevel(App::instance().getHWnd(), (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))) {
		// 生成失敗
		MessageBox(NULL, "キーボード協調モードの設定に失敗しました", "エラー", MB_OK | MB_ICONHAND);
		return;
	}

	// キーボードへのアクセス権の取得
	_pKeyboradDevice->Acquire();
}

void xInput::uninit() {
	// アクセス権開放
	if (_pKeyboradDevice) _pKeyboradDevice->Unacquire();
	SafeRelease(_pKeyboradDevice);
}

void xInput::update() {
	//コントローラーデバイス分だけループ
	for (int nDeviceNum = 0; nDeviceNum < CONTROL_DEVICE_MAX; nDeviceNum++)
	{
		//コントローラーの情報を更新
		UpdateControllerState();
		//コントローラーが接続中かどうか 
		if (_controlDevice[nDeviceNum].bConnected){
			//コントローラーの入力に変化があるかどうか
			if (_controlDevice[nDeviceNum].state.dwPacketNumber != _oldControlDevice[nDeviceNum].dwPacketNumber){
				// 入力情報格納ワーク
				WORD Buttons = _controlDevice[nDeviceNum].state.Gamepad.wButtons;

				const int repeatStartTime = App::instance().getInput()->getRepeatStartTime();
				const int repeatSleepTime = App::instance().getInput()->getRepeatSleepTime();

				// 入力情報をステート毎に分離して管理
				BYTE aKeyState[CONTROL_DEVICE_KEY_MAX];
				int nCntKey = 0;
				//方向キー　受け取り
				aKeyState[0] = (Buttons & XINPUT_GAMEPAD_DPAD_LEFT) ? 1 : 0;
				aKeyState[1] = (Buttons & XINPUT_GAMEPAD_DPAD_RIGHT) ? 1 : 0;
				aKeyState[2] = (Buttons & XINPUT_GAMEPAD_DPAD_UP) ? 1 : 0;
				aKeyState[3] = (Buttons & XINPUT_GAMEPAD_DPAD_DOWN) ? 1 : 0;

				//ABXYボタン受け取り ABXYの順
				aKeyState[4] = (Buttons & XINPUT_GAMEPAD_A) ? 1 : 0;
				aKeyState[5] = (Buttons & XINPUT_GAMEPAD_B) ? 1 : 0;
				aKeyState[6] = (Buttons & XINPUT_GAMEPAD_X) ? 1 : 0;
				aKeyState[7] = (Buttons & XINPUT_GAMEPAD_Y) ? 1 : 0;

				//L1 R1受け取り
				aKeyState[8] = (Buttons & XINPUT_GAMEPAD_LEFT_THUMB) ? 1 : 0;
				aKeyState[9] = (Buttons & XINPUT_GAMEPAD_RIGHT_THUMB) ? 1 : 0;

				//左右スティックボタン行けとり
				aKeyState[10] = (Buttons & XINPUT_GAMEPAD_LEFT_SHOULDER) ? 1 : 0;
				aKeyState[11] = (Buttons & XINPUT_GAMEPAD_RIGHT_SHOULDER) ? 1 : 0;

				//start backボタン受け取り
				aKeyState[12] = (Buttons & XINPUT_GAMEPAD_START) ? 1 : 0;
				aKeyState[13] = (Buttons & XINPUT_GAMEPAD_BACK) ? 1 : 0;

				for (nCntKey = 0; nCntKey < CONTROL_DEVICE_KEY_MAX; nCntKey++) {
					// 前回情報の保存
					BYTE wButtonsOld = _wButtons[nDeviceNum][nCntKey];

					// トリガ情報の作成
					aKeyState[nCntKey] += ((aKeyState[nCntKey] ^ wButtonsOld)& aKeyState[nCntKey]) << 1;

					// リリース情報の作成
					aKeyState[nCntKey] += ((aKeyState[nCntKey] ^ wButtonsOld) & ~wButtonsOld) << 2;

					// リピート情報の作成
					_wButtonsRepeatCnt[nDeviceNum][nCntKey] = (_wButtonsRepeatCnt[nDeviceNum][nCntKey] * (aKeyState[nCntKey] / KEY_PRESS)) + 1;

					aKeyState[nCntKey] += (BYTE)(((_wButtonsRepeatCnt[nDeviceNum][nCntKey] / repeatStartTime)) | ((aKeyState[nCntKey] & KEY_TRIGGER))) << 3;
					if (_wButtonsRepeatCnt[nDeviceNum][nCntKey] / repeatStartTime >= 1) {
						_wButtonsRepeatCnt[nDeviceNum][nCntKey] -= (BYTE)repeatSleepTime;
					}//if

					// グローバルへ移す
					_wButtons[nDeviceNum][nCntKey] = aKeyState[nCntKey];
				}//for

				//現在の状態を保存
				_oldControlDevice[nDeviceNum].dwPacketNumber = _controlDevice[nDeviceNum].state.dwPacketNumber;
			}//if 状態変化確認

		}//if　接続確認
		//接続が切れていたら
		else{
			// 配列のクリア

		}//else
	}//for コントローラー数のループ
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

//コントローラーの状態を更新する
HRESULT xInput::UpdateControllerState(void)
{
	//接続状態を受け取る変数を定義
	DWORD dwResult;
	//コントローラーの状態を受け取る
	dwResult = XInputGetState(_nControlDeviceNumber, &_controlDevice[_nControlDeviceNumber].state);
	//接続中ならば　接続状態をTRUEに 接続成功:ERROR_SUCCESS 接続失敗:ERROR_DEVICE_NOT_CONNECTEDが入っている
	if (dwResult == ERROR_SUCCESS)
		_controlDevice[_nControlDeviceNumber].bConnected = true;
	else
		_controlDevice[_nControlDeviceNumber].bConnected = false;
	//成功したかどうかを返す
	return S_OK;
}

//EOF