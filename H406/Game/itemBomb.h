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

  int getOwnerId(){ return _ownerId; }
  void setOwnerId(int id){ _ownerId = id; }

  int getTargetId(){ return _targetId; }
  void setTargetId(int id){ _targetId = id; }

  bool getUse(){ return _is_use; }
  void setUse(bool use){ _is_use = use; }
  bool getDeath(){ return _is_death; }
  void setDeath(bool death){ _is_death = death; };

  void setRadius(float rad) { _radius = rad; }
  float getRadius() const { return _radius; }

  void setTarget(Player *obj){ _player = obj; }
  void setDripNum(int val){ _dripNum = val; };
  int  getDripNum() const { return _dripNum; };
  void addDripNum(int val) { _dripNum = (_dripNum + val) > 9 ? 9 : _dripNum + val; }

  void use();

private:

  XFileObject* _item;
  Player* _player;

  int _dripNum;

  Vec3 _moveDest;
};

#endif

//EOF