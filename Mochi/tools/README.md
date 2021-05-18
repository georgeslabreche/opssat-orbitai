## Tools
A collection of scripts to generate test commands, calculate classification metrics, and run multi-epoch benchmarks:
- **generate.py**: Writes a list of generated commands to a file which can then be used for batch operations to train models and run inferences.
- **analyze.py**: Calculates classification metrics based on logged inference results in the inference.csv file.
- **benchmark.sh**: Trains all models with a given number of epochs and calculates their classification metrics after each training epoch.
- **inference.sh**: Run inference commands on trained models and calculate their performance metrics.

Instructions on how to use this scripts are commented in their respective source files.

## Evaluating models trained in space
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

Note that only 1D, 2D, and 3D models are evaluated from the ground because there are not test commands with 5D inference inputs in `cmds/inferences_A.txt`. Test 5D inference inputs can be compiled from the experiment's log files on-board the spacecraft or inference can be directly evaluated in space.