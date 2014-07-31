#include "TH1.h"
#include "TFile.h"
#include "TChain.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TMath.h"
#include "TLegend.h"
#include "TCut.h"

void ootpu_comparison(TString files, TString comments="") {

  TChain* chain = new TChain("reduced_tree");
  chain->Add(files);

  TH1F* h20to30 = new TH1F("h20to30",";relIso;(a.u.)", 15, 0., 1.);
  TH1F* h30to40 = new TH1F("h30to40",";relIso;(a.u.)", 15, 0., 1.);
  TH1F* h40to50 = new TH1F("h40to50",";relIso;(a.u.)", 15, 0., 1.);
  TH1F* h50to60 = new TH1F("h50to60",";relIso;(a.u.)", 15, 0., 1.);
  h20to30->SetStats(0);
  h20to30->GetYaxis()->SetLabelSize(0.04);
  //h20to30->GetYaxis()->SetTitleOffset(1.3);
  h20to30->GetXaxis()->SetTitleOffset(1.1);
  h20to30->GetXaxis()->SetTitleFont(132);
  h20to30->GetXaxis()->SetTitleSize(0.042);
  h20to30->GetXaxis()->SetLabelSize(0.04);
  h20to30->SetLineWidth(2);

  TCut mu("num_gen_muons==1&&muon_reco_match>=0");
  chain->Project("h20to30", "muon_relIso", mu+"eoot_pu>=5&&eoot_pu<15");
  chain->Project("h30to40", "muon_relIso", mu+"eoot_pu>=15&&eoot_pu<20");
  chain->Project("h40to50", "muon_relIso", mu+"eoot_pu>=20&&eoot_pu<25");
  chain->Project("h50to60", "muon_relIso", mu+"eoot_pu>=25&&eoot_pu<35");

  float avg1(h20to30->GetMean());
  float avg2(h30to40->GetMean());
  float avg3(h40to50->GetMean());
  float avg4(h50to60->GetMean());

  h20to30->Scale(1/h20to30->Integral());
  h30to40->Scale(1/h30to40->Integral());
  h40to50->Scale(1/h40to50->Integral());
  h50to60->Scale(1/h50to60->Integral());

  h20to30->SetLineColor(1);
  h30to40->SetLineColor(2);
  h40to50->SetLineColor(3);
  h50to60->SetLineColor(4);

  h20to30->SetLineWidth(2);
  h30to40->SetLineWidth(2);
  h40to50->SetLineWidth(2);
  h50to60->SetLineWidth(2);

  TCanvas* c1 = new TCanvas();
  float max = TMath::Max(h20to30->GetMaximum(), h30to40->GetMaximum());
  if (h40to50->GetMaximum()>max) max = h40to50->GetMaximum();
  if (h50to60->GetMaximum()>max) max = h50to60->GetMaximum();

  h20to30->SetMaximum(max*1.1);
  h20to30->Draw("hist");
  h30to40->Draw("hist,same");
  h40to50->Draw("hist,same");
  h50to60->Draw("hist,same");

  TLegend* leg = new TLegend(0.5,0.6,0.9,0.9);
  char label[1000];
  sprintf(label,"5#leqEarly OOTPU<15 (#mu=%3.2f)",avg1);
  leg->AddEntry(h20to30,label,"l");
  sprintf(label,"15#leqEarly OOTPU<20 (#mu=%3.2f)",avg2);
  leg->AddEntry(h30to40,label,"l");
  sprintf(label,"20#leqEarly OOTPU<25 (#mu=%3.2f)",avg3);
  leg->AddEntry(h40to50,label,"l");
  sprintf(label,"25#leqEarly OOTPU<35 (#mu=%3.2f)",avg4);
  leg->AddEntry(h50to60,label,"l");
  leg->Draw();

  TString plotTitle ="relIso_vs_OOTPU"+comments+".pdf";
  c1->Print(plotTitle);
}
