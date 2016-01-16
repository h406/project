//==============================================================================
//
// SelectScene[SelectScene.cpp]
// Author : Yasuaki Yamashita : 2015/10/04
//
//==============================================================================

//******************************************************************************
// include
//******************************************************************************
#include "SelectScene.h"
#include "game.h"
#include "BaceScene.h"
#include "ShuchuSen.h"
#include "ledConnect.h"
#include "BaceScene.h"
#include "playerStatus.h"
#include "dataManager.h"

#include "../QRDecode/qrdecode.h"

//#define _QR_DISABLE__

namespace{
  const D3DXCOLOR kPlayerColor[2] = { D3DXCOLOR(0.2f, 0.5f, 1, 0.6f), D3DXCOLOR(1, 0.8f, 0.3f, 0.6f) };

  const float kModelLength       = 200.0;
  const float kModelLengthSpeed  = 1.2f;
  const float kFSpeed            = 0.02f;
  const float kFspeedAdd         = 0.0023f;

  const Vec3 kModelPos           = Vec3(250.f,0,0);
  const Vec3 kMainCameraPos      = Vec3(0, 120.f, 130.f);
  const Vec3 kPlayerCameraOffset = Vec3(100.f, 0.f, 100.f);
}

//==============================================================================
// init
//------------------------------------------------------------------------------
bool SelectScene::init() {
  _windowScl = (float)(App::instance().getWindowSize().cx / 1280.f);

  if (DataManager::instance().isInstance()){
    DataManager::instance().uninit();
  }
  DataManager::instance().init();

  _curSelectPlayer = 0;
  _playerStickID[0] = 1;
  _playerStickID[1] = 2;
  _inputPermit = false;

  _modelLength = kModelLength;
  _lengthSpeed = kModelLengthSpeed;
  _fspeed = kFSpeed;
  _frame = 0;
  _gameF = 0;
  _stickDecision = false;

  _stickLayer = nullptr;
  for (int i = 0; i < 2; i++){
    _stickHandlePlayer[i] = nullptr;
    _stickBarPlayer[i] = nullptr;
  }
  for (int i = 0; i < 3; i++){
    _stickHandle[i] = nullptr;
    _stickBar[i] = nullptr;
  }

  // 杖生成
  _stickLayer = XFileObject::create("./data/model/Elephant_bar.x");
  this->addChild(_stickLayer);

  _stickLayer->setVisible(false);
  _stickLayer->setPos(kModelPos);

  for (int i = 0; i < 3; i++){
    const auto& barStatus = PlayerStatus::kStickBarStatus[(int)i + 1];
    const auto& handStatus = PlayerStatus::kStickHandleStatus[(int)i + 1];

    _stickBar[i] = XFileObject::create(barStatus.fileName);
    _stickLayer->addChild(_stickBar[i]);

    _stickHandle[i] = XFileObject::create(handStatus.fileName);
    _stickLayer->addChild(_stickHandle[i]);

    _stickBar[i]->setScl(Vec3(0.7f, 0.7f, 0.7f));
    _stickHandle[i]->setScl(Vec3(0.7f, 0.7f, 0.7f));
  }

  // カメラ
  auto camera = App::instance().getRenderer()->getCamera();
  _camera = camera->createCamera();
//  _camera->setPosP({ 0.f, 100.f, 400.f });
  _camera->setPosP({ 0.f, kMainCameraPos.y, kModelPos.x + kMainCameraPos.z });
  _camera->setPosR({ kModelPos.x, 20, 0 });

  _playerCamera = camera->createCamera();
  _playerCamera->setPosP({ 0, 0, 0 });
  _playerCamera->setPosR({ 0, 0, 0 });
  camera->setCamera(_camera, 100);

  _s = ShuchuSen::create("./data/texture/image.png");
  _s->setSize(1500.f * _windowScl, 1500.f * _windowScl);
  _s->setPos(App::instance().getWindowSize().cx*0.5f, App::instance().getWindowSize().cy*0.5f);
  _s->setVisible(false);
  this->addChild(_s);

  _waku = Sprite2D::create("./data/texture/qr_waku_00.png");
  _waku->setSize((float)App::instance().getWindowSize().cx, (float)App::instance().getWindowSize().cy);
  _waku->setPos(App::instance().getWindowSize().cx*0.5f, App::instance().getWindowSize().cy*0.5f);
  _waku->setColor(D3DXCOLOR(1, 1, 1, 0));
  this->addChild(_waku);

  _back = Sprite2D::create("./data/texture/select_01.png");
  _back->setSize((float)App::instance().getWindowSize().cx,(float)App::instance().getWindowSize().cy);
  _back->setPos(App::instance().getWindowSize().cx*0.5f,App::instance().getWindowSize().cy*0.5f);
  _back->setColor(D3DXCOLOR(1,1,1,0));
  this->addChild(_back);

  // てくすちゃーよみこみ
  _backTex[0] = App::instance().getRenderer()->getTexture()->createTexture("./data/texture/select_01.png");
  _backTex[1] = App::instance().getRenderer()->getTexture()->createTexture("./data/texture/select_02.png");
  App::instance().getRenderer()->getTexture()->createTexture("./data/texture/nasi_waku_00.png");

  _mode = SELECT_MODE::PLAYER1_SELECT;
  _select = true;

  _nowReading = Sprite2D::create("./data/texture/now_reading.png");
  _nowReading->setSize(670 * _windowScl, 101 * _windowScl);
  _nowReading->setPos(App::instance().getWindowSize().cx*0.5f,App::instance().getWindowSize().cy*0.5f);
  _nowReading->setColor(D3DXCOLOR(1,1,1,0));
  this->addChild(_nowReading);

#ifndef _QR_DISABLE__
  _isEnd = new bool;
  *_isEnd = false;
  _QRreader = new QRreader();
  _QRthread = new thread([](bool* isEnd,QRreader* reader) {
    for(;!(*isEnd);) {
      reader->update();
    }
    SafeDelete(reader);
    SafeDelete(isEnd);
  },_isEnd,_QRreader);
#endif

  _oji = Sprite2D::create("./data/texture/vs_oji.png");
  _oji->setSize((float)App::instance().getWindowSize().cx,(float)App::instance().getWindowSize().cy);
  _oji->setPos(App::instance().getWindowSize().cx*0.5f,-App::instance().getWindowSize().cy*0.5f);
  this->addChild(_oji);

  _oba = Sprite2D::create("./data/texture/vs_oba.png");
  _oba->setSize((float)App::instance().getWindowSize().cx,(float)App::instance().getWindowSize().cy);
  _oba->setPos(App::instance().getWindowSize().cx*0.5f,App::instance().getWindowSize().cy*1.5f);
  this->addChild(_oba);

  _vs = Sprite2D::create("./data/texture/vs.png");
  _vs->setSize((float)App::instance().getWindowSize().cx * 0.5f, (float)App::instance().getWindowSize().cy * 0.5f);
  _vs->setPos(App::instance().getWindowSize().cx*0.5f,App::instance().getWindowSize().cy*1.5f);
  this->addChild(_vs);

  // サウンドのロード
  // SE
//  App::instance().getSound()->load("./data/sound/se/fanfare.wav");
//  App::instance().getSound()->load("./data/sound/se/rod_select.wav");
  App::instance().getSound()->load("./data/sound/se/qr_ok.wav");
//  App::instance().getSound()->load("./data/sound/se/vs.wav");

  BaceScene::instance()->getLedConnect()->sendEvent(LedEvent::MoveSelect);

  return true;
}

//==============================================================================
// update
//------------------------------------------------------------------------------
void SelectScene::update() {
  const auto color = _back->getColor();
  const auto wakuColor = _waku->getColor();

  _gameF += _fspeed;

  for (int i = 0; i < 3; i++) {
    Vec3 pos = Vec3(cosf(_gameF + D3DX_PI * (2.f / 3.f) * i), 1, sinf(_gameF + D3DX_PI * (2.f / 3.f) * i));
    pos *= _modelLength;
    pos.y = 10.f;

    // pos
    _stickBar[i]->setPos(pos);
    _stickHandle[i]->setPos(pos);

    // rot
    const float rot = atan2(pos.x, pos.z);
    _stickBar[i]->setRotY(rot);
    _stickHandle[i]->setRotY(rot);
  }

  // 杖選択終わったら更新されるやつ
  // 1P
  if (_stickBarPlayer[0] != nullptr){
    Vec3 pos = Vec3(cosf(_gameF + D3DX_PI), 1, sinf(_gameF + D3DX_PI));
    pos *= _modelLength;

    // rot
    const float rot = atan2(pos.x, pos.z);
    _stickBarPlayer[0]->setRotY(rot);
    _stickHandlePlayer[0]->setRotY(rot);
  }
  // 2P
  if (_stickBarPlayer[1] != nullptr){
    Vec3 pos = Vec3(cosf(_gameF + D3DX_PI), 1, sinf(_gameF + D3DX_PI));
    pos *= _modelLength;

    // rot
    const float rot = atan2(pos.x, pos.z);
    _stickBarPlayer[1]->setRotY(rot);
    _stickHandlePlayer[1]->setRotY(rot);
  }

  // UI系
  if(_mode == SELECT_MODE::PLAYER1_SELECT || _mode == SELECT_MODE::PLAYER2_SELECT) {
    _back->setColor(color + (D3DXCOLOR(1,1,1,1) - color) * 0.1f);
    _waku->setColor(wakuColor + (kPlayerColor[_curSelectPlayer] - wakuColor) * 0.1f);
  }
  else {
    const D3DXCOLOR col = kPlayerColor[_curSelectPlayer];
    _back->setColor(color + (D3DXCOLOR(1,1,1,0) - color) * 0.1f);
    _waku->setColor(wakuColor + (D3DXCOLOR(col.r, col.g, col.b, 0) - wakuColor) * 0.1f);
  }

  if(_mode == SELECT_MODE::PLAYER1_QR || _mode == SELECT_MODE::PLAYER2_QR) {
    static float f = 0;
    f += 0.01f;
    _nowReading->setColor(D3DXCOLOR(1,1,1,1));
    _nowReading->setRotY(f);
  }
  else {
    _nowReading->setColor(D3DXCOLOR(1,1,1,0));
  }

  // モードの処理
  switch(_mode) {
  case SELECT_MODE::PLAYER1_SELECT:
    _back->setTexture(_backTex[0]);
    SelectQR(0);
    break;

  case SELECT_MODE::PLAYER1_QR:
    ReadQR(0);
#ifndef _QR_DISABLE__
    App::instance().setTitle(_QRreader->getString().c_str());
#endif
    break;

  case SELECT_MODE::PLAYER1_DECISION:
  {
    _modelLength -= _lengthSpeed; if (_modelLength < 0) _modelLength = 0;
    _fspeed += kFspeedAdd;

    if (_modelLength < 2.f){
      // 杖生成
      CreateStick(0, _playerStickID[0]);

      // 集中線
      _s->setVisible(true);

      // 杖消す
      for (int i = 0; i < 3; i++){
        _stickHandle[i]->setVisible(false);
        _stickBar[i]->setVisible(false);
      }
      // 次のモードへ
      _mode = (SELECT_MODE)((int)_mode + 1);

      // カメラ
      auto camera = App::instance().getRenderer()->getCamera();
      _playerCamera->setPosP({ kModelPos.x - kPlayerCameraOffset.x, 50.f, kPlayerCameraOffset.z });
      _playerCamera->setPosR({ kModelPos.x, 50, 0 });
      camera->setCamera(_playerCamera, 60);
    }
  }
    break;

  case SELECT_MODE::PLAYER1_SHOW:
  {
    _fspeed -= 0.002f;
    if (_fspeed < 0.03f) _fspeed = 0.03f;

    if (App::instance().getInput()->isTrigger(0, VK_INPUT::_1)) {
      _mode = (SELECT_MODE)((int)_mode + 1);

      // カメラ
      auto camera = App::instance().getRenderer()->getCamera();

      // メインカメラ
//      _camera->setPosP({ 0.f, 100.f, 400.f });
      _camera->setPosP({ 0.f, kMainCameraPos.y, kModelPos.x + kMainCameraPos.z });
      _camera->setPosR({ -kModelPos.x, 20, 0 });
      camera->setCamera(_camera, 60);

      // モデル
      _stickLayer->setPos(Vec3(-kModelPos.x, 0.0f, 0.0f));
      _gameF = 0;
      _fspeed = kFSpeed;
      _frame = 0;
      _stickDecision = false;
      _modelLength = kModelLength;
      _lengthSpeed = kModelLengthSpeed;

      // 杖表示
      for (int i = 0; i < 3; i++){
        _stickHandle[i]->setVisible(true);
        _stickBar[i]->setVisible(true);
      }

      // 集中線
      _s->setVisible(false);
    }
  }
    break;

  case SELECT_MODE::PLAYER2_SELECT:
    _back->setTexture(_backTex[1]);
    SelectQR(1);
    break;

  case SELECT_MODE::PLAYER2_QR:
    ReadQR(1);
    break;

  case SELECT_MODE::PLAYER2_DECISION:
  {
    _modelLength -= _lengthSpeed; if (_modelLength < 0) _modelLength = 0;
    _fspeed += kFspeedAdd;

    if (_modelLength < 2.f){
      // 杖生成
      CreateStick(1, _playerStickID[1]);

      // 集中線
      _s->setVisible(true);

      // 杖消す
      for (int i = 0; i < 3; i++){
        _stickHandle[i]->setVisible(false);
        _stickBar[i]->setVisible(false);
      }
      // 次のモードへ
      _mode = (SELECT_MODE)((int)_mode + 1);

      // カメラ
      auto camera = App::instance().getRenderer()->getCamera();
      _playerCamera->setPosP({ -(kModelPos.x - kPlayerCameraOffset.x), 50.f, kPlayerCameraOffset.z });
      _playerCamera->setPosR({ -kModelPos.x, 50, 0 });
      camera->setCamera(_playerCamera, 60);
    }
  }
    break;

  case SELECT_MODE::PLAYER2_SHOW:
  {
    _fspeed -= 0.002f;
    if (_fspeed < 0.03f) _fspeed = 0.03f;

    if (App::instance().getInput()->isTrigger(1, VK_INPUT::_1)) {
      _mode = (SELECT_MODE)((int)_mode + 1);

      // カメラ
      auto camera = App::instance().getRenderer()->getCamera();
      _camera->setPosP({ -300.f, 1000.f, 0.f });
      _camera->setPosR({ 0, 0, 0 });
      camera->setCamera(_camera, 60);
    }
  }
    break;

  case SELECT_MODE::Production:
    {
      if(App::instance().getInput()->isTrigger(0,VK_INPUT::_1)) {
        _mode = (SELECT_MODE)((int)_mode + 1);
      }
      const Vec2 pos = Vec2(App::instance().getWindowSize().cx*0.5f,App::instance().getWindowSize().cy*0.5f);
      _oji->setPos(_oji->getPos() + (pos - _oji->getPos()) * 0.2f);
      _oba->setPos(_oba->getPos() + (pos - _oba->getPos()) * 0.2f);
      _vs->setPos(_vs->getPos() + (pos - _vs->getPos()) * 0.1f);
    }
    break;

  case SELECT_MODE::MAX:
    // 杖登録
    DataManager::instance().setPlayerStatus(0, _playerStickID[0]);
    DataManager::instance().setPlayerStatus(1, _playerStickID[1]);

    App::instance().getSound()->stop("./data/sound/bgm/select_mode.wav");
    BaceScene::instance()->setCurScene(Game::create());
    break;
  }
  _inputPermit = true;
}

//==============================================================================
// CreateStick
//------------------------------------------------------------------------------
void SelectScene::CreateStick(int playerID, int sticlID)
{
  if (playerID >= 2 || sticlID >= 4) return;

  auto _effect = BaceScene::instance()->getEffect();
  const auto& barStatus = PlayerStatus::kStickBarStatus[sticlID];
  const auto& handStatus = PlayerStatus::kStickHandleStatus[sticlID];

  // 生成
  _stickBarPlayer[playerID] = XFileObject::create(barStatus.fileName);
  this->addChild(_stickBarPlayer[playerID]);

  _stickHandlePlayer[playerID] = XFileObject::create(handStatus.fileName);
  this->addChild(_stickHandlePlayer[playerID]);

  _stickBarPlayer[playerID]->setScl(Vec3(0.7f, 0.7f, 0.7f));
  _stickHandlePlayer[playerID]->setScl(Vec3(0.7f, 0.7f, 0.7f));

  _stickBarPlayer[playerID]->setPos(_stickLayer->getPos());
  _stickHandlePlayer[playerID]->setPos(_stickLayer->getPos());

  // 杖登場エフェクト
  Vec3 pos = _stickLayer->getPos();
  int id = 0 , id2 = 0;
  if (playerID == 0){
    id = _effect->play("GetItemBlue.efk", pos);
    id2 = _effect->play("DripGetBlue.efk", pos);
  }else if(playerID == 1){
    id = _effect->play("GetItemYellow.efk", pos);
    id2 = _effect->play("DripGetYellow.efk", pos);
  }

  _effect->setEffectScl(id, Vec3(1.f, 1.f, 1.f));
  _effect->setEffectScl(id2, Vec3(5.f, 5.f, 5.f));
}

//==============================================================================
// selectQR
//------------------------------------------------------------------------------
void SelectScene::SelectQR(int playerID) {
  static float f = D3DX_PI * 0.5f;

  if(App::instance().getInput()->isTrigger(playerID,VK_INPUT::LEFT)) {
    _waku->setTexture("./data/texture/qr_waku_00.png");
    _select = true;
    f = D3DX_PI * 0.5f;
  }
  else if(App::instance().getInput()->isTrigger(playerID,VK_INPUT::RIGHT)) {
    _waku->setTexture("./data/texture/nasi_waku_00.png");
    _select = false;
    f = D3DX_PI* 0.5f;
  }

  if (App::instance().getInput()->isTrigger(playerID, VK_INPUT::_1) && _inputPermit == true) {
    // 杖の抽選
    if (_select == false){
      _playerStickID[playerID] = (rand() % 3) + 1;
    }

    _waku->setTexture("./data/texture/qr_waku_00.png");
    _mode = (SELECT_MODE)((int)_mode + 1 + (_select ? 0 : 1));
    _select = true;
    f = D3DX_PI;

    _curSelectPlayer++;
    if (_curSelectPlayer > 1) _curSelectPlayer = 1;
    _inputPermit = false;
  }
  f += 0.1f;
  const D3DXCOLOR col = kPlayerColor[_curSelectPlayer];
  _waku->setColor(D3DXCOLOR(col.r, col.g, col.b, min(col.a, sinf(f) * 10.f)));
}

//==============================================================================
// readQR
//------------------------------------------------------------------------------
void SelectScene::ReadQR(int playerID) {

  if(App::instance().getInput()->isTrigger(playerID,VK_INPUT::_1)) {
    // とりあえず進む
    _mode = (SELECT_MODE)((int)_mode + 1);

    // hack
    // QRが読み込めない場合のキャンセル（前のモードに戻る）
    //_mode = (SELECT_MODE)((int)_mode - 1);
    //_inputPermit = false;
  }

  // hack
  // if( QRが読めた場合 ){
  //   _playerStickID[playerID] = [QRの番号] + 1;
  //   _mode = (SELECT_MODE)((int)_mode + 1);
  //}

  bool isRead = true;
  int stickId = 0;

  switch (_QRreader->getString().c_str()[0]){
  case '0':
    _playerStickID[playerID] = 0;
    break;
  case '1':
    _playerStickID[playerID] = 1;
    break;
  case '2':
    _playerStickID[playerID] = 2;
    break;
  default:
    isRead = false;
    break;
  }

  if (isRead){
    _mode = (SELECT_MODE)((int)_mode + 1);
  }
}

//==============================================================================
// 
//------------------------------------------------------------------------------
void SelectScene::uninit() {

#ifndef _QR_DISABLE__
  (*_isEnd) = true;
  if(_QRthread->joinable()) {
    _QRthread->join();
  }
  SafeDelete(_QRthread);
#endif
}

//EOF