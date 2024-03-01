#!/bin/bash
pwd_=$PWD
jobDir=/local/storage/$SLURM_JOB_ID
mkdir $jobDir
cp main $jobDir
cd $jobDir
./main 35
mkdir -p ${pwd_}/testdata/cent35/${1}
mv *.root ${pwd_}/testdata/cent35/${1}
find /local/storage -user chunzheng -delete
rm -rf $jobDir