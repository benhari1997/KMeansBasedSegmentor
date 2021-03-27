#include "dataLibrary/dataContainer.h"
#include "dataLibrary/element.h"
#include "generatorLibrary/dataGenerator.h"
#include "generatorLibrary/mersTwistNG.h"
#include "generatorLibrary/prng.h"
#include <fstream>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <iostream>
#include <string>

//-----------DataGenerator CLASS TEST SUIT
/// 1-CLASS CONSTRUCTOR
TEST(DataGenerator_CLASS, Class_PseudoConstructor)
{
    int len = rand() % 8 + 1;
    long seed = rand() + 1;
    PrNG *prng = new PrNG(len, seed);

    {
        EXPECT_THAT(&(DataGenerator<int>::getInstance(*prng)), ::testing::NotNull());
    }

    {
        EXPECT_THAT(&(DataGenerator<double>::getInstance(*prng)), ::testing::NotNull());
    }

    delete prng;
}

/// 2-CLASS FORBIDDEN CONSTRUCTORS
TEST(DataGenerator_CLASS, Forbidden_Duplication)
{
    int len = rand() % 8 + 1;
    long seed = rand() + 1;
    bool tester = false;
    PrNG *prng = new PrNG(len, seed);

    DataGenerator<int> &generator = DataGenerator<int>::getInstance(*prng);
    DataGenerator<int> &fakeGenerator = DataGenerator<int>::getInstance(*prng);

    EXPECT_THAT(&fakeGenerator, ::testing::Eq(&generator));

    delete prng;
}
