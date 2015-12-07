//==============================================================================
//
// wsinput[wsinput.h]
// Author : Yasuaki Yamashita : 2015/10/04
//
//==============================================================================

#pragma once

#ifndef _WSINPUT_H_
#define _WSINPUT_H_

#include "iInput.h"
#include "vInput.h"


struct RecvData {
  Vec3 rot;
  bool isPush;
  int playerID;
};

//==============================================================================
// 
//==============================================================================
class WsInput : public iInput {
public:
  void init(Input* input);
  void uninit(void);
  void update(void);

  bool isPress(int id, VK_INPUT vk) const;
  bool isTrigger(int id, VK_INPUT vk) const;
  bool isRelease(int id, VK_INPUT vk) const;
  bool isRepeat(int id, VK_INPUT vk) const;

  static void wsConnect(WsInput* wsinput);
  static int wsCallBackData(libwebsocket_context *,libwebsocket *,libwebsocket_callback_reasons,void *,void *,size_t);

protected:

private:
  static WsInput* _instance;

  char _sendData[256];
  bool _isend;
  mutex _mutex;
  thread _thread;

  RecvData _recvData[4];

  bool _press[4][(int)VK_INPUT::MAX];
  bool _trigger[4][(int)VK_INPUT::MAX];
  bool _release[4][(int)VK_INPUT::MAX];
  bool _repeat[4][(int)VK_INPUT::MAX];
};

#endif
//EOF