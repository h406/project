//==============================================================================
//
// node[node.h]
// Author : Yasuaki Yamashita : 2015/06/29
//
//==============================================================================

#pragma once

#ifndef _NODE_H_
#define _NODE_H_

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

  // create
  template<class Derived, class... Args>
  static Derived* create(Args&&... args) {
    auto p = new Derived();
    if(p->init(std::forward<Args>(args)...)) {
      return p;
    }
    else {
      delete p;
      return nullptr;
    }
  }

  // update
  virtual void updateChild();
  void drawChild(const Renderer* renderer);

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

  const Vec3& getPos() { return _pos; }
  const Vec3& getRot() { return _rot; }
  const Vec3& getScl() { return _scl; }

  void setPos(const Vec3& pos) { _worldChenged = true; _pos = pos; }
  void setRot(const Vec3& rot) { _worldChenged = true; _rot = rot; }
  void setScl(const Vec3& scl) { _worldChenged = true; _scl = scl; }

  const Matrix& getWorldMtx() { return _mtxWorld; }

protected:
  // node
  node();
  virtual ~node() {};

private:

  // 削除チェック
  void removeCheck();

  void zOderCheck();

  void updateWorldMtx();

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

  bool _destroy;


  Matrix _mtxWorld;

  // 位置など
  Vec3 _pos;
  Vec3 _rot;
  Vec3 _scl;

  bool _worldChenged;
};

#endif
//EOF
