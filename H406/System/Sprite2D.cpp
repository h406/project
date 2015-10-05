//==============================================================================
//
// Sprite2D[Sprite2D.cpp]
// Author : Yasuaki Yamashita : 2015/10/05
//
//==============================================================================

//******************************************************************************
// include
//******************************************************************************
#include "Sprite2D.h"
#include "renderer.h"
#include "app.h"
#include "texture.h"
#include "shader.h"

namespace {
  struct VERTEX_2D {
    D3DXVECTOR3   vtx; // 頂点座標
    D3DXVECTOR2   tex; // テクスチャ座標
  };

  const D3DVERTEXELEMENT9 Vtx2dDecl[] =
  {
    {0,0,D3DDECLTYPE_FLOAT3,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_POSITION,0},
    {0,sizeof(float) * 3,D3DDECLTYPE_FLOAT2,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD,0},
    D3DDECL_END()
  };
}

//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
bool Sprite2D::init() {
  auto renderer = App::instance().getRenderer();
  auto pDevice = renderer->getDevice();

  // 頂点作成
  if(FAILED(pDevice->CreateVertexBuffer(
    sizeof(VERTEX_2D) * 4,
    D3DUSAGE_WRITEONLY,
    NULL,
    D3DPOOL_MANAGED,
    &_vtxBuff,
    NULL
    ))) {
    return false;
  }

  //頂点バッファ
  VERTEX_2D *pVtx = nullptr;
  //位置更新
  _vtxBuff->Lock(0,0,(void **)&pVtx,0);
  pVtx[0].vtx = D3DXVECTOR3(-0.5f,-0.5f,0.f);
  pVtx[1].vtx = D3DXVECTOR3(0.5f,-0.5f,0.f);
  pVtx[2].vtx = D3DXVECTOR3(-0.5f,0.5f,0.f);
  pVtx[3].vtx = D3DXVECTOR3(0.5f,0.5f,0.f);
  pVtx[0].tex = D3DXVECTOR2(0,0);
  pVtx[1].tex = D3DXVECTOR2(1,0);
  pVtx[2].tex = D3DXVECTOR2(0,1);
  pVtx[3].tex = D3DXVECTOR2(1,1);
  _vtxBuff->Unlock();

  // 頂点
  pDevice->CreateVertexDeclaration(Vtx2dDecl,&_p2DDec);

  // シェーダ
  _vtxShaderID = renderer->getShader()->getVtxShader("vs_2d.cso");

  return true;
}

bool Sprite2D::init(const char* file) {
  _textureID = App::instance().getRenderer()->getTexture()->createTexture(file);
  return init();
}

//==============================================================================
//
//------------------------------------------------------------------------------
void Sprite2D::draw(Renderer* renderer) {
  auto pDevice = App::instance().getRenderer()->getDevice();
  static D3DXMATRIX mtxWorld,mtxTmp;
  static float uvAnims[4];
  const float sinb = sinf(getRot().y);
  const float cosb = cosf(getRot().y);

  D3DXMatrixIdentity(&mtxWorld);

  // 移動行列
  D3DXMatrixIdentity(&mtxTmp);
  mtxTmp._41 = getPos().x;
  mtxTmp._42 = getPos().y;
  mtxWorld = mtxTmp * mtxWorld;

  // 回転行列
  mtxTmp._41 = 0;
  mtxTmp._42 = 0;
  mtxTmp._11 = cosb;
  mtxTmp._12 = sinb;
  mtxTmp._21 = -sinb;
  mtxTmp._22 = cosb;
  mtxWorld = mtxTmp * mtxWorld;

  // 拡大行列に倍率を設定
  mtxTmp._11 = getScl().x; // Ｘ倍率
  mtxTmp._12 = 0;
  mtxTmp._21 = 0;
  mtxTmp._22 = getScl().y; // Ｙ倍率
  mtxWorld = mtxTmp * mtxWorld;

  // uvアニメーション計算
  uvAnims[0] = 1; //1.f / sprite->_numU;
  uvAnims[1] = 1; //1.f / sprite->_numV;
  uvAnims[2] = 0; //uvAnims[0] * (sprite->_anim % sprite->_numU) + (uvAnims[0] / sprite->_size.x * sprite->_texPos.x);
  uvAnims[3] = 0; //uvAnims[1] * (sprite->_anim / sprite->_numU) + (uvAnims[1] / sprite->_size.y * sprite->_texPos.y);

  // シェーダ設定
  g_2DEffect->SetMatrix(g_2DHandle.world,&mtxWorld);

  // テクスチャ設定
  g_2DEffect->SetTexture(g_2DHandle.tex,g_TexList[sprite->_tex]);

  // 色
  g_2DEffect->SetFloatArray(g_2DHandle.col,sprite->_col,4);

  // uv
  g_2DEffect->SetFloatArray(g_2DHandle.uvAnim,uvAnims,4);

  // コミット
  g_2DEffect->CommitChanges();

  // 描画
  pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,0,2);
}

//EOF