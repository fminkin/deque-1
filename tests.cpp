#include <gtest/gtest.h>
#include "Deque.hpp"
#include <deque>
#include <algorithm>
#include <random>
#include <ctime>
#include <vector>

std::default_random_engine generator;

using std::deque;
using NDeque::Deque;
#ifdef MAXIMIZE_SIZE
    const int COEFFICIENT = 2;
#else
    const int COEFFICIENT = 0;
#endif

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
    double operationsCount = 0;
    FILE *timeLog = fopen("timeLog.txt", "a");
    
    clock_t start = clock();
    
    size_t maxSize = 0;
    
    std::uniform_int_distribution<int> queryGetter(0, NUMBER_OF_QUERY_TYPES - 1 + COEFFICIENT);
    generator.seed(1);
    Deque<int> my;
    deque<int> stls;
    ASSERT_EQ(my.size(), stls.size());
    ASSERT_EQ(my.empty(), stls.empty());
    
    size_t queries = GetParam();
    for (size_t i = 0; i < queries; ++i)
    {
        int indexOfQuery = queryGetter(generator);
        if (indexOfQuery >= NUMBER_OF_QUERY_TYPES)
        {
            indexOfQuery = (indexOfQuery - NUMBER_OF_QUERY_TYPES > COEFFICIENT / 2 ? PUSH_BACK : PUSH_FRONT);
        }
        EQueries query = static_cast<EQueries> (indexOfQuery);
        
        int value;
        std::pair<size_t, size_t> leftRight;
        switch (query)  
        {
            case PUSH_BACK:
                value = generator();
                my.pushBack(value);
                stls.push_back(value);
                ASSERT_EQ(my.back(), value);
                operationsCount += 1;
            break; case PUSH_FRONT:
                value = generator();
                my.pushFront(value);
                stls.push_front(value);
                ASSERT_EQ(my.front(), value);
                operationsCount += 1;
            break; case POP_BACK:
                if (my.size())
                {
                    my.popBack();
                    stls.pop_back();    
                }
                operationsCount += 1;
            break; case POP_FRONT:
                if (my.size())
                {
                    my.popFront();
                    stls.pop_front();
                }
                ++operationsCount;
            break; case SORT:
                if (my.size())
                {
                    leftRight = chooseSubsegment(my.size());
                    std::sort(my.begin() + leftRight.first, my.begin() + leftRight.second + 1);
                    std::sort(stls.begin() + leftRight.first, stls.begin() + leftRight.second + 1);
                }
                operationsCount += (leftRight.second - leftRight.first + 1) * log(leftRight.second - leftRight.first + 1);
            break; case REVERSE_SORT:
                if (my.size())
                {
                    leftRight = chooseSubsegment(my.size());
                    std::sort(my.rbegin() + leftRight.first, my.rbegin() + leftRight.second + 1);
                    std::sort(stls.rbegin() + leftRight.first, stls.rbegin() + leftRight.second + 1);
                    operationsCount += (leftRight.second - leftRight.first + 1) * log(leftRight.second - leftRight.first + 1);
                }
                operationsCount += (leftRight.second - leftRight.first + 1);
            break; case RANDOM_SHUFFLE:
                if (my.size())
                {
                    leftRight = chooseSubsegment(my.size());
                    std::shuffle(my.begin() + leftRight.first, my.begin() + leftRight.second + 1, std::default_random_engine(generator));
                    std::shuffle(stls.begin() + leftRight.first, stls.begin() + leftRight.second + 1, generator);
                    operationsCount += (leftRight.second - leftRight.first + 1);
                }
            break; case REVERSE:
                if (my.size())
                {
                    leftRight = chooseSubsegment(my.size());
                    std::reverse(my.begin() + leftRight.first, my.begin() + leftRight.second + 1);
                    std::reverse(stls.begin() + leftRight.first, stls.begin() + leftRight.second + 1);
                    operationsCount += (leftRight.second - leftRight.first + 1);
                }
            break; case EQUALITY_CHECK:
                if (my.size())
                {
                    leftRight = chooseSubsegment(my.size());
                    ASSERT_EQ(static_cast<int>(std::equal(my.crbegin() + leftRight.first, my.crbegin() + leftRight.second + 1, stls.crbegin() + leftRight.first)) + i - 1, i);
                    for (size_t j = 0; j < my.size(); ++j)
                    {
                        ASSERT_EQ(my[j], stls[j]);
                    }
                    operationsCount += (leftRight.second - leftRight.first + 1);
                }
            break;
        }
        ASSERT_EQ(my.size(), stls.size());
        ASSERT_EQ(my.empty(), stls.empty());
        maxSize = std::max(maxSize, my.size());
    }
    ASSERT_TRUE(std::equal(my.crbegin(), my.crend(), stls.crbegin()));    
    for (size_t j = 0; j < my.size(); ++j)
    {
        ASSERT_EQ(my[j], stls[j]);
    }
    operationsCount += (my.size());
    fprintf(timeLog, "%lf, %lf\n", operationsCount, static_cast<double>(clock() - start) / CLOCKS_PER_SEC);
    fprintf(stderr, "%zu\n", maxSize);
    fclose(timeLog);
}

#ifndef MAXIMIZE_SIZE

INSTANTIATE_TEST_CASE_P(InstantiationName, DequeFunctionalityTest, ::testing::Values(
                                                                          1, 2, 3, 4, 5, 6, 7, 9,
                                                                          10, 20, 30, 40, 60, 70, 80, 90,
                                                                          100, 200, 300, 400, 500, 700, 800, 900,
                                                                          1000, 2000, 3000, 5000, 6000, 7000, 8000, 9000,
                                                                          10000, 20000, 30000, 40000, 50000, 60000, 70000, 80000, 90000,
                                                                          100000, 200000, 300000, 400000, 500000, 600000, 700000, 800000, 900000
    )
);


#else
INSTANTIATE_TEST_CASE_P(InstantiationName, DequeFunctionalityTest, ::testing::Values(
                                                                          1, 2, 3, 4, 5, 6, 7, 9,
                                                                          10, 20, 30, 40, 60, 70, 80, 90,
                                                                          100, 200, 300, 400, 500, 700, 800, 900,
                                                                          1000, 2000, 3000, 5000, 6000, 7000, 8000, 9000,
                                                                          10000,
                                                                          100000,
                                                                          1000000
    )
);
#endif



int main(int argc, char *argv[])
{
    FILE *timeLog = fopen("timeLog.txt", "w");
    fclose(timeLog);
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}