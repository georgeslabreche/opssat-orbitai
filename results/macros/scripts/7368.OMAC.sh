# SEPP/MCS Linux command macro
# exp144 - MACRO 2: Switch to "Inference" Mode

SEPP:BL ls -R /home/exp144/toGround/ && tail -n 2 /home/exp144/toGround/learning/mochi-*/logs/training.csv && tail -n 2 /home/exp144/toGround/data/data_*.csv
SEPP:BL tar -czvf /home/exp144/exp144_results_20210419B.tar.gz /home/exp144/toGround; ls -ltr /home/exp144/bin/Mochi/logs; rm -f /home/root/esoc-apps/fms/filestore/toGroundLP/img_msec_1617*
SEPP:BL split /home/exp144/exp144_results_20210419B.tar.gz -b 1M /home/exp144/exp144_results_20210419B.tar.gz_ && mv /home/exp144/exp144_results_20210419B.tar.gz_* /home/root/esoc-apps/fms/filestore/toGround
SEPP:BL sed -i 's/=continue_train/=inference/g' /home/exp144/orbit_ai.properties; cat /home/exp144/orbit_ai.properties; tail /home/nmf/supervisor/log0.log.* -n 50; tail /home/exp144/log0.log.* -n 50
SEPP:BL rm -f /home/root/esoc-apps/fms/filestore/toGround/sepp_packetstore_exp510_201809*; rm -f /home/root/esoc-apps/fms/filestore/toGround/sepp_packetstore_exp510_20210413*
SEPP:BL rm -f /home/root/esoc-apps/fms/filestore/toGroundLP/img_msec_1616*; rm -f /home/root/esoc-apps/fms/filestore/toGroundLP/img_msec_1615*
SEPP:BL mv /home/root/esoc-apps/fms/filestore/toGround/sepp_packetstore_exp510_20210417_133116.gz /home/root/esoc-apps/fms/filestore/toGround/sepp_packetstore_exp510_20210417_133116_1.gz; echo "moved"
SEPP:BL mv /home/root/esoc-apps/fms/filestore/toGround/opssat_smartcam_jpeg_exp1000_20210418_065958_previous.tar.gz /home/root/esoc-apps/fms/filestore/toGround/opssat_smartcam_jpeg_exp1000_20210418_065958_1.tar.gz; echo "moved!"
SEPP:BL mv /home/root/esoc-apps/fms/filestore/toGround/sepp_packetstore_exp510_20210418_015959.gz /home/root/esoc-apps/fms/filestore/toGround/sepp_packetstore_exp510_20210418_015959_1.gz; echo "moved"
SEPP:BL mv /home/root/esoc-apps/fms/filestore/toGround/sepp_packetstore_exp510_20210417_192112.gz /home/root/esoc-apps/fms/filestore/toGround/sepp_packetstore_exp510_20210417_192112_1.gz; echo "moved"
SEPP:BL mv /home/root/esoc-apps/fms/filestore/toGround/sepp_packetstore_exp510_20210418_075957.gz /home/root/esoc-apps/fms/filestore/toGround/sepp_packetstore_exp510_20210418_075957_1.gz; echo "moved"
SEPP:BL mv /home/root/esoc-apps/fms/filestore/toGround/sepp_packetstore_exp510_20210418_13* /home/root/esoc-apps/fms/filestore/toGround/sepp_packetstore_exp510_20210418_1400.gz; echo "moved"
SEPP:BL mv /home/root/esoc-apps/fms/filestore/toGround/sepp_packetstore_exp510_20210418_19* /home/root/esoc-apps/fms/filestore/toGround/sepp_packetstore_exp510_20210418_2000.gz; df -h; echo "moved"
OMCS:BL /home/osops/os-tools/fms_upload_wrapper_simple.csh /home/osops/tmp updatedDataASTERIA.zip can
SEPP:BL md5sum /home/root/esoc-apps/fms/filestore/updatedDataASTERIA.zip | grep 9fc6f3d && mv /home/root/esoc-apps/fms/filestore/updatedDataASTERIA.zip /home/exp119/fromGround; ls -lart /home/exp119/fromGround
