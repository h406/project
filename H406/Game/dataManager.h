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
  int getPlayerDripNum(int playerId)const { return _playerDripNum[playerId]; }

protected:
  int _randSeed;
  int _time;
  int _round;
  int _playerDripNum[GameConfig::kPLAYER_MAX];
};

class DataManager {
private:

  class DataEx : public Data {
  public:
    void setRandSeed(int seed){ this->_randSeed = seed; }
    void setTime(int time){ this->_time = time; }
    void setRound(int round){ this->_round = round; }
    void setPlayerDripNum(int playerId, int dripNum){ this->_playerDripNum[playerId] = dripNum; }
  };

public:
  ~DataManager(void){}
  static DataManager& instance(void);
  void init(EventManager* eventManager);
  void uninit(void);
  void update(void);

  Data* getData() { return _data; }

  void event(EventData* eventData);

private:
  DataManager(void);
  DataEx* _data;
};


#endif
//EOF
