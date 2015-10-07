//==============================================================================
//
// effect[effect.h]
// Author : Yasuaki Yamashita : 2015/10/07
//
//==============================================================================

#pragma once

#ifndef _EFFECT_H_
#define _EFFECT_H_

namespace EffekseerRenderer { class Renderer; }
namespace Effekseer {
  class Manager;
  class Effect;
}
namespace EffekseerSound { class Sound; }

class Renderer;

//==============================================================================
// class
//------------------------------------------------------------------------------
class Effect {
public:
  Effect(const Renderer*);
  ~Effect();

  void update();
  void draw(); 

  int getID(const char* file);

  int play(unsigned int id,const Vec3& pos);
  int play(const char* file,const Vec3& pos);

  void stop(int id);
  void setPos(int id,const Vec3& pos);
  void setRot(int id,const Vec3& rot);
  void setScl(int id,const Vec3& scl);
  bool isExists(int id) const;


private:
  // エフェクト
  EffekseerRenderer::Renderer* _effekseerRenderer;
  Effekseer::Manager* _effekseerManager;
  EffekseerSound::Sound*  _effekseerSound;

  map<string,unsigned int> _effectMap;
  vector<Effekseer::Effect*> _effectList;
};
#endif
//EOF