#include <vector>
#include "dataLibrary/element.h"
#include "dataLibrary/pixel.h"
#include "dataLibrary/dataContainer.h"
#include "segmentorLibrary/cluster.h"
#include "segmentorLibrary/segmentor.h"
#include <cmath>
#include <algorithm>
#include <map>
#include <array>


template <typename T>
Segmentor<T>::Segmentor(int t, int k, int i, double e) : cycles{t},
                                                         K{k},
                                                         max_it{i},
                                                         epsilon{e},
                                                         totalWCSS{0},
                                                         num_iter{0}

{
}

template <typename T>
Segmentor<T>::Segmentor() : cycles{1},
                            K{1},
                            max_it{50},
                            epsilon{1E-2},
                            totalWCSS{0},
                            num_iter{0}
{
}


template <typename T>
int Segmentor<T>::getK() const
{
    return (this->K);
}

template <typename T>
int Segmentor<T>::getCycles() const
{
    return (this->cycles);
}

template <typename T>
int Segmentor<T>::getNum_iter() const
{
    return (this->num_iter);
}

template <typename T>
int Segmentor<T>::getMax_it() const
{
    return (this->max_it);
}

template <typename T>
double Segmentor<T>::getEpsilon() const
{
    return (this->epsilon);
}

template <typename T>
double Segmentor<T>::getTotalWCSS() const
{
    return (this->totalWCSS);
}

template <typename T>
void Segmentor<T>::setK(int k)
{
    this->K = k;
}
template <typename T>
void Segmentor<T>::setCycles(int c)
{
    this->cycles = c;
}

template <typename T>
void Segmentor<T>::setMax_it(int i)
{
    this->max_it = i;
}

template <typename T>
void Segmentor<T>::setNum_iter(int count)
{
    this->num_iter = count;
}

template <typename T>
void Segmentor<T>::setTotalWCSS(double w)
{
    this->totalWCSS = w;
}

// Destructor

template class Segmentor<int>;
template class Segmentor<double>;
