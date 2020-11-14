#!/bin/bash

######################################################
# License: CC BY-NC-SA 4.0                           #
# https://creativecommons.org/licenses/by-nc-sa/4.0/ #
# Author: Vit Kucera <vit.kucera@cern.ch>            #
# 2020                                               #
######################################################

# Run histogram comparison for directories with several files and a different list of histograms for each file.

thisDir="$(dirname $(realpath $0))"
script="$thisDir/compare.sh" # path to the bash script for running comparison between files
pathList="$thisDir/lists" # path to the directory with lists

# Load input specification.
source "$thisDir/input.sh"

function do_compare {
  dir1=$1 # first base directory
  dir2=$2 # second base directory
  local -n files=$3 # array of files
  local -n lists=$4 # array of lists to be used for respective files
  echo "$dir1"
  echo "$dir2"
  for ((i = 0; i < ${#files[@]}; ++i)); do
    file=${files[$i]}
    list=${lists[$i]}
    log_file="${file/.root/}_$list.txt"
    echo "Comparing histos from list $list in files $file"
    bash $script "$dir1/$file" "$dir2/$file" "$label1" "$label2" "$list" > "$log_file" 2>&1
    n_id=$(grep -c identical $log_file) # number of identical histograms
    n_tot=$(grep -v '#' $pathList/$list.txt | wc -l) # number of histograms that should be processed
    echo "Identical histograms: $n_id/$n_tot"
    echo "Errors:"
    grep -i error $log_file
  done
}

date +%H:%M:%S
echo "Comparing real data directories:"
mkdir -p "real"
cd "real"
do_compare $base_real1 $base_real2 files_real lists_real
cd ..

echo "Comparing MC data directories:"
mkdir -p "sim"
cd "sim"
do_compare $base_sim1 $base_sim2 files_sim lists_sim
cd ..
date +%H:%M:%S

exit 0

