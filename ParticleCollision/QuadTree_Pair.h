#ifndef _QUADTREE_PAIR_H_
#define _QUADTREE_PAIR_H_

#include <vector>

namespace QuadTree
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

#endif //_QUADTREE_PAIR_H_