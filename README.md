# Image Segmentation
## A K-means based RGB/GrayScale image segmentor

A k-means segmentation project done for academical purposes. This project is a part of training on continuous integration, version control and C++ Programming, using CMake for the build automation and docker/gitlabCI for the CI pipelining.

The goal of the current project is to design and implement a library for multidimensional **K-Means** algorithm with a focus on **image segmentation** use cases. 

## Building

Make sure you have CMake & OpenCV installed as their documentation stipulates to.

Create a build folder to build the project into it and get the final executable called 'appLauncher' under build/app folder.

``` shell
mkdir build
cmake -S . -B build/ -DCMAKE_BUILD_TYPE=RELEASE
cmake --build build/
```

## Usage

Choose the --help option to get the min-manual of the CLI.

``` shell
./appLauncher --help
# Example using default values for maximum iterations
# and centroids intialization while specifying the path
# to save thegenerated data in and the path to save the
# segmentation result file in
./appLauncher --generate data/togenerate/filepath --output segmentation/result/filepath
```

## Contributing

Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

Please make sure to update tests as appropriate.

## License

[GNU AGPLv3](https://choosealicense.com/licenses/agpl-3.0)

----
___
> Authors : (Team **HPC-AI** 2020-2021)
- BENCHARRADA Meryem
- RAMI Kader
- BENHARI Abdessalam
- BECHARA Fadi
- EL FAROUKI Ouadie
- LHEIMEUR Nezar
> Date : 13/11/2020