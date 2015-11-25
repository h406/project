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
  Vec3 getMoveVec(){ return _playerMoveVec; }
  void setMoveVec(const Vec3 vec){ _playerMoveVec = vec; }

  void addDripNum(int num) { _dripNum = (_dripNum + num) > 9 ? 9 : _dripNum + num; }
  void setDripNum(int num) { _dripNum = num; }

  int getPlayerID() const { return _playerID; }
  int getDripNum() const { return _dripNum; }

  void setRadius(float rad) { _radius = rad; }
  float getRadius() const { return _radius; }

  void setWeight(float val){ _weight = val; }
  float getWeight() const { return _weight; }

  int getMaxDripNum() const { return _maxDripNum; }

private:

  // �v���C���[
  XFileObject* _player[2];

  Vec3 _playerMoveVec;
  Vec3 _playerMoveDest;

  // �h��鐔
  int _dripNum;

  // �v���C���[ID
  int _playerID;

  // ���a
  float _radius;

  // �d��(0.0 ~ 1.0)
  float _weight;

  // �h���ő吔
  int _maxDripNum;

  // �����ő吔
  float _maxSpeed;
  
  // ����
  float _inertia;
};

#endif
//EOF