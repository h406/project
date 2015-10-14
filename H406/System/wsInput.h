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

//==============================================================================
// 
//==============================================================================
class WsInput : public iInput {
public:
  void init(Input* input);
  void uninit(void);
  void update(void);

  bool isPress(VK_INPUT vk) const;
  bool isTrigger(VK_INPUT vk) const;
  bool isRelease(VK_INPUT vk) const;
  bool isRepeat(VK_INPUT vk) const;

  static void wsConnect(WsInput* wsinput);
  static int wsCallBackData(libwebsocket_context *,libwebsocket *,libwebsocket_callback_reasons,void *,void *,size_t);

protected:

private:
  static WsInput* _instance;

  char _sendData[256];
  bool _isend;
  mutex _mutex;
  thread _thread;

  Vec3 _jairo;

  bool _press[(int)VK_INPUT::MAX];
  bool _trigger[(int)VK_INPUT::MAX];
  bool _release[(int)VK_INPUT::MAX];
  bool _repeat[(int)VK_INPUT::MAX];
};

#endif
//EOF