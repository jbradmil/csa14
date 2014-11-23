#!/bin/bash

CFADIR=/net/cms2/cms2r0/cfA
OUTPUTDIR=inputs
TREEDIR=reduced_trees
DATASET=$1

if [ ! -d "$OUTPUTDIR/$DATASET" ] 
then 
    echo " [DEBUG] Making directory $OUTPUTDIR/$DATASET " 
    mkdir -p $OUTPUTDIR/$DATASET 
    echo " [DEBUG] Making directory $TREEDIR/$DATASET " 
    mkdir -p $TREEDIR/$DATASET 
fi 

for i in {1..9}
do
    echo "ls -d $CFADIR/$DATASET/*f$i*.root > $OUTPUTDIR/$DATASET/$DATASET.part$i.list"
    ls -d $CFADIR/$DATASET/*f$i*.root > $OUTPUTDIR/$DATASET/$DATASET.part$i.list

    echo "[DEBUG] JobSubmit.csh ./scripts/make_reduced_tree.exe -l -i $OUTPUTDIR/$DATASET/$DATASET.part$i.list"  
    JobSubmit.csh ./scripts/make_reduced_tree.exe -l -i $OUTPUTDIR/$DATASET/$DATASET.part$i.list

done

