# SEPP/MCS Linux command macro
# exp144 - MACRO 3: Downlink trained models and logs

SEPP:BL ls -R /home/exp144/toGround/ && tail -n 2 /home/exp144/toGround/learning/mochi-*/logs/training.csv && tail -n 2 /home/exp144/toGround/data/data_*.csv; mkdir /home/exp144/old_toGround
SEPP:BL tar -czvf /home/exp144/exp144_results_20210418B.tar.gz /home/exp144/toGround; ls -ltr /home/exp144/bin/Mochi/logs
SEPP:BL split /home/exp144/exp144_results_20210418B.tar.gz -b 3M /home/exp144/exp144_results_20210418B.tar.gz_ && mv /home/exp144/exp144_results_20210418B.tar.gz_* /home/root/esoc-apps/fms/filestore/toGround
SEPP:BL mv /home/exp144/toGround /home/exp144/old_toGround/; mv /home/exp144/bin/Mochi/logs/training.csv /home/exp144/old_toGround/; ls -ltR /home/exp144/
SEPP:BL eval 'for f in /home/exp144/bin/Mochi/models/*; do echo -e "\n\n${f}:\n"; cat ${f}; done'
