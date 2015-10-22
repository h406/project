//==============================================================================
//
// node2d[node2d.h]
// Author : Yasuaki Yamashita : 2015/10/22
//
//==============================================================================

#pragma once

#ifndef _NODE2D_H_
#define _NODE2D_H_

#include "node.h"
#include "nodeType.h"

//==============================================================================
// class
//------------------------------------------------------------------------------
class node2d : public node {
public:

  const Vec2& getPos() const { return Vec2(_pos.x,_pos.y); }
  const Vec2& getRot() const { return Vec2(_rot.x,_rot.y); }
  const Vec2& getScl() const { return Vec2(_scl.x,_scl.y); }


  void setPos(float x,float y) { setPosX(x); setPosY(y); setPosZ(0.f); }
  void setRot(float x,float y) { setRotX(x); setRotY(y); setRotZ(0.f); }
  void setScl(float x,float y) { setSclX(x); setSclY(y); setSclZ(1.f); }
  void setRot(float y) { setRotY(y); }

  void setPos(const Vec2& pos) { setPosX(pos.x); setPosY(pos.y); setPosZ(0.f); }
  void setRot(const Vec2& rot) { setRotX(rot.x); setRotY(rot.y); setRotZ(0.f); }
  void setScl(const Vec2& scl) { setSclX(scl.x); setSclY(scl.y); setSclZ(1.f); }

  void setSizeW(float w) { _worldChenged = true; _scl.x = w; }
  void setSizeH(float h) { _worldChenged = true; _scl.y = h; }
  void setSize(float w,float h) { setSizeW(w); setSizeH(h); }

protected:
  virtual void updateWorldMtx();
  virtual NodeType getNodeType() const { return NodeType::normal2D; }


};

#endif
//EOF