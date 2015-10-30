//==============================================================================
//
// main[main.cpp]
// Author : Yasuaki Yamashita : 2015/09/28
//
//==============================================================================
#include "title.h"
#include "game.h"
#include "edgeEffect.h"

//------------------------------------------------------------------------------
// WinMain
//------------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE,HINSTANCE,LPSTR,int) {
  App& app = App::instance(1280,720);

  // タイトル
  app.setTitle("H406");

  // シェーダ適用
  app.getRenderer()->getPostEffect()->addPostEffect(new EdgeEffect());

  // 最初のシーン
  app.setBaceScene(Game::create());

  return app.run();
}

//EOF