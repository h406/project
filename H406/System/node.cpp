//==============================================================================
//
// node[node.cpp]
// Author : Yasuaki Yamashita : 2015/06/29
//
//==============================================================================

//******************************************************************************
// include
//******************************************************************************
#include "node.h"

//==============================================================================
// node
//------------------------------------------------------------------------------
node::node()
  :_parent(nullptr)
  ,_zOrder(0)
  ,_zOrderChenged(false)
  ,_destroy(false)
  ,_pos(0.f,0.f,0.f)
  ,_rot(0.f,0.f,0.f)
  ,_scl(1.f,1.f,1.f)
  ,_worldChenged(true)
  ,_visible(true)
{
}

//==============================================================================
// updateChild
//------------------------------------------------------------------------------
void node::updateWorldMtx() {
  D3DXMATRIX mtxRot;
  D3DXMatrixRotationYawPitchRoll(&mtxRot,_rot.y,_rot.x,_rot.z);
  _mtxWorld._11 = _scl.x * mtxRot._11;
  _mtxWorld._12 = _scl.x * mtxRot._12;
  _mtxWorld._13 = _scl.x * mtxRot._13;
  _mtxWorld._14 = 0.0f;

  _mtxWorld._21 = _scl.y * mtxRot._21;
  _mtxWorld._22 = _scl.y * mtxRot._22;
  _mtxWorld._23 = _scl.y * mtxRot._23;
  _mtxWorld._24 = 0.0f;

  _mtxWorld._31 = _scl.z * mtxRot._31;
  _mtxWorld._32 = _scl.z * mtxRot._32;
  _mtxWorld._33 = _scl.z * mtxRot._33;
  _mtxWorld._34 = 0.0f;

  _mtxWorld._41 = _pos.x;
  _mtxWorld._42 = _pos.y;
  _mtxWorld._43 = _pos.z;
  _mtxWorld._44 = 1.0f;

  if(_parent != nullptr)
    _mtxWorld = _mtxWorld * _parent->getWorldMtx();
}


//==============================================================================
// updateChild
//------------------------------------------------------------------------------
void node::updateChild() {
  // 自分をアップデート
  this->update();

  // 削除リストチェック
  removeCheck();

  // もし死んでいたら
  if(_destroy == true) {
    return;
  }

  zOderCheck();
  
  // update
  for(node* obj : _childList) {
    obj->updateChild();
  }
}

//==============================================================================
// updateMtxChild
//------------------------------------------------------------------------------
void node::updateMtxChild() {
  if(_worldChenged) {
    updateWorldMtx();
  }

  // update
  for(node* obj : _childList) {
    if(_worldChenged) {
      obj->_worldChenged = true;
    }
    obj->updateMtxChild();
  }
}

//==============================================================================
// drawChild
//------------------------------------------------------------------------------
void node::drawChild(const Renderer* renderer, NodeType type) {
  if(_visible && type == this->getNodeType()) {
    this->draw(renderer);
  }

  // 削除リストチェック
  removeCheck();

  for(node* obj : _childList) {
    obj->drawChild(renderer, type);
  }
}

//==============================================================================
// release
//------------------------------------------------------------------------------
void node::release() {

  for(node* obj : _childList) {
    obj->release();
  }

  if(_parent != nullptr) {
    _parent->removeChild(this);
  }

  this->uninit();
  this->_destroy = true;

  delete this;
}

//==============================================================================
// removeCheck
//------------------------------------------------------------------------------
void node::removeCheck() {
  // 削除リストに溜まっていたら
  if(_removeList.size() > 0) {
    for(auto node_ : _removeList) {
      auto remove = [node_](node* obj) { return obj == node_; };
      auto it = remove_if(_childList.begin(),_childList.end(),remove);
      _childList.erase(it);
    }
    _removeList.clear();
  }
}

//==============================================================================
// addChild
//------------------------------------------------------------------------------
void node::addChild(node* node,int zOrder) {
  node->_zOrder = zOrder;
  node->_parent = this;
  _zOrderChenged = true;
  _childList.push_back(node);
}

//==============================================================================
// addChild
//------------------------------------------------------------------------------
void node::addChild(node* node) {
  node->_zOrder = 0;
  node->_parent = this;
  _zOrderChenged = true;
  _childList.push_back(node);
}

//==============================================================================
// remove
//------------------------------------------------------------------------------
void node::removeChild(node* node) {
  _removeList.push_back(node);
}

//==============================================================================
// zOderCheck
//------------------------------------------------------------------------------
void node::zOderCheck() {
  // zOrder
  if(_zOrderChenged) {
    _childList.sort([](node* obj1,node* obj2) {return obj1->_zOrder < obj2->_zOrder;});
    _zOrderChenged = false;
  }
}

//EOF