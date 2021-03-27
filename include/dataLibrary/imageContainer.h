#pragma once
#include "dataContainer.h"
#include "pixel.h"
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

/*
ImageContainer class is a template class that serves as a 2D container of Pixels represented
in a 1D flattened matrix (performance considerations) with a given number of channels (1 : GrayScale, 3 : RGB)).
It's templated as the pixels it contains (check atomicLibrary/../pixel.h)
*/

using namespace cv;

template <typename T>
class ImageContainer
{

private:
    std::vector<Pixel<T> *> values;
    int height;
    int width;
    int channels;

public:
    //Constructors & Destructor
    ImageContainer();
    ImageContainer(int h, int w, int c, bool randomGen); //randomGen : whether or not use randomly generated data
    ImageContainer(int h, int w, int c);
    ImageContainer(int h, int w, std::vector<Pixel<T>> &vals);
    ImageContainer(Mat image); //To be implemented once openCV is integrated with the project

    ImageContainer(const ImageContainer<T> &source);
    ImageContainer(ImageContainer<T> &&source);

    ~ImageContainer();

    //Getters & Setters
    int getHeight() const;
    int getWidth() const;
    int getChannels() const;
    std::vector<Pixel<T>> getValues() const;
    Pixel<T> at(int i, int j) const;
    Pixel<T> at(int i) const;

    void setAt(int indexH, int indexW, Pixel<T> &val);
    void setValues(std::vector<Pixel<T>> vals);
    void fillValues(int size, int c);
};