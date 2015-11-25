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
    PLAYER2_SELECT,
    PLAYER2_QR,
    // ââèo
    Production,
    MAX,
  };

}

class QRreader;

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

  CameraBace* _camera;

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

  bool _select;

  int _backTex[2];
};


#endif
//EOF