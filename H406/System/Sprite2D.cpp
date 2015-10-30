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
    {0,12,D3DDECLTYPE_FLOAT2,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD,0},
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

  // �F�X������
  _numU = 1;
  _numV = 1;
  _animID = 0;
  _texPos = {0,0};
  _texScl = {1.f,1.f};

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
  float uvAnims[4];
  auto shader = renderer->getShader();

  // uv�A�j���[�V�����v�Z
  uvAnims[0] = _texScl.x / _numU;
  uvAnims[1] = _texScl.y / _numV;
  uvAnims[2] = (1.f / _numU) * (_animID % _numU) + (_texPos.x);
  uvAnims[3] = (1.f / _numV) * (_animID / _numU) + (_texPos.y);

  shader->setVtxShader(_vtxShaderID);
  auto vtxShader = shader->getNowVtxShader();

  vtxShader->_constTable->SetMatrix(pDevice, "gWorld",&_mtxWorld);

  UINT nSamplerIndex = shader->getNowPixShader()->_constTable->GetSamplerIndex("TexSamp0");
  if(UINT_MAX != nSamplerIndex) {
    pDevice->SetTexture(nSamplerIndex,renderer->getTexture()->getTexture(_textureID));
  }

  // �F
  vtxShader->_constTable->SetFloatArray(pDevice,"gMaterial",_color,4);

  // uv
  vtxShader->_constTable->SetFloatArray(pDevice,"gUVAnim",uvAnims,4);

  // projMtx
  const D3DXMATRIX mtxProj(
    2 / (float)App::instance().getWindowSize().cx,0.0f,0.0f,0.0f,
    0.0f,-2 / (float)App::instance().getWindowSize().cy,0.0f,0.0f,
    0.0f,0.0f,1.0f,0.0f,
    -1.0f,1.0f,0.0f,1.0f
    );

  // projMtx
  vtxShader->_constTable->SetMatrix(pDevice,"gProj",&mtxProj);

  // WindowSize
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