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
#include "player.h"
#include "colStage.h"
#include "colPlayer.h"
#include "colItem.h"
#include "eventManager.h"
#include "EventList.h"
#include "EventData.h"
#include "guiManager.h"
#include "dataManager.h"
#include "itemManager.h"
#include "title.h"
#include "BaceScene.h"

namespace{
  const Vec2 bordSize = Vec2(1000 / (float)Stage::kNUM_X, 1000 / (float)Stage::kNUM_Y);
  const Vec3 kPLAYER_1_INIT_POS = Vec3(-500 + bordSize.x * 0.5f, 0, 0);
  const Vec3 kPLAYER_2_INIT_POS = Vec3(500 - bordSize.x * 0.5f, 0, 0);
}

//------------------------------------------------------------------------------
// init
//------------------------------------------------------------------------------
bool Game::init() {
  auto camera = App::instance().getRenderer()->getCamera();

  // �C�x���g�}�l�[�W���[
  _eventManager = new EventManager();

  // �f�[�^�}�l�[�W���[
  if(!DataManager::instance().isInstance()) {
    DataManager::instance().init();
  }
  DataManager::instance().setEventManager(_eventManager);

  // �v���C���[1
  _player[0] = Player::create(0);
  _player[0]->setPos(kPLAYER_1_INIT_POS);
  _player[0]->moveRight(0.01f);
  this->addChild(_player[0]);

  // �v���C���[2
  _player[1] = Player::create(1);
  _player[1]->setPos(kPLAYER_2_INIT_POS);
  _player[1]->moveLeft(0.01f);
  this->addChild(_player[1]);

  _mainCamera = camera->createCamera();
  _mainCamera->setPosP({0,600,-900});
  _mainCamera->setPosR({0,0,-70});
  camera->setCamera(_mainCamera, 100);

  _playerCam[0] = camera->createCamera();
  _playerCam[0]->setPosP({0,0,0});
  _playerCam[0]->setPosR({0,0,0});

  _playerCam[1] = camera->createCamera();
  _playerCam[1]->setPosP({0,0,0});
  _playerCam[1]->setPosR({0,0,0});

  // �A�C�e��
  _itemManager = ItemManager::create(_eventManager);
  _itemManager->addPlayer(_player[0]);
  _itemManager->addPlayer(_player[1]);
  this->addChild(_itemManager);

  _roundChangeCam = camera->createCamera();
  _roundChangeCam->setPosP({0,1500,0});
  _roundChangeCam->setPosR({ 0, 0, 50 });
  
  // �X�e�[�W�Ƃ̓����蔻��
  auto hitcheck = ColStage::create(BaceScene::instance()->getStage(),_eventManager);
  hitcheck->addPlayer(_player[0]);
  hitcheck->addPlayer(_player[1]);
  // �Ō�ɏ�����������
  this->addChild(hitcheck,INT_MAX);

  // �v���C���[���m�̓����蔻��
  auto hitCheckPlayer =  ColPlayer::create(_eventManager);
  hitCheckPlayer->addPlayer(_player[0]);
  hitCheckPlayer->addPlayer(_player[1]);
  this->addChild(hitCheckPlayer, INT_MAX - 1);

  // �v���C���[�ƃA�C�e��
  auto hitCheckItem = ColItem::create(BaceScene::instance()->getStage(), _eventManager, _itemManager);
  hitCheckItem->addPlayer(_player[0]);
  hitCheckItem->addPlayer(_player[1]);
  this->addChild(hitCheckItem, INT_MAX - 2);

  // GUI�}�l�[�W���[
  _guiManger = GuiManager::create(_eventManager);
  _guiManger->setMaxDripNum(0, _player[0]->getMaxDripNum());
  _guiManger->setMaxDripNum(1, _player[1]->getMaxDripNum());
  this->addChild(_guiManger);

  // �C�x���g�Z�b�g
  _eventManager->addEventListener(EventList::PLAYER_1_DRIP_GET, bind(&Game::EventListener,this,placeholders::_1));
  _eventManager->addEventListener(EventList::PLAYER_2_DRIP_GET, bind(&Game::EventListener,this,placeholders::_1));
  _eventManager->addEventListener(EventList::PLAYER_1_DRIP_USING, bind(&Game::EventListener,this,placeholders::_1));
  _eventManager->addEventListener(EventList::PLAYER_2_DRIP_USING, bind(&Game::EventListener,this,placeholders::_1));
  _eventManager->addEventListener(EventList::PLAYER_1_ITEM_GET, bind(&Game::EventListener, this, placeholders::_1));
  _eventManager->addEventListener(EventList::PLAYER_2_ITEM_GET, bind(&Game::EventListener, this, placeholders::_1));
  _eventManager->addEventListener(EventList::PLAYER_1_DRIP_RESET, bind(&Game::EventListener, this, placeholders::_1));
  _eventManager->addEventListener(EventList::PLAYER_2_DRIP_RESET, bind(&Game::EventListener, this, placeholders::_1));

  _freezeTime = 0;
  _bultime = 0;
  _nextModeTime = 120;
  _gameMode = Game::MODE_START;

  // �T�E���h�̃��[�h
  // BGM
  App::instance().getSound()->load("./data/sound/bgm/game_main.wav");
  // SE
  App::instance().getSound()->load("./data/sound/se/get_item.wav");
  App::instance().getSound()->load("./data/sound/se/paint.wav");
  App::instance().getSound()->load("./data/sound/se/get_ink.wav");
  App::instance().getSound()->load("./data/sound/se/supply_ink.wav");
  App::instance().getSound()->load("./data/sound/se/hurry_up.wav");
  App::instance().getSound()->load("./data/sound/se/round_finish.wav");
  App::instance().getSound()->load("./data/sound/se/round_start.wav");
  App::instance().getSound()->load("./data/sound/se/item_bomb.wav");
  App::instance().getSound()->load("./data/sound/se/item_beam.wav");
  App::instance().getSound()->load("./data/sound/se/manhole_ok.wav");
  App::instance().getSound()->load("./data/sound/se/manhole_ng.wav");
  App::instance().getSound()->load("./data/sound/se/round_count.wav");

  return true;
}

//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
void Game::update() {
  auto _effect = BaceScene::instance()->getEffect();
  char fps[3];
  sprintf_s(fps, "%d", App::instance().getFps());
  App::instance().setTitle(fps);

  auto input = App::instance().getInput();
  auto _stage = BaceScene::instance()->getStage();

  Vec3 playerPos[2] = {
    _player[0]->getPos(),
    _player[1]->getPos(),
  };

  // camera
  const Vec3 camvec((playerPos[0] + playerPos[1]) * 0.5f);
  const Vec3 playerDir(playerPos[0] - playerPos[1]);
  float length = D3DXVec3Length(&playerDir);
  const float rot = atan2f(300, -900);

  if (length < 300) length = 300;
  if (length >(1201.85042515f/* = sqrtf((8 * 8) + (12 * 12)) * (1000 / (float)Stage::kNUM_X)*/))
    length = (1201.85042515f);

  _mainCamera->setPosP(Vec3(0, sinf(rot) * length, cosf(rot) * length) + camvec);
  _mainCamera->setPosR(camvec);

  switch (_gameMode){
  // �X�^�[�g��
  case Game::MODE_START:
  {
    _nextModeTime--;
    if (_nextModeTime == 0){
      _gameMode = Game::MODE_PLAY;
      _eventManager->dispatchEvent(EventList(int(EventList::ROUND_START)), nullptr);
      App::instance().getSound()->play("./data/sound/se/round_start.wav", false);
    }
  }
  break;

  // �Q�[����
  case Game::MODE_PLAY:
  {
    if (_freezeTime == 0 && _nextModeTime == 0) {
      for (int i = 0; i < 2; i++) {
        if (input->isPress(i, VK_INPUT::UP)) {
          _player[i]->moveUp(1.0f);
        }
        if (input->isPress(i, VK_INPUT::DOWN)) {
          _player[i]->moveDown(1.0f);
        }
        if (input->isPress(i, VK_INPUT::LEFT)) {
          _player[i]->moveLeft(1.0f);
        }
        if (input->isPress(i, VK_INPUT::RIGHT)) {
          _player[i]->moveRight(1.0f);
        }
      }
      // �A�C�e���g�p
      if (input->isTrigger(0, VK_INPUT::_1)) {
        _eventManager->dispatchEvent(EventList(int(EventList::PLAYER_1_USE_ITEM)), nullptr);
      }
      if (input->isTrigger(1, VK_INPUT::_1)) {
        _eventManager->dispatchEvent(EventList(int(EventList::PLAYER_2_USE_ITEM)), nullptr);
      }
    }

    // �{������񐶐�
    if ((rand() % (60 * 1)) == 0){
      int randx = rand() % Stage::kNUM_X;
      int randy = rand() % Stage::kNUM_Y;
      const Vec3 pos = _stage->getFieldMapPos(randx, randy);
      _itemManager->createBomb(pos);
    }
    // �X�s�[�h�A�b�v����
    if ((rand() % (60 * 1)) == 0){
      int randx = rand() % Stage::kNUM_X;
      int randy = rand() % Stage::kNUM_Y;
      //const Vec3 pos = _stage->getFieldMapPos(randx, randy);
      //_itemManager->createAccel(pos);
    }
    // �}���z�[������
    if ((rand() % (60 * 1)) == 0){
      int randx = rand() % Stage::kNUM_X;
      int randy = rand() % Stage::kNUM_Y;
      const Vec3 pos = _stage->getFieldMapPos(randx, randy);
      _itemManager->createManhole(pos);
    }

    // �h��}�X����
    if (_nextModeTime == 0){
      if ((rand() % (60 * 1)) == 0) {
        int randx = rand() % Stage::kNUM_X;
        int randy = rand() % Stage::kNUM_Y;
        _stage->setFieldID(randx, randy, Stage::FIELD_ID::DRIP);

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

    if (_bultime) {
      Vec3 ram(float(rand() % 10), float(rand() % 10), 0);
      _mainCamera->setPosP(_mainCamera->getPosP() + ram);
      _mainCamera->setPosR(camvec + ram);
      _bultime--;
    }

    if(DataManager::instance().getData()->getTime() == 60){
      const int player_map_num[2] = { _stage->getFieldMapNum(Stage::FIELD_ID::PLAYER_1),
                                      _stage->getFieldMapNum(Stage::FIELD_ID::PLAYER_2) };
      if (player_map_num[0] < player_map_num[1]){
        _mapToTime = 3 * player_map_num[1];
      }else{
        _mapToTime = 3 * player_map_num[0];
      }
      _mapToTime += 350;
      _nextModeTime = _mapToTime;
      _gameMode = Game::MODE_ROUND_FINISH;
      _eventManager->dispatchEvent(EventList(int(EventList::ROUND_FINISH)), nullptr);
      _eventManager->dispatchEvent(EventList(int(EventList::PLAYER_1_MAP_SET)), (void*)player_map_num[0]);
      _eventManager->dispatchEvent(EventList(int(EventList::PLAYER_2_MAP_SET)), (void*)player_map_num[1]);
      _eventManager->dispatchEvent(EventList(int(EventList::PLAYER_1_DRIP_RESET)), nullptr);
      _eventManager->dispatchEvent(EventList(int(EventList::PLAYER_2_DRIP_RESET)), nullptr);
      _player[0]->setDripNum(0);
      _player[1]->setDripNum(0);
      _player[0]->setHitEnable(false);
      _player[1]->setHitEnable(false);
      App::instance().getSound()->play("./data/sound/se/round_finish.wav", false);
    }
    // �ł炷SE
    if (DataManager::instance().getData()->getTime() == 14 * 60){
      App::instance().getSound()->play("./data/sound/se/hurry_up.wav", false);
    }

  } // case MODE_PLAY
  break;

  case Game::MODE_ROUND_FINISH:
  {
    _nextModeTime--;
    if(_nextModeTime == _mapToTime - 100){
      auto camera = App::instance().getRenderer()->getCamera();
      camera->setCamera(_roundChangeCam, 90);
      _eventManager->dispatchEvent(EventList(int(EventList::ROUND_RESULT_START)), nullptr);
    }
    if (_nextModeTime == _mapToTime - 200){
      _eventManager->dispatchEvent(EventList(int(EventList::ROUND_RESULT)), nullptr);
    }
    if (_nextModeTime == 80){
      // ���s����
      const int player_map_num[2] = { _stage->getFieldMapNum(Stage::FIELD_ID::PLAYER_1),
                                      _stage->getFieldMapNum(Stage::FIELD_ID::PLAYER_2) };
      if (player_map_num[0] == player_map_num[1]){
        _eventManager->dispatchEvent(EventList(int(EventList::PLAYER_1_ROUND_WIN)), nullptr);
        _eventManager->dispatchEvent(EventList(int(EventList::PLAYER_2_ROUND_WIN)), nullptr);
      } else if (player_map_num[0] > player_map_num[1]){
        _eventManager->dispatchEvent(EventList(int(EventList::PLAYER_1_ROUND_WIN)), nullptr);
      } else {
        _eventManager->dispatchEvent(EventList(int(EventList::PLAYER_2_ROUND_WIN)), nullptr);
      }
    }
    if(_nextModeTime == 0){
      // �X�e�[�W�Ɠh�萔���Z�b�g
      _stage->reset();
      _eventManager->dispatchEvent(EventList(int(EventList::PLAYER_1_DRIP_RESET)), nullptr);
      _eventManager->dispatchEvent(EventList(int(EventList::PLAYER_2_DRIP_RESET)), nullptr);
      _eventManager->dispatchEvent(EventList(int(EventList::ITEM_RESET)), nullptr);

      _nextModeTime = 120;
      _gameMode = Game::MODE_NEXT_ROUND_SETUP;
      _eventManager->dispatchEvent(EventList(int(EventList::ROUND_RESULT_END)), nullptr);
    }
  } // case MODE_ROUND_FINISH
  break;

  // ���Z�b�g�Ƃ�
  case Game::MODE_NEXT_ROUND_SETUP:
  {
    // �v���C���[�ʒu�߂�
    Vec3 playerPosMove[2] = { Vec3((kPLAYER_1_INIT_POS - playerPos[0]) * 0.05f),
                              Vec3((kPLAYER_2_INIT_POS - playerPos[1]) * 0.05f) };
    Vec3 playerPosDest[2] = { _player[0]->getPos() + playerPosMove[0],
                              _player[1]->getPos() + playerPosMove[1]};
    _player[0]->setPos(playerPosDest[0]);
    _player[1]->setPos(playerPosDest[1]);

    _nextModeTime--;
    if (_nextModeTime == 0){
      _player[0]->setPos(kPLAYER_1_INIT_POS);
      _player[1]->setPos(kPLAYER_2_INIT_POS);
      _player[0]->moveRight(0.01f);
      _player[1]->moveLeft(0.01f);
      _player[0]->setHitEnable(true);
      _player[1]->setHitEnable(true);
      _eventManager->dispatchEvent(EventList(int(EventList::NEXT_ROUND)), nullptr);

      auto camera = App::instance().getRenderer()->getCamera();
      camera->setCamera(_mainCamera, 90);

      _gameMode = Game::MODE_START;
      _nextModeTime = 120;
    }
  }
  break;
  } // switch

  if(App::instance().getInput()->isTrigger(0,VK_INPUT::_3)) {
//    BaceScene::instance()->setCurScene(Title::create());
  }

  if (_gameMode != MODE_START){
    DataManager::instance().update();
  }
}

//==============================================================================
// �C�x���g
//------------------------------------------------------------------------------
void Game::EventListener(EventData* eventData) {
  auto _effect = BaceScene::instance()->getEffect();

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

  case EventList::PLAYER_1_DRIP_RESET:
    _player[0]->setDripNum(0);
    break;

  case EventList::PLAYER_2_DRIP_RESET:
    _player[1]->setDripNum(0);
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