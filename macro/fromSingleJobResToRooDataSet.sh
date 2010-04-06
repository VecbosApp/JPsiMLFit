#! /bin/sh 
# ./fromSingleJobResToRooDataSet.sh:
# it runs several scripts that bring from the results of the selection stored in the directory "results/"
# to the merged RooDataSets of the species used for the fit

echo "MERGING TREES STEP..."
chmod u+x ./mergeTrees.sh
./mergeTrees.sh
echo "MERGING TREES STEP DONE."

echo "WEIGHTING TREES STEP..."
chmod u+x ./weightTrees.sh
./weightTrees.sh
echo "MERGING TREES STEP DONE."

echo "MERGING WEIGHTED TREES ACCORDING FIT SPECIES DEFINITION..."
chmod u+x ./mergeMultiSamples.sh
./mergeMultiSamples.sh
echo "MERGING WEIGHTED TREES ACCORDING FIT SPECIES DEFINITION DONE."

echo "CREATING FIT ROODATASETS STEP..."
chmod u+x ./createFitDatasets.sh
./createFitDatasets.sh
echo "CREATING FIT ROODATASETS DONE."
