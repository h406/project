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

//==============================================================================
// init
//------------------------------------------------------------------------------
bool RoundIcon::init(float width, float height)
{
  const Vec2 pos[2] = { Vec2(App::instance().getWindowSize().cx * 0.5f - 220.0f, 70.0f),
                        Vec2(App::instance().getWindowSize().cx * 0.5f + 220.0f, 70.0f) };
  _size = Vec2(width,height);

  for (int i = 0; i < 2; i++){
    for (int j = 0; j < RoundIcon::kROUND_MAX; j++){
      _roundIcon[i][j] = Sprite2D::create();
      _roundIcon[i][j]->setSize(width, height);
      _roundIcon[i][j]->setVisible(false);

      if (i == 0){
        _roundIcon[i][j]->setPos(Vec2(pos[i].x - (_size.x * j), pos[i].y));
      } else{
        _roundIcon[i][j]->setPos(Vec2(pos[i].x + (_size.x * j), pos[i].y));
      }
      this->addChild(_roundIcon[i][j]);
    }
    _roundWinNum[i] = 0;
  }

  return true;
}

//==============================================================================
// update
//------------------------------------------------------------------------------
void RoundIcon::update(void){
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
}


//EOF
