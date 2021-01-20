#!/bin/bash

# How to run this test:
#   1. Start the Online ML server: ./OrbitAI_Mochi 
#   2. Send telnet commands to the server: ./test_server.sh | telnet localhost 9999
#   3. Monitor the model file update: watch -n 0.1 cat models/arow_2D

# Send training commands.
input="../sandbox/fdir/data/svm/train/camera_2d_Xpd3-Ypd6_none.svmdata"
while IFS= read -r line
do
    sleep 0.1
    echo "train ${line/+/''}"
    sleep 0.1
    echo "save"
done < "$input"

echo "exit"

