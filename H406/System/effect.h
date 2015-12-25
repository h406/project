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

#include "CreateFunc.h"
#include "iLayer.h"

using EffectHandle = int;
using EffectFileHandle = unsigned int;

//==============================================================================
// class
//------------------------------------------------------------------------------
class Effect : public iLayer, public CreateFunc<Effect> {
public:
  using CreateFunc::create;

  bool init();
  void uninit();

  void update();
  void draw(const Renderer* renderer);

  EffectFileHandle getID(const char* file);
  EffectHandle play(EffectFileHandle id,const Vec3& pos);
  EffectHandle play(const char* file,const Vec3& pos);

  void stop(EffectHandle effectid);
  void setEffectPos(EffectHandle effectid,const Vec3& pos);
  void setEffectRot(EffectHandle effectid,const Vec3& rot);
  void setEffectScl(EffectHandle effectid,const Vec3& scl);
  bool isExists(EffectHandle effectid) const;

protected:
  virtual NodeType getNodeType() const { return NodeType::effect; }

private:
  // �G�t�F�N�g
  EffekseerRenderer::Renderer* _effekseerRenderer;
  Effekseer::Manager* _effekseerManager;
  EffekseerSound::Sound*  _effekseerSound;

  map<string,EffectFileHandle> _effectMap;
  vector<Effekseer::Effect*> _effectList;
};
#endif
//EOF