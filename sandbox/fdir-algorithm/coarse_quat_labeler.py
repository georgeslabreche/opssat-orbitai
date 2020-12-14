"""
Takes a CSV file from webmust containg the 6 coarse sun sensors values and a CSV file containing the
coarse ADCS quaternion and creates a labeled file with coarse ADCS quaternion converted to Euler
angles as input data.
The labels consist of 0 (off) or 1 (on).
"""

import os
import sys

import numpy as np
import pandas as pd
from scipy.spatial.transform import Rotation as ro

from fdir import create_opssat_devices
from diodes_labeler import process_webmust_diodes_csv


def process_webmust_coarse_quat_csv(csv_file_path, labeled_diodes, devices):
    """
    Parses a webmust CSV file with coarse ADCS quaternion values and returns a formatted panda
    DataFrame with labels for the given devices. Labeled photodiodes values are used to label the
    quaternion by matching timestamps.
    """
    # Read file using line 19 as header and timestamps as index
    nb_line_header = 18
    df = pd.read_csv(csv_file_path, header=nb_line_header, skip_blank_lines=False, index_col=0)
    df.replace(" ", np.nan, inplace=True)
    df.iloc[:, :] = df.iloc[:, :].astype(np.float64)

    # Round index to the second
    df.index.rename("TIMESTAMP", inplace=True)
    threshold_ns = 1 * 1e9
    df.index = pd.to_datetime(df.index, format="%Y-%m-%d %H:%M:%S.%f")
    df.index = pd.to_datetime((np.round(df.index.astype(np.int64) / threshold_ns) * threshold_ns).astype(np.int64))

    # Nice column names
    df.columns = ["w", "x", "y", "z"]  # TODO confirm order

    # Drop lines with 0 only
    df = df[(df.T != 0).any()]

    # Merge by nearest timestamp with tolerance
    df = pd.merge_asof(df,
                       labeled_diodes,
                       left_index=True,
                       right_index=True,
                       direction='nearest',
                       tolerance=pd.Timedelta(seconds=1))

    # Drop nan and photodiodes columns
    df.dropna(inplace=True)
    df.drop(df.columns[-5:-3], axis=1, inplace=True)

    # Cast labels to int
    label_columns = df.columns[-3:]
    df[label_columns] = df[label_columns].astype(np.int8)

    # Convert to euler angles
    df[['x', 'y', 'z']] = df.apply(lambda row: quat_to_euler(row['w'], row['x'], row['y'], row['z']), axis=1)
    df.drop('w', inplace=True, axis=1)

    # Drop duplicates
    df.drop_duplicates(inplace=True, keep=False)

    return df

def quat_to_euler(w, x, y, z):
    """Uses scipy to convert a quaternion to euler angle"""
    # TODO confirm axis order
    return pd.Series(ro.from_quat([x,y,z,w]).as_euler("XYZ", degrees=True))

if __name__ == "__main__":
    # Path from where the script is called
    _CALLER_DIR_NAME = os.getcwd()

    # Path of the folder containing the script
    _DIR_NAME = os.path.dirname(__file__)

    # Path of the default coarse ADCS quaternion input CSV file
    INPUT_FILE = os.path.join(_DIR_NAME, 'data/webmust/O_Q_FB_FI_EST.csv')

    # Path of the default photodiodes input CSV file
    INPUT_FILE_DIODES = os.path.join(_DIR_NAME, 'data/webmust/I_PD_THETA.csv')

    # Check for python command arguments
    if len(sys.argv) == 3:
        try:
            # relative path to input file
            INPUT_FILE = os.path.join(_CALLER_DIR_NAME, sys.argv[1])
            INPUT_FILE_DIODES = os.path.join(_CALLER_DIR_NAME, sys.argv[2])
        except:
            print('Error setting arguments, using default values.')
    elif len(sys.argv) != 1:
        print("Wrong number of arguments")
        exit(1)

    # Compute output file path
    OUTPUT_FILE = os.path.join(_DIR_NAME, 'data/webmust_labeled/', "labeled_" + os.path.basename(INPUT_FILE))

    # Initialize the devices
    DEVICES = create_opssat_devices()

    # Get labeled diodes data
    LABELED_DIODES = process_webmust_diodes_csv(INPUT_FILE_DIODES, DEVICES)

    # Label data
    labeled_euler_df = process_webmust_coarse_quat_csv(INPUT_FILE, LABELED_DIODES, DEVICES)
    print(labeled_euler_df)
    labeled_euler_df.to_csv(OUTPUT_FILE)
