# SEPP/MCS Linux command macro
# exp144 - pack and move 

SEPP:BL sed -i 's/iterations=850/iterations=540/g' /home/exp144/orbit_ai.properties; cat /home/exp144/orbit_ai.properties; df -h; du -sh /home/exp1000/toGround/*
SEPP:BL ls -ltR /home/exp144/toGround/ && tail -n 2 /home/exp144/toGround/learning/mochi-*/logs/training.csv && tail -n 2 /home/exp144/toGround/data/data_*.csv
SEPP:BL tar -czvf /home/exp144/exp144_results_20210417C.tar.gz /home/exp144/toGround
SEPP:BL split /home/exp144/exp144_results_20210417C.tar.gz -b 3M /home/exp144/exp144_results_20210417C.tar.gz_ && mv /home/exp144/exp144_results_20210417C.tar.gz_* /home/root/esoc-apps/fms/filestore/toGround
SEPP:BL cp /home/exp1000/toGround/earth/img_msec_1618348990853_2.png /home/root/esoc-apps/fms/filestore/toGroundLP/; cp /home/exp1000/toGround/earth/img_msec_1618324718129_2.png /home/root/esoc-apps/fms/filestore/toGroundLP/; echo "done"
SEPP:BL cp /home/exp1000/toGround/earth/img_msec_1618324672604_2.png /home/root/esoc-apps/fms/filestore/toGroundLP/; cp /home/exp1000/toGround/earth/img_msec_1618147911653_2.png /home/root/esoc-apps/fms/filestore/toGroundLP/; echo "done"
SEPP:BL cp /home/exp1000/toGround/earth/img_msec_1618348990853_2.png /home/root/png_store/; cp /home/exp1000/toGround/earth/img_msec_1618324718129_2.png /home/root/png_store/; echo "done"
SEPP:BL cp /home/exp1000/toGround/earth/img_msec_1618324672604_2.png /home/root/png_store/; cp /home/exp1000/toGround/earth/img_msec_1618147911653_2.png /home/root/png_store/; ls -ltR /home/exp1000/toGround/; echo "done"
