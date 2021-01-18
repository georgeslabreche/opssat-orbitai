import os
import sys
import glob
import random

from pathlib import Path

import pandas as pd

from fdir import DeviceId

"""
Converts the Euler/Quaternion CSV file to be used with online-multiclass-lpboost.

https://github.com/amirsaffari/online-multiclass-lpboost
"""


def csv2orf_4D(device_id, csv_file):
    df = pd.read_csv(csv_file)

    output_file_data = os.path.join(_DIR_NAME, 'data/orf/all/{}_4d.data'.format(device_id.name))
    output_file_labels = os.path.join(_DIR_NAME, 'data/orf/all/{}_4d.labels'.format(device_id.name))

    # files to create
    with open(output_file_data, 'w') as output, open(output_file_labels, 'w') as output_labels:
        output.write("{} {}\n".format(df.shape[0], 4))
        output_labels.write("{} {}\n".format(df.shape[0], 1))

        for index, row in df.iterrows():
            output.write("{:.7f} {:.7f} {:.7f} {:.7f}\n".format(row['w'], row['x'], row['y'], row['z']))
            output_labels.write("{}\n".format(row[device_id.name + "_STATE"]))


def csv2orf_3D(device_id, csv_file):
    df = pd.read_csv(csv_file)

    output_file_data = os.path.join(_DIR_NAME, 'data/orf/all/{}_3d.data'.format(device_id.name))
    output_file_labels = os.path.join(_DIR_NAME, 'data/orf/all/{}_3d.labels'.format(device_id.name))

    # files to create
    with open(output_file_data, 'w') as output, open(output_file_labels, 'w') as output_labels:
        output.write("{} {}\n".format(df.shape[0], 3))
        output_labels.write("{} {}\n".format(df.shape[0], 1))

        for index, row in df.iterrows():
            output.write("{:.3f} {:.3f} {:.3f}\n".format( row['x_euler'], row['y_euler'], row['z_euler']))
            output_labels.write("{}\n".format(row[device_id.name + "_STATE"]))


def split_orf_files():
    """Split generated ORF files in 2 seperate datasets: training and testing."""

    orf_data_files = glob.glob(os.path.join(_DIR_NAME,'data/orf/all/*.data'))
    for data_file in orf_data_files:
        labels_file = data_file.replace('.data', '.labels')

        # files names for test and train data and labels
        data_file_test = data_file.replace('data/orf/all/', 'data/orf/test/test_')
        data_file_train = data_file.replace('data/orf/all/', 'data/orf/train/train_')
        labels_file_test = data_file_test.replace('.data', '.labels')
        labels_file_train = data_file_train.replace('.data', '.labels')

        with open(data_file) as df, open(labels_file) as lf:
            labels_lines = lf.readlines()
            lines = df.readlines()
            nb_samples = len(lines) - 1
            nb_columns = len(lines[1].split(" "))

            test_lines = set(random.sample(range(1, nb_samples+1), int(nb_samples*0.2)))
            nb_test_samples = len(test_lines)
            nb_train_samples = nb_samples-nb_test_samples

            with open(data_file_test, 'w') as df_test, open(data_file_train, 'w') as df_train:
                with open(labels_file_test, 'w') as lf_test, open(labels_file_train, 'w') as lf_train:
                    df_test.write("{} {}\n".format(nb_test_samples, nb_columns))
                    lf_test.write("{} {}\n".format(nb_test_samples, 1))
                    df_train.write("{} {}\n".format(nb_train_samples, nb_columns))
                    lf_train.write("{} {}\n".format(nb_train_samples, 1))

                    for i in range(1, nb_samples+1):
                        if i in test_lines:
                            df_test.write(lines[i])
                            lf_test.write(labels_lines[i])
                        else:
                            df_train.write(lines[i])
                            lf_train.write(labels_lines[i])


if __name__ == "__main__":
    # Path from where the script is called
    _CALLER_DIR_NAME = os.getcwd()

    # Path of the folder containing the script
    _DIR_NAME = os.path.dirname(__file__)

    # Path of the default coarse ADCS quaternion input CSV file
    INPUT_FILE = os.path.join(_DIR_NAME, 'data/webmust_labeled/labeled_O_Q_FB_FI_EST.csv')

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
    
    for device_id in DeviceId:
        csv2orf_4D(device_id, INPUT_FILE)
        csv2orf_3D(device_id, INPUT_FILE)

    split_orf_files()