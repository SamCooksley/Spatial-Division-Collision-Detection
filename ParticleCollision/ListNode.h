#ifndef _LISTNODE_H_
#define _LISTNODE_H_

template<class T>
class LinkedList;

template<class Type>
class ListNode
{
  friend class LinkedList<Type>; 

private:
  Type m_value; //!< Value to store

  ListNode<Type> *m_next; //!< Next node in the list for forward traversal.
  ListNode<Type> *m_prev; //!< Previous node for backward traversal.

public:
  /**
   * \brief Constructor.
   * \param [in] _value The value to store
   */
  ListNode(const Type &_value) 
  {
    m_value = _value;
    m_prev = m_next = nullptr;
  }

  ~ListNode(void) { } //!< Destructor.

  ListNode<Type> *Next    (void) { return m_next; } //!< Get the next node in the list.
  ListNode<Type> *Previous(void) { return m_prev; } //!< Get the previous node in the list.

  Type &Value(void) { return m_value; } //!< Get the value stored.
};

#endif //_LISTNODE_H_