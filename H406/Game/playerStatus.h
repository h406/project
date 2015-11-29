//==============================================================================
//
// PlayerStatus[playerStatus.h]
// Author : Yasuaki Yamashita : 2015/11/25
//
//==============================================================================

#pragma once

#ifndef _PLAYERSTATUS_H_
#define _PLAYERSTATUS_H_

//==============================================================================
// player
//------------------------------------------------------------------------------
namespace PlayerStatus{

  // �X�e�[�^�X
  struct Status{
    int id;
    // �d�� (1 - 0)
    float weight;
    // ���� (1 - 0)
    float inertia;
    // �ő呬�x
    float maxspeed;
    // �h��ő��
    int maxdripNum;
    // �t�@�C����
    char fileName[256];
  };

  // �_����
  enum class StickBar{
    Normal = 0,
    Elephant,
    Bird,
    Dolphin,
    Max
  };

  // �����ĕ���
  enum class StickHandle {
    Normal = 0,
    Elephant,
    Bird,
    Dolphin,
    Max
  };

  // �X�e�[�^�X���X�g
  const Status kStickBarStatus[] = {
    {(int)StickBar::Normal,     0.5f,  0.2f,  5.f,  0,"./data/model/bar.x"},
    {(int)StickBar::Elephant,   1.f,  0.9f,  5.f,  0,"./data/model/elephant_bar.x"},
    {(int)StickBar::Bird,       0.2f,  0.1f,  20.f,  0,"./data/model/Bird_bar.x"},
    {(int)StickBar::Dolphin,    0.2f,  0.5f,  10.f,  0,"./data/model/Dolphin_bar.x"},
  };

  // �X�e�[�^�X���X�g
  const Status kStickHandleStatus[] = {
    {(int)StickHandle::Normal,  0.5f,  1.f,  5.f,  9,"./data/model/hand.x"},
    {(int)StickHandle::Elephant,1.f,  1.f,  3.f,  9,"./data/model/elephant_hand.x"},
    {(int)StickHandle::Bird,    0.2f,  0.2f,  20.f,  9,"./data/model/Bird_handle.x"},
    {(int)StickHandle::Dolphin, 0.5f,  0.5f,  10.f,  9,"./data/model/Dolphin_handle.x"},
  };

  const unsigned int kStickBarNum = (int)StickBar::Max;
  const unsigned int kStickHandleNum = (int)StickHandle::Max;

  // id
  struct PlayerStatus {
    PlayerStatus(StickBar barID,StickHandle handleID)
      :_barID(barID)
      ,_handleID(handleID) {
    };
    PlayerStatus(unsigned int barID,unsigned int handleID)
      :_barID((StickBar)barID)
      ,_handleID((StickHandle)handleID) {
    };
    PlayerStatus()
      :_barID(StickBar::Normal)
      ,_handleID(StickHandle::Normal) {
    };

    StickBar _barID;
    StickHandle _handleID;
  };

};


#endif
//EOF