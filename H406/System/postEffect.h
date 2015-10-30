//==============================================================================
//
// PostEffect[postEffect.h]
// Author : Yasuaki Yamashita : 2015/10/30
//
//==============================================================================

#pragma once

#ifndef _POSTEFFECT_H_
#define _POSTEFFECT_H_

class iPostEffect;
class Sprite2D;
class Renderer;

//==============================================================================
// class
//------------------------------------------------------------------------------
class PostEffect {
public:
  PostEffect();
  ~PostEffect();

  void update();
  void draw(Renderer* renderer);

  void addPostEffect(iPostEffect* effect);
  void removePostEffect(iPostEffect* effect);

private:
  list<iPostEffect*> _postEffect;
  Sprite2D *_EffectSprite;
};

#endif
//EOF