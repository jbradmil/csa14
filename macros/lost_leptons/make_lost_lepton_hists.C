
#include <iostream>
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
#include "macros/include/my_style.hpp"



using namespace std;

TString plotdir = "macros/lost_leptons/plots/317/";

void make_lost_lepton_hists(TCut cuts, TCut sample, TString plot_title="default", TString signal="", bool plotLog=false )
{

  TH1::SetDefaultSumw2(1);


  if (gSystem->AccessPathName(plotdir))
    gSystem->mkdir(plotdir);

  //gStyle->SetHatchesLineWidth(3);

  // Setup style
  cout << "Setting tdr style."  << endl;
  TStyle *tdrStyle = new TStyle("tdrStyle","Style for P-TDR");
  setTDRStyle(tdrStyle);
  tdrStyle->cd();
  //tdrStyle->SetHatchesLineWidth(3);
  
  TChain* ttbar_ch = new TChain("reduced_tree");
  TChain* qcd_ch = new TChain("reduced_tree");
  TChain* znn_ch = new TChain("reduced_tree");
  TChain* wjets_ch = new TChain("reduced_tree");
  TChain* single_top_ch = new TChain("reduced_tree");
  TChain* other_ch = new TChain("reduced_tree");
  TChain* signal_ch = new TChain("reduced_tree");

  // ttbar_ch->Add("reduced_trees/13TeV/TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1_MINIAODSIM_UCSB2290_v77/*");
  
  // wjets_ch->Add("reduced_trees/13TeV/WJetsToLNu_HT-100to200_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1_MINIAODSIM_UCSB2298_v77/*");
  // wjets_ch->Add("reduced_trees/13TeV/WJetsToLNu_HT-200to400_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1_MINIAODSIM_UCSB2304_v77/*");
  // wjets_ch->Add("reduced_trees/13TeV/WJetsToLNu_HT-400to600_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1_MINIAODSIM_UCSB2305_v77/*");
  // wjets_ch->Add("reduced_trees/13TeV/WJetsToLNu_HT-600toInf_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1_MINIAODSIM_UCSB2306_v77/*");

  // single_top_ch->Add("reduced_trees/13TeV/TToLeptons_s-channel-CSA14_Tune4C_13TeV-aMCatNLO-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1_MINIAODSIM_UCSB2340_v77/*");
  // single_top_ch->Add("reduced_trees/13TeV/TToLeptons_t-channel-CSA14_Tune4C_13TeV-aMCatNLO-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1_MINIAODSIM_UCSB2341_v77/*");
  // single_top_ch->Add("reduced_trees/13TeV/T_tW-channel-DR_Tune4C_13TeV-CSA14-powheg-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1_MINIAODSIM_UCSB2338_v77/*");
  // single_top_ch->Add("reduced_trees/13TeV/TBarToLeptons_s-channel-CSA14_Tune4C_13TeV-aMCatNLO-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1_MINIAODSIM_UCSB2343_v77/*");
  // single_top_ch->Add("reduced_trees/13TeV/TBarToLeptons_t-channel_Tune4C_CSA14_13TeV-aMCatNLO-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1_MINIAODSIM_UCSB2342_v77/*");
  // single_top_ch->Add("reduced_trees/13TeV/Tbar_tW-channel-DR_Tune4C_13TeV-CSA14-powheg-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1_MINIAODSIM_UCSB2339_v77/*");
    
  // other_ch->Add("reduced_trees/13TeV/WH_HToBB_WToLNu_M-125_13TeV_powheg-herwigpp_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2350_v77/*");
  // other_ch->Add("reduced_trees/13TeV/ZH_HToBB_ZToNuNu_M-125_13TeV_powheg-herwigpp_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2351_v77/*");
  // other_ch->Add("reduced_trees/13TeV/TTbarH_M-125_13TeV_amcatnlo-pythia8-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v2_MINIAODSIM_UCSB2349_v77/*");
  // other_ch->Add("reduced_trees/13TeV/TTWJets_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1_MINIAODSIM_UCSB2347_v77/*");
  // other_ch->Add("reduced_trees/13TeV/TTZJets_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1_MINIAODSIM_UCSB2348_v77/*");

  ttbar_ch->Add("reduced_trees/13TeV/skimmed/TTJets*v77*.root");
  qcd_ch->Add("reduced_trees/13TeV/skimmed/QCD_Pt*v77*.root");
  znn_ch->Add("reduced_trees/13TeV/skimmed/ZJets*v77*.root");
  wjets_ch->Add("reduced_trees/13TeV/skimmed/WJets*v77*.root");
  single_top_ch->Add("reduced_trees/13TeV/skimmed/TTo*v77*.root");
  single_top_ch->Add("reduced_trees/13TeV/skimmed/TBarTo*v77*.root");
  single_top_ch->Add("reduced_trees/13TeV/skimmed/*tW*v77*.root");
  other_ch->Add("reduced_trees/13TeV/skimmed/ZH*v77*.root");
  other_ch->Add("reduced_trees/13TeV/skimmed/WH*v77*.root");
  other_ch->Add("reduced_trees/13TeV/skimmed/TTbarH_M-125_13TeV_amcatnlo-pythia8-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v2_MINIAODSIM_UCSB2349_v77*");
  other_ch->Add("reduced_trees/13TeV/skimmed/TTWJets_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1_MINIAODSIM_UCSB2347_v77*");
  other_ch->Add("reduced_trees/13TeV/skimmed/TTZJets_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1_MINIAODSIM_UCSB2348_v77*");



  // ttbar_ch->Add("reduced_trees/13TeV/skimmed/TTJets*v77*.root");
  // qcd_ch->Add("reduced_trees/13TeV/skimmed/QCD_HT*v77*.root");
  // znn_ch->Add("reduced_trees/13TeV/skimmed/ZJets*v77*.root");
  // wjets_ch->Add("reduced_trees/13TeV/skimmed/WJets*v77*.root");
  // single_top_ch->Add("reduced_trees/13TeV/skimmed/TTo*v77*.root");
  // single_top_ch->Add("reduced_trees/13TeV/skimmed/TBarTo*v77*.root");
  // single_top_ch->Add("reduced_trees/13TeV/skimmed/*tW*v77*.root");
  // other_ch->Add("reduced_trees/13TeV/skimmed/ZH*v77*.root");
  // other_ch->Add("reduced_trees/13TeV/skimmed/WH*v77*.root");
  // other_ch->Add("reduced_trees/13TeV/skimmed/TTbarH_M-125_13TeV_amcatnlo-pythia8-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v2_MINIAODSIM_UCSB2349_v77*");
  // other_ch->Add("reduced_trees/13TeV/skimmed/TTWJets_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1_MINIAODSIM_UCSB2347_v77*");
  // other_ch->Add("reduced_trees/13TeV/skimmed/TTZJets_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1_MINIAODSIM_UCSB2348_v77*");



  if (signal.Length()>0) signal_ch->Add(signal);

  cout << "Entries in chains..." << endl;
  cout << "signal N=" << signal_ch->GetEntries() << endl;
  cout << "ttbar N=" << ttbar_ch->GetEntries() << endl;
  cout << "qcd N=" << qcd_ch->GetEntries() << endl;
  cout << "znn N=" << znn_ch->GetEntries() << endl;
  cout << "wjets N=" << wjets_ch->GetEntries() << endl;
  cout << "single_top N=" << single_top_ch->GetEntries() << endl;
  cout << "other N=" << other_ch->GetEntries() << endl;

  // quick skim
  TCut baseline("mht30>200&&min_delta_phi_met_N>4&&num_jets_pt30>=4&&ht30>500");
  // baseline+="num_csvm_jets30>=2";
  TCut old_lep_veto("num_veto_mus==0&&num_veto_els==0"), old_tk_veto("num_iso_tracks_pt15_mT==0");
  // TCut baseline_pre_tk("mht30>200&&min_delta_phi_met_N>4&&num_veto_mus==0&&num_veto_els==0&&num_jets_pt30>=4&&ht30>500");

  // TCut nj1("num_jets_pt30>=4&&num_jets_pt30<=6"), nj2("num_jets_pt30>=7&&num_jets_pt30<=8"), nj3("num_jets_pt30>=9");
  // TCut nb0("num_csvm_jets30==0"), nb1("num_csvm_jets30==1"), nb2("num_csvm_jets30==2"), nb3("num_csvm_jets30>=3");
  // TCut bgeq0("num_csvm_jets30>=0"), bgeq1("num_csvm_jets30>=1"), bgeq2("num_csvm_jets30>=2"), bgeq3("num_csvm_jets30>=3");
  TCut mht1("mht30>200&&mht30<500"), mht2("mht30>500&&mht30<750"), mht3("mht30>750");
  TCut ht1("ht30>500&&ht30<800"), ht2("ht30>800&&ht30<1200"), ht3("ht30>1200");

  TCut box1(mht1+ht1), box2(mht1+ht2), box3(mht1+ht3), box4(mht2+(ht1||ht2)), box5(mht2+ht3), box6(mht3+(ht2||ht3));

  TCut bin0("num_true_els+num_true_mus+num_true_had_taus==0");
  TCut bin1("num_true_els==1&&num_true_mus==0&&num_true_had_taus==0"), bin2("num_true_els==0&&num_true_mus==1&&num_true_had_taus==0"), bin3("num_true_els==0&&num_true_mus==0&&num_true_had_taus==1"),
    bin4("num_true_els+num_true_mus==2&&num_true_had_taus==0"), bin5("num_true_els+num_true_mus==1&&num_true_had_taus==1"), bin6("num_true_els+num_true_mus==0&&num_true_had_taus==2"),
    bin7("num_true_els+num_true_mus+num_true_had_taus>=3");


  const Int_t nbinsx = 8;
  double xlow(0.), xup(8.);
  char *bin_labels[nbinsx] = {"0 e/#mu/#tau_{had}", "1 e","1 #mu","1 #tau_{had}", "2 e/#mu", "1 e/#mu + 1 #tau_{had}", "2 #tau_{had}", "#geq3 e/#mu/#tau_{had}"};
  
  // Book histograms
  // just the events that fail "cuts"
  TH1D * httbar_f = new TH1D("ttbar_f" , "", nbinsx, xlow, xup);
  TH1D * hqcd_f = new TH1D("qcd_f" , "", nbinsx, xlow, xup);
  TH1D * hznn_f = new TH1D("znn_f" , "", nbinsx, xlow, xup);
  TH1D * hwjets_f = new TH1D("wjets_f" , "", nbinsx, xlow, xup);
  TH1D * hother_f = new TH1D("other_f" , "", nbinsx, xlow, xup);
  TH1D * hsingle_top_f = new TH1D("single_top_f" , "", nbinsx, xlow, xup);  
  TH1D * hmc_exp_f = new TH1D("mc_exp_f" , "", nbinsx, xlow, xup);
  TH1D * hsignal_f = new TH1D("signal_f" , "", nbinsx, xlow, xup);
  // just the events that pass "cuts"
  TH1D * httbar_p = new TH1D("ttbar_p" , "", nbinsx, xlow, xup);
  TH1D * hqcd_p = new TH1D("qcd_p" , "", nbinsx, xlow, xup);
  TH1D * hznn_p = new TH1D("znn_p" , "", nbinsx, xlow, xup);
  TH1D * hwjets_p = new TH1D("wjets_p" , "", nbinsx, xlow, xup);
  TH1D * hother_p = new TH1D("other_p" , "", nbinsx, xlow, xup);
  TH1D * hsingle_top_p = new TH1D("single_top_p" , "", nbinsx, xlow, xup);  
  TH1D * hmc_exp_p = new TH1D("mc_exp_p" , "", nbinsx, xlow, xup);
  TH1D * hsignal_p = new TH1D("signal_p" , "", nbinsx, xlow, xup);

  TH1D * hdumb = new TH1D("dumb" , "", nbinsx, xlow, xup);

  // Format cuts
  TCut fail = sample+baseline+!cuts; // the events that pass "cuts"
  TCut cut = sample+baseline+cuts; // the events that pass "cuts"
  if (fail==cut) fail="0==1"; // if not applying any cut, don't fill the fail hist
  TCut weights = "weightppb*4000";
  cout << "Pass: " << cut.GetTitle() << endl;
  cout << "Reject: " << fail.GetTitle() << endl;

  if (signal.Length()>0) {
    cout << "Filling signal hists..." << endl;
    signal_ch->Project("signal_p", "0.5", (cut+bin0)*weights);
    signal_ch->Project("+signal_p", "1.5", (cut+bin1)*weights);
    signal_ch->Project("+signal_p", "2.5", (cut+bin2)*weights);
    signal_ch->Project("+signal_p", "3.5", (cut+bin3)*weights);
    signal_ch->Project("+signal_p", "4.5", (cut+bin4)*weights);
    signal_ch->Project("+signal_p", "5.5", (cut+bin5)*weights);
    signal_ch->Project("+signal_p", "6.5", (cut+bin6)*weights);
    signal_ch->Project("+signal_p", "7.5", (cut+bin7)*weights);
    signal_ch->Project("signal_f", "0.5", (fail+bin0)*weights);
    signal_ch->Project("+signal_f", "1.5", (fail+bin1)*weights);
    signal_ch->Project("+signal_f", "2.5", (fail+bin2)*weights);
    signal_ch->Project("+signal_f", "3.5", (fail+bin3)*weights);
    signal_ch->Project("+signal_f", "4.5", (fail+bin4)*weights);
    signal_ch->Project("+signal_f", "5.5", (fail+bin5)*weights);
    signal_ch->Project("+signal_f", "6.5", (fail+bin6)*weights);
    signal_ch->Project("+signal_f", "7.5", (fail+bin7)*weights);
  } else { // make the plot for the BGs
    cout << "Filling BG hists..." << endl;
    // Fill fail hists
    // bin 0
    //  cout << "ttbar events in bin0: " << ttbar_ch->GetEntries(fail+bin0) << endl;
    ttbar_ch->Project("ttbar_f", "0.5", (fail+bin0)*weights);
    qcd_ch->Project("qcd_f", "0.5", (fail+bin0)*weights);
    znn_ch->Project("znn_f", "0.5", (fail+bin0)*weights);
    wjets_ch->Project("wjets_f", "0.5",(fail+bin0)*weights);
    other_ch->Project("other_f", "0.5", (fail+bin0)*weights);
    single_top_ch->Project("single_top_f", "0.5", (fail+bin0)*weights);
    // bin 1
    //  cout << "ttbar events in bin1: " << ttbar_ch->GetEntries(fail+bin1) << endl;
    ttbar_ch->Project("+ttbar_f", "1.5", (fail+bin1)*weights);
    qcd_ch->Project("+qcd_f", "1.5", (fail+bin1)*weights);
    znn_ch->Project("+znn_f", "1.5", (fail+bin1)*weights);
    wjets_ch->Project("+wjets_f", "1.5",(fail+bin1)*weights);
    other_ch->Project("+other_f", "1.5", (fail+bin1)*weights);
    single_top_ch->Project("+single_top_f", "1.5", (fail+bin1)*weights);
    // bin 2
    //  cout << "ttbar events in bin2: " << ttbar_ch->GetEntries(fail+bin2);
    ttbar_ch->Project("+ttbar_f", "2.5", (fail+bin2)*weights);
    qcd_ch->Project("+qcd_f", "2.5", (fail+bin2)*weights);
    znn_ch->Project("+znn_f", "2.5", (fail+bin2)*weights);   
    wjets_ch->Project("+wjets_f", "2.5",(fail+bin2)*weights);
    other_ch->Project("+other_f", "2.5", (fail+bin2)*weights);
    single_top_ch->Project("+single_top_f", "2.5", (fail+bin2)*weights);
    // bin 3
    //cout << "ttbar events in bin2: " << ttbar_ch->GetEntries(fail+bin3);
    ttbar_ch->Project("+ttbar_f", "3.5", (fail+bin3)*weights);
    qcd_ch->Project("+qcd_f", "3.5", (fail+bin3)*weights);
    znn_ch->Project("+znn_f", "3.5", (fail+bin3)*weights);    
    wjets_ch->Project("+wjets_f", "3.5",(fail+bin3)*weights);
    other_ch->Project("+other_f", "3.5", (fail+bin3)*weights);
    single_top_ch->Project("+single_top_f", "3.5", (fail+bin3)*weights);
    // bin 4
    ttbar_ch->Project("+ttbar_f", "4.5", (fail+bin4)*weights);
    qcd_ch->Project("+qcd_f", "4.5", (fail+bin4)*weights);
    znn_ch->Project("+znn_f", "4.5", (fail+bin4)*weights);    
    wjets_ch->Project("+wjets_f", "4.5",(fail+bin4)*weights);
    other_ch->Project("+other_f", "4.5", (fail+bin4)*weights);
    single_top_ch->Project("+single_top_f", "4.5", (fail+bin4)*weights);
    // bin 5
    ttbar_ch->Project("+ttbar_f", "5.5", (fail+bin5)*weights);
    qcd_ch->Project("+qcd_f", "5.5", (fail+bin5)*weights);
    znn_ch->Project("+znn_f", "5.5", (fail+bin5)*weights); 
    wjets_ch->Project("+wjets_f", "5.5",(fail+bin5)*weights);
    other_ch->Project("+other_f", "5.5", (fail+bin5)*weights);
    single_top_ch->Project("+single_top_f", "5.5", (fail+bin5)*weights);
    // bin 6
    ttbar_ch->Project("+ttbar_f", "6.5", (fail+bin6)*weights);
    qcd_ch->Project("+qcd_f", "6.5", (fail+bin6)*weights);
    znn_ch->Project("+znn_f", "6.5", (fail+bin6)*weights);
    wjets_ch->Project("+wjets_f", "6.5",(fail+bin6)*weights);
    other_ch->Project("+other_f", "6.5", (fail+bin6)*weights);
    single_top_ch->Project("+single_top_f", "6.5", (fail+bin6)*weights);
    // bin 7
    ttbar_ch->Project("+ttbar_f", "7.5", (fail+bin7)*weights);
    qcd_ch->Project("+qcd_f", "7.5", (fail+bin7)*weights);
    znn_ch->Project("+znn_f", "7.5", (fail+bin7)*weights);
    wjets_ch->Project("+wjets_f", "7.5",(fail+bin7)*weights);
    other_ch->Project("+other_f", "7.5", (fail+bin7)*weights);
    single_top_ch->Project("+single_top_f", "7.5", (fail+bin7)*weights);
    // Fill just the ones that pass
    // bin 0
    //  cout << "ttbar events in bin0: " << ttbar_ch->GetEntries(cut+bin0) << endl;
    ttbar_ch->Project("ttbar_p", "0.5", (cut+bin0)*weights);
    qcd_ch->Project("qcd_p", "0.5", (cut+bin0)*weights);
    znn_ch->Project("znn_p", "0.5", (cut+bin0)*weights);
    wjets_ch->Project("wjets_p", "0.5",(cut+bin0)*weights);
    other_ch->Project("other_p", "0.5", (cut+bin0)*weights);
    single_top_ch->Project("single_top_p", "0.5", (cut+bin0)*weights);
    // bin 1
    //  cout << "ttbar events in bin1: " << ttbar_ch->GetEntries(cut+bin1) << endl;
    ttbar_ch->Project("+ttbar_p", "1.5", (cut+bin1)*weights);
    qcd_ch->Project("+qcd_p", "1.5", (cut+bin1)*weights);
    znn_ch->Project("+znn_p", "1.5", (cut+bin1)*weights);
    wjets_ch->Project("+wjets_p", "1.5",(cut+bin1)*weights);
    other_ch->Project("+other_p", "1.5", (cut+bin1)*weights);
    single_top_ch->Project("+single_top_p", "1.5", (cut+bin1)*weights);
    // bin 2
    //  cout << "ttbar events in bin2: " << ttbar_ch->GetEntries(cut+bin2);
    ttbar_ch->Project("+ttbar_p", "2.5", (cut+bin2)*weights);
    qcd_ch->Project("+qcd_p", "2.5", (cut+bin2)*weights);
    znn_ch->Project("+znn_p", "2.5", (cut+bin2)*weights);   
    wjets_ch->Project("+wjets_p", "2.5",(cut+bin2)*weights);
    other_ch->Project("+other_p", "2.5", (cut+bin2)*weights);
    single_top_ch->Project("+single_top_p", "2.5", (cut+bin2)*weights);
    // bin 3
    //cout << "ttbar events in bin2: " << ttbar_ch->GetEntries(cut+bin3);
    ttbar_ch->Project("+ttbar_p", "3.5", (cut+bin3)*weights);
    qcd_ch->Project("+qcd_p", "3.5", (cut+bin3)*weights);
    znn_ch->Project("+znn_p", "3.5", (cut+bin3)*weights);  
    wjets_ch->Project("+wjets_p", "3.5",(cut+bin3)*weights);
    other_ch->Project("+other_p", "3.5", (cut+bin3)*weights);
    single_top_ch->Project("+single_top_p", "3.5", (cut+bin3)*weights);
    // bin 4
    ttbar_ch->Project("+ttbar_p", "4.5", (cut+bin4)*weights);
    qcd_ch->Project("+qcd_p", "4.5", (cut+bin4)*weights);
    znn_ch->Project("+znn_p", "4.5", (cut+bin4)*weights);
    wjets_ch->Project("+wjets_p", "4.5",(cut+bin4)*weights);
    other_ch->Project("+other_p", "4.5", (cut+bin4)*weights);
    single_top_ch->Project("+single_top_p", "4.5", (cut+bin4)*weights);
    // bin 5
    ttbar_ch->Project("+ttbar_p", "5.5", (cut+bin5)*weights);
    qcd_ch->Project("+qcd_p", "5.5", (cut+bin5)*weights);
    znn_ch->Project("+znn_p", "5.5", (cut+bin5)*weights);
    wjets_ch->Project("+wjets_p", "5.5",(cut+bin5)*weights);
    other_ch->Project("+other_p", "5.5", (cut+bin5)*weights);
    single_top_ch->Project("+single_top_p", "5.5", (cut+bin5)*weights);
    // bin 6
    ttbar_ch->Project("+ttbar_p", "6.5", (cut+bin6)*weights);
    qcd_ch->Project("+qcd_p", "6.5", (cut+bin6)*weights);
    znn_ch->Project("+znn_p", "6.5", (cut+bin6)*weights);
    wjets_ch->Project("+wjets_p", "6.5",(cut+bin6)*weights);
    other_ch->Project("+other_p", "6.5", (cut+bin6)*weights);
    single_top_ch->Project("+single_top_p", "6.5", (cut+bin6)*weights);
    // bin 7
    ttbar_ch->Project("+ttbar_p", "7.5", (cut+bin7)*weights);
    qcd_ch->Project("+qcd_p", "7.5", (cut+bin7)*weights);
    znn_ch->Project("+znn_p", "7.5", (cut+bin7)*weights); 
    wjets_ch->Project("+wjets_p", "7.5",(cut+bin7)*weights);
    other_ch->Project("+other_p", "7.5", (cut+bin7)*weights);
    single_top_ch->Project("+single_top_p", "7.5", (cut+bin7)*weights);
  }

  
  // Add up MC histograms
  if (signal.Length()>0) {
    hmc_exp_f->Add(hsignal_f);
    hmc_exp_p->Add(hsignal_p);
  } else {
    hmc_exp_f->Add(httbar_f);
    hmc_exp_f->Add(hqcd_f);
    hmc_exp_f->Add(hznn_f);
    hmc_exp_f->Add(hwjets_f);
    hmc_exp_f->Add(hsingle_top_f);
    hmc_exp_f->Add(hother_f);
    hmc_exp_p->Add(httbar_p);
    hmc_exp_p->Add(hqcd_p);
    hmc_exp_p->Add(hznn_p);
    hmc_exp_p->Add(hwjets_p);
    hmc_exp_p->Add(hsingle_top_p);
    hmc_exp_p->Add(hother_p);
  }
  cout << "... DONE: add all backgrounds to mc_exp_f." << endl;
  hmc_exp_f->Print("all");
  cout << "... DONE: add all backgrounds to mc_exp_p." << endl;
  hmc_exp_p->Print("all");
  
  TString ytitle = "Events / Bin";
  hdumb->GetXaxis()->SetTitle(httbar_p->GetXaxis()->GetTitle());
  hdumb->GetYaxis()->SetTitle(ytitle);

  for (int i=1;i<=nbinsx;i++) {
    hdumb->GetXaxis()->SetBinLabel(i,bin_labels[i-1]);
  }
  hdumb->GetXaxis()->SetLabelFont(132);
  hdumb->GetXaxis()->SetLabelSize(0.1);
  
  THStack * hs = new THStack("hs", "");
  if (signal.Length()>0) {
    hs->Add(hsignal_p);
    hs->Add(hsignal_f);
  } else {
    hs->Add(hother_p); 
    hs->Add(hsingle_top_p);
    hs->Add(hwjets_p);
    hs->Add(hznn_p);
    hs->Add(hqcd_p);
    hs->Add(httbar_p);
    hs->Add(hother_f); 
    hs->Add(hsingle_top_f);
    hs->Add(hwjets_f);
    hs->Add(hznn_f);
    hs->Add(hqcd_f);
    hs->Add(httbar_f);
  }

  // Setup histogram styles
  set_style(httbar_f, "ttbar", true);
  set_style(hqcd_f, "qcd", true);
  set_style(hznn_f, "znn", true);
  set_style(hwjets_f, "wjets", true);
  set_style(hother_f, "other", true);
  set_style(hsingle_top_f, "single_top", true);
  
  set_style(httbar_p, "ttbar");
  set_style(hqcd_p, "qcd");
  set_style(hznn_p, "znn");
  set_style(hwjets_p, "wjets");
  set_style(hother_p, "other");
  set_style(hsingle_top_p, "single_top");

  set_style(hsignal_f, "signal", true);
  set_style(hsignal_p, "signal");
 


  // Setup auxiliary histograms (ratios, errors, etc)
  TH1D* hmc_exp = new TH1D("mc_exp" , "", nbinsx, xlow, xup);
  hmc_exp->Add(hmc_exp_p);
  hmc_exp->Add(hmc_exp_f);
  
  TH1D * staterr_p = (TH1D *) hmc_exp_p->Clone("staterr");
  staterr_p->Sumw2();
  //staterr_p->SetFillColor(kRed);
  staterr_p->SetFillColor(kGray+3);
  staterr_p->SetMarkerSize(0);
  staterr_p->SetFillStyle(3013);
  TH1D * staterr = (TH1D *) hmc_exp->Clone("staterr");
  staterr->Sumw2();
  //staterr->SetFillColor(kRed);
  staterr->SetFillColor(kGray+3);
  staterr->SetMarkerSize(0);
  staterr->SetFillStyle(3144);

 
  // Setup legends
  TLegend * leg1 = new TLegend(0.56, 0.58, 0.80, 0.92);
  set_style(leg1,0.025);
  leg1->SetHeader("Pass");
  leg1->AddEntry(httbar_p, "t#bar{t}", "f");
  leg1->AddEntry(hqcd_p, "QCD", "f");
  leg1->AddEntry(hznn_p, "Z+jets", "f");
  leg1->AddEntry(hwjets_p, "W+jets", "f");
  leg1->AddEntry(hsingle_top_p, "Single Top", "f");
  leg1->AddEntry(hother_p, "Other", "f");
  leg1->AddEntry(staterr_p, "MC uncert.", "f");

  TLegend * leg2 = new TLegend(0.74, 0.58, 0.96, 0.92);
  set_style(leg2,0.025);
  leg2->SetHeader("Reject");
  leg2->AddEntry(httbar_f, "t#bar{t}", "f");
  leg2->AddEntry(hqcd_f, "QCD", "f");
  leg2->AddEntry(hznn_f, "Z+jets", "f");
  leg2->AddEntry(hwjets_f, "W+jets", "f");
  leg2->AddEntry(hsingle_top_f, "Single Top", "f");
  leg2->AddEntry(hother_f, "Other", "f");
  leg2->AddEntry(staterr, "MC uncert.", "f");

  TLegend * legs = new TLegend(0.74, 0.68, 0.96, 0.92);
  set_style(legs,0.025);
  legs->SetHeader("Signal");
  legs->AddEntry(hsignal_f, "Reject", "f");
  legs->AddEntry(hsignal_p, "Save", "f");
    
  double ymax = hs->GetMaximum();

  if(plotLog) {
    hdumb->SetMaximum(1000*ymax);
    hdumb->SetMinimum(0.1);
  }
  else {
    hdumb->SetMaximum(2*ymax);
  }

  // Setup canvas and pads  
  TCanvas * c1 = new TCanvas("c1", "c1", 1000, 800);
  TPad * pad1 = new TPad("pad1", "top pad" , 0.0, 0.3, 1.0, 1.0);
  TPad * pad2 = new TPad("pad2", "bottom pad", 0.0, 0.0, 1.0, 0.3);

  c1->cd();
  c1->SetLogy(plotLog);
  
  hdumb->GetXaxis()->SetLabelSize(0.04);
  hdumb->GetYaxis()->SetLabelSize(0.03);
  hdumb->Draw();
  hs->Draw("hist, same");
  staterr->Draw("e2 same");
  staterr_p->Draw("e2 same");
  if (signal.Length()>0) {
    legs->Draw();
  } else {
    leg1->Draw();
    leg2->Draw();
  }


  TLatex * latex = new TLatex();
  latex->SetNDC();
  latex->SetTextAlign(12);
  latex->SetTextFont(62);
  latex->SetTextSize(0.042);
  latex->DrawLatex(0.19, 0.89, "CMS Simulation");
  latex->SetTextSize(0.03);
  latex->DrawLatex(0.19, 0.84, "#sqrt{s} = 13 TeV, L = 4 fb^{-1}");
  

  TLatex * loss = new TLatex();
  loss->SetTextFont(132);
  loss->SetTextAlign(22);
  for (int i=1;i<=nbinsx;i++) {
    double xf = hmc_exp->GetBinCenter(i);
    double yf = hmc_exp_p->GetBinContent(i);
    double yi = hmc_exp->GetBinContent(i);
    double percent_rej = 100*(yi-yf)/yi;
    TString message = Form("%3.1f%%",percent_rej);
    if(yi/ymax<0.5) loss->DrawLatex(xf, yi+hmc_exp->GetMaximum()/10., message);
    else loss->DrawLatex(xf, 1.1*yi, message);
  } 

  // Print
  cout << "MakePlots(): Printing..." << endl;

  c1->cd();
  if (signal.Length()>0)  {
    if (signal.Contains("1500")) gPad->Print(plotdir+"nc_t1tttt_lost_lepton_bins_"+plot_title+".pdf");
	else gPad->Print(plotdir+"c_t1tttt_lost_lepton_bins_"+plot_title+".pdf");
  }
  else gPad->Print(plotdir+"lost_lepton_bins_"+plot_title+".pdf");


  // Clean up
  delete staterr;
  delete staterr_p;
  delete leg1;
  delete leg2;
  delete latex;
  // delete pave;
  delete hs;
  delete pad1;
  delete pad2;
  delete c1;

  delete hdumb;
  delete httbar_f;
  delete hqcd_f;
  delete hznn_f;
  delete hwjets_f;
  delete hsingle_top_f;
  delete hother_f;
  delete hmc_exp_f;
  delete httbar_p;
  delete hqcd_p;
  delete hznn_p;
  delete hwjets_p;
  delete hsingle_top_p;
  delete hother_p;
  delete hmc_exp_p;
  delete hmc_exp;

  delete hsignal_f;
  delete hsignal_p;

  cout << "MakePlots(): DONE!" << endl;

  return;
}
