#include <vector>
#include "dataLibrary/element.h"
#include "dataLibrary/pixel.h"
#include "dataLibrary/dataContainer.h"
#include "segmentorLibrary/cluster.h"
#include "segmentorLibrary/segmentor.h"
#include "segmentorLibrary/segmentorElement.h"

#include <cmath>
#include <algorithm>
#include <string>
#include <fstream>

template <typename T>
SegmentorElement<T>::SegmentorElement(int t, int k, int i, double e) : Segmentor<T>(t, k, i, e),
                                                                       Clusters{},
                                                                       OptimalCentroids{}

{
    this->Clusters.reserve(k);
    for (int y = 0; y < k; y++)
    {
        this->Clusters.push_back(new ClusterElement<T>());
    }
    this->OptimalCentroids.reserve(this->K);
}

template <typename T>
SegmentorElement<T>::SegmentorElement() : Segmentor<T>(),
                                          Clusters{},
                                          OptimalCentroids{}
{
    this->OptimalCentroids.reserve(1);
    this->Clusters.reserve(1);
    this->Clusters.push_back(new ClusterElement<T>());
}

template <typename T>
SegmentorElement<T>::~SegmentorElement()
{
    for (ClusterElement<T> *clust : this->Clusters)
    {
        delete clust;
    }
}

template <typename T>
std::vector<ClusterElement<T>> SegmentorElement<T>::getClusters(void) const
{
    std::vector<ClusterElement<T>> result;
    result.reserve(this->getK());
    for (int n = 0; n < (this->getK()); n++)
    {
        result.push_back(*(this->Clusters[n]));
    }
    return (result);
}

template <typename T>
void SegmentorElement<T>::setEpsilon(const DataContainer<T> *data, double percentage)
{
    double max = 0;
    double tempmax = 0;
    int datasize = data->getValues().size();
    for (int j = 0; j < datasize - 1; j++)
    {
        for (int i = j + 1; i < datasize; i++)
        {
            tempmax = data->at(j).distTo(data->at(i));
        }
        if (tempmax > max)
        {
            max = tempmax;
        }
    }
    this->epsilon = percentage * max;
}

// initialising centroids by picking elements of the data container with a random index.
//We keep in mind that the indexes already chosen prior to each iteration will not be generated randomly again. This will be confirmed by storing the indexes values in a centrands vector.
template <typename T>
void SegmentorElement<T>::initCentRand(const DataContainer<T> *data)
{
    int datasize = data->getValues().size();
    std::vector<int> centrands{0};
    Element<T> tempC;
    for (int i = 0; i < this->K; i++)
    {
        tempC = data->at(i);
        this->Clusters.at(i)->setCentroid(&tempC);
    }
}

template <typename T>
void SegmentorElement<T>::fitKmeans(const DataContainer<T> *data)
{
    //  for (int u=0, u<this->K, u++)
    // {*(this->Clusters[u])();}
    initCentRand(data);
    double dist;
    double min;
    double dispmax = this->epsilon;
    double disp;
    int datasize = data->getValues().size();
    int count = 0;
    while (dispmax >= (this->epsilon) && count < (this->max_it))
    {
        for (int k = 0; k < this->K; k++)
        {
            (this->Clusters[k])->clearClustElements();
        }
        std::vector<int> TempCount(this->K, 0);
        std::vector<int> TempMapping(datasize, 0);

        for (int j = 0; j < datasize; j++)
        {
            min = data->at(j).distTo((this->Clusters[0])->getCentroid());
            int temp = 0;
            for (int z = 1; z < (this->K); z++)
            {
                dist = (data->at(j)).distTo((this->Clusters[z])->getCentroid());
                if (dist < min)
                {
                    temp = z;
                    min = dist;
                }
            }
            TempCount[temp] += 1;
            TempMapping[j] = temp;
        }
        //reserveCapacity to be implemented in cluster
        for (int i = 0; i < this->K; i++)
        {
            (this->Clusters[i])->allocateClustElements(TempCount[i]);
        }

        for (int j = 0; j < datasize; j++)
        {
            (this->Clusters[TempMapping[j]])->addElement(data->at(j));
        }
        dispmax = 0;
        for (int k = 0; k < this->K; k++)
        {
            disp = (this->Clusters[k])->updateCentroid();
            if (disp > dispmax)
            {
                dispmax = disp;
            }
        }
        count++;
    }
    this->setNum_iter(count);
    for (int r = 0; r < this->K; r++)
    {
        this->OptimalCentroids.push_back((this->Clusters[r])->getCentroid());
    }
    //here we need to get WCSS of the last set of clusters and return it to test in cycles version
    this->setTotalWCSS(0);
    double TWCSS = 0;
    for (int k = 0; k < this->K; k++)
    {
        (this->Clusters[k])->updateWCSS();
        TWCSS += (this->Clusters[k])->getWCSS();
    }
    this->setTotalWCSS(TWCSS);
}

template <typename T>
void SegmentorElement<T>::optimisedWCSS(const DataContainer<T> *data, int c)
{
    this->OptimalCentroids.reserve(this->K);
    this->setTotalWCSS(0);
    this->setCycles(c);
    this->fitKmeans(data);
    double minWCSS = this->getTotalWCSS();
    for (int r = 0; r < this->K; r++)
    {
        this->OptimalCentroids.push_back((this->Clusters[r])->getCentroid());
    }

    for (int i = 1; i < this->getCycles(); i++)
    {
        if (this->getTotalWCSS() < minWCSS)
        {
            this->fitKmeans(data);
            minWCSS = this->getTotalWCSS();
            for (int r = 0; r < this->K; r++)
            {
                this->OptimalCentroids.push_back((this->Clusters[r])->getCentroid());
            }
        }
    }
}

template <typename T>
std::vector<Element<T>> SegmentorElement<T>::getOptimalCentroids(void) const
{
    return OptimalCentroids;
}

//method that generate a csv file with the segmented data
template <typename T>
void SegmentorElement<T>::generateSegmentation()
{
    std::ofstream myfile; // declare a variable that contain an output stream to a file

    myfile.open("DataSegmentation.csv"); //open a file or (create it if not) it will store the result

    std::string line; // variable string that will print the informations in the file

    int dimension = this->Clusters.at(0)->getCentroid().getGeomDim();
    //header line
    for (int i = 0; i < dimension; i++)
    {
        line += "coord ";
        line += std::to_string(i + 1);
        line += ",";
    }
    line += "clusterId,";
    for (int i = 0; i < dimension - 1; i++)
    {
        line += "centrCoord ";
        line += std::to_string(i + 1);
        line += ",";
    }
    line += "centrCoord ";
    line += std::to_string(dimension);

    line += "\n";

    myfile << line; // put the header line in the file

    ClusterElement<T> *clust;
    std::vector<T> coords;
    bool writeCentroid;
    // loop over the clusters to get the elements
    for (int i = 0; i < this->K; i++)
    {
        writeCentroid = true;
        clust = this->Clusters.at(i);
        //loop over each element of the current cluster
        line.clear(); // clear the line variable
        //loop over the element coordiantes
        for (const Element<T> &elem : clust->getClustElements())
        {
            //fill the line variable with the element coordinates
            coords = elem.getCoord();
            for (T &coord : coords)
            {
                line += std::to_string(coord);
                line += ",";
            }
            //add the cluster id
            line += std::to_string(i);
            line += ",";

            if (writeCentroid)
            {
                coords = clust->getCentroid().getCoord();
                for (int i = 0; i < coords.size() - 1; i ++)
                {
                    line += std::to_string(coords[i]);
                    line += ",";
                }
                line += std::to_string(coords[coords.size() - 1]);
                writeCentroid = false;
            }

            line += "\n";
        }

        //put it in the file
        myfile << line;
    }
    //close the file
    myfile.close();
}

template class SegmentorElement<int>;
template class SegmentorElement<double>;
