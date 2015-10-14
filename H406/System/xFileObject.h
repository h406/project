//==============================================================================
//
// xFileObject[xFileObject.h]
// Author : Yasuaki Yamashita : 2015/06/29
//
//==============================================================================

#pragma once

#ifndef _XFILEOBJECT_H_
#define _XFILEOBJECT_H_

#include "DrawObject.h"
#include "CreateFunc.h"

//==============================================================================
// class
//------------------------------------------------------------------------------
class XFileObject : public DrawObject,public CreateFunc<XFileObject>{
public:
  bool init(const char* file);
  virtual void update();
  void draw(const Renderer* renderer);
  void uninit();

  using CreateFunc::create;

protected:
  virtual NodeType getNodeType() const { return NodeType::normal3D; }

private:
  XFileObject() {};
  virtual ~XFileObject() {};

  LPD3DXMESH _pD3DXMesh;         // ���b�V�����ւ̃|�C���^
  LPD3DXBUFFER _pD3DXBuffMat;    // �}�e���A�����ւ̃|�C���^
  DWORD _nNumMat;                // �}�e���A�����̐�

  LPDIRECT3DVERTEXDECLARATION9 _p3DDec;
};

#endif
//EOF
