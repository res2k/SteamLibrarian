/*
  SteamLibrarian
  Copyright (c) 2015 Frank Richter

  This file is part of SteamLibrarian.

  SteamLibrarian is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  SteamLibrarian is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with SteamLibrarian.  If not, see <http://www.gnu.org/licenses/>.
*/

/**\file
 * A Red black tree implementation
 */
#ifndef __REDBLACKTREE_HPP__
#define __REDBLACKTREE_HPP__

#include <memory>
#include <stack>

namespace rbt_detail
{
  template<typename KeyType, typename ValueType>
  struct TypesHelper
  {
    typedef ValueType param_type;
    struct Container
    {
      ValueType value;
      Container() {}
      Container(ValueType&& value) : value(value) {}
      Container& operator=(Container&& other)
      {
        value = std::move(other.value);
        return *this;
      }
      ValueType* ValuePtr() { return &value; }
      const ValueType* ValuePtr() const { return &value; }
    };
    typedef std::pair<const KeyType&, const ValueType&> iterator_value_type;
    static iterator_value_type MakeValue(const KeyType& key, const Container& container)
    {
      return iterator_value_type (key, container.value);
    }
  };

  template<typename KeyType>
  struct TypesHelper <KeyType, void>
  {
    struct undefined;
    typedef undefined* param_type;
    struct Container {
      Container() {}
      Container(param_type&& value) {}
      Container& operator=(Container&& other) { return *this; }
      void* ValuePtr() { return this; /* Non-null value */ }
      const void* ValuePtr() const { return this; /* Non-null value */ }
    };
    typedef const KeyType& iterator_value_type;
    static iterator_value_type MakeValue(const KeyType& key, const Container&)
    {
      return key;
    }
  };
} // namespace detail

/**
 * RedBlackTree code adapted from Julienne Walker:
 * http://www.eternallyconfuzzled.com/tuts/datastructures/jsw_tut_rbtree.aspx
 */
template<typename T, typename V = void>
class RedBlackTree
{
public:
  typedef T key_type;
  typedef typename rbt_detail::TypesHelper<T, V>::iterator_value_type value_type;
private:
  class Node : public rbt_detail::TypesHelper<T, V>::Container
  {
  public:
    T key;
    std::unique_ptr<Node> children[2];
    enum Color { Red, Black };
    Color color;
    size_t userCount;
    size_t counts[2];

    Node(T&& key, size_t userCount = 0)
      : key(key), color(Red), userCount(userCount)
    {
      counts[0] = 1;
      counts[1] = userCount;
    }
    Node(T&& key, typename rbt_detail::TypesHelper<T, V>::param_type&& value, size_t userCount = 0)
      : rbt_detail::TypesHelper<T, V>::Container(value), Node(key, userCount)
    {}

    void MoveValue(Node& from)
    {
      static_cast<rbt_detail::TypesHelper<T, V>::Container&> (*this) =
        std::move(static_cast<rbt_detail::TypesHelper<T, V>::Container&> (from));
    }
  };
  std::unique_ptr<Node> root;

  static bool isRed(const Node* node) { return node && (node->color == Node::Red); }
  static bool isBlack(const Node* node) { return !node || (node->color == Node::Black); }

  std::unique_ptr<Node> internalInsert(std::unique_ptr<Node>&& node, std::unique_ptr<Node>&& newNode)
  {
    if (!node)
    {
      return std::move(newNode);
    }
    else
    {
      int d = (node->key < newNode->key) ? 1 : 0;
      node->children[d] = internalInsert(std::move(node->children[d]), std::move(newNode));

      // Rebalance
      if (isRed(node->children[d].get()))
      {
        if (isRed(node->children[d ^ 1].get()))
        {
          /* Case 1 (two red children, make parent black) */
          node->color = Node::Red;
          node->children[0]->color = Node::Black;
          node->children[1]->color = Node::Black;
        }
        else
        {
          /* Cases 2 & 3 (children of mixed colors) */
          if (isRed(node->children[d]->children[d].get()))
            node = rotate(std::move(node), d ^ 1);
          else if (isRed(node->children[d]->children[d ^ 1].get()))
            node = rotate2(std::move(node), d ^ 1);
        }
      }

      updateCount(node);

      return std::move (node);
    }
  }

  // Single rotation
  std::unique_ptr<Node> rotate(std::unique_ptr<Node>&& node, int d)
  {
    std::unique_ptr<Node> save = std::move(node->children[d ^ 1]);
    node->children[d ^ 1] = std::move(save->children[d]);
    save->children[d] = std::move(node);

    save->children[d]->color = Node::Red;
    save->color = Node::Black;

    updateCount(save->children[d]);
    updateCount(save);

    return std::move(save);
  }

  // Double rotation
  std::unique_ptr<Node> rotate2(std::unique_ptr<Node>&& node, int d)
  {
    node->children[d ^ 1] = std::move(rotate(std::move(node->children[d ^ 1]), d ^ 1));
    return rotate(std::move(node), d);
  }

  void updateCount(const std::unique_ptr<Node>& node)
  {
    for (int n = 0; n < 2; n++)
    {
      size_t c = ((node->children[0]) ? node->children[0]->counts[n] : 0)
               + ((node->children[1]) ? node->children[1]->counts[n] : 0)
               + ((n == 0) ? 1 : node->userCount);
      node->counts[n] = c;
    }
  }

  void updateCountAfterRemove(const std::unique_ptr<Node>& node, const T& key)
  {
    if (!node) return;
    int dir = (node->key < key) ? 1 : 0;
    updateCountAfterRemove(node->children[dir], key);
    updateCount(node);
  }

  size_t queryCount(const T& key, int n) const
  {
    if (!root) return 0;
    /* The "count" is sum of all node counts up until the node itself
    if the tree was traversed depth-first left-to-right.
    Determine this number w/o doing a whole traversal. */
    const Node* node = root.get();
    size_t index = 0;
    while (node)
    {
      if (node->key == key)
      {
        // Node found
        // Increase current index by the count sum of the left subtree.
        if (node->children[0])
          index += node->children[0]->counts[n];
        node = nullptr;
      }
      else if (node->key < key)
      {
        // Go right.
        // The index must be increased by the count sum of the left subtree.
        if (node->children[0])
          index += node->children[0]->counts[n];
        // And, of course, the node itself
        index += (n == 0) ? 1 : node->userCount;
        node = node->children[1].get();
      }
      else
      {
        // Go left
        // Index can stay as-is
        node = node->children[0].get();
      }
    }
    return index;
  }

  void setUserCount(const std::unique_ptr<Node>& node, const T& key, size_t count)
  {
    if (!node) return;
    if (node->key == key)
    {
      node->userCount = count;
    }
    else
    {
      int dir = (node->key < key) ? 1 : 0;
      setUserCount(node->children[dir], key, count);
    }
    updateCount(node);
  }
public:
  RedBlackTree()
  {
  }
  RedBlackTree(const RedBlackTree& other) = delete;
  RedBlackTree(RedBlackTree&& other) : root(std::move(other.root)) {}

  RedBlackTree& operator=(const RedBlackTree& other) = delete;
  RedBlackTree& operator=(RedBlackTree&& other)
  {
    root = std::move(other.root);
    return *this;
  }

  void clear()
  {
    root.reset();
  }

  void insert(T&& key)
  {
    std::unique_ptr<Node> n(new Node(std::move(key)));
    root = internalInsert(std::move(root), std::move(n));
    root->color = Node::Black;
  }

  void insert(T&& key, typename rbt_detail::TypesHelper<T, V>::param_type&& value)
  {
    std::unique_ptr<Node> n(new Node(std::move(key), std::move(value)));
    root = internalInsert(root, n);
    root->color = Node::Black;
  }

  void remove (const T& key)
  {
    if (root)
    {
      /* False tree root */
      std::unique_ptr<Node> head (new Node(T()));
      /* Helpers */
      std::unique_ptr<Node>* pq = &head;
      std::unique_ptr<Node>* pp = nullptr;
      std::unique_ptr<Node>* pg = nullptr;
      /* Found item */
      Node* f = nullptr;
      int dir = 1;

      /* Set up helpers */
      (*pq)->children[1] = std::move (root);

      /* Search and push a red down */
      while ((*pq)->children[dir])
      {
        int last = dir;

        /* Update helpers */
        pg = pp;
        pp = pq;
        pq = &((*pq)->children[dir]);
        dir = ((*pq)->key < key) ? 1 : 0;

        /* Save found node */
        if ((*pq)->key == key)
          f = pq->get();

        /* Push the red node down */
        if (isBlack((*pq).get()) && isBlack((*pq)->children[dir].get()))
        {
          if (isRed((*pq)->children[dir ^ 1].get()))
          {
            (*pp)->children[last] = std::move (rotate(std::move (*pq), dir));
            pp = &((*pp)->children[last]);
          }
          else if (isBlack((*pq)->children[dir ^ 1].get()))
          {
            std::unique_ptr<Node>* ps = &((*pp)->children[last ^ 1]);

            if (*ps)
            {
              if (isBlack((*ps)->children[last ^ 1].get()) && isRed((*ps)->children[last].get()))
              {
                /* Color flip */
                (*pp)->color = Node::Black;
                (*ps)->color = Node::Red;
                (*pq)->color = Node::Red;
              }
              else
              {
                int dir2 = (*pg)->children[1] == *pp ? 1 : 0;

                if (isRed((*ps)->children[last].get()))
                {
                  (*pg)->children[dir2] = rotate2(std::move(*pp), last);
                  pp = &((*pp)->children[last]);
                }
                else if (isRed((*ps)->children[last ^ 1].get()))
                {
                  (*pg)->children[dir2] = rotate(std::move(*pp), last);
                  pp = &((*pp)->children[last]);
                }

                /* Ensure correct coloring */
                (*pq)->color = (*pg)->children[dir2]->color = Node::Red;
                (*pg)->children[dir2]->children[0]->color = Node::Black;
                (*pg)->children[dir2]->children[1]->color = Node::Black;
              }
            }
          }
        }
      }

      /* Replace and remove if found */
      if (f)
      {
        f->key = std::move ((*pq)->key);
        f->MoveValue (**pq);
        (*pp)->children[((*pp)->children[1] == (*pq)) ? 1 : 0] = std::move ((*pq)->children[(*pq)->children[0] ? 0 : 1]);
        updateCountAfterRemove(head->children[1], (*pp)->key);
      }

      /* Update root and make it black */
      root = std::move (head->children[1]);
      if (root)
        root->color = Node::Black;
    }
  }

  size_t size() const { return root ? root->counts[0] : 0; }

  size_t totalUserCount() const { return root ? root->counts[1] : 0; }

  void setUserCount (const T& key, size_t count)
  {
    setUserCount(root, key, count);
  }

  size_t querySortedIndex(const T& key) const
  {
    /* The "sorted index" is the position a node would have
    if the tree was traversed depth-first left-to-right.
    Determine this number w/o doing a whole traversal. */
    return queryCount(key, 0);
  }

  size_t queryUserCount (const T&key) const
  {
    return queryCount(key, 1);
  }

  const T* queryBySortedIndex(size_t index) const
  {
    const Node* node = root.get();
    while (node)
    {
      size_t leftCount = node->children[0] ? node->children[0]->counts[0] : 0;
      if (leftCount == index)
        return &(node->key);
      else if (leftCount > index)
      {
        // Go left
        node = node->children[0].get();
      }
      else
      {
        // Go right
        node = node->children[1].get();
        index -= (leftCount+1);
      }
    }
    return nullptr;
  }

  V* queryValue (const T& key)
  {
    Node* node = root.get();
    while (node)
    {
      if (node->key == key)
      {
        return node->ValuePtr();
      }
      else
      {
        int dir = (node->key < key) ? 1 : 0;
        node = node->children[dir].get();
      }
    }
    return nullptr;
  }

  const V* queryValue(const T& key) const
  {
    const Node* node = root.get();
    while (node)
    {
      if (node->key == key)
      {
        return node->ValuePtr();
      }
      else
      {
        int dir = (node->key < key) ? 1 : 0;
        node = node->children[dir].get();
      }
    }
    return nullptr;
  }

  class const_iterator
  {
    std::stack<std::pair<const Node*, int>> nodes_stack;
    
    void descend(const Node* node)
    {
      if (!node) return;
      while (node)
      {
        nodes_stack.push(std::make_pair(node, 0));
        node = node->children[0].get();
      }
      nodes_stack.top().second = 1;
    }
  public:
    const_iterator() {}
    const_iterator(const RedBlackTree& tree)
    {
      descend(tree.root.get());
    }

    bool operator!=(const const_iterator& other) const
    {
      // Compare positions in stack.
      return nodes_stack != other.nodes_stack;
    }
    const_iterator& operator++()
    {
      while (true)
      {
        while (!nodes_stack.empty() && (nodes_stack.top().second == 2))
        {
          // Node was already visited
          nodes_stack.pop();
        }
        // All nodes visited
        if (nodes_stack.empty()) return *this;
        // Top node and it's left subtree were visited, descend right
        if (nodes_stack.top().second == 1)
        {
          nodes_stack.top().second = 2;
          const Node* right_child(nodes_stack.top().first->children[1].get());
          if (!right_child) continue;
          descend(right_child);
          return *this;
        }
        // Visiting top node
        nodes_stack.top().second = 1;
        return *this;
      }
    }
    value_type operator*() const
    {
      const Node* current_node(nodes_stack.top().first);
      return rbt_detail::TypesHelper<T, V>::MakeValue(current_node->key, *current_node);
    }
  };
  const_iterator begin() const { return const_iterator(*this); }
  const_iterator end() const { return const_iterator(); }
};

#endif // __REDBLACKTREE_HPP__
