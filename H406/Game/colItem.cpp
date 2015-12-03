//==============================================================================
//
// colItem[colItem.cpp]
// Author : masato masuda : 2015/11/25
//
//==============================================================================

//******************************************************************************
// include
//******************************************************************************
#include "colItem.h"
#include "stage.h"
#include "player.h"
#include "EventList.h"
#include "eventManager.h"
#include "itemManager.h"
#include "itemBomb.h"
#include "itemAccel.h"
#include "itemManhole.h"

//==============================================================================
// init
//------------------------------------------------------------------------------
bool ColItem::init(Stage* stage, EventManager *event, ItemManager* item) {
  _stage = stage;
  _event = event;
  _itemManager = item;
  memset(_playerList,0,sizeof(_playerList));
  return true;
}

//==============================================================================
// update
//------------------------------------------------------------------------------
void ColItem::update() {
  const Vec2& stageSize = _stage->getStageSize();
  const float halfSizeX = stageSize.x * 0.5f;
  const float halfSizeZ = stageSize.y * 0.5f;

  // プレイヤーとアイテム
  for (Player* player : _playerList) {
    if (player == nullptr) continue;

    // ボム
    ItemBomb** bombList = _itemManager->getBombList();
    for (int i = 0; i < ItemManager::kBombMax; i++){
      if (bombList[i] == nullptr) continue;
      if (bombList[i]->getOwner() == nullptr){

        const bool isHit = hitCheckCircle(player->getPos(), bombList[i]->getPos(),
                                          player->getRadius(), bombList[i]->getRadius());
        if (isHit){
          _event->dispatchEvent(EventList(int(EventList::PLAYER_1_GET_BOMB) + player->getPlayerID()), (void*)bombList[i]);
          App::instance().getSound()->play("./data/sound/se/get_item.wav", false);
        }
      }
    } // bomb

    // アクセル
    ItemAccel** accelList = _itemManager->getAccelList();
    for (int i = 0; i < ItemManager::kBombMax; i++){
      if (accelList[i] == nullptr) continue;
      if (accelList[i]->getOwner() == nullptr){

        const bool isHit = hitCheckCircle(player->getPos(), accelList[i]->getPos(),
                                          player->getRadius(), accelList[i]->getRadius());
        if (isHit){
          _event->dispatchEvent(EventList(int(EventList::PLAYER_1_GET_ACCEL) + player->getPlayerID()), (void*)accelList[i]);
          App::instance().getSound()->play("./data/sound/se/get_item.wav", false);
        }
      }
    } // accel

    // マンホール
    ItemManhole** manholeList = _itemManager->getManholeList();
    for (int i = 0; i < ItemManager::kManholeMax; i++){
      if (manholeList[i] == nullptr) continue;
      if (manholeList[i]->getOwner() == nullptr){

        const bool isHit = hitCheckCircle(player->getPos(), manholeList[i]->getPos(),
          player->getRadius(), manholeList[i]->getRadius());
        if (isHit){
          _event->dispatchEvent(EventList(int(EventList::PLAYER_1_GET_MANHOLE) + player->getPlayerID()), (void*)manholeList[i]);
          App::instance().getSound()->play("./data/sound/se/get_item.wav", false);
        }
      }
    } // accel
  } // player

  // アイテムとフィールド
  ItemBomb** bombList = _itemManager->getBombList();
  for (int i = 0; i < ItemManager::kBombMax; i++){
    if (bombList[i] == nullptr) continue;
    if (bombList[i]->getUse() && bombList[i]->getDeath() == false){

      const int idX = int((bombList[i]->getPos().x + halfSizeX) / (stageSize.x / (float)Stage::kNUM_X));
      const int idY = int((bombList[i]->getPos().z + halfSizeZ) / (stageSize.y / (float)Stage::kNUM_Y));
      const Stage::FIELD_ID fieldID = _stage->getFieldID(idX, idY);

      // 塗る
      if (fieldID != Stage::FIELD_ID::ITEM && fieldID != Stage::FIELD_ID(int(Stage::FIELD_ID::PLAYER_1) + bombList[i]->getOwner()->getPlayerID()) && bombList[i]->getDripNum() > 0) {
        _stage->setFieldID(idX, idY, Stage::FIELD_ID(int(Stage::FIELD_ID::PLAYER_1) + bombList[i]->getOwner()->getPlayerID()));
        bombList[i]->addDripNum(-1);
        App::instance().getSound()->play("./data/sound/se/paint.wav", false);
      }
    }
  }
}

//==============================================================================
// addPlayer
//------------------------------------------------------------------------------
void ColItem::addPlayer(Player* player) {
  for(Player*& _player : _playerList) {
    if(_player == nullptr) {
      _player = player;
      break;
    }
  }
}

//==============================================================================
// uninit
//------------------------------------------------------------------------------
void ColItem::uninit() {

}

//==============================================================================
// hitCheckCircle
//------------------------------------------------------------------------------
bool ColItem::hitCheckCircle(const Vec3& pos_0, const Vec3& pos_1, float rad_0, float rad_1){
  // 2円の半径の2乗
  float r2 = rad_0 + rad_1;
  r2 *= r2;

  // 2円の中心座標計算
  Vec2 centerPos = Vec2(pos_0.x - pos_1.x, pos_0.z - pos_1.z);
  float  scalar = (centerPos.x * centerPos.x) + (centerPos.y * centerPos.y);

  // hir
  if (r2 >= scalar){
    return true;
  }
  return false;
}


//EOF