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
class ItemManager;
class Shadow;

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
    MODE_START,
    MODE_PLAY,
    MODE_ROUND_FINISH,
    MODE_NEXT_ROUND_SETUP,
    MODE_GAME_END,
  };

private:
  int _freezeTime;
  int _bultime;
  int _nextModeTime;
  int _mapToTime; // �J�E���g�A�b�v�Ɏg��

  Player* _player[kPlayerNum];
  CameraBace* _playerCam[kPlayerNum];
  Sprite3D* _backField;

  CameraBace* _mainCamera;

  Shadow* _playerShadow[kPlayerNum];

  EventManager* _eventManager;
  GuiManager* _guiManger;
  ItemManager* _itemManager;

  GAME_MODE _gameMode;
  CameraBace* _roundChangeCam; // ���Ղ̂��
};

#endif
//EOF