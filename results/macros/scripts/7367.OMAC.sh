# SEPP/MCS Linux command macro
# exp144 - MACRO 3: Downlink trained models and logs

SEPP:BL ls -R /home/exp144/toGround/ && tail -n 2 /home/exp144/toGround/learning/mochi-*/logs/training.csv && tail -n 2 /home/exp144/toGround/data/data_*.csv; ls -ltr /home/root/esoc-apps/fms/filestore/toGround/
SEPP:BL tar -czvf /home/exp144/exp144_results_20210419A.tar.gz /home/exp144/toGround; ls -ltr /home/exp144/bin/Mochi/logs; rm -f /home/root/esoc-apps/fms/filestore/toGroundLP/img_msec_1617*
SEPP:BL split /home/exp144/exp144_results_20210419A.tar.gz -b 1M /home/exp144/exp144_results_20210419A.tar.gz_ && mv /home/exp144/exp144_results_20210419A.tar.gz_* /home/root/esoc-apps/fms/filestore/toGround
SEPP:BL eval 'for f in /home/exp144/bin/Mochi/models/*; do echo -e "\n\n${f}:\n"; cat ${f}; done'; cat /home/exp144/bin/Mochi/logs/orbitai.log; df -h; ls -ltr /home/root/esoc-apps/fms/filestore/toGroundLP/
SEPP:BL rm -f /home/root/esoc-apps/fms/filestore/toGround/sepp_packetstore_exp510_201809*; rm -f /home/root/esoc-apps/fms/filestore/toGround/sepp_packetstore_exp510_20210413*
SEPP:BL rm -f /home/root/esoc-apps/fms/filestore/toGroundLP/img_msec_1616*; rm -f /home/root/esoc-apps/fms/filestore/toGroundLP/img_msec_1615*; tail /home/nmf/supervisor/log0.log.* -n 50; tail /home/exp144/log0.log.* -n 50
SEPP:BL mv /home/root/esoc-apps/fms/filestore/toGround/sepp_packetstore_exp510_20210417_133116.gz /home/root/esoc-apps/fms/filestore/toGround/sepp_packetstore_exp510_20210417_133116_1.gz; echo "moved"
SEPP:BL mv /home/root/esoc-apps/fms/filestore/toGround/opssat_smartcam_jpeg_exp1000_20210418_065958_previous.tar.gz /home/root/esoc-apps/fms/filestore/toGround/opssat_smartcam_jpeg_exp1000_20210418_065958_1.tar.gz; echo "moved!"
SEPP:BL mv /home/root/esoc-apps/fms/filestore/toGround/sepp_packetstore_exp510_20210418_015959.gz /home/root/esoc-apps/fms/filestore/toGround/sepp_packetstore_exp510_20210418_015959_1.gz; echo "moved"
SEPP:BL mv /home/root/esoc-apps/fms/filestore/toGround/sepp_packetstore_exp510_20210417_192112.gz /home/root/esoc-apps/fms/filestore/toGround/sepp_packetstore_exp510_20210417_192112_1.gz; echo "moved"
SEPP:BL mv /home/root/esoc-apps/fms/filestore/toGround/sepp_packetstore_exp510_20210418_075957.gz /home/root/esoc-apps/fms/filestore/toGround/sepp_packetstore_exp510_20210418_075957_1.gz; echo "moved"
SEPP:BL mv /home/root/esoc-apps/fms/filestore/toGround/sepp_packetstore_exp510_20210418_13* /home/root/esoc-apps/fms/filestore/toGround/sepp_packetstore_exp510_20210418_1400.gz; echo "moved"
SEPP:BL mv /home/root/esoc-apps/fms/filestore/toGround/sepp_packetstore_exp510_20210418_19* /home/root/esoc-apps/fms/filestore/toGround/sepp_packetstore_exp510_20210418_2000.gz; df -h; echo "moved"
