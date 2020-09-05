#ifndef RANDOM_INDEX_HPP
#define RANDOM_INDEX_HPP

#include <numeric> 
#include <random>
#include <vector>
#include "constant.hpp"

class RandomIndex
{
  mutable std::vector<int> index;
  mutable std::mt19937_64 mt;

public:
  RandomIndex() : index(K, 0), mt(std::random_device{}())
  {
    std::iota(index.begin(), index.end(), 0);
  }

  RandomIndex(unsigned int seed) : index(K, 0), mt(seed)
  {
    std::iota(index.begin(), index.end(), 0);
  }

  std::vector<int>& operator()()
  {
    std::shuffle(index.begin(), index.end(), mt);

    return index;
  }
};

#endif
