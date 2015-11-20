//==============================================================================
//
// iItem[iItem.h]
// Author : masato masuda : 2015/11/18
//
//==============================================================================
#pragma once

#ifndef _I_ITEM_H_
#define _I_ITEM_H_

//==============================================================================
// class
//------------------------------------------------------------------------------
class iItem : public iLayer{
public:
  virtual int getPlayerId() = 0;
  virtual void setPlayerId(int id) = 0;

  virtual bool getUse() = 0;
  virtual void setUse(bool use) = 0;

protected:

  int _playerId;
  bool _is_use;
};

#endif
//EOF