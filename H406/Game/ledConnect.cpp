//==============================================================================
//
// LedConnect[ledConnect.cpp]
// Author : Yasuaki Yamashita : 2015/12/04
//
//==============================================================================

//******************************************************************************
// include
//******************************************************************************
#include "ledConnect.h"

#ifdef _DEBUG
#pragma comment(lib, "D/websockets_static.lib")
#else
#pragma comment(lib, "R/websockets_static.lib")
#endif

#ifdef _DEBUG
#pragma comment(lib, "D/zlib_internal.lib")
#else
#pragma comment(lib, "R/zlib_internal.lib")
#endif


LedConnect* LedConnect::_insatnce;

//==============================================================================
// 
//------------------------------------------------------------------------------
LedConnect::LedConnect() {
  _isEnd = false;
  _insatnce = this;
  _SendData._event = LedEvent::None;
  _thread = thread(LedConnect::wsConnect, this);
}

//==============================================================================
// 
//------------------------------------------------------------------------------
LedConnect::~LedConnect() {
  _mutex.lock();
  _isEnd = true;
  _mutex.unlock();
  while(_isEnd) Sleep(1);
  _thread.detach();
}


int ShiftJisToUTF8(const char* szShiftJis,char** bufUTF8) {
  wchar_t* bufUnicode;

  // まずUniocdeに変換する
  // サイズを計算する
  int iLenUnicode = MultiByteToWideChar(CP_ACP,0,szShiftJis,strlen(szShiftJis) + 1, NULL,0);

  bufUnicode = new wchar_t[iLenUnicode];

  MultiByteToWideChar(CP_ACP,0,szShiftJis,strlen(szShiftJis) + 1,bufUnicode,MAX_PATH);
  // 次に、UniocdeからUTF8に変換する
  // サイズを計算する
  int iLenUtf8 = WideCharToMultiByte(CP_UTF8,0,bufUnicode,iLenUnicode,NULL,0, NULL,NULL);

  (*bufUTF8) = new char[iLenUtf8];

  WideCharToMultiByte(CP_UTF8,0,bufUnicode,iLenUnicode,*bufUTF8,iLenUtf8,NULL,NULL);

  SafeDeleteArray(bufUnicode);
  return iLenUtf8;
}


//==============================================================================
// WsConnect
//------------------------------------------------------------------------------
void LedConnect::wsConnect(LedConnect* instance) {
  libwebsocket_context *context;
  lws_context_creation_info info;
  int debug_level = 0;

  memset(&info,0,sizeof info);
  info.port = 7682;

  lws_set_log_level(debug_level,lwsl_emit_syslog);
  lwsl_notice("libwebsockets chat server -\n");

  libwebsocket_protocols protocols[] = {
    {
      "led",
      LedConnect::wsCallBackData,
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
    instance->_mutex.lock();
    if(instance->_SendData._event != LedEvent::None) {
      for(auto wsi : instance->_socketList) {
        libwebsocket_write(wsi,(unsigned char*)&instance->_SendData,sizeof(SendData),LWS_WRITE_BINARY);
      }
      instance->_SendData._event = LedEvent::None;
    }
    else if(!instance->_sendText.empty()) {
      char* text = nullptr;
      int length = ShiftJisToUTF8(instance->_sendText.c_str(),&text);
       for(auto wsi : instance->_socketList) {
         libwebsocket_write(wsi,(unsigned char*)text,length,LWS_WRITE_TEXT);
      }
      instance->_sendText = "";
      SafeDeleteArray(text);
    }
    instance->_mutex.unlock();

    int n = libwebsocket_service(context,10);
    if(n >= 0 && instance->_isEnd) {
      break;
    }
  }

  libwebsocket_context_destroy(context);
  
  instance->_isEnd = false;

}

//==============================================================================
// wsCallBackData
//------------------------------------------------------------------------------
int LedConnect::wsCallBackData(
  libwebsocket_context *,
  libwebsocket *wsi,
  libwebsocket_callback_reasons reason,
  void *,
  void *in,
  size_t ) {

  switch(reason) {
    // 新規接続
  case LWS_CALLBACK_ESTABLISHED:
  {
    _insatnce->_mutex.lock();
    _insatnce->_socketList.push_back(wsi);
    _insatnce->_mutex.unlock();
  }
  break;
  // クローズ
  case LWS_CALLBACK_WSI_DESTROY:
  {
    // 接続リスト
    _insatnce->_mutex.lock();
    auto it = remove_if(_insatnce->_socketList.begin(),_insatnce->_socketList.end(),[wsi](libwebsocket* obj) {return wsi == obj;});
    if(it != _insatnce->_socketList.end())
      _insatnce->_socketList.erase(it);
    _insatnce->_mutex.unlock();
  }
  break;

  // クローズ
  case LWS_CALLBACK_PROTOCOL_DESTROY:

    break;
  // 送信処理
  case LWS_CALLBACK_SERVER_WRITEABLE:
  {
    _insatnce->_mutex.lock();
    //libwebsocket_write(wsi,(unsigned char*)instance->_sendData,strlen(_instance->_sendData),LWS_WRITE_TEXT);
    //memset(_instance->_sendData,0,sizeof(_instance->_sendData));
    _insatnce->_mutex.unlock();
  }
  break;
  // 受信処理
  case LWS_CALLBACK_RECEIVE:
  {
    char* data = (char*)in;
    lwsl_notice("ReceiveMessage=[%s]\n",(const char*)in);
    // memcpy(_instance->_sendData,in,strlen((char*)in));
    // libwebsocket_callback_on_writable_all_protocol(libwebsockets_get_protocol(wsi));
  }
  break;

  }

  return 0;
}




//EOF