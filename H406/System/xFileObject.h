//==============================================================================
//
// xFileObject[xFileObject.h]
// Author : Yasuaki Yamashita : 2015/06/29
//
//==============================================================================

#pragma once

#ifndef _XFILEOBJECT_H_
#define _XFILEOBJECT_H_

#include "iDrawObject.h"
//==============================================================================
// class
//------------------------------------------------------------------------------
class XFileObject : public iDrawObject {
public:
  bool init(const char* file);
  virtual void update();
  void draw(const Renderer* renderer);
  void uninit();

  XFileObject() {};
  virtual ~XFileObject() {};

protected:
  virtual NodeType getNodeType() const { return NodeType::normal3D; }

private:
  LPD3DXMESH _pD3DXMesh;         // メッシュ情報へのポインタ
  LPD3DXBUFFER _pD3DXBuffMat;    // マテリアル情報へのポインタ
  DWORD _nNumMat;                // マテリアル情報の数

  unsigned int _textureID;

  LPDIRECT3DVERTEXDECLARATION9 _p3DDec;
};

#endif
//EOF
