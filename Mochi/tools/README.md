## Tools
A collection of scripts to generate test commands, calculate classification metrics, and run multi-epoch benchmarks:
- **generate.py**: Writes a list of generated commands to a file which can then be used for batch operations to train models and run inferences.
- **analyze.py**: Calculates classification metrics based on logged inference results in the inference.csv file.
- **benchmark.sh**: Trains all models with a given number of epochs and calculates their classification metrics after each training epoch.
- **inference.sh**: Run inference commands on trained models and calculate their performance metrics.

Instructions on how to use this scripts are commented in their respective source files.

## Evaluating models trained in space
### 1D, 2D, and 3D models
From this `tools` directory:

1. Put the downlinked model files inside a yyyy-MM-dd folder in the `models_space` directory.
2. Prefix the model files' content with `22 serialization::archive 15 `. This prefix is used when serializing/deserializing model files on the ground in a 64-bit environment but not on the spacecraft's ARM 32-bit environment. The prefix needs to be included for the models to load correctly when evalutating them on a 64-bit environment.
3. Delete the `models` directory and recreate it with the model files from steps 1⁠–2. E.g.: 
```bash
rm -r models
mkdir models
cp -r models_space/2021-05-15/* models/
```
4. Execute the script that will load the models and run inferences using the test inference commands. Be sure to appropriately timestamp the output CSV file for the classfication metrics. E.g.: 
```bash
./inference.sh 0 0.05 cmds/inferences_A.txt metrics/space/inferences_2021-05-15.csv
```

### 5D models
#### Background
The 1D, 2D, and 3D models are trained with PD6 input values. The 5D models are trained with PD1 to PD5 input values to experiment with training models that could be used as fallback in case of PD6 failure.
#### Instructions
Only 1D, 2D, and 3D models can be evaluated with the generated inference commands listed in `cmds/inferences_A.txt`. This is because those commands have dummy values for elevation angle values PD1–PD5 (the data that 5D models use as training and inference inputs). Evaluating 5D inference is done with PD1–PD5 values that are collected on-board the spacecraft, downlinked, and then included in inference commands listed in the `cmds/inferences_5D.txt` file. The following is a sample call to the inference script to evaluate 5D models with data fetched from the OrbitAI logs:
```
./inference.sh 1 0.03 cmds/inferences_5D.txt metrics/space/inferences_include5D_2021-05-15.csv
```

Note that `cmds/inferences_5D.txt` contains a lot of inference commands with the same PD1–PD5 input values and would thus benefit from being downsampled to only contain commands with unique combinations of PD1–PD5 values.

### On-board inference
Evaluating the 5D models is preferably done directly on-board the spacecraft be setting the OrbitAI mode to "inference" and downlinking the results. In this may, the model is evaluated with data that wasn't used as training data.