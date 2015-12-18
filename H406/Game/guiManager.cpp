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
  const D3DXVECTOR2 kTIME_SIZE = D3DXVECTOR2(128.0f, 128.0f) * 0.75f;
  const D3DXVECTOR2 kSTART_TIME_SIZE = D3DXVECTOR2(256.0f, 256.0f);
  const D3DXVECTOR2 kRESULT_NUM_SIZE = D3DXVECTOR2(128.0f * 1.5f, 128.0f * 1.5f) * 1.5f;
  const D3DXVECTOR2 kITEM_SIZE = D3DXVECTOR2(128.0f, 128.0f) * 1.5f;
  const D3DXVECTOR2 kROUND_SIZE = D3DXVECTOR2(64.0f, 64.0f) * 1.2f;
  const D3DXVECTOR2 kOJIOBA_SIZE = D3DXVECTOR2(222, 256) * 1.2f;
  const float kRESULT_NUM_OFFSETX = 300.f * 1.5f;

  const D3DXVECTOR2 kSTRIN_START_SIZE = D3DXVECTOR2(512 * 1.5f, 128 * 1.5f) * 1.5f;
  const D3DXVECTOR2 kSTRIN_FINISH_SIZE = D3DXVECTOR2(512 * 1.5f, 128 * 1.5f) * 1.5f;
  const D3DXVECTOR2 kSTRIN_ROUND_SIZE = D3DXVECTOR2(512 * 1.5f, 128 * 1.5f) * 1.5f;
  const Vec2 kNumSize = Vec2(128.f, 128.f) * 2.0f;
}

//==============================================================================
// init
//------------------------------------------------------------------------------
bool GuiManager::init(EventManager* eventManager)
{
  const D3DXVECTOR2 windowSizeHalf = D3DXVECTOR2(App::instance().getWindowSize().cx * 0.5f, App::instance().getWindowSize().cy * 0.5f);

  memset(_maxDripNum, 0, sizeof(_maxDripNum));
  _isPlay = false;
  _isResult = false;
  _isStart = false;

  // ゲージまとめるやつ
  _gaugeLayer = Sprite2D::create();
  _gaugeLayer->setPos(windowSizeHalf.x, 0.0f);
  _gaugeLayer->setVisible(false);
  this->addChild(_gaugeLayer);

  _gaugeBack = Sprite2D::create("./data/texture/gauge_05.png");
  _gaugeBack->setSize((float)App::instance().getWindowSize().cx, (float)App::instance().getWindowSize().cy);
  _gaugeLayer->addChild(_gaugeBack);

  // ギア
  _gaugeGear = Sprite2D::create("./data/texture/gauge_03.png");
  _gaugeGear->setSize(348.f/* / 1.5f*/, 348.f/* / 1.5f*/);
  _gaugeGear->setPos(0.0f, -_gaugeGear->getScl().y - 100.0f);
  _gaugeLayer->addChild(_gaugeGear);

  const Vec2 kGaugeSize = Vec2(800.f, 300.f)/* / 1.5f*/;
  _gauge[0] = Gauge::create(kGaugeSize.x, kGaugeSize.y);
  _gauge[0]->setPos((kGaugeSize.x * 0.5f) - windowSizeHalf.x, (kGaugeSize.y * 0.5f) - windowSizeHalf.y);
  _gauge[0]->setColor(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
  _gauge[0]->setTexture("./data/texture/gauge_01.png");
  _gaugeLayer->addChild(_gauge[0]);

  _gauge[1] = Gauge::create(kGaugeSize.x, kGaugeSize.y);
  _gauge[1]->setPos(windowSizeHalf.x - kGaugeSize.x * 0.5f, (kGaugeSize.y * 0.5f) - windowSizeHalf.y);
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
  _plus[0]->setSize(kNumSize.x, kNumSize.y);
  _plus[0]->setPos(kNumSize.x, windowSizeHalf.y);
  _plus[0]->setNumU(11);
  _plus[0]->setNumV(1);
  _plus[0]->setAnimID(10);
  _plus[0]->setColor(kPLAYER_COLOR[0]);
  this->addChild(_plus[0]);

  _plus[1] = Sprite2D::create("./data/texture/num2.png");
  _plus[1]->setSize(kNumSize.x, kNumSize.y);
  _plus[1]->setPos(App::instance().getWindowSize().cx - (kNumSize.x * 2.f), windowSizeHalf.y);
  _plus[1]->setNumU(11);
  _plus[1]->setNumV(1);
  _plus[1]->setAnimID(10);
  _plus[1]->setColor(kPLAYER_COLOR[1]);
  this->addChild(_plus[1]);

  _plusNum[0] = NumberSprite::create(1, "./data/texture/num2.png");
  _plusNum[0]->setSize(kNumSize.x, kNumSize.y);
  _plusNum[0]->setPos(kNumSize.x * 2.f, windowSizeHalf.y);
  this->addChild(_plusNum[0]);

  _plusNum[1] = NumberSprite::create(1, "./data/texture/num2.png");
  _plusNum[1]->setSize(kNumSize.x, kNumSize.y);
  _plusNum[1]->setPos(App::instance().getWindowSize().cx - kNumSize.x, windowSizeHalf.y);
  this->addChild(_plusNum[1]);

  _numSpriteScl[0] = _numSpriteScl[1] = 1.f;

  // タイマー
  _time._sprite = NumberSprite::create(2, "./data/texture/num2.png");
  _time._sprite->setSize(kTIME_SIZE.x, kTIME_SIZE.y);
  _time._sprite->setColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
  _time._sprite->setNumber(GameConfig::kONE_ROUND_TIME);
  _time._sprite->setPos(0.0f, -260.0f * 1.5f);
  _time._scl = 1.0f;
  _gaugeLayer->addChild(_time._sprite);

  // ラウンド
  _roundNum._sprite = NumberSprite::create(1, "./data/texture/num2.png");
  _roundNum._sprite->setSize(kROUND_SIZE.x, kROUND_SIZE.y);
  _roundNum._sprite->setColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
  _roundNum._sprite->setNumber(1);
  _roundNum._sprite->setPos(160.0f, -480.0f);
  _roundNum._scl = 1.0f;
  _gaugeLayer->addChild(_roundNum._sprite);

  Vec2 size = Vec2(512.0f, 512.0f) * 0.2f;
  _roundIcon = RoundIcon::create(size.x, size.y);
  _roundIcon->setTexture(0, "./data/texture/win_icon.png");
  _roundIcon->setTexture(1, "./data/texture/win_icon.png");
  this->addChild(_roundIcon);

  // おじさんおばさん
  _oji3._sprite = Sprite2D::create("./data/texture/oji_face.png");
  _oji3._sprite->setSize(kOJIOBA_SIZE.x, kOJIOBA_SIZE.y);
  _oji3._sprite->setPos(kOJIOBA_SIZE.x * 0.5f, App::instance().getWindowSize().cy - kOJIOBA_SIZE.y * 0.5f);
  _oji3._sprite->setAnimID(0);
  _oji3._sprite->setNumU(4);
  _oji3._sprite->setNumV(1);
  _oji3._scl = 1.0f;
  this->addChild(_oji3._sprite);

  _oba3._sprite = Sprite2D::create("./data/texture/oba_face.png");
  _oba3._sprite->setSize(kOJIOBA_SIZE.x, kOJIOBA_SIZE.y);
  _oba3._sprite->setPos(App::instance().getWindowSize().cx - kOJIOBA_SIZE.x * 0.5f, App::instance().getWindowSize().cy - kOJIOBA_SIZE.y * 0.5f);
  _oba3._sprite->setAnimID(0);
  _oba3._sprite->setNumU(4);
  _oba3._sprite->setNumV(1);
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
  _resultNum[0]._sprite->setPos(windowSizeHalf.x - kRESULT_NUM_OFFSETX, windowSizeHalf.y);
  _resultNum[0]._sprite->setVisible(false);
  _resultNum[0]._scl = 1.0f;
  this->addChild(_resultNum[0]._sprite);

  _resultNum[1]._sprite = NumberSprite::create(2, "./data/texture/num2.png");
  _resultNum[1]._sprite->setSize(kRESULT_NUM_SIZE.x, kRESULT_NUM_SIZE.y);
  _resultNum[1]._sprite->setColor(kPLAYER_COLOR[1]);
  _resultNum[1]._sprite->setNumber(0);
  _resultNum[1]._sprite->setPos(windowSizeHalf.x + kRESULT_NUM_OFFSETX, windowSizeHalf.y);
  _resultNum[1]._sprite->setVisible(false);
  _resultNum[1]._scl = 1.0f;
  this->addChild(_resultNum[1]._sprite);

  // [フィニッシュ！]
  _finish._sprite = Sprite2D::create("./data/texture/finish.png");
  _finish._sprite->setSize(kSTRIN_FINISH_SIZE.x, kSTRIN_FINISH_SIZE.y);
  _finish._sprite->setPos(windowSizeHalf.x, windowSizeHalf.y);
  _finish._sprite->setVisible(false);
  _finish._scl = 0.0f;
  this->addChild(_finish._sprite);

  // [スタート！]
  _start._sprite = Sprite2D::create("./data/texture/start.png");
  _start._sprite->setSize(kSTRIN_START_SIZE.x, kSTRIN_START_SIZE.y);
  _start._sprite->setPos(windowSizeHalf.x, windowSizeHalf.y);
  _start._sprite->setVisible(false);
  _start._scl = 0.0f;
  this->addChild(_start._sprite);
  _isStart = false;

  // [ラウンド1]
  _roundStringBack._sprite = Sprite2D::create();
  _roundStringBack._sprite->setSize((float)App::instance().getWindowSize().cx, 350.0f * 1.5f);
  _roundStringBack._sprite->setPos(windowSizeHalf.x, windowSizeHalf.y);
  _roundStringBack._sprite->setColor(D3DXCOLOR(0, 0, 0, 0.5f));
  _roundStringBack._sprite->setVisible(false);
  _roundStringBack._scl = 1.0f;
  this->addChild(_roundStringBack._sprite);

  _roundString._sprite = Sprite2D::create("./data/texture/round1.png");
  _roundString._sprite->setSize(kSTRIN_ROUND_SIZE.x, kSTRIN_ROUND_SIZE.y);
  _roundString._sprite->setPos(windowSizeHalf.x, windowSizeHalf.y);
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
  eventManager->addEventListener(EventList::ROUND_SHOW_BEGIN, bind(&GuiManager::EventListener, this, placeholders::_1));
  eventManager->addEventListener(EventList::ROUND_SHOW_END, bind(&GuiManager::EventListener, this, placeholders::_1));
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
  const D3DXVECTOR2 windowSizeHalf = D3DXVECTOR2(App::instance().getWindowSize().cx * 0.5f, App::instance().getWindowSize().cy * 0.5f);
  const auto deta = DataManager::instance().getData();
  const int playerDripNum[2] = { deta->getPlayerDripNum(0),deta->getPlayerDripNum(1) };

  // 数字
  for (int i = 0; i < 2; i++) {
    _numSpriteScl[i] += (1 - _numSpriteScl[i]) * 0.1f;
    _plusNum[i]->setColor(D3DXCOLOR(kPLAYER_COLOR[i].r, kPLAYER_COLOR[i].g, kPLAYER_COLOR[i].b, min((_numSpriteScl[i] - 1) * 2, 1)));
    _plus[i]->setColor(D3DXCOLOR(kPLAYER_COLOR[i].r, kPLAYER_COLOR[i].g, kPLAYER_COLOR[i].b, min((_numSpriteScl[i] - 1) * 2, 1)));

    _plusNum[i]->setSize(kNumSize.x * _numSpriteScl[i], kNumSize.y * _numSpriteScl[i]);
    _plus[i]->setSize(kNumSize.x * _numSpriteScl[i], kNumSize.y * _numSpriteScl[i]);
  }

  // ゲージ更新
  float gaugeRate[2] = { playerDripNum[0] / (float)_maxDripNum[0], playerDripNum[1] / (float)_maxDripNum[1] };
  _gauge[0]->setRate(gaugeRate[0]);
  _gauge[1]->setRate(gaugeRate[1]);
  // ゲージレイヤー動かす
  Vec2 gaugePos = _gaugeLayer->getPos() + ((windowSizeHalf - _gaugeLayer->getPos()) * 0.07f);
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
      _start._sprite->setSize(kSTRIN_START_SIZE.x *_start._scl, kSTRIN_START_SIZE.y * _start._scl);
    }
    else {
      _start._sprite->setVisible(false);
      _isStart = false;
    }
  }

  // [ラウンド1]の処理
  if (_isPlay == false){
    Vec2 roundPos = _roundString._sprite->getPos();
    Vec2 roundMovePos = (_roundStringPosDest - roundPos) * 0.15f;
    _roundString._sprite->setPos(roundPos + roundMovePos);

    _roundStringBack._scl += (_roundStringSclDest - _roundStringBack._scl) * 0.15f;
    _roundStringBack._sprite->setSize((float)App::instance().getWindowSize().cx, (350.0f * 1.5f) * _roundStringBack._scl);
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

  _finish._scl += (1.0f - _finish._scl) * 0.15f;
  _finish._sprite->setSize(kSTRIN_FINISH_SIZE.x *_finish._scl, kSTRIN_FINISH_SIZE.y * _finish._scl);

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
  const D3DXVECTOR2 windowSizeHalf = D3DXVECTOR2(App::instance().getWindowSize().cx * 0.5f, App::instance().getWindowSize().cy * 0.5f);

  switch (eventData->getEvent()) {
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
    _resultNum[0]._sprite->setPos(-_resultNum[0]._sprite->getScl().x, windowSizeHalf.y);
    _resultNum[1]._sprite->setPos(App::instance().getWindowSize().cx + _resultNum[1]._sprite->getScl().x, windowSizeHalf.y);
    _resultNum[0]._sprite->setSize(kRESULT_NUM_SIZE.x * _resultNumScl[0], kRESULT_NUM_SIZE.y * _resultNumScl[0]);
    _resultNum[1]._sprite->setSize(kRESULT_NUM_SIZE.x * _resultNumScl[1], kRESULT_NUM_SIZE.y * _resultNumScl[1]);
    _resultNumPosDest[0] = Vec2(windowSizeHalf.x - kRESULT_NUM_OFFSETX, windowSizeHalf.y);
    _resultNumPosDest[1] = Vec2(windowSizeHalf.x + kRESULT_NUM_OFFSETX, windowSizeHalf.y);
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

    _roundStringBack._sprite->setVisible(false);
    _roundString._sprite->setVisible(false);
    break;

  case EventList::ROUND_FINISH:
    _finish._sprite->setVisible(true);
    _finish._scl = 0.0f;
    break;

  // [ラウンド1]
  case EventList::ROUND_SHOW_BEGIN:
    _roundStringBack._sprite->setVisible(true);
    _roundString._sprite->setVisible(true);
    if (DataManager::instance().getData()->getRound() == 1){
      _roundString._sprite->setTexture("./data/texture/round1.png");
    } else if (DataManager::instance().getData()->getRound() == 2){
      _roundString._sprite->setTexture("./data/texture/round2.png");
    } else {
      _roundString._sprite->setTexture("./data/texture/round1.png");
    }
    // 文字流す
    _roundString._sprite->setPos(windowSizeHalf.x * 3.0f, windowSizeHalf.y);
    _roundStringPosDest = Vec2(windowSizeHalf.x, windowSizeHalf.y);
    // 黒背景
    _roundStringBack._scl = 0.0f;
    _roundStringSclDest = 1.0f;
    _roundStringBack._sprite->setSize((float)App::instance().getWindowSize().cx, 0.0f);
    break;

  case EventList::ROUND_SHOW_END:
    _roundStringPosDest = Vec2(-windowSizeHalf.x, windowSizeHalf.y);
    _roundStringSclDest = 0.0f;
    break;

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
  }
}


//EOF