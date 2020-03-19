#include "mjts_impl.hpp"

namespace mjts::impl{
  Stat Process::operator()(Player& player, int sht, int mode, std::int64_t disc, std::int64_t wait, int dpt, _Mjts<Stat,Player>& mjts) const
  {
    Stat stat;
    double d = mjts.calc_rate(dpt);

    stat.prb = d;

    for(int i=0; i<=dpt; ++i){
      if(mjts.sht_list[i] <= 1){
        stat.rdy += mjts.rate[i][mjts.rng];
      }
    }
    return stat;
  }

  Mjts Mjts::make_mjts_impl_recu(const CalshtDW& calsht, const int mode_in)
  {
    Selector1R<Stat,Player> ns1(calsht);
    Selector2R<Stat,Player> ns2;
    Condition cond;
    Process proc;
    return Mjts(calsht,mode_in,ns1,ns2,cond,proc);
  }

  Mjts Mjts::make_mjts_impl_prob(const CalshtDW& calsht, const int mode_in, const std::vector<int>& loop)
  {
    Selector1P<Stat,Player> ns1(calsht,loop);
    Selector2P<Stat,Player> ns2(calsht);
    Condition cond;
    Process proc;
    return Mjts(calsht,mode_in,ns1,ns2,cond,proc);
  }

  Stat Mjts::calc1(Player& player, const int rng)
  {
    player.set_wall();
    mjts.sum = player.sum_wall();
    mjts.rng = rng;
    auto [sht,mode,disc,wait] = mjts.calsht(player.hand,player.num/3,mjts.mode_in);

    return mjts.tree_search(player,sht,mode,disc,wait,0);
  }

  std::vector<Stat> Mjts::calc2(Player& player, const int rng)
  {
    std::vector<Stat> stat(K);

    player.set_wall();
    mjts.sum = player.sum_wall();
    mjts.rng = rng;

    for(int i=0; i<K; ++i){
      if(player.hand[i] > 0){
        --player.hand[i];
        --player.num;
        auto [sht,mode,disc,wait] = mjts.calsht(player.hand,player.num/3,mjts.mode_in);
        stat[i] = mjts.tree_search(player,sht,mode,disc,wait,0);
        ++player.hand[i];
        ++player.num;
      }
    }
    return stat;
  }
}
