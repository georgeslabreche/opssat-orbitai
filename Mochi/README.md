# OPS-SAT OrbitAI: Online Machine Learning with MochiMochi
An executable binary for ESA's OPS-SAT spacecraft to enable Machine Learning using MochiMochi in the OrbitAI experiment. After initializing the Git submodules this folder contains:
1. A fork of the MochiMochi repository: [georgeslabreche/MochiMochi](https://github.com/georgeslabreche/MochiMochi).
2. The [v3.3 branch](https://gitlab.com/libeigen/eigen/-/tree/3.3) of the Eigen repository.

## Environments
This project was successfully compiled in C++ on Ubuntu 18.04 LTS. The target platform is the Ångström Linux distribution on the spacecraft's ARM processor.

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
./OrbitAI_Mochi <path_to_properties_file>
```

### Compile and start for the spacecraft
On your local machine
```
make clean
make TARGET=arm
```
On the spacecraft (transfer the binary there)
```
./OrbitAI_Mochi <path_to_properties_file>
```

### Algorithms
Running the program will start a server that accepts commands to train models and make predictions/inferences using the following methodologies:
- ADAM: A Method for Stochastic Optimization.
- ADAGRAD RDA: Adaptive Subgradient Methods for Online Learning and Stochastic Optimization (Adagarad = Adaptive Gradian, RDA = Regularized Dual Averaging).
- AROW: Adaptive Regularization of Weight Vectors.
- SCW: Exact Soft Confidence-Weighted Learning.
- NHERD: Normal Herd (Learning via Gaussian Herding) with full diagonal covariance.
- PA: Passive Aggressive. All three variants: PA, PA-I, PA-II.

#### Operations
The server can execute the following operations:
- reset: delete all model and log files.
- train: train the models with the given inputs.
- infer: predict label with given input.
- save: save the models.
- load: load the previously saved models in order to update them with new training data.
- exit: stop the server and exit the program (does not save the trained models).

#### Training, Continue Training, and Inferring
An ML Server implemented by the OrbitAI serves as a generic entry point that accepts training or inference inputs of any size if the following structure:
- The first four characters is an integer value representing the total length of the message sent to the ML Server.
- The remaining characters compose the input string sent to the update (train) or predict (infer) functions implemented in the [MochiMochi online ML algorithms](https://github.com/georgeslabreche/MochiMochi).

E.g. training or inference strings expected by the ML Server:
```
0041 +1 1:1.232 2:2.412 3:2.123 4:5.23223
0031 -1 1:1.232 2:2.412 3:2.123
```

Note:
- The label values can either be +1 or -1 for binary classification. These label values represent the expected labels (supervised learning).
- The ML Server determines which mode its on from the reading the properties file (Mode 0: New Training, Mode 1: Continue Training, Mode 2: Inference).
### Test the ML Server
#### Training
##### Single Sample
1. Update the properties file to training mode (mode 0).
2. Start the Online ML server: `./OrbitAI_Mochi <path_to_properties_file>`
3. Connect to the ML server: `telnet localhost 9999`
4. Train the models: `0041 +1 1:1.232 2:2.412 3:2.123 4:5.23223`
5. The models are serialized and saved in the `models` directory.

##### Batch Samples
1. Update the properties file to training mode (mode 0).
2. Start the Online ML server: `./OrbitAI_Mochi <path_to_properties_file>`
3. Send telnet commands to the server:
```
TBD
```
4. Monitor a model file being updated: `watch -n 0.1 cat models/AROW`

#### Inferring
When sending inferring messages the expected label is still included. This is for logging purposes.
##### Single Sample
1. Update the properties file to inference mode (mode 2).
2. Start the Online ML server: `./OrbitAI_Mochi <path_to_properties_file>`
3. Connect to the ML server: `telnet localhost 9999`
4. Load the saved models: `load`
5. Predict the label: `0041 +1 1:1.232 2:2.412 3:2.123 4:5.23223`

##### Batch Samples
1. Update the properties file to inference mode (mode 2).
2. Start the Online ML server: `./OrbitAI_Mochi <path_to_properties_file>`
3. Send telnet commands to the server: 
```
TBD
```

#### Validation
TBD

#### Scripts
Some scripts are included in the tools folder:
- **generate.py**: Writes a list of generated commands to a file which can then be used for batch operations to train models and run inferences.
- **analyze.py**: Calculates classification metrics based on logged inference results in the inference.csv file.
- **benchmark.sh**: Trains all models with a given number of epochs and calculates their classification metrics after each training epoch.

Instructions on how to use this scripts are commented in their source file.