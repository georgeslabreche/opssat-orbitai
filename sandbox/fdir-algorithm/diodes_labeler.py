"""
Takes a CSV file from webmust containg the 6 photodiodes values and creates labeled file.
The labels consist of 0 (off) or 1 (on).
"""

import os
import sys

import numpy as np
import pandas as pd

from fdir import create_opssat_devices, PhotoDiode


NEXT_LABEL = None


def label_off():
    """Sets the next label to the OFF value"""
    global NEXT_LABEL
    NEXT_LABEL = "0"


def label_on():
    """Sets the next label to the ON value"""
    global NEXT_LABEL
    NEXT_LABEL = "1"


def process_webmust_diodes_csv(csv_file_path, devices):
    """
    Parses a webmust CSV file with the 6 photodiodes values and returns a formatted panda
    DataFrame with labels for the given devices
    """
    # Create first line with column names
    global NEXT_LABEL

    # Read file using line 21 as header and timestamps as index
    nb_line_header = 20
    df = pd.read_csv(csv_file_path, header=nb_line_header, skip_blank_lines=False, index_col=0)
    df.replace(" ", np.nan, inplace=True)
    df.iloc[:, :] = df.iloc[:, :].astype(np.float64)

    # Round index to the second
    threshold_ns = 1 * 1e9
    df.index = pd.to_datetime(df.index, format="%Y-%m-%d %H:%M:%S.%f")
    df.index = pd.to_datetime(np.round(df.index.astype(np.int64) / threshold_ns) * threshold_ns)

    # Group 2 rows with same index
    df = df.groupby([df.index]).agg([np.nanmin])
    df.columns = df.columns.droplevel(1)

    # Keep only diodes that interest us and rename header
    input_column_indexes = [PhotoDiode.PD3.value, PhotoDiode.PD6.value]
    input_column = [PhotoDiode.PD3.name, PhotoDiode.PD6.name]
    df = df.iloc[:, input_column_indexes]
    df.columns = input_column

    # Create empty column for labels
    label_columns = [d.name + "_STATE" for d in devices]
    df = df.reindex(columns=df.columns.tolist() + label_columns)

    # Fill in labels
    for i in range(len(df.index)):
        for j in range(len(devices)):
            d = devices[j]
            d.check(df[d.photodiode.name][i], label_off, label_on)
            df.iat[i, j+2] = NEXT_LABEL

    # Clean: cast labels to byte, remove duplicates, remove timestamps
    df[label_columns] = df[label_columns].astype(np.int8)
    df.drop_duplicates(inplace=True)
    df.set_index(df.columns[0], inplace=True)

    return df


if __name__ == "__main__":
    # Path from where the script is called
    _CALLER_DIR_NAME = os.getcwd()

    # Path of the folder containing the script
    _DIR_NAME = os.path.dirname(__file__)

    # Path of the default photodiodes input CSV file
    INPUT_FILE = os.path.join(_DIR_NAME, 'data/webmust/I_PD_THETA.csv')

    # Check for python command arguments
    if len(sys.argv) == 2:
        try:
            # relative path to input file
            INPUT_FILE = os.path.join(_CALLER_DIR_NAME, sys.argv[1])
        except:
            print('Error setting arguments, using default values.')
    elif len(sys.argv) != 1:
        print("Wrong number of arguments")
        exit(1)

    # Compute output file path
    OUTPUT_FILE = os.path.join(_DIR_NAME, 'data/webmust_labeled/', "labeled_" + os.path.basename(INPUT_FILE))

    # Initialize the devices
    DEVICES = create_opssat_devices()

    # Label data
    labeled_diodes_df = process_webmust_diodes_csv(INPUT_FILE, DEVICES)
    labeled_diodes_df.to_csv(OUTPUT_FILE)
