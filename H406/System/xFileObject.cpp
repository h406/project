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
  // 頂点要素の配列作成
  const D3DVERTEXELEMENT9 vElement[] = {
    {0,0,D3DDECLTYPE_FLOAT3,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_POSITION,0},  // 座標
    {0,12,D3DDECLTYPE_FLOAT3,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_NORMAL,0},   // 法線
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
    // 読み込み失敗
    MessageBox(NULL,
      TEXT("InitCSceneX:モデルの読み込みに失敗しました"),
      TEXT("警告"),
      MB_OK);
    return false;
  }

  // 頂点
  pDevice->CreateVertexDeclaration(vElement,&_p3DDec);

  // クローンメッシュの作成
  LPD3DXMESH pTempMesh;
  _pD3DXMesh->CloneMesh(D3DXMESH_SYSTEMMEM,&vElement[0],pDevice,&pTempMesh);
  SafeRelease(_pD3DXMesh);

  // オプティマイザ
  pTempMesh->Optimize(D3DXMESH_MANAGED |
    D3DXMESHOPT_COMPACT |
    D3DXMESHOPT_ATTRSORT |
    D3DXMESHOPT_VERTEXCACHE,
    (DWORD*)pAdjBuff->GetBufferPointer(),
    nullptr,nullptr,nullptr,&_pD3DXMesh);

  // あとかたづけ
  SafeRelease(pAdjBuff);
  SafeRelease(pTempMesh);

  // 白色
  _textureID = 0;

  _vtxShaderID = renderer->getShader()->getVtxShader("vs_model.cso");


  auto pD3DXMat = (D3DXMATERIAL*)_pD3DXBuffMat->GetBufferPointer();
  for(int nCntMat = 0; nCntMat < (int)_nNumMat; nCntMat++) {
    if(pD3DXMat[nCntMat].pTextureFilename != nullptr) {
      string texPath = string("./data/texture/") + pD3DXMat[nCntMat].pTextureFilename;
      renderer->getTexture()->createTexture(texPath.c_str());
    }
  }

  return true;
}

static int drawCount = 0;
static bool draw1st = true;

//==============================================================================
// update
//------------------------------------------------------------------------------
void XFileObject::update() {
  if (draw1st){
    char str[256] = {NULL};
    sprintf_s(str, "%d", drawCount);
    App::instance().setTitle(str);
  }

  drawCount = 0;
  draw1st = false;
}

//==============================================================================
// draw
//------------------------------------------------------------------------------
void XFileObject::draw(const Renderer* renderer) {
  D3DXMATERIAL *pD3DXMat;
  D3DMATERIAL9 matDef;// 元々のマテリアル情報;
  const auto pDevice = renderer->getDevice();
  renderer->getShader()->setVtxShader(_vtxShaderID);
  const auto vtxShader = renderer->getShader()->getNowVtxShader();

  // デクラレーション設定
  pDevice->SetVertexDeclaration(_p3DDec);
  
  // 現在のマテリアルを取得
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
  if(UINT_MAX != nSamplerIndex) {
    pDevice->SetTexture(nSamplerIndex,renderer->getTexture()->getTexture(_textureID));
  }

  // モデルの描画
  for(int nCntMat = 0; nCntMat < (int)_nNumMat; nCntMat++) {
    if(pD3DXMat[nCntMat].pTextureFilename != nullptr && UINT_MAX != nSamplerIndex) {
      string texPath = string("./data/texture/") + pD3DXMat[nCntMat].pTextureFilename;
      pDevice->SetTexture(nSamplerIndex,renderer->getTexture()->getTexture(texPath.c_str()));
    }
    vtxShader->_constTable->SetFloatArray(pDevice,"gMaterial",(float*)&pD3DXMat[nCntMat].MatD3D.Diffuse,4);
    // モデルのパーツを描画
    _pD3DXMesh->DrawSubset(nCntMat);
    drawCount++;
//    draw1st = true;
  }

  // マテリアルを元に戻す
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