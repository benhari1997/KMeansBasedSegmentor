#include <iostream>
#include <vector>
#include <cmath>
#include "dataLibrary/element.h"

using namespace std;

//Constructors
//initializes an Element with one dimension
template <typename T>
Element<T>::Element() : geomDim{1},
                        coordinates{} //Delegating constructor
{
    this->coordinates.reserve(1);
    this->coordinates.emplace_back(static_cast<T>(0));
    nElements += 1;
}

// initializes the coordinates of an Element with vect
// and it initializes the geomDim of the Element with the size of vect
template <typename T>
Element<T>::Element(std::vector<T> vect) : coordinates{vect}, geomDim{static_cast<int>(vect.size())}
{
    nElements += 1;
}

// initializes the dimensions of an Element with dim
// fills its coordinates with zeros
template <typename T>
Element<T>::Element(int dim) : geomDim{dim}, coordinates{}
{
    this->coordinates.reserve(dim);
    for (int i = 0; i < dim; i++)
    {
        this->coordinates.emplace_back(static_cast<T>(0));
    }
    nElements += 1;
}

//Copy & Move constr ovverided just for the sake of static nElements update
// Copy constructor
template <typename T>
Element<T>::Element(const Element<T> &source)
{
    this->geomDim = source.getGeomDim();
    this->coordinates = source.getCoord();
    nElements += 1;
}

// Move constructor
template <typename T>
Element<T>::Element(Element<T> &&source)
{
    this->geomDim = source.getGeomDim();
    this->coordinates = source.getCoord();
    nElements += 1;
}

//Destructor

template <typename T>
Element<T>::~Element()
{
    nElements -= 1;
}

//Setters
// sets the coordinates of an Element with a vector coord
template <typename T>
void Element<T>::setCoord(std::vector<T> coord)
{
    this->coordinates = coord;
    this->geomDim = static_cast<int>(coord.size());
}

//Padding along coordinates when the passed dimension is larger than the original
//Truncating along coordinates when the passed dimension is smaller than the original
template <typename T>
void Element<T>::setGeomDim(int dim)
{
    int dimOld = this->geomDim;
    this->geomDim = dim;
    if (dimOld > dim)
    {
        do
        {
            this->coordinates.pop_back();
            dimOld -= 1;
        } while (
            dimOld > dim);
    }
    else
    {
        while (dimOld < dim)
        {
            this->coordinates.push_back(0);
            dimOld += 1;
        }
    }
}

template <typename T>
void Element<T>::setToZero()
{
    std::vector<T> zeros(this->geomDim);
    this->setCoord(zeros);
}

template <typename T>
void Element<T>::setWeights(const std::vector<double> &W)
{
    this->weights.clear();
    this->weights.reserve(W.size());
    for (const double w : W)
    {
        weights.emplace_back(w);
    }
}

//Getters
// returns the coordinates of an Element
template <typename T>
std::vector<T> Element<T>::getCoord() const
{
    return (this->coordinates);
}

// returns the dimension of an Element: 1D, 2D or 3D
template <typename T>
int Element<T>::getGeomDim() const
{
    return (this->geomDim);
}

template <typename T>
std::vector<double> Element<T>::getWeights() const
{
    return (this->weights);
}

//Static members
template <typename T>
unsigned int Element<T>::nElements = 0;

template <typename T>
unsigned int Element<T>::getNElem()
{
    return (nElements);
}

//Distances
// returns the euclidian distance to an Element source
// the formula used for distance is the following
// distance = sqrt((x2-x1)² + (y2-y2)²)
template <typename T>
double Element<T>::distTo(const Element<T> &source)
{
    double result{0};
    int i = 0;
    for (const T &elem : this->coordinates)
    {
        result += pow(elem - source.getCoord().at(i), 2);
        i += 1;
    }
    return (std::sqrt(result));
}

// retruns an integer which represents the id of the nearest centroid
template <typename T>
int Element<T>::argClosest(const std::vector<Element<T> *> &elements)
{
    if (elements.size() == 1)
    {
        return (0);
    }
    double dist = this->distTo(*(elements.at(0)));
    double tempDist = 0;
    int arg = 0;
    for (size_t i = 1; i < elements.size(); i++)
    {
        tempDist = this->distTo(*(elements.at(i)));
        if (tempDist < dist)
        {
            dist = tempDist;
            arg = i;
        }
    }
    return (arg);
}

// return the distance of the Element to the origin
// the origin is the Element with coordinates {0, 0}
// norm = sqrt(x² + y²)
template <typename T>
double Element<T>::norm()
{
    double result{0};
    int i = 0;
    for (const T &elem : this->coordinates)
    {
        result += pow(static_cast<double>(elem), 2);
        i += 1;
    }

    return (std::sqrt(result));
}

//Operators overloading
template <typename T>
Element<T> Element<T>::operator+(const Element &source)
{

    vector<T> sumCoord(this->geomDim, 0);
    int i = 0;
    for (T &elem : sumCoord)
    {
        elem = this->getCoord().at(i) + source.getCoord().at(i);
        i++;
    }

    Element<T> summation(sumCoord);
    return (summation);
}

template <typename T>
Element<T> Element<T>::operator-(const Element &source)
{
    vector<T> diffCoord(this->geomDim, 0);
    int i = 0;
    for (T &elem : diffCoord)
    {
        elem = this->getCoord().at(i) - source.getCoord().at(i);
        i++;
    }

    Element<T> diff(diffCoord);
    return (diff);
}

template <typename T>
Element<T> operator/(const Element<T> &elem, int d)
{
    if (d == 0)
    {
        return (0);
    }
    std::vector<T> dividedCoords(elem.geomDim);
    dividedCoords = elem.coordinates;
    for (T &val : dividedCoords)
    {
        val = val / d;
    }
    Element<T> result(dividedCoords);
    return (result);
}

template <typename T>
std::ostream &operator<<(std::ostream &os, const Element<T> &elem)
{
    os << "Element with Coordinates : (";
    for (const T &elem : elem.coordinates)
    {
        os << elem << " , ";
    }
    os << ") " << endl;
    return (os);
}

template <typename T>
Element<T> &Element<T>::operator=(const Element<T> &source)
{
    this->coordinates = source.getCoord();
    this->geomDim = source.getGeomDim();
    return (*this);
}

template <typename T>
bool operator==(const Element<T> &lhs, const Element<T> &rhs)
{
    bool coordsEq = (lhs.coordinates.size() == (rhs.coordinates.size()));
    int i = 0;
    for (const T &coord : lhs.coordinates)
    {
        coordsEq = coordsEq && (coord == rhs.coordinates[i]);
        i += 1;
    }
    bool dimEq = (lhs.geomDim == rhs.geomDim);

    return (dimEq && coordsEq);
}

//Explicit instanciation of some commonly used types as element coordinates
template class Element<double>;
template class Element<int>;

template bool operator==(const Element<int> &lhs, const Element<int> &rhs);
template bool operator==(const Element<double> &lhs, const Element<double> &rhs);

template Element<int> operator/(const Element<int> &lhs, int rhs);
template Element<double> operator/(const Element<double> &lhs, int rhs);

template std::ostream &operator<<(std::ostream &os, const Element<int> &elem);
template std::ostream &operator<<(std::ostream &os, const Element<double> &elem);
