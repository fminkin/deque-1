#include <gtest/gtest.h>
#include "Deque.hpp"
#include <deque>
#include <algorithm>
#include <random>

std::default_random_engine generator;

using std::deque;
using NDeque::Deque;

using ::testing::TestWithParam;
using ::testing::Values;

enum EQueries
{
    PUSH_BACK,
    PUSH_FRONT,
    POP_BACK,
    POP_FRONT,
    SORT,
    REVERSE_SORT,
    RANDOM_SHUFFLE,
    REVERSE,
    EQUALITY_CHECK,
    NUMBER_OF_QUERY_TYPES
};

class DequeFunctionalityTest : public ::testing::TestWithParam<size_t>
{
    
};

std::pair<size_t, size_t> chooseSubsegment(size_t size)
{
    std::uniform_int_distribution<int> sizeDistribution(0, size - 1);
    size_t left = sizeDistribution(generator);
    size_t right = sizeDistribution(generator);
    if (left > right)
        std::swap(left, right);
    return std::make_pair(left, right);
}

TEST_P(DequeFunctionalityTest, pushTests)
{
    std::uniform_int_distribution<int> backOrFront(0, 1);
    Deque<int> my;
    size_t currentSize = 0;
    ASSERT_EQ(my.size(), currentSize);
    size_t queries = GetParam();
    for (size_t i = 0; i < queries; ++i)
    {
        
        int valueToInsert = generator();
        if (backOrFront(generator) == PUSH_BACK)
        {
            my.pushBack(valueToInsert);
            ASSERT_EQ(my.back(), valueToInsert);
        }
        else
        {
            my.pushFront(valueToInsert);
            ASSERT_EQ(my.front(), valueToInsert);
        }
        ++currentSize;
        ASSERT_EQ(my.size(), currentSize);
    }
}

TEST_P(DequeFunctionalityTest, EveryPossibleTest)
{
    std::uniform_int_distribution<int> queryGetter(0, NUMBER_OF_QUERY_TYPES - 1);
    generator.seed(1);
    Deque<int> my;
    deque<int> stls;
    ASSERT_EQ(my.size(), stls.size());
    size_t queries = GetParam();
    for (size_t i = 0; i < queries; ++i)
    {
        EQueries query = static_cast<EQueries> (queryGetter(generator));
        int value;
        std::pair<size_t, size_t> leftRight;
        switch (query)
        {
            case PUSH_BACK:
                value = generator();
                my.pushBack(value);
                stls.push_back(value);
                ASSERT_EQ(my.back(), value);
            break; case PUSH_FRONT:
                value = generator();
                my.pushFront(value);
                stls.push_front(value);
                ASSERT_EQ(my.front(), value);
            break; case POP_BACK:
                if (my.size())
                {
                    my.popBack();
                    stls.pop_back();    
                }
            break; case POP_FRONT:
                if (my.size())
                {
                    my.popFront();
                    stls.pop_front();
                }
            break; case SORT:
                if (my.size())
                {
                    leftRight = chooseSubsegment(my.size());
                    std::sort(my.begin() + leftRight.first, my.begin() + leftRight.second + 1);
                    std::sort(stls.begin() + leftRight.first, stls.begin() + leftRight.second + 1);
                }
            break; case REVERSE_SORT:
                if (my.size())
                {
                    leftRight = chooseSubsegment(my.size());
                    std::sort(my.rbegin() + leftRight.first, my.rbegin() + leftRight.second + 1);
                    std::sort(stls.rbegin() + leftRight.first, stls.rbegin() + leftRight.second + 1);
                }
            break; case RANDOM_SHUFFLE:
                if (my.size())
                {
                    leftRight = chooseSubsegment(my.size());
                    std::shuffle(my.begin() + leftRight.first, my.begin() + leftRight.second + 1, std::default_random_engine(generator));
                    std::shuffle(stls.begin() + leftRight.first, stls.begin() + leftRight.second + 1, generator);
                }
            break; case REVERSE:
                if (my.size())
                {
                    leftRight = chooseSubsegment(my.size());
                    std::reverse(my.begin() + leftRight.first, my.begin() + leftRight.second + 1);
                    std::reverse(stls.begin() + leftRight.first, stls.begin() + leftRight.second + 1);
                }
            break; case EQUALITY_CHECK:
                if (my.size())
                {
                    leftRight = chooseSubsegment(my.size());
                    ASSERT_EQ((int)std::equal(my.begin() + leftRight.first, my.begin() + leftRight.second + 1, stls.begin() + leftRight.first) + i - 1, i);
                    
                }
            break;
        }
        /*printf("Q: %d\n", query);
        for (auto const &it: my)
            printf("%d ", it);
        printf("\n");
        for (auto const &it: stls)
            printf("%d ", it);
        printf("\n");
        */
        ASSERT_EQ(my.size(), stls.size());
        
    }
    ASSERT_TRUE(std::equal(my.begin(), my.end(), stls.begin()));
}

INSTANTIATE_TEST_CASE_P(InstantiationName, DequeFunctionalityTest, ::testing::Values(1, 10, 100, 1000, 10000, 100000, 1000000));



int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}