//==============================================================================
//
// StageBlock[stageBlock.h]
// Author : Yasuaki Yamashita : 2015/10/19
//
//==============================================================================

#pragma once

#ifndef _STAGEBLOCK_H_
#define _STAGEBLOCK_H_

#include "CreateFunc.h"

//==============================================================================
// 
//------------------------------------------------------------------------------
class StageBlock : public iLayer,public CreateFunc < StageBlock > {
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

  bool init();
  void update();
  void uninit();

  FIELD_ID getFieldID(int x,int y) const;
  void setFieldID(int x,int y,FIELD_ID id);

private:
  Sprite3D* _fieldMap[kNUM_X][kNUM_Y];
  FIELD_ID _field[kNUM_X][kNUM_Y];

};


#endif
//EOF