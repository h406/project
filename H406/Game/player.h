//==============================================================================
//
// player[player.h]
// Author : masato masuda : 2015/10/21
//
//==============================================================================

#pragma once

#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "CreateFunc.h"

//==============================================================================
// class
//------------------------------------------------------------------------------
class Player : public iLayer, public CreateFunc<Player> {
public:
  using CreateFunc::create;

  bool init(int playerID);
  void update();
  void uninit();

  void moveUp(float movement);
  void moveDown(float movement);
  void moveLeft(float movement);
  void moveRight(float movement);
  void moveTop(float movement);
  void moveBottom(float movement);

  void jump(float movement);

  void flipMvementX();
  void flipMvementZ();

  void addDripNum(int num) { _dripNum = (_dripNum + num) > 9 ? 9 : _dripNum + num; }
  void setDripNum(int num) { _dripNum = num; }

  int getPlayerID() const { return _playerID; }
  int getDripNum() const { return _dripNum; }

private:

  // プレイヤー
  XFileObject* _player[2];

  Vec3 _playerMoveVec;
  Vec3 _playerMoveDest;

  // 塗れる数
  int _dripNum;

  // プレイヤーID
  int _playerID;
};

#endif
//EOF