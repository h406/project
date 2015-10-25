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

class Gauge;
class EventManager;

//==============================================================================
// class
//------------------------------------------------------------------------------
class GuiManager : public iLayer2D, public CreateFunc<GuiManager>{
public:

  bool init(void);
  void update();
  void uninit();

  void setNumSpriteScl(int playerId, float scl){ _numSpriteScl[playerId] = scl; }
  void setPlusNumAnimID(int playerId, int animId);

//  void EventListener(EventData* eventData);

private:

  Gauge* _gauge[2];
  Sprite2D* _gaugeBase;

  float _numSpriteScl[2];
  Sprite2D* _numSprite[2];

  Sprite2D* _plus[2];
  Sprite2D* _plusNum[2];
};

#endif

//EOF