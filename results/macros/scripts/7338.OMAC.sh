# SEPP/MCS Linux command macro
# exp144 - pack and move 

SEPP:BL ls -ltR /home/exp144/toGround/ && tail -n 2 /home/exp144/toGround/learning/mochi-*/logs/training.csv && tail -n 2 /home/exp144/toGround/data/data_*.csv
SEPP:BL tar -czvf /home/exp144/exp144_results_20210417B.tar.gz /home/exp144/toGround
SEPP:BL split /home/exp144/exp144_results_20210417B.tar.gz -b 3M /home/exp144/exp144_results_20210417B.tar.gz_ && mv /home/exp144/exp144_results_20210417B.tar.gz_* /home/root/esoc-apps/fms/filestore/toGround
