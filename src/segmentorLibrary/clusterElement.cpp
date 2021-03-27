#include <vector>
#include "dataLibrary/element.h"
#include "dataLibrary/pixel.h"
#include "segmentorLibrary/cluster.h"
#include "segmentorLibrary/clusterElement.h"

#include <cmath>

template <typename T>
ClusterElement<T>::ClusterElement(std::vector<Element<T> *> elements,
                                  Element<T> *centroid) : Cluster<T>(static_cast<int>(ClusterElement<T>::clustersIds.size()),
                                                                     static_cast<int>(elements.size())),
                                                          clustElements{}

{
    this->clustElements.reserve(this->nbElements);
    for (Element<T> *elem : elements)
    {
        this->clustElements.push_back(new Element<T>(*elem));
    }
    this->centroid = new Element<T>(*centroid);
}
template <typename T>
ClusterElement<T>::ClusterElement() : Cluster<T>(static_cast<int>(ClusterElement<T>::clustersIds.size()),
                                                 0),
                                      clustElements{}
{
    this->centroid = new Element<T>();
}

template <typename T>
ClusterElement<T>::ClusterElement(Element<T> *centroid) : Cluster<T>(static_cast<int>(ClusterElement<T>::clustersIds.size()),
                                                                     0),
                                                          clustElements{}

{
    this->centroid = new Element<T>(*centroid);
}

//Copy Constructor
template <typename T>
ClusterElement<T>::ClusterElement(const ClusterElement<T> &source) : Cluster<T>(source)
{
    this->centroid = new Element<T>(source.getCentroid());
    this->clustElements.reserve(source.getNbElements());
    for (const Element<T> &elem : source.getClustElements())
    {
        this->clustElements.push_back(new Element<T>(elem));
    }
}

//Move Constructor
template <typename T>
ClusterElement<T>::ClusterElement(ClusterElement<T> &&source) : Cluster<T>(std::move(source))
{
    this->clustElements = std::move(source.clustElements);
    this->centroid = source.centroid;
    source.centroid = nullptr;
}

//Destructor
template <typename T>
ClusterElement<T>::~ClusterElement()
{
    delete this->centroid;
    for (Element<T> *elem : this->clustElements)
    {
        delete elem;
    }
    this->clustElements.clear();
}

//Getters
template <typename T>
Element<T> ClusterElement<T>::getCentroid() const
{
    return (*(this->centroid));
}

template <typename T>
std::vector<Element<T>> ClusterElement<T>::getClustElements(void) const
{
    std::vector<Element<T>> result;
    result.reserve(this->nbElements);
    for (Element<T> *elem : this->clustElements)
    {
        result.push_back(*elem);
    }
    return (result);
}

//Setters
template <typename T>
void ClusterElement<T>::setCentroid(Element<T> *centr)
{
    this->centroid = new Element<T>(*centr);
    this->clusterUpToDate = false;
}

template <typename T>
void ClusterElement<T>::setClustElements(std::vector<Element<T> *> elements)
{
    this->clearClustElements();
    this->clustElements.reserve(elements.size());
    for (Element<T> *elem : elements)
    {
        this->clustElements.push_back(new Element<T>(*elem));
    }
    this->nbElements = static_cast<int>(elements.size());
    this->clusterUpToDate = false;
}

//KMEANS RELATED METHODS

template <typename T>
void ClusterElement<T>::clearClustElements()
{
    for (Element<T> *elem : this->clustElements)
    {
        delete elem;
    }
    this->clustElements.clear();
    this->clusterUpToDate = false;
}

template <typename T>
double ClusterElement<T>::updateCentroid()
{
    Element<T> oldCentroid = *(this->centroid);
    Element<T> tempSumElements(*(this->centroid));
    tempSumElements.setToZero();

    for (const Element<T> *elem : this->clustElements)
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
void ClusterElement<T>::updateWCSS()
{
    if (this->clusterUpToDate)
    {
        double var = 0;
        for (Element<T> *elem : this->clustElements)
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
void ClusterElement<T>::addElement(Element<T> elem)
{
    this->clustElements.push_back(new Element<T>(elem));
    this->clusterUpToDate = false;
}

template <typename T>
void ClusterElement<T>::allocateClustElements(int size)
{
    this->nbElements = size;
    this->clustElements.reserve(size);
}

template class ClusterElement<int>;
template class ClusterElement<double>;