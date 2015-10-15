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

  auto backBG = MeshCylinder::create(60,10,30.f,30.f);
  backBG->setColor(D3DXCOLOR(1,1,1,1));
  backBG->setTexture("./data/texture/akira001.png");
  this->addChild(backBG);

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
  _effect->setScl(Vec3(10,10,10));
  this->addChild(_effect);

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

  return true;
}

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
  
  if(input->isPress(1,VK_INPUT::UP)) {
    moveDest[1].z += 5;
  }
  if(input->isPress(1,VK_INPUT::DOWN)) {
    moveDest[1].z -= 5;
  }
  if(input->isPress(1,VK_INPUT::LEFT)) {
    moveDest[1].x -= 5;
  }
  if(input->isPress(1,VK_INPUT::RIGHT)) {
    moveDest[1].x += 5;
  }

  if(input->isPress(0,VK_INPUT::UP)) {
    moveDest[0].z += 5;
  }
  if(input->isPress(0,VK_INPUT::DOWN)) {
    moveDest[0].z -= 5;
  }
  if(input->isPress(0,VK_INPUT::LEFT)) {
    moveDest[0].x -= 5;
  }
  if(input->isPress(0,VK_INPUT::RIGHT)) {
    moveDest[0].x += 5;
  }

  _playerMoveVec[0] += (moveDest[0] - _playerMoveVec[0]) * 0.1f;
  _playerMoveVec[1] += (moveDest[1] - _playerMoveVec[1]) * 0.1f;

  playerPos[0] += _playerMoveVec[0];
  playerPos[1] += _playerMoveVec[1];

  for(int i = 0; i < 2; i++) {
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
  }

  _player[0]->setPos(playerPos[0]);
  _player[1]->setPos(playerPos[1]);
  
  const int _playerID[2][2] = {
    {int((playerPos[0].x + 500) / (1000 / (float)kNUM_X)),int((playerPos[0].z + 500) / (1000 / (float)kNUM_Y))},
    {int((playerPos[1].x + 500) / (1000 / (float)kNUM_X)),int((playerPos[1].z + 500) / (1000 / (float)kNUM_Y))}
  };
  
  if(_playerID[0][0] >= 0 && _playerID[0][0] < kNUM_X && _playerID[0][1] >= 0 && _playerID[0][1] < kNUM_Y) {
    _field[_playerID[0][0]][_playerID[0][1]] = FIELD_ID::PLAYER_1;
  }

  if(_playerID[1][0] >= 0 && _playerID[1][0] < kNUM_X && _playerID[1][1] >= 0 && _playerID[1][1] < kNUM_Y) {
    _field[_playerID[1][0]][_playerID[1][1]] = FIELD_ID::PLAYER_2;
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
      }
    }
  }

  const Vec3 camvec((playerPos[0] + playerPos[1]) * 0.5f);
  float length = D3DXVec3Length(&(playerPos[0] - playerPos[1]));
  const float rot = 2.55359006;//atan2(600,-900);

  if(length < 300) length = 300;

  _mainCamera->setPosP(Vec3(0,sinf(rot) * length,cosf(rot) * length) + camvec);
  _mainCamera->setPosR(camvec);
  
 
}

void Game::uninit() {

}

//EOF