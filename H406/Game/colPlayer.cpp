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

///////////////////////////////////////////////////
// パーティクル衝突判定・時刻・位置算出関数
//   rA          : パーティクルAの半径
//   rB          : パーティクルBの半径
//   pre_pos_A   : パーティクルAの前の位置
//   pos_A       : パーティクルAの次の到達位置
//   pre_pos_B   : パーティクルBの前位置
//   pos_B       : パーティクルBの次の到達位置
//   pout_t0 : 最初の衝突位置時間を格納するFLOAT型へのポインタ
//   pout_t1 : 2度目の衝突位置時間を格納するFLOAT型へのポインタ
//   pout_colli_A : パーティクルAの衝突位置を格納するD3DXVECTOR型へのポインタ
//   pout_colli_B : パーティクルAの衝突位置を格納するD3DXVECTOR型へのポインタ
bool CalcParticleCollision(
  FLOAT rA,FLOAT rB,
  const Vec3& pPre_pos_A,const Vec3& pPos_A,
  const Vec3& pPre_pos_B,const Vec3& pPos_B,
  FLOAT *pOut_t0,
  FLOAT *pOut_t1,
  D3DXVECTOR3 *pOut_colli_A,
  D3DXVECTOR3 *pOut_colli_B
  ) {
  // 前位置及び到達位置におけるパーティクル間のベクトルを算出
  D3DXVECTOR3 C0 = pPre_pos_B - pPre_pos_A;
  D3DXVECTOR3 C1 = pPos_B - pPos_A;
  D3DXVECTOR3 D = C1 - C0;

  // 衝突判定用の2次関数係数の算出
  FLOAT P = D3DXVec3LengthSq(&D); if(P == 0) return false; // 同じ方向に移動
  FLOAT Q = D3DXVec3Dot(&C0,&D);
  FLOAT R = D3DXVec3LengthSq(&C0);

  // パーティクル距離
  FLOAT r = rA + rB;

  // 衝突判定式
  FLOAT Judge = Q*Q - P*(R - r*r);
  if(Judge < 0) {
    // 衝突していない
    return false;
  }

  // 衝突時間の算出
  FLOAT t_plus = (-Q + sqrt(Judge)) / P;
  FLOAT t_minus = (-Q - sqrt(Judge)) / P;


  // 衝突位置の決定
  *pOut_colli_A = pPre_pos_A + t_minus * (pPos_A - pPre_pos_A);
  *pOut_colli_B = pPre_pos_B + t_minus * (pPos_B - pPre_pos_B);

  // 衝突時間の決定（t_minus側が常に最初の衝突）
  *pOut_t0 = t_minus;
  *pOut_t1 = t_plus;

  // 時間内衝突できるか？
  // t_minusが1より大きいと届かず衝突していない
  // t_plus、t_minusが両方ともマイナスだと反対方向なので衝突しない
  if((t_minus > 1) || (t_minus < 0 && t_plus < 0))
    return false;

  return true; // 衝突報告

}

///////////////////////////////////////////////////
// パーティクル衝突後速度位置算出関数
//   pColliPos_A : 衝突中のパーティクルAの中心位置
//   pVelo_A     : 衝突の瞬間のパーティクルAの速度
//   pColliPos_B : 衝突中のパーティクルBの中心位置
//   pVelo_B     : 衝突の瞬間のパーティクルBの速度
//   weight_A    : パーティクルAの質量
//   weight_B    : パーティクルBの質量
//   res_A       : パーティクルAの反発率
//   res_B       : パーティクルBの反発率
//   time        : 反射後の移動時間
//   pOut_pos_A  : パーティクルAの反射後位置
//   pOut_velo_A : パーティクルAの反射後速度ベクトル
//   pOut_pos_B  : パーティクルBの反射後位置
//   pOut_velo_B : パーティクルBの反射後速度ベクトル

bool CalcParticleColliAfterPos(
  const Vec3& pColliPos_A,const Vec3& pVelo_A,
  const Vec3& pColliPos_B,const Vec3& pVelo_B,
  FLOAT weight_A,FLOAT weight_B,
  FLOAT res_A,FLOAT res_B,
  FLOAT time,
  D3DXVECTOR3 *pOut_pos_A,D3DXVECTOR3 *pOut_velo_A,
  D3DXVECTOR3 *pOut_pos_B,D3DXVECTOR3 *pOut_velo_B
  ) {
  FLOAT TotalWeight = weight_A + weight_B; // 質量の合計
  FLOAT RefRate = (1 + res_A*res_B); // 反発率
  D3DXVECTOR3 C = pColliPos_B - pColliPos_A; // 衝突軸ベクトル
  D3DXVec3Normalize(&C,&C);
  FLOAT Dot = D3DXVec3Dot(&(pVelo_A - pVelo_B),&C); // 内積算出
  D3DXVECTOR3 ConstVec = RefRate*Dot / TotalWeight * C; // 定数ベクトル

  // 衝突後速度ベクトルの算出
  *pOut_velo_A = -weight_B * ConstVec + pVelo_A;
  *pOut_velo_B = weight_A * ConstVec + pVelo_B;

  // 衝突後位置の算出
  *pOut_pos_A = pColliPos_A + time * (*pOut_velo_A);
  *pOut_pos_B = pColliPos_B + time * (*pOut_velo_B);

  return true;
}


//==============================================================================
// update
//------------------------------------------------------------------------------
void ColPlayer::update() {

  // プレイヤー同士の当たり判定
  if(_playerList[0] != nullptr && _playerList[1] != nullptr) {
    const Vec3 p1 = _playerList[0]->getPos() - _playerList[0]->getMoveVec();
    const Vec3 p2 = _playerList[1]->getPos() - _playerList[1]->getMoveVec();
    Vec3 pos[2];
    Vec3 vec[2];
    float t1,t2;

    const bool isHit = CalcParticleCollision(
      _playerList[0]->getRadius(),
      _playerList[1]->getRadius(),
      Vec3(p1.x,0.f,p1.z),
      Vec3(_playerList[0]->getPos().x,0.f,_playerList[0]->getPos().z),
      Vec3(p2.x,0.f,p2.z),
      Vec3(_playerList[1]->getPos().x,0.f,_playerList[1]->getPos().z),
      &t1, &t2,
      &pos[0],&pos[1]
      );


    if(isHit)
    {
      // 当たったあとのチェック
      CalcParticleColliAfterPos(
        pos[0],Vec3(_playerList[0]->getMoveVec().x,0.f,_playerList[0]->getMoveVec().z),
        pos[1],Vec3(_playerList[1]->getMoveVec().x,0.f,_playerList[1]->getMoveVec().z),
        _playerList[0]->getWeight(),_playerList[1]->getWeight(),
        1.5f,1.5f,
        t1,
        &pos[0],&vec[0],
        &pos[1],&vec[1]
        );

      _playerList[0]->setMoveVec(Vec3(vec[0].x,_playerList[0]->getMoveVec().y,vec[0].y));
      _playerList[1]->setMoveVec(Vec3(vec[1].x,_playerList[1]->getMoveVec().y,vec[1].y));
      _playerList[0]->setPos(Vec3(pos[0].x,_playerList[0]->getPos().y,pos[0].z));
      _playerList[1]->setPos(Vec3(pos[1].x,_playerList[1]->getPos().y,pos[1].z));
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

  // 2円の半径の2乗
  float r2 = rad[0] + rad[1];
  r2 *= r2;

  // 2円の中心座標計算
  Vec2 centerPos = Vec2(pos[0].x - pos[1].x, pos[0].z - pos[1].z);
  float  scalar = (centerPos.x * centerPos.x) + (centerPos.y * centerPos.y);

  // hir
  if (r2 >= scalar){
    return true;
  }
  return false;
}


//EOF