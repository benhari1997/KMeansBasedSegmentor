#include <iostream>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <vector>
#include <cmath>
#include "dataLibrary/pixel.h"
#include "dataLibrary/imageContainer.h"
#include "segmentorLibrary/cluster.h"
#include "segmentorLibrary/clusterPixel.h"
#include "segmentorLibrary/segmentor.h"
#include "segmentorLibrary/segmentorPixel.h"

// 1- Testing the classe's constructors
TEST(SEGMENTOR_PXL_CLASS, Constructor_1)
{
    SegmentorPixel<int> a(3, 5, 7, 0.88);
    EXPECT_EQ(a.getCycles(), 3);
    EXPECT_EQ(a.getK(), 5);
    EXPECT_EQ(a.getMax_it(), 7);
    EXPECT_NEAR(a.getEpsilon(), 0.88, 0.05);
    EXPECT_EQ(a.getClusters().size(), 5);
}

// 1- Testing the class' default constructors

TEST(SEGMENTOR_PXL_CLASS, Constructor_2)
{
    SegmentorPixel<int> a;
    EXPECT_EQ(a.getCycles(), 1);
    EXPECT_EQ(a.getK(), 1);
    EXPECT_EQ(a.getMax_it(), 50);
    EXPECT_NEAR(a.getEpsilon(), 1E-2, 0.001);
    EXPECT_EQ(a.getClusters().size(), 1);
}

//Testing fitKmeans with default weights (0,1)
TEST(SEGMENTOR_PXL_CLASS, fit_kmeans_default)
{
    std::vector<Pixel<int>> imgData;

    imgData.reserve(10 * 10);

    std::vector<int> coords(2);

    std::vector<int> col(1);

    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            coords = {i, j};
            if (i < j)
            {
                col = {255};
            }
            else
            {
                if (i == j)
                {
                    col = {100};
                }
                else
                {
                    col = {0};
                }
            }
            imgData.emplace_back(coords, col);
        }
    }

    ImageContainer<int> image(10, 10, imgData);

    SegmentorPixel<int> imgSegmentor(1, 3, 1000, 0.0001);

    imgSegmentor.fitKmeans(&image);

    std::vector<Pixel<int>> optimalCentroids = imgSegmentor.getOptimalCentroids();

    std::vector<Pixel<int>> expectedCentroid(3);
    std::vector<int> coordsZero(2, 0);
    expectedCentroid.push_back(Pixel<int>(coordsZero, {0}));
    expectedCentroid.push_back(Pixel<int>(coordsZero, {100}));
    expectedCentroid.push_back(Pixel<int>(coordsZero, {255}));

    bool areAtLeastEq = false;
    for (Pixel<int> &pxl : optimalCentroids)
    {
        for (Pixel<int> &pixel : expectedCentroid)
        {
            areAtLeastEq = areAtLeastEq || (pxl.getColors().at(0) == pixel.getColors().at(0));
        }
        EXPECT_THAT(areAtLeastEq, ::testing::Eq(true));
        areAtLeastEq = false;
    }
}

//Testing fitKmeans with default weights (0,1)
TEST(SEGMENTOR_PXL_CLASS, fit_kmeans_mixed)
{
    std::vector<Pixel<int>> imgData;

    std::vector<double> weights{0.5, 0.5};

    imgData.reserve(10 * 10);

    std::vector<int> coords(2);

    std::vector<int> col(1);

    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            coords = {i, j};
            if (i < 5 && j < 5)
            {
                col = {0};
            }
            if (i < 5 && j >= 5)
            {
                col = {80};
            }
            if (i >= 5 && j < 5)
            {
                col = {160};
            }
            if (i >= 5 && j <= 5)
            {
                col = {240};
            }
            imgData.emplace_back(coords, col, weights);
        }
    }

    ImageContainer<int> image(10, 10, imgData);

    SegmentorPixel<int> imgSegmentor(1, 4, 1000, 0.0001);

    imgSegmentor.fitKmeans(&image);

    std::vector<Pixel<int>> optimalCentroids = imgSegmentor.getOptimalCentroids();

    std::vector<Pixel<int>> expectedCentroid;
    expectedCentroid.reserve(4);
    std::vector<int> coordsZero(2, 0);

    expectedCentroid.push_back(Pixel<int>(coordsZero, {0}, weights));
    expectedCentroid.push_back(Pixel<int>(coordsZero, {80}, weights));
    expectedCentroid.push_back(Pixel<int>(coordsZero, {160}, weights));
    expectedCentroid.push_back(Pixel<int>(coordsZero, {240}, weights));

    bool areAtLeastEq = false;
    for (Pixel<int> &pxl : optimalCentroids)
    {
        for (Pixel<int> &pixel : expectedCentroid)
        {
            areAtLeastEq = areAtLeastEq || (pxl.getColors().at(0) == pixel.getColors().at(0));
        }
        EXPECT_THAT(areAtLeastEq, ::testing::Eq(true));
        areAtLeastEq = false;
    }
}
