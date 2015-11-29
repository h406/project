//==============================================================================
//
// itemAccel[itemAccel.cpp]
// Author : masato masuda : 2015/11/28
//
//==============================================================================

//******************************************************************************
// include
//******************************************************************************
#include "itemAccel.h"
#include "player.h"

namespace{
  const float kMovement = 2.0f;
  const int kAccleTime = 60 * 3;
}

//==============================================================================
// init
//------------------------------------------------------------------------------
bool ItemAccel::init(){
  _item = XFileObject::create("./data/model/manho-ru.x");
  _item->setScl(1.0f, 1.0f, 1.0f);
  _item->setPos(0.0f, 0.0f, 0.0f);
  this->addChild(_item);

  _is_use = false;
  _is_death = false;
  _owner = nullptr;
  _target = nullptr;
  _accelTime = 0;
  _moveDest = Vec3(0.0f, 0.0f, 0.0f);
  _radius = 20.0f;

  return true;
}

//==============================================================================
// update
//------------------------------------------------------------------------------
void ItemAccel::update(){
  if (_is_death == false){
    if (_is_use && _owner){

      _accelTime--;
      if (_accelTime < 0){
        _is_death = true;
        return;
      }
      _owner->setAccel(kMovement);
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
void ItemAccel::uninit(){
}

//==============================================================================
// use
//------------------------------------------------------------------------------
void ItemAccel::use(){
  _is_use = true;
  _accelTime = kAccleTime;
}


//EOF
