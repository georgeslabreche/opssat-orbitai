# SEPP/MCS Linux command macro
# exp144 - Downlink trained models and logs

SEPP:BL ls -ltR /home/exp144/toGround/ && tail -n 2 /home/exp144/toGround/learning/mochi-*/logs/training.csv && tail -n 2 /home/exp144/toGround/data/data_*.csv; ls -ltr /home/root/esoc-apps/fms/filestore/toGround/
SEPP:BL tar -czvf /home/exp144/exp144_results_20210420A.tar.gz /home/exp144/toGround; ls -ltr /home/exp144/bin/Mochi/logs; mv /home/exp144/exp144_results_20210419_old.tar.gz /home/root/esoc-apps/fms/filestore/toGround/; echo "moved"
SEPP:BL mv /home/exp144/exp144_results_20210420A.tar.gz /home/root/esoc-apps/fms/filestore/toGround/; df -h; du -sh /home/*; du -sh /home/root/*; du -sh /home/root/esoc-apps/fms/filestore/*; ps -ef | grep exp 
SEPP:BL eval 'for f in /home/exp144/bin/Mochi/models/*; do echo -e "\n\n${f}:\n"; cat ${f}; done'; cat /home/exp144/bin/Mochi/logs/orbitai.log; df -h; ls -ltr /home/root/esoc-apps/fms/filestore/toGround/
