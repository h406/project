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

  // ���_�v�f�̔z��쐬
  const D3DVERTEXELEMENT9 vElement[] = {
    {0,0,D3DDECLTYPE_FLOAT3,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_POSITION,0},	// ���W
    {0,28,D3DDECLTYPE_FLOAT2,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD,0},	// UV
    {0,12,D3DDECLTYPE_FLOAT3,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_NORMAL,0},	// �@��
    D3DDECL_END()
  };

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

  _scl = Vec3(1.f,1.f,1.f);
  _rot = Vec3(0.f,0.f,0.f);
  _pos = Vec3(0.f,0.f,0.f);

  // TODO �V�F�[�_�Ȃ�Ƃ�����
  _vtxShader = renderer->getShader()->getVtxShader("");

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
void XFileObject::draw(Renderer* renderer) {
  D3DXMATERIAL *pD3DXMat;
  D3DMATERIAL9 matDef;// ���X�̃}�e���A�����;
  D3DXMATRIX mtxTmp;
  const auto pDevice = renderer->getDevice();
  const auto vtxShader = renderer->getShader()->getNowVtxShader();
  const auto pixcelShader = renderer->getShader()->getNowPixShader();

  // TODO update�ł��
  D3DXMatrixIdentity(&_mtxWorld);// ���[���h�}�g���b�N�X�̏�����
  D3DXMatrixScaling(&mtxTmp,_scl.x,_scl.y,_scl.z);
  D3DXMatrixMultiply(&_mtxWorld,&_mtxWorld,&mtxTmp);
  D3DXMatrixRotationYawPitchRoll(&mtxTmp,_rot.y,_rot.x,_rot.z);		// ��]
  D3DXMatrixMultiply(&_mtxWorld,&_mtxWorld,&mtxTmp);
  D3DXMatrixTranslation(&mtxTmp,_pos.x,_pos.y,_pos.z);			// �ʒu
  D3DXMatrixMultiply(&_mtxWorld,&_mtxWorld,&mtxTmp);

  pDevice->GetMaterial(&matDef);						// ���݂̃}�e���A�����擾
  pD3DXMat = (D3DXMATERIAL*)_pD3DXBuffMat->GetBufferPointer();

  const float material[4] = {1,1,1,1};
  pixcelShader->_constTable->SetFloatArray(pDevice,"gMaterial",material,sizeof(material) / sizeof(float));
  vtxShader->_constTable->SetFloatArray(pDevice,"gMaterial",material,sizeof(material) / sizeof(float));

  Matrix wvp = _mtxWorld * renderer->getViewMtx() * renderer->getProjMtx();

  vtxShader->_constTable->SetMatrix(pDevice,"gWorld",&_mtxWorld);
  vtxShader->_constTable->SetMatrix(pDevice,"gWVP",&wvp);

  // ���f���̕`��
  for(int nCntMat = 0; nCntMat < (int)_nNumMat; nCntMat++) {
    pixcelShader->_constTable->SetFloatArray(pDevice,"gMaterial",(float*)&pD3DXMat[nCntMat].MatD3D.Diffuse,4);
    _pD3DXMesh->DrawSubset(nCntMat);// ���f���̃p�[�c��`��
  }

  pDevice->SetMaterial(&matDef);		// �}�e���A�������ɖ߂�
}

//==============================================================================
// uninit
//------------------------------------------------------------------------------
void XFileObject::uninit() {
  SafeRelease(_pD3DXMesh);
  SafeRelease(_pD3DXBuffMat);

}

//EOF