//==============================================================================
//
// itemBomb[itemBomb.h]
// Author : masato masuda : 2015/11/18
//
//==============================================================================
#pragma once

#ifndef _ITEM_BOMB_H_
#define _ITEM_BOMB_H_

#include "iItem.h"
#include "CreateFunc.h"

class Player;

//==============================================================================
// class
//------------------------------------------------------------------------------
class ItemBomb : public iItem, public CreateFunc<ItemBomb>{
public:
  using CreateFunc::create;

  bool init();
  void update();
  void uninit();

  int getPlayerId(){ return _playerId; }
  void setPlayerId(int id){ _playerId = id; }
  bool getUse(){ return _is_use; }
  void setUse(bool use){ _is_use = use; }

  void setTarget(Player *obj){ _player = obj; }

private:

  XFileObject* _item;
  Player* _player;

  int _dripNum;

  Vec3 _moveVec;
  Vec3 _moveDest;
};

#endif

//EOF