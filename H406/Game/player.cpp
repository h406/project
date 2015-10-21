//==============================================================================
//
// player[player.cpp]
// Author : masato masuda : 2015/10/21
//
//==============================================================================

//******************************************************************************
// include
//******************************************************************************
#include "player.h"

namespace{
  const Vec2 kPLAYER_SIZE = Vec2(36 * 2, 72 * 2);
}

//==============================================================================
// init
//------------------------------------------------------------------------------
bool Player::init(void)
{
  _player = Sprite3D::create("./data/texture/akira000.png");
  _player->setSize(kPLAYER_SIZE.x, kPLAYER_SIZE.y);
  this->addChild(_player);

  _playerMoveVec = Vec3(0.0f, 0.0, 0.0f);
  _playerMoveDest = Vec3(0.0f, 0.0, 0.0f);

  _freezeTime = 0;

  return true;
}

//==============================================================================
// update
//------------------------------------------------------------------------------
void Player::update(void)
{
  Vec3 playerPos = _player->getPos();

  _playerMoveVec+= (_playerMoveDest - _playerMoveVec) * 0.1f;
  playerPos += _playerMoveVec;
  _player->setPos(playerPos);

  _playerMoveDest = Vec3(0.0f, 0.0, 0.0f);
}

//==============================================================================
// uninit
//------------------------------------------------------------------------------
void Player::uninit(void)
{
}

//==============================================================================
// move
//------------------------------------------------------------------------------
void Player::moveUp(float movement){
  _playerMoveDest.z += movement;
}

void Player::moveDown(float movement){
  _playerMoveDest.z -= movement;
}

void Player::moveLeft(float movement){
  _playerMoveDest.x -= movement;
}

void Player::moveRight(float movement){
  _playerMoveDest.x += movement;
}

void Player::moveTop(float movement){
  _playerMoveDest.y += movement;
}

void Player::moveBottom(float movement){
  _playerMoveDest.y -= movement;
}


//EOF
