#include <vector>
#include "dataLibrary/element.h"
#include "dataLibrary/pixel.h"
#include "segmentorLibrary/cluster.h"
#include <cmath>

template <typename T>
Cluster<T>::Cluster(int id, int nbElem) : clustId{id},
                                          nbElements{nbElem}
{
    clustersIds.push_back(this->clustId);
}

template <typename T>
Cluster<T>::Cluster(const Cluster<T> &source) : wcss{source.wcss},
                                                nbElements{source.nbElements},
                                                clustId{static_cast<int>(Cluster<T>::clustersIds.size())},
                                                clusterUpToDate{source.clusterUpToDate}
{
    clustersIds.push_back(this->clustId);
}
template <typename T>
Cluster<T>::Cluster(Cluster<T> &&source) : wcss{source.wcss},
                                           nbElements{source.nbElements},
                                           clustId{static_cast<int>(Cluster<T>::clustersIds.size() - 1)},
                                           clusterUpToDate{source.clusterUpToDate}
{
    clustersIds.push_back(this->clustId);
}
template <typename T>
Cluster<T>::~Cluster()
{
    clustersIds.pop_back();
}

//Getters
template <typename T>
int Cluster<T>::getClustId() const
{
    return (this->clustId);
}

template <typename T>
double Cluster<T>::getWCSS() const
{
    return (this->wcss);
}

template <typename T>
int Cluster<T>::getNbElements() const
{
    return (this->nbElements);
}

template <typename T>
bool Cluster<T>::isClusterUpToDate() const
{
    return (this->clusterUpToDate);
}

//Static members : list of all existing clusters ids
template <typename T>
std::vector<int> Cluster<T>::clustersIds{};

template <typename T>
std::vector<int> Cluster<T>::getClustersIds()
{
    return (Cluster<T>::clustersIds);
}

template class Cluster<int>;
template class Cluster<double>;