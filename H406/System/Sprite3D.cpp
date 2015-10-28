//==============================================================================
//
// Sprite3D[Sprite3D.cpp]
// Author : Yasuaki Yamashita : 2015/10/05
//
//==============================================================================

//******************************************************************************
// include
//******************************************************************************
#include "Sprite3D.h"
#include "renderer.h"
#include "app.h"
#include "texture.h"
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
}

//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
bool Sprite3D::init() {
  auto renderer = App::instance().getRenderer();
  auto pDevice = renderer->getDevice();

  // 頂点作成
  if(FAILED(pDevice->CreateVertexBuffer(
    sizeof(VERTEX_3D) * 4,
    D3DUSAGE_WRITEONLY,
    NULL,
    D3DPOOL_MANAGED,
    &_vtxBuff,
    NULL
    ))) {
    return false;
  }

  //頂点バッファ
  VERTEX_3D *pVtx = nullptr;
  //位置更新
  _vtxBuff->Lock(0,0,(void **)&pVtx,0);
  pVtx[0].vtx = D3DXVECTOR3(-0.5f,0.5f,0.f);
  pVtx[1].vtx = D3DXVECTOR3(0.5f,0.5f,0.f);
  pVtx[2].vtx = D3DXVECTOR3(-0.5f,-0.5f,0.f);
  pVtx[3].vtx = D3DXVECTOR3(0.5f,-0.5f,0.f);
  pVtx[0].nor = D3DXVECTOR3(0,0,-1);
  pVtx[1].nor = D3DXVECTOR3(0,0,-1);
  pVtx[2].nor = D3DXVECTOR3(0,0,-1);
  pVtx[3].nor = D3DXVECTOR3(0,0,-1);
  pVtx[0].tex = D3DXVECTOR2(0,0);
  pVtx[1].tex = D3DXVECTOR2(1,0);
  pVtx[2].tex = D3DXVECTOR2(0,1);
  pVtx[3].tex = D3DXVECTOR2(1,1);
  _vtxBuff->Unlock();

  // 頂点
  pDevice->CreateVertexDeclaration(vElement,&_p3DDec);

  // シェーダ
  _vtxShaderID = renderer->getShader()->getVtxShader("vs_model.cso");

  // 色
  _color = D3DXCOLOR(1,1,1,1);

  return true;
}

bool Sprite3D::init(const char* file) {
  _textureID = App::instance().getRenderer()->getTexture()->createTexture(file);
  return init();
}

//==============================================================================
//
//------------------------------------------------------------------------------
void Sprite3D::draw(const  Renderer* renderer) {
  auto pDevice = renderer->getDevice();
  auto shader = renderer->getShader();

  shader->setVtxShader(_vtxShaderID);
  auto vtxShader = shader->getNowVtxShader();

  UINT nSamplerIndex = shader->getNowPixShader()->_constTable->GetSamplerIndex("TexSamp0");
  pDevice->SetTexture(nSamplerIndex,renderer->getTexture()->getTexture(_textureID));

  // 色
  vtxShader->_constTable->SetFloatArray(pDevice,"gMaterial",_color,4);

  Matrix norMtx;
  D3DXMatrixInverse(&norMtx,nullptr,&getWorldMtx());
  D3DXMatrixTranspose(&norMtx,&norMtx);
  vtxShader->_constTable->SetMatrix(pDevice,"gNorWorld",&norMtx);

  Matrix wvp = getWorldMtx() * renderer->getCamera()->getViewMtx() * renderer->getProjMtx();
  vtxShader->_constTable->SetMatrix(pDevice,"gWorld",&getWorldMtx());
  vtxShader->_constTable->SetMatrix(pDevice,"gWVP",&wvp);

  // デクラレーション設定
  pDevice->SetVertexDeclaration(_p3DDec);

  // 頂点送信
  pDevice->SetStreamSource(0,_vtxBuff,0,sizeof(VERTEX_3D));

  // 描画
  pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,0,2);
}

//==============================================================================
//
//------------------------------------------------------------------------------
void Sprite3D::update() {

}

//==============================================================================
//
//------------------------------------------------------------------------------
void Sprite3D::uninit() {
  SafeRelease(_vtxBuff);
  SafeRelease(_p3DDec);
}

//EOF