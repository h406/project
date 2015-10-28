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
#include "eventManager.h"
#include "EventList.h"
#include "EventData.h"

#include "game.h"

//==============================================================================
// init
//------------------------------------------------------------------------------
bool GuiManager::init(void)
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

  _numSprite[0] = Sprite2D::create("./data/texture/num.png");
  _numSprite[0]->setSize(128, 128);
  _numSprite[0]->setPos(128, 200);
  _numSprite[0]->setNumU(11);
  _numSprite[0]->setAnimID(10);
  _numSprite[0]->setVisible(false);
  this->addChild(_numSprite[0]);

  // 数字
  _numSprite[1] = Sprite2D::create("./data/texture/num.png");
  _numSprite[1]->setSize(128, 128);
  _numSprite[1]->setPos(1280 - 128, 200);
  _numSprite[1]->setNumU(11);
  _numSprite[1]->setAnimID(10);
  _numSprite[1]->setVisible(false);
  this->addChild(_numSprite[1]);

  _plus[0] = Sprite2D::create("./data/texture/num.png");
  _plus[0]->setSize(128, 128);
  _plus[0]->setPos(128, 328);
  _plus[0]->setNumU(11);
  _plus[0]->setAnimID(10);
  _plus[0]->setVisible(true);
  _plus[0]->setColor(D3DXCOLOR(0, 0, 0, 0));
  this->addChild(_plus[0]);

  _plus[1] = Sprite2D::create("./data/texture/num.png");
  _plus[1]->setSize(128, 128);
  _plus[1]->setPos(1280 - 255, 328);
  _plus[1]->setNumU(11);
  _plus[1]->setAnimID(10);
  _plus[1]->setVisible(true);
  _plus[1]->setColor(D3DXCOLOR(0, 0, 0, 0));
  this->addChild(_plus[1]);

  _plusNum[0] = Sprite2D::create("./data/texture/num.png");
  _plusNum[0]->setSize(128, 128);
  _plusNum[0]->setPos(256, 328);
  _plusNum[0]->setNumU(11);
  _plusNum[0]->setAnimID(10);
  _plusNum[0]->setVisible(true);
  _plusNum[0]->setColor(D3DXCOLOR(0, 0, 0, 0));
  this->addChild(_plusNum[0]);

  _plusNum[1] = Sprite2D::create("./data/texture/num.png");
  _plusNum[1]->setSize(128, 128);
  _plusNum[1]->setPos(1280 - 128, 328);
  _plusNum[1]->setNumU(11);
  _plusNum[1]->setAnimID(10);
  _plusNum[1]->setVisible(true);
  _plusNum[1]->setColor(D3DXCOLOR(0, 0, 0, 0));
  this->addChild(_plusNum[1]);

  _numSpriteScl[0] = _numSpriteScl[1] = 1.f;

//  eventManager->addEventListener(EventList::PLAYER_1_ITEM_GET, bind(&GuiManager::EventListener, this, placeholders::_1));

  return true;
}

//==============================================================================
// update
//------------------------------------------------------------------------------
void GuiManager::update(void)
{
  Game* game = (Game*)iScene::getParent();

  int playerDripNum[2] = {
    game->getPlayerDripNum(0), game->getPlayerDripNum(1)
  };

  // 数字
  for (int i = 0; i < 2; i++) {
    if (playerDripNum[i] > 0) {
      _numSprite[i]->setAnimID(playerDripNum[i]);
      _numSprite[i]->setVisible(true);
    }
    else {
      _numSprite[i]->setVisible(false);
    }
    _numSpriteScl[i] += (1 - _numSpriteScl[i]) * 0.1f;
    _numSprite[i]->setSize(128 * _numSpriteScl[i], 128 * _numSpriteScl[i]);

    _plusNum[i]->setColor(D3DXCOLOR(1, 1, 1, min((_numSpriteScl[i] - 1) * 2, 1)));
    _plus[i]->setColor(D3DXCOLOR(1, 1, 1, min((_numSpriteScl[i] - 1) * 2, 1)));
    _plusNum[i]->setSize(128 * _numSpriteScl[i], 128 * _numSpriteScl[i]);
    _plus[i]->setSize(128 * _numSpriteScl[i], 128 * _numSpriteScl[i]);
  }

  // ゲージ更新
  float gaugeRate[2] = { playerDripNum[0] / (float)9, playerDripNum[1] / (float)9 };
  _gauge[0]->setRate(gaugeRate[0]);
  _gauge[1]->setRate(gaugeRate[1]);

}

//==============================================================================
// uninit
//------------------------------------------------------------------------------
void GuiManager::uninit(void)
{
}

//==============================================================================
// setPlusNumAnimID
//------------------------------------------------------------------------------
void GuiManager::setPlusNumAnimID(int playerId, int animId){
  _plusNum[playerId]->setAnimID(animId);
}

//void GuiManager::EventListener(EventData* eventData)
//{
//}


//EOF