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

[ -d "$pathList" ] || { echo "Creating list directory $pathList"; mkdir -p "$pathList" || { echo "Error: Failed to create list directory."; exit 1; } }

# Load input specification.
source "$thisDir/input.sh"

function make_histogram_list {
  file_root="$1"
  file_list="$2"
  list_histo=$(rootls "$file_root") && \
  for h in $list_histo; do echo $h; done > "$file_list" || \
  { echo "Error: Failed to generate histogram list $file_list from $file_root."; return 1; }
}

function do_compare {
  dir1=$1 # first base directory
  dir2=$2 # second base directory
  local -n files=$3 # array of files
  local -n lists=$4 # array of lists to be used for respective files
  echo "$dir1"
  echo "$dir2"
  for ((i = 0; i < ${#files[@]}; ++i)); do
    file=${files[$i]}
    [ -f "$dir1/$file" ] || { echo "Error: File $dir1/$file does not exist."; exit 1; }
    [ -f "$dir2/$file" ] || { echo "Error: File $dir2/$file does not exist."; exit 1; }
    list=${lists[$i]}
    list_file="$pathList/$list.txt"
    if [ ! -f "$list_file" ]; then
      echo "Generating list $list"
      make_histogram_list "$dir1/$file" "$list_file" || exit 1
    fi
    log_file="${file/.root/}_$list.txt"
    echo "Comparing histos from list $list in files $file"
    bash $script "$dir1/$file" "$dir2/$file" "$label1" "$label2" "$list" > "$log_file" 2>&1
    n_id=$(grep -c identical $log_file) # number of identical histograms
    n_tot=$(grep -v '#' "$list_file" | wc -l) # number of histograms that should be processed
    echo "Identical histograms: $n_id/$n_tot"
    echo "Errors:"
    grep -i error $log_file
  done
}

now="date +%H:%M:%S"
$now

echo "Comparing real data directories:"
dirOut="real"
rm -rf "$dirOut" && \
mkdir -p "$dirOut" && \
cd "$dirOut" || { echo "Error: Failed to make the output directory $dirOut."; exit 1; }
do_compare $base_real1 $base_real2 files_real lists_real
cd ..

echo "Comparing MC data directories:"
dirOut="sim"
rm -rf "$dirOut" && \
mkdir -p "$dirOut" && \
cd "$dirOut" || { echo "Error: Failed to make the output directory $dirOut."; exit 1; }
do_compare $base_sim1 $base_sim2 files_sim lists_sim
cd ..

$now

exit 0

