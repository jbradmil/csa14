#include <iostream>
#include <iomanip> 
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

using namespace std;



float int_lumi(5000.);

void PrintLine(TCut cut, string description, std::vector<TChain*> chains) {

  TH1::SetDefaultSumw2(1);


  // cout << "Cut: " << description << endl;
 
  TH1D *h = new TH1D("h","",2,0,2);
  TCut weight = Form("weightppb*%f",int_lumi);
  double n(0.);
  Double_t err(0.);

  cout << description << "\t";

  for (unsigned int sample(0); sample<chains.size(); sample++) {
    chains[sample]->Project("h","1",cut*weight);
    n=h->IntegralAndError(1,3,err);
    cout << std::setprecision(4) << n << "+/-" << err << "\t";
  }

  cout << endl;


  delete h;

}


void getLostLeptons(float lumi=5000.) {

  int_lumi=lumi;

  TChain* ttbar_25ns_7_0 = new TChain("reduced_tree");
  TChain* t1tttt_1500_100_25ns_7_0 = new TChain("reduced_tree");
  TChain* t1tttt_1200_800_25ns_7_0 = new TChain("reduced_tree");
  TChain* t1bbbb_1500_100_25ns_7_0 = new TChain("reduced_tree");
  TChain* t1bbbb_1000_900_25ns_7_0 = new TChain("reduced_tree");

  TChain* ttbar_50ns_7_0 = new TChain("reduced_tree");
  TChain* t1tttt_1500_100_50ns_7_0 = new TChain("reduced_tree");
  TChain* t1tttt_1200_800_50ns_7_0 = new TChain("reduced_tree");
  TChain* t1bbbb_1500_100_50ns_7_0 = new TChain("reduced_tree");
  TChain* t1bbbb_1000_900_50ns_7_0 = new TChain("reduced_tree");

  TChain* ttbar_25ns_7_2 = new TChain("reduced_tree");
  TChain* t1tttt_1500_100_25ns_7_2 = new TChain("reduced_tree");

  
 
  

  ttbar_25ns_7_0->Add("reduced_trees/skimmed/lost_lepton/TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola_Spring14miniaod-PU20bx25_POSTLS170_V5-v2_MINIAODSIM_UCSB2208_v76_skimmed.root");
  ttbar_50ns_7_0->Add("reduced_trees/skimmed/lost_lepton/TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola_Spring14miniaod-PU_S14_POSTLS170_V6-v1_MINIAODSIM_UCSB2265_v75_skimmed.root");
  ttbar_25ns_7_2->Add("reduced_trees/RelValTTbar_13TeV-MINIAODSIM-PU25ns_PHYS14_25_V1_Phys14-v1_v73/RelValTTbar_13TeV-MINIAODSIM-PU25ns_PHYS14_25_V1_Phys14-v1_v73.root");

  t1tttt_1500_100_25ns_7_0->Add("reduced_trees/SMS-T1tttt_2J_mGl-1500_mLSP-100_Tune4C_13TeV-madgraph-tauola_Spring14miniaod-PU20bx25_POSTLS170_V5-v2_MINIAODSIM_UCSB2268_v76/*.root");
  t1tttt_1200_800_25ns_7_0->Add("reduced_trees/SMS-T1tttt_2J_mGl-1200_mLSP-800_Tune4C_13TeV-madgraph-tauola_Spring14miniaod-PU20bx25_POSTLS170_V5-v2_MINIAODSIM_UCSB2267_v76/*.root");
  t1bbbb_1500_100_25ns_7_0->Add("reduced_trees/SMS-T1bbbb_2J_mGl-1500_mLSP-100_Tune4C_13TeV-madgraph-tauola_Spring14miniaod-PU20bx25_POSTLS170_V5-v3_MINIAODSIM_UCSB2266_v76/*.root");
  t1bbbb_1000_900_25ns_7_0->Add("reduced_trees/SMS-T1bbbb_2J_mGl-1000_mLSP-900_Tune4C_13TeV-madgraph-tauola_Spring14miniaod-PU20bx25_POSTLS170_V5-v2_MINIAODSIM_UCSB2264_v76/*.root");

  t1tttt_1500_100_50ns_7_0->Add("reduced_trees/SMS-T1tttt_2J_mGl-1500_mLSP-100_Tune4C_13TeV-madgraph-tauola_duanders-Spring14dr-PU_S14_POSTLS170_V6AN1-miniAOD706p1-814812ec83fce2f620905d2bb30e9100_USER_UCSB2269_v76/*.root");
  t1tttt_1200_800_50ns_7_0->Add("reduced_trees/SMS-T1tttt_2J_mGl-1200_mLSP-800_Tune4C_13TeV-madgraph-tauola_duanders-Spring14dr-PU_S14_POSTLS170_V6AN1-miniAOD706p1-814812ec83fce2f620905d2bb30e9100_USER_UCSB2270_v76/*.root");
  t1bbbb_1500_100_50ns_7_0->Add("reduced_trees/SMS-T1bbbb_2J_mGl-1500_mLSP-100_Tune4C_13TeV-madgraph-tauola_duanders-Spring14dr-PU_S14_POSTLS170_V6AN1-miniAOD706p1-814812ec83fce2f620905d2bb30e9100_USER_UCSB2272_v76/*.root");
  t1bbbb_1000_900_50ns_7_0->Add("reduced_trees/SMS-T1bbbb_2J_mGl-1000_mLSP-900_Tune4C_13TeV-madgraph-tauola_duanders-Spring14dr-PU_S14_POSTLS170_V6AN1-miniAOD706p1-814812ec83fce2f620905d2bb30e9100_USER_UCSB2271_v76/*.root");

  t1tttt_1500_100_25ns_7_2->Add("reduced_trees/SMS-T1tttt_2J_mGl-1500_mLSP-100_Tune4C_13TeV-madgraph-tauola_Spring14miniaod-PU20bx25_POSTLS170_V5-v1_MINIAODSIM_UCSB2194_v73/*.root");

  std::vector<TChain*> ch_25ns_7_0;
  std::vector<TChain*> ch_50ns_7_0;
  std::vector<TChain*> ch_25ns_7_2;
  ch_25ns_7_0.push_back(ttbar_25ns_7_0);
  ch_25ns_7_0.push_back(t1tttt_1500_100_25ns_7_0);
  ch_25ns_7_0.push_back(t1tttt_1200_800_25ns_7_0);
  ch_25ns_7_0.push_back(t1bbbb_1500_100_25ns_7_0);
  ch_25ns_7_0.push_back(t1bbbb_1000_900_25ns_7_0);

  ch_50ns_7_0.push_back(ttbar_50ns_7_0);
  ch_50ns_7_0.push_back(t1tttt_1500_100_50ns_7_0);
  ch_50ns_7_0.push_back(t1tttt_1200_800_50ns_7_0);
  ch_50ns_7_0.push_back(t1bbbb_1500_100_50ns_7_0);
  ch_50ns_7_0.push_back(t1bbbb_1000_900_50ns_7_0);

  ch_25ns_7_2.push_back(ttbar_25ns_7_2);
  ch_25ns_7_2.push_back(t1tttt_1500_100_25ns_7_2);


  TCut cuts("met>400&&ht30>1000&&num_csvm_jets30>1&&min_delta_phi_met_N>4&&num_jets_pt30>=4");

  printf("Sample\t\tttbar\t\tT1tttt(1500-100)  T1tttt(1200-800)  T1bbbb(1500-100)  T1bbbb(1000-900)\n");



  PrintLine(cuts+"num_reco_veto_muons==0&&num_reco_veto_electrons==0", "0l/25/7_0", ch_25ns_7_0);
  PrintLine(cuts+"num_reco_veto_muons==0&&num_reco_veto_electrons==0", "0l/50/7_0", ch_50ns_7_0);
  PrintLine(cuts+"num_reco_veto_muons==0&&num_reco_veto_electrons==0", "0l/25/7_2", ch_25ns_7_2);

  PrintLine(cuts+"num_reco_veto_muons+num_reco_veto_electrons>=1", "1l/25/7_0", ch_25ns_7_0);
  PrintLine(cuts+"num_reco_veto_muons+num_reco_veto_electrons>=1", "1l/50/7_0", ch_50ns_7_0);
  PrintLine(cuts+"num_reco_veto_muons+num_reco_veto_electrons>=1", "1l/25/7_2", ch_25ns_7_2);

  PrintLine(cuts, "all/25/7_0", ch_25ns_7_0);
  PrintLine(cuts, "all/50/7_0", ch_50ns_7_0);
  PrintLine(cuts, "all/25/7_2", ch_25ns_7_2);


}
