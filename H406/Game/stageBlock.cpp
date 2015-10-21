//==============================================================================
//
// stageBlock[stageBlock.cpp]
// Author : Yasuaki Yamashita : 2015/10/19
//
//==============================================================================

//******************************************************************************
// include
//******************************************************************************
#include "stageBlock.h"

//==============================================================================
// init
//------------------------------------------------------------------------------
bool StageBlock::init() {
  const Vec2 bordSize = Vec2(1000 / (float)kNUM_X,1000 / (float)kNUM_Y);

  for(int x = 0; x < kNUM_X; x++) {
    for(int y = 0; y < kNUM_Y; y++) {
      _fieldMap[x][y] = Sprite3D::create();
      _fieldMap[x][y]->setSize(bordSize.x - 5,bordSize.y - 5);
      _fieldMap[x][y]->setPos(Vec3(x * bordSize.x - 1000.f * 0.5f + bordSize.x * 0.5f,00.1f,y * bordSize.y - 1000.f * 0.5f + bordSize.y * 0.5f));
      _fieldMap[x][y]->setRotX(D3DX_PI * 0.5f);
      this->addChild(_fieldMap[x][y]);
    }
  }

  memset(_field,0,sizeof(_field));

  return true;
}

//==============================================================================
// update
//------------------------------------------------------------------------------
void StageBlock::update() {

  for(int x = 0; x < kNUM_X; ++x) {
    for(int y = 0; y < kNUM_X; ++y) {
      switch(_field[x][y]) {
      case FIELD_ID::PLAYER_1:
        _fieldMap[x][y]->setColor(D3DXCOLOR(0,0,1,1));
        break;
      case FIELD_ID::PLAYER_2:
        _fieldMap[x][y]->setColor(D3DCOLOR_RGBA(255,183,76,255));
        break;
      case FIELD_ID::ITEM:
        _fieldMap[x][y]->setColor(D3DCOLOR_RGBA(255,3,846,255));
        break;
      case FIELD_ID::NONE:
        _fieldMap[x][y]->setColor(D3DXCOLOR(1,1,1,1));
        break;
      }
    }
  }
}

//==============================================================================
// set
//------------------------------------------------------------------------------
void StageBlock::setFieldID(int x,int y,StageBlock::FIELD_ID fieldid) {
  if(x >= 0 && x < kNUM_X && y >= 0 && y < kNUM_Y)
    _field[x][y] = fieldid;
}

//==============================================================================
// get
//------------------------------------------------------------------------------
StageBlock::FIELD_ID StageBlock::getFieldID(int x,int y) const {
  if(x >= 0 && x < kNUM_X && y >= 0 && y < kNUM_Y)
    return _field[x][y];

  // error
  return FIELD_ID::NONE;
}

//==============================================================================
// uninit
//------------------------------------------------------------------------------
void StageBlock::uninit() {

}

//EOF