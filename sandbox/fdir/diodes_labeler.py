"""
Takes a CSV file from webmust containg the 6 coarse sun sensors values and creates a labeled file
with the coarse sun sensors values as input data.
The labels consist of 0 (off) or 1 (on).
"""

import os
import sys
import math
import datetime

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
    global NEXT_LABEL

    # Read file using line 21 as header and timestamps as index
    nb_line_header = 20
    df = pd.read_csv(csv_file_path, header=nb_line_header, skip_blank_lines=False, index_col=0)

    # Round index to the second
    df.index.rename("TIMESTAMP", inplace=True)
    threshold_ns = 1 * 1e9
    df.index = pd.to_datetime(df.index, format="%Y-%m-%d %H:%M:%S.%f")
    df.index = pd.to_datetime((np.round(df.index.astype(np.int64) / threshold_ns) * threshold_ns).astype(np.int64))

    # Cast every cell to number or NaN
    df.replace(" ", np.nan, inplace=True)
    df.iloc[:, :] = df.iloc[:, :].astype(np.float64)

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

    # Clean: cast labels to byte, remove duplicates, NaN and incorrect values
    df[label_columns] = df[label_columns].astype(np.int8)
    df.drop_duplicates(inplace=True)
    df.dropna(inplace=True)
    mask = (np.abs(df[PhotoDiode.PD3.name]) <= math.pi/2*1.01) & (np.abs(df[PhotoDiode.PD6.name]) <= math.pi/2*1.01)
    df = df[mask]

    return df


def generate_perfect_data_set(devices):
    """
    Generates a labeled file with the coarse sun sensors values as input data that covers the entire
    input space possible. AKA a perfect test data set.
    """
    global NEXT_LABEL

    # Range of PD3 and PD6 input angles
    angles_range = range(91)
    nb_samples = len(angles_range)*len(angles_range)

    # Create empty data frame
    input_columns = [PhotoDiode.PD3.name, PhotoDiode.PD6.name]
    label_columns = [d.name + "_STATE" for d in devices]
    columns = input_columns + label_columns

    todays_date = datetime.datetime.now().date()
    index = pd.date_range(todays_date-datetime.timedelta(seconds=nb_samples), periods=nb_samples, freq='S')

    df = pd.DataFrame(np.zeros((len(index), len(columns))), index=index, columns=columns)
    df.index.name = "TIMESTAMP"

    # Fill in data
    for i in range(len(angles_range)):
        for j in range(len(angles_range)):
            row_nb = j+i*len(angles_range)

            # Diodes input
            df.iat[row_nb, 0] = math.radians(angles_range[i])
            df.iat[row_nb, 1] = math.radians(angles_range[j])

            # Labels
            for k in range(len(devices)):
                d = devices[k]
                d.check(df[d.photodiode.name][row_nb], label_off, label_on)
                df.iat[row_nb, k+2] = NEXT_LABEL

    # Clean: cast labels to byte
    df[label_columns] = df[label_columns].astype(np.int8)

    return df

def generate_full_perfect_data_set(devices):
    """
    Generates a labeled file with all the coarse sun sensors values as input data that covers the entire
    input space possible. AKA a perfect test data set.
    """
    global NEXT_LABEL

    # Range of PD3 and PD6 input angles
    angles_range = range(0, 91, 15)
    nb_angles = len(angles_range)

    # Create empty data frame
    input_columns = [PhotoDiode.PD1.name,PhotoDiode.PD2.name,PhotoDiode.PD3.name,
                     PhotoDiode.PD4.name,PhotoDiode.PD5.name, PhotoDiode.PD6.name]
    nb_diodes = len(input_columns)


    nb_samples = nb_angles**nb_diodes

    label_columns = [d.name + "_STATE" for d in devices]
    columns = input_columns + label_columns

    todays_date = datetime.datetime.now().date()
    index = pd.date_range(todays_date-datetime.timedelta(seconds=nb_samples), periods=nb_samples, freq='S')

    df = pd.DataFrame(np.zeros((len(index), len(columns))), index=index, columns=columns)
    df.index.name = "TIMESTAMP"

    # Fill in data
    for e in range(nb_angles):
        for f in range(nb_angles):
            for g in range(nb_angles):
                for h in range(nb_angles):
                    for i in range(nb_angles):
                        for j in range(nb_angles):
                            row_nb = j+nb_angles * (i + nb_angles * (h + nb_angles * (g + nb_angles *(f + nb_angles * e))))

                            # Diodes input
                            df.iat[row_nb, 0] = round(math.radians(angles_range[e]), 2)
                            df.iat[row_nb, 1] = round(math.radians(angles_range[f]), 2)
                            df.iat[row_nb, 2] = round(math.radians(angles_range[g]), 2)
                            df.iat[row_nb, 3] = round(math.radians(angles_range[h]), 2)
                            df.iat[row_nb, 4] = round(math.radians(angles_range[i]), 2)
                            df.iat[row_nb, 5] = round(math.radians(angles_range[j]), 2)

                            # Labels
                            for k in range(len(devices)):
                                d = devices[k]
                                d.check(df[d.photodiode.name][row_nb], label_off, label_on)
                                df.iat[row_nb, k+nb_diodes] = NEXT_LABEL

    # Clean: cast labels to byte
    df[label_columns] = df[label_columns].astype(np.int8)

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

    # Generate data
    #generated_df = generate_perfect_data_set(DEVICES)
    #generated_df = generate_full_perfect_data_set(DEVICES)
    #generated_df.to_csv(os.path.join(_DIR_NAME, 'data/webmust_labeled/', "perfect_full_training_set.csv"))

    # Label data
    labeled_diodes_df = process_webmust_diodes_csv(INPUT_FILE, DEVICES)
    print(labeled_diodes_df)
    labeled_diodes_df.to_csv(OUTPUT_FILE)
