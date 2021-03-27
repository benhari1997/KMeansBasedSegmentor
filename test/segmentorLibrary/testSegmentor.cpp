#include <iostream>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <vector>
#include <cmath>
#include "dataLibrary/element.h"
#include "segmentorLibrary/cluster.h"
#include "segmentorLibrary/segmentor.h"
#include "segmentorLibrary/segmentorElement.h"

// 1- Testing the classe's constructors
TEST(SEGMENTOR_CLASS, Constructor_1)
{
    SegmentorElement<int> a(3, 5, 7, 0.88);
    EXPECT_EQ(a.getCycles(), 3);
    EXPECT_EQ(a.getK(), 5);
    EXPECT_EQ(a.getMax_it(), 7);
    EXPECT_NEAR(a.getEpsilon(), 0.88, 0.05);
    EXPECT_EQ(a.getClusters().size(), 5);
}

// 1- Testing the class' default constructors

TEST(SEGMENTOR_CLASS, Constructor_2)
{
    SegmentorElement<int> a;
    EXPECT_EQ(a.getCycles(), 1);
    EXPECT_EQ(a.getK(), 1);
    EXPECT_EQ(a.getMax_it(), 50);
    EXPECT_NEAR(a.getEpsilon(), 1E-2, 0.001);
    EXPECT_EQ(a.getClusters().size(), 1);
}

//void setEpsilon(const DataContainer<T> &data, double percentage);

TEST(SEGMENTOR_CLASS, setEpsilon)
{
    SegmentorElement<int> a;
    std::vector<Element<int>> b;
    std::vector<int> coords(2);
    for (int i = 0; i < 2; i++)
    {
        coords = {i};
        b.emplace_back(coords);
    }
    DataContainer<int> c(b);
    a.setK(2);
    a.setEpsilon(&c, 0.01);
    EXPECT_NEAR(a.getEpsilon(), 0.01, 0.001);
}

TEST(SEGMENTOR_CLASS, fitkmeans)
{
    SegmentorElement<int> a(1, 2, 2, 0.05);
    std::vector<Element<int>> b;
    std::vector<int> coords(2);
    for (int i = 1; i < 12; i += 10)
    {
        coords = {i};
        b.emplace_back(coords);
    }
    DataContainer<int> c(b);
    double epp = a.getEpsilon();

    a.fitKmeans(&c);
    EXPECT_EQ((a.getClusters().at(0)).getNbElements(), 1);
    EXPECT_THAT((a.getClusters().at(0)).isClusterUpToDate(), testing::Eq(true));
}

TEST(SEGMENTOR_CLASS, fitkmeans2)
{
    SegmentorElement<int> a(1, 2, 10000, 0.005);
    std::vector<Element<int>> b;
    std::vector<int> coords(2);
    for (int i = 0; i < 19; i += 2)
    {
        coords = {i};
        b.emplace_back(coords);
    }
    DataContainer<int> c(b);
    a.fitKmeans(&c);
    EXPECT_EQ((a.getClusters().at(0)).getNbElements(), 5);
    EXPECT_THAT((a.getClusters().at(0)).isClusterUpToDate(), testing::Eq(true));
    EXPECT_THAT((a.getClusters().at(1)).isClusterUpToDate(), testing::Eq(true));
}

TEST(SEGMENTOR_CLASS, fitkmeans3)
{
    SegmentorElement<int> a(1, 2, 10000, 0.005);
    std::vector<Element<int>> b;
    std::vector<int> coords(2);
    for (int i = 0; i < 19; i += 2)
    {
        coords = {i, i};
        b.emplace_back(coords);
    }
    DataContainer<int> c(b);
    a.fitKmeans(&c);
    EXPECT_EQ((a.getClusters().at(0)).getNbElements(), 5);
    EXPECT_THAT((a.getClusters().at(0)).isClusterUpToDate(), testing::Eq(true));
    EXPECT_THAT((a.getClusters().at(1)).isClusterUpToDate(), testing::Eq(true));
}

TEST(SEGMENTOR_CLASS, fitkmeans4)
{
    SegmentorElement<int> a(1, 4, 10000000, 0.0005);
    std::vector<Element<int>> b;
    std::vector<int> coords(3);
    for (int i = 0; i < 159; i += 2)
    {
        coords = {i, i, i};
        b.emplace_back(coords);
    }
    DataContainer<int> c(b);
    a.fitKmeans(&c);
    EXPECT_NEAR((a.getClusters().at(0)).getNbElements(), 20, 1);
    EXPECT_NEAR((a.getClusters().at(1)).getNbElements(), 20, 1);
    EXPECT_NEAR((a.getClusters().at(2)).getNbElements(), 20, 1);
    EXPECT_NEAR((a.getClusters().at(3)).getNbElements(), 20, 1);
    EXPECT_THAT((a.getClusters().at(0)).isClusterUpToDate(), testing::Eq(true));
    EXPECT_THAT((a.getClusters().at(1)).isClusterUpToDate(), testing::Eq(true));
}

TEST(SEGMENTOR_CLASS, fitkmeans5)
{
    SegmentorElement<int> a(1, 1, 1000, 0.05);
    std::vector<Element<int>> b;
    std::vector<int> coords(2);
    std::vector<int> coords1(1);
    coords1 = {5};
    for (int i = 0; i < 12; i += 10)
    {
        coords = {i};
        b.emplace_back(coords);
    }
    DataContainer<int> c(b);
    a.fitKmeans(&c);
    EXPECT_EQ((a.getClusters().at(0)).getNbElements(), 2);
    EXPECT_THAT((a.getClusters().at(0)).isClusterUpToDate(), testing::Eq(true));
    Element<int> expected(coords1);
    EXPECT_EQ(a.getOptimalCentroids().at(0), expected);
}

TEST(SEGMENTOR_CLASS, fitkmeans6)
{
    SegmentorElement<int> a(1, 1, 1000, 0.05);
    std::vector<Element<int>> b;
    std::vector<int> coords(2);
    std::vector<int> coords1(2);
    coords1 = {5, 5};
    for (int i = 0; i < 12; i += 10)
    {
        coords = {i, i};
        b.emplace_back(coords);
    }
    DataContainer<int> c(b);
    a.fitKmeans(&c);
    EXPECT_EQ((a.getClusters().at(0)).getNbElements(), 2);
    EXPECT_THAT((a.getClusters().at(0)).isClusterUpToDate(), testing::Eq(true));
    Element<int> expected(coords1);
    EXPECT_EQ(a.getOptimalCentroids().at(0), expected);
}

TEST(SEGMENTOR_CLASS, fitkmeans7)
{
    SegmentorElement<int> a(1, 1, 1000, 0.05);
    std::vector<Element<int>> b;
    std::vector<int> coords(3);
    std::vector<int> coords1(3);
    coords1 = {5, 5, 5};
    for (int i = 0; i < 12; i += 10)
    {
        coords = {i, i, i};
        b.emplace_back(coords);
    }
    DataContainer<int> c(b);
    a.fitKmeans(&c);
    EXPECT_EQ((a.getClusters().at(0)).getNbElements(), 2);
    EXPECT_THAT((a.getClusters().at(0)).isClusterUpToDate(), testing::Eq(true));
    Element<int> expected(coords1);
    EXPECT_EQ(a.getOptimalCentroids().at(0), expected);
}

TEST(SEGMENTOR_CLASS, epsilon_too_high)
{
    SegmentorElement<int> a(1, 4, 10000000, 10000);
    std::vector<Element<int>> b;
    std::vector<int> coords(3);
    for (int i = 0; i < 159; i += 2)
    {
        coords = {i, i, i};
        b.emplace_back(coords);
    }
    DataContainer<int> c(b);
    a.fitKmeans(&c);
    a.generateSegmentation();
    EXPECT_EQ(a.getNum_iter(), 1);
}

TEST(SEGMENTOR_CLASS, full_kmeans_1D)
{
    std::vector<Element<double>> myData(100);

    int i = 0;
    std::vector<double> coords(1);
    for (Element<double> &val : myData)
    {
        coords[0] = (i % 4) * 10;
        val = Element<double>(coords);
        i += 1;
    }

    std::vector<Element<double>> expectedCentroids(4);
    std::vector<double> coord(1);
    int k = 0;
    for (Element<double> &elem : expectedCentroids)
    {
        coord = {static_cast<double>(k) * 10};
        elem = Element<double>(coord);
        k += 1;
    }

    DataContainer<double> dataCont(myData);

    SegmentorElement<double> segElem(1, 4, 100, 0.0001);

    segElem.fitKmeans(&dataCont);
    std::vector<Element<double>> optimalCentroids = segElem.getOptimalCentroids();

    bool areAtLeastEq = false;
    for (Element<double> &elm : optimalCentroids)
    {
        for (Element<double> &elem : expectedCentroids)
        {
            areAtLeastEq = areAtLeastEq || (elem.getCoord().at(0) == elm.getCoord().at(0));
        }
        EXPECT_THAT(areAtLeastEq, ::testing::Eq(true));
        areAtLeastEq = false;
    }

    segElem.generateSegmentation();
}