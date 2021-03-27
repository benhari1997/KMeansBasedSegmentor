#include <vector>
#include "dataLibrary/element.h"
#include "dataLibrary/pixel.h"
#include "dataLibrary/imageContainer.h"
#include "segmentorLibrary/cluster.h"
#include "segmentorLibrary/segmentor.h"
#include "segmentorLibrary/segmentorPixel.h"
#include <cmath>
#include <algorithm>

#include "opencv2/highgui.hpp"
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"

template <typename T>
SegmentorPixel<T>::SegmentorPixel(int t, int k, int i, double e) : Segmentor<T>(t, k, i, e),
                                                                   Clusters{},
                                                                   OptimalCentroids{}

{
    this->Clusters.reserve(k);
    for (int y = 0; y < k; y++)
    {
        this->Clusters.push_back(new ClusterPixel<T>());
    }
    this->OptimalCentroids.reserve(this->K);
}

template <typename T>
SegmentorPixel<T>::SegmentorPixel() : Segmentor<T>(),
                                      Clusters{},
                                      OptimalCentroids{}
{
    this->OptimalCentroids.reserve(1);
    this->Clusters.reserve(1);
    this->Clusters.push_back(new ClusterPixel<T>());
}

template <typename T>
SegmentorPixel<T>::~SegmentorPixel()
{
    for (ClusterPixel<T> *clust : this->Clusters)
    {
        delete clust;
    }
}

template <typename T>
std::vector<ClusterPixel<T>> SegmentorPixel<T>::getClusters(void) const
{
    std::vector<ClusterPixel<T>> result;
    result.reserve(this->getK());
    for (int n = 0; n < (this->getK()); n++)
    {
        result.push_back(*(this->Clusters[n]));
    }
    return (result);
}

template <typename T>
void SegmentorPixel<T>::setEpsilon(const ImageContainer<T> *data, double percentage)
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
void SegmentorPixel<T>::initCentRand(const ImageContainer<T> *data)
{
    int datasize = data->getValues().size();
    std::vector<Pixel<T>> centrands;
    centrands.reserve(this->K);

    Pixel<T> tempC = data->at(rand() % datasize);
    centrands.push_back(tempC);

    bool diff = true;
    int t = 0;

    for (int i = 1; i < this->K; i++)
    {
        do
        {
            t += 1;
            diff = true;
            tempC = data->at(rand() % datasize);
            for (Pixel<T> pxl : centrands)
            {
                diff = diff && (!(pxl == tempC));
            }
        } while ((!diff) && t < 20);

        centrands.push_back(tempC);
        this->Clusters.at(i)->setCentroid(&tempC);
    }
}

template <typename T>
void SegmentorPixel<T>::fitKmeans(const ImageContainer<T> *data)
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

    Pixel<T> referencePixel(data->at(0));

    std::vector<int> TempCount(this->K, 0);
    std::vector<int> TempMapping(datasize, 0);

    int closeIndex = 0;

    while (dispmax >= (this->epsilon) && count < (this->max_it))
    {
        for (int k = 0; k < this->K; k++)
        {
            (this->Clusters[k])->clearClustElements();
            (this->Clusters[k])->setWeights(referencePixel.getWeights());
        }
        std::fill(TempCount.begin(), TempCount.end(), 0);
        std::fill(TempMapping.begin(), TempMapping.end(), 0);

        for (int j = 0; j < datasize; j++)
        {
            min = data->at(j).distTo((this->Clusters[0])->getCentroid());
            closeIndex = 0;
            for (int z = 1; z < (this->K); z++)
            {
                dist = (data->at(j)).distTo((this->Clusters[z])->getCentroid());
                if (dist < min)
                {
                    closeIndex = z;
                    min = dist;
                }
            }
            TempCount[closeIndex] += 1;
            TempMapping[j] = closeIndex;
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
void SegmentorPixel<T>::optimisedWCSS(const ImageContainer<T> *data, int c)
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
std::vector<Pixel<T>> SegmentorPixel<T>::getOptimalCentroids(void) const
{
    return OptimalCentroids;
}

template <typename T>
void SegmentorPixel<T>::generateSegmentation(const ImageContainer<T> *data)
{
    int height = data->getHeight();
    int width = data->getWidth();
    int channels = data->at(0).getColDim();

    Mat image;
    int i = 0;
    int j = 0;
    Pixel<T> centroid;
    if (channels == 3)
    {
        image = (height, width, CV_8UC3, Scalar(0, 0, 0));
        for (ClusterPixel<T> *clust : this->Clusters)
        {
            centroid = clust->getCentroid();
            for (Pixel<T> &pxl : clust->getClustElements())
            {
                i = pxl.getCoord().at(0);
                j = pxl.getCoord().at(1);
                Vec3b &color = image.at<Vec3b>(i, j);
                for (int c = 0; c < 3; c++)
                {
                    color[c] = centroid.getColors().at(c);
                }
            }
        }
    }
    if (channels == 1)
    {
        image = (height, width, CV_8UC1, Scalar(0));
        for (ClusterPixel<T> *clust : this->Clusters)
        {
            centroid = clust->getCentroid();
            for (Pixel<T> &pxl : clust->getClustElements())
            {
                i = pxl.getCoord().at(0);
                j = pxl.getCoord().at(1);
                uchar &color = image.at<uchar>(i, j);
                color = static_cast<uchar>(centroid.getColors().at(0));
            }
        }
    }

    imwrite("segmentedImage.jpg", image);
}

template class SegmentorPixel<int>;