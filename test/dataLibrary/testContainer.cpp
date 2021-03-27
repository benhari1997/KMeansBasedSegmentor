#include <iostream>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <vector>
#include "dataLibrary/imageContainer.h"
#include "dataLibrary/dataContainer.h"

//-----------DATACONTAINER CLASS TEST SUIT
/// 1 - Testing the classe's constructors
TEST(DATA_CONTAINER_CLASS, Constructor_1)
{
    DataContainer<int> data(10, 2);

    std::vector<Element<int>> elements;

    elements.reserve(10);
    for (int i = 0; i < 10; i++)
    {
        elements.emplace_back(2); //Constructor based on Element dimension : here 2
    }

    EXPECT_EQ(data.getGeoDim(), 2);
    EXPECT_EQ(data.getValues().size(), 10);
    EXPECT_EQ(data.getValues().at(0).getGeomDim(), 2);
    ASSERT_THAT(data.getValues(), ::testing::ElementsAreArray(elements));
}

TEST(DATA_CONTAINER_CLASS, Constructor_2)
{
    DataContainer<int> data(20, 1, false);

    std::vector<Element<int>> elements;

    elements.reserve(20);
    for (int i = 0; i < 20; i++)
    {
        elements.emplace_back(1); //Constructor based on Element dimension : here 2
    }

    EXPECT_EQ(data.getGeoDim(), 1);
    EXPECT_EQ(data.getValues().size(), 20);
    EXPECT_EQ(data.getValues().at(0).getGeomDim(), 1);
    ASSERT_THAT(data.getValues(), ::testing::ElementsAreArray(elements));
}

TEST(DATA_CONTAINER_CLASS, Constructor_3)
{
    DataContainer<int> data;

    std::vector<Element<int>> elements;

    elements.reserve(1);
    elements.emplace_back(); //Constructor based on Element dimension : here 2

    EXPECT_EQ(data.getGeoDim(), 1);
    EXPECT_EQ(data.getValues().size(), 1);
    EXPECT_EQ(data.getValues().at(0).getGeomDim(), 1);
    ASSERT_THAT(data.getValues(), ::testing::ElementsAreArray(elements));
}

TEST(DATA_CONTAINER_CLASS, Constructor_4)
{
    std::vector<Element<int> *> elements;
    std::vector<Element<int>> elementsValues;
    elements.reserve(10);
    elementsValues.reserve(10);
    std::vector<int> coords(2);
    for (int i = 0; i < 10; i++)
    {
        coords = {i, i + 1};
        elements.emplace_back(new Element<int>(coords));
        elementsValues.emplace_back(coords);
    }

    DataContainer<int> data(elements);

    EXPECT_EQ(data.getGeoDim(), 2);
    EXPECT_EQ(data.getValues().size(), 10);
    EXPECT_EQ(data.getValues().at(0).getGeomDim(), 2);
    ASSERT_THAT(data.getValues(), ::testing::ElementsAreArray(elementsValues));
    //No need to clear elements vector pointers, they're destroyed inside
    //DataContainer destructor
}

TEST(DATA_CONTAINER_CLASS, Constructor_5)
{
    std::vector<Element<int>> elements;
    elements.reserve(10);
    std::vector<int> coords(2);
    for (int i = 0; i < 10; i++)
    {
        coords = {i, i * 2};
        elements.emplace_back(coords);
    }

    DataContainer<int> data(elements);

    EXPECT_EQ(data.getGeoDim(), 2);
    EXPECT_EQ(data.getValues().size(), 10);
    EXPECT_EQ(data.getValues().at(0).getGeomDim(), 2);
    ASSERT_THAT(data.getValues(), ::testing::ElementsAreArray(elements));
}

//Copy constructor

TEST(DATA_CONTAINER_CLASS, Constructor_Copy)
{
    std::vector<Element<int>> elements;
    elements.reserve(10);
    std::vector<int> coords(2);
    for (int i = 0; i < 10; i++)
    {
        coords = {i, i * 2};
        elements.emplace_back(coords);
    }

    DataContainer<int> data(elements);

    DataContainer<int> dataCopy(data);

    data.fillValues(5, 3);

    EXPECT_EQ(dataCopy.getGeoDim(), 2);
    EXPECT_EQ(dataCopy.getValues().size(), 10);
    EXPECT_EQ(dataCopy.getValues().at(0).getGeomDim(), 2);
    ASSERT_THAT(dataCopy.getValues(), ::testing::ElementsAreArray(elements));
}

//Move Constructor

TEST(DATA_CONTAINER_CLASS, Constructor_Move)
{
    std::vector<Element<int>> elements;
    elements.reserve(10);
    std::vector<int> coords(2);
    for (int i = 0; i < 10; i++)
    {
        coords = {i, i * 2};
        elements.emplace_back(coords);
    }

    std::vector<DataContainer<int>> myDCotainers;
    myDCotainers.reserve(2);

    myDCotainers.push_back(DataContainer<int>(elements));
    myDCotainers.push_back(DataContainer<int>());

    EXPECT_EQ(myDCotainers.at(0).getGeoDim(), 2);
    EXPECT_EQ(myDCotainers.at(0).getValues().size(), 10);
    EXPECT_EQ(myDCotainers.at(0).getValues().at(0).getGeomDim(), 2);
    ASSERT_THAT(myDCotainers.at(0).getValues(), ::testing::ElementsAreArray(elements));
}

// /// 2 - Testing the classe's Getters/Setters

TEST(DATA_CONTAINER_CLASS, get_at_index)
{
    std::vector<Element<double>> elements;
    elements.reserve(5);
    std::vector<double> coords(2);
    for (int i = 0; i < 5; i++)
    {
        coords = {i * 0.1, i * i * 0.1};
        elements.emplace_back(coords);
    }

    DataContainer<double> data(elements); //2 for dimension of the Element

    ASSERT_EQ(data.at(1), elements.at(1));
    ASSERT_EQ(data.at(2), elements.at(2));
}

TEST(DATA_CONTAINER_CLASS, set_at_index_value)
{
    //T at(int i) const;
    Element<int> atomicElem(2);
    std::vector<Element<int>> elements(10, atomicElem);

    DataContainer<int> data(elements); //2 for dimension of the Element

    std::vector<int> coord{19, 95};
    Element<int> elem(coord);
    elements[2] = elem;

    data.setAt(2, elem);

    EXPECT_EQ(data.at(2), elem);
}

TEST(DATA_CONTAINER_CLASS, get_values)
{
    std::vector<Element<int>> elements;
    elements.reserve(10);
    std::vector<int> coords(2);
    for (int i = 0; i < 10; i++)
    {
        coords = {i + 1, (i + 1) * (i + 1)};
        elements.emplace_back(coords);
    }

    DataContainer<int> data(elements); //2 for dimension of the Element

    ASSERT_THAT(data.getValues(), ::testing::ElementsAreArray(elements));
}

TEST(DATA_CONTAINER_CLASS, set_values_dim)
{

    DataContainer<int> data(25, 3);

    std::vector<Element<int>> elements;

    elements.reserve(10);
    std::vector<int> coords(2);
    for (int i = 0; i < 10; i++)
    {
        coords = {i + 1, (i + 1) * (i + 1)};
        elements.emplace_back(coords);
    }

    data.setValues(elements);

    ASSERT_THAT(data.getValues(), ::testing::ElementsAreArray(elements));
    EXPECT_EQ(data.getGeoDim(), 2);
}

TEST(DATA_CONTAINER_CLASS, set_get_geo_dim)
{
    DataContainer<int> data(25, 3);
    data.setGeoDim(2);
    EXPECT_EQ(data.getGeoDim(), 2);
    EXPECT_EQ(data.getValues().at(0).getGeomDim(), 2);
}

//-----------ImageContainer CLASS TEST SUIT
/// 1 - Testing the class's constructors

TEST(IMAGE_CONTAINER_CLASS, Constructor_1)
{
    ImageContainer<int> imgdata(10, 10, 3);

    std::vector<Pixel<int>> img;

    img.reserve(10 * 10);

    for (int i = 0; i < 100; i++)
    {
        img.emplace_back(3);
    }

    EXPECT_EQ(imgdata.getHeight(), 10);
    EXPECT_EQ(imgdata.getWidth(), 10);
    EXPECT_EQ(imgdata.getChannels(), 3);
    EXPECT_EQ(imgdata.at(0, 0).getColDim(), 3);
    ASSERT_THAT(imgdata.getValues(), ::testing::ElementsAreArray(img));
}

TEST(IMAGE_CONTAINER_CLASS, Constructor_2)
{
    ImageContainer<int> imgdata(10, 5, 1, false);

    std::vector<Pixel<int>> img;

    img.reserve(10 * 5);
    for (int i = 0; i < 50; i++)
    {
        img.emplace_back(1);
    }

    EXPECT_EQ(imgdata.getHeight(), 10);
    EXPECT_EQ(imgdata.getWidth(), 5);
    EXPECT_EQ(imgdata.getChannels(), 1);
    ASSERT_THAT(imgdata.getValues(), ::testing::ElementsAreArray(img));
}

TEST(IMAGE_CONTAINER_CLASS, Constructor_3)
{
    ImageContainer<int> imgdata;

    std::vector<Pixel<int>> img;

    img.emplace_back();

    EXPECT_EQ(imgdata.getHeight(), 1);
    EXPECT_EQ(imgdata.getWidth(), 1);
    EXPECT_EQ(imgdata.getChannels(), 1);
    ASSERT_THAT(imgdata.getValues(), ::testing::ElementsAreArray(img));
}

TEST(IMAGE_CONTAINER_CLASS, Constructor_4)
{
    int h = 50;
    int w = 80;
    int c = 3;
    std::vector<Pixel<int>> imgData;

    std::vector<int> coords(2);
    std::vector<int> colors(3);
    imgData.reserve(h * w);

    for (int i = 0; i < h * w; i++)
    {
        coords = {i / w, i % w}; //True coordinates
        colors = {255, rand() % 255, rand() % 255};
        imgData.emplace_back(coords, colors);
    }

    ImageContainer<int> image(h, w, imgData);

    EXPECT_EQ(image.getHeight(), h);
    EXPECT_EQ(image.getWidth(), w);
    EXPECT_EQ(image.getChannels(), c);
    ASSERT_THAT(image.getValues(), ::testing::ElementsAreArray(imgData));
}

//Copy & Move constructors

TEST(IMAGE_CONTAINER_CLASS, Copy_Constructor)
{
    std::vector<Pixel<int>> imgData;

    std::vector<int> coords(2, 10);
    std::vector<int> colors(3, 255);

    imgData.reserve(12 * 10);

    for (int i = 0; i < 12 * 10; i++)
    {
        coords = {i / 10, i % 10};
        colors = {rand() % 255, 255, rand() % 255};
        imgData.emplace_back(coords, colors);
    }

    ImageContainer<int> image(12, 10, imgData);

    ImageContainer<int> imageCopy(image);

    image.fillValues(5 * 5, 1);

    EXPECT_EQ(imageCopy.getHeight(), 12);
    EXPECT_EQ(imageCopy.getWidth(), 10);
    EXPECT_EQ(imageCopy.getChannels(), 3);
    ASSERT_THAT(imageCopy.getValues(), ::testing::ElementsAreArray(imgData));
}

TEST(IMAGE_CONTAINER_CLASS, Move_Constructor)
{
    std::vector<Pixel<int>> imgData;

    std::vector<int> coords(2, 10);
    std::vector<int> colors(3, 255);

    imgData.reserve(12 * 10);

    for (int i = 0; i < 12 * 10; i++)
    {
        coords = {i / 10, i % 10};
        colors = {rand() % 255, 255, rand() % 255};
        imgData.emplace_back(coords, colors);
    }

    std::vector<ImageContainer<int>> imageBatch;
    imageBatch.reserve(2);
    imageBatch.push_back(ImageContainer<int>(12, 10, imgData));
    imageBatch.push_back(ImageContainer<int>());

    EXPECT_EQ(imageBatch.at(0).getHeight(), 12);
    EXPECT_EQ(imageBatch.at(0).getWidth(), 10);
    EXPECT_EQ(imageBatch.at(0).getChannels(), 3);
    ASSERT_THAT(imageBatch.at(0).getValues(), ::testing::ElementsAreArray(imgData));
}

// 2- testing the classes Getters and Setters

TEST(IMAGE_CONTAINER_CLASS, get_height_width_channels)
{
    ImageContainer<int> imgdata(12, 10, 3);

    EXPECT_EQ(imgdata.getHeight(), 12);
    EXPECT_EQ(imgdata.getWidth(), 10);
    EXPECT_EQ(imgdata.getChannels(), 3);
}

TEST(IMAGE_CONTAINER_CLASS, get_set_values)
{
    std::vector<Pixel<int>> imgData;

    std::vector<int> coords(2, 10);
    std::vector<int> colors(3, 255);

    imgData.reserve(12 * 10);

    for (int i = 0; i < 12 * 10; i++)
    {
        coords = {i / 10, i % 10};
        colors = {rand() % 255, 255, rand() % 255};
        imgData.emplace_back(coords, colors);
    }

    ImageContainer<int> image(5, 5, 1); //5 by 5 grascale pixel
    EXPECT_EQ(image.getChannels(), 1);

    image.setValues(imgData); //Filling using 12 by 10 RGB pixels

    ASSERT_THAT(image.getValues(), ::testing::ElementsAreArray(imgData));
    EXPECT_EQ(image.getChannels(), 3);
}

TEST(IMAGE_CONTAINER_CLASS, get_set_values_diff_dim)
{
    std::vector<Pixel<int>> imgData;

    std::vector<int> coords(2, 10);
    std::vector<int> colors(3, 255);

    imgData.reserve(12 * 10);

    for (int i = 0; i < 12 * 10; i++)
    {
        coords = {i / 10, i % 10};
        colors = {rand() % 255, 255, rand() % 255};
        imgData.emplace_back(coords, colors);
    }

    ImageContainer<int> image;
    image.setValues(imgData);

    ASSERT_THAT(image.getValues(), ::testing::ElementsAreArray(imgData));
}

TEST(IMAGE_CONTAINER_CLASS, get_at_index)
{
    std::vector<Pixel<int>> imgData;

    std::vector<int> coords(2);
    std::vector<int> colors(3);

    int h = 10;
    int w = 5;
    imgData.reserve(h * w);

    for (int i = 0; i < h * w; i++)
    {
        coords = {i / w, i % w};
        colors = {i, 255, rand() % 255};
        imgData.emplace_back(coords, colors);
    }

    ImageContainer<int> image(h, w, imgData);

    ASSERT_EQ(image.at(0, 0), imgData.at(0 * w + 0));
    ASSERT_EQ(image.at(1, 1), imgData.at(1 * w + 1));
    ASSERT_EQ(image.at(5, 2), imgData.at(5 * w + 2));
    ASSERT_EQ(image.at(9, 4), imgData.at(9 * w + 4));
}

TEST(IMAGE_CONTAINER_CLASS, set_at_index)
{
    Pixel<int> atomicPxl(1);
    std::vector<Pixel<int>> imgData(10 * 10, atomicPxl);

    ImageContainer<int> image(10, 10, imgData);

    std::vector<int> coords{2, 8};
    std::vector<int> colors{255};
    Pixel<int> pxl(coords, colors);

    image.setAt(3, 5, pxl);

    EXPECT_EQ(image.at(3, 5), pxl);
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            if (i != 3 && j != 5)
            {
                EXPECT_EQ(image.at(i, j), atomicPxl);
            }
        }
    }
}

TEST(IMAGE_CONTAINER_CLASS, fill_values)
{
    ImageContainer<int> image(15, 10, 1);
    image.fillValues(15 * 10, 1);

    EXPECT_EQ(image.getHeight(), 15);
    EXPECT_EQ(image.getWidth(), 10);
    EXPECT_EQ(image.getChannels(), 1);
    EXPECT_EQ(image.at(0, 0).getCoord().size(), 2);
    EXPECT_EQ(image.at(0, 0).getColDim(), 1);
}