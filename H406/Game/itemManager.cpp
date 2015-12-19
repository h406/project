//==============================================================================
//
// itemManager[itemManager.cpp]
// Author : masato masuda : 2015/11/18
//
//==============================================================================

//******************************************************************************
// include
//******************************************************************************
#include "itemManager.h"
#include "iItem.h"
#include "itemBomb.h"
#include "itemAccel.h"
#include "itemManhole.h"
#include "player.h"
#include "EventList.h"
#include "eventManager.h"
#include "EventData.h"
#include "stage.h"
#include "BaceScene.h"
#include "shadow.h"

//==============================================================================
// init
//------------------------------------------------------------------------------
bool ItemManager::init(EventManager* event){
  _event = event;

  memset(_playerList, 0, sizeof(_playerList));
  memset(_bombList, 0, sizeof(_bombList));
  memset(_accelList, 0, sizeof(_accelList));
  memset(_manholeList, 0, sizeof(_manholeList));
  memset(_playerGetItem, 0, sizeof(_playerGetItem));
  memset(_bombShadow, 0, sizeof(_bombShadow));
  memset(_manholeShadow, 0, sizeof(_manholeShadow));
  memset(_field, 0, sizeof(_field));

  // イベントセット
  _event->addEventListener(EventList::PLAYER_1_GET_BOMB, bind(&ItemManager::EventListener, this, placeholders::_1));
  _event->addEventListener(EventList::PLAYER_2_GET_BOMB, bind(&ItemManager::EventListener, this, placeholders::_1));
  _event->addEventListener(EventList::PLAYER_1_GET_ACCEL, bind(&ItemManager::EventListener, this, placeholders::_1));
  _event->addEventListener(EventList::PLAYER_2_GET_ACCEL, bind(&ItemManager::EventListener, this, placeholders::_1));
  _event->addEventListener(EventList::PLAYER_1_GET_MANHOLE, bind(&ItemManager::EventListener, this, placeholders::_1));
  _event->addEventListener(EventList::PLAYER_2_GET_MANHOLE, bind(&ItemManager::EventListener, this, placeholders::_1));
  _event->addEventListener(EventList::PLAYER_1_USE_ITEM, bind(&ItemManager::EventListener, this, placeholders::_1));
  _event->addEventListener(EventList::PLAYER_2_USE_ITEM, bind(&ItemManager::EventListener, this, placeholders::_1));
  _event->addEventListener(EventList::ITEM_RESET, bind(&ItemManager::EventListener, this, placeholders::_1));
  _event->addEventListener(EventList::ROUND_FINISH, bind(&ItemManager::EventListener, this, placeholders::_1));

  return true;
}

//==============================================================================
// update
//------------------------------------------------------------------------------
void ItemManager::update(){
  // ボム
  for (int i = 0; i < ItemManager::kBombMax; i++){
    if (_bombList[i] != nullptr){
      if (_bombList[i]->getDeath() == true){
        iItem::FIELD_ID id = _bombList[i]->getFieldID();
        _field[id.x][id.y] = false;
        _bombList[i]->release();
        _bombList[i] = nullptr;
      }
    }
  }
  // アクセル
  for (int i = 0; i < ItemManager::kAccelMax; i++){
    if (_accelList[i] != nullptr){
      if (_accelList[i]->getDeath() == true){
        iItem::FIELD_ID id = _accelList[i]->getFieldID();
        _field[id.x][id.y] = false;
        _accelList[i]->release();
        _accelList[i] = nullptr;
      }
    }
  }
  // マンホール
  for (int i = 0; i < ItemManager::kManholeMax; i++){
    if (_manholeList[i] != nullptr){
      if (_manholeList[i]->getDeath() == true){
        iItem::FIELD_ID id = _manholeList[i]->getFieldID();
        _field[id.x][id.y] = false;
        _manholeList[i]->release();
        _manholeList[i] = nullptr;
      }
    }
  }
}

//==============================================================================
// uninit
//------------------------------------------------------------------------------
void ItemManager::uninit(){
}

//==============================================================================
// createBomb
//------------------------------------------------------------------------------
void ItemManager::createBomb(){
  auto _effect = BaceScene::instance()->getEffect();
  for (ItemBomb*& _item : _bombList) {
    if (_item == nullptr) {
      int randx = rand() % ItemManager::kNUM_X;
      int randy = rand() % ItemManager::kNUM_Y;

      if (_field[randx][randy] == true) continue;
      _field[randx][randy] = true;
      auto _stage = BaceScene::instance()->getStage();
      const Vec3 pos = _stage->getFieldMapPos(randx, randy);

      _item = ItemBomb::create();
      _item->setPos(pos);
      _item->setFieldID(randx, randy);
      _item->addEventManager(_event);
      this->addChild(_item);

      //auto shadow = Shadow::create();
      //shadow->setOwner((XFileObject*)_item);
      //shadow->setColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.8f));
      //shadow->setOffsetY(1.0f);
      //shadow->setScl(0.5f, 0.5f, 0.5f);
      //this->addChild(shadow);

      _effect->play("ItemPut.efk", pos);
      break;
    }
  }
}

//==============================================================================
// createAccel
//------------------------------------------------------------------------------
void ItemManager::createAccel(){
  auto _effect = BaceScene::instance()->getEffect();
  for (ItemAccel*& _item : _accelList) {
    if (_item == nullptr) {
      int randx = rand() % ItemManager::kNUM_X;
      int randy = rand() % ItemManager::kNUM_Y;

      if (_field[randx][randy] == true) continue;
      _field[randx][randy] = true;
      auto _stage = BaceScene::instance()->getStage();
      const Vec3 pos = _stage->getFieldMapPos(randx, randy);

      _item = ItemAccel::create();
      _item->setPos(pos);
      _item->setFieldID(randx, randy);
      this->addChild(_item);

      _effect->play("ItemPut.efk", pos);
      break;
    }
  }
}

//==============================================================================
// createManhole
//------------------------------------------------------------------------------
void ItemManager::createManhole(){
  auto _effect = BaceScene::instance()->getEffect();
  for (ItemManhole*& _item : _manholeList) {
    if (_item == nullptr) {
      int randx = rand() % ItemManager::kNUM_X;
      int randy = rand() % ItemManager::kNUM_Y;

      if (_field[randx][randy] == true) continue;
      _field[randx][randy] = true;
      auto _stage = BaceScene::instance()->getStage();
      const Vec3 pos = _stage->getFieldMapPos(randx, randy);

      _item = ItemManhole::create();
      _item->setPos(pos);
      _item->addEventManager(_event);
      _item->setFieldID(randx, randy);
      this->addChild(_item);

      //auto shadow = Shadow::create();
      //shadow->setOwner((XFileObject*)_item);
      //shadow->setColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.8f));
      //shadow->setOffsetY(1.0f);
      //shadow->setScl(0.5f, 0.5f, 0.5f);
      //this->addChild(shadow);

      _effect->play("ItemPut.efk", pos);
      break;
    }
  }
}

//==============================================================================
// addPlayer
//------------------------------------------------------------------------------
void ItemManager::addPlayer(Player* player) {
  for (Player*& _player : _playerList) {
    if (_player == nullptr) {
      _player = player;
      break;
    }
  }
}

//==============================================================================
// イベント
//------------------------------------------------------------------------------
void ItemManager::EventListener(EventData* eventData) {
  const D3DXVECTOR2 windowCenter = D3DXVECTOR2(App::instance().getWindowSize().cx * 0.5f, App::instance().getWindowSize().cy * 0.5f);
  auto _effect = BaceScene::instance()->getEffect();
  bool itmeGet[2] = { false, false };

  switch (eventData->getEvent()) {

  // ボム取得
  case EventList::PLAYER_1_GET_BOMB:
    if (_playerGetItem[0] == nullptr){
      _playerGetItem[0] = (ItemBomb*)eventData->getUserData();
      _playerGetItem[0]->setOwner(_playerList[0]);
      _playerGetItem[0]->setTarget(_playerList[1]);
      itmeGet[0] = true;
      _event->dispatchEvent(EventList(int(EventList::PLAYER_1_USE_ITEM)), nullptr);
    }
    break;

  case EventList::PLAYER_2_GET_BOMB:
    if (_playerGetItem[1] == nullptr){
      _playerGetItem[1] = (ItemBomb*)eventData->getUserData();
      _playerGetItem[1]->setOwner(_playerList[1]);
      _playerGetItem[1]->setTarget(_playerList[0]);
      itmeGet[1] = true;
      _event->dispatchEvent(EventList(int(EventList::PLAYER_2_USE_ITEM)), nullptr);
    }
    break;

    // アクセル取得
  case EventList::PLAYER_1_GET_ACCEL:
    if (_playerGetItem[0] == nullptr){
      _playerGetItem[0] = (ItemAccel*)eventData->getUserData();
      _playerGetItem[0]->setOwner(_playerList[0]);
      _playerGetItem[0]->setTarget(_playerList[1]);
      itmeGet[0] = true;
    }
    break;

  case EventList::PLAYER_2_GET_ACCEL:
    if (_playerGetItem[1] == nullptr){
      _playerGetItem[1] = (ItemAccel*)eventData->getUserData();
      _playerGetItem[1]->setOwner(_playerList[1]);
      _playerGetItem[1]->setTarget(_playerList[0]);
      itmeGet[1] = true;
    }
    break;

  // マンホール取得
  case EventList::PLAYER_1_GET_MANHOLE:
    if (_playerGetItem[0] == nullptr){
      _playerGetItem[0] = (ItemManhole*)eventData->getUserData();
      _playerGetItem[0]->setOwner(_playerList[0]);
      _playerGetItem[0]->setTarget(_playerList[1]);
      _event->dispatchEvent(EventList(int(EventList::PLAYER_1_USE_ITEM)), nullptr);
    }
    break;

  case EventList::PLAYER_2_GET_MANHOLE:
    if (_playerGetItem[1] == nullptr){
      _playerGetItem[1] = (ItemManhole*)eventData->getUserData();
      _playerGetItem[1]->setOwner(_playerList[1]);
      _playerGetItem[1]->setTarget(_playerList[0]);
      _event->dispatchEvent(EventList(int(EventList::PLAYER_2_USE_ITEM)), nullptr);
    }
    break;

  // アイテム使用
  case EventList::PLAYER_1_USE_ITEM:
    {
      if (_playerGetItem[0] == nullptr) break;
      _playerGetItem[0]->use();
      _playerGetItem[0] = nullptr;
    }
    break;

  case EventList::PLAYER_2_USE_ITEM:
    {
      if (_playerGetItem[1] == nullptr) break;
      _playerGetItem[1]->use();
      _playerGetItem[1] = nullptr;
    }
    break;

  // アイテムのリセット
  case EventList::ITEM_RESET:
    {
      for (int i = 0; i < ItemManager::kBombMax; i++){
        if (_bombList[i] != nullptr){
          _bombList[i]->effectStop();
          _bombList[i]->release();
          _bombList[i] = nullptr;
        }
      }
      for (int i = 0; i < ItemManager::kAccelMax; i++){
        if (_accelList[i] != nullptr){
            _accelList[i]->release();
            _accelList[i] = nullptr;
        }
      }
      for (int i = 0; i < ItemManager::kManholeMax; i++){
        if (_manholeList[i] != nullptr){
          _manholeList[i]->release();
          _manholeList[i] = nullptr;
        }
      }
      memset(_playerGetItem, 0, sizeof(_playerGetItem));
      memset(_field, 0, sizeof(_field));
    }
    break;

  case EventList::ROUND_FINISH:
    {
      for (int i = 0; i < ItemManager::kBombMax; i++){
        if (_bombList[i] != nullptr){
          _bombList[i]->setHitEnable(false);
        }
      }
    }
    break;
  }

  // エフェクト
  if (itmeGet[0] == true){
    int id = _effect->play("GetItemBlue.efk", _playerList[0]->getPos());
    _effect->setEffectScl(id, Vec3(0.7f, 0.7f, 0.7f));
  }
  if (itmeGet[1] == true){
    int id = _effect->play("GetItemYellow.efk", _playerList[1]->getPos());
    _effect->setEffectScl(id, Vec3(0.7f, 0.7f, 0.7f));
  }

}

//EOF
