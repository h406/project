//==============================================================================
//
// Stage[stage.cpp]
// Author : Yasuaki Yamashita : 2015/10/19
//
//==============================================================================

//******************************************************************************
// include
//******************************************************************************
#include "stage.h"

namespace{
  const D3DXCOLOR kColorNone(1.0f, 1.0f, 1.0f, 1.0f);
  const D3DXCOLOR kColorDrip(1.0f, 1.0f, 1.0f, 1.0f);
  const D3DXCOLOR kColorPlayer1(0.4f, 0.7f, 1, 1);
  const D3DXCOLOR kColorPlayer2(1.0f, 0.8f, 0.3f, 1);
}

//==============================================================================
// init
//------------------------------------------------------------------------------
bool Stage::init(float stageSizeX,float stageSizeZ) {
  const Vec2 bordSize = Vec2(stageSizeX / (float)kNUM_X,stageSizeZ / (float)kNUM_Y);
  _stageSize = Vec2(stageSizeX,stageSizeZ);

  _instancing = Instancing3D::create("./data/texture/ground.png");
  this->addChild(_instancing); 

  for(int x = 0; x < kNUM_X; x++) {
    for(int y = 0; y < kNUM_Y; y++) {
      _fieldMap[x][y] = Sprite3D::create();
      _fieldMap[x][y]->setSize(bordSize.x - 5,bordSize.y - 5);
      _fieldMap[x][y]->setPos(Vec3(x * bordSize.x - stageSizeX * 0.5f + bordSize.x * 0.5f,00.1f,y * bordSize.y - stageSizeZ * 0.5f + bordSize.y * 0.5f));
      _fieldMap[x][y]->setRotX(D3DX_PI * 0.5f);
//      _fieldMap[x][y]->setVisible(false);
      _fieldMap[x][y]->setNumU(3);
      _fieldMap[x][y]->setNumV(1);
      _fieldMap[x][y]->setAnimID(1);
      _instancing->addChild(_fieldMap[x][y]);
    }
  }

  memset(_field,0,sizeof(_field));

  return true;
}

//==============================================================================
// update
//------------------------------------------------------------------------------
void Stage::update() {
//  for(int x = 0; x < kNUM_X; ++x) {
//    for(int y = 0; y < kNUM_Y; ++y) {
//      switch(_field[x][y]) {
//      case FIELD_ID::PLAYER_1:
//        _fieldMap[x][y]->setColor(kColorPlayer1);
//        break;
//      case FIELD_ID::PLAYER_2:
//        _fieldMap[x][y]->setColor(kColorPlayer2);
//        break;
//      case FIELD_ID::DRIP:
//        _fieldMap[x][y]->setColor(kColorDrip);
//        break;
//      case FIELD_ID::ITEM:
//        _fieldMap[x][y]->setColor(kColorNone);
//        break;
//      case FIELD_ID::NONE:
//        _fieldMap[x][y]->setColor(kColorNone);
//        break;
//      }
//    }
//  }
}

//==============================================================================
// set
//------------------------------------------------------------------------------
void Stage::setFieldID(int x,int y,Stage::FIELD_ID fieldid) {
  if(x >= 0 && x < kNUM_X && y >= 0 && y < kNUM_Y)
    _field[x][y] = fieldid;

  switch (_field[x][y]) {
  case FIELD_ID::PLAYER_1:
    _fieldMap[x][y]->setAnimID(0);
    _fieldMap[x][y]->setColor(kColorPlayer1);
    break;
  case FIELD_ID::PLAYER_2:
    _fieldMap[x][y]->setAnimID(0);
    _fieldMap[x][y]->setColor(kColorPlayer2);
    break;
  case FIELD_ID::DRIP:
    _fieldMap[x][y]->setAnimID(2);
    _fieldMap[x][y]->setColor(kColorDrip);
    break;
  case FIELD_ID::NONE:
    _fieldMap[x][y]->setAnimID(1);
    _fieldMap[x][y]->setColor(kColorNone);
    break;
  }
}

//==============================================================================
// get
//------------------------------------------------------------------------------
Stage::FIELD_ID Stage::getFieldID(int x,int y) const {
  if(x >= 0 && x < kNUM_X && y >= 0 && y < kNUM_Y)
    return _field[x][y];

  // error
  return FIELD_ID::NONE;
}

//==============================================================================
// uninit
//------------------------------------------------------------------------------
void Stage::uninit() {

}

//==============================================================================
// reset
//------------------------------------------------------------------------------
void Stage::reset(void){
  memset(_field, 0, sizeof(_field));

  for (int x = 0; x < kNUM_X; x++) {
    for (int y = 0; y < kNUM_Y; y++) {
      _fieldMap[x][y]->setColor(kColorNone);
      _fieldMap[x][y]->setAnimID(1);
    }
  }
}

//==============================================================================
// getFieldMapNum
//------------------------------------------------------------------------------
int Stage::getFieldMapNum(FIELD_ID id) const{
  int num = 0;

  for (int x = 0; x < kNUM_X; x++) {
    for (int y = 0; y < kNUM_Y; y++) {
      if (_field[x][y] == id){
        num++;
      }
    }
  }
  return num;
}


//EOF