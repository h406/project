//==============================================================================
//
// EventList[EventList.h]
// Author : Yasuaki Yamashita : 2015/10/22
//
//==============================================================================

#pragma once

#ifndef _EVENTLIST_H_
#define _EVENTLIST_H_

//==============================================================================
// eventlist
//------------------------------------------------------------------------------
enum class EventList {
  PLAYER_1_DRIP_GET,
  PLAYER_2_DRIP_GET,
  PLAYER_1_DRIP_USING,
  PLAYER_2_DRIP_USING,
  PLAYER_1_DRIP_RESET,
  PLAYER_2_DRIP_RESET,
  PLAYER_1_ITEM_GET,
  PLAYER_2_ITEM_GET,
  PLAYER_1_ROUND_WIN,
  PLAYER_2_ROUND_WIN,
  PLAYER_1_MAP_SET,
  PLAYER_2_MAP_SET,
  ROUND_START,
  ROUND_FINISH,
  ROUND_RESULT,
  ROUND_RESULT_START,
  ROUND_RESULT_END,
  NEXT_ROUND,
  PLAYER_1_GET_BOMB,
  PLAYER_2_GET_BOMB,
  PLAYER_1_USE_ITEM,
  PLAYER_2_USE_ITEM,
  ITEM_RESET,
  MAX,
};


#endif
//EOF