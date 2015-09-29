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
  ,_pos(0.f,0.f,0.f)
  ,_rot(0.f,0.f,0.f)
  ,_scl(1.f,1.f,1.f)
  ,_destroy(false)
{
}

//==============================================================================
// updateChild
//------------------------------------------------------------------------------
void node::updateChild() {
  // �������A�b�v�f�[�g
  this->update();

  // �폜���X�g�`�F�b�N
  removeCheck();

  // ��������ł�����
  if(_destroy == true) {
    return;
  }

  // zOrder
  if(_zOrderChenged) {
    _childList.sort([](node*& obj1,node*& obj2) {return obj1->_zOrder < obj2->_zOrder;});
    _zOrderChenged = false;
  }
  
  // update
  for(node*& obj : _childList) {
    obj->updateChild();
  }
}

//==============================================================================
// drawChild
//------------------------------------------------------------------------------
void node::drawChild(const Renderer* renderer) {
  this->draw(renderer);
  
  // �폜���X�g�`�F�b�N
  removeCheck();

  for(node*& obj : _childList) {
    obj->drawChild(renderer);
  }
}

//==============================================================================
// release
//------------------------------------------------------------------------------
void node::release() {

  for(node*& obj : _childList) {
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
  // �폜���X�g�ɗ��܂��Ă�����
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

//EOF