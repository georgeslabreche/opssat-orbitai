[![R-CMD-check](https://github.com/imbs-hl/ranger/workflows/R-CMD-check/badge.svg)](https://github.com/imbs-hl/ranger/actions)
![CRAN Downloads month](http://cranlogs.r-pkg.org/badges/ranger?color=brightgreen)
![CRAN Downloads overall](http://cranlogs.r-pkg.org/badges/grand-total/ranger?color=brightgreen)
## ranger: A Fast Implementation of Random Forests
Marvin N. Wright

### Introduction
ranger is a fast implementation of random forests (Breiman 2001) or recursive partitioning, particularly suited for high dimensional data. Classification, regression, and survival forests are supported. Classification and regression forests are implemented as in the original Random Forest (Breiman 2001), survival forests as in Random Survival Forests (Ishwaran et al. 2008). Includes implementations of extremely randomized trees (Geurts et al. 2006) and quantile regression forests (Meinshausen 2006).

ranger is written in C++, but a version for R is available, too. We recommend to use the R version. It is easy to install and use and the results are readily available for further analysis. The R version is as fast as the standalone C++ version.

### Installation
#### R version
To install the ranger R package from CRAN, just run

```R
install.packages("ranger")
```

R version >= 3.1 is required. With recent R versions, multithreading on Windows platforms should just work. If you compile yourself, the new RTools toolchain is required.

To install the development version from GitHub using `devtools`, run

```R
devtools::install_github("imbs-hl/ranger")
```

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
#### R version
For usage of the R version see ?ranger in R. Most importantly, see the Examples section. As a first example you could try 

```R  
ranger(Species ~ ., data = iris)
```

#### Standalone C++ version
In the C++ version type 

```bash
./ranger --help 
```

for a list of commands. First you need a training dataset in a file. This file should contain one header line with variable names and one line with variable values per sample (numeric only). Variable names must not contain any whitespace, comma or semicolon. Values can be seperated by whitespace, comma or semicolon but can not be mixed in one file. A typical call of ranger would be for example

```bash
./ranger --verbose --file ../training/mochi-2021-04-16_18-28-31/logs/training_ranger.csv --depvarname Target --treetype 1 --ntree 10 --nthreads 4
```
