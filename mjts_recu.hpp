#ifndef MJTS_RECU_HPP
#define MJTS_RECU_HPP

#include <bitset>
#include "mjts_core.hpp"
#include "mjts_utils.hpp"

namespace mjts{
  template <class Stat, class Player, class Condition, class Process>
  struct Recu : public _Selector<Stat,Player,Recu<Stat,Player,Condition,Process>,Condition,Process>{
    const CalshtDW& calsht;

    Recu(const CalshtDW& c) : calsht(c) {}

    Stat select1(Player& player, int sht, int mode, std::int64_t disc, std::int64_t wait, int dpt, _Mjts<Stat,Player,Recu<Stat,Player,Condition,Process>,Condition,Process>& mjts) const
    {
      std::bitset<K> bs_wait(wait);
      Stat ret;
      int out = utils::calc_out(player, wait);

      if(out == 0) return ret;

      mjts.out[dpt] = out;

      for(int i=0; i<K; ++i){
        if(bs_wait[i] && player.wall[i]){
          ++player.hand[i];
          ++player.num;
          mjts.tile = i;
          int coeff = player.wall[i];
          --player.wall[i];
          ret += mjts.tree_search(player, sht-1, mode, disc, wait, dpt+1)*coeff;
          --player.hand[i];
          --player.num;
          ++player.wall[i];
        }
      }
      return ret /= out;
    }

    Stat select2(Player& player, int sht, int mode, std::int64_t disc, std::int64_t wait, int dpt, _Mjts<Stat,Player,Recu<Stat,Player,Condition,Process>,Condition,Process>& mjts) const
    {
      std::bitset<K> bs_disc(disc);
      Stat ret;

      for(int i=0; i<K; ++i){
        if(bs_disc[i]){
          --player.hand[i];
          --player.num;
          ret = std::max(ret, mjts.tree_search(player, sht, mode, disc, wait, dpt));
          ++player.hand[i];
          ++player.num;
        }
      }
      return ret;
    }
  };
}

#endif
