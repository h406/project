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
class NumberSprite : public DrawObject,public CreateFunc<NumberSprite> {
public:
  using CreateFunc::create;

  bool init(){ return true; }
  bool init(int digit);
  bool init(int digit, const char* file);
  virtual void update();
  void uninit();

  void setSize(float w, float h);
  void setColor(const D3DXCOLOR& color);

  void setPos(float x, float y, float z){ setPos(Vec3(x, y, z)); }
  void setPos(const Vec3& pos);

  void setVisible(bool visible);

  // åÖ
  void setNumber(int num);

  // uv
  void setNumU(int u);
  void setNumV(int v);

protected:
  virtual NodeType getNodeType() const { return NodeType::normal2D; }

private:

  // åÖêî
  int _digit;

  // êî
  int _num;

  Sprite2D** _numberSprite;

  D3DXCOLOR _color;
};

#endif
//EOF