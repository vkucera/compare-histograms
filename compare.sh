#!/bin/bash

# comment character for skipping histograms
skipString="#"

# files
file1="$(readlink -f "$1")"
file2="$(readlink -f "$2")"

# tags
tag1=$3
tag2=$4

# path to lists of histograms
pathList="/home/vkucera/HFjets/code/compare/lists"
#pathList=`pwd`
lists=( \
effhisto \
efficiencies \
feeddown \
masshisto \
sideband_sub \
unfolding_closure_results \
unfolding_results \
)

# path to histogram 1
dir1=""
branch1=""
#branch1="_Jet_AKTChargedR020_PicoTracks_pT0150_pt_scheme_D02_Default"
#branch1="_C0005"
#branch1="_C0510"
#dir1="V0$branch1/V0histo$branch1"
#dir1="Spectra/"

# path to histogram 2
branch2=""
#branch2="_Default"
#branch2="_Ionut"
#branch2="_clustersAOD_ANTIKT02_B1_Filter00272_Cut00150_Skip00"
dir2=$dir1
#dir2="V0$branch2/V0histo$branch2"

# name of normalisation histogram
hisNorm=""
#hisNorm="_Std/fh1EventCent"

# path to normalisation histogram 1
dirNorm1=""
#dirNorm1=$dir1

# path to normalisation histogram 2
dirNorm2=$dirNorm1
#dirNorm2=$dir2

pathNorm1="$dirNorm1$hisNorm"
pathNorm2="$dirNorm2$hisNorm"

date +%Y-%m-%d_%H-%M-%S
#echo "Lists: ${lists[@]}"

#for list in ${lists[@]}
#do

list="$5"
echo "Processing list $list"
mkdir -p "$list"
cd "$list"
rm -f Ratios.root
list_tmp_1="${list}_tmp1.txt"
list_tmp_2="${list}_tmp2.txt"

for his in $(cat $pathList/$list.txt)
do

if [ "${his:0:1}" = "$skipString" ]
then
echo "Skipping $his"
continue
fi
path1="$dir1$his"
path2="$dir2$his"
echo $path1 >> $list_tmp_1
echo $path2 >> $list_tmp_2

#root -b -q /home/vkucera/HFjets/code/compare/MakeRatio.C\(\"$file1\",\"$file2\",\"$path1\",\"$path2\",\"$pathNorm1\",\"$pathNorm2\",\"$tag1\",\"$tag2\"\)

done

root -b -q /home/vkucera/HFjets/code/compare/MakeRatio.C\(\"$file1\",\"$file2\",\"$list_tmp_1\",\"$list_tmp_2\",\"$pathNorm1\",\"$pathNorm2\",\"$tag1\",\"$tag2\"\)
rm $list_tmp_1 $list_tmp_2

cd ..

#done

#path1="$pathList/$list.txt"
#path2="$path1"
#root -b -q /home/vkucera/HFjets/code/compare/MakeRatio.C\(\"$file1\",\"$file2\",\"$path1\",\"$path2\",\"$pathNorm1\",\"$pathNorm2\",\"$tag1\",\"$tag2\"\)

date +%Y-%m-%d_%H-%M-%S
exit 0

