#include "TH1.h"
#include "TH2.h"
#include "TFile.h"
#include "TChain.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TMath.h"
#include "TLegend.h"
#include "TCut.h"
#include <iostream>
#include <iomanip>

void eff_scat(TString files, int nbins, float low, float high, TString comments="") {

  TChain* chain = new TChain("reduced_tree");
  chain->Add(files);

  TH1::SetDefaultSumw2();

  TH2F* hA = new TH2F("hA",";Early out-of-time interactions;Late out-of-time interactions;Muon rejection rate (I_{rel}<0.2)", nbins, low, high, nbins, low, high);

  hA->SetStats(0);
  hA->GetYaxis()->SetLabelSize(0.04);
  //hA->GetYaxis()->SetTitleOffset(1.3);
  hA->GetXaxis()->SetTitleOffset(1.1);
  hA->GetXaxis()->SetTitleFont(132);
  hA->GetXaxis()->SetTitleSize(0.042);
  hA->GetXaxis()->SetLabelSize(0.04);


  TString mu("num_gen_muons==1&&muon_reco_match>=0");
  TString iso("muon_relIso<0.2");
  for (int binx(0); binx<hA->GetNbinsX(); binx++) {
    for (int biny(0); biny<hA->GetNbinsY(); biny++) {
      float lowx(hA->GetXaxis()->GetBinLowEdge(binx+1)), highx(hA->GetXaxis()->GetBinLowEdge(binx+2));
      float lowy(hA->GetYaxis()->GetBinLowEdge(biny+1)), highy(hA->GetYaxis()->GetBinLowEdge(biny+2));
      TString binning = Form("eoot_pu>%3.2f&&eoot_pu<%3.2f&&loot_pu>%3.2f&&loot_pu<%3.2f",lowx,highx,lowy,highy);
      cout << chain->GetEntries(binning) << " events in this bin." << endl;
      TString cut_denom = Form("(%s)&&(%s)",mu.Data(),binning.Data());
      cout << chain->GetEntries(cut_denom) << " events with a muon." << endl;
      // TString cut_nom = Form("%s&&(%s)",cut_denom.Data(),iso.Data());
      TH1F* iso_1D = new TH1F("iso_1D","",30,0,0.5);
      chain->Project("iso_1D","muon_relIso",cut_denom);
      //  cout << "hist has " << iso_1D->GetEntries() << " entries." << endl;
      float eff(0.);
      if (iso_1D->Integral(1,31)>0) eff = iso_1D->Integral(1,12)/(float)iso_1D->Integral(1,31);
      printf("Bin(%d,%d): %3.2f E to %3.2f E, %3.2f L to %3.2f L: eff = %3.2f/%3.2f = %3.2f\n",binx+1,biny+1, lowx, highx, lowy, highy, iso_1D->Integral(1,12), iso_1D->Integral(1,31), eff);
      hA->SetBinContent(binx+1,biny+1,eff);
      delete iso_1D;
    }
  }

  TCanvas* c1 = new TCanvas();
  hA->Draw("colz");

  TString plotTitle ="effScat_OOTPU_"+comments+".pdf";
  c1->Print(plotTitle);

}
