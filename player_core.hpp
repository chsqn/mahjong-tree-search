#ifndef PLAYER_CORE_HPP
#define PLAYER_CORE_HPP

#include <algorithm>
#include <numeric>
#include <vector>
#include "constant.hpp"

class Player{
public:
  std::vector<int> hand;
  std::vector<int> wall;
  int num = 0;

  Player() : hand(K, 0), wall(K, 0) {}

  void reset()
  {
    std::fill(hand.begin(), hand.end(), 0);
    std::fill(wall.begin(), wall.end(), 0);
  }

  void set_wall()
  {
    for(int i=0; i<K; ++i){
#ifdef THREE_PLAYER
      wall[i] = i>0 && i<8 ? 0 : 4-hand[i];
#else
      wall[i] = 4-hand[i];
#endif
    }
  }

  int sum_wall()
  {
    return std::accumulate(wall.begin(), wall.end(), 0);
  }
};

#endif
