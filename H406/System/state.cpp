//==============================================================================
//
// StateManager[StateManager.cpp]
// Author : Yasuaki Yamashita : 2015/12/25
//
//==============================================================================

//******************************************************************************
// include
//******************************************************************************
#include "state.h"

//==============================================================================
// StateManager
//------------------------------------------------------------------------------
StateManager::StateManager(LPDIRECT3DDEVICE9 device)
  :_device(device) {

  for(int i = 0; i < kStateNum; ++i) {
    _isState[i] = SUCCEEDED(_device->GetRenderState((D3DRENDERSTATETYPE)i,&_nowState[i]));
    _defaultState[i] = _nowState[i];
  }
}

//==============================================================================
// StateManager
//------------------------------------------------------------------------------
StateManager::~StateManager() {

}

//==============================================================================
// SetRendererState
//------------------------------------------------------------------------------
void StateManager::SetRendererState(D3DRENDERSTATETYPE state,DWORD value) {
  if(_isState[state] && _nowState[state] != value) {
    _device->SetRenderState(state,value);
    _nowState[state] = value;
  }
}

//==============================================================================
// ResetRendererState
//------------------------------------------------------------------------------
void StateManager::ResetRendererState() {
  for(int i = 0; i < kStateNum; ++i) {
    SetRendererState((D3DRENDERSTATETYPE)i,_defaultState[i]);
  }
}

//EOF