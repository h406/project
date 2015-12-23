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
      _fieldMap[x][y]->setPos(Vec3(x * bordSize.x - stageSizeX * 0.5f + bordSize.x * 0.5f,0.1f,y * bordSize.y - stageSizeZ * 0.5f + bordSize.y * 0.5f));
      _fieldMap[x][y]->setRotX(D3DX_PI * 0.5f);
//      _fieldMap[x][y]->setTexture("./data/texture/field_none.png");
      _fieldMap[x][y]->setNumU(3);
      _fieldMap[x][y]->setNumV(1);
      _fieldMap[x][y]->setAnimID(1);
      _instancing->addChild(_fieldMap[x][y]);

      _fieldMapAdd[x][y] = Sprite3DAdditive::create();
      _fieldMapAdd[x][y]->setSize(bordSize.x - 5, bordSize.y - 5);
      _fieldMapAdd[x][y]->setPos(Vec3(x * bordSize.x - stageSizeX * 0.5f + bordSize.x * 0.5f, 0.2f, y * bordSize.y - stageSizeZ * 0.5f + bordSize.y * 0.5f));
      _fieldMapAdd[x][y]->setRotX(D3DX_PI * 0.5f);
      _fieldMapAdd[x][y]->setVisible(false);
      _fieldMapAdd[x][y]->setColor(D3DXCOLOR(1, 1, 1, 0));
      this->addChild(_fieldMapAdd[x][y]);
    }
  }

  memset(_field,0,sizeof(_field));
  _play = true;

  return true;
}

//==============================================================================
// update
//------------------------------------------------------------------------------
void Stage::update() {

  for (int x = 0; x < kNUM_X; ++x) {
    for (int y = 0; y < kNUM_Y; ++y) {

      if (_play){
        if (_fieldMapAdd[x][y]->isVisible() == true){
          D3DXCOLOR col = _fieldMapAdd[x][y]->getColor();
          col.a -= 0.02f;
          if (col.a < 0){
            _fieldMapAdd[x][y]->setVisible(false);
          }
          _fieldMapAdd[x][y]->setColor(col);
        }
      }else{
        //D3DXCOLOR col = _fieldMapAdd[x][y]->getColor();
        //col.a -= 0.03f;
        //if (col.a < 0.3f) col.a = 0.3f;
        //_fieldMapAdd[x][y]->setColor(col);
      }
    } // for y
  } // for x
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

    _fieldMapAdd[x][y]->setColor(D3DXCOLOR(0.7f, 0.7f, 1.0f, 1.0f));
//    _fieldMapAdd[x][y]->setColor(kColorNone);
    _fieldMapAdd[x][y]->setVisible(true);
    break;
  case FIELD_ID::PLAYER_2:
    _fieldMap[x][y]->setAnimID(0);
    _fieldMap[x][y]->setColor(kColorPlayer2);

    _fieldMapAdd[x][y]->setColor(D3DXCOLOR(1.0f, 1.0f, 0.7f, 1.0f));
//    _fieldMapAdd[x][y]->setColor(kColorNone);
    _fieldMapAdd[x][y]->setVisible(true);
    break;
  case FIELD_ID::DRIP:
    _fieldMap[x][y]->setAnimID(2);
    _fieldMap[x][y]->setColor(kColorDrip);
    _fieldMapAdd[x][y]->setVisible(false);
    break;
  case FIELD_ID::NONE:
    _fieldMap[x][y]->setAnimID(1);
    _fieldMap[x][y]->setColor(kColorNone);
    _fieldMapAdd[x][y]->setVisible(false);
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
void Stage::reset(FIELD_ID id){
  memset(_field, 0, sizeof(_field));
  _play = true;

  for (int x = 0; x < kNUM_X; x++) {
    for (int y = 0; y < kNUM_Y; y++) {
      _fieldMap[x][y]->setColor(kColorNone);
//      _fieldMap[x][y]->setTexture("./data/texture/field_none.png");
      _fieldMap[x][y]->setAnimID(1);

      switch (id)
      {
      case FIELD_ID::PLAYER_1:
        _fieldMapAdd[x][y]->setColor(D3DXCOLOR(0.2f, 0.5f, 1, 1.1f));
        break;
      case FIELD_ID::PLAYER_2:
        _fieldMapAdd[x][y]->setColor(D3DXCOLOR(1, 0.8f, 0.2f, 1.1f));
        break;
      case FIELD_ID::NONE:
        _fieldMapAdd[x][y]->setColor(D3DXCOLOR(1, 1, 1, 0.6f));
        break;
      default:
        _fieldMapAdd[x][y]->setColor(D3DXCOLOR(1, 1, 1, 0.6f));
        break;
      }
      _fieldMapAdd[x][y]->setVisible(true);
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

//==============================================================================
// getFieldMapNum
//------------------------------------------------------------------------------
void Stage::seekFiledMapIdNoVisible(FIELD_ID id)
{
  for (int x = 0; x < kNUM_X; x++) {
    for (int y = 0; y < kNUM_Y; y++) {
      if (_field[x][y] == id && _fieldMapAdd[x][y]->isVisible() == false){

        _fieldMapAdd[x][y]->setVisible(true);
        if (id == FIELD_ID::PLAYER_1){
          _fieldMapAdd[x][y]->setColor(D3DXCOLOR(0.2f, 0.5f, 1, 0.6f));
        } else if (id == FIELD_ID::PLAYER_2){
          _fieldMapAdd[x][y]->setColor(D3DXCOLOR(1, 1, 0.3f, 0.6f));
        }
        return;
      }
    }
  }
}



//EOF