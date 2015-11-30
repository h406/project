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

namespace{
  const int kFreezTime = 60 * 2;
  const float kJumpPower = 20.0f;
}

//==============================================================================
// init
//------------------------------------------------------------------------------
bool ItemManhole::init(){
  _item = XFileObject::create("./data/model/manho-ru.x");
  _item->setScl(1.0f, 1.0f, 1.0f);
  _item->setPos(0.0f, 0.0f, 0.0f);
  this->addChild(_item);

  _is_use = false;
  _is_death = false;
  _owner = nullptr;
  _target = nullptr;
  _moveDest = Vec3(0.0f, 0.0f, 0.0f);
  _radius = 20.0f;
  _frameCount = 0;

  return true;
}

//==============================================================================
// update
//------------------------------------------------------------------------------
void ItemManhole::update(){
  if (_is_death == false){
    if (_is_use && _owner){

      _frameCount--;
      if (_frameCount < 0){
        _is_death = true;
        return;
      }
      // ˆ—
      _owner->setFreeze(true);
    }
  }

  if (_owner != nullptr){
    _item->setVisible(false);
  }else{
    _item->setVisible(true);
  }
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
  _is_use = true;
  _frameCount = 0;

  const int is_plus = rand() % 2;
  if (is_plus == 0){
    int plus = _owner->getMaxDripNum();
    if (plus > 9) plus = 9;
    
    if (_event){
      _event->dispatchEvent(EventList(int(EventList::PLAYER_1_DRIP_GET) + _owner->getPlayerID()), (void*)plus);
      _owner->addDripNum(plus);
      _owner->jump(kJumpPower);
    }
  }else{
    const Vec3 pos(_owner->getPos().x, 0.0f, _owner->getPos().z);
    _owner->setFreezePos(pos);
    _owner->setMoveVec(Vec3(0.0f, 0.0f, 0.0f));
    _frameCount = kFreezTime;
  }
}


//EOF
