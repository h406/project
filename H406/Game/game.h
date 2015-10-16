//==============================================================================
//
// game[game.h]
// Author : Yasuaki Yamashita : 2015/10/15
//
//==============================================================================

#pragma once

#ifndef _GAME_H_
#define _GAME_H_

//==============================================================================
// 
//------------------------------------------------------------------------------
#include "CreateFunc.h"

class Game : public iScene,public CreateFunc<Game> {
public:
  static const int kNUM_X = 12;
  static const int kNUM_Y = 12;

  enum class FIELD_ID {
    NONE = 0,
    PLAYER_1,
    PLAYER_2,
    ITEM,
  };

  using CreateFunc::create;

  bool init();
  void update();
  void uninit();

private:
  int _freezeTime;
  int _freezeTimePlayer[2];
  int _bultime;

  Sprite3D * _player[2];
  CameraBace* _playerCam[2];
  Sprite3D* _backField;
  Sprite3D* _fieldMap[kNUM_X][kNUM_Y];

  FIELD_ID _field[kNUM_X][kNUM_Y];

  CameraBace* _mainCamera;

  float _numSpriteScl[2];
  int _num[2];
  Sprite2D* _numSprite[2];

  Sprite2D* _plus[2];
  Sprite2D* _plusNum[2];

  Effect* _effect;
  Vec3 _playerMoveVec[2];
};

#endif
//EOF