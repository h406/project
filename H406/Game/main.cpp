//==============================================================================
//
// main[main.cpp]
// Author : Yasuaki Yamashita : 2015/09/28
//
//==============================================================================
#include "title.h"
#include "game.h"
#include "edgeEffect.h"
#include "ssaoEffect.h"
#include "dirlightEffect.h"
#include "parpix.h"
#include "test.h"

//------------------------------------------------------------------------------
// WinMain
//------------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE,HINSTANCE,LPSTR,int) {
  App& app = App::instance(1280,720);

  // タイトル
  app.setTitle("H406");

  // シェーダ適用
  //app.getRenderer()->getPostEffect()->addPostEffect(new DirlightEffect());
  //app.getRenderer()->getPostEffect()->addPostEffect(new Parpix());
  //app.getRenderer()->getPostEffect()->addPostEffect(new EdgeEffect());
  //app.getRenderer()->getPostEffect()->addPostEffect(new SsaoEffect());
  //app.getRenderer()->getPostEffect()->addPostEffect(new TestEffect());
  
  // 最初のシーン
  app.setBaceScene(Game::create());

  return app.run();
}

//EOF