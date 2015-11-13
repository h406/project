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

  const float kPlayerMovement = 5.f;
}

//==============================================================================
// init
//------------------------------------------------------------------------------
bool Player::init(int playerID)
{
  _player[0] = XFileObject::create("./data/model/bar.x");
  this->addChild(_player[0]);

  _player[1] = XFileObject::create("./data/model/hand.x");
  this->addChild(_player[1]);

  _player[0]->setScl(Vec3(0.7f,0.7f,0.7f));
  _player[1]->setScl(Vec3(0.7f,0.7f,0.7f));

  _playerMoveVec = Vec3(0.0f, 0.0, 0.0f);
  _playerMoveDest = Vec3(0.0f, 0.0, 0.0f);

  _playerID = playerID;

  _dripNum = 0;

  return true;
}

//==============================================================================
// update
//------------------------------------------------------------------------------
void Player::update(void)
{
  Vec2 moveDest(_playerMoveDest.x, _playerMoveDest.z);

  float length = D3DXVec2Length(&moveDest);
  if(length > 1.f) {
    length = 1.f;
  }

  D3DXVec2Normalize(&moveDest, &moveDest);

  float rot = atan2(moveDest.y, moveDest.x);
  _playerMoveDest.x = cosf(rot) * length * kPlayerMovement;
  _playerMoveDest.z = sinf(rot) * length * kPlayerMovement;
  _playerMoveDest.y -= 5.0;


  _playerMoveVec+= (_playerMoveDest - _playerMoveVec) * 0.1f;
  _pos += _playerMoveVec;

  rot = D3DX_PI - atan2(_playerMoveVec.z,_playerMoveVec.x);
  _player[0]->setRotY(rot);
  _player[1]->setRotY(rot);


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

void Player::jump(float movement) {
  _playerMoveVec.y = movement;
}

void Player::flipMvementX() {
  _playerMoveVec.x *= -1;
}

void Player::flipMvementZ() {
  _playerMoveVec.z *= -1;
}

//EOF
