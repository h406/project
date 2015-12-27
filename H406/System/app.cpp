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
#include "Sprite2D.h"

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
  ,_input(nullptr)
  ,_fadeTime(0){
  _renderer = new Renderer();
  _renderer->init(this);

  auto fadeBG = Sprite2D::create();
  fadeBG->setSize((float)App::instance().getWindowSize().cx,(float)App::instance().getWindowSize().cy);
  fadeBG->setPos(App::instance().getWindowSize().cx * 0.5f,App::instance().getWindowSize().cy * 0.5f);
  fadeBG->setColor(D3DXCOLOR(1,1,1,1));
  _renderer->setFadeBG(fadeBG);

  _input = new Input();
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

  if(_nextScene != nullptr && _fadeTime == 1.f) {
    setBaceScene(_nextScene->create());
    SafeDelete(_nextScene);
  }

  if(_nextScene != nullptr) {
    _fadeTime += 0.1f;
    if(_fadeTime > 1.f) {
      _fadeTime = 1.f;
    }
  }
  else if(_fadeTime > 0) {
    _fadeTime -= 0.1f;
    if(_fadeTime < 0) {
      _fadeTime = 0;
    }
  }
  auto fadeBG = _renderer->getFadeBG();
  fadeBG->setColor(D3DXCOLOR(1,1,1,_fadeTime));

  if(_baceScene != nullptr) {
    _baceScene->updateChild();
    _renderer->update();
    _baceScene->updateMtxChild();
  }
}

//==============================================================================
// draw
//------------------------------------------------------------------------------
void App::draw() {
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