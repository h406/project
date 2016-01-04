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
#include "result.h"
#include "BaceScene.h"
#include "ledConnect.h"
#include "shadow.h"

namespace{
  const Vec2 bordSize = Vec2(1000 / (float)Stage::kNUM_X, 1000 / (float)Stage::kNUM_Y);
  const Vec3 kPLAYER_1_INIT_POS = Vec3(-500 + bordSize.x * 0.5f, 0, 0);
  const Vec3 kPLAYER_2_INIT_POS = Vec3(500 - bordSize.x * 0.5f, 0, 0);
}

//hack test
int nGameLoopFrame = 0;

//------------------------------------------------------------------------------
// init
//------------------------------------------------------------------------------
bool Game::init() {
  auto camera = App::instance().getRenderer()->getCamera();

  // イベントマネージャー
  _eventManager = new EventManager();

  // データマネージャー
  if(!DataManager::instance().isInstance()) {
    DataManager::instance().init();
  }
  DataManager::instance().setEventManager(_eventManager);

  // ステージのリセット
  auto _stage = BaceScene::instance()->getStage();
  _stage->reset(Stage::FIELD_ID::NONE);

  // プレイヤー1
  _player[0] = Player::create(0);
  _player[0]->setPos(kPLAYER_1_INIT_POS);
  _player[0]->moveRight(0.01f);
  this->addChild(_player[0]);

  // プレイヤー2
  _player[1] = Player::create(1);
  _player[1]->setPos(kPLAYER_2_INIT_POS);
  _player[1]->moveLeft(0.01f);
  this->addChild(_player[1]);

  _mainCamera = camera->createCamera();
  _mainCamera->setPosP({0,600,-900});
  _mainCamera->setPosR({0,0,-70});
  camera->setCamera(_mainCamera, 120);

  _playerCam[0] = camera->createCamera();
  _playerCam[0]->setPosP({0,0,0});
  _playerCam[0]->setPosR({0,0,0});

  _playerCam[1] = camera->createCamera();
  _playerCam[1]->setPosP({0,0,0});
  _playerCam[1]->setPosR({0,0,0});

  // アイテム
  _itemManager = ItemManager::create(_eventManager);
  _itemManager->addPlayer(_player[0]);
  _itemManager->addPlayer(_player[1]);
  this->addChild(_itemManager);

  _roundChangeCam = camera->createCamera();
  _roundChangeCam->setPosP({0,1500,0});
  _roundChangeCam->setPosR({ 0, 0, 50 });
  
  // ステージとの当たり判定
  auto hitcheck = ColStage::create(BaceScene::instance()->getStage(),_eventManager);
  hitcheck->addPlayer(_player[0]);
  hitcheck->addPlayer(_player[1]);
  // 最後に処理をさせる
  this->addChild(hitcheck,INT_MAX);

  // プレイヤー同士の当たり判定
  auto hitCheckPlayer =  ColPlayer::create(_eventManager);
  hitCheckPlayer->addPlayer(_player[0]);
  hitCheckPlayer->addPlayer(_player[1]);
  this->addChild(hitCheckPlayer, INT_MAX - 1);

  // プレイヤーとアイテム
  auto hitCheckItem = ColItem::create(BaceScene::instance()->getStage(), _eventManager, _itemManager);
  hitCheckItem->addPlayer(_player[0]);
  hitCheckItem->addPlayer(_player[1]);
  this->addChild(hitCheckItem, INT_MAX - 2);

  // GUIマネージャー
  _guiManger = GuiManager::create(_eventManager);
  _guiManger->setMaxDripNum(0, _player[0]->getMaxDripNum());
  _guiManger->setMaxDripNum(1, _player[1]->getMaxDripNum());
  this->addChild(_guiManger);

  // 影
  auto shadow1_out = Shadow::create();
  shadow1_out->setOwner(_player[0]);
  shadow1_out->setColor(D3DXCOLOR(0.6f, 0.8f, 1.0f, 1));
  shadow1_out->setOffsetY(0.6f);
  this->addChild(shadow1_out);

  auto shadow1_in = Shadow::create();
  shadow1_in->setOwner(_player[0]);
  shadow1_in->setColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.8f));
  shadow1_in->setScl(0.5f, 1.0f, 0.5f);
  shadow1_in->setOffsetY(0.7f);
  this->addChild(shadow1_in);

  auto shadow2_out = Shadow::create();
  shadow2_out->setOwner(_player[1]);
  shadow2_out->setColor(D3DXCOLOR(1.0f, 0.9f, 0.6f, 1));
  shadow2_out->setOffsetY(0.6f);
  this->addChild(shadow2_out);

  auto shadow2_in = Shadow::create();
  shadow2_in->setOwner(_player[1]);
  shadow2_in->setColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.8f));
  shadow2_in->setScl(0.5f, 1.0f, 0.5f);
  shadow2_in->setOffsetY(0.7f);
  this->addChild(shadow2_in);

  // イベントセット
  _eventManager->addEventListener(EventList::PLAYER_1_DRIP_GET, bind(&Game::EventListener,this,placeholders::_1));
  _eventManager->addEventListener(EventList::PLAYER_2_DRIP_GET, bind(&Game::EventListener,this,placeholders::_1));
  _eventManager->addEventListener(EventList::PLAYER_1_DRIP_USING, bind(&Game::EventListener,this,placeholders::_1));
  _eventManager->addEventListener(EventList::PLAYER_2_DRIP_USING, bind(&Game::EventListener,this,placeholders::_1));
  _eventManager->addEventListener(EventList::PLAYER_1_ITEM_GET, bind(&Game::EventListener, this, placeholders::_1));
  _eventManager->addEventListener(EventList::PLAYER_2_ITEM_GET, bind(&Game::EventListener, this, placeholders::_1));
  _eventManager->addEventListener(EventList::PLAYER_1_DRIP_RESET, bind(&Game::EventListener, this, placeholders::_1));
  _eventManager->addEventListener(EventList::PLAYER_2_DRIP_RESET, bind(&Game::EventListener, this, placeholders::_1));

  // サウンドのロード
  // BGM
  App::instance().getSound()->load("./data/sound/bgm/game_main.wav");
  //  App::instance().getSound()->play("./data/sound/bgm/game_main.wav", true);
  App::instance().getSound()->play("./data/sound/bgm/game_main_loop.wav", true);
  // SE
  App::instance().getSound()->load("./data/sound/se/get_item.wav");
  App::instance().getSound()->load("./data/sound/se/paint.wav");
//  App::instance().getSound()->load("./data/sound/se/get_ink.wav");
//  App::instance().getSound()->load("./data/sound/se/supply_ink.wav");
//  App::instance().getSound()->load("./data/sound/se/hurry_up.wav");
//  App::instance().getSound()->load("./data/sound/se/round_finish.wav");
//  App::instance().getSound()->load("./data/sound/se/round_start.wav");
//  App::instance().getSound()->load("./data/sound/se/item_bomb.wav");
//  App::instance().getSound()->load("./data/sound/se/item_beam.wav");
//  App::instance().getSound()->load("./data/sound/se/manhole_ok.wav");
//  App::instance().getSound()->load("./data/sound/se/manhole_ng.wav");
//  App::instance().getSound()->load("./data/sound/se/round_count.wav");

  _freezeTime = 0;
  _bultime = 0;
  _nextModeTime = 220;
  _gameMode = Game::MODE_START;

  // hack
  nGameLoopFrame = 0;

  float gaugeRate = 0.5f;
  BaceScene::instance()->getLedConnect()->sendEvent(LedEvent::MoveGame);
  BaceScene::instance()->getLedConnect()->sendEvent(LedEvent::ShowGauge, &gaugeRate);

  return true;
}

//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
void Game::update() {
  auto _effect = BaceScene::instance()->getEffect();
  char fps[3] = {NULL};
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
  _mainCamera->setPosR(camvec + Vec3(0, 50.0f, 0));

  // データマネージャーのアップデート
  if (_gameMode != MODE_START){
    DataManager::instance().update();
  }

  // hack
  if(nGameLoopFrame >= 0) {
    ++ nGameLoopFrame;
  }


  //---------------------------------------------------------------------------
  // caseの処理
  //---------------------------------------------------------------------------
  switch (_gameMode){
  // スタート時
  case Game::MODE_START:
  {
    _nextModeTime--;
    if (_nextModeTime == 130){
      _eventManager->dispatchEvent(EventList(int(EventList::ROUND_SHOW_BEGIN)), nullptr);
    }else if(_nextModeTime == 50){
      _eventManager->dispatchEvent(EventList(int(EventList::ROUND_SHOW_END)), nullptr);
    }
    // hack
    else if(_nextModeTime <= 0 && (nGameLoopFrame % 324) == 0) {
      _gameMode = Game::MODE_PLAY;
      _eventManager->dispatchEvent(EventList(int(EventList::ROUND_START)), nullptr);
//      App::instance().getSound()->play("./data/sound/se/round_start.wav", false);
      App::instance().getSound()->stop("./data/sound/bgm/game_main_loop.wav");
      App::instance().getSound()->play("./data/sound/bgm/game_main.wav", false);
      nGameLoopFrame = -1;
      _nextModeTime = 0;
    }
  }
  break;

  // ゲーム中
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
      // アイテム使用
      if (input->isTrigger(0, VK_INPUT::_1)) {
        _eventManager->dispatchEvent(EventList(int(EventList::PLAYER_1_USE_ITEM)), nullptr);
      }
      if (input->isTrigger(1, VK_INPUT::_1)) {
        _eventManager->dispatchEvent(EventList(int(EventList::PLAYER_2_USE_ITEM)), nullptr);
      }
    }

    // 開始15秒間は出ないよ
    if (GameConfig::kTIME_MAX - DataManager::instance().getData()->getTime() >= 15 * 60)
    {
      // ボムちゃん生成
      if ((rand() % (100 * 1)) == 0){
        _itemManager->createBomb();
      }
      // スピードアップ生成
      if ((rand() % (120 * 1)) == 0){
        //_itemManager->createAccel();
      }
      // マンホール生成
      if ((rand() % (100 * 1)) == 0){
        _itemManager->createManhole();
      }
    }

    // 塗るマス生成
    if (_nextModeTime == 0){
      if ((rand() % (70 * 1)) == 0) {
        int randx = rand() % Stage::kNUM_X;
        int randy = rand() % Stage::kNUM_Y;
        _stage->setFieldID(randx, randy, Stage::FIELD_ID::DRIP);

        Vec2 fieldSize = Vec2(1000 / (float)Stage::kNUM_X, 1000 / (float)Stage::kNUM_Y);

        int id = _effect->play("stage_lightup.efk", Vec3(randx * fieldSize.x - 500 + fieldSize.x * 0.5f, 0, randy * fieldSize.y - 500 + fieldSize.y * 0.5f));
        _effect->setEffectScl(id, Vec3(50, 50, 50));
      }
    }

    for (int i = 0; i < 2; i++) {
      _playerCam[i]->setPosP(playerPos[i] + Vec3(0, 50, -200));
      _playerCam[i]->setPosR(playerPos[i] + Vec3(0, 10, 0));
    }

    // finish
    if(DataManager::instance().getData()->getTime() == 60){
      const int player_map_num[2] = { _stage->getFieldMapNum(Stage::FIELD_ID::PLAYER_1),
                                      _stage->getFieldMapNum(Stage::FIELD_ID::PLAYER_2) };
      if (player_map_num[0] < player_map_num[1]){
        _mapToTime = 3 * player_map_num[1];
      }else{
        _mapToTime = 3 * player_map_num[0];
      }
      _mapToTime += 400;
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
      App::instance().getSound()->play("./data/sound/se/hoissle.wav", false);
      App::instance().getSound()->play("./data/sound/bgm/game_main_loop.wav", true);
      App::instance().getSound()->stop("./data/sound/bgm/game_main.wav");
      BaceScene::instance()->getLedConnect()->sendEvent(LedEvent::ShowFinish);
      
      // hack
      nGameLoopFrame = 0;
    }
    // 焦らすSE
    if (DataManager::instance().getData()->getTime() == 15 * 60){
//      App::instance().getSound()->play("./data/sound/se/hurry_up.wav", false);
    }

  } // case MODE_PLAY
  break;

  case Game::MODE_ROUND_FINISH:
  {
    _nextModeTime--;
    if(_nextModeTime == _mapToTime - 130){
      auto camera = App::instance().getRenderer()->getCamera();
      camera->setCamera(_roundChangeCam, 120);
      _eventManager->dispatchEvent(EventList(int(EventList::ROUND_RESULT_START)), nullptr);
    }
    if (_nextModeTime == _mapToTime - 250){
      _eventManager->dispatchEvent(EventList(int(EventList::ROUND_RESULT)), nullptr);
      _stage->setPlay(false);
    }
    if (_nextModeTime == 80){
      // 勝敗判定
      const int player_map_num[2] = { _stage->getFieldMapNum(Stage::FIELD_ID::PLAYER_1),
                                      _stage->getFieldMapNum(Stage::FIELD_ID::PLAYER_2) };
      int winner = 0;

      if (player_map_num[0] >= player_map_num[1]){
        _eventManager->dispatchEvent(EventList(int(EventList::PLAYER_1_ROUND_WIN)), nullptr);
      } else {
        _eventManager->dispatchEvent(EventList(int(EventList::PLAYER_2_ROUND_WIN)), nullptr);
        winner = 1;
      }
      BaceScene::instance()->getLedConnect()->sendEvent(LedEvent::ShowRoundWin, &winner);
    }
    if(_nextModeTime == 0){
      const int win[2] = { DataManager::instance().getData()->getPlayerRoundWin(0),
                           DataManager::instance().getData()->getPlayerRoundWin(1) };
      _stage->setPlay(true);

      // ゲーム終わった？
      if (win[0] >= 2 || win[1] >= 2){
        _gameMode = Game::MODE_GAME_END;
      }else{
        const int player_map_num[2] = { _stage->getFieldMapNum(Stage::FIELD_ID::PLAYER_1),
                                        _stage->getFieldMapNum(Stage::FIELD_ID::PLAYER_2) };

        // ステージと塗り数リセット
        if (player_map_num[0] >= player_map_num[1]){
          _stage->reset(Stage::FIELD_ID::PLAYER_1);
        }else{
          _stage->reset(Stage::FIELD_ID::PLAYER_2);
        }

        _eventManager->dispatchEvent(EventList(int(EventList::PLAYER_1_DRIP_RESET)), nullptr);
        _eventManager->dispatchEvent(EventList(int(EventList::PLAYER_2_DRIP_RESET)), nullptr);
        _eventManager->dispatchEvent(EventList(int(EventList::ITEM_RESET)), nullptr);

        _nextModeTime = 120;
        _gameMode = Game::MODE_NEXT_ROUND_SETUP;
        _eventManager->dispatchEvent(EventList(int(EventList::ROUND_RESULT_END)), nullptr);
      }
    }
  } // case MODE_ROUND_FINISH
  break;

  // リセットとか
  case Game::MODE_NEXT_ROUND_SETUP:
  {
    // プレイヤー位置戻す
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
      camera->setCamera(_mainCamera, 120);

      _gameMode = Game::MODE_START;
      _nextModeTime = 240;

      float gaugeRate = 0.5f;
      BaceScene::instance()->getLedConnect()->sendEvent(LedEvent::ShowGauge, &gaugeRate);
    }
  }
  break;

  // ゲーム終了
  case Game::MODE_GAME_END:
  {
    App::instance().getSound()->stop("./data/sound/bgm/game_main.wav");
    App::instance().getSound()->stop("./data/sound/bgm/game_main_loop.wav");
    _eventManager->dispatchEvent(EventList(int(EventList::ITEM_RESET)), nullptr);
    BaceScene::instance()->setCurScene(Result::create());
  }
  break;
  } // switch

//#ifdef _DEBUG
  if(App::instance().getInput()->isTrigger(0,VK_INPUT::_3)) {
    _gameMode = Game::MODE_GAME_END;
  }
//#endif


  //---------------------------------------------------------------------------
  // LEDに送る処理
  //---------------------------------------------------------------------------
  if (_gameMode != MODE_PLAY) return;
  static int showGauge = 1;

  int time = DataManager::instance().getData()->getTime();
//  if (time > 9 * 60){
    const int player_map_num[2] = { _stage->getFieldMapNum(Stage::FIELD_ID::PLAYER_1),
                                    _stage->getFieldMapNum(Stage::FIELD_ID::PLAYER_2) };
    const int map_num_max = player_map_num[0] + player_map_num[1];
    float rate = 0.5f;
    if (map_num_max != 0){
      rate = (float)player_map_num[0] / (float)map_num_max;
    }

    // 1Pの塗ってるマスの割合送る
    if (time % 20 == 0 && showGauge == 1){
      BaceScene::instance()->getLedConnect()->sendEvent(LedEvent::ShowGauge, &rate);
    }
    // 4秒に1回切り替える
    if (time % (4 * 60) == 0 && time != GameConfig::kTIME_MAX){
      if (showGauge > 0){
        // どっちが優勢
        int win = rate > 0.5 ? 0 : (rate == 0.5 ? 3 : 1);
        BaceScene::instance()->getLedConnect()->sendEvent(LedEvent::ShowLead, &win);
      }
      showGauge *= -1;
    }
    //  }else{
//    if (time % 60 == 0){
//      int cur_time = time / 60;
//      BaceScene::instance()->getLedConnect()->sendEvent(LedEvent::ShowSec, &cur_time);
//    }
//  }
}

//==============================================================================
// イベント
//------------------------------------------------------------------------------
void Game::EventListener(EventData* eventData) {
  auto _effect = BaceScene::instance()->getEffect();

  switch(eventData->getEvent()) {
  // 塗るマス踏んだ
  case EventList::PLAYER_1_DRIP_GET:
    {
    int id = _effect->play("DripGetBlue.efk",_player[0]->getPos());
    _effect->setEffectScl(id, Vec3(1.5f, 1.5f, 1.5f));
    }
    break;
  case EventList::PLAYER_2_DRIP_GET:
    {
    int id = _effect->play("DripGetYellow.efk", _player[1]->getPos());
    _effect->setEffectScl(id, Vec3(1.5f, 1.5f, 1.5f));
    }
    break;

  // 使ってない
  case EventList::PLAYER_1_ITEM_GET:
    break;
  case EventList::PLAYER_2_ITEM_GET:
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