#ifndef _LINKEDLIST_H_
#define _LINKEDLIST_H_

#include "ListNode.h"

template<class Type>
class LinkedList
{
public:
  typedef ListNode<Type> Node;
  typedef unsigned int uint;

private:
  Node *m_first; //!< First node in the list.
  Node *m_last;  //!< Last node in the list.

  uint m_size; //!< Amount of elements.

public: 
  /**
   * \brief Constructor.
   */
  LinkedList(void)
  {
    m_first = m_last = nullptr;
    m_size = 0;
  }

  /**
   * \brief Destructor.
   */
  ~LinkedList(void) 
  {
    clear(); //!< Destroy all elements 
  }

  Node *First(void) { return m_first; } //!< Get the first element.
  Node *Last (void) { return m_last;  } //!< Get the last element.

  uint Size(void) const { return m_size; } //!< Get the element count.

  /** 
   * \brief Add an element to the end of the list.
   *
   * \param [in] _value Value to add.
   */
  void Add(const Type &_value)
  {
    //create a node for the data
    Node *m_new = new Node(_value);
    //link it to the previous last element
    m_new->m_prev = m_last; 

    //set it to the first element if there are no others
    if (m_first == nullptr) { m_first = m_new; }
    //else add it to the last element
    else { m_last->m_next = m_new; }

    //say that its the last element in the list
    m_last = m_new;
    //increase the list size
    m_size++; 
  }

  /**
   * \brief Insert a value in the middle of a list.
   *
   * \param [in] _value Value to add.
   * \param [in] _index Position of the data.
   */
  void Insert(const Type &_value, uint _index)
  {
    Node *next = nullptr;
    Node *prev = nullptr;

    //if it is out of range, set it to the last element
    if (_index >= m_size)
    {
      prev = m_last;
    }
    //else get the nodes that would surround the new element
    else
    {
      //this should not be null as the index range has already been 
      //checked
      next = At(_index); 
      prev = next->m_prev;
    }

    //create a new node for the data
    Node *m_new = new Node(_value);
    //link it between the next and previous nodes
    m_new->m_next = next;
    m_new->m_prev = prev;

    //if there is a previous node, link it to the new element
    if (prev != nullptr)
    {
      prev->m_next = m_new;
    }
    //else it is the first element
    else
    {
      m_first = m_new;
    }

    //if there is a next node, link it
    if (next != nullptr)
    {
      next->m_prev = m_new;
    }
    //else it is the last node
    else
    {
      m_last = m_new;
    }

    //increase the count
    m_size++;
  }

  /**
   * \brief Get the element at the index.
   * speed = O(n/2)
   *
   * \param [in] _index Position to search.
   * \return Returns the node at index.
   */
  Node *At(uint _index)
  {
    //if the index is greater than half the list size
    //treverse the list backwards to reduce iterations.
    int dir = (_index > m_size / 2)? -1: 1;
    //if it is going backward, start at the last node,
    //else start at the first
    Node *curr = (dir < 0)? m_last: m_first;
    //starting index
    int count = (dir < 0)? m_size - 1: 0;

    //while a node exists
    while (curr != nullptr)
    {
      //if it has found the index, exit
      if (count == _index) { break; }

      //go to the next node (or previous if going backwards)
      curr = (dir < 0)? curr->m_prev: curr->m_next;
      //increment the index.
      count += dir;
    }
    
    //return the last node. null if it has not been found
    return curr;
  }

  /**
   * \brief Find the node with the given value.
   * speed = O(n)
   *
   * \param [in] _value Value to find.
   * \return Returns the node with the value.
   */
  Node *Find(const Type &_value)
  {
    //start at the first node
    Node *curr = m_first;

    //repeat until there are no nodes
    while (curr != nullptr) 
    {
      //if the values match, exit
      if (curr->m_value == _value) { break; }

      //go to the next node.
      curr = curr->m_next;
    }

    //return the node, null if not found
    return curr;
  }

  /**
   * \brief Removes a node from a list.
   *
   * \param [in] _node Node to remove.
   */
  void Remove(Node *_node)
  {
    //if the node exists
    if (_node != nullptr)
    {
      //if there is a previous node, update it's next 
      //node value.
      if (_node->m_prev != nullptr)
      {
        _node->m_prev->m_next = _node->m_next;
      }
      //else this node was the first, so set the first
      //to the next node.
      else
      {
        m_first = _node->m_next;
      }

      //if there is a next node, update it's
      //previous node value.
      if (_node->m_next != nullptr)
      {
        _node->m_next->m_prev = _node->m_prev;
      }
      //else this node was the last, so set the 
      //last to the previous.
      else
      {
        m_last = _node->m_prev;
      }

      //delete the node
      delete _node;
      //reduce the list size
      m_size--;
    }
  }

  /**
   * \brief Remove data at an index location.
   * Speed = O(n)
   *
   * \param [in] _index Position to remove the data.
   */
  void RemoveAt(uint _index)
  {
    //get the node at the index and remove it.
    Remove(At(_index));
  }

  /**
   * \brief Remove data.
   * Removes the first occurrence of the data.
   * Speed = O(n)
   *
   * \param [in] _value Data to remove from the list.
   */
  void Remove(const Type &_value)
  {
    //get the node of value and remove it.
    Remove(Find(_value));
  }

  /**
   * \brief Remove all nodes from the list.
   * Speed = O(n)
   */
  void Clear(void)
  {
    //current node being deleted.
    //two values are needed as the next node
    //has to be obtained before the current 
    //one is deleted.
    Node *curr = nullptr;
    //start at the first
    Node *next = m_first; 

    //repeat until there are no nodes.
    while(next != nullptr)
    {
      //update the current node
      curr = next;
      //get the next node
      next = curr->m_next;
      //delete the node
      delete curr;
    }

    //reset list data
    m_first = nullptr;
    m_last = nullptr;
    m_size = 0;
  }
};

#endif //_LINKEDLIST_H_