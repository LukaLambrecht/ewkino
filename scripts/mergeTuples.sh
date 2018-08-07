#!/bin/bash

#include functions to export CMSSW in submit
source setCMSSW.sh

fillJob(){
    name=${1%/}
    name=${name##*/}
    name=${name#ntuples_temp_*}
    name=${name}.root
    if [ ! -d ~/Work/ntuples_unskimmed ]
        then mkdir ~/Work/ntuples_unskimmed
    fi
    echo "if [ -f ~/Work/ntuples_unskimmed/$name ]" >> $2
    echo "    then rm ~/Work/ntuples_unskimmed/$name" >> $2
    echo "fi" >> $2
<<<<<<< HEAD
    echo "hadd ~/Work/ntuples_ewkino/$name ${1}/*root" >> $2
    echo "rm -r ${1}" >> $2
=======
    echo "hadd ~/Work/ntuples_unskimmed/$name ${1}/*root" >> $2
    echo "rm -r $1" >> $2
>>>>>>> master
}

mergeTuple(){
    > mergeJob.sh
    fillJob $1 mergeJob.sh 
    bash mergeJob.sh
    rm mergeJob.sh
}

submitMergeTuple(){
    > mergeJob.sh
    setCMSSW mergeJob.sh
    fillJob $1 mergeJob.sh
<<<<<<< HEAD
    submitJob mergeJob.sh "2:00:00"
=======
    submitJob mergeJob.sh "12:00:00"
>>>>>>> master
    rm mergeJob.sh
}

skimmedTuples=~/Work/ntuples_temp_*
mergeTuples(){
    for d in $skimmedTuples
        do mergeTuple $d
        #rm -r $d
    done
}

submitMergeTuples(){
    for d in $skimmedTuples
        do submitMergeTuple $d
    done
}
