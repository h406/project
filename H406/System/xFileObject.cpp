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
    // 読み込み失敗
    MessageBox(NULL,
      TEXT("InitCSceneX:モデルの読み込みに失敗しました"),
      TEXT("警告"),
      MB_OK);
    return false;
  }

  // 頂点要素の配列作成
  const D3DVERTEXELEMENT9 vElement[] = {
    {0,0,D3DDECLTYPE_FLOAT3,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_POSITION,0},	// 座標
    {0,28,D3DDECLTYPE_FLOAT2,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD,0},	// UV
    {0,12,D3DDECLTYPE_FLOAT3,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_NORMAL,0},	// 法線
    D3DDECL_END()
  };

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

  _scl = Vec3(1.f,1.f,1.f);
  _rot = Vec3(0.f,0.f,0.f);
  _pos = Vec3(0.f,0.f,0.f);

  // TODO シェーダなんとかする
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
  D3DMATERIAL9 matDef;// 元々のマテリアル情報;
  D3DXMATRIX mtxTmp;
  const auto pDevice = renderer->getDevice();
  const auto vtxShader = renderer->getShader()->getNowVtxShader();
  const auto pixcelShader = renderer->getShader()->getNowPixShader();

  // TODO updateでやる
  D3DXMatrixIdentity(&_mtxWorld);// ワールドマトリックスの初期化
  D3DXMatrixScaling(&mtxTmp,_scl.x,_scl.y,_scl.z);
  D3DXMatrixMultiply(&_mtxWorld,&_mtxWorld,&mtxTmp);
  D3DXMatrixRotationYawPitchRoll(&mtxTmp,_rot.y,_rot.x,_rot.z);		// 回転
  D3DXMatrixMultiply(&_mtxWorld,&_mtxWorld,&mtxTmp);
  D3DXMatrixTranslation(&mtxTmp,_pos.x,_pos.y,_pos.z);			// 位置
  D3DXMatrixMultiply(&_mtxWorld,&_mtxWorld,&mtxTmp);

  pDevice->GetMaterial(&matDef);						// 現在のマテリアルを取得
  pD3DXMat = (D3DXMATERIAL*)_pD3DXBuffMat->GetBufferPointer();

  const float material[4] = {1,1,1,1};
  pixcelShader->_constTable->SetFloatArray(pDevice,"gMaterial",material,sizeof(material) / sizeof(float));
  vtxShader->_constTable->SetFloatArray(pDevice,"gMaterial",material,sizeof(material) / sizeof(float));

  Matrix wvp = _mtxWorld * renderer->getViewMtx() * renderer->getProjMtx();

  vtxShader->_constTable->SetMatrix(pDevice,"gWorld",&_mtxWorld);
  vtxShader->_constTable->SetMatrix(pDevice,"gWVP",&wvp);

  // モデルの描画
  for(int nCntMat = 0; nCntMat < (int)_nNumMat; nCntMat++) {
    pixcelShader->_constTable->SetFloatArray(pDevice,"gMaterial",(float*)&pD3DXMat[nCntMat].MatD3D.Diffuse,4);
    _pD3DXMesh->DrawSubset(nCntMat);// モデルのパーツを描画
  }

  pDevice->SetMaterial(&matDef);		// マテリアルを元に戻す
}

//==============================================================================
// uninit
//------------------------------------------------------------------------------
void XFileObject::uninit() {
  SafeRelease(_pD3DXMesh);
  SafeRelease(_pD3DXBuffMat);

}

//EOF