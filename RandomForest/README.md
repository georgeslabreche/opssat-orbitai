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