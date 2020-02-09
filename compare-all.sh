#!/bin/bash

# Run histogram comparison for directories with several files and a different list of histograms for each file


#### real data ####

# path to the first directory
base_real1="/data/DerivedResultsJets/LckINT7HighMultwithJets/vAN-20190909_ROOT6-1-bkp_r-fix/pp_data/resultsMBjetvspt"
#base_real1="/data/DerivedResultsJets/LckINT7HighMultwithJets/vAN-20190909_ROOT6-1-bkp/pp_data/resultsMBjetvspt"

# path to the second directory
base_real2="/data/DerivedResultsJets/LckINT7HighMultwithJets/vAN-20190909_ROOT6-1-bkp_lcworking/pp_data/resultsMBjetvspt"
#base_real2="/data/DerivedResultsJets/LckINT7HighMultwithJets/vAN-20190909_ROOT6-1-bkp_GMr-fix/pp_data/resultsMBjetvspt"
#base_real2="/data/DerivedResultsJets/LckINT7HighMultwithJets/vAN-20190909_ROOT6-1-bkp_cleandb/pp_data/resultsMBjetvspt"

# array of file names
files_real=( "feeddownLcpK0sppMBjetvspt.root" )
#files_real=( "masshisto.root" "sideband_subLcpK0sppMBjetvspt.root" "feeddownLcpK0sppMBjetvspt.root" "unfolding_resultsLcpK0sppMBjetvspt.root" "unfolding_closure_resultsLcpK0sppMBjetvspt.root" )

# array of list names for respective files
lists_real=( "feeddown" )


#### MC data ####

# path to the first directory
base_sim1="/data/DerivedResultsJets/LckINT7HighMultwithJets/vAN-20190909_ROOT6-1-bkp_r-fix/pp_mc_prodLcpK0s/resultsMBjetvspt"
#base_sim1="/data/DerivedResultsJets/LckINT7HighMultwithJets/vAN-20190909_ROOT6-1-bkp/pp_mc_prodLcpK0s/resultsMBjetvspt"

# path to the second directory
base_sim2="/data/DerivedResultsJets/LckINT7HighMultwithJets/vAN-20190909_ROOT6-1-bkp_lcworking/pp_mc_prodLcpK0s/resultsMBjetvspt"
#base_sim2="/data/DerivedResultsJets/LckINT7HighMultwithJets/vAN-20190909_ROOT6-1-bkp_GMr-fix/pp_mc_prodLcpK0s/resultsMBjetvspt"
#base_sim2="/data/DerivedResultsJets/LckINT7HighMultwithJets/vAN-20190909_ROOT6-1-bkp_cleandb/pp_mc_prodLcpK0s/resultsMBjetvspt"

# array of file names
files_sim=( "masshisto.root" "effhisto.root" "efficienciesLcpK0sppMBjetvspt.root" )

# array of list names for respective files
lists_sim=( "masshisto_sim" "effhisto" "efficiencies" )


# Labels to identify histograms on the plots
label1="r-fix"
label2="lcworking"

script="/home/vkucera/HFjets/code/compare/compare.sh" # path to the bash script for running comparison between files
pathList="/home/vkucera/HFjets/code/compare/lists" # path to the directory with lists


function do_compare {
  dir1=$1 # first base directory
  dir2=$2 # second base directory
  local -n files=$3 # array of files
  local -n lists=$4 # array of lists to be used for respective files
  echo "$dir1"
  echo "$dir2"
  for ((i = 0; i < ${#files[@]}; ++i))
  do
    file=${files[$i]}
    list=${lists[$i]}
    log_file="${file/.root/}_$list.txt"
    echo "Comparing histos from list $list in files $file"
    $script "$dir1/$file" "$dir2/$file" "$label1" "$label2" "$list" > "$log_file" 2>&1
    n_id=$(grep -c identical $log_file) # number of identical histograms
    n_tot=$(grep -v '#' $pathList/$list.txt | wc -l) # number of histograms that should be processed
    n_proc=$(grep -c MakeRatio $log_file) # number of actually processed histograms
    echo "Identical histograms: $n_id/$n_tot, processed: $n_proc"
    echo "Errors:"
    grep -i error $log_file
  done
}


echo "Comparing real data directories:"
mkdir -p "real"
cd "real"
do_compare $base_real1 $base_real2 files_real lists_real
cd ..

#echo "Comparing MC data directories:"
#mkdir -p "sim"
#cd "sim"
#do_compare $base_sim1 $base_sim2 files_sim lists_sim
#cd ..

exit 0

