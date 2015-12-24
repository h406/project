//=============================================================================
//
// Instancing[Instancing.cpp]
// Author : Yasuaki Yamashita : 2015/12/22
//
//==============================================================================

//******************************************************************************
// include
//******************************************************************************
#include "Instancing3D.h"
#include "renderer.h"
#include "app.h"
#include "texture.h"
#include "shader.h"
#include "camera.h"

namespace {

  struct VERTEX_3D {
    D3DXVECTOR3   vtx; // 頂点座標
    D3DXVECTOR2   tex; // テクスチャ座標
  };

  struct INSTANCING_DATA {
    Matrix _World;
    Vec4   _Material;
    Vec4   _UV;
  };

  // 頂点要素の配列作成
  const D3DVERTEXELEMENT9 vElement[] = {
    // Stream0
    {0, 0,D3DDECLTYPE_FLOAT3,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_POSITION,0}, // 座標
    {0,12,D3DDECLTYPE_FLOAT2,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD,0}, // UV

    // Stream1
    {1, 0,D3DDECLTYPE_FLOAT4,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD,1},  // WORLD 1行目
    {1,16,D3DDECLTYPE_FLOAT4,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD,2},  // WORLD 2行目
    {1,32,D3DDECLTYPE_FLOAT4,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD,3},  // WORLD 3行目
    {1,48,D3DDECLTYPE_FLOAT4,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD,4},  // WORLD 4行目
    {1,64,D3DDECLTYPE_FLOAT4,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD,5},  // Material
    {1,80,D3DDECLTYPE_FLOAT4,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD,6},  // UV

    D3DDECL_END()
  };

  // インスタンシング個数
  const unsigned int kInstancingNums = 256;
}

//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
bool Instancing3D::init() {
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
  pVtx[0].vtx = Vec3(-0.5f,0.5f,0.f);
  pVtx[1].vtx = Vec3(0.5f,0.5f,0.f);
  pVtx[2].vtx = Vec3(-0.5f,-0.5f,0.f);
  pVtx[3].vtx = Vec3(0.5f,-0.5f,0.f);
  pVtx[0].tex = D3DXVECTOR2(0,0);
  pVtx[1].tex = D3DXVECTOR2(1,0);
  pVtx[2].tex = D3DXVECTOR2(0,1);
  pVtx[3].tex = D3DXVECTOR2(1,1);
  _vtxBuff->Unlock();

  // インスタンシング
  if(FAILED(pDevice->CreateVertexBuffer(
    sizeof(INSTANCING_DATA) * kInstancingNums,
    D3DUSAGE_WRITEONLY,
    NULL,
    D3DPOOL_MANAGED,
    &_InstancingBuff,
    NULL
    ))) {
    return false;
  }
  // インスタンシングデータ
  INSTANCING_DATA *pData = nullptr;
  //位置更新
  _InstancingBuff->Lock(0,0,(void **)&pData,0);
  for(int i = 0; i < kInstancingNums; ++i) {
    ZeroMemory(&pData[i]._World,sizeof(Matrix));
    pData[i]._UV = Vec4(0,0,1,1);
    pData[i]._Material = Vec4(1,1,1,1);
  }
  _InstancingBuff->Unlock();


  // インデックスバッファ
  WORD index[6] = {0,1,2,2,1,3};
  pDevice->CreateIndexBuffer(sizeof(index),0,D3DFMT_INDEX16,D3DPOOL_MANAGED,&_IndexBuff,0);
  void *p = 0;
  _IndexBuff->Lock(0,0,&p,0);
  memcpy(p,index,sizeof(index));
  _IndexBuff->Unlock();

  // 頂点
  pDevice->CreateVertexDeclaration(vElement,&_p3DDec);

  // シェーダ
  _vtxShaderID = renderer->getShader()->getVtxShader("vs_instancing.cso");

  return true;
}

bool Instancing3D::init(const char* file) {
  _textureID = App::instance().getRenderer()->getTexture()->createTexture(file);
  return init();
}

//==============================================================================
//
//------------------------------------------------------------------------------
void Instancing3D::draw(const  Renderer* renderer) {
  auto pDevice = renderer->getDevice();
  auto shader = renderer->getShader();

  shader->setVtxShader(_vtxShaderID);
  auto vtxShader = shader->getNowVtxShader();

  // インスタンシング
  pDevice->SetStreamSourceFreq(0,D3DSTREAMSOURCE_INDEXEDDATA | kInstancingNums);
  pDevice->SetStreamSourceFreq(1,D3DSTREAMSOURCE_INSTANCEDATA | 1u);

  // テクスチャ
  UINT nSamplerIndex = shader->getNowPixShader()->_constTable->GetSamplerIndex("TexSamp0");
  if(UINT_MAX != nSamplerIndex) {
    pDevice->SetTexture(nSamplerIndex,renderer->getTexture()->getTexture(_textureID));
  }

  // VP
  Matrix vp = renderer->getCamera()->getViewMtx() * renderer->getProjMtx();
  vtxShader->_constTable->SetMatrix(pDevice,"gWorld",&getWorldMtx());
  vtxShader->_constTable->SetMatrix(pDevice,"gVP",&vp);

  // デクラレーション設定
  pDevice->SetVertexDeclaration(_p3DDec);

  // 頂点送信
  pDevice->SetStreamSource(0,_vtxBuff,0,sizeof(VERTEX_3D));
  pDevice->SetStreamSource(1,_InstancingBuff,0,sizeof(INSTANCING_DATA));
  pDevice->SetIndices(_IndexBuff);

  // 描画
  pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,4,0,2);

  // 元に戻す
  pDevice->SetStreamSourceFreq(0,1);
  pDevice->SetStreamSourceFreq(1,1);
}

//==============================================================================
// updateMtxChild
//------------------------------------------------------------------------------
void Instancing3D::updateMtxChild() {
  if(_worldChenged) {
    updateWorldMtx();
  }

  int i = 0;
  // インスタンシングデータ
  INSTANCING_DATA *pData = nullptr;
  //位置更新
  _InstancingBuff->Lock(0,0,(void **)&pData,0);
  // update
  for(node* obj : this->getChildList()) {
    DrawObject3D* sprite = (DrawObject3D*)obj;
    const float uvAnims[4] = {
      sprite->getTexScl().x / sprite->getNumU(),
      sprite->getTexScl().y / sprite->getNumV(),
      (1.f / sprite->getNumU()) * (sprite->getAnimID() % sprite->getNumU()) + (sprite->getTexPos().x),
      (1.f / sprite->getNumV()) * (sprite->getAnimID() / sprite->getNumU()) + (sprite->getTexPos().y)
    };

    if(_worldChenged) {
      obj->setWorldChenged(true);
    }
    obj->updateMtxChild();

    pData[i]._World = obj->getWorldMtx();

    pData[i]._UV = uvAnims;
    pData[i]._Material = (float*)sprite->getColor();
    ++i;
  }
  _InstancingBuff->Unlock();
}

//==============================================================================
// uninit
//------------------------------------------------------------------------------
void Instancing3D::uninit() {
  SafeRelease(_vtxBuff);
  SafeRelease(_InstancingBuff);
  SafeRelease(_p3DDec);
  SafeRelease(_IndexBuff);
}

//==============================================================================
// drawChild
//------------------------------------------------------------------------------
void Instancing3D::drawChild(const Renderer* renderer,NodeType type) {
  if(this->isVisible() && type == this->getNodeType()) {
    this->draw(renderer);
  }

  // 削除リストチェック
  removeCheck();

  // ドローしない
  for(node* obj : this->getChildList()) {
    obj->drawChild(renderer,NodeType::none);
  }
}
//EOF