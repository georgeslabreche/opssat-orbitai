# OPS-SAT OrbitAI: Online Machine Learning with MochiMochi
An executable binary for ESA's OPS-SAT spacecraft to enable Machine Learning using MochiMochi in the OrbitAI experiment. After initializing the Git submodules this folder contains:
1. A fork of the MochiMochi repository taken from [olanleed/MochiMochi](https://github.com/olanleed/MochiMochi).
2. The [v3.3 branch](https://gitlab.com/libeigen/eigen/-/tree/3.3) of the Eigen repository.

## Environments
This project was successfully compiled in C++ on Ubuntu 18.04 LTS. The target platform is the Ångström distribution, it's a Linux on the spacecraft's ARM processor.

### Local development
Install the Boost C++ libraries: `sudo apt-get install libboost-all-dev`

### Spacecraft deployment
On your local machine
1. Install the ARM cross-compiler: `sudo apt-get install g++-arm-linux-gnueabihf`
2. Install the ARM Boost C++ libraries
    1. Download the ARM compiled libboost dependencies [here](https://packages.debian.org/sid/armhf/libboost-serialization1.74-dev/download). Choose a server an click "Save file".
    2. Extract the libraries from the downloaded file
        1. `mkdir extracted_deb`
        2. `cp libboost-serialization1.74-dev_1.74.0-9_armhf.deb extracted_deb/`
        3. `cd extracted_deb/ && ar x libboost-serialization1.74-dev_1.74.0-9_armhf.deb data.tar.xz` # ignore symlinks errors
    3. Copy the extracted libraries file to correct location
        1. `sudo cp usr/lib/arm-linux-gnueabihf/*.a /usr/lib/arm-linux-gnueabihf/`
        2. `sudo chmod +r /usr/lib/arm-linux-gnueabihf/*.a`

Nothing specific is required to do on the spacecraft since the libraries are statically linked when compiling.

## Getting Started
Use `Make` to compile for either local development or the spacecraft.

### Compile and start for local development
```
make clean
make
./OrbitAI_Mochi
```

### Compile and start for the spacecraft
On your local machine
```
make clean
make TARGET=arm
```
On the spacecraft (transfer the binary there)
```
./OrbitAI_Mochi
```

### Training
Running the program will start a server that accepts commands to train models using the following methodologies:
- ADAM: A Method for Stochastic Optimization.
- ADAGRAD RDA: Adaptive Subgradient Methods for Online Learning and Stochastic Optimization (Adagarad = Adaptive Gradian, RDA = Regularized Dual Averaging).
- AROW: Adaptive Regularization of Weight Vectors.
- SCW: Exact Soft Confidence-Weighted Learning.
- NHERD: Normal Herd (Learning via Gaussian Herding) with full diagonal covariance.
- PA: Passive Aggressive. All three variants: PA, PA-I, PA-II.

### Commands
The server accepts the following commands:
- reset: delete all model and log files.
- train: train the models with the given inputs.
- infer: predict label with given input.
- save: save the models.
- load: load the previously saved models in order to update them with new training data.
- exit: stop the server and exit the program (does not save the trained models).

### Training
The given training data is specific to the OrbitAI experiment. A generic solution is possible but not in the scope of this experiment for the sake of rapid prototyping.
- The `train` command takes 8 parameters: the expected label, the photodiode elevation angle values, the photodiode data acquisition timestamp (milliseconds) e.g.: 
```
train 1 0.11 0.22 0.33 0.44 0.55 1.43 1615255005626
train 0 0.11 0.22 0.33 0.44 0.55 0.43 1615255010842
```
- Label values can either be 1 or 0 for binary classification. Label values of -1 are interpreted as 0.
- Expected photodiode elevation angles values are PD6 for Camera and Optical RX FDIR model training and PD3 for Star Tracker model training.
= The photodiode acquisition timestamp is only passed for logging purposes, it is not used for training.

### Test the ML Server

#### Training
##### Single Sample
1. Start the Online ML server: `./OrbitAI_Mochi`
2. Connect to the ML server: `telnet localhost 9999`
3. Train the models: `train 1 0.11 0.22 0.33 0.44 0.55 1.43 1615255005626`
4. Save the models: `save`

##### Batch Samples
1. Start the Online ML server: `./OrbitAI_Mochi`
2. Send telnet commands to the server:
```
eval 'input="test_data/camera_tiny.txt"; echo "reset"; while IFS= read -r line; do sleep 0.02; echo "train ${line/+/''} $(date +%s000)"; done < "$input";' | telnet localhost 9999
```
3. Monitor a model file being updated: `watch -n 0.1 cat models/arow_2D`

### Inferring

##### Single Sample
1. Start the Online ML server: `./OrbitAI_Mochi`
2. Connect to the ML server: `telnet localhost 9999`
3. Load the saved models: `load`
4. Predict the label: `infer 1 0.11 0.22 0.33 0.44 0.55 1.43 1615255005626`

##### Batch Samples
1. Start the Online ML server: `./OrbitAI_Mochi`
2. Send telnet commands to the server: 
```
eval 'echo "load"; sleep 1; input="test_data/camera_tiny.txt"; while IFS= read -r line; do sleep 0.02; echo "infer ${line/+/''} $(date +%s000)"; done < "$input";' | telnet localhost 9999
```

### Validation
As a pre-requisite, make sure that `src/OrbitAI_Mochi.cpp` is compiled with the `LOG_TIMES` define set to 0. This validation procedure is to check that the models save and load as expected. First, train and infer data without loading the the saved model files:
1. Start the Online ML server: `./OrbitAI_Mochi`
2. Delete any previously saved models and log files followed by training and infering test data: 
```
eval 'echo "reset"; sleep 1; input="test_data/camera_train_and_infer_small.txt"; while IFS= read -r line; do sleep 0.02; echo "${line/+/''} $(date +%s000)"; done < "$input";' | telnet localhost 9999
```
3. Rename the `logs/training.csv` file to `logs/training1.csv` so the logged trianing data is not lost the next time that the `reset` command is invoked: `mv logs/training.csv logs/training1.csv`
4. Rename the `logs/inference.csv` file to `logs/inference1.csv` so the logged inference results are not lost the next time that the `reset` command is invoked: `mv logs/inference.csv logs/inference1.csv`

Retrain the models from scratch but this time skip the inference:
1. Start the Online ML server: `./OrbitAI_Mochi`
2. Train and save the models:
```
eval 'echo "reset"; sleep 1; input="test_data/camera_small.txt"; while IFS= read -r line; do sleep 0.02; echo "train ${line/+/''} $(date +%s000)"; done < "$input";' | telnet localhost 9999
```
3. Diff `logs/training1.csv` and `logs/training.csv`. Both files should be exactly the same: `diff logs/training1.csv logs/training.csv`

Load the saved models and infer given validation inputs. The inference results will be compared against those obtained in step 3:
1. Start the Online ML server: `./OrbitAI_Mochi`
2. Load saved models and infer:
```
eval 'echo "load"; sleep 1; input="test_data/camera_validation_small.txt"; while IFS= read -r line; do sleep 0.02; echo "infer ${line/+/''} $(date +%s000)"; done < "$input";' | telnet localhost 9999
```
3. Diff `logs/inference1.csv` and `logs/inference.csv`. Both files should be exactly the same: `diff logs/inference1.csv logs/inference.csv`

### Scripts
Some scripts are included in the tools folder:
- **generate.py**: Writes a list of generated commands to a file which can then be used for batch operations to train models and run inferences.
- **analyze.py**: Calculates classification metrics based on logged inference results in the inference.csv file.
- **benchmark.sh**: Trains all models with a given number of epochs and calculates their classification metrics after each training epoch.

Instructions on how to use this scripts are commented in their source file.

## MochiMochi
Some example training datasets are taken from:
- [SVMLight](http://svmlight.joachims.org/)'s "Inductive SVM" for `example1`.
- [TinySVM](http://chasen.org/~taku/software/TinySVM/) for `example2`.

OPS-SAT photodiode sun elevation angles training data are taken from the spacecraft's WebMUST platform.

### Examples
To compile the MochiMochi examples cd into an example directory and use `Make`. E.g.:
```
cd MochiMochi/examples/binary_classifier/scw
./clean.sh
cmake .
make
```

For each ML methodology, two training examples are given in each of the following sections:
- With training data taken from [SVMLight](http://svmlight.joachims.org/).
- With training data taken from OPS-SAT's WebMUST and represent photodiode elevation angles values.

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