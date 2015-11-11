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

namespace{
  const D3DXCOLOR kPLAYER_COLOR[2] = { D3DXCOLOR(0.0f, 0.3f, 1.0f, 1.0f), D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f) };
  const D3DXVECTOR2 kTIME_SIZE = D3DXVECTOR2(128.0f, 128.0f);
}

//==============================================================================
// init
//------------------------------------------------------------------------------
bool GuiManager::init(EventManager* eventManager)
{
  // ゲージ
  _gaugeBase = Sprite2D::create("./data/texture/e.png");
  _gaugeBase->setColor(D3DXCOLOR(1, 1, 1, 1));
  _gaugeBase->setSize((float)App::instance().getWindowSize().cx, (float)App::instance().getWindowSize().cy);
  _gaugeBase->setPos(App::instance().getWindowSize().cx * 0.5f, App::instance().getWindowSize().cy * 0.5f);
  this->addChild(_gaugeBase);

  _gauge[0] = Gauge::create(520.f, 92.6667f);
  _gauge[0]->setPos(Vec2(520.f, 92.6667f) * 0.5f);
  _gauge[0]->setColor(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
  _gauge[0]->setTexture("./data/texture/e1.png");
  this->addChild(_gauge[0]);

  _gauge[1] = Gauge::create(554.f, 100.f);
  _gauge[1]->setPos(App::instance().getWindowSize().cx - 554.f * 0.5f, 100.f * 0.5f);
  _gauge[1]->setColor(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
  _gauge[1]->setFlip(true);
  _gauge[1]->setTexture("./data/texture/e2.png");
  this->addChild(_gauge[1]);

  // 数字
  _numSprite[0] = NumberSprite::create(1, "./data/texture/num.png");
  _numSprite[0]->setSize(128, 128);
  _numSprite[0]->setPos(128, 200);
  _numSprite[0]->setColor(kPLAYER_COLOR[0]);
  _numSprite[0]->setVisible(false);
  this->addChild(_numSprite[0]);

  _numSprite[1] = NumberSprite::create(1, "./data/texture/num.png");
  _numSprite[1]->setSize(128, 128);
  _numSprite[1]->setPos(1280 - 128, 200);
  _numSprite[1]->setColor(kPLAYER_COLOR[1]);
  _numSprite[1]->setVisible(false);
  this->addChild(_numSprite[1]);

  _plus[0] = Sprite2D::create("./data/texture/num.png");
  _plus[0]->setSize(128, 128);
  _plus[0]->setPos(128, 328);
  _plus[0]->setNumU(11);
  _plus[0]->setNumV(2);
  _plus[0]->setAnimID(10);
  _plus[0]->setColor(kPLAYER_COLOR[0]);
  _plus[0]->setVisible(true);
  this->addChild(_plus[0]);

  _plusHighLight[0] = Sprite2D::create("./data/texture/num.png");
  _plusHighLight[0]->setSize(128, 128);
  _plusHighLight[0]->setPos(128, 328);
  _plusHighLight[0]->setNumU(11);
  _plusHighLight[0]->setNumV(2);
  _plusHighLight[0]->setAnimID(20);
  _plusHighLight[0]->setVisible(true);
  this->addChild(_plusHighLight[0]);

  _plus[1] = Sprite2D::create("./data/texture/num.png");
  _plus[1]->setSize(128, 128);
  _plus[1]->setPos(1280 - 255, 328);
  _plus[1]->setNumU(11);
  _plus[1]->setNumV(2);
  _plus[1]->setAnimID(10);
  _plus[1]->setColor(kPLAYER_COLOR[1]);
  _plus[1]->setVisible(true);
  this->addChild(_plus[1]);

  _plusHighLight[1] = Sprite2D::create("./data/texture/num.png");
  _plusHighLight[1]->setSize(128, 128);
  _plusHighLight[1]->setPos(1280 - 255, 328);
  _plusHighLight[1]->setNumU(11);
  _plusHighLight[1]->setNumV(2);
  _plusHighLight[1]->setAnimID(20);
  _plusHighLight[1]->setVisible(true);
  this->addChild(_plusHighLight[1]);

  _plusNum[0] = NumberSprite::create(1, "./data/texture/num.png");
  _plusNum[0]->setSize(128, 128);
  _plusNum[0]->setPos(256, 328);
  _plusNum[0]->setVisible(true);
  this->addChild(_plusNum[0]);

  _plusNum[1] = NumberSprite::create(1, "./data/texture/num.png");
  _plusNum[1]->setSize(128, 128);
  _plusNum[1]->setPos(1280 - 128, 328);
  _plusNum[1]->setVisible(true);
  this->addChild(_plusNum[1]);

  _numSpriteScl[0] = _numSpriteScl[1] = 1.f;

  // タイマー
  _time = NumberSprite::create(2, "./data/texture/num.png");
  _time->setSize(kTIME_SIZE.x, kTIME_SIZE.y);
  _time->setColor(D3DXCOLOR(1.0f, 0.0f, 0.7f, 1.0f));
  _time->setNumber(60);
  _time->setPos(App::instance().getWindowSize().cx * 0.5f, 128.0f);
  this->addChild(_time);
  _timeScl = 1.0f;

  // ラウンド
  _roundNum = NumberSprite::create(1, "./data/texture/num.png");
  _roundNum->setSize(64, 64);
  _roundNum->setColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
  _roundNum->setNumber(1);
  _roundNum->setPos(App::instance().getWindowSize().cx * 0.5f + 32.0f, 32.0f);
  this->addChild(_roundNum);

  _roundIcon = RoundIcon::create(50.0f, 50.0f);
  _roundIcon->setTexture(0, "./data/texture/neko000.png");
  _roundIcon->setTexture(1, "./data/texture/neko000.png");
  this->addChild(_roundIcon);

  // [フィニッシュ！]
  _stringFinish = Sprite2D::create("./data/texture/finish.png");
  _stringFinish->setSize(580 * 1.5f, 150 * 1.5f);
  _stringFinish->setPos(App::instance().getWindowSize().cx * 0.5f, App::instance().getWindowSize().cy * 0.5f);
  _stringFinish->setVisible(false);
  this->addChild(_stringFinish);

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

  return true;
}

//==============================================================================
// update
//------------------------------------------------------------------------------
void GuiManager::update(void)
{
  const auto deta = DataManager::instance().getData();
  const int playerDripNum[2] = {
    deta->getPlayerDripNum(0),deta->getPlayerDripNum(1)
  };

  // 数字
  for (int i = 0; i < 2; i++) {
    if (playerDripNum[i] > 0) {
      _numSprite[i]->setNumber(playerDripNum[i]);
      _numSprite[i]->setVisible(true);
    }
    else {
      _numSprite[i]->setVisible(false);
    }
    _numSpriteScl[i] += (1 - _numSpriteScl[i]) * 0.1f;
    _numSprite[i]->setSize(128 * _numSpriteScl[i], 128 * _numSpriteScl[i]);

    _plusNum[i]->setColor(D3DXCOLOR(kPLAYER_COLOR[i].r, kPLAYER_COLOR[i].g, kPLAYER_COLOR[i].b, min((_numSpriteScl[i] - 1) * 2, 1)));
    _plusNum[i]->setColorHighLight(D3DXCOLOR(1, 1, 1, min((_numSpriteScl[i] - 1) * 2, 1)));
    _plus[i]->setColor(D3DXCOLOR(kPLAYER_COLOR[i].r, kPLAYER_COLOR[i].g, kPLAYER_COLOR[i].b, min((_numSpriteScl[i] - 1) * 2, 1)));
    _plusHighLight[i]->setColor(D3DXCOLOR(1, 1, 1, min((_numSpriteScl[i] - 1) * 2, 1)));

    _plusNum[i]->setSize(128 * _numSpriteScl[i], 128 * _numSpriteScl[i]);
    _plus[i]->setSize(128 * _numSpriteScl[i], 128 * _numSpriteScl[i]);
    _plusHighLight[i]->setSize(128 * _numSpriteScl[i], 128 * _numSpriteScl[i]);
  }

  // ゲージ更新
  float gaugeRate[2] = { playerDripNum[0] / (float)9, playerDripNum[1] / (float)9 };
  _gauge[0]->setRate(gaugeRate[0]);
  _gauge[1]->setRate(gaugeRate[1]);

  // タイマー更新
  int time = DataManager::instance().getData()->getTime();
  if (time < 0){
    _time->setNumber(0);
    _stringFinish->setVisible(true);
  }
  else {
    _time->setNumber(time / 60);
    _stringFinish->setVisible(false);
  }

  if (time == 60 * 4 || time == 60 * 3 || time == 60 * 2){
    _timeScl = 2.5f;
  }
  _timeScl += (1 - _timeScl) * 0.05f;
  _time->setSize(kTIME_SIZE.x * _timeScl, kTIME_SIZE.y * _timeScl);
  if (time > 0){
  } else{
  }
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
    _numSpriteScl[0] = 0.7f;
    break;
  case EventList::PLAYER_2_DRIP_USING:
    _numSpriteScl[1] = 0.7f;
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

    // ラウンド勝ち
  case EventList::PLAYER_1_ROUND_WIN:
    _roundIcon->setRoundWinNum(0, DataManager::instance().getData()->getPlayerRoundWin(0));
    break;
  case EventList::PLAYER_2_ROUND_WIN:
    _roundIcon->setRoundWinNum(1, DataManager::instance().getData()->getPlayerRoundWin(1));
    break;

  case EventList::NEXT_ROUND:
    int round = DataManager::instance().getData()->getRound();
    _roundNum->setNumber(round++);
    break;

  }
}


//EOF