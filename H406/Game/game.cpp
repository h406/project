//==============================================================================
//
// game[title.cpp]
// Author : Yasuaki Yamashita : 2015/10/04
//
//==============================================================================

//******************************************************************************
// include
//******************************************************************************
#include "game.h"
#include "stage.h"
#include "staticStage.h"
#include "player.h"
#include "colStage.h"
#include "eventManager.h"
#include "EventList.h"
#include "EventData.h"

//------------------------------------------------------------------------------
// init
//------------------------------------------------------------------------------
bool Game::init() {
  auto camera = App::instance().getRenderer()->getCamera();
  const Vec2 bordSize = Vec2(1000 / (float)Stage::kNUM_X,1000 / (float)Stage::kNUM_Y);

  auto e = Sprite2D::create("./data/texture/e.png");
  e->setColor(D3DXCOLOR(1,1,1,1));
  e->setSize((float)App::instance().getWindowSize().cx,(float)App::instance().getWindowSize().cy);
  e->setPos(App::instance().getWindowSize().cx * 0.5f,App::instance().getWindowSize().cy * 0.5f,0);
  this->addChild(e);

  // 動かないステージオブジェクト群
  auto staticStage = StaticStage::create();
  this->addChild(staticStage);

  // イベントマネージャー
  _eventManager = new EventManager();

  // プレイヤー
  _player[0] = Player::create(0);
  _player[0]->setPos(Vec3(-500 + bordSize.x * 0.5f,0,0));
  this->addChild(_player[0]);

  _player[1] = Player::create(1);
  _player[1]->setPos(Vec3(500 - bordSize.x * 0.5f, 0, 0));
  this->addChild(_player[1]);

  _mainCamera = camera->createCamera();
  _mainCamera->setPosP({0,600,-900});
  _mainCamera->setPosR({0,0,-70});
  camera->setCamera(_mainCamera);

  _playerCam[0] = camera->createCamera();
  _playerCam[0]->setPosP({0,0,0});
  _playerCam[0]->setPosR({0,0,0});

  _playerCam[1] = camera->createCamera();
  _playerCam[1]->setPosP({0,0,0});
  _playerCam[1]->setPosR({0,0,0});

  _effect = Effect::create();
  _effect->setScl(Vec3(1,1,1));
  this->addChild(_effect);

  _numSprite[0] = Sprite2D::create("./data/texture/num.png");
  _numSprite[0]->setSize(128,128);
  _numSprite[0]->setPos(128,200,0);
  _numSprite[0]->setNumU(11);
  _numSprite[0]->setAnimID(10);
  _numSprite[0]->setVisible(false);
  this->addChild(_numSprite[0]);

  _numSprite[1] = Sprite2D::create("./data/texture/num.png");
  _numSprite[1]->setSize(128,128);
  _numSprite[1]->setPos(1280 - 128,200,0);
  _numSprite[1]->setNumU(11);
  _numSprite[1]->setAnimID(10);
  _numSprite[1]->setVisible(false);
  this->addChild(_numSprite[1]);

  _plus[0] = Sprite2D::create("./data/texture/num.png");
  _plus[0]->setSize(128,128);
  _plus[0]->setPos(128,328,0);
  _plus[0]->setNumU(11);
  _plus[0]->setAnimID(10);
  _plus[0]->setVisible(true);
  _plus[0]->setColor(D3DXCOLOR(0,0,0,0));
  this->addChild(_plus[0]);

  _plus[1] = Sprite2D::create("./data/texture/num.png");
  _plus[1]->setSize(128,128);
  _plus[1]->setPos(1280 - 255 ,328,0);
  _plus[1]->setNumU(11);
  _plus[1]->setAnimID(10);
  _plus[1]->setVisible(true);
  _plus[1]->setColor(D3DXCOLOR(0,0,0,0));
  this->addChild(_plus[1]);

  _plusNum[0] = Sprite2D::create("./data/texture/num.png");
  _plusNum[0]->setSize(128,128);
  _plusNum[0]->setPos(256,328,0);
  _plusNum[0]->setNumU(11);
  _plusNum[0]->setAnimID(10);
  _plusNum[0]->setVisible(true);
  _plusNum[0]->setColor(D3DXCOLOR(0,0,0,0));
  this->addChild(_plusNum[0]);

  _plusNum[1] = Sprite2D::create("./data/texture/num.png");
  _plusNum[1]->setSize(128,128);
  _plusNum[1]->setPos(1280 - 128, 328,0);
  _plusNum[1]->setNumU(11);
  _plusNum[1]->setAnimID(10);
  _plusNum[1]->setVisible(true);
  _plusNum[1]->setColor(D3DXCOLOR(0,0,0,0));
  this->addChild(_plusNum[1]);

  _numSpriteScl[0] = _numSpriteScl[1] = 1;

  // ステージブロック
  _stage = Stage::create(1000.f,1000.f);
  this->addChild(_stage);

  // ステージとの当たり判定
  auto hitcheck = ColStage::create(_stage,_eventManager);
  hitcheck->addPlayer(_player[0]);
  hitcheck->addPlayer(_player[1]);
  // 最後に処理をさせる
  this->addChild(hitcheck,INT_MAX);

  // イベントセット
  _eventManager->addEventListener(EventList::PLAYER_1_ITEM_GET, bind(&Game::EventListener,this,placeholders::_1));
  _eventManager->addEventListener(EventList::PLAYER_2_ITEM_GET, bind(&Game::EventListener,this,placeholders::_1));
  _eventManager->addEventListener(EventList::PLAYER_1_ITEM_USING, bind(&Game::EventListener,this,placeholders::_1));
  _eventManager->addEventListener(EventList::PLAYER_2_ITEM_USING, bind(&Game::EventListener,this,placeholders::_1));

  _freezeTime = 0;
  _bultime = 0;

  return true;
}


//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
void Game::update() {
  auto input = App::instance().getInput();

  Vec3 playerPos[2] = {
    _player[0]->getPos(),
    _player[1]->getPos(),
  };

  if (_freezeTime == 0) {
    for (int i = 0; i < 2; i++) {
      if (input->isPress(i, VK_INPUT::UP)) {
        _player[i]->moveUp(5.0f);
      }
      if (input->isPress(i, VK_INPUT::DOWN)) {
        _player[i]->moveDown(5.0f);
      }
      if (input->isPress(i, VK_INPUT::LEFT)) {
        _player[i]->moveLeft(5.0f);
      }
      if (input->isPress(i, VK_INPUT::RIGHT)) {
        _player[i]->moveRight(5.0f);
      }
    }
  }

  if((rand() % (60 * 1)) == 0) {
    _stage->setFieldID(rand() % Stage::kNUM_X,rand() % Stage::kNUM_Y,Stage::FIELD_ID::ITEM);
  }

  for(int i = 0;i < 2;i++) {
    if(_player[i]->getDripNum() > 0) {
      _numSprite[i]->setAnimID(_player[i]->getDripNum());
      _numSprite[i]->setVisible(true);
    }
    else {
      _numSprite[i]->setVisible(false);
    }
    _numSpriteScl[i] += (1 - _numSpriteScl[i]) * 0.1f;
    _numSprite[i]->setSize(128 * _numSpriteScl[i],128 * _numSpriteScl[i]);

    _plusNum[i]->setColor(D3DXCOLOR(1,1,1,min((_numSpriteScl[i] - 1) * 2, 1)));
    _plus[i]->setColor(D3DXCOLOR(1,1,1,min((_numSpriteScl[i] - 1) * 2,1)));
    _plusNum[i]->setSize(128 * _numSpriteScl[i],128 * _numSpriteScl[i]);
    _plus[i]->setSize(128 * _numSpriteScl[i],128 * _numSpriteScl[i]);
  }

  if(input->isTrigger(0,VK_INPUT::_2)) {
    _freezeTime = 75;
  }

  if(_freezeTime != 0) {
    switch(_freezeTime) {
    case 75:
      App::instance().getRenderer()->getCamera()->setCamera(_playerCam[0],30);
      break;

    case 60:
      _effect->play("test2.efk",playerPos[0] + Vec3(0,20,0));
      _bultime = 550;
      break;

    case 10:
      App::instance().getRenderer()->getCamera()->setCamera(_mainCamera, 10);
      break;
    }

    _freezeTime--;
  }

  for(int i = 0;i < 2;i++) {
    _playerCam[i]->setPosP(playerPos[i] + Vec3(0,50,-200));
    _playerCam[i]->setPosR(playerPos[i] + Vec3(0,10,0));
  }

  const Vec3 camvec((playerPos[0] + playerPos[1]) * 0.5f);
  const Vec3 playerDir(playerPos[0] - playerPos[1]);
  float length = D3DXVec3Length(&playerDir);
  const float rot = atan2f(300,-900);

  if(length < 300) length = 300;
  if(length >(1201.85042515f/* = sqrtf((8 * 8) + (12 * 12)) * (1000 / (float)Stage::kNUM_X)*/))
    length = (1201.85042515f);

  _mainCamera->setPosP(Vec3(0,sinf(rot) * length,cosf(rot) * length) + camvec);
  _mainCamera->setPosR(camvec);

  if(_bultime) {
    Vec3 ram(float(rand() % 10),float(rand() % 10),0);
    _mainCamera->setPosP(_mainCamera->getPosP() + ram);
    _mainCamera->setPosR(camvec + ram);
    _bultime--;
  }
}

//==============================================================================
// イベント
//------------------------------------------------------------------------------
void Game::EventListener(EventData* eventData) {
  switch(eventData->getEvent()) {
  // アイテム取得した
  case EventList::PLAYER_1_ITEM_GET:
    _effect->play("get.efk",_player[0]->getPos());
    _numSpriteScl[0] = 2.f;
    _plusNum[0]->setAnimID((int)(eventData->getUserData()));
    break;

  case EventList::PLAYER_2_ITEM_GET:
    _effect->play("get.efk",_player[1]->getPos());
    _numSpriteScl[1] = 2.f;
    _plusNum[1]->setAnimID((int)(eventData->getUserData()));
    break;

  // アイテム使用
  case EventList::PLAYER_1_ITEM_USING:
    _numSpriteScl[0] = 0.7f;
    break;

  case EventList::PLAYER_2_ITEM_USING:
    _numSpriteScl[1] = 0.7f;
    break;

  }
}

//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
void Game::uninit() {
  SafeDelete(_eventManager);
}

//EOF