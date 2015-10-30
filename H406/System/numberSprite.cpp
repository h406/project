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

//==============================================================================
// init
//------------------------------------------------------------------------------
bool NumberSprite::init(int digit, const char* file)
{
  _digit = digit;

  _numberSprite = new Sprite2D*[digit];
  for (int i = 0; i < _digit; i++){
    _numberSprite[i] = Sprite2D::create(file);
    _numberSprite[i]->setAnimID(0);
    this->addChild(_numberSprite[i]);
  }

  return true;
}

//==============================================================================
// update
//------------------------------------------------------------------------------
void NumberSprite::update(void)
{
}

//==============================================================================
// uninit
//------------------------------------------------------------------------------
void NumberSprite::uninit(void)
{
  SafeDeleteArray(_numberSprite);
}

//==============================================================================
// setNumber
//------------------------------------------------------------------------------
void NumberSprite::setNumber(int num)
{
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
  for (int i = 0; i < _digit; i++){
    const float posX = ((w * _digit) * 0.5f) - (w * 0.5f);
    _numberSprite[i]->setSize(w, h);
    _numberSprite[i]->setPosX((i * w) - posX);
  }
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
}

//EOF
