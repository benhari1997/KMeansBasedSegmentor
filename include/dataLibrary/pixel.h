#pragma once
#include <iostream>
#include "element.h"

/*
+ Pixel is a derived Element class since it's also an element in a geometrical space. What is 
new in Pixel is that it has set of colour values too, also stored in a separate vector.
For instance, a 1D colour vector can be thought of as a grayscale pixel, while a 3D one can 
map to an RGB pixel.
+ Pixel class can also be templated based on the colors values. For instance, photos may vary
depending on their 'bit depth' which is the equivalent of numerical FP precision, (8bits, 10bits etc..)
Some color spaces can even be continuous and thus require a double representation.
+ Pixel doesn't have to be templated based on the coordinates type (as the Parent class Element)
since basically all images (raster) are represented on discrete space, and thus use ultimately
integers for their coordinates.
*/
template <typename I>
class Pixel : public Element<int>
{

private:
    int colDim;            //Colour dimensions (1D : GS, 3D : RGB etc..)
    std::vector<I> colors; //Colours values (stored within an int vector of dimension 'colDim')
                           //to be used in the norm & distTo methods

    //Pixels count
    static unsigned int nPixels;

public:
    //Constructors
    Pixel();
    Pixel(std::vector<int> coords, std::vector<I> colors);                           //Values based constr
    Pixel(std::vector<int> coords, std::vector<I> colors, std::vector<double> weights); //Values based constr + weights
    explicit Pixel(int colDim);                                                      //colors dimensions based constr
    Pixel(const Pixel<I> &source);
    Pixel(Pixel<I> &&source);

    //Destructor
    virtual ~Pixel();

    //Getters
    std::vector<I> getColors() const;
    int getColDim() const;

    static unsigned int getNPixels();

    //Setters
    void setCols(std::vector<I> cols);
    void setColDims(int dim);

    virtual void setToZero() override;

    //Other member methods
    double distTo(const Pixel<I> &pxl);                    //Distance to other pixel(spatial distance)
    virtual double norm() override;                        // distance to the origin
    int argClosest(const std::vector<Pixel<I> *> &pixels); //index of the closest pixel in the vector passed

    //Arithmetic Operators Overloading
    Pixel<I> operator+(const Pixel<I> &source);
    Pixel<I> operator-(const Pixel<I> &source);
    Pixel<I> &operator=(const Pixel<I> &source);
    Pixel<I> &operator=(Pixel<I> &&source);
    Pixel<I> operator/(int);

    //Stream Operators
    template <typename T>
    friend std::ostream &operator<<(std::ostream &os, const Pixel<T> &elem);

    template <typename T>
    friend bool operator==(const Pixel<T> &lhs, const Pixel<T> &rhs);
};
