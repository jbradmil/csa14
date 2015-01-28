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

FILE * file;
TChain* ch;

float int_lumi(5000.);

void PrintLine(TCut cut, string description) {

 
  TH1D *h = new TH1D("h","",2,0,2);
  TCut weight = Form("weightppb*%f",int_lumi);
  int n(0);
  float n_weighted(0.);

  n=ch->GetEntries(cut);
  // Fill hist to get each weighted yield
  ch->Project("h","1",cut*weight);
  n_weighted=h->Integral();

  fprintf(file, "%s\t%d\t\t%3.3f\n", description.c_str(), n, n_weighted);
  printf("%s\t%d\t\t%3.3f\n", description.c_str(), n, n_weighted);

  delete h;

}

void getCutflowOneSample(TString sample, TString output="") {

  TH1::SetDefaultSumw2();

  TString outfileName;
  if (output.EqualTo("")) {
    outfileName=sample;
    outfileName.ReplaceAll(".root",".txt");
    outfileName.ReplaceAll("reduced_trees/","");
    file = fopen ("macros/cutflow/output/"+outfileName,"w");
  }
  else file = fopen ("macros/cutflow/output/"+output,"w");

  ch = new TChain("reduced_tree");
  ch->Add(sample);

  TCut ht("ht30>500"), njets("jet4_pt>30"), mht30("mht30>200"), mu_veto("num_reco_veto_muons==0"), el_veto("num_reco_veto_electrons==0"), mdp("min_delta_phi_met_N>4"), isoTk("num_iso_tracks_pt15_mT==0"),  baseline(ht+njets+mht30+mu_veto+el_veto+mdp+isoTk);

  fprintf(file,"Cut\t\t\tUnweighted\tweighted\n");
  PrintLine("", "Preselection");
  PrintLine(njets, "nJets >=4");
  PrintLine(ht+njets, "HT > 500 GeV");
  PrintLine(ht+mht30+njets, "mht30 > 200");
  PrintLine(ht+njets+mht30+mu_veto, "Muon veto");
  PrintLine(ht+njets+mht30+mu_veto+el_veto, "Electron veto");
  PrintLine(ht+njets+mht30+mu_veto+el_veto+mdp, "mdpN > 4");
  PrintLine(ht+njets+mht30+mu_veto+el_veto+mdp+"num_iso_tracks_pt15==0", "Isotk veto");
  PrintLine(baseline, "Isotk veto_mT");
  fprintf(file,"******* b-tag bins *******\n");
  PrintLine(baseline+"num_csvm_jets30==0", "==0 CSVM");
  PrintLine(baseline+"num_csvm_jets30==1", "==1 CSVM");
  PrintLine(baseline+"num_csvm_jets30==2", "==2 CSVM");
  PrintLine(baseline+"num_csvm_jets30>=3", ">=3 CSVM");
  // PrintLine(baseline+"num_csvm_jets30>=2&&num_iso_tracks_pt15_mT==0", "Isotk veto");
  // PrintLine(baseline+"num_csvm_jets30>=2&&num_iso_tracks_pt15_mT==0&&fatpT30_MJ>200", "MJ>200");

  fclose(file);

}
