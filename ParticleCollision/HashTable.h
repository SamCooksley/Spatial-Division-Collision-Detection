#ifndef _HASHTABLE_H_
#define _HASHTABLE_H_

#include <vector>

#include "Maths.h"

template<class Type>
class HashTable
{
public:
  typedef std::vector<Type> Bucket;

protected:
  int m_size;          //!< Amount of buckets.
  Bucket* m_hashTable; //!< Hash table with the size of the grid.

public:
  HashTable(void)  { m_size = 0; m_hashTable = nullptr; } //!< Constructor.
  virtual ~HashTable(void) { Destroy(); } //!< Destructor.

  void Destroy(void)
  {
    //if a hash table exists, delete it
    if (m_hashTable != nullptr)
    {
      delete [] m_hashTable;
      m_hashTable = nullptr;
      m_size = 0;
    }
  }

  /**
   * \brief Set the size of the hash table.
   *
   * \param [in] _size Amount of buckets.
   */
  void Create(int _size)
  {
    //if there is already a hash table, delete it.
    Destroy();

    //set the size
    m_size = _size;
    //create the grid with the new size 
    m_hashTable = new Bucket[m_size];
  }

  /**
   * \brief Empty all buckets. 
   */
  void Clear(void)
  {
    //go through all buckets and empty them.
    for (int i = 0; i < m_size; i++)
    {
      m_hashTable[i].clear();
    }
  }

  void Add(int _index, const Type &_object) { m_hashTable[_index].emplace_back(_object); } //!< Add an object to a bucket in the hash table.

  /**
   * \brief Check if a hash exists.
   *
   * \param [in] _index The hash to check.
   * \return Returns true if hash exists.
   */
  bool Exists(int _index)
  {
    return _index >= 0 && _index < m_size;
  }

  /**
   * \brief Get the size of the bucket with the maximum amount of objects.
   * \return Returns the highest bucket size.
   */
  int MaxBucketSize(void) const
  {
    //the the size of the first bucket
    int max = m_hashTable[0].size();

    //check each cell and compare to the max.
    //ignore the first as it is the current max
    for (int i = 1; i < m_size; i++)
    {
      int size = m_hashTable[i].size();
      if (size > max) { max = size; }
    }

    //return the maximum  
    return max;
  }

  int Size(void) const { return m_size; } //!< Get the size of the hash table.
};

#endif //_HASHTABLE_H_