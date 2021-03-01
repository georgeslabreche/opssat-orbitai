"""
Simulates the OPS-SAT on-board FDIR algorithm to protect optical devices from exposure to sunlight
"""

from enum import Enum
import csv
import threading
import time
import sys
import os

# Photodiode 6 elevation threshold for the Camera:
#   - FOV 18.63 deg (in lens specs) and 21 deg (in ICD)
#   - Elevation threshold is 90 deg - (FOV + margin) = 60 deg (1.0472 rad)
PD_ELEVATION_THRESHOLD_HD_CAM = 1.0472

# Photodiode 6 elevation threshold for the Optical RX:
#  - FOV 12.2 deg (for 542 nm) and 82.65 deg (for optics, as per CAD model)
#  - PD elevation threshold is 90 deg - (82.65 deg / 2 FOV + 10 deg margin) = 38 deg (0.663225 rad)
PD_ELEVATION_THRESHOLD_OPT_RX = 0.6632

# Photodiode 3 elevation threshold for the Startracker:
#   - FOV 33.1 deg, as per the CAD model
#   - Elevation threshold is 63 deg (1.09956 rad)
PD_ELEVATION_THRESHOLD_IADCS_ST = 1.0996


class PhotoDiode(Enum):
    """Photodiode enum."""
    PD1 = 0
    PD2 = 1
    PD3 = 2
    PD4 = 3
    PD5 = 4
    PD6 = 5


class DeviceId(Enum):
    """Device id enum."""
    HD_CAMERA = 0
    OPTICAL_RX = 1
    STAR_TRACKER = 2


class DeviceState(Enum):
    """Device state enum."""
    OFF = 0
    ON = 1


class Device():
    """Device class."""
    device_id = None
    state = None
    threshold = None
    turn_off_timer = None

    def __init__(self, device_id, state, photodiode, threshold_angle):
        self.device_id = device_id
        self.state = state
        self.photodiode = photodiode
        self.threshold_angle = threshold_angle

    @property
    def name(self):
        """Returns the name of the device"""
        return self.device_id.name

    def turn_off(self):
        """Turn off the device."""
        print('Turn OFF ' + self.name)
        self.state = DeviceState.OFF

    def check(self, elevation_angle, callback_danger, callback_safe):
        """Check photodiode elevation angle against device threshold."""
        if elevation_angle > self.threshold_angle:
            callback_danger()
        else:
            callback_safe()

    def start_turn_off_timer(self):
        """Start the timer to turn off the device"""
        # It's not possible to know if the star tracker is on/off so act as if it always on
        # This is not a problem for the HD Camera and the Optical RX.
        if self.device_id == DeviceId.STAR_TRACKER or self.state == DeviceState.ON:
            if self.turn_off_timer is None or not self.turn_off_timer.is_alive():
                # Trigger timer.
                print('Start timer to turn OFF ' + self.name)
                self.turn_off_timer = threading.Timer(DEVICE_TURN_OFF_TIMER_DURATION, self.turn_off)
                self.turn_off_timer.start()

    def cancel_turn_off_timer(self):
        """Stop the timer to turn off the device"""
        if self.turn_off_timer is not None and self.turn_off_timer.is_alive():
            # Cancer timer
            print('Cancel timer to turn OFF ' + self.name)
            self.turn_off_timer.cancel()

    def wait_for_timer(self):
        """Waits for the internal timer to finish"""
        if self.turn_off_timer is not None and self.turn_off_timer.is_alive():
            self.turn_off_timer.join()


def create_opssat_devices():
    """Returns a list of 3 devices instances representing OPS-SAT optical devices"""
    return [Device(DeviceId.STAR_TRACKER, DeviceState.ON, PhotoDiode.PD3, PD_ELEVATION_THRESHOLD_IADCS_ST),
            Device(DeviceId.HD_CAMERA, DeviceState.ON, PhotoDiode.PD6, PD_ELEVATION_THRESHOLD_HD_CAM),
            Device(DeviceId.OPTICAL_RX, DeviceState.ON, PhotoDiode.PD6, PD_ELEVATION_THRESHOLD_OPT_RX)]


if __name__ == "__main__":
    # Path from where the script is called
    _CALLER_DIR_NAME = os.getcwd()

    # Path of the folder containing the script
    _DIR_NAME = os.path.dirname(__file__)

    # Path of the default input CSV file
    INPUT_FILE = os.path.join(_DIR_NAME, 'data/fdir_examples/inputs.csv')

    # Photodiode polling period (seconds).
    PD_POLLING_PERIOD = 1

    # Device turn off timer duration (seconds)
    DEVICE_TURN_OFF_TIMER_DURATION = 5

    # Check for python command arguments
    if len(sys.argv) == 4:
        try:
            # photodiode polling period (seconds)
            PD_POLLING_PERIOD = int(sys.argv[1])
            # device turn off timer duration (seconds)
            DEVICE_TURN_OFF_TIMER_DURATION = int(sys.argv[2])
            # relative path to input file
            INPUT_FILE = os.path.join(_CALLER_DIR_NAME, sys.argv[3])
        except:
            print('Error setting arguments, using default values.')
    elif len(sys.argv) != 1:
        print("Wrong number of arguments")
        exit(1)

    # Initialize the devices
    devices = create_opssat_devices()

    # Read a CSV file as photodiode reading inputs
    with open(INPUT_FILE, newline='') as csvfile:
        # Iterate each row
        for row in csv.DictReader(csvfile):
            for device in devices:
                device.check(float(row[device.name]),
                             device.start_turn_off_timer,
                             device.cancel_turn_off_timer)

            # Sleep in between photodiode value polling
            time.sleep(PD_POLLING_PERIOD)

    # Wait for all devices timer to finish
    for device in devices:
        device.wait_for_timer()

    # Show final devices state
    print("\nDevice - State")
    for device in devices:
        print(device.name + " - " + str(device.state.name))
