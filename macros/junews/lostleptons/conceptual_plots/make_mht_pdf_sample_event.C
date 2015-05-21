#include "../plotting.h"
#include "../utils.h"
#include "../RatioError.h"
#include "TMath.h"
#include "TStyle.h"

void make_mht_pdf_sample_event(double ptW=450., double ht30=1000., int num_jets_pt30=7, TString outdir="macros/junews/lostleptons/default/") {

  TStyle *tdrStyle = new TStyle("tdrStyle","Style for P-TDR");
  setTDRStyle(tdrStyle);
  tdrStyle->cd();
  
  const int nbinsx(8); // ptW
  double binsx[nbinsx+1] = {200, 250, 300, 350, 400, 500, 600, 750, 1050};
  TH1D* hMHTpred= new TH1D("hMHTpred", ";H_{T}^{miss} [GeV];Events / bin", nbinsx, binsx); 
  RatioError* pred = new RatioError(hMHTpred);

  gStyle->SetOptStat(0);
  TH1::StatOverflows(kTRUE);
  TH1::SetDefaultSumw2(1);
  TH3::SetDefaultSumw2(1);

  TFile* fRLF = TFile::Open(outdir+"RLF/RLF.root");
  TH3D* hRLF = (TH3D*)fRLF->Get("hRLF");
  TFile* fDTT = TFile::Open(outdir+"PDF/ll_PDFs.root");
  TH1D* hPDFlost_ht1nj1= (TH1D*) fDTT->Get("hPDFlost_ht1nj1");
  TH1D* hPDFlost_ht1nj2= (TH1D*) fDTT->Get("hPDFlost_ht1nj2");
  TH1D* hPDFlost_ht1nj3= (TH1D*) fDTT->Get("hPDFlost_ht1nj3");
  TH1D* hPDFlost_ht2nj1= (TH1D*) fDTT->Get("hPDFlost_ht2nj1");
  TH1D* hPDFlost_ht2nj2= (TH1D*) fDTT->Get("hPDFlost_ht2nj2");
  TH1D* hPDFlost_ht2nj3= (TH1D*) fDTT->Get("hPDFlost_ht2nj3");
  TH1D* hPDFlost_ht3nj1= (TH1D*) fDTT->Get("hPDFlost_ht3nj1");
  TH1D* hPDFlost_ht3nj2= (TH1D*) fDTT->Get("hPDFlost_ht3nj2");
  TH1D* hPDFlost_ht3nj3= (TH1D*) fDTT->Get("hPDFlost_ht3nj3");

  double RLF = hRLF->GetBinContent(hRLF->FindBin(ptW, ht30, num_jets_pt30));
  double eRLF = hRLF->GetBinError(hRLF->FindBin(ptW, ht30, num_jets_pt30));

  printf("Event properties: ptW=%3.2f HT=%3.2f, NJets=%d, RLF=%3.2f+/-%3.2f\n", ptW, ht30, num_jets_pt30, RLF, eRLF);

  if (num_jets_pt30>=4&num_jets_pt30<=6) {
    if (ht30>500&&ht30<800) pred->FillMHTPDF(hPDFlost_ht1nj1, ptW, RLF, eRLF, true);
    else if (ht30>800&&ht30<1200) pred->FillMHTPDF(hPDFlost_ht2nj1, ptW, RLF, eRLF, true);
    else if (ht30>1200) pred->FillMHTPDF(hPDFlost_ht3nj1, ptW, RLF, eRLF, true);
  }
  else if (num_jets_pt30>=7&num_jets_pt30<=8) {
    if (ht30>500&&ht30<800) pred->FillMHTPDF(hPDFlost_ht1nj2, ptW, RLF, eRLF, true);
    else if (ht30>800&&ht30<1200) pred->FillMHTPDF(hPDFlost_ht2nj2, ptW, RLF, eRLF, true);
    else if (ht30>1200) pred->FillMHTPDF(hPDFlost_ht3nj2, ptW, RLF, eRLF, true);
  }
  else if (num_jets_pt30>=9){
    if (ht30>500&&ht30<800) pred->FillMHTPDF(hPDFlost_ht1nj3, ptW, RLF, eRLF, true);
    else if (ht30>800&&ht30<1200) pred->FillMHTPDF(hPDFlost_ht2nj3, ptW, RLF, eRLF, true);
    else if (ht30>1200) pred->FillMHTPDF(hPDFlost_ht3nj3, ptW, RLF, eRLF, true);
  }

  pred->SetPredictionsAndErrors(true);
  TH1D* hPred = pred->GetPrediction();
  set_style(hPred, "ttbar");

  TCanvas * c1 = new TCanvas("c1", "c1", 700, 700);
  c1->SetFillStyle(4000);
  hPred->GetYaxis()->SetTitle("Contribution from event (a.u.)");
  hPred->Draw("hist");
  TString plottitle = Form("%ssample_event_ptw%d_ht%d_nj%d.pdf",outdir.Data(),TMath::Nint(ptW), TMath::Nint(ht30), num_jets_pt30);
  c1->Print(plottitle);


}
