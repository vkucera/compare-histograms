# compare-histograms
 Scripts and macros to compare histograms between two directories with ROOT files

All the settings are done inside `input.sh`.

Change `ana_name`, `ana_type`, `base1`, `base2`, `label1`, `label2`.

If you want to divide the histograms by the numbers of events, you can either specify the histogram name in `his_norm`, or you can set the numbers of events explicitly in `n_ev_real1`, `n_ev_real2`, `n_ev_sim1`, `n_ev_sim2`.

The lists of histograms can be generated automatically if the histograms are in the main directories of the ROOT files.
Otherwise, the lists have to be added manually.

When you want to run the comparison, go to a directory where you want the output to be saved and run:
```bash
bash <path to this directory>/compare-all.sh
```
