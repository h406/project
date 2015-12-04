//==============================================================================
//
// BaceScene[BaceScene.cpp]
// Author : Yasuaki Yamashita : 2015/11/11
//
//==============================================================================

//******************************************************************************
// include
//******************************************************************************
#include "BaceScene.h"
#include "staticStage.h"
#include "stage.h"
#include "ledConnect.h"

namespace {
  Vec2 kStageSize(1000,1000);
}

// static instance
BaceScene* BaceScene::_instance;


//==============================================================================
// init
//------------------------------------------------------------------------------
bool BaceScene::init() {
  // instance
  _instance = this;

  // 動かないステージオブジェクト群
  auto staticStage = StaticStage::create();
  this->addChild(staticStage);

  // ステージブロック
  _stage = Stage::create(1000.f,1000.f);
  this->addChild(_stage);

  // えっふぇくと
  _effect = Effect::create();
  _effect->setScl(Vec3(1,1,1));
  this->addChild(_effect);

  // LED
  _ledConnect = new LedConnect();

  _curScene = nullptr;
  _releaseScene = nullptr;
  return true;
}

//==============================================================================
// update
//------------------------------------------------------------------------------
void BaceScene::update() {
  if(_releaseScene != nullptr) {
    _releaseScene->release();
    _releaseScene = nullptr;
  }
}

//==============================================================================
// uninit
//------------------------------------------------------------------------------
void BaceScene::uninit() {

  delete _ledConnect;
}

//==============================================================================
// setCurScene
//------------------------------------------------------------------------------
void BaceScene::setCurScene(iScene* scene) {
  if(_curScene != nullptr) {
    _releaseScene = _curScene;
    _curScene = nullptr;
  }
  this->addChild(_curScene = scene);
  
}

//EOF