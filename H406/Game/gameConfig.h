//==============================================================================
//
// gameConfig[gameConfig.h]
// Author : masato masuda : 2015/10/27
//
//==============================================================================
#pragma once

#ifndef _GAME_CONFIG_H_
#define _GAME_CONFIG_H_

namespace GameConfig {
  const int kRAND_SEED = 1;
  const int kPLAYER_MAX = 2;
  const int kONE_ROUND_TIME = 60;
  const int kTIME_MAX = kONE_ROUND_TIME * 60 + 59;
  const int kROUND_MAX = 3;
};

#endif

// EOF