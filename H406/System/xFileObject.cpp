//==============================================================================
//
// xFileObject[xFileObject.cpp]
// Author : Yasuaki Yamashita : 2015/06/29
//
//==============================================================================

//******************************************************************************
// include
//******************************************************************************
#include "xFileObject.h"
#include "app.h"
#include "renderer.h"
#include "shader.h"
#include "camera.h"

namespace {
  // ���_�v�f�̔z��쐬
  const D3DVERTEXELEMENT9 vElement[] = {
    {0,0,D3DDECLTYPE_FLOAT3,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_POSITION,0},  // ���W
    {0,12,D3DDECLTYPE_FLOAT3,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_NORMAL,0},   // �@��
    {0,24,D3DDECLTYPE_FLOAT2,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD,0}, // UV
    D3DDECL_END()
  };
}

//==============================================================================
// init
//------------------------------------------------------------------------------
bool XFileObject::init(const char* file) {
  LPD3DXBUFFER pAdjBuff;
  auto renderer = App::instance().getRenderer();
  const auto pDevice = renderer->getDevice();

  if(FAILED(D3DXLoadMeshFromX(file,
    D3DXMESH_SYSTEMMEM,
    pDevice,
    &pAdjBuff,
    &_pD3DXBuffMat,
    NULL,
    &_nNumMat,
    &_pD3DXMesh))) {
    // �ǂݍ��ݎ��s
    MessageBox(NULL,
      TEXT("InitCSceneX:���f���̓ǂݍ��݂Ɏ��s���܂���"),
      TEXT("�x��"),
      MB_OK);
    return false;
  }

  // ���_
  pDevice->CreateVertexDeclaration(vElement,&_p3DDec);

  // �N���[�����b�V���̍쐬
  LPD3DXMESH pTempMesh;
  _pD3DXMesh->CloneMesh(D3DXMESH_SYSTEMMEM,&vElement[0],pDevice,&pTempMesh);
  SafeRelease(_pD3DXMesh);

  // �I�v�e�B�}�C�U
  pTempMesh->Optimize(D3DXMESH_MANAGED |
    D3DXMESHOPT_COMPACT |
    D3DXMESHOPT_ATTRSORT |
    D3DXMESHOPT_VERTEXCACHE,
    (DWORD*)pAdjBuff->GetBufferPointer(),
    nullptr,nullptr,nullptr,&_pD3DXMesh);

  // ���Ƃ����Â�
  SafeRelease(pAdjBuff);
  SafeRelease(pTempMesh);

  // ���F
  _textureID = 0;

  _vtxShaderID = renderer->getShader()->getVtxShader("vs_model.cso");

  return true;
}

//==============================================================================
// update
//------------------------------------------------------------------------------
void XFileObject::update() {

}

//==============================================================================
// draw
//------------------------------------------------------------------------------
void XFileObject::draw(const Renderer* renderer) {
  D3DXMATERIAL *pD3DXMat;
  D3DMATERIAL9 matDef;// ���X�̃}�e���A�����;
  const auto pDevice = renderer->getDevice();
  renderer->getShader()->setVtxShader(_vtxShaderID);
  const auto vtxShader = renderer->getShader()->getNowVtxShader();

  // �f�N�����[�V�����ݒ�
  pDevice->SetVertexDeclaration(_p3DDec);
  
  // ���݂̃}�e���A�����擾
  pDevice->GetMaterial(&matDef);
  pD3DXMat = (D3DXMATERIAL*)_pD3DXBuffMat->GetBufferPointer();

  Matrix norMtx;
  D3DXMatrixInverse(&norMtx,nullptr,&getWorldMtx());
  D3DXMatrixTranspose(&norMtx,&norMtx);
  vtxShader->_constTable->SetMatrix(pDevice,"gNorWorld",&norMtx);

  Matrix wvp = getWorldMtx() * renderer->getCamera()->getViewMtx() * renderer->getProjMtx();
  vtxShader->_constTable->SetMatrix(pDevice,"gWorld", &getWorldMtx());
  vtxShader->_constTable->SetMatrix(pDevice,"gWVP",&wvp);

  UINT nSamplerIndex = renderer->getShader()->getNowPixShader()->_constTable->GetSamplerIndex("TexSamp0");
  pDevice->SetTexture(nSamplerIndex,renderer->getTexture()->getTexture(_textureID));

  // ���f���̕`��
  for(int nCntMat = 0; nCntMat < (int)_nNumMat; nCntMat++) {
    vtxShader->_constTable->SetFloatArray(pDevice,"gMaterial",(float*)&pD3DXMat[nCntMat].MatD3D.Diffuse,4);
    // ���f���̃p�[�c��`��
    _pD3DXMesh->DrawSubset(nCntMat);
  }

  // �}�e���A�������ɖ߂�
  pDevice->SetMaterial(&matDef);
}

//==============================================================================
// uninit
//------------------------------------------------------------------------------
void XFileObject::uninit() {
  SafeRelease(_pD3DXMesh);
  SafeRelease(_pD3DXBuffMat);
  SafeRelease(_p3DDec);
}

//EOF