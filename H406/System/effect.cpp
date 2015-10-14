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
#include "sound.h"

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
bool Effect::init() {

  // �`��p�C���X�^���X�̐���
  _effekseerRenderer = ::EffekseerRendererDX9::Renderer::Create(App::instance().getRenderer()->getDevice(),2000);

  // �G�t�F�N�g�Ǘ��p�C���X�^���X�̐���
  _effekseerManager = ::Effekseer::Manager::Create(2000);

  // �`��p�C���X�^���X����`��@�\��ݒ�
  _effekseerManager->SetSpriteRenderer(_effekseerRenderer->CreateSpriteRenderer());
  _effekseerManager->SetRibbonRenderer(_effekseerRenderer->CreateRibbonRenderer());
  _effekseerManager->SetRingRenderer(_effekseerRenderer->CreateRingRenderer());
  _effekseerManager->SetModelRenderer(_effekseerRenderer->CreateModelRenderer());

  // �`��p�C���X�^���X����e�N�X�`���̓Ǎ��@�\��ݒ�
  // �Ǝ��g���\�A���݂̓t�@�C������ǂݍ���ł���B
  _effekseerManager->SetTextureLoader(_effekseerRenderer->CreateTextureLoader());
  _effekseerManager->SetModelLoader(_effekseerRenderer->CreateModelLoader());

  // ���Đ��p�C���X�^���X�̐���
  _effekseerSound = ::EffekseerSound::Sound::Create(App::instance().getSound()->getDevice(),16,16);

  // ���Đ��p�C���X�^���X����Đ��@�\���w��
  _effekseerManager->SetSoundPlayer(_effekseerSound->CreateSoundPlayer());

  // ���Đ��p�C���X�^���X����T�E���h�f�[�^�̓Ǎ��@�\��ݒ�
  // �Ǝ��g���\�A���݂̓t�@�C������ǂݍ���ł���B
  _effekseerManager->SetSoundLoader(_effekseerSound->CreateSoundLoader());

  // ���W�n
  _effekseerManager->SetCoordinateSystem(Effekseer::CoordinateSystem::LH);

  // �G�t�F�N�g�̓Ǎ�
  namespace sys = std::tr2::sys;
  sys::path p(kBaceEffectFolder);
  std::for_each(sys::directory_iterator(p),sys::directory_iterator(),
    [this](const sys::path& p) {
    if(sys::is_regular_file(p)) { // �t�@�C���Ȃ�...
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

  return true;
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
void Effect::setEffectPos(int id,const Vec3& pos) {
  _effekseerManager->SetLocation(id,pos.x,pos.y,pos.z);
}

//==============================================================================
// effect
//------------------------------------------------------------------------------
void Effect::setEffectRot(int id,const Vec3& rot) {
  _effekseerManager->SetRotation(id,rot.x,rot.y,rot.z);
}

//==============================================================================
// effect
//------------------------------------------------------------------------------
void Effect::setEffectScl(int id,const Vec3& scl) {
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
  //�G�t�F�N�g�̍Đ�
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

  //�G�t�F�N�g�̍Đ�
  return play(it->second, pos);
}

//==============================================================================
// effect
//------------------------------------------------------------------------------
void Effect::uninit() {
  // �G�t�F�N�g�����[�X
  for(auto effect : _effectList) {
    SafeRelease(effect);
  }

  // �G�t�F�N�g�Ǘ��p�C���X�^���X��j��
  _effekseerManager->Destroy();
  // �T�E���h�p�C���X�^���X��j��
  _effekseerSound->Destory();
  // �`��p�C���X�^���X��j��
  _effekseerRenderer->Destory();
}

//==============================================================================
// update
//------------------------------------------------------------------------------
void Effect::update() {

}

//==============================================================================
// draw
//------------------------------------------------------------------------------
void Effect::draw(const Renderer* renderer) {
  _effekseerRenderer->BeginRendering();
  _effekseerManager->Draw();
  _effekseerRenderer->EndRendering();
}

//==============================================================================
// updateWorldMtx
//------------------------------------------------------------------------------
void Effect::updateWorldMtx() {
  Effekseer::Matrix44 proj,view;
  D3DXMATRIX temp;
  const Renderer* renderer = App::instance().getRenderer();
  const Camera* camera = renderer->getCamera();

  // �e�Ăяo��
  node::updateWorldMtx();

  temp = _mtxWorld * camera->getViewMtx();
  memcpy(&proj,&renderer->getProjMtx(),sizeof(Effekseer::Matrix44));
  memcpy(&view,&temp,sizeof(Effekseer::Matrix44));

  // �J�����擾
  Effekseer::Vector3D p,r,u;
  memcpy(&p,&camera->getCurrentCamera()->getPosP(),sizeof(Effekseer::Vector3D));
  memcpy(&r,&camera->getCurrentCamera()->getPosR(),sizeof(Effekseer::Vector3D));
  memcpy(&u,&camera->getCurrentCamera()->getVecU(),sizeof(Effekseer::Vector3D));
  
  // ���e�s��̍X�V
  _effekseerRenderer->SetProjectionMatrix(proj);
  // �J�����s��̍X�V
  _effekseerRenderer->SetCameraMatrix(view);
  // 3D�T�E���h�p���X�i�[�ݒ�̍X�V
  _effekseerSound->SetListener(p,r,u);

  // �S�ẴG�t�F�N�g�̍X�V
  _effekseerManager->Update();
}

//EOF