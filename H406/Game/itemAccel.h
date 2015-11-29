//==============================================================================
//
// itemAccel[itemAccel.h]
// Author : masato masuda : 2015/11/28
//
//==============================================================================
#pragma once

#ifndef _ITEM_ACCEL_H_
#define _ITEM_ACCEL_H_

#include "iItem.h"
#include "CreateFunc.h"

class Player;

//==============================================================================
// class
//------------------------------------------------------------------------------
class ItemAccel : public iItem, public CreateFunc<ItemAccel>{
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

  void setAccelTime(int val){ _accelTime = val; };
  int  getAccelTime() const { return _accelTime; };

  void use();

private:

  XFileObject* _item;

  int _accelTime;
  Vec3 _moveDest;
};

#endif

//EOF