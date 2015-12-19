//==============================================================================
//
// TestEffect[test.cpp]
// Author : Yasuaki Yamashita : 2015/10/30
//
//==============================================================================

//******************************************************************************
// include
//******************************************************************************
#include "test.h"

//==============================================================================
// init
//------------------------------------------------------------------------------
bool TestEffect::init() {


  App::instance().getRenderer()->getTexture()->createTexture("./data/texture/rand.dds");
  return true;
}

//==============================================================================
// update
//------------------------------------------------------------------------------
void TestEffect::update() {

}

//==============================================================================
// draw
//------------------------------------------------------------------------------
void TestEffect::draw(Renderer *renderer,Sprite2D* sprite) {
  const auto pDevice = renderer->getDevice();
  auto shader = renderer->getShader();

  shader->setPixShader("ps_test.cso");
  auto constTable = shader->getNowPixShader()->_constTable;

  pDevice->SetTexture(0,renderer->getNormalDepthTex());
  pDevice->SetTexture(1,renderer->getPosTex());
  pDevice->SetTexture(2,renderer->getColorTex());

  const SIZE size = App::instance().getWindowSize();
  const float w = (float)size.cx;
  const float h = (float)size.cy;

  Vec3 dir[4];
  Vec3 col[4];

  static float f = 0;

  f += 0.02f;

  col[0] = Vec3(1,0,0);
  col[1] = Vec3(0,1,0);
  col[2] = Vec3(0,0,1);
  col[3] = Vec3(1,1,1);

  constTable->SetFloatArray(pDevice,"gCameraPos",(float*)&renderer->getCamera()->getCurrentCamera()->getPosP(),sizeof(float) * 3);
  constTable->SetFloat(pDevice,"gFar",2000);

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
  pDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
  pDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ONE);
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
void TestEffect::uninit() {

}

//EOF