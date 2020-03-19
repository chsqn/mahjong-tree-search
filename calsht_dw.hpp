#ifndef CALSHT_DW_HPP
#define CALSHT_DW_HPP

#include <cstdint>
#include <tuple>
#include <vector>
#include "constant.hpp"

class CalshtDW{
private:
  using Vec  = std::vector<std::int64_t>;
  using Iter = std::vector<Vec>::iterator;

  std::vector<Vec> mp1;
  std::vector<Vec> mp2;

  Iter itr1;
  Iter itr2;

  void shift(int& lv, int rv, std::int64_t& lx, std::int64_t rx, std::int64_t& ly, std::int64_t ry) const;
  void add(Vec& lhs, const Vec& rhs) const;
  void add(Vec& lhs, const Vec& rhs, int j) const;
  Iter read_file(Iter first, Iter last, const char* file_name) const;
  std::tuple<int,std::int64_t,std::int64_t> calc_lh(const int* t, int m) const;
  std::tuple<int,std::int64_t,std::int64_t> calc_sp(const int* t) const;
  std::tuple<int,std::int64_t,std::int64_t> calc_to(const int* t) const;

public:
  CalshtDW();
  bool operator!() const;
  std::tuple<int,int,std::int64_t,std::int64_t> operator()(const std::vector<int>& t, int m, int mode) const;
};

#endif
