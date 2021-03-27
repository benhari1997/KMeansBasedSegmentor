#include <vector>
#include "dataLibrary/element.h"
#include "dataLibrary/pixel.h"
#include "segmentorLibrary/cluster.h"
#include "segmentorLibrary/clusterPixel.h"

#include <cmath>

template <typename T>
ClusterPixel<T>::ClusterPixel(std::vector<Pixel<T> *> elements,
                              Pixel<T> *centroid) : Cluster<T>(static_cast<int>(ClusterPixel<T>::clustersIds.size()),
                                                               static_cast<int>(elements.size())),
                                                    clustElements{}

{
    this->clustElements.reserve(this->nbElements);
    for (Pixel<T> *elem : elements)
    {
        this->clustElements.push_back(new Pixel<T>(*elem));
    }
    this->centroid = new Pixel<T>(*centroid);
}
template <typename T>
ClusterPixel<T>::ClusterPixel() : Cluster<T>(static_cast<int>(ClusterPixel<T>::clustersIds.size()),
                                             0),
                                  clustElements{}
{
    this->centroid = new Pixel<T>();
}

template <typename T>
ClusterPixel<T>::ClusterPixel(Pixel<T> *centroid) : Cluster<T>(static_cast<int>(ClusterPixel<T>::clustersIds.size()),
                                                               0),
                                                    clustElements{}

{
    this->centroid = new Pixel<T>(*centroid);
}

//Copy Constructor
template <typename T>
ClusterPixel<T>::ClusterPixel(const ClusterPixel<T> &source) : Cluster<T>(source)
{
    this->centroid = new Pixel<T>(source.getCentroid());
    this->clustElements.reserve(source.getNbElements());
    for (const Pixel<T> &elem : source.getClustElements())
    {
        this->clustElements.push_back(new Pixel<T>(elem));
    }
}

//Move Constructor
template <typename T>
ClusterPixel<T>::ClusterPixel(ClusterPixel<T> &&source) : Cluster<T>(std::move(source))
{
    this->clustElements = std::move(source.clustElements);
    this->centroid = source.centroid;
    source.centroid = nullptr;
}

//Destructor
template <typename T>
ClusterPixel<T>::~ClusterPixel()
{
    delete this->centroid;
    for (Pixel<T> *elem : this->clustElements)
    {
        delete elem;
    }
    this->clustElements.clear();
}

//Getters
template <typename T>
Pixel<T> ClusterPixel<T>::getCentroid() const
{
    return (*(this->centroid));
}

template <typename T>
std::vector<Pixel<T>> ClusterPixel<T>::getClustElements(void) const
{
    std::vector<Pixel<T>> result;
    result.reserve(this->nbElements);
    for (Pixel<T> *elem : this->clustElements)
    {
        result.push_back(*elem);
    }
    return (result);
}

//Setters
template <typename T>
void ClusterPixel<T>::setCentroid(Pixel<T> *centr)
{
    this->centroid = new Pixel<T>(*centr);
    this->clusterUpToDate = false;
}

template <typename T>
void ClusterPixel<T>::setClustElements(std::vector<Pixel<T> *> elements)
{
    this->clearClustElements();
    this->clustElements.reserve(elements.size());
    for (Pixel<T> *elem : elements)
    {
        this->clustElements.push_back(new Pixel<T>(*elem));
    }
    this->nbElements = static_cast<int>(elements.size());
    this->clusterUpToDate = false;
}

template <typename T>
void ClusterPixel<T>::setWeights(std::vector<double> w)
{
    this->centroid->setWeights(w);
    for (Pixel<T> *pxl : this->clustElements)
    {
        pxl->setWeights(w);
    }
}

//KMEANS RELATED METHODS

template <typename T>
void ClusterPixel<T>::clearClustElements()
{
    for (Pixel<T> *elem : this->clustElements)
    {
        delete elem;
    }
    this->clustElements.clear();
    this->clusterUpToDate = false;
}

template <typename T>
double ClusterPixel<T>::updateCentroid()
{
    Pixel<T> oldCentroid = *(this->centroid);
    Pixel<T> tempSumElements(*(this->centroid));
    tempSumElements.setToZero();

    for (const Pixel<T> *elem : this->clustElements)
    {
        tempSumElements = tempSumElements + *elem;
    }

    tempSumElements = tempSumElements / (this->nbElements);
    //Computing the barycenter & the difference with the old one
    double displacement = 0;
    (*(this->centroid)) = tempSumElements;
    displacement = (*(this->centroid)).distTo(oldCentroid);

    this->clusterUpToDate = true;
    return (displacement);
}

template <typename T>
void ClusterPixel<T>::updateWCSS()
{
    if (this->clusterUpToDate)
    {
        double var = 0;
        for (Pixel<T> *elem : this->clustElements)
        {
            var += std::pow((*elem).distTo(*(this->centroid)), 2);
        }
        this->wcss = var;
    }
    else
    {
        this->wcss = -1;
        std::cout << "WCSS couldn't get updated, the centroid doesn't match the actual barycentre" << std::endl;
    }
}

//Adding a new element to clustElements
template <typename T>
void ClusterPixel<T>::addElement(Pixel<T> elem)
{
    this->clustElements.push_back(new Pixel<T>(elem));
    this->clusterUpToDate = false;
}

template <typename T>
void ClusterPixel<T>::allocateClustElements(int size)
{
    this->nbElements = size;
    this->clustElements.reserve(size);
}

template class ClusterPixel<int>;
template class ClusterPixel<double>;