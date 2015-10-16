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

//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
bool Game::init() {
  auto camera = App::instance().getRenderer()->getCamera();
  const Vec2 bordSize = Vec2(1000 / (float)kNUM_X,1000 / (float)kNUM_Y);

  auto backBG = MeshCylinder::create(60,10,1000.f,30.f);
  backBG->setColor(D3DXCOLOR(1,1,1,1));
  backBG->setTexture("./data/texture/akira001.png");
  this->addChild(backBG);

  auto e = Sprite2D::create("./data/texture/e.png");
  e->setColor(D3DXCOLOR(1,1,1,1));
  e->setSize(App::instance().getWindowSize().cx,App::instance().getWindowSize().cy);
  e->setPos(App::instance().getWindowSize().cx * 0.5f,App::instance().getWindowSize().cy * 0.5f,0);
  this->addChild(e);

  auto ground = MeshField::create(100,100,5000.f,5000.f);
  ground->setTexture("./data/texture/pave006.bmp");
  ground->setPos(Vec3(0,-1,0));
  this->addChild(ground);

  MeshField* fence = MeshField::create(10,10,1000.f,100.f);
  fence->setTexture("./data/texture/face_fence01.png");
  fence->setRot(Vec3(D3DX_PI * -0.5f,0,0));
  fence->setPos(Vec3(0,50,500));
  this->addChild(fence);

  fence = MeshField::create(10,10,1000.f,100.f);
  fence->setTexture("./data/texture/face_fence01.png");
  fence->setRot(Vec3(D3DX_PI * -0.5f,D3DX_PI * -0.5f,0));
  fence->setPos(Vec3(-500,50,0));
  this->addChild(fence);

  fence = MeshField::create(10,10,1000.f,100.f);
  fence->setTexture("./data/texture/face_fence01.png");
  fence->setRot(Vec3(D3DX_PI * -0.5f,D3DX_PI * 0.5f,0));
  fence->setPos(Vec3(500,50,0));
  this->addChild(fence);

  _player[0] = Sprite3D::create("./data/texture/akira000.png");
  _player[0]->setSize(36 * 2,72 * 2);
  _player[0]->setPos(-500 + bordSize.x * 0.5f,0,0);
  this->addChild(_player[0]);

  _player[1] = Sprite3D::create("./data/texture/akira000.png");
  _player[1]->setSize(36 * 2,72 * 2);
  _player[1]->setPos(500 - bordSize.x * 0.5f,0,0);
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
  _plus[1]->setPos(1280 - 128,200,0);
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
  _plusNum[1]->setPos(1280 - 128,200,0);
  _plusNum[1]->setNumU(11);
  _plusNum[1]->setAnimID(10);
  _plusNum[1]->setVisible(true);
  _plusNum[1]->setColor(D3DXCOLOR(0,0,0,0));
  this->addChild(_plusNum[1]);

  _num[0] = _num[1] = 0;
  _numSpriteScl[0] = _numSpriteScl[1] = 1;

  for(int x = 0; x < kNUM_X; x++) {
    for(int y = 0; y < kNUM_Y; y++) {
      _fieldMap[x][y] = Sprite3D::create();
      _fieldMap[x][y]->setSize(bordSize.x - 5,bordSize.y - 5);
      _fieldMap[x][y]->setPos(Vec3(x * bordSize.x - 1000.f * 0.5f + bordSize.x * 0.5f,00.1f,y * bordSize.y - 1000.f * 0.5f + bordSize.y * 0.5f));
      _fieldMap[x][y]->setRotX(D3DX_PI * 0.5f);
      this->addChild(_fieldMap[x][y]);
    }
  }

  memset(_playerMoveVec,0,sizeof(_playerMoveVec));
  memset(_field,0,sizeof(_field));

  _freezeTime = 0;
  _bultime = 0;

  return true;
}

//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
void Game::update() {
  auto input = App::instance().getInput();
  
  Vec3 moveDest[2] = {
    Vec3(0,0,0),
    Vec3(0,0,0),
  };

  Vec3 playerPos[2] = {
    _player[0]->getPos(),
    _player[1]->getPos(),
  };
  if(_freezeTime == 0) {
    for(int i = 0; i < 2; i++) {
      if(input->isPress(i,VK_INPUT::UP)) {
        moveDest[i].z += 5;
      }
      if(input->isPress(i,VK_INPUT::DOWN)) {
        moveDest[i].z -= 5;
      }
      if(input->isPress(i,VK_INPUT::LEFT)) {
        moveDest[i].x -= 5;
      }
      if(input->isPress(i,VK_INPUT::RIGHT)) {
        moveDest[i].x += 5;
      }

      _playerMoveVec[i] += (moveDest[i] - _playerMoveVec[i]) * 0.1f;
      playerPos[i] += _playerMoveVec[i];

      if(playerPos[i].x < -500) {
        playerPos[i].x = -500 + abs(playerPos[i].x + 500);
        _playerMoveVec[i].x *= -1;
      }
      if(playerPos[i].z < -500) {
        playerPos[i].z = -500 + abs(playerPos[i].z + 500);
        _playerMoveVec[i].z *= -1;
      }
      if(playerPos[i].x > 500) {
        playerPos[i].x = 500 - (playerPos[i].x - 500);
        _playerMoveVec[i].x *= -1;
      }
      if(playerPos[i].z > 500) {
        playerPos[i].z = 500 - (playerPos[i].z - 500);
        _playerMoveVec[i].z *= -1;
      }
      _player[i]->setPos(playerPos[i]);
    }
  }

  
  const int _playerID[2][2] = {
    {int((playerPos[0].x + 500) / (1000 / (float)kNUM_X)),int((playerPos[0].z + 500) / (1000 / (float)kNUM_Y))},
    {int((playerPos[1].x + 500) / (1000 / (float)kNUM_X)),int((playerPos[1].z + 500) / (1000 / (float)kNUM_Y))}
  };
  
  for(int i = 0; i < 2; i++) {
    if(_playerID[i][0] >= 0 && _playerID[i][0] < kNUM_X && _playerID[i][1] >= 0 && _playerID[i][1] < kNUM_Y) {
      if(_field[_playerID[i][0]][_playerID[i][1]] == FIELD_ID::ITEM && input->isPress(i,VK_INPUT::_1)) {
        const int plus = rand() % 4 + 1;
        _field[_playerID[i][0]][_playerID[i][1]] = FIELD_ID(int(FIELD_ID::PLAYER_1) + i);
        _plusNum[i]->setAnimID(plus);
        _num[i] += plus;
        if(_num[i] > 9) _num[i] = 9;
        _numSpriteScl[i] = 2;
        _effect->play("get.efk",playerPos[i]);
      }
      else if(_field[_playerID[i][0]][_playerID[i][1]] != FIELD_ID::ITEM && _field[_playerID[i][0]][_playerID[i][1]] != FIELD_ID(int(FIELD_ID::PLAYER_1) + i) && _num[i] > 0) {
        _field[_playerID[i][0]][_playerID[i][1]] = FIELD_ID(int(FIELD_ID::PLAYER_1) + i);
        _numSpriteScl[i] = 0.7f;
        _num[i] --;
        _effect->play("shot.efk",playerPos[i]);
      }
    }
  }

  if((rand() % (60 * 1)) == 0) {
    _field[rand() % kNUM_X][rand() % kNUM_Y] = FIELD_ID::ITEM;
  }

  for(int x = 0; x < kNUM_X; ++x) {
    for(int y = 0; y < kNUM_X; ++y) {
      switch(_field[x][y]) {
      case FIELD_ID::PLAYER_1:
        _fieldMap[x][y]->setColor(D3DXCOLOR(0,0,1,1));
        break;
      case FIELD_ID::PLAYER_2:
        _fieldMap[x][y]->setColor(D3DXCOLOR(0,0.5f,0,1));
        break;
      case FIELD_ID::ITEM:
        _fieldMap[x][y]->setColor(D3DCOLOR_RGBA(255,183,76,255));
        break;
      case FIELD_ID::NONE:
        _fieldMap[x][y]->setColor(D3DXCOLOR(1,1,1,1));
        break;
      }
    }
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
  float length = D3DXVec3Length(&(playerPos[0] - playerPos[1]));
  const float rot = atan2(300,-900);

  if(length < 300) length = 300;

  _mainCamera->setPosP(Vec3(0,sinf(rot) * length,cosf(rot) * length) + camvec);
  _mainCamera->setPosR(camvec);

  if(_bultime) {
    Vec3 ram(rand() % 10,rand() % 10,0);
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