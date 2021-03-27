#pragma once
#include <vector>
#include "dataLibrary/pixel.h"
#include "dataLibrary/dataContainer.h"
#include "cluster.h"

template <typename T>
class ClusterPixel : public Cluster<T>
{
private:
    Pixel<T> *centroid{nullptr};           // contain a pointer to a centroid element
    std::vector<Pixel<T> *> clustElements; // vector of pointers to the elements of the cluster (elements around the centroid)

public:
    //Constructors & Destructors
    ClusterPixel();                                    //No args
    ClusterPixel(Pixel<T> *centroid);                  //centroid
    ClusterPixel(std::vector<Pixel<T> *>, Pixel<T> *); //cluster elements + centroid

    //Copy & Move
    ClusterPixel(const ClusterPixel<T> &source);
    ClusterPixel(ClusterPixel<T> &&source);

    //Destructor
    virtual ~ClusterPixel();

    //Getters
    Pixel<T> getCentroid() const;
    std::vector<Pixel<T>> getClustElements(void) const;

    //Setters
    void setCentroid(Pixel<T> *);
    void setClustElements(std::vector<Pixel<T> *>);
    void setWeights(std::vector<double>);

    //Update Cluster centroid based on the actual Cluster elements
    //Returns the displacement of the centroid (euclidean distance between old and new 'position')
    double updateCentroid() override;

    //Compute & Update the cluster's WCSS
    void updateWCSS() override;

    virtual void clearClustElements() override; //empty the vector clustElements (can only be called from inside)

    //Reserve a given size of clustElements
    void allocateClustElements(int size) override;

    //Adding a new element to clustElements
    void addElement(Pixel<T>);
};
