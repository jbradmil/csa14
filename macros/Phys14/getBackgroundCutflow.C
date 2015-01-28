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
// #include "RooStats/NumberCountingUtils.h"

using namespace std;

TChain* ttbar_ch;
TChain* qcd_ch;
TChain* znn_ch;
TChain* wjets_ch;
TChain* single_top_ch;
TChain* other_ch;

FILE * file;

float int_lumi(4000.);

void PrintHeader() {

  fprintf(file, "\\begin{table}[h] \n");
  fprintf(file, "\\centering\n");
  fprintf(file, "\\begin{tabular}{lcccccc}\n");
  fprintf(file, "\\hline\n");
  fprintf(file, "\\hline\n");
  fprintf(file, "Cut & $t\\bar{t}$ & QCD  & $Z+$jets  & $W+$jets  & Single top & Other \\\\ \\hline\n");

}

void PrintTrailer(TCut final_cut, string description) {

  TH1D *h = new TH1D("h","",2,0,2);
  TCut weight = Form("weightppb*%f",int_lumi);
  double n_ttbar(0.), n_qcd(0.), n_znn(0.), n_wjets(0.), n_single_top(0.), n_other(0.);
  Double_t e_ttbar(0.), e_qcd(0.), e_znn(0.), e_wjets(0.), e_single_top(0.), e_other(0.);

  // Fill hist to get yield and error
  znn_ch->Project("h","1",final_cut*weight);
  n_znn=h->IntegralAndError(1,3,e_znn);
  wjets_ch->Project("h","1",final_cut*weight);
  n_wjets=h->IntegralAndError(1,3,e_wjets);
  ttbar_ch->Project("h","1",final_cut*weight);
  n_ttbar=h->IntegralAndError(1,3,e_ttbar);
  qcd_ch->Project("h","1",final_cut*weight);
  n_qcd=h->IntegralAndError(1,3,e_qcd);
  single_top_ch->Project("h","1",final_cut*weight);
  n_single_top=h->IntegralAndError(1,3,e_single_top);
  other_ch->Project("h","1",final_cut*weight);
  n_other=h->IntegralAndError(1,3,e_other);

  fprintf(file, "%s & $%3.0f\\pm%3.0f$ & $%3.0f\\pm%3.0f$ & $%3.0f\\pm%3.0f$ & $%3.0f\\pm%3.0f$ & $%3.0f\\pm%3.0f$ & $%3.0f\\pm%3.0f$ \\\\\n", 
	  description.c_str(), n_ttbar,  n_qcd, n_znn,   n_wjets,  n_single_top,   n_other);
  printf("%s & $%3.0f\\pm%3.0f$ & $%3.0f\\pm%3.0f$ & $%3.0f\\pm%3.0f$ & $%3.0f\\pm%3.0f$ & $%3.0f\\pm%3.0f$ & $%3.0f\\pm%3.0f$ \\\\\n", 
	 description.c_str(), n_ttbar,  e_ttbar, n_qcd, e_qcd, n_znn, e_znn,  n_wjets, e_wjets, n_single_top, e_single_top , n_other, e_other); 
  fprintf(file, "\\hline\n");
  fprintf(file, "\\hline\n");
  fprintf(file, "\\end{tabular}\n");
  fprintf(file, "\\end{table}\n");
  
}

void PrintLine(TCut cut, string description) {

  TH1::SetDefaultSumw2(1);


  // cout << "Cut: " << description << endl;
 
  TH1D *h = new TH1D("h","",2,0,2);
  TCut weight = Form("weightppb*%f",int_lumi);
  double n_ttbar(0.), n_qcd(0.), n_znn(0.), n_wjets(0.), n_single_top(0.), n_other(0.);
  Double_t e_ttbar(0.), e_qcd(0.), e_znn(0.), e_wjets(0.), e_single_top(0.), e_other(0.);

  // Fill hist to get each weighted yield
  znn_ch->Project("h","1",cut*weight);
  n_znn=h->IntegralAndError(1,3,e_znn);
  wjets_ch->Project("h","1",cut*weight);
  n_wjets=h->IntegralAndError(1,3,e_wjets);
  ttbar_ch->Project("h","1",cut*weight);
  n_ttbar=h->IntegralAndError(1,3,e_ttbar);
  qcd_ch->Project("h","1",cut*weight);
  n_qcd=h->IntegralAndError(1,3,e_qcd);
  single_top_ch->Project("h","1",cut*weight);
  n_single_top=h->IntegralAndError(1,3,e_single_top);
  other_ch->Project("h","1",cut*weight);
  n_other=h->IntegralAndError(1,3,e_other);

  fprintf(file, "%s & %3.0f & %3.0f & %3.0f & %3.0f & %3.0f & %3.0f \\\\\n", 
	  description.c_str(), n_ttbar,  n_qcd, n_znn,   n_wjets,  n_single_top,   n_other);
  printf("%s & %3.0f & %3.0f & %3.0f & %3.0f & %3.0f & %3.0f \\\\\n", 
	 description.c_str(), n_ttbar,  n_qcd, n_znn,   n_wjets,  n_single_top,   n_other); 


  delete h;

}


void getBackgroundCutflow(TString title, float lumi=4000.) {

  int_lumi=lumi;

  ttbar_ch = new TChain("reduced_tree");
  qcd_ch = new TChain("reduced_tree");
  znn_ch = new TChain("reduced_tree");
  wjets_ch = new TChain("reduced_tree");
  single_top_ch = new TChain("reduced_tree");
  other_ch = new TChain("reduced_tree");

  ttbar_ch->Add("reduced_trees/13TeV/TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1_MINIAODSIM_UCSB2290_v77/*");
  ttbar_ch->Add("reduced_trees/13TeV/TTbarH_M-125_13TeV_amcatnlo-pythia8-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v2_MINIAODSIM_UCSB2349_v77/*");
  ttbar_ch->Add("reduced_trees/13TeV/TTWJets_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1_MINIAODSIM_UCSB2347_v77/*");
  ttbar_ch->Add("reduced_trees/13TeV/TTZJets_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1_MINIAODSIM_UCSB2348_v77/*");
  
  qcd_ch->Add("reduced_trees/13TeV/QCD_HT_250To500_13TeV-madgraph_Phys14DR-PU20bx25_PHYS14_25_V1_ext1-v2_MINIAODSIM_UCSB2309_v77/*");
  qcd_ch->Add("reduced_trees/13TeV/QCD_HT_250To500_13TeV-madgraph_Phys14DR-PU20bx25_PHYS14_25_V1-v1_MINIAODSIM_UCSB2308_v77/*");
  qcd_ch->Add("reduced_trees/13TeV/QCD_HT-500To1000_13TeV-madgraph_Phys14DR-PU20bx25_PHYS14_25_V1_ext1-v1_MINIAODSIM_UCSB2294_v77/*");
  qcd_ch->Add("reduced_trees/13TeV/QCD_HT-500To1000_13TeV-madgraph_Phys14DR-PU20bx25_PHYS14_25_V1-v1_MINIAODSIM_UCSB2293_v77/*");
  qcd_ch->Add("reduced_trees/13TeV/QCD_HT_1000ToInf_13TeV-madgraph_Phys14DR-PU20bx25_PHYS14_25_V1-v1_MINIAODSIM_UCSB2296_v77/*");
  qcd_ch->Add("reduced_trees/13TeV/QCD_HT_1000ToInf_13TeV-madgraph_Phys14DR-PU20bx25_PHYS14_25_V1_ext1-v1_MINIAODSIM_UCSB2297_v77/*");

  znn_ch->Add("reduced_trees/13TeV/ZJetsToNuNu_HT-100to200_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1_MINIAODSIM_UCSB2332_v77/*");
  znn_ch->Add("reduced_trees/13TeV/ZJetsToNuNu_HT-200to400_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1_MINIAODSIM_UCSB2333_v77/*");
  znn_ch->Add("reduced_trees/13TeV/ZJetsToNuNu_HT-400to600_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v2_MINIAODSIM_UCSB2344_v77/*");
  znn_ch->Add("reduced_trees/13TeV/ZJetsToNuNu_HT-600toInf_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1_MINIAODSIM_UCSB2346_v77/*");

  wjets_ch->Add("reduced_trees/13TeV/WJetsToLNu_HT-100to200_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1_MINIAODSIM_UCSB2298_v77/*");
  wjets_ch->Add("reduced_trees/13TeV/WJetsToLNu_HT-200to400_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1_MINIAODSIM_UCSB2304_v77/*");
  wjets_ch->Add("reduced_trees/13TeV/WJetsToLNu_HT-400to600_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1_MINIAODSIM_UCSB2305_v77/*");
  wjets_ch->Add("reduced_trees/13TeV/WJetsToLNu_HT-600toInf_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1_MINIAODSIM_UCSB2306_v77/*");

  single_top_ch->Add("reduced_trees/13TeV/TToLeptons_s-channel-CSA14_Tune4C_13TeV-aMCatNLO-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1_MINIAODSIM_UCSB2340_v77/*");
  single_top_ch->Add("reduced_trees/13TeV/TToLeptons_t-channel-CSA14_Tune4C_13TeV-aMCatNLO-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1_MINIAODSIM_UCSB2341_v77/*");
  single_top_ch->Add("reduced_trees/13TeV/T_tW-channel-DR_Tune4C_13TeV-CSA14-powheg-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1_MINIAODSIM_UCSB2338_v77/*");
  single_top_ch->Add("reduced_trees/13TeV/TBarToLeptons_s-channel-CSA14_Tune4C_13TeV-aMCatNLO-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1_MINIAODSIM_UCSB2343_v77/*");
  single_top_ch->Add("reduced_trees/13TeV/TBarToLeptons_t-channel_Tune4C_CSA14_13TeV-aMCatNLO-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1_MINIAODSIM_UCSB2342_v77/*");
  single_top_ch->Add("reduced_trees/13TeV/Tbar_tW-channel-DR_Tune4C_13TeV-CSA14-powheg-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1_MINIAODSIM_UCSB2339_v77/*");
    
  other_ch->Add("reduced_trees/13TeV/WH_HToBB_WToLNu_M-125_13TeV_powheg-herwigpp_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2350_v77/*");
  other_ch->Add("reduced_trees/13TeV/ZH_HToBB_ZToNuNu_M-125_13TeV_powheg-herwigpp_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2351_v77/*");

  // TFileCollection* ttbar_fc = new TFileCollection("ttbar_fc","","reduced_trees/tfcs/13TeV/Phys14/ttbar.txt");
  // TFileCollection* qcd_fc = new TFileCollection("qcd_fc","","reduced_trees/tfcs/13TeV/Phys14/qcd.txt");
  // TFileCollection* znn_fc = new TFileCollection("znn_fc","","reduced_trees/tfcs/13TeV/Phys14/znn.txt");
  // TFileCollection* wjets_fc = new TFileCollection("wjets_fc","","reduced_trees/tfcs/13TeV/Phys14/wjets.txt");
  // TFileCollection* single_top_fc = new TFileCollection("single_top_fc","","reduced_trees/tfcs/13TeV/Phys14/single_top.txt");
  // TFileCollection* other_fc = new TFileCollection("other_fc","","reduced_trees/tfcs/13TeV/Phys14/other.txt");
  
  // ttbar_ch->AddFileInfoList((TCollection*)ttbar_fc->GetList());
  // qcd_ch->AddFileInfoList((TCollection*)qcd_fc->GetList());
  // znn_ch->AddFileInfoList((TCollection*)znn_fc->GetList());
  // wjets_ch->AddFileInfoList((TCollection*)wjets_fc->GetList());
  // single_top_ch->AddFileInfoList((TCollection*)single_top_fc->GetList());
  // other_ch->AddFileInfoList((TCollection*)other_fc->GetList());

  file = fopen ("macros/Phys14/output/"+title+".tex", "w");
  PrintHeader();

  TCut ht("ht30>500"), njets("jet4_pt>30"), mht30("mht30>200"), mu_veto("num_reco_veto_muons==0"), el_veto("num_reco_veto_electrons==0"), mdp("min_delta_phi_met_N>4"), isoTk("num_iso_tracks_pt15_mT==0"),  baseline(ht+njets+mht30+mu_veto+el_veto+mdp+isoTk);

  PrintLine("", "Start");
  PrintLine(njets, "$\\njets >=4$");
  PrintLine(ht+njets, "$\\HT > 500$ GeV");
  PrintLine(ht+mht30+njets, "$\\MHT > 200$ GeV");
  PrintLine(ht+njets+mht30+mu_veto, "Muon veto");
  PrintLine(ht+njets+mht30+mu_veto+el_veto, "Electron veto");
  PrintLine(ht+njets+mht30+mu_veto+el_veto+mdp, "$\\dphin>4$");
  PrintTrailer(baseline, "Iso. track veto");


  fclose(file);

}
