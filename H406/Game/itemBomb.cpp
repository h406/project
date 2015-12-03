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

namespace{
  const float kMovement = 3.0f;
  const float kOffsetPosY = 3.0f;
}

//==============================================================================
// init
//------------------------------------------------------------------------------
bool ItemBomb::init(){
  _item = XFileObject::create("./data/model/pet.x");
  _item->setScl(3.0f, 3.0f, 3.0f);
  _item->setPos(0.0f, 0.0f, 0.0f);
  this->addChild(_item);

  _is_use = false;
  _is_death = false;
  _hitEnable = true;
  _owner = nullptr;
  _target = nullptr;
  _dripNum = 0;
  _moveDest = Vec3(0.0f, 0.0f, 0.0f);
  _radius = 40.0f;

  return true;
}

//==============================================================================
// update
//------------------------------------------------------------------------------
void ItemBomb::update(){
  if (_is_death == false){
    if (_is_use && _owner){
      if (_hitEnable == false) return;

      // Ž€‚ñ‚¾
      if (_dripNum <= 0){
        _is_death = true;
        return;
      }
      Vec3 targetPos = _target->getPos();
      Vec2 vec = Vec2(_pos.x - targetPos.x, _pos.z - targetPos.z);

      D3DXVec2Normalize(&vec, &vec);
      float length = D3DXVec2Length(&vec);
      if (length > 1.f) length = 1.f;

      float rot = atan2(vec.y, vec.x);
      _moveDest.x = cosf(rot) * length * kMovement;
      _moveDest.z = sinf(rot) * length * kMovement;
      _pos -= _moveDest;

      rot = D3DX_PI - atan2(_moveDest.z, _moveDest.x);
      rot -= D3DX_PI / 2;
      _item->setRotY(rot);
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
  _is_use = true;
  _dripNum = _owner->getDripNum();
  _pos = _owner->getPos();
  _pos.y = kOffsetPosY;
  if (_event){
    _event->dispatchEvent(EventList(int(EventList::PLAYER_1_DRIP_RESET) + _owner->getPlayerID()), nullptr);
  }
}


//EOF
