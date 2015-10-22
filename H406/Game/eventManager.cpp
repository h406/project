//==============================================================================
//
// eventManager[eventManager.cpp]
// Author : Yasuaki Yamashita : 2015/10/22
//
//==============================================================================

//******************************************************************************
// include
//******************************************************************************
#include "eventManager.h"
#include "EventList.h"
#include "EventData.h"

//------------------------------------------------------------------------------
// イベント
//------------------------------------------------------------------------------
void EventManager::dispatchEvent(EventList event,void * userData) {
  auto row = _eventListener.equal_range(event);
  EventData data(event,userData);
  for(auto it = row.first; it != row.second; ++it) {
    (it->second)(&data);
  }
}

//------------------------------------------------------------------------------
// イベントプッシュ
//------------------------------------------------------------------------------
void EventManager::addEventListener(EventList event,std::function<void(EventData*)> function) {
  _eventListener.insert(pair<EventList,std::function<void(EventData*)>>(event,function));
}

//EOF