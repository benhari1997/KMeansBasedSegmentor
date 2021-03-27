#include <iostream>
#include <memory>
#include "dataLibrary/element.h"
#include "dataLibrary/dataContainer.h"
#include <exception>

using namespace std;

// Constructors
// takes a vector of elements and initializes a DataContainer values 
// and its elements dimensions
template <typename T>
DataContainer<T>::DataContainer(std::vector<Element<T> *> vals)
    : values(vals), geoDim(vals.at(0)->getGeomDim())
{
}

// takes a reference of o vector of elements and initializes a
// DataContainer values
template <typename T>
DataContainer<T>::DataContainer(std::vector<Element<T>> &vals)
    : values{}
{
    values.reserve(vals.size());
    for (const Element<T> &val : vals)
    {
        values.push_back(new Element<T>(val));
    }
    this->geoDim = this->values.at(0)->getGeomDim();
}

// takes an integer of the size of the DataContainer and the dimensions of 
// its elements and inializes a DataConatiner with elements of dimension dim
template <typename T>
DataContainer<T>::DataContainer(int size, int dim)
    : geoDim(dim), values{}
{
    this->fillValues(size, dim);
}

template <typename T>
DataContainer<T>::DataContainer(int size, int dim, bool randomGen)
    : DataContainer(size, dim)
{
    // if (randomGen){
    // feed values with random generated data
    //}
}

// initializes the DataContainer with One 1D Element
template <typename T>
DataContainer<T>::DataContainer()
    : DataContainer(1, 1)
{}

// Destructors
template <typename T>
DataContainer<T>::~DataContainer()
{
    for (Element<T> *elem : (this->values))
    {
        delete elem;
    }
    this->values.clear();
}

//Copy Constructor
template <typename T>
DataContainer<T>::DataContainer(const DataContainer<T> &source)
{
    this->geoDim = source.getGeoDim();
    this->values.reserve(source.getValues().size());
    for (const Element<T> &elem : source.getValues())
    {
        this->values.push_back(new Element<T>(elem));
    }
}

//Move constructor
template <typename T>
DataContainer<T>::DataContainer(DataContainer<T> &&source)
{
    this->geoDim = source.getGeoDim();
    this->values = std::move(source.values);
}

// fills values of a DataContainer with size elements of dimension dim
template <typename T>
void DataContainer<T>::fillValues(int size, int dim)
{
    if (this->values.size() > 0)
    {
        for (Element<T> *elem : this->values)
        {
            delete elem;
        }
        this->values.clear();
    }
    this->values.reserve(size);
    for (int i = 0; i < size; i++)
    {
        // initialize values by Elements of dimension dim
        this->values.push_back(new Element<T>(dim));
    }
}

// Getters
// returns a vector containing all elements of a DataContainer
template <typename T>
std::vector<Element<T>> DataContainer<T>::getValues() const
{
    std::vector<Element<T>> returnedValues;
    returnedValues.reserve(static_cast<int>(this->values.size()));
    for (const Element<T> *elem : this->values)
    {
        returnedValues.emplace_back(*elem);
    }

    return returnedValues;
}

// returns the dimension of elements of a  DataContainer
template <typename T>
int DataContainer<T>::getGeoDim() const
{
    return geoDim;
}

// returns an element of the DataContainer positioned at the index i
template <typename T>
Element<T> DataContainer<T>::at(int i) const
{
   
    return (*(this->values.at(i))); 
   
}

// Setters
// sets an element in a specific index of a DataContainer
template <typename T>
void DataContainer<T>::setAt(int index, Element<T> &val)
{
    *(this->values)[index] = val;
}

// sets the values of a DataContainer with the vector vals
// sets also the geoDin of the DataContainer
template <typename T>
void DataContainer<T>::setValues(std::vector<Element<T>> vals)
{
for (auto elem : this->values)
    {
        delete elem;
    }
    this->values.clear();
    this->values.reserve(vals.size());
    for (const Element<T> &elem : vals)
    {
        values.push_back(new Element<T>(elem));
    }
    this->geoDim = vals.at(0).getGeomDim();
}

// sets the dimensions of elements of a DataContainer
template <typename T>
void DataContainer<T>::setGeoDim(int gDim)
{
    this->geoDim = gDim;
    for (Element<T> *elem : this->values)
    {
        (*elem).setGeomDim(gDim);
    }
}

template class DataContainer<int>;
template class DataContainer<double>;
