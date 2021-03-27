#include "generatorLibrary/dataGenerator.h"
#include "dataLibrary/dataContainer.h"
#include "dataLibrary/element.h"
#include "generatorLibrary/prng.h"
#include <iostream>

template <typename T>
DataGenerator<T>::DataGenerator(PrNG& _prng): prng(_prng) {}

template <typename T>
DataGenerator<T>& DataGenerator<T>::getInstance(PrNG& prng)
{
    static DataGenerator<T> instance(prng);
    return instance;
}

template <typename T>
DataContainer<T> DataGenerator<T>::generate(
	long nb,
	int dim,
	std::ostream& file)
{
    std::vector<Element<T>*> elements(nb);
    std::vector<T> coords(dim);

    // fill the container with randomly generated data
    for (int i = 0; i < nb; i++) {
        for (int j = 0; j < dim; j++)
            coords[j] = static_cast<T>(this->prng.rand());
        elements[i] = new Element<T>(coords);
    }
    if (&file != &std::cout) {
        for (Element<T>* elem : elements) {
            for (int i = 0; i < dim - 1; i++)
                file << elem->getCoord()[i] << ",";
            file << elem->getCoord()[dim - 1] << std::endl;
        }
    }
    return DataContainer<T>(elements);
}

template class DataGenerator<int>;
template class DataGenerator<double>;
