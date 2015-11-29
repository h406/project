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
#include "dataManager.h"
#include "playerStatus.h"

namespace{
  const Vec2 kPLAYER_SIZE = Vec2(36 * 2, 72 * 2);
}

//==============================================================================
// init
//------------------------------------------------------------------------------
bool Player::init(int playerID)
{
  const auto status = DataManager::instance().getData()->getPlayerStatus(playerID);
  const auto& barStatus = PlayerStatus::kStickBarStatus[(int)status._barID];
  const auto& handStatus = PlayerStatus::kStickHandleStatus[(int)status._handleID];

  _player[0] = XFileObject::create(barStatus.fileName);
  this->addChild(_player[0]);

  _player[1] = XFileObject::create(handStatus.fileName);
  this->addChild(_player[1]);

  _player[0]->setScl(Vec3(0.7f,0.7f,0.7f));
  _player[1]->setScl(Vec3(0.7f,0.7f,0.7f));

  _playerMoveVec = Vec3(0.0f, 0.0, 0.0f);
  _playerMoveDest = Vec3(0.0f, 0.0, 0.0f);

  // プレイヤーID
  _playerID = playerID;
  // 所持塗れる数
    _dripNum = 0;
  // 半径
  _radius = 20.0f;
  // 重さ
  _weight = barStatus.weight * handStatus.weight;
  // 慣性
  _inertia = barStatus.inertia * barStatus.inertia;
  // マックススピード
  _maxSpeed = (barStatus.maxspeed + barStatus.maxspeed) * 0.5f;
  // 塗れる最大数
  _maxDripNum = barStatus.maxdripNum + handStatus.maxdripNum;
  // アイテムによる加速
  _accel = 1.0f;
  // フリーズ
  _freeze = false;

  return true;
}

//==============================================================================
// update
//------------------------------------------------------------------------------
void Player::update(void)
{
  if (_freeze == false){
    Vec2 moveDest(_playerMoveDest.x, _playerMoveDest.z);

    float length = D3DXVec2Length(&moveDest);
    if (length > 1.f) {
      length = 1.f;
    }

    D3DXVec2Normalize(&moveDest, &moveDest);

    float rot = atan2(moveDest.y, moveDest.x);
    _playerMoveDest.x = (cosf(rot) * length * _maxSpeed) * _accel;
    _playerMoveDest.z = (sinf(rot) * length * _maxSpeed) * _accel;
    _playerMoveDest.y -= 5.0;

    _playerMoveVec.x += (_playerMoveDest.x - _playerMoveVec.x) * _inertia;
    _playerMoveVec.y += (_playerMoveDest.y - _playerMoveVec.y) * 0.1f;
    _playerMoveVec.z += (_playerMoveDest.z - _playerMoveVec.z) * _inertia;
    _pos += _playerMoveVec;

    if (length >= 0.01f) {
      rot = D3DX_PI - atan2(_playerMoveVec.z, _playerMoveVec.x) + D3DX_PI * 0.5f;
      _player[0]->setRotY(rot);
      _player[1]->setRotY(rot);
    }
  }
  else{
    _playerMoveDest.y -= 5.0;
    _playerMoveVec.y += (_playerMoveDest.y - _playerMoveVec.y) * 0.1f;
    _pos.y = _playerMoveVec.y;

    const int offset = 5;
    Vec3 ram(float(rand() % offset) - offset, 0.0f, float(rand() % offset) - offset);
    _pos = _freezePos + ram;
  }

  _freeze = false;
  _accel = 1.0f;
  _playerMoveDest = Vec3(0.0f, 0.0f, 0.0f);
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
