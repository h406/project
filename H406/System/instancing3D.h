//==============================================================================
//
// Instancing3D[Instancing3D.h]
// Author : Yasuaki Yamashita : 2015/12/22
//
//==============================================================================

#pragma once

#ifndef _Instancing_H_
#define _Instancing_H_

#include "DrawObject.h"
#include "CreateFunc.h"

//==============================================================================
// class
//------------------------------------------------------------------------------
class Instancing3D : public DrawObject,public CreateFunc<Instancing3D> {
public:
  bool init();
  bool init(const char* file);
  virtual void update() {};
  void draw(const Renderer* renderer);
  void uninit();

  using CreateFunc::create;

  void updateMtxChild();

protected:
  virtual NodeType getNodeType() const { return NodeType::normal3D; }

private:
  LPDIRECT3DVERTEXBUFFER9 _vtxBuff;
  LPDIRECT3DVERTEXBUFFER9 _InstancingBuff;
  LPDIRECT3DINDEXBUFFER9  _IndexBuff;

  LPDIRECT3DVERTEXDECLARATION9 _p3DDec;

};

#endif
//EOF