/* 
   Script for drawing a branch from reduced trees with a set of cuts.
*/

#include <iostream>
#include <vector>
#include <math.h> 
#include "TMath.h"
#include "TROOT.h"
#include "TFile.h"
#include "TSystem.h"
#include "TChain.h"
#include "TTree.h"
#include "TCut.h"
#include "THStack.h"
#include "TLine.h"
#include "TFileCollection.h"
#include "TPaveText.h"
#include "../include/my_style.hpp"
#include "../include/remove_duplicate_events.h"
#include "../../inc/timer.hpp"

TChain* ttbar_ch;
TChain* qcd_ch;
TChain* znn_ch;
TChain* wjets_ch;
TChain* single_top_ch;
TChain* other_ch;
TChain* data_obs_ch;
// TChain* t1tttt_1500_100_ch;
// TChain* t1tttt_1200_800_ch;
// TChain* t1bbbb_1500_100_ch;
// TChain* t1bbbb_1000_900_ch;


using namespace std;

TString plotdir = "plots/127/";

void draw_from_trees(TString var, TCut other_cuts, TCut btag_cut,
		     /*TString weights,*/ TString title, int nbinsx, 
		     double xlow, double xup,
		     TString options="plotSig:plotLog:plotData",
		     TString plot_title="default")
{

  // bool plotSig = options.Contains("plotSig") && (!options.Contains("!plotSig"));
  bool plotLog = options.Contains("plotLog") && (!options.Contains("!plotLog"));
  bool plotData = options.Contains("plotData") && (!options.Contains("!plotData"));
  //  bool plotT1tttt = options.Contains("T1tttt") && (!options.Contains("!T1tttt"));
  // bool sigStack = options.Contains("sigStack") && (!options.Contains("!sigStack"));

  // Book histograms
  TH1D * httbar = new TH1D("ttbar" , title, nbinsx, xlow, xup);
  TH1D * hqcd = new TH1D("qcd" , title, nbinsx, xlow, xup);
  TH1D * hznn = new TH1D("znn" , title, nbinsx, xlow, xup);
  TH1D * hwjets = new TH1D("wjets" , title, nbinsx, xlow, xup);
  TH1D * hother = new TH1D("other" , title, nbinsx, xlow, xup);
  TH1D * hsingle_top = new TH1D("single_top" , title, nbinsx, xlow, xup);  
  TH1D * hmc_exp = new TH1D("mc_exp" , title, nbinsx, xlow, xup);
  TH1D * hdummy = new TH1D("dummy" , title, nbinsx, xlow, xup);
  TH1D * hdata_obs = new TH1D("data_obs" , title, nbinsx, xlow, xup);  
  // TH1D * ht1bbbb_1500_100 = new TH1D("t1bbbb_1500_100" , title, nbinsx, xlow, xup);
  // TH1D * ht1bbbb_1000_900 = new TH1D("t1bbbb_1000_900" , title, nbinsx, xlow, xup);
  // TH1D * ht1tttt_1500_100 = new TH1D("t1tttt_1500_100" , title, nbinsx, xlow, xup);
  // TH1D * ht1tttt_1200_800 = new TH1D("t1tttt_1200_800" , title, nbinsx, xlow, xup);

  // Format cuts
  TCut cut_MC(other_cuts);
  TCut cut_data(other_cuts+btag_cut+"passes2012RA2bTrigger");

  TCut weight("weightppb*trigger_eff_weight*19399");

  // Trigger weights (not needed)
  TCut met1_ht1("ht50>400&&ht50<500&&met>125&&met<150"), met2_ht1("ht50>400&&ht50<500&&met>150&&met<250"), met1_ht2("ht50>500&&ht50<800&&met>125&&met<150"), met2_ht2("ht50>500&&ht50<800&&met>150&&met<250"), met3_ht3("ht50>800||met>250");

  // btag weights
  TCut w_0b("Prob0"), w_geq1b("ProbGEQ1"), w_1b("Prob1"), w_geq2b("ProbGEQ2"), w_2b("Prob2"), w_geq3b("ProbGEQ3"), w_3b("Prob3"), w_geq4b("ProbGEQ4");
  // qcd_pt
  //  TCut qcd_0b("1.66"), qcd_1b("1.23"), qcd_2b("0.82"), qcd_3b("1.03"), qcd_geq4b("1.03");
  // qcd_ht
  // TCut qcd_0b("1.846"), qcd_1b("1.471"), qcd_2b("1.045"), qcd_geq3b("1.342");
  TCut qcd_0b("1.716"), qcd_1b("1.389"), qcd_2b("0.865"), qcd_geq3b("1.");
  if (plot_title.Contains("_raw")) {
    qcd_0b = "1.";
    qcd_1b = "1.";
    qcd_2b = "1.";
    qcd_geq3b = "1.";
  }
  TString s_btag_cut(btag_cut.GetTitle());
  if (s_btag_cut.Contains("==0")) weight*=w_0b;
  else if (s_btag_cut.Contains(">=1")) weight*=w_geq1b;
  else if (s_btag_cut.Contains("==1")) weight*=w_1b;
  else if (s_btag_cut.Contains(">=2")) weight*=w_geq2b;
  else if (s_btag_cut.Contains("==2")) weight*=w_2b;
  else if (s_btag_cut.Contains(">=3")) weight*=w_geq3b;
  else if (s_btag_cut.Contains("==3")) weight*=w_3b;
  else if (s_btag_cut.Contains(">=4")) weight*=w_geq4b; 
  else cout << "No btag selection." << endl;
  // TCut ttbar_weight("(weightppb*5000)/top_pt_weight_official");

  TCut ewk_weight(weight);
  
  TCut qcd_weight(weight);
  // QCD fudge factors
  if (s_btag_cut.Contains("==0")) qcd_weight*=qcd_0b;
  else if (s_btag_cut.Contains("==1")) qcd_weight*=qcd_1b;
  else if (s_btag_cut.Contains("==2")) qcd_weight*=qcd_2b;
  else if (s_btag_cut.Contains(">=3")) qcd_weight*=qcd_geq3b;
  else cout << "No qcd btag weight." << endl;
  

  // TCut qcd_trig_eff_met1_ht1(qcd_weight*"0.86"), qcd_trig_eff_met2_ht1(qcd_weight*"0.88"), qcd_trig_eff_met1_ht2(qcd_weight*"0.67"), qcd_trig_eff_met2_ht2(qcd_weight*"1.00");
  // TCut ewk_trig_eff_met1_ht1(ewk_weight*"0.91"), ewk_trig_eff_met2_ht1(ewk_weight*"0.98"), ewk_trig_eff_met1_ht2(ewk_weight*"0.95"), ewk_trig_eff_met2_ht2(ewk_weight*"0.99");

  // TString cut_weighted_qcd = Form("%s+%s+%s+%s+%s",((cut_MC+met1_ht1)*qcd_trig_eff_met1_ht1).GetTitle(), ((cut_MC+met2_ht1)*qcd_trig_eff_met2_ht1).GetTitle(), ((cut_MC+met1_ht2)*qcd_trig_eff_met1_ht2).GetTitle(), ((cut_MC+met2_ht2)*qcd_trig_eff_met2_ht2).GetTitle(), ((cut_MC+met3_ht3)*qcd_weight).GetTitle());
  // TString cut_weighted_ewk = Form("%s+%s+%s+%s+%s",((cut_MC+met1_ht1)*ewk_trig_eff_met1_ht1).GetTitle(), ((cut_MC+met2_ht1)*ewk_trig_eff_met2_ht1).GetTitle(), ((cut_MC+met1_ht2)*ewk_trig_eff_met1_ht2).GetTitle(), ((cut_MC+met2_ht2)*ewk_trig_eff_met2_ht2).GetTitle(), ((cut_MC+met3_ht3)*ewk_weight).GetTitle());

  //  if (s_btag_cut.Length()<1) cut_MC = (other_cuts+"num_csvm_jets==0")*w_0b+(other_cuts+"num_csvm_jets==1")*w_1b+(other_cuts+"num_csvm_jets==2")*w_2b
  //			       +(other_cuts+"num_csvm_jets==3")*w_3b+(other_cuts+"num_csvm_jets>=4")*w_geq4b;

  TCut cut_weighted_qcd = cut_MC*qcd_weight;
  TCut cut_weighted_ewk = cut_MC*ewk_weight;

  if (var.Contains("num_csvm")) {
    cout << "Filling histograms for " << var.Data() << endl;
    cout << "QCD selection: " << cut_weighted_qcd.GetTitle() << endl;
    ttbar_ch->Project("ttbar","0.5",cut_weighted_ewk*w_0b);
    qcd_ch->Project("qcd","0.5",cut_weighted_qcd*w_0b*qcd_0b);
    znn_ch->Project("znn","0.5",cut_weighted_ewk*w_0b);
    wjets_ch->Project("wjets","0.5",cut_weighted_ewk*w_0b);
    other_ch->Project("other","0.5",cut_weighted_ewk*w_0b);
    single_top_ch->Project("single_top","0.5",cut_weighted_ewk*w_0b);
    ttbar_ch->Project("+ttbar","1.5",cut_weighted_ewk*w_1b);
    qcd_ch->Project("+qcd","1.5",cut_weighted_qcd*w_1b*qcd_1b);
    znn_ch->Project("+znn","1.5",cut_weighted_ewk*w_1b);
    wjets_ch->Project("+wjets","1.5",cut_weighted_ewk*w_1b);
    other_ch->Project("+other","1.5",cut_weighted_ewk*w_1b);
    single_top_ch->Project("+single_top","1.5",cut_weighted_ewk*w_1b);
    ttbar_ch->Project("+ttbar","2.5",cut_weighted_ewk*w_2b);
    qcd_ch->Project("+qcd","2.5",cut_weighted_qcd*w_2b*qcd_2b);
    znn_ch->Project("+znn","2.5",cut_weighted_ewk*w_2b);
    wjets_ch->Project("+wjets","2.5",cut_weighted_ewk*w_2b);
    other_ch->Project("+other","2.5",cut_weighted_ewk*w_2b);
    single_top_ch->Project("+single_top","2.5",cut_weighted_ewk*w_2b);
    ttbar_ch->Project("+ttbar","3.5",cut_weighted_ewk*w_geq3b);
    qcd_ch->Project("+qcd","3.5",cut_weighted_qcd*w_geq3b*qcd_geq3b);
    znn_ch->Project("+znn","3.5",cut_weighted_ewk*w_geq3b);
    wjets_ch->Project("+wjets","3.5",cut_weighted_ewk*w_geq3b);
    other_ch->Project("+other","3.5",cut_weighted_ewk*w_geq3b);
    single_top_ch->Project("+single_top","3.5",cut_weighted_ewk*w_geq3b);
    data_obs_ch->Project("data_obs",var,cut_data);
  } else{
    // if (s_btag_cut.Length()<1) cut_weighted_qcd*="1.9"; // e.g. for n jets plot
    cout << "Filling histograms for " << var.Data() << endl;
    ttbar_ch->Project("ttbar",var,cut_weighted_ewk);
    qcd_ch->Project("qcd",var,cut_weighted_qcd);
    znn_ch->Project("znn",var,cut_weighted_ewk);
    wjets_ch->Project("wjets",var,cut_weighted_ewk);
    other_ch->Project("other",var,cut_weighted_ewk);
    single_top_ch->Project("single_top",var,cut_weighted_ewk);
    data_obs_ch->Project("data_obs",var,cut_data);
    // t1bbbb_1500_100_ch->Project("t1bbbb_1500_100",var,(cut)*weights);
    // t1bbbb_1000_900_ch->Project("t1bbbb_1000_900",var,(cut)*weights);
    // t1tttt_1500_100_ch->Project("t1tttt_1500_100",var,(cut)*weights);
    // t1tttt_1200_800_ch->Project("t1tttt_1200_800",var,(cut)*weights);
  }
  cout << "Cuts: " << cut_data.GetTitle() << endl;
  cout << "QCD Selection: " << cut_weighted_qcd.GetTitle() << endl;


  bool addOverflow(true);
  Double_t e_overflow(0.), i_overflow(0.);
  if (addOverflow) {
    i_overflow=httbar->IntegralAndError(nbinsx,nbinsx+1,e_overflow);
    httbar->SetBinContent(nbinsx, i_overflow);
    httbar->SetBinError(nbinsx, e_overflow);
    i_overflow=hqcd->IntegralAndError(nbinsx,nbinsx+1,e_overflow);
    hqcd->SetBinContent(nbinsx, i_overflow);
    hqcd->SetBinError(nbinsx, e_overflow);
    i_overflow=hznn->IntegralAndError(nbinsx,nbinsx+1,e_overflow);
    hznn->SetBinContent(nbinsx, i_overflow);
    hznn->SetBinError(nbinsx, e_overflow);
    i_overflow=hwjets->IntegralAndError(nbinsx,nbinsx+1,e_overflow);
    hwjets->SetBinContent(nbinsx, i_overflow);
    hwjets->SetBinError(nbinsx, e_overflow);
    i_overflow=hsingle_top->IntegralAndError(nbinsx,nbinsx+1,e_overflow);
    hsingle_top->SetBinContent(nbinsx, i_overflow);
    hsingle_top->SetBinError(nbinsx, e_overflow);
    i_overflow=hother->IntegralAndError(nbinsx,nbinsx+1,e_overflow);
    hother->SetBinContent(nbinsx, i_overflow);
    hother->SetBinError(nbinsx, e_overflow);
    i_overflow=hdata_obs->IntegralAndError(nbinsx,nbinsx+1,e_overflow);
    hdata_obs->SetBinContent(nbinsx, i_overflow);
    hdata_obs->SetBinError(nbinsx, e_overflow);
    // i_overflow=ht1bbbb_1500_100->IntegralAndError(nbinsx,nbinsx+1,e_overflow);
    // ht1bbbb_1500_100->SetBinContent(nbinsx, i_overflow);
    // ht1bbbb_1500_100->SetBinError(nbinsx, e_overflow);
    // i_overflow=ht1bbbb_1000_900->IntegralAndError(nbinsx,nbinsx+1,e_overflow);
    // ht1bbbb_1000_900->SetBinContent(nbinsx, i_overflow);
    // ht1bbbb_1000_900->SetBinError(nbinsx, e_overflow);
    // i_overflow=ht1tttt_1500_100->IntegralAndError(nbinsx,nbinsx+1,e_overflow);
    // ht1tttt_1500_100->SetBinContent(nbinsx, i_overflow);
    // ht1tttt_1500_100->SetBinError(nbinsx, e_overflow);
    // i_overflow=ht1tttt_1200_800->IntegralAndError(nbinsx,nbinsx+1,e_overflow);
    // ht1tttt_1200_800->SetBinContent(nbinsx, i_overflow);
    // ht1tttt_1200_800->SetBinError(nbinsx, e_overflow);
  }

  
  // Add up MC histograms
  hmc_exp->Add(httbar);
  hmc_exp->Add(hqcd);
  hmc_exp->Add(hznn);
  hmc_exp->Add(hwjets);
  hmc_exp->Add(hsingle_top);
  hmc_exp->Add(hother);


  double binwidth = (xup - xlow) / nbinsx;
  TString ytitle = Form("Events / %.3f", binwidth);
  hdummy->GetXaxis()->SetTitle(httbar->GetXaxis()->GetTitle());
  hdummy->GetYaxis()->SetTitle(ytitle);
  cout << "... DONE: add all backgrounds to mc_exp." << endl;
  
  Double_t ttbar_e(0.), qcd_e(0.), znn_e(0.), wjets_e(0.), other_e(0.), single_top_e(0.), bg_tot_e(0.), data_obs_e(0.);
  double ttbar_n(httbar->IntegralAndError(0,nbinsx, ttbar_e));
  double qcd_n(hqcd->IntegralAndError(0,nbinsx, qcd_e));
  double znn_n(hznn->IntegralAndError(0,nbinsx, znn_e));
  double wjets_n(hwjets->IntegralAndError(0,nbinsx, wjets_e));
  double other_n(hother->IntegralAndError(0,nbinsx, other_e));
  double single_top_n(hsingle_top->IntegralAndError(0,nbinsx, single_top_e));
  double bg_tot(hmc_exp->IntegralAndError(0,nbinsx, bg_tot_e));
  double data_obs_tot(hdata_obs->IntegralAndError(0,nbinsx, data_obs_e));

  // double t1tttt_1500_100_n(ht1tttt_1500_100->IntegralAndError(0,nbinsx+1, t1tttt_1500_100_e));


  printf("Counts before cut: %s\n",var.Data());
  printf("&ttbar&qcd&znn&wjets&single top&other&SM total&Data\\\\ \n");
  printf("%s & %3.2f+-%3.2f & %3.2f+-%3.2f & %3.2f+-%3.2f & %3.2f+-%3.2f & %3.2f+-%3.2f & %3.2f+-%3.2f & %3.2f+-%3.2f & %3.0f+-%3.2f \\\\\n",
	 var.Data(),
	 ttbar_n,ttbar_e,
	 qcd_n,qcd_e,
	 znn_n,znn_e,
	 wjets_n,wjets_e,
	 single_top_n,single_top_e,
	 other_n,other_e,
	 //	 hmc_exp->GetBinContent(1), hmc_exp->GetBinError(1),
	 bg_tot,bg_tot_e,
	 data_obs_tot,data_obs_e);


  cout << "... DONE: filled histograms." << endl;
 
  // if (sigStack) {
  //   for (int bin(0); bin<nbinsx; bin++) {
  //     ht1bbbb_1500_100->SetBinContent(bin+1, hmc_exp->GetBinContent(bin+1));
  //     ht1bbbb_1000_900->SetBinContent(bin+1, hmc_exp->GetBinContent(bin+1));
  //     ht1tttt_1500_100->SetBinContent(bin+1, hmc_exp->GetBinContent(bin+1));
  //     ht1tttt_1200_800->SetBinContent(bin+1, hmc_exp->GetBinContent(bin+1));
  //   }
  // }
 
  set_style(httbar, "ttbar");
  set_style(hqcd, "qcd");
  set_style(hznn, "znn");
  set_style(hwjets, "wjets");
  set_style(hother, "other");
  set_style(hsingle_top, "single_top");
  set_style(hdata_obs, "data_obs");


  THStack * hs = new THStack("hs", "");
  hs->Add(hother); 
  hs->Add(hsingle_top);
  hs->Add(hwjets);
  hs->Add(hznn);
  // if (plot_title.Contains("2012")) {
  // hs->Add(httbar);
  //  hs->Add(hqcd);
  // }else {
  hs->Add(hqcd);
  hs->Add(httbar);
  //  }


  //hs->GetYaxis()->SetTitle("Events / 5 fb^{-1}");
  //hs->GetXaxis()->SetTitle(httbar->GetXaxis()->GetTitle());

  // Setup histogram styles
 
  // Setup auxiliary histograms (ratios, errors, etc)
  TH1D * staterr = (TH1D *) hmc_exp->Clone("staterr");
  staterr->Sumw2();
  //staterr->SetFillColor(kRed);
  staterr->SetFillColor(kGray+3);
  staterr->SetMarkerSize(0);
  staterr->SetFillStyle(3013);

  cout << "Compute ratio hist..." << endl;
  TH1D * ratio = (TH1D *) hdata_obs->Clone("ratio");
  ratio->Sumw2();
  ratio->SetMarkerSize(0.8);
  //ratio->SetMarkerSize(0.5);
  ratio->Divide(hdata_obs, hmc_exp, 1., 1., "");
  TH1D * ratiostaterr = (TH1D *) hmc_exp->Clone("ratiostaterr");
  ratiostaterr->Sumw2();
  ratiostaterr->SetStats(0);
  ratiostaterr->SetTitle(title);
  ratiostaterr->GetYaxis()->SetTitle("Data/MC");
  ratiostaterr->SetMaximum(2.2);
  ratiostaterr->SetMinimum(0);
  ratiostaterr->SetMarkerSize(0);
  //ratiostaterr->SetFillColor(kRed);
  ratiostaterr->SetFillColor(kGray+3);
  ratiostaterr->SetFillStyle(3013);
  ratiostaterr->GetXaxis()->SetLabelSize(0.12);
  ratiostaterr->GetXaxis()->SetTitleSize(0.14);
  ratiostaterr->GetXaxis()->SetTitleOffset(1.10);
  ratiostaterr->GetYaxis()->SetLabelSize(0.10);
  ratiostaterr->GetYaxis()->SetTitleSize(0.12);
  ratiostaterr->GetYaxis()->SetTitleOffset(0.6);
  ratiostaterr->GetYaxis()->SetNdivisions(505);
  TLine* ratiounity = new TLine(xlow,1,xup,1);
  ratiounity->SetLineStyle(2);
  for (Int_t i = 0; i < hmc_exp->GetNbinsX()+2; i++) {
    ratiostaterr->SetBinContent(i, 1.0);
    if (hmc_exp->GetBinContent(i) > 1e-6) { //< not empty
      double binerror = hmc_exp->GetBinError(i) / hmc_exp->GetBinContent(i);
      ratiostaterr->SetBinError(i, binerror);
    } else {
      ratiostaterr->SetBinError(i, 999.);
    }
  }
  TH1D * ratiosysterr = (TH1D *) ratiostaterr->Clone("ratiosysterr");
  ratiosysterr->Sumw2();
  ratiosysterr->SetMarkerSize(0);
  ratiosysterr->SetFillColor(kYellow-4);
  //ratiosysterr->SetFillStyle(3002);
  ratiosysterr->SetFillStyle(1001);
  for (Int_t i = 0; i < hmc_exp->GetNbinsX()+2; i++) {
    if (hmc_exp->GetBinContent(i) > 1e-6) { //< not empty
      double binerror2 = (pow(hmc_exp->GetBinError(i), 2) +
			  pow(httbar->GetBinContent(i), 2) +
			  pow(hqcd->GetBinContent(i), 2) +
			  pow(hznn->GetBinContent(i), 2) +
			  pow(hwjets->GetBinContent(i), 2) +
			  pow(hsingle_top->GetBinContent(i), 2) +
			  pow(hother->GetBinContent(i), 2)
			  );
      double binerror = sqrt(binerror2);
      ratiosysterr->SetBinError(i, binerror / hmc_exp->GetBinContent(i));
    }
  }

 
  // Setup legends
  TLegend * leg1 = new TLegend(0.48, 0.68, 0.72, 0.92);
  set_style(leg1,0.035);
  if (plotData) leg1->AddEntry(hdata_obs, "Data", "pel");
  leg1->AddEntry(httbar, "t#bar{t}", "f");
  leg1->AddEntry(hqcd, "QCD", "f");
  leg1->AddEntry(hznn, "Z+jets", "f");
  // if (plotSig) {
  //   leg1->AddEntry(ht1bbbb_1500_100, "#splitline{T1bbbb}{(1500,100) GeV}", "l");
  //   leg1->AddEntry(ht1bbbb_1000_900, "#splitline{T1bbbb}{(1000,900) GeV}", "l");
  //   leg1->AddEntry(ht1tttt_1500_100, "#splitline{T1tttt}{(1500,100) GeV}", "l");
  //   leg1->AddEntry(ht1tttt_1200_800, "#splitline{T1tttt}{(1200,800) GeV}", "l");
  // }
 

  TLegend * leg2 = new TLegend(0.72, 0.68, 0.94, 0.92);
  set_style(leg2,0.035);


  leg2->AddEntry(hwjets, "W+jets", "f");
  leg2->AddEntry(hsingle_top, "Single Top", "f");
  leg2->AddEntry(hother, "Other", "f");
  leg2->AddEntry(staterr, "MC uncert.", "f");
    
  TLegend * ratioleg = new TLegend(0.72, 0.88, 0.94, 0.96);
  set_style(ratioleg);
  ratioleg->SetTextSize(0.07);
  ratioleg->AddEntry(ratiostaterr, "MC uncert. (stat)", "f");

  double ymax = hs->GetMaximum();
  if (hdata_obs->GetMaximum()>ymax) ymax=hdata_obs->GetMaximum();


  if(plotLog) {
    hdummy->SetMaximum(100*ymax);
    hdummy->SetMinimum(0.1);
  }
  else {
    hdummy->SetMaximum(1.5*ymax);
  }
  // Vertical lines for cuts
  //  TLine* line_low = new TLine(cut_low,0,cut_low,1.5*ymax);
  //  TLine* line_high = new TLine(cut_high,0,cut_high,1.5*ymax);
  //  set_style(line_low);
  //  set_style(line_high);

  // Setup canvas and pads  
  TCanvas * c1 = new TCanvas("c1", "c1", 700, 700);
  TPad * pad1 = new TPad("pad1", "top pad" , 0.0, 0.3, 1.0, 1.0);
  pad1->SetBottomMargin(0.0);
  pad1->Draw();
  TPad * pad2 = new TPad("pad2", "bottom pad", 0.0, 0.0, 1.0, 0.3);
  pad2->SetTopMargin(0.0);
  pad2->SetBottomMargin(0.35);
  pad2->Draw();
  pad1->cd();
  pad1->SetLogy(plotLog);
  // Draw hists
  cout << "Draw hists..." << endl;
  hdummy->Draw();
  hdummy->GetXaxis()->SetLabelSize(0.03);
  hdummy->GetYaxis()->SetLabelSize(0.03);
  hs->Draw("hist,same");
  //  TString full_title(hdummy->GetTitle()+";"+httbar->GetXaxis()->GetTitle()+";"+ytitle);
  //  hs->SetTitle(full_title);
  // hs->GetXaxis()->SetTitle(httbar->GetXaxis()->GetTitle());
  // hs->GetYaxis()->SetTitle(ytitle);
  // hs->GetXaxis()->SetLabelSize(0.03);
  // hs->GetYaxis()->SetLabelSize(0.03);
  staterr->Draw("e2 same");

  // Draw legends
  leg1->Draw();
  leg2->Draw();
  TLatex * latex = new TLatex();
  latex->SetNDC();
  latex->SetTextAlign(12);
  latex->SetTextFont(62);
  latex->SetTextSize(0.042);
  latex->DrawLatex(0.19, 0.89, "CMS Internal");
  latex->SetTextSize(0.03);
  latex->DrawLatex(0.19, 0.84, "#sqrt{s} = 8 TeV, L = 19.4 fb^{-1}");

  //    TLegend * leg2 = new TLegend(0.72, 0.68, 0.94, 0.92);
  TLatex * nbLabel = new TLatex();
  nbLabel->SetNDC();
  nbLabel->SetTextAlign(12);
  nbLabel->SetTextFont(62);
  nbLabel->SetTextSize(0.062);
  TString s_nbLabel("");
  if (s_btag_cut.Contains("==0")) s_nbLabel="N_{b-jets}=0";
  else if (s_btag_cut.Contains("==1")) s_nbLabel="N_{b-jets}=1";
  else if (s_btag_cut.Contains("==2")) s_nbLabel="N_{b-jets}=2";
  else if (s_btag_cut.Contains(">=3")) s_nbLabel="N_{b-jets}#geq3";
  if (s_nbLabel.Length()>0) nbLabel->DrawLatex(0.77, 0.60, s_nbLabel);

  TPaveText * pave = new TPaveText(0.18, 0.86, 0.28, 0.96, "brNDC");
  pave->SetLineColor(0);
  pave->SetFillColor(0);
  pave->SetShadowColor(0);
  pave->SetBorderSize(1);
  cout << "Plot data..." << endl;
  if (plotData) hdata_obs->Draw("e1 same");
  hdummy->GetXaxis()->SetLabelSize(0);
  // Draw ratio
  pad2->cd();
  pad2->SetGridy(0);
  ratiostaterr->Draw("e2");
  //ratiosysterr->Draw("e2 same");
  ratiostaterr->Draw("e2 same");
  ratiounity->Draw();
  ratio->Draw("e1 same");
  ratioleg->Draw();
  // Kolmogorov-Smirnov test and Chi2 test
     
  // double nchisq = hdata_obs->Chi2Test(hmc_exp, "UWCHI2/NDF"); // MC uncert. (stat)
  // //double kolprob = hdata_obs->KolmogorovTest(hmc_exp); // MC uncert. (stat)
  // TText * text = pave->AddText(Form("#chi_{#nu}^{2} = %.3f", nchisq));
  // //TText * text = pave->AddText(Form("#chi_{#nu}^{2} = %.3f, K_{s} = %.3f", nchisq, kolprob));
  // text->SetTextFont(62);
  // text->SetTextSize(0.07);
  // text->SetTextSize(0.06);
  if (plotData) pave->Draw();
  pad1->cd();
  gPad->RedrawAxis();
  gPad->Modified();
  gPad->Update();
  pad2->cd();
  gPad->RedrawAxis();
  gPad->Modified();
  gPad->Update();

  c1->cd();
  
  // if (plotSig) {
  //   ht1bbbb_1500_100->SetLineColor(2);
  //   ht1bbbb_1500_100->SetLineWidth(4);
  //   ht1bbbb_1500_100->SetFillColor(0);
  //   ht1bbbb_1000_900->SetLineColor(2);
  //   ht1bbbb_1000_900->SetLineWidth(4);
  //   ht1bbbb_1000_900->SetLineStyle(7);
  //   ht1bbbb_1000_900->SetFillColor(0);
  //   ht1tttt_1500_100->SetLineColor(kGreen);
  //   ht1tttt_1500_100->SetLineWidth(4);
  //   ht1tttt_1500_100->SetFillColor(0);
  //   ht1tttt_1200_800->SetLineColor(kGreen);
  //   ht1tttt_1200_800->SetLineStyle(7);
  //   ht1tttt_1200_800->SetLineWidth(4);
  //   ht1tttt_1200_800->SetFillColor(0);
  //   ht1bbbb_1500_100->Draw("hist same");
  //   ht1bbbb_1000_900->Draw("hist same");
  //   ht1tttt_1500_100->Draw("hist same");
  //   ht1tttt_1200_800->Draw("hist same");
  // }
  // if (cut_low>0) line_low->Draw("same");
  // if (cut_high>0) line_high->Draw("same");



  
  // Print
  cout << "MakePlots(): Printing..." << endl;

  c1->cd();
  if (plot_title.EqualTo("default")) plot_title=plotdir+var;
  gPad->Print(plotdir+plot_title+".pdf");


  // Clean up

  delete httbar;
  delete hqcd;
  delete hznn;
  delete hwjets;
  delete hsingle_top;
  delete hother;
  delete hdata_obs;
  delete hmc_exp;
  delete hdummy;

  // delete ht1bbbb_1500_100;
  // delete ht1bbbb_1000_900;
  // delete ht1tttt_1500_100;
  // delete ht1tttt_1200_800;

  delete staterr;
  delete ratio;
  delete ratiostaterr;
  delete ratiosysterr;
  delete leg1;
  delete leg2;
  delete ratioleg;
  delete latex;
  delete pave;
  delete hs;
  delete pad1;
  delete pad2;
  delete c1;

  cout << "MakePlots(): DONE!" << endl;

  return;
}

int main() {

  TH1::SetDefaultSumw2(1);
  //gROOT->SetBatch(1);


  if (gSystem->AccessPathName(plotdir))
    gSystem->mkdir(plotdir);

  // Setup style
  cout << "Setting tdr style."  << endl;
  TStyle *tdrStyle = new TStyle("tdrStyle","Style for P-TDR");
  setTDRStyle(tdrStyle);
  tdrStyle->cd();

  ttbar_ch = new TChain("reduced_tree");
  qcd_ch = new TChain("reduced_tree");
  znn_ch = new TChain("reduced_tree");
  wjets_ch = new TChain("reduced_tree");
  single_top_ch = new TChain("reduced_tree");
  other_ch = new TChain("reduced_tree");
  data_obs_ch = new TChain("reduced_tree");
  // TFileCollection* ttbar_fc = new TFileCollection("ttbar_fc","","../../reduced_trees/tfcs/8TeV/ttbar.txt");
  // TFileCollection* qcd_fc = new TFileCollection("qcd_fc","","../../reduced_trees/tfcs/8TeV/qcd.txt");
  // TFileCollection* znn_fc = new TFileCollection("znn_fc","","../../reduced_trees/tfcs/8TeV/znn.txt");
  // TFileCollection* wjets_fc = new TFileCollection("wjets_fc","","../../reduced_trees/tfcs/8TeV/wjets.txt");
  // TFileCollection* single_top_fc = new TFileCollection("single_top_fc","","../../reduced_trees/tfcs/8TeV/single_top.txt");
  // TFileCollection* other_fc = new TFileCollection("other_fc","","../../reduced_trees/tfcs/8TeV/other.txt");
  
  // ttbar_ch->AddFileInfoList((TCollection*)ttbar_fc->GetList());
  // qcd_ch->AddFileInfoList((TCollection*)qcd_fc->GetList());
  // znn_ch->AddFileInfoList((TCollection*)znn_fc->GetList());
  // wjets_ch->AddFileInfoList((TCollection*)wjets_fc->GetList());
  // single_top_ch->AddFileInfoList((TCollection*)single_top_fc->GetList());
  // other_ch->AddFileInfoList((TCollection*)other_fc->GetList());
   


  // normal skim
  ttbar_ch->Add("../../reduced_trees/8TeV/skimmed/TT*.root");
  qcd_ch->Add("../../reduced_trees/8TeV/skimmed/QCD_HT*.root"); 
  znn_ch->Add("../../reduced_trees/8TeV/skimmed/ZJets*.root");
  wjets_ch->Add("../../reduced_trees/8TeV/skimmed/W*Jets*.root");
  single_top_ch->Add("../../reduced_trees/8TeV/skimmed/T_*.root");
  single_top_ch->Add("../../reduced_trees/8TeV/skimmed/TBar_*.root");
  other_ch->Add("../../reduced_trees/8TeV/skimmed/WH*.root");
  data_obs_ch->Add("../../reduced_trees/8TeV/skimmed/MET*.root");
  data_obs_ch->Add("../../reduced_trees/8TeV/skimmed/HT*.root");
  data_obs_ch->Add("../../reduced_trees/8TeV/skimmed/Jet*.root");
  remove_duplicate_events(data_obs_ch);


  cout << "Entries in chains..." << endl;
  cout << "ttbar N=" << ttbar_ch->GetEntries() << endl;
  cout << "qcd N=" << qcd_ch->GetEntries() << endl;
  cout << "znn N=" << znn_ch->GetEntries() << endl;
  cout << "wjets N=" << wjets_ch->GetEntries() << endl;
  cout << "single_top N=" << single_top_ch->GetEntries() << endl;
  cout << "other N=" << other_ch->GetEntries() << endl;
  cout << "data_obs N=" << data_obs_ch->GetEntries() << endl;

  // quick skim
  TCut trigger("passesJSONCut&&passesPVCut&&passes2012METCleaningCut");
  TCut baseline("met>150&&ht50>500&&jet2_pt>70&&jet3_pt>50&&num_csvm_jets>=0&&num_reco_veto_electrons==0&&num_reco_veto_muons==0&&min_delta_phi_met_N_2012>4&&num_iso_tracks_pt15==0");
  TCut SL("met>150&&ht50>500&&jet2_pt>70&&jet3_pt>50&&num_csvm_jets>=0&&min_delta_phi_met_N_2012>4&&(num_reco_veto_muons+num_reco_veto_electrons==1)&&((num_reco_veto_muons==1&&mT_mu<100)||(num_reco_veto_electrons==1&&mT_el<100))");
  TCut SL_sb("met>125&&ht50>400&&jet2_pt>70&&jet3_pt>50&&num_csvm_jets>=0&&min_delta_phi_met_N_2012>4&&(num_reco_veto_muons+num_reco_veto_electrons==1)&&((num_reco_veto_muons==1&&mT_mu<100)||(num_reco_veto_electrons==1&&mT_el<100))");
  //  TCut jae("met>100&&ht40>500&&jet2_pt>70&&jet3_pt>50&&num_csvm_jets>=1&&min_delta_phi_met_N_2012>4&&(num_reco_veto_muons+num_reco_veto_electrons==1)&&((num_reco_veto_muons==1&&mT_mu<100)||(num_reco_veto_electrons==1&&mT_el<100))");
  TCut LDP("met>150&&ht50>500&&jet2_pt>70&&jet3_pt>50&&num_csvm_jets>=0&&num_reco_veto_electrons==0&&num_reco_veto_muons==0&&min_delta_phi_met_N_2012<4&&num_iso_tracks_pt15==0");
  TCut start("met>200&&fatpT30_MJ>400&&jet3_pt>50");
  TCut jack("fatpT30_MJ>400&&met>200&&jet3_pt>50&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_csvm_jets>=2");
  TCut baselineHT("met>400&&ht30>1000&&num_csvm_jets30>1&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>=4");
  TCut baselineMJ("met>400&&fatpT30_MJ>300&&num_csvm_jets30>1&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>=4");
  TCut search(jack+"met>200&&min_delta_phi_met_N>4&&num_reco_veto_electrons==0&&num_reco_veto_muons==0&&num_csvm_jets30>=2");
  TCut lepVeto("num_reco_veto_electrons==0&&num_reco_veto_muons==0");
  TCut qcd_control("ht50>400&&jet1_pt>70&jet2_pt>70&&jet3_pt>50&&min_delta_phi_met_N<4&&num_reco_veto_electrons==0&&num_reco_veto_muons==0");

  TCut ZL_tight("met>250&&ht50>500&&jet2_pt>70&&jet4_pt>50&&num_csvm_jets>=0&&num_reco_veto_electrons==0&&num_reco_veto_muons==0&&min_delta_phi_met_N_2012>4&&num_iso_tracks_pt15==0");


  /*draw_from_trees(TString var, TCut other_cuts,
    TString weights, TString title, int nbinsx, 
    double xlow, double xup,
    TString options="plotSig:plotLog:plotData",
    double cut_low=-1, double cut_high=-1,
    TString plot_title="default")
  */

  
  TCut AN_baseline("met>200&&ht40>750&&jet4_pt>40&&num_reco_veto_electrons==0&&num_reco_veto_muons==0&&min_delta_phi_met_N>4");
  TCut AN_LDP("met>200&&ht40>750&&jet4_pt>40&&num_reco_veto_electrons==0&&num_reco_veto_muons==0&&min_delta_phi_met_N<4");

  TCut AN_Zrich("met>500&&ht40>750&&jet4_pt>40&&num_reco_veto_electrons==0&&num_reco_veto_muons==0&&num_iso_tracks_pt15_mT==0&&min_delta_phi_met_N>4");
  TCut LL("met>250&&ht50>500&&jet3_pt>50&&num_reco_veto_electrons==0&&num_reco_veto_muons==0&&min_delta_phi_met_N_2012>4&&num_iso_tracks_pt15_mT==1");



 

  // Search region

  draw_from_trees("min_delta_phi_met_N",trigger+"met>200&&ht40>750&&jet4_pt>40&&num_reco_veto_electrons==0&&num_reco_veto_muons==0","num_csvm_jets40==0",
    		  ";#Delta#Phi^{min}_{N};Events / 20 fb^{-1}", 20, 0., 40,
   		  "plotData",
    		  "min_delta_phi_met_N_0b_127_QCD_fudged");
  draw_from_trees("min_delta_phi_met_N",trigger+"met>200&&ht40>750&&jet4_pt>40&&num_reco_veto_electrons==0&&num_reco_veto_muons==0","num_csvm_jets40==1",
    		  ";#Delta#Phi^{min}_{N};Events / 20 fb^{-1}", 20, 0., 40,
   		  "plotData",
    		  "min_delta_phi_met_N_1b_127_QCD_fudged");
  draw_from_trees("min_delta_phi_met_N",trigger+"met>200&&ht40>750&&jet4_pt>40&&num_reco_veto_electrons==0&&num_reco_veto_muons==0","num_csvm_jets40==2",
    		  ";#Delta#Phi^{min}_{N};Events / 20 fb^{-1}", 20, 0., 40,
   		  "plotData",
    		  "min_delta_phi_met_N_2b_127_QCD_fudged");
  draw_from_trees("min_delta_phi_met_N",trigger+"met>200&&ht40>750&&jet4_pt>40&&num_reco_veto_electrons==0&&num_reco_veto_muons==0","num_csvm_jets40>=3",
    		  ";#Delta#Phi^{min}_{N};Events / 20 fb^{-1}", 20, 0., 40,
   		  "plotData",
    		  "min_delta_phi_met_N_geq3b_127_QCD_fudged");
  draw_from_trees("num_csvm_jets40",trigger+AN_baseline,"",
  		  ";N_{b-jets} (p_{T} > 40 GeV);Events / 5 fb^{-1}", 4, 0, 4,
  		  "plotData",
  		  "num_csvm_jets40_baseline_127_QCD_fudged");
  draw_from_trees("num_csvm_jets40",trigger+AN_baseline,"",
  		  ";N_{b-jets} (p_{T} > 40 GeV);Events / 5 fb^{-1}", 4, 0, 4,
  		  "plotData",
  		  "num_csvm_jets40_baseline_127_QCD_raw");
  draw_from_trees("ht40",trigger+AN_baseline,"num_csvm_jets40==0",
  		  ";H_{T} [GeV];Events / 5 fb^{-1}", 14, 750, 2150,
  		  "plotData",
  		  "ht40_0b_baseline_127_QCD_fudged");
  draw_from_trees("ht40",trigger+AN_baseline,"num_csvm_jets40==1",
  		  ";H_{T} [GeV];Events / 5 fb^{-1}", 14, 750, 2150,
  		  "plotData",
  		  "ht40_1b_baseline_127_QCD_fudged");
  draw_from_trees("ht40",trigger+AN_baseline,"num_csvm_jets40==2",
  		  ";H_{T} [GeV];Events / 5 fb^{-1}", 14, 750, 2150,
  		  "plotData",
  		  "ht40_2b_baseline_127_QCD_fudged");
  draw_from_trees("ht40",trigger+AN_baseline,"num_csvm_jets40>=3",
  		  ";H_{T} [GeV];Events / 5 fb^{-1}", 14, 750, 2150,
  		  "plotData",
  		  "ht40_geq3b_baseline_127_QCD_fudged");
  draw_from_trees("met",trigger+AN_baseline,"num_csvm_jets40==0",
  		  ";E_{T}^{miss} [GeV];Events / 5 fb^{-1}", 12, 200, 500,
  		  "plotData",
  		  "met_0b_baseline_127_QCD_fudged");
  draw_from_trees("met",trigger+AN_baseline,"num_csvm_jets40==1",
  		  ";E_{T}^{miss} [GeV];Events / 5 fb^{-1}", 12, 200, 500,
  		  "plotData",
  		  "met_1b_baseline_127_QCD_fudged");
  draw_from_trees("met",trigger+AN_baseline,"num_csvm_jets40==2",
  		  ";E_{T}^{miss} [GeV];Events / 5 fb^{-1}", 12, 200, 500,
  		  "plotData",
  		  "met_2b_baseline_127_QCD_fudged");
  draw_from_trees("met",trigger+AN_baseline,"num_csvm_jets40>=3",
  		  ";E_{T}^{miss} [GeV];Events / 5 fb^{-1}", 12, 200, 500,
  		  "plotData",
  		  "met_geq3b_baseline_127_QCD_fudged");
  draw_from_trees("num_jets_pt40",trigger+AN_baseline,"num_csvm_jets40==0",
  		  ";N_{jets (R=0.5)} (p_{T} > 40 GeV);Events / 5 fb^{-1}", 5, 4, 9,
  		  "plotData",
  		  "num_jets_pt40_0b_baseline_127_QCD_fudged");
  draw_from_trees("num_jets_pt40",trigger+AN_baseline,"num_csvm_jets40==1",
  		  ";N_{jets (R=0.5)} (p_{T} > 40 GeV);Events / 5 fb^{-1}", 5, 4, 9,
  		  "plotData",
  		  "num_jets_pt40_1b_baseline_127_QCD_fudged");
  draw_from_trees("num_jets_pt40",trigger+AN_baseline,"num_csvm_jets40==2",
  		  ";N_{jets (R=0.5)} (p_{T} > 40 GeV);Events / 5 fb^{-1}", 5, 4, 9,
  		  "plotData",
  		  "num_jets_pt40_2b_baseline_127_QCD_fudged");
  draw_from_trees("num_jets_pt40",trigger+AN_baseline,"num_csvm_jets40>=3",
  		  ";N_{jets (R=0.5)} (p_{T} > 40 GeV);Events / 5 fb^{-1}", 5, 4, 9,
  		  "plotData",
  		  "num_jets_pt40_geq3b_baseline_127_QCD_fudged");

  draw_from_trees("num_fatpT30_jets",trigger+AN_baseline,"num_csvm_jets40==0",
  		  ";N_{jets (R=1.2)} (p_{T} > 50 GeV);Events / 5 fb^{-1}", 6, 0, 6,
  		  "plotData",
  		  "num_fatpT30_jets_baseline_0b_127_QCD_fudged");
  draw_from_trees("num_fatpT30_jets",trigger+AN_baseline,"num_csvm_jets40==1",
  		  ";N_{jets (R=1.2)} (p_{T} > 50 GeV);Events / 5 fb^{-1}", 6, 0, 6,
  		  "plotData",
  		  "num_fatpT30_jets_baseline_1b_127_QCD_fudged");
  draw_from_trees("num_fatpT30_jets",trigger+AN_baseline,"num_csvm_jets40==2",
  		  ";N_{jets (R=1.2)} (p_{T} > 50 GeV);Events / 5 fb^{-1}", 6, 0, 6,
  		  "plotData",
  		  "num_fatpT30_jets_baseline_2b_127_QCD_fudged");
  draw_from_trees("num_fatpT30_jets",trigger+AN_baseline,"num_csvm_jets40>=3",
  		  ";N_{jets (R=1.2)} (p_{T} > 50 GeV);Events / 5 fb^{-1}", 6, 0, 6,
  		  "plotData",
  		  "num_fatpT30_jets_baseline_geq3b_127_QCD_fudged");
  draw_from_trees("fatpT30_MJ",trigger+AN_baseline,"num_csvm_jets40==0",
  		  ";#Sigmam_{J} [GeV];Events / 5 fb^{-1}", 12, 0, 600,
  		  "plotData",
  		  "fatpT30_MJ_0b_baseline_127_QCD_fudged");
  draw_from_trees("fatpT30_MJ",trigger+AN_baseline,"num_csvm_jets40==1",
  		  ";#Sigmam_{J} [GeV];Events / 5 fb^{-1}", 12, 0, 600,
  		  "plotData",
  		  "fatpT30_MJ_1b_baseline_127_QCD_fudged");
  draw_from_trees("fatpT30_MJ",trigger+AN_baseline,"num_csvm_jets40==2",
  		  ";#Sigmam_{J} [GeV];Events / 5 fb^{-1}", 12, 0, 600,
  		  "plotData",
  		  "fatpT30_MJ_2b_baseline_127_QCD_fudged");
  draw_from_trees("fatpT30_MJ",trigger+AN_baseline,"num_csvm_jets40>=3",
  		  ";#Sigmam_{J} [GeV];Events / 5 fb^{-1}", 12, 0, 600,
  		  "plotData",
  		  "fatpT30_MJ_geq3b_baseline_127_QCD_fudged");
   draw_from_trees("highest_mJ",trigger+AN_baseline,"num_csvm_jets40==0",
  		  ";m_{J,1} [GeV];Events / 5 fb^{-1}", 12, 0, 600,
  		  "plotData",
  		  "highest_mJ_0b_baseline_127_QCD_fudged");
  draw_from_trees("highest_mJ",trigger+AN_baseline,"num_csvm_jets40==1",
  		  ";m_{J,1} [GeV];Events / 5 fb^{-1}", 12, 0, 600,
  		  "plotData",
  		  "highest_mJ_1b_baseline_127_QCD_fudged");
  draw_from_trees("highest_mJ",trigger+AN_baseline,"num_csvm_jets40==2",
  		  ";m_{J,1} [GeV];Events / 5 fb^{-1}", 12, 0, 600,
  		  "plotData",
  		  "highest_mJ_2b_baseline_127_QCD_fudged");
  draw_from_trees("highest_mJ",trigger+AN_baseline,"num_csvm_jets40>=3",
  		  ";m_{J,1} [GeV];Events / 5 fb^{-1}", 12, 0, 600,
  		  "plotData",
  		  "highest_mJ_geq3b_baseline_127_QCD_fudged");
  draw_from_trees("scnd_highest_mJ",trigger+AN_baseline,"num_csvm_jets40==0",
  		  ";m_{J,2} [GeV];Events / 5 fb^{-1}", 12, 0, 600,
  		  "plotData",
  		  "scnd_highest_mJ_0b_baseline_127_QCD_fudged");
  draw_from_trees("scnd_highest_mJ",trigger+AN_baseline,"num_csvm_jets40==1",
  		  ";m_{J,2} [GeV];Events / 5 fb^{-1}", 12, 0, 600,
  		  "plotData",
  		  "scnd_highest_mJ_1b_baseline_127_QCD_fudged");
  draw_from_trees("scnd_highest_mJ",trigger+AN_baseline,"num_csvm_jets40==2",
  		  ";m_{J,2} [GeV];Events / 5 fb^{-1}", 12, 0, 600,
  		  "plotData",
  		  "scnd_highest_mJ_2b_baseline_127_QCD_fudged");
  draw_from_trees("scnd_highest_mJ",trigger+AN_baseline,"num_csvm_jets40>=3",
  		  ";m_{J,2} [GeV];Events / 5 fb^{-1}", 12, 0, 600,
  		  "plotData",
  		  "scnd_highest_mJ_geq3b_baseline_127_QCD_fudged");
  draw_from_trees("thrd_highest_mJ",trigger+AN_baseline,"num_csvm_jets40==0",
  		  ";m_{J,3} [GeV];Events / 5 fb^{-1}", 12, 0, 600,
  		  "plotData",
  		  "thrd_highest_mJ_0b_baseline_127_QCD_fudged");
  draw_from_trees("thrd_highest_mJ",trigger+AN_baseline,"num_csvm_jets40==1",
  		  ";m_{J,3} [GeV];Events / 5 fb^{-1}", 12, 0, 600,
  		  "plotData",
  		  "thrd_highest_mJ_1b_baseline_127_QCD_fudged");
  draw_from_trees("thrd_highest_mJ",trigger+AN_baseline,"num_csvm_jets40==2",
  		  ";m_{J,3} [GeV];Events / 5 fb^{-1}", 12, 0, 600,
  		  "plotData",
  		  "thrd_highest_mJ_2b_baseline_127_QCD_fudged");
  draw_from_trees("thrd_highest_mJ",trigger+AN_baseline,"num_csvm_jets40>=3",
  		  ";m_{J,3} [GeV];Events / 5 fb^{-1}", 12, 0, 600,
  		  "plotData",
  		  "thrd_highest_mJ_geq3b_baseline_127_QCD_fudged");
  draw_from_trees("frth_highest_mJ",trigger+AN_baseline,"num_csvm_jets40==0",
  		  ";m_{J,4} [GeV];Events / 5 fb^{-1}", 12, 0, 600,
  		  "plotData",
  		  "frth_highest_mJ_0b_baseline_127_QCD_fudged");
  draw_from_trees("frth_highest_mJ",trigger+AN_baseline,"num_csvm_jets40==1",
  		  ";m_{J,4} [GeV];Events / 5 fb^{-1}", 12, 0, 600,
  		  "plotData",
  		  "frth_highest_mJ_1b_baseline_127_QCD_fudged");
  draw_from_trees("frth_highest_mJ",trigger+AN_baseline,"num_csvm_jets40==2",
  		  ";m_{J,4} [GeV];Events / 5 fb^{-1}", 12, 0, 600,
  		  "plotData",
  		  "frth_highest_mJ_2b_baseline_127_QCD_fudged");
  draw_from_trees("frth_highest_mJ",trigger+AN_baseline,"num_csvm_jets40>=3",
  		  ";m_{J,4} [GeV];Events / 5 fb^{-1}", 12, 0, 600,
  		  "plotData",
  		  "frth_highest_mJ_geq3b_baseline_127_QCD_fudged");
   draw_from_trees("fatpT30_jet1_pt",trigger+AN_baseline,"num_csvm_jets40==0",
  		  ";p_{T,1} [GeV];Events / 5 fb^{-1}", 20, 0, 1000,
  		  "plotData",
  		  "fatpT30_jet1_pt_0b_baseline_127_QCD_fudged");
  draw_from_trees("fatpT30_jet1_pt",trigger+AN_baseline,"num_csvm_jets40==1",
  		  ";p_{T,1} [GeV];Events / 5 fb^{-1}", 20, 0, 1000,
  		  "plotData",
  		  "fatpT30_jet1_pt_1b_baseline_127_QCD_fudged");
  draw_from_trees("fatpT30_jet1_pt",trigger+AN_baseline,"num_csvm_jets40==2",
  		  ";p_{T,1} [GeV];Events / 5 fb^{-1}", 20, 0, 1000,
  		  "plotData",
  		  "fatpT30_jet1_pt_2b_baseline_127_QCD_fudged");
  draw_from_trees("fatpT30_jet1_pt",trigger+AN_baseline,"num_csvm_jets40>=3",
  		  ";p_{T,1} [GeV];Events / 5 fb^{-1}", 20, 0, 1000,
  		  "plotData",
  		  "fatpT30_jet1_pt_geq3b_baseline_127_QCD_fudged");
  draw_from_trees("fatpT30_jet2_pt",trigger+AN_baseline,"num_csvm_jets40==0",
  		  ";p_{T,2} [GeV];Events / 5 fb^{-1}", 20, 0, 1000,
  		  "plotData",
  		  "fatpT30_jet2_pt_0b_baseline_127_QCD_fudged");
  draw_from_trees("fatpT30_jet2_pt",trigger+AN_baseline,"num_csvm_jets40==1",
  		  ";p_{T,2} [GeV];Events / 5 fb^{-1}", 20, 0, 1000,
  		  "plotData",
  		  "fatpT30_jet2_pt_1b_baseline_127_QCD_fudged");
  draw_from_trees("fatpT30_jet2_pt",trigger+AN_baseline,"num_csvm_jets40==2",
  		  ";p_{T,2} [GeV];Events / 5 fb^{-1}", 20, 0, 1000,
  		  "plotData",
  		  "fatpT30_jet2_pt_2b_baseline_127_QCD_fudged");
  draw_from_trees("fatpT30_jet2_pt",trigger+AN_baseline,"num_csvm_jets40>=3",
  		  ";p_{T,2} [GeV];Events / 5 fb^{-1}", 20, 0, 1000,
  		  "plotData",
  		  "fatpT30_jet2_pt_geq3b_baseline_127_QCD_fudged");
  draw_from_trees("fatpT30_jet3_pt",trigger+AN_baseline,"num_csvm_jets40==0",
  		  ";p_{T,3} [GeV];Events / 5 fb^{-1}", 20, 0, 1000,
  		  "plotData",
  		  "fatpT30_jet3_pt_0b_baseline_127_QCD_fudged");
  draw_from_trees("fatpT30_jet3_pt",trigger+AN_baseline,"num_csvm_jets40==1",
  		  ";p_{T,3} [GeV];Events / 5 fb^{-1}", 20, 0, 1000,
  		  "plotData",
  		  "fatpT30_jet3_pt_1b_baseline_127_QCD_fudged");
  draw_from_trees("fatpT30_jet3_pt",trigger+AN_baseline,"num_csvm_jets40==2",
  		  ";p_{T,3} [GeV];Events / 5 fb^{-1}", 20, 0, 1000,
  		  "plotData",
  		  "fatpT30_jet3_pt_2b_baseline_127_QCD_fudged");
  draw_from_trees("fatpT30_jet3_pt",trigger+AN_baseline,"num_csvm_jets40>=3",
  		  ";p_{T,3} [GeV];Events / 5 fb^{-1}", 20, 0, 1000,
  		  "plotData",
  		  "fatpT30_jet3_pt_geq3b_baseline_127_QCD_fudged");
  draw_from_trees("fatpT30_jet4_pt",trigger+AN_baseline,"num_csvm_jets40==0",
  		  ";p_{T,4} [GeV];Events / 5 fb^{-1}", 20, 0, 1000,
  		  "plotData",
  		  "fatpT30_jet4_pt_0b_baseline_127_QCD_fudged");
  draw_from_trees("fatpT30_jet4_pt",trigger+AN_baseline,"num_csvm_jets40==1",
  		  ";p_{T,4} [GeV];Events / 5 fb^{-1}", 20, 0, 1000,
  		  "plotData",
  		  "fatpT30_jet4_pt_1b_baseline_127_QCD_fudged");
  draw_from_trees("fatpT30_jet4_pt",trigger+AN_baseline,"num_csvm_jets40==2",
  		  ";p_{T,4} [GeV];Events / 5 fb^{-1}", 20, 0, 1000,
  		  "plotData",
  		  "fatpT30_jet4_pt_2b_baseline_127_QCD_fudged");
  draw_from_trees("fatpT30_jet4_pt",trigger+AN_baseline,"num_csvm_jets40>=3",
  		  ";p_{T,4} [GeV];Events / 5 fb^{-1}", 20, 0, 1000,
  		  "plotData",
  		  "fatpT30_jet4_pt_geq3b_baseline_127_QCD_fudged");
  draw_from_trees("fatpT30_jet1_eta",trigger+AN_baseline,"num_csvm_jets40==0",
  		  ";#eta_{1};Events / 5 fb^{-1}", 16, -3.2, 3.2,
  		  "plotData",
  		  "fatpT30_jet1_eta_0b_baseline_127_QCD_fudged");
  draw_from_trees("fatpT30_jet1_eta",trigger+AN_baseline,"num_csvm_jets40==1",
  		  ";#eta_{1};Events / 5 fb^{-1}", 16, -3.2, 3.2,
  		  "plotData",
  		  "fatpT30_jet1_eta_1b_baseline_127_QCD_fudged");
  draw_from_trees("fatpT30_jet1_eta",trigger+AN_baseline,"num_csvm_jets40==2",
  		  ";#eta_{1};Events / 5 fb^{-1}", 16, -3.2, 3.2,
  		  "plotData",
  		  "fatpT30_jet1_eta_2b_baseline_127_QCD_fudged");
  draw_from_trees("fatpT30_jet1_eta",trigger+AN_baseline,"num_csvm_jets40>=3",
  		  ";#eta_{1};Events / 5 fb^{-1}", 16, -3.2, 3.2,
  		  "plotData",
  		  "fatpT30_jet1_eta_geq3b_baseline_127_QCD_fudged");
  draw_from_trees("fatpT30_jet2_eta",trigger+AN_baseline+"fatpT30_jet2_pt>0","num_csvm_jets40==0",
  		  ";#eta_{2};Events / 5 fb^{-1}", 16, -3.2, 3.2,
  		  "plotData",
  		  "fatpT30_jet2_eta_0b_baseline_127_QCD_fudged");
  draw_from_trees("fatpT30_jet2_eta",trigger+AN_baseline+"fatpT30_jet2_pt>0","num_csvm_jets40==1",
  		  ";#eta_{2};Events / 5 fb^{-1}", 16, -3.2, 3.2,
  		  "plotData",
  		  "fatpT30_jet2_eta_1b_baseline_127_QCD_fudged");
  draw_from_trees("fatpT30_jet2_eta",trigger+AN_baseline+"fatpT30_jet2_pt>0","num_csvm_jets40==2",
  		  ";#eta_{2};Events / 5 fb^{-1}", 16, -3.2, 3.2,
  		  "plotData",
  		  "fatpT30_jet2_eta_2b_baseline_127_QCD_fudged");
  draw_from_trees("fatpT30_jet2_eta",trigger+AN_baseline+"fatpT30_jet2_pt>0","num_csvm_jets40>=3",
  		  ";#eta_{2};Events / 5 fb^{-1}", 16, -3.2, 3.2,
  		  "plotData",
  		  "fatpT30_jet2_eta_geq3b_baseline_127_QCD_fudged");
  draw_from_trees("fatpT30_jet3_eta",trigger+AN_baseline+"fatpT30_jet3_pt>0","num_csvm_jets40==0",
  		  ";#eta_{3};Events / 5 fb^{-1}", 16, -3.2, 3.2,
  		  "plotData",
  		  "fatpT30_jet3_eta_0b_baseline_127_QCD_fudged");
  draw_from_trees("fatpT30_jet3_eta",trigger+AN_baseline+"fatpT30_jet3_pt>0","num_csvm_jets40==1",
  		  ";#eta_{3};Events / 5 fb^{-1}", 16, -3.2, 3.2,
  		  "plotData",
  		  "fatpT30_jet3_eta_1b_baseline_127_QCD_fudged");
  draw_from_trees("fatpT30_jet3_eta",trigger+AN_baseline+"fatpT30_jet3_pt>0","num_csvm_jets40==2",
  		  ";#eta_{3};Events / 5 fb^{-1}", 16, -3.2, 3.2,
  		  "plotData",
  		  "fatpT30_jet3_eta_2b_baseline_127_QCD_fudged");
  draw_from_trees("fatpT30_jet3_eta",trigger+AN_baseline+"fatpT30_jet3_pt>0","num_csvm_jets40>=3",
  		  ";#eta_{3};Events / 5 fb^{-1}", 16, -3.2, 3.2,
  		  "plotData",
  		  "fatpT30_jet3_eta_geq3b_baseline_127_QCD_fudged");
  draw_from_trees("fatpT30_jet4_eta",trigger+AN_baseline+"fatpT30_jet4_pt>0","num_csvm_jets40==0",
  		  ";#eta_{4};Events / 5 fb^{-1}", 16, -3.2, 3.2,
  		  "plotData",
  		  "fatpT30_jet4_eta_0b_baseline_127_QCD_fudged");
  draw_from_trees("fatpT30_jet4_eta",trigger+AN_baseline+"fatpT30_jet4_pt>0","num_csvm_jets40==1",
  		  ";#eta_{4};Events / 5 fb^{-1}", 16, -3.2, 3.2,
  		  "plotData",
  		  "fatpT30_jet4_eta_1b_baseline_127_QCD_fudged");
  draw_from_trees("fatpT30_jet4_eta",trigger+AN_baseline+"fatpT30_jet4_pt>0","num_csvm_jets40==2",
  		  ";#eta_{4};Events / 5 fb^{-1}", 16, -3.2, 3.2,
  		  "plotData",
  		  "fatpT30_jet4_eta_2b_baseline_127_QCD_fudged");
  draw_from_trees("fatpT30_jet4_eta",trigger+AN_baseline+"fatpT30_jet4_pt>0","num_csvm_jets40>=3",
  		  ";#eta_{4};Events / 5 fb^{-1}", 16, -3.2, 3.2,
  		  "plotData",
  		  "fatpT30_jet4_eta_geq3b_baseline_127_QCD_fudged");
  
  // LDP
  draw_from_trees("ht40",trigger+AN_LDP,"num_csvm_jets40==0",
  		  ";H_{T} [GeV];Events / 5 fb^{-1}", 14, 750, 2150,
  		  "plotData",
  		  "ht40_0b_LDP_127_QCD_fudged");
  draw_from_trees("ht40",trigger+AN_LDP,"num_csvm_jets40==1",
  		  ";H_{T} [GeV];Events / 5 fb^{-1}", 14, 750, 2150,
  		  "plotData",
  		  "ht40_1b_LDP_127_QCD_fudged");
  draw_from_trees("ht40",trigger+AN_LDP,"num_csvm_jets40==2",
  		  ";H_{T} [GeV];Events / 5 fb^{-1}", 14, 750, 2150,
  		  "plotData",
  		  "ht40_2b_LDP_127_QCD_fudged");
  draw_from_trees("ht40",trigger+AN_LDP,"num_csvm_jets40>=3",
  		  ";H_{T} [GeV];Events / 5 fb^{-1}", 14, 750, 2150,
  		  "plotData",
  		  "ht40_geq3b_LDP_127_QCD_fudged");
  draw_from_trees("fatpT30_jet1_pt",trigger+AN_LDP,"num_csvm_jets40==0",
  		  ";p_{T,1} [GeV];Events / 5 fb^{-1}", 20, 0, 1000,
  		  "plotData",
  		  "fatpT30_jet1_pt_0b_LDP_127_QCD_fudged");
  draw_from_trees("fatpT30_jet1_pt",trigger+AN_LDP,"num_csvm_jets40==1",
  		  ";p_{T,1} [GeV];Events / 5 fb^{-1}", 20, 0, 1000,
  		  "plotData",
  		  "fatpT30_jet1_pt_1b_LDP_127_QCD_fudged");
  draw_from_trees("fatpT30_jet1_pt",trigger+AN_LDP,"num_csvm_jets40==2",
  		  ";p_{T,1} [GeV];Events / 5 fb^{-1}", 20, 0, 1000,
  		  "plotData",
  		  "fatpT30_jet1_pt_2b_LDP_127_QCD_fudged");
  draw_from_trees("fatpT30_jet1_pt",trigger+AN_LDP,"num_csvm_jets40>=3",
  		  ";p_{T,1} [GeV];Events / 5 fb^{-1}", 20, 0, 1000,
  		  "plotData",
  		  "fatpT30_jet1_pt_geq3b_LDP_127_QCD_fudged");
  draw_from_trees("fatpT30_jet2_pt",trigger+AN_LDP,"num_csvm_jets40==0",
  		  ";p_{T,2} [GeV];Events / 5 fb^{-1}", 20, 0, 1000,
  		  "plotData",
  		  "fatpT30_jet2_pt_0b_LDP_127_QCD_fudged");
  draw_from_trees("fatpT30_jet2_pt",trigger+AN_LDP,"num_csvm_jets40==1",
  		  ";p_{T,2} [GeV];Events / 5 fb^{-1}", 20, 0, 1000,
  		  "plotData",
  		  "fatpT30_jet2_pt_1b_LDP_127_QCD_fudged");
  draw_from_trees("fatpT30_jet2_pt",trigger+AN_LDP,"num_csvm_jets40==2",
  		  ";p_{T,2} [GeV];Events / 5 fb^{-1}", 20, 0, 1000,
  		  "plotData",
  		  "fatpT30_jet2_pt_2b_LDP_127_QCD_fudged");
  draw_from_trees("fatpT30_jet2_pt",trigger+AN_LDP,"num_csvm_jets40>=3",
  		  ";p_{T,2} [GeV];Events / 5 fb^{-1}", 20, 0, 1000,
  		  "plotData",
  		  "fatpT30_jet2_pt_geq3b_LDP_127_QCD_fudged");
  draw_from_trees("fatpT30_jet3_pt",trigger+AN_LDP,"num_csvm_jets40==0",
  		  ";p_{T,3} [GeV];Events / 5 fb^{-1}", 20, 0, 1000,
  		  "plotData",
  		  "fatpT30_jet3_pt_0b_LDP_127_QCD_fudged");
  draw_from_trees("fatpT30_jet3_pt",trigger+AN_LDP,"num_csvm_jets40==1",
  		  ";p_{T,3} [GeV];Events / 5 fb^{-1}", 20, 0, 1000,
  		  "plotData",
  		  "fatpT30_jet3_pt_1b_LDP_127_QCD_fudged");
  draw_from_trees("fatpT30_jet3_pt",trigger+AN_LDP,"num_csvm_jets40==2",
  		  ";p_{T,3} [GeV];Events / 5 fb^{-1}", 20, 0, 1000,
  		  "plotData",
  		  "fatpT30_jet3_pt_2b_LDP_127_QCD_fudged");
  draw_from_trees("fatpT30_jet3_pt",trigger+AN_LDP,"num_csvm_jets40>=3",
  		  ";p_{T,3} [GeV];Events / 5 fb^{-1}", 20, 0, 1000,
  		  "plotData",
  		  "fatpT30_jet3_pt_geq3b_LDP_127_QCD_fudged");
  draw_from_trees("fatpT30_jet4_pt",trigger+AN_LDP,"num_csvm_jets40==0",
  		  ";p_{T,4} [GeV];Events / 5 fb^{-1}", 20, 0, 1000,
  		  "plotData",
  		  "fatpT30_jet4_pt_0b_LDP_127_QCD_fudged");
  draw_from_trees("fatpT30_jet4_pt",trigger+AN_LDP,"num_csvm_jets40==1",
  		  ";p_{T,4} [GeV];Events / 5 fb^{-1}", 20, 0, 1000,
  		  "plotData",
  		  "fatpT30_jet4_pt_1b_LDP_127_QCD_fudged");
  draw_from_trees("fatpT30_jet4_pt",trigger+AN_LDP,"num_csvm_jets40==2",
  		  ";p_{T,4} [GeV];Events / 5 fb^{-1}", 20, 0, 1000,
  		  "plotData",
  		  "fatpT30_jet4_pt_2b_LDP_127_QCD_fudged");
  draw_from_trees("fatpT30_jet4_pt",trigger+AN_LDP,"num_csvm_jets40>=3",
  		  ";p_{T,4} [GeV];Events / 5 fb^{-1}", 20, 0, 1000,
  		  "plotData",
  		  "fatpT30_jet4_pt_geq3b_LDP_127_QCD_fudged");
  
  draw_from_trees("highest_mJ",trigger+AN_LDP,"num_csvm_jets40==0",
  		  ";m_{J,1} [GeV];Events / 5 fb^{-1}", 12, 0, 600,
  		  "plotData",
  		  "highest_mJ_0b_LDP_127_QCD_fudged");
  draw_from_trees("highest_mJ",trigger+AN_LDP,"num_csvm_jets40==1",
  		  ";m_{J,1} [GeV];Events / 5 fb^{-1}", 12, 0, 600,
  		  "plotData",
  		  "highest_mJ_1b_LDP_127_QCD_fudged");
  draw_from_trees("highest_mJ",trigger+AN_LDP,"num_csvm_jets40==2",
  		  ";m_{J,1} [GeV];Events / 5 fb^{-1}", 12, 0, 600,
  		  "plotData",
  		  "highest_mJ_2b_LDP_127_QCD_fudged");
  draw_from_trees("highest_mJ",trigger+AN_LDP,"num_csvm_jets40>=3",
  		  ";m_{J,1} [GeV];Events / 5 fb^{-1}", 12, 0, 600,
  		  "plotData",
  		  "highest_mJ_geq3b_LDP_127_QCD_fudged");
  draw_from_trees("scnd_highest_mJ",trigger+AN_LDP,"num_csvm_jets40==0",
  		  ";m_{J,2} [GeV];Events / 5 fb^{-1}", 12, 0, 600,
  		  "plotData",
  		  "scnd_highest_mJ_0b_LDP_127_QCD_fudged");
  draw_from_trees("scnd_highest_mJ",trigger+AN_LDP,"num_csvm_jets40==1",
  		  ";m_{J,2} [GeV];Events / 5 fb^{-1}", 12, 0, 600,
  		  "plotData",
  		  "scnd_highest_mJ_1b_LDP_127_QCD_fudged");
  draw_from_trees("scnd_highest_mJ",trigger+AN_LDP,"num_csvm_jets40==2",
  		  ";m_{J,2} [GeV];Events / 5 fb^{-1}", 12, 0, 600,
  		  "plotData",
  		  "scnd_highest_mJ_2b_LDP_127_QCD_fudged");
  draw_from_trees("scnd_highest_mJ",trigger+AN_LDP,"num_csvm_jets40>=3",
  		  ";m_{J,2} [GeV];Events / 5 fb^{-1}", 12, 0, 600,
  		  "plotData",
  		  "scnd_highest_mJ_geq3b_LDP_127_QCD_fudged");
  draw_from_trees("thrd_highest_mJ",trigger+AN_LDP,"num_csvm_jets40==0",
  		  ";m_{J,3} [GeV];Events / 5 fb^{-1}", 12, 0, 600,
  		  "plotData",
  		  "thrd_highest_mJ_0b_LDP_127_QCD_fudged");
  draw_from_trees("thrd_highest_mJ",trigger+AN_LDP,"num_csvm_jets40==1",
  		  ";m_{J,3} [GeV];Events / 5 fb^{-1}", 12, 0, 600,
  		  "plotData",
  		  "thrd_highest_mJ_1b_LDP_127_QCD_fudged");
  draw_from_trees("thrd_highest_mJ",trigger+AN_LDP,"num_csvm_jets40==2",
  		  ";m_{J,3} [GeV];Events / 5 fb^{-1}", 12, 0, 600,
  		  "plotData",
  		  "thrd_highest_mJ_2b_LDP_127_QCD_fudged");
  draw_from_trees("thrd_highest_mJ",trigger+AN_LDP,"num_csvm_jets40>=3",
  		  ";m_{J,3} [GeV];Events / 5 fb^{-1}", 12, 0, 600,
  		  "plotData",
  		  "thrd_highest_mJ_geq3b_LDP_127_QCD_fudged");
  draw_from_trees("frth_highest_mJ",trigger+AN_LDP,"num_csvm_jets40==0",
  		  ";m_{J,4} [GeV];Events / 5 fb^{-1}", 12, 0, 600,
  		  "plotData",
  		  "frth_highest_mJ_0b_LDP_127_QCD_fudged");
  draw_from_trees("frth_highest_mJ",trigger+AN_LDP,"num_csvm_jets40==1",
  		  ";m_{J,4} [GeV];Events / 5 fb^{-1}", 12, 0, 600,
  		  "plotData",
  		  "frth_highest_mJ_1b_LDP_127_QCD_fudged");
  draw_from_trees("frth_highest_mJ",trigger+AN_LDP,"num_csvm_jets40==2",
  		  ";m_{J,4} [GeV];Events / 5 fb^{-1}", 12, 0, 600,
  		  "plotData",
  		  "frth_highest_mJ_2b_LDP_127_QCD_fudged");
  draw_from_trees("frth_highest_mJ",trigger+AN_LDP,"num_csvm_jets40>=3",
  		  ";m_{J,4} [GeV];Events / 5 fb^{-1}", 12, 0, 600,
  		  "plotData",
  		  "frth_highest_mJ_geq3b_LDP_127_QCD_fudged");
  
  draw_from_trees("num_fatpT30_jets",trigger+AN_LDP,"num_csvm_jets40==0",
  		  ";N_{jets (R=1.2)} (p_{T} > 50 GeV);Events / 5 fb^{-1}", 6, 0, 6,
  		  "plotData",
  		  "num_fatpT30_jets_LDP_0b_127_QCD_fudged");
  draw_from_trees("num_fatpT30_jets",trigger+AN_LDP,"num_csvm_jets40==1",
  		  ";N_{jets (R=1.2)} (p_{T} > 50 GeV);Events / 5 fb^{-1}", 6, 0, 6,
  		  "plotData",
  		  "num_fatpT30_jets_LDP_1b_127_QCD_fudged");
  draw_from_trees("num_fatpT30_jets",trigger+AN_LDP,"num_csvm_jets40==2",
  		  ";N_{jets (R=1.2)} (p_{T} > 50 GeV);Events / 5 fb^{-1}", 6, 0, 6,
  		  "plotData",
  		  "num_fatpT30_jets_LDP_2b_127_QCD_fudged");
  draw_from_trees("num_fatpT30_jets",trigger+AN_LDP,"num_csvm_jets40>=3",
  		  ";N_{jets (R=1.2)} (p_{T} > 50 GeV);Events / 5 fb^{-1}", 6, 0, 6,
  		  "plotData",
  		  "num_fatpT30_jets_LDP_geq3b_127_QCD_fudged");
    
  draw_from_trees("fatpT30_jet1_energy",trigger+AN_LDP,"num_csvm_jets40==0",
  		  ";E_{1} [GeV];Events / 5 fb^{-1}", 20, 0, 1000,
  		  "plotData",
  		  "fatpT30_jet1_energy_0b_LDP_127_QCD_fudged");
  draw_from_trees("fatpT30_jet1_energy",trigger+AN_LDP,"num_csvm_jets40==1",
  		  ";E_{1} [GeV];Events / 5 fb^{-1}", 20, 0, 1000,
  		  "plotData",
  		  "fatpT30_jet1_energy_1b_LDP_127_QCD_fudged");
  draw_from_trees("fatpT30_jet1_energy",trigger+AN_LDP,"num_csvm_jets40==2",
  		  ";E_{1} [GeV];Events / 5 fb^{-1}", 20, 0, 1000,
  		  "plotData",
  		  "fatpT30_jet1_energy_2b_LDP_127_QCD_fudged");
  draw_from_trees("fatpT30_jet1_energy",trigger+AN_LDP,"num_csvm_jets40>=3",
  		  ";E_{1} [GeV];Events / 5 fb^{-1}", 20, 0, 1000,
  		  "plotData",
  		  "fatpT30_jet1_energy_geq3b_LDP_127_QCD_fudged");
  draw_from_trees("fatpT30_jet2_energy",trigger+AN_LDP,"num_csvm_jets40==0",
  		  ";E_{2} [GeV];Events / 5 fb^{-1}", 20, 0, 1000,
  		  "plotData",
  		  "fatpT30_jet2_energy_0b_LDP_127_QCD_fudged");
  draw_from_trees("fatpT30_jet2_energy",trigger+AN_LDP,"num_csvm_jets40==1",
  		  ";E_{2} [GeV];Events / 5 fb^{-1}", 20, 0, 1000,
  		  "plotData",
  		  "fatpT30_jet2_energy_1b_LDP_127_QCD_fudged");
  draw_from_trees("fatpT30_jet2_energy",trigger+AN_LDP,"num_csvm_jets40==2",
  		  ";E_{2} [GeV];Events / 5 fb^{-1}", 20, 0, 1000,
  		  "plotData",
  		  "fatpT30_jet2_energy_2b_LDP_127_QCD_fudged");
  draw_from_trees("fatpT30_jet2_energy",trigger+AN_LDP,"num_csvm_jets40>=3",
  		  ";E_{2} [GeV];Events / 5 fb^{-1}", 20, 0, 1000,
  		  "plotData",
  		  "fatpT30_jet2_energy_geq3b_LDP_127_QCD_fudged");
  draw_from_trees("fatpT30_jet3_energy",trigger+AN_LDP,"num_csvm_jets40==0",
  		  ";E_{3} [GeV];Events / 5 fb^{-1}", 20, 0, 1000,
  		  "plotData",
  		  "fatpT30_jet3_energy_0b_LDP_127_QCD_fudged");
  draw_from_trees("fatpT30_jet3_energy",trigger+AN_LDP,"num_csvm_jets40==1",
  		  ";E_{3} [GeV];Events / 5 fb^{-1}", 20, 0, 1000,
  		  "plotData",
  		  "fatpT30_jet3_energy_1b_LDP_127_QCD_fudged");
  draw_from_trees("fatpT30_jet3_energy",trigger+AN_LDP,"num_csvm_jets40==2",
  		  ";E_{3} [GeV];Events / 5 fb^{-1}", 20, 0, 1000,
  		  "plotData",
  		  "fatpT30_jet3_energy_2b_LDP_127_QCD_fudged");
  draw_from_trees("fatpT30_jet3_energy",trigger+AN_LDP,"num_csvm_jets40>=3",
  		  ";E_{3} [GeV];Events / 5 fb^{-1}", 20, 0, 1000,
  		  "plotData",
  		  "fatpT30_jet3_energy_geq3b_LDP_127_QCD_fudged");
  draw_from_trees("fatpT30_jet4_energy",trigger+AN_LDP,"num_csvm_jets40==0",
  		  ";E_{4} [GeV];Events / 5 fb^{-1}", 20, 0, 1000,
  		  "plotData",
  		  "fatpT30_jet4_energy_0b_LDP_127_QCD_fudged");
  draw_from_trees("fatpT30_jet4_energy",trigger+AN_LDP,"num_csvm_jets40==1",
  		  ";E_{4} [GeV];Events / 5 fb^{-1}", 20, 0, 1000,
  		  "plotData",
  		  "fatpT30_jet4_energy_1b_LDP_127_QCD_fudged");
  draw_from_trees("fatpT30_jet4_energy",trigger+AN_LDP,"num_csvm_jets40==2",
  		  ";E_{4} [GeV];Events / 5 fb^{-1}", 20, 0, 1000,
  		  "plotData",
  		  "fatpT30_jet4_energy_2b_LDP_127_QCD_fudged");
  draw_from_trees("fatpT30_jet4_energy",trigger+AN_LDP,"num_csvm_jets40>=3",
  		  ";E_{4} [GeV];Events / 5 fb^{-1}", 20, 0, 1000,
  		  "plotData",
  		  "fatpT30_jet4_energy_geq3b_LDP_127_QCD_fudged");
  
  draw_from_trees("fatpT30_jet1_phi",trigger+AN_LDP+"fatpT30_jet1_pt>0","num_csvm_jets40==0",
  		  ";#phi_{1};Events / 5 fb^{-1}", 16, -3.2, 3.2,
  		  "plotData",
  		  "fatpT30_jet1_phi_0b_LDP_127_QCD_fudged");
  draw_from_trees("fatpT30_jet1_phi",trigger+AN_LDP+"fatpT30_jet1_pt>0","num_csvm_jets40==1",
  		  ";#phi_{1};Events / 5 fb^{-1}", 16, -3.2, 3.2,
  		  "plotData",
  		  "fatpT30_jet1_phi_1b_LDP_127_QCD_fudged");
  draw_from_trees("fatpT30_jet1_phi",trigger+AN_LDP+"fatpT30_jet1_pt>0","num_csvm_jets40==2",
  		  ";#phi_{1};Events / 5 fb^{-1}", 16, -3.2, 3.2,
  		  "plotData",
  		  "fatpT30_jet1_phi_2b_LDP_127_QCD_fudged");
  draw_from_trees("fatpT30_jet1_phi",trigger+AN_LDP+"fatpT30_jet1_pt>0","num_csvm_jets40>=3",
  		  ";#phi_{1};Events / 5 fb^{-1}", 16, -3.2, 3.2,
  		  "plotData",
  		  "fatpT30_jet1_phi_geq3b_LDP_127_QCD_fudged");
  draw_from_trees("fatpT30_jet2_phi",trigger+AN_LDP+"fatpT30_jet2_pt>0","num_csvm_jets40==0",
  		  ";#phi_{2};Events / 5 fb^{-1}", 16, -3.2, 3.2,
  		  "plotData",
  		  "fatpT30_jet2_phi_0b_LDP_127_QCD_fudged");
  draw_from_trees("fatpT30_jet2_phi",trigger+AN_LDP+"fatpT30_jet2_pt>0","num_csvm_jets40==1",
  		  ";#phi_{2};Events / 5 fb^{-1}", 16, -3.2, 3.2,
  		  "plotData",
  		  "fatpT30_jet2_phi_1b_LDP_127_QCD_fudged");
  draw_from_trees("fatpT30_jet2_phi",trigger+AN_LDP+"fatpT30_jet2_pt>0","num_csvm_jets40==2",
  		  ";#phi_{2};Events / 5 fb^{-1}", 16, -3.2, 3.2,
  		  "plotData",
  		  "fatpT30_jet2_phi_2b_LDP_127_QCD_fudged");
  draw_from_trees("fatpT30_jet2_phi",trigger+AN_LDP+"fatpT30_jet2_pt>0","num_csvm_jets40>=3",
  		  ";#phi_{2};Events / 5 fb^{-1}", 16, -3.2, 3.2,
  		  "plotData",
  		  "fatpT30_jet2_phi_geq3b_LDP_127_QCD_fudged");
  draw_from_trees("fatpT30_jet3_phi",trigger+AN_LDP+"fatpT30_jet3_pt>0","num_csvm_jets40==0",
  		  ";#phi_{3};Events / 5 fb^{-1}", 16, -3.2, 3.2,
  		  "plotData",
  		  "fatpT30_jet3_phi_0b_LDP_127_QCD_fudged");
  draw_from_trees("fatpT30_jet3_phi",trigger+AN_LDP+"fatpT30_jet3_pt>0","num_csvm_jets40==1",
  		  ";#phi_{3};Events / 5 fb^{-1}", 16, -3.2, 3.2,
  		  "plotData",
  		  "fatpT30_jet3_phi_1b_LDP_127_QCD_fudged");
  draw_from_trees("fatpT30_jet3_phi",trigger+AN_LDP+"fatpT30_jet3_pt>0","num_csvm_jets40==2",
  		  ";#phi_{3};Events / 5 fb^{-1}", 16, -3.2, 3.2,
  		  "plotData",
  		  "fatpT30_jet3_phi_2b_LDP_127_QCD_fudged");
  draw_from_trees("fatpT30_jet3_phi",trigger+AN_LDP+"fatpT30_jet3_pt>0","num_csvm_jets40>=3",
  		  ";#phi_{3};Events / 5 fb^{-1}", 16, -3.2, 3.2,
  		  "plotData",
  		  "fatpT30_jet3_phi_geq3b_LDP_127_QCD_fudged");
  draw_from_trees("fatpT30_jet4_phi",trigger+AN_LDP+"fatpT30_jet4_pt>0","num_csvm_jets40==0",
  		  ";#phi_{4};Events / 5 fb^{-1}", 16, -3.2, 3.2,
  		  "plotData",
  		  "fatpT30_jet4_phi_0b_LDP_127_QCD_fudged");
  draw_from_trees("fatpT30_jet4_phi",trigger+AN_LDP+"fatpT30_jet4_pt>0","num_csvm_jets40==1",
  		  ";#phi_{4};Events / 5 fb^{-1}", 16, -3.2, 3.2,
  		  "plotData",
  		  "fatpT30_jet4_phi_1b_LDP_127_QCD_fudged");
  draw_from_trees("fatpT30_jet4_phi",trigger+AN_LDP+"fatpT30_jet4_pt>0","num_csvm_jets40==2",
  		  ";#phi_{4};Events / 5 fb^{-1}", 16, -3.2, 3.2,
  		  "plotData",
  		  "fatpT30_jet4_phi_2b_LDP_127_QCD_fudged");
  draw_from_trees("fatpT30_jet4_phi",trigger+AN_LDP+"fatpT30_jet4_pt>0","num_csvm_jets40>=3",
  		  ";#phi_{4};Events / 5 fb^{-1}", 16, -3.2, 3.2,
  		  "plotData",
  		  "fatpT30_jet4_phi_geq3b_LDP_127_QCD_fudged");
  
  draw_from_trees("fatpT30_jet1_eta",trigger+AN_LDP,"num_csvm_jets40==0",
  		  ";#eta_{1};Events / 5 fb^{-1}", 16, -3.2, 3.2,
  		  "plotData",
  		  "fatpT30_jet1_eta_0b_LDP_127_QCD_fudged");
  draw_from_trees("fatpT30_jet1_eta",trigger+AN_LDP,"num_csvm_jets40==1",
  		  ";#eta_{1};Events / 5 fb^{-1}", 16, -3.2, 3.2,
  		  "plotData",
  		  "fatpT30_jet1_eta_1b_LDP_127_QCD_fudged");
  draw_from_trees("fatpT30_jet1_eta",trigger+AN_LDP,"num_csvm_jets40==2",
  		  ";#eta_{1};Events / 5 fb^{-1}", 16, -3.2, 3.2,
  		  "plotData",
  		  "fatpT30_jet1_eta_2b_LDP_127_QCD_fudged");
  draw_from_trees("fatpT30_jet1_eta",trigger+AN_LDP,"num_csvm_jets40>=3",
  		  ";#eta_{1};Events / 5 fb^{-1}", 16, -3.2, 3.2,
  		  "plotData",
  		  "fatpT30_jet1_eta_geq3b_LDP_127_QCD_fudged");
  draw_from_trees("fatpT30_jet2_eta",trigger+AN_LDP+"fatpT30_jet2_pt>0","num_csvm_jets40==0",
  		  ";#eta_{2};Events / 5 fb^{-1}", 16, -3.2, 3.2,
  		  "plotData",
  		  "fatpT30_jet2_eta_0b_LDP_127_QCD_fudged");
  draw_from_trees("fatpT30_jet2_eta",trigger+AN_LDP+"fatpT30_jet2_pt>0","num_csvm_jets40==1",
  		  ";#eta_{2};Events / 5 fb^{-1}", 16, -3.2, 3.2,
  		  "plotData",
  		  "fatpT30_jet2_eta_1b_LDP_127_QCD_fudged");
  draw_from_trees("fatpT30_jet2_eta",trigger+AN_LDP+"fatpT30_jet2_pt>0","num_csvm_jets40==2",
  		  ";#eta_{2};Events / 5 fb^{-1}", 16, -3.2, 3.2,
  		  "plotData",
  		  "fatpT30_jet2_eta_2b_LDP_127_QCD_fudged");
  draw_from_trees("fatpT30_jet2_eta",trigger+AN_LDP+"fatpT30_jet2_pt>0","num_csvm_jets40>=3",
  		  ";#eta_{2};Events / 5 fb^{-1}", 16, -3.2, 3.2,
  		  "plotData",
  		  "fatpT30_jet2_eta_geq3b_LDP_127_QCD_fudged");
  draw_from_trees("fatpT30_jet3_eta",trigger+AN_LDP+"fatpT30_jet3_pt>0","num_csvm_jets40==0",
  		  ";#eta_{3};Events / 5 fb^{-1}", 16, -3.2, 3.2,
  		  "plotData",
  		  "fatpT30_jet3_eta_0b_LDP_127_QCD_fudged");
  draw_from_trees("fatpT30_jet3_eta",trigger+AN_LDP+"fatpT30_jet3_pt>0","num_csvm_jets40==1",
  		  ";#eta_{3};Events / 5 fb^{-1}", 16, -3.2, 3.2,
  		  "plotData",
  		  "fatpT30_jet3_eta_1b_LDP_127_QCD_fudged");
  draw_from_trees("fatpT30_jet3_eta",trigger+AN_LDP+"fatpT30_jet3_pt>0","num_csvm_jets40==2",
  		  ";#eta_{3};Events / 5 fb^{-1}", 16, -3.2, 3.2,
  		  "plotData",
  		  "fatpT30_jet3_eta_2b_LDP_127_QCD_fudged");
  draw_from_trees("fatpT30_jet3_eta",trigger+AN_LDP+"fatpT30_jet3_pt>0","num_csvm_jets40>=3",
  		  ";#eta_{3};Events / 5 fb^{-1}", 16, -3.2, 3.2,
  		  "plotData",
  		  "fatpT30_jet3_eta_geq3b_LDP_127_QCD_fudged");
  draw_from_trees("fatpT30_jet4_eta",trigger+AN_LDP+"fatpT30_jet4_pt>0","num_csvm_jets40==0",
  		  ";#eta_{4};Events / 5 fb^{-1}", 16, -3.2, 3.2,
  		  "plotData",
  		  "fatpT30_jet4_eta_0b_LDP_127_QCD_fudged");
  draw_from_trees("fatpT30_jet4_eta",trigger+AN_LDP+"fatpT30_jet4_pt>0","num_csvm_jets40==1",
  		  ";#eta_{4};Events / 5 fb^{-1}", 16, -3.2, 3.2,
  		  "plotData",
  		  "fatpT30_jet4_eta_1b_LDP_127_QCD_fudged");
  draw_from_trees("fatpT30_jet4_eta",trigger+AN_LDP+"fatpT30_jet4_pt>0","num_csvm_jets40==2",
  		  ";#eta_{4};Events / 5 fb^{-1}", 16, -3.2, 3.2,
  		  "plotData",
  		  "fatpT30_jet4_eta_2b_LDP_127_QCD_fudged");
  draw_from_trees("fatpT30_jet4_eta",trigger+AN_LDP+"fatpT30_jet4_pt>0","num_csvm_jets40>=3",
  		  ";#eta_{4};Events / 5 fb^{-1}", 16, -3.2, 3.2,
  		  "plotData",
  		  "fatpT30_jet4_eta_geq3b_LDP_127_QCD_fudged");
  
 
  draw_from_trees("fatpT30_MJ",trigger+AN_LDP,"num_csvm_jets40==0",
  		  ";#Sigmam_{J} [GeV];Events / 5 fb^{-1}", 12, 0, 600,
  		  "plotData",
  		  "fatpT30_MJ_0b_LDP_127_QCD_fudged");
  draw_from_trees("fatpT30_MJ",trigger+AN_LDP,"num_csvm_jets40==1",
  		  ";#Sigmam_{J} [GeV];Events / 5 fb^{-1}", 12, 0, 600,
  		  "plotData",
  		  "fatpT30_MJ_1b_LDP_127_QCD_fudged");
  draw_from_trees("fatpT30_MJ",trigger+AN_LDP,"num_csvm_jets40==2",
  		  ";#Sigmam_{J} [GeV];Events / 5 fb^{-1}", 12, 0, 600,
  		  "plotData",
  		  "fatpT30_MJ_2b_LDP_127_QCD_fudged");
  draw_from_trees("fatpT30_MJ",trigger+AN_LDP,"num_csvm_jets40>=3",
  		  ";#Sigmam_{J} [GeV];Events / 5 fb^{-1}", 12, 0, 600,
  		  "plotData",
  		  "fatpT30_MJ_geq3b_LDP_127_QCD_fudged");
  
  draw_from_trees("num_csvm_jets40",trigger+AN_LDP,"",
  		  ";N_{b-jets} (p_{T} > 40 GeV);Events / 5 fb^{-1}", 4, 0, 4,
  		  "plotData",
  		  "num_csvm_jets40_LDP_nm1_127_QCD_fudged");
  draw_from_trees("num_jets_pt40",trigger+AN_LDP,"num_csvm_jets40==0",
  		  ";N_{jets (R=0.5)} (p_{T} > 40 GeV);Events / 5 fb^{-1}", 5, 4, 9,
  		  "plotData",
  		  "num_jets_pt40_LDP_0b_127_QCD_fudged");
  draw_from_trees("num_jets_pt40",trigger+AN_LDP,"num_csvm_jets40==1",
  		  ";N_{jets (R=0.5)} (p_{T} > 40 GeV);Events / 5 fb^{-1}", 5, 4, 9,
  		  "plotData",
  		  "num_jets_pt40_LDP_1b_127_QCD_fudged");
  draw_from_trees("num_jets_pt40",trigger+AN_LDP,"num_csvm_jets40==2",
  		  ";N_{jets (R=0.5)} (p_{T} > 40 GeV);Events / 5 fb^{-1}", 5, 4, 9,
  		  "plotData",
  		  "num_jets_pt40_LDP_2b_127_QCD_fudged");
  draw_from_trees("num_jets_pt40",trigger+AN_LDP,"num_csvm_jets40>=3",
  		  ";N_{jets (R=0.5)} (p_{T} > 40 GeV);Events / 5 fb^{-1}", 5, 4, 9,
  		  "plotData",
  		  "num_jets_pt40_LDP_geq3b_127_QCD_fudged");
  
  draw_from_trees("met",trigger+AN_LDP,"num_csvm_jets40==0",
  		  ";E_{T}^{miss} [GeV];Events / 5 fb^{-1}", 12, 200, 500,
  		  "plotData",
  		  "met_0b_LDP_127_QCD_fudged");
  draw_from_trees("met",trigger+AN_LDP,"num_csvm_jets40==1",
  		  ";E_{T}^{miss} [GeV];Events / 5 fb^{-1}", 12, 200, 500,
  		  "plotData",
  		  "met_1b_LDP_127_QCD_fudged");
  draw_from_trees("met",trigger+AN_LDP,"num_csvm_jets40==2",
  		  ";E_{T}^{miss} [GeV];Events / 5 fb^{-1}", 12, 200, 500,
  		  "plotData",
  		  "met_2b_LDP_127_QCD_fudged");
  draw_from_trees("met",trigger+AN_LDP,"num_csvm_jets40>=3",
  		  ";E_{T}^{miss} [GeV];Events / 5 fb^{-1}", 12, 200, 500,
  		  "plotData",
  		  "met_geq3b_LDP_127_QCD_fudged");

    
  // LDP raw (for QCD fudge factors)
  
  draw_from_trees("num_csvm_jets40",trigger+AN_baseline,"",
		  ";N_{b-jets} (p_{T} > 40 GeV);Events / 5 fb^{-1}", 4, 0, 4,
		  "plotData",
		  "num_csvm_jets40_nm1_127_QCD_raw");
  draw_from_trees("num_csvm_jets40",trigger+AN_LDP,"",
		  ";N_{b-jets} (p_{T} > 40 GeV);Events / 5 fb^{-1}", 4, 0, 4,
		  "plotData",
		  "num_csvm_jets40_LDP_nm1_127_QCD_raw");
  draw_from_trees("met",trigger+AN_LDP,"num_csvm_jets40==0",
		  ";E_{T}^{miss} [GeV];Events / 5 fb^{-1}", 12, 200, 500,
		  "plotData",
		  "met_0b_127_QCD_raw");
  draw_from_trees("met",trigger+AN_LDP,"num_csvm_jets40==1",
		  ";E_{T}^{miss} [GeV];Events / 5 fb^{-1}", 12, 200, 500,
		  "plotData",
		  "met_1b_127_QCD_raw");
  draw_from_trees("met",trigger+AN_LDP,"num_csvm_jets40==2",
		  ";E_{T}^{miss} [GeV];Events / 5 fb^{-1}", 12, 200, 500,
		  "plotData",
		  "met_2b_127_QCD_raw");
  draw_from_trees("met",trigger+AN_LDP,"num_csvm_jets40>=3",
		  ";E_{T}^{miss} [GeV];Events / 5 fb^{-1}", 12, 200, 500,
		  "plotData",
		  "met_geq3b_127_QCD_raw");
  draw_from_trees("ht40",trigger+AN_LDP,"num_csvm_jets40==0",
  		  ";H_{T} [GeV];Events / 5 fb^{-1}", 14, 750, 2050,
  		  "plotData",
  		  "ht40_0b_127_QCD_raw");
  draw_from_trees("ht40",trigger+AN_LDP,"num_csvm_jets40==1",
  		  ";H_{T} [GeV];Events / 5 fb^{-1}", 14, 750, 2050,
  		  "plotData",
  		  "ht40_1b_127_QCD_raw");
  draw_from_trees("ht40",trigger+AN_LDP,"num_csvm_jets40==2",
  		  ";H_{T} [GeV];Events / 5 fb^{-1}", 14, 750, 2050,
  		  "plotData",
  		  "ht40_2b_127_QCD_raw");
  draw_from_trees("ht40",trigger+AN_LDP,"num_csvm_jets40>=3",
  		  ";H_{T} [GeV];Events / 5 fb^{-1}", 14, 750, 2050,
  		  "plotData",
  		  "ht40_geq3b_127_QCD_raw");
  draw_from_trees("fatpT30_MJ",trigger+AN_LDP,"num_csvm_jets40==0",
  		  ";#Sigmam_{J} [GeV];Events / 5 fb^{-1}", 12, 0, 600,
  		  "plotData",
  		  "fatpT30_MJ_0b_127_QCD_raw");
  draw_from_trees("fatpT30_MJ",trigger+AN_LDP,"num_csvm_jets40==1",
  		  ";#Sigmam_{J} [GeV];Events / 5 fb^{-1}", 12, 0, 600,
  		  "plotData",
  		  "fatpT30_MJ_1b_127_QCD_raw");
  draw_from_trees("fatpT30_MJ",trigger+AN_LDP,"num_csvm_jets40==2",
  		  ";#Sigmam_{J} [GeV];Events / 5 fb^{-1}", 12, 0, 600,
  		  "plotData",
  		  "fatpT30_MJ_2b_127_QCD_raw");
  draw_from_trees("fatpT30_MJ",trigger+AN_LDP,"num_csvm_jets40>=3",
  		  ";#Sigmam_{J} [GeV];Events / 5 fb^{-1}", 12, 0, 600,
  		  "plotData",
  		  "fatpT30_MJ_geq3b_127_QCD_raw");
  draw_from_trees("num_jets_pt40",trigger+AN_baseline,"",
		  ";N_{jets (R=0.5)} (p_{T} > 40 GeV);Events / 5 fb^{-1}", 5, 4, 9,
		  "plotData",
		  "num_jets_pt40_nm1_127_QCD_raw");
  draw_from_trees("num_jets_pt40",trigger+AN_LDP,"",
		  ";N_{jets (R=0.5)} (p_{T} > 40 GeV);Events / 5 fb^{-1}", 5, 4, 9,
		  "plotData",
		  "num_jets_pt40_LDP_nm1_127_QCD_raw");
  
 

  draw_from_trees("min_delta_phi_met_N",trigger+"met>200&&ht40>500&&jet4_pt>40&&num_reco_veto_electrons==0&&num_reco_veto_muons==0","",
    		  ";#Delta#Phi^{min}_{N};Events / 20 fb^{-1}", 20, 0., 40,
   		  "plotData",
    		  "min_delta_phi_met_N_nm1_127_QCD_raw");
  draw_from_trees("min_delta_phi_met_N",trigger+"met>200&&ht40>500&&jet4_pt>40&&num_reco_veto_electrons==0&&num_reco_veto_muons==0","",
    		  ";#Delta#Phi^{min}_{N};Events / 20 fb^{-1}", 20, 0., 40,
   		  "plotData:plotLog",
    		  "min_delta_phi_met_N_nm1_log_127_QCD_raw");

    

  

  // Z-rich
  /*
    draw_from_trees("met",trigger+AN_Zrich,"num_csvm_jets40==0",
    ";E_{T}^{miss} [GeV];Events / 5 fb^{-1}", 10, 500, 1000 ,
    "plotData",
    "met_0b_Zrich_127_QCD_fudged");
    draw_from_trees("met",trigger+AN_Zrich,"num_csvm_jets40==1",
    ";E_{T}^{miss} [GeV];Events / 5 fb^{-1}", 10, 500, 1000 ,
    "plotData",
    "met_1b_Zrich_127_QCD_fudged");
    draw_from_trees("met",trigger+AN_Zrich,"num_csvm_jets40==2",
    ";E_{T}^{miss} [GeV];Events / 5 fb^{-1}", 10, 500, 1000 ,
    "plotData",
    "met_2b_Zrich_127_QCD_fudged");
    draw_from_trees("met",trigger+AN_Zrich,"num_csvm_jets40>=3",
    ";E_{T}^{miss} [GeV];Events / 5 fb^{-1}", 10, 500, 1000 ,
    "plotData",
    "met_geq3b_Zrich_127_QCD_fudged");
    draw_from_trees("ht40",trigger+AN_Zrich,"num_csvm_jets40==0",
    ";H_{T} [GeV];Events / 5 fb^{-1}", 7, 750, 2150,
    "plotData",
    "ht40_0b_Zrich_127_QCD_fudged");
    draw_from_trees("ht40",trigger+AN_Zrich,"num_csvm_jets40==1",
    ";H_{T} [GeV];Events / 5 fb^{-1}", 7, 750, 2150,
    "plotData",
    "ht40_1b_Zrich_127_QCD_fudged");
    draw_from_trees("ht40",trigger+AN_Zrich,"num_csvm_jets40==2",
    ";H_{T} [GeV];Events / 5 fb^{-1}", 7, 750, 2150,
    "plotData",
    "ht40_2b_Zrich_127_QCD_fudged");
    draw_from_trees("ht40",trigger+AN_Zrich,"num_csvm_jets40>=3",
    ";H_{T} [GeV];Events / 5 fb^{-1}", 7, 750, 2150,
    "plotData",
    "ht40_geq3b_Zrich_127_QCD_fudged");

    draw_from_trees("fatpT30_MJ",trigger+AN_Zrich,"num_csvm_jets40==0",
    ";#Sigmam_{J} [GeV];Events / 5 fb^{-1}", 12, 0, 600,
    "plotData",
    "fatpT30_MJ_0b_Zrich_127_QCD_fudged");
    draw_from_trees("fatpT30_MJ",trigger+AN_Zrich,"num_csvm_jets40==1",
    ";#Sigmam_{J} [GeV];Events / 5 fb^{-1}", 12, 0, 600,
    "plotData",
    "fatpT30_MJ_1b_Zrich_127_QCD_fudged");
    draw_from_trees("fatpT30_MJ",trigger+AN_Zrich,"num_csvm_jets40==2",
    ";#Sigmam_{J} [GeV];Events / 5 fb^{-1}", 12, 0, 600,
    "plotData",
    "fatpT30_MJ_2b_Zrich_127_QCD_fudged");
    draw_from_trees("fatpT30_MJ",trigger+AN_Zrich,"num_csvm_jets40>=3",
    ";#Sigmam_{J} [GeV];Events / 5 fb^{-1}", 12, 0, 600,
    "plotData",
    "fatpT30_MJ_geq3b_Zrich_127_QCD_fudged");
    draw_from_trees("num_jets_pt40",trigger+AN_Zrich,"num_csvm_jets40==0",
    ";N_{jets (R=0.5)} (p_{T} > 40 GeV);Events / 5 fb^{-1}", 3, 1, 4,
    "plotData",
    "num_jets_pt40_Zrich_0b_127_QCD_fudged");
    draw_from_trees("num_jets_pt40",trigger+AN_Zrich,"num_csvm_jets40==1",
    ";N_{jets (R=0.5)} (p_{T} > 40 GeV);Events / 5 fb^{-1}", 3, 1, 4,
    "plotData",
    "num_jets_pt40_Zrich_1b_127_QCD_fudged");
    draw_from_trees("num_jets_pt40",trigger+AN_Zrich,"num_csvm_jets40==2",
    ";N_{jets (R=0.5)} (p_{T} > 40 GeV);Events / 5 fb^{-1}", 3, 1, 4,
    "plotData",
    "num_jets_pt40_Zrich_2b_127_QCD_fudged");
    draw_from_trees("num_jets_pt40",trigger+AN_Zrich,"num_csvm_jets40>=3",
    ";N_{jets (R=0.5)} (p_{T} > 40 GeV);Events / 5 fb^{-1}", 3, 1, 4,
    "plotData",
    "num_jets_pt40_Zrich_geq3b_127_QCD_fudged");
  */
  return 0;
  
}

