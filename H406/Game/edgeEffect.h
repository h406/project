//==============================================================================
//
// edgeEffect[edgeEffect.h]
// Author : Yasuaki Yamashita : 2015/10/30
//
//==============================================================================

#pragma once

#ifndef _EDGE_EFFECT_H_
#define _EDGE_EFFECT_H_

//==============================================================================
// class
//------------------------------------------------------------------------------
class EdgeEffect : public iPostEffect{
public:
  bool init();
  void update();
  void draw(Renderer *renderer,Sprite2D* sprite);
};

#endif
//EOF