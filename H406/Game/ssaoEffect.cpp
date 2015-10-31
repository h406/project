//==============================================================================
//
// SsaoEffect[SsaoEffect.cpp]
// Author : Yasuaki Yamashita : 2015/10/30
//
//==============================================================================

//******************************************************************************
// include
//******************************************************************************
#include "ssaoEffect.h"

//==============================================================================
// init
//------------------------------------------------------------------------------
bool SsaoEffect::init() {
  const auto pDevice = App::instance().getRenderer()->getDevice();
  pDevice->CreateTexture(16,16,1,0,D3DFMT_A32B32G32R32F,D3DPOOL_MANAGED,&_rayMap,0);
  D3DXFillTexture(_rayMap,makeRayMap,0);

  App::instance().getRenderer()->getTexture()->createTexture("./data/texture/rand.dds");

  return true;
}

//==============================================================================
// update
//------------------------------------------------------------------------------
void SsaoEffect::update() {

}

//==============================================================================
// draw
//------------------------------------------------------------------------------
void SsaoEffect::draw(Renderer *renderer,Sprite2D* sprite) {
  const auto pDevice = renderer->getDevice();
  auto shader = renderer->getShader();

  const SIZE size = App::instance().getWindowSize();
  const float w = (float)size.cx;
  const float h = (float)size.cy;

  shader->setPixShader("ps_ssao.cso");

  pDevice->SetTexture(0,renderer->getNormalDepthTex());
  pDevice->SetTexture(1,renderer->getTexture()->getTexture("./data/texture/rand.dds"));

  pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
  pDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_ZERO);
  pDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_SRCALPHA);

  struct T4VERTEX {
    float p[4];
    float t[2];
  }  Vertex[4] = {
    {0.0f,0,0.1f,1.0f,0.0f,0.0f},
    {w,0,0.1f,1.0f,1.0f,0.0f},
    {w,h,0.1f,1.0f,1.0f,1.0f},
    {0.0f,h,0.1f,1.0f,0.0f,1.0f},
  };

  pDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_TEX4);
  pDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN,2,Vertex,sizeof(T4VERTEX));
  pDevice->SetFVF(NULL);

  pDevice->SetRenderState(D3DRS_ZENABLE,TRUE);
  pDevice->SetRenderState(D3DRS_LIGHTING,TRUE);
  pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,FALSE);
}

//==============================================================================
// uninit
//------------------------------------------------------------------------------
void SsaoEffect::uninit() {
  SafeRelease(_rayMap);
}

//==============================================================================
// uninit
//------------------------------------------------------------------------------
void WINAPI SsaoEffect::makeRayMap(D3DXVECTOR4* pOut,const D3DXVECTOR2*,const D3DXVECTOR2*,void*) {
  /*
  float r = 1.0f * (float)rand() / (float)RAND_MAX;
  float t = 6.2831853f * (float)rand() / ((float)RAND_MAX + 1.0f);
  float cp = 2.0f * (float)rand() / (float)RAND_MAX - 1.0f;
  float sp = sqrt(1.0f - cp * cp);
  float ct = cos(t), st = sin(t);

  pOut->x = abs(r * sp * ct);
  pOut->y = abs(r * sp * st);
  pOut->z = abs(r * cp);
  pOut->w = 0;
  //*/

  //*/
  static int i = 0;

  D3DXVECTOR3 vec;
  switch(i % 16) {
  case 0: vec = D3DXVECTOR3(0.53812504f,0.18565957f,-0.43192f); break;
  case 1: vec = D3DXVECTOR3(0.13790712f,0.24864247f,0.44301823f); break;
  case 2: vec = D3DXVECTOR3(0.33715037f,0.56794053f,-0.005789503f); break;
  case 3: vec = D3DXVECTOR3(-0.6999805f,-0.04511441f,-0.0019965635f); break;
  case 4: vec = D3DXVECTOR3(0.06896307f,-0.15983082f,-0.85477847f); break;
  case 5: vec = D3DXVECTOR3(0.056099437f,0.006954967f,-0.1843352f); break;
  case 6: vec = D3DXVECTOR3(-0.014653638f,0.14027752f,0.0762037f); break;
  case 7: vec = D3DXVECTOR3(0.010019933f,-0.1924225f,-0.034443386f); break;
  case 8: vec = D3DXVECTOR3(-0.35775623f,-0.5301969f,-0.43581226f); break;
  case 9: vec = D3DXVECTOR3(-0.3169221f,0.106360726f,0.015860917f); break;
  case 10:vec = D3DXVECTOR3(0.010350345f,-0.58698344f,0.0046293875f); break;
  case 11:vec = D3DXVECTOR3(-0.08972908f,-0.49408212f,0.3287904f); break;
  case 12:vec = D3DXVECTOR3(0.7119986f,-0.0154690035f,-0.09183723f); break;
  case 13:vec = D3DXVECTOR3(-0.053382345f,0.059675813f,-0.5411899f); break;
  case 14:vec = D3DXVECTOR3(0.035267662f,-0.063188605f,0.54602677f); break;
  case 15:vec = D3DXVECTOR3(-0.47761092f,0.2847911f,-0.0271716f); break;
  }
  // テクスチャなので0-1の値しか持てないので、-1,1を0,1へ変換
  pOut->x = (vec.x + 1.f)*0.5f;
  pOut->y = (vec.y + 1.f)*0.5f;
  pOut->z = (vec.z + 1.f)*0.5f;
  pOut->w = 0.f;
  ++i;
  //*/
}


//EOF