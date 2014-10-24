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

  TCut ht("ht50>500"), njets("jet3_pt>50"), met("met>175"), mu_veto("num_reco_veto_muons==0"), el_veto("num_reco_veto_electrons==0"), mdp("min_delta_phi_met_N>4"), baseline(ht+njets+met+mu_veto+el_veto+mdp);

  fprintf(file,"Cut\t\t\tUnweighted\tweighted\n");
  PrintLine("", "Start");
  PrintLine(ht, "HT > 500 GeV");
  PrintLine(ht+njets, "nJets >=3");
  PrintLine(ht+njets+met, "met > 175");
  PrintLine(ht+njets+met+mu_veto, "Muon veto");
  PrintLine(ht+njets+met+mu_veto+el_veto, "Electron veto");
  PrintLine(baseline, "mdpN > 4");
  fprintf(file,"******* b-tag bins *******\n");
  PrintLine(baseline+"num_csvm_jets==2", "2 CSVM");
  PrintLine(baseline+"num_csvm_jets==3", "3 CSVM");
  PrintLine(baseline+"num_csvm_jets>3",">=4 CSVM");

  fclose(file);

}
