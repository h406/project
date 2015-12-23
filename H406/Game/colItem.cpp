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
#include "BaceScene.h"
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
  auto _effect = BaceScene::instance()->getEffect();
  const Vec2& stageSize = _stage->getStageSize();
  const float halfSizeX = stageSize.x * 0.5f;
  const float halfSizeZ = stageSize.y * 0.5f;

  // �v���C���[�ƃA�C�e��
  for (Player* player : _playerList) {
    if (player == nullptr) continue;
    if (player->getHitEnable() == false) continue;

    // �{��
    ItemBomb** bombList = _itemManager->getBombList();
    for (int i = 0; i < ItemManager::kBombMax; i++){
      if (bombList[i] == nullptr) continue;
      if (bombList[i]->getOwner() == nullptr){

        const bool isHit = hitCheckCircle(player->getPos(), bombList[i]->getPos(),
                                          player->getRadius(), bombList[i]->getRadius());
        if (isHit){
          _event->dispatchEvent(EventList(int(EventList::PLAYER_1_GET_BOMB) + player->getPlayerID()), (void*)bombList[i]);
          App::instance().getSound()->play("./data/sound/se/get_item.wav", false);
          bombList[i]->getOwner()->jump(10.0f);
        }
      }
    } // bomb

    // �A�N�Z��
    ItemAccel** accelList = _itemManager->getAccelList();
    for (int i = 0; i < ItemManager::kAccelMax; i++){
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

    // �}���z�[��
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

  // �A�C�e���ƃt�B�[���h
  ItemBomb** bombList = _itemManager->getBombList();
  for (int i = 0; i < ItemManager::kBombMax; i++){
    if (bombList[i] == nullptr) continue;
    if (bombList[i]->getUse() && bombList[i]->getDeath() == false){

      int idX = int((bombList[i]->getPos().x + halfSizeX) / (stageSize.x / (float)Stage::kNUM_X));
      int idY = int((bombList[i]->getPos().z + halfSizeZ) / (stageSize.y / (float)Stage::kNUM_Y));
      if (idX > 11) idX = 11;
      if (idX < 0) idX = 0;
      if (idY > 11) idY = 11;
      if (idY < 0) idY = 0;

      const Stage::FIELD_ID fieldID = _stage->getFieldID(idX, idY);

      // �h��
      if (fieldID != Stage::FIELD_ID(int(Stage::FIELD_ID::PLAYER_1) + bombList[i]->getOwner()->getPlayerID()) && bombList[i]->getDripNum() > 0) {
        _stage->setFieldID(idX, idY, Stage::FIELD_ID(int(Stage::FIELD_ID::PLAYER_1) + bombList[i]->getOwner()->getPlayerID()));
        bombList[i]->addDripNum(-1);
        App::instance().getSound()->play("./data/sound/se/paint.wav", false);

        // �h��G�t�F�N�g
        Vec3 effectPos = _stage->getFieldMapPos(idX, idY);
        if (bombList[i]->getOwner()->getPlayerID() == 0){
          int id = _effect->play("DrawFloorBlue.efk", effectPos);
          _effect->setEffectScl(id, Vec3(1.2f, 1.2f, 1.2f));
        }
        else if (bombList[i]->getOwner()->getPlayerID() == 1){
          int id = _effect->play("DrawFloorYellow.efk", effectPos);
          _effect->setEffectScl(id, Vec3(1.2f, 1.2f, 1.2f));
        } // effect
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
  // 2�~�̔��a��2��
  float r2 = rad_0 + rad_1;
  r2 *= r2;

  // 2�~�̒��S���W�v�Z
  Vec2 centerPos = Vec2(pos_0.x - pos_1.x, pos_0.z - pos_1.z);
  float  scalar = (centerPos.x * centerPos.x) + (centerPos.y * centerPos.y);

  // hir
  if (r2 >= scalar){
    return true;
  }
  return false;
}


//EOF