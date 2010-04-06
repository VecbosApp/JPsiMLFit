#! /bin/sh
# ./createFitDatasets.sh assumes that merged trees (one per species as defined in createFitDatasets.cc) 
# exist in results/datasets_trees/
# creates the fit RooDataSets in directory results/datasets

mkdir -p results/datasets

root -l -b <<EOF

.L createFitDatasets.cc++
createAll();

.q

EOF
