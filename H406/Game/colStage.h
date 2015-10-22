//==============================================================================
//
// colStage[colStage.h]
// Author : Yasuaki Yamashita : 2015/10/21
//
//==============================================================================

#pragma once

#ifndef _COLSTAGE_H_
#define _COLSTAGE_H_

#include "CreateFunc.h"

class Player;
class Stage;
class EventManager;

//==============================================================================
// class
//------------------------------------------------------------------------------
class ColStage : public iLayer,public CreateFunc<ColStage> {
public:
  const static int kMaxPlayers = 4;

  bool init(Stage* stage,EventManager *event);
  void update();
  void uninit();
  void addPlayer(Player *obj);

private:
  Player* _playerList[kMaxPlayers];
  Stage* _stage;
  EventManager* _event;
};

#endif
//EOF