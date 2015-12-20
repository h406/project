//==============================================================================
//
// wsInput[wsInput.cpp]
// Author : Yasuaki Yamashita : 2015/10/10
//
//==============================================================================

//******************************************************************************
// include
//******************************************************************************
#include "wsInput.h"

#ifdef _DEBUG
#pragma comment(lib, "D/websockets_static.lib")
#else
#pragma comment(lib, "R/websockets_static.lib")
#endif

#define _IS_Calibration_
#define _IS_TEXT_

WsInput* WsInput::_instance = nullptr;

//==============================================================================
// WsInput
//------------------------------------------------------------------------------
void WsInput::init(Input* input) {
  UnusedParam(input);

  _instance = this;
  _isend = false;
  memset(_sendData,0,sizeof(_sendData));
  memset(_press,0,sizeof(_press));
  memset(_trigger,0,sizeof(_trigger));
  memset(_release,0,sizeof(_release));
  memset(_repeat,0,sizeof(_repeat));
  memset(_recvData,0,sizeof(_recvData));
  memset(_cavRot,0,sizeof(_cavRot));
  memset(_cavCnt,0,sizeof(_cavCnt));

  thread tmp = thread(WsInput::wsConnect,this);
  _thread.swap(tmp);
}

//==============================================================================
// WsInput
//------------------------------------------------------------------------------
void WsInput::uninit() {
  _mutex.lock();
  _isend = true;
  _mutex.unlock();
  while(_isend) Sleep(1);
  _thread.detach();
}

//==============================================================================
// update
//------------------------------------------------------------------------------
void WsInput::update() {
  bool press[14] = {false};
  char title[256] = {0};

  //sprintf_s(title,"x=%1.5f y=%1.5f z=%1.5f push=%d",_recvData[0].rot.x,_recvData[0].rot.y,_recvData[0].rot.z,_recvData[0].isPush ? 1 : 0);
  //sprintf_s(title,"x=%f y=%f z=%f push=%d",_rot[0].x,_rot[0].y,_rot[0].z,_recvData[0].isPush ? 1 : 0);
  sprintf_s(title,"x=%1.5f y=%1.5f z=%1.5f push=%d",_cavRot[0].x - _recvData[0].rot.x,_cavRot[0].y - _recvData[0].rot.y,_cavRot[0].z - _recvData[0].rot.z,_recvData[0].isPush ? 1 : 0);
  //App::instance().setTitle(title);

  for(int i = 0; i < 4; i++) {
#ifdef _IS_Calibration_
    if(D3DXVec3Length(&_recvData[i].rot) == 0) {
      //continue;
    }
    if(_cavCnt[i] < 60) {
      _cavCnt[i]++;
      _cavRot[i] += _recvData[i].rot;
      continue;
    }
    else if(_cavCnt[i] == 60) {
      _cavRot[i] = _cavRot[i] / 60.f;
      _cavCnt[i]++;
    }
#endif

    press[(int)VK_INPUT::LEFT]  = _cavRot[i].x + _recvData[i].rot.x < -0.15f;
    press[(int)VK_INPUT::RIGHT] = _cavRot[i].x + _recvData[i].rot.x >  0.15f;
    press[(int)VK_INPUT::DOWN]  = _cavRot[i].y + _recvData[i].rot.y < -0.15f;
    press[(int)VK_INPUT::UP]    = _cavRot[i].y + _recvData[i].rot.y >  0.15f;
    press[(int)VK_INPUT::_1]    = _recvData[i].isPush;

    for(int nCntKey = 0; nCntKey < 14; nCntKey++) {
      // トリガー作成
      _trigger[i][nCntKey] = (press[nCntKey] != _press[i][nCntKey]) && press[nCntKey];
      // リリース作成
      _release[i][nCntKey] = (press[nCntKey] != _press[i][nCntKey]) && !press[nCntKey];
      // プレス作成
      _press[i][nCntKey] = press[nCntKey];
    }
  }
}

void da(int level,const char* text) {
  OutputDebugString(text);
}

//==============================================================================
// WsInput
//------------------------------------------------------------------------------
void WsInput::wsConnect(WsInput* wsinput) {
  libwebsocket_context *context;
  lws_context_creation_info info;
  int debug_level = 7;

  memset(&info,0,sizeof info);
  info.port = 7681;

  lws_set_log_level(debug_level,da);
  lwsl_notice("libwebsockets chat server -\n");

  libwebsocket_protocols protocols[] = {
    {
      "input",
      WsInput::wsCallBackData,
      0,
      128
    },
    {NULL,NULL,0,0} /* terminator */
  };

  info.iface = nullptr;
  info.protocols = protocols;
  info.ssl_cert_filepath = nullptr;
  info.ssl_private_key_filepath = nullptr;

  info.gid = -1;
  info.uid = -1;
  info.options = 0;
  info.user = nullptr;

  context = libwebsocket_create_context(&info);

  for(;;) {
    int n = libwebsocket_service(context,10);
    if(n >= 0 && wsinput->_isend) {
      break;
    }
  }

  libwebsocket_context_destroy(context);

  wsinput->_isend = false;
}

//==============================================================================
// wsCallBackData
//------------------------------------------------------------------------------
int WsInput::wsCallBackData(
  libwebsocket_context *context,
  libwebsocket *wsi,
  libwebsocket_callback_reasons reason,
  void *user,
  void *in,
  size_t len) {
  
  UnusedParam(context);
  UnusedParam(len);

  switch(reason) {
  // 新規接続
  case LWS_CALLBACK_ESTABLISHED:
  {
  }
  break;
  // クローズ
  case LWS_CALLBACK_PROTOCOL_DESTROY:
  {
  }
  break;
  // 送信処理d
  case LWS_CALLBACK_SERVER_WRITEABLE:
  {
  }
  break;
  // 受信処理
  case LWS_CALLBACK_RECEIVE:
  {
    _instance->_mutex.lock();
#ifdef _IS_TEXT_
    const char* test = (const char*)in;
    int playerID = test[0] - '0';
    float b = atof(&test[2]);
    switch(test[1])
    {
    case 'u':
      _instance->_recvData[playerID].isPush = false;
      break;
    case 'd':
      _instance->_recvData[playerID].isPush = true;
      break;
    case 'x':
      _instance->_recvData[playerID].rot.x = b;
      break;
    case 'y':
      _instance->_recvData[playerID].rot.y = b;
      break;
    case 'z':
      _instance->_recvData[playerID].rot.z = b;
      break;
    }

#else
    RecvData* data = (RecvData*)in;
    memcpy(&_instance->_recvData[data->playerID],data,sizeof(RecvData));
#endif
    _instance->_mutex.unlock();
  }
  break;
  }

  return 0;

}

bool WsInput::isPress(int id, VK_INPUT vk) const {
  return _press[id][(int)vk];
}

bool WsInput::isTrigger(int id, VK_INPUT vk) const {
  return _trigger[id][(int)vk];
}

bool WsInput::isRelease(int id, VK_INPUT vk) const {
  return _release[id][(int)vk];
}

bool WsInput::isRepeat(int id, VK_INPUT vk) const {
  _asm int 3
  return _repeat[id][(int)vk];
}

//EOF