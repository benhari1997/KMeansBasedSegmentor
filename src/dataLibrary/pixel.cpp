#include <iostream>
#include <vector>
#include <cmath>
#include "dataLibrary/element.h"
#include "dataLibrary/pixel.h"
#include <exception>

//Constructors
// initializes a pixel with coordinates and colors
// initializes the color dimension: 1D or 3D
template <typename T>
Pixel<T>::Pixel(std::vector<int> coord, std::vector<T> col) : Element<int>(coord), colors{col}, colDim{static_cast<int>(col.size())}
{
    nPixels += 1;
    this->weights.push_back(1);
    this->weights[0] = 0;
}
template <typename T>
Pixel<T>::Pixel(std::vector<int> coords, std::vector<T> colors, std::vector<double> weights) : Pixel<T>(coords, colors)
{
    this->setWeights(weights);
}

// initializes a pixel with coords = {0, 0} and colors={0} or {0, 0, 0}
// geoDim must be 2
template <typename T>
Pixel<T>::Pixel(int colDim) : Element<int>(2), colDim{colDim}, colors{}
{

    this->colors.reserve(colDim);
    for (int i = 0; i < colDim; i++)
    {
        this->colors.emplace_back(static_cast<T>(0));
    }
    nPixels += 1;
    this->weights.push_back(1);
    this->weights[0] = 0;
}

// initializes a grayscale pixel
template <typename T>
Pixel<T>::Pixel() : Pixel<T>(1) //Constructor Delegation (int)
{
}

//Copy & Move (for the sake of nPixel static member update)
template <typename T>
Pixel<T>::Pixel(const Pixel<T> &source) : Element<int>(source)
{
    this->colors = source.getColors();
    this->colDim = source.getColDim();
    this->weights = source.getWeights();
    nPixels += 1;
}

template <typename T>
Pixel<T>::Pixel(Pixel<T> &&source) : Element<int>(std::move(source))
{
    this->colors = source.getColors();
    this->colDim = source.getColDim();
    this->weights = source.getWeights();
}

//Destructor
template <typename T>
Pixel<T>::~Pixel()
{
    nPixels -= 1;
}

//Getters & Setters
// returns the color dimension of a vector
template <typename T>
int Pixel<T>::getColDim() const
{
    return (this->colDim);
}

// returns the vector of colors
// its size could be weather 1 or 3
template <typename T>
std::vector<T> Pixel<T>::getColors() const
{
    return (this->colors);
}

// sets dim as color dimension of a Pixel
// in the case of old dimension = 3 and dim = 1 : we delete the 2 last values of colors
// in the case of old dimension = 1 and dim = 3 : we add 2 null values to colors
template <typename T>
void Pixel<T>::setColDims(int dim)
{
    int dimOld = this->colDim;
    this->colDim = dim;
    if (dimOld > dim)
    {
        do
        {
            this->colors.pop_back();
            dimOld -= 1;
        } while (
            dimOld > dim);
    }
    else
    {
        while (dimOld < dim)
        {
            this->colors.push_back(static_cast<T>(0));
            dimOld += 1;
        }
    }
}

// sets the colors of the pixel
template <typename T>
void Pixel<T>::setCols(std::vector<T> cols)
{
    this->colors = cols;
    this->colDim = static_cast<int>(cols.size());
}

template <typename T>
void Pixel<T>::setToZero()
{
    std::vector<int> zeros(this->geomDim);
    std::vector<T> zerosCols(this->colDim);
    this->setCoord(zeros);
    this->setCols(zerosCols);
}

//Static members
template <typename T>
unsigned int Pixel<T>::nPixels = 0;

template <typename T>
unsigned int Pixel<T>::getNPixels()
{
    return (nPixels);
}

//Distance

// calculates the distance between the current pixel and a pixel pxl
// the distance depends on colors and coordinates
// takes in argument the weights associated to the coordinates part (weightPos) and to the colors part (weightCol)
// the formula of the distance is calculated as follows:
// distance = weightPos*sqrt((x2-x1)²+(y2-y1)²)+weightCol*sqrt((r2-r1)²+(g2-g1)²+(b2-b1)²)
template <typename T>
double Pixel<T>::distTo(const Pixel<T> &pxl)
{
    double resultPos{0};
    int i = 0;
    for (const int &elem : this->coordinates)
    {
        resultPos += pow(elem - pxl.getCoord().at(i), 2);
        i += 1;
    }

    double resultCol{0};
    i = 0;
    for (const T &elem : this->colors)
    {
        resultCol += pow(elem - pxl.getColors().at(i), 2);
        i += 1;
    }

    double result = (this->weights.at(0)) * sqrt(resultPos) + (this->weights.at(1)) * sqrt(resultCol);
    return (result);
}

// calculates the distance between the current pixel and a pixel pxl
// this distance depends only on colors
// distance = sqrt((r2-r1)²+(g2-g1)²+(b2-b1)²)
template <typename T>
int Pixel<T>::argClosest(const std::vector<Pixel<T> *> &pixels)
{
    if (pixels.size() == 1)
    {
        return (0);
    }
    double dist = this->distTo(*(pixels.at(0)));
    double tempDist = 0;
    size_t arg = 0;
    for (size_t i = 1; i < pixels.size(); i++)
    {
        tempDist = this->distTo(*(pixels.at(i)));
        if (tempDist < dist)
        {
            dist = tempDist;
            arg = i;
        }
    }
    return (static_cast<int>(arg));
}

template <typename T>
double Pixel<T>::norm()
{
    double resultCol{0};
    double resultCoord{0};
    int i = 0;
    for (const T &elem : this->coordinates)
    {
        resultCoord += pow(elem, 2);
        i += 1;
    }
    i = 0;
    for (const T &col : this->colors)
    {
        resultCol += pow(col, 2);
        i += 1;
    }
    double result = (this->weights.at(0)) * std::sqrt(resultCoord) + (this->weights.at(1)) * std::sqrt(resultCol);
    return (result);
}

//Arithmetic Operators
template <typename T>
Pixel<T> Pixel<T>::operator+(const Pixel<T> &source)
{
    std::vector<int> sumCoord(this->geomDim, 0);
    int i = 0;
    for (int &elem : sumCoord)
    {
        elem = this->getCoord().at(i) + source.getCoord().at(i);
        i++;
    }

    std::vector<T> sumCol(this->colDim, 0);
    i = 0;
    for (T &elem : sumCol)
    {
        elem = this->getColors().at(i) + source.getColors().at(i);
        i++;
    }
    Pixel<T> summation(sumCoord, sumCol);
    return (summation);
}

template <typename T>
Pixel<T> Pixel<T>::operator-(const Pixel<T> &source)
{
    std::vector<int> sumCoord(this->geomDim, 0);
    int i = 0;
    for (int &elem : sumCoord)
    {
        elem = this->getCoord().at(i) - source.getCoord().at(i);
        i++;
    }

    std::vector<T> sumCol(this->colDim, 0);
    i = 0;
    for (T &elem : sumCol)
    {
        elem = this->getColors().at(i) - source.getColors().at(i);
        i++;
    }
    Pixel<T> diff(sumCoord, sumCol);
    return (diff);
}

template <typename T>
Pixel<T> &Pixel<T>::operator=(const Pixel<T> &source)
{
    this->coordinates = source.getCoord();
    this->geomDim = source.getGeomDim();
    this->colDim = source.getColDim();
    this->colors = source.getColors();
    nElements += 1;
    return (*this);
}

template <typename T>
Pixel<T> &Pixel<T>::operator=(Pixel<T> &&source)
{
    this->coordinates = source.getCoord();
    this->geomDim = source.getGeomDim();
    this->colDim = source.getColDim();
    this->colors = source.getColors();
    nElements += 1;
    return (*this);
}

template <typename T>
Pixel<T> Pixel<T>::operator/(int d)
{
    if (d == 0)
    {
        return (*this);
    }
    std::vector<int> dividedCoords(this->geomDim);
    dividedCoords = this->getCoord();
    for (int &val : dividedCoords)
    {
        val = val / d;
    }

    std::vector<T> dividedCols(this->colDim);
    dividedCols = this->getColors();
    for (T &val : dividedCols)
    {
        val = val / d;
    }
    Pixel<T> result(dividedCoords, dividedCols);
    return (result);
}

//Stream Operator
template <typename T>
std::ostream &operator<<(std::ostream &os, const Pixel<T> &elem)
{
    //Serialize the base class os operator to use it here
    os << "Pixel with Colors : (";
    for (const T &col : elem.getColors())
    {
        os << col << " , ";
    }
    os << ") " << std::endl;

    return (os);
}

//Comparison operator
template <typename T>
bool operator==(const Pixel<T> &lhs, const Pixel<T> &rhs)
{
    //Element base
    bool coordsEq = (lhs.coordinates.size() == (rhs.coordinates.size()));
    int i = 0;
    for (int coord : lhs.coordinates)
    {
        coordsEq = coordsEq && (coord == rhs.coordinates[i]);
        i += 1;
    }
    bool dimEq = (lhs.geomDim == rhs.geomDim);

    //Colors specificities
    bool colsEq = (lhs.colors.size() == (rhs.colors.size()));
    i = 0;
    for (T col : lhs.colors)
    {
        coordsEq = coordsEq && (col == rhs.colors[i]);
        i += 1;
    }
    bool colDimEq = (lhs.colDim == rhs.colDim);

    return (dimEq && coordsEq && colDimEq && colsEq);
}

//Instantiate some Pixel class templates
template class Pixel<int>;
template class Pixel<double>;

template bool operator==(const Pixel<int> &lhs, const Pixel<int> &rhs);
template bool operator==(const Pixel<double> &lhs, const Pixel<double> &rhs);

template std::ostream &operator<<(std::ostream &os, const Pixel<int> &elem);
template std::ostream &operator<<(std::ostream &os, const Pixel<double> &elem);