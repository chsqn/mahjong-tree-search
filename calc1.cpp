#include <iostream>
#include <chrono>
#include <string>
#include "calsht_dw.hpp"
#include "player_core.hpp"
#include "settile.hpp"
#include "mjts_recu.hpp"
#include "mjts_prob.hpp"
#include "mjts_impl.hpp"

int main()
{
  constexpr int M = 13;
  constexpr int MODE = 7;

  CalshtDW calsht;
  Player player;
#ifdef RECU
  mjts::impl::Mjts<mjts::Recu> mjts_impl(calsht, 7);
#elif defined PROB
  mjts::impl::Mjts<mjts::Prob> mjts_impl(calsht, 7, 50, 20, 10, 2);
#endif

  int rng;

  player.num = M;
  std::string str;

  std::cout << "Enter " << M << " tiles.\n";
  std::cin >> str;
  std::cout << "Enter Range\n";
  std::cin >> rng;
  set_tile(str, player.hand);
  auto [sht, mode, disc, wait] = calsht(player.hand, M/3, MODE);
  std::cout << "\nThe shanten number is " << sht-1 << std::endl;

  auto start = std::chrono::system_clock::now();
  auto stat = mjts_impl.calc1(player, rng);
  auto end = std::chrono::system_clock::now();

  std::cout << "\nPrb\tRdy" << std::endl;
  std::cout << stat.prb << '\t' << stat.rdy << std::endl;
  std::cout << "\nTime (msec.)\t" << std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count() << std::endl;

  return 0;
}
