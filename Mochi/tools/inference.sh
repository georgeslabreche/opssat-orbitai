#!/bin/bash

# Process inference commands written in a file and use the results to calculate perfomance metrics of the models.
# Each line in the given file must contain a single command. First the train commands followed by the infer commands.
#
# The cmds/inferences_A.txt file used in this bash script was manually created by copying cmds/generated_A.txt and
# removing all training commands so that only inference commands are listed.

# Usage: ./inference.sh <float:sleep> <string:commands_filename>
#   - sleep: sleep time between each commands sent.
#   - commands_filename: path to the file listing all the commands to send to the Mochi server.
#
# e.g.: ./inference.sh 0.03 cmds/inferences_A.txt

if [ $# -ne 2 ]; then
    echo "Invalid number of parameters. Usage: ./inference.sh <float:sleep> <string:commands_filename>"
    exit 1
fi

# Clear the logs, for good measure.
rm logs/inference.csv
rm logs/training.csv

PORT=9999

# Start the Mochi server.
../OrbitAI_Mochi $PORT &

# Wait a bit to make sure that the Mochi server has started nicely.
sleep 1

# Load models and infer.
eval 'echo "load"; sleep 1; input="$2"; while IFS= read -r cmd; do sleep $1; echo ${cmd}; done < "$input"; echo "exit"' | telnet localhost $PORT

# Caculate classification metrics of the trained models.
python3 analyze.py logs/inference.csv metrics/ground/inference.csv






