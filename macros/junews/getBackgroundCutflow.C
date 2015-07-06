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

//FILE * file;

float int_lumi(10000.);

void PrintHeader() {

  printf( "\\begin{sidewaystable}[h] \n");
  printf( "\\centering\n");
  printf( "\\begin{tabular}{lcccccc|c}\n");
  printf( "\\hline\n");
  printf( "\\hline\n");
  printf( "Cut & $t\\bar{t}$ & QCD  & $Z+$jets  & $W+$jets  & Single top & Other \\\\ \\hline\n");

}

void PrintTrailer() {

  printf( "\\hline\n");
  printf( "\\hline\n");
  printf( "\\end{tabular}\n");
  printf( "\\end{sidewaysytable}\n");
  
}

void PrintLine(TCut cut, string description, TCut baseline="") {

  TH1::SetDefaultSumw2(1);


  // cout << "Cut: " << description << endl;
 
  TH1D *h = new TH1D("h","",2,0,2);
  TCut weight = Form("weightppb*%f",int_lumi);
  double n_ttbar(0.), n_qcd(0.), n_znn(0.), n_wjets(0.), n_single_top(0.), n_other(0.), n_SM(0.);
  double b_ttbar(0.), b_qcd(0.), b_znn(0.), b_wjets(0.), b_single_top(0.), b_other(0.), b_SM(0.);
  Double_t e_ttbar(0.), e_qcd(0.), e_znn(0.), e_wjets(0.), e_single_top(0.), e_other(0.), e_SM(0.);
  double eff_ttbar(0.), eff_qcd(0.), eff_znn(0.), eff_wjets(0.), eff_single_top(0.), eff_other(0.), eff_SM(0.);

  // Fill hist to get baseline for efficiency calculation
  znn_ch->Project("h","1",baseline*weight);
  b_znn=h->IntegralAndError(1,3,e_znn);
  wjets_ch->Project("h","1",baseline*weight);
  b_wjets=h->IntegralAndError(1,3,e_wjets);
  ttbar_ch->Project("h","1",baseline*weight);
  b_ttbar=h->IntegralAndError(1,3,e_ttbar);
  qcd_ch->Project("h","1",baseline*weight);
  b_qcd=h->IntegralAndError(1,3,e_qcd);
  single_top_ch->Project("h","1",baseline*weight);
  b_single_top=h->IntegralAndError(1,3,e_single_top);
  other_ch->Project("h","1",baseline*weight);
  b_other=h->IntegralAndError(1,3,e_other);

  znn_ch->Project("h","1",baseline*weight);
  wjets_ch->Project("+h","1",baseline*weight);
  ttbar_ch->Project("+h","1",baseline*weight);
  qcd_ch->Project("+h","1",baseline*weight);
  single_top_ch->Project("+h","1",baseline*weight);
  other_ch->Project("+h","1",baseline*weight);
  b_SM=h->IntegralAndError(1,3,e_SM);
  
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

  znn_ch->Project("h","1",cut*weight);
  wjets_ch->Project("+h","1",cut*weight);
  ttbar_ch->Project("+h","1",cut*weight);
  qcd_ch->Project("+h","1",cut*weight);
  single_top_ch->Project("+h","1",cut*weight);
  other_ch->Project("+h","1",cut*weight);
  n_SM=h->IntegralAndError(1,3,e_SM);

  eff_ttbar=n_ttbar/b_ttbar;
  eff_qcd=n_qcd/b_qcd;
  eff_znn=n_znn/b_znn;
  eff_wjets=n_wjets/b_wjets;
  eff_single_top=n_single_top/b_single_top;
  eff_other=n_other/b_other;
  eff_SM=n_SM/b_SM;


  // fprintf(file, "%s & $%3.0f\\pm%3.0f\;(%3.2f)$ & $%3.0f\\pm%3.0f\;(%3.2f)$ & $%3.0f\\pm%3.0f\;(%3.2f)$ & $%3.0f\\pm%3.0f\;(%3.2f)$ & $%3.0f\\pm%3.0f\;(%3.2f)$ & $%3.0f\\pm%3.0f\;(%3.2f)$ \\\\\n", 
  // 	  description.c_str(), n_ttbar,  n_qcd, n_znn,   n_wjets,  n_single_top,   n_other);
  if (n_SM<100) {
    printf("%s & $%3.2f\\pm%3.2f\\;(%3.2f)$ & $%3.2f\\pm%3.2f\\;(%3.2f)$ & $%3.2f\\pm%3.2f\\;(%3.2f)$ & $%3.2f\\pm%3.2f\\;(%3.2f)$ & $%3.2f\\pm%3.2f\\;(%3.2f)$ & $%3.2f\\pm%3.2f\\;(%3.2f)$ & $%3.2f\\pm%3.2f\\;(%3.2f)$ \\\\\n", 
	   description.c_str(), n_ttbar,  e_ttbar, eff_ttbar, n_qcd, e_qcd, eff_qcd, n_znn, e_znn,  eff_znn, n_wjets, e_wjets, eff_wjets, n_single_top, e_single_top, eff_single_top, n_other, e_other, eff_other, n_SM, e_SM, eff_SM); 
  }
  else {
    printf("%s & $%3.2f\\;(%3.2f)$ & $%3.2f\\;(%3.2f)$ & $%3.2f\\;(%3.2f)$ & $%3.2f\\;(%3.2f)$ & $%3.2f\\;(%3.2f)$ & $%3.2f\\;(%3.2f)$ & $%3.2f\\;(%3.2f)$ \\\\\n", 
	   description.c_str(), n_ttbar,  eff_ttbar, n_qcd, eff_qcd, n_znn, eff_znn, n_wjets,  eff_wjets, n_single_top, eff_single_top, n_other, eff_other, n_SM, eff_SM); 
   }

  delete h;

}


void getBackgroundCutflow(float lumi=10000.) {

  int_lumi=lumi;

  ttbar_ch = new TChain("reduced_tree");
  qcd_ch = new TChain("reduced_tree");
  znn_ch = new TChain("reduced_tree");
  wjets_ch = new TChain("reduced_tree");
  single_top_ch = new TChain("reduced_tree");
  other_ch = new TChain("reduced_tree");

  ttbar_ch->Add("/cms8r0/johnbr/2014/csa14/reduced_trees/13TeV/TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1_MINIAODSIM_UCSB2406_v78/*");
  
  qcd_ch->Add("/cms8r0/johnbr/2014/csa14/reduced_trees/13TeV/QCD_Pt-300to470_Tune4C_13TeV_pythia8_Phys14DR-PU20bx25_trkalmb_castor_PHYS14_25_V1-v2_MINIAODSIM_UCSB2376_v78/*");
  qcd_ch->Add("/cms8r0/johnbr/2014/csa14/reduced_trees/13TeV/QCD_Pt-470to600_Tune4C_13TeV_pythia8_Phys14DR-PU20bx25_trkalmb_castor_PHYS14_25_V1-v2_MINIAODSIM_UCSB2375_v78/*");
  //temp_ch->Add("save/13TeV/QCD_Pt-600to800_Tune4C_13TeV_pythia8_Phys14DR-PU20bx25_trkalmb_castor_PHYS14_25_V1-v1_MINIAODSIM_UCSB2374_v78/*");
  qcd_ch->Add("/cms8r0/johnbr/2014/csa14/reduced_trees/13TeV/QCD_Pt-600to800_Tune4C_13TeV_pythia8_Phys14DR-PU20bx25_trkalmb_castor_PHYS14_25_V1-v1_MINIAODSIM_UCSB2374_v78/*");
  qcd_ch->Add("/cms8r0/johnbr/2014/csa14/reduced_trees/13TeV/QCD_Pt-800to1000_Tune4C_13TeV_pythia8_Phys14DR-PU20bx25_trkalmb_castor_PHYS14_25_V1-v2_MINIAODSIM_UCSB2373_v78/*");
  qcd_ch->Add("/cms8r0/johnbr/2014/csa14/reduced_trees/13TeV/QCD_Pt-1000to1400_Tune4C_13TeV_pythia8_Phys14DR-PU20bx25_trkalmb_castor_PHYS14_25_V1-v1_MINIAODSIM_UCSB2372_v78/*");
  qcd_ch->Add("/cms8r0/johnbr/2014/csa14/reduced_trees/13TeV/QCD_Pt-1400to1800_Tune4C_13TeV_pythia8_Phys14DR-PU20bx25_trkalmb_castor_PHYS14_25_V1-v1_MINIAODSIM_UCSB2371_v78/*");
  qcd_ch->Add("/cms8r0/johnbr/2014/csa14/reduced_trees/13TeV/QCD_Pt-1800to2400_Tune4C_13TeV_pythia8_Phys14DR-PU20bx25_trkalmb_PHYS14_25_V1-v2_MINIAODSIM_UCSB2370_v78/*");
  qcd_ch->Add("/cms8r0/johnbr/2014/csa14/reduced_trees/13TeV/QCD_Pt-2400to3200_Tune4C_13TeV_pythia8_Phys14DR-PU20bx25_trkalmb_PHYS14_25_V1-v1_MINIAODSIM_UCSB2369_v78/*");
  qcd_ch->Add("/cms8r0/johnbr/2014/csa14/reduced_trees/13TeV/QCD_Pt-3200_Tune4C_13TeV_pythia8_Phys14DR-PU20bx25_trkalmb_PHYS14_25_V1-v1_MINIAODSIM_UCSB2368_v78/*");

  znn_ch->Add("/cms8r0/johnbr/2014/csa14/reduced_trees/13TeV/ZJetsToNuNu_HT-100to200_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1_MINIAODSIM_UCSB2408_v78/*");
  znn_ch->Add("/cms8r0/johnbr/2014/csa14/reduced_trees/13TeV/ZJetsToNuNu_HT-200to400_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1_MINIAODSIM_UCSB2381_v78/*");
  znn_ch->Add("/cms8r0/johnbr/2014/csa14/reduced_trees/13TeV/ZJetsToNuNu_HT-400to600_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v2_MINIAODSIM_UCSB2380_v78/*");
  znn_ch->Add("/cms8r0/johnbr/2014/csa14/reduced_trees/13TeV/ZJetsToNuNu_HT-600toInf_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1_MINIAODSIM_UCSB2379_v78/*");

  wjets_ch->Add("/cms8r0/johnbr/2014/csa14/reduced_trees/13TeV/WJetsToLNu_HT-100to200_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1_MINIAODSIM_UCSB2407_v78/*");
  wjets_ch->Add("/cms8r0/johnbr/2014/csa14/reduced_trees/13TeV/WJetsToLNu_HT-200to400_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1_MINIAODSIM_UCSB2405_v78/*");
  wjets_ch->Add("/cms8r0/johnbr/2014/csa14/reduced_trees/13TeV/WJetsToLNu_HT-400to600_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1_MINIAODSIM_UCSB2398_v78/*");
  wjets_ch->Add("/cms8r0/johnbr/2014/csa14/reduced_trees/13TeV/WJetsToLNu_HT-600toInf_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1_MINIAODSIM_UCSB2397_v78/*");

  single_top_ch->Add("/cms8r0/johnbr/2014/csa14/reduced_trees/13TeV/TBarToLeptons_s-channel-CSA14_Tune4C_13TeV-aMCatNLO-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1_MINIAODSIM_UCSB2411_v78/*");
  single_top_ch->Add("/cms8r0/johnbr/2014/csa14/reduced_trees/13TeV/TBarToLeptons_t-channel_Tune4C_CSA14_13TeV-aMCatNLO-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1_MINIAODSIM_UCSB2412_v78/*");
  single_top_ch->Add("/cms8r0/johnbr/2014/csa14/reduced_trees/13TeV/Tbar_tW-channel-DR_Tune4C_13TeV-CSA14-powheg-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1_MINIAODSIM_UCSB2395_v78/*");
  single_top_ch->Add("/cms8r0/johnbr/2014/csa14/reduced_trees/13TeV/TToLeptons_s-channel-CSA14_Tune4C_13TeV-aMCatNLO-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1_MINIAODSIM_UCSB2410_v78/*");
  single_top_ch->Add("/cms8r0/johnbr/2014/csa14/reduced_trees/13TeV/TToLeptons_t-channel-CSA14_Tune4C_13TeV-aMCatNLO-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1_MINIAODSIM_UCSB2409_v78/*");
  single_top_ch->Add("/cms8r0/johnbr/2014/csa14/reduced_trees/13TeV/T_tW-channel-DR_Tune4C_13TeV-CSA14-powheg-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1_MINIAODSIM_UCSB2393_v78/*");
 
  other_ch->Add("/cms8r0/johnbr/2014/csa14/reduced_trees/13TeV/TTWJets_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1_MINIAODSIM_UCSB2390_v78/*");
  other_ch->Add("/cms8r0/johnbr/2014/csa14/reduced_trees/13TeV/TTZJets_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1_MINIAODSIM_UCSB2384_v78/*");
  other_ch->Add("/cms8r0/johnbr/2014/csa14/reduced_trees/13TeV/WH_HToBB_WToLNu_M-125_13TeV_powheg-herwigpp_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2382_v78/*");
  other_ch->Add("/cms8r0/johnbr/2014/csa14/reduced_trees/13TeV/TTbarH_M-125_13TeV_amcatnlo-pythia8-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v2_MINIAODSIM_UCSB2391_v78/*");
  other_ch->Add("/cms8r0/johnbr/2014/csa14/reduced_trees/13TeV/ZH_HToBB_ZToNuNu_M-125_13TeV_powheg-herwigpp_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2392_v78/*");


  //   ttbar_ch->Add("/cms8r0/johnbr/2014/csa14/reduced_trees/13TeV/skimmed/TTJets*v78*.root");
  // //  qcd_ch->Add("/cms8r0/johnbr/2014/csa14/reduced_trees/13TeV/skimmed/QCD_HT*v78*.root");
  // qcd_ch->Add("/cms8r0/johnbr/2014/csa14/reduced_trees/13TeV/skimmed/QCD_Pt*v78*.root");
  // znn_ch->Add("/cms8r0/johnbr/2014/csa14/reduced_trees/13TeV/skimmed/ZJets*v78*.root");
  // wjets_ch->Add("/cms8r0/johnbr/2014/csa14/reduced_trees/13TeV/skimmed/WJets*v78*.root");
  // single_top_ch->Add("/cms8r0/johnbr/2014/csa14/reduced_trees/13TeV/skimmed/TTo*v78*.root");
  // single_top_ch->Add("/cms8r0/johnbr/2014/csa14/reduced_trees/13TeV/skimmed/TBarTo*v78*.root");
  // single_top_ch->Add("/cms8r0/johnbr/2014/csa14/reduced_trees/13TeV/skimmed/*tW*v78*.root");
  // other_ch->Add("/cms8r0/johnbr/2014/csa14/reduced_trees/13TeV/skimmed/ZH*v78*.root");
  // other_ch->Add("/cms8r0/johnbr/2014/csa14/reduced_trees/13TeV/skimmed/WH*v78*.root");
  // other_ch->Add("/cms8r0/johnbr/2014/csa14/reduced_trees/13TeV/skimmed/TTbar*v78*.root");
  // other_ch->Add("/cms8r0/johnbr/2014/csa14/reduced_trees/13TeV/skimmed/TTW*v78*.root");
  // other_ch->Add("/cms8r0/johnbr/2014/csa14/reduced_trees/13TeV/skimmed/TTZ*v78*.root");
  


  //  file = fopen ("macros/Phys14/output/"+title+".tex", "w");
  PrintHeader();




 TCut ht("ht30>500"), njets("num_jets_pt30>=4"), mht30("mht30>200"), mdp("min_delta_phi_met_N>6");
 TCut mdp_new("passDeltaPhiCut");
  //  TCut  mu_veto("num_veto_mus==0"), el_veto("num_veto_els==0");
  TCut mu_veto("num_veto_mus==0"), el_veto("num_veto_els==0");
  TCut mutkveto("num_mu_tracks_mT==0"), eltkveto("num_el_tracks_mT==0"), hadtkveto("num_had_tracks_mT==0");
  TCut cleaning("passesBadJetFilter&&PBNRcode>0");

  TCut nj50("num_jets_pt50>=4");
  TCut mdp5("min_delta_phi_met_N>5"), mdp6("min_delta_phi_met_N>6"), mdp7("min_delta_phi_met_N>7"), mdp8("min_delta_phi_met_N>8");

  TCut lv2("Sum$(mu_pt>10&&mu_vid&&mu_mini_iso_tr02<0.2)+Sum$(el_pt>10&&el_vid&&el_mini_iso_tr02<0.1)==0");
  TCut isoTk("num_iso_tracks_pt15_mT==0");
  TCut tk3veto(mutkveto+eltkveto+hadtkveto);
  TCut baseline(njets+ht+mht30+mu_veto+el_veto+mutkveto+eltkveto+hadtkveto+mdp_new+cleaning);
  TCut mdp_mht4("min_delta_phi_mht_N>4");
  TCut MJ("fatpT30_MJ>200");
  TCut ttrich(ht+mht30+mu_veto+el_veto+mdp+isoTk+"num_jets_pt30>=6&&num_csvm_jets30>=2");

  TCut nj1("num_jets_pt30>=4&&num_jets_pt30<=6"), nj2("num_jets_pt30>=7&&num_jets_pt30<=8"), nj3("num_jets_pt30>=9");
  TCut nb0("num_csvm_jets30==0"), nb1("num_csvm_jets30==1"), nb2("num_csvm_jets30==2"), nb3("num_csvm_jets30>=3");
  TCut bgeq0("num_csvm_jets30>=0"), bgeq1("num_csvm_jets30>=1"), bgeq2("num_csvm_jets30>=2"), bgeq3("num_csvm_jets30>=3"), bleq1("num_csvm_jets30<=1");

  TCut se1("Sum$(mu_pt>10&&mu_dB_iso_R04<0.4&&mu_mT<100)==0&&Sum$(el_pt>10&&el_mT<100&&((abs(el_eta)>1.479 &&el_dB_iso_R03<0.212604) || el_dB_iso_R03<0.164369) )==1");
  TCut smu1("Sum$(mu_pt>10&&mu_dB_iso_R04<0.4&&mu_mT<100)==1&&Sum$(el_pt>10&&el_mT<100&&((abs(el_eta)>1.479 &&el_dB_iso_R03<0.212604) || el_dB_iso_R03<0.164369) )==0");

  TCut mht1("mht30>200&&mht30<500"), mht2("mht30>500&&mht30<750"), mht3("mht30>750");
  TCut ht1("ht30>500&&ht30<800"), ht2("ht30>800&&ht30<1200"), ht3("ht30>1200");

  TCut box1(mht1&&ht1), box2(mht1&&ht2), box3(mht1&&ht3), box4(mht2&&(ht1||ht2)), box5(mht2||ht3), box6(mht3&&(ht2||ht3));

  PrintLine("", "Start" "");
  PrintLine(njets, "$\\njets >=4$", "");
  PrintLine(njets+ht,"$\\HT > 500$ GeV", njets);
  PrintLine(mht30+njets+ht, "$\\MHT > 200$ GeV", njets+ht);
  PrintLine(njets+ht+mht30+mu_veto, "$\\mu$ veto", njets+ht+mht30);
  PrintLine(njets+ht+mht30+mu_veto+el_veto, "$e$ veto", njets+ht+mht30+mu_veto);
  PrintLine(njets+ht+mht30+mu_veto+el_veto+mutkveto, "$\\mu$ track veto", njets+ht+mht30+mu_veto+el_veto);
  PrintLine(njets+ht+mht30+mu_veto+el_veto+mutkveto+eltkveto, "$e$ track veto", njets+ht+mht30+mu_veto+el_veto+mutkveto);
  PrintLine(njets+ht+mht30+mu_veto+el_veto+mutkveto+eltkveto+hadtkveto, "Had. track veto", njets+ht+mht30+mu_veto+el_veto+mutkveto+eltkveto);
  PrintLine(njets+ht+mht30+mu_veto+el_veto+mutkveto+eltkveto+hadtkveto+mdp_new, "$\\Delta\\phi$ cuts", njets+ht+mht30+mu_veto+el_veto+mutkveto+eltkveto+hadtkveto);
  PrintLine(njets+ht+mht30+mu_veto+el_veto+mutkveto+eltkveto+hadtkveto+mdp_new+cleaning, "Evt. cleaning", njets+ht+mht30+mu_veto+el_veto+tk3veto+mdp_new);
  printf("\\hline\\multicolumn{8}{c}{$\\nbjets$ bins}\\\\\\hline\n");
  PrintLine(baseline+nb0, "$\\nbjets=0$", baseline);
  PrintLine(baseline+nb1, "$\\nbjets=1$", baseline);
  PrintLine(baseline+nb2, "$\\nbjets=2$", baseline);
  PrintLine(baseline+nb3, "$\\nbjets\\geq3$", baseline);
  
  PrintTrailer();
}
