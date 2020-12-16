# OPS-SAT OrbitAI FDIR Algorithm
Optical device FDIR algorithm as implemented on the OPS-SAT spacecraft's OBSW.

## Installation

1. Install virtualenv: `pip3 install --user virtualenv`
2. Create a virtual environment called `venv`: `python3 -m venv venv`
3. Start the virtual environment: `source venv/bin/activate`
4. Install the required packages in the virtual environment: `pip install -r requirements.txt`

## Starting

### FDIR algorithm demonstration
1. Create photodiode input values for each device in CSV file `data/fdir_examples/inputs.csv`
2. Run the program: `python3 fdir.py`
3. Optionally, run the program with photodiode elevation polling period (s), device turn off timer duration (s) and relative path to another input file: `python3 fdir.py 1 5 data/fdir_examples/inputs2.csv`

### Diodes labeler
1. Place photodiode input values in webmust CSV file `data/webmust/I_PD_THETA.csv`
2. Run the program: `python3 diodes_labeler.py`, the formatted and labeled file is generated in `data/webmust_labeled/`
3. Optionally, run the program with relative path to another webmust input file: `python3 diodes_labeler.py data/webmust_labeled/I_PD_THETA2.csv`

### Quaternion labeler
1. Place quaternion input values in webmust CSV file `data/webmust/O_Q_FB_FI_EST.csv`
2. Place photodiode input values in webmust CSV file `data/webmust/I_PD_THETA.csv`
3. Run the program: `python3 coarse_quat_labeler.py`, the formatted and labeled file is generated in `data/webmust_labeled/`
4. Optionally, run the program with relative path to others webmust input files: `python3 coarse_quat_labeler.py data/webmust/O_Q_FB_FI_EST.csv data/webmust/I_PD_THETA.csv`

### Visualize spacecraft's attitude changes
0. The visualizer reads the labeled coarse ADCS quaternion file `data/webmust_labeled/labeled_O_Q_FB_FI_EST.csv`.
1. Run the program: `python3 viz_sc_attitude.py`
