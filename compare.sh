#!/bin/bash

######################################################
# License: CC BY-NC-SA 4.0                           #
# https://creativecommons.org/licenses/by-nc-sa/4.0/ #
# Author: Vit Kucera <vit.kucera@cern.ch>            #
# 2020                                               #
######################################################

# Compare histograms in two files.

# arguments:
# $1 file 1
# $2 file 2
# $3 tag 1
# $4 tag 2
# $5 histogram list name

# comment character for skipping histograms
skipString="#"

# input files
file1="$(readlink -f "$1")"
file2="$(readlink -f "$2")"

# Check correct input files.
[[ -z $file1 || -z $file2 ]] && { echo "Error: Empty file path!"; exit 1; }
[ ! -f $file1 ] && { echo "Error: File $file1 does not exist!"; exit 1; }
[ ! -f $file2 ] && { echo "Error: File $file2 does not exist!"; exit 1; }

# tags used as labels on plots
tag1="$3"
tag2="$4"

thisDir="$(dirname $(realpath $0))"

# path to lists of histograms
pathList="$thisDir/lists"
# histogram list file
list="$5"
list_file="$pathList/$list.txt"

# customisation of paths

# path to histogram 1
branch1=""
dir1=""
#dir1="V0$branch1/V0histo$branch1"
#dir1="Spectra/"

# path to histogram 2
branch2="$branch1"
dir2="$dir1"
#dir2="V0$branch2/V0histo$branch2"

# name of normalisation histogram
hisNorm=""
#hisNorm="histonorm"
#hisNorm="_Std/fh1EventCent"

# path to normalisation histogram 1
dirNorm1=""
#dirNorm1="$dir1"

# path to normalisation histogram 2
dirNorm2="$dirNorm1"
#dirNorm2="$dir2"

pathNorm1="$dirNorm1$hisNorm"
pathNorm2="$dirNorm2$hisNorm"

#### EXECUTION ####

date +%Y-%m-%d_%H-%M-%S

echo "Processing list $list"
[ -f "$list_file" ] || { echo "Error: Cannot find list file $list_file"; exit 1; }

mkdir -p "$list"
cd "$list"
rm -f Ratios.root
# temporary list files with modified paths
list_tmp_1="${list}_tmp1.txt"
list_tmp_2="${list}_tmp2.txt"

# The following loop allows to use the list file as a template and to modify the paths by appending dir1 and dir2.
# histogram loop START
for his in $(cat $list_file); do
  if [ "${his:0:1}" = "$skipString" ]; then
    echo "Skipping $his"
    continue
  fi
  path1="$dir1$his"
  path2="$dir2$his"
  echo $path1 >> $list_tmp_1
  echo $path2 >> $list_tmp_2

  # This is the old slow version. Only one histogram is processed per MakeRatio instance.
  #root -b -q "$thisDir/MakeRatio.C(\"$file1\",\"$file2\",\"$path1\",\"$path2\",\"$pathNorm1\",\"$pathNorm2\",\"$tag1\",\"$tag2\")"
done
# histogram loop END

# This is the new fast version. List files are passed to the macro and looping over histograms is done inside the main function while input/output files are opened only once.
root -b -q "$thisDir/MakeRatio.C(\"$file1\",\"$file2\",\"$list_tmp_1\",\"$list_tmp_2\",\"$pathNorm1\",\"$pathNorm2\",\"$tag1\",\"$tag2\")"
rm $list_tmp_1 $list_tmp_2

# Use the following lines if you want to use the list file directly for both files and you don't need to modify the histogram paths.
#root -b -q "$thisDir/MakeRatio.C(\"$file1\",\"$file2\",\"$list_file\",\"$list_file\",\"$pathNorm1\",\"$pathNorm2\",\"$tag1\",\"$tag2\")"

cd ..

date +%Y-%m-%d_%H-%M-%S

exit 0

