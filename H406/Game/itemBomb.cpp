//==============================================================================
//
// itemBomb[itemBomb.cpp]
// Author : masato masuda : 2015/11/18
//
//==============================================================================

//******************************************************************************
// include
//******************************************************************************
#include "itemBomb.h"
#include "player.h"
#include "eventManager.h"
#include "EventList.h"
#include "BaceScene.h"
#include "shadow.h"

namespace{
  const float kMovement = 3.0f;
  const float kOffsetPosY = 25.0f;
  const float kMaxSpeed = 12.0f;
}

//==============================================================================
// init
//------------------------------------------------------------------------------
bool ItemBomb::init(){
  _item = XFileObject::create("./data/model/pet.x");
  _item->setScl(1.3f, 1.3f, 1.3f);
  _item->setPos(0.0f, kOffsetPosY, 0.0f);
  this->addChild(_item);

  _is_use = false;
  _is_death = false;
  _hitEnable = true;
  _owner = nullptr;
  _target = nullptr;
  _dripNum = 0;
  _moveDest = Vec3(0.0f, 0.0f, 0.0f);
  _radius = 40.0f;
  _fieldID.x = -1;
  _fieldID.y = -1;
  _movementPos = 0.0f;
  _movementRot = 0.0f;
  _nowFrame = 0;
  _allFrame = 90;
  _effectID = -1;

  auto shadow = Shadow::create();
  shadow->setColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.6f));
  shadow->setScl(0.6f, 0.6f, 0.6f);
  this->addChild(shadow);

  return true;
}

//==============================================================================
// update
//------------------------------------------------------------------------------
void ItemBomb::update(){
  if (_is_death == false){
    if (_is_use && _owner){
      auto _effect = BaceScene::instance()->getEffect();

      // 死んだ
      if (_nowFrame >= _allFrame && _hitEnable == false){
        _is_death = true;
        _effect->stop(_effectID);
        return;
      }

      // 塗り終わった
      if (_dripNum <= 0 && _hitEnable == true){
        _hitEnable = false;
        _nowFrame = 0;
        _allFrame = 60;
      }

      // 移動処理
      Vec3 targetPos = _target->getPos();
      Vec2 vec = Vec2(_pos.x - targetPos.x, _pos.z - targetPos.z);

      D3DXVec2Normalize(&vec, &vec);
      float length = D3DXVec2Length(&vec);
      if (length > 1.f) length = 1.f;

      float posmove = 0.0f;
      float rotmove = 0.0f;
      if (_hitEnable){
        // 加速
        posmove = accelerated(0.2f, kMaxSpeed, _nowFrame, _allFrame);
        rotmove = accelerated(0.0f, 1.0f, _nowFrame, _allFrame);
        _movementPos = posmove;
        _movementRot = rotmove;
      }else{
        // 減速
//        posmove = 0.0f;
        posmove = accelerated(_movementPos, 0.0f, int(_nowFrame), int(_allFrame));
        rotmove = accelerated(_movementRot, 0.0f, _nowFrame, _allFrame);
        posmove *= 0.1f;
      }
      if (_nowFrame < _allFrame) _nowFrame++;

      // 移動量加算
      float rot = atan2(vec.y, vec.x);
      _moveDest.x = cosf(rot) * length * posmove;
      _moveDest.z = sinf(rot) * length * posmove;
      _pos -= _moveDest;
      _rot.y += rotmove;

      // エフェクト
      Vec3 effectPos = _pos;
      effectPos.y = _item->getPos().y;
      _effect->setEffectPos(_effectID, effectPos);
      _effect->setEffectRot(_effectID, _rot);
    }
  }

  if (_is_use == false && _owner != nullptr){
    _item->setVisible(false);
  }else{
    _item->setVisible(true);
  }

  _moveDest = Vec3(0.0f, 0.0f, 0.0f);
}

//==============================================================================
// uninit
//------------------------------------------------------------------------------
void ItemBomb::uninit(){
}

//==============================================================================
// use
//------------------------------------------------------------------------------
void ItemBomb::use(){
  auto _effect = BaceScene::instance()->getEffect();
  _is_use = true;
  _dripNum = _owner->getDripNum() + 3;
//  _pos = _owner->getPos();
//  _pos.y = 0.0f;
  _item->setPosY(kOffsetPosY * 0.5f);
  _item->setRotX(D3DX_PI * 0.5f);

  _effectID = _effect->play("Bomchuu.efk", _pos);
  _effect->setEffectScl(_effectID, Vec3(1.5f,1.5f,1.5f));

  if (_event){
    _event->dispatchEvent(EventList(int(EventList::PLAYER_1_DRIP_RESET) + _owner->getPlayerID()), nullptr);
  }
}

//==============================================================================
// effectStop
//------------------------------------------------------------------------------
void ItemBomb::effectStop(){
  if (_effectID < 0) return;
  auto _effect = BaceScene::instance()->getEffect();
  _effect->stop(_effectID);
}

//==============================================================================
// accelerated
//------------------------------------------------------------------------------
float ItemBomb::accelerated(float start, float dest, int nowFrame, int allFrame){
  //移動間隔を算出
  float rate = float(nowFrame) / float(allFrame);
  //移動倍率の算出
  rate *= rate;
  //経過時間を算出して移動値を算出する
  float ret = (float(start) + float(dest - start) * rate);
  return ret;
}


//EOF
