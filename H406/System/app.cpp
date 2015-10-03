//==============================================================================
//
// app[app.cpp]
// Author : Yasuaki Yamashita : 2015/09/28
//
//==============================================================================

//******************************************************************************
// include
//******************************************************************************
#include "app.h"
#include "renderer.h"
#include "input.h"

//==============================================================================
// instance
//------------------------------------------------------------------------------
App& App::instance(int width,int height) {
  static App app(width,height);
  return app;
}

//==============================================================================
// const
//------------------------------------------------------------------------------
App::App(int width,int height)
  : Window(width,height)
  ,_baceNode(nullptr)
  ,_renderer(new Renderer(this))
  ,_input(new Input()){

  
}

//==============================================================================
// dest
//------------------------------------------------------------------------------
App::~App() {
  if(_baceNode != nullptr) {
    _baceNode->release();
    _baceNode = nullptr;
  }

  SafeDelete(_renderer);
  SafeDelete(_input);
}

//==============================================================================
// update
//------------------------------------------------------------------------------
void App::update() {
  if(_baceNode != nullptr)
    _baceNode->updateChild();

  _renderer->draw(_baceNode);
}

//==============================================================================
// setBaceNode
//------------------------------------------------------------------------------
void App::setBaceNode(node* baceNode) {
  if(baceNode == nullptr) {
    return;
  }

  if(_baceNode != nullptr) {
    _baceNode->release();
  }

  _baceNode = baceNode;
}


//EOF