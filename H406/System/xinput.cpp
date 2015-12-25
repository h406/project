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
		//コントローラーの情報を更新
		UpdateControllerState(nId);
	}
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

	memset(_aButtons, 0, sizeof(_aButtons));
	memset(_aButtonsTrigger, 0, sizeof(_aButtonsTrigger));
	memset(_aButtonsRelease, 0, sizeof(_aButtonsRelease));
	memset(_aButtonsRepeat, 0, sizeof(_aButtonsRepeat));
}

void xInput::uninit() {
	// アクセス権開放
	if (_pKeyboradDevice) _pKeyboradDevice->Unacquire();
	SafeRelease(_pKeyboradDevice);
}

void xInput::update() {
	//コントローラーの数分ループ
	for (int nId = 0; nId < CONTROL_DEVICE_NUMBER_MAX; nId++)
	{
		//コントローラーの情報を更新
		UpdateControllerState(nId);
		//コントローラーが接続中かどうか 
		if (_controlDevice[nId].bConnected){
			// 入力情報格納ワーク
			WORD wButtons = _controlDevice[nId].state.Gamepad.wButtons;
			SHORT sThumbLX = _controlDevice[nId].state.Gamepad.sThumbLX;
			SHORT sThumbLY = _controlDevice[nId].state.Gamepad.sThumbLY;

			// 入力情報をステート毎に分離して管理
			BYTE aKeyState[CONTROL_DEVICE_KEY_MAX];
			int nCntKey = 0;
			//方向キー　受け取り
			aKeyState[0] = (wButtons & XINPUT_GAMEPAD_DPAD_LEFT) ? 1 : 0;
			aKeyState[1] = (wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) ? 1 : 0;
			aKeyState[2] = (wButtons & XINPUT_GAMEPAD_DPAD_UP) ? 1 : 0;
			aKeyState[3] = (wButtons & XINPUT_GAMEPAD_DPAD_DOWN) ? 1 : 0;

			//アナログスティック　受け取り(方向キーとOR制御で取る) アナログスティック半倒し以上で取る
			aKeyState[0] = aKeyState[0] | ((sThumbLX < -CONTROL_STICK_RANGE_MAX / 2) ? 1 : 0);
			aKeyState[1] = aKeyState[1] | ((sThumbLX > CONTROL_STICK_RANGE_MAX / 2) ? 1 : 0);
			aKeyState[2] = aKeyState[2] | ((sThumbLY > CONTROL_STICK_RANGE_MAX / 2) ? 1 : 0);
			aKeyState[3] = aKeyState[3] | ((sThumbLY < -CONTROL_STICK_RANGE_MAX / 2) ? 1 : 0);

			//ABXYボタン受け取り ABXYの順
			aKeyState[4] = (wButtons & XINPUT_GAMEPAD_A) ? 1 : 0;
			aKeyState[5] = (wButtons & XINPUT_GAMEPAD_B) ? 1 : 0;
			aKeyState[6] = (wButtons & XINPUT_GAMEPAD_X) ? 1 : 0;
			aKeyState[7] = (wButtons & XINPUT_GAMEPAD_Y) ? 1 : 0;

			//L1 R1受け取り
			aKeyState[8] = (wButtons & XINPUT_GAMEPAD_LEFT_THUMB) ? 1 : 0;
			aKeyState[9] = (wButtons & XINPUT_GAMEPAD_RIGHT_THUMB) ? 1 : 0;

			//左右スティックボタン行けとり
			aKeyState[10] = (wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) ? 1 : 0;
			aKeyState[11] = (wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) ? 1 : 0;

			//start backボタン受け取り
			aKeyState[12] = (wButtons & XINPUT_GAMEPAD_START) ? 1 : 0;
			aKeyState[13] = (wButtons & XINPUT_GAMEPAD_BACK) ? 1 : 0;

			for (nCntKey = 0; nCntKey < CONTROL_DEVICE_KEY_MAX; nCntKey++) {
				// トリガー作成 現在トリガーが押している間常にON
				_aButtonsTrigger[nId][nCntKey] = (aKeyState[nCntKey] ^ _aButtons[nId][nCntKey]) & aKeyState[nCntKey];

				// リリース作成　現在離した後他のキーが押されるまでON
				_aButtonsRelease[nId][nCntKey] = (aKeyState[nCntKey] ^ _aButtons[nId][nCntKey]) & ~aKeyState[nCntKey];

				// グローバルへ移す
				_aButtons[nId][nCntKey] = aKeyState[nCntKey];

				// リピート作成
				if (_aButtons[nId][nCntKey] & 0x01) {
					//0x20になったら0に戻す
					(_aButtonsRepeat[nId][nCntKey] < 0x20) ? _aButtonsRepeat[nId][nCntKey]++ : _aButtonsRepeat[nId][nCntKey] = 0;
				}
				else {
					_aButtonsRepeat[nId][nCntKey] = 0;
				}
			}//for

		}//if　接続確認
		//接続が切れていたら
		else{
			// 配列のクリア

		}//else
	}//for コントローラー分のループ
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

//コントローラーの状態を更新する
HRESULT xInput::UpdateControllerState(int nId)
{
	//接続状態を受け取る変数を定義
	DWORD dwResult;
	//コントローラーの状態を受け取る
	dwResult = XInputGetState(nId, &_controlDevice[nId].state);
	//接続中ならば　接続状態をTRUEに 接続成功:ERROR_SUCCESS 接続失敗:ERROR_DEVICE_NOT_CONNECTEDが入っている
	if (dwResult == ERROR_SUCCESS)
		_controlDevice[nId].bConnected = true;
	else
		_controlDevice[nId].bConnected = false;
	//成功したかどうかを返す
	return S_OK;
}

//EOF