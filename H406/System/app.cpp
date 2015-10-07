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
#include "node.h"
#include "iScene.h"
#include "sound.h"

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
  ,_baceScene(nullptr)
  ,_sound(new Sound())
  ,_input(new Input()){
  _renderer = new Renderer(this);
}

//==============================================================================
// dest
//------------------------------------------------------------------------------
App::~App() {
  if(_baceScene != nullptr) {
    _baceScene->release();
    _baceScene = nullptr;
  }

  SafeDelete(_renderer);
  SafeDelete(_input);
  SafeDelete(_sound);
}

//==============================================================================
// update
//------------------------------------------------------------------------------
void App::update() {
  _input->update();

  if(_baceScene != nullptr) {
    _baceScene->updateChild();
    _renderer->update();
    _baceScene->updateMtxChild();
  }

  _renderer->draw(_baceScene);
}

//==============================================================================
// setBaceNode
//------------------------------------------------------------------------------
void App::setBaceScene(iScene* baceScene) {
  if(baceScene == nullptr) {
    return;
  }

  if(_baceScene != nullptr) {
    _baceScene->release();
  }

  _baceScene = baceScene;
}


//EOF