#ifndef MJTS_PROB_HPP
#define MJTS_PROB_HPP

#include <bitset>
#include <vector>
#include "mjts_core.hpp"
#include "mjts_utils.hpp"
#include "random_index.hpp"

namespace mjts{
  template <class Stat, class Player, class Condition, class Process>
  struct Prob : public _Selector<Stat,Player,Prob<Stat,Player,Condition,Process>,Condition,Process>{
    const CalshtDW& calsht;
    std::vector<int> loop;
    std::vector<int> cnt;
    std::mt19937_64 mt;
    RandomIndex random_index;

    template <class... Args>
    Prob(const CalshtDW& c, Args... args) : calsht(c), loop({args...}), cnt(K), mt(std::random_device{}()) {}

    void count_necessary_tiles(Player& player, const int mode_in, const std::int64_t disc)
    {
      std::bitset<K> bs_disc(disc);

      for(int k=0; k<K; ++k){
        cnt[k] = -1;

        if(bs_disc[k]){
          --player.hand[k];
          auto [sht_, mode_, disc_, wait_] = calsht(player.hand, player.num/3, mode_in);
          std::bitset<K> bs_wait(wait_);
          cnt[k] = 0;

          for(int l=0; l<K; ++l){
            cnt[k] += bs_wait[l] ? player.wall[l]:0;
          }
          ++player.hand[k];
        }
      }
    }

    Stat select1(Player& player, int sht, int mode, std::int64_t disc, std::int64_t wait, int dpt, _Mjts<Stat,Player,Prob<Stat,Player,Condition,Process>,Condition,Process>& mjts)
    {
      std::bitset<K> bs_wait(wait);
      Stat ret;
      int out = utils::calc_out(player, wait);

      if(out == 0) return ret;

      mjts.out[dpt] = out;

      int num = dpt >= loop.size() ? loop.back() : loop[dpt];

      for(int i=0; i<num; ++i){
        int p = 0;

        for(int c=mt()%out+1; c>0; ++p){
          if(bs_wait[p]) c -= player.wall[p];
        }
        mjts.tile = p-1;
        --player.wall[p-1];
        ++player.hand[p-1];
        ++player.num;
        ret += mjts.tree_search(player, sht-1, mode, disc, wait, dpt+1);
        ++player.wall[p-1];
        --player.hand[p-1];
        --player.num;
      }
      return ret /= num;
    }

    Stat select2(Player& player, int sht, int mode, std::int64_t disc, std::int64_t wait, int dpt, _Mjts<Stat,Player,Prob<Stat,Player,Condition,Process>,Condition,Process>& mjts)
    {
      count_necessary_tiles(player, mjts.mode_in, disc);

      auto& index = random_index();
      int p = index[0];
      int max = cnt[p];

      for(int k=1; k<K; ++k){
        if(int i=index[k]; cnt[i]>max){
          p = i;
          max = cnt[i];
        }
      }
      --player.hand[p];
      --player.num;
      Stat ret = mjts.tree_search(player, sht, mode, disc, wait, dpt);
      ++player.hand[p];
      ++player.num;

      return ret;
    }
  };
}

#endif
