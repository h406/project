//==============================================================================
//
// colPlayer[colPlayer.h]
// Author : masato masuda : 2015/11/13
//
//==============================================================================
#pragma once

#ifndef _COLPLAYER_H_
#define _COLPLAYER_H_

#include "CreateFunc.h"

class Player;
class EventManager;

//==============================================================================
// class
//------------------------------------------------------------------------------
class ColPlayer : public iLayer,public CreateFunc<ColPlayer> {
public:
  const static int kMaxPlayers = 4;

  bool init(EventManager *event);
  void update();
  void uninit();
  void addPlayer(Player *obj);

private:

  bool hitCheckCircle(Player* player1, Player* player2);
  Player* _playerList[kMaxPlayers];
  EventManager* _event;
};

#endif
//EOF