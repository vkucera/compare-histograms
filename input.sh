#!/bin/bash

######################################################
# License: CC BY-NC-SA 4.0                           #
# https://creativecommons.org/licenses/by-nc-sa/4.0/ #
# Author: Vit Kucera <vit.kucera@cern.ch>            #
# 2020                                               #
######################################################

# Input specification for compare-all.sh

####################################################################################################

# directories with results

# Lc FF
res_0909_qm="/mnt/temp/Vit/results-reference/vAN-20190909_ROOT6-1_QM19" # pp_mc_prodLcpK0s, QM 2019 results
res_0909_lcff="/mnt/temp/Vit/results-reference/vAN-20190909_ROOT6-1-bkp_lcff" # pp_mc_prodLcpK0s, latest lcff branch, almost identical with qm2019
res_0909_lcworking="/mnt/temp/Vit/results-reference/vAN-20190909_ROOT6-1-bkp_lcworking" # pp_mc_prodLcpK0s, lcworking branch, after factorisation, changed fitter, before cleaning of processer, was identical with master
res_0909_master="/data/DerivedResultsJets/LckINT7HighMultwithJets/vAN-20190909_ROOT6-1-bkp_master_before_br" # pp_mc_prodLcpK0s, identical with lcworking, additional sb histograms (hzsub_noteffscaled)
res_0909_master_br="/mnt/temp/Vit/results-reference/vAN-20190909_ROOT6-1-bkp_master_br" # pp_mc_prodLcpK0s, additional sb histograms, fixed branching ratio, replacement of z
res_0909_before_fd="/data/DerivedResultsJets/LckINT7HighMultwithJets/vAN-20190909_ROOT6-1-bkp_before_fd" # pp_mc_prodLcpK0s, current version
res_0909_now="/data/DerivedResultsJets/LckINT7HighMultwithJets/vAN-20190909_ROOT6-1-bkp" # pp_mc_prodLcpK0s, current version

res_0201_std_model="/data/DerivedResultsJets/LckINT7withJets/vAN-20200201_ROOT6-1_D2H_std-model" # pp_mc_prodD2H, trained on D2H MC
res_0201_Luigi="/data/DerivedResultsJets/LckINT7withJets/vAN-20200201_ROOT6-1_D2H_Luigi" # pp_mc_prodD2H, trained with Luigi's settings on dedicated MC from 2019/10/03
res_0201_now="/data/DerivedResultsJets/LckINT7withJets/vAN-20200201_ROOT6-1" # pp_mc_prodD2H, current version

res_0304_now="/data/DerivedResultsJets/LckAnywithJets/vAN-20200304_ROOT6-1/ff/default/default" # pp_mc_prodD2H, current version

res_0304_test_now="/data/DerivedResultsJets/LcVitTest/vAN-20200304_ROOT6-1/ff/default/default" # pp_mc_prodD2H, current version

# D0 FF
res_0201_D0_FF_ref="/data/DerivedResultsJets/D0kINT7withJets/vAN-20200201_ROOT6-1_fd" # pp_mc_prodD2H, good feeddown
res_0201_D0_FF_ml="/data/DerivedResultsJets/D0kINT7withJets/vAN-20200201_ROOT6-1_ml" # pp_mc_prodD2H, good feeddown
res_0201_D0_FF_std="/data/DerivedResultsJets/D0kINT7withJets/vAN-20200201_ROOT6-1_std" # pp_mc_prodD2H, good feeddown
res_0201_D0_FF_now="/data/DerivedResultsJets/D0kINT7withJets/vAN-20200201_ROOT6-1" # pp_mc_prodD2H, current version

# D0 zg
res_0201_D0_zg_ml="/data/DerivedResultsJets/D0kINT7withJets/vAN-20200201_ROOT6-1_ml" # pp_mc_prodD2H, current version
res_0201_D0_zg_std="/data/DerivedResultsJets/D0kINT7withJets/vAN-20200201_ROOT6-1_std" # pp_mc_prodD2H, current version
res_0201_D0_zg_now="/data/DerivedResultsJets/D0kINT7withJets/vAN-20200201_ROOT6-1" # pp_mc_prodD2H, current version

res_0304_D0_zg_first="/data/DerivedResultsJets/D0kAnywithJets/vAN-20200304_ROOT6-1" # pp_mc_prodD2H
res_0304_D0_zg_default="/data/DerivedResultsJets/D0kAnywithJets/vAN-20200304_ROOT6-1/zg/default/default" # pp_mc_prodD2H

res_0304_D0_zg_analyzer_test_fd_std="/data/DerivedResultsJets/D0kAnywithJets/vAN-20200304_ROOT6-1/zg/analyzer/test_fd_std" # pp_mc_prodD2H, testing powheg, histo
res_0304_D0_zg_analyzer_test_fd_new="/data/DerivedResultsJets/D0kAnywithJets/vAN-20200304_ROOT6-1/zg/analyzer/test_fd_new" # pp_mc_prodD2H, testing powheg tree
res_0304_D0_zg_default_sys0="/data/DerivedResultsJets/D0kAnywithJets/vAN-20200304_ROOT6-1/zg/default_sys_complete_0/default" # pp_mc_prodD2H, first sys, new powheg tree
res_0304_D0_zg_default_prw="/data/DerivedResultsJets/D0kAnywithJets/vAN-20200304_ROOT6-1/zg_preview/default/default"

res_0304_D0_zg_default_prel="/data/DerivedResultsJets/D0kAnywithJets/vAN-20200304_ROOT6-1/zg_approval_20200515_final/default/default"
res_0304_D0_zg_default_now="/data/DerivedResultsJets/D0kAnywithJets/vAN-20200304_ROOT6-1/zg/default/default"

# D0 Rg
res_0304_D0_rg_default_prel="/data/DerivedResultsJets/D0kAnywithJets/vAN-20200304_ROOT6-1/rg_approval_20200515_final/default/default"
res_0304_D0_rg_default_now="/data/DerivedResultsJets/D0kAnywithJets/vAN-20200304_ROOT6-1/rg/default/default"

# D0 nSD
res_0304_D0_nsd_default_prel="/data/DerivedResultsJets/D0kAnywithJets/vAN-20200304_ROOT6-1/nsd_approval_20200515_final/default/default"
res_0304_D0_nsd_default_now="/data/DerivedResultsJets/D0kAnywithJets/vAN-20200304_ROOT6-1/nsd/default/default"

# D0 delta R
res_0304_D0_deltaR="/data/Derived_testResults/Jets/D0kAnywithJets/vAN-20200304_ROOT6-1/ff/default/default"
res_0824_D0_deltaR="/data/Derived_testResults/Jets/D0kAnywithJets/vAN-20200824_ROOT6-1/ff/default/default"

# Lc delta R
res_0304_Lc_deltaR="/data/Derived_testResults/Jets/Lc/vAN-20200304_ROOT6-1/ff/default/default"
res_0824_Lc_deltaR="/data/Derived_testResults/Jets/Lc/vAN-20200824_ROOT6-1/ff/default/default"

####################################################################################################

# analysis strings
ana_name="D0pp" # LcpK0spp, D0pp
ana_type="jet_zg" # MBjetvspt, jet_zg

# main result directories (Select from the list above.)
base1="$res_0304_D0_zg_default_now" # test
base2="$res_0304_D0_zg_default_prel" # reference

# labels to identify histograms on the plots
label1="D0 now" # test
label2="D0 prel" # reference

# names of result subdirectories
#dir_res="results${ana_type}"
dir_res1="resultsMBjetvspt"
dir_res2="resultsMBjetvspt"

# name of histogram to get the number of events for normalisation
his_norm=""
#his_norm="histonorm"
#his_norm="_Std/fh1EventCent"

####################################################################################################

#### real data ####

# names of data type subdirectories
dir_real="pp_data" # real data

# full paths to the directories
base_real1="${base1}/${dir_real}/${dir_res1}"
base_real2="${base2}/${dir_real}/${dir_res2}"

# array of file names
files_real=( \
"masshisto.root" \
"sideband_subtracted.root" \
"feeddown.root" \
"unfolding_results.root" \
"unfolding_closure.root" \
)

# array of histogram list names for respective files
lists_real=( \
"masshisto_real_${ana_name}_${ana_type}" \
"sideband_subtracted_${ana_name}_${ana_type}" \
"feeddown_${ana_name}_${ana_type}" \
"unfolding_results_${ana_name}_${ana_type}" \
"unfolding_closure_${ana_name}_${ana_type}" \
)

# numbers of events for normalisation when his_norm is not specified
n_ev_real1=-1
n_ev_real2=-1

####################################################################################################

#### MC data ####

# names of data type subdirectories
dir_sim1="pp_mc_prodD2H" # MC data 1, pp_mc_prodLcpK0s or pp_mc_prodD2H
dir_sim2="pp_mc_prodD2H" # MC data 2, pp_mc_prodLcpK0s or pp_mc_prodD2H

# full paths to the directories
base_sim1="${base1}/${dir_sim1}/${dir_res1}"
base_sim2="${base2}/${dir_sim2}/${dir_res2}"

# array of file names
files_sim=( \
"masshisto.root" \
"effhisto.root" \
"resphisto.root" \
"efficiencies.root" \
)

# array of histogram list names for respective files
lists_sim=( \
"masshisto_sim_${ana_name}_${ana_type}" \
"effhisto_${ana_name}_${ana_type}" \
"resphisto_${ana_name}_${ana_type}"
"efficiencies_${ana_name}_${ana_type}" \
)

# numbers of events for normalisation when his_norm is not specified
n_ev_sim1=-1
n_ev_sim2=-1

####################################################################################################
