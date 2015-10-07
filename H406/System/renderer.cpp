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
#include "effect.h"

//==============================================================================
// renderer
//------------------------------------------------------------------------------
Renderer::Renderer(const App* app) {
  
  // デバイス
  createDevice(app->getWindowSize(), app->getHWnd());

  // テクスチャマネージャ
  _texture = new Texture(this);

  // カメラ
  _camera = new Camera();

  // シェーダ
  _shader = new Shader(this);

  // effect
  _effect = new Effect(this);
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

  // 現在のディ3スプレイモードを取得
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

  _pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);			// アルファブレンドの有効化
  _pD3DDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);		// ソース
  _pD3DDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);	// デスト
  _pD3DDevice->SetRenderState(D3DRS_ALPHAFUNC,D3DCMP_GREATEREQUAL);
  _pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

  //---- サンプラーステートの設定 ----
  // テクスチャUV値の繰り返し設定(テクスチャの繰り返し描画指定)
  _pD3DDevice->SetSamplerState(0,D3DSAMP_ADDRESSU,D3DTADDRESS_WRAP);
  _pD3DDevice->SetSamplerState(0,D3DSAMP_ADDRESSV,D3DTADDRESS_WRAP);
  // テクスチャの拡縮時の補間設定
  _pD3DDevice->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);	// 線形処理(縮小)
  _pD3DDevice->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);	// 線形処理(拡大)

  //---- テクスチャステージステートの設定 ----
  // 不透明テクスチャにα値を適用させる
  _pD3DDevice->SetTextureStageState(0,D3DTSS_ALPHAOP,D3DTOP_MODULATE); // 透過をブレンド
  _pD3DDevice->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_TEXTURE);	// 引数１：テクスチャを指定 
  _pD3DDevice->SetTextureStageState(0,D3DTSS_ALPHAARG2,D3DTA_CURRENT);	// 引数２：現在のポリゴンの不透明度を指定

  _pD3DDevice->SetRenderState(D3DRS_LIGHTING,FALSE);

  // いらない
  pD3D->Release();

  // プロジェクションマトリックス
  D3DXMatrixIdentity(&_mtxProj);
  D3DXMatrixPerspectiveFovLH(&_mtxProj,		// プロジェクションマトリックスの初期化
    D3DX_PI / 4.0f,				// 視野角
    (float)windowSize.cx / (float)windowSize.cy,	// アスペクト比
    0.0f,						// rear値
    1000.0f);					// far値

  // ビューポート設定
  D3DVIEWPORT9 vp;
  vp.X = 0;
  vp.Y = 0;
  vp.Width = (DWORD)windowSize.cx;
  vp.Height = (DWORD)windowSize.cy;
  vp.MaxZ = 1;
  vp.MinZ = 0;
  _pD3DDevice->SetViewport(&vp);
}

//==============================================================================
// Renderer
//------------------------------------------------------------------------------
Renderer::~Renderer() {
  SafeRelease(_pD3DDevice);
  SafeDelete(_texture);
  SafeDelete(_camera);
  SafeDelete(_shader);
  SafeDelete(_effect);
}

//==============================================================================
// update
//------------------------------------------------------------------------------
void Renderer::update() {
  _camera->update();

  _effect->update();
}

//==============================================================================
// draw
//------------------------------------------------------------------------------
bool Renderer::draw(node* baceNode) {
    // シーンの描画開始
  if(SUCCEEDED(_pD3DDevice->BeginScene())) {
    // シーンのクリア
    _pD3DDevice->Clear(0,NULL,(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),D3DCOLOR_XRGB(100,100,100),1.0f,0);

    // ベースピクセルシェーダー
    _shader->setPixShader("ps_bace.cso");
    
    // ベースノード
    if(baceNode != nullptr) {
      baceNode->drawChild(this);
    }

    // エフェクト表示
    _effect->draw();

    // シーンの描画終了
    _pD3DDevice->EndScene();
    // シーンの表示
    _pD3DDevice->Present(NULL,NULL,NULL,NULL);
  }

  return true;
}

//EOF