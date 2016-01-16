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

namespace{
  const Vec2 kPlayerSpriteSIze = Vec2(222.0f, 512.0f) * 1.2f;
}

//==============================================================================
// init
//------------------------------------------------------------------------------
bool Result::init() {
  const D3DXVECTOR2 windowSizeHalf = D3DXVECTOR2(App::instance().getWindowSize().cx * 0.5f, App::instance().getWindowSize().cy * 0.5f);
  _windowScl = (float)(App::instance().getWindowSize().cx / 1280.f);
  _isVisible = false;

  auto camera = App::instance().getRenderer()->getCamera();
  _camera = camera->createCamera();
  _camera->setPosP({0,5,0});
  _camera->setPosR({0,50,0});
  camera->setCamera(_camera, 90);
  _frameCount = D3DX_PI;

  _winSprite = Sprite2D::create();
  _winSprite->setSize(0.0f,0.0f);
  _winSprite->setPos(App::instance().getWindowSize().cx * 0.5f, App::instance().getWindowSize().cy * 0.5f);
  _winSprite->setColor(D3DXCOLOR(1, 1, 1, 1));
  this->addChild(_winSprite);
  _winSpriteScl = 0.0f;

  // 勝者
  _winPlayerId = 0;
  const int win[2] = { DataManager::instance().getData()->getPlayerRoundWin(0),
                       DataManager::instance().getData()->getPlayerRoundWin(1) };

  if (win[0] >= win[1]){
    _winSprite->setTexture("./data/texture/win_00.png");
  } else{
    _winPlayerId = 1;
    _winSprite->setTexture("./data/texture/win_01.png");
  }

  // プレイヤー
  _player = Player::create(_winPlayerId);
  _player->setPos(Vec3(0.0f, 1500.0f, 0.0f));
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

  _playerSprite = Sprite2D::create();
  const char* fileName[2] = { "./data/texture/oji_pic.png", "./data/texture/oba_pic.png" };
  const Vec2 spritePos[2] = { Vec2((-kPlayerSpriteSIze.x * _windowScl) * 0.55f, (float)App::instance().getWindowSize().cy - (kPlayerSpriteSIze.y * _windowScl) * 0.45f),
                              Vec2((float)App::instance().getWindowSize().cx + (kPlayerSpriteSIze.x * _windowScl) * 0.55f, (float)App::instance().getWindowSize().cy - (kPlayerSpriteSIze.y * _windowScl) * 0.45f) };

  _playerSprite->setSize(kPlayerSpriteSIze.x * _windowScl, kPlayerSpriteSIze.y * _windowScl);
  _playerSprite->setPos(spritePos[_winPlayerId]);
  _playerSprite->setTexture(fileName[_winPlayerId]);
  _playerSprite->setNumU(4);
  _playerSprite->setNumV(1);
  _playerSprite->setAnimID(3);
  this->addChild(_playerSprite);

//  _playerSpritePosDest = spritePos[_winPlayerId];

  // サウンドロード
//  App::instance().getSound()->load("./data/sound/se/system_ok.wav");
  App::instance().getSound()->play("./data/sound/bgm/game_main.wav",true);

  BaceScene::instance()->getLedConnect()->sendEvent(LedEvent::MoveResult, &_winPlayerId);

  return true;
}

//==============================================================================
// update
//------------------------------------------------------------------------------
void Result::update() {

  const Vec3 playerPos = _player->getPos();
  _player->moveBottom(10.0f);

  const float cameraLength = 180.0f;
  _camera->setPosP({ sinf(_frameCount) * cameraLength, 75.0f, cosf(_frameCount) * cameraLength });
  _camera->setPosR({ 0.0f, playerPos.y + 50.0f, 0.0f });

  if (_player->getPos().y < 1.0f){

    _frameCount += 0.01f;
    _winSpriteScl += (1.f - _winSpriteScl) * 0.1f;
    _winSprite->setSize((float)App::instance().getWindowSize().cx * _winSpriteScl, (float)App::instance().getWindowSize().cy * _winSpriteScl);

    // おじおば
    const Vec2 spritePos[2] = { Vec2((kPlayerSpriteSIze.x * _windowScl) * 0.55f, (float)App::instance().getWindowSize().cy - (kPlayerSpriteSIze.y * _windowScl) * 0.45f),
                                Vec2((float)App::instance().getWindowSize().cx - (kPlayerSpriteSIze.x * _windowScl) * 0.55f, (float)App::instance().getWindowSize().cy - (kPlayerSpriteSIze.y * _windowScl) * 0.45f) };
    Vec2 pos = _playerSprite->getPos() + ((spritePos[_winPlayerId] - _playerSprite->getPos()) * 0.1f);
    _playerSprite->setPos(pos);

  //  if (abs(spritePos[_winPlayerId].x) - abs(pos.x) < 1.f){
  //    _isVisible = true;
  //  }
  //  if (_isVisible){
  //  }
  }

//  if(App::instance().getInput()->isRelease(0,VK_INPUT::_2)) {
  if (App::instance().getInput()->isTrigger(0, VK_INPUT::_1) || App::instance().getInput()->isTrigger(1, VK_INPUT::_1)) {
    BaceScene::instance()->setCurScene(Title::create());
//    App::instance().getSound()->play("./data/sound/se/system_ok.wav", false);
    App::instance().getSound()->stop("./data/sound/bgm/game_main.wav");
    App::instance().getSound()->stop("./data/sound/bgm/game_main_loop.wav");
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