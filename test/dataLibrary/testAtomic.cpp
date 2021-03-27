#include <iostream>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <vector>
#include "dataLibrary/element.h"
#include "dataLibrary/pixel.h"
#include <math.h>

TEST(INIT_TEST, test_0)
{
    ASSERT_EQ(1, 1);
}

//-----------------------------------------------------------------------------------
//------------------------------PIXEL CLASS TEST SUIT--------------------------------
//-----------------------------------------------------------------------------------

/// 1 - Testing the classe's constructors
TEST(ELEMENT_CLASS, Constructor_1)
{
    Element<int> elem1; //??calling constr with () -> prototyped function not called (was a variable de finition intended?)
    EXPECT_EQ(elem1.getGeomDim(), 1);
    EXPECT_EQ(elem1.getCoord().size(), 1);
}

TEST(ELEMENT_CLASS, Constructor_2)
{
    Element<int> elem1(2);
    EXPECT_EQ(elem1.getGeomDim(), 2);
    EXPECT_EQ(elem1.getCoord().size(), 2);
}

TEST(ELEMENT_CLASS, Constructor_3)
{
    std::vector<int> coords = {4, 20};
    Element<int> elem1(coords);
    EXPECT_EQ(elem1.getGeomDim(), 2);
    ASSERT_THAT(elem1.getCoord(), ::testing::ElementsAreArray(coords));
}

TEST(ELEMENT_CLASS, Constructor_4)
{
    std::vector<double> coords = {22, 11};

    Element<double> *elem1 = new Element<double>(coords);
    Element<double> elem2(*elem1);

    delete elem1;

    EXPECT_EQ(elem2.getGeomDim(), 2);
    ASSERT_THAT(elem2.getCoord(), ::testing::ElementsAreArray(coords));
}

TEST(ELEMENT_CLASS, Constructor_move)
{
    std::vector<Element<int>> elementsVector;
    elementsVector.reserve(3);

    elementsVector.push_back(Element<int>());
    elementsVector.push_back(Element<int>(2));
    elementsVector.push_back(Element<int>({22, 11}));

    EXPECT_EQ(Element<int>::getNElem(), 3);
}
TEST(ELEMENT_CLASS, Constructor_copy)
{
    std::vector<double> coords = {22, 11};

    Element<double> elem1(coords);
    Element<double> elem2(elem1);
    Element<double> elem3(elem2);

    EXPECT_EQ(Element<double>::getNElem(), 3);
}

TEST(ELEMENT_CLASS, Destructor)
{
    std::vector<double> coords = {22, 11};

    Element<double> elem1(coords);
    EXPECT_EQ(Element<double>::getNElem(), 1);
    {
        Element<double> elem2;
        Element<double> elem3({19, 95});
        EXPECT_EQ(Element<double>::getNElem(), 3);
        EXPECT_EQ(Element<int>::getNElem(), 0);
    }

    Element<double> *elem4 = new Element<double>();

    EXPECT_EQ(Element<double>::getNElem(), 2);
    EXPECT_EQ(Element<int>::getNElem(), 0);

    delete (elem4);

    EXPECT_EQ(Element<double>::getNElem(), 1);
    EXPECT_EQ(Element<int>::getNElem(), 0);
}

/// 2 - Testing the classe's Getters/Setters

TEST(ELEMENT_CLASS, Get_Set_1)
{
    Element<int> elem1;
    elem1.setGeomDim(3);
    EXPECT_EQ(elem1.getGeomDim(), 3);
    std::vector<int> zeros(3);
    ASSERT_THAT(elem1.getCoord(), ::testing::ElementsAreArray(zeros));
}

TEST(ELEMENT_CLASS, Get_Set_2)
{
    Element<int> elem1;
    std::vector<int> coords = {1, 3, 5};
    elem1.setCoord(coords);
    EXPECT_EQ(elem1.getGeomDim(), 3);
    ASSERT_THAT(elem1.getCoord(), ::testing::ElementsAreArray(coords));
}

TEST(ELEMENT_CLASS, Get_Set_3)
{
    Element<int> elem1;
    std::vector<int> coords = {1, 2};
    elem1.setCoord(coords);
    elem1.setGeomDim(3);

    coords.push_back(0);
    EXPECT_EQ(elem1.getGeomDim(), 3);
    ASSERT_THAT(elem1.getCoord(), ::testing::ElementsAreArray(coords));
}

TEST(ELEMENT_CLASS, Get_Set_4)
{
    Element<int> elem1;
    std::vector<int> coords = {1, 2, 3};
    elem1.setCoord(coords);
    elem1.setGeomDim(2);

    coords.pop_back();
    EXPECT_EQ(elem1.getGeomDim(), 2);
    ASSERT_THAT(elem1.getCoord(), ::testing::ElementsAreArray(coords));
}

/// 3 - Testing the classe's operators

TEST(ELEMENT_CLASS, Arithm_Operator_1)
{
    std::vector<int> coords1 = {1, 2, 3};
    std::vector<int> coords2 = {-1, -2, -3};
    std::vector<int> zeros(3);
    std::vector<int> doubles = {2, 4, 6};

    Element<int> elem1(coords1);
    Element<int> elem2(coords2);

    Element<int> elem3(elem1 + elem2);
    Element<int> elem4(elem1 - elem2);
    ASSERT_THAT(elem3.getCoord(), ::testing::ElementsAreArray(zeros));
    ASSERT_THAT(elem4.getCoord(), ::testing::ElementsAreArray(doubles));
}

/// 4 - Testing the classe's methods

TEST(ELEMENT_CLASS, distanceTo_Trivial)
{
    Element<int> elem1;
    EXPECT_EQ(elem1.distTo(elem1), 0);
}

TEST(ELEMENT_CLASS, distanceTo_Trivial_2)
{
    std::vector<int> coords{1, 2, 3};
    Element<int> elem1(coords);
    double distance = elem1.distTo(elem1);
    EXPECT_EQ(distance, 0);
}

TEST(ELEMENT_CLASS, distanceTo_Diff_Element)
{
    std::vector<int> coords{3, 4};
    Element<int> elem1(coords);
    Element<int> elem2(2);

    double distance = elem1.distTo(elem2);

    EXPECT_NEAR(distance, 5.0, 1E-6);
}

TEST(ELEMENT_CLASS, distanceTo_Diff_Element_Dim)
{
    std::vector<int> coords{3, 4};
    Element<int> elem1(coords);
    Element<int> elem2(5);

    double distance = elem1.distTo(elem2);

    EXPECT_NEAR(distance, 5.0, 1E-6);
}

TEST(ELEMENT_CLASS, argument_Of_The_Closest_Centroid)
{

    Element<int> elem1({5, 5});
    Element<int> elem2({0, 4});
    Element<int> elem3({50, 31});

    Element<int> centroid1({0, 5});
    Element<int> centroid2({2, 2});
    Element<int> centroid3({50, 30});

    std::vector<Element<int> *> centroidVector{&centroid1, &centroid2, &centroid3};

    EXPECT_EQ(elem1.argClosest(centroidVector), 1);
    EXPECT_EQ(elem2.argClosest(centroidVector), 0);
    EXPECT_EQ(elem3.argClosest(centroidVector), 2);
}

TEST(ELEMENT_CLASS, norm_Distance_To_Origin)
{
    std::vector<int> coord{2};
    Element<int> elem1(coord);
    Element<int> elem2({3, 4});
    Element<int> elem3({1, 1, 1});

    EXPECT_NEAR(elem1.norm(), 2, 1E-6);
    EXPECT_NEAR(elem2.norm(), 5, 1E-6);
    EXPECT_NEAR(elem3.norm(), std::sqrt(3), 1E-6);
}

//-----------------------------------------------------------------------------------
//------------------------------PIXEL CLASS TEST SUIT--------------------------------
//-----------------------------------------------------------------------------------

/// 1 - Testing the classe's constructors
TEST(PIXEL_CLASS, Constructor_1)
{
    Pixel<int> pixel1; //??calling constr with () -> prototyped function not called (was a variable de finition intended?)
    EXPECT_EQ(pixel1.getGeomDim(), 2);
    EXPECT_EQ(pixel1.getCoord().size(), 2);

    EXPECT_EQ(pixel1.getColDim(), 1);
    EXPECT_EQ(pixel1.getColors().size(), 1);
}

TEST(PIXEL_CLASS, Constructor_2)
{
    Pixel<int> pixel1(3);
    EXPECT_EQ(pixel1.getGeomDim(), 2);
    EXPECT_EQ(pixel1.getCoord().size(), 2);

    EXPECT_EQ(pixel1.getColDim(), 3);
    EXPECT_EQ(pixel1.getColors().size(), 3);
}

TEST(PIXEL_CLASS, Constructor_3)
{
    std::vector<int> coords = {4, 20};
    std::vector<int> colors = {255, 0, 255};

    Pixel<int> pixel1(coords, colors);

    EXPECT_EQ(pixel1.getGeomDim(), 2);
    EXPECT_EQ(pixel1.getCoord().size(), 2);

    EXPECT_EQ(pixel1.getColDim(), 3);
    EXPECT_EQ(pixel1.getColors().size(), 3);

    ASSERT_THAT(pixel1.getCoord(), ::testing::ElementsAreArray(coords));
    ASSERT_THAT(pixel1.getColors(), ::testing::ElementsAreArray(colors));
}

TEST(PIXEL_CLASS, Constructor_4)
{
    std::vector<int> coords = {22, 11};
    std::vector<double> colors = {0.5};

    Pixel<double> *pixel1 = new Pixel<double>(coords, colors);
    Pixel<double> pixel2(*pixel1);

    delete pixel1;

    EXPECT_EQ(pixel2.getGeomDim(), 2);
    EXPECT_EQ(pixel2.getCoord().size(), 2);

    EXPECT_EQ(pixel2.getColDim(), 1);
    EXPECT_EQ(pixel2.getColors().size(), 1);

    ASSERT_THAT(pixel2.getCoord(), ::testing::ElementsAreArray(coords));
    ASSERT_THAT(pixel2.getColors(), ::testing::ElementsAreArray(colors));
}

TEST(PIXEL_CLASS, Constructor_copy_1)
{
    std::vector<int> coords = {22, 11};
    std::vector<double> colors = {0, 0, 255};

    Pixel<double> elem1(coords, colors);
    Pixel<double> elem2(elem1);
    Pixel<double> elem3(elem2);

    EXPECT_EQ(Pixel<double>::getNPixels(), 3);
}

TEST(PIXEL_CLASS, Constructor_copy_2)
{
    std::vector<Pixel<int>> elementsVector;
    elementsVector.reserve(2);

    std::vector<int> coords = {22, 11};
    std::vector<int> colors = {0, 0, 255};

    Pixel<int> pixel1(coords, colors); //1
    Pixel<int> pixel2;                 //2
    Pixel<int> pixel3(3);           //3

    elementsVector.push_back(pixel1); //4
    elementsVector.push_back(pixel2); //5
    EXPECT_EQ(Pixel<int>::getNPixels(), 5);

    //Allocation of new vector due to over capacity
    elementsVector.push_back(pixel3); //6

    EXPECT_EQ(Pixel<int>::getNPixels(), 6);
}

TEST(PIXEL_CLASS, Constructor_move)
{
    std::vector<Pixel<int>> elementsVector;
    elementsVector.reserve(2);

    std::vector<int> coords = {22, 11};
    std::vector<int> colors = {0, 0, 255};

    elementsVector.push_back(Pixel<int>());     //Move : 0
    elementsVector.push_back(Pixel<int>(3)); //Move : 0

    EXPECT_EQ(Pixel<int>::getNPixels(), 0);

    //Allocation of new vector due to over capacity
    elementsVector.push_back(Pixel<int>(coords, colors));

    EXPECT_EQ(Pixel<int>::getNPixels(), 0);
}

/// 2 - Testing the classe's Getters/Setters

TEST(PIXEL_CLASS, Get_Set_1)
{
    Pixel<int> pixel1;
    pixel1.setColDims(3);
    EXPECT_EQ(pixel1.getColDim(), 3);
    std::vector<int> zeros(3);
    ASSERT_THAT(pixel1.getColors(), ::testing::ElementsAreArray(zeros));
}

TEST(PIXEL_CLASS, Get_Set_2)
{
    Pixel<int> pixel1;
    std::vector<int> cols = {1, 3, 5};
    pixel1.setCols(cols);
    EXPECT_EQ(pixel1.getColDim(), 3);
    ASSERT_THAT(pixel1.getColors(), ::testing::ElementsAreArray(cols));
}

TEST(PIXEL_CLASS, Get_Set_3)
{
    Pixel<int> pixel1;
    std::vector<int> cols = {1, 2};
    pixel1.setCols(cols);
    pixel1.setColDims(3);

    cols.push_back(0);
    EXPECT_EQ(pixel1.getColDim(), 3);
    ASSERT_THAT(pixel1.getColors(), ::testing::ElementsAreArray(cols));
}

TEST(PIXEL_CLASS, Get_Set_4)
{
    Pixel<int> pixel1;
    std::vector<int> cols = {255, 0, 255};
    pixel1.setCols(cols);
    pixel1.setColDims(2);

    cols.pop_back();
    EXPECT_EQ(pixel1.getColDim(), 2);
    ASSERT_THAT(pixel1.getColors(), ::testing::ElementsAreArray(cols));
}

/// 3 - Testing the classe's operators

TEST(PIXEL_CLASS, Arithm_Operator_1)
{
    std::vector<int> coords1 = {1, 2, 3};
    std::vector<int> coords2 = {-1, -2, -3};
    std::vector<int> colors = {255, 0, 255};

    std::vector<int> zeros(3);
    std::vector<int> doubles = {2, 4, 6};

    Pixel<int> pixel1(coords1, colors);
    Pixel<int> pixel2(coords2, colors);

    Pixel<int> pixel3(pixel1 + pixel2);
    Pixel<int> pixel4(pixel1 - pixel2);
    ASSERT_THAT(pixel3.getCoord(), ::testing::ElementsAreArray(zeros));
    ASSERT_THAT(pixel4.getCoord(), ::testing::ElementsAreArray(doubles));
}

/// 4 - Testing the classe's methods
//// Distance
TEST(PIXEL_CLASS, distance_To_Trivial)
{
    Pixel<int> pixel1;
    EXPECT_NEAR(pixel1.distTo(pixel1), 0, 1E-6);
    EXPECT_NEAR(pixel1.distTo(pixel1), 0, 1E-6);
}

TEST(PIXEL_CLASS, distance_To_Trivial_2)
{
    std::vector<int> coords1{33, 66};
    std::vector<int> coords2{66, 33};
    std::vector<int> cols{255, 0, 255};
    Pixel<int> pixel1(coords1, cols);
    pixel1.setWeights({0, 1});
    Pixel<int> pixel2(coords2, cols);
    pixel2.setWeights({0, 1});
    double distance = pixel1.distTo(pixel2);
    EXPECT_NEAR(distance, 0, 1E-6);
}

TEST(PIXEL_CLASS, distanceTo_Diff_Pixel_Cols)
{
    std::vector<int> coords{3, 4};
    std::vector<int> cols1{251, 0, 255};
    std::vector<int> cols2{255, 3, 255};
    Pixel<int> pixel1(coords, cols1);
    pixel1.setWeights({0, 1});
    Pixel<int> pixel2(coords, cols2);
    pixel2.setWeights({0, 1});

    double distance = pixel1.distTo(pixel2);
    double distanceInv = pixel2.distTo(pixel1);

    EXPECT_NEAR(distance, 5.0, 1E-6);
    EXPECT_NEAR(distanceInv, 5.0, 1E-6);
}

TEST(PIXEL_CLASS, distanceTo_Diff_Pixel_mixed)
{
    std::vector<int> coords1{3, 4};
    std::vector<int> coords2{0, 8};

    std::vector<int> cols1{10};
    std::vector<int> cols2{15};

    Pixel<int> pixel1(coords1, cols1);
    pixel1.setWeights({0.5, 0.5});
    Pixel<int> pixel2(coords2, cols2);
    pixel2.setWeights({0.5, 0.5});

    double distance = pixel1.distTo(pixel2);
    double distanceInv = pixel2.distTo(pixel1);

    EXPECT_NEAR(distance, 5, 1E-6);
    EXPECT_NEAR(distanceInv, 5, 1E-6);
}

TEST(PIXEL_CLASS, distance_To_Diff_Pixel_Coords)
{
    std::vector<int> coords1{10, 6};
    std::vector<int> coords2{18, 0};
    std::vector<int> cols{251, 252, 253};

    Pixel<int> pixel1(coords1, cols);
    pixel1.setWeights({1, 0});
    Pixel<int> pixel2(coords2, cols);
    pixel2.setWeights({1, 0});

    double colDiff = pixel1.distTo(pixel2);
    double colDiffInv = pixel2.distTo(pixel1);

    EXPECT_NEAR(colDiff, 10, 1E-6);
    EXPECT_NEAR(colDiffInv, 10, 1E-6);
}


//// ArgClosest
TEST(PIXEL_CLASS, argument_Of_The_Closest_Centroid_default)
{
    //By default, weights are (0,1) : 0 for coordinates & 1 for colors
    Pixel<int> pixel1({100, 200}, {1, 1, 1});
    Pixel<int> pixel2({8, 8}, {255, 255, 255});

    Pixel<int> centroid1({800, 800}, {150, 150, 150});
    Pixel<int> centroid2({0, 0}, {2, 2, 2});

    std::vector<Pixel<int> *> centroidVector{&centroid1, &centroid2};

    EXPECT_EQ(pixel1.argClosest(centroidVector), 1);
    EXPECT_EQ(pixel2.argClosest(centroidVector), 0);
}

TEST(PIXEL_CLASS, argument_Of_The_Closest_Centroid_Mixed)
{
    //Demonstrating that unbalanced weights affect the 'proximity' concept between pixels

    Pixel<int> pixel1({10, 10}, {1});

    Pixel<int> pixel2({20, 15}, {61});

    pixel1.setWeights({0.2, 0.8});
    pixel2.setWeights({0.2, 0.8});

    Pixel<int> centroid1({14, 13}, {60});
    Pixel<int> centroid2({25, 20}, {3});

    std::vector<Pixel<int> *> centroidVector{&centroid1, &centroid2};

    EXPECT_EQ(pixel1.argClosest(centroidVector), 1);
    EXPECT_EQ(pixel2.argClosest(centroidVector), 0);
}

//// Norm
TEST(PIXEL_CLASS, norm_by_default)
{
    Pixel<int> pixel1({3, 4}, {0, 0, 2});
    EXPECT_NEAR(pixel1.norm(), 2, 1E-6);
}

TEST(PIXEL_CLASS, norm_mixed)
{
    Element<int> *pixel1 = new Pixel<int>({3, 4}, {0, 0, 2});
    pixel1->setWeights({0.5, 0.5});
    EXPECT_NEAR(pixel1->norm(), (0.5 * 5 + 0.5 * 2), 1E-6);
}

TEST(PIXEL_CLASS, norm_coords)
{
    Element<int> *pixel1 = new Pixel<int>({3, 4}, {0, 0, 2});
    pixel1->setWeights({1, 0});
    EXPECT_NEAR(pixel1->norm(), 5, 1E-6);
}