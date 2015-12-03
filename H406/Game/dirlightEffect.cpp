//==============================================================================
//
// DirlightEffect[DirlightEffect.cpp]
// Author : Yasuaki Yamashita : 2015/10/30
//
//==============================================================================

//******************************************************************************
// include
//******************************************************************************
#include "dirlightEffect.h"

//==============================================================================
// init
//------------------------------------------------------------------------------
bool DirlightEffect::init() {
  return true;
}

//==============================================================================
// update
//------------------------------------------------------------------------------
void DirlightEffect::update() {

}

//==============================================================================
// draw
//------------------------------------------------------------------------------
void DirlightEffect::draw(Renderer *renderer,Sprite2D* sprite) {
  const auto pDevice = renderer->getDevice();
  auto shader = renderer->getShader();

  shader->setPixShader("ps_dirlight.cso");

  pDevice->SetTexture(0,renderer->getNormalDepthTex());

  Vec4 color[3];
  float power[3];
  Vec3 dir[3];

  color[0] = Vec4(1.f,1.f,1.f,1);
  power[0] = 1.0f;
  dir[0] = Vec3(0.5f,-1,2);

  color[1] = Vec4(1.f,1.f,1.f,1);
  power[1] = 0.4f;
  dir[1] = Vec3(-0.5f,-1,-2);

  color[2] = Vec4(1.f,1.f,1.f,1);
  power[2] = 0.5f;
  dir[2] = Vec3(0.5f,3.f,0);

  for(int i = 0; i < 3; i++) {
    D3DXVec3Normalize(&dir[i],&dir[i]);
  }

  shader->getNowPixShader()->_constTable->SetFloatArray(pDevice,"gLightDir",(float*)dir,sizeof(float) * 3 * 3);
  shader->getNowPixShader()->_constTable->SetFloatArray(pDevice,"gLightPower",power,sizeof(float) * 3);
  shader->getNowPixShader()->_constTable->SetFloatArray(pDevice,"gLightCol",(float*)color,sizeof(float) * 4 * 3);

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
void DirlightEffect::uninit() {

}

//EOF