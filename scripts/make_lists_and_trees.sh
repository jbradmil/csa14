#!/bin/bash

CFADIR=/net/cms2/cms2r0/cfA
OUTPUTDIR=inputs
TREEDIR=reduced_trees
DATASET=$1
ENERGY=13TeV
if [[ $DATASET == *v74 ]] || [[ $DATASET == *v71 ]] || [[ $DATASET == *v6* ]];
then
    ENERGY=8TeV
fi

if [ ! -d "$OUTPUTDIR/$ENERGY/$DATASET" ] 
then 
    echo " [DEBUG] Making directory $OUTPUTDIR/$ENERGY/$DATASET " 
    mkdir -p $OUTPUTDIR/$ENERGY/$DATASET 
    echo " [DEBUG] Making directory $TREEDIR/$ENERGY/$DATASET " 
    mkdir -p $TREEDIR/$ENERGY/$DATASET 
fi 

for i in {3..9}
do
    echo "ls -d $CFADIR/$DATASET/*f$i*.root > $OUTPUTDIR/$ENERGY/$DATASET/$DATASET.part$i.list"
    ls -d $CFADIR/$DATASET/*f$i*.root > $OUTPUTDIR/$ENERGY/$DATASET/$DATASET.part$i.list

    echo "[DEBUG] JobSubmit.csh ./scripts/make_reduced_tree.exe -l -i $OUTPUTDIR/$ENERGY/$DATASET/$DATASET.part$i.list"  
    JobSubmit.csh ./scripts/make_reduced_tree.exe -l -i $OUTPUTDIR/$ENERGY/$DATASET/$DATASET.part$i.list

done

