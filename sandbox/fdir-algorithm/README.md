# OPS-SAT OrbitAI FDIR Algorithm
Optical device FDIR algorithm as implemented on the OPS-SAT spacecraft's OBSW.

## Installation

1. Install the virtual environment: `pip3 install --user virtualenv`
2. Create the virtual environment: `python3 -m venv venv`
3. Start the virtual environment: `source venv/bin/activate`

## Starting

### FDIR algorithm demonstration
1. Create photodiode input values in CSV file `data/fdir_examples/inputs.csv`
2. Run the program: `python3 fdir.py`
3. Optionally, run the program with photodiode elevation polling period (s), device turn off timer duration (s) and relative path to another input file: `python3 fdir.py 1 5 data/fdir_examples/inputs2.csv`

### Data labeler
1. Create photodiode input values in webmust CSV file `data/webmust/I_PD_THETA.csv`
2. Run the program: `python3 data_labeler.py`, the formatted and labeled file is generated in `data/webmust_labeled/`
3. Optionally, run the program with relative path to another webmust input file: `python3 data_labeler.py data/webmust_labeled/I_PD_THETA2.csv`
