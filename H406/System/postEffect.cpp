//==============================================================================
//
// PostEffect[postEffect.cpp]
// Author : Yasuaki Yamashita : 2015/10/30
//
//==============================================================================

//******************************************************************************
// include
//******************************************************************************
#include "postEffect.h"
#include "iPostEffect.h"
#include "Sprite2D.h"

//------------------------------------------------------------------------------
// PostEffect
//------------------------------------------------------------------------------
PostEffect::PostEffect() {
  _EffectSprite = Sprite2D::create();
  _EffectSprite->setSize((float)App::instance().getWindowSize().cx,(float)App::instance().getWindowSize().cy);
  _EffectSprite->setColor(D3DXCOLOR(1,1,1,1));
  _EffectSprite->setTexture((unsigned int)0);
  _EffectSprite->updateChild();
  _EffectSprite->updateMtxChild();
}

//------------------------------------------------------------------------------
// PostEffect
//------------------------------------------------------------------------------
PostEffect::~PostEffect() {
  // 
  for(iPostEffect* effect : _postEffect) {
    effect->uninit();
    SafeDelete(effect);
  }

  _postEffect.clear();

  // 
  _EffectSprite->release();
}

//------------------------------------------------------------------------------
// update
//------------------------------------------------------------------------------
void PostEffect::update() {
  for(iPostEffect* effect : _postEffect) {
    effect->update();
  }
}

//------------------------------------------------------------------------------
// draw
//------------------------------------------------------------------------------
void PostEffect::draw(Renderer* renderer) {
  for(iPostEffect* effect : _postEffect) {
    effect->draw(renderer, _EffectSprite);
  }
}

//------------------------------------------------------------------------------
// addPostEffect
//------------------------------------------------------------------------------
void PostEffect::addPostEffect(iPostEffect* effect) {
  effect->init();
  _postEffect.push_back(effect);
}

///==============================================================================
// remove 
//------------------------------------------------------------------------------
void PostEffect::removePostEffect(iPostEffect* effect) {
  auto it = remove_if(_postEffect.begin(),_postEffect.end(),[effect](iPostEffect* removeEffect) {return effect == removeEffect;});
  _postEffect.erase(it);
}

//EOF