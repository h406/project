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
    ITEM,
  };

  bool init(float stageSizeX,float stageSizeZ);
  void update();
  void uninit();

  FIELD_ID getFieldID(int x,int y) const;
  void setFieldID(int x,int y,FIELD_ID id);
  void reset(void);

  const Vec2& getStageSize() const { return _stageSize; }

private:
  Sprite3D* _fieldMap[kNUM_X][kNUM_Y];
  FIELD_ID _field[kNUM_X][kNUM_Y];

  Vec2 _stageSize;
};


#endif
//EOF