#pragma once
#include <vector>
#include "dataLibrary/element.h"
#include "dataLibrary/dataContainer.h"
#include "cluster.h"

template <typename T>
class ClusterElement : public Cluster<T>
{
private:
    Element<T> *centroid{nullptr};           // contain a pointer to a centroid element
    std::vector<Element<T> *> clustElements; // vector of pointers to the elements of the cluster (elements around the centroid)

public:
    //Constructors & Destructors
    ClusterElement();                                        //No args
    ClusterElement(Element<T> *centroid);                    //centroid
    ClusterElement(std::vector<Element<T> *>, Element<T> *); //cluster elements + centroid

    //Copy & Move
    ClusterElement(const ClusterElement<T> &source);
    ClusterElement(ClusterElement<T> &&source);

    //Destructor
    virtual ~ClusterElement();

    //Getters
    Element<T> getCentroid() const;
    std::vector<Element<T>> getClustElements(void) const;

    //Setters
    void setCentroid(Element<T> *);
    void setClustElements(std::vector<Element<T> *>);

    //Update Cluster centroid based on the actual Cluster elements
    //Returns the displacement of the centroid (euclidean distance between old and new 'position')
    double updateCentroid() override;

    //Compute & Update the cluster's WCSS
    void updateWCSS() override;

    virtual void clearClustElements() override; //empty the vector clustElements (can only be called from inside)

    //Reserve a given size of clustElements
    void allocateClustElements(int size) override;

    //Adding a new element to clustElements
    void addElement(Element<T> );
};
