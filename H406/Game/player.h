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

  bool init();
  void update();
  void uninit();

  void Player::setPos(const Vec3& pos){ _player->setPos(pos);}
  const Vec3& Player::getPos(void) const{return _player->getPos();}
  void Player::setRot(const Vec3& rot){_player->setPos(rot);}
  const Vec3& Player::getRot(void) const{ return _player->getRot(); }

  void moveUp(float movement);
  void moveDown(float movement);
  void moveLeft(float movement);
  void moveRight(float movement);
  void moveTop(float movement);
  void moveBottom(float movement);

private:

  // ÉvÉåÉCÉÑÅ[
  Sprite3D* _player;

  Vec3 _playerMoveVec;
  Vec3 _playerMoveDest;

  int _freezeTime;
};

#endif
//EOF