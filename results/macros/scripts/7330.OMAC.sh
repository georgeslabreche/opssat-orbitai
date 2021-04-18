# SEPP/MCS Linux command macro
# move 1 image from exp1000 folder to png_store and toGroundLP folder
# copy 1 image from exp122 input image folder to toGroundLP folder
# common libraries for exp122 and exp130 to exp_shared_lib

SEPP:BL systemctl stop nmf_supervisor; opkg install --force-reinstall /home/root/opkg_pkg_repo/nmf-ops-sat_2.0.0-prerelease-10-g0b27984.ipk
OMCS sleep 30
SEPP:BL opkg status nmf-ops-sat; sed -i 's/.*helpertools.configurations.pollgps.*/helpertools.configurations.pollgps=false/' /home/nmf/supervisor/provider.properties; cat /home/nmf/supervisor/provider.properties
SEPP:BL systemctl restart nmf_supervisor; echo 'Restarting NMF supervisor after disabling GPS...'
SEPP:BL sed -i 's/iterations=1000/iterations=850/g' /home/exp144/orbit_ai.properties; cat /home/exp144/orbit_ai.properties
SEPP:BL date; cp /home/exp122/imagesToProcessOffline/img_msec_1618086593411_2.png /home/root/esoc-apps/fms/filestore/toGroundLP/
SEPP:BL chown root:root /home/root/esoc-apps/fms/filestore/toGroundLP/img_msec_1618086593411_2.png
SEPP:BL mv /home/exp1000/toGround/earth/img_msec_1618324718129_2.png /home/root/png_store/; cp /home/root/png_store/img_msec_1618324718129_2.png /home/root/esoc-apps/fms/filestore/toGroundLP/; echo "moved"; mkdir -p /home/exp_shared_lib
SEPP:BL cp /home/exp122/lib/common* /home/exp_shared_lib/; cp /home/exp122/lib/jai* /home/exp_shared_lib/
SEPP:BL chown root:root /home/exp_shared_lib/*; chmod 664 /home/exp_shared_lib/*;ls -al /home/exp_shared_lib
 
