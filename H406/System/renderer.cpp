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

//hack
LPDIRECT3DTEXTURE9       rayMap;

static void WINAPI makeRayMap(D3DXVECTOR4* pOut,const D3DXVECTOR2*,const D3DXVECTOR2*,void* ) {
  /*
  float r = 1.0f * (float)rand() / (float)RAND_MAX;
  float t = 6.2831853f * (float)rand() / ((float)RAND_MAX + 1.0f);
  float cp = 2.0f * (float)rand() / (float)RAND_MAX - 1.0f;
  float sp = sqrt(1.0f - cp * cp);
  float ct = cos(t), st = sin(t);

  pOut->x = abs(r * sp * ct);
  pOut->y = abs(r * sp * st);
  pOut->z = abs(r * cp);
  pOut->w = 0;
  */
  static int i = 0;

  D3DXVECTOR3 vec;
  switch(i % 16) {
  case 0: vec = D3DXVECTOR3(0.53812504f,0.18565957f,-0.43192f); break;
  case 1: vec = D3DXVECTOR3(0.13790712f,0.24864247f,0.44301823f); break;
  case 2: vec = D3DXVECTOR3(0.33715037f,0.56794053f,-0.005789503f); break;
  case 3: vec = D3DXVECTOR3(-0.6999805f,-0.04511441f,-0.0019965635f); break;
  case 4: vec = D3DXVECTOR3(0.06896307f,-0.15983082f,-0.85477847f); break;
  case 5: vec = D3DXVECTOR3(0.056099437f,0.006954967f,-0.1843352f); break;
  case 6: vec = D3DXVECTOR3(-0.014653638f,0.14027752f,0.0762037f); break;
  case 7: vec = D3DXVECTOR3(0.010019933f,-0.1924225f,-0.034443386f); break;
  case 8: vec = D3DXVECTOR3(-0.35775623f,-0.5301969f,-0.43581226f); break;
  case 9: vec = D3DXVECTOR3(-0.3169221f,0.106360726f,0.015860917f); break;
  case 10:vec = D3DXVECTOR3(0.010350345f,-0.58698344f,0.0046293875f); break;
  case 11:vec = D3DXVECTOR3(-0.08972908f,-0.49408212f,0.3287904f); break;
  case 12:vec = D3DXVECTOR3(0.7119986f,-0.0154690035f,-0.09183723f); break;
  case 13:vec = D3DXVECTOR3(-0.053382345f,0.059675813f,-0.5411899f); break;
  case 14:vec = D3DXVECTOR3(0.035267662f,-0.063188605f,0.54602677f); break;
  case 15:vec = D3DXVECTOR3(-0.47761092f,0.2847911f,-0.0271716f); break;
  }
  // テクスチャなので0-1の値しか持てないので、-1,1を0,1へ変換
  pOut->x = (vec.x + 1.f)*0.5f;
  pOut->y = (vec.y + 1.f)*0.5f;
  pOut->z = (vec.z + 1.f)*0.5f;
  pOut->w = 0.f;
  ++i;
}

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

  _pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);			// アルファブレンドの有効化
  _pD3DDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);		// ソース
  _pD3DDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);	// デスト
  _pD3DDevice->SetRenderState(D3DRS_ALPHAFUNC,D3DCMP_GREATEREQUAL);
  _pD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE,TRUE);
  _pD3DDevice->SetRenderState(D3DRS_ALPHAREF,0x66);
  _pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE,TRUE);
  _pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
  _pD3DDevice->SetRenderState(D3DRS_SHADEMODE,D3DSHADE_GOURAUD);
  _pD3DDevice->SetRenderState(D3DRS_LIGHTING,FALSE);
  //_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,FALSE);


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

  _pD3DDevice->SetRenderState(D3DRS_LIGHTING,FALSE);

  // いらない
  pD3D->Release();

  // プロジェクションマトリックス
  D3DXMatrixIdentity(&_mtxProj);
  D3DXMatrixPerspectiveFovLH(&_mtxProj,		// プロジェクションマトリックスの初期化
    D3DX_PI / 4.0f,				// 視野角
    (float)windowSize.cx / (float)windowSize.cy,	// アスペクト比
    10.0f,						// rear値
    2500.0f);					// far値

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
  _pD3DDevice->CreateTexture(windowSize.cx,windowSize.cy,1,D3DUSAGE_RENDERTARGET,D3DFMT_A32B32G32R32F,D3DPOOL_DEFAULT,&_TexNormal,0);
  _TexNormal->GetSurfaceLevel(0,&_SurNormal);
  _pD3DDevice->CreateTexture(windowSize.cx,windowSize.cy,1,D3DUSAGE_RENDERTARGET,D3DFMT_A32B32G32R32F,D3DPOOL_DEFAULT,&_TexDepth,0);
  _TexDepth->GetSurfaceLevel(0,&_SurDepth);
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

  SafeRelease(_SurNormal);
  SafeRelease(_TexNormal);
  SafeRelease(_SurDepth);
  SafeRelease(_TexDepth);
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

    // シーンの描画開始
  if(SUCCEEDED(_pD3DDevice->BeginScene())) {

    // レンダーターゲット設定
    _pD3DDevice->SetRenderTarget(1,_SurNormal);
    _pD3DDevice->SetRenderTarget(2,_SurDepth);
    _pD3DDevice->SetRenderTarget(3,_SurColor);

    // シーンのクリア
    _pD3DDevice->Clear(0,NULL,(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),D3DCOLOR_XRGB(100,100,100),1.0f,0);
    
    // ベースノード
    if(baceNode != nullptr) {
      {
        // ベースピクセルシェーダー
        _shader->setPixShader("ps_bace.cso");
        _pD3DDevice->SetRenderTarget(1,nullptr);
        _pD3DDevice->SetRenderTarget(2,nullptr);
        _pD3DDevice->SetRenderTarget(3,nullptr);

        // 普通の3D
        baceNode->drawChild(this,NodeType::lightOff3D);
      }

      _pD3DDevice->SetRenderTarget(1,_SurNormal);
      _pD3DDevice->SetRenderTarget(2,_SurDepth);
      _pD3DDevice->SetRenderTarget(3,_SurColor);

      // ベースピクセルシェーダー
      _shader->setPixShader("ps_bace3d.cso");

      // far値
      _shader->getNowPixShader()->_constTable->SetFloat(_pD3DDevice,"gFar",2000.0f);

      Vec4 color[3];
      float power[3];
      Vec3 dir[3];

      color[0] = Vec4(1.f,1.f,1.f,1);
      power[0] = 1.0f;
      dir[0] = Vec3(0.5f,-1,2);

      color[1] = Vec4(1.f,1.f,1.f,1);
      power[1] = 0.3f;
      dir[1] = Vec3(-0.5f,-1,-2);

      color[2] = Vec4(1.f,1.f,1.f,1);
      power[2] = 0.6f;
      dir[2] = Vec3(0.5f,-3.f,0);

      for(int i = 0; i < 3; i++) {
        D3DXVec3Normalize(&dir[i],&dir[i]);
      }

      _shader->getNowPixShader()->_constTable->SetFloatArray(_pD3DDevice,"gLightDir",(float*)dir,sizeof(float) * 3 * 3);
      _shader->getNowPixShader()->_constTable->SetFloatArray(_pD3DDevice,"gLightPower",power,sizeof(float) * 3);
      _shader->getNowPixShader()->_constTable->SetFloatArray(_pD3DDevice,"gLightCol",(float*)color,sizeof(float) * 4 * 3);

      // 普通の3D
      baceNode->drawChild(this,NodeType::normal3D);

      // ポストエフェクト
      for(int i = 0; i < 4; ++i) {
        _pD3DDevice->SetSamplerState(i,D3DSAMP_ADDRESSU,D3DTADDRESS_CLAMP);
        _pD3DDevice->SetSamplerState(i,D3DSAMP_ADDRESSV,D3DTADDRESS_CLAMP);
        _pD3DDevice->SetSamplerState(i,D3DSAMP_MINFILTER,D3DTEXF_POINT);
        _pD3DDevice->SetSamplerState(i,D3DSAMP_MAGFILTER,D3DTEXF_POINT);
      }
      _pD3DDevice->SetRenderTarget(1,nullptr);
      _pD3DDevice->SetRenderTarget(2,nullptr);
      _pD3DDevice->SetRenderTarget(3,nullptr);
      _postEffect->draw(this);
      
      for(int i = 0; i < 4; ++i) {
        _pD3DDevice->SetTexture(i,nullptr);
      }

      // hack ssao

      /*
      {
        const SIZE size = App::instance().getWindowSize();
        const float w = (float)size.cx;
        const float h = (float)size.cy;

        _shader->setPixShader("ps_ssao.cso");
      
        _pD3DDevice->SetRenderTarget(1,nullptr);
        _pD3DDevice->SetTexture(0,rayMap);
        _pD3DDevice->SetTexture(1,_TexNormalDepth);


        for(int i = 0; i < 4; ++i) {
          //---- サンプラーステートの設定 ----
          _pD3DDevice->SetSamplerState(i,D3DSAMP_ADDRESSU,D3DTADDRESS_CLAMP);
          _pD3DDevice->SetSamplerState(i,D3DSAMP_ADDRESSV,D3DTADDRESS_CLAMP);
          _pD3DDevice->SetSamplerState(i,D3DSAMP_MINFILTER,D3DTEXF_POINT);
          _pD3DDevice->SetSamplerState(i,D3DSAMP_MAGFILTER,D3DTEXF_POINT);
        }
        _pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
        _pD3DDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_ZERO);
        _pD3DDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_SRCALPHA);


        struct T4VERTEX {
          float p[4];
          float t[2];
        }  Vertex[4] = {
          {0.0f,0,0.1f,1.0f,0.0f,0.0f},
          {w,0,0.1f,1.0f,1.0f,0.0f},
          {w,h,0.1f,1.0f,1.0f,1.0f},
          {0.0f,h,0.1f,1.0f,0.0f,1.0f},
        };

        _pD3DDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_TEX4);
        _pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN,2,Vertex,sizeof(T4VERTEX));
        _pD3DDevice->SetFVF(NULL);
        _pD3DDevice->SetTexture(0,nullptr);
        _pD3DDevice->SetTexture(1,NULL);
        _pD3DDevice->SetRenderState(D3DRS_ZENABLE,TRUE);
        _pD3DDevice->SetRenderState(D3DRS_LIGHTING,TRUE);
        _pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,FALSE);
      }
      //*/

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