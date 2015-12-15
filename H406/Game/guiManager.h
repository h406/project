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

  void setMaxDripNum(int id, int val){ _maxDripNum[id] = val; }
  void EventListener(EventData* eventData);

private:

  // ゲージ関係
  Sprite2D* _gaugeLayer;
  Gauge* _gauge[2];
  Sprite2D* _gaugeBase;
  Sprite2D* _gaugeBack;
  Sprite2D* _gaugeGear;

  // 塗るやつ系
  float _numSpriteScl[2];
  NumberSprite* _numSprite[2];
  Sprite2D* _plus[2];
  Sprite2D* _plusHighLight[2];
  NumberSprite* _plusNum[2];

  // タイマー
  GUI_NUMBER_2D _time;

  // ラウンド
  GUI_NUMBER_2D _roundNum;
  RoundIcon* _roundIcon; // 勝ち星

  // リザルト関連
  GUI_NUMBER_2D _resultNum[2];
  float _resultNumScl[2];
  Vec2 _resultNumPosDest[2];

  // 文字系
  GUI_SPRITE_2D _finish;
  GUI_SPRITE_2D _start;
  GUI_SPRITE_2D _roundString;

  // リザルト
  int _resultNumCount;
  int _frameCount;
  bool _isResult;
  bool _isStart;
  bool _isPlay;

  // アイテム
  GUI_SPRITE_2D _item[2];
  GUI_SPRITE_2D _itemBase[2];

  int _maxDripNum[2];

  // おじさんおばさん
  GUI_SPRITE_2D _oba3;
  GUI_SPRITE_2D _oji3;
  Vec2 _ojiobaPosDest[2];
};

#endif

//EOF