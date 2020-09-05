#ifndef MJTS_CORE_HPP
#define MJTS_CORE_HPP

#include <cstdint>
#include <random>
#include "constant.hpp"
#include "calsht_dw.hpp"

namespace mjts{
  constexpr int DPT_MAX = 64;
  constexpr int RNG_MAX = 64;

  template <class Stat, class Player, class Selector, class Condition, class Process>
  struct _Mjts;

  template <class Stat, class Player, class Selector, class Condition, class Process>
  struct _Selector{
    Stat select1(Player& player, int sht, int mode, std::int64_t disc, std::int64_t wait, int dpt, _Mjts<Stat,Player,Selector,Condition,Process>& mjts) const
    {
      return static_cast<const Selector&>(*this).select1(player, sht, mode, disc, wait, dpt, mjts);
    }
    Stat select2(Player& player, int sht, int mode, std::int64_t disc, std::int64_t wait, int dpt, _Mjts<Stat,Player,Selector,Condition,Process>& mjts) const
    {
      return static_cast<const Selector&>(*this).select2(player, sht, mode, disc, wait, dpt, mjts);
    }
  };

  template <class Stat, class Player, class Selector, class Condition, class Process>
  struct _Mjts{
    const CalshtDW& calsht;
    const int mode_in;
    Selector selector;
    Condition condition;
    Process process;
    int tile;
    std::vector<int> out;
    std::vector<int> sht_list;

    template <class... Args>
    _Mjts(const CalshtDW& c, const int m, Args... args) : calsht(c), mode_in(m), selector(c, args...), out(RNG_MAX, 0), sht_list(DPT_MAX, 0) {}

    Stat tree_search(Player& player, int sht, int mode, std::int64_t disc, std::int64_t wait, int dpt)
    {
      sht_list[dpt] = sht;

      if(condition(player,sht,dpt)){
        return process(player, sht, mode, dpt, tile, out, sht_list);
      }
      auto [sht_, mode_, disc_, wait_] = calsht(player.hand,player.num/3,mode_in);

      if(player.num%3 == 1){
        return selector.select1(player, sht_, mode_, disc_, wait_, dpt, *this);
      }
      else{
        return selector.select2(player, sht_, mode_, disc_, wait_, dpt, *this);
      }
    }
  };
}

#endif
