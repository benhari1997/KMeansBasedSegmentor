#include <iostream>
#include "dataLibrary/imageContainer.h"
#include <exception>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

// Constructors

// initializes the height, width and number of channels of an ImageContainer
// and it fills its values with new pixels
template <typename T>
ImageContainer<T>::ImageContainer(int h, int w, int c)
    : height(h),
      width(w),
      channels(c),
      values{}
{
    this->fillValues(h * w, c);
}

// initializes an ImageContainer with one grayscale pixel
template <typename T>
ImageContainer<T>::ImageContainer()
    : ImageContainer(1, 1, 1)
{
}

template <typename T>
ImageContainer<T>::ImageContainer(int h, int w, int c, bool randomGen)
    : ImageContainer(h, w, c)
{
    //if(randomGen)
    //fill the image with randomly generated data
    //(external function to be implemented and called here)
}

// initializes the height and width  of an ImageContainer
// and it fills its values with vals
template <typename T>
ImageContainer<T>::ImageContainer(int h, int w, std::vector<Pixel<T>> &vals)
    : ImageContainer(h, w, vals.at(0).getColDim())
{
    for (int i = 0; i < h * w; i++)
    {
        *(this->values[i]) = vals.at(i);
    }
}
//COnstructing using opencv Mat
template <typename T>
ImageContainer<T>::ImageContainer(cv::Mat image) : ImageContainer<T>(image.rows,
                                                                     image.cols,
                                                                     image.channels())
{
    std::vector<int> coords(2);
    std::vector<T> cols(this->channels);
    Pixel<T> pxl;
    
    for (int i = 0; i < this->height; i++)
    {
        for (int j = 0; j < this->width; j++)
        {
            coords = {i, j};
            for (int c = 0; c < this->channels; c++)
            {
                if (this->channels == 3)
                {
                    cols[c] = static_cast<T>(image.at<Vec3b>(i, j)[c]);
                }
                else
                {
                    cols[c] = static_cast<T>(image.at<uchar>(i, j));
                }
            }
            pxl = Pixel<T>(coords, cols);
            this->setAt(i, j, pxl);
        }
    }
}

//Copy constructor
template <typename T>
ImageContainer<T>::ImageContainer(const ImageContainer<T> &source)
{
    this->height = source.getHeight();
    this->width = source.getWidth();
    this->channels = source.getChannels();
    this->values.reserve(source.getValues().size());
    for (const Pixel<T> &pxl : source.getValues())
    {
        this->values.push_back(new Pixel<T>(pxl));
    }
    std::cout << "Image copy constr called" << std::endl;
}

//Move constructor
template <typename T>
ImageContainer<T>::ImageContainer(ImageContainer<T> &&source)
{
    this->height = source.getHeight();
    this->width = source.getWidth();
    this->channels = source.getChannels();
    this->values = std::move(source.values);
    std::cout << "Image move constr called" << std::endl;
}

// Destructor
template <typename T>
ImageContainer<T>::~ImageContainer()
{
    for (Pixel<T> *pxl : (this->values))
    {
        delete pxl;
    }
    this->values.clear();
}

// Setters
// sets a pixel val in the specific indexH and indexW
template <typename T>
void ImageContainer<T>::setAt(int indexH, int indexW, Pixel<T> &val)
{
    // determine the index in the flattened image
    //Throw exception if index is out of bounds
    int index = indexH * (this->width) + indexW;
    *((this->values)[index]) = val;
}

// clears the existing values of an ImageConatiner and sets its values with vals
// updates its number of channels with the number of channels of pixels in vals
template <typename T>
void ImageContainer<T>::setValues(std::vector<Pixel<T>> vals)
{
    for (auto pxl : this->values)
    {
        delete pxl;
    }
    this->values.clear();

    this->values.reserve(vals.size());
    for (const Pixel<T> &pxl : vals)
    {
        values.push_back(new Pixel<T>(pxl));
    }
    this->channels = vals.at(0).getColDim();
}

// Getters
// returns the height of an ImageContainer
template <typename T>
int ImageContainer<T>::getHeight() const
{
    return this->height;
}

// returns the width of an ImageContainer
template <typename T>
int ImageContainer<T>::getWidth() const
{
    return this->width;
}

// returns the number of channels of an ImageContainer
template <typename T>
int ImageContainer<T>::getChannels() const
{
    return this->channels;
}

// deletes any existing values of an ImageContainer
// and initializes its values by Pixels of dimention 2 and number of channels c
template <typename T>
void ImageContainer<T>::fillValues(int size, int c)
{
    if (this->values.size() > 0)
    {
        for (Pixel<T> *pxl : this->values)
        {
            delete pxl;
        }
        this->values.clear();
    }
    this->values.reserve(size);
    for (int i = 0; i < size; i++)
    {
        this->values.push_back(new Pixel<T>(c)); // c : GrayScale or RGB
    }
}

// returns a vector of values of the ImageContainer
template <typename T>
std::vector<Pixel<T>> ImageContainer<T>::getValues() const
{
    std::vector<Pixel<T>> returnedValues;
    returnedValues.reserve(static_cast<int>(this->values.size()));
    for (const Pixel<T> *pxl : this->values)
    {
        returnedValues.emplace_back(*pxl);
    }

    return returnedValues;
}

// returns a pixel positioned at the indexes i and j
template <typename T>
Pixel<T> ImageContainer<T>::at(int i, int j) const
{

    // determine the index of the flattened image
    int index = i * this->width + j;
    return (*(this->values.at(index)));
}
// returns a pixel positioned at the flattened index
template <typename T>
Pixel<T> ImageContainer<T>::at(int i) const
{
    return (*(this->values.at(i)));
}

//Explicit instantiation of ImageContainer template class
template class ImageContainer<int>;
template class ImageContainer<double>;
// template class ImageContainer<unsigned int>;
