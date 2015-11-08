//==============================================================================
//
// NumberSprite[numberSprite.h]
// Author : masato masuda : 2015/10/17
//
//==============================================================================
#pragma once
#ifndef _NUMBER_SPRITE_H_
#define _NUMBER_SPRITE_H_

//==============================================================================
// include
//==============================================================================
#include "DrawObject.h"
#include "CreateFunc.h"

class Sprite2D;

//==============================================================================
// class
//------------------------------------------------------------------------------
class NumberSprite : public iLayer2D, public CreateFunc<NumberSprite> {
public:
  bool init(int digit, const char* file);
  void update(void);
  void uninit(void);

  void setSize(float w, float h);
  void setColor(const D3DXCOLOR& color);
  void setColorHighLight(const D3DXCOLOR& color);
  void setVisible(bool visible);

  // 桁
  void setNumber(int num);

  // uv
  void setNumU(int u);
  void setNumV(int v);

private:
  // 桁数
  int _digit;
  int _texNumU;
  int _texNumV;

  Sprite2D** _numberSprite; // 普通のところ
  Sprite2D** _numberSpriteHightLight; // テカテカのところ
};

#endif
//EOF