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

namespace{
  const float kMovement = 2.0f;
}


//==============================================================================
// init
//------------------------------------------------------------------------------
bool ItemBomb::init(){
  _item = XFileObject::create("./data/model/bomb.x");
  _item->setPos(0.0f, 0.0f, 0.0f);
  _item->setScl(1.0f, 1.0f, 1.0f);
  this->addChild(_item);

  _is_use = false;
  _playerId = 0;
  _dripNum = 0;
  _moveDest = Vec3(0.0f, 0.0f, 0.0f);
  _player = NULL;

  return true;
}

//==============================================================================
// update
//------------------------------------------------------------------------------
void ItemBomb::update(){

  if (_is_use && _player){
    Vec3 playerPos = _player->getPos();
    Vec2 vec = Vec2(_pos.x - playerPos.x, _pos.z - playerPos.z);

    D3DXVec2Normalize(&vec, &vec);
    float length = D3DXVec2Length(&vec);
    if (length > 1.f) length = 1.f;

    float rot = atan2(vec.y, vec.x);
    _moveDest.x = cosf(rot) * length * kMovement;
    _moveDest.z = sinf(rot) * length * kMovement;

    _pos -= _moveDest;

    rot = D3DX_PI - atan2(_moveVec.z, _moveVec.x);
    _item->setRotY(rot);
  }
  _moveDest = Vec3(0.0f, 0.0f, 0.0f);
}

//==============================================================================
// uninit
//------------------------------------------------------------------------------
void ItemBomb::uninit(){
}


//EOF
