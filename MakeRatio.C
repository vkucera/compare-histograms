#include <iostream>
#include <vector>
#include <sstream>

#include "TROOT.h"
#include "TSystem.h"
#include "TString.h"
#include "TFile.h"
#include "TDirectoryFile.h"
#include "TList.h"
#include "TMath.h"

#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "THnSparse.h"

#include "TStyle.h"
#include "TCanvas.h"
#include "TGraphErrors.h"
#include "TMultiGraph.h"
#include "TLatex.h"
#include "TLine.h"
#include "TLegend.h"

#include "AuxROOTFunctions.h"

TObject* GetHistogram(TFile* file, TString sPath, TString& sNameHis, Int_t& iDegree);
void AdjustRangeGraph(TGraph* gr, Double_t dMargin = 0.1);
void OptimizeBinningTwo(TH1D** phis1, TH1D** phis2, Double_t dNMin = 100);
Int_t Run(TObject* his1, TObject* his2, TString sNameHis, Int_t iDegree, TString sTag1 = "1", TString sTag2 = "2", TH1D* hisNorm1H1 = 0, TH1D* hisNorm2H1 = 0, TFile* fileOut = 0);

// settings
Bool_t bSkipIdentical = 1; // do not make plots for identical histograms
Bool_t bSave = 1; // save ratios in a file
Bool_t bCorrelated = 0; // divide combined error by sqrt(2) if both histograms are from the same sample
Bool_t bCompareWithErr = 0; // for 2D ratios, make a second plot and display ratio also as (ratio - 1)/error_ratio
Bool_t bTHnSlices = 0; // make all projections in slices of others axes
Int_t iNTHnSlices = 4; //24; // number of slices for each axis
Bool_t bRestrict = 0; // restrict range of y-axis (and z-axis for 2D) manually, using dRatioMin and dRatioMax
Bool_t bRestrictAuto = 1; // restrict range of y-axis (and z-axis for 2D) automatically
Double_t dRatioMin = 0.9; // minimum of y-axis (and z-axis for 2D)
Double_t dRatioMax = 1.3; // maximum of y-axis (and z-axis for 2D)
Bool_t bOptimizeBinning = 0; // rebin histograms so that there are at least dNMin entries in each bin in the not empty range
Double_t dNMin = 200; // minimum number of entries in each filled bin after rebinning (default 100)
Bool_t bBinWidth = 0; // divide 1D histograms by bin width for preserving shapes of plotted distributions
Bool_t bDrawOverlap = 1; // divide canvas and plot the overlap of histograms in the upper half and their ratio in the lower half

// axis: m_V0; pt_V0; eta_V0; pt_jet
Int_t iAxesSkipProj[] = { -1}; // list of axes to skip when making projection
const Int_t iNAxesSkipProj = sizeof(iAxesSkipProj) / sizeof(iAxesSkipProj[0]);
Int_t iAxesSkipSlice[] = { -1}; // list of axes to skip when making slices
const Int_t iNAxesSkipSlice = sizeof(iAxesSkipSlice) / sizeof(iAxesSkipSlice[0]);

void MakeRatio(TString sNameFile1, TString sNameFile2, TString sPath1, TString sPath2, TString sPathNorm1 = "", TString sPathNorm2 = "", TString sTag1 = "1", TString sTag2 = "2")
{
  gStyle->SetOptStat(0);

  if(!sNameFile1.Length() || !sNameFile2.Length() || !sPath1.Length() || !sPath2.Length())
  {
    printf("Error: Invalid input\n");
    return;
  }

  // input
  TFile* file1 = 0;
  TFile* file2 = 0;
  TFile* fileOut = 0;
  ifstream fileList1; // txt file with a list of histogram paths
  ifstream fileList2; // txt file with a list of histogram paths
  TObject* his1 = 0;
  TObject* his2 = 0;
  // normalisation histograms
  TH1D* hisNorm1H1 = 0;
  TH1D* hisNorm2H1 = 0;
  // histogram dimensions: 1 - TH1, 2 - TH2, 3 - TH3, 4 - THnSparse
  Int_t iDegreeHis1 = 1;
  Int_t iDegreeHis2 = 1;
  Int_t iDegreeHisNorm1 = 1;
  Int_t iDegreeHisNorm2 = 1;

  TString sNameFileOut = "Ratios.root";
  TString sNameHis = "";
  TString sNameHisNorm = "";
  TString sSkipString = "#"; // string to mark histograms in a list to be skipped

  printf("Opening file 1 %s ", sNameFile1.Data());
  file1 = new TFile(sNameFile1.Data(), "READ");
  if(file1->IsZombie())
  {
    printf("failed (Error)\n");
    return;
  }
  printf("OK\n");

  printf("Opening file 2 %s ", sNameFile2.Data());
  file2 = new TFile(sNameFile2.Data(), "READ");
  if(file2->IsZombie())
  {
    printf("failed (Error)\n");
    return;
  }
  printf("OK\n");

  if(bSave)
  {
    printf("Opening output file %s ", sNameFileOut.Data());
    fileOut = new TFile(sNameFileOut.Data(), "UPDATE");
    if(fileOut->IsZombie())
    {
      printf("failed (Error)\n");
      return;
    }
    printf("OK\n");
  }

  if(sPathNorm1.Length())
  {
    hisNorm1H1 = (TH1D*)GetHistogram(file1, sPathNorm1, sNameHisNorm, iDegreeHisNorm1);
    if(!hisNorm1H1)
    {
//      printf("Error: Failed to load norm histogram 1 %s\n", sNameHisNorm.Data());
      return;
    }

    hisNorm2H1 = (TH1D*)GetHistogram(file2, sPathNorm2, sNameHisNorm, iDegreeHisNorm2);
    if(!hisNorm2H1)
    {
//      printf("Error: Failed to load norm histogram 2 %s\n", sNameHisNorm.Data());
      return;
    }
  }

  Bool_t bListInput = kFALSE; // input paths point to list files
  Bool_t bSameLists = kFALSE; // both input paths point to the same list file
  if(sPath1.EndsWith(".txt") && sPath2.EndsWith(".txt"))
  {
    bListInput = kTRUE;
    fileList1.open(sPath1.Data());
    if(!fileList1.good())
    {
      printf("Error: Failed to open file %s\n", sPath1.Data());
      return;
    }
    if(sPath1.EqualTo(sPath2))
      bSameLists = kTRUE;
    else
    {
      fileList2.open(sPath2.Data());
      if(!fileList2.good())
      {
        printf("Error: Failed to open file %s\n", sPath2.Data());
        return;
      }
    }
  }

  Bool_t bStop = kFALSE;
  while(!bStop && (!bListInput || (fileList1.good() && (bSameLists || fileList2.good()))))
  {
    if(bListInput)
    {
      fileList1 >> sPath1;
      if(!bSameLists)
        fileList2 >> sPath2;
      else
        sPath2 = sPath1;
      if(!sPath1.Length() || !sPath2.Length())
        continue;
      if(sPath1.BeginsWith(sSkipString.Data()))
      {
        printf("Skipping %s\n", sPath1.Data());
        continue;
      }
      if(!bSameLists && sPath2.BeginsWith(sSkipString.Data()))
      {
        printf("Skipping %s\n", sPath2.Data());
        continue;
      }
    }
    else // exit the loop if processing a single histogram
      bStop = kTRUE;

    his1 = GetHistogram(file1, sPath1, sNameHis, iDegreeHis1);
    if(!his1)
    {
//    printf("Error: Failed to load histogram 1 %s\n", sNameHis.Data());
      continue;
    }

    his2 = GetHistogram(file2, sPath2, sNameHis, iDegreeHis2);
    if(!his2)
    {
//    printf("Error: Failed to load histogram 2 %s\n", sNameHis.Data());
      continue;
    }

    if(iDegreeHis1 != iDegreeHis2)
    {
      printf("Histograms have different degrees: %d %d\n", iDegreeHis1, iDegreeHis2);
      continue;
    }

    if(iDegreeHis1 == 0)
    {
      printf("Error: Not a histogram\n");
      continue;
    }

    // make the ratio
    Int_t iResult = Run(his1, his2, sNameHis, iDegreeHis1, sTag1, sTag2, hisNorm1H1, hisNorm2H1, fileOut);
//    printf("Run returned %d\n", iResult);
  }

  if(bListInput)
  {
    fileList1.close();
    if(!bSameLists)
      fileList2.close();
  }

  file1->Close();
  file2->Close();
  if(bSave)
    fileOut->Close();
}

Int_t Run(TObject* his1, TObject* his2, TString sNameHis, Int_t iDegree, TString sTag1, TString sTag2, TH1D* hisNorm1H1, TH1D* hisNorm2H1, TFile* fileOut)
{
  // Make ratios his1/his2 and make plots.
  // return values:
  //  0 - Ratios were made successfully.
  //  1 - Histograms his1 and his2 are identical and no ratios were made.
  // -1 - Error

  // canvases
  TCanvas* canRatio = 0;
  TCanvas* canRatio2D = 0;
  TCanvas* canRatioDev = 0;
  Int_t iCanHeight = 600;
  Int_t iCanWidth = 800;
  TString sNameCan = "Ratio";
  TString kImageSuf = "png";
//  kImageSuf = "eps";
  TLegend* legend = 0;
  Double_t legx1 = 0.8, legy1 = 0.7, legx2 = 0.9, legy2 = 0.9;
  Double_t dFontSizeLeg = 0.05;
  Double_t dFontSizeAxis = 0.05;
  TLine* lineOne = 0;
  TPad* pad = 0;
  Double_t padx1 = 0., pady1 = 0., padx2 = 0.95, pady2 = 0.95, padymid = 0.5;

  // graphs
  TGraphErrors* grRatio = 0;
  TGraphErrors* grHis1 = 0;
  TGraphErrors* grHis2 = 0;
  TMultiGraph* mgrHis = 0;

  // names
  TString sTitle = "";
  TString sTitleAxis = "";
  TString sLabelRatio = "ratio";
  TString sLabelRatioDev = "(ratio #minus 1)/#it{#sigma}_{ratio}";
  sLabelRatio += Form(" %s/%s", sTag1.Data(), sTag2.Data());

  // loaded histograms
  TH1D* his1H1 = 0;
  TH1D* his2H1 = 0;
  TH2D* his1H2 = 0;
  TH2D* his2H2 = 0;
  TH3D* his1H3 = 0;
  TH3D* his2H3 = 0;
  THnSparseD* his1Hn = 0;
  THnSparseD* his2Hn = 0;
  RooUnfoldResponse* his1R = 0;
  RooUnfoldResponse* his2R = 0;
  Int_t iNDim1 = 0;
  Int_t iNDim2 = 0;
  std::vector<TH1D*> vecHis[2];
  // ratios
  TH1D* hisRatioH1 = 0;
  TH2D* hisRatioH2 = 0;
  TH2D* hisRatioH2Dev = 0;
  std::vector<TH1D*> vecRatio;

  printf("Processing %s (deg %d)\n", sNameHis.Data(), iDegree);

  switch(iDegree)
  {
    case 1:
    {
      his1H1 = (TH1D*)his1;
      his2H1 = (TH1D*)his2;
      if(AreIdentical(his1H1, his2H1))
      {
        printf("Histograms %s are identical.\n", sNameHis.Data());
        if(bSkipIdentical)
        {
          printf("No plots needed. ;-)\n");
          return 1;
        }
      }
//      printf("Histogram %s: title %s, name %s\n", sNameHis.Data(), his1H1->GetTitle(), his1H1->GetName());
      if(!strlen(his1H1->GetTitle()))  // Make sure the histograms don't have empty titles.
        his1H1->SetTitle(his1H1->GetName());
      if(bOptimizeBinning)
        OptimizeBinningTwo(&his1H1, &his2H1, dNMin);
      hisRatioH1 = DivideHistograms1D(his1H1, his2H1);
      if(!hisRatioH1)
        return -1;
      hisRatioH1->SetTitle(his1H1->GetTitle());
      break;
    }
    case 2:
    {
      his1H2 = (TH2D*)his1;
      his2H2 = (TH2D*)his2;
      if(AreIdentical(his1H2, his2H2))
      {
        printf("Histograms %s are identical.\n", sNameHis.Data());
        if(bSkipIdentical)
        {
          printf("No plots needed. ;-)\n");
          return 1;
        }
      }
//      printf("Histogram %s: title %s, name %s\n", sNameHis.Data(), his1H2->GetTitle(), his1H2->GetName());
      if(!strlen(his1H2->GetTitle()))  // Make sure the histograms don't have empty titles.
        his1H2->SetTitle(his1H2->GetName());
//      printf("Histogram %s: title %s, name %s\n", sNameHis.Data(), his1H2->GetTitle(), his1H2->GetName());
      hisRatioH2 = DivideHistograms2D(his1H2, his2H2);
      if(!hisRatioH2)
        return -1;
      hisRatioH2->SetTitle(his1H2->GetTitle());
      // projections, 0 - y, 1 - x
      for(Int_t iAx = 0; iAx < 2; iAx++)
      {
        Bool_t bSkip = 0;
        for(Int_t iAxSkip = 0; iAxSkip < iNAxesSkipProj; iAxSkip++)
        {
          if(iAx == iAxesSkipProj[iAxSkip])
          {
            bSkip = 1;
            break;
          }
        }
        if(bSkip)
          continue;
        if(bTHnSlices)
        {
          for(Int_t iAx2 = 0; iAx2 < 2; iAx2++)
          {
            if(iAx2 == iAx)
              continue;
            bSkip = 0;
            for(Int_t iAxSkip = 0; iAxSkip < iNAxesSkipSlice; iAxSkip++)
            {
              if(iAx2 == iAxesSkipSlice[iAxSkip])
              {
                bSkip = 1;
                break;
              }
            }
            if(bSkip)
              continue;
            TAxis* axis = (iAx == 0 ? his1H2->GetYaxis() : his1H2->GetXaxis());
            Int_t iNBinsAx2 = axis->GetNbins();
            if(iNBinsAx2 < iNTHnSlices) // impossible to divide axis
              continue;
            sTitleAxis = axis->GetTitle();
            Double_t dXMin = axis->GetXmin();
            Double_t dStep = (axis->GetXmax() - dXMin) / iNTHnSlices; // width of slice
            for(Int_t iSlice = 1; iSlice <= iNTHnSlices; iSlice++)
            {
              Double_t dSliceMin = dXMin + (iSlice - 1) * dStep;
              Double_t dSliceMax = dXMin + iSlice * dStep;
              Int_t iBinMin = axis->FindFixBin(dSliceMin);
              Int_t iBinMax = axis->FindFixBin(dSliceMax);
              if(iAx == 0)
              {
                his1H1 = his1H2->ProjectionX(Form("%s_%d_%d_%d_1", his1H2->GetName(), iAx, iAx2, iSlice), iBinMin, iBinMax, "e"); // make projection
                his2H1 = his2H2->ProjectionX(Form("%s_%d_%d_%d_2", his2H2->GetName(), iAx, iAx2, iSlice), iBinMin, iBinMax, "e"); // different name needed
              }
              else
              {
                his1H1 = his1H2->ProjectionY(Form("%s_%d_%d_%d_1", his1H2->GetName(), iAx, iAx2, iSlice), iBinMin, iBinMax, "e"); // make projection
                his2H1 = his2H2->ProjectionY(Form("%s_%d_%d_%d_2", his2H2->GetName(), iAx, iAx2, iSlice), iBinMin, iBinMax, "e"); // different name needed
              }
              if(bOptimizeBinning)
                OptimizeBinningTwo(&his1H1, &his2H1, dNMin);
              (vecHis[0]).push_back(his1H1);
              (vecHis[1]).push_back(his2H1);
              hisRatioH1 = DivideHistograms1D(his1H1, his2H1);
              if(!hisRatioH1)
                return -1;
              hisRatioH1->SetTitle(Form("%s_P%d_A%d_S%d: %s %g - %g", his1H1->GetTitle(), iAx, iAx2, iSlice, sTitleAxis.Data(), dSliceMin, dSliceMax));
              vecRatio.push_back(hisRatioH1);
            }
          }
        }
        else
        {
          if(iAx == 0)
          {
            his1H1 = his1H2->ProjectionX(Form("%s_%d_1", his1H2->GetName(), iAx), 1, his1H2->GetYaxis()->GetNbins(), "e");
            his2H1 = his2H2->ProjectionX(Form("%s_%d_2", his2H2->GetName(), iAx), 1, his2H2->GetYaxis()->GetNbins(), "e"); // different name needed
          }
          else
          {
            his1H1 = his1H2->ProjectionY(Form("%s_%d_1", his1H2->GetName(), iAx), 1, his1H2->GetXaxis()->GetNbins(), "e");
            his2H1 = his2H2->ProjectionY(Form("%s_%d_2", his2H2->GetName(), iAx), 1, his2H2->GetXaxis()->GetNbins(), "e"); // different name needed
          }
          if(bOptimizeBinning)
            OptimizeBinningTwo(&his1H1, &his2H1, dNMin);
          (vecHis[0]).push_back(his1H1);
          (vecHis[1]).push_back(his2H1);
          hisRatioH1 = DivideHistograms1D(his1H1, his2H1);
          if(!hisRatioH1)
            return -1;
          hisRatioH1->SetTitle(Form("%s_P%d", his1H1->GetTitle(), iAx));
          vecRatio.push_back(hisRatioH1);
        }
      }
      break;
    }
    case 3:
    {
      his1H3 = (TH3D*)his1;
      his2H3 = (TH3D*)his2;
      if(AreIdentical(his1H3, his2H3))
      {
        printf("Histograms %s are identical.\n", sNameHis.Data());
        if(bSkipIdentical)
        {
          printf("No plots needed. ;-)\n");
          return 1;
        }
      }
      printf("Histograms %s are different but that is all for now.\n", sNameHis.Data());
      return 0;
      break;
    }
    case 4:
    {
      his1Hn = (THnSparseD*)his1;
      his2Hn = (THnSparseD*)his2;
      if(AreIdentical(his1Hn, his2Hn))
      {
        printf("Histograms %s are identical.\n", sNameHis.Data());
        if(bSkipIdentical)
        {
          printf("No plots needed. ;-)\n");
          return 1;
        }
      }
      if(!strlen(his1Hn->GetTitle()))  // Make sure the histograms don't have empty titles.
        his1Hn->SetTitle(his1Hn->GetName());
      iNDim1 = his1Hn->GetNdimensions();
      iNDim2 = his2Hn->GetNdimensions();
      if(iNDim1 != iNDim2)
      {
        printf("Error: Different dimensions\n");
        return -1;
      }
      for(Int_t iAx = 0; iAx < iNDim1; iAx++)
      {
        Bool_t bSkip = 0;
        for(Int_t iAxSkip = 0; iAxSkip < iNAxesSkipProj; iAxSkip++)
        {
          if(iAx == iAxesSkipProj[iAxSkip])
          {
            bSkip = 1;
            break;
          }
        }
        if(bSkip)
          continue;
        if(bTHnSlices)
        {
          for(Int_t iAx2 = 0; iAx2 < iNDim1; iAx2++)
          {
            if(iAx2 == iAx)
              continue;
            bSkip = 0;
            for(Int_t iAxSkip = 0; iAxSkip < iNAxesSkipSlice; iAxSkip++)
            {
              if(iAx2 == iAxesSkipSlice[iAxSkip])
              {
                bSkip = 1;
                break;
              }
            }
            if(bSkip)
              continue;
            Int_t iNBinsAx2 = his1Hn->GetAxis(iAx2)->GetNbins();
            sTitleAxis = his1Hn->GetAxis(iAx2)->GetTitle();
            if(iNBinsAx2 < iNTHnSlices) // impossible to divide axis
              continue;
            Double_t dXMin = his1Hn->GetAxis(iAx2)->GetXmin();
            Double_t dStep = (his1Hn->GetAxis(iAx2)->GetXmax() - dXMin) / iNTHnSlices; // width of slice
            for(Int_t iSlice = 1; iSlice <= iNTHnSlices; iSlice++)
            {
              Double_t dSliceMin = dXMin + (iSlice - 1) * dStep;
              Double_t dSliceMax = dXMin + iSlice * dStep;
              his1Hn->GetAxis(iAx2)->SetRangeUser(dSliceMin, dSliceMax); // restrict range of second axis
              his2Hn->GetAxis(iAx2)->SetRangeUser(dSliceMin, dSliceMax);
              his1H1 = his1Hn->Projection(iAx); // make projection
              his2H1 = his2Hn->Projection(iAx);
              if(bOptimizeBinning)
                OptimizeBinningTwo(&his1H1, &his2H1, dNMin);
              (vecHis[0]).push_back(his1H1);
              (vecHis[1]).push_back(his2H1);
              hisRatioH1 = DivideHistograms1D(his1H1, his2H1);
              if(!hisRatioH1)
                return -1;
              hisRatioH1->SetTitle(Form("%s_P%d_A%d_S%d: %s %g - %g", his1H1->GetTitle(), iAx, iAx2, iSlice, sTitleAxis.Data(), dSliceMin, dSliceMax));
              vecRatio.push_back(hisRatioH1);
            }
            his1Hn->GetAxis(iAx2)->SetRange(); // reset range of the second axis
            his2Hn->GetAxis(iAx2)->SetRange();
          }
        }
        else
        {
          his1H1 = his1Hn->Projection(iAx);
          his2H1 = his2Hn->Projection(iAx);
          if(bOptimizeBinning)
            OptimizeBinningTwo(&his1H1, &his2H1, dNMin);
          (vecHis[0]).push_back(his1H1);
          (vecHis[1]).push_back(his2H1);
          hisRatioH1 = DivideHistograms1D(his1H1, his2H1);
          if(!hisRatioH1)
            return -1;
          hisRatioH1->SetTitle(Form("%s_P%d", his1H1->GetTitle(), iAx));
          vecRatio.push_back(hisRatioH1);
        }
      }
      break;
    }
    case 5:
    {
      his1R = (RooUnfoldResponse*)his1;
      his2R = (RooUnfoldResponse*)his2;
      if(AreIdentical(his1R, his2R))
      {
        printf("Objects %s are identical.\n", sNameHis.Data());
        if(bSkipIdentical)
        {
          printf("No plots needed. ;-)\n");
          return 1;
        }
      }
      // Run recursively for response histograms.
      TH1* hMeas1 = his1R->Hmeasured();
      TH1* hMeas2 = his2R->Hmeasured();
      Run(hMeas1, hMeas2, sNameHis + "_measured", his1R->GetDimensionMeasured(), sTag1, sTag2, hisNorm1H1, hisNorm2H1, fileOut);
      TH1* hTruth1 = his1R->Htruth();
      TH1* hTruth2 = his2R->Htruth();
      Run(hTruth1, hTruth2, sNameHis + "_truth", his1R->GetDimensionTruth(), sTag1, sTag2, hisNorm1H1, hisNorm2H1, fileOut);
      TH1* hFake1 = his1R->Hfakes();
      TH1* hFake2 = his2R->Hfakes();
      Run(hFake1, hFake2, sNameHis + "_fakes", his1R->GetDimensionTruth(), sTag1, sTag2, hisNorm1H1, hisNorm2H1, fileOut);
      TH2* hResp1 = his1R->Hresponse();
      TH2* hResp2 = his2R->Hresponse();
      Run(hResp1, hResp2, sNameHis + "_response", 2, sTag1, sTag2, hisNorm1H1, hisNorm2H1, fileOut);
      return 0;
      break;
    }
    default:
    {
      printf("Error: Not implemented\n");
      return -1;
      break;
    }
  }

  // normalise by number of events
  Int_t iBinNorm = 1;
  Double_t dNorm1 = 1;
  Double_t dNorm2 = 1;
  Double_t dRatioNorm = 1;
  if(hisNorm1H1 && hisNorm2H1)
  {
    Double_t dInf = -1;
    if(iBinNorm == -1)
    {
      dNorm1 = hisNorm1H1->Integral();
      dNorm2 = hisNorm2H1->Integral();
    }
    else
    {
      dNorm1 = hisNorm1H1->GetBinContent(iBinNorm);
      dNorm2 = hisNorm2H1->GetBinContent(iBinNorm);
    }
    dRatioNorm = DivideNumbers(dNorm1, dNorm2, dInf);
    if(dRatioNorm == dInf || dNorm1 <= 0 || dNorm2 <= 0)
    {
      printf("Error: Wrong normalisation\n");
      return -1;
    }
    printf("Scaling with normalization factor: %g\n", dRatioNorm);
    switch(iDegree)
    {
      case 1:
        his1H1->Scale(1. / dNorm1);
        his2H1->Scale(1. / dNorm2);
        hisRatioH1->Scale(1. / dRatioNorm);
        break;
      case 2:
        his1H2->Scale(1. / dNorm1);
        his2H2->Scale(1. / dNorm2);
        hisRatioH2->Scale(1. / dRatioNorm);
//        break;
      case 4:
        for(UInt_t iAx = 0; iAx < vecRatio.size(); iAx++)
        {
          (vecHis[0][iAx])->Scale(1. / dNorm1);
          (vecHis[1][iAx])->Scale(1. / dNorm2);
          (vecRatio[iAx])->Scale(1. / dRatioNorm);
        }
        break;
      default:
        break;
    }
  }

  // divide by bin width for preserving shapes of plotted distributions
  if(bBinWidth)
  {
    switch(iDegree)
    {
      case 1:
        his1H1->Scale(1., "width");
        his2H1->Scale(1., "width");
        break;
      case 2:
        his1H2->Scale(1., "width");
        his2H2->Scale(1., "width");
//        break;
      case 4:
        for(UInt_t iAx = 0; iAx < vecRatio.size(); iAx++)
        {
          (vecHis[0][iAx])->Scale(1., "width");
          (vecHis[1][iAx])->Scale(1., "width");
        }
        break;
      default:
        break;
    }
  }

  // correct back for the combination of correlated errors.
  if(bCorrelated)
  {
    switch(iDegree)
    {
      case 1:
        for(Int_t iX = 1; iX <= hisRatioH1->GetXaxis()->GetNbins(); iX++)
          hisRatioH1->SetBinError(iX, hisRatioH1->GetBinError(iX) / TMath::Sqrt(2.));
        break;
      case 2:
        for(Int_t iX = 1; iX <= hisRatioH2->GetXaxis()->GetNbins(); iX++)
        {
          for(Int_t iY = 1; iY <= hisRatioH2->GetYaxis()->GetNbins(); iY++)
          {
            hisRatioH2->SetBinError(iX, iY, hisRatioH2->GetBinError(iX, iY) / TMath::Sqrt(2.));
          }
        }
//        break;
      case 4:
        for(UInt_t iAx = 0; iAx < vecRatio.size(); iAx++)
          for(Int_t iX = 1; iX <= (vecRatio[0])->GetXaxis()->GetNbins(); iX++)
            (vecRatio[iAx])->SetBinError(iX, (vecRatio[iAx])->GetBinError(iX) / TMath::Sqrt(2.));
        break;
      default:
        break;
    }
  }

  // display ratio in 2D as (ratio - 1)/error_ratio
  if(bCompareWithErr && iDegree == 2)
  {
    hisRatioH2Dev = (TH2D*)hisRatioH2->Clone();
    hisRatioH2Dev->Reset();
    Double_t dDev;
    Double_t dErr;
    Double_t dInf = 666;
    for(Int_t iX = 1; iX <= hisRatioH2->GetXaxis()->GetNbins(); iX++)
    {
      for(Int_t iY = 1; iY <= hisRatioH2->GetYaxis()->GetNbins(); iY++)
      {
        dDev = hisRatioH2->GetBinContent(iX, iY);
        dErr = hisRatioH2->GetBinError(iX, iY);
        dDev -= 1.;
        if(dErr == 0)
          continue;
        dDev = DivideNumbers(dDev, dErr, dInf);
        if(dDev == dInf)
          continue;
        hisRatioH2Dev->SetBinContent(iX, iY, dDev);
      }
    }
  }

  // write ratio histograms in the output files
  if(bSave)
  {
    fileOut->cd();
    switch(iDegree)
    {
      case 1:
        hisRatioH1->Write(sNameHis.Data());
        break;
      case 2:
        hisRatioH2->Write(sNameHis.Data());
        if(bCompareWithErr)
          hisRatioH2Dev->Write(Form("%s-Dev", sNameHis.Data()));
//        break;
      case 4:
        for(UInt_t iAx = 0; iAx < vecRatio.size(); iAx++)
          (vecRatio[iAx])->Write(Form("%s_%s%d", sNameHis.Data(), (bTHnSlices ? "S" : ""), iAx));
        break;
      default:
        break;
    }
  }

  // plot
  sTitle = sNameHis;
  switch(iDegree)
  {
    case 1:
      canRatio = new TCanvas(sNameCan.Data(), "", iCanWidth, (bDrawOverlap ? 1.5 : 1.0) * iCanHeight);
      canRatio->SetLeftMargin(0.12);
      canRatio->SetBottomMargin(0.12);
      sTitle = hisRatioH1->GetTitle();
      if(bDrawOverlap)
      {
        mgrHis = new TMultiGraph();
        canRatio->Divide(1, 2, 0.01, 0.);
        pad = (TPad*)canRatio->cd(1);
        pad->SetPad(padx1, padymid, padx2, pady2);
        pad = (TPad*)canRatio->cd(2);
        pad->SetPad(padx1, pady1, padx2, padymid);
        pad->SetBottomMargin(0.15);
        grHis1 = MakeGraphErrors(his1H1, sTag1.Data(), iMyColors[1], iMyMarkersFull[0]);
        grHis2 = MakeGraphErrors(his2H1, sTag2.Data(), iMyColors[2], iMyMarkersFull[0]);
        mgrHis->Add(grHis1);
        mgrHis->Add(grHis2);
        mgrHis->SetTitle(Form("%s;%s;%s", sTitle.Data(), his1H1->GetXaxis()->GetTitle(), his1H1->GetYaxis()->GetTitle()));
        canRatio->cd(1);
        mgrHis->Draw("AP0");
        canRatio->cd(1);
        mgrHis->GetXaxis()->SetLimits(his1H1->GetXaxis()->GetXmin(), his1H1->GetXaxis()->GetXmax());
        mgrHis->GetXaxis()->SetLabelSize(dFontSizeAxis);
        mgrHis->GetXaxis()->SetTitleSize(dFontSizeAxis);
        mgrHis->GetYaxis()->SetLabelSize(dFontSizeAxis);
        mgrHis->GetYaxis()->SetTitleSize(dFontSizeAxis);
        legend = canRatio->cd(1)->BuildLegend(legx1, legy1, legx2, legy2);
        SetLegend(legend, dFontSizeLeg);
      }
      grRatio = MakeGraphErrors(hisRatioH1, "", iMyColors[0], iMyMarkersFull[0]);
      grRatio->SetTitle(Form("%s;%s;%s", sTitle.Data(), hisRatioH1->GetXaxis()->GetTitle(), sLabelRatio.Data()));
      if(bRestrict)
      {
        grRatio->SetMinimum(dRatioMin);
        grRatio->SetMaximum(dRatioMax);
      }
      if(bRestrictAuto)
        AdjustRangeGraph(grRatio);
      canRatio->cd(bDrawOverlap ? 2 : 1);
      printf("Drawing ratio.\n");
      grRatio->Draw("AP0");
      grRatio->GetXaxis()->SetLimits(hisRatioH1->GetXaxis()->GetXmin(), hisRatioH1->GetXaxis()->GetXmax());
      grRatio->GetXaxis()->SetLabelSize(dFontSizeAxis);
      grRatio->GetXaxis()->SetTitleSize(dFontSizeAxis);
      grRatio->GetYaxis()->SetTitleOffset(1.08);
      grRatio->GetYaxis()->SetLabelSize(dFontSizeAxis);
      grRatio->GetYaxis()->SetTitleSize(dFontSizeAxis);
      if(grRatio->GetMaximum() > 1 && grRatio->GetMinimum() < 1)
      {
        lineOne = new TLine(hisRatioH1->GetXaxis()->GetXmin(), 1, hisRatioH1->GetXaxis()->GetXmax(), 1);
        lineOne->Draw();
      }
      canRatio->SaveAs(Form("%s_%s.%s", sNameCan.Data(), sNameHis.Data(), kImageSuf.Data()));
      delete hisRatioH1;
      delete grRatio;
      if(bDrawOverlap)
      {
        delete grHis1;
        delete grHis2;
        delete mgrHis;
      }
      delete canRatio;
      break;
    case 2:
      canRatio2D = new TCanvas(Form("%s2D", sNameCan.Data()), "", 2 * iCanWidth, 2 * iCanHeight);
      canRatio2D->Divide(2, 2);
      sTitle = hisRatioH2->GetTitle();
      hisRatioH2->SetTitle(Form("%s;%s;%s;%s", sTitle.Data(), his1H2->GetXaxis()->GetTitle(), his1H2->GetYaxis()->GetTitle(), sLabelRatio.Data()));
      canRatio2D->SetRightMargin(0.2);
      if(bRestrict)
      {
        hisRatioH2->GetZaxis()->SetRangeUser(dRatioMin, dRatioMax);
      }
      canRatio2D->cd(1);
      his1H2->Draw("colz");
      canRatio2D->cd(2);
      his2H2->Draw("colz");
      canRatio2D->cd(3);
      printf("Drawing 2D ratio.\n");
      hisRatioH2->Draw("colz");
      if(bCompareWithErr)
      {
        canRatioDev = new TCanvas(Form("%s-Dev", sNameCan.Data()), "", iCanWidth, iCanHeight);
        canRatioDev->cd();
        sTitle = hisRatioH2Dev->GetTitle();
        hisRatioH2Dev->SetTitle(Form("%s;%s;%s;%s", sTitle.Data(), his1H2->GetXaxis()->GetTitle(), his1H2->GetYaxis()->GetTitle(), sLabelRatioDev.Data()));
        canRatioDev->SetRightMargin(0.2);
        hisRatioH2Dev->Draw("colz");
//        canRatioDev->SaveAs(Form("%s_%s-Dev.%s", sNameCan.Data(), sNameHis.Data(), kImageSuf.Data()));
        canRatio2D->cd(4);
        hisRatioH2Dev->Draw("colz");
        delete canRatioDev;
      }
      canRatio2D->SaveAs(Form("%s_%s.%s", sNameCan.Data(), sNameHis.Data(), kImageSuf.Data()));
      delete hisRatioH2;
      if(hisRatioH2Dev)
        delete hisRatioH2Dev;
      delete canRatio2D;
//      break;
    case 4:
      for(UInt_t iAx = 0; iAx < vecRatio.size(); iAx++)
      {
        canRatio = new TCanvas(sNameCan.Data(), "", iCanWidth, (bDrawOverlap ? 1.5 : 1.0) * iCanHeight);
        canRatio->SetLeftMargin(0.12);
        canRatio->SetBottomMargin(0.12);
        sTitle = (vecRatio[iAx])->GetTitle();
        if(bDrawOverlap)
        {
          mgrHis = new TMultiGraph();
          canRatio->Divide(1, 2, 0.01, 0.);
          pad = (TPad*)canRatio->cd(1);
          pad->SetPad(padx1, padymid, padx2, pady2);
          pad = (TPad*)canRatio->cd(2);
          pad->SetPad(padx1, pady1, padx2, padymid);
          pad->SetBottomMargin(0.15);
          grHis1 = MakeGraphErrors(vecHis[0][iAx], sTag1.Data(), iMyColors[1], iMyMarkersFull[0]);
          grHis2 = MakeGraphErrors(vecHis[1][iAx], sTag2.Data(), iMyColors[2], iMyMarkersFull[0]);
          mgrHis->Add(grHis1);
          mgrHis->Add(grHis2);
          mgrHis->SetTitle(Form("%s;%s;%s", sTitle.Data(), vecHis[0][iAx]->GetXaxis()->GetTitle(), vecHis[0][iAx]->GetYaxis()->GetTitle()));
          canRatio->cd(1);
          mgrHis->Draw("AP0");
          mgrHis->GetXaxis()->SetLimits(vecHis[0][iAx]->GetXaxis()->GetXmin(), vecHis[0][iAx]->GetXaxis()->GetXmax());
          mgrHis->GetXaxis()->SetLabelSize(dFontSizeAxis);
          mgrHis->GetXaxis()->SetTitleSize(dFontSizeAxis);
          mgrHis->GetYaxis()->SetLabelSize(dFontSizeAxis);
          mgrHis->GetYaxis()->SetTitleSize(dFontSizeAxis);
          legend = canRatio->cd(1)->BuildLegend(legx1, legy1, legx2, legy2);
          SetLegend(legend, dFontSizeLeg);
        }
        grRatio = MakeGraphErrors(vecRatio[iAx], "", iMyColors[0], iMyMarkersFull[0]);
        grRatio->SetTitle(Form("%s;%s;%s", sTitle.Data(), vecRatio[iAx]->GetXaxis()->GetTitle(), sLabelRatio.Data()));
        if(bRestrict)
        {
          grRatio->SetMinimum(dRatioMin);
          grRatio->SetMaximum(dRatioMax);
        }
        if(bRestrictAuto)
          AdjustRangeGraph(grRatio);
        canRatio->cd(bDrawOverlap ? 2 : 1);
        printf("Drawing ratio.\n");
        grRatio->Draw("AP0");
        grRatio->GetXaxis()->SetLimits(vecRatio[iAx]->GetXaxis()->GetXmin(), vecRatio[iAx]->GetXaxis()->GetXmax());
        grRatio->GetXaxis()->SetLabelSize(dFontSizeAxis);
        grRatio->GetXaxis()->SetTitleSize(dFontSizeAxis);
        grRatio->GetYaxis()->SetTitleOffset(1.08);
        grRatio->GetYaxis()->SetLabelSize(dFontSizeAxis);
        grRatio->GetYaxis()->SetTitleSize(dFontSizeAxis);
        if(grRatio->GetMaximum() > 1 && grRatio->GetMinimum() < 1)
        {
          lineOne = new TLine(vecRatio[iAx]->GetXaxis()->GetXmin(), 1, vecRatio[iAx]->GetXaxis()->GetXmax(), 1);
          lineOne->Draw();
        }
        canRatio->SaveAs(Form("%s_%s_%s%d.%s", sNameCan.Data(), sNameHis.Data(), (bTHnSlices ? "S" : ""), iAx, kImageSuf.Data()));
        delete grRatio;
        if(bDrawOverlap)
        {
          delete grHis1;
          delete grHis2;
          delete mgrHis;
        }
        delete canRatio;
      }
      break;
    default:
      break;
  }
  return 0;
}

TObject* GetHistogram(TFile* file, TString sPath, TString& sNameHis, Int_t& iDegree)
{
  // allowed path structures:
  // file/his
  // file/dir/his
  // file/list/his
  // file/dir/list/his
  // file/dir0/.../dirn/his

  TString sNameLevel0 = "";
  TString sNameLevel1 = "";

  TDirectoryFile* dir0 = 0;
  TList* list0 = 0;
  TList* list1 = 0;
  TObject* his = 0;

  TObjArray* arrayPath = 0;
  arrayPath = sPath.Tokenize("/");
  Int_t iNParts = arrayPath->GetEntriesFast();
  if(iNParts == 0)
  {
    printf("Error: No name found.\n");
    delete arrayPath;
    return NULL;
  }
  sNameHis = ((TObjString*)(arrayPath->At(iNParts - 1)))->GetString();

  // try the simple way first
  // expect file/his or file/dir0/.../dirn/his
  printf("Loading object %s from %s\n", sNameHis.Data(), sPath.Data());
  his = file->Get(sPath.Data());

  if(his)
  {
    delete arrayPath;
  }
  else // try the complicated way
  {
    if(iNParts > 3)
    {
      printf("Error: Too many levels.\n");
      delete arrayPath;
      return NULL;
    }
    if(iNParts > 1)
    {
      sNameLevel0 = ((TObjString*)(arrayPath->At(0)))->GetString();
      if(iNParts == 3)
        sNameLevel1 = ((TObjString*)(arrayPath->At(1)))->GetString();
    }
    delete arrayPath;

    TString sFullPath = "";
    sFullPath += (sNameLevel0.Length() ? sNameLevel0 + "/" : "");
    sFullPath += (sNameLevel1.Length() ? sNameLevel1 + "/" : "");
    sFullPath += sNameHis;
    printf("Interpreted path: %s\n", sFullPath.Data());

    if(sNameLevel0.Length())
    {
      printf("Loading level 0: %s ", sNameLevel0.Data());
      file->GetObject(sNameLevel0.Data(), dir0); // try file/dir/his or file/dir/list/his
      file->GetObject(sNameLevel0.Data(), list0); // try file/list/his
      if(!dir0 && !list0)
      {
        printf("failed (Error)\n");
        return NULL;
      }
      printf("OK\n");

      if(sNameLevel1.Length()) // expect file/dir/list/his
      {
        printf("Loading level 1: %s ", sNameLevel1.Data());
        if(!dir0)
        {
          printf("failed, dir0 not found (Error)\n");
          return NULL;
        }
        dir0->GetObject(sNameLevel1.Data(), list1);
        if(!list1)
        {
          printf("failed, list not found (Error)\n");
          return NULL;
        }
        printf("OK\n");
      }
    }

//    printf("Loading histogram %s\n", sNameHis.Data());
    if(list1) // file/dir/list/his
    {
//    printf("Loading from list\n");
      his = list1->FindObject(sNameHis.Data());
    }
    else if(list0) // file/list/his
    {
//    printf("Loading from list\n");
      his = list0->FindObject(sNameHis.Data());
    }
    else if(dir0) // file/dir/his (this should be handled by the simple way)
    {
//    printf("Loading from dir\n");
      his = dir0->Get(sNameHis.Data());
    }
    else // file/his (this should be handled by the simple way)
    {
//    printf("Loading from file\n");
      his = file->Get(sNameHis.Data());
    }
  }

  if(!his)
  {
    printf("Error: Failed to load object %s\n", sNameHis.Data());
    return NULL;
  }

  // determine degree of histogram
  if(his->InheritsFrom("RooUnfoldResponse"))
    iDegree = 5;
  else if(his->InheritsFrom("THnSparse"))
    iDegree = 4;
  else if(his->InheritsFrom("TH3"))
    iDegree = 3;
  else if(his->InheritsFrom("TH2"))
    iDegree = 2;
  else if(his->InheritsFrom("TH1"))
    iDegree = 1;
  else
  {
    printf("Error: Object %s is not histogram-like!\n", sNameHis.Data());
    iDegree = 0;
  }

  return his;
}

void AdjustRangeGraph(TGraph* gr, Double_t dMargin)
{
  if(!gr)
  {
    printf("AdjustRangeGraph: Error: No graph!\n");
    return;
  }
  //printf("AdjustRangeGraph: Run\n");
  Int_t iNPoints = gr->GetN();
  Double_t dX, dY;
  Double_t dYMax = 0;
  Double_t dYMin = 0;
  for(Int_t iP = 0; iP < iNPoints; iP++)
  {
    gr->GetPoint(iP, dX, dY);
    if(dY == 0.)
      continue;
    if(dYMin == 0.)
      dYMin = dY;
    if(dYMax == 0.)
      dYMax = dY;
    dYMin = TMath::Min(dYMin, dY);
    dYMax = TMath::Max(dYMax, dY);
  }
  if(dYMin != 0. && dYMax != 0. && dYMin != dYMax)
  {
    Double_t dHeight = dYMax - dYMin;
    gr->SetMinimum(dYMin - dHeight * dMargin);
    gr->SetMaximum(dYMax + dHeight * dMargin);
  }
}

void OptimizeBinningTwo(TH1D** phis1, TH1D** phis2, Double_t dNMin)
{
  // optimise binning of phis1 and rebin phis2 accordingly
  Bool_t bTwice = 1; // optimise binning of his2 after rebinning as well and rebin phis1 accordingly
  TH1D* his1 = *phis1;
  TH1D* his2 = *phis2;
  his1 = OptimizeBinning(his1, dNMin);
  if(his1->GetXaxis()->IsVariableBinSize()) // histogram was rebinned
  {
    //printf("Binning change for his1.\n");
//    his2 = (TH1D*)his2->Rebin(his1->GetNbinsX(), Form("%s-2-Rebin", his2->GetName()), his1->GetXaxis()->GetXbins()->GetArray());
    his2 = (TH1D*)his2->Rebin(his1->GetNbinsX(), "", his1->GetXaxis()->GetXbins()->GetArray());
  }
  //else
    //printf("No change for his1.\n");
  if(bTwice)
  {
    his2 = OptimizeBinning(his2, dNMin);
    if(his2->GetXaxis()->IsVariableBinSize()) // histogram was rebinned
    {
      //printf("Binning change for his2.\n");
//    his1 = (TH1D*)his1->Rebin(his2->GetNbinsX(), Form("%s-2-Rebin", his1->GetName()), his2->GetXaxis()->GetXbins()->GetArray());
      his1 = (TH1D*)his1->Rebin(his2->GetNbinsX(), "", his2->GetXaxis()->GetXbins()->GetArray());
    }
    //else
      //printf("No change for his2.\n");
  }
  *phis1 = his1;
  *phis2 = his2;
}
