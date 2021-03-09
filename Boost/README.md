# OPS-SAT OrbitAI: Online Multi-Class LPBoost
An executable binary for ESA's OPS-SAT spacecraft to enable Machine Learning using Online Multi-Class LPBoost in the OrbitAI experiment. After initializing the Git submodules this folder contains:
1. A fork of the Online Multi-Class LPBoost repository taken from [amirsaffari/online-multiclass-lpboost](https://github.com/amirsaffari/online-multiclass-lpboost).
2. The [v2.0 branch](https://gitlab.com/libeigen/eigen/-/tree/2.0) of the Eigen repository.

## Environments
This project was successfully compiled in in Ubuntu 18.04 LTS. The target platform is the Ångström distribution is a Linux on the spacecraft's ARM processor.

### Development
1. Install the ARM cross-compiler: `sudo apt-get install gcc-arm-linux-gnueabihf`.
2. Install the Boost C++ libraries: `sudo apt-get install libboost-all-dev`.
3. install libconfig++: `sudo apt-get install -y libconfig++-dev`.

Alternatively, for step 3, download [libconfig v1.7.2](http://hyperrealm.github.io/libconfig/dist/libconfig-1.7.2.tar.gz) and install with `./configure` followed by `sudo make install`.

### Spacecraft
#### libconfig++
The libconfig++ library needs to be packaged for and deployed to the spacecraft:
1. Check if the libray isn't already installed in `/usr/lib`. If it is already there than skip this package deployment.
2. Download the ARM compiled libconfig dependency, [here](https://packages.debian.org/sid/armhf/libconfig++9v5/download).
3. Make sure that .so, and .so.X.XX.X files are where the compiler expects them, e.g. in `/usr/lib/arm-linux-gnueabihf/`.
4. Note that the .so files are simply symlink to the .so.X.XX.X file.
5. These files will have to be created on the SEPP via IPK installation.

#### Boost C++ Library
The Boost C++ library needs to be packaged for and deployed to the spacecraft:
1. Check if the libray isn't already installed in `/usr/lib`. If it is already there than skip this package deployment.
2. Cherry-pick download the ARM compiled libboost dependencies, [here](https://packages.debian.org/source/stretch/armhf/boost1.62).
3. Make sure that the .so and .so.X.XX.X files are where the compiler expects them, e.g. in `/usr/lib/arm-linux-gnueabihf/`.
4. Note that the .so files are simply symlink to the .so.X.XX.X file.

## Getting Started
- Use `make` to compile for either the local development environment or the spacecraft. 
- More details in `online-multiclass-lpboost/README.md` and `online-multiclass-lpboost/INSTALL`.

### Development
```
cd online-multiclass-lpboost
make clean
make
./OrbitAI_OMCBoost -c conf/omcb.conf --data data/dna-train.data --labels data/dna-train.labels --new models/model_larank --larank
```

### Spacecraft
```
cd online-multiclass-lpboost
make clean
make TARGET=arm
./OrbitAI_OMCBoost -c conf/omcb.conf --data data/dna-train.data --labels data/dna-train.labels --new models/model_larank --larank
```