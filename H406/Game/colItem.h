//==============================================================================
//
// colItem[colItem.h]
// Author : masato masuda : 2015/11/25
//
//==============================================================================
#pragma once

#ifndef _COLITEM_H_
#define _COLITEM_H_

#include "CreateFunc.h"

class Player;
class Stage;
class EventManager;
class ItemManager;

//==============================================================================
// class
//------------------------------------------------------------------------------
class ColItem : public iLayer,public CreateFunc<ColItem> {
public:
  const static int kMaxPlayers = 4;

  bool init(Stage* stage, EventManager* event, ItemManager* item);
  void update();
  void uninit();
  void addPlayer(Player* obj);

private:

  bool hitCheckCircle(const Vec3& pos_0, const Vec3& pos_1, float rad_0, float rad_1);
  Player* _playerList[kMaxPlayers];
  EventManager* _event;
  ItemManager* _itemManager;
  Stage* _stage;
};

#endif
//EOF