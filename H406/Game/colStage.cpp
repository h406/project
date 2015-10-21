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

//==============================================================================
// init
//------------------------------------------------------------------------------
bool ColStage::init(Stage* stage) {
  _stage = stage;
  
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

  // ステージ当たり判定
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

  // アイテム取得
  for(Player* player : _playerList) {
    if(player == nullptr) continue;
    const int idX = int((player->getPos().x + halfSizeX) / (stageSize.x / (float)Stage::kNUM_X));
    const int idY = int((player->getPos().z + halfSizeZ) / (stageSize.y / (float)Stage::kNUM_Y));

    const Stage::FIELD_ID fieldID = _stage->getFieldID(idX,idY);

    if(idX >= 0 && idX < Stage::kNUM_X && idY >= 0 && idY < Stage::kNUM_Y) {
      if(fieldID == Stage::FIELD_ID::ITEM) {
        const int plus = rand() % 4 + 1;
        _stage->setFieldID(idX,idY,Stage::FIELD_ID(int(Stage::FIELD_ID::PLAYER_1) + player->getPlayerID()));
        //_plusNum[i]->setAnimID(plus);
        //_num[i] += plus;
        player->addDripNum(plus);
        //if(_num[i] > 9) _num[i] = 9;
        //_numSpriteScl[i] = 2;
        //_effect->play("get.efk",playerPos[i]);

        player->jump(10.f);
      }
      else if(fieldID != Stage::FIELD_ID::ITEM && fieldID != Stage::FIELD_ID(int(Stage::FIELD_ID::PLAYER_1) + player->getPlayerID()) && player->getDripNum() > 0) {
        _stage->setFieldID(idX,idY,Stage::FIELD_ID(int(Stage::FIELD_ID::PLAYER_1) + player->getPlayerID()));
        //_numSpriteScl[i] = 0.7f;
        //_num[i] --;
        player->addDripNum(-1);
        //_effect->play("shot.efk",playerPos[i]);
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