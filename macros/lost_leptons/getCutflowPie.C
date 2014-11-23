#include <iostream>
#include <stdio.h>
#include <vector>
#include <math.h> 
#include "TROOT.h"
#include "TFile.h"
#include "TStyle.h"
#include "TSystem.h"
#include "TChain.h"
#include "TTree.h"
#include "TCut.h"
#include "THStack.h"
#include "TLine.h"
#include "TPie.h"
#include "TPieSlice.h"
#include "TCanvas.h"
#include "macros/include/my_style.hpp"

using namespace std;

double int_lumi(5000.);

void getCutflowPie(TCut cut, string description) {


  TChain* ttbar_ch = new TChain("reduced_tree");
  TChain* qcd_ch = new TChain("reduced_tree");
  TChain* znn_ch = new TChain("reduced_tree");
  TChain* wjets_ch = new TChain("reduced_tree");
  TChain* single_top_ch = new TChain("reduced_tree");
  TChain* other_ch = new TChain("reduced_tree");


  ttbar_ch->Add("reduced_trees/skimmed/TTJets*PU20*v76*.root");
  qcd_ch->Add("reduced_trees/skimmed/QCD_Pt-*v75*.root");
  znn_ch->Add("reduced_trees/skimmed/ZJets*v75*.root");
  wjets_ch->Add("reduced_trees/skimmed/WJets*v75*.root");
  single_top_ch->Add("reduced_trees/skimmed/TTo*v75*.root");
  single_top_ch->Add("reduced_trees/skimmed/TBarTo*v75*.root");
  single_top_ch->Add("reduced_trees/skimmed/*tW*v75*.root");
  other_ch->Add("reduced_trees/skimmed/*HToBB*v75*.root");
  other_ch->Add("reduced_trees/skimmed/TTbarH*v75*.root");
  
  gStyle->SetTextSize(0.05);            // Set global text size
  gStyle->SetTitleSize(0.05);     // Set the 2 axes title size
  gStyle->SetTitleFont(132);     
  gStyle->SetLabelSize(0.05);     // Set the 2 axes label size
  gStyle->SetLabelFont(132);    
  gStyle->SetCanvasDefW(1200);

  TCanvas* can = new TCanvas();


  TH1D *h = new TH1D("h","",2,0,2);
  TCut weight = Form("weightppb*%f",int_lumi);
  float n_ttbar(0.), n_qcd(0.), n_znn(0.), n_wjets(0.), n_single_top(0.), n_other(0.), n_SM(0.);

  
  ttbar_ch->Project("h","1",cut*weight);
  n_ttbar=h->Integral();
  qcd_ch->Project("h","1",cut*weight);
  n_qcd=h->Integral();
  znn_ch->Project("h","1",cut*weight);
  n_znn=h->Integral();
  wjets_ch->Project("h","1",cut*weight);
  n_wjets=h->Integral();
  single_top_ch->Project("h","1",cut*weight);
  n_single_top=h->Integral();
  other_ch->Project("h","1",cut*weight);
  n_other=h->Integral();

  //  n_SM=n_qcd+n_ttbar+n_znn+n_wjets+n_single_top+n_other;


  printf("Cut, ttbar, QCD, znn, w+jets, single top, other, SM Total\n");
  printf("%s, %3.1f, %3.1f, %3.1f, %3.1f, %3.1f, %3.1f, %3.1f\n", description.c_str(), n_ttbar, n_qcd, n_znn, n_wjets, n_single_top, n_other, n_SM);
  //  fprintf(file, "%s, %3.1f, %3.1f, %3.1f, %3.1f, %3.1f, %3.1f, %3.1f\n", description.c_str(), n_qcd, n_ttbar, n_znn, n_wjets, n_single_top, n_other, n_SM);

  vector<double> yields;
  yields.push_back(n_ttbar);
  yields.push_back(n_qcd);
  yields.push_back(n_znn);
  yields.push_back(n_wjets);
  yields.push_back(n_single_top);
  yields.push_back(n_other);
  vector<string> labels;
  labels.push_back("t#bar{t}");
  labels.push_back("QCD");
  labels.push_back("Z#rightarrow#nu#nu");
  labels.push_back("W+jets");
  labels.push_back("single top");
  labels.push_back("other");
  vector<int> colors;
  colors.push_back(1000);
  colors.push_back(1001);
  colors.push_back(1002);
  colors.push_back(1004);
  colors.push_back(1005);
  colors.push_back(1003);

  TPie pie("pie1", "", yields.size(), &yields[0]);
  pie.SetFillColors(&colors[0]);
  // pie.SetLabels(&labels[0]);
  for(unsigned ind(0); ind<yields.size(); ind++){
    pie.GetSlice(ind)->SetTitle(labels[ind].c_str());
    //  pie->GetSlice(ind)->SetTitleColor(colors[ind]);
  }
  pie.SetLabelFormat("%txt: %val (%perc)");
  pie.SetRadius(0.3);
  pie.Draw(""); 
  TString pname = "macros/lost_leptons/plots/BGpiechart_"+description+".pdf";
  can->Print(pname);


  // delete h;

}


  // PrintLine("mht50>600&&num_csvm_jets>=2&&num_jets_pt50>9&&num_reco_veto_muons==0&&num_reco_veto_electrons==0", "Nhan");
  // PrintLine("met>600&&num_csvm_jets30>=2&&min_delta_phi_met_N>4&&num_jets_pt30>9&&num_reco_veto_muons==0&&num_reco_veto_electrons==0", "Jack");


  // PrintLine(lep, "No leptons");
  // PrintLine(lep+"met>400", "met>400");
  // PrintLine(start, "ht>1000");
  // PrintLine(start+"num_jets_pt30>3", ">=4 jets");
  // PrintLine(start+"num_jets_pt30>3&&num_csvm_jets30>0", ">= 1 CSVM");
  // PrintLine(start+"num_jets_pt30>3&&num_csvm_jets30>1", ">= 2 CSVM");
  // PrintLine(start+"num_jets_pt30>3&&num_csvm_jets30>1&&min_delta_phi_met_N>4", "mdpN>4");

  // PrintLine(start+"num_jets_pt30>9&&min_delta_phi_met_N>4", "10 jets");
  // PrintLine(start+"met>600&&num_jets_pt30>9&&min_delta_phi_met_N>4", "10 jets, met>600");
  // PrintLine(start+"num_jets_pt30>9&&num_csvm_jets30>1&&min_delta_phi_met_N>4", "10 jets, 2b");
  // PrintLine(start+"met>600&&num_jets_pt30>9&&num_csvm_jets30>1&&min_delta_phi_met_N>4", "10 jets, 2b, met>600");

  // PrintLine(start+"jet4_pt>200&&min_delta_phi_met_N>4", "4 high-pt jets");
  // PrintLine(start+"met>600&&jet4_pt>200&&min_delta_phi_met_N>4", "4 high-pt jets, met>600");
  // PrintLine(start+"jet4_pt>200&&num_csvm_jets30>1&&min_delta_phi_met_N>4", "4 high-pt jets, 2b");
  // PrintLine(start+"met>600&&jet4_pt>200&&num_csvm_jets30>1&&min_delta_phi_met_N>4", "4 high-pt jets, 2b, met>600");

  // // PrintLine(lep, "Start");
  // // PrintLine(lep+"met>400", "MET > 400 GeV");
  // // PrintLine(lep+"met>400&&ht30>1000", "HT > 1000 GeV");
  // // PrintLine(lep+"met>400&&ht30>1000&&num_jets_pt30>=4", ">= 4 jets");
  // PrintLine(start, "MET > 400 GeV & HT > 1000 GeV & >= 4 jets");
  // // PrintLine(start+"num_reco_veto_muons==0", "Muon veto");
  // // PrintLine(start+"num_reco_veto_muons==0&&num_reco_veto_electrons==0", "Electron veto");
  // PrintLine(start+"min_delta_phi_met_N>4", "mdpN>4");
  // PrintLine(start+"num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_csvl_jets30>1&&min_delta_phi_met_N>4", ">= 2 CSVL");
  // PrintLine(baseline, ">= 6 jets (30 GeV)");
  // PrintLine(baseline+"num_jets_pt50>=6", ">= 6 jets (50 GeV)");
  // PrintLine(baseline+"num_jets_pt50>=8", ">= 8 jets (50 GeV)");
  // PrintLine(baseline+"num_jets_pt50>=8&&met>600", ">= 8 jets (50 GeV) & MET > 600");
  // PrintLine(baseline+"jet3_pt>300", ">= 6 jets (30 GeV) >=3 jets (300 GeV)");
