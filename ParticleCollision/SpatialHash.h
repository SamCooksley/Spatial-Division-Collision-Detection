#ifndef _SPATIALHASH_H_
#define _SPATIALHASH_H_

#include "Maths.h"
#include "Renderer.h"

#include "HashTable.h"

template<class Type>
class SpatialHash : public HashTable<Type>
{
private:
  int m_width;  //!< Amount of columns.
  int m_height; //!< Amount of rows. 

  float m_cellWidth;  //!< Width of a cell. 
  float m_cellHeight; //!< Height of a cell.

public: 
  /**
   * \brief Constructor.
   */
  SpatialHash(void) 
  {
    m_width = m_height = 0;
    m_cellWidth = m_cellHeight = 0;
  } 

  ~SpatialHash(void) { } //!< Destructor.

  /**
   * \brief Set the size and create the hash table.
   *
   * \param [in] _width
   * \param [in] _height
   */
  void Create(int _width, int _height)
  {
    m_width = _width;
    m_height = _height;

    //create the hash table with the size of the total cell count
    HashTable<Type>::Create(m_width * m_height);
  }

  /**
   * \brief Set the size of the grid.
   *
   * \param [in] _width  Total width.
   * \param [in] _height Total height.
   */
  void SetSize(float _width, float _height)
  {
    //calculate the cell size from the new total size.
    m_cellWidth  = _width  / m_height;
    m_cellHeight = _height / m_width;
  }

  /**
   * \brief Hash the position into an index for the hash table.
   *
   * \param [in] _position Position to hash.
   * \return Returns the hash value (index).
   */
  int Hash(const Vector2 &_position) const
  {
    //convert the world position to cell position
    int x, y;
    GetCell(_position, x, y);

    //convert the cell position to a unique number.
    return HashCell(x, y);
  }

  /**
   * \brief Get the cell that a position is in.
   *
   * \param [in]  _position Position to get the cell of.
   * \param [out] _x        Cell x position.
   * \param [out] _y        Cell y position.
   */
  void GetCell(const Vector2 &_position, int &_x, int &_y) const
  {
    _x = (int)floor(_position.x / m_cellWidth);  //get the column of the position 
    _y = (int)floor(_position.y / m_cellHeight); //get the row of the position
  }

  /**
   * \brief Get the hash of a cell position.
   *
   * \return Returns the hash value.
   */
  int HashCell(int _x, int _y)
  {
    //convert the cell position to a unique number.
    return _x + _y * m_width; 
  }

  /**
   * \brief performs _operation for each element combination in each bucket.
   *
   * \param [in] _operation Function to run for each element-element combination.
   */
  template<class Functor>
  void CheckCollision(Functor &_operation)
  {
    //dont check collisions if there is no hashtable
    if (m_hashTable == nullptr) { return; }

    //go through each bucket
    for (int c = 0; c < m_size; c++)
    {
      //get the bucket
      auto& cell = m_hashTable[c];

      //go through each particle in the bucket. 
      //ignore the last element as it would not find 
      //anything to collide against.
      //use signed int iterator as the cell size may be 
      //negative (if there is no objects in a bucket). 
      for (int i = 0; i < (int)cell.size() - 1; i++)
      {
        //get the particle
        auto& p1 = *cell.at(i);

        //go through all other particles that have not been
        //checked.
        //start at i so it does not check previously checked
        //combinations. Add one so it does not check itself.
        for (unsigned int j = i + 1; j < cell.size(); j++)
        {
          //get the other particle
          auto& p2 = *cell.at(j);
          
          //do the collision operation
          _operation(p1, p2);
        }
      }
    }
  }

  /**
   * \brief Draw the spatial hash.
   * Draws a grid where the colour represents the amount of objects
   * in the bucket. White is empty and grey is maximum.
   *
   * \param [in] _renderer Renderer to draw to.
   */
  void Draw(Renderer &_renderer)
  {
    //dont draw if there is no hashtable
    if (m_hashTable == nullptr) { return; }

    //rect to draw cells
    SDL_Rect rect = { 0, 0, (int)ceil(m_cellWidth), (int)ceil(m_cellHeight) };

    //get the max amount of particles in a cell
    int max = MaxBucketSize();
    max = (max < 1? 1: max); //avoid divide by 0

    //go through all buckets.
    for (int i = 0; i < m_size; i++)
    {
      //get the bucket size percentage of the max bucket size
      float t = (float)m_hashTable[i].size() / max;
      //use white for no elements in the bucket and
      //grey for the highest.
      Uint8 col = (Uint8)Lerp(255, 100, t);

      //get the position of the cell
      rect.x = (int)(i % m_width  * m_cellWidth);
      rect.y = (int)(i / m_height * m_cellHeight);

      //set the colour of the cell.
      _renderer.SetRenderColour(col, col, col);

      //draw the cell
      SDL_RenderFillRect(_renderer.Get(), &rect);
    }
  }

  int Width(void)  const { return m_width;  } //!< Get the width.
  int Height(void) const { return m_height; } //!< Get the height.
};

#endif _SPATIALHASH_H_