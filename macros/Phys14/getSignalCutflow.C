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

TChain* SM_ch;
TChain* t1tttt_1500_100_ch;
TChain* t1tttt_1200_800_ch;
TChain* t1bbbb_1500_100_ch;
TChain* t1bbbb_1000_900_ch;
TChain* t1qqqq_1400_100_ch;
TChain* t1qqqq_1000_800_ch;

FILE * file;

float int_lumi(4000.);

void PrintHeader() {

  fprintf(file, "\\begin{table}[h] \n");
  fprintf(file, "\\centering\n");
  fprintf(file, "\\begin{tabular}{lcccccc}\n");
  fprintf(file, "\\hline\n");
  fprintf(file, "\\hline\n");
  fprintf(file, "\\multirow{2}{*}{Cut} & T1tttt & T1tttt  & T1bbbb  & T1bbbb  & T1qqqq & T1qqqq \\\\\n");
  fprintf(file, " & (1500, 100) & (1200, 800) & (1500, 100) & (1000, 900) & (1400, 100) & (1000, 800) \\\\ \\hline\n");

}

void PrintTrailer(TCut final_cut) {

  TH1D *h = new TH1D("h","",2,0,2);
  TCut weight = Form("weightppb*%f",int_lumi);
  double na_t1tttt_1500_100(0.), na_t1tttt_1200_800(0.), na_t1bbbb_1500_100(0.), na_t1bbbb_1000_900(0.), na_t1qqqq_1400_100(0.), na_t1qqqq_1000_800(0.);
  double nb_t1tttt_1500_100(0.), nb_t1tttt_1200_800(0.), nb_t1bbbb_1500_100(0.), nb_t1bbbb_1000_900(0.), nb_t1qqqq_1400_100(0.), nb_t1qqqq_1000_800(0.);

  // Fill hist to get initital yield
  t1bbbb_1500_100_ch->Project("h","1",""*weight);
  na_t1bbbb_1500_100=h->Integral(1,3);
  t1bbbb_1000_900_ch->Project("h","1",""*weight);
  na_t1bbbb_1000_900=h->Integral(1,3);
  t1tttt_1500_100_ch->Project("h","1",""*weight);
  na_t1tttt_1500_100=h->Integral(1,3);
  t1tttt_1200_800_ch->Project("h","1",""*weight);
  na_t1tttt_1200_800=h->Integral(1,3);
  t1qqqq_1400_100_ch->Project("h","1",""*weight);
  na_t1qqqq_1400_100=h->Integral(1,3);
  t1qqqq_1000_800_ch->Project("h","1",""*weight);
  na_t1qqqq_1000_800=h->Integral(1,3);
  // Fill hist to get final yield
  t1bbbb_1500_100_ch->Project("h","1",final_cut*weight);
  nb_t1bbbb_1500_100=h->Integral(1,3);
  t1bbbb_1000_900_ch->Project("h","1",final_cut*weight);
  nb_t1bbbb_1000_900=h->Integral(1,3);
  t1tttt_1500_100_ch->Project("h","1",final_cut*weight);
  nb_t1tttt_1500_100=h->Integral(1,3);
  t1tttt_1200_800_ch->Project("h","1",final_cut*weight);
  nb_t1tttt_1200_800=h->Integral(1,3);
  t1qqqq_1400_100_ch->Project("h","1",final_cut*weight);
  nb_t1qqqq_1400_100=h->Integral(1,3);
  t1qqqq_1000_800_ch->Project("h","1",final_cut*weight);
  nb_t1qqqq_1000_800=h->Integral(1,3);

  fprintf(file, "\\hline\n");
  fprintf(file, "Baseline efficiency & %3.2f & %3.2f & %3.2f & %3.2f & %3.2f & %3.2f \\\\\n", 
	  nb_t1tttt_1500_100/na_t1tttt_1500_100,  nb_t1tttt_1200_800/na_t1tttt_1200_800,
	  nb_t1bbbb_1500_100/na_t1bbbb_1500_100,   nb_t1bbbb_1000_900/na_t1bbbb_1000_900,
	  nb_t1qqqq_1400_100/na_t1qqqq_1400_100,   nb_t1qqqq_1000_800/na_t1qqqq_1000_800);
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
  double n_t1tttt_1500_100(0.), n_t1tttt_1200_800(0.), n_t1bbbb_1500_100(0.), n_t1bbbb_1000_900(0.), n_t1qqqq_1400_100(0.), n_t1qqqq_1000_800(0.);
  Double_t e_t1tttt_1500_100(0.), e_t1tttt_1200_800(0.), e_t1bbbb_1500_100(0.), e_t1bbbb_1000_900(0.), e_t1qqqq_1400_100(0.), e_t1qqqq_1000_800(0.);

  // Fill hist to get each weighted yield
  t1bbbb_1500_100_ch->Project("h","1",cut*weight);
  n_t1bbbb_1500_100=h->IntegralAndError(1,3,e_t1bbbb_1500_100);
  t1bbbb_1000_900_ch->Project("h","1",cut*weight);
  n_t1bbbb_1000_900=h->IntegralAndError(1,3,e_t1bbbb_1000_900);
  t1tttt_1500_100_ch->Project("h","1",cut*weight);
  n_t1tttt_1500_100=h->IntegralAndError(1,3,e_t1tttt_1500_100);
  t1tttt_1200_800_ch->Project("h","1",cut*weight);
  n_t1tttt_1200_800=h->IntegralAndError(1,3,e_t1tttt_1200_800);
  t1qqqq_1400_100_ch->Project("h","1",cut*weight);
  n_t1qqqq_1400_100=h->IntegralAndError(1,3,e_t1qqqq_1400_100);
  t1qqqq_1000_800_ch->Project("h","1",cut*weight);
  n_t1qqqq_1000_800=h->IntegralAndError(1,3,e_t1qqqq_1000_800);

  fprintf(file, "%s & %3.1f & %3.1f & %3.1f & %3.1f & %3.1f & %3.1f \\\\\n", 
	  description.c_str(), n_t1tttt_1500_100,  n_t1tttt_1200_800, n_t1bbbb_1500_100,   n_t1bbbb_1000_900,  n_t1qqqq_1400_100,   n_t1qqqq_1000_800); 


  delete h;

}


void getSignalCutflow(TString title, float lumi=4000.) {

  int_lumi=lumi;

  SM_ch = new TChain("reduced_tree");

  t1tttt_1500_100_ch = new TChain("reduced_tree");
  t1tttt_1200_800_ch = new TChain("reduced_tree");
  t1bbbb_1500_100_ch = new TChain("reduced_tree");
  t1bbbb_1000_900_ch = new TChain("reduced_tree");
  t1qqqq_1400_100_ch = new TChain("reduced_tree");
  t1qqqq_1000_800_ch = new TChain("reduced_tree");

  SM_ch->Add("reduced_trees/13TeV/skimmed/*root");
  t1tttt_1500_100_ch->Add("reduced_trees/13TeV/SMS-T1tttt_2J_mGl-1500_mLSP-100_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2299_v77/*.root");
  t1tttt_1200_800_ch->Add("reduced_trees/13TeV/SMS-T1tttt_2J_mGl-1200_mLSP-800_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2301_v77/*.root");
  t1bbbb_1500_100_ch->Add("reduced_trees/13TeV/SMS-T1bbbb_2J_mGl-1500_mLSP-100_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2300_v77/*.root");
  t1bbbb_1000_900_ch->Add("reduced_trees/13TeV/SMS-T1bbbb_2J_mGl-1000_mLSP-900_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2288_v77/*.root");
  t1qqqq_1400_100_ch->Add("reduced_trees/13TeV/SMS-T1qqqq_2J_mGl-1400_mLSP-100_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2302_v77/*.root");
  t1qqqq_1000_800_ch->Add("reduced_trees/13TeV/SMS-T1qqqq_2J_mGl-1000_mLSP-800_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2303_v77/*.root");


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
  PrintLine(baseline, "Iso. track veto");

  PrintTrailer(baseline);

  fclose(file);

}
