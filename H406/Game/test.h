//==============================================================================
//
// testEffect[test.h]
// Author : Yasuaki Yamashita : 2015/10/31
//
//==============================================================================

#pragma once

#ifndef _TEST_EFFECT_H_
#define _TEST_EFFECT_H_

//==============================================================================
// class
//------------------------------------------------------------------------------
class TestEffect : public iPostEffect {
public:
  bool init();
  void update();
  void draw(Renderer *renderer,Sprite2D* sprite);
  void uninit();
};

#endif
//EOF