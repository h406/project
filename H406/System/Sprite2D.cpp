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
    D3DXVECTOR3   vtx; // ���_���W
    D3DXVECTOR2   tex; // �e�N�X�`�����W
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

  // ���_�쐬
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

  //���_�o�b�t�@
  VERTEX_2D *pVtx = nullptr;
  //�ʒu�X�V
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

  // ���_
  pDevice->CreateVertexDeclaration(Vtx2dDecl,&_p2DDec);

  // �V�F�[�_
  _vtxShaderID = renderer->getShader()->getVtxShader("vs_2d.cso");

  // �F
  _color = D3DXCOLOR(1,1,1,1);

  _textureID = 0;

  return true;
}

bool Sprite2D::init(const char* file) {
  _textureID = App::instance().getRenderer()->getTexture()->createTexture(file);
  return init();
}

//==============================================================================
//
//------------------------------------------------------------------------------
void Sprite2D::draw(const  Renderer* renderer) {
  auto pDevice = renderer->getDevice();
  static D3DXMATRIX mtxWorld,mtxTmp;
  static float uvAnims[4];
  const float sinb = sinf(getRot().y);
  const float cosb = cosf(getRot().y);

  // projMtx
  const D3DXMATRIX mtxProj(
    2 / (float)App::instance().getWindowSize().cx,0.0f,0.0f,0.0f,
    0.0f,-2 / (float)App::instance().getWindowSize().cy,0.0f,0.0f,
    0.0f,0.0f,1.0f,0.0f,
    -1.0f,1.0f,0.0f,1.0f
  );

  D3DXMatrixIdentity(&mtxWorld);

  // �ړ��s��
  D3DXMatrixIdentity(&mtxTmp);
  mtxTmp._41 = getPos().x;
  mtxTmp._42 = getPos().y;
  mtxWorld = mtxTmp * mtxWorld;

  // ��]�s��
  mtxTmp._41 = 0;
  mtxTmp._42 = 0;
  mtxTmp._11 = cosb;
  mtxTmp._12 = sinb;
  mtxTmp._21 = -sinb;
  mtxTmp._22 = cosb;
  mtxWorld = mtxTmp * mtxWorld;

  // �g��s��ɔ{����ݒ�
  mtxTmp._11 = getScl().x; // �w�{��
  mtxTmp._12 = 0;
  mtxTmp._21 = 0;
  mtxTmp._22 = getScl().y; // �x�{��
  mtxWorld = mtxTmp * mtxWorld;

  // uv�A�j���[�V�����v�Z
  uvAnims[0] = 1; //1.f / sprite->_numU;
  uvAnims[1] = 1; //1.f / sprite->_numV;
  uvAnims[2] = 0; //uvAnims[0] * (sprite->_anim % sprite->_numU) + (uvAnims[0] / sprite->_size.x * sprite->_texPos.x);
  uvAnims[3] = 0; //uvAnims[1] * (sprite->_anim / sprite->_numU) + (uvAnims[1] / sprite->_size.y * sprite->_texPos.y);

  auto shader = renderer->getShader();

  shader->setVtxShader(_vtxShaderID);

  auto vtxShader = shader->getNowVtxShader();

  vtxShader->_constTable->SetMatrix(pDevice, "gWorld",&mtxWorld);

  UINT nSamplerIndex = shader->getNowPixShader()->_constTable->GetSamplerIndex("TexSamp0");

  pDevice->SetTexture(nSamplerIndex,renderer->getTexture()->getTexture(_textureID));

  vtxShader->_constTable->SetMatrix(pDevice,"gProj",&mtxProj);

  // �F
  vtxShader->_constTable->SetFloatArray(pDevice,"gMaterial",_color,4);

  // uv
  vtxShader->_constTable->SetFloatArray(pDevice,"gUVAnim",uvAnims,4);

  // WindowSize�K�p
  const float pWindowSize[] = {(float)App::instance().getWindowSize().cx,(float)App::instance().getWindowSize().cy};
  vtxShader->_constTable->SetFloatArray(pDevice,"gScreenSize",pWindowSize,sizeof(pWindowSize) / sizeof(float));

  // �f�N�����[�V�����ݒ�
  pDevice->SetVertexDeclaration(_p2DDec);

  // ���_���M
  pDevice->SetStreamSource(0,_vtxBuff,0,sizeof(VERTEX_2D));

  // �`��
  pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,0,2);
}

//==============================================================================
//
//------------------------------------------------------------------------------
void Sprite2D::update() {

}

//==============================================================================
//
//------------------------------------------------------------------------------
void Sprite2D::uninit() {
  SafeRelease(_vtxBuff);
  SafeRelease(_p2DDec);
}

//EOF