//==============================================================================
//
// node2d[node2d.cpp]
// Author : Yasuaki Yamashita : 2015/10/22
//
//==============================================================================

//******************************************************************************
// include
//******************************************************************************
#include "node2d.h"

//------------------------------------------------------------------------------
// updateWorldMtx
//------------------------------------------------------------------------------
void node2d::updateWorldMtx() {
  D3DXMATRIX mtxTmp;
  const float sinb = sinf(getRot().y);
  const float cosb = cosf(getRot().y);

  _mtxWorld._11 = _scl.x * cosb;
  _mtxWorld._12 = _scl.x * -sinb;
  _mtxWorld._13 = 0.0f;
  _mtxWorld._14 = 0.0f;
  
  _mtxWorld._21 = _scl.y * sinb;
  _mtxWorld._22 = _scl.y * cosb;
  _mtxWorld._23 = 0.0f;
  _mtxWorld._24 = 0.0f;
  
  _mtxWorld._31 = 0.0f;
  _mtxWorld._32 = 0.0f;
  _mtxWorld._33 = 1.0f;
  _mtxWorld._34 = 0.0f;
  
  _mtxWorld._41 = _pos.x;
  _mtxWorld._42 = _pos.y;
  _mtxWorld._43 = 0.0f;
  _mtxWorld._44 = 1.0f;

  if(getParent() != nullptr && getParent()->getNodeType() == NodeType::normal2D)
    _mtxWorld = _mtxWorld * getParent()->getWorldMtx();
}

//EOF