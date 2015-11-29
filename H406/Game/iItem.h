//==============================================================================
//
// iItem[iItem.h]
// Author : masato masuda : 2015/11/18
//
//==============================================================================
#pragma once

#ifndef _I_ITEM_H_
#define _I_ITEM_H_

class Player;

//==============================================================================
// class
//------------------------------------------------------------------------------
class iItem : public iLayer{
public:

  virtual Player* getOwner() = 0;
  virtual void setOwner(Player* obj) = 0;

  virtual Player* getTarget() = 0;
  virtual void setTarget(Player* obj) = 0;

  virtual bool getUse() = 0;
  virtual void setUse(bool use) = 0;

  virtual bool getDeath() = 0;
  virtual void setDeath(bool death) = 0;

  virtual void setRadius(float rad) = 0;
  virtual float getRadius() const = 0;

  virtual void use() = 0;

protected:

  Player* _owner;
  Player* _target;

  bool _is_use;
  bool _is_death;
  float _radius;
};

#endif
//EOF