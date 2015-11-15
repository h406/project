//==============================================================================
//
// guiManager[guiManager.h]
// Author : masuda masato : 2015/10/23
//
//==============================================================================
#pragma once

#ifndef _GUI_MANAGER_H_
#define _GUI_MANAGER_H_

#include "CreateFunc.h"

class EventManager;
class EventData;
class Gauge;
class NumberSprite;
class RoundIcon;

//==============================================================================
// class
//------------------------------------------------------------------------------
class GuiManager : public iLayer2D, public CreateFunc<GuiManager>{
public:

  struct GUI_SPRITE_2D{
    Sprite2D* _sprite;
    float _scl;
  };

  struct GUI_NUMBER_2D{
    NumberSprite* _sprite;
    float _scl;
  };

  bool init(EventManager* eventManager);
  void update();
  void uninit();

  void EventListener(EventData* eventData);
private:

  Gauge* _gauge[2];
  Sprite2D* _gaugeBase;

  float _numSpriteScl[2];
  NumberSprite* _numSprite[2];

  Sprite2D* _plus[2];
  Sprite2D* _plusHighLight[2];
  NumberSprite* _plusNum[2];

  GUI_NUMBER_2D _time;
  GUI_NUMBER_2D _roundNum;
  GUI_NUMBER_2D _resultNum[2];
  float _resultNumScl[2];
  Vec2 _resultNumPosDest[2];

  // •¶ŽšŒn
  GUI_SPRITE_2D _finish;
  GUI_SPRITE_2D _start;

  int _resultNumCount;
  int _frameCount;
  bool _is_result;
  bool _is_start;

  RoundIcon* _roundIcon;
};

#endif

//EOF