//==============================================================================
//
// effect[effect.cpp]
// Author : Yasuaki Yamashita : 2015/10/07
//
//==============================================================================

//******************************************************************************
// include
//******************************************************************************
#include "effect.h"
#include "renderer.h"
#include "app.h"
#include "camera.h"

// effect
#include "Effekseer.h"
#include "EffekseerRendererDX9.h"
#include "EffekseerSoundXAudio2.h"

#ifdef _DEBUG 
#pragma comment(lib, "Effekseer.Debug.lib")
#pragma comment(lib, "EffekseerRendererDX9.Debug.lib")
#pragma comment(lib, "EffekseerSoundXAudio2.Debug.lib")
#else
#pragma comment(lib, "Effekseer.Release.lib")
#pragma comment(lib, "EffekseerRendererDX9.Release.lib")
#pragma comment(lib, "EffekseerSoundXAudio2.Release.lib")
#endif

namespace {

  const string kBaceEffectFolder("./data/effect/");
}

//==============================================================================
// effect
//------------------------------------------------------------------------------
Effect::Effect(const Renderer* renderer) {

  // 描画用インスタンスの生成
  _effekseerRenderer = ::EffekseerRendererDX9::Renderer::Create(renderer->getDevice(),2000);

  // エフェクト管理用インスタンスの生成
  _effekseerManager = ::Effekseer::Manager::Create(2000);

  // 描画用インスタンスから描画機能を設定
  _effekseerManager->SetSpriteRenderer(_effekseerRenderer->CreateSpriteRenderer());
  _effekseerManager->SetRibbonRenderer(_effekseerRenderer->CreateRibbonRenderer());
  _effekseerManager->SetRingRenderer(_effekseerRenderer->CreateRingRenderer());
  _effekseerManager->SetModelRenderer(_effekseerRenderer->CreateModelRenderer());

  // 描画用インスタンスからテクスチャの読込機能を設定
  // 独自拡張可能、現在はファイルから読み込んでいる。
  _effekseerManager->SetTextureLoader(_effekseerRenderer->CreateTextureLoader());
  _effekseerManager->SetModelLoader(_effekseerRenderer->CreateModelLoader());

  // 音再生用インスタンスの生成
  _effekseerSound = ::EffekseerSound::Sound::Create(App::instance().getSound()->getDevice(),16,16);

  // 音再生用インスタンスから再生機能を指定
  _effekseerManager->SetSoundPlayer(_effekseerSound->CreateSoundPlayer());

  // 音再生用インスタンスからサウンドデータの読込機能を設定
  // 独自拡張可能、現在はファイルから読み込んでいる。
  _effekseerManager->SetSoundLoader(_effekseerSound->CreateSoundLoader());

  // 座標系
  _effekseerManager->SetCoordinateSystem(Effekseer::CoordinateSystem::LH);

  // エフェクトの読込
  namespace sys = std::tr2::sys;
  sys::path p(kBaceEffectFolder);
  std::for_each(sys::directory_iterator(p),sys::directory_iterator(),
    [this](const sys::path& p) {
    if(sys::is_regular_file(p)) { // ファイルなら...
      const string& filename = p.filename();
      const string filepath = kBaceEffectFolder + filename;
      WCHAR wFilePath[255];
      WCHAR wPath[255];

      MultiByteToWideChar(CP_ACP,0,filepath.c_str(),-1,wFilePath,(int)(sizeof(wFilePath) / sizeof(wchar_t)) - 1);
      MultiByteToWideChar(CP_ACP,0,kBaceEffectFolder.c_str(),-1,wPath,(int)(sizeof(wPath) / sizeof(wchar_t)) - 1);

      Effekseer::Effect* efk_effect = Effekseer::Effect::Create(_effekseerManager,(EFK_CHAR*)wFilePath,1,(EFK_CHAR*)wPath);
      _effectList.push_back(efk_effect);
      _effectMap.insert(pair<string,int>(p.filename(),_effectList.size() - 1));
    }
  });

}

//==============================================================================
// effect
//------------------------------------------------------------------------------
void Effect::stop(int id) {
  _effekseerManager->StopEffect(id);
}

//==============================================================================
// effect
//------------------------------------------------------------------------------
void Effect::setPos(int id,const Vec3& pos) {
  _effekseerManager->SetLocation(id,pos.x,pos.y,pos.z);
}

//==============================================================================
// effect
//------------------------------------------------------------------------------
void Effect::setRot(int id,const Vec3& rot) {
  _effekseerManager->SetRotation(id,rot.x,rot.y,rot.z);
}

//==============================================================================
// effect
//------------------------------------------------------------------------------
void Effect::setScl(int id,const Vec3& scl) {
  _effekseerManager->SetScale(id,scl.x,scl.y,scl.z);
}


//==============================================================================
// effect
//------------------------------------------------------------------------------
bool Effect::isExists(int id) const {
  return _effekseerManager->Exists(id);
}

//==============================================================================
// effect
//------------------------------------------------------------------------------
int Effect::play(unsigned int id, const Vec3& pos) {
  //エフェクトの再生
  return  _effekseerManager->Play(_effectList[id],pos.x,pos.y,pos.z);
}

//==============================================================================
// effect
//------------------------------------------------------------------------------
int Effect::play(const char* file,const Vec3& pos) {
  auto it = _effectMap.find(file);
  
  if(it == _effectMap.end()) {
    return -1;
  }

  //エフェクトの再生
  return play(it->second, pos);
}

//==============================================================================
// effect
//------------------------------------------------------------------------------
Effect::~Effect() {
  // エフェクトリリース
  for(auto effect : _effectList) {
    SafeRelease(effect);
  }

  // エフェクト管理用インスタンスを破棄
  _effekseerManager->Destroy();
  // サウンド用インスタンスを破棄
  _effekseerSound->Destory();
  // 描画用インスタンスを破棄
  _effekseerRenderer->Destory();
}

//==============================================================================
// update
//------------------------------------------------------------------------------
void Effect::update() {
  Effekseer::Matrix44 proj,view;
  const Renderer* renderer = App::instance().getRenderer();
  auto camera = renderer->getCamera();

  memcpy(&proj,&renderer->getProjMtx(),sizeof(Effekseer::Matrix44));
  memcpy(&view,&camera->getViewMtx(),sizeof(Effekseer::Matrix44));
  
  // 投影行列の更新
  _effekseerRenderer->SetProjectionMatrix(proj);
  // カメラ行列の更新
  _effekseerRenderer->SetCameraMatrix(view);

  //TODO 
  // 3Dサウンド用リスナー設定の更新
  //_effekseerSound->SetListener(リスナー位置,注目点,上方向ベクトル);

  // 全てのエフェクトの更新
  _effekseerManager->Update();

}

//==============================================================================
// draw
//------------------------------------------------------------------------------
void Effect::draw() {
  _effekseerRenderer->BeginRendering();
  _effekseerManager->Draw();
  _effekseerRenderer->EndRendering();
}

//EOF