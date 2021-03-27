#pragma once
#include <vector>
#include "dataLibrary/element.h"
#include "dataLibrary/pixel.h"
#include "dataLibrary/dataContainer.h"
#include "segmentorLibrary/cluster.h"
#include "segmentorLibrary/clusterElement.h"
#include "segmentorLibrary/segmentor.h"

template <typename T>
class SegmentorElement : public Segmentor<T>
{
private:
    std::vector<ClusterElement<T> *> Clusters;
    std::vector<Element<T>> OptimalCentroids;

public:
    SegmentorElement();
    SegmentorElement(int t, int k, int i, double e);
    ~SegmentorElement();

    std::vector<ClusterElement<T>> getClusters(void) const;

    void setEpsilon(const DataContainer<T> *data, double percentage);

    void fitKmeans(const DataContainer<T> *data);
    void optimisedWCSS(const DataContainer<T> *data, int c);
    std::vector<Element<T>> getOptimalCentroids(void) const;
    void initCentRand(const DataContainer<T> *data);

    void generateSegmentation();
    // yujie's optimal function should be here void InitCent(DataContainer<T> &data);
};
