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
#include "player.h"
#include "EventList.h"
#include "eventManager.h"
#include "EventData.h"

//==============================================================================
// init
//------------------------------------------------------------------------------
bool ItemManager::init(EventManager* event){
  _event = event;

  memset(_playerList, 0, sizeof(_playerList));
  memset(_bombList, 0, sizeof(_bombList));
  memset(_playerGetItem, 0, sizeof(_playerGetItem));

  // イベントセット
  _event->addEventListener(EventList::PLAYER_1_GET_BOMB, bind(&ItemManager::EventListener, this, placeholders::_1));
  _event->addEventListener(EventList::PLAYER_2_GET_BOMB, bind(&ItemManager::EventListener, this, placeholders::_1));
  _event->addEventListener(EventList::PLAYER_1_USE_ITEM, bind(&ItemManager::EventListener, this, placeholders::_1));
  _event->addEventListener(EventList::PLAYER_2_USE_ITEM, bind(&ItemManager::EventListener, this, placeholders::_1));
  _event->addEventListener(EventList::ITEM_RESET, bind(&ItemManager::EventListener, this, placeholders::_1));

  return true;
}

//==============================================================================
// update
//------------------------------------------------------------------------------
void ItemManager::update(){
  for (int i = 0; i < kBombMax; i++){
    if (_bombList[i] != nullptr){
      if (_bombList[i]->getDeath() == true){
        _bombList[i]->release();
        _bombList[i] = nullptr;
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
void ItemManager::createBomb(const Vec3& pos){
  for (ItemBomb*& _bomb : _bombList) {
    if (_bomb == nullptr) {
      _bomb = ItemBomb::create();
      _bomb->setPos(pos);
      this->addChild(_bomb);
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

  switch (eventData->getEvent()) {

  case EventList::PLAYER_1_GET_BOMB:
    if (_playerGetItem[0] == nullptr){
      _playerGetItem[0] = (ItemBomb*)eventData->getUserData();
      _playerGetItem[0]->setOwnerId(0);
      _playerGetItem[0]->setTargetId(1);
    }
    break;

  case EventList::PLAYER_2_GET_BOMB:
    if (_playerGetItem[1] == nullptr){
      _playerGetItem[1] = (ItemBomb*)eventData->getUserData();
      _playerGetItem[1]->setOwnerId(1);
      _playerGetItem[1]->setTargetId(0);
    }
    break;

  case EventList::PLAYER_1_USE_ITEM:
    {
    if (_playerGetItem[0] == nullptr) break;
    ItemBomb* bomb = (ItemBomb*)_playerGetItem[0];
    bomb->setPos(_playerList[bomb->getOwnerId()]->getPos());
    bomb->setTarget(_playerList[bomb->getTargetId()]);
    bomb->setDripNum(_playerList[bomb->getOwnerId()]->getDripNum() + 1);
    bomb->setUse(true);
    _event->dispatchEvent(EventList(int(EventList::PLAYER_1_DRIP_RESET)), nullptr);
    _playerList[0]->setDripNum(0);
    _playerGetItem[0] = nullptr;
    }
    break;

  case EventList::PLAYER_2_USE_ITEM:
    {
    if (_playerGetItem[1] == nullptr) break;
    ItemBomb* bomb = (ItemBomb*)_playerGetItem[1];
    bomb->setPos(_playerList[bomb->getOwnerId()]->getPos());
    bomb->setTarget(_playerList[bomb->getTargetId()]);
    bomb->setDripNum(_playerList[bomb->getOwnerId()]->getDripNum() + 1);
    bomb->setUse(true);
    _event->dispatchEvent(EventList(int(EventList::PLAYER_2_DRIP_RESET)), nullptr);
    _playerList[1]->setDripNum(0);
    _playerGetItem[1] = nullptr;
    }
    break;

  case EventList::ITEM_RESET:
    {
      for (int i = 0; i < kBombMax; i++){
        if (_bombList[i] != nullptr){
          _bombList[i]->release();
          _bombList[i] = nullptr;
        }
      }
      memset(_playerGetItem, 0, sizeof(_playerGetItem));
    }
    break;
  }
}

//EOF
