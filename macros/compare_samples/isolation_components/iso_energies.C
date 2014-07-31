#include "TH1.h"
#include "TFile.h"
#include "TChain.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TMath.h"
#include "TLegend.h"
#include "TCut.h"

void iso_energies(TString files, TString comments="") {

  TChain* chain = new TChain("reduced_tree");
  chain->Add(files);

  TH1F* hNH = new TH1F("hNH",";Isolation Component p_{T} [GeV]", 100, 0., 2.);
  TH1F* hCH = new TH1F("hCH",";Isolation Component p_{T} [GeV]", 100, 0., 2.);
  TH1F* hph = new TH1F("hph",";Isolation Component p_{T} [GeV]", 100, 0., 2.);
  TH1F* hPU = new TH1F("hPU",";Isolation Component p_{T} [GeV]", 100, 0., 2.);
  hNH->SetStats(0);
  hNH->GetYaxis()->SetLabelSize(0.04);
  //hNH->GetYaxis()->SetTitleOffset(1.3);
  hNH->GetXaxis()->SetTitleOffset(1.1);
  hNH->GetXaxis()->SetTitleFont(132);
  hNH->GetXaxis()->SetTitleSize(0.042);
  hNH->GetXaxis()->SetLabelSize(0.04);
  hNH->SetLineWidth(2);

  TCut mu("num_gen_muons==1&&muon_reco_match>=0");
  chain->Project("hNH", "muon_NH_Et", mu);
  chain->Project("hCH", "muon_CH_pt", mu);
  chain->Project("hph", "muon_ph_Et", mu);
  chain->Project("hPU", "muon_PU_pt", mu);

  // float avg1(hNH->GetMean());
  // float avg2(hCH->GetMean());
  // float avg3(hph->GetMean());
  // float avg4(hPU->GetMean());

  hNH->Scale(1/hNH->Integral());
  hCH->Scale(1/hCH->Integral());
  hph->Scale(1/hph->Integral());
  hPU->Scale(1/hPU->Integral());

  hNH->SetLineColor(2);
  hCH->SetLineColor(3);
  hph->SetLineColor(4);
  hPU->SetLineColor(5);

  hNH->SetLineWidth(2);
  hCH->SetLineWidth(2);
  hph->SetLineWidth(2);
  hPU->SetLineWidth(2);

  TCanvas* c1 = new TCanvas();
  c1->SetLogy();
  float max = TMath::Max(hNH->GetMaximum(), hCH->GetMaximum());
  if (hph->GetMaximum()>max) max = hph->GetMaximum();
  if (hPU->GetMaximum()>max) max = hPU->GetMaximum();

  hNH->SetMaximum(max*1.1);
  hNH->Draw("hist");
  hCH->Draw("hist,same");
  hph->Draw("hist,same");
  //  hPU->Draw("hist,same");

  TLegend* leg = new TLegend(0.75,0.7,0.9,0.9);
  leg->SetFillStyle(0);
  //  char label[1000];
  //  sprintf(label,"5#leqLate OOTPU<15 (#mu=%3.2f)",avg1);
  leg->AddEntry(hNH,"NH E_{T}","l");
  //  sprintf(label,"15#leqLate OOTPU<20 (#mu=%3.2f)",avg2);
  leg->AddEntry(hCH,"CH p_{T}","l");
  //  sprintf(label,"20#leqLate OOTPU<25 (#mu=%3.2f)",avg3);
  leg->AddEntry(hph,"#gamma E_{T}","l");
  //  sprintf(label,"25#leqLate OOTPU<35 (#mu=%3.2f)",avg4);
  //  leg->AddEntry(hPU,"PU p_{T}","l");
  leg->Draw();

  TString plotTitle ="iso_comp_energies_"+comments+".pdf";
  c1->Print(plotTitle);
}
