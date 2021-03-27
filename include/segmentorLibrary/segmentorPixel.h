#pragma once
#include <vector>
#include "dataLibrary/pixel.h"
#include "dataLibrary/imageContainer.h"
#include "segmentorLibrary/cluster.h"
#include "segmentorLibrary/clusterPixel.h"
#include "segmentorLibrary/segmentor.h"

template <typename T>
class SegmentorPixel : public Segmentor<T>
{
private:
    std::vector<ClusterPixel<T> *> Clusters;
    std::vector<Pixel<T>> OptimalCentroids;

public:
    SegmentorPixel();
    SegmentorPixel(int t, int k, int i, double e);
    ~SegmentorPixel();

    std::vector<ClusterPixel<T>> getClusters(void) const;

    void setEpsilon(const ImageContainer<T> *data, double percentage);

    void fitKmeans(const ImageContainer<T> *data);
    void optimisedWCSS(const ImageContainer<T> *data, int c);
    std::vector<Pixel<T>> getOptimalCentroids(void) const;
    void initCentRand(const ImageContainer<T> *data);

    void generateSegmentation(const ImageContainer<T> *data); //pass imagecontainer?

    // yujie's optimal function should be here void InitCent(ImageContainer<T> &data);
};