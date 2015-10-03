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
  Matrix _WVPOld;          // ���[���h�}�g���b�N�X
  LPD3DXMESH _pD3DXMesh;         // ���b�V�����ւ̃|�C���^
  LPD3DXBUFFER _pD3DXBuffMat;    // �}�e���A�����ւ̃|�C���^
  DWORD _nNumMat;                // �}�e���A�����̐�


};

#endif
//EOF
