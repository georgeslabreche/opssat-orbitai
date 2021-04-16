# SEPP/MCS Linux command macro
# exp144 - MACRO 2: Switch to "Inference" Mode

SEPP:BL ls -R /home/exp144/toGround/ && tail -n 2 /home/exp144/toGround/learning/mochi-*/logs/training.csv && tail -n 2 /home/exp144/toGround/data/data_*.csv
SEPP:BL sed -i 's/=continue_train/=inference/g' /home/exp144/orbit_ai.properties; cat /home/exp144/orbit_ai.properties
