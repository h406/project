//==============================================================================
//
// meshCylinder [meshCylinder.h]
// Author : masato masuda : 2015/10/14
//
//==============================================================================
#pragma once
#ifndef _MESH_CYLINDER_H_
#define _MESH_CYLINDER_H_

#include "DrawObject.h"
#include "CreateFunc.h"

//==============================================================================
// class
//------------------------------------------------------------------------------
class MeshCylinder : public DrawObject, public CreateFunc<MeshCylinder> {
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