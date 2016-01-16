//==============================================================================
//
// roundIcon[roundIcon.cpp]
// Author : masato masuda : 2015/11/08
//
//==============================================================================

//******************************************************************************
// include
//******************************************************************************
#include "roundIcon.h"
#include "dataManager.h"

namespace{
  const Vec2 kTargetPos = Vec2(216.0f, 109.0f);
  const float kOffsetX = 4.0f;
}

//==============================================================================
// init
//------------------------------------------------------------------------------
bool RoundIcon::init(float width, float height)
{
  _windowScl = (float)(App::instance().getWindowSize().cx / 1280.f);
  const Vec2 pos[2] = { Vec2(App::instance().getWindowSize().cx * 0.5f - (kTargetPos.x * _windowScl), (kTargetPos.y * _windowScl)),
                        Vec2(App::instance().getWindowSize().cx * 0.5f + (kTargetPos.x * _windowScl), (kTargetPos.y * _windowScl)) };

  _size = Vec2(width, height);
  _numScl = 1.0f;
  _is_move = false;

  for (int i = 0; i < 2; i++){
    for (int j = 0; j < RoundIcon::kROUND_MAX; j++){
      _roundIcon[i][j] = Sprite2D::create();
      _roundIcon[i][j]->setSize(width, height);
      _roundIcon[i][j]->setVisible(false);

      if (i == 0){
        _roundIcon[i][j]->setPos(Vec2(pos[i].x - (_size.x * j) - (kOffsetX * j), pos[i].y));
      } else{
        _roundIcon[i][j]->setPos(Vec2(pos[i].x + (_size.x * j) + (kOffsetX * j), pos[i].y));
      }
      this->addChild(_roundIcon[i][j]);
    }
    _roundWinNum[i] = 0;
    _prevPlayerWin[i] = false;
  }

  return true;
}

//==============================================================================
// update
//------------------------------------------------------------------------------
void RoundIcon::update(void){
  const Vec2 pos[2] = { Vec2(App::instance().getWindowSize().cx * 0.5f - (kTargetPos.x * _windowScl), (kTargetPos.y * _windowScl)),
                        Vec2(App::instance().getWindowSize().cx * 0.5f + (kTargetPos.x * _windowScl), (kTargetPos.y * _windowScl)) };

  for (int i = 0; i < 2; i++){
    for (int j = 0; j < RoundIcon::kROUND_MAX; j++){
      if (_roundWinNum[i] > j){
        _roundIcon[i][j]->setVisible(true);
      }
      else{
        _roundIcon[i][j]->setVisible(false);
      }
    }
  }

  if (_is_move){
    for (int i = 0; i < 2; i++){
      for (int j = 0; j < RoundIcon::kROUND_MAX; j++){
        int curId = _roundWinNum[i] - 1;

        if (_prevPlayerWin[i] && curId == j){
          _numScl += (1 - _numScl) * 0.1f;
          _roundIcon[i][curId]->setSize(_size.x *_numScl, _size.y * _numScl);

          Vec2 startPos = _roundIcon[i][curId]->getPos();
          Vec2 destPos(0.0f, 0.0f);
          if (i == 0){
            destPos = Vec2(pos[i].x - (_size.x * curId) - ((kOffsetX * _windowScl) * curId), pos[i].y);
          }
          else{
            destPos = Vec2(pos[i].x + (_size.x * curId) + ((kOffsetX * _windowScl) * curId), pos[i].y);
          }
          Vec2 movePos = Vec2((destPos - startPos) * 0.1f) + startPos;

          _roundIcon[i][curId]->setPos(movePos);
        }
      }
    }
  }
}

//==============================================================================
// uninit
//------------------------------------------------------------------------------
void RoundIcon::uninit(void){
}

//==============================================================================
// setTexture
//------------------------------------------------------------------------------
void RoundIcon::setTexture(int playerID, const char* tex){
  for (int i = 0; i < RoundIcon::kROUND_MAX; i++){
    _roundIcon[playerID][i]->setTexture(tex);
  }
}

//==============================================================================
// setRoundWinNum
//------------------------------------------------------------------------------
void RoundIcon::setRoundWinNum(int playerId, int num){
  if (num > kROUND_MAX){
    _roundWinNum[playerId] = kROUND_MAX;
  }
  else{
    _roundWinNum[playerId] = num;
  }
  _numScl = 10.0f;
  _prevPlayerWin[playerId] = true;
  _is_move = true;

  _roundIcon[playerId][_roundWinNum[playerId] - 1]->setPos(Vec2(App::instance().getWindowSize().cx * 0.5f, App::instance().getWindowSize().cy * 0.5f));
}


//EOF
