"""
Takes a CSV file from webmust containg the 6 photodiodes values and creates labeled file.
The labels consist of 0 (off) or 1 (on).
"""

import os
import sys
from fdir import create_opssat_devices


NEXT_LABEL = None


def label_off():
    """Sets the next label to the OFF value"""
    global NEXT_LABEL
    NEXT_LABEL = "0"


def label_on():
    """Sets the next label to the ON value"""
    global NEXT_LABEL
    NEXT_LABEL = "1"


def process_webmust_csv(csv_file, devices):
    """Parses a webmust CSV file with the 6 photodiodes values and return new formatted and labeled lines"""
    # Create first line with column names
    global NEXT_LABEL
    output_lines = [",".join([d.name for d in devices] + [d.name + "_state" for d in devices]) + "\n"]

    # Skip header
    nb_line_header = 21
    for i in range(nb_line_header):
        csv_file.readline()

    # Process lines with values
    line = csv_file.readline()
    while line:
        # Get diodes values
        line_splitted = line.split(",")
        diode3 = line_splitted[3]
        diode6 = line_splitted[6].strip('\n')

        if diode6 != " " and diode3 != " ":
            # Create new line with diode values
            new_line = [diode6] + [diode6] + [diode3]

            # Append labels for each device
            for i in range(len(new_line)):
                devices[i].check(float(new_line[i]), label_off, label_on)
                new_line.append(NEXT_LABEL)

            output_lines.append(",".join(new_line) + "\n")

        line = csv_file.readline()

    return output_lines


if __name__ == "__main__":
    # Path from where the script is called
    _CALLER_DIR_NAME = os.getcwd()

    # Path of the folder containing the script
    _DIR_NAME = os.path.dirname(__file__)

    # Path of the default input CSV file
    INPUT_FILE = os.path.join(_DIR_NAME, 'data/webmust/I_PD_THETA.csv')

    # Check for python command arguments
    if len(sys.argv) == 2:
        try:
            # relative path to input file
            INPUT_FILE = os.path.join(_CALLER_DIR_NAME, sys.argv[1])
        except:
            print('Error setting arguments, using default values.')

    # Compute output file path
    OUTPUT_FILE = os.path.join(_DIR_NAME, 'data/webmust_labeled/', "labeled_" + os.path.basename(INPUT_FILE))

    # Initialize the devices
    devices = create_opssat_devices()

    # Read a webmust CSV file with photodiode reading inputs
    with open(INPUT_FILE, 'r') as input_file:
        # Write the labeled file in another CSV file
        with open(OUTPUT_FILE, 'w') as output_file:
            output_file.writelines(process_webmust_csv(input_file, devices))
