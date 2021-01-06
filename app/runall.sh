#!/bin/bash

# declare an array of transformation functions for 2 dimensional inputs.
declare -a arr_2D=("none" "linear" "polynomial" "rbf")

# cd into the classification example folder.
cd MochiMochi/examples/binary_classifier


################################
# ADAM
################################
cd ADAM

for i in "${arr_2D[@]}"
do
   echo "--------------------------------"
   echo "> ADAM > 2D > $i"
   time ./adam.out --dim 2 --train ../../../../../sandbox/fdir-algorithm/data/svm/train/camera_2d_$i.svmdata --test ../../../../../sandbox/fdir-algorithm/data/svm/test/camera_2d_$i.svmdata
done

# 6 dimension feature space
echo "--------------------------------"
echo "> ADAM > 6D"
time ./adam.out --dim 6 --train ../../../../../sandbox/fdir-algorithm/data/svm/train/camera_6d_from_2d_A.svmdata --test ../../../../../sandbox/fdir-algorithm/data/svm/test/camera_6d_from_2d_A.svmdata


################################
# RDA
################################
cd ../adagrad_rda

for i in "${arr_2D[@]}"
do
   echo "--------------------------------"
   echo "> RDA > 2D > $i"
   time ./rda.out --dim 2 --train ../../../../../sandbox/fdir-algorithm/data/svm/train/camera_2d_$i.svmdata --test ../../../../../sandbox/fdir-algorithm/data/svm/test/camera_2d_$i.svmdata --eta 0.1 --lambda 0.000001
done

# 6 dimension feature space
echo "--------------------------------"
echo "> RDA > 6D"
time ./rda.out --dim 6 --train ../../../../../sandbox/fdir-algorithm/data/svm/train/camera_6d_from_2d_A.svmdata --test ../../../../../sandbox/fdir-algorithm/data/svm/test/camera_6d_from_2d_A.svmdata --eta 0.1 --lambda 0.000001


################################
# AROW
################################
cd ../arow

for i in "${arr_2D[@]}"
do
   echo "--------------------------------"
   echo "> AROW > 2D > $i"
   time ./arow.out --dim 2 --train ../../../../../sandbox/fdir-algorithm/data/svm/train/camera_2d_$i.svmdata --test ../../../../../sandbox/fdir-algorithm/data/svm/test/camera_2d_$i.svmdata --r 0.8
done

# 6 dimension feature space
echo "--------------------------------"
echo "> AROW > 6D"
time ./arow.out --dim 6 --train ../../../../../sandbox/fdir-algorithm/data/svm/train/camera_6d_from_2d_A.svmdata --test ../../../../../sandbox/fdir-algorithm/data/svm/test/camera_6d_from_2d_A.svmdata --r 0.8


################################
# SCW
################################
cd ../scw

for i in "${arr_2D[@]}"
do
   echo "--------------------------------"
   echo "> SCW > 2D > $i"
   time ./scw.out --dim 2 --train ../../../../../sandbox/fdir-algorithm/data/svm/train/camera_2d_$i.svmdata --test ../../../../../sandbox/fdir-algorithm/data/svm/test/camera_2d_$i.svmdata --c 0.1 --eta 0.95
done

# 6 dimension feature space
echo "--------------------------------"
echo "> SCW > 6D"
time ./scw.out --dim 6 --train ../../../../../sandbox/fdir-algorithm/data/svm/train/camera_6d_from_2d_A.svmdata --test ../../../../../sandbox/fdir-algorithm/data/svm/test/camera_6d_from_2d_A.svmdata --c 0.1 --eta 0.9


################################
# NHERD
################################
cd ../nherd

for i in "${arr_2D[@]}"
do
   echo "--------------------------------"
   echo "> NHERD > 2D > $i"
   time ./nherd.out --dim 2 --train ../../../../../sandbox/fdir-algorithm/data/svm/train/camera_2d_$i.svmdata --test ../../../../../sandbox/fdir-algorithm/data/svm/test/camera_2d_$i.svmdata --c 0.1 --diagonal 0
done

# 6 dimension feature space
echo "--------------------------------"
echo "> NHERD > 6D"
time ./nherd.out --dim 6 --train ../../../../../sandbox/fdir-algorithm/data/svm/train/camera_6d_from_2d_A.svmdata --test ../../../../../sandbox/fdir-algorithm/data/svm/test/camera_6d_from_2d_A.svmdata --c 0.1 --diagonal 0


################################
# PA
################################
cd ../pa

for i in "${arr_2D[@]}"
do
   echo "--------------------------------"
   echo "> PA > 2D > $i"
   time ./pa.out --dim 2 --train ../../../../../sandbox/fdir-algorithm/data/svm/train/camera_2d_$i.svmdata --test ../../../../../sandbox/fdir-algorithm/data/svm/test/camera_2d_$i.svmdata --c 0.1 --select 1
done

# 6 dimension feature space
echo "--------------------------------"
echo "> PA > 6D"
time ./pa.out --dim 6 --train ../../../../../sandbox/fdir-algorithm/data/svm/train/camera_6d_from_2d_A.svmdata --test ../../../../../sandbox/fdir-algorithm/data/svm/test/camera_6d_from_2d_A.svmdata --c 0.1 --select 1