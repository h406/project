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
#include "fxaa.h"
#include "SelectScene.h"

#include "BaceScene.h"

//------------------------------------------------------------------------------
// WinMain
//------------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE,HINSTANCE,LPSTR,int) {
  const int windowWidth  = int(1280 * 0.5f);
  const int windowHeight = int(720  * 0.5f);
  //App& app = App::instance(windowWidth, windowHeight);
  App& app = App::instance(1280, 720);

  // タイトル
  app.setTitle("H406");

//#ifndef _DEBUG
  // シェーダ適用
  //app.getRenderer()->getPostEffect()->addPostEffect(new TestEffect());
  //app.getRenderer()->getPostEffect()->addPostEffect(new FxaaEffect());
  app.getRenderer()->getPostEffect()->addPostEffect(new DirlightEffect());
  app.getRenderer()->getPostEffect()->addPostEffect(new Parpix());
  //app.getRenderer()->getPostEffect()->addPostEffect(new EdgeEffect());
  //app.getRenderer()->getPostEffect()->addPostEffect(new SsaoEffect());
  //app.getRenderer()->getPostEffect()->addPostEffect(new TestEffect());
//#endif


  auto scene = BaceScene::create();
  scene->setCurScene(Title::create());
  
  // 最初のシーン
  app.setBaceScene(scene);

  return app.run();
}


//EOF