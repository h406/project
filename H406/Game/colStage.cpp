//==============================================================================
//
// colStage[colStage.cpp]
// Author : Yasuaki Yamashita : 2015/10/21
//
//==============================================================================

//******************************************************************************
// include
//******************************************************************************
#include "colStage.h"
#include "stage.h"
#include "player.h"
#include "EventList.h"
#include "eventManager.h"
#include "BaceScene.h"

//==============================================================================
// init
//------------------------------------------------------------------------------
bool ColStage::init(Stage* stage,EventManager* event) {
  _stage = stage;
  _event = event;
  memset(_playerList,0,sizeof(_playerList));
  return true;
}

//==============================================================================
// update
//------------------------------------------------------------------------------
void ColStage::update() {
  auto _effect = BaceScene::instance()->getEffect();
  const Vec2& stageSize = _stage->getStageSize();
  const float halfSizeX = stageSize.x * 0.5f;
  const float halfSizeZ = stageSize.y * 0.5f;

  // ステージ当たり判定
  for(Player* player : _playerList) {
    if(player == nullptr) continue;

    Vec3 playerPos = player->getPos();
    if(playerPos.x < -halfSizeX) {
      playerPos.x = -halfSizeX;//+abs(playerPos.x + halfSizeX);
      //player->flipMvementX();
      player->setMoveVec(Vec3(0,player->getMoveVec().y,0));
    }
    if(playerPos.z < -halfSizeZ) {
      playerPos.z = -halfSizeZ;// + abs(playerPos.z + halfSizeZ);
      player->flipMvementZ();
      player->setMoveVec(Vec3(0,player->getMoveVec().y,0));
    }
    if(playerPos.x > halfSizeX) {
      playerPos.x = halfSizeX;// - (playerPos.x - halfSizeX);
      player->flipMvementX();
      player->setMoveVec(Vec3(0,player->getMoveVec().y,0));
    }
    if(playerPos.z > halfSizeZ) {
      playerPos.z = halfSizeZ;//- (playerPos.z - halfSizeZ);
      player->flipMvementZ();
      player->setMoveVec(Vec3(0,player->getMoveVec().y,0));
    }
    if(playerPos.y < 0.0f) {
      playerPos.y = 0.0f;
    }
    player->setPos(playerPos);
  }

  // 塗るやつ
  for(Player* player : _playerList) {
    if(player == nullptr) continue;
    if(player->getHitEnable() == false) continue;

    const int idX = int((player->getPos().x + halfSizeX) / (stageSize.x / (float)Stage::kNUM_X));
    const int idY = int((player->getPos().z + halfSizeZ) / (stageSize.y / (float)Stage::kNUM_Y));

    const Stage::FIELD_ID fieldID = _stage->getFieldID(idX,idY);

    // DRIP
    if(idX >= 0 && idX < Stage::kNUM_X && idY >= 0 && idY < Stage::kNUM_Y) {
      if(fieldID == Stage::FIELD_ID::DRIP) {
        const int maxDripNum = (player->getMaxDripNum() / 2) + 1;
        const int plus = rand() % maxDripNum + 2;
        _stage->setFieldID(idX,idY,Stage::FIELD_ID(int(Stage::FIELD_ID::PLAYER_1) + player->getPlayerID()));
        _event->dispatchEvent(EventList(int(EventList::PLAYER_1_DRIP_GET) + player->getPlayerID()), (void*) plus);

        // HACK どこでやるか？？
        const float jump = plus * 1.5f + 5.0f;
        player->addDripNum(plus);
        player->jump(jump);
        // se
//        App::instance().getSound()->play("./data/sound/se/get_ink.wav", false);
//        App::instance().getSound()->play("./data/sound/se/supply_ink.wav", false);
        App::instance().getSound()->play("./data/sound/se/get_item.wav", false);
      }
      // ITEM
      else if (fieldID == Stage::FIELD_ID::ITEM) {
      }
      else if (fieldID != Stage::FIELD_ID(int(Stage::FIELD_ID::PLAYER_1) + player->getPlayerID()) && player->getDripNum() > 0) {
        _stage->setFieldID(idX, idY, Stage::FIELD_ID(int(Stage::FIELD_ID::PLAYER_1) + player->getPlayerID()));
        _event->dispatchEvent(EventList(int(EventList::PLAYER_1_DRIP_USING) + player->getPlayerID()));

        // HACK どこでやるか？？
        player->addDripNum(-1);

        // 塗るエフェクト
        Vec3 effectPos = _stage->getFieldMapPos(idX, idY);
        if (player->getPlayerID() == 0){
          int id = _effect->play("DrawFloorBlue.efk", effectPos);
          _effect->setEffectScl(id, Vec3(1.2f, 1.2f, 1.2f));
        } else if (player->getPlayerID() == 1){
          int id = _effect->play("DrawFloorYellow.efk", effectPos);
          _effect->setEffectScl(id, Vec3(1.2f, 1.2f, 1.2f));
        }
        // se
        App::instance().getSound()->play("./data/sound/se/paint.wav", false);
      }
    }
  }
}

//==============================================================================
// addPlayer
//------------------------------------------------------------------------------
void ColStage::addPlayer(Player* player) {
  for(Player*& _player : _playerList) {
    if(_player == nullptr) {
      _player = player;
      break;
    }
  }
}

//==============================================================================
// update
//------------------------------------------------------------------------------
void ColStage::uninit() {

}


//EOF