#pragma once
#include <iostream>
#include <vector>

/*
+ Element is the most abstract atomic class. It represents a point in any X-dimensional space
using its GEOMETRICAL Coordinates that we store inside a vector.
+ The Element class can be templated based on the coordinates data type.
For instance, in a continuous RxR space, a point is represented using doubles while in a raster 
(bitmap image) it can be represented in a discrete space of Integer.
*/
template <typename T>
class Element
{
protected:
    std::vector<T> coordinates;
    int geomDim; //Spatial Dimension (1,2 or 3D)
    //Elements count (static)
    static unsigned int nElements;
    std::vector<double> weights{1}; //vector of doubles holding the weights of the different dimensions

public:
    //Constructors
    Element();
    Element(int geomDim); //geometric dimension (1, 2 or 3)
    explicit Element(std::vector<T> coords);

    Element(const Element<T> &source);
    Element(Element<T> &&source);
    //Destructor
    virtual ~Element();

    //Getters
    std::vector<T> getCoord() const;
    int getGeomDim() const;
    std::vector<double> getWeights() const;

    static unsigned int getNElem();

    //setters 
    void setCoord(std::vector<T> vect);
    void setGeomDim(int dim);
    void setWeights(const std::vector<double> &W);

    virtual void setToZero();

    //Other member methods
    double distTo(const Element<T> &elem);                     //Distance to other element
    int argClosest(const std::vector<Element<T> *> &elements); //index of the closest element in the vector passed
    virtual double norm();                                     // distance to the origin

    //Arithmetic Operators Overloading
    Element<T> operator+(const Element<T> &source);
    Element<T> operator-(const Element<T> &source);
    
    Element<T> &operator=(const Element<T> &source);

    template <typename I>
    friend Element<I> operator/(const Element<I> &elem, int d);

    //Stream Operators
    template <typename I>
    friend std::ostream &operator<<(std::ostream &os, const Element<I> &elem);

    template <typename I>
    friend bool operator==(const Element<I> &lhs, const Element<I> &rhs);
};
