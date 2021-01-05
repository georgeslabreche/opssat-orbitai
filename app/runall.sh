#!/bin/bash

# declare an array of transformation functions for 2 dimensional inputs.
declare -a arr_2D=("none" "linear" "polynomial" "rbf")

# cd into the classification example folder.
cd MochiMochi/examples/binary_classifier

# ADAM
########
cd ADAM

for i in "${arr_2D[@]}"
do
   echo "--------------------------------"
   echo "> ADAM > $i"
   time ./adam.out --dim 2 --train ../../../../../sandbox/fdir-algorithm/data/svm/train/camera_2d_$i.svmdata --test ../../../../../sandbox/fdir-algorithm/data/svm/test/camera_2d_$i.svmdata
done


# RDA
########
cd ../adagrad_rda

for i in "${arr_2D[@]}"
do
   echo "--------------------------------"
   echo "> RDA > $i"
   time ./rda.out --dim 2 --train ../../../../../sandbox/fdir-algorithm/data/svm/train/camera_2d_$i.svmdata --test ../../../../../sandbox/fdir-algorithm/data/svm/test/camera_2d_$i.svmdata --eta 0.1 --lambda 0.000001
done


# AROW
########
cd ../arow

for i in "${arr_2D[@]}"
do
   echo "--------------------------------"
   echo "> AROW > $i"
   time ./arow.out --dim 2 --train ../../../../../sandbox/fdir-algorithm/data/svm/train/camera_2d_$i.svmdata --test ../../../../../sandbox/fdir-algorithm/data/svm/test/camera_2d_$i.svmdata --r 0.8
done

# SCW
########
cd ../scw

for i in "${arr_2D[@]}"
do
   echo "--------------------------------"
   echo "> SCW > $i"
   time ./scw.out --dim 2 --train ../../../../../sandbox/fdir-algorithm/data/svm/train/camera_2d_$i.svmdata --test ../../../../../sandbox/fdir-algorithm/data/svm/test/camera_2d_$i.svmdata --c 0.1 --eta 0.95
done

# NHERD
########
cd ../nherd

for i in "${arr_2D[@]}"
do
   echo "--------------------------------"
   echo "> NHERD > $i"
   time ./nherd.out --dim 2 --train ../../../../../sandbox/fdir-algorithm/data/svm/train/camera_2d_$i.svmdata --test ../../../../../sandbox/fdir-algorithm/data/svm/test/camera_2d_$i.svmdata --c 0.1 --diagonal 0
done

# PA
########
cd ../pa

for i in "${arr_2D[@]}"
do
   echo "--------------------------------"
   echo "> PA > $i"
   time ./pa.out --dim 2 --train ../../../../../sandbox/fdir-algorithm/data/svm/train/camera_2d_$i.svmdata --test ../../../../../sandbox/fdir-algorithm/data/svm/test/camera_2d_$i.svmdata --c 0.1 --select 1
done



