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

## MochiMochi Integration
The OrbitAI app integrates the [MochiMochi Online Machine Learning Library](https://github.com/georgeslabreche/MochiMochi) for binary classification. The following online machine learning (ML) classes are provided by MochiMochi:
- [ADAGRAD_RDA](https://github.com/georgeslabreche/MochiMochi/blob/orbitai/mochimochi/classifier/binary/adagrad_rda.hpp): Adaptive Subgradient Methods for Online Learning and Stochastic Optimization (Adagarad = Adaptive Gradian, RDA = Regularized Dual Averaging).
- [ADAM](https://github.com/georgeslabreche/MochiMochi/blob/orbitai/mochimochi/classifier/binary/adam.hpp): A Method for Stochastic Optimization.
- [AROW](https://github.com/georgeslabreche/MochiMochi/blob/orbitai/mochimochi/classifier/binary/arow.hpp): Adaptive Regularization of Weight Vectors.
- [NHERD](https://github.com/georgeslabreche/MochiMochi/blob/orbitai/mochimochi/classifier/binary/nherd.hpp): Normal Herd (Learning via Gaussian Herding) with full diagonal covariance.
- [PA](https://github.com/georgeslabreche/MochiMochi/blob/orbitai/mochimochi/classifier/binary/pa.hpp): Passive Aggressive. All three variants: PA, PA-I, PA-II.
- [SCW](https://github.com/georgeslabreche/MochiMochi/blob/orbitai/mochimochi/classifier/binary/scw.hpp): Exact Soft Confidence-Weighted Learning.

These online ML classes are grouped as **Concrete Classes** on the following UML class diagram ([pdf](https://github.com/georgeslabreche/opssat-orbitai/raw/main/Mochi/docs/OrbitAI_Mochi_UML_Class_Diagram.pdf)):

![UML Class Diagram for MochiMochi integration into the OrbitAI app](https://github.com/georgeslabreche/opssat-orbitai/raw/main/Mochi/docs/OrbitAI_Mochi_UML_Class_Diagram.png)

Instances of the **Concrete Classes** are created via the **Concrete Creators** implemented as part of the [factory design pattern](https://refactoring.guru/design-patterns/factory-method/cpp/example), see [here](https://github.com/georgeslabreche/MochiMochi/blob/orbitai/mochimochi/classifier/factory/binary_oml_factory.hpp). A Client App interact with OrbitAI's online ML algorithms by sending commands to the `SocketServer` instance, henceforth referred to as the *ML Server*. These commands are documented in the *[Algorithms](https://github.com/georgeslabreche/opssat-orbitai/tree/main/Mochi#algorithms)* section of this README.

### The `BinaryOML` interface
- Declares the operations that all `BinaryOML` **Concrete Classes** must implement.
- The **Concrete Classes** are the online ML classes provided by the MochiMochi library.

### The `BinaryOMLCreator` class
- Contains the core logic that relies on objects that implement the `BinaryOML` interface.
- Declares the factory method that returns a concrete online ML object which implements the `BinaryOML` interface.
- Its subclasses are **Concreate Creators** that implement the factory method.
- The **Concrete Creators** override the factory method in order to change the resulting `BinaryOML`'s type.

### The `BinaryOMLInterface` interface
- Exists so that the [proxy design pattern](ttps://refactoring.guru/design-patterns/proxy/cpp/example) can be implemented by a client – i.e, the OrbitAI app.
- Within OrbitAI, the Proxy class that implements this interface is `MochiMochiProxy`, see [here](https://github.com/georgeslabreche/opssat-orbitai/blob/main/Mochi/src/MochiMochiProxy.hpp).
- Declares common operations for the `BinaryOMLCreator` class and whatever Proxy class will be implemented.

### The `MochiMochiProxy` class
- Exists so that multiple models can be trained "simultenously" given a single training data input.
- Multiple models can be trained for a single input when more than one training algorithm is enabled via the OrbitAI app's properties file. For instance, enabling both the ADAM and AROW algorithms will instanciate the `ADAM` and `AROW` **concrete classes** via the `BinaryADAMCreator` and `BinaryAROWCreator` **concrete creators**.
- Invoking the `MochiMochiProxy` functions defined in `BinaryOMLInterface` will invoke the equivalent functions for all enabled online ML **concrete classes**. This is achieved via a simple loop that fetches instances of the enabled online ML **concrete classes** stored in `MochiMochiProxy`'s `m_bomlCreatorVector` vector property.
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
Running the program will start a server that accepts commands to train models and make predictions/inferences using the online ML methodologies implemented by the MochiMochi library.

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