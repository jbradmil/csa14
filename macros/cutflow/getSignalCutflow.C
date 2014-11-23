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
#include "RooStats/NumberCountingUtils.h"

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
 
  TH1D *h = new TH1D("h","",2,0,2);
  TCut weight = Form("weightppb*%f",int_lumi);
  double n_SM(0.), n_t1tttt_1500_100(0.), n_t1tttt_1200_800(0.), n_t1bbbb_1500_100(0.), n_t1bbbb_1000_900(0.);
  double sig_t1tttt_1500_100(0.), sig_t1tttt_1200_800(0.), sig_t1bbbb_1500_100(0.), sig_t1bbbb_1000_900(0.);
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
  n_SM=h->IntegralAndError(1,3,e_SM);

  double fstat_bg = e_SM/n_SM;
  double fsyst_bg=0.2;
  double ferr_bg = TMath::Sqrt(fstat_bg*fstat_bg+fsyst_bg*fsyst_bg);

  sig_t1tttt_1500_100 = RooStats::NumberCountingUtils::BinomialObsZ(n_t1tttt_1500_100+n_SM,n_SM,ferr_bg);
  if (sig_t1tttt_1500_100<0) sig_t1tttt_1500_100=0;
  sig_t1tttt_1200_800 = RooStats::NumberCountingUtils::BinomialObsZ(n_t1tttt_1200_800+n_SM,n_SM,ferr_bg);
  if (sig_t1tttt_1200_800<0) sig_t1tttt_1200_800=0;
  sig_t1bbbb_1500_100 = RooStats::NumberCountingUtils::BinomialObsZ(n_t1bbbb_1500_100+n_SM,n_SM,ferr_bg);
  if (sig_t1bbbb_1500_100<0) sig_t1bbbb_1500_100=0;
  sig_t1bbbb_1000_900 = RooStats::NumberCountingUtils::BinomialObsZ(n_t1bbbb_1000_900+n_SM,n_SM,ferr_bg);
  if (sig_t1bbbb_1000_900<0) sig_t1bbbb_1000_900=0;

  // sig_t1tttt_1500_100 = 2*(TMath::Sqrt(n_t1tttt_1500_100+n_SM)-TMath::Sqrt(n_SM)); 
  // sig_t1tttt_1200_800 = 2*(TMath::Sqrt(n_t1tttt_1200_800+n_SM)-TMath::Sqrt(n_SM)); 
  // sig_t1bbbb_1500_100 = 2*(TMath::Sqrt(n_t1bbbb_1500_100+n_SM)-TMath::Sqrt(n_SM)); 
  // sig_t1bbbb_1000_900 = 2*(TMath::Sqrt(n_t1bbbb_1000_900+n_SM)-TMath::Sqrt(n_SM)); 

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
  t1tttt_1500_100_ch = new TChain("reduced_tree");
  t1tttt_1200_800_ch = new TChain("reduced_tree");
  t1bbbb_1500_100_ch = new TChain("reduced_tree");
  t1bbbb_1000_900_ch = new TChain("reduced_tree");


  
  TFileCollection* ttbar_fc = new TFileCollection("ttbar_fc","","reduced_trees/tfcs/ttbar.txt");
  TFileCollection* qcd_fc = new TFileCollection("qcd_fc","","reduced_trees/tfcs/qcd.txt");
  TFileCollection* znn_fc = new TFileCollection("znn_fc","","reduced_trees/tfcs/znn.txt");
  TFileCollection* wjets_fc = new TFileCollection("wjets_fc","","reduced_trees/tfcs/wjets.txt");
  TFileCollection* single_top_fc = new TFileCollection("single_top_fc","","reduced_trees/tfcs/single_top.txt");
  TFileCollection* other_fc = new TFileCollection("other_fc","","reduced_trees/tfcs/other.txt");
  
  SM_ch->AddFileInfoList((TCollection*)ttbar_fc->GetList());
  SM_ch->AddFileInfoList((TCollection*)qcd_fc->GetList());
  SM_ch->AddFileInfoList((TCollection*)znn_fc->GetList());
  SM_ch->AddFileInfoList((TCollection*)wjets_fc->GetList());
  SM_ch->AddFileInfoList((TCollection*)single_top_fc->GetList());
  SM_ch->AddFileInfoList((TCollection*)other_fc->GetList());

  t1tttt_1500_100_ch->Add("reduced_trees/SMS-T1tttt_2J_mGl-1500_mLSP-100_Tune4C_13TeV-madgraph-tauola_Spring14miniaod-PU20bx25_POSTLS170_V5-v2_MINIAODSIM_UCSB2268_v76/*.root");
  t1tttt_1200_800_ch->Add("reduced_trees/SMS-T1tttt_2J_mGl-1200_mLSP-800_Tune4C_13TeV-madgraph-tauola_Spring14miniaod-PU20bx25_POSTLS170_V5-v2_MINIAODSIM_UCSB2267_v76/*.root");
  t1bbbb_1500_100_ch->Add("reduced_trees/SMS-T1bbbb_2J_mGl-1500_mLSP-100_Tune4C_13TeV-madgraph-tauola_Spring14miniaod-PU20bx25_POSTLS170_V5-v3_MINIAODSIM_UCSB2266_v76/*.root");
  t1bbbb_1000_900_ch->Add("reduced_trees/SMS-T1bbbb_2J_mGl-1000_mLSP-900_Tune4C_13TeV-madgraph-tauola_Spring14miniaod-PU20bx25_POSTLS170_V5-v2_MINIAODSIM_UCSB2264_v76/*.root");


  // if (skimmed) {
  //   ttbar_ch->Add("reduced_trees/skimmed/TTJets*PU20*v76*.root");
  //   SM_ch->Add("reduced_trees/skimmed/QCD_Pt-*v75*.root");
  //   //  qcd_ch->Add("reduced_trees/skimmed/low-pt-qcd/QCD_Pt-*.root");
  //   SM_ch->Add("reduced_trees/skimmed/ZJets*v75*.root");
  //   SM_ch->Add("reduced_trees/skimmed/WJets*v75*.root");
  //   SM_ch->Add("reduced_trees/skimmed/TTo*v75*.root");
  //   SM_ch->Add("reduced_trees/skimmed/TBarTo*v75*.root");
  //   SM_ch->Add("reduced_trees/skimmed/*tW*v75*.root");
  //   SM_ch->Add("reduced_trees/skimmed/*HToBB*v75*.root");
  //   SM_ch->Add("reduced_trees/skimmed/TTbarH*v75*.root");
  // } else{
  //   ttbar_ch->Add("reduced_trees/TTJets*PU20*v76*.root");
  //   SM_ch->Add("reduced_trees/QCD_Pt-*v75*.root");
  //   //  qcd_ch->Add("reduced_trees/low-pt-qcd/QCD_Pt-*.root");
  //   SM_ch->Add("reduced_trees/ZJets*v75*.root");
  //   SM_ch->Add("reduced_trees/WJets*v75*.root");
  //   SM_ch->Add("reduced_trees/TTo*v75*.root");
  //   SM_ch->Add("reduced_trees/TBarTo*v75*.root");
  //   SM_ch->Add("reduced_trees/*tW*v75*.root");
  //   SM_ch->Add("reduced_trees/*HToBB*v75*.root");
  //   SM_ch->Add("reduced_trees/TTbarH*v75*.root");
  // }
  // t1tttt_1500_100_ch->Add("reduced_trees/SMS-T1tttt_2J_mGl-1500*v76*.root");
  // t1tttt_1200_800_ch->Add("reduced_trees/SMS-T1tttt_2J_mGl-1200*v76*.root");
  // t1bbbb_1500_100_ch->Add("reduced_trees/SMS-T1bbbb_2J_mGl-1500*v76*.root");
  // t1bbbb_1000_900_ch->Add("reduced_trees/SMS-T1bbbb_2J_mGl-1000*v76*.root");


  TCut lep("num_reco_veto_muons==0&&num_reco_veto_electrons==0");
  TCut start(lep+"met>400&&ht30>1000");
  TCut baseline(start+"num_csvm_jets30>1&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>=6");

  file = fopen ("macros/cutflow/output/"+title+".csv", "w");
  fprintf(file,"Cut, T1bbbb(1500-100), , T1bbbb(1000-900), , T1tttt(1500-100), , T1tttt(1200-800), , SM\n");

  PrintLine("mht50>600&&num_csvm_jets>=2&&num_jets_pt50>9&&num_reco_veto_muons==0&&num_reco_veto_electrons==0", "Nhan");
  PrintLine("met>600&&ht30>1000&&num_csvm_jets30>=2&&min_delta_phi_met_N>4&&num_jets_pt30>9&&num_reco_veto_muons==0&&num_reco_veto_electrons==0", "Jack");


  PrintLine("met>400&&ht30>1000&&num_csvm_jets>1&&min_delta_phi_met_N>4&&num_jets_pt30>7&&num_csvm_jets30>1&&num_reco_veto_muons==0&&num_reco_veto_electrons==0", "0l");
  PrintLine("met>400&&ht30>1000&&num_csvm_jets>1&&min_delta_phi_met_N>4&&num_jets_pt30>7&&num_csvm_jets30>1&&num_reco_veto_muons+num_reco_veto_electrons>=1", "1l");

  

  PrintLine(lep, "No leptons");
  // lep+="num_iso_tracks==0";
  // PrintLine(lep, "Isotk veto");
  PrintLine(lep+"met>400", "met>400");
  PrintLine(start, "ht>1000");
  PrintLine(start+"num_jets_pt30>3", ">=4 jets");
  PrintLine(start+"num_jets_pt30>3&&num_csvm_jets30>0", ">= 1 CSVM");
  PrintLine(start+"num_jets_pt30>3&&num_csvm_jets30>1", ">= 2 CSVM");
  PrintLine(start+"num_jets_pt30>3&&num_csvm_jets30>1&&min_delta_phi_met_N>4", "mdpN>4");

  PrintLine(start+"num_jets_pt30>9&&met>600&&min_delta_phi_met_N>4", "10 jets, met>600");
  PrintLine(start+"num_jets_pt30>9&&mht>600&&min_delta_phi_met_N>4", "10 jets, mht>600");
  PrintLine(start+"met>600&&num_jets_pt30>9&&num_csvm_jets30>1&&min_delta_phi_met_N>4", "10 jets, 2b, met>600");
  PrintLine(start+"mht>600&&num_jets_pt30>9&&num_csvm_jets30>1&&min_delta_phi_met_N>4", "10 jets, 2b, mht>600");

  PrintLine(start+"num_jets_pt30>9&&met>600&&min_delta_phi_met_N>4", "10 jets");
  PrintLine(start+"met>600&&num_jets_pt30>9&&num_csvm_jets30>1&&min_delta_phi_met_N>4", "10 jets, 2b");

  PrintLine(start+"num_csvm_jets30>1&&jet3_pt>100&&min_delta_phi_met_N>4&&met>700", "2b, 3j100");
  PrintLine(start+"met>700&&jet3_pt>300&&num_csvm_jets30>1&&min_delta_phi_met_N>4", "2b, 3j300");
  PrintLine(start+"num_csvm_jets30>1&&jet3_pt>100&&min_delta_phi_met_N>4&&mht>700", "2b, 3j100, mht");
  PrintLine(start+"mht>700&&jet3_pt>300&&num_csvm_jets30>1&&min_delta_phi_met_N>4", "2b, 3j300, mht");

  PrintLine(start+"num_csvm_jets30>1&&num_csvl_jets30>3&&met>600&&jet3_pt>100&&min_delta_phi_met_N>4&&met>700", "4b, 3j100");
  PrintLine(start+"met>700&&jet3_pt>300&&num_csvm_jets30>1&&num_csvl_jets30>3&&min_delta_phi_met_N>4", "4b, 3j300");

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
