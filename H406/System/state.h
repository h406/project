//==============================================================================
//
// StateManager[StateManager.h]
// Author : Yasuaki Yamashita : 2015/12/25
//
//==============================================================================

#pragma once

#ifndef _STATEMANAGER_H_
#define _STATEMANAGER_H_

//==============================================================================
// class
//------------------------------------------------------------------------------
class StateManager {
public:
  static const int kStateNum = 256;

  StateManager(LPDIRECT3DDEVICE9 device);
  ~StateManager();

  // レンダーステート
  void SetRendererState(D3DRENDERSTATETYPE state, DWORD value);

  // リセット
  void ResetRendererState();


private:
  LPDIRECT3DDEVICE9 _device;
  DWORD _nowState[kStateNum];
  DWORD _defaultState[kStateNum];

  bool _isState[kStateNum];
};

#endif
//EOF