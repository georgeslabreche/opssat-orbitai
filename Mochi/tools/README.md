## Tools
A collection of scripts to generate test commands, calculate classification metrics, and run multi-epoch benchmarks:
- **generate.py**: Writes a list of generated commands to a file which can then be used for batch operations to train models and run inferences.
- **analyze.py**: Calculates classification metrics based on logged inference results in the inference.csv file.
- **benchmark.sh**: Trains all models with a given number of epochs and calculates their classification metrics after each training epoch.
- **inference.sh**: Run inference commands on trained models and calculate their performance metrics.

Instructions on how to use this scripts are commented in their respective source files.
