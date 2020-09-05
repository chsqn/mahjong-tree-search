#ifndef MJTS_IMPL_HPP
#define MJTS_IMPL_HPP

#include <vector>
#include "player_core.hpp"

namespace mjts::impl{
  struct Stat{
    double prb = 0.0;
    double rdy = 0.0;

    friend bool operator>(const Stat& lhs, const Stat& rhs) {return lhs.prb > rhs.prb;}
    friend bool operator<(const Stat& lhs, const Stat& rhs) {return lhs.prb < rhs.prb;}
    Stat& operator*(const int n) { prb *= n; rdy *= n; return *this;}
    Stat& operator+=(const Stat& stat) { prb += stat.prb; rdy += stat.rdy; return *this;}
    Stat& operator/=(const int n) { prb /= n; rdy /= n; return *this;}
  };

  struct Condition{
    bool operator()(Player& player, int sht, int dpt) const {return player.num%3==2 && sht==0 && dpt>0;}
  };

  struct Process{
    utils::Rate rate;

    Stat operator()(Player& player, int sht, int mode, int dpt, int tile, const std::vector<int>& out, const std::vector<int>& sht_list)
    {
      Stat stat;
      double d = rate(out, dpt);

      stat.prb = d;

      for(int i=0; i<=dpt; ++i){
        if(sht_list[i] <= 1){
          stat.rdy += rate.value[i][rate.rng];
        }
      }
      return stat;
    }
  };

  template <template <class, class, class, class> class Selector>
  struct Mjts : public _Mjts<Stat,Player,Selector<Stat,Player,Condition,Process>,Condition,Process>{
    using super = _Mjts<Stat,Player,Selector<Stat,Player,Condition,Process>,Condition,Process>;

    template <class... Args>
    Mjts(const CalshtDW& c, const int m, Args... args) : super(c, m, args...) {}

    Stat calc1(Player& player, int rng)
    {
      player.set_wall();
      super::process.rate.sum = player.sum_wall();
      super::process.rate.rng = rng;
      auto [sht,mode,disc,wait] = super::calsht(player.hand, player.num/3, super::mode_in);
      auto stat =  super::tree_search(player, sht, mode, disc, wait, 0);

      return stat;
    }

    std::vector<Stat> calc2(Player& player, int rng)
    {
      std::vector<Stat> stat(K);

      player.set_wall();
      super::process.rate.sum = player.sum_wall();
      super::process.rate.rng = rng;
      auto [sht, mode, disc, wait] = super::calsht(player.hand, player.num/3, super::mode_in);

      for(int i=0; i<K; ++i){
        if(disc&(INT64_C(1)<<i)){
          --player.hand[i];
          --player.num;

          auto [sht_, mode_, disc_, wait_] = super::calsht(player.hand, player.num/3, super::mode_in);

          stat[i] = super::tree_search(player, sht_, mode_, disc_, wait_, 0);
          ++player.hand[i];
          ++player.num;
        }
      }
      return stat;
    }
  };
}

#endif
