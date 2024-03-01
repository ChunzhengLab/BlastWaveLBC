#!/bin/bash
pwd_=$PWD
jobDir=/local/storage/$SLURM_JOB_ID
mkdir $jobDir
cp main $jobDir
cd $jobDir
./main 55
mkdir -p ${pwd_}/testdata/cent55/${1}
mv *.root ${pwd_}/testdata/cent55/${1}
find /local/storage -user chunzheng -delete
rm -rf $jobDir