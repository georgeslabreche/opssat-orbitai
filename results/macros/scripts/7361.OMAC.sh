# SEPP/MCS Linux command macro
# exp144 - MACRO 3: Downlink trained models and logs

SEPP:BL ls -R /home/exp144/toGround/ && tail -n 2 /home/exp144/toGround/learning/mochi-*/logs/training.csv && tail -n 2 /home/exp144/toGround/data/data_*.csv; mkdir /home/exp144/old_toGround
SEPP:BL eval 'for f in /home/exp144/bin/Mochi/models/*; do echo -e "\n\n${f}:\n"; cat ${f}; done'; cat /home/exp144/bin/Mochi/logs/orbitai.log; df -h
