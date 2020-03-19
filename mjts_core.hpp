#ifndef MJTS_CORE_HPP
#define MJTS_CORE_HPP

#include <array>
#include <bitset>
#include <cstdint>
#include <functional>
#include <random>
#include <utility>
#include "constant.hpp"
#include "calsht_dw.hpp"

namespace mjts{
  constexpr int DPT_MAX = 20;
  constexpr int RNG_MAX = 40;

  template <class Stat, class Player>
  struct _Mjts;

  template <class Stat, class Player>
  using Selector = std::function<Stat(Player&,int,int,std::int64_t,std::int64_t,int,_Mjts<Stat,Player>&)>;
  template <class Player>
  using Condition = std::function<bool(Player&,int,int)>;

  template <class Stat, class Player>
  struct Selector1R{
    const CalshtDW& calsht;
    Selector1R(const CalshtDW& c) : calsht(c) {}
    Stat operator()(Player& player, int sht, int mode, std::int64_t disc, std::int64_t wait, int dpt, _Mjts<Stat,Player>& mjts) const;
  };

  template <class Stat, class Player>
  struct Selector2R{
    Stat operator()(Player& player, int sht, int mode, std::int64_t disc, std::int64_t wait, int dpt, _Mjts<Stat,Player>& mjts) const;
  };

  template <class Stat, class Player>
  struct Selector1P{
    const CalshtDW& calsht;
    mutable std::vector<int> loop;
    mutable std::mt19937_64 mt;
    Selector1P(const CalshtDW& c, const std::vector<int>& l) : calsht(c), loop(DPT_MAX), mt(std::random_device{}())
    {
      auto it = loop.begin();
      for(const auto& n : l) *it++ = n;
      std::fill(it, loop.end(), l.back());
    }
    Stat operator()(Player& player, int sht, int mode, std::int64_t disc, std::int64_t wait, int dpt, _Mjts<Stat,Player>& mjts) const;
  };

  template <class Stat, class Player>
  struct Selector2P{
    const CalshtDW& calsht;
    mutable std::mt19937_64 mt;
    mutable std::array<int,K> cnt;
    Selector2P(const CalshtDW& c) : calsht(c), mt(std::random_device{}()) {}
    Stat operator()(Player& player, int sht, int mode, std::int64_t disc, std::int64_t wait, int dpt, _Mjts<Stat,Player>& mjts) const;
  };

  template <class Stat, class Player>
  struct _Mjts{
    const CalshtDW& calsht;
    const int mode_in;
    const Selector<Stat,Player> selector1;
    const Selector<Stat,Player> selector2;
    const Condition<Player> condition;
    const Selector<Stat,Player> process;
    int tile;
    int sum;
    int rng;
    std::array<int,RNG_MAX> out;
    std::array<int,DPT_MAX> sht_list;
    std::array<std::array<double,RNG_MAX>,DPT_MAX> rate;
    Stat tree_search(Player& player, int sht, int mode, std::int64_t disc, std::int64_t wait, int dpt);
    double calc_rate(int dpt);
    _Mjts(const CalshtDW& c, const int m, const Selector<Stat,Player> ns1, const Selector<Stat,Player> ns2, const Condition<Player> cond, const Selector<Stat,Player> proc) :
      calsht(c), mode_in(m), selector1(ns1), selector2(ns2), condition(cond), process(proc) {}
  };

  template <class Player>
  void count_necessary_tiles(const CalshtDW& calsht, Player& player, const int mode_in, const std::int64_t disc, std::array<int,K>& cnt)
  {
    std::bitset<K> bs_disc(disc);

    for(int k=0; k<K; ++k){
      cnt[k] = -1;

      if(bs_disc[k]){
        --player.hand[k];
        auto [sht_,mode_,disc_,wait_] = calsht(player.hand,player.num/3,mode_in);
        std::bitset<K> bs_wait(wait_);
        cnt[k] = 0;

        for(int l=0; l<K; ++l){
          cnt[k] += bs_wait[l] ? player.wall[l]:0;
        }
        ++player.hand[k];
      }
    }
  }

  template <class Player>
  int calc_out(const Player& player, const std::int64_t num)
  {
    int ret = 0;

    for(int i=0; i<K; ++i){
      if(num&(UINT64_C(1)<<i)) ret += player.wall[i];
    }
    return ret;
  }

  template <class Stat, class Player>
  Stat Selector1R<Stat,Player>::operator()(Player& player, int sht, int mode, std::int64_t disc, std::int64_t wait, int dpt, _Mjts<Stat,Player>& mjts) const
  {
    std::bitset<K> bs_wait(wait);
    Stat ret;
    int out = calc_out(player,wait);

    if(out == 0) return ret;

    mjts.out[dpt] = out;

    for(int i=0; i<K; ++i){
      if(bs_wait[i] && player.wall[i]){
        ++player.hand[i];
        ++player.num;
        mjts.tile = i;
        mjts.sht_list[dpt] = sht;
        int coeff = player.wall[i];
        --player.wall[i];
        ret += mjts.tree_search(player,(bs_wait[i]?sht-1:sht),mode,disc,wait,dpt+1)*coeff;
        --player.hand[i];
        --player.num;
        ++player.wall[i];
      }
    }
    return ret /= out;
  }

  template <class Stat, class Player>
  Stat Selector2R<Stat,Player>::operator()(Player& player, int sht, int mode, std::int64_t disc, std::int64_t wait, int dpt, _Mjts<Stat,Player>& mjts) const
  {
    std::bitset<K> bs_disc(disc);
    Stat ret;

    for(int i=0; i<K; ++i){
      if(bs_disc[i]){
        --player.hand[i];
        --player.num;
        ret = std::max(ret, mjts.tree_search(player,sht,mode,disc,wait,dpt));
        ++player.hand[i];
        ++player.num;
      }
    }
    return ret;
  }

  template <class Stat, class Player>
  Stat Selector1P<Stat,Player>::operator()(Player& player, int sht, int mode, std::int64_t disc, std::int64_t wait, int dpt, _Mjts<Stat,Player>& mjts) const
  {
    std::bitset<K> bs_wait(wait);
    Stat ret;
    int out = calc_out(player,wait);

    if(out == 0) return ret;

    mjts.out[dpt] = out;

    for(int i=0; i<loop[dpt]; ++i){
      int p = 0;

      for(int c=mt()%out+1; c>0; ++p){
        if(bs_wait[p]) c -= player.wall[p];
      }
      mjts.tile = p-1;
      mjts.sht_list[dpt] = sht;
      --player.wall[p-1];
      ++player.hand[p-1];
      ++player.num;
      ret += mjts.tree_search(player,(bs_wait[p-1]?sht-1:sht),mode,disc,wait,dpt+1);
      ++player.wall[p-1];
      --player.hand[p-1];
      --player.num;
    }
    return ret /= loop[dpt];
  }

  template <class Stat, class Player>
  Stat Selector2P<Stat,Player>::operator()(Player& player, int sht, int mode, std::int64_t disc, std::int64_t wait, int dpt, _Mjts<Stat,Player>& mjts) const
  {
    count_necessary_tiles(calsht,player,mjts.mode_in,disc,cnt);

    int n = mt()%K;
    int p = n;
    int max = cnt[n];

    for(int k=n+1; k<n+K; ++k){
      if(int i=k%K; cnt[i]>max){
        p = i;
        max = cnt[i];
      }
    }
    --player.hand[p];
    --player.num;
    Stat ret = mjts.tree_search(player,sht,mode,disc,wait,dpt);
    ++player.hand[p];
    ++player.num;

    return ret;
  }

  template <class Stat, class Player>
  Stat _Mjts<Stat,Player>::tree_search(Player& player, int sht, int mode, std::int64_t disc, std::int64_t wait, int dpt)
  {
    if(condition(player,sht,dpt)){
      return process(player,sht,mode,disc,wait,dpt,*this);
    }
    auto [sht_,mode_,disc_,wait_] = calsht(player.hand,player.num/3,mode_in);

    if(player.num%3 == 1){
      return selector1(player,sht_,mode_,disc_,wait_,dpt,*this);
    }
    else{
      return selector2(player,sht_,mode_,disc_,wait_,dpt,*this);
    }
  }

  template <class Stat, class Player>
  double _Mjts<Stat,Player>::calc_rate(int dpt)
  {
    rate[0][0] = 1.0;

    for(int k=0; k<RNG_MAX-1; ++k){
      rate[0][k+1] = (1-1.0*out[0]/(sum-k))*rate[0][k];

      for(int j=1; j<dpt; ++j){
        rate[j][k+1] = (1-1.0*out[j]/(sum-k))*rate[j][k]+1.0*out[j-1]/(sum-k)*rate[j-1][k];
      }
      rate[dpt][k+1] = 1.0*out[dpt-1]/(sum-k)*rate[dpt-1][k]+rate[dpt][k];
    }
    return rate[dpt][rng];
  }
}

#endif
