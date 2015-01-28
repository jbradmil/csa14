#include <iostream>
#include <stdio.h>
#include <vector>
#include <math.h> 
#include "TROOT.h"
#include "TFile.h"
#include "TSystem.h"
#include "TChain.h"
#include "TTree.h"
#include "TCut.h"
#include "THStack.h"
#include "TLine.h"
#include "TFileCollection.h"
#include "TMath.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TLine.h"
#include "RooStats/NumberCountingUtils.h"
#include "macros/include/my_style.hpp"

using namespace std;

TChain* SM_ch;
TChain* t1tttt_1500_100_ch;
TChain* t1tttt_1200_800_ch;
TChain* t1bbbb_1500_100_ch;
TChain* t1bbbb_1000_900_ch;
TChain *t1qqqq_1400_100_ch;
TChain *t1qqqq_1000_800_ch;

TGraph* GetZbiGraph(TCut cut, TChain* sig_ch, double tau) {

  printf("Cuts: %s\n",cut.GetTitle());

  TH1::SetDefaultSumw2(1);

  Double_t x[21], y[21];

  x[0]=0;
  y[0]=0;

  for (int bin(0); bin<20; bin++) {

    x[bin+1]=bin+1;
    TH1D *h = new TH1D("h","",2,0,2);
    TCut weight = Form("weightppb*1000*%d",bin+1);
    double n_SM(0.), n_sig(0.);
    double sigZBi(0.);
    Double_t e_SM(0.);

    // Fill hist to get each weighted yield
    sig_ch->Project("h","1",cut*weight);
    n_sig=h->Integral(1,3);

    SM_ch->Project("h","1",cut*weight);
    n_SM=h->IntegralAndError(1,3,e_SM);

    sigZBi = RooStats::NumberCountingUtils::BinomialWithTauExpZ(n_sig,n_SM, tau);

    y[bin+1]=sigZBi;
    printf("n_sig: %3.1f,  n_SM: %3.1f,  ZBi: %3.1f\n",n_sig, n_SM, sigZBi);

    delete h;

  }

  TGraph* gr = new TGraph(21,x,y);

  return gr;

}


void plotZBi_vs_lumi_R1() {


  SM_ch = new TChain("reduced_tree");
  t1tttt_1500_100_ch = new TChain("reduced_tree");
  t1tttt_1200_800_ch = new TChain("reduced_tree");
  t1bbbb_1500_100_ch = new TChain("reduced_tree");
  t1bbbb_1000_900_ch = new TChain("reduced_tree");
  t1qqqq_1400_100_ch = new TChain("reduced_tree");
  t1qqqq_1000_800_ch = new TChain("reduced_tree");
  


  
  // TFileCollection* ttbar_fc = new TFileCollection("ttbar_fc","","reduced_trees/tfcs/ttbar.txt");
  // TFileCollection* qcd_fc = new TFileCollection("qcd_fc","","reduced_trees/tfcs/qcd.txt");
  // TFileCollection* znn_fc = new TFileCollection("znn_fc","","reduced_trees/tfcs/znn.txt");
  // TFileCollection* wjets_fc = new TFileCollection("wjets_fc","","reduced_trees/tfcs/wjets.txt");
  // TFileCollection* single_top_fc = new TFileCollection("single_top_fc","","reduced_trees/tfcs/single_top.txt");
  // TFileCollection* other_fc = new TFileCollection("other_fc","","reduced_trees/tfcs/other.txt");
  
  // SM_ch->AddFileInfoList((TCollection*)ttbar_fc->GetList());
  // SM_ch->AddFileInfoList((TCollection*)qcd_fc->GetList());
  // SM_ch->AddFileInfoList((TCollection*)znn_fc->GetList());
  // SM_ch->AddFileInfoList((TCollection*)wjets_fc->GetList());
  // SM_ch->AddFileInfoList((TCollection*)single_top_fc->GetList());
  // SM_ch->AddFileInfoList((TCollection*)other_fc->GetList());

  SM_ch->Add("reduced_trees/13TeV/skimmed/TTJets*v77*.root");
  SM_ch->Add("reduced_trees/13TeV/skimmed/TTbar*v77*.root");
  SM_ch->Add("reduced_trees/13TeV/skimmed/TTW*v77*.root");
  SM_ch->Add("reduced_trees/13TeV/skimmed/TTZ*v77*.root");
  SM_ch->Add("reduced_trees/13TeV/skimmed/QCD_HT*v77*.root");
  SM_ch->Add("reduced_trees/13TeV/skimmed/ZJets*v77*.root");
  SM_ch->Add("reduced_trees/13TeV/skimmed/WJets*v77*.root");
  SM_ch->Add("reduced_trees/13TeV/skimmed/TTo*v77*.root");
  SM_ch->Add("reduced_trees/13TeV/skimmed/TBarTo*v77*.root");
  SM_ch->Add("reduced_trees/13TeV/skimmed/*tW*v77*.root");
  SM_ch->Add("reduced_trees/13TeV/skimmed/ZH*v77*.root");
  SM_ch->Add("reduced_trees/13TeV/skimmed/WH*v77*.root");


  t1tttt_1500_100_ch->Add("reduced_trees/13TeV/SMS-T1tttt_2J_mGl-1500_mLSP-100_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2299_v77/*.root");
  t1tttt_1200_800_ch->Add("reduced_trees/13TeV/SMS-T1tttt_2J_mGl-1200_mLSP-800_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2301_v77/*.root");
  t1bbbb_1500_100_ch->Add("reduced_trees/13TeV/SMS-T1bbbb_2J_mGl-1500_mLSP-100_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2300_v77/*.root");
  t1bbbb_1000_900_ch->Add("reduced_trees/13TeV/SMS-T1bbbb_2J_mGl-1000_mLSP-900_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2288_v77/*.root");
  t1qqqq_1400_100_ch->Add("reduced_trees/13TeV/SMS-T1qqqq_2J_mGl-1400_mLSP-100_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2302_v77/*.root");
  t1qqqq_1000_800_ch->Add("reduced_trees/13TeV/SMS-T1qqqq_2J_mGl-1000_mLSP-800_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2303_v77/*.root");
  

  TCut ht("ht30>500"), njets("jet4_pt>30"), mht30("mht30>200"), mu_veto("num_reco_veto_muons==0"), el_veto("num_reco_veto_electrons==0"), mdp("min_delta_phi_met_N>4"), isoTk("num_iso_tracks_pt15_mT==0"),  start(ht+njets+mht30+mu_veto+el_veto+mdp+isoTk);

  // tight (tau=10)
  TGraph* gr_t1tttt_1500_100 = GetZbiGraph(start+"mht30>500&&ht30>1000&&num_jets_pt30>=9&&num_csvm_jets30>=2", t1tttt_1500_100_ch, 5);
  TGraph* gr_t1tttt_1200_800 = GetZbiGraph(start+"mht30>200&&ht30>500&&num_jets_pt30>=9&&num_csvm_jets30>=2", t1tttt_1200_800_ch, 5);
  TGraph* gr_t1bbbb_1500_100 = GetZbiGraph(start+"mht30>500&&ht30>1200&&num_jets_pt30>=4&&num_csvm_jets30>=2", t1bbbb_1500_100_ch, 5);
  TGraph* gr_t1bbbb_1000_900 = GetZbiGraph(start+"mht30>500&&ht30>500&&num_jets_pt30>=4&&num_csvm_jets30>=2", t1bbbb_1000_900_ch, 5);
  TGraph* gr_t1qqqq_1400_100 = GetZbiGraph(start+"mht30>400&&ht30>1400&&num_jets_pt30>=4", t1qqqq_1400_100_ch, 5);
  TGraph* gr_t1qqqq_1000_800 = GetZbiGraph(start+"mht30>300&&ht30>500&&num_jets_pt30>=6", t1qqqq_1000_800_ch, 5);

  gr_t1tttt_1500_100->SetLineColor(kGreen);  
  gr_t1tttt_1200_800->SetLineColor(kGreen);  
  gr_t1bbbb_1500_100->SetLineColor(2);  
  gr_t1bbbb_1000_900->SetLineColor(2);
  gr_t1qqqq_1400_100->SetLineColor(1006);  
  gr_t1qqqq_1000_800->SetLineColor(1006);
  
  // gr_t1tttt_1500_100->SetLineColor(1000);  
  // gr_t1tttt_1200_800->SetLineColor(1001);  
  // gr_t1bbbb_1500_100->SetLineColor(1002);  
  // gr_t1bbbb_1000_900->SetLineColor(1004);
  // gr_t1qqqq_1400_100->SetLineColor(1005);  
  // gr_t1qqqq_1000_800->SetLineColor(1006);

  gr_t1tttt_1500_100->SetLineWidth(3);  
  gr_t1tttt_1200_800->SetLineWidth(3);  
  gr_t1bbbb_1500_100->SetLineWidth(3);  
  gr_t1bbbb_1000_900->SetLineWidth(3); 
  gr_t1qqqq_1400_100->SetLineWidth(3);  
  gr_t1qqqq_1000_800->SetLineWidth(3);

  gr_t1tttt_1200_800->SetLineStyle(7);  
  gr_t1bbbb_1000_900->SetLineStyle(7); 
  gr_t1qqqq_1000_800->SetLineStyle(7); 

  TCanvas * c1 = new TCanvas("c1", "c1", 700, 700);

  gr_t1qqqq_1000_800->Draw("AL");  
  gr_t1tttt_1500_100->Draw("L");  
  gr_t1tttt_1200_800->Draw("L");  
  gr_t1bbbb_1500_100->Draw("L");  
  gr_t1bbbb_1000_900->Draw("L");
  gr_t1qqqq_1400_100->Draw("L");  

  gr_t1qqqq_1000_800->SetMaximum(8.3);
  gr_t1qqqq_1000_800->SetTitle(";#intL dt [fb^{-1}]; Z_{Bi} [#sigma]");
  gr_t1qqqq_1000_800->GetXaxis()->SetTitleFont(132);
  gr_t1qqqq_1000_800->GetXaxis()->SetTitleSize(0.03);
  gr_t1qqqq_1000_800->GetXaxis()->SetTitleOffset(1.3);
  gr_t1qqqq_1000_800->GetYaxis()->SetTitleFont(132);
  c1->Update();

  TLine* sig1 = new TLine(0,1,20,1);
  TLine* sig2 = new TLine(0,2,20,2);
  TLine* sig3 = new TLine(0,3,20,3);
  TLine* sig4 = new TLine(0,4,20,4);
  TLine* sig5 = new TLine(0,5,20,5);
  TLine* sig6 = new TLine(0,6,20,6);
  TLine* sig7 = new TLine(0,7,20,7);
  TLine* sig8 = new TLine(0,8,20,8);
  sig1->SetLineStyle(7);
  sig2->SetLineStyle(7);
  sig3->SetLineStyle(7);
  sig4->SetLineStyle(7);
  sig5->SetLineStyle(7);
  sig6->SetLineStyle(7);
  sig7->SetLineStyle(7);
  sig8->SetLineStyle(7);

  TLine* lumi1 = new TLine(1,0,1,8.1);
  TLine* lumi5 = new TLine(5,0,5,8.1);
  TLine* lumi10 = new TLine(10,0,10,8.1);
  lumi1->SetLineStyle(7);
  lumi5->SetLineStyle(7);
  lumi10->SetLineStyle(7);


  sig1->Draw("same");
  sig2->Draw("same");
  sig3->Draw("same");
  sig4->Draw("same");
  sig5->Draw("same");
  sig6->Draw("same");
  sig7->Draw("same");
  sig8->Draw("same");
  lumi1->Draw("same");
  lumi5->Draw("same");
  lumi10->Draw("same");

  c1->Print("macros/Phys14/plots/ZBi_vs_lumi_126_R0p5.pdf");

}
