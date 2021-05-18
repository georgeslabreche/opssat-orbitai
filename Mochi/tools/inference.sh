#!/bin/bash

# Process inference commands written in a file and use the results to calculate perfomance metrics of the models.
# Each line in the given file must contain a single command. First the train commands followed by the infer commands.
#
# The cmds/inferences_A.txt file used in this bash script was manually created by copying cmds/generated_A.txt and
# removing all training commands so that only inference commands are listed.

# Usage: ./inference.sh <int:analyze_5d_flag> <float:sleep> <string:commands_filename> <string:csv_results_filname>
#   - analyze_5d_flag: Flag to indicate if classification metrics should be calculated for models trained in 5D input space.
#                      1 for True or 0 for False.
#   - sleep: sleep time between each commands sent.
#   - commands_filename: path to the file listing all the commands to send to the Mochi server.
#   - csv_results_filname: path of the CSV file in which the inference results will be written.
#
# e.g.: ./inference.sh 0 0.03 cmds/inferences_A.txt metrics/space/inferences_2021-05-15.csv

if [ $# -ne 4 ]; then
    echo "Invalid number of parameters. Usage: ./inference.sh <float:sleep> <string:commands_filename>"
    exit 1
fi

# Clear the logs, for good measure.
rm -r logs
mkdir logs

PORT=9999

# Start the Mochi server.
../OrbitAI_Mochi $PORT &

# Wait a bit to make sure that the Mochi server has started nicely.
sleep 1

# Load models and infer.
eval 'echo "load"; sleep 1; input="$3"; while IFS= read -r cmd; do sleep $2; echo ${cmd}; done < "$input"; echo "exit"' | telnet localhost $PORT

# Caculate classification metrics of the trained models.
python3 analyze.py $1 logs/inference.csv $4






