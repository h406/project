//==============================================================================
//
// parpixEffect[parpix.h]
// Author : Yasuaki Yamashita : 2015/10/31
//
//==============================================================================

#pragma once

#ifndef _PARPIX_H_
#define _PARPIX_H_

//==============================================================================
// class
//------------------------------------------------------------------------------
class Parpix : public iPostEffect {
public:
  bool init();
  void update();
  void draw(Renderer *renderer,Sprite2D* sprite);
  void uninit();
};

#endif
//EOF