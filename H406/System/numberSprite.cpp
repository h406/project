//==============================================================================
//
// NumberSprite[numberSprite.cpp]
// Author : masato masuda : 2015/10/17
//
//==============================================================================

//******************************************************************************
// include
//******************************************************************************
#include "numberSprite.h"
#include "Sprite2D.h"

namespace {
}

//==============================================================================
// init
//------------------------------------------------------------------------------
bool NumberSprite::init(int digit) {

  _digit = digit;
  _num = 0;

  _numberSprite = new Sprite2D*[digit];
  for (int i = 0; i < _digit; i++){
    _numberSprite[i] = Sprite2D::create("./data/texture/num.png");
    _numberSprite[i]->init();
    _numberSprite[i]->setAnimID(0);
    this->addChild(_numberSprite[i]);
  }

  return true;
}

bool NumberSprite::init(int digit, const char* file) {
  _textureID = App::instance().getRenderer()->getTexture()->createTexture(file);
  return init(digit);
}

//==============================================================================
// update
//------------------------------------------------------------------------------
void NumberSprite::update() {
}

//==============================================================================
// uninit
//------------------------------------------------------------------------------
void NumberSprite::uninit() {
  SafeDeleteArray(_numberSprite);
}

//==============================================================================
// setNumber
//------------------------------------------------------------------------------
void NumberSprite::setNumber(int num)
{
  _num = num;

  for (int i = 0; i < _digit; i++){
    int cur_num = ((int)num) % (int)pow(10.0f, _digit - i) / (int)pow(10.0f, _digit - i - 1);
    _numberSprite[i]->setAnimID(cur_num);
  }
}

//==============================================================================
// setSize
//------------------------------------------------------------------------------
void NumberSprite::setSize(float w, float h)
{
  _scl.x = w; _scl.y = h;

  for (int i = 0; i < _digit; i++){
    _numberSprite[i]->setSize(w, h);
  }
  setPos(_pos);
}

//==============================================================================
// setColor
//------------------------------------------------------------------------------
void NumberSprite::setColor(const D3DXCOLOR& color)
{
  for (int i = 0; i < _digit; i++){
    _numberSprite[i]->setColor(color);
  }
}

//==============================================================================
// setPos
//------------------------------------------------------------------------------
void NumberSprite::setPos(const Vec3& pos)
{
  _pos = pos;

  for (int i = 0; i < _digit; i++){
    float pos_x = pos.x + (_scl.x  * i) - (_scl.x * (_digit/2));
    _numberSprite[i]->setPos(pos_x, pos.y, pos.z);
  }
}

//==============================================================================
// setNumU
//------------------------------------------------------------------------------
void NumberSprite::setNumU(int u)
{
  for (int i = 0; i < _digit; i++){
    _numberSprite[i]->setNumU(u);
  }
}

//==============================================================================
// setNumV
//------------------------------------------------------------------------------
void NumberSprite::setNumV(int v)
{
  for (int i = 0; i < _digit; i++){
    _numberSprite[i]->setNumV(v);
  }
}

//==============================================================================
// setVisible
//------------------------------------------------------------------------------
void NumberSprite::setVisible(bool visible)
{
  node::setVisible(visible);

  for (int i = 0; i < _digit; i++){
    _numberSprite[i]->setVisible(visible);
  }

/*
  bool prev_check = true;
  if (visible == true){
    for (int i = 0; i < _digit; i++){

      int cur_num = ((int)_num) % (int)pow(10.0f, _digit - i) / (int)pow(10.0f, _digit - i - 1);
      if (cur_num == 0){
        if (prev_check == true){
          _numberSprite[i]->setVisible(false);
          prev_check = true;
        }
      }
      else{
        prev_check = false;
        _numberSprite[i]->setVisible(true);
      }
    }
  }
  else{
    for (int i = 0; i < _digit; i++){
      _numberSprite[i]->setVisible(false);
    }
  }
*/
}

//EOF
