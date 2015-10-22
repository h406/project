//==============================================================================
//
// eventManager[eventManager.h]
// Author : Yasuaki Yamashita : 2015/10/22
//
//==============================================================================

#pragma once

#ifndef _EVENTMANAGER_H_
#define _EVENTMANAGER_H_

enum class EventList;
class EventData;

//==============================================================================
// class
//------------------------------------------------------------------------------
class EventManager {
public:
  // �C�x���g
  void dispatchEvent(EventList event,void* userData = nullptr);

  // �C�x���g�v�b�V��
  void addEventListener(EventList event,std::function<void(EventData*)> function);

private:
  multimap<EventList,std::function<void(EventData*)>> _eventListener;

};

#endif
//EOF