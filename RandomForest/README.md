# OPS-SAT OrbitAI: Random Forest with ranger
An executable binary for ESA's OPS-SAT spacecraft to enable Machine Learning using ranger in the OrbitAI experiment. After initializing the Git submodules this folder contains a fork of the ranger repository: [georgeslabreche/ranger](https://github.com/georgeslabreche/ranger).

## Environments
1. This project was successfully compiled in C++ on Ubuntu 18.04 LTS. The target platform is the Ångström Linux distribution on the spacecraft's ARM processor.
2. To compile for the spacecraft's ARM processor install the ARM cross-compiler on your local development machine: `sudo apt-get install g++-arm-linux-gnueabihf`.

## Getting started
Use `Make` to compile for either local development or the spacecraft.

### For local development
```
make clean
make
./OrbitAI_ranger --help
```

### For the spacecraft
On your local machine
```
make clean
make TARGET=arm
```
On the spacecraft (transfer the binary there)
```
./OrbitAI_ranger --help
```

### Usage
See the usage section in rangers' [README](https://github.com/georgeslabreche/ranger#usage).

## Testing
The ranger implementation of Random Forest was tested with training data that was logged when OrbitAI v1 was running on-board the spacecraft.
- The `training_data.csv` file is the `training.csv` file archived in [mochi-2021-04-18_02-51-48](https://github.com/georgeslabreche/opssat-orbitai/tree/main/results/learning/mochi-2021-04-18_02-51-48).
- The `test_data.csv` file is the `training.csv` file archived in [mochi-2021-05-15_15-26-59](https://github.com/georgeslabreche/opssat-orbitai/tree/main/results/learning/mochi-2021-05-15_15-26-59).

### Training
```
./OrbitAI_ranger --verbose --file test_data/training_data.csv --includevars PD1,PD2,PD3,PD4,PD5,PD6,LABEL --depvarname LABEL --treetype 1 --ntree 10 --write
```

### Predicting
Predictions can either be made by feeding a single data sample as an input or by batch processing a CSV file or by directly. Note that even though predictions are being made the data input still includes the target label.

#### Single input
```
./OrbitAI_ranger --verbose --includevars PD1,PD2,PD3,PD4,PD5,PD6,LABEL --input 0.01,0,1.12,0.29,0,0.02,1 --predict ranger_out.forest
./OrbitAI_ranger --verbose --includevars PD1,PD2,PD3,PD4,PD5,PD6,LABEL --input 0.01,0,0.28,0.2,0.01,1.16,0 --predict ranger_out.forest
```


#### Batch
```
./OrbitAI_ranger --verbose --includevars PD1,PD2,PD3,PD4,PD5,PD6,LABEL --file test_data/test_data.csv --predict ranger_out.forest
```

### Classification metrics
**TODO:** Write python scripts to calculate classification metrics.
