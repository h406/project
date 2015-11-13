//==============================================================================
//
// colPlayer[colPlayer.cpp]
// Author : masato masuda : 2015/11/13
//
//==============================================================================

//******************************************************************************
// include
//******************************************************************************
#include "colPlayer.h"
#include "player.h"
#include "EventList.h"
#include "eventManager.h"

//==============================================================================
// init
//------------------------------------------------------------------------------
bool ColPlayer::init(EventManager* event) {
  _event = event;
  memset(_playerList,0,sizeof(_playerList));

  return true;
}

//==============================================================================
// update
//------------------------------------------------------------------------------
void ColPlayer::update() {

  // ƒvƒŒƒCƒ„[“¯Žm‚Ì“–‚½‚è”»’è
  if (_playerList[0] != nullptr && _playerList[1] != nullptr){

    // hit
    if (hitCheckCircle(_playerList[0], _playerList[1])){
//      Vec3 moveVec[2] = { _playerList[0]->getMoveVec(), _playerList[1]->getMoveVec() };
    }
  }
}

//==============================================================================
// addPlayer
//------------------------------------------------------------------------------
void ColPlayer::addPlayer(Player* player) {
  for(Player*& _player : _playerList) {
    if(_player == nullptr) {
      _player = player;
      break;
    }
  }
}

//==============================================================================
// uninit
//------------------------------------------------------------------------------
void ColPlayer::uninit() {

}


//==============================================================================
// hitCheckCircle
//------------------------------------------------------------------------------
bool ColPlayer::hitCheckCircle(Player* player1, Player* player2){
  Vec3 pos[2] = { player1->getPos(), player2->getPos() };
  float rad[2] = { player1->getRadius(), player2->getRadius() };

  // 2‰~‚Ì”¼Œa‚Ì2æ
  float r2 = rad[0] + rad[1];
  r2 *= r2;

  // 2‰~‚Ì’†SÀ•WŒvŽZ
  Vec2 centerPos = Vec2(pos[0].x - pos[1].x, pos[0].z - pos[1].z);
  float  scalar = (centerPos.x * centerPos.x) + (centerPos.y * centerPos.y);

  // hir
  if (r2 >= scalar){
    return true;
  }
  return false;
}


//EOF