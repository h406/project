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
#pragma comment(lib, "websockets_d.lib")
#else
#pragma comment(lib, "websockets.lib")
#endif

WsInput* WsInput::_instance = nullptr;

//==============================================================================
// WsInput
//------------------------------------------------------------------------------
void WsInput::init(Input* input) {
  _instance = this;
  _isend = false;
  memset(_sendData,0,sizeof(_sendData));
  memset(_press,0,sizeof(_press));
  memset(_trigger,0,sizeof(_trigger));
  memset(_release,0,sizeof(_release));
  memset(_repeat,0,sizeof(_repeat));
  _jairo = Vec3(0,0,0);
  _thread.swap(thread(WsInput::wsConnect,this));
}

//==============================================================================
// WsInput
//------------------------------------------------------------------------------
void WsInput::uninit() {
  _mutex.lock();
  _isend = true;
  _mutex.unlock();
  _thread.detach();
}

//==============================================================================
// update
//------------------------------------------------------------------------------
void WsInput::update() {
  for(auto& it : _press) {
    it = false;
  }

  if(abs(_jairo.x) > 10) {
    if(_jairo.x > 0) {
      _press[(int)VK_INPUT::RIGHT] = true;
    }
    else {
      _press[(int)VK_INPUT::LEFT] = true;
    }
  }
  if(abs(_jairo.y) > 10) {
    if(_jairo.y < 0) {
      _press[(int)VK_INPUT::UP] = true;
    }
    else {
      _press[(int)VK_INPUT::DOWN] = true;
    }
  }
}

//==============================================================================
// WsInput
//------------------------------------------------------------------------------
void WsInput::wsConnect(WsInput* wsinput) {
  libwebsocket_context *context;
  lws_context_creation_info info;
  int debug_level = 0;

  memset(&info,0,sizeof info);
  info.port = 7681;

  lws_set_log_level(debug_level,lwsl_emit_syslog);
  lwsl_notice("libwebsockets chat server -\n");

  libwebsocket_protocols protocols[] = {
    {
      "chat",
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
    int n = libwebsocket_service(context,50);
    if(n >= 0 && wsinput->_isend) {
      break;
    }
  }

  libwebsocket_context_destroy(context);
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
  // 送信処理
  case LWS_CALLBACK_SERVER_WRITEABLE:
  {
    _instance->_mutex.lock();
    libwebsocket_write(wsi,(unsigned char*)_instance->_sendData,strlen(_instance->_sendData),LWS_WRITE_TEXT);
    memset(_instance->_sendData,0,sizeof(_instance->_sendData));
    _instance->_mutex.unlock();
  }
  break;
  // 受信処理
  case LWS_CALLBACK_RECEIVE:
  {
    char* data = (char*)in;
    lwsl_notice("ReceiveMessage=[%s]\n",(const char*)in);
    // memcpy(_instance->_sendData,in,strlen((char*)in));
    // libwebsocket_callback_on_writable_all_protocol(libwebsockets_get_protocol(wsi));

    float dat = (float)atof(&data[1]);
    char d = data[0];

    _instance->_mutex.lock();
    if(d == 'b')
      _instance->_jairo.y = dat;
    if(d == 'g')
      _instance->_jairo.z = dat;
    if(d == 'a')
      _instance->_jairo.x = dat;

    _instance->_mutex.unlock();

    //_instance->_mutex.lock();
    //if(key == 0) {
    //  _instance->_press[(int)VK_INPUT::LEFT] = d;
    //}
    //if(key == 1) {
    //  _instance->_press[(int)VK_INPUT::UP] = d;
    //}
    //if(key == 2) {
    //  _instance->_press[(int)VK_INPUT::RIGHT] = d;
    //}
    //if(key == 3) {
    //  _instance->_press[(int)VK_INPUT::DOWN] = d;
    //}
    //_instance->_mutex.unlock();
  }
  break;

  }

  return 0;
}


bool WsInput::isPress(VK_INPUT vk) const {
  return _press[(int)vk];
}

bool WsInput::isTrigger(VK_INPUT vk) const {
  return false;
}

bool WsInput::isRelease(VK_INPUT vk) const {
  return false;
}

bool WsInput::isRepeat(VK_INPUT vk) const {
  return false;
}

//EOF