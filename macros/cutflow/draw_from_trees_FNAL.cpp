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
TChain* t1qqqq_1400_100_ch;
TChain* t1qqqq_1000_800_ch;


using namespace std;

TString plotdir = "plots/n_minus_1/FNAL/";

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
  bool qqqq = options.Contains("qqqq") && (!options.Contains("!qqqq"));

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
  TH1D * ht1qqqq_1400_100 = new TH1D("t1qqqq_1400_100" , title, nbinsx, xlow, xup);
  TH1D * ht1qqqq_1000_800 = new TH1D("t1qqqq_1000_800" , title, nbinsx, xlow, xup);

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
  t1qqqq_1400_100_ch->Project("t1qqqq_1400_100",var,(cut)*weights);
  t1qqqq_1000_800_ch->Project("t1qqqq_1000_800",var,(cut)*weights);


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
    i_overflow=ht1qqqq_1400_100->IntegralAndError(nbinsx,nbinsx+1,e_overflow);
    ht1qqqq_1400_100->SetBinContent(nbinsx, i_overflow);
    ht1qqqq_1400_100->SetBinError(nbinsx, e_overflow);
    i_overflow=ht1qqqq_1000_800->IntegralAndError(nbinsx,nbinsx+1,e_overflow);
    ht1qqqq_1000_800->SetBinContent(nbinsx, i_overflow);
    ht1qqqq_1000_800->SetBinError(nbinsx, e_overflow);
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
  
  Double_t ttbar_e(0.), qcd_e(0.), znn_e(0.), wjets_e(0.), other_e(0.), single_top_e(0.), bg_tot_e(0.), t1bbbb_1500_100_e(0.);
  double ttbar_n(httbar->IntegralAndError(0,nbinsx+1, ttbar_e));
  double qcd_n(hqcd->IntegralAndError(0,nbinsx+1, qcd_e));
  double znn_n(hznn->IntegralAndError(0,nbinsx+1, znn_e));
  double wjets_n(hwjets->IntegralAndError(0,nbinsx+1, wjets_e));
  double other_n(hother->IntegralAndError(0,nbinsx+1, other_e));
  double single_top_n(hsingle_top->IntegralAndError(0,nbinsx+1, single_top_e));
  double bg_tot(hmc_exp->IntegralAndError(0,nbinsx+1, bg_tot_e));
  double t1bbbb_1500_100_n(ht1bbbb_1500_100->IntegralAndError(0,nbinsx+1, t1bbbb_1500_100_e));


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
	 t1bbbb_1500_100_n,t1bbbb_1500_100_e);


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
  if (plot_title.Contains("1119")) {
   hs->Add(hqcd);
   hs->Add(httbar);
  }else {
    hs->Add(httbar);
    hs->Add(hqcd);
  }


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
    if (qqqq) {
      leg1->AddEntry(ht1qqqq_1400_100, "#splitline{T1qqqq}{(1400,100) GeV}", "l");
      leg1->AddEntry(ht1qqqq_1000_800, "#splitline{T1qqqq}{(1000,800) GeV}", "l");
    } else {
      leg1->AddEntry(ht1bbbb_1500_100, "#splitline{T1bbbb}{(1500,100) GeV}", "l");
      leg1->AddEntry(ht1bbbb_1000_900, "#splitline{T1bbbb}{(1000,900) GeV}", "l");
    }
      leg1->AddEntry(ht1tttt_1500_100, "#splitline{T1tttt}{(1500,100) GeV}", "l");
      leg1->AddEntry(ht1tttt_1200_800, "#splitline{T1tttt}{(1200,800) GeV}", "l");
    }
 

  TLegend * leg2 = new TLegend(0.72, 0.68, 0.94, 0.92);
  set_style(leg2,0.025);
  if (plot_title.Contains("1119")) {
    leg2->AddEntry(httbar, "t#bar{t}", "f");
    leg2->AddEntry(hqcd, "QCD", "f");
  }else{
    leg2->AddEntry(hqcd, "QCD", "f");
    leg2->AddEntry(httbar, "t#bar{t}", "f");
  }
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
    ht1qqqq_1400_100->SetLineColor(1006);
    ht1qqqq_1400_100->SetLineWidth(4);
    ht1qqqq_1400_100->SetFillColor(0);
    ht1qqqq_1000_800->SetLineColor(1006);
    ht1qqqq_1000_800->SetLineWidth(4);
    ht1qqqq_1000_800->SetLineStyle(7);
    ht1qqqq_1000_800->SetFillColor(0);
    if (qqqq) {
      ht1qqqq_1400_100->Draw("hist same");
      ht1qqqq_1000_800->Draw("hist same");
    } else{
      ht1bbbb_1500_100->Draw("hist same");
      ht1bbbb_1000_900->Draw("hist same");
    }
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
  delete ht1qqqq_1400_100;
  delete ht1qqqq_1000_800;
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
  t1qqqq_1400_100_ch = new TChain("reduced_tree");
  t1qqqq_1000_800_ch = new TChain("reduced_tree");

  
  TFileCollection* ttbar_fc = new TFileCollection("ttbar_fc","","../../reduced_trees/tfcs/ttbar.txt");
  TFileCollection* qcd_fc = new TFileCollection("qcd_fc","","../../reduced_trees/tfcs/qcd_v2.txt");
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
  t1qqqq_1400_100_ch->Add("../../reduced_trees/SMS-T1qqqq_2J_mGl-1400_mLSP-100_Tune4C_13TeV-madgraph-tauola_Spring14miniaod-PU20bx25_POSTLS170_V5-v1_MINIAODSIM_UCSB2140_v75/*.root");
  t1qqqq_1000_800_ch->Add("../../reduced_trees/SMS-T1qqqq_2J_mGl-1000_mLSP-800_Tune4C_13TeV-madgraph-tauola_Spring14miniaod-PU20bx25_POSTLS170_V5-v1_MINIAODSIM_UCSB2139_v75/*.root");

  cout << "Entries in chains..." << endl;
  cout << "t1tttt_1500_100 N=" << t1tttt_1500_100_ch->GetEntries() << endl;
  cout << "t1tttt_1200_800 N=" << t1tttt_1200_800_ch->GetEntries() << endl;
  cout << "t1bbbb_1500_100 N=" << t1bbbb_1500_100_ch->GetEntries() << endl;
  cout << "t1bbbb_1000_900 N=" << t1bbbb_1000_900_ch->GetEntries() << endl;
  cout << "t1qqqq_1400_100 N=" << t1qqqq_1400_100_ch->GetEntries() << endl;
  cout << "t1qqqq_1000_800 N=" << t1qqqq_1000_800_ch->GetEntries() << endl;
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

  // TCut t1tttt_1500_100_ht("ht30>500&&met>600&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_csvm_jets>=2&&num_jets_pt30>=10");
  // TCut t1tttt_1500_100_mj("fatpT30_MJ>250&&met>600&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_csvm_jets>=2&&num_jets_pt30>=10");


  draw_from_trees("num_jets_pt50","ht30>1000&&met>400&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_csvm_jets30>=2","(weightppb*5000)",
  		  ";n_{jets} (p_{T} > 50 GeV);Events / 5 fb^{-1}", 15, 0, 15,
  		  "plotSig:plotLog",-1,-1,
  		  "num_jets_pt50_t1bbbb_1500_100_ht_1119"); 
  draw_from_trees("num_jets_pt30","ht30>1000&&met>400&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_csvm_jets30>=2","(weightppb*5000)",
  		  ";n_{jets} (p_{T} > 30 GeV);Events / 5 fb^{-1}", 15, 0, 15,
  		  "plotSig:plotLog",-1,-1,
  		  "num_jets_pt30_t1bbbb_1500_100_ht_1119"); 

  draw_from_trees("num_jets_pt50","ht30>500&&met>500&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_csvm_jets30>=2","(weightppb*5000)",
  		  ";n_{jets} (p_{T} > 50 GeV);Events / 5 fb^{-1}", 15, 0, 15,
  		  "plotSig:plotLog",-1,-1,
  		  "num_jets_pt50_t1bbbb_1000_900_ht_1119");


  draw_from_trees("num_jets_pt50","ht30>400&&met>500&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0","(weightppb*5000)",
  		  ";n_{jets} (p_{T} > 50 GeV);Events / 5 fb^{-1}", 15, 0, 15,
  		  "qqqq:plotSig:plotLog",-1,-1,
  		  "num_jets_pt50_t1qqqq_1000_800_ht_1119");
  draw_from_trees("num_jets_pt50","ht30>1400&&met>500&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0","(weightppb*5000)",
  		  ";n_{jets} (p_{T} > 50 GeV);Events / 5 fb^{-1}", 15, 0, 15,
  		  "qqqq:plotSig:plotLog",-1,-1,
  		  "num_jets_pt50_t1qqqq_1400_100_ht_1119");

  draw_from_trees("num_jets_pt30","ht30>500&&met>500&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_csvm_jets30>=2","(weightppb*5000)",
  		  ";n_{jets} (p_{T} > 30 GeV);Events / 5 fb^{-1}", 15, 0, 15,
  		  "plotSig:plotLog",-1,-1,
  		  "num_jets_pt30_t1bbbb_1000_900_ht_1119");
  draw_from_trees("num_jets_pt30","ht30>1400&&met>600&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_csvm_jets30>=2","(weightppb*5000)",
  		  ";n_{jets} (p_{T} > 30 GeV);Events / 5 fb^{-1}", 15, 0, 15,
  		  "plotSig:plotLog",-1,-1,
  		  "num_jets_pt30_t1bbbb_1500_100_ht_1119");

  draw_from_trees("num_jets_pt30","ht30>400&&met>500&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0","(weightppb*5000)",
  		  ";n_{jets} (p_{T} > 30 GeV);Events / 5 fb^{-1}", 15, 0, 15,
  		  "qqqq:plotSig:plotLog",-1,-1,
  		  "num_jets_pt30_t1qqqq_1000_800_ht_1119");
  draw_from_trees("num_jets_pt30","ht30>1400&&met>500&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0","(weightppb*5000)",
  		  ";n_{jets} (p_{T} > 30 GeV);Events / 5 fb^{-1}", 15, 0, 15,
  		  "qqqq:plotSig:plotLog",-1,-1,
  		  "num_jets_pt30_t1qqqq_1400_100_ht_1119");


  draw_from_trees("met","ht30>500&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>=4","(weightppb*5000)",
   		  ";E_{T}^{miss} [GeV];Events / 5 fb^{-1}", 15, 0, 1500.,
   		  "qqqq:plotSig:plotLog",-1,-1,
   		  "met_t1qqqq_1400_100_ht_1118_workshop");

  draw_from_trees("ht30","met>500&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_csvm_jets>=3&&num_jets_pt30>=4","(weightppb*10000)",
   		  ";H_{T} [GeV];Events / 5 fb^{-1}", 15, 0, 3000,
   		  "plotSig:",-1,-1,
   		  "ht30_t1bbbb_1000_900_1112_4j_3b_met500_10ifb");
  draw_from_trees("fatpT30_MJ","met>500&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_csvm_jets>=3&&num_jets_pt30>=4","(weightppb*10000)",
   		  ";M_{J} [GeV];Events / 5 fb^{-1}", 15, 0, 1500,
   		  "plotSig",-1,-1,
   		  "fatpT30_MJ_t1bbbb_1000_900_1112_4j_3b_met500_10ifb");

  draw_from_trees("ht30","met>500&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_csvm_jets>=2&&num_jets_pt30>=6","(weightppb*10000)",
   		  ";H_{T} [GeV];Events / 5 fb^{-1}", 15, 0, 3000,
   		  "plotSig:",-1,-1,
   		  "ht30_t1tttt_1500_100_1112_6j_2b_met500_10ifb");
  draw_from_trees("fatpT30_MJ","met>500&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_csvm_jets>=2&&num_jets_pt30>=6","(weightppb*10000)",
   		  ";M_{J} [GeV];Events / 5 fb^{-1}", 15, 0, 1500,
   		  "plotSig",-1,-1,
   		  "fatpT30_MJ_t1tttt_1500_100_1112_6j_2b_met500_10ifb");

  draw_from_trees("ht30","met>600&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_csvm_jets>=2&&num_jets_pt30>=10","(weightppb*10000)",
   		  ";H_{T} [GeV];Events / 5 fb^{-1}", 15, 0, 3000,
   		  "plotSig:",-1,-1,
   		  "ht30_t1tttt_1500_100_1112_10j_2b_met600_10ifb");
  draw_from_trees("fatpT30_MJ","met>600&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_csvm_jets>=2&&num_jets_pt30>=10","(weightppb*10000)",
   		  ";M_{J} [GeV];Events / 5 fb^{-1}", 15, 0, 1500,
   		  "plotSig",-1,-1,
   		  "fatpT30_MJ_t1tttt_1500_100_1112_10j_2b_met600_10ifb");



  // TCut t1qqqq_1000_800_ht("ht30>500&&met>600&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>=8");
  // TCut t1qqqq_1000_800_mj("fatpT30_MJ>200&&met>600&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>=8");

  draw_from_trees("met","ht30>500&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>=8","(weightppb*5000)",
   		  ";E_{T}^{miss} [GeV];Events / 5 fb^{-1}", 15, 0, 1500.,
   		  "qqqq:plotSig:plotLog",-1,-1,
   		  "met_t1qqqq_1000_800_ht_1112");
  draw_from_trees("num_jets_pt30","ht30>500&&met>600&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0","(weightppb*5000)",
  		  ";n_{jets} (p_{T} > 30 GeV);Events / 5 fb^{-1}", 15, 0, 15,
  		  "qqqq:plotSig:plotLog",-1,-1,
  		  "num_jets_pt30_t1qqqq_1000_800_ht_1112");
  draw_from_trees("ht30","met>600&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>=8","(weightppb*5000)",
   		  ";H_{T} [GeV];Events / 5 fb^{-1}", 15, 0, 3000,
   		  "qqqq:plotSig:",-1,-1,
   		  "ht30_t1qqqq_1000_800_ht_1112");

  draw_from_trees("met","fatpT30_MJ>200&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>=8","(weightppb*5000)",
   		  ";E_{T}^{miss} [GeV];Events / 5 fb^{-1}", 15, 0, 1500.,
   		  "qqqq:plotSig:plotLog",-1,-1,
   		  "met_t1qqqq_1000_800_mj_1112");
  draw_from_trees("num_jets_pt30","fatpT30_MJ>200&&met>600&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0","(weightppb*5000)",
  		  ";n_{jets} (p_{T} > 30 GeV);Events / 5 fb^{-1}", 15, 0, 15,
  		  "qqqq:plotSig:plotLog",-1,-1,
  		  "num_jets_pt30_t1qqqq_1000_800_mj_1112");
  draw_from_trees("fatpT30_MJ","met>600&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>=8","(weightppb*5000)",
   		  ";M_{J} [GeV];Events / 5 fb^{-1}", 15, 0, 1500,
   		  "qqqq:plotSig",-1,-1,
   		  "fatpT30_MJ_t1qqqq_1000_800_mj_1112");

  // TCut t1qqqq_1400_100_ht("ht30>500&&met>650&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&jet4_pt>200");
  // TCut t1qqqq_1400_100_mj("fatpT30_MJ>200&&met>650&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&jet4_pt>200");



  draw_from_trees("jet4_pt","ht30>500&&met>650&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0","(weightppb*5000)",
  		  ";p^{jet 4}_{T} [GeV];Events / 5 fb^{-1}", 10, 0, 500,
  		  "qqqq:plotSig:plotLog",-1,-1,
  		  "jet4_pt_t1qqqq_1400_100_ht_1112");
  draw_from_trees("ht30","met>650&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&jet4_pt>200","(weightppb*5000)",
   		  ";H_{T} [GeV];Events / 5 fb^{-1}", 15, 0, 3000,
   		  "qqqq:plotSig:",-1,-1,
   		  "ht30_t1qqqq_1400_100_ht_1112");

  draw_from_trees("met","fatpT30_MJ>200&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&jet4_pt>200","(weightppb*5000)",
   		  ";E_{T}^{miss} [GeV];Events / 5 fb^{-1}", 15, 0, 1500.,
   		  "qqqq:plotSig:plotLog",-1,-1,
   		  "met_t1qqqq_1400_100_mj_1112");
  draw_from_trees("jet4_pt","fatpT30_MJ>200&&met>650&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0","(weightppb*5000)",
  		  ";p^{jet 4}_{T} [GeV];Events / 5 fb^{-1}", 10, 0, 500,
  		  "qqqq:plotSig:plotLog",-1,-1,
  		  "jet4_pt_t1qqqq_1400_100_mj_1112");
  draw_from_trees("fatpT30_MJ","met>650&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&jet4_pt>200","(weightppb*5000)",
   		  ";M_{J} [GeV];Events / 5 fb^{-1}", 15, 0, 1500,
   		  "qqqq:plotSig",-1,-1,
   		  "fatpT30_MJ_t1qqqq_1400_100_mj_1112");

  // TCut t1bbbb_1000_900_ht("ht30>100&&met>800&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_csvm_jets>=2&&num_jets_pt30>=6");
  // TCut t1bbbb_1000_900_mj("fatpT30_MJ>100&&met>800&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_csvm_jets>=2&&num_jets_pt30>=6");

  draw_from_trees("met","ht30>100&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_csvm_jets>=2&&num_jets_pt30>=6","(weightppb*5000)",
   		  ";E_{T}^{miss} [GeV];Events / 5 fb^{-1}", 15, 0, 1500.,
   		  "plotSig:plotLog",-1,-1,
   		  "met_t1bbbb_1000_900_ht_1112");
  draw_from_trees("num_jets_pt30","ht30>100&&met>800&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_csvm_jets>=2","(weightppb*5000)",
  		  ";n_{jets} (p_{T} > 30 GeV);Events / 5 fb^{-1}", 15, 0, 15,
  		  "plotSig:",-1,-1,
  		  "num_jets_pt30_t1bbbb_1000_900_ht_1112");
  draw_from_trees("ht30","met>800&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_csvm_jets>=2&&num_jets_pt30>=6","(weightppb*5000)",
   		  ";H_{T} [GeV];Events / 5 fb^{-1}", 15, 0, 3000,
   		  "plotSig:",-1,-1,
   		  "ht30_t1bbbb_1000_900_ht_1112");

  draw_from_trees("met","fatpT30_MJ>100&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_csvm_jets>=2&&num_jets_pt30>=6","(weightppb*5000)",
   		  ";E_{T}^{miss} [GeV];Events / 5 fb^{-1}", 15, 0, 1500.,
   		  "plotSig:plotLog",-1,-1,
   		  "met_t1bbbb_1000_900_mj_1112");
  draw_from_trees("num_jets_pt30","fatpT30_MJ>100&&met>800&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_csvm_jets>=2","(weightppb*5000)",
  		  ";n_{jets} (p_{T} > 30 GeV);Events / 5 fb^{-1}", 15, 0, 15,
  		  "plotSig:",-1,-1,
  		  "num_jets_pt30_t1bbbb_1000_900_mj_1112");
  draw_from_trees("fatpT30_MJ","met>800&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_csvm_jets>=2&&num_jets_pt30>=6","(weightppb*5000)",
   		  ";M_{J} [GeV];Events / 5 fb^{-1}", 15, 0, 1500,
   		  "plotSig",-1,-1,
   		  "fatpT30_MJ_t1bbbb_1000_900_mj_1112");

  // TCut t1bbbb_1500_100_ht("ht30>500&&met>750&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_csvm_jets>=2&&jet4_pt>200");
  // TCut t1bbbb_1500_100_mj("fatpT30_MJ>550&&met>750&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_csvm_jets>=2&&jet4_pt>200");

  draw_from_trees("met","ht30>500&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_csvm_jets>=2&&jet4_pt>200","(weightppb*5000)",
   		  ";E_{T}^{miss} [GeV];Events / 5 fb^{-1}", 15, 0, 1500.,
   		  "plotSig:plotLog",-1,-1,
   		  "met_t1bbbb_1500_100_ht_1112");
  draw_from_trees("jet4_pt","ht30>500&&met>750&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_csvm_jets>=2","(weightppb*5000)",
  		  ";p^{jet 4}_{T} [GeV];Events / 5 fb^{-1}", 10, 0, 500,
  		  "plotSig:",-1,-1,
  		  "jet4_pt_t1bbbb_1500_100_ht_1112");
  draw_from_trees("ht30","met>750&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_csvm_jets>=2&&jet4_pt>200","(weightppb*5000)",
   		  ";H_{T} [GeV];Events / 5 fb^{-1}", 15, 0, 3000,
   		  "plotSig:",-1,-1,
   		  "ht30_t1bbbb_1500_100_ht_1112");

  draw_from_trees("met","fatpT30_MJ>550&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_csvm_jets>=2&&jet4_pt>200","(weightppb*5000)",
   		  ";E_{T}^{miss} [GeV];Events / 5 fb^{-1}", 15, 0, 1500.,
   		  "plotSig:plotLog",-1,-1,
   		  "met_t1bbbb_1500_100_mj_1112");
  draw_from_trees("jet4_pt","fatpT30_MJ>550&&met>750&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_csvm_jets>=2","(weightppb*5000)",
  		  ";p^{jet 4}_{T} [GeV];Events / 5 fb^{-1}", 10, 0, 500,
  		  "plotSig:",-1,-1,
  		  "jet4_pt_t1bbbb_1500_100_mj_1112");
  draw_from_trees("fatpT30_MJ","met>750&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_csvm_jets>=2&&jet4_pt>200","(weightppb*5000)",
   		  ";M_{J} [GeV];Events / 5 fb^{-1}", 15, 0, 1500,
   		  "plotSig",-1,-1,
   		  "fatpT30_MJ_t1bbbb_1500_100_mj_1112");

  // TCut t1tttt_1200_800_ht("ht30>500&&met>600&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_csvm_jets>=2&&num_jets_pt30>=10");
  // TCut t1tttt_1200_800_mj("fatpT30_MJ>250&&met>600&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_csvm_jets>=2&&num_jets_pt30>=10");

  draw_from_trees("met","ht30>200&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_csvm_jets>=3&&num_jets_pt30>=10","(weightppb*5000)",
   		  ";E_{T}^{miss} [GeV];Events / 5 fb^{-1}", 15, 0, 1500.,
   		  "plotSig:plotLog",-1,-1,
   		  "met_t1tttt_1200_800_ht_1112");
  draw_from_trees("num_jets_pt30","ht30>200&&met>300&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_csvm_jets>=3","(weightppb*5000)",
  		  ";n_{jets} (p_{T} > 30 GeV);Events / 5 fb^{-1}", 15, 0, 15,
  		  "plotSig:",-1,-1,
  		  "num_jets_pt30_t1tttt_1200_800_ht_1112");
  draw_from_trees("ht30","met>300&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_csvm_jets>=3&&num_jets_pt30>=10","(weightppb*5000)",
   		  ";H_{T} [GeV];Events / 5 fb^{-1}", 15, 0, 3000,
   		  "plotSig:",-1,-1,
   		  "ht30_t1tttt_1200_800_ht_1112");

  draw_from_trees("met","fatpT30_MJ>100&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_csvm_jets>=3&&num_jets_pt30>=10","(weightppb*5000)",
   		  ";E_{T}^{miss} [GeV];Events / 5 fb^{-1}", 15, 0, 1500.,
   		  "plotSig:plotLog",-1,-1,
   		  "met_t1tttt_1200_800_mj_1112");
  draw_from_trees("num_jets_pt30","fatpT30_MJ>100&&met>300&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_csvm_jets>=3","(weightppb*5000)",
  		  ";n_{jets} (p_{T} > 30 GeV);Events / 5 fb^{-1}", 15, 0, 15,
  		  "plotSig:",-1,-1,
  		  "num_jets_pt30_t1tttt_1200_800_mj_1112");
  draw_from_trees("fatpT30_MJ","met>300&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_csvm_jets>=3&&num_jets_pt30>=10","(weightppb*5000)",
   		  ";M_{J} [GeV];Events / 5 fb^{-1}", 15, 0, 1500,
   		  "plotSig",-1,-1,
   		  "fatpT30_MJ_t1tttt_1200_800_mj_1112");

  // TCut t1tttt_1500_100_ht("ht30>500&&met>600&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_csvm_jets>=2&&num_jets_pt30>=10");
  // TCut t1tttt_1500_100_mj("fatpT30_MJ>250&&met>600&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_csvm_jets>=2&&num_jets_pt30>=10");

  draw_from_trees("met","ht30>500&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_csvm_jets>=2&&num_jets_pt30>=10","(weightppb*5000)",
   		  ";E_{T}^{miss} [GeV];Events / 5 fb^{-1}", 15, 0, 1500.,
   		  "plotSig:plotLog",-1,-1,
   		  "met_t1tttt_1500_100_ht_1112");
  draw_from_trees("num_jets_pt30","ht30>500&&met>600&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_csvm_jets>=2","(weightppb*5000)",
  		  ";n_{jets} (p_{T} > 30 GeV);Events / 5 fb^{-1}", 15, 0, 15,
  		  "plotSig:",-1,-1,
  		  "num_jets_pt30_t1tttt_1500_100_ht_1112");
  draw_from_trees("ht30","met>600&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_csvm_jets>=2&&num_jets_pt30>=10","(weightppb*5000)",
   		  ";H_{T} [GeV];Events / 5 fb^{-1}", 15, 0, 3000,
   		  "plotSig:",-1,-1,
   		  "ht30_t1tttt_1500_100_ht_1112");

  draw_from_trees("met","fatpT30_MJ>250&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_csvm_jets>=2&&num_jets_pt30>=10","(weightppb*5000)",
   		  ";E_{T}^{miss} [GeV];Events / 5 fb^{-1}", 15, 0, 1500.,
   		  "plotSig:plotLog",-1,-1,
   		  "met_t1tttt_1500_100_mj_1112");
  draw_from_trees("num_jets_pt30","fatpT30_MJ>250&&met>600&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_csvm_jets>=2","(weightppb*5000)",
  		  ";n_{jets} (p_{T} > 30 GeV);Events / 5 fb^{-1}", 15, 0, 15,
  		  "plotSig:",-1,-1,
  		  "num_jets_pt30_t1tttt_1500_100_mj_1112");
  draw_from_trees("fatpT30_MJ","met>600&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_csvm_jets>=2&&num_jets_pt30>=10","(weightppb*5000)",
   		  ";M_{J} [GeV];Events / 5 fb^{-1}", 15, 0, 1500,
   		  "plotSig",-1,-1,
   		  "fatpT30_MJ_t1tttt_1500_100_mj_1112");
 
  return 0;
  
}

