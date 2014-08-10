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

  TH1F* h0to10 = new TH1F("h0to10",";I_{rel}(#mu);(a.u.)", 30, 0., 0.5);
  TH1F* h10to15 = new TH1F("h10to15",";relIso;(a.u.)", 30, 0., 0.5);
  TH1F* h15to20 = new TH1F("h15to20",";relIso;(a.u.)", 30, 0., 0.5);
  TH1F* h20to25 = new TH1F("h20to25",";relIso;(a.u.)", 30, 0., 0.5);
  TH1F* h25to40 = new TH1F("h25to40",";relIso;(a.u.)", 30, 0., 0.5);
  h0to10->SetStats(0);
  h0to10->GetYaxis()->SetLabelSize(0.04);
  //h0to10->GetYaxis()->SetTitleOffset(1.3);
  h0to10->GetXaxis()->SetTitleOffset(1.1);
  h0to10->GetXaxis()->SetTitleFont(132);
  h0to10->GetXaxis()->SetTitleSize(0.042);
  h0to10->GetXaxis()->SetLabelSize(0.04);
  h0to10->SetLineWidth(2);

  TCut mu("num_gen_muons==1&&muon_reco_match>=0");
  /*
  chain->Project("h0to10", "gen_mu1.reco_relIso", mu+"num_primary_vertices<=10");
  chain->Project("h10to15", "gen_mu1.reco_relIso", mu+"num_primary_vertices>10&&num_primary_vertices<=15");
  chain->Project("h15to20", "gen_mu1.reco_relIso", mu+"num_primary_vertices>15&&num_primary_vertices<=20");
  chain->Project("h20to25", "gen_mu1.reco_relIso", mu+"num_primary_vertices>20&&num_primary_vertices<=25");
  chain->Project("h25to40", "muon_relIso", mu+"num_primary_vertices>25");
  */
  
    chain->Project("h0to10", "muon_relIso", mu+"num_primary_vertices<=13");
    chain->Project("h10to15", "muon_relIso", mu+"num_primary_vertices>13&&num_primary_vertices<=17");
    chain->Project("h15to20", "muon_relIso", mu+"num_primary_vertices>17&&num_primary_vertices<=21");
    chain->Project("h25to40", "muon_relIso", mu+"num_primary_vertices>21");
  

  float avg1(h0to10->GetMean());
  float avg2(h10to15->GetMean());
  float avg3(h15to20->GetMean());
  float avg4(h20to25->GetMean());
  float avg5(h25to40->GetMean());

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

  h0to10->SetLineWidth(2);
  h10to15->SetLineWidth(2);
  h15to20->SetLineWidth(2);
  h20to25->SetLineWidth(2);
  h25to40->SetLineWidth(2);

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

  TLegend* leg = new TLegend(0.50,0.6,0.9,0.9);
  char label[1000];
  sprintf(label,"0>NPV#leq13 (#mu=%3.2f)",avg1);
  leg->AddEntry(h0to10,label,"l");
  sprintf(label,"13<NPV#leq17 (#mu=%3.2f)",avg2);
  leg->AddEntry(h10to15,label,"l");
  sprintf(label,"17<NPV#leq21 (#mu=%3.2f)",avg3);
  leg->AddEntry(h15to20,label,"l");
  //sprintf(label,"20<NPV#leq25 (#mu=%3.2f)",avg4);
  //leg->AddEntry(h20to25,label,"l");
  sprintf(label,"NPV>21 (#mu=%3.2f)",avg5);
  leg->AddEntry(h25to40,label,"l");
  leg->Draw();

  TString plotTitle ="relIso_vs_NPV"+comments+".pdf";
  c1->Print(plotTitle);
}
