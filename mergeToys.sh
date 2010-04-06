#! /bin/sh

# usage: ./mergeToys dir

dir=$1

touch $dir/merged-results.dat

cat $dir/results-*dat >> $dir/merged-results.dat



