#include "generatorLibrary/prng.h"
#include "generatorLibrary/mersTwistNG.h"
#include "generatorLibrary/mswSeqNG.h"
#include "generatorLibrary/xorshiftNG.h"
#include "generatorLibrary/linCongNG.h"
#include <cstdlib>
#include <iostream>
#include <vector>
#include <math.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

//-----------PRNG CLASS TEST SUIT
/// 1-CLASS CONSTRUCTOR
TEST(PrNG_CLASS, Class_Constructor)
{
	int len = rand() % 8 + 1;
	long seed = rand() + 1;
	PrNG *prngEl = new PrNG(len, seed);

	EXPECT_EQ(prngEl->getLen(), len);
	EXPECT_EQ(prngEl->getSeed(), seed);

	delete prngEl;
}

/// 2-CLASS DESTRUCTOR
TEST(PrNG_CLASS, Class_Destructor)
{
	PrNG prngEl{1, 11111};
	{
		PrNG prngEl{1, 22222};
	}

	EXPECT_THAT(prngEl.rand(), ::testing::Ne(22222));
}

/// 3-SETTERS & GETTERS
TEST(PrNG_CLASS, Get_Set)
{
	int len = rand() % 8 + 1;
	long seed = rand() + 1;
	PrNG *prngEl = new PrNG(1, 11111);

	prngEl->setLen(len);
	prngEl->setSeed(seed);
	EXPECT_THAT(prngEl->getLen(), ::testing::TypedEq<int>(len));
	EXPECT_THAT(prngEl->getSeed(), ::testing::TypedEq<long>(seed));

	delete prngEl;
}

/// 4-METHODS
TEST(PrNG_CLASS, Iterate_Method)
{
	int len = rand() % 8 + 1;
	long seed = rand() + 1;
	PrNG *prngEl = new PrNG(len, seed);

	EXPECT_THAT(prngEl->iterate(), ::testing::TypedEq<long>(seed));

	delete prngEl;
}

TEST(PrNG_CLASS, Rand_Method)
{
	int len = rand() % 8 + 1;
	long seed = rand() + 1;
	double randVal;
	PrNG *prngEl = new PrNG(len, seed);

	{
		long intpart = prngEl->iterate() % (long)(pow(10, len) - 1);
		double realpart = intpart / (pow(10, len) - 1);
		randVal = intpart + realpart;
	}

	EXPECT_THAT(prngEl->rand(), ::testing::TypedEq<double>(randVal));
	EXPECT_THAT(prngEl->rand(), ::testing::Le(pow(10, len) - 1));

	delete prngEl;
}

//-----------MersTwistNG CLASS TEST SUIT
/// 1-CLASS CONSTRUCTOR
TEST(MersTwistNG_CLASS, Class_Constructor)
{
	int len = rand() % 8 + 1;
	long seed = rand() + 1;
	MersTwistNG *mTwistEl = new MersTwistNG(len, seed);

	EXPECT_EQ(mTwistEl->getLen(), len);
	EXPECT_EQ(mTwistEl->getSeed(), seed);

	delete mTwistEl;
}

/// 2-METHODS
TEST(MersTwistNG_CLASS, Twist_Method)
{
	int len = rand() % 8 + 1;
	long seed = rand() + 1;
	std::vector<double> twistMat1, twistMat2;
	MersTwistNG *mTwistEl = new MersTwistNG(len, seed);

	for (int i = 0; i < 624; i++)
	{
		twistMat1.push_back(mTwistEl->rand());
	};
	for (int i = 0; i < 624; i++)
	{
		twistMat2.push_back(mTwistEl->rand());
	};

	EXPECT_THAT(twistMat1, ::testing::Ne(twistMat2));

	delete mTwistEl;
}

TEST(MersTwistNG_CLASS, rand_Method)
{
	int len = rand() % 8 + 1;
	long seed = rand() + 1;
	double rand;
	std::vector<double> randVector;
	MersTwistNG *mTwistEl = new MersTwistNG(len, seed);

	for (int i = 1; i < 200; i++)
	{
		rand = mTwistEl->rand();
		EXPECT_THAT(rand, ::testing::Le(pow(10, len) - 1));

		randVector.push_back(rand);
	}

	for (int i = 5; i < 100; i++)
	{

		EXPECT_THAT(std::vector<int>(randVector.begin(), randVector.begin() + i),
					::testing::Ne(std::vector<int>(randVector.begin() + i, randVector.begin() + 2 * i)));
	}

	delete mTwistEl;
}

//-----------MswSeqNG CLASS TEST SUIT
/// 1-CLASS CONSTRUCTOR
TEST(MswSeqNG_CLASS, Class_Constructor)
{
	int len = rand() % 8 + 1;
	long seed = rand() + 1;
	MswSecNG *mswSeqEl = new MswSecNG(len, seed);

	EXPECT_EQ(mswSeqEl->getLen(), len);
	EXPECT_EQ(mswSeqEl->getSeed(), seed);

	delete mswSeqEl;
}

/// 2-METHODS
TEST(MswSeqNG_CLASS, rand_Method)
{
	int len = rand() % 8 + 1;
	long seed = rand() + 1;
	double rand;
	std::vector<double> randVector;
	MswSecNG *mswSeqEl = new MswSecNG(len, seed);

	for (int i = 1; i < 200; i++)
	{
		rand = mswSeqEl->rand();
		EXPECT_THAT(rand, ::testing::Le(pow(10, len) - 1));

		randVector.push_back(rand);
	}

	for (int i = 5; i < 100; i++)
	{

		EXPECT_THAT(std::vector<int>(randVector.begin(), randVector.begin() + i),
					::testing::Ne(std::vector<int>(randVector.begin() + i, randVector.begin() + 2 * i)));
	}

	delete mswSeqEl;
}

//-----------LinCongNG CLASS TEST SUIT
/// 1-CLASS CONSTRUCTOR
TEST(LinCongNG_CLASS, Class_Constructor)
{
	int len = rand() % 8 + 1;
	long seed = rand() + 1;
	LinCongNG *linCongEl = new LinCongNG(len, seed);

	EXPECT_EQ(linCongEl->getLen(), len);
	EXPECT_EQ(linCongEl->getSeed(), seed);

	delete linCongEl;
}

/// 2-METHODS
TEST(LinCongNG_CLASS, rand_Method)
{
	int len = rand() % 8 + 1;
	long seed = rand() + 1;
	double rand;
	std::vector<double> randVector;
	LinCongNG *linCongEl = new LinCongNG(len, seed);

	for (int i = 1; i < 200; i++)
	{
		rand = linCongEl->rand();
		EXPECT_THAT(rand, ::testing::Le(pow(10, len) - 1));

		randVector.push_back(rand);
	}

	for (int i = 5; i < 100; i++)
	{

		EXPECT_THAT(std::vector<int>(randVector.begin(), randVector.begin() + i),
					::testing::Ne(std::vector<int>(randVector.begin() + i, randVector.begin() + 2 * i)));
	}

	delete linCongEl;
}

// ---------- - XorshiftNG CLASS TEST SUIT
/// 1-CLASS CONSTRUCTOR
TEST(XorshiftNG_CLASS, Class_Constructor)
{
	int len = rand() % 8 + 1;
	long seed = rand() + 1;
	XorshiftNG *xorshiftEl = new XorshiftNG(len, seed);

	EXPECT_EQ(xorshiftEl->getLen(), len);
	EXPECT_EQ(xorshiftEl->getSeed(), seed);

	delete xorshiftEl;
}

/// 2-METHODS
TEST(XorshiftNG_CLASS, rand_Method)
{
	int len = rand() % 8 + 1;
	long seed = rand() + 1;
	double rand;
	std::vector<double> randVector;
	XorshiftNG *xorshiftEl = new XorshiftNG(len, seed);

	for (int i = 1; i < 200; i++)
	{
		rand = xorshiftEl->rand();
		EXPECT_THAT(rand, ::testing::Le(pow(10, len) - 1));

		randVector.push_back(rand);
	}

	for (int i = 5; i < 100; i++)
	{

		EXPECT_THAT(std::vector<int>(randVector.begin(), randVector.begin() + i),
					::testing::Ne(std::vector<int>(randVector.begin() + i, randVector.begin() + 2 * i)));
	}

	delete xorshiftEl;
}
