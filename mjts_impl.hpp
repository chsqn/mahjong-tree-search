#ifndef MJTS_IMPL_HPP
#define MJTS_IMPL_HPP

#include <vector>
#include "mjts_core.hpp"
#include "player_core.hpp"

namespace mjts::impl{
  struct Stat{
    double prb = 0.0;
    double rdy = 0.0;
    friend bool operator>(const Stat& lhs, const Stat& rhs) {return lhs.prb > rhs.prb;}
    friend bool operator<(const Stat& lhs, const Stat& rhs) {return lhs.prb < rhs.prb;}
    Stat& operator*(const int n) {prb*=n; rdy*=n; return *this;}
    Stat& operator+=(const Stat& stat) {prb+=stat.prb; rdy+=stat.rdy; return *this;}
    Stat& operator/=(const int n) {prb/=n; rdy/=n; return *this;}
  };

  struct Condition{
    bool operator()(Player& player, int sht, int dpt) const {return player.num%3==2 && sht==0 && dpt>0;}
  };

  struct Process{
    Stat operator()(Player& player, int sht, int mode, std::int64_t disc, std::int64_t wait, int dpt, _Mjts<Stat,Player>& mjts) const;
  };

  struct Mjts{
    _Mjts<Stat,Player> mjts;
    Mjts(const CalshtDW& c, const int m, Selector<Stat,Player> ns1, Selector<Stat,Player> ns2, Condition cond, Selector<Stat,Player> proc) :
      mjts(c,m,ns1,ns2,cond,proc) {}
    Stat calc1(Player& player, int rng);
    std::vector<Stat> calc2(Player& player, int rng);
    static Mjts make_mjts_impl_recu(const CalshtDW& calsht, int mode_in);
    static Mjts make_mjts_impl_prob(const CalshtDW& calsht, int mode_in, const std::vector<int>& loop);
  };
}

#endif
