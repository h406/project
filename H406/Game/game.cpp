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
#include "stageBlock.h"
#include "staticStage.h"
#include "player.h"
#include "gauge.h"


//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
bool Game::init() {
  auto camera = App::instance().getRenderer()->getCamera();
  const Vec2 bordSize = Vec2(1000 / (float)StageBlock::kNUM_X,1000 / (float)StageBlock::kNUM_Y);

  auto e = Sprite2D::create("./data/texture/e.png");
  e->setColor(D3DXCOLOR(1,1,1,1));
  e->setSize((float)App::instance().getWindowSize().cx,(float)App::instance().getWindowSize().cy);
  e->setPos(App::instance().getWindowSize().cx * 0.5f,App::instance().getWindowSize().cy * 0.5f,0);
  this->addChild(e);

  // 動かないステージオブジェクト群
  auto staticStage = StaticStage::create();
  this->addChild(staticStage);

  _player[0] = Player::create();
  _player[0]->setPos(Vec3(-500 + bordSize.x * 0.5f,0,0));
  this->addChild(_player[0]);

  _player[1] = Player::create();
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

  _backField = Sprite3D::create();
  _backField->setSize(1000,1000);
  _backField->setColor(D3DXCOLOR(0,0,0,1));
  _backField->setRotX(D3DX_PI * 0.5f);
  this->addChild(_backField);

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

  _num[0] = _num[1] = 0;
  _numSpriteScl[0] = _numSpriteScl[1] = 1;

  // ステージブロック
  _stageBlock = StageBlock::create();
  this->addChild(_stageBlock);

  _gauge[0] = Gauge::create();
  _gauge[0]->setScl(500.0f, 50.0f, 0.0f);
  _gauge[0]->setPos(250.0f, 25.0f, 0.0f);
  _gauge[0]->setColor(D3DXCOLOR(0.0f,0.0f,1.0f,1.0f));;
  this->addChild(_gauge[0]);

  _gauge[1] = Gauge::create();
  _gauge[1]->setScl(500.0f, 50.0f, 0.0f);
  _gauge[1]->setPos(App::instance().getWindowSize().cx - 250.0f, 25.0f, 0.0f);
  _gauge[1]->setColor(D3DXCOLOR(1.0f,1.0f,0.0f,1.0f));;
  _gauge[1]->setFlip(true);
  this->addChild(_gauge[1]);

  memset(_playerMoveVec,0,sizeof(_playerMoveVec));

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
//      _player[i]->moveBottom(5.0f);

    if (playerPos[i].x < -500) {
      playerPos[i].x = -500 + abs(playerPos[i].x + 500);
      _playerMoveVec[i].x *= -1;
    }
    if (playerPos[i].z < -500) {
      playerPos[i].z = -500 + abs(playerPos[i].z + 500);
      _playerMoveVec[i].z *= -1;
    }
    if (playerPos[i].x > 500) {
      playerPos[i].x = 500 - (playerPos[i].x - 500);
      _playerMoveVec[i].x *= -1;
    }
    if (playerPos[i].z > 500) {
      playerPos[i].z = 500 - (playerPos[i].z - 500);
      _playerMoveVec[i].z *= -1;
    }
    if (playerPos[i].y < 0.0f){
      playerPos[i].y = 0.0f;
    }
    _player[i]->setPos(playerPos[i]);
    }
  }
  
  const int _playerID[2][2] = {
    {int((playerPos[0].x + 500) / (1000 / (float)StageBlock::kNUM_X)),int((playerPos[0].z + 500) / (1000 / (float)StageBlock::kNUM_Y))},
    {int((playerPos[1].x + 500) / (1000 / (float)StageBlock::kNUM_X)),int((playerPos[1].z + 500) / (1000 / (float)StageBlock::kNUM_Y))}
  };
  
  for(int i = 0; i < 2; i++) {
    const StageBlock::FIELD_ID fieldID = _stageBlock->getFieldID(_playerID[i][0],_playerID[i][1]);

    if(_playerID[i][0] >= 0 && _playerID[i][0] < StageBlock::kNUM_X && _playerID[i][1] >= 0 && _playerID[i][1] < StageBlock::kNUM_Y) {
      if(fieldID == StageBlock::FIELD_ID::ITEM) {
        const int plus = rand() % 4 + 1;
        _stageBlock->setFieldID(_playerID[i][0] , _playerID[i][1], StageBlock::FIELD_ID(int(StageBlock::FIELD_ID::PLAYER_1) + i));
        _plusNum[i]->setAnimID(plus);
        _num[i] += plus;
        if(_num[i] > 9) _num[i] = 9;
        _numSpriteScl[i] = 2;
        _effect->play("get.efk",playerPos[i]);

        _playerMoveVec[i].y = 10;
      }
      else if(fieldID != StageBlock::FIELD_ID::ITEM && fieldID != StageBlock::FIELD_ID(int(StageBlock::FIELD_ID::PLAYER_1) + i) && _num[i] > 0) {
        _stageBlock->setFieldID(_playerID[i][0],_playerID[i][1],StageBlock::FIELD_ID(int(StageBlock::FIELD_ID::PLAYER_1) + i));
        _numSpriteScl[i] = 0.7f;
        _num[i] --;
        _effect->play("shot.efk",playerPos[i]);
      }
    }
  }

  if((rand() % (60 * 1)) == 0) {
    _stageBlock->setFieldID(rand() % StageBlock::kNUM_X,rand() % StageBlock::kNUM_Y,StageBlock::FIELD_ID::ITEM);
  }

  for(int i = 0;i < 2;i++) {
    if(_num[i] > 0) {
      _numSprite[i]->setAnimID(_num[i]);
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

  float gaugeRate[2] = { _num[0] / (float)9, _num[1] / (float)9 };
  _gauge[0]->setRate(gaugeRate[0]);
  _gauge[1]->setRate(gaugeRate[1]);

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

  /*
  1201.85042515f = sqrtf((8 * 8) + (12 * 12)) * (1000 / (float)StageBlock::kNUM_X);
  */

  if(length > (1201.85042515f)) length = (1201.85042515f);

  _mainCamera->setPosP(Vec3(0,sinf(rot) * length,cosf(rot) * length) + camvec);
  _mainCamera->setPosR(camvec);

  if(_bultime) {
    Vec3 ram(float(rand() % 10),float(rand() % 10),0);
    _mainCamera->setPosP(_mainCamera->getPosP() + ram);
    _mainCamera->setPosR(camvec + ram);
    _bultime--;
  }


}

//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
void Game::uninit() {

}

//EOF