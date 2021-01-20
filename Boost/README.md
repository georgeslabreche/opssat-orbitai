# OPS-SAT Online Multi-Class LPBoost
An executable binary for ESA's OPS-SAT spacecrat to enable Machine Learning using Online Multi-Class LPBoost. After initializing the Git submodules this folder contains:
1. A fork of the Online Multi-Class LPBoost repository taken from [amirsaffari/online-multiclass-lpboost](https://github.com/amirsaffari/online-multiclass-lpboost).
2. The [v2.0 branch](https://gitlab.com/libeigen/eigen/-/tree/2.0) of the Eigen repository.

## Environments
This project was successfully compiled in in Ubuntu 18.04 LTS. The target platform is the Ångström distribution is a Linux on the spacecraft's ARM processor.

### Development
1. Install the ARM cross-compilers: `sudo apt-get install gcc-arm-linux-gnueabihf`.
2. Download and install libconfig++: `sudo apt-get install -y libconfig++-dev`.

Alternatively, for step 2, download [libconfig v1.7.2](http://hyperrealm.github.io/libconfig/dist/libconfig-1.7.2.tar.gz) and install with `./configure` followed by `sudo make install`.

### Spacecraft
The libconfig++ library need to be packaged for and deployed to the spacecraft:
1. Download the ARM compiled libconfig dependency, [here](https://packages.debian.org/sid/armhf/libconfig++9v5/download).
2. Make sure that .so, and .so.X.XX.X files are where the compiler expects them, e.g. in `/usr/lib/arm-linux-gnueabihf/`.
3. Note that the .so files are simply symlink to the .so.X.XX.X file.
4. These files will have to be created on the SEPP via IPK installation.

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