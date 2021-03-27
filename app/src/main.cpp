#include <fstream>
#include <iostream>
#include "generatorLibrary/dataGenerator.h"
#include "generatorLibrary/mersTwistNG.h"
#include "generatorLibrary/prng.h"
#include "segmentorLibrary/segmentorElement.h"
#include "dataLibrary/imageContainer.h"
#include "segmentorLibrary/segmentorPixel.h"
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>

static void show_usage(std::string name)
{
    std::cerr << "Usage: " << name << " (-g csv_path | -i img_path) [PARAMS]"
              << "PARAMETERS:\n"
              << "\t-h,--help\t\t\tShow this help message\n"
              << "\t-g,--generate CSV_FILE_NAME\tSpecify the randomly generated output file path to save\n"
              << "\t-i,--img IMG_FILE_NAME\t\tSpecify the input image to use for the segmentation\n"
              << "\t-m,--max MAX_ITERATIONS\t\tSpecify the maximum number of iterations for k-means\n"
              << "\t-K,--nclusters CLUSTERS_NUMBER\tSpecify the number of final segmented clusters\n"
              << std::endl;
}

using namespace cv;
int main(int argc, char **argv)
{
    std::cout << "----KMEANS IMAGE SEGMENTATION-----" << std::endl;

    // Ensure parameters formatting
    std::string placeholder = "";
    if (argc > 1)
        placeholder = argv[1];
    if (argc == 2 && (placeholder == "-h" || placeholder == "-help"))
    {
        show_usage(argv[0]);
        return 0;
    }
    else if (argc != 7)
    {
        show_usage(argv[0]);
        return 1;
    }

    // Parse usage mode
    bool random = true;
    std::string img_in;
    std::string csv_out;
    int max_iter = 10;
    int k = 3;
    if (placeholder == "-i" || placeholder == "--img")
    {
        random = false;
        img_in = argv[2];
    }
    else if (placeholder == "-g" || placeholder == "--generate")
    {
        csv_out = argv[2];
    }
    else
    {
        show_usage(argv[0]);
        return 1;
    }

    // Parse parameters
    for (int i = 3; i < 7; i += 2)
    {
        placeholder = argv[i];
        if (placeholder == "-m" || placeholder == "--max")
        {
            placeholder = argv[i + 1];
            max_iter = std::stoi(placeholder);
        }
        else if (placeholder == "-K" || placeholder == "--nclusters")
        {
            placeholder = argv[i + 1];
            k = std::stoi(placeholder);
        }
    }

    if (random)
    {
        PrNG *merstwist = new MersTwistNG(5, 521367821);
        DataGenerator<double> &generator = DataGenerator<double>::getInstance(*merstwist);
        std::ofstream generated_file;
        generated_file.open(csv_out);
        DataContainer<double> container = generator.generate(1000, 5, generated_file);
        generated_file.close();
        SegmentorElement<double> segmentor(1, k, max_iter, 0.0001);
        segmentor.fitKmeans(&container);
        segmentor.generateSegmentation();
    }
    else
    {
        Mat image = imread(img_in);
        const ImageContainer<int> *imageCont = new ImageContainer<int>(image);
        SegmentorPixel<int> imageSeg(1, k, max_iter, 0.001);
        imageSeg.fitKmeans(imageCont);
        imageSeg.generateSegmentation(imageCont);
    }

    return 0;
}
