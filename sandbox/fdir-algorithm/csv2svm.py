import os
import csv
import glob
from enum import Enum
from pathlib import Path
import random

ROUND_PD = True
ROUND_PD_DIGITS = 1

# We always round Euler angles.

ROUND_QUATERNION = True
ROUND_QUATERNION_DIGITS = 1


class CsvHeaderElevationAngles(Enum):
    TIMESTAMP = 0
    PD3 = 1 # Elevation angle for Star Tracker surface.
    PD6 = 2 # Elevation angle for HD Camera and Optical RX surface.
    STAR_TRACKER_STATE = 3
    HD_CAMERA_STATE = 4
    OPTICAL_RX_STATE = 5


class CsvHeaderQuatEuler(Enum):
    TIMESTAMP = 0
    w = 1
    x = 2
    y = 3
    z = 4
    x_euler = 5
    y_euler = 6
    z_euler = 7
    STAR_TRACKER_STATE = 8
    HD_CAMERA_STATE = 9
    OPTICAL_RX_STATE = 10


class DeviceId(Enum):
    """Device id enum."""
    HD_CAMERA = 0
    OPTICAL_RX = 1
    STAR_TRACKER = 2

# 1 dimension input: Photodiode value.

def csv2svm_1D(device_id):
    """1 dimensional input with photodiode elevation angle."""

    svm_filename = None
    pd_col_index = None
    device_state_col_index = None

    if device_id == DeviceId.HD_CAMERA:
        svm_filename = 'data/svm/all/camera_1d.svmdata'
        pd_col_index = CsvHeaderElevationAngles.PD6.value
        device_state_col_index = CsvHeaderElevationAngles.HD_CAMERA_STATE.value

    elif device_id == DeviceId.OPTICAL_RX:
        svm_filename = 'data/svm/all/optical_rx_1d.svmdata'
        pd_col_index = CsvHeaderElevationAngles.PD6.value
        device_state_col_index = CsvHeaderElevationAngles.OPTICAL_RX_STATE.value

    elif device_id == DeviceId.STAR_TRACKER:
        svm_filename = 'data/svm/all/star_tracker_1d.svmdata'
        pd_col_index = CsvHeaderElevationAngles.PD3.value
        device_state_col_index = CsvHeaderElevationAngles.STAR_TRACKER_STATE.value

    else:
        # Invalid device id.
        return None


    # Delete any previously created SVM data file.
    if Path(svm_filename).is_file():
        os.remove(svm_filename) 

    # SVM file writer.
    with open(svm_filename, 'a') as svm_file:
        
        # CSV file reader.
        with open('data/webmust_labeled/perfect_training_set.csv') as csv_file:

            # Read the CSV file.
            csv_reader = csv.reader(csv_file, delimiter=',')

            # Skip the header row.
            next(csv_reader, None)

            # Convert each CSV row into an SVM line.
            for row in csv_reader:
                pd = round(float(row[pd_col_index]), ROUND_PD_DIGITS) if ROUND_PD else float(row[pd_col_index])
                device_state = row[device_state_col_index]

                # Construct the SVM data line for the current CSV data row.
                svm_line = ('+1' if int(device_state) == 1 else '-1') + ' 1:' + str(pd)

                # Write the SVM data line into a file
                svm_file.write(svm_line + '\n')


def csv2svm_2D(device_id):
    """2 dimensional input with 2 photodiode values."""

    svm_filename = None
    pd3_col_index = None
    pd6_col_index = None
    device_state_col_index = None

    if device_id == DeviceId.HD_CAMERA:
        svm_filename = 'data/svm/all/camera_2d.svmdata'
        pd3_col_index = CsvHeaderElevationAngles.PD3.value
        pd6_col_index = CsvHeaderElevationAngles.PD6.value
        device_state_col_index = CsvHeaderElevationAngles.HD_CAMERA_STATE.value

    elif device_id == DeviceId.OPTICAL_RX:
        svm_filename = 'data/svm/all/optical_rx_2d.svmdata'
        pd3_col_index = CsvHeaderElevationAngles.PD3.value
        pd6_col_index = CsvHeaderElevationAngles.PD6.value
        device_state_col_index = CsvHeaderElevationAngles.OPTICAL_RX_STATE.value

    elif device_id == DeviceId.STAR_TRACKER:
        svm_filename = 'data/svm/all/star_tracker_2d.svmdata'
        pd3_col_index = CsvHeaderElevationAngles.PD3.value
        pd6_col_index = CsvHeaderElevationAngles.PD6.value
        device_state_col_index = CsvHeaderElevationAngles.STAR_TRACKER_STATE.value

    else:
        # Invalid device id.
        return None


    # Delete any previously created SVM data file.
    if Path(svm_filename).is_file():
        os.remove(svm_filename) 

    # SVM file writer.
    with open(svm_filename, 'a') as svm_file:
        
        # CSV file reader.
        with open('data/webmust_labeled/perfect_training_set.csv') as csv_file:

            # Read the CSV file.
            csv_reader = csv.reader(csv_file, delimiter=',')

            # Skip the header row.
            next(csv_reader, None)

            # Convert each CSV row into an SVM line.
            for row in csv_reader:

                pd3 = round(float(row[pd3_col_index]), ROUND_PD_DIGITS) if ROUND_PD else float(row[pd3_col_index])
                pd6 = round(float(row[pd6_col_index]), ROUND_PD_DIGITS) if ROUND_PD else float(row[pd6_col_index])
                device_state = row[device_state_col_index]

                # Construct the SVM data line for the current CSV data row.
                svm_line = ('+1' if int(device_state) == 1 else '-1') + ' 1:' + str(pd3) + ' 2:' + str(pd6)

                # Write the SVM data line into a file
                svm_file.write(svm_line + '\n')


def csv2svm_3D(device_id):
    """3 dimensional input with Euler angles."""

    svm_filename = None
    device_state_col_index = None

    if device_id == DeviceId.HD_CAMERA:
        svm_filename = 'data/svm/all/camera_3d.svmdata'
        device_state_col_index = CsvHeaderQuatEuler.HD_CAMERA_STATE.value

    elif device_id == DeviceId.OPTICAL_RX:
        svm_filename = 'data/svm/all/optical_rx_3d.svmdata'
        device_state_col_index = CsvHeaderQuatEuler.OPTICAL_RX_STATE.value

    elif device_id == DeviceId.STAR_TRACKER:
        svm_filename = 'data/svm/all/star_tracker_3d.svmdata'
        device_state_col_index = CsvHeaderQuatEuler.STAR_TRACKER_STATE.value

    else:
        # Invalid device id.
        return None

    # Delete any previously created SVM data file.
    if Path(svm_filename).is_file():
        os.remove(svm_filename) 

    # SVM file writer.
    with open(svm_filename, 'a') as svm_file:
        
        # CSV file reader.
        with open('data/webmust_labeled/labeled_O_Q_FB_FI_EST.csv') as csv_file:

            # Read the CSV file.
            csv_reader = csv.reader(csv_file, delimiter=',')

            # Skip the header row.
            next(csv_reader, None)

            # Convert each CSV row into an SVM line.
            for row in csv_reader:
                x_euler = int(round(float(row[CsvHeaderQuatEuler.x_euler.value]), 0))
                y_euler = int(round(float(row[CsvHeaderQuatEuler.y_euler.value]), 0))
                z_euler = int(round(float(row[CsvHeaderQuatEuler.z_euler.value]), 0))

                device_state = row[device_state_col_index]

                # Construct the SVM data line for the current CSV data row.
                svm_line = ('+1' if int(device_state) == 1 else '-1') + ' 1:' + str(x_euler) + ' 2:' + str(y_euler) + ' 3:' + str(z_euler)

                # Write the SVM data line into a file
                svm_file.write(svm_line + '\n')


def csv2svm_4D(device_id):
    """4 dimensional input with quaternion."""

    svm_filename = None
    device_state_col_index = None

    if device_id == DeviceId.HD_CAMERA:
        svm_filename = 'data/svm/all/camera_4d.svmdata'
        device_state_col_index = CsvHeaderQuatEuler.HD_CAMERA_STATE.value

    elif device_id == DeviceId.OPTICAL_RX:
        svm_filename = 'data/svm/all/optical_rx_4d.svmdata'
        device_state_col_index = CsvHeaderQuatEuler.OPTICAL_RX_STATE.value

    elif device_id == DeviceId.STAR_TRACKER:
        svm_filename = 'data/svm/all/star_tracker_4d.svmdata'
        device_state_col_index = CsvHeaderQuatEuler.STAR_TRACKER_STATE.value

    else:
        # Invalid device id.
        return None

    # Delete any previously created SVM data file.
    if Path(svm_filename).is_file():
        os.remove(svm_filename) 

    # SVM file writer.
    with open(svm_filename, 'a') as svm_file:
        
        # CSV file reader.
        with open('data/webmust_labeled/labeled_O_Q_FB_FI_EST.csv') as csv_file:

            # Read the CSV file.
            csv_reader = csv.reader(csv_file, delimiter=',')

            # Skip the header row.
            next(csv_reader, None)

            # Convert each CSV row into an SVM line.
            for row in csv_reader:
                w = round(float(row[CsvHeaderQuatEuler.w.value]), ROUND_QUATERNION) if ROUND_QUATERNION else float(row[CsvHeaderQuatEuler.w.value])
                x = round(float(row[CsvHeaderQuatEuler.x.value]), ROUND_QUATERNION) if ROUND_QUATERNION else float(row[CsvHeaderQuatEuler.x.value])
                y = round(float(row[CsvHeaderQuatEuler.y.value]), ROUND_QUATERNION) if ROUND_QUATERNION else float(row[CsvHeaderQuatEuler.y.value])
                z = round(float(row[CsvHeaderQuatEuler.z.value]), ROUND_QUATERNION) if ROUND_QUATERNION else float(row[CsvHeaderQuatEuler.z.value])

                device_state = row[device_state_col_index]

                # Construct the SVM data line for the current CSV data row.
                svm_line = ('+1' if int(device_state) == 1 else '-1') + ' 1:' + str(w) + ' 2:' + str(x) + ' 3:' + str(y) + ' 4:' + str(z)

                # Write the SVM data line into a file
                svm_file.write(svm_line + '\n')


def split_svm_files():
    """Split generated SVM files in 2 seperate datasets: training and testing."""

    svm_files = glob.glob('data/svm/all/*.svmdata')
    for f in svm_files:

        # Filenames of the test and training SVM data files that will be created.
        f_test = f.replace('data/svm/all/', 'data/svm/test/')
        f_train = f.replace('data/svm/all/', 'data/svm/train/')

        # Delete test and training SVM data files if they exist already.
        if Path(f_test).is_file():
            os.remove(f_test) 

        if Path(f_train).is_file():
            os.remove(f_train) 
        
        # Read line and save it in a new file as either a test line or a training line.
        with open(f) as fp:
            svm_line = fp.readline()

            while svm_line:

                # 30% of the data as test data.
                if random.randint(0, 100) <= 30:
                    with open(f_test, 'a') as svm_test_file:
                        svm_test_file.write(svm_line)

                else: # 70% of the data as training data.
                    with open(f_train, 'a') as svm_train_file:
                        svm_train_file.write(svm_line)

                svm_line = fp.readline()


# 1 dimension input: 1 PD value.
csv2svm_1D(DeviceId.HD_CAMERA)
csv2svm_1D(DeviceId.OPTICAL_RX)
csv2svm_1D(DeviceId.STAR_TRACKER)

# 2 dimension input: 2 PD values.
csv2svm_2D(DeviceId.HD_CAMERA)
csv2svm_2D(DeviceId.OPTICAL_RX)
csv2svm_2D(DeviceId.STAR_TRACKER)

# 3 dimension input: Euler angles. 
csv2svm_3D(DeviceId.HD_CAMERA)
csv2svm_3D(DeviceId.OPTICAL_RX)
csv2svm_3D(DeviceId.STAR_TRACKER)

# 4 dimension input: Quaternions.
csv2svm_4D(DeviceId.HD_CAMERA)
csv2svm_4D(DeviceId.OPTICAL_RX)
csv2svm_4D(DeviceId.STAR_TRACKER)

# Splitting the generated SVM files into training and testing datasets.
split_svm_files()