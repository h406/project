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

//==============================================================================
// class
//------------------------------------------------------------------------------
class ItemManager : public iLayer, public CreateFunc<ItemManager>{
public:
  using CreateFunc::create;

  bool init();
  void update();
  void uninit();

  void createBomb(int ownerId, int targetId, int dripNum);
  void addPlayer(Player* obj);

  ItemBomb** getBombList(){ return _bombList; }

  const static int kMaxPlayers = 2;
  const static int kBombMax = 5;

private:

  ItemBomb* _bombList[kBombMax];

  vector<iItem*> _itemList;
  Player* _playerList[kMaxPlayers];

};

#endif

//EOF