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
// �p�[�e�B�N���Փ˔���E�����E�ʒu�Z�o�֐�
//   rA          : �p�[�e�B�N��A�̔��a
//   rB          : �p�[�e�B�N��B�̔��a
//   pre_pos_A   : �p�[�e�B�N��A�̑O�̈ʒu
//   pos_A       : �p�[�e�B�N��A�̎��̓��B�ʒu
//   pre_pos_B   : �p�[�e�B�N��B�̑O�ʒu
//   pos_B       : �p�[�e�B�N��B�̎��̓��B�ʒu
//   pout_t0 : �ŏ��̏Փˈʒu���Ԃ��i�[����FLOAT�^�ւ̃|�C���^
//   pout_t1 : 2�x�ڂ̏Փˈʒu���Ԃ��i�[����FLOAT�^�ւ̃|�C���^
//   pout_colli_A : �p�[�e�B�N��A�̏Փˈʒu���i�[����D3DXVECTOR�^�ւ̃|�C���^
//   pout_colli_B : �p�[�e�B�N��A�̏Փˈʒu���i�[����D3DXVECTOR�^�ւ̃|�C���^
bool CalcParticleCollision(
  FLOAT rA,FLOAT rB,
  const Vec3& pPre_pos_A,const Vec3& pPos_A,
  const Vec3& pPre_pos_B,const Vec3& pPos_B,
  FLOAT *pOut_t0,
  FLOAT *pOut_t1,
  D3DXVECTOR3 *pOut_colli_A,
  D3DXVECTOR3 *pOut_colli_B
  ) {
  // �O�ʒu�y�ѓ��B�ʒu�ɂ�����p�[�e�B�N���Ԃ̃x�N�g�����Z�o
  D3DXVECTOR3 C0 = pPre_pos_B - pPre_pos_A;
  D3DXVECTOR3 C1 = pPos_B - pPos_A;
  D3DXVECTOR3 D = C1 - C0;

  // �Փ˔���p��2���֐��W���̎Z�o
  FLOAT P = D3DXVec3LengthSq(&D); if(P == 0) return false; // ���������Ɉړ�
  FLOAT Q = D3DXVec3Dot(&C0,&D);
  FLOAT R = D3DXVec3LengthSq(&C0);

  // �p�[�e�B�N������
  FLOAT r = rA + rB;

  // �Փ˔��莮
  FLOAT Judge = Q*Q - P*(R - r*r);
  if(Judge < 0) {
    // �Փ˂��Ă��Ȃ�
    return false;
  }

  // �Փˎ��Ԃ̎Z�o
  FLOAT t_plus = (-Q + sqrt(Judge)) / P;
  FLOAT t_minus = (-Q - sqrt(Judge)) / P;


  // �Փˈʒu�̌���
  *pOut_colli_A = pPre_pos_A + t_minus * (pPos_A - pPre_pos_A);
  *pOut_colli_B = pPre_pos_B + t_minus * (pPos_B - pPre_pos_B);

  // �Փˎ��Ԃ̌���it_minus������ɍŏ��̏Փˁj
  *pOut_t0 = t_minus;
  *pOut_t1 = t_plus;

  // ���ԓ��Փ˂ł��邩�H
  // t_minus��1���傫���Ɠ͂����Փ˂��Ă��Ȃ�
  // t_plus�At_minus�������Ƃ��}�C�i�X���Ɣ��Ε����Ȃ̂ŏՓ˂��Ȃ�
  if((t_minus > 1) || (t_minus < 0 && t_plus < 0))
    return false;

  return true; // �Փ˕�

}

///////////////////////////////////////////////////
// �p�[�e�B�N���Փˌ㑬�x�ʒu�Z�o�֐�
//   pColliPos_A : �Փ˒��̃p�[�e�B�N��A�̒��S�ʒu
//   pVelo_A     : �Փ˂̏u�Ԃ̃p�[�e�B�N��A�̑��x
//   pColliPos_B : �Փ˒��̃p�[�e�B�N��B�̒��S�ʒu
//   pVelo_B     : �Փ˂̏u�Ԃ̃p�[�e�B�N��B�̑��x
//   weight_A    : �p�[�e�B�N��A�̎���
//   weight_B    : �p�[�e�B�N��B�̎���
//   res_A       : �p�[�e�B�N��A�̔�����
//   res_B       : �p�[�e�B�N��B�̔�����
//   time        : ���ˌ�̈ړ�����
//   pOut_pos_A  : �p�[�e�B�N��A�̔��ˌ�ʒu
//   pOut_velo_A : �p�[�e�B�N��A�̔��ˌ㑬�x�x�N�g��
//   pOut_pos_B  : �p�[�e�B�N��B�̔��ˌ�ʒu
//   pOut_velo_B : �p�[�e�B�N��B�̔��ˌ㑬�x�x�N�g��

bool CalcParticleColliAfterPos(
  const Vec3& pColliPos_A,const Vec3& pVelo_A,
  const Vec3& pColliPos_B,const Vec3& pVelo_B,
  FLOAT weight_A,FLOAT weight_B,
  FLOAT res_A,FLOAT res_B,
  FLOAT time,
  D3DXVECTOR3 *pOut_pos_A,D3DXVECTOR3 *pOut_velo_A,
  D3DXVECTOR3 *pOut_pos_B,D3DXVECTOR3 *pOut_velo_B
  ) {
  FLOAT TotalWeight = weight_A + weight_B; // ���ʂ̍��v
  FLOAT RefRate = (1 + res_A*res_B); // ������
  D3DXVECTOR3 C = pColliPos_B - pColliPos_A; // �Փˎ��x�N�g��
  D3DXVec3Normalize(&C,&C);
  FLOAT Dot = D3DXVec3Dot(&(pVelo_A - pVelo_B),&C); // ���ώZ�o
  D3DXVECTOR3 ConstVec = RefRate*Dot / TotalWeight * C; // �萔�x�N�g��

  // �Փˌ㑬�x�x�N�g���̎Z�o
  *pOut_velo_A = -weight_B * ConstVec + pVelo_A;
  *pOut_velo_B = weight_A * ConstVec + pVelo_B;

  // �Փˌ�ʒu�̎Z�o
  *pOut_pos_A = pColliPos_A + time * (*pOut_velo_A);
  *pOut_pos_B = pColliPos_B + time * (*pOut_velo_B);

  return true;
}


//==============================================================================
// update
//------------------------------------------------------------------------------
void ColPlayer::update() {

  // �v���C���[���m�̓����蔻��
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
      // �����������Ƃ̃`�F�b�N
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

  // 2�~�̔��a��2��
  float r2 = rad[0] + rad[1];
  r2 *= r2;

  // 2�~�̒��S���W�v�Z
  Vec2 centerPos = Vec2(pos[0].x - pos[1].x, pos[0].z - pos[1].z);
  float  scalar = (centerPos.x * centerPos.x) + (centerPos.y * centerPos.y);

  // hir
  if (r2 >= scalar){
    return true;
  }
  return false;
}


//EOF