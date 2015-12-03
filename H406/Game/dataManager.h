//==============================================================================
//
// dataManager[dataManager.h]
// Author : masato masuda : 2015/10/27
//
//==============================================================================
#pragma once

#ifndef _DATA_MANAGER_H_
#define _DATA_MANAGER_H_

#include "gameConfig.h"
#include "playerStatus.h"

class EventData;
class EventManager;

//==============================================================================
// class
//------------------------------------------------------------------------------
class Data {
public:
  int getRandSeed()const { return _randSeed; }
  int getTime()const { return _time; }
  int getRound()const { return _round; }
  int getPlayerRoundWin(int playerId)const { return _playerRoundWin[playerId]; }
  int getPlayerDripNum(int playerId)const { return _playerDripNum[playerId]; }
  int getPlayerMapNum(int playerId)const { return _playerMapNum[playerId]; }
  int getPlayerMaxDripNum(int playerId)const { return _playerMaxDripNum[playerId]; }
  const PlayerStatus::PlayerStatus& getPlayerStatus(int playerID) const { return _playerStatus[playerID]; }
protected:
  int _randSeed;
  int _time;
  int _round;
  int _playerRoundWin[GameConfig::kPLAYER_MAX];
  int _playerDripNum[GameConfig::kPLAYER_MAX];
  int _playerMapNum[GameConfig::kPLAYER_MAX];
  int _playerMaxDripNum[GameConfig::kPLAYER_MAX];
  PlayerStatus::PlayerStatus _playerStatus[GameConfig::kPLAYER_MAX];
};

//==============================================================================
// class
//------------------------------------------------------------------------------
class DataManager {
private:

  class DataEx : public Data {
  public:
    void setRandSeed(int seed){ this->_randSeed = seed; }
    void setTime(int time){ this->_time = time; }
    void setRound(int round){ this->_round = round; }
    void setPlayerRoundWin(int playerId, int roundWin){ this->_playerRoundWin[playerId] = roundWin; }
    void setPlayerDripNum(int playerId, int dripNum){ this->_playerDripNum[playerId] = dripNum; }
    void setPlayerMapNum(int playerId, int mapNum){ this->_playerMapNum[playerId] = mapNum; }
    void setPlayerMaxDripNum(int playerId, int maxNum){ this->_playerMaxDripNum[playerId] = maxNum; }
    void setPlayerStatus(int playerId, const PlayerStatus::PlayerStatus& status) { this->_playerStatus[playerId] = status; }
  };

public:
  ~DataManager(void){}
  static DataManager& instance(void);
  void init();
  void uninit(void);
  void update(void);

  void setEventManager(EventManager* eventManager);

  Data* getData() { return _data; }
  void event(EventData* eventData);
  bool isInstance(void){ return _data != nullptr ? true : false; }

private:
  DataManager(void);
  DataEx* _data;
};


#endif
//EOF
