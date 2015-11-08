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

namespace{
  const int kTEXTURE_U = 11;
  const int kTEXTURE_V = 2;
}

//==============================================================================
// init
//------------------------------------------------------------------------------
bool NumberSprite::init(int digit, const char* file)
{
  _digit = digit;
  _texNumU = kTEXTURE_U;
  _texNumV = kTEXTURE_V;

  _numberSprite = new Sprite2D*[digit];
  _numberSpriteHightLight = new Sprite2D*[digit];

  for (int i = 0; i < _digit; i++){
    _numberSprite[i] = Sprite2D::create(file);
    _numberSprite[i]->setAnimID(0);
    _numberSprite[i]->setNumU(kTEXTURE_U);
    _numberSprite[i]->setNumV(kTEXTURE_V);
    this->addChild(_numberSprite[i]);

    _numberSpriteHightLight[i] = Sprite2D::create(file);
    _numberSpriteHightLight[i]->setAnimID(0);
    _numberSpriteHightLight[i]->setNumU(_texNumU);
    _numberSpriteHightLight[i]->setNumV(_texNumV);
    this->addChild(_numberSpriteHightLight[i]);
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
  SafeDeleteArray(_numberSpriteHightLight);
}

//==============================================================================
// setNumber
//------------------------------------------------------------------------------
void NumberSprite::setNumber(int num)
{
  for (int i = 0; i < _digit; i++){
    int cur_num = ((int)num) % (int)pow(10.0f, _digit - i) / (int)pow(10.0f, _digit - i - 1);
    _numberSprite[i]->setAnimID(cur_num);
    _numberSpriteHightLight[i]->setAnimID(cur_num + _texNumU);
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

    _numberSpriteHightLight[i]->setSize(w, h);
    _numberSpriteHightLight[i]->setPosX((i * w) - posX);
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
// setColorHighLight
//------------------------------------------------------------------------------
void NumberSprite::setColorHighLight(const D3DXCOLOR& color)
{
  for (int i = 0; i < _digit; i++){
    _numberSpriteHightLight[i]->setColor(color);
  }
}

//==============================================================================
// setNumU
//------------------------------------------------------------------------------
void NumberSprite::setNumU(int u)
{
  _texNumU = u;
  for (int i = 0; i < _digit; i++){
    _numberSprite[i]->setNumU(u);
    _numberSpriteHightLight[i]->setNumU(u);
  }
}

//==============================================================================
// setNumV
//------------------------------------------------------------------------------
void NumberSprite::setNumV(int v)
{
  _texNumV = v;
  for (int i = 0; i < _digit; i++){
    _numberSprite[i]->setNumV(v);
    _numberSpriteHightLight[i]->setNumV(v);
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
    _numberSpriteHightLight[i]->setVisible(visible);
  }
}

//EOF
