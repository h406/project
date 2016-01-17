//==============================================================================
//
// itemManhole[itemManhole.cpp]
// Author : masato masuda : 2015/11/27
//
//==============================================================================

//******************************************************************************
// include
//******************************************************************************
#include "itemManhole.h"
#include "player.h"
#include "eventManager.h"
#include "EventList.h"
#include "BaceScene.h"
#include "shadow.h"

namespace{
  const int kFreezTime = 60 * 2;
  const float kPlayerJumpPower = 15.0f;
  const float kJumpPower = 50.0f;
}

//==============================================================================
// init
//------------------------------------------------------------------------------
bool ItemManhole::init(){
  _item = XFileObject::create("./data/model/manhole.x");
  _item->setScl(1.35f, 1.2f, 1.35f);
  _item->setPos(0.0f, 0.0f, 0.0f);
  this->addChild(_item);

  _is_use = false;
  _is_death = false;
  _owner = nullptr;
  _target = nullptr;
  _moveDest = Vec3(0.0f, 0.0f, 0.0f);
  _moveVec = Vec3(0.0f, 0.0f, 0.0f);
  _radius = 40.0f;
  _frameCount = 0;
  _fieldID.x = -1;
  _fieldID.y = -1;

  auto shadow = Shadow::create();
  shadow->setColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.55f));
  shadow->setScl(1.2f, 1.2f, 1.2f);
  shadow->setOffsetY(0.35f);
  this->addChild(shadow);

  return true;
}

//==============================================================================
// update
//------------------------------------------------------------------------------
void ItemManhole::update(){
  if (_is_death == false){
    if (_is_use && _owner){

      Vec3 pos = _item->getPos();
      Vec3 rot = _item->getRot();

      _frameCount--;
      if (_frameCount < 0 && pos.y <= -100.0f){
          _is_death = true;
        return;
      }
      // プレイヤーフリーズ処理
      if (_frameCount > 0){
        _owner->setFreeze(true);
      }

      _moveDest.y -= 4.0f;
      _moveVec.y += (_moveDest.y - _moveVec.y) * 0.1f;
//      _pos.y += _moveVec.y;
//      _rot.x += 0.25f;
//      _rot.y += 0.1f;
      pos.y += _moveVec.y;
      rot.x += 0.25f;
      rot.y += 0.1f;
      _item->setPos(pos);
      _item->setRot(rot);
    }
  }
  _moveDest = Vec3(0.0f, 0.0f, 0.0f);
}

//==============================================================================
// uninit
//------------------------------------------------------------------------------
void ItemManhole::uninit(){
}

//==============================================================================
// use
//------------------------------------------------------------------------------
void ItemManhole::use(){
  if (_event == nullptr) return;
  auto _effect = BaceScene::instance()->getEffect();
  _is_use = true;
  _frameCount = 0;

  const int is_plus = rand() % 3;
  if (is_plus == 0){
    // 失敗
    const Vec3 pos(_owner->getPos().x, 0.0f, _owner->getPos().z);
    _owner->setFreezePos(pos);
    _owner->setMoveVec(Vec3(0.0f, 0.0f, 0.0f));
    _frameCount = kFreezTime;
    _moveVec.y += kJumpPower;
    App::instance().getSound()->play("./data/sound/se/manhole_ng.wav", false);
    int id = _effect->play("Manhole.efk", _pos);
    _effect->setEffectScl(id, Vec3(1.5f, 1.5f, 1.5f));
  } else{
    // 成功
    _is_death = true;
    int plus = _owner->getMaxDripNum();
    _event->dispatchEvent(EventList(int(EventList::PLAYER_1_DRIP_GET) + _owner->getPlayerID()), (void*)plus);
    _owner->addDripNum(plus);
    _owner->jump(kPlayerJumpPower);
    App::instance().getSound()->play("./data/sound/se/manhole_ok.wav", false);
  }
}


//EOF
