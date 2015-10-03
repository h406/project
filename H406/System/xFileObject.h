//==============================================================================
//
// xFileObject[xFileObject.h]
// Author : Yasuaki Yamashita : 2015/06/29
//
//==============================================================================

#pragma once

#ifndef _XFILEOBJECT_H_
#define _XFILEOBJECT_H_

#include "iObject.h"
//==============================================================================
// class
//------------------------------------------------------------------------------
class XFileObject : public iObject {
public:
  bool init(const char* file);
  virtual void update();
  void draw(Renderer* renderer);
  void uninit();

private:
  XFileObject() {};
  virtual ~XFileObject() {};
  Matrix _WVPOld;          // ワールドマトリックス
  LPD3DXMESH _pD3DXMesh;         // メッシュ情報へのポインタ
  LPD3DXBUFFER _pD3DXBuffMat;    // マテリアル情報へのポインタ
  DWORD _nNumMat;                // マテリアル情報の数


};

#endif
//EOF
