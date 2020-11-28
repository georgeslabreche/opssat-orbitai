from enum import Enum
import random
import math
import progressbar
from river import compose
from river import linear_model
from river import metrics
from river import preprocessing

# TODO:
#   1. Train a model that labels all 3 devices at once rather than one model per device.

# Training sample size.
TRAINING_SAMPLE_SIZE = 3000

# Verbosity for mispredictions during training.
DISPLAY_MISPREDICTION_DETAILS = False

# Photodiode elevation threshold for the Camera:
#   - FOV 18.63 deg (in lens specs) and 21 deg (in ICD)
#   - Elevation threshold is 90 deg - (FOV + margin) = 60 deg (1.0472 rad)
PD_ELEVATION_THRESHOLD_HD_CAM = 1.0472

# Photodiode elevation threshold for the Optical RX:
#  - FOV 12.2 deg (for 542 nm) and 82.65 deg (for optics, as per CAD model)
#  - PD elevation threshold is 90 deg - (82.65 deg / 2 FOV + 10 deg margin) = 38 deg (0.663225 rad)
PD_ELEVATION_THRESHOLD_OPT_RX = 0.6632

# Photodiode elevation threshold for the Startracker:
#   - FOV 33.1 deg, as per the CAD model
#   - Elevation threshold is 63 deg (1.09956 rad)
PD_ELEVATION_THRESHOLD_IADCS_ST = 1.0996

# The model that will be trained.
model = compose.Pipeline(
    preprocessing.StandardScaler(),
    linear_model.LogisticRegression())

# Metric to track accuracy as the model learns,
metric = metrics.Accuracy()

# Count how many mispredictions.
misprediction_counter = 0

class DeviceId(Enum):
    """Device id enum."""
    HD_CAMERA       = 0
    OPTICAL_RX      = 1
    STAR_TRACKER    = 2


class DeviceState(Enum):
    """Device state enum."""
    OFF = 0
    ON  = 1


def optical_device_target_state(device_id, elevation_angle):
    """Check if given device should be turned off given the elevation angle. """

    if device_id is DeviceId.HD_CAMERA and elevation_angle >= PD_ELEVATION_THRESHOLD_HD_CAM:
        return DeviceState.OFF
    
    elif device_id is DeviceId.OPTICAL_RX and elevation_angle >= PD_ELEVATION_THRESHOLD_OPT_RX:
        return DeviceState.OFF

    elif device_id is DeviceId.STAR_TRACKER and elevation_angle >= PD_ELEVATION_THRESHOLD_IADCS_ST:
        return DeviceState.OFF

    else:
        return DeviceState.ON


def train_optical_device_fdir(x, y):
    """The training function to apply when receiving photodiode elevation angle and device status."""

    # Indicate which global variables we will use.
    global model
    global metric
    global misprediction_counter

    # Change the input type.
    x = {'angle': x}

    # Change the expected label/output type.
    y = False if y is DeviceState.OFF else True

    # Make a prediction using the current state of the model.
    y_pred = model.predict_one(x)

    # Update the accuracy metric based on the predicted output versus the target output.
    metric = metric.update(y, y_pred)

    # Make the model learn from the fetched input and target output.
    model = model.learn_one(x, y)

    # Count and display mispredictions.
    if(y != y_pred):

        # Increment counter.
        misprediction_counter = misprediction_counter + 1

        # Verbosity.
        if DISPLAY_MISPREDICTION_DETAILS:
            print("    Angle: " + str(x['angle'])) 
            print("Predicted: " + ("ON" if y_pred==1 else "OFF")) 
            print(" Expected: " + ("ON" if y==1 else "OFF")) 
            print(" " + str(metric))
            print("\n")


if __name__ == "__main__":
    """The main function."""

    for i in progressbar.progressbar(range(TRAINING_SAMPLE_SIZE), redirect_stdout=True):
        # Get a random photodiode elevation angle value to use as a data input to train the model.
        # 
        # TODO: It would be much more intersting to use the FDIR output to train a model that uses photodiode 
        # input alternatives. We can still train with photodiodes inputs, of course, but let's not stop there.
        # 
        # Why would it be more interesting:
        #
        #   1. Replicating the FDIR with the same input values is a bit of a silly training considering that the possible 
        #      combinations are very limited with angles only ranging from 0 deg to 90 deg.
        #
        #   2. Training with photodiode inputs is furthermore simplistic when we consider that the labels have a very
        #      predictable pattern; a first sequence as ON and the rest as OFF. There's no seemingly random distribution.
        #
        #   3. We can present the exciting case that we are building a redundant system using different inputs in case of 
        #      photodiode failure(s).
        #
        #   4. In the example bellow, we only need to replace 'angle' with whatever input we want to train the model with 
        #      but still keep the expected/target label output returned by the FDIR. It's a small change to the experiment
        #      but with much more interesting potentials for study and comparative analysis. We could do it with quatenrions,
        #      sun vectors, and FSS.
        angle = math.radians(random.randint(0, 90))

        # Get HD Camera device status returned by the FDIR algorithm and use it as the target label expected by the model.
        # If we want to train a model for another device then change the device id here.
        label = optical_device_target_state(DeviceId.HD_CAMERA, angle)

        # Train the FDIR model.
        train_optical_device_fdir(angle, label)

    # Display misprediction count and rate.
    misprediction_percentage = round((misprediction_counter / TRAINING_SAMPLE_SIZE) * 100, 2)
    print("\nMispredictions: " + str(misprediction_counter) + "/" + str(TRAINING_SAMPLE_SIZE) + " (" + str(misprediction_percentage) + "%)\n")

