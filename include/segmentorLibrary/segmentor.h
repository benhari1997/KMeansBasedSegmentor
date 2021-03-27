#pragma once
#include <vector>
#include "dataLibrary/element.h"
#include "dataLibrary/pixel.h"
#include "dataLibrary/dataContainer.h"
#include "segmentorLibrary/cluster.h"

template <typename T>
class Segmentor
{
protected:
    int cycles;
    int K;
    int max_it;
    double epsilon;
    double totalWCSS;
    int num_iter;

    void setNum_iter(int count);

public:
    Segmentor();
    Segmentor(int t, int k, int i, double e);

    void setK(int k);
    void setCycles(int c);
    void setMax_it(int m);
    void setTotalWCSS(double w);

    int getK() const;
    int getCycles() const;
    int getMax_it() const;
    int getNum_iter() const;
    virtual double getEpsilon() const;
    double getTotalWCSS() const;

};