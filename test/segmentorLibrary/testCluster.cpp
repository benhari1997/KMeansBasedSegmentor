#include <iostream>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <vector>
#include <cmath>
#include "dataLibrary/element.h"
#include "dataLibrary/pixel.h"
#include "segmentorLibrary/cluster.h"
#include "segmentorLibrary/clusterElement.h"
#include "segmentorLibrary/clusterPixel.h"

// 1- Testing the classe's constructors
TEST(CLUSTER_CLASS, Constructor_1)
{
    std::vector<Element<int> *> elementsPtrs;
    std::vector<Element<int>> elements;
    elementsPtrs.reserve(10);
    elements.reserve(10);
    std::vector<int> coords(2);
    for (int i = 0; i < 10; i++)
    {
        coords = {i, i * 2};
        elementsPtrs.push_back(new Element<int>(coords)); //Constructor based on Element coordinates
        elements.emplace_back(coords);
    }

    Element<int> *centroidPtr = new Element<int>({5, 5});

    ClusterElement<int> cluster(elementsPtrs, centroidPtr);

    EXPECT_EQ(cluster.getClustId(), 0);
    EXPECT_EQ(cluster.getCentroid(), *centroidPtr);
    EXPECT_EQ(cluster.getNbElements(), elementsPtrs.size());
    ASSERT_THAT(cluster.getClustElements(), ::testing::ElementsAreArray(elements));

    for (Element<int> *elem : elementsPtrs)
    {
        delete elem;
    }
    elementsPtrs.clear();
    delete (centroidPtr);
}

TEST(CLUSTER_CLASS, Constructor_2)
{
    Element<int> *centroidPtr = new Element<int>({5, 5});

    ClusterElement<int> cluster(centroidPtr);

    EXPECT_EQ(cluster.getClustId(), 0);
    EXPECT_EQ(cluster.getCentroid(), *centroidPtr);
    EXPECT_EQ(cluster.getNbElements(), 0);
    EXPECT_EQ(cluster.getClustElements().size(), 0);
    delete (centroidPtr);
}

TEST(CLUSTER_CLASS, Constructor_3)
{
    ClusterElement<int> cluster;

    EXPECT_EQ(cluster.getClustId(), 0);
    EXPECT_EQ(cluster.getNbElements(), 0);
    EXPECT_EQ(cluster.getClustElements().size(), 0);
}

//Copy constructor

TEST(CLUSTER_CLASS, Constructor_Copy)
{

    std::vector<Element<int> *> elementsPtrs;
    std::vector<Element<int>> elements;
    elementsPtrs.reserve(10);
    elements.reserve(10);
    std::vector<int> coords(2);
    for (int i = 0; i < 10; i++)
    {
        coords = {i, i * 2};
        elementsPtrs.push_back(new Element<int>(coords)); //Constructor based on Element coordinates
        elements.emplace_back(coords);
    }

    Element<int> *centroidPtr = new Element<int>({5, 5});

    ClusterElement<int> *cluster = new ClusterElement<int>(elementsPtrs, centroidPtr);
    ClusterElement<int> clusterCopy(*cluster);

    delete cluster;

    EXPECT_EQ(clusterCopy.getClustId(), 1);
    EXPECT_EQ(clusterCopy.getCentroid(), *centroidPtr);
    EXPECT_EQ(clusterCopy.getNbElements(), elementsPtrs.size());
    ASSERT_THAT(clusterCopy.getClustElements(), ::testing::ElementsAreArray(elements));

    for (Element<int> *elem : elementsPtrs)
    {
        delete elem;
    }
    elementsPtrs.clear();
    delete (centroidPtr);
}

TEST(CLUSTER_CLASS, Constructor_Move)
{

    std::vector<Element<int> *> elementsPtrs;
    std::vector<Element<int>> elements;
    elementsPtrs.reserve(5);
    elements.reserve(5);
    std::vector<int> coords(2);
    for (int i = 0; i < 5; i++)
    {
        coords = {i * i, i * 2};
        elementsPtrs.push_back(new Element<int>(coords)); //Constructor based on Element coordinates
        elements.emplace_back(coords);
    }
    Element<int> *centroidPtr = new Element<int>({5, 5});

    std::vector<ClusterElement<int>> myClusters;
    myClusters.reserve(3);

    myClusters.push_back(ClusterElement<int>());
    myClusters.push_back(ClusterElement<int>(centroidPtr));
    myClusters.push_back(ClusterElement<int>(elementsPtrs, centroidPtr));

    EXPECT_EQ(myClusters.at(2).getClustId(), 2);
    EXPECT_EQ(myClusters.at(2).getNbElements(), elementsPtrs.size());
    ASSERT_THAT(myClusters.at(2).getClustElements(), ::testing::ElementsAreArray(elements));
    EXPECT_EQ(ClusterElement<int>::getClustersIds().size(), 3);

    for (Element<int> *elem : elementsPtrs)
    {
        delete elem;
    }
    elementsPtrs.clear();
    delete (centroidPtr);
}

// 2 - Testing the Classe's Setters & Getters

TEST(CLUSTER_CLASS, Set_Centroid)
{

    ClusterElement<int> cluster;

    Element<int> *centroidPtr = new Element<int>(2);
    Element<int> centroid = *centroidPtr;

    cluster.setCentroid(centroidPtr);

    delete centroidPtr;

    EXPECT_EQ(cluster.getCentroid(), centroid);
}

TEST(CLUSTER_CLASS, Set_Clust_Elements)
{

    ClusterElement<int> cluster;

    std::vector<Element<int> *> elementsPtrs;
    std::vector<Element<int>> elements;
    elementsPtrs.reserve(5);
    elements.reserve(5);
    std::vector<int> coords(2);
    for (int i = 0; i < 5; i++)
    {
        coords = {i * i, i * 2};
        elementsPtrs.push_back(new Element<int>(coords)); //Constructor based on Element coordinates
        elements.emplace_back(coords);
    }

    cluster.setClustElements(elementsPtrs);

    for (Element<int> *elem : elementsPtrs)
    {
        delete elem;
    }
    elementsPtrs.clear();

    ASSERT_THAT(cluster.getClustElements(), ::testing::ElementsAreArray(elements));
    EXPECT_EQ(cluster.getNbElements(), elements.size());
}

// 3 - Testing the classe's KMEANS Related methods
// 3.1 Updating the centroid's values (barycentre method) & retrieving its displacement

TEST(CLUSTER_CLASS, Update_Centroid_Stays)
{
    /**
     * The test is about constructing a cluster with 4 2D elements distributed arround
     * the centroid (so basically the centroid is the real barycentre)
     * We should expect no displacement of the centroid after call to updateCentroid.
     * We should also expect the centroid coordinates to stay the same
    */
    std::vector<Element<int> *> elementsPtrs;
    elementsPtrs.reserve(4);
    std::vector<int> coords(2);

    int translateBy = 100;

    elementsPtrs.push_back(new Element<int>({5 + translateBy, 5}));
    elementsPtrs.push_back(new Element<int>({5, 5 + translateBy}));
    elementsPtrs.push_back(new Element<int>({5 - translateBy, 5}));
    elementsPtrs.push_back(new Element<int>({5, 5 - translateBy}));

    Element<int> *centroidPtr = new Element<int>({5, 5});

    ClusterElement<int> cluster(elementsPtrs, centroidPtr);

    double displacement = cluster.updateCentroid();

    EXPECT_NEAR(displacement, 0.0, 1E-6);

    for (int i = 0; i < 2; i++)
    {
        EXPECT_NEAR(cluster.getCentroid().getCoord().at(i), (*centroidPtr).getCoord().at(i), 1E-6);
    }

    for (Element<int> *elem : elementsPtrs)
    {
        delete elem;
    }
    elementsPtrs.clear();
}

TEST(CLUSTER_CLASS, Update_Centroid_Change_1D)
{

    /**
     *This test is about initializing a cluster with a set of random 1D Elements & a centroid 
     * located at the origin of the space (0).
     * When we update the centroid, we expect it to move to the actual barycentre of the set
     * of elements, with a displacement equal to this new 1D coordinate (since it started from 0)
     * Using double for templating the class templates here is crucial since using the int
     * will cause a lot of information loss due to rounding & casting operations.
    */

    std::vector<Element<double> *> elementsPtrs;
    elementsPtrs.reserve(50);
    std::vector<double> coords(1);
    double meanPosition = 0;
    for (int i = 0; i < 50; i++)
    {
        coords[0] = rand() % 100;
        elementsPtrs.push_back(new Element<double>(coords));
        meanPosition += coords[0];
    }
    meanPosition /= 50;

    Element<double> *centroidPtr = new Element<double>(1);

    ClusterElement<double> cluster(elementsPtrs, centroidPtr);

    EXPECT_EQ(cluster.isClusterUpToDate(), false);

    double displacement = cluster.updateCentroid();

    EXPECT_EQ(cluster.isClusterUpToDate(), true);

    EXPECT_NEAR(displacement, meanPosition, 1E-6);

    EXPECT_NEAR(cluster.getCentroid().getCoord().at(0), meanPosition, 1E-6);

    for (Element<double> *elem : elementsPtrs)
    {
        delete elem;
    }
    elementsPtrs.clear();

    delete centroidPtr;
}

// 3.2 Updating the centroid's values (barycentre method) & computing its WCSS

TEST(CLUSTER_CLASS, Update_WCSS)
{
    /**
     * In this test, we initialize a cluster with 4 2D Elements and a centered centroid. 
     * The goal here is to test both the Update_wcss (within cluster sum of squares)
     * and the boolean upToDate attribute since they depend on each others 
     * One should make sure the cluster is up to date before computing WCSS otherwise 
     * it doesn't make sense since the centroid won't be the barycentre.
     * Here we expect the cluster is outdated until the centroid is updated (through updateCentroid()),
     * and to go outdated once :
     * 1 - the centroid is set using the setter
     * 2 - the clustElements are set using the setter
     * 
    */

    std::vector<Element<int> *> elementsPtrs;
    elementsPtrs.reserve(5);
    std::vector<int> coords(2);

    int translateBy = 1;

    elementsPtrs.push_back(new Element<int>({5 + translateBy, 5}));
    elementsPtrs.push_back(new Element<int>({5, 5 + translateBy}));
    elementsPtrs.push_back(new Element<int>({5 - translateBy, 5}));
    elementsPtrs.push_back(new Element<int>({5, 5 - translateBy}));

    Element<int> *centroidPtr = new Element<int>({5, 5});

    ClusterElement<int> cluster(elementsPtrs, centroidPtr);

    //Newly constructed : outdated
    EXPECT_EQ(cluster.isClusterUpToDate(), false);

    double displacement = cluster.updateCentroid();

    //Newly centroid updated : up to date
    EXPECT_EQ(cluster.isClusterUpToDate(), true);

    //Thus we can compute the WCSS
    cluster.updateWCSS();

    EXPECT_NEAR(cluster.getWCSS(), 4.0, 1E-6);

    //We change the clustElements
    elementsPtrs.push_back(new Element<int>({5, 5 - translateBy}));

    cluster.setClustElements(elementsPtrs);

    //clustElements changed : cluster outdated
    EXPECT_EQ(cluster.isClusterUpToDate(), false);
    EXPECT_EQ(cluster.getNbElements(), 5);

    //An Outdated cluster is not allowed to compute the WCSS : returns -1
    cluster.updateWCSS();

    EXPECT_NEAR(cluster.getWCSS(), -1, 1E-6);

    for (Element<int> *elem : elementsPtrs)
    {
        delete elem;
    }
    elementsPtrs.clear();
}

// //////////////////////////////

TEST(CLUSTER_CLASS_PIXEL, Constructor_1)
{
    std::vector<Pixel<int> *> elementsPtrs;
    std::vector<Pixel<int>> elements;
    elementsPtrs.reserve(10);
    elements.reserve(10);
    std::vector<int> coords(2);
    std::vector<int> cols(3);
    for (int i = 0; i < 10; i++)
    {
        coords = {i, i * 2};
        cols = {i % 255, (i * 2) % 255, 0};
        elementsPtrs.push_back(new Pixel<int>(coords, cols)); //Constructor based on Element coordinates
        elements.emplace_back(coords, cols);
    }

    Pixel<int> *centroidPtr = new Pixel<int>(coords, cols);

    ClusterPixel<int> cluster(elementsPtrs, centroidPtr);

    EXPECT_EQ(cluster.getClustId(), 0);
    EXPECT_EQ(cluster.getCentroid(), *centroidPtr);
    EXPECT_EQ(cluster.getNbElements(), elementsPtrs.size());
    ASSERT_THAT(cluster.getClustElements(), ::testing::ElementsAreArray(elements));

    for (Pixel<int> *elem : elementsPtrs)
    {
        delete elem;
    }
    elementsPtrs.clear();
    delete (centroidPtr);
}

TEST(CLUSTER_CLASS_PIXEL, Constructor_2)
{
    Pixel<int> *centroidPtr = new Pixel<int>({5, 5}, {255, 0, 200});

    ClusterPixel<int> cluster(centroidPtr);

    EXPECT_EQ(cluster.getClustId(), 0);
    EXPECT_EQ(cluster.getCentroid(), *centroidPtr);
    EXPECT_EQ(cluster.getNbElements(), 0);
    EXPECT_EQ(cluster.getClustElements().size(), 0);
    delete (centroidPtr);
}

//Copy constructor

TEST(CLUSTER_CLASS_PIXEL, Constructor_Copy)
{

    std::vector<Pixel<int> *> elementsPtrs;
    std::vector<Pixel<int>> elements;
    elementsPtrs.reserve(10);
    elements.reserve(10);
    std::vector<int> coords(2);
    std::vector<int> cols(3);
    for (int i = 0; i < 10; i++)
    {
        coords = {i, i * 2};
        cols = {i % 255, (i * 2) % 255, 0};
        elementsPtrs.push_back(new Pixel<int>(coords, cols)); //Constructor based on Element coordinates
        elements.emplace_back(coords, cols);
    }

    Pixel<int> *centroidPtr = new Pixel<int>({5, 5}, {255, 255, 255});

    ClusterPixel<int> *cluster = new ClusterPixel<int>(elementsPtrs, centroidPtr);
    ClusterPixel<int> clusterCopy(*cluster);

    delete cluster;

    EXPECT_EQ(clusterCopy.getClustId(), 1);
    EXPECT_EQ(clusterCopy.getCentroid(), *centroidPtr);
    EXPECT_EQ(clusterCopy.getNbElements(), elementsPtrs.size());
    ASSERT_THAT(clusterCopy.getClustElements(), ::testing::ElementsAreArray(elements));

    for (Pixel<int> *elem : elementsPtrs)
    {
        delete elem;
    }
    elementsPtrs.clear();
    delete (centroidPtr);
}

TEST(CLUSTER_CLASS_PIXEL, Constructor_Move)
{
    std::vector<Pixel<int> *> elementsPtrs;
    std::vector<Pixel<int>> elements;
    elementsPtrs.reserve(5);
    elements.reserve(5);
    std::vector<int> coords(2);
    std::vector<int> cols(3);

    for (int i = 0; i < 5; i++)
    {
        coords = {i * i, i * 2};
        cols = {i % 255, 255, i % 200};
        elementsPtrs.push_back(new Pixel<int>(coords, cols)); //Constructor based on Element coordinates
        elements.emplace_back(coords, cols);
    }
    Pixel<int> *centroidPtr = new Pixel<int>({5, 5}, {255, 255, 0});

    std::vector<ClusterPixel<int>> myClusters;
    myClusters.reserve(3);

    myClusters.push_back(ClusterPixel<int>());
    myClusters.push_back(ClusterPixel<int>(centroidPtr));
    myClusters.push_back(ClusterPixel<int>(elementsPtrs, centroidPtr));

    EXPECT_EQ(myClusters.at(2).getClustId(), 2);
    EXPECT_EQ(myClusters.at(1).getCentroid(), *centroidPtr);
    EXPECT_EQ(myClusters.at(2).getNbElements(), elementsPtrs.size());
    ASSERT_THAT(myClusters.at(2).getClustElements(), ::testing::ElementsAreArray(elements));
    EXPECT_EQ(ClusterPixel<int>::getClustersIds().size(), 3);

    for (Pixel<int> *elem : elementsPtrs)
    {
        delete elem;
    }
    elementsPtrs.clear();
    delete (centroidPtr);
}

// 2 - Testing the Classe's Setters & Getters

TEST(CLUSTER_CLASS_PIXEL, Set_Centroid)
{

    ClusterPixel<int> cluster;

    Pixel<int> *centroidPtr = new Pixel<int>(3);
    Pixel<int> centroid(3);

    cluster.setCentroid(centroidPtr);

    delete centroidPtr;

    EXPECT_EQ(cluster.getCentroid(), centroid);
}

TEST(CLUSTER_CLASS_PIXEL, Set_Clust_Elements)
{
    ClusterPixel<int> cluster;

    std::vector<Pixel<int> *> elementsPtrs;
    std::vector<Pixel<int>> elements;
    elementsPtrs.reserve(5);
    elements.reserve(5);
    std::vector<int> coords(2);
    std::vector<int> cols(1); //Grayscale example
    for (int i = 0; i < 5; i++)
    {
        coords = {i * i, i * 2};
        cols = {i % 255};
        elementsPtrs.push_back(new Pixel<int>(coords, cols)); //Constructor based on Element coordinates
        elements.emplace_back(coords, cols);
    }

    cluster.setClustElements(elementsPtrs);

    for (Pixel<int> *elem : elementsPtrs)
    {
        delete elem;
    }
    elementsPtrs.clear();

    ASSERT_THAT(cluster.getClustElements(), ::testing::ElementsAreArray(elements));
    EXPECT_EQ(cluster.getNbElements(), elements.size());
}

// 3 - Testing the classe's KMEANS Related methods
// 3.1 Updating the centroid's values (barycentre method) & retrieving its displacement

//TODO : Implement real KMEANS Pixel related tests (including colors metrics, mixed metrics etc..)

TEST(CLUSTER_CLASS_PIXEL, Update_Centroid_Stays)
{
    /**
     * The test is about constructing a cluster with 4 2D elements distributed arround
     * the centroid (so basically the centroid is the real barycentre)
     * We should expect no displacement of the centroid after call to updateCentroid.
     * We should also expect the centroid coordinates to stay the same
    */
    std::vector<Pixel<int> *> elementsPtrs;
    elementsPtrs.reserve(4);
    std::vector<int> coords(2);

    int translateBy = 100;

    elementsPtrs.push_back(new Pixel<int>({5 + translateBy, 5}, {20}));
    elementsPtrs.push_back(new Pixel<int>({5, 5 + translateBy}, {15}));
    elementsPtrs.push_back(new Pixel<int>({5 - translateBy, 5}, {5}));
    elementsPtrs.push_back(new Pixel<int>({5, 5 - translateBy}, {0}));

    Pixel<int> *centroidPtr = new Pixel<int>({5, 5}, {10});

    ClusterPixel<int> cluster(elementsPtrs, centroidPtr);

    double displacement = cluster.updateCentroid();

    EXPECT_NEAR(displacement, 0.0, 1E-6);

    for (int i = 0; i < 2; i++)
    {
        EXPECT_NEAR(cluster.getCentroid().getCoord().at(i), (*centroidPtr).getCoord().at(i), 1E-6);
    }

    EXPECT_EQ(cluster.getCentroid().getColors().at(0), (*centroidPtr).getColors().at(0));

    for (Pixel<int> *elem : elementsPtrs)
    {
        delete elem;
    }
    elementsPtrs.clear();
    delete centroidPtr;
}

TEST(CLUSTER_CLASS_PIXEL, Update_Centroid_GrayScale_Changes)
{
    std::vector<Pixel<int> *> elementsPtrs;
    elementsPtrs.reserve(4);
    std::vector<int> coords(2);

    int translateBy = 100;

    elementsPtrs.push_back(new Pixel<int>({5 + translateBy, 5}, {10}));
    elementsPtrs.push_back(new Pixel<int>({5, 5 + translateBy}, {20}));
    elementsPtrs.push_back(new Pixel<int>({5 - translateBy, 5}, {30}));
    elementsPtrs.push_back(new Pixel<int>({5, 5 - translateBy}, {40}));

    Pixel<int> *centroidPtr = new Pixel<int>({5, 5}, {0});

    ClusterPixel<int> cluster(elementsPtrs, centroidPtr);

    double displacement = cluster.updateCentroid();

    EXPECT_NEAR(displacement, 25.0, 1E-6);

    for (int i = 0; i < 2; i++)
    {
        EXPECT_NEAR(cluster.getCentroid().getCoord().at(i), (*centroidPtr).getCoord().at(i), 1E-6);
    }

    EXPECT_NEAR(cluster.getCentroid().getColors().at(0), 25, 1E-4);

    cluster.updateWCSS();

    EXPECT_NEAR(cluster.getWCSS(), 50 + 100 + 150 + 200, 1E-4);

    for (Pixel<int> *elem : elementsPtrs)
    {
        delete elem;
    }
    elementsPtrs.clear();
    delete centroidPtr;
}

TEST(CLUSTER_CLASS_PIXEL, Update_Centroid_GrayScale_Changes_Mixed)
{
    std::vector<Pixel<int> *> elementsPtrs;
    elementsPtrs.reserve(4);
    std::vector<int> coords(2);

    int translateBy = 100;

    elementsPtrs.push_back(new Pixel<int>({5 + translateBy, 5}, {10}));
    elementsPtrs.push_back(new Pixel<int>({5, 5 + translateBy}, {20}));
    elementsPtrs.push_back(new Pixel<int>({5 - translateBy, 5}, {20}));
    elementsPtrs.push_back(new Pixel<int>({5, 5 - translateBy}, {10}));

    for (Pixel<int> *p : elementsPtrs)
    {
        p->setWeights({0.5, 0.5});
    }

    Pixel<int> *centroidPtr = new Pixel<int>({5, 5}, {0});

    ClusterPixel<int> cluster(elementsPtrs, centroidPtr);

    double displacement = cluster.updateCentroid();

    EXPECT_NEAR(displacement, 15.0, 1E-6);

    for (int i = 0; i < 2; i++)
    {
        EXPECT_NEAR(cluster.getCentroid().getCoord().at(i), (*centroidPtr).getCoord().at(i), 1E-6);
    }

    EXPECT_NEAR(cluster.getCentroid().getColors().at(0), 15, 1E-4);

    cluster.updateWCSS();

    double realWCSS = (50 + 2.5) * (50 + 2.5) * 4;
    EXPECT_NEAR(cluster.getWCSS(), realWCSS, 1E-4);

    for (Pixel<int> *elem : elementsPtrs)
    {
        delete elem;
    }
    elementsPtrs.clear();
    delete centroidPtr;
}