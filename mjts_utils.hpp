#ifndef MJTS_UTILS_HPP
#define MJTS_UTILS_HPP

#include "mjts_core.hpp"

namespace mjts::utils{
  struct Rate{
    int sum;
    int rng;
    std::vector<std::vector<double>> value;

    Rate() : value(DPT_MAX, std::vector<double>(RNG_MAX, 0.0)) {}

    double operator()(const std::vector<int>& out, const int dpt)
    {
      value[0][0] = 1.0;

      for(int k=0; k<rng; ++k){
        value[0][k+1] = (1-1.0*out[0]/(sum-k))*value[0][k];

        for(int j=1; j<dpt; ++j){
          value[j][k+1] = (1-1.0*out[j]/(sum-k))*value[j][k]+1.0*out[j-1]/(sum-k)*value[j-1][k];
        }
        value[dpt][k+1] = 1.0*out[dpt-1]/(sum-k)*value[dpt-1][k]+value[dpt][k];
      }
      return value[dpt][rng];
    }
  };

  template <class Player>
  int calc_out(const Player& player, const std::int64_t wait)
  {
    int ret = 0;

    for(int i=0; i<K; ++i){
      if(wait&(INT64_C(1)<<i)) ret += player.wall[i];
    }
    return ret;
  }
}

#endif
