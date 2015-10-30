//==============================================================================
//
// EdgeEffect[EdgeEffect.cpp]
// Author : Yasuaki Yamashita : 2015/10/30
//
//==============================================================================

//******************************************************************************
// include
//******************************************************************************
#include "edgeEffect.h"

//==============================================================================
// init
//------------------------------------------------------------------------------
bool EdgeEffect::init() {
  return true;
}

//==============================================================================
// update
//------------------------------------------------------------------------------
void EdgeEffect::update() {

}

//==============================================================================
// draw
//------------------------------------------------------------------------------
void EdgeEffect::draw(Renderer *renderer,Sprite2D* sprite) {
  const auto pDevice = renderer->getDevice();
  auto shader = renderer->getShader();

  shader->setPixShader("ps_edge.cso");

  pDevice->SetTexture(0,renderer->getNormalTex());
  pDevice->SetTexture(1,renderer->getDepthTex());

  const SIZE size = App::instance().getWindowSize();
  const float w = (float)size.cx;
  const float h = (float)size.cy;

  struct T4VERTEX {
    float p[4];
    float t[2];
  }  Vertex[4] = {
    {0.0f,0,0.1f,1.0f,0.0f,0.0f},
    {w,0,0.1f,1.0f,1.0f,0.0f},
    {w,h,0.1f,1.0f,1.0f,1.0f},
    {0.0f,h,0.1f,1.0f,0.0f,1.0f},
  };

  pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
  pDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_TEX1);
  pDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN,2,Vertex,sizeof(T4VERTEX));
  pDevice->SetFVF(NULL);
  pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
}

//EOF