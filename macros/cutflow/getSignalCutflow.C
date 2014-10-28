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

using namespace std;

TChain* SM_ch;
TChain* ttbar_ch;
TChain* t1tttt_1500_100_ch;
TChain* t1tttt_1200_800_ch;
TChain* t1bbbb_1500_100_ch;
TChain* t1bbbb_1000_900_ch;

FILE * file;

float int_lumi(5000.);

void PrintLine(TCut cut, string description) {

  TH1::SetDefaultSumw2(1);


  // cout << "Cut: " << description << endl;
 
  TH1F *h = new TH1F("h","",2,0,2);
  TCut weight = Form("weightppb*%f",int_lumi);
  double n_SM(0.), n_t1tttt_1500_100(0.), n_t1tttt_1200_800(0.), n_t1bbbb_1500_100(0.), n_t1bbbb_1000_900(0.);
  double sig_SM(0.), sig_t1tttt_1500_100(0.), sig_t1tttt_1200_800(0.), sig_t1bbbb_1500_100(0.), sig_t1bbbb_1000_900(0.);
  Double_t e_SM(0.), e_t1tttt_1500_100(0.), e_t1tttt_1200_800(0.), e_t1bbbb_1500_100(0.), e_t1bbbb_1000_900(0.);

  // Fill hist to get each weighted yield
  t1bbbb_1500_100_ch->Project("h","1",cut*weight);
  n_t1bbbb_1500_100=h->IntegralAndError(1,3,e_t1bbbb_1500_100);
  t1bbbb_1000_900_ch->Project("h","1",cut*weight);
  n_t1bbbb_1000_900=h->IntegralAndError(1,3,e_t1bbbb_1000_900);
  t1tttt_1500_100_ch->Project("h","1",cut*weight);
  n_t1tttt_1500_100=h->IntegralAndError(1,3,e_t1tttt_1500_100);
  t1tttt_1200_800_ch->Project("h","1",cut*weight);
  n_t1tttt_1200_800=h->IntegralAndError(1,3,e_t1tttt_1200_800);

  SM_ch->Project("h","1",cut*weight);
  TCut ttbar_weight("(weightppb*5000)/top_pt_weight_official");
  ttbar_ch->Project("+h","1",cut*ttbar_weight);
  n_SM=h->IntegralAndError(1,3,e_SM);

  sig_t1tttt_1500_100 = 2*(TMath::Sqrt(n_t1tttt_1500_100+n_SM)-TMath::Sqrt(n_SM)); 
  sig_t1tttt_1200_800 = 2*(TMath::Sqrt(n_t1tttt_1200_800+n_SM)-TMath::Sqrt(n_SM)); 
  sig_t1bbbb_1500_100 = 2*(TMath::Sqrt(n_t1bbbb_1500_100+n_SM)-TMath::Sqrt(n_SM)); 
  sig_t1bbbb_1000_900 = 2*(TMath::Sqrt(n_t1bbbb_1000_900+n_SM)-TMath::Sqrt(n_SM)); 

  // sig_t1tttt_1500_100 = n_t1tttt_1500_100/TMath::Sqrt(n_SM); 
  // sig_t1tttt_1200_800 = n_t1tttt_1200_800/TMath::Sqrt(n_SM); 
  // sig_t1bbbb_1500_100 = n_t1bbbb_1500_100/TMath::Sqrt(n_SM); 
  // sig_t1bbbb_1000_900 = n_t1bbbb_1000_900/TMath::Sqrt(n_SM); 

  // printf("%s, %3.1f+-%3.1f, %3.1f+-%3.1f, %3.1f+-%3.1f, %3.1f+-%3.1f , %3.1f+-%3.1f\n",
  // 	  description.c_str(), n_t1bbbb_1500_100, e_t1bbbb_1500_100, n_t1bbbb_1000_900, e_t1bbbb_1000_900,
  // 	  n_t1tttt_1500_100, e_t1tttt_1500_100, n_t1tttt_1200_800, e_t1tttt_1200_800, n_SM, e_SM);  
  // if (errors) fprintf(file, "%s, %3.1f+-%3.1f, %3.1f+-%3.1f, %3.1f+-%3.1f, %3.1f+-%3.1f , %3.1f+-%3.1f\n",
  // 		      description.c_str(), n_t1bbbb_1500_100, e_t1bbbb_1500_100, n_t1bbbb_1000_900, e_t1bbbb_1000_900,
  // 		      n_t1tttt_1500_100, e_t1tttt_1500_100, n_t1tttt_1200_800, e_t1tttt_1200_800, n_SM, e_SM);
  // else fprintf(file, "%s, %3.1f, %3.1f, %3.1f, %3.1f , %3.1f\n",
  // 	       description.c_str(), n_t1bbbb_1500_100, n_t1bbbb_1000_900,
  // 		      n_t1tttt_1500_100, n_t1tttt_1200_800, n_SM);

  printf("%s, %3.1f, %3.1f, %3.1f, %3.1f, %3.1f, %3.1f, %3.1f, %3.1f , %3.1f\n",
       description.c_str(), n_t1bbbb_1500_100, sig_t1bbbb_1500_100, n_t1bbbb_1000_900, sig_t1bbbb_1000_900,
	 n_t1tttt_1500_100, sig_t1tttt_1500_100, n_t1tttt_1200_800, sig_t1tttt_1200_800, n_SM); 
  fprintf(file, "%s, %3.1f, %3.1f, %3.1f, %3.1f, %3.1f, %3.1f, %3.1f, %3.1f , %3.1f\n",
       description.c_str(), n_t1bbbb_1500_100, sig_t1bbbb_1500_100, n_t1bbbb_1000_900, sig_t1bbbb_1000_900,
	 n_t1tttt_1500_100, sig_t1tttt_1500_100, n_t1tttt_1200_800, sig_t1tttt_1200_800, n_SM); 


  delete h;

}


void getSignalCutflow(TString title, float lumi=5000.) {

  int_lumi=lumi;

  SM_ch = new TChain("reduced_tree");
  ttbar_ch = new TChain("reduced_tree");
  t1tttt_1500_100_ch = new TChain("reduced_tree");
  t1tttt_1200_800_ch = new TChain("reduced_tree");
  t1bbbb_1500_100_ch = new TChain("reduced_tree");
  t1bbbb_1000_900_ch = new TChain("reduced_tree");


  ttbar_ch->Add("reduced_trees/skimmed/TTJets*PU20*v76*.root");
  SM_ch->Add("reduced_trees/skimmed/QCD_Pt-*v75*.root");
  //  qcd_ch->Add("reduced_trees/skimmed/low-pt-qcd/QCD_Pt-*.root");
  SM_ch->Add("reduced_trees/skimmed/ZJets*v75*.root");
  SM_ch->Add("reduced_trees/skimmed/WJets*v75*.root");
  SM_ch->Add("reduced_trees/skimmed/TTo*v75*.root");
  SM_ch->Add("reduced_trees/skimmed/TBarTo*v75*.root");
  SM_ch->Add("reduced_trees/skimmed/*tW*v75*.root");
  SM_ch->Add("reduced_trees/skimmed/*HToBB*v75*.root");
  SM_ch->Add("reduced_trees/skimmed/TTbarH*v75*.root");

  t1tttt_1500_100_ch->Add("reduced_trees/skimmed/SMS-T1tttt_2J_mGl-1500*v75*.root");
  t1tttt_1200_800_ch->Add("reduced_trees/skimmed/SMS-T1tttt_2J_mGl-1200*v75*.root");
  t1bbbb_1500_100_ch->Add("reduced_trees/skimmed/SMS-T1bbbb_2J_mGl-1500*v75*.root");
  t1bbbb_1000_900_ch->Add("reduced_trees/skimmed/SMS-T1bbbb_2J_mGl-1000*v75*.root");

  TCut lep("num_reco_veto_muons==0&&num_reco_veto_electrons==0");
  TCut start(lep+"met>400&&ht30>1000");
  TCut baseline(start+"num_csvm_jets30>1&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>=6");

  file = fopen ("macros/cutflow/output/"+title+".csv", "w");
  fprintf(file,"Cut, T1bbbb(1500-100), , T1bbbb(1000-900), , T1tttt(1500-100), , T1tttt(1200-800), , SM\n");

  

  PrintLine(lep, "No leptons");
  PrintLine(lep+"met>400", "met>400");
  PrintLine(start, "ht>1000");
  PrintLine(start+"num_jets_pt30>3", ">=4 jets");
  PrintLine(start+"num_jets_pt30>3&&num_csvm_jets30>1", ">= 2 CSVM");
  PrintLine(start+"num_jets_pt30>3&&num_csvm_jets30>1&&min_delta_phi_met_N>4", "mdpN>4");

  PrintLine(start+"num_jets_pt30>9&&min_delta_phi_met_N>4", "10 jets");
  PrintLine(start+"met>600&&num_jets_pt30>9&&min_delta_phi_met_N>4", "10 jets, met>600");
  PrintLine(start+"num_jets_pt30>9&&num_csvm_jets30>1&&min_delta_phi_met_N>4", "10 jets, 2b");
  PrintLine(start+"met>600&&num_jets_pt30>9&&num_csvm_jets30>1&&min_delta_phi_met_N>4", "10 jets, 2b, met>600");

  PrintLine(start+"jet4_pt>200&&min_delta_phi_met_N>4", "4 high-pt jets");
  PrintLine(start+"met>600&&jet4_pt>200&&min_delta_phi_met_N>4", "4 high-pt jets, met>600");
  PrintLine(start+"jet4_pt>200&&num_csvm_jets30>1&&min_delta_phi_met_N>4", "4 high-pt jets, 2b");
  PrintLine(start+"met>600&&jet4_pt>200&&num_csvm_jets30>1&&min_delta_phi_met_N>4", "4 high-pt jets, 2b, met>600");

  // PrintLine(lep, "Start");
  // PrintLine(lep+"met>400", "MET > 400 GeV");
  // PrintLine(lep+"met>400&&ht30>1000", "HT > 1000 GeV");
  // PrintLine(lep+"met>400&&ht30>1000&&num_jets_pt30>=4", ">= 4 jets");
  PrintLine(start, "MET > 400 GeV & HT > 1000 GeV & >= 4 jets");
  // PrintLine(start+"num_reco_veto_muons==0", "Muon veto");
  // PrintLine(start+"num_reco_veto_muons==0&&num_reco_veto_electrons==0", "Electron veto");
  PrintLine(start+"min_delta_phi_met_N>4", "mdpN>4");
  PrintLine(start+"num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_csvl_jets30>1&&min_delta_phi_met_N>4", ">= 2 CSVL");
  PrintLine(baseline, ">= 6 jets (30 GeV)");
  PrintLine(baseline+"num_jets_pt50>=6", ">= 6 jets (50 GeV)");
  PrintLine(baseline+"num_jets_pt50>=8", ">= 8 jets (50 GeV)");
  PrintLine(baseline+"num_jets_pt50>=8&&met>600", ">= 8 jets (50 GeV) & MET > 600");
  PrintLine(baseline+"jet3_pt>300", ">= 6 jets (30 GeV) >=3 jets (300 GeV)");

  fclose(file);

}
