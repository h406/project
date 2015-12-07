//==============================================================================
//
// controller[controller.h]
// Author : Yasuaki Yamashita : 2015/12/07
//
//==============================================================================

#pragma once

#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "network/WebSocket.h"
#include <string>

struct SendData {
  cocos2d::Vec3 rot;
  bool isPush;
  int playerID;
};

//==============================================================================
// class
//------------------------------------------------------------------------------
class Controller : public cocos2d::Layer, public cocos2d::network::WebSocket::Delegate {
public:
  static cocos2d::Scene* createScene();

  virtual bool init();

  ~Controller();

  CREATE_FUNC(Controller);

  static const std::string& getURL(const char* url = nullptr) {
    static std::string _url = "";
    if(url != nullptr) _url = url;
    return _url;
  }

  static const int getPlayerID(int id = -1) {
    static int _id = -1;
    if(id != -1) _id = id;
    return _id;
  }

  void update(float delta);


private:
  cocos2d::network::WebSocket* _websocket;
  cocos2d::Sprite* _sprite;
  SendData _sendData;
  bool _key[5];

  void createWebsocket();

  void onAcceleration(cocos2d::Acceleration* acc,cocos2d::Event* events);

  //webSocket
  virtual void onOpen(cocos2d::network::WebSocket* ws);
  virtual void onMessage(cocos2d::network::WebSocket* ws,const cocos2d::network::WebSocket::Data& data);
  virtual void onClose(cocos2d::network::WebSocket* ws);
  virtual void onError(cocos2d::network::WebSocket* ws,const cocos2d::network::WebSocket::ErrorCode& error);
};


#endif
//EOF