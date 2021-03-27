#include "dataLibrary/dataContainer.h"
#include "dataLibrary/element.h"
#include "generatorLibrary/dataGenerator.h"
#include "generatorLibrary/mersTwistNG.h"
#include "generatorLibrary/prng.h"
#include <algorithm>
#include <cstdio>
#include <fstream>
#include <iostream>

#define IS_TRUE(name, x, pass, fail)                                         \
    {                                                                        \
        if (!(x))                                                            \
        {                                                                    \
            std::cerr << "\033[1;31m" << __FUNCTION__ << name << " failed"   \
                                                                 " on line " \
                      << __LINE__ << "\033[0m" << std::endl;                 \
            fail += 1;                                                       \
        }                                                                    \
        else                                                                 \
        {                                                                    \
            std::cout << name << ": \033[4;32mPASSED\033[0m." << std::endl;  \
            pass += 1;                                                       \
        }                                                                    \
    }

int main()
{
    int fail = 0;
    int pass = 0;
    std::string name = "";

    PrNG *merstwist = new MersTwistNG(5, 12345);
    DataGenerator<int> &generator = DataGenerator<int>::getInstance(*merstwist);

    name = "Generator deterministic size test";
    DataContainer<int> container = generator.generate(120, 5);
    IS_TRUE(name, container.getValues().size() == 120, pass, fail);

    name = "Generator randomization test";
    DataContainer<int> container1 = generator.generate(100, 5);
    DataContainer<int> container2 = generator.generate(100, 5);
    int similar = 0;
    for (int i = 0; i < 100; i++)
        similar += container1.getValues().at(i) == container2.getValues().at(i) ? 1 : 0;
    IS_TRUE(name, similar < 100, pass, fail);

    name = "Generator coords length consistency test";
    DataContainer<int> container3 = generator.generate(100, 5);
    int count = 0;
    for (Element<int> elem : container3.getValues())
        for (int coord : elem.getCoord())
            count += coord < 100000 ? 1 : 0;
    IS_TRUE(name, count == 500, pass, fail);

    name = "Generator file integrity test";
    std::string filename = "generated.csv";
    std::ofstream file;
    file.open(filename);
    generator.generate(120, 5, file);
    file.close();
    std::ifstream ifile(filename);
    int count_comma = std::count(
        std::istreambuf_iterator<char>(ifile),
        std::istreambuf_iterator<char>(), ',');
    ifile.close();
    ifile.open(filename);
    int count_endl = std::count(
        std::istreambuf_iterator<char>(ifile),
        std::istreambuf_iterator<char>(), '\n');
    ifile.close();
    std::remove(filename.c_str());
    IS_TRUE(name, count_endl == 120 && count_comma == 480, pass, fail);

    std::cout << "PASSED: " << pass << std::endl;
    std::cout << "FAILED: " << fail << std::endl;

    return fail == 0 ? 0 : 1;
}
