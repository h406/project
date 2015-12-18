//==============================================================================
//
// LedConnect[ledConnect.h]
// Author : Yasuaki Yamashita : 2015/12/04
//
//==============================================================================

#pragma once

#ifndef _LEDCONNECT_H_
#define _LEDCONNECT_H_

enum LedEvent {
  None = 0,
  Connect,
  MoveTitle,
  MoveSelect,
  MoveGame,
  MoveResult,
  ShowGauge,
  ShowLead,
  ShowSec,
  ShowFinish,
  ShowRoundWin,
};

struct SendData {
  LedEvent _event;
  union {
    int _s32;
    unsigned int _u32;
    float _f32;
    char _s8[4];
    unsigned char _u8[4];
  };
};

//==============================================================================
// 
//------------------------------------------------------------------------------
class LedConnect {
public:
  LedConnect();
  ~LedConnect();

  static void wsConnect(LedConnect* instnace);
  static int wsCallBackData(libwebsocket_context *,libwebsocket *,libwebsocket_callback_reasons,void *,void *,size_t);


  void sendEvent(LedEvent events) {
    _mutex.lock();
    memset(&_SendData._s32,0,4);
    _SendData._event = events;
    _mutex.unlock();
  }

  template<typename _T>
  void sendEvent(LedEvent events,_T* data) {
    _mutex.lock();
      memcpy(&_SendData._s32,data,4);
    _SendData._event = events;
    _mutex.unlock();
  }

  void sendText(const char* text) { 
    _mutex.lock();
    _sendText = text;
    _mutex.unlock();
  }

private:
  
  SendData _SendData;
  string _sendText;

  mutex _mutex;
  thread _thread;
  vector<libwebsocket*> _socketList;
  static LedConnect* _insatnce;
  bool _isEnd;
};

#endif
//EOF