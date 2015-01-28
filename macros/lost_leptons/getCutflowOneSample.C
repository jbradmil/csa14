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

float int_lumi(4000.);

void PrintLine(TCut cut, string description,  bool fix_ttbar=false) {

 
  TH1D *h = new TH1D("h","",2,0,2);
  TCut weight = Form("weightppb*%f",int_lumi);
  // if (fix_ttbar) weight=Form("3.17760399999999981e-05*%f",int_lumi);
  int n(0);
  float n_weighted(0.);

  n=ch->GetEntries(cut);
  // Fill hist to get each weighted yield
  ch->Project("h","1",cut*weight);
  n_weighted=h->Integral();

  printf("%s\t%d\t\t%3.3f\n", description.c_str(), n, n_weighted);
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
    file = fopen ("macros/lost_leptons/output/"+outfileName,"w");
  }
  else file = fopen ("macros/lost_leptons/output/"+output,"w");

  ch = new TChain("reduced_tree");
  ch->Add(sample);

  TCut ht("ht30>500"), njets("jet4_pt>30"), mht30("mht30>200"), mdp("min_delta_phi_met_N>4"), nb("num_csvm_jets30>=2"), presel(ht+njets+mht30+mdp);

  TCut mu_veto("num_reco_veto_muons==0"), el_veto("num_reco_veto_electrons==0");
  TCut isoTk("num_iso_tracks_pt15"), isoTk_mT("num_iso_tracks_pt15_mT==0");
  TCut tau_veto("num_taus_POG_phys14_ID==0"), tau_veto_mT("num_taus_POG_phys14_ID_mT100==0");
  TCut new_tk_veto("num_mu_iso_tracks_mT==0&&num_e_iso_tracks_mT==0&&num_had_iso_tracks_mT==0");

  bool fix_ttbar(sample.Contains("TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1_MINIAODSIM_UCSB2290_v77"));
    //bool fix_ttbar(false);
  fprintf(file,"Cut\t\t\tUnweighted\tweighted\n");
  PrintLine("", "Before cuts", fix_ttbar);
  PrintLine(njets, "nJets >=4", fix_ttbar);
  PrintLine(ht+njets, "HT > 500 GeV", fix_ttbar);
  PrintLine(ht+mht30+njets, "mht30 > 200", fix_ttbar);
  PrintLine(presel, "Preselection", fix_ttbar);
  fprintf(file,"******* Lepton vetoes *******\n");
  //  PrintLine(presel+mu_veto+el_veto+"mht30>400&&ht30>1000", "Sanity", fix_ttbar);
  PrintLine(presel+mu_veto, "Muon veto", fix_ttbar);
  PrintLine(presel+mu_veto+el_veto, "Electron veto", fix_ttbar);
  fprintf(file,"******* Other possibilities *******\n");
  // PrintLine(presel+mu_veto+el_veto+tau_veto, "Tau veto", fix_ttbar);
  // PrintLine(presel+mu_veto+el_veto+tau_veto_mT, "Tau mT100", fix_ttbar);
  // PrintLine(presel+mu_veto+el_veto+"num_iso_tracks_pt10==0", "Isotk10 veto", fix_ttbar);
  // PrintLine(presel+mu_veto+el_veto+"num_iso_tracks_pt10_mT==0", "Isotk10mT100", fix_ttbar);
  // PrintLine(presel+mu_veto+el_veto+"num_iso_tracks_pt15==0", "Isotk15 veto", fix_ttbar);
  PrintLine(presel+mu_veto+el_veto+"num_iso_tracks_pt15_mT==0", "Isotk15mT100", fix_ttbar);
  // PrintLine(presel+mu_veto+el_veto+tau_veto_mT+"num_iso_tracks_pt15_mT==0", "IsotkTaumT100", fix_ttbar);
  PrintLine(presel+mu_veto+el_veto+"num_mu_iso_tracks_mT==0", "MuTkVeto", fix_ttbar);
  PrintLine(presel+mu_veto+el_veto+"num_e_iso_tracks_mT==0", "ElTkVeto", fix_ttbar);
  PrintLine(presel+mu_veto+el_veto+"num_e_iso_tracks_mT==0&&num_mu_iso_tracks_mT==0", "LepTkVeto", fix_ttbar);
  PrintLine(presel+mu_veto+el_veto+"num_had_iso_tracks_mT==0", "HadTkVeto", fix_ttbar);
  PrintLine(presel+mu_veto+el_veto+new_tk_veto, "NewTkVeto", fix_ttbar);
 
  PrintLine(presel+"met>500&&ht30>1000&&num_jets_pt30>=9", "MET>500&HT>1000&9j", fix_ttbar);
  PrintLine(presel+mu_veto+el_veto+"met>500&&ht30>1000&&num_jets_pt30>=9", "st. lepVeto", fix_ttbar);
  PrintLine(presel+mu_veto+el_veto+"num_iso_tracks_pt15_mT==0&&met>500&&ht30>1000&&num_jets_pt30>=9", "+TkVeto", fix_ttbar);
  PrintLine(presel+mu_veto+el_veto+new_tk_veto+"met>500&&ht30>1000&&num_jets_pt30>=9", "+NewTkVeto", fix_ttbar);
  PrintLine(presel+"num_reco_veto_muons_mT100==0&&(num_reco_veto_electrons==0||(num_reco_veto_electrons==1&&electron_mT>100))&&met>500&&ht30>1000&&num_jets_pt30>=9", "lepVeto_mT", fix_ttbar);
  PrintLine(presel+"num_iso_tracks_pt15_mT==0&&met>500&&ht30>1000&&num_jets_pt30>=9", "OldTkVeto", fix_ttbar);
  PrintLine(presel+new_tk_veto+"met>500&&ht30>1000&&num_jets_pt30>=9", "NewTkVeto", fix_ttbar);

  
  PrintLine(presel+"met>500&&ht30>1500", "MET>500&HT>1500", fix_ttbar);
  PrintLine(presel+mu_veto+el_veto+"met>500&&ht30>1500", "st. lepVeto", fix_ttbar);
  PrintLine(presel+mu_veto+el_veto+"num_iso_tracks_pt15_mT==0&&met>500&&ht30>1500", "+OldTkVeto", fix_ttbar);
  PrintLine(presel+mu_veto+el_veto+new_tk_veto+"met>500&&ht30>1500", "+NewTkVeto", fix_ttbar);
  PrintLine(presel+"num_reco_veto_muons_mT100==0&&(num_reco_veto_electrons==0||(num_reco_veto_electrons==1&&electron_mT>100))&&met>500&&ht30>1500", "lepVeto_mT", fix_ttbar);
  PrintLine(presel+"num_iso_tracks_pt15_mT==0&&met>500&&ht30>1500", "OldTkVeto", fix_ttbar);
  PrintLine(presel+new_tk_veto+"met>500&&ht30>1500", "NewTkVeto", fix_ttbar);

  // PrintLine(presel+"met>400&&ht30>1400", "MET>400&HT>1400", fix_ttbar);
  // PrintLine(presel+mu_veto+el_veto+"met>400&&ht30>1400", "+lepVeto", fix_ttbar);
  // PrintLine(presel+mu_veto+el_veto+"num_iso_tracks_pt15_mT==0&&met>400&&ht30>1400", "+TkTauVeto", fix_ttbar);

  //PrintLine(presel+mu_veto+el_veto+"met>500&&num_jets_pt30>=8", "MET>500&8jets", fix_ttbar);

  fclose(file);

}
