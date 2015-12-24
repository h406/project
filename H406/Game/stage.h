//==============================================================================
//
// Stage[Stage.h]
// Author : Yasuaki Yamashita : 2015/10/19
//
//==============================================================================

#pragma once

#ifndef _STAGE_H_
#define _STAGE_H_

#include "CreateFunc.h"

//==============================================================================
// 
//------------------------------------------------------------------------------
class Stage : public iLayer,public CreateFunc < Stage > {
public:
  using CreateFunc::create;

  static const int kNUM_X = 12;
  static const int kNUM_Y = 12;

  enum class FIELD_ID {
    NONE = 0,
    PLAYER_1,
    PLAYER_2,
    DRIP,
    ITEM,
  };

  bool init(float stageSizeX,float stageSizeZ);
  void update();
  void uninit();

  FIELD_ID getFieldID(int x,int y) const;
  void setFieldID(int x,int y,FIELD_ID id);
  void reset(FIELD_ID id);

  const Vec3& getFieldMapPos(int x, int y) const { return _fieldMap[x][y]->getPos(); }
  const Vec2& getStageSize() const { return _stageSize; }

  // あまり使わないでね
  int getFieldMapNum(FIELD_ID id) const;

  // 指定したIDのマップ光らせるやつ
  void seekFiledMapIdNoVisible(FIELD_ID id);

  // updateのON/OFF
  bool isPlay(){ return _play; }
  void setPlay(bool val){ _play = val; }

private:
  Instancing3D* _instancing;

  InstancingObject3D* _fieldMap[kNUM_X][kNUM_Y];
  FIELD_ID _field[kNUM_X][kNUM_Y];

  // 光る床
  Sprite3DAdditive* _fieldMapAdd[kNUM_X][kNUM_Y];;

  Vec2 _stageSize;

  // updateのON/OFF
  bool _play;
};


#endif
//EOF