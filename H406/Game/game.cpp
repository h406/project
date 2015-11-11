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
#include "guiManager.h"
#include "dataManager.h"

//------------------------------------------------------------------------------
// init
//------------------------------------------------------------------------------
bool Game::init() {
  auto camera = App::instance().getRenderer()->getCamera();
  const Vec2 bordSize = Vec2(1000 / (float)Stage::kNUM_X,1000 / (float)Stage::kNUM_Y);

  // �����Ȃ��X�e�[�W�I�u�W�F�N�g�Q
  auto staticStage = StaticStage::create();
  this->addChild(staticStage);

  // �C�x���g�}�l�[�W���[
  _eventManager = new EventManager();

  // �f�[�^�}�l�[�W���[
  DataManager::instance().init(_eventManager);

  // �v���C���[1
  _player[0] = Player::create(0);
  _player[0]->setPos(Vec3(-500 + bordSize.x * 0.5f,0,0));
  this->addChild(_player[0]);

  // �v���C���[2
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

  _roundChangeCam = camera->createCamera();
  _roundChangeCam->setPosP({0,1500,0});
  _roundChangeCam->setPosR({ 0, 0, 50 });

  _effect = Effect::create();
  _effect->setScl(Vec3(1,1,1));
  this->addChild(_effect);

  // �X�e�[�W�u���b�N
  _stage = Stage::create(1000.f,1000.f);
  this->addChild(_stage);

  // �X�e�[�W�Ƃ̓����蔻��
  auto hitcheck = ColStage::create(_stage,_eventManager);
  hitcheck->addPlayer(_player[0]);
  hitcheck->addPlayer(_player[1]);
  // �Ō�ɏ�����������
  this->addChild(hitcheck,INT_MAX);

  // GUI�}�l�[�W���[
  _guiManger = GuiManager::create(_eventManager);
  this->addChild(_guiManger);

  // �C�x���g�Z�b�g
  _eventManager->addEventListener(EventList::PLAYER_1_DRIP_GET, bind(&Game::EventListener,this,placeholders::_1));
  _eventManager->addEventListener(EventList::PLAYER_2_DRIP_GET, bind(&Game::EventListener,this,placeholders::_1));
  _eventManager->addEventListener(EventList::PLAYER_1_DRIP_USING, bind(&Game::EventListener,this,placeholders::_1));
  _eventManager->addEventListener(EventList::PLAYER_2_DRIP_USING, bind(&Game::EventListener,this,placeholders::_1));
  _eventManager->addEventListener(EventList::PLAYER_1_ITEM_GET, bind(&Game::EventListener, this, placeholders::_1));
  _eventManager->addEventListener(EventList::PLAYER_2_ITEM_GET, bind(&Game::EventListener, this, placeholders::_1));

  _freezeTime = 0;
  _bultime = 0;
  _nextModeTime = 0;
  _gameMode = Game::MODE_PLAY;

  return true;
}

//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
void Game::update() {
  char fps[3];
  sprintf_s(fps, "%d", App::instance().getFps());
  App::instance().setTitle(fps);

  switch (_gameMode){
   // �Q�[����
  case Game::MODE_PLAY:
  {
    auto input = App::instance().getInput();

    Vec3 playerPos[2] = {_player[0]->getPos(), _player[1]->getPos()};

    if (_freezeTime == 0 && _nextModeTime == 0) {
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

    if (_nextModeTime == 0){
      if ((rand() % (60 * 1)) == 0) {
        int randx = rand() % Stage::kNUM_X;
        int randy = rand() % Stage::kNUM_Y;
        _stage->setFieldID(randx, randy, Stage::FIELD_ID::DRIP);

        Vec2 fieldSize = Vec2(1000 / (float)Stage::kNUM_X, 1000 / (float)Stage::kNUM_Y);

        int id = _effect->play("stage_lightup.efk", Vec3(randx * fieldSize.x - 500 + fieldSize.x * 0.5f, 0, randy * fieldSize.y - 500 + fieldSize.y * 0.5f));
        _effect->setEffectScl(id, Vec3(50, 50, 50));
      }
      if ((rand() % (120 * 1)) == 0) {
        int randx = rand() % Stage::kNUM_X;
        int randy = rand() % Stage::kNUM_Y;
        _stage->setFieldID(randx, randy, Stage::FIELD_ID::ITEM);

        Vec2 fieldSize = Vec2(1000 / (float)Stage::kNUM_X, 1000 / (float)Stage::kNUM_Y);

        int id = _effect->play("stage_lightup.efk", Vec3(randx * fieldSize.x - 500 + fieldSize.x * 0.5f, 0, randy * fieldSize.y - 500 + fieldSize.y * 0.5f));
        _effect->setEffectScl(id, Vec3(50, 50, 50));
      }

      if (input->isTrigger(0, VK_INPUT::_2)) {
        _freezeTime = 75;
      }
    }

    if (_freezeTime != 0) {
      switch (_freezeTime) {
      case 75:
        App::instance().getRenderer()->getCamera()->setCamera(_playerCam[0], 30);
        break;

      case 60:
        _effect->play("test2.efk", playerPos[0] + Vec3(0, 20, 0));
        _bultime = 550;
        break;

      case 10:
        App::instance().getRenderer()->getCamera()->setCamera(_mainCamera, 10);
        break;
      }

      _freezeTime--;
    }

    for (int i = 0; i < 2; i++) {
      _playerCam[i]->setPosP(playerPos[i] + Vec3(0, 50, -200));
      _playerCam[i]->setPosR(playerPos[i] + Vec3(0, 10, 0));
    }

    const Vec3 camvec((playerPos[0] + playerPos[1]) * 0.5f);
    const Vec3 playerDir(playerPos[0] - playerPos[1]);
    float length = D3DXVec3Length(&playerDir);
    const float rot = atan2f(300, -900);

    if (length < 300) length = 300;
    if (length >(1201.85042515f/* = sqrtf((8 * 8) + (12 * 12)) * (1000 / (float)Stage::kNUM_X)*/))
      length = (1201.85042515f);

    _mainCamera->setPosP(Vec3(0, sinf(rot) * length, cosf(rot) * length) + camvec);
    _mainCamera->setPosR(camvec);

    if (_bultime) {
      Vec3 ram(float(rand() % 10), float(rand() % 10), 0);
      _mainCamera->setPosP(_mainCamera->getPosP() + ram);
      _mainCamera->setPosR(camvec + ram);
      _bultime--;
    }

    if(DataManager::instance().getData()->getTime() == 60){
      const int player_map_num[2] = { _stage->getFieldMapNum(Stage::FIELD_ID::PLAYER_1),
                                      _stage->getFieldMapNum(Stage::FIELD_ID::PLAYER_2) };
      // ���s����
      if (player_map_num[0] == player_map_num[1]){
        _eventManager->dispatchEvent(EventList(int(EventList::PLAYER_1_ROUND_WIN)), nullptr);
        _eventManager->dispatchEvent(EventList(int(EventList::PLAYER_2_ROUND_WIN)), nullptr);
      } else if (player_map_num[0] > player_map_num[1]){
        _eventManager->dispatchEvent(EventList(int(EventList::PLAYER_1_ROUND_WIN)), nullptr);
      } else {
        _eventManager->dispatchEvent(EventList(int(EventList::PLAYER_2_ROUND_WIN)), nullptr);
      }

      auto camera = App::instance().getRenderer()->getCamera();
      camera->setCamera(_roundChangeCam, 90);

      _nextModeTime = 130;
      _gameMode = Game::MODE_ROUND_CHANGE;
    }

    DataManager::instance().update();

  } // case MODE_PLAY
  break;

  // ���E���h�̍���
  case Game::MODE_ROUND_CHANGE:
  {
    // �ʏ�J�����Ɉڍs
    _nextModeTime--;
    if (_nextModeTime == 0){
      auto camera = App::instance().getRenderer()->getCamera();
      camera->setCamera(_mainCamera, 90);

      _nextModeTime = 120;
      _gameMode = Game::MODE_NEXT_ROUND_SETUP;
    }
  }
  break;

  // ���Z�b�g�Ƃ�
  case Game::MODE_NEXT_ROUND_SETUP:
  {
    _nextModeTime--;
    // ���Z�b�g
    if (_nextModeTime == 0){
      const Vec2 bordSize = Vec2(1000 / (float)Stage::kNUM_X, 1000 / (float)Stage::kNUM_Y);
      _stage->reset();
      _player[0]->setPos(Vec3(-500 + bordSize.x * 0.5f, 0, 0));
      _player[1]->setPos(Vec3(500 - bordSize.x * 0.5f, 0, 0));
      _player[0]->setDripNum(0);
      _player[1]->setDripNum(0);
      _eventManager->dispatchEvent(EventList(int(EventList::NEXT_ROUND)), nullptr);

      _gameMode = Game::MODE_PLAY;
    }
  }
  break;
  } // switch
}

//==============================================================================
// �C�x���g
//------------------------------------------------------------------------------
void Game::EventListener(EventData* eventData) {
  switch(eventData->getEvent()) {
  // �A�C�e���擾����
  case EventList::PLAYER_1_DRIP_GET:
    _effect->play("get.efk",_player[0]->getPos());
    break;

  case EventList::PLAYER_2_DRIP_GET:
    _effect->play("get.efk",_player[1]->getPos());
    break;

  case EventList::PLAYER_1_ITEM_GET:
    
    _effect->play("get.efk", _player[0]->getPos());
    break;

  case EventList::PLAYER_2_ITEM_GET:
    _effect->play("get.efk", _player[1]->getPos());
    break;
  }
}

//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
void Game::uninit() {
  SafeDelete(_eventManager);
  DataManager::instance().uninit();
}

//EOF