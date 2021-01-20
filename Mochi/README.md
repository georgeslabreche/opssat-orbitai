# OPS-SAT OrbitAI: Online Machine Learning with MochiMochi
An executable binary for ESA's OPS-SAT spacecraft to enable Machine Learning using MochiMochi in the OrbitAI experiment. After initializing the Git submodules this folder contains:
1. A fork of the MochiMochi repository taken from [olanleed/MochiMochi](https://github.com/olanleed/MochiMochi).
2. The [v3.3 branch](https://gitlab.com/libeigen/eigen/-/tree/3.3) of the Eigen repository.

## Environments
This project was successfully compiled in in Ubuntu 18.04 LTS. The target platform is the Ångström distribution is a Linux on the spacecraft's ARM processor.

### Development
1. Install the ARM cross-compiler: `sudo apt-get install gcc-arm-linux-gnueabihf`.
2. Install the Boost C++ libraries: `sudo apt-get install libboost-all-dev`.

### Spacecraft
The Boost C++ library needs to be packaged for and deployed to the spacecraft:
1. Cherry-pick download the ARM compiled libboost dependencies, [here](https://packages.debian.org/source/stretch/armhf/boost1.62).
2. Make sure that the .so and .so.X.XX.X files are where the compiler expects them, e.g. in `/usr/lib/arm-linux-gnueabihf/`.
3. Note that the .so files are simply symlink to the .so.X.XX.X file.

## Getting Started
Use `Make` to compile for either the local development environment or the spacecraft. 

### Compile and start for development
```
make clean
make
./OrbitAI_Mochi
```

### Compile and start for the spacecraft
```
make clean
make TARGET=arm
./OrbitAI_Mochi
```

### Training
Running the program will start a server that accepts commands to train models using the following methodologies:
- ARROW
- SCW
- NHERD
- PA

### Commands
The server accepts the following commands:
- train: train the models with the given inputs.
- save: save the models.
- load: load the previously saved models in order to update them with new training data.
- exit: stop the server and exit the program (does not save the trained models).

### Training
The given training data is specific to the OrbitAI experiment. A generic solution is possible but in the scope of this experiment for the sake of rapid prototyping.
- The `train` command takes 3 parameters: the label and two photodiode elevation angle values, e.g.: `train 1 1.43 0.45` or `train 0 0.43 0.1`.
- Label values can either be 1 or 0 for binary classification. Label values of -1 are interpreted as 0.
- Expected photodiode elevation angles values are PD3 and PD6.

### Test Server
1. Start the Online ML server: `./OrbitAI_Mochi `
2. Send telnet commands to the server: `./test_server.sh | telnet localhost 9999`
3. Monitor a model file being updated: `watch -n 0.1 cat models/arow_2D`

## MochiMochi
Some example training datasets are taken from:
- [SVMLight](http://svmlight.joachims.org/)'s "Inductive SVM" for `example1`.
- [TinySVM](http://chasen.org/~taku/software/TinySVM/) for `example2`.

OPS-SAT photodiode sun elevation angles training data are taken from the spacecraft's WebMUST platform.

### Examples
To compile the MochiMochi examples cd into an example director and use `Make`. E.g.:
```
cd MochiMochi/examples/binary_classifier/scw
./clean.sh
cmake .
make
```

For each ML methodology, two training examples are given in each of the following sections:
- With training data taken from [SVMLight](http://svmlight.joachims.org/).
- With training data taken from OPS-SAT's WebMUST and represent  photodiode elevation angles values.

#### ADAM
```
./adam.out --dim 9947 --train ../../training_data/example1/train.dat --test ../../training_data/example1/test.dat
./adam.out --dim 2 --train ../../../../../sandbox/fdir/data/svm/train/camera_2d_none.svmdata --test ../../../../../sandbox/fdir/data/svm/test/camera_2d_from_1d_A_Xpd6_power.svmdata
```

### Adagarad RDA
```
./rda.out --dim 9947 --train ../../training_data/example1/train.dat --test ../../training_data/example1/test.dat --eta 0.1 --lambda 0.000001
./rda.out --dim 2 --train ../../../../../sandbox/fdir/data/svm/train/camera_2d_none.svmdata --test ../../../../../sandbox/fdir/data/svm/test/camera_2d_from_1d_A_Xpd6_power.svmdata --eta 0.1 --lambda 0.000001
```

### AROW
```
./arow.out --dim 9947 --train ../../training_data/example1/train.dat --test ../../training_data/example1/test.dat --r 0.8
./arow.out --dim 2 --train ../../../../../sandbox/fdir/data/svm/train/camera_2d_none.svmdata --test ../../../../../sandbox/fdir/data/svm/test/camera_2d_from_1d_A_Xpd6_power.svmdata --r 0.8
```

### SCW
```
./scw.out --dim 9947 --train ../../training_data/example1/train.dat --test ../../training_data/example1/test.dat --c 0.1 --eta 0.95
./scw.out --dim 2 --train ../../../../../sandbox/fdir/data/svm/train/camera_2d_none.svmdata --test ../../../../../sandbox/fdir/data/svm/test/camera_2d_from_1d_A_Xpd6_power.svmdata --c 0.1 --eta 0.95
```

### NHERD
```
./nherd.out --dim 9947 --train ../../training_data/example1/train.dat --test ../../training_data/example1/test.dat --c 0.1 --diagonal 0
./nherd.out --dim 2 --train ../../../../../sandbox/fdir/data/svm/train/camera_2d_none.svmdata --test ../../../../../sandbox/fdir/data/svm/test/camera_2d_from_1d_A_Xpd6_power.svmdata --c 0.1 --diagonal 0
```

### PA
```
./pa.out --dim 9947 --train ../../training_data/example1/train.dat --test ../../training_data/example1/test.dat --c 0.1 --select 1
./pa.out --dim 2 --train ../../../../../sandbox/fdir/data/svm/train/camera_2d_none.svmdata --test ../../../../../sandbox/fdir/data/svm/test/camera_2d_from_1d_A_Xpd6_power.svmdata --c 0.1 --select 1
```

### Run all examples
To run all examples with all OPS-SAT training and test data:
- Make sure that all examples are compiled.
- `./runall_examples.sh`.
- Sample results are available in `results.txt`.