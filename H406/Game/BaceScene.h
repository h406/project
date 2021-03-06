//==============================================================================
//
// BaceScene[BaceScene.h]
// Author : Yasuaki Yamashita : 2015/11/11
//
//==============================================================================

#pragma once

#ifndef _BACESCENE_H_
#define _BACESCENE_H_

#include "CreateFunc.h"

class Stage;
class LedConnect;

//==============================================================================
// class
//------------------------------------------------------------------------------
class BaceScene : public iScene,public CreateFunc<BaceScene> {
public:
  using CreateFunc::create;

  bool init();
  void update();
  void uninit();

  void setCurScene(iScene* scene);
  iScene* getCurScene() const { return _curScene; }
  Stage* getStage() const { return _stage; }
  Effect* getEffect() const { return _effect; }
  LedConnect* getLedConnect() const { return _ledConnect; }

  static BaceScene* instance() { return _instance; }

private:
  iScene* _curScene;
  Stage* _stage;

  iScene* _releaseScene;
  
  static BaceScene* _instance;

  Effect* _effect;

  LedConnect* _ledConnect;

};

#endif
//EOF