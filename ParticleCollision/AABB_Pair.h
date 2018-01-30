#ifndef _AABB_PAIR_H_
#define _AABB_PAIR_H_

#include <vector>

namespace AABBTree
{
  /**
   * \brief Group two items.
   */
  template <class T>
  struct Pair
  {
   public:
    T* a;
    T* b;
  };

  template <class T>
  using PairList = std::vector<Pair<T>>;
}

#endif //_COLLIDERPAIR_H_