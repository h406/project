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

//==============================================================================
// App
//------------------------------------------------------------------------------
class App : public Window {
public:
  static App& instance(int width = 0,int height = 0);
  const Renderer* getRenderer() const { return _renderer; }
  const Input* getInput() const { return _input; }

  void setBaceNode(node* baceNode);

protected:
  void update();

private:
  App(int width, int height);
  virtual ~App();

  node* _baceNode;
  Renderer* _renderer;
  Input* _input;
};


#endif
//EOF