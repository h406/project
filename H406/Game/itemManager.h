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

  // �A�C�e������
  void createBomb(const Vec3& pos);
  void createAccel(const Vec3& pos);
  void createManhole(const Vec3& pos);

  // �A�C�e�����X�g�擾
  ItemBomb** getBombList(){ return _bombList; }
  ItemAccel** getAccelList(){ return _accelList; }
  ItemManhole** getManholeList(){ return _manholeList; }

  void EventListener(EventData* eventData);

  const static int kMaxPlayers = 2;
  const static int kBombMax = 3;
  const static int kAccelMax = 3;
  const static int kManholeMax = 3;

private:

  EventManager* _event;

  // �e�A�C�e�����X�g
  ItemBomb* _bombList[kBombMax];
  ItemAccel* _accelList[kAccelMax];
  ItemManhole* _manholeList[kManholeMax];

  // �v���C���[
  Player* _playerList[kMaxPlayers];
  // �v���C���[�̏����A�C�e�����
  iItem* _playerGetItem[kMaxPlayers];
};

#endif

//EOF