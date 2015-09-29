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

//==============================================================================
// App
//------------------------------------------------------------------------------
class App : public Window {
public:
  static App& instance(int width,int height);
  const Renderer* getRenderer() const { return _renderer; }

  void setBaceNode(node* baceNode);

protected:
  void update();

private:
  App(int width, int height);
  virtual ~App();

  node* _baceNode;
  Renderer* _renderer;
};


#endif
//EOF