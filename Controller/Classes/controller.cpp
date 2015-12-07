#include "controller.h"

USING_NS_CC;
using namespace ui;
using namespace network;
using namespace std;

Scene* Controller::createScene() {
  // 'scene' is an autorelease object
  auto scene = Scene::create();

  // 'layer' is an autorelease object
  auto layer = Controller::create();

  // add layer as a child to scene
  scene->addChild(layer);

  // return the scene
  return scene;
}


Controller::~Controller() {

}


bool Controller::init() {
  if(!Layer::init()) {
    return false;
  }

  _websocket = nullptr;

  // ドメインとポートは自分のサーバーに合わせる
  createWebsocket();

  Size visibleSize = Director::getInstance()->getVisibleSize();
  Vec2 origin = Director::getInstance()->getVisibleOrigin();
  _sprite = Sprite::create("HelloWorld.png");
  _sprite->setPosition(visibleSize * 0.5f);
  this->addChild(_sprite);

  // アクセラレーション
  Device::setAccelerometerEnabled(true);
  auto acclistener = EventListenerAcceleration::create(CC_CALLBACK_2(Controller::onAcceleration,this));
  Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(acclistener,this);

  // updateを呼ぶようにする
  this->scheduleUpdate();

  _sendData.isPush = false;
  _sendData.playerID = this->getPlayerID();
  _sendData.rot = Vec3(0,0,0);

  for(int i = 0; i < 5; i++) _key[i] = false;

  auto listener = cocos2d::EventListenerKeyboard::create();
  listener->onKeyPressed = [this](cocos2d::EventKeyboard::KeyCode keyCode,cocos2d::Event* keyEvent) {
    if(keyCode == cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW) {
      _key[0] = true;
    }
    if(keyCode == cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW) {
      _key[1] = true;
    }
    if(keyCode == cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW) {
      _key[2] = true;
    }
    if(keyCode == cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW) {
      _key[3] = true;
    }
    if(keyCode == cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_SHIFT) {
      _key[4] = true;
    }
  };

  listener->onKeyReleased = [this](cocos2d::EventKeyboard::KeyCode keyCode,cocos2d::Event* keyEvent) {
    if(keyCode == cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW) {
      _key[0] = false;
    }
    if(keyCode == cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW) {
      _key[1] = false;
    }
    if(keyCode == cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW) {
      _key[2] = false;
    }
    if(keyCode == cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW) {
      _key[3] = false;
    }
    if(keyCode == cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_SHIFT) {
      _key[4] = false;
    }
  };

  this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,this);

  return true;
}

void Controller::update(float delta) {
  if(_key[0]) {
    _sendData.rot.x = -1;
  }
  if(_key[1]) {
    _sendData.rot.x = 1;
  }
  if(_key[2]) {
    _sendData.rot.y = -1;
  }
  if(_key[3]) {
    _sendData.rot.y = 1;
  }
  _sendData.isPush = _key[4];

  if(_websocket->getReadyState() == WebSocket::State::OPEN) {
    _websocket->send((unsigned char*)&_sendData,sizeof(SendData));
    _sendData.rot = Vec3(0,0,0);
  }
}

vector<string> split(const string &str,char delim) {
  vector<string> res;
  size_t current = 0,found;
  while((found = str.find_first_of(delim,current)) != string::npos) {
    res.push_back(string(str,current,found - current));
    current = found + 1;
  }
  res.push_back(string(str,current,str.size() - current));
  return res;
}

void Controller::createWebsocket() {
  if(_websocket != nullptr) {
    _websocket->close();
    CC_SAFE_DELETE(_websocket);
  }

  auto del = split(getURL().c_str(), '/');
  vector<string> name;
  name.push_back(del[3]);
  _websocket = new WebSocket;
  _websocket->init(*this,getURL().c_str(),&name);
}

void Controller::onAcceleration(cocos2d::Acceleration* acc,cocos2d::Event* events) {
  _sendData.rot.x = acc->x;
  _sendData.rot.y = acc->y;
  _sendData.rot.z = acc->z;
}

void Controller::onOpen(WebSocket* ws) {
  _sprite->setVisible(false);
  this->unschedule("restart");
}

void Controller::onMessage(WebSocket* ws,const WebSocket::Data& data) {

}

void Controller::onClose(WebSocket* ws) {
  _sprite->setVisible(true);
  this->scheduleOnce([this](float delta) {
    createWebsocket();
  },1,"restart");
}

void Controller::onError(WebSocket* ws,const WebSocket::ErrorCode& error) {

}