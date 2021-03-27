#pragma once
#include "dataLibrary/dataContainer.h"
#include "generatorLibrary/prng.h"
#include <iostream>
/*
** DataGenerator is the class responsible for producing randomized data
** containers <Class: DataContainer> of elements <Class: Element<T>>
** according to the specified dimensionality, and using the specified
** pseudo-random number generator
*/
template <typename T>
class DataGenerator {
private:
    PrNG& prng;
    DataGenerator(PrNG&);

public:
    // pseudo-constructor
    static DataGenerator<T>& getInstance(PrNG&);

    // forbidden constructors
    DataGenerator(const DataGenerator<T>&) = delete;
    DataGenerator(DataGenerator<T>&&) = delete;
    DataGenerator<T>& operator=(const DataGenerator<T>&) = delete;
    DataGenerator<T>& operator=(const DataGenerator<T>&&) = delete;

    // function to generate a randomly
    // filled data container
    // input:
    // 	- (long) number of elements
    // 	  to generate
    // 	- (int) dimensionality of
    // 	  generated elements
    // 	- (std::ofstream) <optional>
    // 	  file outstream to save in the
    // 	  generated data matrix
    DataContainer<T> generate(
        long nb,
        int dim,
        std::ostream& file = std::cout);
};

