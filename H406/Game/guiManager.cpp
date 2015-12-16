//==============================================================================
//
// guiManager[guiManager.cpp]
// Author : masuda masato : 2015/10/23
//
//==============================================================================

//******************************************************************************
// include
//******************************************************************************
#include "guiManager.h"
#include "gauge.h"
#include "../System/numberSprite.h"
#include "roundIcon.h"

#include "eventManager.h"
#include "EventList.h"
#include "EventData.h"
#include "game.h"
#include "dataManager.h"
#include "stage.h"
#include "BaceScene.h"

namespace{
  const D3DXCOLOR kPLAYER_COLOR[2] = { D3DXCOLOR(0.0f, 0.3f, 1.0f, 1.0f), D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f) };
  const D3DXVECTOR2 kTIME_SIZE = D3DXVECTOR2(128.0f, 128.0f) * 0.5f;
  const D3DXVECTOR2 kSTART_TIME_SIZE = D3DXVECTOR2(256.0f, 256.0f);
  const D3DXVECTOR2 kRESULT_NUM_SIZE = D3DXVECTOR2(128.0f * 1.5f, 128.0f * 1.5f);
  const D3DXVECTOR2 kITEM_SIZE = D3DXVECTOR2(128.0f, 128.0f);
  const D3DXVECTOR2 kOJIOBA_SIZE = D3DXVECTOR2(256 , 256) * 0.8f;
}

//==============================================================================
// init
//------------------------------------------------------------------------------
bool GuiManager::init(EventManager* eventManager)
{
  const D3DXVECTOR2 windowCenter = D3DXVECTOR2(App::instance().getWindowSize().cx * 0.5f, App::instance().getWindowSize().cy * 0.5f);

  memset(_maxDripNum, 0, sizeof(_maxDripNum));
  _isPlay = false;
  _isResult = false;
  _isStart = false;

  // ゲージまとめるやつ
  _gaugeLayer = Sprite2D::create();
  _gaugeLayer->setPos(windowCenter.x, 0.0f);
  _gaugeLayer->setVisible(false);
  this->addChild(_gaugeLayer);

  _gaugeBack = Sprite2D::create("./data/texture/gauge_05.png");
  _gaugeBack->setSize((float)App::instance().getWindowSize().cx, (float)App::instance().getWindowSize().cy);
  _gaugeLayer->addChild(_gaugeBack);

  // ギア
  _gaugeGear = Sprite2D::create("./data/texture/gauge_03.png");
  _gaugeGear->setSize(348.f / 1.5f, 348.f / 1.5f);
  _gaugeGear->setPos(0.0f, -280.0f);
  _gaugeLayer->addChild(_gaugeGear);

  const Vec2 kGaugeSize = Vec2(800.f, 300.f) / 1.5f;
  _gauge[0] = Gauge::create(kGaugeSize.x, kGaugeSize.y);
  _gauge[0]->setPos((kGaugeSize.x * 0.5f) - windowCenter.x, (kGaugeSize.y * 0.5f) - windowCenter.y);
  _gauge[0]->setColor(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
  _gauge[0]->setTexture("./data/texture/gauge_01.png");
  _gaugeLayer->addChild(_gauge[0]);

  _gauge[1] = Gauge::create(kGaugeSize.x, kGaugeSize.y);
  _gauge[1]->setPos(windowCenter.x - kGaugeSize.x * 0.5f, (kGaugeSize.y * 0.5f) - windowCenter.y);
  _gauge[1]->setColor(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
  _gauge[1]->setFlip(true);
  _gauge[1]->setTexture("./data/texture/gauge_02.png");
  _gaugeLayer->addChild(_gauge[1]);

  // ゲージ
  _gaugeBase = Sprite2D::create("./data/texture/gauge_00.png");
  _gaugeBase->setColor(D3DXCOLOR(1, 1, 1, 1));
  _gaugeBase->setSize((float)App::instance().getWindowSize().cx, (float)App::instance().getWindowSize().cy);
  _gaugeLayer->addChild(_gaugeBase);

  // 数字
  _plus[0] = Sprite2D::create("./data/texture/num2.png");
  _plus[0]->setSize(128, 128);
  _plus[0]->setPos(128, 328);
  _plus[0]->setNumU(11);
  _plus[0]->setNumV(1);
  _plus[0]->setAnimID(10);
  _plus[0]->setColor(kPLAYER_COLOR[0]);
  this->addChild(_plus[0]);

  _plus[1] = Sprite2D::create("./data/texture/num2.png");
  _plus[1]->setSize(128, 128);
  _plus[1]->setPos(1280 - 255, 328);
  _plus[1]->setNumU(11);
  _plus[1]->setNumV(1);
  _plus[1]->setAnimID(10);
  _plus[1]->setColor(kPLAYER_COLOR[1]);
  this->addChild(_plus[1]);

  _plusNum[0] = NumberSprite::create(1, "./data/texture/num2.png");
  _plusNum[0]->setSize(128, 128);
  _plusNum[0]->setPos(256, 328);
  this->addChild(_plusNum[0]);

  _plusNum[1] = NumberSprite::create(1, "./data/texture/num2.png");
  _plusNum[1]->setSize(128, 128);
  _plusNum[1]->setPos(1280 - 128, 328);
  this->addChild(_plusNum[1]);

  _numSpriteScl[0] = _numSpriteScl[1] = 1.f;

  // タイマー
  _time._sprite = NumberSprite::create(2, "./data/texture/num2.png");
  _time._sprite->setSize(kTIME_SIZE.x, kTIME_SIZE.y);
  _time._sprite->setColor(D3DXCOLOR(1.0f, 0.0f, 0.7f, 1.0f));
  _time._sprite->setNumber(GameConfig::kONE_ROUND_TIME);
  _time._sprite->setPos(0.0f, -260.0f);
  _time._scl = 1.0f;
  _gaugeLayer->addChild(_time._sprite);

  // ラウンド
  _roundNum._sprite = NumberSprite::create(1, "./data/texture/num2.png");
  _roundNum._sprite->setSize(64, 64);
  _roundNum._sprite->setColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
  _roundNum._sprite->setNumber(1);
  _roundNum._sprite->setPos(100.0f, -320.0f);
  _roundNum._scl = 1.0f;
  _gaugeLayer->addChild(_roundNum._sprite);

  Vec2 size = Vec2(512.0f, 512.0f) * 0.1f;
  _roundIcon = RoundIcon::create(size.x, size.y);
  _roundIcon->setTexture(0, "./data/texture/win_icon.png");
  _roundIcon->setTexture(1, "./data/texture/win_icon.png");
  this->addChild(_roundIcon);

  // おじさんおばさん
  _oji3._sprite = Sprite2D::create("./data/texture/oba3_2.png");
  _oji3._sprite->setSize(kOJIOBA_SIZE.x, kOJIOBA_SIZE.y);
  _oji3._sprite->setPos(kOJIOBA_SIZE.x * 0.5f, App::instance().getWindowSize().cy - kOJIOBA_SIZE.y * 0.5f);
  _oji3._scl = 1.0f;
  this->addChild(_oji3._sprite);

  _oba3._sprite = Sprite2D::create("./data/texture/oji3_2.png");
  _oba3._sprite->setSize(kOJIOBA_SIZE.x, kOJIOBA_SIZE.y);
  _oba3._sprite->setPos(App::instance().getWindowSize().cx - kOJIOBA_SIZE.x * 0.5f, App::instance().getWindowSize().cy - kOJIOBA_SIZE.y * 0.5f);
  _oba3._scl = 1.0f;
  this->addChild(_oba3._sprite);

  _ojiobaPosDest[0] = Vec2(kOJIOBA_SIZE.x * 0.5f, App::instance().getWindowSize().cy - kOJIOBA_SIZE.y * 0.5f);
  _ojiobaPosDest[1] = Vec2(App::instance().getWindowSize().cx - kOJIOBA_SIZE.x * 0.5f, App::instance().getWindowSize().cy - kOJIOBA_SIZE.y * 0.5f);
  _oji3._sprite->setPos(-kOJIOBA_SIZE.x, App::instance().getWindowSize().cy - kOJIOBA_SIZE.y * 0.5f);
  _oba3._sprite->setPos(App::instance().getWindowSize().cx + kOJIOBA_SIZE.x, App::instance().getWindowSize().cy - kOJIOBA_SIZE.y * 0.5f);

  // ラウンド結果数字
  _resultNumCount = 0;
  _isResult = false;
  _resultNum[0]._sprite = NumberSprite::create(2, "./data/texture/num2.png");
  _resultNum[0]._sprite->setSize(kRESULT_NUM_SIZE.x, kRESULT_NUM_SIZE.y);
  _resultNum[0]._sprite->setColor(kPLAYER_COLOR[0]);
  _resultNum[0]._sprite->setNumber(0);
  _resultNum[0]._sprite->setPos(windowCenter.x - 300.f, windowCenter.y);
  _resultNum[0]._sprite->setVisible(false);
  _resultNum[0]._scl = 1.0f;
  this->addChild(_resultNum[0]._sprite);

  _resultNum[1]._sprite = NumberSprite::create(2, "./data/texture/num2.png");
  _resultNum[1]._sprite->setSize(kRESULT_NUM_SIZE.x, kRESULT_NUM_SIZE.y);
  _resultNum[1]._sprite->setColor(kPLAYER_COLOR[1]);
  _resultNum[1]._sprite->setNumber(0);
  _resultNum[1]._sprite->setPos(windowCenter.x + 300.f, windowCenter.y);
  _resultNum[1]._sprite->setVisible(false);
  _resultNum[1]._scl = 1.0f;
  this->addChild(_resultNum[1]._sprite);

  // [フィニッシュ！]
  _finish._sprite = Sprite2D::create("./data/texture/finish.png");
  _finish._sprite->setSize(512 * 1.5f, 128 * 1.5f);
  _finish._sprite->setPos(App::instance().getWindowSize().cx * 0.5f, App::instance().getWindowSize().cy * 0.5f);
  _finish._sprite->setVisible(false);
  _finish._scl = 0.0f;
  this->addChild(_finish._sprite);

  // [スタート！]
  _start._sprite = Sprite2D::create("./data/texture/start.png");
  _start._sprite->setSize(512 * 1.5f, 128 * 1.5f);
  _start._sprite->setPos(App::instance().getWindowSize().cx * 0.5f, App::instance().getWindowSize().cy * 0.5f);
  _start._sprite->setVisible(false);
  _start._scl = 0.0f;
  this->addChild(_start._sprite);
  _isStart = false;

  // [ラウンド1]
  _roundString._sprite = Sprite2D::create("./data/texture/start.png");
  _roundString._sprite->setSize(512 * 1.5f, 128 * 1.5f);
  _roundString._sprite->setPos(App::instance().getWindowSize().cx * 0.5f, App::instance().getWindowSize().cy * 0.5f);
  _roundString._sprite->setVisible(false);
  _roundString._scl = 1.0f;
  this->addChild(_roundString._sprite);

  // イベントセット
  eventManager->addEventListener(EventList::PLAYER_1_DRIP_GET,bind(&GuiManager::EventListener,this,placeholders::_1));
  eventManager->addEventListener(EventList::PLAYER_2_DRIP_GET,bind(&GuiManager::EventListener,this,placeholders::_1));
  eventManager->addEventListener(EventList::PLAYER_1_DRIP_USING,bind(&GuiManager::EventListener,this,placeholders::_1));
  eventManager->addEventListener(EventList::PLAYER_2_DRIP_USING,bind(&GuiManager::EventListener,this,placeholders::_1));
  eventManager->addEventListener(EventList::PLAYER_1_ITEM_GET, bind(&GuiManager::EventListener, this, placeholders::_1));
  eventManager->addEventListener(EventList::PLAYER_2_ITEM_GET, bind(&GuiManager::EventListener, this, placeholders::_1));
  eventManager->addEventListener(EventList::PLAYER_1_ROUND_WIN, bind(&GuiManager::EventListener, this, placeholders::_1));
  eventManager->addEventListener(EventList::PLAYER_2_ROUND_WIN, bind(&GuiManager::EventListener, this, placeholders::_1));
  eventManager->addEventListener(EventList::NEXT_ROUND, bind(&GuiManager::EventListener, this, placeholders::_1));
  eventManager->addEventListener(EventList::ROUND_START, bind(&GuiManager::EventListener, this, placeholders::_1));
  eventManager->addEventListener(EventList::ROUND_RESULT, bind(&GuiManager::EventListener, this, placeholders::_1));
  eventManager->addEventListener(EventList::ROUND_RESULT_START, bind(&GuiManager::EventListener, this, placeholders::_1));
  eventManager->addEventListener(EventList::ROUND_RESULT_END, bind(&GuiManager::EventListener, this, placeholders::_1));
  eventManager->addEventListener(EventList::ROUND_FINISH, bind(&GuiManager::EventListener, this, placeholders::_1));
  // アイテム系
  //eventManager->addEventListener(EventList::PLAYER_1_GET_BOMB, bind(&GuiManager::EventListener, this, placeholders::_1));
  //eventManager->addEventListener(EventList::PLAYER_2_GET_BOMB, bind(&GuiManager::EventListener, this, placeholders::_1));
  //eventManager->addEventListener(EventList::PLAYER_1_GET_ACCEL, bind(&GuiManager::EventListener, this, placeholders::_1));
  //eventManager->addEventListener(EventList::PLAYER_2_GET_ACCEL, bind(&GuiManager::EventListener, this, placeholders::_1));
  //eventManager->addEventListener(EventList::PLAYER_1_USE_ITEM, bind(&GuiManager::EventListener, this, placeholders::_1));
  //eventManager->addEventListener(EventList::PLAYER_2_USE_ITEM, bind(&GuiManager::EventListener, this, placeholders::_1));
  //eventManager->addEventListener(EventList::ITEM_RESET, bind(&GuiManager::EventListener, this, placeholders::_1));

  return true;
}

//==============================================================================
// update
//------------------------------------------------------------------------------
void GuiManager::update(void)
{
  const D3DXVECTOR2 windowCenter = D3DXVECTOR2(App::instance().getWindowSize().cx * 0.5f, App::instance().getWindowSize().cy * 0.5f);
  const auto deta = DataManager::instance().getData();
  const int playerDripNum[2] = { deta->getPlayerDripNum(0),deta->getPlayerDripNum(1) };

  // 数字
  for (int i = 0; i < 2; i++) {
    _numSpriteScl[i] += (1 - _numSpriteScl[i]) * 0.1f;
    _plusNum[i]->setColor(D3DXCOLOR(kPLAYER_COLOR[i].r, kPLAYER_COLOR[i].g, kPLAYER_COLOR[i].b, min((_numSpriteScl[i] - 1) * 2, 1)));
    _plus[i]->setColor(D3DXCOLOR(kPLAYER_COLOR[i].r, kPLAYER_COLOR[i].g, kPLAYER_COLOR[i].b, min((_numSpriteScl[i] - 1) * 2, 1)));

    _plusNum[i]->setSize(128 * _numSpriteScl[i], 128 * _numSpriteScl[i]);
    _plus[i]->setSize(128 * _numSpriteScl[i], 128 * _numSpriteScl[i]);
  }

  // ゲージ更新
  float gaugeRate[2] = { playerDripNum[0] / (float)_maxDripNum[0], playerDripNum[1] / (float)_maxDripNum[1] };
  _gauge[0]->setRate(gaugeRate[0]);
  _gauge[1]->setRate(gaugeRate[1]);
  // ゲージレイヤー動かす
  Vec2 gaugePos = _gaugeLayer->getPos() + ((windowCenter - _gaugeLayer->getPos()) * 0.07f);
  _gaugeLayer->setPos(gaugePos);
  _gaugeLayer->setPos(gaugePos);
  // ギア回転
  _gaugeGear->setRot(_gaugeGear->getRot().y + 0.01f);

  // タイマー更新
  int time = DataManager::instance().getData()->getTime();
  if (time < 60){
    _time._sprite->setVisible(false);
  } else {
    _time._sprite->setNumber(time / 60);
  }
  // 終了3秒前カウント
  if (time == 60 * 4 || time == 60 * 3 || time == 60 * 2){
    _time._scl = 2.5f;
  }
  _time._scl += (1 - _time._scl) * 0.07f;
  _time._sprite->setSize(kTIME_SIZE.x * _time._scl, kTIME_SIZE.y * _time._scl);

  // スタート文字
  if (_isStart){
    if (time > (GameConfig::kONE_ROUND_TIME * 60) - (1 * 60)){
      _start._scl += (1.5f - _start._scl) * 0.15f;
      _start._sprite->setSize((512 * 1.5f) *_start._scl, (128 * 1.5f) * _start._scl);
    }
    else {
      _start._sprite->setVisible(false);
      _isStart = false;
    }
  }

  if (_isResult){
    //ラウンド結果のカウントアップ
    _frameCount++;
    if (_frameCount > 2){
      _resultNumCount++;
      _frameCount = 0;
      if (DataManager::instance().getData()->getPlayerMapNum(0) >= _resultNumCount){
        _resultNum[0]._sprite->setNumber(_resultNumCount);
        _resultNum[0]._scl = 1.5f;
      }
      if (DataManager::instance().getData()->getPlayerMapNum(1) >= _resultNumCount){
        _resultNum[1]._sprite->setNumber(_resultNumCount);
        _resultNum[1]._scl = 1.5f;
      }
    }
    _resultNum[0]._scl += (_resultNumScl[0] - _resultNum[0]._scl) * 0.2f;
    _resultNum[0]._sprite->setSize(kRESULT_NUM_SIZE.x *_resultNum[0]._scl, kRESULT_NUM_SIZE.y * _resultNum[0]._scl);
    _resultNum[1]._scl += (_resultNumScl[1] - _resultNum[1]._scl) * 0.2f;
    _resultNum[1]._sprite->setSize(kRESULT_NUM_SIZE.x *_resultNum[1]._scl, kRESULT_NUM_SIZE.y * _resultNum[1]._scl);
  }

  Vec2 rNumPos[2] = { _resultNum[0]._sprite->getPos(), _resultNum[1]._sprite->getPos() };
  Vec2 movePos[2] = { (_resultNumPosDest[0] - rNumPos[0]) * 0.05f, (_resultNumPosDest[1] - rNumPos[1]) * 0.05f };
  _resultNum[0]._sprite->setPos(rNumPos[0] + movePos[0]);
  _resultNum[1]._sprite->setPos(rNumPos[1] + movePos[1]);

  _finish._scl += (1.5f - _finish._scl) * 0.15f;
  _finish._sprite->setSize((512 * 1.5f) *_finish._scl, (128 * 1.5f) * _finish._scl);

  // おじさんおばさん
  if (_isPlay){
    auto _stage = BaceScene::instance()->getStage();
    float oji3num = _stage->getFieldMapNum(Stage::FIELD_ID::PLAYER_1) / (12.f * 12.f);
    float oba3num = _stage->getFieldMapNum(Stage::FIELD_ID::PLAYER_2) / (12.f * 12.f);
    Vec2 oji3vec = Vec2(kOJIOBA_SIZE.x * 0.5f, App::instance().getWindowSize().cy - kOJIOBA_SIZE.y * 0.5f);
    Vec2 oba3vec = Vec2(App::instance().getWindowSize().cx - kOJIOBA_SIZE.x * 0.5f, App::instance().getWindowSize().cy - kOJIOBA_SIZE.y * 0.5f);

    _ojiobaPosDest[0] = oji3vec * (1 - oji3num) + oba3vec * oji3num;
    _ojiobaPosDest[1] = oba3vec * (1 - oba3num) + oji3vec * oba3num;

    if (DataManager::instance().getData()->getTime() < 20 * 60) {
      const float t = (DataManager::instance().getData()->getTime() / 60.f - 20) / 5.f;
      const float y = (App::instance().getWindowSize().cy - (kOJIOBA_SIZE.y * 0.5f)) - (kOJIOBA_SIZE.y)* t;
      _oji3._sprite->setPosY(y);
      _oba3._sprite->setPosY(y);
    }
  }
  Vec2 ojiobaPos[2] = { _oji3._sprite->getPos(), _oba3._sprite->getPos() };
  Vec2 ojiobaMovePos[2] = { (_ojiobaPosDest[0] - ojiobaPos[0]) * 0.05f, (_ojiobaPosDest[1] - ojiobaPos[1]) * 0.05f };
  _oji3._sprite->setPos(ojiobaPos[0] + ojiobaMovePos[0]);
  _oba3._sprite->setPos(ojiobaPos[1] + ojiobaMovePos[1]);
}

//==============================================================================
// uninit
//------------------------------------------------------------------------------
void GuiManager::uninit(void)
{
}

//==============================================================================
// イベント
//------------------------------------------------------------------------------
void GuiManager::EventListener(EventData* eventData) {
  const D3DXVECTOR2 windowCenter = D3DXVECTOR2(App::instance().getWindowSize().cx * 0.5f, App::instance().getWindowSize().cy * 0.5f);

  switch (eventData->getEvent()) {

  // ボム取得
  case EventList::PLAYER_1_GET_BOMB:
    break;
  case EventList::PLAYER_2_GET_BOMB:
    break;
  // アクセル取得
  case EventList::PLAYER_1_GET_ACCEL:
    break;
  case EventList::PLAYER_2_GET_ACCEL:
    break;
  // アイテム使用
  case EventList::PLAYER_1_USE_ITEM:
    break;
  case EventList::PLAYER_2_USE_ITEM:
    break;
  // アイテム表示リセット
  case EventList::ITEM_RESET:
    break;

  // 塗るの取得した
  case EventList::PLAYER_1_DRIP_GET:
    _numSpriteScl[0] = 2.0f;
    _plusNum[0]->setNumber((int)eventData->getUserData());
    break;
  case EventList::PLAYER_2_DRIP_GET:
    _numSpriteScl[1] = 2.0f;
    _plusNum[1]->setNumber((int)eventData->getUserData());
    break;

  // 塗るの使用
  case EventList::PLAYER_1_DRIP_USING:
    break;
  case EventList::PLAYER_2_DRIP_USING:
    break;

  // アイテム取得した
  case EventList::PLAYER_1_ITEM_GET:
    _numSpriteScl[0] = 2.0f;
    _plusNum[0]->setNumber((int)eventData->getUserData());
    break;
  case EventList::PLAYER_2_ITEM_GET:
    _numSpriteScl[1] = 2.0f;
    _plusNum[1]->setNumber((int)eventData->getUserData());
    break;

  // 1ラウンド勝ち
  case EventList::PLAYER_1_ROUND_WIN:
    _roundIcon->setRoundWinNum(0, DataManager::instance().getData()->getPlayerRoundWin(0));
    _resultNumScl[0] = 1.5f;
    break;
  case EventList::PLAYER_2_ROUND_WIN:
    _roundIcon->setRoundWinNum(1, DataManager::instance().getData()->getPlayerRoundWin(1));
    _resultNumScl[1] = 1.5f;
    break;

  case EventList::ROUND_RESULT_START:
    _finish._sprite->setVisible(false);
    _resultNum[0]._sprite->setVisible(true);
    _resultNum[1]._sprite->setVisible(true);
    _resultNum[0]._sprite->setNumber(0);
    _resultNum[1]._sprite->setNumber(0);
    _resultNumScl[0] = 1.0f;
    _resultNumScl[1] = 1.0f;
    _resultNum[0]._sprite->setPos(-200.0f, windowCenter.y);
    _resultNum[1]._sprite->setPos(App::instance().getWindowSize().cx + 200.0f, windowCenter.y);
    _resultNum[0]._sprite->setSize(kRESULT_NUM_SIZE.x * _resultNumScl[0], kRESULT_NUM_SIZE.y * _resultNumScl[0]);
    _resultNum[1]._sprite->setSize(kRESULT_NUM_SIZE.x * _resultNumScl[1], kRESULT_NUM_SIZE.y * _resultNumScl[1]);
    _resultNumPosDest[0] = Vec2(windowCenter.x - 300.f, windowCenter.y);
    _resultNumPosDest[1] = Vec2(windowCenter.x + 300.f, windowCenter.y);
      break;

  case EventList::ROUND_RESULT_END:
    _resultNumPosDest[0] = Vec2(-400.0f, _resultNum[0]._sprite->getPos().y);
    _resultNumPosDest[1] = Vec2(App::instance().getWindowSize().cx + 400.0f, _resultNum[1]._sprite->getPos().y);
    break;

  case EventList::ROUND_RESULT:
    _isResult = true;
    _resultNumCount = 0;
    _frameCount = 0;
    _finish._sprite->setVisible(false);
    break;

  case EventList::NEXT_ROUND:
    _roundNum._sprite->setNumber(DataManager::instance().getData()->getRound());
    _time._sprite->setVisible(true);
    _isResult = false;
    _isPlay = false;
    _resultNum[0]._sprite->setVisible(false);
    _resultNum[1]._sprite->setVisible(false);
    _roundIcon->setMove(false);
    _roundIcon->setPlayerWin(0, false);
    _roundIcon->setPlayerWin(1, false);

    _ojiobaPosDest[0] = Vec2(kOJIOBA_SIZE.x * 0.5f, App::instance().getWindowSize().cy - kOJIOBA_SIZE.y * 0.5f);
    _ojiobaPosDest[1] = Vec2(App::instance().getWindowSize().cx - kOJIOBA_SIZE.x * 0.5f, App::instance().getWindowSize().cy - kOJIOBA_SIZE.y * 0.5f);
    _oji3._sprite->setPos(-kOJIOBA_SIZE.x, App::instance().getWindowSize().cy - kOJIOBA_SIZE.y * 0.5f);
    _oba3._sprite->setPos(App::instance().getWindowSize().cx + kOJIOBA_SIZE.x, App::instance().getWindowSize().cy - kOJIOBA_SIZE.y * 0.5f);
    break;

  case EventList::ROUND_START:
    _isStart = true;
    _isPlay = true;
    _start._sprite->setVisible(true);
    _start._scl = 0.0f;
    break;

  case EventList::ROUND_FINISH:
    _finish._sprite->setVisible(true);
    _finish._scl = 0.0f;
    break;
  }
}


//EOF