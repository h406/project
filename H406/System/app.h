//==============================================================================
//
// app[app.h]
// Author : Yasuaki Yamashita : 2015/09/28
//
//==============================================================================

#pragma once

#ifndef _APP_H_
#define _APP_H_

#include "window.h"

class Renderer;
class Input;
class node;
class Sound;
using iScene = node;

class BaceNextScene {
public:
  virtual node* create() = 0;
};

template <class _SCENE>
class NextScene : public BaceNextScene {
public:
  node* create() {
    return _SCENE::create();
  }
};

//==============================================================================
// App
//------------------------------------------------------------------------------
class App : public Window {
public:
  static App& instance(int width = 0,int height = 0);
  const Renderer* getRenderer() const { return _renderer; }
  const Input* getInput() const { return _input; }
  Sound* getSound() { return _sound; }

  void setBaceScene(iScene* baceScene);

  template <class _SCENE>
  bool createNextScene() {
    if(_nextScene == nullptr && _fadeTime == 0) {
      _nextScene = new NextScene<_SCENE>();
      return true;
    }
    return false;
  }

protected:
  void update();

private:
  App(int width, int height);
  virtual ~App();

  iScene* _baceScene;
  Renderer* _renderer;
  Input* _input;
  Sound* _sound;

  BaceNextScene* _nextScene;
  float _fadeTime;
};


#endif
//EOF