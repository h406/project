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

  const static int kMaxPlayers = 2;

private:

  vector<iItem*> _itemList;
  Player* _playerList[kMaxPlayers];
};

#endif

//EOF