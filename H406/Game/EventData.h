//==============================================================================
//
// EventData[EventData.h]
// Author : Yasuaki Yamashita : 2015/10/22
//
//==============================================================================

#pragma once

#ifndef _EVENTDATA_H_
#define _EVENTDATA_H_

enum class EventList;

//==============================================================================
//  class
//------------------------------------------------------------------------------
class EventData {
public:
  EventData(EventList event,void* userData)
    :_event(event)
    ,_userData(userData) {
  };

  EventList getEvent() const { return _event; }
  void* getUserData() const { return _userData; }

private:
  void* _userData;
  EventList _event;
};

#endif
//EOF