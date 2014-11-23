/* 
   Script for drawing a branch from reduec trees with a set of cuts.
*/

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
#include "../include/my_style.hpp"

TChain* ttbar_ch;
TChain* qcd_ch;
TChain* znn_ch;
TChain* wjets_ch;
TChain* single_top_ch;
TChain* other_ch;
TChain* t1tttt_1500_100_ch;
TChain* t1tttt_1200_800_ch;
TChain* t1bbbb_1500_100_ch;
TChain* t1bbbb_1000_900_ch;


using namespace std;

TString plotdir = "plots/n_minus_1/";

void draw_from_trees(TString var, TCut other_cuts,
		     TString weights, TString title, int nbinsx, 
		     double xlow, double xup,
		     TString options="plotSig:plotLog:plotData",
		     double cut_low=-1, double cut_high=-1,
		     TString plot_title="default")
{

  bool plotSig = options.Contains("plotSig") && (!options.Contains("!plotSig"));
  bool plotLog = options.Contains("plotLog") && (!options.Contains("!plotLog"));
  bool plotData = options.Contains("plotData") && (!options.Contains("!plotData"));
  //  bool plotT1tttt = options.Contains("T1tttt") && (!options.Contains("!T1tttt"));
  bool sigStack = options.Contains("sigStack") && (!options.Contains("!sigStack"));

  // Book histograms
  TH1D * httbar = new TH1D("ttbar" , title, nbinsx, xlow, xup);
  TH1D * hqcd = new TH1D("qcd" , title, nbinsx, xlow, xup);
  TH1D * hznn = new TH1D("znn" , title, nbinsx, xlow, xup);
  TH1D * hwjets = new TH1D("wjets" , title, nbinsx, xlow, xup);
  TH1D * hother = new TH1D("other" , title, nbinsx, xlow, xup);
  TH1D * hmc_exp = new TH1D("mc_exp" , title, nbinsx, xlow, xup);
  TH1D * hsingle_top = new TH1D("single_top" , title, nbinsx, xlow, xup);  
  TH1D * ht1bbbb_1500_100 = new TH1D("t1bbbb_1500_100" , title, nbinsx, xlow, xup);
  TH1D * ht1bbbb_1000_900 = new TH1D("t1bbbb_1000_900" , title, nbinsx, xlow, xup);
  TH1D * ht1tttt_1500_100 = new TH1D("t1tttt_1500_100" , title, nbinsx, xlow, xup);
  TH1D * ht1tttt_1200_800 = new TH1D("t1tttt_1200_800" , title, nbinsx, xlow, xup);

  // Format cuts
  TCut cut(other_cuts);

  // TCut ttbar_weight("(weightppb*5000)/top_pt_weight_official");

  TCut ttbar_weight("(weightppb*5000)");

  cout << "Filling histograms for " << var.Data() << endl;
  ttbar_ch->Project("ttbar",var,(cut)*ttbar_weight);
  qcd_ch->Project("qcd",var,cut*weights);
  znn_ch->Project("znn",var,cut*weights);
  wjets_ch->Project("wjets",var,(cut)*weights);
  other_ch->Project("other",var,cut*weights);
  single_top_ch->Project("single_top",var,cut*weights);
  t1bbbb_1500_100_ch->Project("t1bbbb_1500_100",var,(cut)*weights);
  t1bbbb_1000_900_ch->Project("t1bbbb_1000_900",var,(cut)*weights);
  t1tttt_1500_100_ch->Project("t1tttt_1500_100",var,(cut)*weights);
  t1tttt_1200_800_ch->Project("t1tttt_1200_800",var,(cut)*weights);



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
    i_overflow=ht1bbbb_1500_100->IntegralAndError(nbinsx,nbinsx+1,e_overflow);
    ht1bbbb_1500_100->SetBinContent(nbinsx, i_overflow);
    ht1bbbb_1500_100->SetBinError(nbinsx, e_overflow);
    i_overflow=ht1bbbb_1000_900->IntegralAndError(nbinsx,nbinsx+1,e_overflow);
    ht1bbbb_1000_900->SetBinContent(nbinsx, i_overflow);
    ht1bbbb_1000_900->SetBinError(nbinsx, e_overflow);
    i_overflow=ht1tttt_1500_100->IntegralAndError(nbinsx,nbinsx+1,e_overflow);
    ht1tttt_1500_100->SetBinContent(nbinsx, i_overflow);
    ht1tttt_1500_100->SetBinError(nbinsx, e_overflow);
    i_overflow=ht1tttt_1200_800->IntegralAndError(nbinsx,nbinsx+1,e_overflow);
    ht1tttt_1200_800->SetBinContent(nbinsx, i_overflow);
    ht1tttt_1200_800->SetBinError(nbinsx, e_overflow);
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
  hmc_exp->GetXaxis()->SetTitle(httbar->GetXaxis()->GetTitle());
  hmc_exp->GetYaxis()->SetTitle(ytitle);
  cout << "... DONE: add all backgrounds to mc_exp." << endl;
  
  Double_t ttbar_e(0.), qcd_e(0.), znn_e(0.), wjets_e(0.), other_e(0.), single_top_e(0.), bg_tot_e(0.), t1tttt_1500_100_e(0.);
  double ttbar_n(httbar->IntegralAndError(0,nbinsx+1, ttbar_e));
  double qcd_n(hqcd->IntegralAndError(0,nbinsx+1, qcd_e));
  double znn_n(hznn->IntegralAndError(0,nbinsx+1, znn_e));
  double wjets_n(hwjets->IntegralAndError(0,nbinsx+1, wjets_e));
  double other_n(hother->IntegralAndError(0,nbinsx+1, other_e));
  double single_top_n(hsingle_top->IntegralAndError(0,nbinsx+1, single_top_e));
  double bg_tot(hmc_exp->IntegralAndError(0,nbinsx+1, bg_tot_e));
  double t1tttt_1500_100_n(ht1tttt_1500_100->IntegralAndError(0,nbinsx+1, t1tttt_1500_100_e));


  printf("Counts before cut: %s\n",var.Data());
  printf("&ttbar&qcd&znn&wjets&single top&other&t1bbbb_1500_100\\\\ \n");
  printf("%s & %3.2f+-%3.2f & %3.2f+-%3.2f & %3.2f+-%3.2f & %3.2f+-%3.2f & %3.2f+-%3.2f & %3.2f+-%3.2f & %3.2f+-%3.2f & %3.2f+-%3.2f \\\\\n",
	 var.Data(),
	 ttbar_n,ttbar_e,
	 qcd_n,qcd_e,
	 znn_n,znn_e,
	 wjets_n,wjets_e,
	 single_top_n,single_top_e,
	 other_n,other_e,
	 //	 hmc_exp->GetBinContent(1), hmc_exp->GetBinError(1),
	 bg_tot,bg_tot_e,
	 t1tttt_1500_100_n,t1tttt_1500_100_e);


  cout << "... DONE: filled histograms." << endl;
 
  if (sigStack) {
    for (int bin(0); bin<nbinsx; bin++) {
      ht1bbbb_1500_100->SetBinContent(bin+1, hmc_exp->GetBinContent(bin+1));
      ht1bbbb_1000_900->SetBinContent(bin+1, hmc_exp->GetBinContent(bin+1));
      ht1tttt_1500_100->SetBinContent(bin+1, hmc_exp->GetBinContent(bin+1));
      ht1tttt_1200_800->SetBinContent(bin+1, hmc_exp->GetBinContent(bin+1));
    }
  }
 
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
  set_style(httbar, "ttbar");
  set_style(hqcd, "qcd");
  set_style(hznn, "znn");
  set_style(hwjets, "wjets");
  set_style(hother, "other");
  set_style(hsingle_top, "single_top");

  // Setup auxiliary histograms (ratios, errors, etc)
  TH1D * staterr = (TH1D *) hmc_exp->Clone("staterr");
  staterr->Sumw2();
  //staterr->SetFillColor(kRed);
  staterr->SetFillColor(kGray+3);
  staterr->SetMarkerSize(0);
  staterr->SetFillStyle(3013);

 
  // Setup legends
  TLegend * leg1 = new TLegend(0.48, 0.68, 0.72, 0.92);
  set_style(leg1,0.025);
  if (plotData) leg1->AddEntry(hsingle_top, "Data", "pel");
  if (plotSig) {
      leg1->AddEntry(ht1bbbb_1500_100, "#splitline{T1bbbb}{(1500,100) GeV}", "l");
      leg1->AddEntry(ht1bbbb_1000_900, "#splitline{T1bbbb}{(1000,900) GeV}", "l");
      leg1->AddEntry(ht1tttt_1500_100, "#splitline{T1tttt}{(1500,100) GeV}", "l");
      leg1->AddEntry(ht1tttt_1200_800, "#splitline{T1tttt}{(1200,800) GeV}", "l");
    }
 

  TLegend * leg2 = new TLegend(0.72, 0.68, 0.94, 0.92);
  set_style(leg2,0.025);
  //if (plot_title.Contains("2012")) {
  //  leg2->AddEntry(hqcd, "QCD", "f");
  //  leg2->AddEntry(httbar, "t#bar{t}", "f");
  // }else{
     leg2->AddEntry(httbar, "t#bar{t}", "f");
     leg2->AddEntry(hqcd, "QCD", "f");
  // }
  leg2->AddEntry(hznn, "Z+jets", "f");
  leg2->AddEntry(hwjets, "W+jets", "f");
  leg2->AddEntry(hsingle_top, "Single Top", "f");
  leg2->AddEntry(hother, "Other", "f");
  leg2->AddEntry(staterr, "MC uncert.", "f");
    
  double ymax = hs->GetMaximum();
  if (ht1tttt_1500_100->GetMaximum()>ymax) ymax=ht1tttt_1500_100->GetMaximum();
  if (ht1bbbb_1500_100->GetMaximum()>ymax) ymax=ht1bbbb_1500_100->GetMaximum();


  if(plotLog) {
    hs->SetMaximum(100*ymax);
    hs->SetMinimum(0.1);
  }
  else {
    hs->SetMaximum(2*ymax);
    if (plot_title.Contains("baseline")) hs->SetMaximum(1.3*ymax);
  }
  // Vertical lines for cuts
  TLine* line_low = new TLine(cut_low,0,cut_low,1.5*ymax);
  TLine* line_high = new TLine(cut_high,0,cut_high,1.5*ymax);
  set_style(line_low);
  set_style(line_high);

  // Setup canvas and pads  
  TCanvas * c1 = new TCanvas("c1", "c1", 700, 700);
  TPad * pad1 = new TPad("pad1", "top pad" , 0.0, 0.3, 1.0, 1.0);
  TPad * pad2 = new TPad("pad2", "bottom pad", 0.0, 0.0, 1.0, 0.3);
  if(plotData) {
    pad1->SetBottomMargin(0.0);
    pad1->Draw();
    pad2->SetTopMargin(0.0);
    pad2->SetBottomMargin(0.35);
    pad2->Draw();
    pad1->cd();
    pad1->SetLogy(plotLog);
  }
  else {
    c1->cd();
    c1->SetLogy(plotLog);
  }
  // Draw hists
  hs->Draw("hist");
  hs->SetTitle(hmc_exp->GetTitle());
  hs->GetXaxis()->SetTitle(httbar->GetXaxis()->GetTitle());
  hs->GetYaxis()->SetTitle(ytitle);
  hs->GetXaxis()->SetLabelSize(0.03);
  hs->GetYaxis()->SetLabelSize(0.03);
  if (plotData) 
    {
      //  hsingle_top->Draw("e1 same");
      hs->GetXaxis()->SetLabelSize(0);
    }
  staterr->Draw("e2 same");
  if (plotSig) {
    ht1bbbb_1500_100->SetLineColor(2);
    ht1bbbb_1500_100->SetLineWidth(4);
    ht1bbbb_1500_100->SetFillColor(0);
    ht1bbbb_1000_900->SetLineColor(2);
    ht1bbbb_1000_900->SetLineWidth(4);
    ht1bbbb_1000_900->SetLineStyle(7);
    ht1bbbb_1000_900->SetFillColor(0);
    ht1tttt_1500_100->SetLineColor(kGreen);
    ht1tttt_1500_100->SetLineWidth(4);
    ht1tttt_1500_100->SetFillColor(0);
    ht1tttt_1200_800->SetLineColor(kGreen);
    ht1tttt_1200_800->SetLineStyle(7);
    ht1tttt_1200_800->SetLineWidth(4);
    ht1tttt_1200_800->SetFillColor(0);
    ht1bbbb_1500_100->Draw("hist same");
    ht1bbbb_1000_900->Draw("hist same");
    ht1tttt_1500_100->Draw("hist same");
    ht1tttt_1200_800->Draw("hist same");
  }
  if (cut_low>0) line_low->Draw("same");
  if (cut_high>0) line_high->Draw("same");


  // Draw legends
  leg1->Draw();
  leg2->Draw();
  TLatex * latex = new TLatex();
  latex->SetNDC();
  latex->SetTextAlign(12);
  latex->SetTextFont(62);
  latex->SetTextSize(0.042);
  latex->DrawLatex(0.19, 0.89, "CMS Simulation");
  latex->SetTextSize(0.03);
  latex->DrawLatex(0.19, 0.84, "#sqrt{s} = 13 TeV, L = 5 fb^{-1}");
  
  // Print
  cout << "MakePlots(): Printing..." << endl;

  c1->cd();
  if (plot_title.EqualTo("default")) plot_title=plotdir+var;
  gPad->Print(plotdir+plot_title+".pdf");


  // Clean up
  delete staterr;
  delete leg1;
  delete leg2;
  delete latex;
  // delete pave;
  delete hs;
  delete pad1;
  delete pad2;
  delete c1;

  delete httbar;
  delete hqcd;
  delete hznn;
  delete hwjets;
  delete hsingle_top;
  delete hother;
  delete hmc_exp;

  delete ht1bbbb_1500_100;
  delete ht1bbbb_1000_900;
  delete ht1tttt_1500_100;
  delete ht1tttt_1200_800;

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
  t1tttt_1500_100_ch = new TChain("reduced_tree");
  t1tttt_1200_800_ch = new TChain("reduced_tree");
  t1bbbb_1500_100_ch = new TChain("reduced_tree");
  t1bbbb_1000_900_ch = new TChain("reduced_tree");

  
  TFileCollection* ttbar_fc = new TFileCollection("ttbar_fc","","../../reduced_trees/tfcs/ttbar.txt");
  TFileCollection* qcd_fc = new TFileCollection("qcd_fc","","../../reduced_trees/tfcs/qcd_v2.txt");
  // TFileCollection* qcd_fc = new TFileCollection("qcd_fc","","../../reduced_trees/tfcs/qcd.txt");
  TFileCollection* znn_fc = new TFileCollection("znn_fc","","../../reduced_trees/tfcs/znn.txt");
  TFileCollection* wjets_fc = new TFileCollection("wjets_fc","","../../reduced_trees/tfcs/wjets.txt");
  TFileCollection* single_top_fc = new TFileCollection("single_top_fc","","../../reduced_trees/tfcs/single_top.txt");
  TFileCollection* other_fc = new TFileCollection("other_fc","","../../reduced_trees/tfcs/other.txt");
  
  ttbar_ch->AddFileInfoList((TCollection*)ttbar_fc->GetList());
  qcd_ch->AddFileInfoList((TCollection*)qcd_fc->GetList());
  znn_ch->AddFileInfoList((TCollection*)znn_fc->GetList());
  wjets_ch->AddFileInfoList((TCollection*)wjets_fc->GetList());
  single_top_ch->AddFileInfoList((TCollection*)single_top_fc->GetList());
  other_ch->AddFileInfoList((TCollection*)other_fc->GetList());
   



  // ttbar_ch->Add("../../save/TTJets*PU20*v76*.root");
  //  //ttbar_ch->Add("../../save/TTJets*PU_S14*v75*.root");
  // qcd_ch->Add("../../save/QCD_Pt-*v75*.root");
  // //  qcd_ch->Add("../../save/low-pt-qcd/QCD_Pt-*.root");
  // znn_ch->Add("../../save/ZJets*v75*.root");
  // wjets_ch->Add("../../save/WJets*v75*.root");
  // single_top_ch->Add("../../save/TTo*v75*.root");
  // single_top_ch->Add("../../save/TBarTo*v75*.root");
  // single_top_ch->Add("../../save/*tW*v75*.root");
  // other_ch->Add("../../save/*HToBB*v75*.root");
  // other_ch->Add("../../save/TTbarH*v75*.root");



  t1tttt_1500_100_ch->Add("../../reduced_trees/SMS-T1tttt_2J_mGl-1500_mLSP-100_Tune4C_13TeV-madgraph-tauola_Spring14miniaod-PU20bx25_POSTLS170_V5-v2_MINIAODSIM_UCSB2268_v76/*.root");
  t1tttt_1200_800_ch->Add("../../reduced_trees/SMS-T1tttt_2J_mGl-1200_mLSP-800_Tune4C_13TeV-madgraph-tauola_Spring14miniaod-PU20bx25_POSTLS170_V5-v2_MINIAODSIM_UCSB2267_v76/*.root");
  t1bbbb_1500_100_ch->Add("../../reduced_trees/SMS-T1bbbb_2J_mGl-1500_mLSP-100_Tune4C_13TeV-madgraph-tauola_Spring14miniaod-PU20bx25_POSTLS170_V5-v3_MINIAODSIM_UCSB2266_v76/*.root");
  t1bbbb_1000_900_ch->Add("../../reduced_trees/SMS-T1bbbb_2J_mGl-1000_mLSP-900_Tune4C_13TeV-madgraph-tauola_Spring14miniaod-PU20bx25_POSTLS170_V5-v2_MINIAODSIM_UCSB2264_v76/*.root");

  cout << "Entries in chains..." << endl;
  cout << "t1tttt_1500_100 N=" << t1tttt_1500_100_ch->GetEntries() << endl;
  cout << "t1tttt_1200_800 N=" << t1tttt_1200_800_ch->GetEntries() << endl;
  cout << "t1bbbb_1500_100 N=" << t1bbbb_1500_100_ch->GetEntries() << endl;
  cout << "t1bbbb_1000_900 N=" << t1bbbb_1000_900_ch->GetEntries() << endl;
  cout << "ttbar N=" << ttbar_ch->GetEntries() << endl;
  cout << "qcd N=" << qcd_ch->GetEntries() << endl;
  cout << "znn N=" << znn_ch->GetEntries() << endl;
  cout << "wjets N=" << wjets_ch->GetEntries() << endl;
  cout << "single_top N=" << single_top_ch->GetEntries() << endl;
  cout << "other N=" << other_ch->GetEntries() << endl;

  // quick skim
  TCut start("met>200&&fatpT30_MJ>400&&jet3_pt>50");
  TCut jack("fatpT30_MJ>400&&met>200&&jet3_pt>50&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_csvm_jets>=2");
  TCut baseline(start+"num_csvm_jets>1&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt50>=8");
  TCut baselineHT("met>400&&ht30>1000&&num_csvm_jets30>1&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>=4");
  TCut baselineMJ("met>400&&fatpT30_MJ>300&&num_csvm_jets30>1&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>=4");
  TCut search(jack+"met>200&&min_delta_phi_met_N>4&&num_reco_veto_electrons==0&&num_reco_veto_muons==0&&num_csvm_jets30>=2");
  TCut lepVeto("num_reco_veto_electrons==0&&num_reco_veto_muons==0");
  TCut qcd_control("ht50>400&&jet1_pt>70&jet2_pt>70&&jet3_pt>50&&min_delta_phi_met_N<4&&num_reco_veto_electrons==0&&num_reco_veto_muons==0");



  /*draw_from_trees(TString var, TCut other_cuts,
    TString weights, TString title, int nbinsx, 
    double xlow, double xup,
    TString options="plotSig:plotLog:plotData",
    double cut_low=-1, double cut_high=-1,
    TString plot_title="default")
  */


  // Binning N-1
  draw_from_trees("ht30","met>400&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>=4&&min_delta_phi_met_N>4&&num_csvm_jets30>=2","(weightppb*5000)",
   		  ";H_{T} [GeV];Events / 5 fb^{-1}", 20, 0, 4000.,
   		  "plotSig:plotLog",-1,-1,
   		  "ht30_nm1_1119_v2");
  draw_from_trees("met","ht30>500&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>=4&&min_delta_phi_met_N>4&&num_csvm_jets30>=2","(weightppb*5000)",
   		  ";E_{T}^{miss} [GeV];Events / 5 fb^{-1}", 30, 0, 1500.,
   		  "plotSig:plotLog",-1,-1,
   		  "met_nm1_1119_v2");
  draw_from_trees("min_delta_phi_met_N","met>400&&ht30>500&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>=4&&num_csvm_jets30>=2","(weightppb*5000)",
    		  ";#Delta#Phi^{min}_{N};Events / 20 fb^{-1}", 20, 0., 40,
   		  "plotSig:plotLog",-1,-1,
    		  "min_delta_phi_met_N_nm1_1119_v2");
  draw_from_trees("num_reco_veto_electrons+num_reco_veto_muons","met>400&&ht30>500&&num_jets_pt30>=4&&min_delta_phi_met_N>4&&num_csvm_jets30>=2","(weightppb*5000)",
		  ";n_{e+#mu} (p_{T} > 10 GeV);Events / 5 fb^{-1}", 6, 0, 6,
		  "plotSig:plotLog",-1,-1,
		  "num_reco_veto_lepton_nm1_1119_v2");
  draw_from_trees("num_csvm_jets30","met>400&&ht30>500&&num_jets_pt30>=4&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0","(weightppb*5000)",
		  ";n_{CSVM} (p_{T} > 30 GeV);Events / 5 fb^{-1}", 6, 0, 6,
		  "plotSig:plotLog",-1,-1,
		  "num_csvm_jets30_nm1_1119_v2");
  draw_from_trees("num_jets_pt30","met>400&&ht30>500&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_csvm_jets30>=2","(weightppb*5000)",
		  ";n_{jets} (p_{T} > 30 GeV);Events / 5 fb^{-1}", 15, 0, 15,
		  "plotSig:plotLog",-1,-1,
		  "num_jets_pt30_nm1_1119_v2");


  // MJ / HT slide
  draw_from_trees("ht30","met>200&&num_csvm_jets30>1&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>=4","(weightppb*5000)",
  		  ";H_{T} [GeV];Events / 5 fb^{-1}", 20, 0, 4000,
  		  "plotSig:plotLog",-1,-1,
  		  "ht30_1119_4j30_met250_nm1");
  draw_from_trees("fatpT30_MJ","met>200&&num_csvm_jets30>1&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>=4","(weightppb*5000)",
  		  ";M_{J} [GeV];Events / 5 fb^{-1}", 20, 0., 1500.,
  		  "plotSig:plotLog",-1,-1,
  		  "fatpT30_MJ_1119_4j30_met250_nm1");
  draw_from_trees("ht30","met>400&&num_csvm_jets30>1&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>=8","(weightppb*5000)",
  		  ";H_{T} [GeV];Events / 5 fb^{-1}", 10, 0, 4000.,
  		  "plotSig:plotLog",-1,-1,
  		  "ht30_1119_8j30_nm1");
  draw_from_trees("fatpT30_MJ","met>400&&num_csvm_jets30>1&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>=8","(weightppb*5000)",
  		  ";M_{J} [GeV];Events / 5 fb^{-1}", 10, 0., 1500.,
  		  "plotSig:plotLog",-1,-1,
  		  "fatpT30_MJ_1119_8j30_nm1");
  draw_from_trees("ht30","met>400&&num_csvm_jets30>1&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>=10","(weightppb*5000)",
  		  ";H_{T} [GeV];Events / 5 fb^{-1}", 10, 0, 4000,
  		  "plotSig:",-1,-1,
  		  "ht30_1119_10j30_nm1");
  draw_from_trees("fatpT30_MJ","met>400&&num_csvm_jets30>1&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>=10","(weightppb*5000)",
  		  ";M_{J} [GeV];Events / 5 fb^{-1}", 10, 0., 1500.,
  		  "plotSig:",-1,-1,
  		  "fatpT30_MJ_1119_10j30_nm1");
  draw_from_trees("ht30","met>400&&num_csvm_jets30>1&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>=4","(weightppb*5000)",
  		  ";H_{T} [GeV];Events / 5 fb^{-1}", 20, 0, 4000,
  		  "plotSig:plotLog",-1,-1,
  		  "ht30_1119_4j30_nm1");
  draw_from_trees("fatpT30_MJ","met>400&&num_csvm_jets30>1&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>=4","(weightppb*5000)",
  		  ";M_{J} [GeV];Events / 5 fb^{-1}", 20, 0., 1500.,
  		  "plotSig:plotLog",-1,-1,
  		  "fatpT30_MJ_1119_4j30_nm1");
  draw_from_trees("ht30","met>400&&num_csvm_jets30>1&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>=6","(weightppb*5000)",
  		  ";H_{T} [GeV];Events / 5 fb^{-1}", 20, 0, 4000,
  		  "plotSig:plotLog",-1,-1,
  		  "ht30_1119_6j30_nm1");
  draw_from_trees("fatpT30_MJ","met>400&&num_csvm_jets30>1&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>=6","(weightppb*5000)",
  		  ";M_{J} [GeV];Events / 5 fb^{-1}", 20, 0., 1500.,
  		  "plotSig:plotLog",-1,-1,
  		  "fatpT30_MJ_1119_6j30_nm1");





  // ZBi
  draw_from_trees("met","met>=400&&ht30>1400&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>=4&&min_delta_phi_met_N>4&&num_csvm_jets>1","(weightppb*5000)",
   		  ";E_{T}^{miss} [GeV];Events / 5 fb^{-1}", 10, 400, 1400.,
   		  "plotSig:",-1,-1,
   		  "met_ZBi_T1bbbb_nm1_1119");
  draw_from_trees("met","ht30>1000&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>=10&&min_delta_phi_met_N>4&&num_csvm_jets>1","(weightppb*5000)",
   		  ";E_{T}^{miss} [GeV];Events / 5 fb^{-1}", 10, 300, 1300.,
   		  "plotSig:",-1,-1,
   		  "met_ZBi_T1tttt_nm1");

  // draw_from_trees("min_delta_phi_met_N","met>400&&ht30>1000&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>=4&&num_csvm_jets30>=2","(weightppb*5000)",
  //   		  ";#Delta#Phi^{min}_{N};Events / 20 fb^{-1}", 20, 0., 40,
  //  		  "plotSig:plotLog",-1,-1,
  //   		  "min_delta_phi_met_N_nm1_v2_1119");

  //   TCut jack("ht50>500&&met>200&&jet3_pt>50&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_csvl_jets>=2");




  // RA2b 2012
  draw_from_trees("min_delta_phi_met_N","met>125&&ht50>400&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&jet3_pt>50&&jet2_pt>70&&num_csvm_jets>0","(weightppb*5000)",
    		  ";#Delta#Phi^{min}_{N};Events / 20 fb^{-1}", 20, 0., 40,
   		  "plotSig:plotLog",-1,-1,
    		  "min_delta_phi_met_N_2012_nm1");
  draw_from_trees("met","ht50>400&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&jet3_pt>50&&jet2_pt>70&&min_delta_phi_met_N>4&&num_csvm_jets>0","(weightppb*5000)",
   		  ";E_{T}^{miss} [GeV];Events / 5 fb^{-1}", 50, 0, 1500.,
   		  "plotSig:plotLog",-1,-1,
   		  "met_2012_nm1");
  draw_from_trees("ht50","met>125&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&jet3_pt>50&&jet2_pt>70&&min_delta_phi_met_N>4&&num_csvm_jets>0","(weightppb*5000)",
   		  ";H_{T} [GeV];Events / 5 fb^{-1}", 40, 0, 4000.,
   		  "plotSig:plotLog",-1,-1,
   		  "ht50_2012_nm1");
  draw_from_trees("num_reco_veto_electrons+num_reco_veto_muons","met>125&&ht50>400&&jet3_pt>50&&jet2_pt>70&&min_delta_phi_met_N>4&&num_csvm_jets>0","(weightppb*5000)",
		  ";n_{e+#mu} (p_{T} > 10 GeV);Events / 5 fb^{-1}", 6, 0, 6,
		  "plotSig:plotLog",-1,-1,
		  "num_reco_veto_leptons_2012_nm1");
  draw_from_trees("num_csvm_jets","met>125&&ht50>400&&jet3_pt>50&&jet2_pt>70&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0","(weightppb*5000)",
		  ";n_{CSVM} (p_{T} > 50 GeV);Events / 5 fb^{-1}", 6, 0, 6,
		  "plotSig:plotLog",-1,-1,
		  "num_csvm_jets_2012_nm1");
  draw_from_trees("num_jets_pt50","met>125&&ht50>400&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_csvm_jets>0","(weightppb*5000)",
		  ";n_{jets} (p_{T} > 50 GeV);Events / 5 fb^{-1}", 15, 0, 15,
		  "plotSig:plotLog",-1,-1,
		  "num_jets_pt50_2012_nm1");

  // lost leptons workshop
  draw_from_trees("num_reco_veto_electrons+num_reco_veto_muons","met>400&&ht30>1000&&num_csvm_jets>1&&min_delta_phi_met_N>4&&num_jets_pt30>3&&num_csvm_jets30>1","(weightppb*5000)",
		  ";n_{e+#mu} (p_{T} > 10 GeV);Events / 5 fb^{-1}", 6, 0, 6,
		  "plotSig:",-1,-1,
		  "num_reco_veto_leptons_1113_25ns"); 
  draw_from_trees("num_reco_veto_electrons+num_reco_veto_muons","met>400&&ht30>1000&&num_csvm_jets>1&&min_delta_phi_met_N>4&&num_jets_pt30>3&&num_csvm_jets30>1","(weightppb*5000)",
		  ";n_{e+#mu} (p_{T} > 10 GeV);Events / 5 fb^{-1}", 6, 0, 6,
		  "plotSig:plotLog",-1,-1,
		  "num_reco_veto_leptons_1113_25ns_log"); 

  // N-1 (10/19)
  // TCut start("met>200&&fatpT30_MJ>400&&num_csvm_jets>1&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt50>=8");

  // Can't run these on skims
  // draw_from_trees("mht","ht30>1000&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>3&&min_delta_phi_met_N>4&&num_csvm_jets30>1","(weightppb*5000)",
  //  		  ";H_{T}^{miss} [GeV];Events / 5 fb^{-1}", 50, 0, 1500.,
  //  		  "plotSig:plotLog",-1,-1,
  //  		  "mht_1029_2b_nm1");
  // draw_from_trees("met","ht30>1000&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>3&&min_delta_phi_met_N>4&&num_csvm_jets30>1","(weightppb*5000)",
  //  		  ";E_{T}^{miss} [GeV];Events / 5 fb^{-1}", 50, 0, 1500.,
  //  		  "plotSig:plotLog",-1,-1,
  //  		  "met_1029_2b_nm1");
  // draw_from_trees("num_jets_pt30","met>400&&ht30>1000&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&min_delta_phi_met_N>4&&num_csvm_jets30>1","(weightppb*5000)",
  // 		  ";n_{jets} (p_{T} > 30 GeV);Events / 5 fb^{-1}", 15, 0, 15,
  // 		  "plotSig:plotLog",-1,-1,
  // 		  "num_jets_pt30_1029_2b_nm1");
  // draw_from_trees("ht30","met>400&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>3&&min_delta_phi_met_N>4&&num_csvm_jets30>1","(weightppb*5000)",
  //  		  ";H_{T} [GeV];Events / 5 fb^{-1}", 40, 0, 4000,
  //  		  "plotSig:plotLog",-1,-1,
  //  		  "ht30_1029_2b_nm1");

  // draw_from_trees("num_jets_pt30","met>400&&ht30>1000&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&min_delta_phi_met_N>4","(weightppb*5000)",
  // 		  ";n_{jets} (p_{T} > 30 GeV);Events / 5 fb^{-1}", 15, 0, 15,
  // 		  "plotSig:plotLog",-1,-1,
  // 		  "num_jets_pt30_1029_nobr_nm1");
  // draw_from_trees("met","ht30>1000&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>3&&min_delta_phi_met_N>4","(weightppb*5000)",
  //  		  ";E_{T}^{miss} [GeV];Events / 5 fb^{-1}", 50, 0, 1500.,
  //  		  "plotSig:plotLog",-1,-1,
  //  		  "met_1029_nobr_nm1");
  // draw_from_trees("ht30","met>400&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>3&&min_delta_phi_met_N>4","(weightppb*5000)",
  //  		  ";H_{T} [GeV];Events / 5 fb^{-1}", 40, 0, 4000,
  //  		  "plotSig:plotLog",-1,-1,
  //  		  "ht30_1029_nobr_nm1");


  // draw_from_trees("met","num_reco_veto_muons==0&&num_reco_veto_electrons==0&&ht30>1000","(weightppb*5000)",
  //  		  ";E_{T}^{miss} [GeV];Events / 5 fb^{-1}", 60, 0, 1500.,
  //  		  "plotSig:plotLog",-1,-1,
  //  		  "met_1023_ht1000");
  // draw_from_trees("ht30","num_reco_veto_muons==0&&num_reco_veto_electrons==0","(weightppb*5000)",
  //  		  ";H_{T} [GeV];Events / 5 fb^{-1}", 80, 0, 4000,
  //  		  "plotSig:plotLog",-1,-1,
  //  		  "ht30_1023_no_lep");


  // draw_from_trees("met","ht40>1000&&num_csvm_jets30>1&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>=6","(weightppb*5000)",
  // 		  ";E_{T}^{miss} [GeV];Events / 5 fb^{-1}", 10, 400, 1400.,
  // 		  "plotSig:",-1,-1,
  // 		  "met_high_1019_6j30_nm1");
  // draw_from_trees("met","ht40>1000&&num_csvm_jets30>1&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt50>=8","(weightppb*5000)",
  // 		  ";E_{T}^{miss} [GeV];Events / 5 fb^{-1}", 10, 400, 1400.,
  // 		  "plotSig:",-1,-1,
  // 		  "met_high_1019_8j50_nm1");
  // draw_from_trees("met","ht40>1000&&num_csvm_jets30>1&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>=6","(weightppb*5000)",
  // 		  ";E_{T}^{miss} [GeV];Events / 5 fb^{-1}", 25, 0, 800.,
  // 		  "plotSig:plotLog",-1,-1,
  // 		  "met_1019_6j30_nm1");
  // draw_from_trees("met","ht40>1000&&num_csvm_jets30>1&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt50>=8","(weightppb*5000)",
  // 		  ";E_{T}^{miss} [GeV];Events / 5 fb^{-1}", 25, 0, 800.,
  // 		  "plotSig:plotLog",-1,-1,
  // 		  "met_1019_8j50_nm1");

  // draw_from_trees("ht40","met>400&&num_csvm_jets30>1&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>=6","(weightppb*5000)",
  // 		  ";H_{T} [GeV];Events / 5 fb^{-1}", 25, 200, 4200,
  // 		  "plotSig:",-1,-1,
  // 		  "ht40_1019_6j30_nm1");
  // draw_from_trees("ht40","met>400&&num_csvm_jets30>1&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt50>=8","(weightppb*5000)",
  // 		  ";H_{T} [GeV];Events / 5 fb^{-1}", 25, 200, 4200.,
  // 		  "plotSig:",-1,-1,
  // 		  "ht40_1019_8j50_nm1");
  // draw_from_trees("fatpT30_MJ","met>400&&num_csvm_jets30>1&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>=6","(weightppb*5000)",
  // 		  ";M_{J} [GeV];Events / 5 fb^{-1}", 20, 0., 1000.,
  // 		  "plotSig:",-1,-1,
  // 		  "fatpT30_MJ_1019_6j30_nm1");
  // draw_from_trees("fatpT30_MJ","met>400&&num_csvm_jets30>1&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt50>=8","(weightppb*5000)",
  // 		  ";M_{J} [GeV];Events / 5 fb^{-1}", 20, 0., 1000.,
  // 		  "plotSig:",-1,-1,
  // 		  "fatpT30_MJ_1019_8j50_nm1");

  // draw_from_trees("min_delta_phi_met","met<400&&ht30>1000&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>9","(weightppb*5000)",
  //   		  ";#Delta#Phi^{min};Events / 20 fb^{-1}", 32, 0., 3.2,
  //  		  "plotSig:",-1,-1,
  //   		  "min_delta_phi_met_1026_metl400_10j30");
  // draw_from_trees("min_delta_phi_met","met<400&&ht30>1000&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&jet4_pt>200","(weightppb*5000)",
  //   		  ";#Delta#Phi^{min};Events / 20 fb^{-1}", 32, 0., 3.2,
  //  		  "plotSig:",-1,-1,
  //   		  "min_delta_phi_met_1026_metl400_4j200");


  // draw_from_trees("min_delta_phi_met_N","met<400&&ht30>1000&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&jet4_pt>200","(weightppb*5000)",
  //   		  ";#Delta#Phi^{min}_{N};Events / 20 fb^{-1}", 20, 0., 40,
  //  		  "plotSig:",-1,-1,
  //   		  "min_delta_phi_met_N_1025_metl400_ht1000_4b200"); 
  // draw_from_trees("min_delta_phi_met_N","met<400&&ht30>1000&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&jet4_pt>150","(weightppb*5000)",
  //   		  ";#Delta#Phi^{min}_{N};Events / 20 fb^{-1}", 20, 0., 40,
  //  		  "plotSig:",-1,-1,
  //   		  "min_delta_phi_met_N_1025_metl400_ht1000_4b150"); 
  // draw_from_trees("min_delta_phi_met_N","met<400&&ht30>1000&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>9","(weightppb*5000)",
  //   		  ";#Delta#Phi^{min}_{N};Events / 20 fb^{-1}", 20, 0., 40,
  //  		  "plotSig:",-1,-1,
  //   		  "min_delta_phi_met_N_1025_metl400_ht1000_10b30"); 
  // draw_from_trees("min_delta_phi_met_N","met<400&&ht30>1000&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt50>7","(weightppb*5000)",
  //   		  ";#Delta#Phi^{min}_{N};Events / 20 fb^{-1}", 20, 0., 40,
  //  		  "plotSig:",-1,-1,
  //   		  "min_delta_phi_met_N_1025_metl400_ht1000_8b30"); 

  // draw_from_trees("mht","ht30>1000&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>9&&min_delta_phi_met_N>4&&num_csvm_jets30>1","(weightppb*5000)",
  // 		  ";H_{T}^{miss} [GeV];Events / 5 fb^{-1}", 15, 0, 1200.,
  // 		  "plotSig:plotLog",-1,-1,
  // 		  "mht_1026_10j30");
  // draw_from_trees("mht","ht30>1000&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&jet4_pt>200&&min_delta_phi_met_N>4&&num_csvm_jets30>1","(weightppb*5000)",
  // 		  ";H_{T}^{miss} [GeV];Events / 5 fb^{-1}", 15, 0, 1200.,
  // 		  "plotSig:plotLog",-1,-1,
  // 		  "mht_1026_4j200");
  // draw_from_trees("met_over_sqrt_ht30","ht30>1000&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>9&&min_delta_phi_met_N>4&&num_csvm_jets30>1","(weightppb*5000)",
  // 		 ";E_{T}^{miss}/#sqrt{H_{T}} [GeV^{1/2}];Events / 20 fb^{-1}", 10, 0., 50.,
  // 		 "plotSig:plotLog",-1,-1,
  // 		 "met_over_sqrt_ht30_10j30_1026");  
  // draw_from_trees("met_over_sqrt_ht30","ht30>1000&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&jet4_pt>200&&min_delta_phi_met_N>4&&num_csvm_jets30>1","(weightppb*5000)",
  // 		 ";E_{T}^{miss}/#sqrt{H_{T}} [GeV^{1/2}];Events / 20 fb^{-1}", 10, 0., 50.,
  // 		 "plotSig:plotLog",-1,-1,
  // 		 "met_over_sqrt_ht30_4j200_1026");  
  // draw_from_trees("min_delta_phi_met","met<400&&ht30>1000&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&jet4_pt>200&&num_csvm_jets30>1","(weightppb*5000)",
  //   		  ";#Delta#phi^{min};Events / 20 fb^{-1}", 32, 0., 3.2,
  //  		  "plotSig:",-1,-1,
  //   		  "min_delta_phi_met_1029_4j200_lowmet");
  // draw_from_trees("min_delta_phi_met_N","met<400&&ht30>1000&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&jet4_pt>200&&num_csvm_jets30>1","(weightppb*5000)",
  //   		  ";#Delta#phi^{min}_{N};Events / 20 fb^{-1}", 20, 0., 40,
  //  		  "plotSig:",-1,-1,
  //   		  "min_delta_phi_met_N_1029_4j200_lowmet"); 
  // draw_from_trees("min_delta_phi_met","met<400&&ht30>1000&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt50>7&&num_csvm_jets30>1","(weightppb*5000)",
  //   		  ";#Delta#phi^{min};Events / 20 fb^{-1}", 32, 0., 3.2,
  //  		  "plotSig:",-1,-1,
  //   		  "min_delta_phi_met_1029_8j50_lowmet");
  // draw_from_trees("min_delta_phi_met_N","met<400&&ht30>1000&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt50>7&&num_csvm_jets30>1","(weightppb*5000)",
  //   		  ";#Delta#phi^{min}_{N};Events / 20 fb^{-1}", 20, 0., 40,
  //  		  "plotSig:",-1,-1,
  //   		  "min_delta_phi_met_N_1029_8j50_lowmet"); 

  // draw_from_trees("min_delta_phi_met","met>200&&met<400&&ht30>1000&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>6&&num_csvm_jets30>1","(weightppb*5000)",
  //   		  ";#Delta#phi^{min};Events / 20 fb^{-1}", 32, 0., 3.2,
  //  		  "plotSig:",-1,-1,
  //   		  "min_delta_phi_met_1030_6j30_lowmet");
  // draw_from_trees("min_delta_phi_met_N","met>200&&met<400&&ht30>1000&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>6&&num_csvm_jets30>1","(weightppb*5000)",
  //   		  ";#Delta#phi^{min}_{N};Events / 20 fb^{-1}", 20, 0., 40,
  //  		  "plotSig:",-1,-1,
  //   		  "min_delta_phi_met_N_1030_6j30_lowmet"); 

  // draw_from_trees("mht/sqrt(ht50)","ht30>1000&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>3&&num_csvm_jets30>1&&min_delta_phi_met_N>4","(weightppb*5000)",
  //   		  ";H_{T}^{miss}/#sqrt{H_{T}} [GeV^{1/2}];Events / 5 fb^{-1}", 25, 0., 50,
  //  		  "plotSig:plotLog",-1,-1,
  //   		  "mht_over_sqrt_ht50_1102_nm1_ht30");
  // draw_from_trees("mht","ht30>1000&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>3&&num_csvm_jets30>1&&min_delta_phi_met_N>4","(weightppb*5000)",
  //   		  ";H_{T}^{miss} [GeV];Events / 5 fb^{-1}", 30, 0., 1500,
  //  		  "plotSig:plotLog",-1,-1,
  //   		  "mht_1102_nm1_ht30");

  // Run on skims

  // 50 ns!
  // draw_from_trees("num_reco_veto_electrons+num_reco_veto_muons","met>400&&ht30>1000&&num_csvm_jets>1&&min_delta_phi_met_N>4&&num_jets_pt30>6","(weightppb*5000)",
  // 		  ";n_{e+#mu} (p_{T} > 10 GeV);Events / 5 fb^{-1}", 6, 0, 6,
  // 		  "plotSig",-1,-1,
  // 		  "num_reco_veto_leptons_1029_2b_6j30_50ns"); 
  // draw_from_trees("num_reco_veto_electrons+num_reco_veto_muons","met>400&&ht30>1000&&num_csvm_jets>1&&min_delta_phi_met_N>4&&num_jets_pt30>6","(weightppb*5000)",
  // 		  ";n_{e+#mu} (p_{T} > 10 GeV);Events / 5 fb^{-1}", 6, 0, 6,
  // 		  "plotSig",-1,-1,
  // 		  "num_reco_veto_leptons_1029_2b_6j30_50ns"); 

  // draw_from_trees("num_reco_veto_electrons+num_reco_veto_muons","met>400&&ht30>1000&&num_csvm_jets>1&&min_delta_phi_met_N>4&&num_jets_pt30>9","(weightppb*5000)",
  // 		  ";n_{e+#mu} (p_{T} > 10 GeV);Events / 5 fb^{-1}", 6, 0, 6,
  // 		  "plotSig",-1,-1,
  // 		  "num_reco_veto_leptons_1029_2b_10j30_50ns"); 
  // draw_from_trees("num_reco_veto_electrons+num_reco_veto_muons","met>400&&ht30>1000&&num_csvm_jets>1&&min_delta_phi_met_N>4&&jet3_pt>300","(weightppb*5000)",
  // 		  ";n_{e+#mu} (p_{T} > 10 GeV);Events / 5 fb^{-1}", 6, 0, 6,
  // 		  "plotSig",-1,-1,
  // 		  "num_reco_veto_leptons_1029_2b_3j300_50ns"); 

  //25 ns

  // min_mT
  draw_from_trees("min_mTWB_Wmass","met>400&&ht30>1000&&num_reco_veto_muons+num_reco_veto_electrons==0&&num_jets_pt30>3&&num_csvm_jets30>1&&min_delta_phi_met_N>4","(weightppb*5000)",
    		  ";m_{T}^{min} (b, MET) [GeV];Events / 20 fb^{-1}", 20, 0., 1000.,
   		  "plotSig:plotLog",-1,-1,
    		  "min_mTWB_Wmass_1105_4j30");
  draw_from_trees("min_mTWB_Wmass","met>400&&ht30>1000&&num_reco_veto_muons+num_reco_veto_electrons==0&&num_jets_pt30>5&&num_csvm_jets30>1&&min_delta_phi_met_N>4","(weightppb*5000)",
    		  ";m_{T}^{min} (b, MET) [GeV];Events / 20 fb^{-1}", 20, 0., 1000.,
   		  "plotSig:plotLog",-1,-1,
    		  "min_mTWB_Wmass_1105_6j30");
  draw_from_trees("min_mTWB_Wmass","met>400&&ht30>1000&&num_reco_veto_muons+num_reco_veto_electrons==0&&num_jets_pt50>7&&num_csvm_jets30>1&&min_delta_phi_met_N>4","(weightppb*5000)",
    		  ";m_{T}^{min} (b, MET) [GeV];Events / 20 fb^{-1}", 20, 0., 1000.,
   		  "plotSig:",-1,-1,
    		  "min_mTWB_Wmass_1105_8j50");
  draw_from_trees("min_mTWB_Wmass","met>400&&ht30>1000&&num_reco_veto_muons+num_reco_veto_electrons==0&&num_jets_pt50>7&&jet3_pt>100&&min_delta_phi_met_N>4","(weightppb*5000)",
    		  ";m_{T}^{min} (b, MET) [GeV];Events / 20 fb^{-1}", 20, 0., 1000.,
   		  "plotSig:",-1,-1,
    		  "min_mTWB_Wmass_1105_3j100");
  draw_from_trees("min_mTWB_Wmass","met>400&&ht30>1000&&num_reco_veto_muons+num_reco_veto_electrons==0&&num_jets_pt50>7&&jet3_pt>300&&min_delta_phi_met_N>4","(weightppb*5000)",
    		  ";m_{T}^{min} (b, MET) [GeV];Events / 20 fb^{-1}", 10, 0., 1000.,
   		  "plotSig:",-1,-1,
    		  "min_mTWB_Wmass_1105_3j300");
  draw_from_trees("min_mTWB_Wmass","met>200&&met<400&&ht30>1000&&num_reco_veto_muons+num_reco_veto_electrons==0&&num_jets_pt30>3&&num_csvm_jets30>1&&min_delta_phi_met_N>4","(weightppb*5000)",
    		  ";m_{T}^{min} (b, MET) [GeV];Events / 20 fb^{-1}", 20, 0., 1000.,
   		  "plotSig:plotLog",-1,-1,
    		  "min_mTWB_Wmass_1105_4j30_modmet");
  draw_from_trees("min_mTWB_Wmass","met>200&&met<400&&ht30>1000&&num_reco_veto_muons+num_reco_veto_electrons==0&&num_jets_pt30>5&&num_csvm_jets30>1&&min_delta_phi_met_N>4","(weightppb*5000)",
    		  ";m_{T}^{min} (b, MET) [GeV];Events / 20 fb^{-1}", 20, 0., 1000.,
   		  "plotSig:plotLog",-1,-1,
    		  "min_mTWB_Wmass_1105_6j30_modmet");
  draw_from_trees("min_mTWB_Wmass","met>200&&met<400&&ht30>1000&&num_reco_veto_muons+num_reco_veto_electrons==0&&num_jets_pt50>7&&num_csvm_jets30>1&&min_delta_phi_met_N>4","(weightppb*5000)",
    		  ";m_{T}^{min} (b, MET) [GeV];Events / 20 fb^{-1}", 20, 0., 1000.,
   		  "plotSig:",-1,-1,
    		  "min_mTWB_Wmass_1105_8j50_modmet");
  draw_from_trees("min_mTWB_Wmass","met>200&&met<400&&ht30>1000&&num_reco_veto_muons+num_reco_veto_electrons==0&&num_jets_pt50>7&&jet3_pt>100&&min_delta_phi_met_N>4","(weightppb*5000)",
    		  ";m_{T}^{min} (b, MET) [GeV];Events / 20 fb^{-1}", 20, 0., 1000.,
   		  "plotSig:",-1,-1,
    		  "min_mTWB_Wmass_1105_3j100_modmet");
  draw_from_trees("min_mTWB_Wmass","met>200&&met<400&&ht30>1000&&num_reco_veto_muons+num_reco_veto_electrons==0&&num_jets_pt50>7&&jet3_pt>300&&min_delta_phi_met_N>4","(weightppb*5000)",
    		  ";m_{T}^{min} (b, MET) [GeV];Events / 20 fb^{-1}", 10, 0., 1000.,
   		  "plotSig:",-1,-1,
    		  "min_mTWB_Wmass_1105_3j300_modmet");


  // lepton definitions
  draw_from_trees("muon_dphi_met","muon_pt>0&&met>200&&met<400&&ht30>1000&&num_csvm_jets>1&&min_delta_phi_met_N>4&&num_jets_pt30>7&&num_csvm_jets30>1&&num_reco_veto_muons==1","(weightppb*5000)",
		  ";#Delta#phi (#mu, MET) [GeV];Events / 5 fb^{-1}", 16, 0, 3.2,
		  "plotSig:",-1,-1,
		  "muon_dphi_met_1105_8j30_1l_modmet"); 
  draw_from_trees("electron_dphi_met","electron_pt>0&&met>200&&met<400&&ht30>1000&&num_csvm_jets>1&&min_delta_phi_met_N>4&&num_jets_pt30>7&&num_csvm_jets30>1&&num_reco_veto_electrons==1","(weightppb*5000)",
		  ";#Delta#phi (e, MET) [GeV];Events / 5 fb^{-1}", 16, 0, 3.2,
		  "plotSig:",-1,-1,
		  "electron_dphi_met_1105_8j30_1l_modmet"); 

  draw_from_trees("muon_mT","muon_pt>0&&met>200&&met<400&&ht30>1000&&num_csvm_jets>1&&min_delta_phi_met_N>4&&num_jets_pt30>7&&num_csvm_jets30>1&&num_reco_veto_muons==1","(weightppb*5000)",
		  ";m_{T} (#mu, MET) [GeV];Events / 5 fb^{-1}", 20, 0, 400,
		  "plotSig:",-1,-1,
		  "muon_mT_1105_8j30_1l_modmet"); 
  draw_from_trees("electron_mT","electron_pt>0&&met>200&&met<400&&ht30>1000&&num_csvm_jets>1&&min_delta_phi_met_N>4&&num_jets_pt30>7&&num_csvm_jets30>1&&num_reco_veto_electrons==1","(weightppb*5000)",
		  ";m_{T} (e, MET) [GeV];Events / 5 fb^{-1}", 20, 0, 400,
		  "plotSig:",-1,-1,
		  "electron_mT_1105_8j30_1l_modmet"); 

  draw_from_trees("muon_dphi_met","muon_pt>0&&met>400&&ht30>1000&&num_csvm_jets>1&&min_delta_phi_met_N>4&&num_jets_pt30>7&&num_csvm_jets30>1&&num_reco_veto_muons==1","(weightppb*5000)",
		  ";#Delta#phi (#mu, MET) [GeV];Events / 5 fb^{-1}", 16, 0, 3.2,
		  "plotSig:",-1,-1,
		  "muon_dphi_met_1105_8j30_1l"); 
  draw_from_trees("electron_dphi_met","electron_pt>0&&met>400&&ht30>1000&&num_csvm_jets>1&&min_delta_phi_met_N>4&&num_jets_pt30>7&&num_csvm_jets30>1&&num_reco_veto_electrons==1","(weightppb*5000)",
		  ";#Delta#phi (e, MET) [GeV];Events / 5 fb^{-1}", 16, 0, 3.2,
		  "plotSig:",-1,-1,
		  "electron_dphi_met_1105_8j30_1l"); 

  draw_from_trees("muon_mT","muon_pt>0&&met>400&&ht30>1000&&num_csvm_jets>1&&min_delta_phi_met_N>4&&num_jets_pt30>7&&num_csvm_jets30>1&&num_reco_veto_muons==1","(weightppb*5000)",
		  ";m_{T} (#mu, MET) [GeV];Events / 5 fb^{-1}", 20, 0, 400,
		  "plotSig:",-1,-1,
		  "muon_mT_1105_8j30_1l"); 
  draw_from_trees("electron_mT","electron_pt>0&&met>400&&ht30>1000&&num_csvm_jets>1&&min_delta_phi_met_N>4&&num_jets_pt30>7&&num_csvm_jets30>1&&num_reco_veto_electrons==1","(weightppb*5000)",
		  ";m_{T} (e, MET) [GeV];Events / 5 fb^{-1}", 20, 0, 400,
		  "plotSig:",-1,-1,
		  "electron_mT_1105_8j30_1l"); 

  draw_from_trees("muon_dphi_met","muon_pt>0&&met>400&&ht30>1000&&num_csvm_jets>1&&min_delta_phi_met_N>4&&num_jets_pt30>7&&num_csvm_jets30>1","(weightppb*5000)",
		  ";#Delta#phi (#mu, MET) [GeV];Events / 5 fb^{-1}", 16, 0, 3.2,
		  "plotSig:",-1,-1,
		  "muon_dphi_met_1105_8j30"); 
  draw_from_trees("electron_dphi_met","electron_pt>0&&met>400&&ht30>1000&&num_csvm_jets>1&&min_delta_phi_met_N>4&&num_jets_pt30>7&&num_csvm_jets30>1","(weightppb*5000)",
		  ";#Delta#phi (e, MET) [GeV];Events / 5 fb^{-1}", 16, 0, 3.2,
		  "plotSig:",-1,-1,
		  "electron_dphi_met_1105_8j30"); 

  draw_from_trees("muon_mT","muon_pt>0&&met>400&&ht30>1000&&num_csvm_jets>1&&min_delta_phi_met_N>4&&num_jets_pt30>7&&num_csvm_jets30>1","(weightppb*5000)",
		  ";m_{T} (#mu, MET) [GeV];Events / 5 fb^{-1}", 20, 0, 400,
		  "plotSig:",-1,-1,
		  "muon_mT_1105_8j30"); 
  draw_from_trees("electron_mT","electron_pt>0&&met>400&&ht30>1000&&num_csvm_jets>1&&min_delta_phi_met_N>4&&num_jets_pt30>7&&num_csvm_jets30>1","(weightppb*5000)",
		  ";m_{T} (e, MET) [GeV];Events / 5 fb^{-1}", 20, 0, 400,
		  "plotSig:",-1,-1,
		  "electron_mT_1105_8j30"); 

  draw_from_trees("num_reco_veto_electrons+num_reco_veto_muons","met>400&&ht30>1000&&num_csvm_jets>1&&min_delta_phi_met_N>4&&num_jets_pt30>7&&num_csvm_jets30>1","(weightppb*5000)",
		  ";n_{e+#mu} (p_{T} > 10 GeV);Events / 5 fb^{-1}", 6, 0, 6,
		  "plotSig:",-1,-1,
		  "num_reco_veto_leptons_1105_8j30"); 
  draw_from_trees("num_reco_veto_electrons_mT100+num_reco_veto_muons_mT100","met>400&&ht30>1000&&num_csvm_jets>1&&min_delta_phi_met_N>4&&num_jets_pt30>7&&num_csvm_jets30>1","(weightppb*5000)",
		  ";n_{e+#mu} (p_{T} > 10 GeV);Events / 5 fb^{-1}", 6, 0, 6,
		  "plotSig:",-1,-1,
		  "num_reco_veto_leptons_mT100_1105_8j30"); 
  draw_from_trees("num_reco_veto_electrons_mT100_orth+num_reco_veto_muons_mT100_orth","met>400&&ht30>1000&&num_csvm_jets>1&&min_delta_phi_met_N>4&&num_jets_pt30>7&&num_csvm_jets30>1","(weightppb*5000)",
		  ";n_{e+#mu} (p_{T} > 10 GeV);Events / 5 fb^{-1}", 6, 0, 6,
		  "plotSig:",-1,-1,
		  "num_reco_veto_leptons_mT100_orth_1105_8j30"); 





  // central fat jets?
  draw_from_trees("fatpT30central_MJ","met>400&&num_csvm_jets30>1&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>=4","(weightppb*5000)",
  		  ";M_{J} [GeV];Events / 5 fb^{-1}", 30, 0., 1500.,
  		  "plotSig:plotLog",-1,-1,
  		  "fatpT30central_MJ_1105_nm1");
  draw_from_trees("fatpT30central_MJ","met>400&&num_csvm_jets30>1&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>=6","(weightppb*5000)",
  		  ";M_{J} [GeV];Events / 5 fb^{-1}", 30, 0., 1500.,
  		  "plotSig:plotLog",-1,-1,
  		  "fatpT30central_MJ_1105_6j30");
  draw_from_trees("fatpT30central_MJ","met>400&&num_csvm_jets30>1&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>=8","(weightppb*5000)",
  		  ";M_{J} [GeV];Events / 5 fb^{-1}", 15, 0., 1500.,
  		  "plotSig:",-1,-1,
  		  "fatpT30central_MJ_1105_8j30");
  draw_from_trees("fatpT30central_MJ","met>400&&num_csvm_jets30>1&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>=10","(weightppb*5000)",
  		  ";M_{J} [GeV];Events / 5 fb^{-1}", 15, 0., 1500.,
  		  "plotSig:",-1,-1,
  		  "fatpT30central_MJ_1105_10j30");
  draw_from_trees("num_fatpT30_jets_pt100","met>400&&num_csvm_jets30>1&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>=4","(weightppb*5000)",
  		  ";n_{FJ} (p_{T}>100 GeV);Events / 5 fb^{-1}", 8, 0., 8.,
  		  "plotSig:plotLog",-1,-1,
  		  "num_fatpT30_jets_pt100_1105_nm1");
  draw_from_trees("num_fatpT30_jets_pt100","met>400&&num_csvm_jets30>1&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>=6","(weightppb*5000)",
  		  ";n_{FJ} (p_{T}>100 GeV);Events / 5 fb^{-1}", 8, 0., 8.,
  		  "plotSig:",-1,-1,
  		  "num_fatpT30_jets_pt100_1105_6j30");
  draw_from_trees("num_fatpT30_jets_pt100","met>400&&num_csvm_jets30>1&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>=8","(weightppb*5000)",
  		  ";n_{FJ} (p_{T}>100 GeV);Events / 5 fb^{-1}", 8, 0., 8.,
  		  "plotSig:",-1,-1,
  		  "num_fatpT30_jets_pt100_1105_8j30");
  draw_from_trees("num_fatpT30_jets_pt100","met>400&&num_csvm_jets30>1&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>=10","(weightppb*5000)",
  		  ";n_{FJ} (p_{T}>100 GeV);Events / 5 fb^{-1}", 8, 0., 8.,
  		  "plotSig:",-1,-1,
  		  "num_fatpT30_jets_pt100_1105_10j30");
  draw_from_trees("num_fatpT30central_jets_pt100","met>400&&num_csvm_jets30>1&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>=4","(weightppb*5000)",
  		  ";n_{FJ} (p_{T}>100 GeV);Events / 5 fb^{-1}", 8, 0., 8.,
  		  "plotSig:plotLog",-1,-1,
  		  "num_fatpT30central_jets_pt100_1105_nm1");
  draw_from_trees("num_fatpT30central_jets_pt100","met>400&&num_csvm_jets30>1&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>=6","(weightppb*5000)",
  		  ";n_{FJ} (p_{T}>100 GeV);Events / 5 fb^{-1}", 8, 0., 8.,
  		  "plotSig:",-1,-1,
  		  "num_fatpT30central_jets_pt100_1105_6j30");
  draw_from_trees("num_fatpT30central_jets_pt100","met>400&&num_csvm_jets30>1&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>=8","(weightppb*5000)",
  		  ";n_{FJ} (p_{T}>100 GeV);Events / 5 fb^{-1}", 8, 0., 8.,
  		  "plotSig:",-1,-1,
  		  "num_fatpT30central_jets_pt100_1105_8j30");
  draw_from_trees("num_fatpT30central_jets_pt100","met>400&&num_csvm_jets30>1&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>=10","(weightppb*5000)",
  		  ";n_{FJ} (p_{T}>100 GeV);Events / 5 fb^{-1}", 8, 0., 8.,
  		  "plotSig:",-1,-1,
  		  "num_fatpT30central_jets_pt100_1105_10j30");



  // compare MJ, HT
  draw_from_trees("fatpT30_MJ","met>400&&num_csvm_jets30>1&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>=4","(weightppb*5000)",
  		  ";M_{J} [GeV];Events / 5 fb^{-1}", 30, 0., 1500.,
  		  "plotSig:plotLog",-1,-1,
  		  "fatpT30_MJ_1104_nm1");
  draw_from_trees("fatpT30_MJ","met>400&&num_csvm_jets30>1&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>=6","(weightppb*5000)",
  		  ";M_{J} [GeV];Events / 5 fb^{-1}", 30, 0., 1500.,
  		  "plotSig:plotLog",-1,-1,
  		  "fatpT30_MJ_1104_6j30");
  draw_from_trees("fatpT30_MJ","met>400&&num_csvm_jets30>1&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>=8","(weightppb*5000)",
  		  ";M_{J} [GeV];Events / 5 fb^{-1}", 15, 0., 1500.,
  		  "plotSig:",-1,-1,
  		  "fatpT30_MJ_1104_8j30");
  draw_from_trees("fatpT30_MJ","met>400&&num_csvm_jets30>1&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>=10","(weightppb*5000)",
  		  ";M_{J} [GeV];Events / 5 fb^{-1}", 15, 0., 1500.,
  		  "plotSig:",-1,-1,
  		  "fatpT30_MJ_1104_10j30");
  draw_from_trees("ht30","met>400&&num_csvm_jets30>1&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>=4","(weightppb*5000)",
  		  ";H_{T} [GeV];Events / 5 fb^{-1}", 30, 0., 4500.,
  		  "plotSig:plotLog",-1,-1,
  		  "ht30_1104_nm1");
  draw_from_trees("ht30","met>400&&num_csvm_jets30>1&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>=6","(weightppb*5000)",
  		  ";H_{T} [GeV];Events / 5 fb^{-1}", 30, 0., 4500.,
  		  "plotSig:plotLog",-1,-1,
  		  "ht30_1104_6j30");
  draw_from_trees("ht30","met>400&&num_csvm_jets30>1&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>=8","(weightppb*5000)",
  		  ";H_{T} [GeV];Events / 5 fb^{-1}", 15, 0., 4500.,
  		  "plotSig:",-1,-1,
  		  "ht30_1104_8j30");
  draw_from_trees("ht30","met>400&&num_csvm_jets30>1&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>=10","(weightppb*5000)",
  		  ";H_{T} [GeV];Events / 5 fb^{-1}", 15, 0., 4500.,
  		  "plotSig:",-1,-1,
  		  "ht30_1104_10j30");

  draw_from_trees("num_fatpT30_jets_pt100","met>400&&num_csvm_jets30>1&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>=4","(weightppb*5000)",
  		  ";n_{FJ} (p_{T}>100 GeV);Events / 5 fb^{-1}", 8, 0., 8.,
  		  "plotSig:plotLog",-1,-1,
  		  "num_fatpT30_jets_pt100_1104_nm1");
  draw_from_trees("num_fatpT30_jets_pt100","met>400&&num_csvm_jets30>1&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>=6","(weightppb*5000)",
  		  ";n_{FJ} (p_{T}>100 GeV);Events / 5 fb^{-1}", 8, 0., 8.,
  		  "plotSig:",-1,-1,
  		  "num_fatpT30_jets_pt100_1104_6j30");
  draw_from_trees("num_fatpT30_jets_pt100","met>400&&num_csvm_jets30>1&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>=8","(weightppb*5000)",
  		  ";n_{FJ} (p_{T}>100 GeV);Events / 5 fb^{-1}", 8, 0., 8.,
  		  "plotSig:",-1,-1,
  		  "num_fatpT30_jets_pt100_1104_8j30");
  draw_from_trees("num_fatpT30_jets_pt100","met>400&&num_csvm_jets30>1&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>=10","(weightppb*5000)",
  		  ";n_{FJ} (p_{T}>100 GeV);Events / 5 fb^{-1}", 8, 0., 8.,
  		  "plotSig:",-1,-1,
  		  "num_fatpT30_jets_pt100_1104_10j30");


  draw_from_trees("highest_mJ",baselineMJ,"(weightppb*5000)",
  		  ";m_{FJ} (FJ 1) [GeV];Events / 5 fb^{-1}", 20, 0., 1000.,
  		  "plotSig:plotLog",-1,-1,
  		  "highest_mJ_4j30_1104_nm1");
  draw_from_trees("scnd_highest_mJ",baselineMJ,"(weightppb*5000)",
  		  ";m_{FJ} (FJ 2) [GeV];Events / 5 fb^{-1}", 8, 0., 400.,
  		  "plotSig:plotLog",-1,-1,
  		  "scnd_highest_mJ_4j30_1104_nm1");
  draw_from_trees("thrd_highest_mJ",baselineMJ,"(weightppb*5000)",
  		  ";m_{FJ} (FJ 3) [GeV];Events / 5 fb^{-1}", 8, 0., 400.,
  		  "plotSig:plotLog",-1,-1,
  		  "thrd_highest_mJ_4j30_1104_nm1");
  draw_from_trees("frth_highest_mJ",baselineMJ,"(weightppb*5000)",
  		  ";m_{FJ} (FJ 4) [GeV];Events / 5 fb^{-1}", 8, 0., 400.,
  		  "plotSig:plotLog",-1,-1,
  		  "frth_highest_mJ_4j30_1104_nm1");
  draw_from_trees("highest_mJ",baselineMJ+"num_jets_pt30>=6","(weightppb*5000)",
  		  ";m_{FJ} (FJ 1) [GeV];Events / 5 fb^{-1}", 20, 0., 1000.,
  		  "plotSig",-1,-1,
  		  "highest_mJ_6j30_1104_nm1");
  draw_from_trees("scnd_highest_mJ",baselineMJ+"num_jets_pt30>=6","(weightppb*5000)",
  		  ";m_{FJ} (FJ 2) [GeV];Events / 5 fb^{-1}", 8, 0., 400.,
  		  "plotSig",-1,-1,
  		  "scnd_highest_mJ_6j30_1104_nm1");
  draw_from_trees("thrd_highest_mJ",baselineMJ+"num_jets_pt30>=6","(weightppb*5000)",
  		  ";m_{FJ} (FJ 3) [GeV];Events / 5 fb^{-1}", 8, 0., 400.,
  		  "plotSig",-1,-1,
  		  "thrd_highest_mJ_6j30_1104_nm1");
  draw_from_trees("frth_highest_mJ",baselineMJ+"num_jets_pt30>=6","(weightppb*5000)",
  		  ";m_{FJ} (FJ 4) [GeV];Events / 5 fb^{-1}", 8, 0., 400.,
  		  "plotSig",-1,-1,
  		  "frth_highest_mJ_6j30_1104_nm1");
  draw_from_trees("highest_mJ",baselineMJ+"num_jets_pt30>=8","(weightppb*5000)",
  		  ";m_{FJ} (FJ 1) [GeV];Events / 5 fb^{-1}", 20, 0., 1000.,
  		  "plotSig",-1,-1,
  		  "highest_mJ_8j30_1104_nm1");
  draw_from_trees("scnd_highest_mJ",baselineMJ+"num_jets_pt30>=8","(weightppb*5000)",
  		  ";m_{FJ} (FJ 2) [GeV];Events / 5 fb^{-1}", 8, 0., 400.,
  		  "plotSig",-1,-1,
  		  "scnd_highest_mJ_8j30_1104_nm1");
  draw_from_trees("thrd_highest_mJ",baselineMJ+"num_jets_pt30>=8","(weightppb*5000)",
  		  ";m_{FJ} (FJ 3) [GeV];Events / 5 fb^{-1}", 8, 0., 400.,
  		  "plotSig",-1,-1,
  		  "thrd_highest_mJ_8j30_1104_nm1");
  draw_from_trees("frth_highest_mJ",baselineMJ+"num_jets_pt30>=8","(weightppb*5000)",
  		  ";m_{FJ} (FJ 4) [GeV];Events / 5 fb^{-1}", 8, 0., 400.,
  		  "plotSig",-1,-1,
  		  "frth_highest_mJ_8j30_1104_nm1");

  draw_from_trees("fatpT30_MJ/num_fatpT30_jets",baselineMJ,"(weightppb*5000)",
  		  ";<m_{FJ}> [GeV];Events / 5 fb^{-1}", 20, 0., 1000.,
  		  "plotSig:plotLog",-1,-1,
  		  "avg_mJ_4j30_1104_nm1");
  draw_from_trees("fatpT30_MJ/num_fatpT30_jets",baselineMJ+"num_jets_pt30>=6","(weightppb*5000)",
  		  ";<m_{FJ}> [GeV];Events / 5 fb^{-1}", 20, 0., 1000.,
  		  "plotSig:plotLog",-1,-1,
  		  "avg_mJ_6j30_1104_nm1");
  draw_from_trees("fatpT30_MJ/num_fatpT30_jets",baselineMJ+"num_jets_pt30>=8","(weightppb*5000)",
  		  ";<m_{FJ}> [GeV];Events / 5 fb^{-1}", 20, 0., 1000.,
  		  "plotSig:",-1,-1,
  		  "avg_mJ_8j30_1104_nm1");


  // ATLAS n-1 selection
  draw_from_trees("mht/sqrt(ht50)","num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt50>7&&num_csvm_jets30>1","(weightppb*5000)",
    		  ";H_{T}^{miss}/#sqrt{H_{T}} [GeV^{1/2}];Events / 5 fb^{-1}", 25, 0., 50,
   		  "plotSig:plotLog",-1,-1,
    		  "mht_over_sqrt_ht50_1102_ATLAS_2b");
  draw_from_trees("met/sqrt(ht50)","num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt50>7&&num_csvm_jets30>1","(weightppb*5000)",
    		  ";E_{T}^{miss}/#sqrt{H_{T}} [GeV^{1/2}];Events / 5 fb^{-1}", 25, 0., 50,
   		  "plotSig:plotLog",-1,-1,
    		  "met_over_sqrt_ht50_1102_ATLAS_2b");


  // MJ n-1 selection
  draw_from_trees("met","min_delta_phi_met_N>4&&ht30>1000&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>3&&num_csvm_jets30>1","(weightppb*5000)",
    		  ";E_{T}^{miss} [GeV];Events / 5 fb^{-1}", 30, 0., 1500,
   		  "plotSig:plotLog",-1,-1,
    		  "met_1102_nm1_ht30");
  draw_from_trees("ht30","min_delta_phi_met_N>4&&met>400&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>3&&num_csvm_jets30>1","(weightppb*5000)",
    		  ";H_{T} [GeV];Events / 5 fb^{-1}", 40, 0., 4000,
   		  "plotSig:plotLog",-1,-1,
    		  "ht30_1102_nm1_ht30");
  draw_from_trees("min_delta_phi_met","met>400&&ht30>1000&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>3&&num_csvm_jets30>1","(weightppb*5000)",
    		  ";#Delta#phi^{min};Events / 5 fb^{-1}", 32, 0., 3.2,
   		  "plotSig:plotLog",-1,-1,
    		  "min_delta_phi_met_1102_nm1_ht30");
  draw_from_trees("min_delta_phi_met_N","met>400&&ht30>1000&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>3&&num_csvm_jets30>1","(weightppb*5000)",
    		  ";#Delta#phi^{min}_{N};Events / 5 fb^{-1}", 20, 0., 40,
   		  "plotSig:plotLog",-1,-1,
    		  "min_delta_phi_met_N_1102_nm1_ht30"); 
  draw_from_trees("num_reco_veto_electrons+num_reco_veto_muons","met>400&&ht30>1000&&num_csvm_jets>1&&min_delta_phi_met_N>4&&num_jets_pt30>3&&num_csvm_jets30>1","(weightppb*5000)",
		  ";n_{e+#mu} (p_{T} > 10 GeV);Events / 5 fb^{-1}", 6, 0, 6,
		  "plotSig:plotLog",-1,-1,
		  "num_reco_veto_leptons_1102_nm1_ht30"); 
  draw_from_trees("num_csvm_jets30","met>400&&ht30>1000&&num_reco_veto_electrons==0&&num_reco_veto_muons==0&&min_delta_phi_met_N>4&&num_jets_pt30>3","(weightppb*5000)",
		  ";n_{CSVM} (p_{T} > 30 GeV);Events / 5 fb^{-1}", 6, 0, 6,
		  "plotSig:plotLog",-1,-1,
		  "num_csvm_jets30_1102_nm1_ht30");
  draw_from_trees("num_jets_pt30","met>400&&ht30>1000&&num_reco_veto_electrons==0&&num_reco_veto_muons==0&&min_delta_phi_met_N>4&&num_csvm_jets30>1","(weightppb*5000)",
		  ";n_{jets} (p_{T} > 30 GeV);Events / 5 fb^{-1}", 15, 0, 15,
		  "plotSig:plotLog",-1,-1,
		  "num_jets_pt30_1102_nm1_ht30"); 

  draw_from_trees("met","min_delta_phi_met_N>4&&fatpT30_MJ>500&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>3&&num_csvm_jets30>1","(weightppb*5000)",
    		  ";E_{T}^{miss} [GeV];Events / 5 fb^{-1}", 30, 0., 1500,
   		  "plotSig:plotLog",-1,-1,
    		  "met_1102_nm1_MJ500");
  draw_from_trees("min_delta_phi_met","met>400&&fatpT30_MJ>500&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>3&&num_csvm_jets30>1","(weightppb*5000)",
    		  ";#Delta#phi^{min};Events / 5 fb^{-1}", 32, 0., 3.2,
   		  "plotSig:plotLog",-1,-1,
    		  "min_delta_phi_met_1102_nm1_MJ500");
  draw_from_trees("min_delta_phi_met_N","met>400&&fatpT30_MJ>500&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>3&&num_csvm_jets30>1","(weightppb*5000)",
    		  ";#Delta#phi^{min}_{N};Events / 5 fb^{-1}", 20, 0., 40,
   		  "plotSig:plotLog",-1,-1,
    		  "min_delta_phi_met_N_1102_nm1_MJ500"); 
  draw_from_trees("num_reco_veto_electrons+num_reco_veto_muons","met>400&&fatpT30_MJ>500&&num_csvm_jets>1&&min_delta_phi_met_N>4&&num_jets_pt30>3&&num_csvm_jets30>1","(weightppb*5000)",
		  ";n_{e+#mu} (p_{T} > 10 GeV);Events / 5 fb^{-1}", 6, 0, 6,
		  "plotSig:plotLog",-1,-1,
		  "num_reco_veto_leptons_1102_nm1_MJ500"); 
  draw_from_trees("num_csvm_jets30","met>400&&fatpT30_MJ>500&&num_reco_veto_electrons==0&&num_reco_veto_muons==0&&min_delta_phi_met_N>4&&num_jets_pt30>3","(weightppb*5000)",
		  ";n_{CSVM} (p_{T} > 30 GeV);Events / 5 fb^{-1}", 6, 0, 6,
		  "plotSig:plotLog",-1,-1,
		  "num_csvm_jets30_1102_nm1_MJ500");
  draw_from_trees("num_jets_pt30","met>400&&fatpT30_MJ>500&&num_reco_veto_electrons==0&&num_reco_veto_muons==0&&min_delta_phi_met_N>4&&num_csvm_jets30>1","(weightppb*5000)",
		  ";n_{jets} (p_{T} > 30 GeV);Events / 5 fb^{-1}", 15, 0, 15,
		  "plotSig:plotLog",-1,-1,
		  "num_jets_pt30_1102_nm1_MJ500"); 





  draw_from_trees("num_reco_veto_electrons+num_reco_veto_muons","met>400&&ht30>1000&&num_csvm_jets>1&&min_delta_phi_met_N>4&&num_jets_pt30>=8","(weightppb*5000)",
		  ";n_{e+#mu} (p_{T} > 10 GeV);Events / 5 fb^{-1}", 6, 0, 6,
		  "plotSig",-1,-1,
		  "num_reco_veto_leptons_1031_2b_8j30");
  draw_from_trees("num_reco_veto_electrons_mT100+num_reco_veto_muons_mT100","met>400&&ht30>1000&&num_csvm_jets>1&&min_delta_phi_met_N>4&&num_jets_pt30>=8","(weightppb*5000)",
		  ";n_{e+#mu} (p_{T} > 10 GeV);Events / 5 fb^{-1}", 6, 0, 6,
		  "plotSig",-1,-1,
		  "num_reco_veto_leptons_mT100_1031_2b_8j30");





  draw_from_trees("mT","met>400&&ht30>1000&&num_reco_veto_muons+num_reco_veto_electrons==1&&num_jets_pt30>3&&num_csvm_jets30>1","(weightppb*5000)",
    		  ";m_{T} (lep, MET) [GeV];Events / 20 fb^{-1}", 20, 0., 200.,
   		  "plotSig:plotLog",-1,-1,
    		  "mT_SL_1031_3j30");
  draw_from_trees("mT","met>400&&ht30>1000&&num_reco_veto_muons+num_reco_veto_electrons==1&&num_jets_pt30>5&&num_csvm_jets30>1","(weightppb*5000)",
    		  ";m_{T} (lep, MET) [GeV];Events / 20 fb^{-1}", 20, 0., 200.,
   		  "plotSig:",-1,-1,
    		  "mT_SL_1031_6j30");


  draw_from_trees("min_delta_phi_met","met>400&&ht30>1000&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>6&&num_csvm_jets30>1","(weightppb*5000)",
    		  ";#Delta#phi^{min};Events / 20 fb^{-1}", 32, 0., 3.2,
   		  "plotSig:",-1,-1,
    		  "min_delta_phi_met_1030_6j30_highmet");
  draw_from_trees("min_delta_phi_met_N","met>400&&ht30>1000&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>6&&num_csvm_jets30>1","(weightppb*5000)",
    		  ";#Delta#phi^{min}_{N};Events / 20 fb^{-1}", 20, 0., 40,
   		  "plotSig:",-1,-1,
    		  "min_delta_phi_met_N_1030_6j30_highmet"); 


  draw_from_trees("num_reco_veto_electrons+num_reco_veto_muons","met>400&&ht30>1000&&num_csvm_jets>1&&min_delta_phi_met_N>4&&jet3_pt>300","(weightppb*5000)",
		  ";n_{e+#mu} (p_{T} > 10 GeV);Events / 5 fb^{-1}", 6, 0, 6,
		  "plotSig",-1,-1,
		  "num_reco_veto_leptons_1029_2b_3j300_25ns"); 


  draw_from_trees("mht","ht30>1000&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>=10","(weightppb*5000)",
  		  ";H_{T}^{miss} [GeV];Events / 5 fb^{-1}", 10, 400, 1400.,
  		  "plotSig:",-1,-1,
  		  "mht_high_10j30_nobr_1029");
  draw_from_trees("mht","ht30>1000&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>=10&&num_csvm_jets30>1","(weightppb*5000)",
  		  ";H_{T}^{miss} [GeV];Events / 5 fb^{-1}", 10, 400, 1400.,
  		  "plotSig:",-1,-1,
  		  "mht_high_10j30_2bm_1029");

  draw_from_trees("ht30","met>400&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&jet3_pt>300","(weightppb*5000)",
  		  ";H_{T} [GeV];Events / 5 fb^{-1}", 20, 1000, 3000,
  		  "plotSig:",-1,-1,
  		  "ht30_high_1029_3j300_nobreq");
  draw_from_trees("ht30","met>400&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&jet3_pt>300&&num_csvm_jets30>1","(weightppb*5000)",
  		  ";H_{T} [GeV];Events / 5 fb^{-1}", 10, 1000, 3000,
  		  "plotSig:",-1,-1,
  		  "ht30_high_1029_3j300_2bm");

  draw_from_trees("min_delta_phi_met","met>400&&ht30>1000&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&jet4_pt>200&&num_csvm_jets30>1","(weightppb*5000)",
    		  ";#Delta#phi^{min};Events / 20 fb^{-1}", 32, 0., 3.2,
   		  "plotSig:",-1,-1,
    		  "min_delta_phi_met_1029_4j200_nm1");
  draw_from_trees("min_delta_phi_met_N","met>400&&ht30>1000&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&jet4_pt>200&&num_csvm_jets30>1","(weightppb*5000)",
    		  ";#Delta#phi^{min}_{N};Events / 20 fb^{-1}", 20, 0., 40,
   		  "plotSig:",-1,-1,
    		  "min_delta_phi_met_N_1029_4j200_nm1"); 
  draw_from_trees("min_delta_phi_met","met>400&&ht30>1000&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt100>=3&&num_csvm_jets30>1","(weightppb*5000)",
    		  ";#Delta#phi^{min};Events / 20 fb^{-1}", 32, 0., 3.2,
   		  "plotSig:",-1,-1,
    		  "min_delta_phi_met_1029_3j100_nm1");
  draw_from_trees("min_delta_phi_met_N","met>400&&ht30>1000&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt100>=3&&num_csvm_jets30>1","(weightppb*5000)",
    		  ";#Delta#phi^{min}_{N};Events / 20 fb^{-1}", 20, 0., 40,
   		  "plotSig:",-1,-1,
    		  "min_delta_phi_met_N_1029_3j100_nm1"); 

  draw_from_trees("jet1_pt",baselineHT+"met>600","(weightppb*5000)",
		  ";p_{T} (jet 1) [GeV];Events / 5 fb^{-1}", 12, 0, 1200.,
		  "plotSig:",-1,-1,
		  "jet1_pt_1029_met600");  
  draw_from_trees("jet2_pt",baselineHT+"met>600","(weightppb*5000)",
		  ";p_{T} (jet 2) [GeV];Events / 5 fb^{-1}", 10, 0, 800.,
		  "plotSig:",-1,-1,
		  "jet2_pt_1029_met600"); 
  draw_from_trees("jet3_pt",baselineHT+"met>600","(weightppb*5000)",
		  ";p_{T} (jet 3) [GeV];Events / 5 fb^{-1}", 10, 0, 500.,
		  "plotSig:",-1,-1,
		  "jet3_pt_1029_met600");
  draw_from_trees("jet4_pt",baselineHT+"met>600","(weightppb*5000)",
		  ";p_{T} (jet 4) [GeV];Events / 5 fb^{-1}", 10, 0, 500.,
		  "plotSig:",-1,-1,
		  "jet4_pt_1029_met600");

  draw_from_trees("num_csvm_jets30","met>400&&ht30>1000&&num_reco_veto_electrons==0&&num_reco_veto_muons==0&&min_delta_phi_met_N>4&&num_jets_pt100>=3","(weightppb*5000)",
		  ";n_{CSVM} (p_{T} > 30 GeV);Events / 5 fb^{-1}", 4, 2, 6,
		  "plotSig:",-1,-1,
		  "num_csvm_jets30_1029_2b_3j100");
  draw_from_trees("num_csvl_jets30","met>400&&ht30>1000&&num_reco_veto_electrons==0&&num_reco_veto_muons==0&&min_delta_phi_met_N>4&&num_jets_pt100>=3&&num_csvm_jets30>1","(weightppb*5000)",
		  ";n_{CSVL} (p_{T} > 30 GeV);Events / 5 fb^{-1}", 4, 2, 6,
		  "plotSig:",-1,-1,
		  "num_csvl_jets30_1029_2bm_3j100"); 
  draw_from_trees("met","num_csvm_jets30>1&&ht30>1000&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt50>=3","(weightppb*5000)",
  		  ";E_{T}^{miss} [GeV];Events / 5 fb^{-1}", 10, 400, 1400.,
  		  "plotSig:plotLog",-1,-1,
  		  "met_3j50_2b_1029");
  draw_from_trees("met","num_csvm_jets30>1&&ht30>1000&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt100>=3","(weightppb*5000)",
  		  ";E_{T}^{miss} [GeV];Events / 5 fb^{-1}", 10, 400, 1400.,
  		  "plotSig:plotLog",-1,-1,
  		  "met_3j100_2b_1029");
  draw_from_trees("met","num_csvm_jets30>2&&ht30>1000&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt100>=3","(weightppb*5000)",
  		  ";E_{T}^{miss} [GeV];Events / 5 fb^{-1}", 10, 400, 1400.,
  		  "plotSig:",-1,-1,
  		  "met_3j100_3b_1029");
  draw_from_trees("met","num_csvm_jets30>1&&num_csvl_jets30>3&&ht30>1000&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt100>=3","(weightppb*5000)",
  		  ";E_{T}^{miss} [GeV];Events / 5 fb^{-1}", 10, 400, 1400.,
  		  "plotSig:",-1,-1,
  		  "met_3j100_4b_1029");

  draw_from_trees("met","num_csvm_jets30>1&&ht30>1000&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&jet3_pt>300","(weightppb*5000)",
  		  ";E_{T}^{miss} [GeV];Events / 5 fb^{-1}", 10, 400, 1400.,
  		  "plotSig:",-1,-1,
  		  "met_3j300_2b_1029");
  draw_from_trees("met","num_csvm_jets30>2&&ht30>1000&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&jet3_pt>300","(weightppb*5000)",
  		  ";E_{T}^{miss} [GeV];Events / 5 fb^{-1}", 10, 400, 1400.,
  		  "plotSig:",-1,-1,
  		  "met_3j300_3b_1029");

  draw_from_trees("jet1_pt",baselineHT,"(weightppb*5000)",
		  ";p_{T} (jet 1) [GeV];Events / 5 fb^{-1}", 12, 0, 1200.,
		  "plotSig:",-1,-1,
		  "jet1_pt_1029_nm1");  
  draw_from_trees("jet2_pt",baselineHT,"(weightppb*5000)",
		  ";p_{T} (jet 2) [GeV];Events / 5 fb^{-1}", 10, 0, 800.,
		  "plotSig:",-1,-1,
		  "jet2_pt_1029_nm1"); 
  draw_from_trees("jet3_pt",baselineHT,"(weightppb*5000)",
		  ";p_{T} (jet 3) [GeV];Events / 5 fb^{-1}", 10, 0, 500.,
		  "plotSig:",-1,-1,
		  "jet3_pt_1029_nm1");
  draw_from_trees("jet4_pt",baselineHT,"(weightppb*5000)",
		  ";p_{T} (jet 4) [GeV];Events / 5 fb^{-1}", 10, 0, 500.,
		  "plotSig:",-1,-1,
		  "jet4_pt_1029_nm1");


  draw_from_trees("met","ht30>1000&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt50>=8","(weightppb*5000)",
  		  ";E_{T}^{miss} [GeV];Events / 5 fb^{-1}", 10, 400, 1400.,
  		  "plotSig:",-1,-1,
  		  "met_8j50_nobr_1029");
  draw_from_trees("met","ht30>1000&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt50>=8&&num_csvm_jets30>1","(weightppb*5000)",
  		  ";E_{T}^{miss} [GeV];Events / 5 fb^{-1}", 10, 400, 1400.,
  		  "plotSig:",-1,-1,
  		  "met_8j50_2bm_1029");

  draw_from_trees("min_delta_phi_met","met>400&&ht30>1000&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>3","(weightppb*5000)",
    		  ";#Delta#phi^{min};Events / 20 fb^{-1}", 32, 0., 3.2,
   		  "plotSig:plotLog",-1,-1,
    		  "min_delta_phi_met_1029_2b_nm1");
  draw_from_trees("min_delta_phi_met_N","met>400&&ht30>1000&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>3","(weightppb*5000)",
    		  ";#Delta#phi^{min}_{N};Events / 20 fb^{-1}", 20, 0., 40,
   		  "plotSig:plotLog",-1,-1,
    		  "min_delta_phi_met_N_1029_2b_nm1"); 
  draw_from_trees("num_reco_veto_electrons+num_reco_veto_muons","met>400&&ht30>1000&&num_csvm_jets>1&&min_delta_phi_met_N>4&&num_jets_pt30>3","(weightppb*5000)",
		  ";n_{e+#mu} (p_{T} > 10 GeV);Events / 5 fb^{-1}", 6, 0, 6,
		  "plotSig:plotLog",-1,-1,
		  "num_reco_veto_leptons_1029_2b_nm1"); 
  draw_from_trees("num_csvm_jets30","met>400&&ht30>1000&&num_reco_veto_electrons==0&&num_reco_veto_muons==0&&min_delta_phi_met_N>4&&num_jets_pt30>3","(weightppb*5000)",
		  ";n_{CSVM} (p_{T} > 30 GeV);Events / 5 fb^{-1}", 6, 0, 6,
		  "plotSig:plotLog",-1,-1,
		  "num_csvm_jets30_1029_2b_nm1"); 

  draw_from_trees("min_delta_phi_met","met>400&&ht30>1000&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>9","(weightppb*5000)",
    		  ";#Delta#phi^{min};Events / 20 fb^{-1}", 32, 0., 3.2,
   		  "plotSig:",-1,-1,
    		  "min_delta_phi_met_1026_metg400_10j30");
  draw_from_trees("min_delta_phi_met","met>400&&ht30>1000&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&jet4_pt>200","(weightppb*5000)",
    		  ";#Delta#phi^{min};Events / 20 fb^{-1}", 32, 0., 3.2,
   		  "plotSig:",-1,-1,
    		  "min_delta_phi_met_1026_metg400_4j200");

  draw_from_trees("num_reco_veto_electrons+num_reco_veto_muons","met>400&&ht30>1000&&num_csvm_jets>1&&min_delta_phi_met_N>4&&jet4_pt>200","(weightppb*5000)",
		  ";n_{e+#mu} (p_{T} > 10 GeV);Events / 5 fb^{-1}", 6, 0, 6,
		  "plotSig:",-1,-1,
		  "num_reco_veto_leptons_4j200_1025_nm1"); 
  draw_from_trees("num_reco_veto_electrons+num_reco_veto_muons","met>400&&ht30>1000&&num_csvm_jets>1&&min_delta_phi_met_N>4&&num_jets_pt30>=10","(weightppb*5000)",
		  ";n_{e+#mu} (p_{T} > 10 GeV);Events / 5 fb^{-1}", 6, 0, 6,
   		  "plotSig:",-1,-1,
    		  "num_reco_veto_leptons_10j30_1025_nm1");

  draw_from_trees("min_delta_phi_met_N","met>400&&ht30>1000&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&jet4_pt>200","(weightppb*5000)",
    		  ";#Delta#phi^{min}_{N};Events / 20 fb^{-1}", 20, 0., 40,
   		  "plotSig:",-1,-1,
    		  "min_delta_phi_met_N_1025_metg400_ht1000_4b200"); 
  draw_from_trees("min_delta_phi_met_N","met>400&&ht30>1000&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&jet4_pt>150","(weightppb*5000)",
    		  ";#Delta#phi^{min}_{N};Events / 20 fb^{-1}", 20, 0., 40,
   		  "plotSig:",-1,-1,
    		  "min_delta_phi_met_N_1025_metg400_ht1000_4b150"); 
  draw_from_trees("min_delta_phi_met_N","met>400&&ht30>1000&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>9","(weightppb*5000)",
    		  ";#Delta#phi^{min}_{N};Events / 20 fb^{-1}", 20, 0., 40,
   		  "plotSig:",-1,-1,
    		  "min_delta_phi_met_N_1025_metg400_ht1000_10b30"); 
  draw_from_trees("min_delta_phi_met_N","met>400&&ht30>1000&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt50>7","(weightppb*5000)",
    		  ";#Delta#phi^{min}_{N};Events / 20 fb^{-1}", 20, 0., 40,
   		  "plotSig:",-1,-1,
    		  "min_delta_phi_met_N_1025_metg400_ht1000_8b30"); 

 draw_from_trees("num_jets_pt30","met>400&&ht30>1000&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_csvm_jets30>1","(weightppb*5000)",
		  ";n_{jets} (p_{T} > 30 GeV);Events / 5 fb^{-1}", 15, 0, 15,
		  "plotSig:",-1,-1,
		  "num_jets_pt30_1025_2breq");
  draw_from_trees("num_jets_pt50","met>400&&ht30>1000&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_csvm_jets30>1","(weightppb*5000)",
		  ";n_{jets} (p_{T} > 50 GeV);Events / 5 fb^{-1}", 15, 0, 15,
		  "plotSig:",-1,-1,
		  "num_jets_pt50_1025_2breq");
  draw_from_trees("num_jets_pt100","met>400&&ht30>1000&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_csvm_jets30>1","(weightppb*5000)",
		  ";n_{jets} (p_{T} > 100 GeV);Events / 5 fb^{-1}", 15, 0, 15,
		  "plotSig:",-1,-1,
		  "num_jets_pt100_1025_2breq");

  draw_from_trees("ht30","met>400&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>=10","(weightppb*5000)",
  		  ";H_{T} [GeV];Events / 5 fb^{-1}", 20, 1000, 3000,
  		  "plotSig:",-1,-1,
  		  "ht30_high_1025_10j30_nobreq");
  draw_from_trees("ht30","met>400&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>=10&&num_csvm_jets30>1","(weightppb*5000)",
  		  ";H_{T} [GeV];Events / 5 fb^{-1}", 10, 1000, 3000,
  		  "plotSig:",-1,-1,
  		  "ht30_high_1025_10j30_2bm");

  draw_from_trees("met","ht30>1000&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>=10","(weightppb*5000)",
		";E_{T}^{miss} [GeV];Events / 5 fb^{-1}", 10, 400, 1400.,
  		  "plotSig:",-1,-1,
  		  "met_high_10j30_nobreq_1025");
  draw_from_trees("met","ht30>1000&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>=10&&num_csvm_jets30>1","(weightppb*5000)",
  		  ";E_{T}^{miss} [GeV];Events / 5 fb^{-1}", 10, 400, 1400.,
  		  "plotSig:",-1,-1,
  		  "met_high_10j30_2bm_1025");

  draw_from_trees("ht30","met>400&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&jet4_pt>150","(weightppb*5000)",
  		  ";H_{T} [GeV];Events / 5 fb^{-1}", 10, 1000, 4000,
  		  "plotSig:",-1,-1,
  		  "ht30_high_1025_4j200_nobreq");
  draw_from_trees("ht30","met>400&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&jet4_pt>150&&num_csvm_jets30>1","(weightppb*5000)",
  		  ";H_{T} [GeV];Events / 5 fb^{-1}", 10, 1000, 4000,
  		  "plotSig:",-1,-1,
  		  "ht30_high_1025_4j200_2bm");

  draw_from_trees("met","ht30>1000&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>=10","(weightppb*5000)",
		";E_{T}^{miss} [GeV];Events / 5 fb^{-1}", 10, 400, 1400.,
  		  "plotSig:",-1,-1,
  		  "met_high_10j30_nobreq_1025");
  draw_from_trees("met","ht30>1000&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>=10&&num_csvm_jets30>1","(weightppb*5000)",
  		  ";E_{T}^{miss} [GeV];Events / 5 fb^{-1}", 10, 400, 1400.,
  		  "plotSig:",-1,-1,
  		  "met_high_10j30_2bm_1025");

  draw_from_trees("ht30","met>400&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&jet4_pt>150","(weightppb*5000)",
  		  ";H_{T} [GeV];Events / 5 fb^{-1}", 10, 1000, 4000,
  		  "plotSig:",-1,-1,
  		  "ht30_high_1025_4j150_nobreq");
  draw_from_trees("ht30","met>400&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&jet4_pt>150&&num_csvm_jets30>1","(weightppb*5000)",
  		  ";H_{T} [GeV];Events / 5 fb^{-1}", 10, 1000, 4000,
  		  "plotSig:",-1,-1,
  		  "ht30_high_1025_4j150_2bm");

  draw_from_trees("met","ht30>1000&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&jet4_pt>150","(weightppb*5000)",
		";E_{T}^{miss} [GeV];Events / 5 fb^{-1}", 10, 400, 1400.,
  		  "plotSig:",-1,-1,
  		  "met_high_4j150_nobreq_1025");
  draw_from_trees("met","ht30>1000&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&jet4_pt>150&&num_csvm_jets30>1","(weightppb*5000)",
  		  ";E_{T}^{miss} [GeV];Events / 5 fb^{-1}", 10, 400, 1400.,
  		  "plotSig:",-1,-1,
  		  "met_high_4j150_2bm_1025");

  draw_from_trees("ht30","met>400&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&jet4_pt>200","(weightppb*5000)",
  		  ";H_{T} [GeV];Events / 5 fb^{-1}", 10, 1000, 4000,
  		  "plotSig:",-1,-1,
  		  "ht30_high_1025_4j200_nobreq");
  draw_from_trees("ht30","met>400&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&jet4_pt>200&&num_csvm_jets30>1","(weightppb*5000)",
  		  ";H_{T} [GeV];Events / 5 fb^{-1}", 10, 1000, 4000,
  		  "plotSig:",-1,-1,
  		  "ht30_high_1025_4j200_2bm");

  draw_from_trees("met","ht30>1000&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&jet4_pt>200","(weightppb*5000)",
		";E_{T}^{miss} [GeV];Events / 5 fb^{-1}", 10, 400, 1400.,
  		  "plotSig:",-1,-1,
  		  "met_high_4j200_nobreq_1025");
  draw_from_trees("met","ht30>1000&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&jet4_pt>200&&num_csvm_jets30>1","(weightppb*5000)",
  		  ";E_{T}^{miss} [GeV];Events / 5 fb^{-1}", 10, 400, 1400.,
  		  "plotSig:",-1,-1,
  		  "met_high_4j200_2bm_1025");


  draw_from_trees("num_csvl_jets30","met>400&&ht30>1000&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&min_delta_phi_met_N>4","(weightppb*5000)",
		  ";n_{CSVL} (p_{T} > 30 GeV);Events / 5 fb^{-1}", 6, 0, 6,
		  "plotSig:plotLog",-1,-1,
		  "num_csvl_jets30_1023_4j30_log");
  draw_from_trees("num_csvm_jets30","met>400&&ht30>1000&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&min_delta_phi_met_N>4","(weightppb*5000)",
		  ";n_{CSVM} (p_{T} > 30 GeV);Events / 5 fb^{-1}", 6, 0, 6,
		  "plotSig:plotLog",-1,-1,
		  "num_csvm_jets30_1023_4j30_log");
  draw_from_trees("num_csvt_jets30","met>400&&ht30>1000&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&min_delta_phi_met_N>4","(weightppb*5000)",
		  ";n_{CSVT} (p_{T} > 30 GeV);Events / 5 fb^{-1}", 6, 0, 6,
		  "plotSig:plotLog",-1,-1,
		  "num_csvt_jets30_1023_4j30_log");
  draw_from_trees("num_csvl_jets30","met>400&&ht30>1000&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&min_delta_phi_met_N>4","(weightppb*5000)",
		  ";n_{CSVL} (p_{T} > 30 GeV);Events / 5 fb^{-1}", 6, 0, 6,
		  "plotSig:",-1,-1,
		  "num_csvl_jets30_1023_4j30");
  draw_from_trees("num_csvm_jets30","met>400&&ht30>1000&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&min_delta_phi_met_N>4","(weightppb*5000)",
		  ";n_{CSVM} (p_{T} > 30 GeV);Events / 5 fb^{-1}", 6, 0, 6,
		  "plotSig:",-1,-1,
		  "num_csvm_jets30_1023_4j30");
  draw_from_trees("num_csvt_jets30","met>400&&ht30>1000&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&min_delta_phi_met_N>4","(weightppb*5000)",
		  ";n_{CSVT} (p_{T} > 30 GeV);Events / 5 fb^{-1}", 6, 0, 6,
		  "plotSig:",-1,-1,
		  "num_csvt_jets30_1023_4j30");

 

  draw_from_trees("ht30","met>400&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>=6","(weightppb*5000)",
  		  ";H_{T} [GeV];Events / 5 fb^{-1}", 20, 1000, 3000,
  		  "plotSig:",-1,-1,
  		  "ht30_high_1023_6j30_nm1");
  draw_from_trees("ht30","met>400&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt50>=8","(weightppb*5000)",
  		  ";H_{T} [GeV];Events / 5 fb^{-1}", 20, 1000, 3000,
  		  "plotSig:",-1,-1,
  		  "ht30_high_1023_8j50_nm1");
  draw_from_trees("ht30","met>400&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>=6&&num_csvm_jets30>1","(weightppb*5000)",
  		  ";H_{T} [GeV];Events / 5 fb^{-1}", 10, 1000, 3000,
  		  "plotSig:",-1,-1,
  		  "ht30_high_1023_6j30_2bm_nm1");
  draw_from_trees("ht30","met>400&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt50>=8&&num_csvm_jets30>1","(weightppb*5000)",
  		  ";H_{T} [GeV];Events / 5 fb^{-1}", 10, 1000, 3000,
  		  "plotSig:",-1,-1,
  		  "ht30_high_1023_8j50_2bm_nm1");

  draw_from_trees("met","ht30>1000&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>=6","(weightppb*5000)",
		";E_{T}^{miss} [GeV];Events / 5 fb^{-1}", 10, 400, 1400.,
  		  "plotSig:",-1,-1,
  		  "met_high_6j30_nobreq_1023_nm1");
  draw_from_trees("met","ht30>1000&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt50>=8","(weightppb*5000)",
  		  ";E_{T}^{miss} [GeV];Events / 5 fb^{-1}", 10, 400, 1400.,
  		  "plotSig:",-1,-1,
  		  "met_high_8j50_nobreq_1023_nm1");
  draw_from_trees("met","ht30>1000&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>=6&&num_csvm_jets30>1","(weightppb*5000)",
  		  ";E_{T}^{miss} [GeV];Events / 5 fb^{-1}", 10, 400, 1400.,
  		  "plotSig:",-1,-1,
  		  "met_high_6j30_2bm_1023_nm1");
  draw_from_trees("met","ht30>1000&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt50>=8&&num_csvm_jets30>1","(weightppb*5000)",
  		  ";E_{T}^{miss} [GeV];Events / 5 fb^{-1}", 10, 400, 1400.,
  		  "plotSig:",-1,-1,
  		  "met_hihg_8j50_2bm_1023_nm1");




 draw_from_trees("num_jets_pt30","met>400&&ht30>1000&&num_reco_veto_muons==0&&num_reco_veto_electrons==0","(weightppb*5000)",
		  ";n_{jets} (p_{T} > 30 GeV);Events / 5 fb^{-1}", 15, 0, 15,
		  "plotSig:",-1,-1,
		  "num_jets_pt30_1023_nobreq");
  draw_from_trees("num_jets_pt50","met>400&&ht30>1000&&num_reco_veto_muons==0&&num_reco_veto_electrons==0","(weightppb*5000)",
		  ";n_{jets} (p_{T} > 50 GeV);Events / 5 fb^{-1}", 15, 0, 15,
		  "plotSig:",-1,-1,
		  "num_jets_pt50_1023_nobreq");
  draw_from_trees("num_jets_pt100","met>400&&ht30>1000&&num_reco_veto_muons==0&&num_reco_veto_electrons==0","(weightppb*5000)",
		  ";n_{jets} (p_{T} > 100 GeV);Events / 5 fb^{-1}", 15, 0, 15,
		  "plotSig:",-1,-1,
		  "num_jets_pt100_1023_nobreq");
 draw_from_trees("num_jets_pt30","met>400&&ht30>1000&&num_reco_veto_muons==0&&num_reco_veto_electrons==0","(weightppb*5000)",
		  ";n_{jets} (p_{T} > 30 GeV);Events / 5 fb^{-1}", 15, 0, 15,
		  "plotSig:plotLog",-1,-1,
		  "num_jets_pt30_1023_nobreq_log");
  draw_from_trees("num_jets_pt50","met>400&&ht30>1000&&num_reco_veto_muons==0&&num_reco_veto_electrons==0","(weightppb*5000)",
		  ";n_{jets} (p_{T} > 50 GeV);Events / 5 fb^{-1}", 15, 0, 15,
		  "plotSig:plotLog",-1,-1,
		  "num_jets_pt50_1023_nobreq_log");
  draw_from_trees("num_jets_pt100","met>400&&ht30>1000&&num_reco_veto_muons==0&&num_reco_veto_electrons==0","(weightppb*5000)",
		  ";n_{jets} (p_{T} > 100 GeV);Events / 5 fb^{-1}", 15, 0, 15,
		  "plotSig:plotLog",-1,-1,
		  "num_jets_pt100_1023_nobreq_log");
  draw_from_trees("min_delta_phi_met","met>400&&ht30>1000&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>3","(weightppb*5000)",
    		  ";#Delta#phi^{min};Events / 20 fb^{-1}", 32, 0., 3.2,
   		  "plotSig:",-1,-1,
    		  "min_delta_phi_met_1023_met400_ht1000_4b30");
  draw_from_trees("min_delta_phi_met","met>400&&ht30>1000&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>3","(weightppb*5000)",
    		  ";#Delta#phi^{min};Events / 20 fb^{-1}", 32, 0., 3.2,
   		  "plotSig:plotLog",-1,-1,
    		  "min_delta_phi_met_1023_met400_ht1000_4b30_log"); 
 draw_from_trees("min_delta_phi_met_N","met>400&&ht30>1000&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>3","(weightppb*5000)",
    		  ";#Delta#phi^{min}_{N};Events / 20 fb^{-1}", 20, 0., 40.,
   		  "plotSig:",-1,-1,
    		  "min_delta_phi_met_N_1023_met400_ht1000_4b30");
  draw_from_trees("min_delta_phi_met_N","met>400&&ht30>1000&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>3","(weightppb*5000)",
    		  ";#Delta#phi^{min}_{N};Events / 20 fb^{-1}", 20, 0., 40.,
   		  "plotSig:plotLog",-1,-1,
    		  "min_delta_phi_met_N_1023_met400_ht1000_4b30_log");
 draw_from_trees("num_jets_pt30","met>400&&ht30>1000&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&min_delta_phi_met_N>4","(weightppb*5000)",
		  ";n_{jets} (p_{T} > 30 GeV);Events / 5 fb^{-1}", 15, 0, 15,
		  "plotSig:",-1,-1,
		  "num_jets_pt30_1023_mdp");
  draw_from_trees("num_jets_pt50","met>400&&ht30>1000&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&min_delta_phi_met_N>4","(weightppb*5000)",
		  ";n_{jets} (p_{T} > 50 GeV);Events / 5 fb^{-1}", 15, 0, 15,
		  "plotSig:",-1,-1,
		  "num_jets_pt50_1023_mdp");
  draw_from_trees("num_jets_pt100","met>400&&ht30>1000&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&min_delta_phi_met_N>4","(weightppb*5000)",
		  ";n_{jets} (p_{T} > 100 GeV);Events / 5 fb^{-1}", 15, 0, 15,
		  "plotSig:",-1,-1,
		  "num_jets_pt100_1023_mdp");
 draw_from_trees("num_jets_pt30","met>400&&ht30>1000&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&min_delta_phi_met_N>4","(weightppb*5000)",
		  ";n_{jets} (p_{T} > 30 GeV);Events / 5 fb^{-1}", 15, 0, 15,
		  "plotSig:plotLog",-1,-1,
		  "num_jets_pt30_1023_mdp_log");
  draw_from_trees("num_jets_pt50","met>400&&ht30>1000&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&min_delta_phi_met_N>4","(weightppb*5000)",
		  ";n_{jets} (p_{T} > 50 GeV);Events / 5 fb^{-1}", 15, 0, 15,
		  "plotSig:plotLog",-1,-1,
		  "num_jets_pt50_1023_mdp_log");
  draw_from_trees("num_jets_pt100","met>400&&ht30>1000&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&min_delta_phi_met_N>4","(weightppb*5000)",
		  ";n_{jets} (p_{T} > 100 GeV);Events / 5 fb^{-1}", 15, 0, 15,
		  "plotSig:plotLog",-1,-1,
		  "num_jets_pt100_1023_mdp_log");


  draw_from_trees("num_csvl_jets30","met>400&&ht40>1000&&num_reco_veto_muons==0&&num_reco_veto_electrons==0","(weightppb*5000)",
		  ";n_{CSVL} (p_{T} > 30 GeV);Events / 5 fb^{-1}", 6, 0, 6,
		  "plotSig:plotLog",-1,-1,
		  "num_csvl_jets30_1023_ht1000_met400_log");
  draw_from_trees("num_csvm_jets30","met>400&&ht40>1000&&num_reco_veto_muons==0&&num_reco_veto_electrons==0","(weightppb*5000)",
		  ";n_{CSVM} (p_{T} > 30 GeV);Events / 5 fb^{-1}", 6, 0, 6,
		  "plotSig:plotLog",-1,-1,
		  "num_csvm_jets30_1023_ht1000_met400_log");
  draw_from_trees("num_csvt_jets30","met>400&&ht40>1000&&num_reco_veto_muons==0&&num_reco_veto_electrons==0","(weightppb*5000)",
		  ";n_{CSVT} (p_{T} > 30 GeV);Events / 5 fb^{-1}", 6, 0, 6,
		  "plotSig:plotLog",-1,-1,
		  "num_csvt_jets30_1023_ht1000_met400_log");
  draw_from_trees("num_csvl_jets30","met>400&&ht40>1000&&num_reco_veto_muons==0&&num_reco_veto_electrons==0","(weightppb*5000)",
		  ";n_{CSVL} (p_{T} > 30 GeV);Events / 5 fb^{-1}", 6, 0, 6,
		  "plotSig:",-1,-1,
		  "num_csvl_jets30_1023_ht1000_met400");
  draw_from_trees("num_csvm_jets30","met>400&&ht40>1000&&num_reco_veto_muons==0&&num_reco_veto_electrons==0","(weightppb*5000)",
		  ";n_{CSVM} (p_{T} > 30 GeV);Events / 5 fb^{-1}", 6, 0, 6,
		  "plotSig:",-1,-1,
		  "num_csvm_jets30_1023_ht1000_met400");
  draw_from_trees("num_csvt_jets30","met>400&&ht40>1000&&num_reco_veto_muons==0&&num_reco_veto_electrons==0","(weightppb*5000)",
		  ";n_{CSVT} (p_{T} > 30 GeV);Events / 5 fb^{-1}", 6, 0, 6,
		  "plotSig:",-1,-1,
		  "num_csvt_jets30_1023_ht1000_met400");




  draw_from_trees("min_delta_phi_met","met<400&&ht40>1000&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_csvl_jets30>1","(weightppb*5000)",
    		  ";#Delta#phi^{min};Events / 20 fb^{-1}", 32, 0., 3.2,
   		  "plotSig:",-1,-1,
    		  "min_delta_phi_met_1023_met400_ht1000_2bl");
  draw_from_trees("min_delta_phi_met","met<400&&ht40>1000&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_csvl_jets30>1","(weightppb*5000)",
    		  ";#Delta#phi^{min};Events / 20 fb^{-1}", 32, 0., 3.2,
   		  "plotSig:plotLog",-1,-1,
    		  "min_delta_phi_met_1023_met400_ht1000_2bl_log"); 
  draw_from_trees("min_delta_phi_met_N","met<400&&ht40>1000&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_csvl_jets30>1","(weightppb*5000)",
    		  ";#Delta#phi^{min}_{N};Events / 20 fb^{-1}", 20, 0., 40.,
   		  "plotSig:",-1,-1,
    		  "min_delta_phi_met_N_1023_met400_ht1000_2bl");
  draw_from_trees("min_delta_phi_met_N","met<400&&ht40>1000&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_csvl_jets30>1","(weightppb*5000)",
    		  ";#Delta#phi^{min}_{N};Events / 20 fb^{-1}", 20, 0., 40.,
   		  "plotSig:plotLog",-1,-1,
    		  "min_delta_phi_met_N_1023_met400_ht1000_2bl_log");

 draw_from_trees("num_jets_pt30","met>400&&ht40>1000&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&min_delta_phi_met_N>4&&num_csvl_jets30>1","(weightppb*5000)",
		  ";n_{jets} (p_{T} > 30 GeV);Events / 5 fb^{-1}", 15, 0, 15,
		  "plotSig:",-1,-1,
		  "num_jets_pt30_1023_bl");
  draw_from_trees("num_jets_pt50","met>400&&ht40>1000&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&min_delta_phi_met_N>4&&num_csvl_jets30>1","(weightppb*5000)",
		  ";n_{jets} (p_{T} > 50 GeV);Events / 5 fb^{-1}", 15, 0, 15,
		  "plotSig:",-1,-1,
		  "num_jets_pt50_1023_bl");
  draw_from_trees("num_jets_pt100","met>400&&ht40>1000&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&min_delta_phi_met_N>4&&num_csvl_jets30>1","(weightppb*5000)",
		  ";n_{jets} (p_{T} > 100 GeV);Events / 5 fb^{-1}", 15, 0, 15,
		  "plotSig:",-1,-1,
		  "num_jets_pt100_1023_bl");
 draw_from_trees("num_jets_pt30","met>400&&ht40>1000&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&min_delta_phi_met_N>4&&num_csvl_jets30>1","(weightppb*5000)",
		  ";n_{jets} (p_{T} > 30 GeV);Events / 5 fb^{-1}", 15, 0, 15,
		  "plotSig:plotLog",-1,-1,
		  "num_jets_pt30_1023_bl_log");
  draw_from_trees("num_jets_pt50","met>400&&ht40>1000&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&min_delta_phi_met_N>4&&num_csvl_jets30>1","(weightppb*5000)",
		  ";n_{jets} (p_{T} > 50 GeV);Events / 5 fb^{-1}", 15, 0, 15,
		  "plotSig:plotLog",-1,-1,
		  "num_jets_pt50_1023_bl_log");
  draw_from_trees("num_jets_pt100","met>400&&ht40>1000&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&min_delta_phi_met_N>4&&num_csvl_jets30>1","(weightppb*5000)",
		  ";n_{jets} (p_{T} > 100 GeV);Events / 5 fb^{-1}", 15, 0, 15,
		  "plotSig:plotLog",-1,-1,
		  "num_jets_pt100_1023_bl_log");



  draw_from_trees("fatpT30_jet1_mJ",baselineMJ,"(weightppb*5000)",
  		  ";m_{FJ} (FJ 1) [GeV];Events / 5 fb^{-1}", 20, 0., 1000.,
  		  "plotSig",-1,-1,
  		  "fatpT30_jet1_mJ_6j30_1021_nm1");
  draw_from_trees("fatpT30_jet2_mJ",baselineMJ,"(weightppb*5000)",
  		  ";m_{FJ} (FJ 2) [GeV];Events / 5 fb^{-1}", 8, 0., 400.,
  		  "plotSig",-1,-1,
  		  "fatpT30_jet2_mJ_6j30_1021_nm1");
  draw_from_trees("fatpT30_jet3_mJ",baselineMJ,"(weightppb*5000)",
  		  ";m_{FJ} (FJ 3) [GeV];Events / 5 fb^{-1}", 8, 0., 400.,
  		  "plotSig",-1,-1,
  		  "fatpT30_jet3_mJ_6j30_1021_nm1");
 draw_from_trees("fatpT30_jet4_mJ",baselineMJ,"(weightppb*5000)",
  		  ";m_{FJ} (FJ 4) [GeV];Events / 5 fb^{-1}", 8, 0., 400.,
  		  "plotSig",-1,-1,
  		  "fatpT30_jet4_mJ_6j30_1021_nm1");
  draw_from_trees("fatpT30_jet1_mJ",baselineMJ+"num_jets_pt50>=8","(weightppb*5000)",
  		  ";m_{FJ} (FJ 1) [GeV];Events / 5 fb^{-1}", 20, 0., 1000.,
  		  "plotSig",-1,-1,
  		  "fatpT30_jet1_mJ_8j50_1021_nm1");
  draw_from_trees("fatpT30_jet2_mJ",baselineMJ+"num_jets_pt50>=8","(weightppb*5000)",
  		  ";m_{FJ} (FJ 2) [GeV];Events / 5 fb^{-1}", 8, 0., 400.,
  		  "plotSig",-1,-1,
  		  "fatpT30_jet2_mJ_8j50_1021_nm1");
  draw_from_trees("fatpT30_jet3_mJ",baselineMJ+"num_jets_pt50>=8","(weightppb*5000)",
  		  ";m_{FJ} (FJ 3) [GeV];Events / 5 fb^{-1}", 8, 0., 400.,
  		  "plotSig",-1,-1,
  		  "fatpT30_jet3_mJ_8j50_1021_nm1");
 draw_from_trees("fatpT30_jet4_mJ",baselineMJ+"num_jets_pt50>=8","(weightppb*5000)",
  		  ";m_{FJ} (FJ 4) [GeV];Events / 5 fb^{-1}", 8, 0., 400.,
  		  "plotSig",-1,-1,
  		  "fatpT30_jet4_mJ_8j50_1021_nm1");

  draw_from_trees("min_delta_phi_met","met<400&&ht40>1000&&num_csvm_jets>1&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>=6","(weightppb*5000)",
    		  ";#Delta#phi^{min};Events / 20 fb^{-1}", 32, 0., 3.2,
   		  "plotSig:",-1,-1,
    		  "min_delta_phi_met_1019_6j30_lowmet_ht_nm1");   
  draw_from_trees("min_delta_phi_met_N","met<400&&ht40>1000&&num_csvm_jets>1&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>=6","(weightppb*5000)",
    		  ";#Delta#phi^{min}_{N};Events / 20 fb^{-1}", 20, 0., 40.,
   		  "plotSig:",-1,-1,
    		  "min_delta_phi_met_N_1019_6j30_lowmet_ht_nm1"); 
  draw_from_trees("min_delta_phi_met","met<400&&ht40>1000&&num_csvm_jets>1&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt50>=8","(weightppb*5000)",
    		  ";#Delta#phi^{min};Events / 20 fb^{-1}", 32, 0., 3.2,
   		  "plotSig:",-1,-1,
    		  "min_delta_phi_met_1019_8j50_lowmet_ht_nm1"); 
  draw_from_trees("min_delta_phi_met_N","met<400&&ht40>1000&&num_csvm_jets>1&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt50>=8","(weightppb*5000)",
    		  ";#Delta#phi^{min}_{N};Events / 20 fb^{-1}", 20, 0., 40.,
   		  "plotSig:",-1,-1,
    		  "min_delta_phi_met_N_1019_8j50_lowmet_ht_nm1"); 



  draw_from_trees("num_reco_veto_electrons+num_reco_veto_muons","met>400&&ht40>1000&&num_csvm_jets>1&&min_delta_phi_met_N>4&&num_jets_pt30>=6","(weightppb*5000)",
		  ";n_{e+#mu} (p_{T} > 10 GeV);Events / 5 fb^{-1}", 6, 0, 6,
		  "plotSig:",-1,-1,
		  "num_reco_veto_leptons_6j30_1019_ht_nm1"); 
  draw_from_trees("min_delta_phi_met","met>400&&ht40>1000&&num_csvm_jets>1&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>=6","(weightppb*5000)",
    		  ";#Delta#phi^{min};Events / 20 fb^{-1}", 32, 0., 3.2,
   		  "plotSig:",-1,-1,
    		  "min_delta_phi_met_1019_6j30_ht_nm1"); 
  
  draw_from_trees("min_delta_phi_met_N","met>400&&ht40>1000&&num_csvm_jets>1&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>=6","(weightppb*5000)",
    		  ";#Delta#phi^{min}_{N};Events / 20 fb^{-1}", 20, 0., 40.,
   		  "plotSig:",-1,-1,
    		  "min_delta_phi_met_N_1019_6j30_ht_nm1"); 

  draw_from_trees("num_reco_veto_electrons+num_reco_veto_muons","met>400&&ht40>1000&&num_csvm_jets>1&&min_delta_phi_met_N>4&&num_jets_pt50>=6","(weightppb*5000)",
		  ";n_{e+#mu} (p_{T} > 10 GeV);Events / 5 fb^{-1}", 6, 0, 6,
		  "plotSig:",-1,-1,
		  "num_reco_veto_leptons_6j50_1019_ht_nm1"); 
  draw_from_trees("min_delta_phi_met_N","met>400&&ht40>1000&&num_csvm_jets>1&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt50>=6","(weightppb*5000)",
    		  ";#Delta#phi^{min}_{N};Events / 20 fb^{-1}", 20, 0., 40.,
   		  "plotSig:",-1,-1,
    		  "min_delta_phi_met_N_1019_6j50_ht_nm1"); 
  draw_from_trees("num_reco_veto_electrons+num_reco_veto_muons","met>400&&ht40>1000&&num_csvm_jets>1&&min_delta_phi_met_N>4&&num_jets_pt50>=8","(weightppb*5000)",
		  ";n_{e+#mu} (p_{T} > 10 GeV);Events / 5 fb^{-1}", 6, 0, 6,
		  "plotSig:",-1,-1,
		  "num_reco_veto_leptons_8j50_1019_ht_nm1"); 
  draw_from_trees("min_delta_phi_met","met>400&&ht40>1000&&num_csvm_jets>1&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt50>=8","(weightppb*5000)",
    		  ";#Delta#phi^{min};Events / 20 fb^{-1}", 32, 0., 3.2,
   		  "plotSig:",-1,-1,
    		  "min_delta_phi_met_1019_8j50_ht_nm1"); 

  draw_from_trees("min_delta_phi_met_N","met>400&&ht40>1000&&num_csvm_jets>1&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt50>=8","(weightppb*5000)",
    		  ";#Delta#phi^{min}_{N};Events / 20 fb^{-1}", 20, 0., 40.,
   		  "plotSig:",-1,-1,
    		  "min_delta_phi_met_N_1019_8j50_ht_nm1"); 

  draw_from_trees("num_csvl_jets30","met>400&&ht40>1000&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&min_delta_phi_met_N>4&&num_jets_pt50>=8","(weightppb*5000)",
		  ";n_{CSVL} (p_{T} > 30 GeV);Events / 5 fb^{-1}", 6, 0, 6,
		  "plotSig:",-1,-1,
		  "num_csvl_jets30_1019_ht_nm1");
  draw_from_trees("num_csvm_jets30","met>400&&ht40>1000&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&min_delta_phi_met_N>4&&num_jets_pt50>=8","(weightppb*5000)",
		  ";n_{CSVM} (p_{T} > 30 GeV);Events / 5 fb^{-1}", 6, 0, 6,
		  "plotSig:",-1,-1,
		  "num_csvm_jets30_1019_ht_nm1");
  draw_from_trees("num_csvt_jets30","met>400&&ht40>1000&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&min_delta_phi_met_N>4&&num_jets_pt50>=8","(weightppb*5000)",
		  ";n_{CSVT} (p_{T} > 30 GeV);Events / 5 fb^{-1}", 6, 0, 6,
		  "plotSig:",-1,-1,
		  "num_csvt_jets30_1019_ht_nm1");
  draw_from_trees("num_jets_pt30","met>400&&ht40>1000&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&min_delta_phi_met_N>4&&num_csvm_jets30>1","(weightppb*5000)",
		  ";n_{jets} (p_{T} > 30 GeV);Events / 5 fb^{-1}", 15, 0, 15,
		  "plotSig:",-1,-1,
		  "num_jets_pt30_1019_ht_nm1");
  draw_from_trees("num_jets_pt50","met>400&&ht40>1000&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&min_delta_phi_met_N>4&&num_csvm_jets30>1","(weightppb*5000)",
		  ";n_{jets} (p_{T} > 50 GeV);Events / 5 fb^{-1}", 15, 0, 15,
		  "plotSig:",-1,-1,
		  "num_jets_pt50_1019_ht_nm1");
  draw_from_trees("num_jets_pt70","met>400&&ht40>1000&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&min_delta_phi_met_N>4&&num_csvm_jets30>1","(weightppb*5000)",
		  ";n_{jets} (p_{T} > 70 GeV);Events / 5 fb^{-1}", 15, 0, 15,
		  "plotSig:",-1,-1,
		  "num_jets_pt70_1019_ht_nm1");
  draw_from_trees("num_jets_pt100","met>400&&ht40>1000&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&min_delta_phi_met_N>4&&num_csvm_jets30>1","(weightppb*5000)",
		  ";n_{jets} (p_{T} > 100 GeV);Events / 5 fb^{-1}", 15, 0, 15,
		  "plotSig:",-1,-1,
		  "num_jets_pt100_1019_ht_nm1");

  draw_from_trees("fatpT30_jet3_mJ",baseline,"(weightppb*5000)",
  		  ";m_{FJ} (FJ 3) [GeV];Events / 5 fb^{-1}", 12, 0., 300.,
  		  "plotSig",-1,-1,
  		  "fatpT30_jet3_mJ_1016_nm1");
  draw_from_trees("fatpT30_jet4_mJ",baseline,"(weightppb*5000)",
  		  ";m_{FJ} (FJ 4) [GeV];Events / 5 fb^{-1}", 12, 0., 300.,
  		  "plotSig",-1,-1,
  		  "fatpT30_jet4_mJ_1016_nm1");
  draw_from_trees("num_csvm_jets30",baseline,"(weightppb*5000)",
		  ";n_{CSVM} (p_{T} > 30 GeV);Events / 5 fb^{-1}", 4, 2, 6,
		  "plotSig:",-1,-1,
		  "num_csvm_jets30_1016_nm1"); 
  draw_from_trees("num_csvl_jets30",baseline,"(weightppb*5000)",
		  ";n_{CSVL} (p_{T} > 30 GeV);Events / 5 fb^{-1}", 4, 2, 6,
		  "plotSig:",-1,-1,
		  "num_csvl_jets30_1016_nm1"); 
  draw_from_trees("num_csvt_jets30",baseline,"(weightppb*5000)",
		  ";n_{CSVT} (p_{T} > 30 GeV);Events / 5 fb^{-1}", 6, 0, 6,
		  "plotSig:",-1,-1,
		  "num_csvt_jets30_1016_nm1"); 
  draw_from_trees("fatpT30_MJ","met>200&&num_csvm_jets>1&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt50>=8","(weightppb*5000)",
  		  ";M_{J} [GeV];Events / 5 fb^{-1}", 20, 0., 1000.,
  		  "plotSig:",-1,-1,
  		  "fatpT30_MJ_1016_nm1");
  draw_from_trees("fatpT30_MJ",baseline,"(weightppb*5000)",
  		  ";M_{J} [GeV];Events / 5 fb^{-1}", 12, 400., 1000.,
  		  "plotSig:",-1,-1,
  		  "fatpT30_MJ_high_1016_nm1");

  draw_from_trees("num_jets_pt50","met>200&&fatpT30_MJ>400&&num_csvm_jets>1&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0","(weightppb*5000)",
		  ";n_{jets} (p_{T} > 50 GeV);Events / 5 fb^{-1}", 15, 0, 15,
		  "plotSig:",-1,-1,
		  "num_jets_pt50_1016_nm1");  
  draw_from_trees("num_jets_pt50",baseline,"(weightppb*5000)",
		  ";n_{jets} (p_{T} > 50 GeV);Events / 5 fb^{-1}", 7, 8, 15,
		  "plotSig:",-1,-1,
		  "num_jets_pt50_high_1016_nm1");  
  draw_from_trees("met","fatpT30_MJ>400&&num_csvm_jets>1&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt50>=8","(weightppb*5000)",
		  ";E_{T}^{miss} [GeV];Events / 5 fb^{-1}", 25, 0, 500.,
		  "plotSig:",-1,-1,
		  "met_1016_nm1");
  draw_from_trees("met",baseline,"(weightppb*5000)",
		  ";E_{T}^{miss} [GeV];Events / 5 fb^{-1}", 10, 200, 500.,
		  "plotSig:",-1,-1,
		  "met_high_1016_nm1");
  draw_from_trees("fatpT30_jet1_mJ",baseline,"(weightppb*5000)",
  		  ";m_{FJ} (FJ 1) [GeV];Events / 5 fb^{-1}", 20, 0., 500.,
  		  "plotSig",-1,-1,
  		  "fatpT30_jet1_mJ_1016_nm1");
  draw_from_trees("fatpT30_jet2_mJ",baseline,"(weightppb*5000)",
  		  ";m_{FJ} (FJ 2) [GeV];Events / 5 fb^{-1}", 12, 0., 300.,
  		  "plotSig",-1,-1,
  		  "fatpT30_jet2_mJ_1016_nm1");




  draw_from_trees("mht","fatpT30_MJ>400&&num_csvm_jets>1&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt50>=8","(weightppb*5000)",
		  ";H_{T}^{miss} [GeV];Events / 5 fb^{-1}", 50, 0, 1000.,
		  "plotSig:",-1,-1,
		  "mht_1016_nm1");
  draw_from_trees("met_over_sqrt_ht50","met>200&&fatpT30_MJ>400&&num_csvm_jets>1&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt50>=8","(weightppb*5000)",
		 ";E_{T}^{miss}/#sqrt{H_{T}} [GeV^{1/2}];Events / 20 fb^{-1}", 25, 0., 50.,
		 "plotSig:",-1,-1,
		 "met_over_sqrt_ht50_1016_nm1");  



  draw_from_trees("num_jets_pt100","met>200&&fatpT30_MJ>400&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_csvm_jets>=2","(weightppb*5000)",
		  ";n_{jets} (p_{T} > 100 GeV);Events / 5 fb^{-1}", 13, 2, 15,
		  "plotSig:",-1,-1,
		  "num_jets_pt100_MJ400_met200_nm1");
  draw_from_trees("num_jets_pt50","met>200&&fatpT30_MJ>400&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_csvm_jets>=2","(weightppb*5000)",
		  ";n_{jets} (p_{T} > 50 GeV);Events / 5 fb^{-1}", 13, 2, 15,
		  "plotSig:",-1,-1,
		  "num_jets_pt50_MJ400_met200_nm1");  
  draw_from_trees("num_jets_pt40","met>200&&fatpT30_MJ>400&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_csvm_jets>=2","(weightppb*5000)",
		  ";n_{jets} (p_{T} > 40 GeV);Events / 5 fb^{-1}", 13, 2, 15,
		  "plotSig:",-1,-1,
		  "num_jets_pt40_MJ400_met200_nm1");  
  draw_from_trees("num_jets_pt30","met>200&&fatpT30_MJ>400&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_csvm_jets>=2","(weightppb*5000)",
		  ";n_{jets} (p_{T} > 30 GeV);Events / 5 fb^{-1}", 13, 2, 15,
		  "plotSig:",-1,-1,
		  "num_jets_pt30_MJ400_met200_nm1");  
   draw_from_trees("num_fatpT30_jets",jack,"(weightppb*5000)",
   		  ";n_{FJ} (p_{T} > 50 GeV);Events / 5 fb^{-1}", 10, 0., 10.,
   		  "plotSig:",-1,-1,
		   "num_fatpT30_jets_MJ400_met200_nm1");
   draw_from_trees("num_fatpT30_jets_pt100",jack,"(weightppb*5000)",
   		  ";n_{FJ} (p_{T} > 100 GeV);Events / 5 fb^{-1}", 10, 0., 10.,
   		  "plotSig:",-1,-1,
		   "num_fatpT30_jets_pt100_MJ400_met200_nm1");
   draw_from_trees("num_fatpT30_jets_pt150",jack,"(weightppb*5000)",
   		  ";n_{FJ} (p_{T} > 150 GeV);Events / 5 fb^{-1}", 10, 0., 10.,
   		  "plotSig:",-1,-1,
		   "num_fatpT30_jets_pt150_MJ400_met200_nm1");
  draw_from_trees("num_fatpT30_jets_pt200",jack,"(weightppb*5000)",
   		  ";n_{FJ} (p_{T} > 200 GeV);Events / 5 fb^{-1}", 10, 0., 10.,
   		  "plotSig:",-1,-1,
		   "num_fatpT30_jets_pt200_MJ400_met200_nm1");
  draw_from_trees("num_jets_pt100","met>200&&fatpT30_MJ>400&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_csvm_jets>=2","(weightppb*5000)",
		  ";n_{jets} (p_{T} > 100 GeV);Events / 5 fb^{-1}", 13, 2, 15,
		  "plotSig:plotLog:",-1,-1,
		  "num_jets_pt100_MJ400_met200_log_nm1");
  draw_from_trees("num_jets_pt50","met>200&&fatpT30_MJ>400&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_csvm_jets>=2","(weightppb*5000)",
		  ";n_{jets} (p_{T} > 50 GeV);Events / 5 fb^{-1}", 13, 2, 15,
		  "plotSig:plotLog:",-1,-1,
		  "num_jets_pt50_MJ400_met200_log_nm1");  
  draw_from_trees("num_jets_pt40","met>200&&fatpT30_MJ>400&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_csvm_jets>=2","(weightppb*5000)",
		  ";n_{jets} (p_{T} > 40 GeV);Events / 5 fb^{-1}", 13, 2, 15,
		  "plotSig:plotLog:",-1,-1,
		  "num_jets_pt40_MJ400_met200_log_nm1");  
  draw_from_trees("num_jets_pt30","met>200&&fatpT30_MJ>400&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_csvm_jets>=2","(weightppb*5000)",
		  ";n_{jets} (p_{T} > 30 GeV);Events / 5 fb^{-1}", 13, 2, 15,
		  "plotSig:plotLog:",-1,-1,
		  "num_jets_pt30_MJ400_met200_log_nm1");  
   draw_from_trees("num_fatpT30_jets",jack,"(weightppb*5000)",
   		  ";n_{FJ} (p_{T} > 50 GeV);Events / 5 fb^{-1}", 10, 0., 10.,
   		  "plotSig:plotLog:",-1,-1,
		   "num_fatpT30_jets_MJ400_met200_log_nm1");
   draw_from_trees("num_fatpT30_jets_pt100",jack,"(weightppb*5000)",
   		  ";n_{FJ} (p_{T} > 100 GeV);Events / 5 fb^{-1}", 10, 0., 10.,
   		  "plotSig:plotLog:",-1,-1,
		   "num_fatpT30_jets_pt100_MJ400_met200_log_nm1");
   draw_from_trees("num_fatpT30_jets_pt150",jack,"(weightppb*5000)",
   		  ";n_{FJ} (p_{T} > 150 GeV);Events / 5 fb^{-1}", 10, 0., 10.,
   		  "plotSig:plotLog:",-1,-1,
		   "num_fatpT30_jets_pt150_MJ400_met200_log_nm1");
  draw_from_trees("num_fatpT30_jets_pt200",jack,"(weightppb*5000)",
   		  ";n_{FJ} (p_{T} > 200 GeV);Events / 5 fb^{-1}", 10, 0., 10.,
   		  "plotSig:plotLog:",-1,-1,
		   "num_fatpT30_jets_pt200_MJ400_met200_log_nm1");


  draw_from_trees("fatpT30_jet1_mJ","fatpT30_MJ>400&&met>200&&jet3_pt>50&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_csvm_jets>=2","(weightppb*5000)",
  		  ";m_{FJ} (FJ 1) [GeV];Events / 5 fb^{-1}", 20, 0., 1000.,
  		  "plotSig:plotLog",-1,-1,
  		  "fatpT30_jet1_mJ_MJ400_nm1_log");
  draw_from_trees("fatpT30_jet1_mJ","fatpT30_MJ>400&&met>200&&jet3_pt>50&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_csvm_jets>=2","(weightppb*5000)",
  		  ";m_{FJ} (FJ 1) [GeV];Events / 5 fb^{-1}", 20, 0., 500.,
  		  "plotSig",-1,-1,
  		  "fatpT30_jet1_mJ_MJ400_nm1");
  draw_from_trees("fatpT30_jet2_mJ","fatpT30_MJ>400&&met>200&&jet3_pt>50&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_csvm_jets>=2","(weightppb*5000)",
  		  ";m_{FJ} (FJ 2) [GeV];Events / 5 fb^{-1}", 20, 0., 800.,
  		  "plotSig:plotLog",-1,-1,
  		  "fatpT30_jet2_mJ_MJ400_nm1_log");
  draw_from_trees("fatpT30_jet2_mJ","fatpT30_MJ>400&&met>200&&jet3_pt>50&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_csvm_jets>=2","(weightppb*5000)",
  		  ";m_{FJ} (FJ 2) [GeV];Events / 5 fb^{-1}", 12, 0., 300.,
  		  "plotSig",-1,-1,
  		  "fatpT30_jet2_mJ_MJ400_nm1");

  draw_from_trees("fatpT30_MJ","fatpT30_jet1_mJ>150&&fatpT30_jet2_mJ>100&&met>200&&jet3_pt>50&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_csvm_jets>=2","(weightppb*5000)",
  		  ";M_{J} [GeV];Events / 5 fb^{-1}", 40, 0., 2000.,
  		  "plotSig:plotLog",-1,-1,
  		  "fatpT30_MJ_2mJ_log_nm1");
  draw_from_trees("fatpT30_MJ","fatpT30_jet1_mJ>150&&fatpT30_jet2_mJ>100&&met>200&&jet3_pt>50&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_csvm_jets>=2","(weightppb*5000)",
  		  ";M_{J} [GeV];Events / 5 fb^{-1}", 16, 0., 800.,
  		  "plotSig:",-1,-1,
  		  "fatpT30_MJ_2mJ_nm1");





  draw_from_trees("ht40","num_jets_pt40>=8&&met>200&&jet3_pt>50&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_csvm_jets>=2","(weightppb*5000)",
  		  ";H_{T} [GeV];Events / 5 fb^{-1}", 40, 350., 4350.,
  		  "plotSig:plotLog",-1,-1,
  		  "ht40_8jets_log_nm1");
  draw_from_trees("ht40","num_jets_pt40>=8&&met>200&&jet3_pt>50&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_csvm_jets>=2","(weightppb*5000)",
  		  ";H_{T} [GeV];Events / 5 fb^{-1}", 15, 350., 1850.,
  		  "plotSig:",-1,-1,
  		  "ht40_8jets_nm1");
  draw_from_trees("fatpT30_MJ","num_jets_pt40>=8&&met>200&&jet3_pt>50&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_csvm_jets>=2","(weightppb*5000)",
  		  ";M_{J} [GeV];Events / 5 fb^{-1}", 40, 0., 2000.,
  		  "plotSig:plotLog",-1,-1,
  		  "fatpT30_MJ_8jets_log_nm1");
  draw_from_trees("fatpT30_MJ","num_jets_pt40>=8&&met>200&&jet3_pt>50&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_csvm_jets>=2","(weightppb*5000)",
  		  ";M_{J} [GeV];Events / 5 fb^{-1}", 16, 0., 800.,
  		  "plotSig:",-1,-1,
  		  "fatpT30_MJ_8jets_nm1");



  draw_from_trees("jet1_pt","fatpT30_MJ>600&&met>200&&jet3_pt>50&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_csvm_jets>=2","(weightppb*5000)",
		  ";p_{T} (jet 1) [GeV];Events / 5 fb^{-1}", 20, 50, 1050.,
		  "plotSig:plotLog",-1,-1,
		  "jet1_pt_log_nm1"); 
  draw_from_trees("jet1_pt","fatpT30_MJ>600&&met>200&&jet3_pt>50&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_csvm_jets>=2","(weightppb*5000)",
		  ";p_{T} (jet 1) [GeV];Events / 5 fb^{-1}", 20, 50, 1050.,
		  "plotSig:",-1,-1,
		  "jet1_pt_nm1"); 
  draw_from_trees("jet2_pt","fatpT30_MJ>600&&met>200&&jet3_pt>50&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_csvm_jets>=2","(weightppb*5000)",
		  ";p_{T} (jet 2) [GeV];Events / 5 fb^{-1}", 20, 50, 1050.,
		  "plotSig:plotLog",-1,-1,
		  "jet2_pt_log_nm1"); 
  draw_from_trees("jet2_pt","fatpT30_MJ>600&&met>200&&jet3_pt>50&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_csvm_jets>=2","(weightppb*5000)",
		  ";p_{T} (jet 2) [GeV];Events / 5 fb^{-1}", 20, 50, 1050.,
		  "plotSig:",-1,-1,
		  "jet2_pt_nm1"); 
  draw_from_trees("jet3_pt","fatpT30_MJ>600&&met>200&&jet3_pt>50&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_csvm_jets>=2","(weightppb*5000)",
		  ";p_{T} (jet 3) [GeV];Events / 5 fb^{-1}", 20, 50, 1050.,
		  "plotSig:plotLog",-1,-1,
		  "jet3_pt_log_nm1"); 
  draw_from_trees("jet3_pt","fatpT30_MJ>600&&met>200&&jet3_pt>50&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_csvm_jets>=2","(weightppb*5000)",
		  ";p_{T} (jet 3) [GeV];Events / 5 fb^{-1}", 20, 50, 1050.,
		  "plotSig:",-1,-1,
		  "jet3_pt_nm1"); 
  draw_from_trees("met","fatpT30_MJ>600&&jet3_pt>50&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_csvm_jets>=2","(weightppb*5000)",
		  ";E_{T}^{miss} [GeV];Events / 5 fb^{-1}", 25, 0, 500.,
		  "plotSig:",-1,-1,
		  "met_MJ600_nm1");
  draw_from_trees("met","fatpT30_MJ>600&&jet3_pt>50&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_csvm_jets>=2","(weightppb*5000)",
		  ";E_{T}^{miss} [GeV];Events / 5 fb^{-1}", 50, 0, 1000.,
		  "plotSig:plotLog",-1,-1,
		  "met_MJ600_log_nm1"); 
  draw_from_trees("mht","fatpT30_MJ>600&&jet3_pt>50&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_csvm_jets>=2","(weightppb*5000)",
		  ";H_{T}^{miss} [GeV];Events / 5 fb^{-1}", 50, 0, 1000.,
		  "plotSig:",-1,-1,
		  "mht_MJ600_nm1");
  draw_from_trees("mht","fatpT30_MJ>600&&jet3_pt>50&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_csvm_jets>=2","(weightppb*5000)",
		  ";H_{T}^{miss} [GeV];Events / 5 fb^{-1}", 50, 0, 1000.,
		  "plotSig:plotLog",-1,-1,
		  "mht_MJ600_log_nm1"); 
  draw_from_trees("met_over_sqrt_ht50","fatpT30_MJ>600&&jet3_pt>50&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_csvm_jets>=2","(weightppb*5000)",
		 ";E_{T}^{miss}/#sqrt{H_{T}} [GeV^{1/2}];Events / 20 fb^{-1}", 25, 0., 50.,
		 "plotSig:",-1,-1,
		 "met_over_sqrt_ht50_MJ600_nm1");  
  draw_from_trees("met_over_sqrt_ht50","fatpT30_MJ>600&&jet3_pt>50&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_csvm_jets>=2","(weightppb*5000)",
		 ";E_{T}^{miss}/#sqrt{H_{T}} [GeV^{1/2}];Events / 20 fb^{-1}", 25, 0., 50.,
		 "plotSig:plotLog",-1,-1,
		 "met_over_sqrt_ht50_MJ600_log_nm1");  
  draw_from_trees("min_delta_phi_met_N","fatpT30_MJ>600&&met>200&&jet3_pt>50&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_csvm_jets>=2","(weightppb*5000)",
    		  ";#Delta#phi^{min}_{N};Events / 20 fb^{-1}", 20, 0., 40.,
   		  "plotSig:",-1,-1,
    		  "min_delta_phi_met_N_MJ600_nm1"); 
  draw_from_trees("num_jets_pt50","met>200&&fatpT30_MJ>600&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_csvm_jets>=2","(weightppb*5000)",
		  ";n_{jets} (p_{T} > 50 GeV);Events / 5 fb^{-1}", 13, 2, 15,
		  "plotSig:",-1,-1,
		  "num_jets_pt50_MJ600_nm1");  
  draw_from_trees("num_jets_pt30","met>200&&fatpT30_MJ>600&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_csvm_jets30>=2","(weightppb*5000)",
		  ";n_{jets} (p_{T} > 30 GeV);Events / 5 fb^{-1}", 13, 2, 15,
		  "plotSig:",-1,-1,
		  "num_jets_pt30_MJ600_nm1");  
  draw_from_trees("num_csvm_jets","met>200&&fatpT30_MJ>600&&jet3_pt>50&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0","(weightppb*5000)",
		  ";n_{CSVM} (p_{T} > 50 GeV);Events / 5 fb^{-1}", 6, 0, 6,
		  "plotSig:",-1,-1,
		  "num_csvm_jets_MJ600_nm1"); 
  draw_from_trees("num_csvm_jets30","met>200&&fatpT30_MJ>600&&jet3_pt>30&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0","(weightppb*5000)",
		  ";n_{CSVM} (p_{T} > 30 GeV);Events / 5 fb^{-1}", 6, 0, 6,
		  "plotSig:",-1,-1,
		  "num_csvm_jets30_MJ600_nm1"); 
  draw_from_trees("num_reco_veto_muons","met>200&&fatpT30_MJ>600&&jet3_pt>50&&min_delta_phi_met_N>4&&num_reco_veto_electrons==0&&num_csvm_jets>=2","(weightppb*5000)",
		  ";n_{#mu} (p_{T} > 10 GeV);Events / 5 fb^{-1}", 6, 0, 6,
		  "plotSig:",-1,-1,
		  "num_reco_veto_muons_MJ600_nm1"); 
  draw_from_trees("num_reco_veto_electrons","met>200&&fatpT30_MJ>600&&jet3_pt>50&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_csvm_jets>=2","(weightppb*5000)",
		  ";n_{e} (p_{T} > 10 GeV);Events / 5 fb^{-1}", 6, 0, 6,
		  "plotSig:",-1,-1,
		  "num_reco_veto_electrons_MJ600_nm1"); 
  draw_from_trees("num_reco_veto_electrons+num_reco_veto_muons","met>200&&fatpT30_MJ>600&&jet3_pt>50&&min_delta_phi_met_N>4&&num_csvm_jets>=2","(weightppb*5000)",
		  ";n_{e+#mu} (p_{T} > 10 GeV);Events / 5 fb^{-1}", 6, 0, 6,
		  "plotSig:",-1,-1,
		  "num_reco_veto_leptons_MJ600_nm1"); 

  draw_from_trees("ht50","met>200&&jet3_pt>50&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_csvm_jets>=2","(weightppb*5000)",
  		  ";H_{T} [GeV];Events / 5 fb^{-1}", 40, 150., 4150.,
  		  "plotSig:plotLog",-1,-1,
  		  "ht50_nm1_log");
  draw_from_trees("ht50","met>200&&jet3_pt>50&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_csvm_jets>=2","(weightppb*5000)",
  		  ";H_{T} [GeV];Events / 5 fb^{-1}", 20, 150., 2150.,
  		  "plotSig:",-1,-1,
  		  "ht50_nm1");
  draw_from_trees("fatpT30_MJ","met>200&&jet3_pt>50&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_csvm_jets>=2","(weightppb*5000)",
  		  ";M_{J} [GeV];Events / 5 fb^{-1}", 40, 0., 2000.,
  		  "plotSig:plotLog",-1,-1,
  		  "fatpT30_MJ_nm1_log");
  draw_from_trees("fatpT30_MJ","met>200&&jet3_pt>50&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_csvm_jets>=2","(weightppb*5000)",
  		  ";M_{J} [GeV];Events / 5 fb^{-1}", 16, 0., 800.,
  		  "plotSig:",-1,-1,
  		  "fatpT30_MJ_nm1");


  // draw_from_trees("ht50",search+"num_csvm_jets30>=4","(weightppb*5000)",
  // 		  ";H_{T} [GeV];Events / 5 fb^{-1}", 40, 150., 4150.,
  // 		  "plotSig:plotLog:T1tttt",-1,-1,
  // 		  "ht_T1tttt_geq4bm");
  // draw_from_trees("ht50",search+"num_csvm_jets30==2","(weightppb*5000)",
  // 		  ";H_{T} [GeV];Events / 5 fb^{-1}", 40, 150., 4150.,
  // 		  "plotSig:plotLog",-1,-1,
  // 		  "ht_T1bbbb_2bm");
  // draw_from_trees("ht50",search+"num_csvm_jets30==2","(weightppb*5000)",
  // 		  ";H_{T} [GeV];Events / 5 fb^{-1}", 40, 150., 4150.,
  // 		  "plotSig:plotLog:T1tttt",-1,-1,
  // 		  "ht_T1tttt_2bm");
  // draw_from_trees("ht50",search+"num_csvm_jets30==3","(weightppb*5000)",
  // 		  ";H_{T} [GeV];Events / 5 fb^{-1}", 40, 150., 4150.,
  // 		  "plotSig:plotLog",-1,-1,
  // 		  "ht_T1bbbb_3bm");
  // draw_from_trees("ht50",search+"num_csvm_jets30==3","(weightppb*5000)",
  // 		  ";H_{T} [GeV];Events / 5 fb^{-1}", 40, 150., 4150.,
  // 		  "plotSig:plotLog:T1tttt",-1,-1,
  // 		  "ht_T1tttt_3bm");
  // draw_from_trees("ht50",search+"num_csvm_jets30>=2","(weightppb*5000)",
  // 		  ";H_{T} [GeV];Events / 5 fb^{-1}", 40, 150., 4150.,
  // 		  "plotSig:plotLog",-1,-1,
  // 		  "ht_T1bbbb_geq2bm");
  // draw_from_trees("ht50",search+"num_csvm_jets30>=2","(weightppb*5000)",
  // 		  ";H_{T} [GeV];Events / 5 fb^{-1}", 40, 150., 4150.,
  // 		  "plotSig:plotLog:T1tttt",-1,-1,
  // 		  "ht_T1tttt_geq2bm");

 
  // draw_from_trees("MJ_pt100",search+"num_csvm_jets30>=4","(weightppb*5000)",
  // 		  ";M_{J} [GeV];Events / 5 fb^{-1}", 50, 0., 1500.,
  // 		  "plotSig:plotLog",-1,-1,
  // 		  "MJ_pt100_T1bbbb_geq4bm");
  // draw_from_trees("MJ_pt100",search+"num_csvm_jets30>=4","(weightppb*5000)",
  // 		  ";M_{J} [GeV];Events / 5 fb^{-1}", 50, 0., 1500.,
  // 		  "plotSig:plotLog:T1tttt",-1,-1,
  // 		  "MJ_pt100_T1tttt_geq4bm");
  // draw_from_trees("MJ_pt100",search+"num_csvm_jets30==2","(weightppb*5000)",
  // 		  ";M_{J} [GeV];Events / 5 fb^{-1}", 50, 0., 1500.,
  // 		  "plotSig:plotLog",-1,-1,
  // 		  "MJ_pt100_T1bbbb_2bm");
  // draw_from_trees("MJ_pt100",search+"num_csvm_jets30==2","(weightppb*5000)",
  // 		  ";M_{J} [GeV];Events / 5 fb^{-1}", 50, 0., 1500.,
  // 		  "plotSig:plotLog:T1tttt",-1,-1,
  // 		  "MJ_pt100_T1tttt_2bm");
  // draw_from_trees("MJ_pt100",search+"num_csvm_jets30==3","(weightppb*5000)",
  // 		  ";M_{J} [GeV];Events / 5 fb^{-1}", 50, 0., 1500.,
  // 		  "plotSig:plotLog",-1,-1,
  // 		  "MJ_pt100_T1bbbb_3bm");
  // draw_from_trees("MJ_pt100",search+"num_csvm_jets30==3","(weightppb*5000)",
  // 		  ";M_{J} [GeV];Events / 5 fb^{-1}", 50, 0., 1500.,
  // 		  "plotSig:plotLog:T1tttt",-1,-1,
  // 		  "MJ_pt100_T1tttt_3bm");
  // draw_from_trees("MJ_pt100",search+"num_csvm_jets30>=2","(weightppb*5000)",
  // 		  ";M_{J} [GeV];Events / 5 fb^{-1}", 50, 0., 1500.,
  // 		  "plotSig:plotLog",-1,-1,
  // 		  "MJ_pt100_T1bbbb_geq2bm");
  // draw_from_trees("MJ_pt100",search+"num_csvm_jets30>=2","(weightppb*5000)",
  // 		  ";M_{J} [GeV];Events / 5 fb^{-1}", 50, 0., 1500.,
  // 		  "plotSig:plotLog:T1tttt",-1,-1,
  // 		  "MJ_pt100_T1tttt_geq2bm");

  // draw_from_trees("MJ",search+"num_csvm_jets30>=4","(weightppb*5000)",
  // 		  ";M_{J} [GeV];Events / 5 fb^{-1}", 50, 0., 1500.,
  // 		  "plotSig:plotLog",-1,-1,
  // 		  "MJ_T1bbbb_geq4bm");
  // draw_from_trees("MJ",search+"num_csvm_jets30>=4","(weightppb*5000)",
  // 		  ";M_{J} [GeV];Events / 5 fb^{-1}", 50, 0., 1500.,
  // 		  "plotSig:plotLog:T1tttt",-1,-1,
  // 		  "MJ_T1tttt_geq4bm");
  // draw_from_trees("MJ",search+"num_csvm_jets30==2","(weightppb*5000)",
  // 		  ";M_{J} [GeV];Events / 5 fb^{-1}", 50, 0., 1500.,
  // 		  "plotSig:plotLog",-1,-1,
  // 		  "MJ_T1bbbb_2bm");
  // draw_from_trees("MJ",search+"num_csvm_jets30==2","(weightppb*5000)",
  // 		  ";M_{J} [GeV];Events / 5 fb^{-1}", 50, 0., 1500.,
  // 		  "plotSig:plotLog:T1tttt",-1,-1,
  // 		  "MJ_T1tttt_2bm");
  // draw_from_trees("MJ",search+"num_csvm_jets30==3","(weightppb*5000)",
  // 		  ";M_{J} [GeV];Events / 5 fb^{-1}", 50, 0., 1500.,
  // 		  "plotSig:plotLog",-1,-1,
  // 		  "MJ_T1bbbb_3bm");
  // draw_from_trees("MJ",search+"num_csvm_jets30==3","(weightppb*5000)",
  // 		  ";M_{J} [GeV];Events / 5 fb^{-1}", 50, 0., 1500.,
  // 		  "plotSig:plotLog:T1tttt",-1,-1,
  // 		  "MJ_T1tttt_3bm");
  // draw_from_trees("MJ",search+"num_csvm_jets30>=2","(weightppb*5000)",
  // 		  ";M_{J} [GeV];Events / 5 fb^{-1}", 50, 0., 1500.,
  // 		  "plotSig:plotLog",-1,-1,
  // 		  "MJ_T1bbbb_geq2bm");
  // draw_from_trees("MJ",search+"num_csvm_jets30>=2","(weightppb*5000)",
  // 		  ";M_{J} [GeV];Events / 5 fb^{-1}", 50, 0., 1500.,
  // 		  "plotSig:plotLog:T1tttt",-1,-1,
  // 		  "MJ_T1tttt_geq2bm");

  // draw_from_trees("fat_jet1_mJ",search+"num_csvm_jets30>=4","(weightppb*5000)",
  // 		  ";m_{J} (fat jet 1) [GeV];Events / 5 fb^{-1}", 50, 0., 1000.,
  // 		  "plotSig:plotLog",-1,-1,
  // 		  "fat_jet1_mJ_T1bbbb_geq4bm");
  // draw_from_trees("fat_jet1_mJ",search+"num_csvm_jets30>=4","(weightppb*5000)",
  // 		  ";m_{J} (fat jet 1) [GeV];Events / 5 fb^{-1}", 50, 0., 1000.,
  // 		  "plotSig:plotLog:T1tttt",-1,-1,
  // 		  "fat_jet1_mJ_T1tttt_geq4bm");
  // draw_from_trees("fat_jet1_mJ",search+"num_csvm_jets30==2","(weightppb*5000)",
  // 		  ";m_{J} (fat jet 1) [GeV];Events / 5 fb^{-1}", 50, 0., 1000.,
  // 		  "plotSig:plotLog",-1,-1,
  // 		  "fat_jet1_mJ_T1bbbb_2bm");
  // draw_from_trees("fat_jet1_mJ",search+"num_csvm_jets30==2","(weightppb*5000)",
  // 		  ";m_{J} (fat jet 1) [GeV];Events / 5 fb^{-1}", 50, 0., 1000.,
  // 		  "plotSig:plotLog:T1tttt",-1,-1,
  // 		  "fat_jet1_mJ_T1tttt_2bm");
  // draw_from_trees("fat_jet1_mJ",search+"num_csvm_jets30==3","(weightppb*5000)",
  // 		  ";m_{J} (fat jet 1) [GeV];Events / 5 fb^{-1}", 50, 0., 1000.,
  // 		  "plotSig:plotLog:T1tttt",-1,-1,
  // 		  "fat_jet1_mJ_T1tttt_3bm");
  // draw_from_trees("fat_jet1_mJ",search+"num_csvm_jets30==3","(weightppb*5000)",
  // 		  ";m_{J} (fat jet 1) [GeV];Events / 5 fb^{-1}", 50, 0., 1000.,
  // 		  "plotSig:plotLog",-1,-1,
  // 		  "fat_jet1_mJ_T1bbbb_3bm");
  // draw_from_trees("fat_jet1_mJ",search+"num_csvm_jets30>=2","(weightppb*5000)",
  // 		  ";m_{J} (fat jet 1) [GeV];Events / 5 fb^{-1}", 50, 0., 1000.,
  // 		  "plotSig:plotLog",-1,-1,
  // 		  "fat_jet1_mJ_T1bbbb_geq2bm");
  // draw_from_trees("fat_jet1_mJ",search+"num_csvm_jets30>=2","(weightppb*5000)",
  // 		  ";m_{J} (fat jet 1) [GeV];Events / 5 fb^{-1}", 50, 0., 1000.,
  // 		  "plotSig:plotLog:T1tttt",-1,-1,
  // 		  "fat_jet1_mJ_T1tttt_geq2bm");

  // draw_from_trees("fat_jet2_mJ",search+"num_csvm_jets30>=4","(weightppb*5000)",
  // 		  ";m_{J} (fat jet 2) [GeV];Events / 5 fb^{-1}", 40, 0., 800.,
  // 		  "plotSig:plotLog",-1,-1,
  // 		  "fat_jet2_mJ_T1bbbb_geq4bm");
  // draw_from_trees("fat_jet2_mJ",search+"num_csvm_jets30>=4","(weightppb*5000)",
  // 		  ";m_{J} (fat jet 2) [GeV];Events / 5 fb^{-1}", 40, 0., 800.,
  // 		  "plotSig:plotLog:T1tttt",-1,-1,
  // 		  "fat_jet2_mJ_T1tttt_geq4bm");
  // draw_from_trees("fat_jet2_mJ",search+"num_csvm_jets30==2","(weightppb*5000)",
  // 		  ";m_{J} (fat jet 2) [GeV];Events / 5 fb^{-1}", 40, 0., 800.,
  // 		  "plotSig:plotLog",-1,-1,
  // 		  "fat_jet2_mJ_T1bbbb_2bm");
  // draw_from_trees("fat_jet2_mJ",search+"num_csvm_jets30==2","(weightppb*5000)",
  // 		  ";m_{J} (fat jet 2) [GeV];Events / 5 fb^{-1}", 40, 0., 800.,
  // 		  "plotSig:plotLog:T1tttt",-1,-1,
  // 		  "fat_jet2_mJ_T1tttt_2bm");
  // draw_from_trees("fat_jet2_mJ",search+"num_csvm_jets30==3","(weightppb*5000)",
  // 		  ";m_{J} (fat jet 2) [GeV];Events / 5 fb^{-1}", 40, 0., 800.,
  // 		  "plotSig:plotLog",-1,-1,
  // 		  "fat_jet2_mJ_T1bbbb_3bm");
  // draw_from_trees("fat_jet2_mJ",search+"num_csvm_jets30==3","(weightppb*5000)",
  // 		  ";m_{J} (fat jet 2) [GeV];Events / 5 fb^{-1}", 40, 0., 800.,
  // 		  "plotSig:plotLog:T1tttt",-1,-1,
  // 		  "fat_jet2_mJ_T1tttt_3bm");
  // draw_from_trees("fat_jet2_mJ",search+"num_csvm_jets30>=2","(weightppb*5000)",
  // 		  ";m_{J} (fat jet 2) [GeV];Events / 5 fb^{-1}", 40, 0., 800.,
  // 		  "plotSig:plotLog",-1,-1,
  // 		  "fat_jet2_mJ_T1bbbb_geq2bm");
  // draw_from_trees("fat_jet2_mJ",search+"num_csvm_jets30>=2","(weightppb*5000)",
  // 		  ";m_{J} (fat jet 2) [GeV];Events / 5 fb^{-1}", 40, 0., 800.,
  // 		  "plotSig:plotLog:T1tttt",-1,-1,
  // 		  "fat_jet2_mJ_T1tttt_geq2bm");

  // draw_from_trees("fat_jet1_pt",search+"num_csvm_jets30>=4","(weightppb*5000)",
  // 		  ";p_{T} (fat jet 1) [GeV];Events / 5 fb^{-1}", 50, 150., 1550.,
  // 		  "plotSig:plotLog",-1,-1,
  // 		  "fat_jet1_pt_T1bbbb_geq4bm");
  // draw_from_trees("fat_jet1_pt",search+"num_csvm_jets30>=4","(weightppb*5000)",
  // 		  ";p_{T} (fat jet 1) [GeV];Events / 5 fb^{-1}", 50, 150., 1550.,
  // 		  "plotSig:plotLog:T1tttt",-1,-1,
  // 		  "fat_jet1_pt_T1tttt_geq4bm");
  // draw_from_trees("fat_jet1_pt",search+"num_csvm_jets30==2","(weightppb*5000)",
  // 		  ";p_{T} (fat jet 1) [GeV];Events / 5 fb^{-1}", 50, 150., 1550.,
  // 		  "plotSig:plotLog",-1,-1,
  // 		  "fat_jet1_pt_T1bbbb_2bm");
  // draw_from_trees("fat_jet1_pt",search+"num_csvm_jets30==2","(weightppb*5000)",
  // 		  ";p_{T} (fat jet 1) [GeV];Events / 5 fb^{-1}", 50, 150., 1550.,
  // 		  "plotSig:plotLog:T1tttt",-1,-1,
  // 		  "fat_jet1_pt_T1tttt_2bm");
  // draw_from_trees("fat_jet1_pt",search+"num_csvm_jets30==3","(weightppb*5000)",
  // 		  ";p_{T} (fat jet 1) [GeV];Events / 5 fb^{-1}", 50, 150., 1550.,
  // 		  "plotSig:plotLog",-1,-1,
  // 		  "fat_jet1_pt_T1bbbb_3bm");
  // draw_from_trees("fat_jet1_pt",search+"num_csvm_jets30==3","(weightppb*5000)",
  // 		  ";p_{T} (fat jet 1) [GeV];Events / 5 fb^{-1}", 50, 150., 1550.,
  // 		  "plotSig:plotLog:T1tttt",-1,-1,
  // 		  "fat_jet1_pt_T1tttt_3bm");
  // draw_from_trees("fat_jet1_pt",search+"num_csvm_jets30>=2","(weightppb*5000)",
  // 		  ";p_{T} (fat jet 1) [GeV];Events / 5 fb^{-1}", 50, 150., 1550.,
  // 		  "plotSig:plotLog",-1,-1,
  // 		  "fat_jet1_pt_T1bbbb_geq2bm");
  // draw_from_trees("fat_jet1_pt",search+"num_csvm_jets30>=2","(weightppb*5000)",
  // 		  ";p_{T} (fat jet 1) [GeV];Events / 5 fb^{-1}", 50, 150., 1550.,
  // 		  "plotSig:plotLog:T1tttt",-1,-1,
  // 		  "fat_jet1_pt_T1tttt_geq2bm");

  // draw_from_trees("fat_jet2_pt",search+"num_csvm_jets30>=4","(weightppb*5000)",
  // 		  ";p_{T} (fat jet 2) [GeV];Events / 5 fb^{-1}", 30, 20., 920.,
  // 		  "plotSig:plotLog",-1,-1,
  // 		  "fat_jet2_pt_T1bbbb_geq4bm");
  // draw_from_trees("fat_jet2_pt",search+"num_csvm_jets30>=4","(weightppb*5000)",
  // 		  ";p_{T} (fat jet 2) [GeV];Events / 5 fb^{-1}", 30, 20., 920.,
  // 		  "plotSig:plotLog:T1tttt",-1,-1,
  // 		  "fat_jet2_pt_T1tttt_geq4bm");
  // draw_from_trees("fat_jet2_pt",search+"num_csvm_jets30==2","(weightppb*5000)",
  // 		  ";p_{T} (fat jet 2) [GeV];Events / 5 fb^{-1}", 30, 20., 920.,
  // 		  "plotSig:plotLog",-1,-1,
  // 		  "fat_jet2_pt_T1bbbb_2bm");
  // draw_from_trees("fat_jet2_pt",search+"num_csvm_jets30==2","(weightppb*5000)",
  // 		  ";p_{T} (fat jet 2) [GeV];Events / 5 fb^{-1}", 30, 20., 920.,
  // 		  "plotSig:plotLog:T1tttt",-1,-1,
  // 		  "fat_jet2_pt_T1tttt_2bm");
  // draw_from_trees("fat_jet2_pt",search+"num_csvm_jets30==3","(weightppb*5000)",
  // 		  ";p_{T} (fat jet 2) [GeV];Events / 5 fb^{-1}", 30, 20., 920.,
  // 		  "plotSig:plotLog",-1,-1,
  // 		  "fat_jet2_pt_T1bbbb_3bm");
  // draw_from_trees("fat_jet2_pt",search+"num_csvm_jets30==3","(weightppb*5000)",
  // 		  ";p_{T} (fat jet 2) [GeV];Events / 5 fb^{-1}", 30, 20., 920.,
  // 		  "plotSig:plotLog:T1tttt",-1,-1,
  // 		  "fat_jet2_pt_T1tttt_3bm");
  // draw_from_trees("fat_jet2_pt",search+"num_csvm_jets30>=2","(weightppb*5000)",
  // 		  ";p_{T} (fat jet 2) [GeV];Events / 5 fb^{-1}", 30, 20., 920.,
  // 		  "plotSig:plotLog",-1,-1,
  // 		  "fat_jet2_pt_T1bbbb_geq2bm");
  // draw_from_trees("fat_jet2_pt",search+"num_csvm_jets30>=2","(weightppb*5000)",
  // 		  ";p_{T} (fat jet 2) [GeV];Events / 5 fb^{-1}", 30, 20., 920.,
  // 		  "plotSig:plotLog:T1tttt",-1,-1,
  // 		  "fat_jet2_pt_T1tttt_geq2bm");

  // draw_from_trees("num_fat_jets",search+"num_csvm_jets30>=4","(weightppb*5000)",
  // 		  ";n_{J};Events / 5 fb^{-1}", 10, 0., 10.,
  // 		  "plotSig:",-1,-1,
  // 		  "num_fat_jets_T1bbbb_geq4bm");
  // draw_from_trees("num_fat_jets",search+"num_csvm_jets30>=4","(weightppb*5000)",
  // 		  ";n_{J};Events / 5 fb^{-1}", 10, 0., 10.,
  // 		  "plotSig:T1tttt",-1,-1,
  // 		  "num_fat_jets_T1tttt_geq4bm");
  // draw_from_trees("num_fat_jets",search+"num_csvm_jets30==2","(weightppb*5000)",
  // 		  ";n_{J};Events / 5 fb^{-1}", 10, 0., 10.,
  // 		  "plotSig:",-1,-1,
  // 		  "num_fat_jets_T1bbbb_2bm");
  // draw_from_trees("num_fat_jets",search+"num_csvm_jets30==2","(weightppb*5000)",
  // 		  ";n_{J};Events / 5 fb^{-1}", 10, 0., 10.,
  // 		  "plotSig:T1tttt",-1,-1,
  // 		  "num_fat_jets_T1tttt_2bm");
  // draw_from_trees("num_fat_jets",search+"num_csvm_jets30==3","(weightppb*5000)",
  // 		  ";n_{J};Events / 5 fb^{-1}", 10, 0., 10.,
  // 		  "plotSig:",-1,-1,
  // 		  "num_fat_jets_T1bbbb_3bm");
  // draw_from_trees("num_fat_jets",search+"num_csvm_jets30==3","(weightppb*5000)",
  // 		  ";n_{J};Events / 5 fb^{-1}", 10, 0., 10.,
  // 		  "plotSig:T1tttt",-1,-1,
  // 		  "num_fat_jets_T1tttt_3bm");
  // draw_from_trees("num_fat_jets",search+"num_csvm_jets30>=2","(weightppb*5000)",
  // 		  ";n_{J};Events / 5 fb^{-1}", 10, 0., 10.,
  // 		  "plotSig:",-1,-1,
  // 		  "num_fat_jets_T1bbbb_geq2bm");
  // draw_from_trees("num_fat_jets",search+"num_csvm_jets30>=2","(weightppb*5000)",
  // 		  ";n_{J};Events / 5 fb^{-1}", 10, 0., 10.,
  // 		  "plotSig:T1tttt",-1,-1,
  // 		  "num_fat_jets_T1tttt_geq2bm");

  // draw_from_trees("num_fat_jets_pt100",search+"num_csvm_jets30>=4","(weightppb*5000)",
  // 		  ";n_{J} (p_{T} > 100 GeV);Events / 5 fb^{-1}", 10, 0., 10.,
  // 		  "plotSig:",-1,-1,
  // 		  "num_fat_jets_pt100_T1bbbb_geq4bm");
  // draw_from_trees("num_fat_jets_pt100",search+"num_csvm_jets30>=4","(weightppb*5000)",
  // 		  ";n_{J} (p_{T} > 100 GeV);Events / 5 fb^{-1}", 10, 0., 10.,
  // 		  "plotSig:T1tttt",-1,-1,
  // 		  "num_fat_jets_pt100_T1tttt_geq4bm");
  // draw_from_trees("num_fat_jets_pt100",search+"num_csvm_jets30==2","(weightppb*5000)",
  // 		  ";n_{J} (p_{T} > 100 GeV);Events / 5 fb^{-1}", 10, 0., 10.,
  // 		  "plotSig:",-1,-1,
  // 		  "num_fat_jets_pt100_T1bbbb_2bm");
  // draw_from_trees("num_fat_jets_pt100",search+"num_csvm_jets30==2","(weightppb*5000)",
  // 		  ";n_{J} (p_{T} > 100 GeV);Events / 5 fb^{-1}", 10, 0., 10.,
  // 		  "plotSig:T1tttt",-1,-1,
  // 		  "num_fat_jets_pt100_T1tttt_2bm");
  // draw_from_trees("num_fat_jets_pt100",search+"num_csvm_jets30==3","(weightppb*5000)",
  // 		  ";n_{J} (p_{T} > 100 GeV);Events / 5 fb^{-1}", 10, 0., 10.,
  // 		  "plotSig:",-1,-1,
  // 		  "num_fat_jets_pt100_T1bbbb_3bm");
  // draw_from_trees("num_fat_jets_pt100",search+"num_csvm_jets30==3","(weightppb*5000)",
  // 		  ";n_{J} (p_{T} > 100 GeV);Events / 5 fb^{-1}", 10, 0., 10.,
  // 		  "plotSig:T1tttt",-1,-1,
  // 		  "num_fat_jets_pt100_T1tttt_3bm");
  // draw_from_trees("num_fat_jets_pt100",search+"num_csvm_jets30>=2","(weightppb*5000)",
  // 		  ";n_{J} (p_{T} > 100 GeV);Events / 5 fb^{-1}", 10, 0., 10.,
  // 		  "plotSig:",-1,-1,
  // 		  "num_fat_jets_pt100_T1bbbb_geq2bm");
  // draw_from_trees("num_fat_jets_pt100",search+"num_csvm_jets30>=2","(weightppb*5000)",
  // 		  ";n_{J} (p_{T} > 100 GeV);Events / 5 fb^{-1}", 10, 0., 10.,
  // 		  "plotSig:T1tttt",-1,-1,
  // 		  "num_fat_jets_pt100_T1tttt_geq2bm");

  // draw_from_trees("met_over_sqrt_ht50",search+"num_csvm_jets30>=4","(weightppb*5000)",
  // 		  ";E_{T}^{miss}/#sqrt{H_{T}} [GeV^{1/2}];Events / 20 fb^{-1}", 25, 0., 50.,
  // 		  "plotSig:plotLog:T1tttt",-1,-1,
  // 		  "met_over_sqrt_ht50_t1tttt_geq4bm"); 
  // draw_from_trees("met_over_sqrt_ht50",search+"num_csvm_jets30>=4","(weightppb*5000)",
  // 		  ";E_{T}^{miss}/#sqrt{H_{T}} [GeV^{1/2}];Events / 20 fb^{-1}", 25, 0., 50.,
  // 		  "plotSig:plotLog:",-1,-1,
  // 		  "met_over_sqrt_ht50_t1bbbb_geq4bm"); 
  // draw_from_trees("met_over_sqrt_ht50",search+"num_csvm_jets30==2","(weightppb*5000)",
  // 		  ";E_{T}^{miss}/#sqrt{H_{T}} [GeV^{1/2}];Events / 20 fb^{-1}", 25, 0., 50.,
  // 		  "plotSig:plotLog:T1tttt",-1,-1,
  // 		  "met_over_sqrt_ht50_t1tttt_2bm"); 
  // draw_from_trees("met_over_sqrt_ht50",search+"num_csvm_jets30==2","(weightppb*5000)",
  // 		  ";E_{T}^{miss}/#sqrt{H_{T}} [GeV^{1/2}];Events / 20 fb^{-1}", 25, 0., 50.,
  // 		  "plotSig:plotLog:",-1,-1,
  // 		  "met_over_sqrt_ht50_t1bbbb_2bm"); 
  // draw_from_trees("met_over_sqrt_ht50",search+"num_csvm_jets30==3","(weightppb*5000)",
  // 		  ";E_{T}^{miss}/#sqrt{H_{T}} [GeV^{1/2}];Events / 20 fb^{-1}", 25, 0., 50.,
  // 		  "plotSig:plotLog:",-1,-1,
  // 		  "met_over_sqrt_ht50_t1bbbb_3bm"); 
  // draw_from_trees("met_over_sqrt_ht50",search+"num_csvm_jets30==3","(weightppb*5000)",
  // 		  ";E_{T}^{miss}/#sqrt{H_{T}} [GeV^{1/2}];Events / 20 fb^{-1}", 25, 0., 50.,
  // 		  "plotSig:plotLog:T1tttt",-1,-1,
  // 		  "met_over_sqrt_ht50_t1tttt_3bm"); 
  // draw_from_trees("met_over_sqrt_ht50",search+"num_csvm_jets30>=2","(weightppb*5000)",
  // 		  ";E_{T}^{miss}/#sqrt{H_{T}} [GeV^{1/2}];Events / 20 fb^{-1}", 25, 0., 50.,
  // 		  "plotSig:plotLog:T1tttt",-1,-1,
  // 		  "met_over_sqrt_ht50_t1tttt_geq2bm"); 
  // draw_from_trees("met_over_sqrt_ht50",search+"num_csvm_jets30>=2","(weightppb*5000)",
  // 		  ";E_{T}^{miss}/#sqrt{H_{T}} [GeV^{1/2}];Events / 20 fb^{-1}", 25, 0., 50.,
  // 		  "plotSig:plotLog:",-1,-1,
  // 		  "met_over_sqrt_ht50_t1bbbb_geq2bm"); 
 
  return 0;
  
}

