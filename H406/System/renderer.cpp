//==============================================================================
//
// Renderer[renderer.cpp]
// Author : Yasuaki Yamashita : 2015/06/29
//
//==============================================================================

//******************************************************************************
// include
//******************************************************************************
#include "app.h"
#include "renderer.h"
#include "texture.h"
#include "camera.h"
#include "shader.h"
#include "node.h"
#include "postEffect.h"

//==============================================================================
// renderer
//------------------------------------------------------------------------------
bool Renderer::init(const App* app) {
  
  // デバイス
  createDevice(app->getWindowSize(), app->getHWnd());

  // テクスチャマネージャ
  _texture = new Texture(this);

  // カメラ
  _camera = new Camera();

  // シェーダ
  _shader = new Shader(this);

  // フェード
  _fadeBG = nullptr;

  // ポストエフェクト
  _postEffect = new PostEffect();

  return true;
}

//==============================================================================
// デバイス生成
//------------------------------------------------------------------------------
void Renderer::createDevice(const SIZE& windowSize, HWND hWnd) {

  D3DDISPLAYMODE d3ddm;
  D3DPRESENT_PARAMETERS d3dpp;	// ディスプレイへの出力方法を決める情報

  // Direct3Dオブジェクトの生成
  LPDIRECT3D9 pD3D = Direct3DCreate9(D3D_SDK_VERSION);

  if(pD3D == NULL) {
    MessageBox(NULL,"Direct3Dオブジェクトの生成に失敗しました。","エラー通知",MB_OK);
    return;
  }

  // 現在のディスプレイモードを取得
  if(FAILED(pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT,&d3ddm))) {
    return;
  }

  ZeroMemory(&d3dpp,sizeof(d3dpp));	// 0でbitを初期化
  d3dpp.BackBufferWidth = windowSize.cx;	// バッファーの幅
  d3dpp.BackBufferHeight = windowSize.cy;	// バッファーの高さ
  d3dpp.BackBufferFormat = d3ddm.Format;	// カラーモードの指定
  d3dpp.BackBufferCount = 1;				// バッファーの数
  d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;	// バッファ切り替えをディスプレイドライバに委任
  d3dpp.Windowed = TRUE;				// ウィンドウ/フルスクリーンを指定
  d3dpp.EnableAutoDepthStencil = TRUE;	// 深度ステンシルバッファを使用
  d3dpp.AutoDepthStencilFormat = D3DFMT_D16;	// 16bitに指定
  d3dpp.FullScreen_RefreshRateInHz = 0;  // 他のウィンドウもいるので、レートを指定しない
  d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE; // 描画を即時に指定(リフレッシュレートとの不一致を考慮)
  d3dpp.hDeviceWindow = hWnd;

  // デバイスオブジェクトの生成
  // [デバイス作成制御]<描画>と<頂点処理>をハードウェアで行なう
  if(FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT,
    D3DDEVTYPE_HAL,
    nullptr,
    D3DCREATE_HARDWARE_VERTEXPROCESSING,
    &d3dpp,&_pD3DDevice))) {
    // 取得失敗
    MessageBox(nullptr,TEXT("デバイスオブジェクトの生成に失敗しました\r\n動作環境が古い可能性があります"),TEXT("警告"),MB_OK);
    SafeRelease(pD3D);
    return;
  }

  //---- レンダーステートの設定 ----
  _pD3DDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW);	// カリングの設定

  _pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,FALSE);			// アルファブレンドの有効化
  _pD3DDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);		// ソース
  _pD3DDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);	// デスト
  _pD3DDevice->SetRenderState(D3DRS_ALPHAFUNC,D3DCMP_GREATEREQUAL);
  _pD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE,TRUE);
  _pD3DDevice->SetRenderState(D3DRS_ALPHAREF,0x66);
  _pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE,TRUE);
  _pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
  _pD3DDevice->SetRenderState(D3DRS_SHADEMODE,D3DSHADE_GOURAUD);
  _pD3DDevice->SetRenderState(D3DRS_LIGHTING,FALSE);


  for(int i = 0; i < 4; ++i) {
    //---- サンプラーステートの設定 ----
    // テクスチャUV値の繰り返し設定(テクスチャの繰り返し描画指定)
    _pD3DDevice->SetSamplerState(i,D3DSAMP_ADDRESSU,D3DTADDRESS_WRAP);
    _pD3DDevice->SetSamplerState(i,D3DSAMP_ADDRESSV,D3DTADDRESS_WRAP);
    // テクスチャの拡縮時の補間設定
    _pD3DDevice->SetSamplerState(i,D3DSAMP_MINFILTER,D3DTEXF_ANISOTROPIC);	// 線形処理(縮小)
    _pD3DDevice->SetSamplerState(i,D3DSAMP_MAGFILTER,D3DTEXF_ANISOTROPIC);	// 線形処理(拡大)

    //---- テクスチャステージステートの設定 ----
    // 不透明テクスチャにα値を適用させる
    _pD3DDevice->SetTextureStageState(i,D3DTSS_ALPHAOP,D3DTOP_MODULATE); // 透過をブレンド
    _pD3DDevice->SetTextureStageState(i,D3DTSS_ALPHAARG1,D3DTA_TEXTURE);	// 引数１：テクスチャを指定 
    _pD3DDevice->SetTextureStageState(i,D3DTSS_ALPHAARG2,D3DTA_CURRENT);	// 引数２：現在のポリゴンの不透明度を指定
  }


  // いらない
  pD3D->Release();

  // プロジェクションマトリックス
  D3DXMatrixIdentity(&_mtxProj);
  D3DXMatrixPerspectiveFovLH(&_mtxProj,		// プロジェクションマトリックスの初期化
    D3DX_PI / 4.0f,				// 視野角
    (float)windowSize.cx / (float)windowSize.cy,	// アスペクト比
    10.0f,						// rear値
    5000.0f);					// far値

  // ビューポート設定
  D3DVIEWPORT9 vp;
  vp.X = 0;
  vp.Y = 0;
  vp.Width = (DWORD)windowSize.cx;
  vp.Height = (DWORD)windowSize.cy;
  vp.MaxZ = 1;
  vp.MinZ = 0;
  _pD3DDevice->SetViewport(&vp);

  // マルチレンダー
  _pD3DDevice->CreateTexture(windowSize.cx,windowSize.cy,1,D3DUSAGE_RENDERTARGET,D3DFMT_A16B16G16R16F,D3DPOOL_DEFAULT,&_TexNormalDepth,0);
  _TexNormalDepth->GetSurfaceLevel(0,&_SurNormalDepth);
  _pD3DDevice->CreateTexture(windowSize.cx,windowSize.cy,1,D3DUSAGE_RENDERTARGET,D3DFMT_A16B16G16R16F,D3DPOOL_DEFAULT,&_TexPos,0);
  _TexPos->GetSurfaceLevel(0,&_SurPos);
  _pD3DDevice->CreateTexture(windowSize.cx,windowSize.cy,1,D3DUSAGE_RENDERTARGET,D3DFMT_A8R8G8B8,D3DPOOL_DEFAULT,&_TexColor,0);
  _TexColor->GetSurfaceLevel(0,&_SurColor);
}

//==============================================================================
// Renderer
//------------------------------------------------------------------------------
Renderer::~Renderer() {

  SafeDelete(_texture);
  SafeDelete(_camera);
  SafeDelete(_shader);
  SafeDelete(_postEffect);

  SafeRelease(_SurNormalDepth);
  SafeRelease(_TexNormalDepth);
  SafeRelease(_SurPos);
  SafeRelease(_TexPos);
  SafeRelease(_TexColor);
  SafeRelease(_SurColor);
  SafeRelease(_pD3DDevice);

  if(_fadeBG != nullptr) {
    _fadeBG->release();
  }
}

//==============================================================================
// update
//------------------------------------------------------------------------------
void Renderer::update() {
  _camera->update();

  if(_fadeBG != nullptr) {
    _fadeBG->updateChild();
    _fadeBG->updateMtxChild();
  }

  _postEffect->update();
}

//==============================================================================
// draw
//------------------------------------------------------------------------------
bool Renderer::draw(node* baceNode) {

  // レンダーターゲット設定
  _pD3DDevice->SetRenderTarget(1,_SurNormalDepth);
  _pD3DDevice->SetRenderTarget(2,_SurPos);
  _pD3DDevice->SetRenderTarget(3,_SurColor);

  if(SUCCEEDED(_pD3DDevice->BeginScene())) {
    // シーンのクリア
    _pD3DDevice->Clear(0,NULL,(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),D3DCOLOR_XRGB(0,0,0),1.0f,0);
    // シーンの描画終了
    _pD3DDevice->EndScene();
  }

  _pD3DDevice->SetRenderTarget(1,nullptr);
  _pD3DDevice->SetRenderTarget(2,nullptr);
  _pD3DDevice->SetRenderTarget(3,nullptr);

  // シーンの描画開始
  if(SUCCEEDED(_pD3DDevice->BeginScene())) {
    
    // シーンのクリア
    _pD3DDevice->Clear(0,NULL,(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),D3DCOLOR_XRGB(0,0,0),1.0f,0);
    
    // ベースノード
    if(baceNode != nullptr) {
      {
        // ベースピクセルシェーダー
        _shader->setPixShader("ps_bace.cso");

        // 普通の3D
        baceNode->drawChild(this,NodeType::lightOff3D);
      }

      // シーンの描画終了
      _pD3DDevice->EndScene();
    }

    _pD3DDevice->SetRenderTarget(1,_SurNormalDepth);
    _pD3DDevice->SetRenderTarget(2,_SurPos);
    _pD3DDevice->SetRenderTarget(3,_SurColor);

    // シーンの描画開始
    if(SUCCEEDED(_pD3DDevice->BeginScene())) {
      // ベースピクセルシェーダー
      _shader->setPixShader("ps_bace3d.cso");

      // far値
      _shader->getNowPixShader()->_constTable->SetFloat(_pD3DDevice,"gFar",2000.0f);

      // 普通の3D
      baceNode->drawChild(this,NodeType::normal3D);

      // シーンの描画終了
      _pD3DDevice->EndScene();
    }

    _pD3DDevice->SetRenderTarget(1,nullptr);
    _pD3DDevice->SetRenderTarget(2,nullptr);
    _pD3DDevice->SetRenderTarget(3,nullptr);

    // シーンの描画開始
    if(SUCCEEDED(_pD3DDevice->BeginScene())) {

      _pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE,FALSE);
      // ポストエフェクト
      for(int i = 0; i < 4; ++i) {
        _pD3DDevice->SetSamplerState(i,D3DSAMP_ADDRESSU,D3DTADDRESS_CLAMP);
        _pD3DDevice->SetSamplerState(i,D3DSAMP_ADDRESSV,D3DTADDRESS_CLAMP);
        _pD3DDevice->SetSamplerState(i,D3DSAMP_MINFILTER,D3DTEXF_POINT);
        _pD3DDevice->SetSamplerState(i,D3DSAMP_MAGFILTER,D3DTEXF_POINT);
        _pD3DDevice->SetSamplerState(i,D3DSAMP_MIPFILTER,D3DTEXF_POINT);
      }
      _postEffect->draw(this);

      for(int i = 0; i < 4; ++i) {
        _pD3DDevice->SetTexture(i,nullptr);
      }

      _pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE,TRUE);

      // ベースピクセルシェーダー
      _shader->setPixShader("ps_bace.cso");

      // エフェクト
      baceNode->drawChild(this,NodeType::effect);

      //---- サンプラーステートの設定 ----
      for(int i = 0; i < 4; ++i) {
        _pD3DDevice->SetSamplerState(i,D3DSAMP_ADDRESSU,D3DTADDRESS_WRAP);
        _pD3DDevice->SetSamplerState(i,D3DSAMP_ADDRESSV,D3DTADDRESS_WRAP);
        _pD3DDevice->SetSamplerState(i,D3DSAMP_MINFILTER,D3DTEXF_ANISOTROPIC);
        _pD3DDevice->SetSamplerState(i,D3DSAMP_MAGFILTER,D3DTEXF_ANISOTROPIC);
        _pD3DDevice->SetSamplerState(i,D3DSAMP_MIPFILTER,D3DTEXF_NONE);
      }

      // 普通の2D
      baceNode->drawChild(this,NodeType::normal2D);
    }

    // フェード用
    if(_fadeBG != nullptr) {
      _fadeBG->draw(this);
    }

    // シーンの描画終了
    _pD3DDevice->EndScene();
    // シーンの表示
    _pD3DDevice->Present(NULL,NULL,NULL,NULL);
  }

  return true;
}

//EOF