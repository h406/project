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
  const Vec2& stageSize = _stage->getStageSize();
  const float halfSizeX = stageSize.x * 0.5f;
  const float halfSizeZ = stageSize.y * 0.5f;

  // �X�e�[�W�����蔻��
  for(Player* player : _playerList) {
    if(player == nullptr) continue;
    Vec3 playerPos = player->getPos();
    if(playerPos.x < -halfSizeX) {
      playerPos.x = -halfSizeX + abs(playerPos.x + halfSizeX);
      player->flipMvementX();
    }
    if(playerPos.z < -halfSizeZ) {
      playerPos.z = -halfSizeZ + abs(playerPos.z + halfSizeZ);
      player->flipMvementZ();
    }
    if(playerPos.x > halfSizeX) {
      playerPos.x = halfSizeX - (playerPos.x - halfSizeX);
      player->flipMvementX();
    }
    if(playerPos.z > halfSizeZ) {
      playerPos.z = halfSizeZ - (playerPos.z - halfSizeZ);
      player->flipMvementZ();
    }
    if(playerPos.y < 0.0f) {
      playerPos.y = 0.0f;
    }
    player->setPos(playerPos);
  }

  // �A�C�e���擾
  for(Player* player : _playerList) {
    if(player == nullptr) continue;
    const int idX = int((player->getPos().x + halfSizeX) / (stageSize.x / (float)Stage::kNUM_X));
    const int idY = int((player->getPos().z + halfSizeZ) / (stageSize.y / (float)Stage::kNUM_Y));

    const Stage::FIELD_ID fieldID = _stage->getFieldID(idX,idY);

    if(idX >= 0 && idX < Stage::kNUM_X && idY >= 0 && idY < Stage::kNUM_Y) {
      if(fieldID == Stage::FIELD_ID::ITEM) {
        const int plus = rand() % 4 + 1;
        _stage->setFieldID(idX,idY,Stage::FIELD_ID(int(Stage::FIELD_ID::PLAYER_1) + player->getPlayerID()));
        _event->dispatchEvent(EventList(int(EventList::PLAYER_1_ITEM_GET) + player->getPlayerID()), (void*) plus);

        // HACK �ǂ��ł�邩�H�H
        player->addDripNum(plus);
        player->jump(10.f);
      }
      else if(fieldID != Stage::FIELD_ID::ITEM && fieldID != Stage::FIELD_ID(int(Stage::FIELD_ID::PLAYER_1) + player->getPlayerID()) && player->getDripNum() > 0) {
        _stage->setFieldID(idX,idY,Stage::FIELD_ID(int(Stage::FIELD_ID::PLAYER_1) + player->getPlayerID()));
        _event->dispatchEvent(EventList(int(EventList::PLAYER_1_ITEM_USING) + player->getPlayerID()));

        // HACK �ǂ��ł�邩�H�H
        player->addDripNum(-1);
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