# SEPP/MCS Linux command macro
# exp144 - MACRO 3: Downlink trained models and logs

SEPP:BL ls -R /home/exp144/toGround/ && tail -n 2 /home/exp144/toGround/learning/mochi-*/logs/training.csv && tail -n 2 /home/exp144/toGround/data/data_*.csv
SEPP:BL tar -czvf /home/exp144/exp144_results_20210418A.tar.gz /home/exp144/toGround; ls -ltr /home/exp144/bin/Mochi/logs
SEPP:BL split /home/exp144/exp144_results_20210418A.tar.gz -b 3M /home/exp144/exp144_results_20210418A.tar.gz_ && mv /home/exp144/exp144_results_20210418A.tar.gz_* /home/root/esoc-apps/fms/filestore/toGround
SEPP:BL cp /home/exp1000/toGround/earth/img_msec_1618564160675_2.png /home/root/esoc-apps/fms/filestore/toGroundLP/; cp /home/exp1000/toGround/earth/img_msec_1618564195957_2.png /home/root/esoc-apps/fms/filestore/toGroundLP/; echo "done"
SEPP:BL cp /home/exp1000/toGround/earth/img_msec_1618564231395_2.png /home/root/esoc-apps/fms/filestore/toGroundLP/; cp /home/exp1000/toGround/earth/img_msec_1618564268835_2.png /home/root/esoc-apps/fms/filestore/toGroundLP/; echo "done"
