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
class ItemAccel;
class ItemManhole;

class EventManager;
class EventData;
class shadow;

//==============================================================================
// class
//------------------------------------------------------------------------------
class ItemManager : public iLayer, public CreateFunc<ItemManager>{
public:
  using CreateFunc::create;

  bool init(EventManager* event);
  void update();
  void uninit();

  void addPlayer(Player* obj);

  // アイテム生成
  void createBomb();
  void createAccel();
  void createManhole();

  // アイテムリスト取得
  ItemBomb** getBombList(){ return _bombList; }
  ItemAccel** getAccelList(){ return _accelList; }
  ItemManhole** getManholeList(){ return _manholeList; }

  void EventListener(EventData* eventData);

  const static int kMaxPlayers = 2;
  const static int kBombMax = 3;
  const static int kAccelMax = 3;
  const static int kManholeMax = 3;
  // stage
  static const int kNUM_X = 12;
  static const int kNUM_Y = 12;

private:

  EventManager* _event;

  // 各アイテムリスト
  ItemBomb* _bombList[kBombMax];
  ItemAccel* _accelList[kAccelMax];
  ItemManhole* _manholeList[kManholeMax];

  // 影
  shadow* _bombShadow[kBombMax];
  shadow* _manholeShadow[kManholeMax];

  // プレイヤー
  Player* _playerList[kMaxPlayers];
  // プレイヤーの所持アイテム情報
  iItem* _playerGetItem[kMaxPlayers];

  // フィールド
  bool _field[kNUM_X][kNUM_Y];

};

#endif

//EOF