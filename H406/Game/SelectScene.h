//==============================================================================
//
// SelectScene[SelectScene.h]
// Author : Yasuaki Yamashita : 2015/11/13
//
//==============================================================================

#pragma once

#ifndef _SELECTSCENE_H_
#define _SELECTSCENE_H_

#include "CreateFunc.h"


namespace {
  enum class SELECT_MODE {
    PLAYER1_SELECT,
    PLAYER1_QR,
    PLAYER1_DECISION,
    PLAYER1_SHOW,
    PLAYER2_SELECT,
    PLAYER2_QR,
    PLAYER2_DECISION,
    PLAYER2_SHOW,
    // ââèo
    Production,
    MAX,
  };

}

class QRreader;
class ShuchuSen;

//==============================================================================
// class
//------------------------------------------------------------------------------
class SelectScene : public iScene,public CreateFunc < SelectScene > {
public:
  using CreateFunc::create;

  bool init();
  void update();
  void uninit();

private:
  void SelectQR(int playerID);
  void ReadQR(int playerID);
  void CreateStick(int playerID, int sticlID);

  CameraBace* _camera;
  CameraBace* _playerCamera;

  Sprite2D* _back;
  Sprite2D* _waku;

  Sprite2D* _nowReading;

  SELECT_MODE _mode;

  bool* _isEnd;
  thread* _QRthread;
  QRreader* _QRreader;

  Sprite2D* _oji;
  Sprite2D* _oba;
  Sprite2D* _vs;
  ShuchuSen* _s;

  bool _select;

  int _backTex[2];
  float _windowScl;

  XFileObject* _stickLayer;
  XFileObject* _stickHandlePlayer[2];
  XFileObject* _stickBarPlayer[2];
  XFileObject* _stickHandle[3];
  XFileObject* _stickBar[3];

  int _curSelectPlayer = 0;
  bool _inputPermit;

  int _playerStickID[2];

  float _modelLength;
  float _lengthSpeed;
  float _gameF;
  float _fspeed;
  int _frame;
  bool _stickDecision;
};


#endif
//EOF