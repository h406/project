//==============================================================================
//
// FxaaEffect[fxaa.cpp]
// Author : Yasuaki Yamashita : 2015/10/30
//
//==============================================================================

//******************************************************************************
// include
//******************************************************************************
#include "fxaa.h"

//==============================================================================
// init
//------------------------------------------------------------------------------
bool FxaaEffect::init() {
  return true;
}

//==============================================================================
// update
//------------------------------------------------------------------------------
void FxaaEffect::update() {

}

//==============================================================================
// draw
//------------------------------------------------------------------------------
void FxaaEffect::draw(Renderer *renderer,Sprite2D* ) {
  const auto pDevice = renderer->getDevice();
  auto shader = renderer->getShader();

  shader->setPixShader("ps_fxaa.cso");
  pDevice->SetTexture(0,renderer->getColorTex());

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

  pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,FALSE);
  pDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCCOLOR);
  pDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_ZERO);
  pDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_TEX1);
  pDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN,2,Vertex,sizeof(T4VERTEX));
  pDevice->SetFVF(NULL);
  pDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);		// ソース
  pDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);	// デスト
  pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,FALSE);
}

//==============================================================================
// uninit
//------------------------------------------------------------------------------
void FxaaEffect::uninit() {

}

//EOF