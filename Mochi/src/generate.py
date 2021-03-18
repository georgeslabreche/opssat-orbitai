import time
import csv
from datetime import datetime

'''
This script generates training commands that can be sent a running Mochi server.
From the Mochi root directory, the list of generated commands are saved in:
- test_data/generated_A.txt
- test_data/generated_B.txt
- test_data/generated_C.txt

From the Mochi root directory:

1. Generated the training commands: python3 src/generate.py
2. Start the Online ML server: ./OrbitAI_Mochi
3. Train the models: eval 'echo "reset"; sleep 1; input="test_data/generated_A.txt"; while IFS= read -r cmd; do sleep 0.02; echo ${cmd}; done < "$input";' | telnet localhost 9999
4. Check that training log file was created: logs/training.csv
5. Calculate inference/prediction accuracies: python3 src/accuracy.py
'''

# Write training and inference commands from generated data.
def write_commands_from_generated_data(output_filepath, start=0, end=1.57, step=0.01, include_decrement_data=True):

    # Photodiode 6 elevation threshold for the Camera:
    #   - FOV 18.63 deg (in lens specs) and 21 deg (in ICD)
    #   - Elevation threshold is 90 deg - (FOV + margin) = 60 deg (1.0472 rad)
    PD_ELEVATION_THRESHOLD_HD_CAM = 1.0472

    # Min elevation angle measurement is 0 degrees.
    MIN_PD6 = 0

    # Max elevation angle measurement is 90 degrees, i.e. 1.57 radian.
    MAX_PD6 = 1.57

    if start < 0:
        print("Start PD6 elevation angle measurement cannot be less than 0 radians.")
        exit(0)

    if start > MAX_PD6:
        print("Start PD6 elevation angle measurement cannot be greater than 1.57 radians.")
        exit(0)

    if end > MAX_PD6:
        print("End PD6 elevation angle measurement cannot be greater than 1.57 radians.")
        exit(0)

    pd6 = start

    # Start time in milliseconds.
    # To simulate data input for logging photodiode value acquisition time.
    start_time = int(time.time() * 1000)

    is_incrementing = True

    # Generate train and infer commands using made up data.
    with open(output_filepath, "w") as data_file: 

        # Generate training commands incrementing PD6 elevation angles from 0 to 90 and then decrementing back to 0.
        while True:

            # Make sure training photodiode data only has 2 digits after the floating point.
            pd6 = round(pd6, 2)

            if pd6 >= 0:

                # Label '1' means camera can be turned ON.
                # Label '-1' means camera must be turned OFF.
                label = 1 if pd6 < PD_ELEVATION_THRESHOLD_HD_CAM else -1

                # Generate train command and write it.
                # Use dummy values for pd1 through pd5.
                cmd = 'train ' + str(label) + ' 0.11 0.22 0.33 0.44 0.55 ' + str(pd6) + ' ' + str(start_time) + '\n'
                data_file.write(cmd)
            
            # Exit loop after decrement phase is done.
            if pd6 < 0:
                break

            # Increment phase.
            elif pd6 < MAX_PD6 and is_incrementing:

                # Increment photodiode elevation angle value.
                pd6 += step

            # Decrement phase.
            elif include_decrement_data:
                # Start decrementing.
                is_incrementing = False
                
                # Decrement photodiode elevation angle value.
                pd6 -= step

            # Exit loop if we're not including decremented values.
            else:
                break

            # Photodiode data acquisition is planned for every 5 seconds.
            start_time += 5000 

        # Generate inference commands incrementing from 0 to 90 for PD6 elevation angle values
        pd6 = MIN_PD6

        while True:

            # Make sure training photodiode data only has 2 digits after the floating point.
            pd6 = round(pd6, 2)

            # Label '1' means camera can be turned ON.
            # Label '-1' means camera must be turned OFF.
            label = 1 if pd6 < PD_ELEVATION_THRESHOLD_HD_CAM else -1

            # Generate train command and write it.
            # Use dummy values for pd1 through pd5.
            cmd = 'infer ' + str(label) + ' 0.11 0.22 0.33 0.44 0.55 ' + str(pd6) + ' ' + str(start_time) + '\n'
            data_file.write(cmd)

            # Photodiode data acquisition is planned for every 5 seconds.
            start_time += 5000

            # Increment photodiode elevation angle value.
            if pd6 < MAX_PD6:
                pd6 += step

            # Exit loop
            else:
                break



# Write commands from webmust data.
def write_commands_from_webmust_data(output_filepath, is_partial=True, skip_rows=False, skip_num=5):

    # Path of the WebMUST data files.
    WEBMUST_DATA_FILEPATH_FULL = '../sandbox/fdir/data/webmust_labeled/perfect_full_training_set.csv'
    WEBMUST_DATA_FILEPATH_PARTIAL = '../sandbox/fdir/data/webmust_labeled/perfect_training_set.csv'

    # Select which one to use.
    WEBMUST_DATA_FILEPATH = WEBMUST_DATA_FILEPATH_PARTIAL if is_partial else WEBMUST_DATA_FILEPATH_FULL

    # Data fetched from WebMUST.
    with open(WEBMUST_DATA_FILEPATH) as csvfile:
        reader = csv.DictReader(csvfile) 

        # The row counter.
        row_index = 0

        cmd_args_list = []

        # Skip head
        for row in reader:

            # Convert timestamp to milliseconds.
            timestamp_obj = datetime.strptime(row['TIMESTAMP'],
                        '%Y-%m-%d %H:%M:%S')
            timestamp_millisec = int(timestamp_obj.timestamp() * 1000)

            # Command arguments for the train or infer commands
            cmd_args = ''

            if is_partial:
                # Generate train command and write it.
                # Use dummy values for pd1, pd2, pd4, and pd5 because they are expected by the command but not available in the partial WebMUST dataset.
                cmd_args = row['HD_CAMERA_STATE'] + ' 0.11 0.22 ' + str(round(float(row['PD3']), 2)) + ' 0.44 0.55 ' + str(round(float(row['PD6']), 2)) + ' ' + str(timestamp_millisec) + '\n'

            else:
                # Generate train command and write it.
                cmd_args = row['HD_CAMERA_STATE'] + ' ' + row['PD1'] + ' ' + row['PD2'] + ' ' + row['PD3'] + ' ' + row['PD4'] + ' ' + row['PD5'] + ' ' + row['PD6'] + ' ' + str(timestamp_millisec) + '\n'

            # The photodiode elevation angle value does not change for the first few seconds.
            # Only grab row data so that we have a training set that represent 5 second increments instead of every second.
            if skip_rows is True:
                if row_index % skip_num == 0:
                    cmd_args_list.append(cmd_args)

            else:
                cmd_args_list.append(cmd_args)
                
            # Increment row counter.
            row_index += 1

    # determine sample size of training data vs sample size of test data
    sample_size_training = int(0.3 * len(cmd_args_list))
    sample_size_testing = len(cmd_args_list) - sample_size_training

    # Generate train and inference test commands using WebMUST data.
    with open(output_filepath, "w") as data_file:
        for i in range(sample_size_training+1):
            data_file.write('train ' + cmd_args_list[i])

        for j in range(i+1, sample_size_testing+1):
            data_file.write('infer ' + cmd_args_list[j])


# Main application function.
if __name__ == "__main__":

    # Write commands from generated data.
    write_commands_from_generated_data("test_data/generated_A.txt", 0.7, 1.3, 0.01, False)
    write_commands_from_generated_data("test_data/generated_B.txt", 0, 1.57, 0.01, False)
    write_commands_from_generated_data("test_data/generated_C.txt", 0, 1.57, 0.01, True)