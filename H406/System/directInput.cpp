//==============================================================================
//
// DirectInput[DirectInput.cpp]
// Author : Masato Masuda : 2015/10/10
//
//==============================================================================

//******************************************************************************
// include
//******************************************************************************
#include "app.h"
#include "DirectInput.h"
#include "input.h"
#include "vInput.h"

//=============================================================================
// static
//=============================================================================
Input* DirectInput::_input = nullptr;
GUID DirectInput::_padDiscrimination[DirectInput::INPUT_MAX];
int DirectInput::_padCount = 0;

//=============================================================================
// constructor
//=============================================================================
DirectInput::DirectInput(void)
{
  for (int i = 0; i < INPUT_MAX; i++){
    _pDirectInputDevice[i] = nullptr;
  }
}

//=============================================================================
// init
//=============================================================================
void DirectInput::init(Input* input)
{
  // input
  _input = input;

  // 配列のクリア
  memset(_inputVTKey, 0, sizeof(_inputVTKey));
  memset(_padRepeat, 0, sizeof(_padRepeat));
  memset(_padCrossRepeat, 0, sizeof(_padCrossRepeat));

  // virtual用配列
  _inputVTKey[(int)VK_INPUT::LEFT] = DIRECT_INPUT_PAD_LEFT;
  _inputVTKey[(int)VK_INPUT::RIGHT] = DIRECT_INPUT_PAD_RIGHT;
  _inputVTKey[(int)VK_INPUT::UP] = DIRECT_INPUT_PAD_UP;
  _inputVTKey[(int)VK_INPUT::DOWN] = DIRECT_INPUT_PAD_DOWN;
  _inputVTKey[(int)VK_INPUT::_1] = DIRECT_INPUT_PAD_A;
  _inputVTKey[(int)VK_INPUT::_2] = DIRECT_INPUT_PAD_B;
  _inputVTKey[(int)VK_INPUT::_3] = DIRECT_INPUT_PAD_C;
  _inputVTKey[(int)VK_INPUT::_4] = DIRECT_INPUT_PAD_X;
  _inputVTKey[(int)VK_INPUT::_5] = DIRECT_INPUT_PAD_Y;
  _inputVTKey[(int)VK_INPUT::_6] = DIRECT_INPUT_PAD_Z;
  _inputVTKey[(int)VK_INPUT::_7] = DIRECT_INPUT_PAD_L;
  _inputVTKey[(int)VK_INPUT::_8] = DIRECT_INPUT_PAD_R;
  _inputVTKey[(int)VK_INPUT::_9] = DIRECT_INPUT_PAD_START;
  _inputVTKey[(int)VK_INPUT::_10] = DIRECT_INPUT_PAD_M;

  // デバイス生成
  for (_padCount = 0; _padCount < INPUT_MAX; _padCount++){

    // デバイスオブジェクトを作成
    if (FAILED(input->getInputDevice()->EnumDevices(DI8DEVCLASS_GAMECTRL,
      (LPDIENUMDEVICESCALLBACKA)EnumDevicesCallback,
      this,
      DIEDFL_ATTACHEDONLY))){
      // 失敗
      return;
    }

    // 作成失敗
    if (_pDirectInputDevice[_padCount] == NULL){
      return;
    }

    // データフォーマットの設定
    if (FAILED(_pDirectInputDevice[_padCount]->SetDataFormat(&c_dfDIJoystick2))){
      // 失敗
      return;
    }

    // 協調モードの設定
    if (FAILED(_pDirectInputDevice[_padCount]->SetCooperativeLevel(App::instance().getHWnd(), (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))){
      // 失敗
      return;
    }

    // 十字キーの範囲等指定
    if (FAILED(_pDirectInputDevice[_padCount]->EnumObjects((LPDIENUMDEVICEOBJECTSCALLBACKA)EnumAxesCallback, this, DIDFT_AXIS))){
      // 失敗
      return;
    }

    // アクセス権の取得
    _pDirectInputDevice[_padCount]->Acquire();
  }

}

//=============================================================================
// uninit
//=============================================================================
void DirectInput::uninit(void)
{
  // アクセス権開放
  for (int i = 0; i < INPUT_MAX; i++){
    if (_pDirectInputDevice[i]) _pDirectInputDevice[i]->Unacquire();
    SafeRelease(_pDirectInputDevice[i]);
  }
}

//=============================================================================
// update
//=============================================================================
void DirectInput::update(void)
{
  DIJOYSTATE2	joystate = {NULL};
  HRESULT		hr = 0;

  const int repeatStartTime = App::instance().getInput()->getRepeatStartTime();
  const int repeatSleepTime = App::instance().getInput()->getRepeatSleepTime();

  for (int i = 0; i < INPUT_MAX; i++){

    if (_pDirectInputDevice[i] != NULL)
    {
      // ジョイパッドデータの取得
      _pDirectInputDevice[i]->Poll();

      // 入力の受付開始
      _pDirectInputDevice[i]->Acquire();

      if (SUCCEEDED(hr = _pDirectInputDevice[i]->GetDeviceState(sizeof(DIJOYSTATE2), &joystate)))
      {
        // 前のデータを保存
        memcpy(&_padDataLast[i], &_padData[i], sizeof(DIJOYSTATE2));

        // パッドデータ取得
        _pDirectInputDevice[i]->GetDeviceState(sizeof(DIJOYSTATE2), &_padData[i]);

        // パッド用リピート
        for (int j = 0; j < DIRECT_INPUT_PAD_MAX; j++)
        {
          if (isPress(i, (VK_INPUT)j)){
            if (_padRepeat[j - DIRECT_PP_MAX][i] < 0xF0){
              _padRepeat[j - DIRECT_PP_MAX][i] ++;

              if (_padRepeat[j - DIRECT_PP_MAX][i] > repeatStartTime){
                _padRepeat[j - DIRECT_PP_MAX][i] -= (BYTE)repeatSleepTime;
              }
            }
          }
          else{
            _padRepeat[j - DIRECT_PP_MAX][i] = 0;
          }
        }

        // 十字キー用リピート
        for (int j = 0; j < DIRECT_PP_MAX; j++)
        {
          if (isCrossKey(i, (DIRECT_INPUT_PAD)(j - DIRECT_PP_MAX))){
            if (_padCrossRepeat[j][i] < 0xF0){
              _padCrossRepeat[j][i] ++;

              if (_padCrossRepeat[j][i] > repeatStartTime){
                _padCrossRepeat[j][i] -= (BYTE)repeatSleepTime;
              }
            }
          }
          else {
            _padCrossRepeat[j][i] = 0;
          }
        }

      }
      else if (hr == DIERR_INPUTLOST)
      {
        // アクセス権の取得
        _pDirectInputDevice[i]->Acquire();
      }
    }
  }
}

//=============================================================================
// isPress
//=============================================================================
bool DirectInput::isPress(int id, VK_INPUT vk) const
{
  if ((int)_inputVTKey[(int)vk] < 0){
    return isCrossKey(id, _inputVTKey[(int)vk]);
  }
  return _padData[id].rgbButtons[_inputVTKey[(int)vk]] ? true : false;
}

//=============================================================================
// isTrigger
//=============================================================================
bool DirectInput::isTrigger(int id, VK_INPUT vk) const
{
  if ((int)_inputVTKey[(int)vk] < 0){
    return _padCrossRepeat[_inputVTKey[(int)vk] + DIRECT_PP_MAX][id] == (BYTE)1;
  }
  return ((_padData[id].rgbButtons[_inputVTKey[(int)vk]] ^ _padDataLast[id].rgbButtons[_inputVTKey[(int)vk]])
    & _padData[id].rgbButtons[_inputVTKey[(int)vk]]) ? true : false;
}

//=============================================================================
// isRelease
//=============================================================================
bool DirectInput::isRelease(int id, VK_INPUT vk) const
{
  if ((int)_inputVTKey[(int)vk] < 0){
    return isCrossKey(id, _inputVTKey[(int)vk]);
  }
  return ((_padData[id].rgbButtons[_inputVTKey[(int)vk]] ^ _padDataLast[id].rgbButtons[_inputVTKey[(int)vk]])
    & ~_padData[id].rgbButtons[_inputVTKey[(int)vk]]) ? true : false;
}

//=============================================================================
// isRepeat
//=============================================================================
bool DirectInput::isRepeat(int id, VK_INPUT vk) const
{
  const int repeatStartTime = App::instance().getInput()->getRepeatStartTime();

  if ((int)_inputVTKey[(int)vk] < 0){
    if (_padCrossRepeat[_inputVTKey[(int)vk ] + DIRECT_PP_MAX][id] >= (BYTE)repeatStartTime){
      return true;
    }
    else if (_padCrossRepeat[_inputVTKey[(int)vk] + DIRECT_PP_MAX][id] == (BYTE)1)
    {
      return true;
    }
  }
  else if(_padRepeat[_inputVTKey[(int)vk]][id] >= (BYTE)repeatStartTime){
      return true;
  }
  return isTrigger(id, vk);
}

//=============================================================================
// 十字キー用
//=============================================================================
inline bool DirectInput::isCrossKey(int id, DIRECT_INPUT_PAD vk) const
{
  BYTE re = 0;

  // 押されてる角度から場所特定
  switch (_padData[id].rgdwPOV[0]){
  case 0:
    re = DIRECT_PP_UP;
    break;
  case 4500:
    re = DIRECT_PP_UP | DIRECT_PP_RIGHT;
    break;
  case 9000:
    re = DIRECT_PP_RIGHT;
    break;
  case 13500:
    re = DIRECT_PP_RIGHT | DIRECT_PP_DOWN;
    break;
  case 18000:
    re = DIRECT_PP_DOWN;
    break;
  case 22500:
    re = DIRECT_PP_DOWN | DIRECT_PP_LEFT;
    break;
  case 27000:
    re = DIRECT_PP_LEFT;
    break;
  case 31500:
    re = DIRECT_PP_LEFT | DIRECT_PP_UP;
    break;
  }

  // アナログスティック調べる
  if (_padData[id].lX > 100) re |= DIRECT_PP_RIGHT;
  else if (_padData[id].lX < -100) re |= DIRECT_PP_LEFT;
  if (_padData[id].lY > 100) re |= DIRECT_PP_DOWN;
  else if (_padData[id].lY < -100) re |= DIRECT_PP_UP;

  switch ((int)vk)
  {
  case DIRECT_INPUT_PAD_UP:
    return re & DIRECT_PP_UP ? true : false;
  case DIRECT_INPUT_PAD_DOWN:
    return re & DIRECT_PP_DOWN ? true : false;
  case DIRECT_INPUT_PAD_LEFT:
    return re & DIRECT_PP_LEFT ? true : false;
  case DIRECT_INPUT_PAD_RIGHT:
    return re & DIRECT_PP_RIGHT ? true : false;
  }

  return NULL;
}

//=============================================================================
// 問い合わせ用コールバック
//=============================================================================
bool CALLBACK DirectInput::EnumDevicesCallback(const DIDEVICEINSTANCE* didInstance, void* ref)
{
  DirectInput* dInput = (DirectInput*)ref;
  DIDEVCAPS devCaps;

  Input* input = _input;
  if (input == nullptr) return false;

  // 既に取得済みのデバイスか調べる
  for (int i = 0; i < _padCount; i++){
    if (_padDiscrimination[i] == didInstance->guidInstance)
      return DIENUM_CONTINUE;
  }

  // ジョイスティックへのインターフェイスを取得
  if (FAILED(input->getInputDevice()->CreateDevice(didInstance->guidInstance, &dInput->_pDirectInputDevice[_padCount], NULL))){
    return false;
  }

  // ジョイスティックの能力を調べる
  devCaps.dwSize = sizeof(DIDEVCAPS);

  if (FAILED(dInput->_pDirectInputDevice[_padCount]->GetCapabilities(&devCaps))){
    SafeRelease(dInput->_pDirectInputDevice[_padCount]);
    return false;
  }

  // デバイスの識別子を保存
  _padDiscrimination[_padCount] = didInstance->guidInstance;

  return true;
}

//=============================================================================
// アナログスティック用コールバック
//=============================================================================
bool CALLBACK DirectInput::EnumAxesCallback(const DIDEVICEOBJECTINSTANCE* didoi, void* ref)
{
  DirectInput* dInput = (DirectInput*)ref;

  // 軸の値の設定するよ
  DIPROPRANGE diprg;
  ZeroMemory(&diprg, sizeof(diprg));

  // 範囲を設定（-1000～1000）
  diprg.diph.dwSize = sizeof(diprg);
  diprg.diph.dwHeaderSize = sizeof(diprg.diph);
  diprg.diph.dwObj = didoi->dwType;
  diprg.diph.dwHow = DIPH_BYID;
  diprg.lMin = -1000;
  diprg.lMax = +1000;

  if (FAILED(dInput->_pDirectInputDevice[_padCount]->SetProperty(DIPROP_RANGE, &diprg.diph))){
    return false;
  }

  return true;
}

//EOF