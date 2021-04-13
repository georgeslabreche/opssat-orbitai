## MochiMochi
The MochiMochi submodule is forked from [olanleed/MochiMochi](https://github.com/olanleed/MochiMochi). The following are instructions on how to compile and run MochiMochi independently from the OrbitAI app.

Some example training datasets are taken from:
- [SVMLight](http://svmlight.joachims.org/)'s "Inductive SVM" for `example1`.
- [TinySVM](http://chasen.org/~taku/software/TinySVM/) for `example2`.

OPS-SAT photodiode sun elevation angles training data are taken from the spacecraft's WebMUST platform.

### Examples
To compile the MochiMochi examples cd into an example directory and use `Make`. E.g.:
```
cd MochiMochi/examples/binary_classifier/scw
./clean.sh
cmake .
make
```

For each ML methodology, two training examples are given in each of the following sections:
- With training data taken from [SVMLight](http://svmlight.joachims.org/).
- With training data taken from OPS-SAT's WebMUST and represent photodiode elevation angles values.

#### ADAM
```
./adam.out --dim 9947 --train ../../training_data/example1/train.dat --test ../../training_data/example1/test.dat
./adam.out --dim 2 --train ../../../../../sandbox/fdir/data/svm/train/camera_2d_none.svmdata --test ../../../../../sandbox/fdir/data/svm/test/camera_2d_from_1d_A_Xpd6_power.svmdata
```

### Adagarad RDA
```
./rda.out --dim 9947 --train ../../training_data/example1/train.dat --test ../../training_data/example1/test.dat --eta 0.1 --lambda 0.000001
./rda.out --dim 2 --train ../../../../../sandbox/fdir/data/svm/train/camera_2d_none.svmdata --test ../../../../../sandbox/fdir/data/svm/test/camera_2d_from_1d_A_Xpd6_power.svmdata --eta 0.1 --lambda 0.000001
```

### AROW
```
./arow.out --dim 9947 --train ../../training_data/example1/train.dat --test ../../training_data/example1/test.dat --r 0.8
./arow.out --dim 2 --train ../../../../../sandbox/fdir/data/svm/train/camera_2d_none.svmdata --test ../../../../../sandbox/fdir/data/svm/test/camera_2d_from_1d_A_Xpd6_power.svmdata --r 0.8
```

### SCW
```
./scw.out --dim 9947 --train ../../training_data/example1/train.dat --test ../../training_data/example1/test.dat --c 0.1 --eta 0.95
./scw.out --dim 2 --train ../../../../../sandbox/fdir/data/svm/train/camera_2d_none.svmdata --test ../../../../../sandbox/fdir/data/svm/test/camera_2d_from_1d_A_Xpd6_power.svmdata --c 0.1 --eta 0.95
```

### NHERD
```
./nherd.out --dim 9947 --train ../../training_data/example1/train.dat --test ../../training_data/example1/test.dat --c 0.1 --diagonal 0
./nherd.out --dim 2 --train ../../../../../sandbox/fdir/data/svm/train/camera_2d_none.svmdata --test ../../../../../sandbox/fdir/data/svm/test/camera_2d_from_1d_A_Xpd6_power.svmdata --c 0.1 --diagonal 0
```

### PA
```
./pa.out --dim 9947 --train ../../training_data/example1/train.dat --test ../../training_data/example1/test.dat --c 0.1 --select 1
./pa.out --dim 2 --train ../../../../../sandbox/fdir/data/svm/train/camera_2d_none.svmdata --test ../../../../../sandbox/fdir/data/svm/test/camera_2d_from_1d_A_Xpd6_power.svmdata --c 0.1 --select 1
```

### Run all examples
To run all examples with all OPS-SAT training and test data:
- Make sure that all examples are compiled.
- `./runall_examples.sh`.
- Sample results are available in `results.txt`.