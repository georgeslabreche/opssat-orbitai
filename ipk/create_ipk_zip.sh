#!/bin/bash

# ---- INIT

# app info
exp_id="exp144"
exp_name="OrbitAI"
exp_version="1.0" # TODO get git tag
exp_author="LabrecheSoto"

if [ "$#" -ne 1 ]; then
    echo "Please pass as argument the location of the built NMF app folder"
    exit
fi

# get built NMF app
nmf_app_built=$1

# build Mochi binaries
mochi_dir="../Mochi"
cd $mochi_dir
make clean
make TARGET=arm

cd ../ipk

# ---- Create required structure

# copy NMF app and it's configuration
cp -r "$nmf_app_built"/home .
cp ../nmf/space-app/conf/orbit_ai.properties home/"$exp_id"

# copy Mochi binaries
mkdir -p home/"$exp_id"/bin/Mochi
cp "$mochi_dir"/OrbitAI_Mochi home/"$exp_id"/bin/Mochi

# ---- ZIP

zip -r "$exp_name"_"$exp_version"_"$exp_author".zip home
rm -r home