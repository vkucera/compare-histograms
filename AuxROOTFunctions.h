Int_t iMyColors[] = {1, kBlue, 2, 8, kMagenta - 3, kOrange - 3, kRed + 2, 3}; // my personal colours
const Int_t iNMyColors = sizeof(iMyColors) / sizeof(iMyColors[0]);
Int_t iMyMarkersEmpty[] = {kOpenCircle, kOpenSquare, kOpenDiamond, kOpenCross, 30, kOpenTriangleUp, 32}; // my personal markers (empty)
const Int_t iNMyMarkersEmpty = sizeof(iMyMarkersEmpty) / sizeof(iMyMarkersEmpty[0]);
Int_t iMyMarkersFull[] = {kFullCircle, kFullSquare, 33, 34, kFullStar, kFullTriangleUp, kFullTriangleDown}; // my personal markers (full)
const Int_t iNMyMarkersFull = sizeof(iMyMarkersFull) / sizeof(iMyMarkersFull[0]);
Int_t lineStyles[5] = {1, 2, 3, 4, 5};
const Int_t iNLineStyles = sizeof(lineStyles) / sizeof(lineStyles[0]);

// ALICE official colours and markers
const Int_t alicolors[] = {kBlack, kRed + 1 , kBlue + 1, kGreen + 3, kMagenta + 1, kOrange - 1, kCyan + 2, kYellow + 2};
const Int_t alimarkers[] = {kFullCircle, kFullSquare, kFullCross, kFullDiamond, kFullStar, kOpenCircle, kOpenSquare, kOpenCross, kOpenDiamond, kOpenStar};
const Float_t alisizes[] = {1, 1, 1.3, 1.5, 1.5, 1, 1, 1.3, 1.5, 1.5};

void DDraw(TObject* object, TString drawOption = "", Int_t drawCounter = 0)
{
  if(drawCounter > 0)
    object->Draw(Form("SAME%s", drawOption.Data()));
  else
    object->Draw(drawOption.Data());
}

TList* GetList(TFile* file, TString dirName, TString listName)
{
  printf("Loading directory %s ", dirName.Data());
  TDirectoryFile* dir = (TDirectoryFile*)file->Get(dirName.Data());
  if(!dir)
  {
    printf("failed, Error\n");
    return NULL;
  }
  printf("OK\n");
  printf("Loading list %s ", listName.Data());
  TList* list = (TList*)dir->Get(listName.Data());
  if(!list)
  {
    printf("failed, Error\n");
    return NULL;
  }
  printf("OK\n");
  return list;
}

TH1F* GetHistogram1F(TList* list, TString hisName)
{
  printf("Loading histogram %s ", hisName.Data());
  TH1F* histogram = (TH1F*)list->FindObject(hisName.Data());
  if(!histogram)
  {
    printf("failed, Error\n");
    return NULL;
  }
  printf("OK\n");
  return histogram;
}

TH2F* GetHistogram2F(TList* list, TString hisName)
{
  printf("Loading histogram %s ", hisName.Data());
  TH2F* histogram = (TH2F*)list->FindObject(hisName.Data());
  if(!histogram)
  {
    printf("failed, Error\n");
    return NULL;
  }
  printf("OK\n");
  return histogram;
}

TH3F* GetHistogram3F(TList* list, TString hisName)
{
  printf("Loading histogram %s ", hisName.Data());
  TH3F* histogram = (TH3F*)list->FindObject(hisName.Data());
  if(!histogram)
  {
    printf("failed, Error\n");
    return NULL;
  }
  printf("OK\n");
  return histogram;
}

TH1D* GetHistogram1D(TList* list, TString hisName)
{
  printf("Loading histogram %s ", hisName.Data());
  TH1D* histogram = (TH1D*)list->FindObject(hisName.Data());
  if(!histogram)
  {
    printf("failed, Error\n");
    return NULL;
  }
  printf("OK\n");
  return histogram;
}

TH2D* GetHistogram2D(TList* list, TString hisName)
{
  printf("Loading histogram %s ", hisName.Data());
  TH2D* histogram = (TH2D*)list->FindObject(hisName.Data());
  if(!histogram)
  {
    printf("failed, Error\n");
    return NULL;
  }
  printf("OK\n");
  return histogram;
}

TH3D* GetHistogram3D(TList* list, TString hisName)
{
  printf("Loading histogram %s ", hisName.Data());
  TH3D* histogram = (TH3D*)list->FindObject(hisName.Data());
  if(!histogram)
  {
    printf("failed, Error\n");
    return NULL;
  }
  printf("OK\n");
  return histogram;
}

THnSparseD* GetSparseD(TList* list, TString hisName)
{
  printf("Loading histogram %s ", hisName.Data());
  THnSparseD* histogram = (THnSparseD*)list->FindObject(hisName.Data());
  if(!histogram)
  {
    printf("failed, Error\n");
    return NULL;
  }
  printf("OK\n");
  return histogram;
}

void CheckHistogram(TH1* hist)
{
  printf("\nChecking histogram %s\n", hist->GetName());
  UInt_t iEntriesHist = 0;
  Double_t fEntriesHist = 0.;
  for(Int_t i = 0; i < (hist->GetNbinsX() + 2); i++)
  {
//      printf("%f\n",fEntriesHist);
//      Int_t iTempSum = iEntriesHist;
//      Float_t fTempSum = fEntriesHist;
//      Double_t fTempSum = fEntriesHist;
    iEntriesHist += (int)hist->GetBinContent(i);
    fEntriesHist += hist->GetBinContent(i);
//      fEntriesHist=fEntriesHist+(hist->GetBinContent(i));
//      fEntriesHist=fTempSum+(hist->GetBinContent(i)); // NEFUNGUJE PRO FLOAT!!!
//      printf("%d+%d=%d\n",fTempSum,hist->GetBinContent(i),fEntriesHist);
//      printf("%d+%d=%d, %f+%f=%f(%f) => %d, %f\n",iTempSum,hist->GetBinContent(i),iEntriesHist,fTempSum,hist->GetBinContent(i),fEntriesHist,fTempSum+(hist->GetBinContent(i)),iEntriesHist-iTempSum-hist->GetBinContent(i),fEntriesHist-fTempSum-hist->GetBinContent(i));
    printf("%g - %g : %g\n", hist->GetXaxis()->GetBinLowEdge(i), hist->GetXaxis()->GetBinLowEdge(i + 1), hist->GetBinContent(i));
//      printf("%f\n",fEntriesHist);
  }
//  UInt_t iEntriesHistInt = (int)hist->Integral(0,hist->GetNbinsX()+1);
  Double_t fEntriesHistInt = hist->Integral(0, hist->GetNbinsX() + 1);
//  printf("\nEntries: %d, %f, Sum: %d, %f, Underflow: %d, Overflow: %d\n",hist->GetEntries(),hist->GetEntries(),iEntriesHist,fEntriesHist,hist->GetBinContent(0),hist->GetBinContent(hist->GetNbinsX()+1));
  printf("\nEntries: %.f, Sum: %.f, Integral: %.f, Underflow: %.f, Overflow: %.f\n", hist->GetEntries(), fEntriesHist, fEntriesHistInt, hist->GetBinContent(0), hist->GetBinContent(hist->GetNbinsX() + 1));
  printf("Histogram %s status: ", hist->GetName());
  if(hist->GetEntries() == fEntriesHist)
  {
    printf("OK");
  }
  else
  {
    printf("Bad (Difference: %.f)", hist->GetEntries() - fEntriesHist);
  }

  if(fEntriesHist == fEntriesHistInt)
  {
    printf(", OK");
  }
  else
  {
    printf(", Bad (Difference: %.f)", fEntriesHist - fEntriesHistInt);
  }
  printf("\n\n");
}

Double_t GetHistEntries(TH1* hist)
{
  if(!hist)
    return 0;
  Double_t fEntriesHist = 0;
  for(Int_t i = 1; i <= (hist->GetNbinsX()); i++)
  {
    fEntriesHist += hist->GetBinContent(i);
  }
  return fEntriesHist;
}

void SetLegend(TLegend* leg, Float_t fSizeText = 0.034)
{
  // setup legend
//   leg->SetFillStyle(0); // transparent
  leg->SetFillColor(kWhite); // white background
  leg->SetBorderSize(0); // no visible border
  leg->SetTextSize(fSizeText);
  return;
}

Double_t GetHistError(TH1* his)
{
  if(!his)
    return 0;
  Double_t dSumError2 = 0;
  Double_t dError = 0;
  for(Int_t i = 1; i <= (his->GetNbinsX()); i++)
  {
    dError = his->GetBinError(i);
    dSumError2 += (dError * dError);
  }
  return TMath::Sqrt(dSumError2);
}

TGraphErrors* MakeGraphErrors(TH1* his, TString title = "", Int_t color = kBlack, Int_t style = 2, Float_t size = 1)
{
  if(!his)
    return NULL;
  TGraphErrors* grHis = new TGraphErrors(his);
  if(title.Length())
    grHis->SetTitle(title.Data());
  else
    grHis->SetTitle(his->GetName());
  grHis->SetLineColor(color);
  grHis->SetMarkerColor(color);
  grHis->SetMarkerStyle(style);
  grHis->SetMarkerSize(size);
  return grHis;
}

void SetGraphAsymmErrors(TGraphAsymmErrors* grHis, TString title = "", Int_t color = kBlack, Int_t style = 2, Int_t size = 1)
{
  if(!grHis)
    return;
  if(title.Length())
    grHis->SetTitle(title.Data());
  else
    grHis->SetTitle(grHis->GetName());
  grHis->SetLineColor(color);
  grHis->SetMarkerColor(color);
  grHis->SetMarkerStyle(style);
  grHis->SetMarkerSize(size);
  return;
}

Double_t SqrtPlus(Double_t dNum1, Double_t dNum2)
{
  return TMath::Sqrt(dNum1 * dNum1 + dNum2 * dNum2);
}

Double_t SqrtMinus(Double_t dNum1, Double_t dNum2)
{
  return TMath::Sqrt(TMath::Abs(dNum1 * dNum1 - dNum2 * dNum2));
}

Double_t AddNumbersError(Double_t dNum1, Double_t dErr1, Double_t dNum2, Double_t dErr2, Double_t* dErrTot)
{
  Double_t dSum = dNum1 + dNum2;
  Double_t dErr = SqrtPlus(dErr1, dErr2);
  *dErrTot = dErr;
  return dSum;
}

Double_t DivideNumbers(Double_t dNum1, Double_t dNum2, Double_t dResultInf = 1e20)
{
  if(dNum2 == 0.)
    return dResultInf;
  else
    return dNum1 / dNum2;
}

Double_t DivideNumbersError(Double_t dNum1, Double_t dErr1, Double_t dNum2, Double_t dErr2, Double_t* dErrTot, Double_t dResultInf = 1e20)
{
  Double_t dRatio = DivideNumbers(dNum1, dNum2, dResultInf);
  Double_t dErr = 0;
  if(dRatio == dResultInf)
    dErr = dResultInf;
  else
    dErr = SqrtPlus(dErr1 / dNum2, dRatio * dErr2 / dNum2);
//  printf("DivideNumbersError: (%f +- %f)/(%f +- %f) = %f +- %f\n",dNum1,dErr1,dNum2,dErr2,dRatio,dErr);
  *dErrTot = dErr;
  return dRatio;
}

Double_t MultiplyNumbersError(Double_t dNum1, Double_t dErr1, Double_t dNum2, Double_t dErr2, Double_t* dErrTot)
{
  Double_t dProduct = dNum1 * dNum2;
  Double_t dErr = SqrtPlus(dErr1 * dNum2, dErr2 * dNum1);
//  printf("MultiplyNumbersError: (%f +- %f)*(%f +- %f) = %f +- %f\n",dNum1,dErr1,dNum2,dErr2,dProduct,dErr);
  *dErrTot = dErr;
  return dProduct;
}

TH1D* DivideHistograms1D(TH1* his1, TH1* his2, TString sNameDiv = "")
{
  if(!his1 || !his2)
  {
    printf("DivideHistograms1D: Error: Invalid histograms!\n");
    return NULL;
  }
  printf("DivideHistograms1D: Start: %s/%s\n", his1->GetName(), his2->GetName());
  if(!CompareAxes(his1->GetXaxis(), his2->GetXaxis()))
  {
    printf("DivideHistograms1D: Error: Axis bins do not match!\n");
    return NULL;
  }
  if(!sNameDiv.Length())
    sNameDiv = Form("%s%s", his1->GetName(), "-Div");
  TH1D* hisDiv = (TH1D*)his1->Clone(sNameDiv.Data());
  hisDiv->Reset(); // reset integral
  hisDiv->SetTitle(Form("%s/%s;%s;%s/%s", his1->GetName(), his2->GetName(), his1->GetXaxis()->GetTitle(), his1->GetYaxis()->GetTitle(), his2->GetYaxis()->GetTitle()));
  Double_t dRatio = 0;
  Double_t dErr = 0;
  Double_t dInf = -1;
  for(Int_t iBin = 1; iBin <= his1->GetNbinsX(); iBin++)
  {
    dRatio = DivideNumbersError(his1->GetBinContent(iBin), his1->GetBinError(iBin), his2->GetBinContent(iBin), his2->GetBinError(iBin), &dErr, dInf);
    if(dRatio == dInf)
      continue;
    hisDiv->SetBinContent(iBin, dRatio);
    hisDiv->SetBinError(iBin, dErr);
  }
  printf("DivideHistograms1D: End: %s/%s\n", his1->GetName(), his2->GetName());
  return hisDiv;
}

TH2D* DivideHistograms2D(TH2* his1, TH2* his2, TString sNameDiv = "")
{
  if(!his1 || !his2)
  {
    printf("DivideHistograms2D: Error: Invalid histograms!\n");
    return NULL;
  }
  printf("DivideHistograms2D: Start: %s/%s\n", his1->GetName(), his2->GetName());
  if(!CompareAxes2D(his1, his2))
    return NULL;
  if(!sNameDiv.Length())
    sNameDiv = Form("%s%s", his1->GetName(), "-Div");
  TH2D* hisDiv = (TH2D*)his1->Clone(sNameDiv.Data());
  hisDiv->Reset(); // reset integral
  hisDiv->SetTitle(Form("%s/%s", his1->GetName(), his2->GetName()));
  Double_t dRatio = 0;
  Double_t dErr = 0;
  Double_t dInf = -1;
  for(Int_t iBinX = 1; iBinX <= his1->GetNbinsX(); iBinX++)
    for(Int_t iBinY = 1; iBinY <= his1->GetNbinsY(); iBinY++)
    {
      dRatio = DivideNumbersError(his1->GetBinContent(iBinX, iBinY), his1->GetBinError(iBinX, iBinY), his2->GetBinContent(iBinX, iBinY), his2->GetBinError(iBinX, iBinY), &dErr, dInf);
      if(dRatio == dInf)
        continue;
      hisDiv->SetBinContent(iBinX, iBinY, dRatio);
      hisDiv->SetBinError(iBinX, iBinY, dErr);
    }
  printf("DivideHistograms2D: End: %s/%s\n", his1->GetName(), his2->GetName());
  return hisDiv;
}

TH2D* MultiplyHistograms2D(TH2* his1, TH2* his2, TString sNameMult = "")
{
  printf("MultiplyHistograms2D: Start: %s*%s\n", his1->GetName(), his2->GetName());
  if(!CompareAxes2D(his1, his2))
    return NULL;
  if(!sNameMult.Length())
    sNameMult = Form("%s%s", his1->GetName(), "-Mult");
  TH2D* hisMult = (TH2D*)his1->Clone(sNameMult.Data());
  hisMult->Reset(); // reset integral
  hisMult->SetTitle(Form("%s*%s", his1->GetName(), his2->GetName()));
  Double_t dProduct = 0;
  Double_t dErr = 0;
  for(Int_t iBinX = 1; iBinX <= his1->GetNbinsX(); iBinX++)
    for(Int_t iBinY = 1; iBinY <= his1->GetNbinsY(); iBinY++)
    {
      dProduct = MultiplyNumbersError(his1->GetBinContent(iBinX, iBinY), his1->GetBinError(iBinX, iBinY), his2->GetBinContent(iBinX, iBinY), his2->GetBinError(iBinX, iBinY), &dErr);
      hisMult->SetBinContent(iBinX, iBinY, dProduct);
      hisMult->SetBinError(iBinX, iBinY, dErr);
    }
  printf("MultiplyHistograms2D: End: %s*%s\n", his1->GetName(), his2->GetName());
  return hisMult;
}

TH1D* DivideHistogram(TH1* his1, Double_t dNumber, Double_t dError, Bool_t bBinWidth = 0, TString sNameDiv = "")
{
  if(!his1)
  {
    printf("DivideHistogram: Error: Invalid histogram!\n");
    return NULL;
  }
  if(dNumber == 0)
  {
    printf("DivideHistogram: Error: Division by zero not allowed\n");
    return NULL;
  }
  printf("DivideHistogram: Start: %s/%f\n", his1->GetName(), dNumber);
  if(!sNameDiv.Length())
    sNameDiv = Form("%s%s", his1->GetName(), "-Div");
  TH1D* hisDiv = (TH1D*)his1->Clone(sNameDiv.Data());
  hisDiv->Reset(); // reset integral
  hisDiv->SetTitle(Form("%s/%g", his1->GetName(), dNumber));
  Double_t dRatio = 0;
  Double_t dErr = 0;
  Double_t dInf = -1;
  for(Int_t iBin = 1; iBin <= his1->GetNbinsX(); iBin++)
  {
    dRatio = DivideNumbersError(his1->GetBinContent(iBin), his1->GetBinError(iBin), dNumber, dError, &dErr, dInf);
    if(dRatio == dInf)
      continue;
    hisDiv->SetBinContent(iBin, dRatio);
    hisDiv->SetBinError(iBin, dErr);
  }
  // divide by bin width
  if(bBinWidth)
    hisDiv->Scale(1., "width");
  printf("DivideHistogram: End: %s/%f\n", his1->GetName(), dNumber);
  return hisDiv;
}

TH1D* MultiplyHistogram(TH1* his1, Double_t dNumber, Double_t dError, Bool_t bBinWidth = 0, TString sNameMult = "")
{
  printf("%s: Start: %s*%g\n", __func__, his1->GetName(), dNumber);
  if(!his1)
  {
    printf("%s: Error: Invalid histogram!\n", __func__);
    return NULL;
  }
  if(!sNameMult.Length())
    sNameMult = Form("%s%s", his1->GetName(), "-Mult");
  TH1D* hisMult = (TH1D*)his1->Clone(sNameMult.Data());
  hisMult->Reset(); // reset integral
  hisMult->SetTitle(Form("%s*%g", his1->GetName(), dNumber));
  Double_t dProduct = 0;
  Double_t dErr = 0;
  for(Int_t iBin = 1; iBin <= his1->GetNbinsX(); iBin++)
  {
    dProduct = MultiplyNumbersError(his1->GetBinContent(iBin), his1->GetBinError(iBin), dNumber, dError, &dErr);
    hisMult->SetBinContent(iBin, dProduct);
    hisMult->SetBinError(iBin, dErr);
  }
  // Divide by bin width
  if(bBinWidth)
    hisMult->Scale(1., "width");
  printf("%s: End: %s*%f\n", __func__, his1->GetName(), dNumber);
  return hisMult;
}

Bool_t CompareAxes(TAxis* ax1, TAxis* ax2)
{
  if(!ax1 || !ax2)
  {
    printf("CompareAxes: Error: Invalid axes!\n");
    return kFALSE;
  }
  std::vector<Double_t> axisX1;
  std::vector<Double_t> axisX2;
  for(Int_t iBin = 1; iBin <= ax1->GetNbins() + 1; iBin++)
    axisX1.push_back(ax1->GetBinLowEdge(iBin));
  for(Int_t iBin = 1; iBin <= ax2->GetNbins() + 1; iBin++)
    axisX2.push_back(ax2->GetBinLowEdge(iBin));
  if(axisX1 != axisX2)
    return kFALSE;
  return kTRUE;
}

Bool_t CompareAxes2D(TH2* his1, TH2* his2)
{
  if(!his1 || !his2)
  {
    printf("CompareAxes2D: Error: Invalid histograms!\n");
    return kFALSE;
  }
  if(!CompareAxes(his1->GetXaxis(), his2->GetXaxis()))
  {
    printf("CompareAxes2D: Error: Axis bins x do not match!\n");
    return kFALSE;
  }
  if(!CompareAxes(his1->GetYaxis(), his2->GetYaxis()))
  {
    printf("CompareAxes2D: Error: Axis bins y do not match!\n");
    return kFALSE;
  }
  return kTRUE;
}

Bool_t AreIdentical(TH1* his1, TH1* his2)
{
  if(!his1 || !his2)
  {
    printf("AreIdentical: Error: Invalid histograms!\n");
    return kFALSE;
  }

  // Compare number of entries
  if(his1->GetEntries() != his2->GetEntries())
    return kFALSE;

  // Compare axis binning
  if(!CompareAxes(his1->GetXaxis(), his2->GetXaxis()) || !CompareAxes(his1->GetYaxis(), his2->GetYaxis()) || !CompareAxes(his1->GetZaxis(), his2->GetZaxis()))
    return kFALSE;

  // Compare bin content
  for(Int_t iBinZ = 0; iBinZ <= his1->GetNbinsZ() + 1; iBinZ++)
    for(Int_t iBinY = 0; iBinY <= his1->GetNbinsY() + 1; iBinY++)
      for(Int_t iBinX = 0; iBinX <= his1->GetNbinsX() + 1; iBinX++)
      {
        Int_t iBin = his1->GetBin(iBinX, iBinY, iBinZ);
        if(his1->GetBinContent(iBin) != his2->GetBinContent(iBin) || his1->GetBinError(iBin) != his2->GetBinError(iBin))
          return kFALSE;
      }

  return kTRUE;
}

Bool_t AreIdentical(THnSparse* his1, THnSparse* his2)
{
  if(!his1 || !his2)
  {
    printf("AreIdentical: Error: Invalid histograms!\n");
    return kFALSE;
  }

  // Compare number of dimensions
  if (his1->GetNdimensions() != his2->GetNdimensions())
    return kFALSE;

  // Compare number of entries
  if(his1->GetEntries() != his2->GetEntries())
    return kFALSE;

  // Compare number of filled bins
  if (his1->GetNbins() != his2->GetNbins())
    return kFALSE;

  // Compare axis binning
  for (Int_t iAx = 0; iAx < his1->GetNdimensions(); iAx++)
  {
    if(!CompareAxes(his1->GetAxis(iAx), his2->GetAxis(iAx)))
      return kFALSE;
  }

  // Compare bin content
  for(Int_t iBin = 0; iBin < his1->GetNbins(); iBin++)
  {
    if(his1->GetBinContent(iBin) != his2->GetBinContent(iBin) || his1->GetBinError(iBin) != his2->GetBinError(iBin))
      return kFALSE;
  }

  return kTRUE;
}

Bool_t AreIdentical(RooUnfoldResponse* his1, RooUnfoldResponse* his2)
{
  if(!his1 || !his2)
  {
    printf("AreIdentical: Error: Invalid histograms!\n");
    return kFALSE;
  }

  // Compare number of dimensions
  if (his1->GetDimensionMeasured() != his2->GetDimensionMeasured() || his1->GetDimensionTruth() != his2->GetDimensionTruth())
    return kFALSE;

  // Compare number of bins
  if (his1->GetNbinsMeasured() != his2->GetNbinsMeasured() || his1->GetNbinsTruth() != his2->GetNbinsTruth())
    return kFALSE;

  // Compare bin content
  TH1* hFake1 = his1->Hfakes();
  TH1* hFake2 = his2->Hfakes();
  if (!AreIdentical(hFake1, hFake2))
    return kFALSE;
  TH1* hMeas1 = his1->Hmeasured();
  TH1* hMeas2 = his2->Hmeasured();
  if (!AreIdentical(hMeas1, hMeas2))
    return kFALSE;
  TH1* hTruth1 = his1->Htruth();
  TH1* hTruth2 = his2->Htruth();
  if (!AreIdentical(hTruth1, hTruth2))
    return kFALSE;
  TH2* hResp1 = his1->Hresponse();
  TH2* hResp2 = his2->Hresponse();
  if (!AreIdentical(hResp1, hResp2))
    return kFALSE;

  return kTRUE;
}

Double_t CalculateSimilarity(Double_t dNumTest, Double_t dNumTestErr, Double_t dNumRef, Double_t dNumRefErr, Double_t* dResultErr, Double_t dResultInf = 1e20)
{
  Double_t dResult = dResultInf;
  if(dNumRefErr == 0)
    *dResultErr = dResultInf;
  else
  {
    dResult = (dNumTest - dNumRef) / dNumRefErr;
    *dResultErr = dNumTestErr / dNumRefErr;
  }
  return dResult;
}

Double_t CalculateSimilaritySym(Double_t dNum1, Double_t dNum1Err, Double_t dNum2, Double_t dNum2Err, Double_t* dResultErr, Double_t dResultInf = 1e20)
{
  Double_t dResult = dResultInf;
  if(dNum1Err == 0 && dNum2Err == 0)
    *dResultErr = dResultInf;
  else
  {
    dResult = (dNum1 - dNum2) / SqrtPlus(dNum1Err, dNum2Err);
    *dResultErr = 0;
  }
  return dResult;
}

TH1D* CompareHistograms1D(TH1* hisTest, TH1* hisRef, TString sNameComp = "", Bool_t bCompErr = 0)
{
  printf("CompareHistograms1D: Start: %s/%s\n", hisTest->GetName(), hisRef->GetName());
  if(!hisTest || !hisRef)
  {
    printf("CompareHistograms1D: Error: Invalid histograms!\n");
    return NULL;
  }
  std::vector<Double_t> axisX1;
  std::vector<Double_t> axisX2;
  for(Int_t iBin = 1; iBin <= hisTest->GetNbinsX() + 1; iBin++)
    axisX1.push_back(hisTest->GetXaxis()->GetBinLowEdge(iBin));
  for(Int_t iBin = 1; iBin <= hisRef->GetNbinsX() + 1; iBin++)
    axisX2.push_back(hisRef->GetXaxis()->GetBinLowEdge(iBin));
  if(axisX1 != axisX2)
  {
    printf("CompareHistograms1D: Error: Axis bins do not match!\n");
    return NULL;
  }
  if(!sNameComp.Length())
    sNameComp = Form("%s-vs-%s", hisTest->GetName(), hisRef->GetName());
  TH1D* hisComp = (TH1D*)hisTest->Clone(sNameComp.Data());
  hisComp->Reset(); // reset integral
  hisComp->SetTitle(Form("%s vs %s;%s;(#it{y}_{test}-#it{y}_{ref})/#it{#sigma}_{ref}", hisTest->GetName(), hisRef->GetName(), hisTest->GetXaxis()->GetTitle()));
  Double_t dSim = 0;
  Double_t dErr = 0;
  Double_t dInf = -1;
  for(Int_t iBin = 1; iBin <= hisTest->GetNbinsX(); iBin++)
  {
//      dSim = CalculateSimilarity(hisTest->GetBinContent(iBin),hisTest->GetBinError(iBin),hisRef->GetBinContent(iBin),hisRef->GetBinError(iBin),&dErr,dInf);
    dSim = CalculateSimilaritySym(hisTest->GetBinContent(iBin), hisTest->GetBinError(iBin), hisRef->GetBinContent(iBin), hisRef->GetBinError(iBin), &dErr, dInf);
    if(dSim == dInf)
      continue;
    if(bCompErr)
    {
      dErr = DivideNumbers(hisTest->GetBinError(iBin), hisRef->GetBinError(iBin), dInf);
      if(dErr == dInf)
        continue;
      hisComp->SetBinContent(iBin, dErr);
      hisComp->SetBinError(iBin, 0);
    }
    else
    {
      hisComp->SetBinContent(iBin, dSim);
      hisComp->SetBinError(iBin, dErr);
    }
  }
  printf("CompareHistograms1D: End: %s/%s\n", hisTest->GetName(), hisRef->GetName());
  return hisComp;
}

//Double_t* GetBinEdges(TAxis* axis, Int_t* nbins)
//{
//  const Int_t iNbins = axis->GetNbins();
//  std::vector<Double_t> array;
//  for (Int_t i=0; i<=iNbins; i++)
//    array.push_back(axis->GetBinLowEdge(i+1));
//  *nbins = iNbins;
//  return &array[0];
//}

Bool_t GetParabola(Double_t xA, Double_t yA, Double_t xB, Double_t yB, Double_t xC, Double_t yC, Double_t* a, Double_t* b, Double_t* c)
{
  if((xA == xB) || (xA == xC) || (xC == xB))
  {
    printf("GetParabola: Error: Equal x values\n");
    return kFALSE;
  }
  Double_t DxB = xB - xA;
  Double_t DxC = xC - xA;
  Double_t DyB = yB - yA;
  Double_t DyC = yC - yA;
  Double_t aP = (DyB / DxB - DyC / DxC) / (DxB - DxC);
  Double_t bP = (DyC * DxB / DxC - DyB * DxC / DxB) / (DxB - DxC);
  *a = aP;
  *b = bP - 2 * aP * xA;
  *c = yA + aP * xA * xA - bP * xA;
  return kTRUE;
}

Double_t GetOverlapFraction(Double_t dXBinMin, Double_t dXBinMax, Double_t dXRangeMin, Double_t dXRangeMax)
{
  if(dXBinMin >= dXBinMax)
  {
    printf("GetOverlapFraction: Error: dXBinMin>=dXBinMax\n");
    return 0.;
  }
  if(dXRangeMin >= dXRangeMax)
  {
    printf("GetOverlapFraction: Error: dXRangeMin>=dXRangeMax\n");
    return 0.;
  }
  if((dXBinMax <= dXRangeMin) || (dXRangeMax <= dXBinMin))
    return 0.;
  Double_t dXOverlapMin = TMath::Max(dXBinMin, dXRangeMin);
  Double_t dXOverlapMax = TMath::Min(dXBinMax, dXRangeMax);
  return (dXOverlapMax - dXOverlapMin) / (dXBinMax - dXBinMin);
}

// Does not work
TH1D* GetHistogramFromGraph(TGraph* grIn)
{
  if(!grIn)
  {
    printf("GetHistogramFromGraph: Error: No TGraph\n");
    return NULL;
  }
  printf("TGraph: %s, %s\n", grIn->GetName(), grIn->GetTitle());

  // Get the number of data points
  Int_t a = grIn->GetN();
//  const Int_t iNBins = a;
//  static const Int_t iNBins = a;
  static const Int_t iNBins = 33;
  printf("Nbins: %d\n", iNBins);
  printf("Nbins: %d\n", a);
  Double_t dArrayBins[iNBins + 1];

  // Get data points and errors
  Double_t* dArrayX = grIn->GetX();
  Double_t* dArrayEXHigh = grIn->GetEXhigh();
  Double_t* dArrayEXLow = grIn->GetEXlow();
  Double_t* dArrayY = grIn->GetY();
  Double_t* dArrayEYHigh = grIn->GetEYhigh();
//  Double_t* dArrayEYLow = grIn->GetEYlow();

  // Get an array of bin edges
  for(Int_t i = 0; i < iNBins; i++)
    dArrayBins[i] = dArrayX[i] - dArrayEXLow[i];
  dArrayBins[iNBins] = dArrayX[iNBins - 1] + dArrayEXHigh[iNBins - 1];

  // Print the bin edges
  for(Int_t i = 0; i <= iNBins; i++)
    printf("Bin %d: %f\n", i, dArrayBins[i]);

  // Create the target histogram
  TH1D* his = new TH1D(Form("his%s", grIn->GetName()), grIn->GetTitle(), iNBins, dArrayBins);

  // Copy data points values into the histogram bins
  for(Int_t iBin = 1; iBin <= iNBins; iBin++)
  {
    his->SetBinContent(iBin, dArrayY[iBin - 1]);
    his->SetBinError(iBin, dArrayEYHigh[iBin - 1]);
  }
  return his;
}

Double_t Energy(Double_t dMass, Double_t dMom)
{
  return TMath::Sqrt(dMass * dMass + dMom * dMom);
}

Double_t Momentum(Double_t dPt, Double_t dAngle)
{
  Double_t dSin = TMath::Sin(dAngle);
  if(dSin == 0)
    return 1e20;
  return dPt / dSin;
}

Double_t Rapidity(Double_t dAngle, Double_t dMass, Double_t dPt)
{
  Double_t dP = Momentum(dPt, dAngle);
  Double_t dE = Energy(dMass, dP);
  Double_t dDenom = dE - dP * TMath::Cos(dAngle);
  if(dDenom == 0)
    return 1e20;
  Double_t dNum = dE + dP * TMath::Cos(dAngle);
  return 0.5 * TMath::Log(dNum / dDenom);
}

Double_t Pseudorapidity(Double_t dAngle)
{
  return -TMath::Log(TMath::Tan(dAngle / 2));
}

Double_t Angle(Double_t dEta)
{
  return 2 * TMath::ATan(TMath::Exp(-dEta));
}

Bool_t RebinHistogram2D(TH2* hisSource, TH2* hisTarget)
{
  if(!hisSource || !hisTarget)
  {
    printf("RebinHistogram2D: Error: Invalid histogram(s)\n");
    return kFALSE;
  }
  TAxis* xaxis;
  TAxis* yaxis;
  xaxis = hisSource->GetXaxis();
  yaxis = hisSource->GetYaxis();
  for(int j = 1; j <= yaxis->GetNbins(); j++)
    for(int i = 1; i <= xaxis->GetNbins(); i++)
      hisTarget->Fill(xaxis->GetBinCenter(i), yaxis->GetBinCenter(j), hisSource->GetBinContent(i, j));
  return kTRUE;
}

TGraphAsymmErrors* GetExtremes(TH1D** arrayHis, const Int_t iNHis, const Int_t iHisRef, Double_t dErrRel = 0)
{
  Int_t iMethod = 4;
  // 0 - asymm. maximum deviation (default)
  // 1 - symm. maximum deviation
  // 2 - symm. mean absolute deviation
  // 3 - symm. rms (sqrt(sum(err^2)/n))
  // 4 - symm. weighted rms
  // 5 - symm. weighted mean absolute deviation
  Bool_t bCorrelatedErr = 0; // divide combined error by sqrt(2)
  if(iNHis <= 0)
  {
    printf("GetExtremes: Error: Wrong number of array elements.\n");
    return NULL;
  }
  if(iHisRef < 0 || iHisRef > (iNHis - 1))
  {
    printf("GetExtremes: Error: Wrong index of reference histogram.\n");
    return NULL;
  }
  for(Int_t iHis = 0; iHis < iNHis; iHis++)
  {
    if(!(arrayHis[iHis]))
    {
      printf("GetExtremes: Error: Invalid histogram %d/%d.\n", iHis, iNHis);
      return NULL;
    }
  }
  TH1D* hisRef = (TH1D*)arrayHis[iHisRef]; // reference histogram
  Int_t iNBinsX = hisRef->GetNbinsX(); // number of bins
  TGraphAsymmErrors* grExtremes = new TGraphAsymmErrors(hisRef); // output graph

  Double_t dValRef, dValVar, dValRefErr, dValVarErr, dMin, dMax, dErrMax, dErrMin, dErrSum, dErrSqSum, dDiff, dWeight, dWeightSum, dWeightErr, dErrSqWSum, dInf;
  dInf = 1e20;
  for(Int_t iBin = 1; iBin <= iNBinsX; iBin++) // loop over bins
  {
    Bool_t bStatus = kTRUE;
    dErrMin = 0;
    dErrMax = 0;
    dValRef = hisRef->GetBinContent(iBin); // reference y value
    dValRefErr = hisRef->GetBinError(iBin); // reference y value
    dErrSum = 0; // initialisation of the sum of all errors (method 2)
    dErrSqSum = 0; // initialisation of the sum of squares of all errors (method 3)
    dErrSqWSum = 0; // initialisation of the sum of weighted squares of all errors (method 4)
//    dWeightSum = 0; // initialisation of the sum of weights (method 4)
    printf("GetExtremes: Processing bin %d/%d, ref: %g\n", iBin, iNBinsX, dValRef);
    if(iNHis == 1)
    {
      dErrMax = TMath::Abs(dValRef * dErrRel);
      dErrMin = TMath::Abs(dValRef * dErrRel);
    }
    else
    {
      dMax = dValRef; // initial maximum
      dMin = dValRef; // initial minimum
      for(Int_t iVar = 0; iVar < iNHis; iVar++) // loop over histograms
      {
        if(iVar == iHisRef)
          continue;
        dValVar = arrayHis[iVar]->GetBinContent(iBin);
        dValVarErr = arrayHis[iVar]->GetBinError(iBin);
        dDiff = TMath::Abs(dValVar - dValRef);
        dErrSum += dDiff;
        dErrSqSum += dDiff * dDiff;
        dWeight = TMath::Abs(CalculateSimilaritySym(dValVar, dValVarErr, dValRef, dValRefErr, &dWeightErr, dInf)); // |difference|/sigma_combined
        if(bCorrelatedErr)
          dWeight *= TMath::Sqrt(2.); // correlated errors
        if(iMethod == 4 && dWeightErr == dInf)
        {
          printf("GetExtremes: Error: Both errors are zero, skipping bin.\n");
          bStatus = kFALSE;
          break;
        }
        dWeight = 1 - TMath::Gaus(dWeight); // probability of not being a fluctuation
        dErrSqWSum += dDiff * dDiff * dWeight;
//        dWeightSum += dWeight;
        printf("Weight = %g\n", dWeight);

        printf("GetExtremes: Processing histogram %d/%d: %g\n", iVar, iNHis, dValVar);
        dMin = TMath::Min(dMin, dValVar);
        dMax = TMath::Max(dMax, dValVar);
      }
      if(bStatus)
      {
        // asymmetric maximum deviation (default)
        dErrMax = (dMax - dValRef);
        dErrMin = (dValRef - dMin);
        // symmetric maximum deviation
        if(iMethod == 1)
        {
          dErrMax = TMath::Max(dErrMax, dErrMin);
          dErrMin = dErrMax;
        }
        // symmetric mean absolute deviation
        else if(iMethod == 2)
        {
          dErrMax = dErrSum / (iNHis - 1);
          dErrMin = dErrMax;
        }
        // symmetric RMS
        else if(iMethod == 3)
        {
          dErrMax = TMath::Sqrt(dErrSqSum / (iNHis - 1));
          dErrMin = dErrMax;
        }
        // symmetric weighted RMS
        else if(iMethod == 4)
        {
//          dErrMax = 0;
//          if(dWeightSum != 0.)
//            dErrMax = TMath::Sqrt(dErrSqWSum / dWeightSum);
          dErrMax = TMath::Sqrt(dErrSqWSum / (iNHis - 1));
          dErrMin = dErrMax;
        }
      }
    }
    if(dValRef != 0)
    {
      printf("GetExtremes: Max err: %g %%\n", 100 * dErrMax / TMath::Abs(dValRef));
      printf("GetExtremes: Min err: %g %%\n", 100 * dErrMin / TMath::Abs(dValRef));
    }
    grExtremes->SetPointEYhigh(iBin - 1, dErrMax);
    grExtremes->SetPointEYlow(iBin - 1, dErrMin);
  }
  return grExtremes;
}

TGraphAsymmErrors* GetExtremesCombined(TGraphAsymmErrors** arrayGr, const Int_t iNGr, const Int_t iGrRef)
{
  Int_t iMethod = 1; // 0 - asymm. maximum err, 1 - asymm. sqrt(sum err^2)
  if(iNGr <= 0)
  {
    printf("GetExtremesCombined: Error: Wrong number of array elements.\n");
    return NULL;
  }
  if(iGrRef < 0 || iGrRef > (iNGr - 1))
  {
    printf("GetExtremesCombined: Error: Wrong index of reference graph.\n");
    return NULL;
  }
  for(Int_t iHis = 0; iHis < iNGr; iHis++)
  {
    if(!(arrayGr[iHis]))
    {
      printf("GetExtremesCombined: Error: Invalid graph %d/%d.\n", iHis, iNGr);
      return NULL;
    }
  }
  TGraphAsymmErrors* grRef = (TGraphAsymmErrors*)arrayGr[iGrRef]; // reference graph
  Int_t iNBinsX = grRef->GetN(); // number of points
  TGraphAsymmErrors* grExtremes = new TGraphAsymmErrors(*grRef); // output graph

  Double_t dValXRef, dValYRef, dValXVar, dValYVar, dErrMax, dErrMin, dErrMaxVar, dErrMinVar, dErrMaxTotSqr, dErrMinTotSqr;
  for(Int_t iBin = 0; iBin < iNBinsX; iBin++) // loop over bins
  {
    grRef->GetPoint(iBin, dValXRef, dValYRef); // reference x-y value
    dErrMin = grRef->GetErrorYlow(iBin); // initial minimum error
    dErrMax = grRef->GetErrorYhigh(iBin); // initial maximum error
    dErrMaxTotSqr = 0; // initialisation of the sum of (positive err)^2
    dErrMinTotSqr = 0; // initialisation of the sum of (negative err)^2
    printf("GetExtremesCombined: Processing point %d/%d, ref: %g, %g\n", iBin, iNBinsX, dValXRef, dValYRef);
    for(Int_t iVar = 0; iVar < iNGr; iVar++) // loop over histograms
    {
      arrayGr[iVar]->GetPoint(iBin, dValXVar, dValYVar);
      dErrMaxVar = arrayGr[iVar]->GetErrorYhigh(iBin);
      dErrMinVar = arrayGr[iVar]->GetErrorYlow(iBin);
      dErrMaxTotSqr += dErrMaxVar * dErrMaxVar;
      dErrMinTotSqr += dErrMinVar * dErrMinVar;
      printf("GetExtremesCombined: Processing graph %d/%d: %g, %g + %g - %g\n", iVar, iNGr, dValXVar, dValYVar, dErrMaxVar, dErrMinVar);
      if((dValXVar != dValXRef) || (dValYVar != dValYRef))
      {
        printf("GetExtremesCombined: Error: Points are different.\n");
        delete grExtremes;
        return NULL;
      }
      dErrMin = TMath::Max(dErrMin, dErrMinVar);
      dErrMax = TMath::Max(dErrMax, dErrMaxVar);
    }
    if(iMethod == 1)
    {
      dErrMin = TMath::Sqrt(dErrMinTotSqr);
      dErrMax = TMath::Sqrt(dErrMaxTotSqr);
    }
    if(dValYRef != 0)
    {
      printf("GetExtremesCombined: Max err: %g %%\n", 100 * dErrMax / TMath::Abs(dValYRef));
      printf("GetExtremesCombined: Min err: %g %%\n", 100 * dErrMin / TMath::Abs(dValYRef));
    }
    grExtremes->SetPointEYlow(iBin, dErrMin);
    grExtremes->SetPointEYhigh(iBin, dErrMax);
  }
  return grExtremes;
}

TGraphAsymmErrors* DivideGraphAsymm(TGraphAsymmErrors* grNum, TGraphAsymmErrors* grDen)
{
  if(!grNum || !grDen)
    return NULL;
  Double_t dValXNum, dValYNum, dErrYPlusNum, dErrYMinusNum, dValXDen, dValYDen, dErrYPlusDen, dErrYMinusDen, dValYDiv, dErrYPlusDiv, dErrYMinusDiv;
  TGraphAsymmErrors* grDiv = new TGraphAsymmErrors(*grNum);
  Int_t iNBinsX = grNum->GetN(); // number of points
  for(Int_t iBin = 0; iBin < iNBinsX; iBin++) // loop over points
  {
    grNum->GetPoint(iBin, dValXNum, dValYNum); // x, y
    dErrYMinusNum = grNum->GetErrorYlow(iBin); // negative error
    dErrYPlusNum = grNum->GetErrorYhigh(iBin); // positive error
    grDen->GetPoint(iBin, dValXDen, dValYDen);
    dErrYMinusDen = grDen->GetErrorYlow(iBin); // negative error
    dErrYPlusDen = grDen->GetErrorYhigh(iBin); // positive error
    if(dValXNum != dValXDen)
    {
      printf("DivideGraphAsymm: Error: Points have different x.\n");
      delete grDiv;
      return NULL;
    }
    dValYDiv = DivideNumbersError(dValYNum, dErrYPlusNum, dValYDen, dErrYMinusDen, &dErrYPlusDiv, -333);
    dValYDiv = DivideNumbersError(dValYNum, dErrYMinusNum, dValYDen, dErrYPlusDen, &dErrYMinusDiv, -333);
    grDiv->SetPoint(iBin, dValXNum, dValYDiv);
    grDiv->SetPointEYlow(iBin, dErrYMinusDiv);
    grDiv->SetPointEYhigh(iBin, dErrYPlusDiv);
  }
  return grDiv;
}

TH1D* GetRelativeErrors(TH1D* his)
{
  if(!his)
  {
    printf("GetRelativeErrors: Error: No histogram.\n");
    return NULL;
  }
  Double_t dVal, dErr, dErrRel;
  Double_t dInf = 1e20;
  TH1D* hisErrRel = (TH1D*)his->Clone();
  for(Int_t iBin = 1; iBin <= hisErrRel->GetNbinsX(); iBin++)
  {
    dVal = hisErrRel->GetBinContent(iBin); // y
    dErr = hisErrRel->GetBinError(iBin); // error
    dErrRel = DivideNumbers(dErr, dVal, dInf);
    if(dErrRel == dInf)
      continue;
    hisErrRel->SetBinContent(iBin, 0);
    hisErrRel->SetBinError(iBin, dErrRel);
  }
  return hisErrRel;
}

TGraphAsymmErrors* GetRelativeErrors(TGraphAsymmErrors* gr)
{
  if(!gr)
  {
    printf("GetRelativeErrors: Error: No graph.\n");
    return NULL;
  }
  Double_t dValX, dValY, dErrYPlus, dErrYMinus;
  Double_t dErrRelPlus, dErrRelMinus;
  Double_t dInf = 1e20;
  TGraphAsymmErrors* grErrRel = (TGraphAsymmErrors*)gr->Clone();
  for(Int_t iPoint = 0; iPoint < grErrRel->GetN(); iPoint++)
  {
    grErrRel->GetPoint(iPoint, dValX, dValY); // x, y
    dErrYMinus = grErrRel->GetErrorYlow(iPoint); // negative error
    dErrYPlus = grErrRel->GetErrorYhigh(iPoint); // positive error
    dErrRelMinus = DivideNumbers(dErrYMinus, dValY, dInf);
    if(dErrRelMinus == dInf)
      continue;
    dErrRelPlus = DivideNumbers(dErrYPlus, dValY, dInf);
    if(dErrRelPlus == dInf)
      continue;
    grErrRel->SetPoint(iPoint, dValX, 0);
    grErrRel->SetPointEYlow(iPoint, dErrRelMinus);
    grErrRel->SetPointEYhigh(iPoint, dErrRelPlus);
  }
  return grErrRel;
}

Bool_t ApplyRelativeErrors(TGraphAsymmErrors* gr, TGraphAsymmErrors* grErrRel)
{
  if(!gr || !grErrRel)
    return kFALSE;
  Double_t dValX, dValY, dErrYPlus, dErrYMinus;
  Double_t dErrRelPlus, dErrRelMinus;
  for(Int_t iPoint = 0; iPoint < gr->GetN(); iPoint++)
  {
    gr->GetPoint(iPoint, dValX, dValY); // x, y

    dErrRelMinus = grErrRel->GetErrorYlow(iPoint); // negative error
    dErrRelPlus = grErrRel->GetErrorYhigh(iPoint); // positive error

    dErrYMinus = dValY * dErrRelMinus;
    dErrYPlus = dValY * dErrRelPlus;

    gr->SetPointEYlow(iPoint, dErrYMinus);
    gr->SetPointEYhigh(iPoint, dErrYPlus);
  }
  return kTRUE;
}

void PrintBinning(TH1* his)
{
  if(!his)
  {
    printf("PrintBinning: Bad histogram\n");
    return;
  }
  Int_t iNBins = his->GetNbinsX();
  for(Int_t i = 1; i <= iNBins + 1; i++)
  {
//    printf("%g - %g : %g\n", hist->GetXaxis()->GetBinLowEdge(i), hist->GetXaxis()->GetBinLowEdge(i + 1), hist->GetBinContent(i));
    printf("%g", his->GetXaxis()->GetBinLowEdge(i));
    if(i != iNBins + 1)
      printf(", ");
    else
      printf("\n");
  }
}

void PrintContent(TH1* his)
{
  if(!his)
  {
    printf("PrintContent: Bad histogram\n");
    return;
  }
  Int_t iNBins = his->GetNbinsX();
  for(Int_t iBin = 1; iBin <= iNBins; iBin++)
    printf("%g - %g: %g\n", his->GetXaxis()->GetBinLowEdge(iBin), his->GetXaxis()->GetBinUpEdge(iBin), his->GetBinContent(iBin));
}

TH1D* OptimizeBinning(TH1D* hisSource, Double_t dNMin = 100)
{
  // change binning of the histogram so that there are at least dNMin entries in each not empty bin
  if(!hisSource)
    return NULL;
  /*
  printf("Source:\n");
  PrintBinning(hisSource);
  PrintContent(hisSource);
  */
  if(hisSource->Integral() < dNMin) // too few entries
    return hisSource;
  Int_t iNBinsOld = 0;
  Int_t iNBinsNew = 0;
  Double_t dBinEdge = 0;
  TString sNameNew = "";
  std::vector<Double_t> vecArrayXNew;
  iNBinsOld = hisSource->GetNbinsX();
//  sNameNew = Form("%s-Rebin", hisSource->GetName());
  Double_t dSum = 0;
  // find first and last not empty bin
  Int_t iBinFirst = -1, iBinLast = iNBinsOld;
  for(Int_t iBin = 1; iBin <= iNBinsOld; iBin++)
  {
    if(hisSource->GetBinContent(iBin) > 0)
    {
      if(iBinFirst == -1)
        iBinFirst = iBin;
      iBinLast = iBin;
    }
  }
//  printf("First: %d, last: %d\n",iBinFirst,iBinLast);
  for(Int_t iBin = iBinFirst; iBin <= iBinLast; iBin++) // avoid rebinning histograms with already good binning
  {
    if(hisSource->GetBinContent(iBin) < dNMin)
      break;
//    printf("Bin %d OK\n",iBin);
    if(iBin == iBinLast)
      return hisSource;
  }
//  printf("Full range: %g - %g (%d bins), not empty range: %g - %g (bins: %d - %d).\n", hisSource->GetXaxis()->GetXmin(), hisSource->GetXaxis()->GetXmax(), iNBinsOld, hisSource->GetXaxis()->GetBinLowEdge(iBinFirst), hisSource->GetXaxis()->GetBinUpEdge(iBinLast), iBinFirst, iBinLast);
  vecArrayXNew.push_back(hisSource->GetXaxis()->GetXmin());
  if(iBinFirst > 1) // first bin will be empty
  {
    iNBinsNew++;
    vecArrayXNew.push_back(hisSource->GetXaxis()->GetBinLowEdge(iBinFirst));
  }
  for(Int_t iBin = iBinFirst; iBin <= iBinLast; iBin++)
  {
    dSum += hisSource->GetBinContent(iBin);
    if(dSum >= dNMin)
    {
      iNBinsNew++;
      if(iBin < iBinLast && hisSource->Integral(iBin + 1, iBinLast) < dNMin) // not enough remaining entries for another bin
        iBin = iBinLast; // include the remaining old bins into the current new bin (last not empty bin)
      dBinEdge = hisSource->GetXaxis()->GetBinUpEdge(iBin);
      vecArrayXNew.push_back(dBinEdge);
      dSum = 0;
    }
  }
  if(iBinLast < iNBinsOld) // last bin will be empty
  {
    iNBinsNew++;
    vecArrayXNew.push_back(hisSource->GetXaxis()->GetBinUpEdge(iNBinsOld));
  }
  TH1D* hisRebin = (TH1D*)hisSource->Rebin(iNBinsNew, sNameNew.Data(), &vecArrayXNew[0]);
//  printf("Check number of bins: %d %d %d\n", hisRebin->GetNbinsX(), iNBinsNew, (int)vecArrayXNew.size() - 1);
//  printf("Integrals: old %g, new %g\n", hisSource->Integral(), hisRebin->Integral());
  /*
  printf("Result:\n");
  PrintBinning(hisRebin);
  PrintContent(hisRebin);
  */
  return hisRebin;
}
