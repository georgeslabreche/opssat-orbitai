#!/bin/bash

# results change depending on what is selected as X or Y in the data input space.
declare -a arr_2D_axes_combos=("Xpd3-Ypd6" "Xpd6-Ypd3")

# results change depending on what is selected as Xin the data input space.
declare -a arr_1D_axes_combos=("Xpd3" "Xpd6")

# declare an array of transformation functions for 2D --> 2D.
declare -a arr_2D_from_2D=("none" "linear" "polynomial" "rbf")

# declare an array of transformation functions for 1D --> 2D.
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
for x in "${arr_1D_axes_combos[@]}"
do
   for f in "${arr_2D_from_1D[@]}"
   do
      echo "--------------------------------"
      echo "> ADAM > 1D to 2D > ${x} > ${f}"
      time ./adam.out --dim 2 --train ../../../../../sandbox/fdir-algorithm/data/svm/train/camera_2d_from_1d_A_${x}_${f}.svmdata --test ../../../../../sandbox/fdir-algorithm/data/svm/test/camera_2d_from_1d_A_${x}_${f}.svmdata
   done
done

# 2 dimension feature space
for xy in "${arr_2D_axes_combos[@]}"
do
   for f in "${arr_2D_from_2D[@]}"
   do
      echo "--------------------------------"
      echo "> ADAM > 2D > ${xy} > ${f}"
      time ./adam.out --dim 2 --train ../../../../../sandbox/fdir-algorithm/data/svm/train/camera_2d_${xy}_${f}.svmdata --test ../../../../../sandbox/fdir-algorithm/data/svm/test/camera_2d_${xy}_${f}.svmdata
   done
done

# 3 dimension feature space
echo "--------------------------------"
echo "> ADAM > 3D"
time ./adam.out --dim 3 --train ../../../../../sandbox/fdir-algorithm/data/svm/train/camera_3d.svmdata --test ../../../../../sandbox/fdir-algorithm/data/svm/test/camera_3d.svmdata


# 3 dimension feature space obtained from transforming 1D input space to higher dimension
for x in "${arr_1D_axes_combos[@]}"
do
   echo "--------------------------------"
   echo "> ADAM > 1D to 3D > ${x}"
   time ./adam.out --dim 3 --train ../../../../../sandbox/fdir-algorithm/data/svm/train/camera_3d_from_1d_A_${x}.svmdata --test ../../../../../sandbox/fdir-algorithm/data/svm/test/camera_3d_from_1d_A_${x}.svmdata
done


# 3 dimension feature space obtained from transforming 2D input space to higher dimension
for xy in "${arr_2D_axes_combos[@]}"
do
   echo "--------------------------------"
   echo "> ADAM > 2D to 3D > ${xy}"
   time ./adam.out --dim 3 --train ../../../../../sandbox/fdir-algorithm/data/svm/train/camera_3d_from_2d_A_${xy}.svmdata --test ../../../../../sandbox/fdir-algorithm/data/svm/test/camera_3d_from_2d_A_${xy}.svmdata
done

# 4 dimension feature space from quaternion data
echo "--------------------------------"
echo "> ADAM > 4D"
time ./adam.out --dim 4 --train ../../../../../sandbox/fdir-algorithm/data/svm/train/camera_4d.svmdata --test ../../../../../sandbox/fdir-algorithm/data/svm/test/camera_4d.svmdata

# 6 dimension feature space from transforming 2D input space to higher dimension
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
for x in "${arr_1D_axes_combos[@]}"
do
   for f in "${arr_2D_from_1D[@]}"
   do
      echo "--------------------------------"
      echo "> RDA > 1D to 2D > ${x} > ${f}"
      time ./rda.out --dim 2 --train ../../../../../sandbox/fdir-algorithm/data/svm/train/camera_2d_from_1d_A_${x}_${f}.svmdata --test ../../../../../sandbox/fdir-algorithm/data/svm/test/camera_2d_from_1d_A_${x}_${f}.svmdata --eta 0.1 --lambda 0.000001
   done
done

# 2 dimension feature space
for xy in "${arr_2D_axes_combos[@]}"
do
   for f in "${arr_2D_from_2D[@]}"
   do
      echo "--------------------------------"
      echo "> RDA > 2D > ${xy} > ${f}"
      time ./rda.out --dim 2 --train ../../../../../sandbox/fdir-algorithm/data/svm/train/camera_2d_${xy}_${f}.svmdata --test ../../../../../sandbox/fdir-algorithm/data/svm/test/camera_2d_${xy}_${f}.svmdata --eta 0.1 --lambda 0.000001
   done
done

# 3 dimension feature space
echo "--------------------------------"
echo "> RDA > 3D"
time ./rda.out --dim 3 --train ../../../../../sandbox/fdir-algorithm/data/svm/train/camera_3d.svmdata --test ../../../../../sandbox/fdir-algorithm/data/svm/test/camera_3d.svmdata --eta 0.1 --lambda 0.000001

# 3 dimension feature space obtained from transforming 1D input space to higher dimension
for x in "${arr_1D_axes_combos[@]}"
do
   echo "--------------------------------"
   echo "> RDA > 1D to 3D > ${x}"
   time ./rda.out --dim 3 --train ../../../../../sandbox/fdir-algorithm/data/svm/train/camera_3d_from_1d_A_${x}.svmdata --test ../../../../../sandbox/fdir-algorithm/data/svm/test/camera_3d_from_1d_A_${x}.svmdata --eta 0.1 --lambda 0.000001
done

# 3 dimension feature space obtained from transforming 2D input space to higher dimension
for xy in "${arr_2D_axes_combos[@]}"
do
   echo "--------------------------------"
   echo "> RDA > 2D to 3D > ${xy}"
   time ./rda.out --dim 3 --train ../../../../../sandbox/fdir-algorithm/data/svm/train/camera_3d_from_2d_A_${xy}.svmdata --test ../../../../../sandbox/fdir-algorithm/data/svm/test/camera_3d_from_2d_A_${xy}.svmdata --eta 0.1 --lambda 0.000001
done

# 4 dimension feature space from quaternion data
echo "--------------------------------"
echo "> RDA > 4D"
time ./rda.out --dim 4 --train ../../../../../sandbox/fdir-algorithm/data/svm/train/camera_4d.svmdata --test ../../../../../sandbox/fdir-algorithm/data/svm/test/camera_4d.svmdata --eta 0.1 --lambda 0.000001

# 6 dimension feature space
echo "--------------------------------"
echo "> RDA > 2D to 6D"
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
for x in "${arr_1D_axes_combos[@]}"
do
   for f in "${arr_2D_from_1D[@]}"
   do
      echo "--------------------------------"
      echo "> AROW > 1D to 2D > ${x} > ${f}"
      time ./arow.out --dim 2 --train ../../../../../sandbox/fdir-algorithm/data/svm/train/camera_2d_from_1d_A_${x}_${f}.svmdata --test ../../../../../sandbox/fdir-algorithm/data/svm/test/camera_2d_from_1d_A_${x}_${f}.svmdata --r 0.8
   done
done

# 2 dimension feature space
for xy in "${arr_2D_axes_combos[@]}"
do
   for f in "${arr_2D_from_2D[@]}"
   do
      echo "--------------------------------"
      echo "> AROW > 2D > ${xy} > ${f}"
      time ./arow.out --dim 2 --train ../../../../../sandbox/fdir-algorithm/data/svm/train/camera_2d_${xy}_${f}.svmdata --test ../../../../../sandbox/fdir-algorithm/data/svm/test/camera_2d_${xy}_${f}.svmdata --r 0.8
   done
done

# 3 dimension feature space
echo "--------------------------------"
echo "> AROW > 3D"
time ./arow.out --dim 3 --train ../../../../../sandbox/fdir-algorithm/data/svm/train/camera_3d.svmdata --test ../../../../../sandbox/fdir-algorithm/data/svm/test/camera_3d.svmdata --r 0.8

# 3 dimension feature space obtained from transforming 1D input space to higher dimension
for x in "${arr_1D_axes_combos[@]}"
do
   echo "--------------------------------"
   echo "> AROW > 1D to 3D > ${x}"
   time ./arow.out --dim 3 --train ../../../../../sandbox/fdir-algorithm/data/svm/train/camera_3d_from_1d_A_${x}.svmdata --test ../../../../../sandbox/fdir-algorithm/data/svm/test/camera_3d_from_1d_A_${x}.svmdata --r 0.8
done

# 3 dimension feature space obtained from transforming 2D input space to higher dimension
for xy in "${arr_2D_axes_combos[@]}"
do
   echo "--------------------------------"
   echo "> AROW > 2D to 3D > ${xy}"
   time ./arow.out --dim 3 --train ../../../../../sandbox/fdir-algorithm/data/svm/train/camera_3d_from_2d_A_${xy}.svmdata --test ../../../../../sandbox/fdir-algorithm/data/svm/test/camera_3d_from_2d_A_${xy}.svmdata --r 0.8
done

# 4 dimension feature space from quaternion data
echo "--------------------------------"
echo "> AROW > 4D"
time ./arow.out --dim 4 --train ../../../../../sandbox/fdir-algorithm/data/svm/train/camera_4d.svmdata --test ../../../../../sandbox/fdir-algorithm/data/svm/test/camera_4d.svmdata --r 0.8

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
for x in "${arr_1D_axes_combos[@]}"
   do
      for f in "${arr_2D_from_1D[@]}"
      do
      echo "--------------------------------"
      echo "> SCW > 1D to 2D > ${x} > ${f}"
      time ./scw.out --dim 2 --train ../../../../../sandbox/fdir-algorithm/data/svm/train/camera_2d_from_1d_A_${x}_${f}.svmdata --test ../../../../../sandbox/fdir-algorithm/data/svm/test/camera_2d_from_1d_A_${x}_${f}.svmdata --c 0.1 --eta 0.95
   done
done

# 2 dimension feature space
for xy in "${arr_2D_axes_combos[@]}"
do
   for f in "${arr_2D_from_2D[@]}"
   do
      echo "--------------------------------"
      echo "> SCW > 2D > ${xy} > ${f}"
      time ./scw.out --dim 2 --train ../../../../../sandbox/fdir-algorithm/data/svm/train/camera_2d_${xy}_${f}.svmdata --test ../../../../../sandbox/fdir-algorithm/data/svm/test/camera_2d_${xy}_${f}.svmdata --c 0.1 --eta 0.95
   done
done

# 3 dimension feature space
echo "--------------------------------"
echo "> SCW > 3D"
time ./scw.out --dim 3 --train ../../../../../sandbox/fdir-algorithm/data/svm/train/camera_3d.svmdata --test ../../../../../sandbox/fdir-algorithm/data/svm/test/camera_3d.svmdata --c 0.1 --eta 0.95

# 3 dimension feature space obtained from transforming 1D input space to higher dimension
for x in "${arr_1D_axes_combos[@]}"
do
   echo "--------------------------------"
   echo "> SCW > 1D to 3D > ${x}"
   time ./scw.out --dim 3 --train ../../../../../sandbox/fdir-algorithm/data/svm/train/camera_3d_from_1d_A_${x}.svmdata --test ../../../../../sandbox/fdir-algorithm/data/svm/test/camera_3d_from_1d_A_${x}.svmdata --c 0.1 --eta 0.95
done

# 3 dimension feature space obtained from transforming 2D input space to higher dimension
for xy in "${arr_2D_axes_combos[@]}"
do
   echo "--------------------------------"
   echo "> SCW > 2D to 3D > ${xy}"
   time ./scw.out --dim 3 --train ../../../../../sandbox/fdir-algorithm/data/svm/train/camera_3d_from_2d_A_${xy}.svmdata --test ../../../../../sandbox/fdir-algorithm/data/svm/test/camera_3d_from_2d_A_${xy}.svmdata --c 0.1 --eta 0.95
done

# 4 dimension feature space from quaternion data
echo "--------------------------------"
echo "> SCW > 4D"
time ./scw.out --dim 4 --train ../../../../../sandbox/fdir-algorithm/data/svm/train/camera_4d.svmdata --test ../../../../../sandbox/fdir-algorithm/data/svm/test/camera_4d.svmdata --c 0.1 --eta 0.95

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
for x in "${arr_1D_axes_combos[@]}"
do
   for f in "${arr_2D_from_1D[@]}"
   do
      echo "--------------------------------"
      echo "> NHERD > 1D to 2D > ${x} > ${f}"
      time ./nherd.out --dim 2 --train ../../../../../sandbox/fdir-algorithm/data/svm/train/camera_2d_from_1d_A_${x}_${f}.svmdata --test ../../../../../sandbox/fdir-algorithm/data/svm/test/camera_2d_from_1d_A_${x}_${f}.svmdata --c 0.1 --diagonal 0
   done
done

# 2 dimension feature space
for xy in "${arr_2D_axes_combos[@]}"
do
   for f in "${arr_2D_from_2D[@]}"
   do
      echo "--------------------------------"
      echo "> NHERD > 2D > ${xy} > ${f}"
      time ./nherd.out --dim 2 --train ../../../../../sandbox/fdir-algorithm/data/svm/train/camera_2d_${xy}_${f}.svmdata --test ../../../../../sandbox/fdir-algorithm/data/svm/test/camera_2d_${xy}_${f}.svmdata --c 0.1 --diagonal 0
   done
done

# 3 dimension feature space
echo "--------------------------------"
echo "> NHERD > 3D"
time ./nherd.out --dim 3 --train ../../../../../sandbox/fdir-algorithm/data/svm/train/camera_3d.svmdata --test ../../../../../sandbox/fdir-algorithm/data/svm/test/camera_3d.svmdata --c 0.1 --diagonal 0

# 3 dimension feature space obtained from transforming 1D input space to higher dimension
for x in "${arr_1D_axes_combos[@]}"
do
   echo "--------------------------------"
   echo "> NHERD > 1D to 3D > ${x}"
   time ./nherd.out --dim 3 --train ../../../../../sandbox/fdir-algorithm/data/svm/train/camera_3d_from_1d_A_${x}.svmdata --test ../../../../../sandbox/fdir-algorithm/data/svm/test/camera_3d_from_1d_A_${x}.svmdata --c 0.1 --diagonal 0
done

# 3 dimension feature space obtained from transforming 2D input space to higher dimension
for xy in "${arr_2D_axes_combos[@]}"
do
   echo "--------------------------------"
   echo "> NHERD > 2D to 3D > ${xy}"
   time ./nherd.out --dim 3 --train ../../../../../sandbox/fdir-algorithm/data/svm/train/camera_3d_from_2d_A_${xy}.svmdata --test ../../../../../sandbox/fdir-algorithm/data/svm/test/camera_3d_from_2d_A_${xy}.svmdata --c 0.1 --diagonal 0
done

# 4 dimension feature space from quaternion data
echo "--------------------------------"
echo "> NHERD > 4D"
time ./nherd.out --dim 4 --train ../../../../../sandbox/fdir-algorithm/data/svm/train/camera_4d.svmdata --test ../../../../../sandbox/fdir-algorithm/data/svm/test/camera_4d.svmdata --c 0.1 --diagonal 0

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
for x in "${arr_1D_axes_combos[@]}"
do
   for f in "${arr_2D_from_1D[@]}"
   do
      echo "--------------------------------"
      echo "> PA > 1D to 2D > ${x} > ${f}"
      time ./pa.out --dim 2 --train ../../../../../sandbox/fdir-algorithm/data/svm/train/camera_2d_from_1d_A_${x}_${f}.svmdata --test ../../../../../sandbox/fdir-algorithm/data/svm/test/camera_2d_from_1d_A_${x}_${f}.svmdata --c 0.1 --select 1
   done
done

# 2 dimension feature space
for xy in "${arr_2D_axes_combos[@]}"
do
   for f in "${arr_2D_from_2D[@]}"
   do
      echo "--------------------------------"
      echo "> PA > 2D > ${xy} > ${f}"
      time ./pa.out --dim 2 --train ../../../../../sandbox/fdir-algorithm/data/svm/train/camera_2d_${xy}_${f}.svmdata --test ../../../../../sandbox/fdir-algorithm/data/svm/test/camera_2d_${xy}_${f}.svmdata --c 0.1 --select 1
   done
done

# 3 dimension feature space
echo "--------------------------------"
echo "> PA > 3D"
time ./pa.out --dim 3 --train ../../../../../sandbox/fdir-algorithm/data/svm/train/camera_3d.svmdata --test ../../../../../sandbox/fdir-algorithm/data/svm/test/camera_3d.svmdata --c 0.1 --select 1

# 3 dimension feature space obtained from transforming 1D input space to higher dimension
for x in "${arr_1D_axes_combos[@]}"
do
   echo "--------------------------------"
   echo "> PA > 1D to 3D > ${x}"
   time ./pa.out --dim 3 --train ../../../../../sandbox/fdir-algorithm/data/svm/train/camera_3d_from_1d_A_${x}.svmdata --test ../../../../../sandbox/fdir-algorithm/data/svm/test/camera_3d_from_1d_A_${x}.svmdata --c 0.1 --select 1
done

# 3 dimension feature space obtained from transforming 2D input space to higher dimension
for xy in "${arr_2D_axes_combos[@]}"
do
   echo "--------------------------------"
   echo "> PA > 2D to 3D > ${xy}"
   time ./pa.out --dim 3 --train ../../../../../sandbox/fdir-algorithm/data/svm/train/camera_3d_from_2d_A_${xy}.svmdata --test ../../../../../sandbox/fdir-algorithm/data/svm/test/camera_3d_from_2d_A_${xy}.svmdata --c 0.1 --select 1
done

# 4 dimension feature space from quaternion data
echo "--------------------------------"
echo "> PA > 4D"
time ./pa.out --dim 4 --train ../../../../../sandbox/fdir-algorithm/data/svm/train/camera_4d.svmdata --test ../../../../../sandbox/fdir-algorithm/data/svm/test/camera_4d.svmdata --c 0.1 --select 1

# 6 dimension feature space
echo "--------------------------------"
echo "> PA > 2D to 6D"
time ./pa.out --dim 6 --train ../../../../../sandbox/fdir-algorithm/data/svm/train/camera_6d_from_2d_A.svmdata --test ../../../../../sandbox/fdir-algorithm/data/svm/test/camera_6d_from_2d_A.svmdata --c 0.1 --select 1