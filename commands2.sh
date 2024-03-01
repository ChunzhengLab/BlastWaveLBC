#!/bin/bash
pwd_=$PWD
jobDir=/local/storage/$SLURM_JOB_ID
mkdir $jobDir
cp main $jobDir
cd $jobDir
./main 45
mkdir -p ${pwd_}/testdata/cent45/${1}
mv *.root ${pwd_}/testdata/cent45/${1}
find /local/storage -user chunzheng -delete
rm -rf $jobDir