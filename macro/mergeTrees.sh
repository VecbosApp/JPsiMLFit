#! /bin/sh 
# ./mergeTrees.sh expects results of the selection in subdirectories
# it creates a merged root file in the self-created results/merged/

mkdir -p results/merged

hadd results/merged/JPsi_ee.root results/JPsiEE/JPsiEE_*_-kine.root
hadd results/merged/QCD_EMDoubleEnriched_Pt20.root    results/QCD_EMDoubleEnriched_Pt20/QCD_EMDoubleEnriched_Pt20_*_-kine.root
hadd results/merged/QCD_EMDoubleEnriched_Pt6to20.root results/QCD_EMDoubleEnriched_Pt6to20/QCD_EMDoubleEnriched_Pt6to20_*_-kine.root

