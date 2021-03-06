//==============================================================================
//
// roundIcon[roundIcon.h]
// Author : masato masuda : 2015/11/08
//
//==============================================================================
#pragma once

#ifndef _ROUND_ICON_H_
#define _ROUND_ICON_H_

#include "CreateFunc.h"

//==============================================================================
// class
//------------------------------------------------------------------------------
class RoundIcon : public iLayer2D, public CreateFunc<RoundIcon> {
public:
  using CreateFunc::create;

  bool init(float width, float height);
  void update();
  void uninit();

  void setTexture(int playerID, const char* tex);
  void setRoundWinNum(int playerId, int num);
  void setPlayerWin(int playerId, bool val){ _prevPlayerWin[playerId] = val; }
  void setMove(bool move){ _is_move = move; }

  static const int kROUND_MAX = 3;

private:
  Vec2 _size;
  Sprite2D* _roundIcon[2][kROUND_MAX];

  int _roundWinNum[2];
  float _numScl;
  bool _prevPlayerWin[2];
  bool _is_move;
  float _windowScl;
};

#endif
//EOF