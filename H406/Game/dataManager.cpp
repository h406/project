//==============================================================================
//
// dataManager[dataManager.h]
// Author : masato masuda : 2015/10/27
//
//==============================================================================

//******************************************************************************
// include
//******************************************************************************
#include "dataManager.h"
#include "eventManager.h"
#include "EventList.h"
#include "EventData.h"

//==============================================================================
// instance
//------------------------------------------------------------------------------
DataManager& DataManager::instance(void) {
  static DataManager dataManager;
  return dataManager;
}

//==============================================================================
// constructor
//------------------------------------------------------------------------------
DataManager::DataManager(void) 
: _data(nullptr){
}

//==============================================================================
// init
//------------------------------------------------------------------------------
void DataManager::init(EventManager* eventManager)
{
  _data = new DataEx();

  _data->setRandSeed(GameConfig::kRAND_SEED);
  _data->setTime(GameConfig::kTIME_MAX);
  _data->setRound(1);
  for (int i = 0; i < GameConfig::kPLAYER_MAX; i++){
    _data->setPlayerDripNum(i, 0);
    _data->setPlayerRoundWin(i, 0);
  }

  // イベント登録
  eventManager->addEventListener(EventList::PLAYER_1_ITEM_GET,bind(&DataManager::event,this,placeholders::_1));
  eventManager->addEventListener(EventList::PLAYER_2_ITEM_GET,bind(&DataManager::event,this,placeholders::_1));
  eventManager->addEventListener(EventList::PLAYER_1_ITEM_USING,bind(&DataManager::event,this,placeholders::_1));
  eventManager->addEventListener(EventList::PLAYER_2_ITEM_USING,bind(&DataManager::event,this,placeholders::_1));
  eventManager->addEventListener(EventList::PLAYER_1_ROUND_WIN, bind(&DataManager::event, this, placeholders::_1));
  eventManager->addEventListener(EventList::PLAYER_2_ROUND_WIN, bind(&DataManager::event, this, placeholders::_1));
  eventManager->addEventListener(EventList::NEXT_ROUND, bind(&DataManager::event, this, placeholders::_1));
}

//==============================================================================
// uninit
//------------------------------------------------------------------------------
void DataManager::uninit(void)
{
  SafeDelete(_data);
}

//==============================================================================
// update
//------------------------------------------------------------------------------
void DataManager::update(void)
{
  int time = _data->getTime();
  time--;
  if (time < 0){
    time = 0;
  }
  _data->setTime(time);
}

//==============================================================================
// event
//------------------------------------------------------------------------------
void DataManager::event(EventData* eventData)
{
  switch (eventData->getEvent()){
  case EventList::PLAYER_1_ITEM_GET:
    _data->setPlayerDripNum(0,_data->getPlayerDripNum(0) + (int)eventData->getUserData());
    if(_data->getPlayerDripNum(0) > 9)
      _data->setPlayerDripNum(0,9);
    break;
  case EventList::PLAYER_2_ITEM_GET:
    _data->setPlayerDripNum(1,_data->getPlayerDripNum(1) + (int)eventData->getUserData());
    if(_data->getPlayerDripNum(1) > 9)
      _data->setPlayerDripNum(1,9);
    break;
  case EventList::PLAYER_1_ITEM_USING:
    _data->setPlayerDripNum(0,_data->getPlayerDripNum(0) - 1);
    break;
  case EventList::PLAYER_2_ITEM_USING:
    _data->setPlayerDripNum(1,_data->getPlayerDripNum(1) - 1);
    break;

    // ラウンド
  case EventList::PLAYER_1_ROUND_WIN:
    _data->setPlayerRoundWin(0, _data->getPlayerRoundWin(0) + 1);
    break;
  case EventList::PLAYER_2_ROUND_WIN:
    _data->setPlayerRoundWin(1, _data->getPlayerRoundWin(1) + 1);
    break;

  case EventList::NEXT_ROUND:
    _data->setTime(GameConfig::kTIME_MAX);
    _data->setRound(_data->getRound() + 1);

    // 取得数
    _data->setPlayerDripNum(0, 0);
    _data->setPlayerDripNum(1, 0);
    break;
  }
}

//EOF
