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

  RoundIcon* _roundIcon;
};

#endif

//EOF