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

  virtual int getOwnerId() = 0;
  virtual void setOwnerId(int id) = 0;

  virtual int getTargetId() = 0;
  virtual void setTargetId(int id) = 0;

  virtual bool getUse() = 0;
  virtual void setUse(bool use) = 0;

  virtual bool getDeath() = 0;
  virtual void setDeath(bool death) = 0;

  virtual void setRadius(float rad) = 0;
  virtual float getRadius() const = 0;

  virtual void use() = 0;

protected:

  int _ownerId;
  int _targetId;

  bool _is_use;
  bool _is_death;
  float _radius;
};

#endif
//EOF