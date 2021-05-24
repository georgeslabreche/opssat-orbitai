ranger is a fast implementation of random forests (Breiman 2001) or recursive partitioning, particularly suited for high dimensional data. Classification, regression, and survival forests are supported. Classification and regression forests are implemented as in the original Random Forest (Breiman 2001), survival forests as in Random Survival Forests (Ishwaran et al. 2008). Includes implementations of extremely randomized trees (Geurts et al. 2006) and quantile regression forests (Meinshausen 2006).

The GitHub repo: https://github.com/imbs-hl/ranger

### Installation

#### Standalone C++ version
To install the C++ version of ranger in Linux or Mac OS X you will need a compiler supporting C++11 (i.e. gcc >= 4.7 or Clang >= 3.0) and Cmake. To build start a terminal from the ranger main directory and run the following commands

```bash
cd cpp_version
mkdir build
cd build
cmake ..
make
```

After compilation there should be an executable called "ranger" in the build directory. 
To run the C++ version in Microsoft Windows please cross compile or ask for a binary.

### Usage

#### Standalone C++ version
In the C++ version type 

```bash
./ranger --help 
```

for a list of commands. First you need a training dataset in a file. This file should contain one header line with variable names and one line with variable values per sample (numeric only). Variable names must not contain any whitespace, comma or semicolon. Values can be seperated by whitespace, comma or semicolon but can not be mixed in one file. A typical call of ranger, when in the build directory, would be for example

```bash
./ranger --verbose --file ../training/mochi-2021-04-16_18-28-31/logs/training_ranger.csv --depvarname Target --treetype 1 --ntree 10 --nthreads 4
```

All the training datasets are in /ranger/training/ folder. 
New types of datasets suitable for ranger have been created (training_ranger.csv), containing a header line with the name of each variable. The name of the dependent variable is 'Target'.

Save a forest to file ranger_out.forest: 
```bash
./ranger --verbose --file ../training/mochi-2021-04-16_18-28-31/logs/training_ranger.csv --depvarname Target --treetype 1 --ntree 10 --nthreads 4 --write
```

Load and use a saved forest from file ranger_out.forest: 
```bash
./ranger --verbose --file ../training/mochi-2021-04-16_18-28-31/logs/training_ranger.csv --depvarname Target --treetype 1 --ntree 10 --nthreads 4 --predict ranger_out.forest
```
