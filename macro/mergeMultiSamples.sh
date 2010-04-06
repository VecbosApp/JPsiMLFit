#! /bin/sh 
# ./mergeTrees.sh expects results of single sample files in results/merged/
# it creates a merged root file for each single specie of the fit in results/datasets

mkdir -p results/datasets_trees

echo "Now merging species..."

# signal is always a species per se
cp results/merged/JPsi_ee.root results/datasets_trees

# merging all QCD samples
hadd results/datasets_trees/QCD_EMDoubleEnriched.root results/merged/QCD_EMDoubleEnriched_Pt20.root results/merged/QCD_EMDoubleEnriched_Pt6to20.root

# merging all samples to create 'data'
hadd results/datasets_trees/JPsi_ee_data.root results/merged/JPsi_ee.root results/merged/QCD_EMDoubleEnriched_Pt6to20.root results/merged/QCD_EMDoubleEnriched_Pt20.root
