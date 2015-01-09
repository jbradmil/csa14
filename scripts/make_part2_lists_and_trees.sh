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

echo "ls -d $CFADIR/$DATASET/*f2_*.root > $OUTPUTDIR/$ENERGY/$DATASET/$DATASET.part20.list"
ls -d $CFADIR/$DATASET/*f2_*.root > $OUTPUTDIR/$ENERGY/$DATASET/$DATASET.part20.list
echo "ls -d $CFADIR/$DATASET/*f20*.root >> $OUTPUTDIR/$ENERGY/$DATASET/$DATASET.part20.list"
ls -d $CFADIR/$DATASET/*f20*.root >> $OUTPUTDIR/$ENERGY/$DATASET/$DATASET.part20.list
echo "[DEBUG] JobSubmit.csh ./scripts/make_reduced_tree.exe -l -i $OUTPUTDIR/$ENERGY/$DATASET/$DATASET.part20.list"  
JobSubmit.csh ./scripts/make_reduced_tree.exe -l -i $OUTPUTDIR/$ENERGY/$DATASET/$DATASET.part20.list

for i in {1..9}
do
    echo "ls -d $CFADIR/$DATASET/*f2$i*.root > $OUTPUTDIR/$ENERGY/$DATASET/$DATASET.part2$i.list"
    ls -d $CFADIR/$DATASET/*f2$i*.root > $OUTPUTDIR/$ENERGY/$DATASET/$DATASET.part2$i.list

    echo "[DEBUG] JobSubmit.csh ./scripts/make_reduced_tree.exe -l -i $OUTPUTDIR/$ENERGY/$DATASET/$DATASET.part2$i.list"  
    JobSubmit.csh ./scripts/make_reduced_tree.exe -l -i $OUTPUTDIR/$ENERGY/$DATASET/$DATASET.part2$i.list

done

