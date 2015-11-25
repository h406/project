//==============================================================================
//
// itemManager[itemManager.h]
// Author : masato masuda : 2015/11/18
//
//==============================================================================
#pragma once

#ifndef _ITEM_MANAGER_H_
#define _ITEM_MANAGER_H_

#include "CreateFunc.h"

class Player;
class iItem;
class ItemBomb;
class EventManager;
class EventData;

//==============================================================================
// class
//------------------------------------------------------------------------------
class ItemManager : public iLayer, public CreateFunc<ItemManager>{
public:
  using CreateFunc::create;

  bool init(EventManager* event);
  void update();
  void uninit();

  void addPlayer(Player* obj);

  void createBomb(const Vec3& pos);

  ItemBomb** getBombList(){ return _bombList; }

  void EventListener(EventData* eventData);

  const static int kMaxPlayers = 2;
  const static int kBombMax = 5;

private:

  EventManager* _event;

  ItemBomb* _bombList[kBombMax];
//  vector<iItem*> _itemList;
  Player* _playerList[kMaxPlayers];
  iItem* _playerGetItem[kMaxPlayers];

};

#endif

//EOF