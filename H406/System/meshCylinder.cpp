//==============================================================================
//
// meshCylinder [meshCylinder.cpp]
// Author : masato masuda : 2015/10/14
//
//==============================================================================

//******************************************************************************
// include
//******************************************************************************
#include "meshCylinder.h"
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
bool MeshCylinder::init(int nNumBlockX,int nNumBlockZ,float fSizeBlockX,float fSizeBlockZ) {
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
void MeshCylinder::initVtx() {

  VERTEX_3D *pVtx = nullptr;
  WORD *pIndex = nullptr;
  int vtxWork = 0;

  // 半径の計算
  float	cylinderVtxX = 0 - ((_nNumBlockX * _fSizeBlockX)) * 0.5f;
  float	cylinderVtxY = 0 - ((_nNumBlockY * _fSizeBlockY)) * 0.5f;

  // 頂点バッファ設定
  _vtxBuff->Lock(0,0,(void **)&pVtx,0);

  for (float cntZ = 0.0f; cntZ <= _nNumBlockY; cntZ++)
  {
    for (float cntX = 0.0f; cntX <= _nNumBlockX; cntX++)
    {
      // 角度の算出
      float	angle = (float)(((D3DX_PI * 2) / _nNumBlockX) * cntX);
      float	length = (_nNumBlockX * _fSizeBlockX) * 0.5f;

      // 座標
      pVtx[vtxWork].vtx = D3DXVECTOR3((-cos(angle) * length),
        (-(cntZ * _fSizeBlockY)) - (cylinderVtxY * 2),
        (sin(angle) * length));

      // 法線
      pVtx[vtxWork].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

      // テクスチャー
      //pVtx[vtxWork].tex = D3DXVECTOR2((float)(cntX / _nNumBlockX), (float)(cntZ / _nNumBlockX));
      pVtx[vtxWork].tex = D3DXVECTOR2(cntX, cntZ);

      // ワーク更新
      vtxWork++;
    }

    // 座標始点Xを戻す
    cylinderVtxX = 0 - (_nNumBlockX  * _fSizeBlockX) * 0.5f;
  }

  _vtxBuff->Unlock();

  // インデックスバッファ設定
  _indexBuff->Lock(0,0,(void **)&pIndex,0);

  int idxCnt = 0;
  for (int indexCnt = 0; indexCnt < _nNumVertexIndex; indexCnt++){
    // 重複している場合
    if (indexCnt != 0 && (((indexCnt - (2 * _nNumBlockX + 2)) % ((2 * _nNumBlockX + 2) + 2) == 0)
      || (indexCnt - ((2 * _nNumBlockX + 2) + 2)) % ((2 * _nNumBlockX + 2) + 2) == 0)){
      // 前回と同じインデックス番号を入れる
      pIndex[indexCnt] = pIndex[indexCnt - 1];
      idxCnt--;
    }else{
      // 重複していない場合
      pIndex[indexCnt] = (WORD)((_nNumBlockX + 1) + (idxCnt / 2)) - (WORD)((idxCnt % 2) * (_nNumBlockX + 1));
    }

    // ワーク用
    idxCnt++;
  }

  _indexBuff->Unlock();
}

//==============================================================================
// uninit
//------------------------------------------------------------------------------
void MeshCylinder::uninit() {
  SafeRelease(_vtxBuff);
  SafeRelease(_indexBuff);
  SafeRelease(_p3DDec);
  SafeDelete(_pVtxList);
  SafeDelete(_pNorList);
}

//==============================================================================
// update
//------------------------------------------------------------------------------
void MeshCylinder::update() {

}

//==============================================================================
// draw
//------------------------------------------------------------------------------
void MeshCylinder::draw(const Renderer* renderer) {

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