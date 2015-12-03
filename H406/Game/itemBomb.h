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
class EventManager;

//==============================================================================
// class
//------------------------------------------------------------------------------
class ItemBomb : public iItem, public CreateFunc<ItemBomb>{
public:
  using CreateFunc::create;

  bool init();
  void update();
  void uninit();

  Player* getOwner(){ return _owner; }
  void setOwner(Player* obj){ _owner = obj; }

  Player* getTarget(){ return _target; }
  void setTarget(Player* obj){ _target = obj; }

  bool getUse(){ return _is_use; }
  void setUse(bool use){ _is_use = use; }
  bool getDeath(){ return _is_death; }
  void setDeath(bool death){ _is_death = death; };

  void setRadius(float rad) { _radius = rad; }
  float getRadius() const { return _radius; }

  void setDripNum(int val){ _dripNum = val; };
  int  getDripNum() const { return _dripNum; };
  void addDripNum(int val) { _dripNum = (_dripNum + val) > 9 ? 9 : _dripNum + val; }

  void addEventManager(EventManager* eventM){ _event = eventM; }
  void setHitEnable(bool hit){ _hitEnable = hit; }
  bool getHitEnable(){ return _hitEnable; }

  void use();

private:

  XFileObject* _item;
  EventManager* _event;

  int _dripNum;
  Vec3 _moveDest;
  bool _hitEnable;
};

#endif

//EOF