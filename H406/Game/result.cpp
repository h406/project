//==============================================================================
//
// result[result.cpp]
// Author : masato masuda : 2015/12/09
//
//==============================================================================

//******************************************************************************
// include
//******************************************************************************
#include "result.h"
#include "title.h"
#include "BaceScene.h"
#include "ledConnect.h"
#include "BaceScene.h"
#include "dataManager.h"
#include "player.h"
#include "colStage.h"
#include "eventManager.h"
#include "ShuchuSen.h"

static float f =  D3DX_PI * 0.5f;

//==============================================================================
// init
//------------------------------------------------------------------------------
bool Result::init() {
  const D3DXVECTOR2 windowSizeHalf = D3DXVECTOR2(App::instance().getWindowSize().cx * 0.5f, App::instance().getWindowSize().cy * 0.5f);
  _windowScl = (float)(App::instance().getWindowSize().cx / 1280.f);

  auto camera = App::instance().getRenderer()->getCamera();
  _camera = camera->createCamera();
  _camera->setPosP({0,5,0});
  _camera->setPosR({0,50,0});
  camera->setCamera(_camera, 100);
  f = -D3DX_PI * 0.5f;

  _winSprite = Sprite2D::create();
  _winSprite->setSize((float)App::instance().getWindowSize().cx, (float)App::instance().getWindowSize().cy);
  _winSprite->setPos(App::instance().getWindowSize().cx * 0.5f, App::instance().getWindowSize().cy * 0.45f);
  _winSprite->setColor(D3DXCOLOR(1, 1, 1, 1));
  this->addChild(_winSprite);
  _winSpriteScl = 0.0f;

  // 勝者
  int winPlayerId = 0;
  const int win[2] = { DataManager::instance().getData()->getPlayerRoundWin(0),
                       DataManager::instance().getData()->getPlayerRoundWin(1) };

  if (win[0] >= win[1]){
    _winSprite->setTexture("./data/texture/win_00.png");
  } else{
    winPlayerId = 1;
    _winSprite->setTexture("./data/texture/win_01.png");
  }

  // プレイヤー
  _player = Player::create(winPlayerId);
  _player->setPos(Vec3(0.0f, 500.0f, 0.0f));
  _player->setHitEnable(false);
  this->addChild(_player);

  // フィールドとの当たり判定
  EventManager* eventManager = nullptr;
  auto hitcheck = ColStage::create(BaceScene::instance()->getStage(), eventManager);
  hitcheck->addPlayer(_player);
  this->addChild(hitcheck, INT_MAX);

  auto _s = ShuchuSen::create("./data/texture/image.png");
  _s->setSize(1500.f * _windowScl, 1500.f * _windowScl);
  _s->setPos(App::instance().getWindowSize().cx*0.5f, App::instance().getWindowSize().cy*0.5f);
  this->addChild(_s);

  // サウンドロード
//  App::instance().getSound()->load("./data/sound/se/system_ok.wav");

  BaceScene::instance()->getLedConnect()->sendEvent(LedEvent::MoveResult);

  return true;
}

//==============================================================================
// update
//------------------------------------------------------------------------------
void Result::update() {
  f += 0.01f;
  const Vec3 playerPos = _player->getPos();

  _camera->setPosP({ sinf(f) * 200, 75.0f, cosf(f) * 200 });
  _camera->setPosR({ 0.0f, playerPos.y + 50.0f, 0.0f });

  _winSpriteScl += (1.f - _winSpriteScl) * 0.15f;
  _winSprite->setSize((float)App::instance().getWindowSize().cx * _winSpriteScl, (float)App::instance().getWindowSize().cy * _winSpriteScl);

  if(App::instance().getInput()->isRelease(0,VK_INPUT::_2)) {
    BaceScene::instance()->setCurScene(Title::create());
//    App::instance().getSound()->play("./data/sound/se/system_ok.wav", false);
    App::instance().getSound()->stop("./data/sound/bgm/game_main.wav");
  }

  if(App::instance().getInput()->isRelease(0,VK_INPUT::_3)) {
//    BaceScene::instance()->getLedConnect()->sendText("ああああああああああああああああああ");
  }

  auto input = App::instance().getInput();
  if (input->isPress(0, VK_INPUT::UP)) {
  }
  if (input->isPress(0, VK_INPUT::DOWN)) {
  }
  if (input->isPress(0, VK_INPUT::LEFT)) {
  }
  if (input->isPress(0, VK_INPUT::RIGHT)) {
  }
  if (input->isPress(1, VK_INPUT::UP)) {
  }
  if (input->isPress(1, VK_INPUT::DOWN)) {
  }
  if (input->isPress(1, VK_INPUT::LEFT)) {
  }
  if (input->isPress(1, VK_INPUT::RIGHT)) {
  }

}

//==============================================================================
// uninit
//------------------------------------------------------------------------------
void Result::uninit() {
  // 生成したカメラを全て削除
  App::instance().getRenderer()->getCamera()->releaseCamera(_camera);
}


//EOF