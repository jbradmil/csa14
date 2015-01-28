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
#include "TPie.h"
#include "TPieSlice.h"
#include "TCanvas.h"
#include "macros/include/my_style.hpp"

using namespace std;


float int_lumi(4000.);

void bgPie(TCut selection="", TString tag="", float lumi=4000.) {

  int_lumi=lumi;
  TChain* ttbar_ch = new TChain("reduced_tree");
  TChain* qcd_ch = new TChain("reduced_tree");
  TChain* znn_ch = new TChain("reduced_tree");
  TChain* wjets_ch = new TChain("reduced_tree");
  TChain* single_top_ch = new TChain("reduced_tree");
  TChain* other_ch = new TChain("reduced_tree");


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

  ttbar_ch->Add("reduced_trees/13TeV/skimmed/TTJets*v77*.root");
  ttbar_ch->Add("reduced_trees/13TeV/skimmed/TTbar*v77*.root");
  ttbar_ch->Add("reduced_trees/13TeV/skimmed/TTW*v77*.root");
  ttbar_ch->Add("reduced_trees/13TeV/skimmed/TTZ*v77*.root");
  qcd_ch->Add("reduced_trees/13TeV/skimmed/QCD_HT*v77*.root");
  znn_ch->Add("reduced_trees/13TeV/skimmed/ZJets*v77*.root");
  wjets_ch->Add("reduced_trees/13TeV/skimmed/WJets*v77*.root");
  single_top_ch->Add("reduced_trees/13TeV/skimmed/TTo*v77*.root");
  single_top_ch->Add("reduced_trees/13TeV/skimmed/TBarTo*v77*.root");
  single_top_ch->Add("reduced_trees/13TeV/skimmed/*tW*v77*.root");
  other_ch->Add("reduced_trees/13TeV/skimmed/ZH*v77*.root");
  other_ch->Add("reduced_trees/13TeV/skimmed/WH*v77*.root");


  TCut ht("ht30>500"), njets("jet4_pt>30"), mht30("mht30>200"), mu_veto("num_reco_veto_muons==0"), el_veto("num_reco_veto_electrons==0"), mdp("min_delta_phi_met_N>4"), isoTk("num_iso_tracks_pt15_mT==0"),  baseline(ht+njets+mht30+mu_veto+el_veto+mdp+isoTk);

  TCanvas* can = new TCanvas("can", "can", 800, 800);
  TString pname, Hname = "histo", title;



  TH1D histo(Hname, "",100, 0, 10000);



  TCut cuts(baseline+selection);

  TChain* chains[6] = {ttbar_ch, qcd_ch, znn_ch, wjets_ch, single_top_ch, other_ch};
  
  vector<TString> labels;
  labels.push_back("t#bar{t}");
  labels.push_back("QCD");
  labels.push_back("Z+jets");
  labels.push_back("W+jets");
  labels.push_back("Single top");
  labels.push_back("Other");
  vector<float> yields;
  //vector<string> labels;
  vector<int> colors;
  colors.push_back(1000);
  colors.push_back(1001);
  colors.push_back(1002);
  colors.push_back(1004);
  colors.push_back(1005);
  colors.push_back(1003);
  // colors.push_back(kCyan+2);

  TCut totCut = (cuts)*"(weightppb*4000)";
  for(unsigned ind(0); ind<colors.size(); ind++){
    chains[ind]->Project(Hname, "met", totCut);
    yields.push_back(histo.Integral(0,101));
  }

  TPie pie("pie1", title, yields.size(), &yields[0]);
  pie.SetFillColors(&colors[0]);
  // pie.SetLabels(&labels[0]);
  for(unsigned ind(0); ind<colors.size(); ind++){
    pie.GetSlice(ind)->SetTitle(labels[ind].Data());
    cout << labels[ind] << ": " << yields[ind] << endl;
    //  pie.GetSlice(ind)->SetTitleFont(132);
    //  pie->GetSlice(ind)->SetTitleColor(colors[ind]);
  }
  // pie.SetLabelFormat("%txt: %val (%perc)");
   pie.SetLabelFormat("");
  // pie.SetLabelsOffset(0.03);
  pie.SetRadius(0.3);
  // pie.Draw("3d"); 
  pie.Draw("");
  pname = "macros/Phys14/plots/bg_piechart_"+tag+".pdf";
  can->Print(pname);
  
}
