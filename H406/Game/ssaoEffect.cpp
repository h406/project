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
  pDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_SRCCOLOR);
  pDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_ZERO);

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
}

//EOF