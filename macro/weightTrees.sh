#! /bin/sh 

# this script replaces every merged tree with the same tree with one more branch, 
# containing the event weight for that sample
# the event weight is evaluated with the total number of generated events, cross section and 
# eventual prescale and the wanted luminosity using 
# weight = xsec * filtereff * lumi * prescale / ngen;
# the values can be evaluated with the program weights.cc

# usage: ./weightTrees.sh

echo "Adding weights..."
root -l -b <<EOF

.L addWeightsToTree.cc++

addWeights("results/merged/JPsi_ee.root", 0.455319);
addWeights("results/merged/QCD_EMDoubleEnriched_Pt20.root", 6.5702);
addWeights("results/merged/QCD_EMDoubleEnriched_Pt6to20.root", 48.0664);

.q

EOF

echo "done weighting."


