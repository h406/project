//==============================================================================
//
// MeshField[MeshField.cpp]
// Author : Yasuaki Yamashita : 2015/10/08
//
//==============================================================================

//******************************************************************************
// include
//******************************************************************************
#include "MeshField.h"
#include "app.h"
#include "renderer.h"
#include "shader.h"
#include "camera.h"

namespace {
  struct VERTEX_3D {
    D3DXVECTOR3   vtx; // 頂点座標
    D3DXVECTOR3   nor; // 法線座標
    D3DXVECTOR2   tex; // テクスチャ座標
  };

  // 頂点要素の配列作成
  const D3DVERTEXELEMENT9 vElement[] = {
    {0,0,D3DDECLTYPE_FLOAT3,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_POSITION,0},  // 座標
    {0,12,D3DDECLTYPE_FLOAT3,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_NORMAL,0},   // 法線
    {0,24,D3DDECLTYPE_FLOAT2,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD,0}, // UV
    D3DDECL_END()
  };

  int XZtoIDX(int x,int z, int _nNumBlockX) { return ((z)* (_nNumBlockX + 1) + (x)); }
}

//==============================================================================
// init
//------------------------------------------------------------------------------
bool MeshField::init(int nNumBlockX,int nNumBlockZ,float fSizeBlockX,float fSizeBlockZ) {
  _nNumBlockX = nNumBlockX;
  _nNumBlockY = nNumBlockZ;

  _fSizeBlockX = fSizeBlockX;
  _fSizeBlockY = fSizeBlockZ;

  _nNumVertex = (_nNumBlockX + 1) * (_nNumBlockY + 1);
  _nNumVertexIndex = ((_nNumBlockX * 2 + 2) * _nNumBlockY) + (_nNumBlockY * 2 - 2);
  _nNumPolygon = _nNumVertexIndex - 2;

  _pVtxList = new D3DXVECTOR3[_nNumVertex];
  _pNorList = new D3DXVECTOR3[(_nNumBlockY * _nNumBlockX) * 2];

  memset(_pVtxList,0,sizeof(D3DXVECTOR3) * _nNumVertex);
  memset(_pNorList,0,sizeof(D3DXVECTOR3) * (_nNumBlockY * _nNumBlockX) * 2);

  // デバイス
  LPDIRECT3DDEVICE9 pDevice = App::instance().getRenderer()->getDevice();

  // 頂点
  pDevice->CreateVertexDeclaration(vElement,&_p3DDec);

  // シェーダ
  _vtxShaderID = App::instance().getRenderer()->getShader()->getVtxShader("vs_model.cso");

  // 頂点バッファ取得
  if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * _nNumVertex,
    D3DUSAGE_WRITEONLY,
    NULL,
    D3DPOOL_MANAGED,
    &_vtxBuff,
    NULL
    ))) {
    return false;
  }

  // インデックスバッファ取得
  if(FAILED(pDevice->CreateIndexBuffer(sizeof(WORD) * _nNumVertexIndex,
    D3DUSAGE_WRITEONLY,
    D3DFMT_INDEX16,
    D3DPOOL_MANAGED,
    &_indexBuff,
    NULL))) {
    return false;
  }

  // 頂点設定
  initVtx();

  _textureID = 0;
  _color = D3DXCOLOR(1,1,1,1);

  return true;
}

//==============================================================================
// init
//------------------------------------------------------------------------------
void MeshField::initVtx() {
  VERTEX_3D *pVtx,*tmp;
  WORD *pIndex;

  // 頂点バッファ設定
  _vtxBuff->Lock(0,0,(void **)&pVtx,0);

  tmp = pVtx;

  for(int z = 0; z < _nNumBlockY + 1; z++) {
    for(int x = 0; x < _nNumBlockX + 1; x++) {
      pVtx->vtx = D3DXVECTOR3(x*(_fSizeBlockX / (float)_nNumBlockX) - _fSizeBlockX / 2.0f,0.f,z*(-(abs(_fSizeBlockY) / (float)_nNumBlockX)) + _fSizeBlockY / 2.0f);
      pVtx->nor = D3DXVECTOR3(0.0f,0.0f,0.0f);
      pVtx->tex = D3DXVECTOR2(_fSizeBlockX / _fSizeBlockY / _nNumBlockX*x * 3,1 / (float)_nNumBlockY*(float)z*3.f);

      _pVtxList[z * (_nNumBlockX + 1) + x] = pVtx->vtx;
      pVtx++;
    }
  }

  // 面法線用
  pVtx = tmp;
  
  for(int z = 0; z < _nNumBlockY; z++) {
    for(int x = 0; x < _nNumBlockX; x++) {
      D3DXVECTOR3 _1st,_2nd;
      _1st = pVtx[XZtoIDX(x + 1,z,_nNumBlockX)].vtx - pVtx[XZtoIDX(x,z,_nNumBlockX)].vtx;
      _2nd = pVtx[XZtoIDX(x + 1,z + 1,_nNumBlockX)].vtx - pVtx[XZtoIDX(x + 1,z,_nNumBlockX)].vtx;
      D3DXVec3Cross(&_pNorList[(x * 2) + z * _nNumBlockX * 2],&_1st,&_2nd);
      D3DXVec3Normalize(&_pNorList[(x * 2) + z * _nNumBlockX * 2],&_pNorList[(x * 2) + z * _nNumBlockX * 2]);

      _1st = pVtx[XZtoIDX(x,z + 1,_nNumBlockX)].vtx - pVtx[XZtoIDX(x,z,_nNumBlockX)].vtx;
      _2nd = pVtx[XZtoIDX(x + 1,z + 1,_nNumBlockX)].vtx - pVtx[XZtoIDX(x,z + 1,_nNumBlockX)].vtx;
      D3DXVec3Cross(&_pNorList[(x * 2) + z * _nNumBlockX * 2 + 1],&_2nd,&_1st);
      D3DXVec3Normalize(&_pNorList[(x * 2) + z * _nNumBlockX * 2 + 1],&_pNorList[(x * 2) + z * _nNumBlockX * 2 + 1]);
    }
  }

  for(int z = 0; z < _nNumBlockY; z++) {
    for(int x = 0; x < _nNumBlockX; x++) {
      pVtx[(z + 1)*(_nNumBlockX + 1) + x].nor += _pNorList[(x * 2) + z * _nNumBlockX * 2];
      pVtx[z   *(_nNumBlockX + 1) + x].nor += _pNorList[(x * 2) + z * _nNumBlockX * 2];
      pVtx[(z + 1)*(_nNumBlockX + 1) + x + 1].nor += _pNorList[(x * 2) + z * _nNumBlockX * 2];

      pVtx[z   *(_nNumBlockX + 1) + x].nor += _pNorList[(x * 2) + z * _nNumBlockX * 2 + 1];
      pVtx[z   *(_nNumBlockX + 1) + x + 1].nor += _pNorList[(x * 2) + z * _nNumBlockX * 2 + 1];
      pVtx[(z + 1)*(_nNumBlockX + 1) + x + 1].nor += _pNorList[(x * 2) + z * _nNumBlockX * 2 + 1];
    }
  }

  for(int i = 0; i<_nNumVertex; i++) {
    D3DXVec3Normalize(&pVtx[i].nor,&pVtx[i].nor);
  }
  _vtxBuff->Unlock();

  // インデックスバッファ設定
  _indexBuff->Lock(0,0,(void **)&pIndex,0);
  for(int z = 0; z < _nNumBlockY; z++) {
    if(z != 0) {
      *pIndex = (WORD)((_nNumBlockX + 1)*(z + 1));
      pIndex++;
    }
    *pIndex = (WORD)((_nNumBlockX + 1)*(z + 1));
    pIndex++;

    for(int x = 0; x < _nNumBlockX; x++) {
      *pIndex = (WORD)((_nNumBlockX + 1)*z + x);
      pIndex++;
      *pIndex = (WORD)((_nNumBlockX + 1)*(z + 1) + x + 1);
      pIndex++;
    }

    *pIndex = (WORD)((_nNumBlockX + 1)*z + (_nNumBlockX));
    pIndex++;

    if(z != _nNumBlockY - 1) {
      *pIndex = (WORD)((_nNumBlockX + 1)*z + (_nNumBlockX));
      pIndex++;
    }
  }
  _indexBuff->Unlock();
}

//==============================================================================
// uninit
//------------------------------------------------------------------------------
void MeshField::uninit() {
  SafeRelease(_vtxBuff);
  SafeRelease(_indexBuff);
  SafeRelease(_p3DDec);
  SafeDelete(_pVtxList);
  SafeDelete(_pNorList);
}

//==============================================================================
// update
//------------------------------------------------------------------------------
void MeshField::update() {


}

//==============================================================================
// draw
//------------------------------------------------------------------------------
void MeshField::draw(const Renderer* renderer) {

  auto pDevice = renderer->getDevice();
  auto shader = renderer->getShader();

  shader->setVtxShader(_vtxShaderID);
  auto vtxShader = shader->getNowVtxShader();

  UINT nSamplerIndex = shader->getNowPixShader()->_constTable->GetSamplerIndex("TexSamp0");
  pDevice->SetTexture(nSamplerIndex,renderer->getTexture()->getTexture(_textureID));

  // 色
  vtxShader->_constTable->SetFloatArray(pDevice,"gMaterial",_color,4);

  Matrix wvp = getWorldMtx() * renderer->getCamera()->getViewMtx() * renderer->getProjMtx();
  vtxShader->_constTable->SetMatrix(pDevice,"gWorld",&getWorldMtx());
  vtxShader->_constTable->SetMatrix(pDevice,"gWVP",&wvp);

  // デクラレーション設定
  pDevice->SetVertexDeclaration(_p3DDec);

  // 頂点送信
  pDevice->SetStreamSource(0,_vtxBuff,0,sizeof(VERTEX_3D));
  pDevice->SetIndices(_indexBuff);

  //様々なオブジェクトの描画処理を行う
  pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,0,0,_nNumVertex,0,_nNumPolygon);
}

//EOF