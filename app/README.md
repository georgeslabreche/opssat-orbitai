# OrbitAi Onboard Online Machine Learning
Using Online ML to train FDIR models for the OPS-SAT spacecraft.

## Development Environment
1. Install ARM cross-compilers: `sudo apt-get install gcc-arm-linux-gnueabihf`
2. Include boost libraries: serizaliazation, math, and archive.
3. Fetch source code for the submodules: `git submodule update --init`


Comment out the following compiler setting in `CMakeLists.txt` to compile for your development environment instead of for ARM:

```
SET(CMAKE_C_COMPILER "/usr/bin/arm-linux-gnueabihf-gcc")
SET(CMAKE_CXX_COMPILER "/usr/bin/arm-linux-gnueabihf-g++")
```

## MochiMochi
The example training datasets are taken from:
- [SVMLight](http://svmlight.joachims.org/)'s "Inductive SVM" for `example2`.
- [TinySVM](http://chasen.org/~taku/software/TinySVM/) for  `example2`.

OrbitAI data sets are taken from WebMUST.

To compile the MochiMochi examples go into the example directory, e.g. `MochiMochi/examples/binary_classifier/scw`, and use Make:
```
./clean.sh
cmake .
make
```

### ADAM
```
./adam.out --dim 9947 --train ../../training_data/example1/train.dat --test ../../training_data/example1/test.dat
./adam.out --dim 2 --train ../../../../../sandbox/fdir-algorithm/data/svm/train/camera_2d.svmdata --test ../../../../../sandbox/fdir-algorithm/data/svm/test/camera_2d.svmdata
```

### Adagarad RDA
```
./rda.out --dim 9947 --train ../../training_data/example1/train.dat --test ../../training_data/example1/test.dat --eta 0.1 --lambda 0.000001
./rda.out --dim 9947 --train ../../../../../sandbox/fdir-algorithm/data/svm/train/camera_2d.svmdata --test ../../../../../sandbox/fdir-algorithm/data/svm/test/camera_2d.svmdata --eta 0.1 --lambda 0.000001
```

### AROW
```
./arow.out --dim 9947 --train ../../training_data/example1/train.dat --test ../../training_data/example1/test.dat --r 0.8
./arow.out --dim 9947 --train ../../../../../sandbox/fdir-algorithm/data/svm/train/camera_2d.svmdata --test ../../../../../sandbox/fdir-algorithm/data/svm/test/camera_2d.svmdata --r 0.8
```

### SCW
```
./scw.out --dim 9947 --train ../../training_data/example1/train.dat --test ../../training_data/example1/test.dat --c 0.1 --eta 0.95
./scw.out --dim 9947 --train ../../../../../sandbox/fdir-algorithm/data/svm/train/camera_2d.svmdata --test ../../../../../sandbox/fdir-algorithm/data/svm/test/camera_2d.svmdata --c 0.1 --eta 0.95
```

### NHERD
```
./nherd.out --dim 9947 --train ../../training_data/example1/train.dat --test ../../training_data/example1/test.dat --c 0.1 --diagonal 0
./nherd.out --dim 9947 --train ../../../../../sandbox/fdir-algorithm/data/svm/train/camera_2d.svmdata --test ../../../../../sandbox/fdir-algorithm/data/svm/test/camera_2d.svmdata --c 0.1 --diagonal 0
```

### PA
```
./pa.out --dim 9947 --train ../../training_data/example1/train.dat --test ../../training_data/example1/test.dat --c 0.1 --select 1
./pa.out --dim 9947 --train ../../../../../sandbox/fdir-algorithm/data/svm/train/camera_2d.svmdata --test ../../../../../sandbox/fdir-algorithm/data/svm/test/camera_2d.svmdata --c 0.1 --select 1
```

## Orbitai
### Compile
```
./clean.sh
cmake .
make
```

### Run
This is not implemented yet:
`./orbitai.out`
