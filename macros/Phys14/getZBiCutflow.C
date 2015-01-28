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
TChain* sig_ch;

FILE * file;

double tau=1.0;

float int_lumi(4000.);

void PrintHeader() {

  fprintf(file, "\\begin{table}[h] \n");
  fprintf(file, "\\centering\n");
  fprintf(file, "\\begin{tabular}{lccc}\n");
  fprintf(file, "\\hline\n");
  fprintf(file, "\\hline\n");
  fprintf(file, "Cut & SIG & SM & $Z_{Bi}$ \\\\ \\hline\n");

}

void PrintTrailer() {

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
  double n_sig(0.),  n_SM(0.);
  Double_t e_sig(0.), e_SM(0.);

  // Fill hist to get each weighted yield
  sig_ch->Project("h","1",cut*weight);
  n_sig=h->IntegralAndError(1,3,e_sig);
  SM_ch->Project("h","1",cut*weight);
  n_SM=h->IntegralAndError(1,3,e_SM);

  double ZBi = RooStats::NumberCountingUtils::BinomialWithTauExpZ(n_sig, n_SM, tau);

  fprintf(file, "%s & $%3.1f \\pm %3.1f$ & $%3.1f \\pm %3.1f$ & $%3.1f$ \\\\\n", 
	  description.c_str(), n_sig,  e_sig, n_SM, e_SM, ZBi); 


  delete h;

}


void getZBiCutflow(TString sig_path, TString title, float lumi=4000.) {

  int_lumi=lumi;

  SM_ch = new TChain("reduced_tree");

  sig_ch = new TChain("reduced_tree");

  SM_ch->Add("reduced_trees/13TeV/skimmed/*root");
  sig_ch->Add(sig_path);

  file = fopen ("macros/Phys14/output/"+title+".tex", "w");
  PrintHeader();

  TCut ht("ht30>500"), njets("jet4_pt>30"), mht30("mht30>200"), mu_veto("num_reco_veto_muons==0"), el_veto("num_reco_veto_electrons==0"), mdp("min_delta_phi_met_N>4"), isoTk("num_iso_tracks_pt15_mT==0"),  baseline(ht+njets+mht30+mu_veto+el_veto+mdp+isoTk);

 // T1tttt (1500, 100)
   // PrintLine(baseline, "Baseline");
   // PrintLine(baseline+"ht30>1000", "$\\HT>1000$");
   // PrintLine(baseline+"ht30>1000&&num_csvm_jets30>=2", "$\\nbjets\\geq2$");
   // PrintLine(baseline+"ht30>1000&&num_csvm_jets30>=2&&num_jets_pt30>=9", "$\\njets\\geq9$");
   // PrintLine(baseline+"ht30>1000&&num_csvm_jets30>=2&&num_jets_pt30>=9&&mht30>500", "$\\MHT>500$");

   // T1tttt (1200, 800)
   // PrintLine(baseline, "Baseline");
   // PrintLine(baseline+"num_csvm_jets30>=2", "$\\nbjets\\geq2$");
   // PrintLine(baseline+"num_csvm_jets30>=2&&num_jets_pt30>=9", "$\\njets\\geq9$");
   
  // T1bbbb (1500, 100)
   PrintLine(baseline, "Baseline");
   PrintLine(baseline+"num_csvm_jets30>=2", "$\\nbjets\\geq2$");
   PrintLine(baseline+"num_csvm_jets30>=2&&ht30>1200", "$\\HT>1200$");
   PrintLine(baseline+"num_csvm_jets30>=2&&ht30>1200&&mht30>500", "$\\MHT>500");
  
  // T1bbbb (1000, 900)
  // PrintLine(baseline,"Baseline");
  // PrintLine(baseline+"num_csvm_jets30>=2", "$\\nbjets\\geq2$");
  // PrintLine(baseline+"num_csvm_jets30>=2&&mht30>500", "$\\MHT>500$");

   // T1qqqq (1400, 100)
   // PrintLine(baseline, "Baseline");
   // PrintLine(baseline+"ht30>1400", "$\\HT>1400$");
   // PrintLine(baseline+"ht30>1400&&mht30>400", "$\\MHT>400$");

   // T1qqqq (1000, 800)
   // PrintLine(baseline, "Baseline");
   // PrintLine(baseline+"mht30>300", "$\\MHT>300$");
   // PrintLine(baseline+"mht30>300&&num_jets_pt30>=6", "$\\njets\\geq6$");
   
   PrintTrailer();

  fclose(file);

}
