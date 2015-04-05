#include <cxxtest/TestSuite.h>

#include "../RedBlackTree.hpp"

#include <boost/array.hpp>
#include <boost/iterator/counting_iterator.hpp>

#include <algorithm>

template<typename TreeType, int N>
class TreeTester
{
public:
  typedef typename TreeType::key_type value_type;
  void operator()()
  {
    value_type insert_order[N];
    std::copy(boost::counting_iterator<value_type>(0), boost::counting_iterator<value_type>(N), insert_order);
    do
    {
      {
        TreeType tree;
        for (value_type v : insert_order)
        {
          tree.insert(std::move(v));
        }
        for (size_t i = 0; i < tree.size(); i++)
        {
          const value_type* p = tree.queryBySortedIndex(i);
          TS_ASSERT(p);
          TS_ASSERT_EQUALS(*p, static_cast<value_type> (i));
        }
      }

      value_type delete_order[N];
      std::copy(boost::counting_iterator<value_type>(0), boost::counting_iterator<value_type>(N), delete_order);
      do
      {
        for (int delete_num = 1; delete_num <= N; delete_num++)
        {
          TreeType tree;
          for (value_type v : insert_order)
          {
            tree.insert(std::move(v));
          }

          std::set<value_type> expected(boost::counting_iterator<value_type>(0),
                                        boost::counting_iterator<value_type>(N));

          for (int d = 0; d < delete_num; d++)
          {
            expected.erase(delete_order[d]);
          }
          for (int d = 0; d < delete_num; d++)
          {
            tree.remove(delete_order[d]);
          }
          TS_ASSERT_EQUALS(expected.size(), tree.size());
          int i = 0;
          for (value_type v : expected)
          {
            const value_type* p = tree.queryBySortedIndex(i);
            TS_ASSERT(p);
            TS_ASSERT_EQUALS(*p, v);
            i++;
          }
        }
      } while (std::next_permutation(&(delete_order[0]), &(delete_order[N])));
    } while (std::next_permutation(&(insert_order[0]), &(insert_order[N])));
  }
};

template<typename TreeType, int N>
struct TestTrees
{
  static void Test();
};

template<typename TreeType>
struct TestTrees<TreeType, 0>
{
  static void Test()
  {
  }
};

template<typename TreeType, int N>
void TestTrees<TreeType, N>::Test()
{
  TestTrees<TreeType, N - 1>::Test();

  TreeTester<TreeType, N> t;
  t();
}

class RedBlackTreeTestSuite : public CxxTest::TestSuite
{
public:
  typedef RedBlackTree<int> tree_type;

  void testRemove1()
  {
    int insert_order[] = { 1, 0, 2 };
    {
      tree_type tree;
      for (int v : insert_order)
      {
        tree.insert(std::move(v));
      }
      for (size_t i = 0; i < tree.size(); i++)
      {
        const int* p = tree.queryBySortedIndex(i);
        TS_ASSERT(p);
        TS_ASSERT_EQUALS(*p, static_cast<int> (i));
      }
    }

    int delete_order[] = { 0, 1, 2 };
    {
      tree_type tree;
      for (int v : insert_order)
      {
        tree.insert(std::move(v));
      }

      std::set<int> expected(boost::counting_iterator<int>(0),
        boost::counting_iterator<int>(3));

      expected.erase(delete_order[0]);
      expected.erase(delete_order[1]);
      tree.remove(delete_order[0]);
      tree.remove(delete_order[1]);

      TS_ASSERT_EQUALS(expected.size(), tree.size());
      int i = 0;
      for (int v : expected)
      {
        const int* p = tree.queryBySortedIndex(i);
        TS_ASSERT(p);
        TS_ASSERT_EQUALS(*p, v);
        i++;
      }
    }
  }

  void testRemove2()
  {
    int insert_order[] = { 0, 1, 2, 3 };
    {
      tree_type tree;
      for (int v : insert_order)
      {
        tree.insert(std::move(v));
      }
      for (size_t i = 0; i < tree.size(); i++)
      {
        const int* p = tree.queryBySortedIndex(i);
        TS_ASSERT(p);
        TS_ASSERT_EQUALS(*p, static_cast<int> (i));
      }
    }

    int delete_order[] = { 1, 0, 2, 3 };
    {
      tree_type tree;
      for (int v : insert_order)
      {
        tree.insert(std::move(v));
      }

      std::set<int> expected(boost::counting_iterator<int>(0),
        boost::counting_iterator<int>(sizeof(delete_order)/sizeof(delete_order[0])));

      expected.erase(delete_order[0]);
      tree.remove(delete_order[0]);

      TS_ASSERT_EQUALS(expected.size(), tree.size());
      int i = 0;
      for (int v : expected)
      {
        const int* p = tree.queryBySortedIndex(i);
        TS_ASSERT(p);
        TS_ASSERT_EQUALS(*p, v);
        i++;
      }
    }
  }

  void testExhaustive()
  {
    /* Exhaustively tests all insertion and deletion
     * combinations.
     * But takes a *long* time, thus commented out... */
    //TestTrees<tree_type, 7>::Test();
  }
};

