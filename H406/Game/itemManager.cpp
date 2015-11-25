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

//==============================================================================
// init
//------------------------------------------------------------------------------
bool ItemManager::init(){
  memset(_playerList, 0, sizeof(_playerList));
  memset(_bombList, 0, sizeof(_bombList));
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

  //for (auto obj : _itemList) {
  //  if (obj->getDeath() == true){
  //    auto remove = [obj](iItem* item){return obj == item; };
  //    auto it = remove_if(_itemList.begin(), _itemList.end(), remove);
  //    _itemList.erase(it);
  //    obj->release();
  //  }
  //}
}

//==============================================================================
// uninit
//------------------------------------------------------------------------------
void ItemManager::uninit(){
}

//==============================================================================
// createBomb
//------------------------------------------------------------------------------
void ItemManager::createBomb(int ownerId, int targetId, int dripNum){
  //iItem* bomb = ItemBomb::create();
  //this->addChild(bomb);
  //_bombList.push_back(bomb);
  //bomb->setUse(true);
  //bomb->setPos(_playerList[ownerId]->getPos());
  //bomb->setTarget(_playerList[targetId]);
  //bomb->setDripNum(dripNum * 60);

  for (ItemBomb*& _bomb : _bombList) {
    if (_bomb == nullptr) {
      _bomb = ItemBomb::create();
      _bomb->setUse(true);
      _bomb->setPos(_playerList[ownerId]->getPos());
      _bomb->setTarget(_playerList[targetId]);
      _bomb->setDripNum((dripNum + 1) * 60);
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

//EOF
