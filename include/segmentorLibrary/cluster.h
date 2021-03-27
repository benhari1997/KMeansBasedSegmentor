#pragma once
#include <vector>
#include "dataLibrary/element.h"
#include "dataLibrary/pixel.h"
#include "dataLibrary/dataContainer.h"

template <typename T>
class Cluster
{
protected:
    int nbElements{0}; // the nb of elements for the cluster
    int clustId;       //id of the cluster
    double wcss{0};    //Within-Cluster-Sum-of-Squares : distances of Elements within a cluster to their centroid.

    bool clusterUpToDate{false}; //a boolean that indicates whether the centroid is actually
                                 //the barycentre of the the cluster elements or not
                                 //It's usefull to make sure we're computing the right wcss
                                 //and that our cluster's data is synchronized

    static std::vector<int> clustersIds;

public:
    //Constructors & Destructors
    Cluster(){};                     //No args
    Cluster(int id, int nbElements); //No args

    Cluster(const Cluster<T> &source);
    Cluster(Cluster<T> &&source);

    //Destructor
    virtual ~Cluster();

    //Getters

    int getNbElements() const;
    int getClustId() const;
    double getWCSS() const;

    bool isClusterUpToDate() const; //Whether the centroid is actually the barycentre of elements

    static std::vector<int> getClustersIds();

    // init randomly the centroids of the cluster from an element of the datacontainer
    // void initCentroid(DataContainer<T> &); //Should be inside segmentor

    //Update Cluster centroid based on the actual Cluster elements
    //Returns the displacement of the centroid (euclidean distance between old and new 'position')
    virtual double updateCentroid() = 0;

    //Compute & Update the cluster's WCSS
    virtual void updateWCSS() = 0;

    virtual void clearClustElements() = 0; //empty the vector clustElements (can only be called from inside)

    //Reserve a given size of clustElements
    virtual void allocateClustElements(int size) = 0;
};
