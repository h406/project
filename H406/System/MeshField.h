//==============================================================================
//
// MeshField[MeshField.h]
// Author : Yasuaki Yamashita : 2015/10/05
//
//==============================================================================

#pragma once

#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_

#include "DrawObject.h"
#include "CreateFunc.h"

//==============================================================================
// class
//------------------------------------------------------------------------------
class MeshField : public DrawObject, public CreateFunc<MeshField> {
public:
  using CreateFunc::create;

  bool init(int nNumBlockX,int nNumBlockZ,float fSizeBlockX,float fSizeBlockZ);

  virtual void update();
  void draw(const Renderer* renderer);
  void uninit();

  void setColor(const D3DXCOLOR& color) { _color = color; }

protected:
  virtual NodeType getNodeType() const { return NodeType::normal3D; }

private:
  void initVtx();
  D3DXCOLOR _color;

  LPDIRECT3DINDEXBUFFER9  _indexBuff;
  LPDIRECT3DVERTEXBUFFER9 _vtxBuff;
  LPDIRECT3DVERTEXDECLARATION9 _p3DDec;

  int _nNumBlockX, _nNumBlockY;     // ブロック数
  int _nNumVertex;                  // 総頂点数
  int _nNumPolygon;                 // 総ポリゴン数
  float _fSizeBlockX,_fSizeBlockY;  // ブロックサイズ
  int _nNumVertexIndex;

  D3DXVECTOR3* _pVtxList;
  D3DXVECTOR3* _pNorList;
};

#endif
//EOF