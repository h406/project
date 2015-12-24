//==============================================================================
//
// meshDome [meshDome.h]
// Author : masato masuda : 2015/10/14
//
//==============================================================================
#pragma once
#ifndef _MESH_DOME_H_
#define _MESH_DOME_H_

#include "DrawObject.h"
#include "CreateFunc.h"

//==============================================================================
// class
//------------------------------------------------------------------------------
class MeshDome : public DrawObject, public CreateFunc<MeshDome> {
public:

  using CreateFunc::create;

  bool init(int nNumBlockX,int nNumBlockZ,float fSizeBlockX,float fSizeBlockZ);

  virtual void update();
  void draw(const Renderer* renderer);
  void uninit();

protected:
  virtual NodeType getNodeType() const { return NodeType::lightOff3D; }

private:
  void initVtx();

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