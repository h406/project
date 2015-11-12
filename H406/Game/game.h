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

class Stage;
class Player;
class EventManager;
class EventData;
class GuiManager;

class Game : public iScene,public CreateFunc<Game> {
public:
  using CreateFunc::create;

  static const int kPlayerNum = 2;

  bool init();
  void update();
  void uninit();

  void EventListener(EventData* eventData);

  enum GAME_MODE{
    MODE_NONE = -1,
    MODE_PLAY,
    MODE_ROUND_CHANGE,
    MODE_NEXT_ROUND_SETUP
  };

private:
  int _freezeTime;
  int _bultime;
  int _nextModeTime;


  Player* _player[kPlayerNum];
  CameraBace* _playerCam[kPlayerNum];
  Sprite3D* _backField;

  CameraBace* _mainCamera;

  Effect* _effect;
  EventManager* _eventManager;
  GuiManager* _guiManger;

  GAME_MODE _gameMode;
  CameraBace* _roundChangeCam; // λαΥΜβΒ
//  CameraBace* _interpolationCam; // vC[ΜSβΤ
};

#endif
//EOF