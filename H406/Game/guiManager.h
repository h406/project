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

  NumberSprite* _time;
  NumberSprite* _roundNum;
  NumberSprite* _startNum;

  GUI_SPRITE_2D _finish;
  GUI_SPRITE_2D _start;

  float _timeScl;
  float _startNumScl;

  RoundIcon* _roundIcon;
  float _roundScl;
};

#endif

//EOF