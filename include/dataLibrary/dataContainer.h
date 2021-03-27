#pragma once
#include <vector>
#include "element.h"
/*
DataContainer<T> is a simple template class that serves as a 1D container of Elements<T> templated 
with type T. (T will thus refer to the type of the coordinates in our project)
*/
template <typename T>
class DataContainer
{
private:
    // The set of elements inside a DataContainer
    std::vector<Element<T> *> values;
    // The dimension of elements of a DataContainer
    int geoDim;

public:
    //Constructors & Destructor
    DataContainer();
    DataContainer(int size, int dim);
    DataContainer(int size, int dim, bool randomGen);
    DataContainer(std::vector<Element<T> *> vals); //Risky, needs deep copy for more robust implementation
    DataContainer(std::vector<Element<T>> &vals);

    DataContainer(const DataContainer<T> &source);
    DataContainer(DataContainer<T> &&source);

    ~DataContainer();

    //Getters & Setters
    Element<T> at(int i) const;
    std::vector<Element<T>> getValues() const;
    int getGeoDim() const;

    void setAt(int index, Element<T> &val);
    void setValues(std::vector<Element<T>> vals);
    void fillValues(int size, int dim);
    void setGeoDim(int gDim);
};
