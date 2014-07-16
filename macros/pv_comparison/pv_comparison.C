#include "TH1.h"
#include "TFile.h"
#include "TChain.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TMath.h"
#include "TLegend.h"
#include "TCut.h"

void pv_comparison(TString files, TString comments="") {

  TChain* chain = new TChain("reduced_tree");
  chain->Add(files);

  TH1F* h0to10 = new TH1F("h0to10",";relIso;(a.u.)", 30, 0., 1.);
  TH1F* h10to15 = new TH1F("h10to15",";relIso;(a.u.)", 30, 0., 1.);
  TH1F* h15to20 = new TH1F("h15to20",";relIso;(a.u.)", 30, 0., 1.);
  TH1F* h20to25 = new TH1F("h20to25",";relIso;(a.u.)", 30, 0., 1.);
  TH1F* h25to40 = new TH1F("h25to40",";relIso;(a.u.)", 30, 0., 1.);
  h0to10->SetStats(0);
  h0to10->GetYaxis()->SetLabelSize(0.04);
  //h0to10->GetYaxis()->SetTitleOffset(1.3);
  h0to10->GetXaxis()->SetTitleOffset(1.1);
  h0to10->GetXaxis()->SetTitleFont(132);
  h0to10->GetXaxis()->SetTitleSize(0.042);
  h0to10->GetXaxis()->SetLabelSize(0.04);
  h0to10->SetLineWidth(2);

  TCut mu("num_gen_muons==1&&gen_mu1.mus_match>=0");
  chain->Project("h0to10", "gen_mu1.reco_relIso", mu+"num_primary_vertices<=10");
  chain->Project("h10to15", "gen_mu1.reco_relIso", mu+"num_primary_vertices>10&&num_primary_vertices<=15");
  chain->Project("h15to20", "gen_mu1.reco_relIso", mu+"num_primary_vertices>15&&num_primary_vertices<=20");
  chain->Project("h20to25", "gen_mu1.reco_relIso", mu+"num_primary_vertices>20&&num_primary_vertices<=25");
  chain->Project("h25to40", "gen_mu1.reco_relIso", mu+"num_primary_vertices>25");

  h0to10->Scale(1/h0to10->Integral());
  h10to15->Scale(1/h10to15->Integral());
  h15to20->Scale(1/h15to20->Integral());
  h20to25->Scale(1/h20to25->Integral());
  h25to40->Scale(1/h25to40->Integral());

  h0to10->SetLineColor(1);
  h10to15->SetLineColor(2);
  h15to20->SetLineColor(3);
  h20to25->SetLineColor(4);
  h25to40->SetLineColor(6);

  TCanvas* c1 = new TCanvas();
  float max = TMath::Max(h0to10->GetMaximum(), h10to15->GetMaximum());
  if (h15to20->GetMaximum()>max) max = h15to20->GetMaximum();
  if (h20to25->GetMaximum()>max) max = h20to25->GetMaximum();
  if (h25to40->GetMaximum()>max) max = h25to40->GetMaximum();

  h0to10->SetMaximum(max*1.1);
  h0to10->Draw("hist");
  h10to15->Draw("hist,same");
  h15to20->Draw("hist,same");
  h20to25->Draw("hist,same");
  h25to40->Draw("hist,same");

  TLegend* leg = new TLegend(0.75,0.6,0.9,0.9);
  leg->AddEntry(h0to10,"NPV#leq10","l");
  leg->AddEntry(h10to15,"10<NPV#leq15","l");
  leg->AddEntry(h15to20,"15<NPV#leq20","l");
  leg->AddEntry(h20to25,"20<NPV#leq25","l");
  leg->AddEntry(h25to40,"NPV>25","l");
  leg->Draw();

  TString plotTitle ="relIso_vs_NPV"+comments+".pdf";
  c1->Print(plotTitle);
}
