#!/bin/bash
pwd_=$PWD
jobDir=/local/storage/$SLURM_JOB_ID
mkdir $jobDir
cp main $jobDir
cd $jobDir
./main 55
for file in *.root; do
    mv "$file" "${file%.root}_${1}.root"
done
mkdir -p ${pwd_}/testdata/cent55
mv *_${1}.root ${pwd_}/testdata/cent55
rm -rf $jobDir
