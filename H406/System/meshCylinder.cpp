//==============================================================================
//
// meshCylinder [meshCylinder.cpp]
// Author : masato masuda : 2015/10/14
//
//==============================================================================

//******************************************************************************
// include
//******************************************************************************
#include "meshCylinder.h"
#include "app.h"
#include "renderer.h"
#include "shader.h"
#include "camera.h"

namespace {
  struct VERTEX_3D {
    D3DXVECTOR3   vtx; // ���_���W
    D3DXVECTOR3   nor; // �@�����W
    D3DXVECTOR2   tex; // �e�N�X�`�����W
  };

  // ���_�v�f�̔z��쐬
  const D3DVERTEXELEMENT9 vElement[] = {
    {0,0,D3DDECLTYPE_FLOAT3,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_POSITION,0},  // ���W
    {0,12,D3DDECLTYPE_FLOAT3,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_NORMAL,0},   // �@��
    {0,24,D3DDECLTYPE_FLOAT2,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD,0}, // UV
    D3DDECL_END()
  };

  int XZtoIDX(int x,int z, int _nNumBlockX) { return ((z)* (_nNumBlockX + 1) + (x)); }
}

//==============================================================================
// init
//------------------------------------------------------------------------------
bool MeshCylinder::init(int nNumBlockX,int nNumBlockZ,float fSizeBlockX,float fSizeBlockZ) {
  _nNumBlockX = nNumBlockX;
  _nNumBlockY = nNumBlockZ;

  _fSizeBlockX = fSizeBlockX;
  _fSizeBlockY = fSizeBlockZ;

  _nNumVertex = (_nNumBlockX + 1) * (_nNumBlockY + 1);
  _nNumVertexIndex = ((_nNumBlockX * 2 + 2) * _nNumBlockY) + (_nNumBlockY * 2 - 2);
  _nNumPolygon = _nNumVertexIndex - 2;

  _pVtxList = new D3DXVECTOR3[_nNumVertex];
  _pNorList = new D3DXVECTOR3[(_nNumBlockY * _nNumBlockX) * 2];

  memset(_pVtxList,0,sizeof(D3DXVECTOR3) * _nNumVertex);
  memset(_pNorList,0,sizeof(D3DXVECTOR3) * (_nNumBlockY * _nNumBlockX) * 2);

  // �f�o�C�X
  LPDIRECT3DDEVICE9 pDevice = App::instance().getRenderer()->getDevice();

  // ���_
  pDevice->CreateVertexDeclaration(vElement,&_p3DDec);

  // �V�F�[�_
  _vtxShaderID = App::instance().getRenderer()->getShader()->getVtxShader("vs_model.cso");

  // ���_�o�b�t�@�擾
  if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * _nNumVertex,
    D3DUSAGE_WRITEONLY,
    NULL,
    D3DPOOL_MANAGED,
    &_vtxBuff,
    NULL
    ))) {
    return false;
  }

  // �C���f�b�N�X�o�b�t�@�擾
  if(FAILED(pDevice->CreateIndexBuffer(sizeof(WORD) * _nNumVertexIndex,
    D3DUSAGE_WRITEONLY,
    D3DFMT_INDEX16,
    D3DPOOL_MANAGED,
    &_indexBuff,
    NULL))) {
    return false;
  }

  // ���_�ݒ�
  initVtx();

  _textureID = 0;
  _color = D3DXCOLOR(1,1,1,1);

  return true;
}

//==============================================================================
// init
//------------------------------------------------------------------------------
void MeshCylinder::initVtx() {

  VERTEX_3D *pVtx = nullptr;
  WORD *pIndex = nullptr;
  int vtxWork = 0;

  // ���a�̌v�Z
  float	cylinderVtxX = 0 - ((_nNumBlockX * _fSizeBlockX)) * 0.5f;
  float	cylinderVtxY = 0 - ((_nNumBlockY * _fSizeBlockY)) * 0.5f;

  // ���_�o�b�t�@�ݒ�
  _vtxBuff->Lock(0,0,(void **)&pVtx,0);

  for (float cntZ = 0.0f; cntZ <= _nNumBlockY; cntZ++)
  {
    for (float cntX = 0.0f; cntX <= _nNumBlockX; cntX++)
    {
      // �p�x�̎Z�o
      float	angle = (float)(((D3DX_PI * 2) / _nNumBlockX) * cntX);
      float	length = (_nNumBlockX * _fSizeBlockX) * 0.5f;

      // ���W
      pVtx[vtxWork].vtx = D3DXVECTOR3((-cos(angle) * length),
        (-(cntZ * _fSizeBlockY)) - (cylinderVtxY * 2),
        (sin(angle) * length));

      // �@��
      pVtx[vtxWork].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

      // �e�N�X�`���[
      //pVtx[vtxWork].tex = D3DXVECTOR2((float)(cntX / _nNumBlockX), (float)(cntZ / _nNumBlockX));
      pVtx[vtxWork].tex = D3DXVECTOR2(cntX, cntZ);

      // ���[�N�X�V
      vtxWork++;
    }

    // ���W�n�_X��߂�
    cylinderVtxX = 0 - (_nNumBlockX  * _fSizeBlockX) * 0.5f;
  }

  _vtxBuff->Unlock();

  // �C���f�b�N�X�o�b�t�@�ݒ�
  _indexBuff->Lock(0,0,(void **)&pIndex,0);

  int idxCnt = 0;
  for (int indexCnt = 0; indexCnt < _nNumVertexIndex; indexCnt++){
    // �d�����Ă���ꍇ
    if (indexCnt != 0 && (((indexCnt - (2 * _nNumBlockX + 2)) % ((2 * _nNumBlockX + 2) + 2) == 0)
      || (indexCnt - ((2 * _nNumBlockX + 2) + 2)) % ((2 * _nNumBlockX + 2) + 2) == 0)){
      // �O��Ɠ����C���f�b�N�X�ԍ�������
      pIndex[indexCnt] = pIndex[indexCnt - 1];
      idxCnt--;
    }else{
      // �d�����Ă��Ȃ��ꍇ
      pIndex[indexCnt] = (WORD)((_nNumBlockX + 1) + (idxCnt / 2)) - (WORD)((idxCnt % 2) * (_nNumBlockX + 1));
    }

    // ���[�N�p
    idxCnt++;
  }

  _indexBuff->Unlock();
}

//==============================================================================
// uninit
//------------------------------------------------------------------------------
void MeshCylinder::uninit() {
  SafeRelease(_vtxBuff);
  SafeRelease(_indexBuff);
  SafeRelease(_p3DDec);
  SafeDelete(_pVtxList);
  SafeDelete(_pNorList);
}

//==============================================================================
// update
//------------------------------------------------------------------------------
void MeshCylinder::update() {

}

//==============================================================================
// draw
//------------------------------------------------------------------------------
void MeshCylinder::draw(const Renderer* renderer) {

  auto pDevice = renderer->getDevice();
  auto shader = renderer->getShader();

  shader->setVtxShader(_vtxShaderID);
  auto vtxShader = shader->getNowVtxShader();

  UINT nSamplerIndex = shader->getNowPixShader()->_constTable->GetSamplerIndex("TexSamp0");
  pDevice->SetTexture(nSamplerIndex,renderer->getTexture()->getTexture(_textureID));

  // �F
  vtxShader->_constTable->SetFloatArray(pDevice,"gMaterial",_color,4);

  Matrix wvp = getWorldMtx() * renderer->getCamera()->getViewMtx() * renderer->getProjMtx();
  vtxShader->_constTable->SetMatrix(pDevice,"gWorld",&getWorldMtx());
  vtxShader->_constTable->SetMatrix(pDevice,"gWVP",&wvp);

  // �f�N�����[�V�����ݒ�
  pDevice->SetVertexDeclaration(_p3DDec);

  // ���_���M
  pDevice->SetStreamSource(0,_vtxBuff,0,sizeof(VERTEX_3D));
  pDevice->SetIndices(_indexBuff);

  //�l�X�ȃI�u�W�F�N�g�̕`�揈�����s��
  pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,0,0,_nNumVertex,0,_nNumPolygon);
}

//EOF