#include <iostream>
#include <bitset>
#include <chrono>
#include <cstdint>
#include <map>
#include <string>
#include "calsht_dw.hpp"
#include "player_core.hpp"
#include "settile.hpp"
#include "mjts_impl.hpp"

int main()
{
  constexpr int M = 14;
  constexpr int MODE = 7;

  CalshtDW calsht;
  Player player;
#ifdef RECU
  auto mjts_impl = mjts::impl::Mjts::make_mjts_impl_recu(calsht,7);
#elif defined PROB
  auto mjts_impl = mjts::impl::Mjts::make_mjts_impl_prob(calsht,7,{50,20,10,2});
#endif

  int rng;

  player.num = M;
  std::string str;

  if(!calsht) return 1;

  std::map<int,std::string> dict = {
    {0,"1m"},{1,"2m"},{2,"3m"},{3,"4m"},{4,"5m"},{5,"6m"},{6,"7m"},{7,"8m"},{8,"9m"},
    {9,"1p"},{10,"2p"},{11,"3p"},{12,"4p"},{13,"5p"},{14,"6p"},{15,"7p"},{16,"8p"},{17,"9p"},
    {18,"1s"},{19,"2s"},{20,"3s"},{21,"4s"},{22,"5s"},{23,"6s"},{24,"7s"},{25,"8s"},{26,"9s"},
    {27,"1z"},{28,"2z"},{29,"3z"},{30,"4z"},{31,"5z"},{32,"6z"},{33,"7z"}
  };

  std::cout << "Enter " << M << " tiles.\n";
  std::cin >> str;
  std::cout << "Enter Range\n";
  std::cin >> rng;
  set_tile(str,player.hand);
  auto [sht,mode,disc,wait] = calsht(player.hand,M/3,MODE);
  std::cout << "\nThe shanten number is " << sht-1 << std::endl;
  std::bitset<K> bs(disc);

  auto start = std::chrono::system_clock::now();
  auto stat = mjts_impl.calc2(player,rng);
  auto end = std::chrono::system_clock::now();
  std::cout << "\nTile\tDisc\tPrb\tRdy" << std::endl;

  for(int i=0; i<K; ++i){
    if(player.hand[i] > 0){
      std::cout << dict[i] << '\t' << bs[i] << '\t' << stat[i].prb << '\t' << stat[i].rdy << std::endl;
    }
  }
  std::cout << "\nTime (msec.)\t" << std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count() << std::endl;

  return 0;
}
