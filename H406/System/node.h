//==============================================================================
//
// node[node.h]
// Author : Yasuaki Yamashita : 2015/06/29
//
//==============================================================================

#pragma once

#ifndef _NODE_H_
#define _NODE_H_

#include "nodeType.h"

//==============================================================================
//
//------------------------------------------------------------------------------
class Renderer;

//==============================================================================
// node
//------------------------------------------------------------------------------
class node {
public :
  virtual void update() = 0;
  virtual void draw(const Renderer* ) {};
  virtual void uninit() = 0;

  // update
  virtual void updateChild();
  void drawChild(const Renderer* renderer,NodeType type);

  // release
  void release();

  // add
  void addChild(node* node,int zOrder);
  void addChild(node* node);

  // remove
  void removeChild(node* node);

  // parent
  void setParent(node* node) {
    _parent = node;
  }

  const Vec3& getPos() const { return _pos; }
  const Vec3& getRot() const { return _rot; }
  const Vec3& getScl() const { return _scl; }

  void setPosX(float x) { _worldChenged = true; _pos.x = x; }
  void setPosY(float y) { _worldChenged = true; _pos.y = y; }
  void setPosZ(float z) { _worldChenged = true; _pos.z = z; }

  void setRotX(float x) { _worldChenged = true; _rot.x = x; }
  void setRotY(float y) { _worldChenged = true; _rot.y = y; }
  void setRotZ(float z) { _worldChenged = true; _rot.z = z; }

  void setSclX(float x) { _worldChenged = true; _scl.x = x; }
  void setSclY(float y) { _worldChenged = true; _scl.y = y; }
  void setSclZ(float z) { _worldChenged = true; _scl.z = z; }

  void setPos(float x, float y, float z) { setPosX(x); setPosY(y); setPosZ(z); }
  void setRot(float x, float y, float z) { setRotX(x); setRotY(y); setRotZ(z); }
  void setScl(float x, float y, float z) { setSclX(x); setSclY(y); setSclZ(z); }

  void setPos(const Vec3& pos) { _worldChenged = true; _pos = pos; }
  void setRot(const Vec3& rot) { _worldChenged = true; _rot = rot; }
  void setScl(const Vec3& scl) { _worldChenged = true; _scl = scl; }

  const Matrix& getWorldMtx() { return _mtxWorld; }

  void updateMtxChild();

  void setVisible(bool visible) { _visible = visible; }
  bool isVisible() const { return _visible; }

  virtual NodeType getNodeType() const { return NodeType::default; }

protected:
  // node
  node();
  virtual ~node() {};

  // 位置など
  Vec3 _pos;
  Vec3 _rot;
  Vec3 _scl;

  virtual void updateWorldMtx();

  Matrix _mtxWorld;

  bool _worldChenged;

  node* getParent() const { return _parent; }

private:

  // 削除チェック
  void removeCheck();

  void zOderCheck();

  // 親
  node* _parent;

  // 子
  std::list<node*> _childList;

  // 削除するリスト
  std::list<node*> _removeList;

  // 
  int _zOrder;

  // 
  bool _zOrderChenged;

  // 削除
  bool _destroy;

  // 表示するか
  bool _visible;
};

#endif
//EOF
