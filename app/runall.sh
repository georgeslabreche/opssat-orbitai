#!/bin/bash

# declare an array of transformation functions for 2 dimensional inputs.
declare -a arr_2D_from_2D=("none" "linear" "polynomial" "rbf")

declare -a arr_2D_from_1D=("power" "modulo")

# cd into the classification example folder.
cd MochiMochi/examples/binary_classifier


################################
# ADAM
################################
cd ADAM

# 1 dimension feature space
echo "--------------------------------"
echo "> ADAM > 1D"
time ./adam.out --dim 1 --train ../../../../../sandbox/fdir-algorithm/data/svm/train/camera_1d.svmdata --test ../../../../../sandbox/fdir-algorithm/data/svm/test/camera_1d.svmdata

# 2 dimension feature space obtained from transforming 1D input space to higher dimension
for i in "${arr_2D_from_1D[@]}"
do
   echo "--------------------------------"
   echo "> ADAM > 1D to 2D > $i"
   time ./adam.out --dim 2 --train ../../../../../sandbox/fdir-algorithm/data/svm/train/camera_2d_from_1d_A_$i.svmdata --test ../../../../../sandbox/fdir-algorithm/data/svm/test/camera_2d_from_1d_A_$i.svmdata
done

# 2 dimension feature space
for i in "${arr_2D_from_2D[@]}"
do
   echo "--------------------------------"
   echo "> ADAM > 2D > $i"
   time ./adam.out --dim 2 --train ../../../../../sandbox/fdir-algorithm/data/svm/train/camera_2d_$i.svmdata --test ../../../../../sandbox/fdir-algorithm/data/svm/test/camera_2d_$i.svmdata
done

# 6 dimension feature space
echo "--------------------------------"
echo "> ADAM > 2D to 6D"
time ./adam.out --dim 6 --train ../../../../../sandbox/fdir-algorithm/data/svm/train/camera_6d_from_2d_A.svmdata --test ../../../../../sandbox/fdir-algorithm/data/svm/test/camera_6d_from_2d_A.svmdata


################################
# RDA
################################
cd ../adagrad_rda

# 1 dimension feature space
echo "--------------------------------"
echo "> RDA > 1D"
time ./rda.out --dim 1 --train ../../../../../sandbox/fdir-algorithm/data/svm/train/camera_1d.svmdata --test ../../../../../sandbox/fdir-algorithm/data/svm/test/camera_1d.svmdata --eta 0.1 --lambda 0.000001

# 2 dimension feature space obtained from transforming 1D input space to higher dimension
for i in "${arr_2D_from_1D[@]}"
do
   echo "--------------------------------"
   echo "> RDA > 1D to 2D > $i"
   time ./rda.out --dim 2 --train ../../../../../sandbox/fdir-algorithm/data/svm/train/camera_2d_from_1d_A_$i.svmdata --test ../../../../../sandbox/fdir-algorithm/data/svm/test/camera_2d_from_1d_A_$i.svmdata --eta 0.1 --lambda 0.000001
done

# 2 dimension feature space
for i in "${arr_2D_from_2D[@]}"
do
   echo "--------------------------------"
   echo "> RDA > 2D > $i"
   time ./rda.out --dim 2 --train ../../../../../sandbox/fdir-algorithm/data/svm/train/camera_2d_$i.svmdata --test ../../../../../sandbox/fdir-algorithm/data/svm/test/camera_2d_$i.svmdata --eta 0.1 --lambda 0.000001
done

# 6 dimension feature space
echo "--------------------------------"
echo "> RDA > 2D --> 2D to 6D"
time ./rda.out --dim 6 --train ../../../../../sandbox/fdir-algorithm/data/svm/train/camera_6d_from_2d_A.svmdata --test ../../../../../sandbox/fdir-algorithm/data/svm/test/camera_6d_from_2d_A.svmdata --eta 0.1 --lambda 0.000001


################################
# AROW
################################
cd ../arow

# 1 dimension feature space
echo "--------------------------------"
echo "> AROW > 1D"
time ./arow.out --dim 6 --train ../../../../../sandbox/fdir-algorithm/data/svm/train/camera_1d.svmdata --test ../../../../../sandbox/fdir-algorithm/data/svm/test/camera_1d.svmdata --r 0.8

# 2 dimension feature space obtained from transforming 1D input space to higher dimension
for i in "${arr_2D_from_1D[@]}"
do
   echo "--------------------------------"
   echo "> AROW > 1D to 2D > $i"
   time ./arow.out --dim 2 --train ../../../../../sandbox/fdir-algorithm/data/svm/train/camera_2d_from_1d_A_$i.svmdata --test ../../../../../sandbox/fdir-algorithm/data/svm/test/camera_2d_from_1d_A_$i.svmdata --r 0.8
done

# 2 dimension feature space
for i in "${arr_2D_from_2D[@]}"
do
   echo "--------------------------------"
   echo "> AROW > 2D > $i"
   time ./arow.out --dim 2 --train ../../../../../sandbox/fdir-algorithm/data/svm/train/camera_2d_$i.svmdata --test ../../../../../sandbox/fdir-algorithm/data/svm/test/camera_2d_$i.svmdata --r 0.8
done

# 6 dimension feature space
echo "--------------------------------"
echo "> AROW > 2D to 6D"
time ./arow.out --dim 6 --train ../../../../../sandbox/fdir-algorithm/data/svm/train/camera_6d_from_2d_A.svmdata --test ../../../../../sandbox/fdir-algorithm/data/svm/test/camera_6d_from_2d_A.svmdata --r 0.8


################################
# SCW
################################
cd ../scw

# 1 dimension feature space
echo "--------------------------------"
echo "> SCW > 1D"
time ./scw.out --dim 6 --train ../../../../../sandbox/fdir-algorithm/data/svm/train/camera_1d.svmdata --test ../../../../../sandbox/fdir-algorithm/data/svm/test/camera_1d.svmdata --c 0.1 --eta 0.95

# 2 dimension feature space obtained from transforming 1D input space to higher dimension
for i in "${arr_2D_from_1D[@]}"
do
   echo "--------------------------------"
   echo "> SCW > 1D to 2D > $i"
   time ./scw.out --dim 2 --train ../../../../../sandbox/fdir-algorithm/data/svm/train/camera_2d_from_1d_A_$i.svmdata --test ../../../../../sandbox/fdir-algorithm/data/svm/test/camera_2d_from_1d_A_$i.svmdata --c 0.1 --eta 0.95
done

# 2 dimension feature space
for i in "${arr_2D_from_2D[@]}"
do
   echo "--------------------------------"
   echo "> SCW > 2D > $i"
   time ./scw.out --dim 2 --train ../../../../../sandbox/fdir-algorithm/data/svm/train/camera_2d_$i.svmdata --test ../../../../../sandbox/fdir-algorithm/data/svm/test/camera_2d_$i.svmdata --c 0.1 --eta 0.95
done

# 6 dimension feature space
echo "--------------------------------"
echo "> SCW > 2D to 6D"
time ./scw.out --dim 6 --train ../../../../../sandbox/fdir-algorithm/data/svm/train/camera_6d_from_2d_A.svmdata --test ../../../../../sandbox/fdir-algorithm/data/svm/test/camera_6d_from_2d_A.svmdata --c 0.1 --eta 0.95


################################
# NHERD
################################
cd ../nherd

# 1 dimension feature space
echo "--------------------------------"
echo "> NHERD > 1D"
time ./nherd.out --dim 1 --train ../../../../../sandbox/fdir-algorithm/data/svm/train/camera_1d.svmdata --test ../../../../../sandbox/fdir-algorithm/data/svm/test/camera_1d.svmdata --c 0.1 --diagonal 0

# 2 dimension feature space obtained from transforming 1D input space to higher dimension
for i in "${arr_2D_from_1D[@]}"
do
   echo "--------------------------------"
   echo "> NHERD > 1D to 2D > $i"
   time ./nherd.out --dim 2 --train ../../../../../sandbox/fdir-algorithm/data/svm/train/camera_2d_from_1d_A_$i.svmdata --test ../../../../../sandbox/fdir-algorithm/data/svm/test/camera_2d_from_1d_A_$i.svmdata --c 0.1 --diagonal 0
done

# 2 dimension feature space
for i in "${arr_2D_from_2D[@]}"
do
   echo "--------------------------------"
   echo "> NHERD > 2D > $i"
   time ./nherd.out --dim 2 --train ../../../../../sandbox/fdir-algorithm/data/svm/train/camera_2d_$i.svmdata --test ../../../../../sandbox/fdir-algorithm/data/svm/test/camera_2d_$i.svmdata --c 0.1 --diagonal 0
done

# 6 dimension feature space
echo "--------------------------------"
echo "> NHERD > 2D to 6D"
time ./nherd.out --dim 6 --train ../../../../../sandbox/fdir-algorithm/data/svm/train/camera_6d_from_2d_A.svmdata --test ../../../../../sandbox/fdir-algorithm/data/svm/test/camera_6d_from_2d_A.svmdata --c 0.1 --diagonal 0


################################
# PA
################################
cd ../pa

# 1 dimension feature space
echo "--------------------------------"
echo "> PA > 1D"
time ./pa.out --dim 1 --train ../../../../../sandbox/fdir-algorithm/data/svm/train/camera_1d.svmdata --test ../../../../../sandbox/fdir-algorithm/data/svm/test/camera_1d.svmdata --c 0.1 --select 1

# 2 dimension feature space obtained from transforming 1D input space to higher dimension
for i in "${arr_2D_from_1D[@]}"
do
   echo "--------------------------------"
   echo "> PA > 1D to 2D > $i"
   time ./pa.out --dim 2 --train ../../../../../sandbox/fdir-algorithm/data/svm/train/camera_2d_from_1d_A_$i.svmdata --test ../../../../../sandbox/fdir-algorithm/data/svm/test/camera_2d_from_1d_A_$i.svmdata --c 0.1 --select 1
done

# 2 dimension feature space
for i in "${arr_2D_from_2D[@]}"
do
   echo "--------------------------------"
   echo "> PA > 2D > $i"
   time ./pa.out --dim 2 --train ../../../../../sandbox/fdir-algorithm/data/svm/train/camera_2d_$i.svmdata --test ../../../../../sandbox/fdir-algorithm/data/svm/test/camera_2d_$i.svmdata --c 0.1 --select 1
done

# 6 dimension feature space
echo "--------------------------------"
echo "> PA > 2D to 6D"
time ./pa.out --dim 6 --train ../../../../../sandbox/fdir-algorithm/data/svm/train/camera_6d_from_2d_A.svmdata --test ../../../../../sandbox/fdir-algorithm/data/svm/test/camera_6d_from_2d_A.svmdata --c 0.1 --select 1