//==============================================================================
//
// result[result.h]
// Author : masato masuda : 2015/12/09
//
//==============================================================================

#pragma once

#ifndef _RESULT_H_
#define _RESULT_H_

#include "CreateFunc.h"

class CameraBace;
class Player;

//==============================================================================
// 
//==============================================================================
class Result : public iScene ,public CreateFunc<Result> {
public:
  using CreateFunc::create;

  bool init();
  void update();
  void uninit();

private:

  CameraBace* _camera;
  Sprite2D* _winSprite;
  float _winSpriteScl;

  Player* _player;
  Sprite2D* _playerSprite;
  Vec2 _playerSpritePosDest;

  float _windowScl;
};

#endif
//EOF