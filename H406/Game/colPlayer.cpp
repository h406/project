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
	//px : 0,	// x À•W
	//py : 0,	// y À•W
	//dx : 2,	// x •ûŒü‚Ì‘¬“x
	//dy : 3,	// y •ûŒü‚Ì‘¬“x
	//r  :10,	// ”¼Œa
	//m  : 1	// Ž¿—Ê

      Vec3 pos[2] = { _playerList[0]->getPos(), _playerList[1]->getPos() };

      Vec2 vec = Vec2(pos[0].x - pos[1].x, pos[0].z - pos[1].z);
      float len = sqrt(vec.x * vec.x + vec.y * vec.y);
      float distance = (_playerList[0]->getRadius() + _playerList[1]->getRadius()) - len;

      if (len > 0)	len = 1 / len;
      vec *= len;

      distance /= 2.0;
      pos[0] += Vec3(vec.x, 0.0f, vec.y) * distance;
      pos[1] -= Vec3(vec.x, 0.0f, vec.y) * distance;

      float e = 1.0;

      Vec2 moveVec[2] = { Vec2(_playerList[0]->getMoveVec().x, _playerList[0]->getMoveVec().z),
                          Vec2(_playerList[1]->getMoveVec().x, _playerList[1]->getMoveVec().z) };
      D3DXVec2Normalize(&moveVec[0], &moveVec[0]);
      D3DXVec2Normalize(&moveVec[1], &moveVec[1]);

#define am _playerList[0]->getWeight()
#define bm _playerList[1]->getWeight()
#define amx (moveVec[0].x)
#define amy (moveVec[0].
#define bmx (moveVec[1].x)
#define bmy (moveVec[1].z)

      float adx = (am * amx + bm * bmx + bmx * e * bm - amx * e * bm) / (am + bm);
      float bdx = -e * (bmx - amx) + adx;
      float ady = (am * amy + bm * bmy + bmy * e * bm - amy * e * bm) / (am + bm);
      float bdy = -e * (bmy - amy) + ady;

      //float e = 1.0;
      //float adx = (a.m * amx + b.m * bmx + bmx * e * b.m - amx * e * b.m) / (a.m + b.m);
      //float bdx = -e * (bmx - amx) + adx;
      //float ady = (a.m * amy + b.m * bmy + bmy * e * b.m - amy * e * b.m) / (a.m + b.m);
      //float bdy = -e * (bmy - amy) + ady;

      moveVec[0] += Vec3(adx, 0.0f, ady);
      moveVec[1] += Vec3(bdx, 0.0f, bdy);



      _playerList[0]->setMoveVec(moveVec[0]);
      _playerList[1]->setMoveVec(moveVec[1]);

      _playerList[0]->setPos(pos[0]);
      _playerList[1]->setPos(pos[1]);

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