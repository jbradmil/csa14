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

TString plotdir = "plots/125/";

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
  TH1D * ht1qqqq_1400_100 = new TH1D("t1qqqq_1400_100" , title, nbinsx, xlow, xup);
  TH1D * ht1qqqq_1000_800 = new TH1D("t1qqqq_1000_800" , title, nbinsx, xlow, xup);

  // Format cuts
  TCut cut(other_cuts);

  // TCut ttbar_weight("(weightppb*4000)/top_pt_weight_official");

  TCut ttbar_weight("(3.17760399999999981e-05*4000)");

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
  if (httbar->Integral()>hqcd->Integral()) {
    hs->Add(hqcd);
    hs->Add(httbar);
  } else {
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
  TLegend * leg1 = new TLegend(0.48, 0.6, 0.72, 0.92);
  set_style(leg1,0.025);
  if (plotData) leg1->AddEntry(hsingle_top, "Data", "pel");
  if (plotSig) {
    leg1->AddEntry(ht1bbbb_1500_100, "#splitline{T1bbbb}{(1500,100) GeV}", "l");
    leg1->AddEntry(ht1bbbb_1000_900, "#splitline{T1bbbb}{(1000,900) GeV}", "l");
    leg1->AddEntry(ht1tttt_1500_100, "#splitline{T1tttt}{(1500,100) GeV}", "l");
    leg1->AddEntry(ht1tttt_1200_800, "#splitline{T1tttt}{(1200,800) GeV}", "l");
    leg1->AddEntry(ht1qqqq_1400_100, "#splitline{T1qqqq}{(1400,100) GeV}", "l");
    leg1->AddEntry(ht1qqqq_1000_800, "#splitline{T1qqqq}{(1000,800) GeV}", "l");
  }
 

  TLegend * leg2 = new TLegend(0.72, 0.6, 0.94, 0.92);
  set_style(leg2,0.025);
  if (httbar->Integral()>hqcd->Integral()) {
    leg2->AddEntry(httbar, "t#bar{t}", "f");
    leg2->AddEntry(hqcd, "QCD", "f");
  } else {
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
  if (ht1qqqq_1400_100->GetMaximum()>ymax) ymax=ht1qqqq_1400_100->GetMaximum();


  if(plotLog) {
    hs->SetMaximum(200*ymax);
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
    ht1bbbb_1500_100->Draw("hist same");
    ht1bbbb_1000_900->Draw("hist same");
    ht1tttt_1500_100->Draw("hist same");
    ht1tttt_1200_800->Draw("hist same");
    ht1qqqq_1400_100->Draw("hist same");
    ht1qqqq_1000_800->Draw("hist same");
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
  latex->DrawLatex(0.19, 0.84, "#sqrt{s} = 13 TeV, L = 4 fb^{-1}");
  
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
  delete ht1qqqq_1400_100;
  delete ht1qqqq_1000_800;

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

  
  TFileCollection* ttbar_fc = new TFileCollection("ttbar_fc","","../../reduced_trees/tfcs/13TeV/Phys14/ttbar.txt");
  TFileCollection* qcd_fc = new TFileCollection("qcd_fc","","../../reduced_trees/tfcs/13TeV/Phys14/qcd.txt");
  TFileCollection* znn_fc = new TFileCollection("znn_fc","","../../reduced_trees/tfcs/13TeV/Phys14/znn.txt");
  TFileCollection* wjets_fc = new TFileCollection("wjets_fc","","../../reduced_trees/tfcs/13TeV/Phys14/wjets.txt");
  TFileCollection* single_top_fc = new TFileCollection("single_top_fc","","../../reduced_trees/tfcs/13TeV/Phys14/single_top.txt");
  TFileCollection* other_fc = new TFileCollection("other_fc","","../../reduced_trees/tfcs/13TeV/Phys14/other.txt");
  
   ttbar_ch->AddFileInfoList((TCollection*)ttbar_fc->GetList());
   qcd_ch->AddFileInfoList((TCollection*)qcd_fc->GetList());
   znn_ch->AddFileInfoList((TCollection*)znn_fc->GetList());
   wjets_ch->AddFileInfoList((TCollection*)wjets_fc->GetList());
   single_top_ch->AddFileInfoList((TCollection*)single_top_fc->GetList());
   other_ch->AddFileInfoList((TCollection*)other_fc->GetList());
   



   //ttbar_ch->Add("../../reduced_trees/13TeV/skimmed/TTJets*v77*.root");
   //ttbar_ch->Add("../../reduced_trees/13TeV/skimmed/TTbar*v77*.root");
   //ttbar_ch->Add("../../reduced_trees/13TeV/skimmed/TTW*v77*.root");
   //ttbar_ch->Add("../../reduced_trees/13TeV/skimmed/TTZ*v77*.root");
   //  qcd_ch->Add("../../reduced_trees/13TeV/skimmed/QCD_HT*v77*.root");
   //  znn_ch->Add("../../reduced_trees/13TeV/skimmed/ZJets*v77*.root");
   //  wjets_ch->Add("../../reduced_trees/13TeV/skimmed/WJets*v77*.root");
   //  single_top_ch->Add("../../reduced_trees/13TeV/skimmed/TTo*v77*.root");
   //  single_top_ch->Add("../../reduced_trees/13TeV/skimmed/TBarTo*v77*.root");
   //  single_top_ch->Add("../../reduced_trees/13TeV/skimmed/*tW*v77*.root");
   //  other_ch->Add("../../reduced_trees/13TeV/skimmed/ZH*v77*.root");
   //  other_ch->Add("../../reduced_trees/13TeV/skimmed/WH*v77*.root");



  t1tttt_1500_100_ch->Add("../../reduced_trees/13TeV/SMS-T1tttt_2J_mGl-1500_mLSP-100_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2299_v77/*.root");
  t1tttt_1200_800_ch->Add("../../reduced_trees/13TeV/SMS-T1tttt_2J_mGl-1200_mLSP-800_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2301_v77/*.root");
  t1bbbb_1500_100_ch->Add("../../reduced_trees/13TeV/SMS-T1bbbb_2J_mGl-1500_mLSP-100_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2300_v77/*.root");
  t1bbbb_1000_900_ch->Add("../../reduced_trees/13TeV/SMS-T1bbbb_2J_mGl-1000_mLSP-900_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2288_v77/*.root");
  t1qqqq_1400_100_ch->Add("../../reduced_trees/13TeV/SMS-T1qqqq_2J_mGl-1400_mLSP-100_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2302_v77/*.root");
  t1qqqq_1000_800_ch->Add("../../reduced_trees/13TeV/SMS-T1qqqq_2J_mGl-1000_mLSP-800_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2303_v77/*.root");

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
  TCut baseline("mht30>200&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>=4&&ht30>500&&num_iso_tracks_pt15_mT==0");
  TCut baseline_pre_tk("mht30>200&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>=4&&ht30>500");

  TCut nj1("num_jets_pt30>=4&&num_jets_pt30<=6"), nj2("num_jets_pt30>=7&&num_jets_pt30<=8"), nj3("num_jets_pt30>=9");
  TCut nb0("num_csvm_jets30==0"), nb1("num_csvm_jets30==1"), nb2("num_csvm_jets30==2"), nb3("num_csvm_jets30>=3");
  TCut bgeq0("num_csvm_jets30>=0"), bgeq1("num_csvm_jets30>=1"), bgeq2("num_csvm_jets30>=2"), bgeq3("num_csvm_jets30>=3");
  TCut mht1("mht30>200&&mht30<500"), mht2("mht30>500&&mht30<750"), mht3("mht30>750");
  TCut ht1("ht30>500&&ht30<800"), ht2("ht30>800&&ht30<1200"), ht3("ht30>1200");


  /*draw_from_trees(TString var, TCut other_cuts,
    TString weights, TString title, int nbinsx, 
    double xlow, double xup,
    TString options="plotSig:plotLog:plotData",
    double cut_low=-1, double cut_high=-1,
    TString plot_title="default")
  */


  // Baseline N-1
   draw_from_trees("ht30","mht30>200&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>=4&&min_delta_phi_met_N>4&&num_iso_tracks_pt15_mT==0","(weightppb*4000)",
    		  ";H_{T} [GeV];Events / 5 fb^{-1}", 20, 0, 4000.,
    		  "plotSig:plotLog",-1,-1,
    		  "ht30_baseline_nm1_125");
   draw_from_trees("mht30","ht30>500&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>=4&&min_delta_phi_met_N>4&&num_iso_tracks_pt15_mT==0","(weightppb*4000)",
    		  ";H_{T}^{miss} [GeV];Events / 5 fb^{-1}", 30, 0, 1500.,
    		  "plotSig:plotLog",-1,-1,
    		  "mht30_baseline_nm1_125");
   draw_from_trees("min_delta_phi_met_N","mht30>200&&ht30>500&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_jets_pt30>=4&&num_iso_tracks_pt15_mT==0","(weightppb*4000)",
     		  ";#Delta#Phi^{min}_{N};Events / 20 fb^{-1}", 20, 0., 40,
    		  "plotSig:plotLog",-1,-1,
     		  "min_delta_phi_met_N_baseline_nm1_125");
   draw_from_trees("num_reco_veto_electrons+num_reco_veto_muons","mht30>200&&ht30>500&&num_jets_pt30>=4&&min_delta_phi_met_N>4&&num_iso_tracks_pt15_mT==0","(weightppb*4000)",
   		  ";n_{e+#mu} (p_{T} > 10 GeV);Events / 5 fb^{-1}", 6, 0, 6,
   		  "plotSig:plotLog",-1,-1,
   		  "num_reco_veto_lepton_baseline_nm1_125");
   draw_from_trees("num_iso_tracks_pt15_mT","mht30>200&&ht30>500&&num_jets_pt30>=4&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0","(weightppb*4000)",
   		  ";n_{IsoTk} (p_{T} > 15 GeV);Events / 5 fb^{-1}", 6, 0, 6,
   		  "plotSig:plotLog",-1,-1,
   		  "num_iso_tracks_pt15_mT0_baseline_nm1_125");
   draw_from_trees("num_jets_pt30","mht30>200&&ht30>500&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_iso_tracks_pt15_mT==0","(weightppb*4000)",
   		  ";n_{jets} (p_{T} > 30 GeV);Events / 5 fb^{-1}", 15, 0, 15,
   		  "plotSig:plotLog",-1,-1,
   		  "num_jets_pt30_baseline_nm1_125");
   draw_from_trees("num_csvm_jets30","mht30>200&&ht30>500&&num_jets_pt30>=4&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_iso_tracks_pt15_mT==0","(weightppb*4000)",
   		  ";n_{CSVM} (p_{T} > 30 GeV);Events / 5 fb^{-1}", 6, 0, 6,
   		  "plotSig:plotLog",-1,-1,
   		  "num_csvm_jets30_baseline_nm1_125");

 
    // Tracks and taus
  draw_from_trees("iso_track_mT",baseline_pre_tk+"iso_track_pt>0","(weightppb*4000)",
		  ";m_{T} (IsoTk, E_{T}^{miss}) [GeV];Events / 5 fb^{-1}", 10, 0, 200,
		  ":plotSig",100,-1,
		  "iso_track_mT_120");
  draw_from_trees("iso_track_mT",baseline_pre_tk+"iso_track_pt>0","(weightppb*4000)",
		  ";m_{T} (IsoTk, E_{T}^{miss}) [GeV];Events / 5 fb^{-1}", 10, 0, 200,
		  "plotLog:plotSig",100,-1,
		  "iso_track_mT_log_120");
  draw_from_trees("num_iso_tracks_pt15",baseline_pre_tk,"(weightppb*4000)",
		  ";n_{IsoTk} (p_{T} > 15 GeV);Events / 5 fb^{-1}", 6, 0, 6,
		  "plotLog:plotSig",-1,-1,
		  "num_iso_tracks_pt15_120");
  draw_from_trees("num_iso_tracks_pt15_mT",baseline_pre_tk,"(weightppb*4000)",
		  ";n_{IsoTk} (p_{T} > 15 GeV);Events / 5 fb^{-1}", 6, 0, 6,
		  "plotLog:plotSig",-1,-1,
		  "num_iso_tracks_pt15_mT_120");

  draw_from_trees("e_track_pt",baseline_pre_tk+"num_mu_iso_tracks_mT==0&&num_had_iso_tracks_mT==0&&e_track_pt>0&&e_track_mT<100&&e_track_relIso<0.2","(weightppb*4000)",
		  ";p_{Tl}^{tk};Events / 5 fb^{-1}", 20, 0, 100,
		  ":plotSig",-1,-1,
		  "e_track_pt_120_nm1");
  draw_from_trees("e_track_pt",baseline_pre_tk+"num_mu_iso_tracks_mT==0&&num_had_iso_tracks_mT==0&&e_track_pt>0&&e_track_mT<100&&e_track_relIso<0.2","(weightppb*4000)",
		  ";p_{T}^{tk};Events / 5 fb^{-1}", 20, 0, 100,
		  "plotLog:plotSig",-1,-1,
		  "e_track_pt_120_nm1_log");
  draw_from_trees("e_track_relIso",baseline_pre_tk+"num_mu_iso_tracks_mT==0&&num_had_iso_tracks_mT==0&&e_track_pt>5&&e_track_mT<100","(weightppb*4000)",
		  ";I_{rel}^{tk};Events / 5 fb^{-1}", 20, 0, 1,
		  ":plotSig",-1,-1,
		  "e_track_relIso_120_nm1");
  draw_from_trees("e_track_relIso",baseline_pre_tk+"num_mu_iso_tracks_mT==0&&num_had_iso_tracks_mT==0&&e_track_pt>5&&e_track_mT<100","(weightppb*4000)",
		  ";I_{rel}^{tk};Events / 5 fb^{-1}", 20, 0, 1,
		  "plotLog:plotSig",-1,-1,
		  "e_track_relIso_120_nm1_log");
  draw_from_trees("e_track_mT",baseline_pre_tk+"num_mu_iso_tracks_mT==0&&num_had_iso_tracks_mT==0&&e_track_pt>5&&e_track_relIso<0.2","(weightppb*4000)",
		  ";m_{T} (IsoTk, E_{T}^{miss}) [GeV];Events / 5 fb^{-1}", 10, 0, 200,
		  ":plotSig",100,-1,
		  "e_track_mT_120_nm1");
  draw_from_trees("e_track_mT",baseline_pre_tk+"num_mu_iso_tracks_mT==0&&num_had_iso_tracks_mT==0&&e_track_pt>5&&e_track_relIso<0.2","(weightppb*4000)",
		  ";m_{T} (IsoTk, E_{T}^{miss}) [GeV];Events / 5 fb^{-1}", 10, 0, 200,
		  "plotLog:plotSig",100,-1,
		  "e_track_mT_log_120_nm1");
	
      draw_from_trees("e_track_mT",baseline_pre_tk+"e_track_pt>0","(weightppb*4000)",
		  ";m_{T} (IsoTk, E_{T}^{miss}) [GeV];Events / 5 fb^{-1}", 10, 0, 200,
		  ":plotSig",100,-1,
		  "e_track_mT_120");
    draw_from_trees("e_track_mT",baseline_pre_tk+"e_track_pt>0","(weightppb*4000)",
		  ";m_{T} (IsoTk, E_{T}^{miss}) [GeV];Events / 5 fb^{-1}", 10, 0, 200,
		  "plotLog:plotSig",100,-1,
		  "e_track_mT_log_120");
    draw_from_trees("num_e_iso_tracks",baseline_pre_tk,"(weightppb*4000)",
		  ";n_{IsoTk};Events / 5 fb^{-1}", 6, 0, 6,
		  "plotLog:plotSig",-1,-1,
		  "num_e_iso_tracks_pt15_120");
        draw_from_trees("num_e_iso_tracks_mT",baseline_pre_tk,"(weightppb*4000)",
		  ";n_{IsoTk};Events / 5 fb^{-1}", 6, 0, 6,
		  "plotLog:plotSig",-1,-1,
		  "num_e_iso_tracks_mT_120");
	draw_from_trees("e_track_pt",baseline_pre_tk+"e_track_pt>0&&e_track_mT<100","(weightppb*4000)",
			";p_{T}^{tk} [GeV];Events / 5 fb^{-1}", 20, 0, 100,
			":plotSig",-1,-1,
			"e_track_pt_120");
	draw_from_trees("e_track_pt",baseline_pre_tk+"e_track_pt>0&&e_track_mT<100","(weightppb*4000)",
			";p_{T}^{tk} [GeV];Events / 5 fb^{-1}", 20, 0, 100,
			"plotLog:plotSig",-1,-1,
			"e_track_pt_log_120");
		draw_from_trees("e_track_pt",baseline_pre_tk+"e_track_pt>0&&e_track_mT<100&&e_track_relIso<0.2","(weightppb*4000)",
			";p_{T}^{tk} [GeV];Events / 5 fb^{-1}", 20, 0, 100,
			":plotSig",-1,-1,
			"e_track_pt_relIso0p2_120");
	draw_from_trees("e_track_pt",baseline_pre_tk+"e_track_pt>0&&e_track_mT<100&&e_track_relIso<0.2","(weightppb*4000)",
			";p_{T}^{tk} [GeV];Events / 5 fb^{-1}", 20, 0, 100,
			"plotLog:plotSig",-1,-1,
			"e_track_pt_relIso0p2_log_120");
	draw_from_trees("e_track_relIso",baseline_pre_tk+"e_track_pt>0&&e_track_mT<100","(weightppb*4000)",
			";I_{rel}^{tk};Events / 5 fb^{-1}", 20, 0, 1,
			":plotSig",-1,-1,
			"e_track_relIso_120");
	draw_from_trees("e_track_relIso",baseline_pre_tk+"e_track_pt>0&&e_track_mT<100","(weightppb*4000)",
			";I_{rel}^{tk};Events / 5 fb^{-1}", 20, 0, 1,
			"plotLog:plotSig",-1,-1,
			"e_track_relIso_log_120");

	draw_from_trees("mu_track_pt",baseline_pre_tk+"num_e_iso_tracks_mT==0&&num_had_iso_tracks_mT==0&&mu_track_pt>0&&mu_track_relIso<0.2&&mu_track_mT<100","(weightppb*4000)",
			";p_{T}^{tk} {GeV];Events / 5 fb^{-1}", 20, 0, 100,
			":plotSig",-1,-1,
			"mu_track_pt_120_nm1");
	draw_from_trees("mu_track_pt",baseline_pre_tk+"num_e_iso_tracks_mT==0&&num_had_iso_tracks_mT==0&&mu_track_pt>0&&mu_track_relIso<0.2&&mu_track_mT<100","(weightppb*4000)",
			";p_{T}^{tk} [GeV];Events / 5 fb^{-1}", 20, 0, 100,
			"plotLog:plotSig",-1,-1,
			"mu_track_pt_120_nm1_log");
	draw_from_trees("mu_track_relIso",baseline_pre_tk+"num_e_iso_tracks_mT==0&&num_had_iso_tracks_mT==0&&mu_track_pt>5&&mu_track_mT<100","(weightppb*4000)",
			";I_{rel}^{tk};Events / 5 fb^{-1}", 20, 0, 1,
			":plotSig",-1,-1,
			"mu_track_relIso_120_nm1");
	draw_from_trees("mu_track_relIso",baseline_pre_tk+"num_e_iso_tracks_mT==0&&num_had_iso_tracks_mT==0&&mu_track_pt>5&&mu_track_mT<100","(weightppb*4000)",
			";I_{rel}^{tk};Events / 5 fb^{-1}", 20, 0, 1,
			"plotLog:plotSig",-1,-1,
			"mu_track_relIso_120_nm1_log");
	draw_from_trees("mu_track_mT",baseline_pre_tk+"num_e_iso_tracks_mT==0&&num_had_iso_tracks_mT==0&&mu_track_pt>5&&mu_track_relIso<0.2","(weightppb*4000)",
		  ";m_{T} (IsoTk, E_{T}^{miss}) [GeV];Events / 5 fb^{-1}", 10, 0, 200,
		  ":plotSig",100,-1,
		  "mu_track_mT_120_nm1");
    draw_from_trees("mu_track_mT",baseline_pre_tk+"num_e_iso_tracks_mT==0&&num_had_iso_tracks_mT==0&&mu_track_pt>5&&mu_track_relIso<0.2","(weightppb*4000)",
		  ";m_{T} (IsoTk, E_{T}^{miss}) [GeV];Events / 5 fb^{-1}", 10, 0, 200,
		  "plotLog:plotSig",100,-1,
		  "mu_track_mT_log_120_nm1");
	
      draw_from_trees("mu_track_mT",baseline_pre_tk+"mu_track_pt>0","(weightppb*4000)",
		  ";m_{T} (IsoTk, E_{T}^{miss}) [GeV];Events / 5 fb^{-1}", 10, 0, 200,
		  ":plotSig",100,-1,
		  "mu_track_mT_120");
    draw_from_trees("mu_track_mT",baseline_pre_tk+"mu_track_pt>0","(weightppb*4000)",
		  ";m_{T} (IsoTk, E_{T}^{miss}) [GeV];Events / 5 fb^{-1}", 10, 0, 200,
		  "plotLog:plotSig",100,-1,
		  "mu_track_mT_log_120");
    draw_from_trees("num_mu_iso_tracks",baseline_pre_tk,"(weightppb*4000)",
		  ";n_{IsoTk};Events / 5 fb^{-1}", 6, 0, 6,
		  "plotLog:plotSig",-1,-1,
		  "num_mu_iso_tracks_pt15_120");
        draw_from_trees("num_mu_iso_tracks_mT",baseline_pre_tk,"(weightppb*4000)",
		  ";n_{IsoTk};Events / 5 fb^{-1}", 6, 0, 6,
		  "plotLog:plotSig",-1,-1,
		  "num_mu_iso_tracks_mT_120");
		draw_from_trees("mu_track_pt",baseline_pre_tk+"mu_track_pt>0&&mu_track_mT<100","(weightppb*4000)",
			";p_{T}^{tk} [GeV];Events / 5 fb^{-1}", 20, 0, 100,
			":plotSig",-1,-1,
			"mu_track_pt_120");
	draw_from_trees("mu_track_pt",baseline_pre_tk+"mu_track_pt>0&&mu_track_mT<100","(weightppb*4000)",
			";p_{T}^{tk} [GeV];Events / 5 fb^{-1}", 20, 0, 100,
			"plotLog:plotSig",-1,-1,
			"mu_track_pt_log_120");
	draw_from_trees("mu_track_pt",baseline_pre_tk+"mu_track_pt>0&&mu_track_mT<100&&mu_track_relIso<0.2","(weightppb*4000)",
			";p_{T}^{tk} [GeV];Events / 5 fb^{-1}", 20, 0, 100,
			":plotSig",-1,-1,
			"mu_track_pt_relIso0p2_120");
	draw_from_trees("mu_track_pt",baseline_pre_tk+"mu_track_pt>0&&mu_track_mT<100&&mu_track_relIso<0.2","(weightppb*4000)",
			";p_{T}^{tk} [GeV];Events / 5 fb^{-1}", 20, 0, 100,
			"plotLog:plotSig",-1,-1,
			"mu_track_pt_relIso0p2_log_120");


	draw_from_trees("mu_track_relIso",baseline_pre_tk+"mu_track_pt>0&&mu_track_mT<100","(weightppb*4000)",
			";I_{rel}^{tk};Events / 5 fb^{-1}", 20, 0, 1,
			":plotSig",-1,-1,
			"mu_track_relIso_120");
	draw_from_trees("mu_track_relIso",baseline_pre_tk+"mu_track_pt>0&&mu_track_mT<100","(weightppb*4000)",
			";I_{rel}^{tk};Events / 5 fb^{-1}", 20, 0, 1,
			"plotLog:plotSig",-1,-1,
			"mu_track_relIso_log_120");


			draw_from_trees("had_track_pt",baseline_pre_tk+"num_e_iso_tracks_mT==0&&num_mu_iso_tracks_mT==0&&had_track_pt>0&&had_track_relIso<0.1&&had_track_mT<100","(weightppb*4000)",
			";p_{T}^{tk} [GeV];Events / 5 fb^{-1}", 20, 0, 100,
			":plotSig",-1,-1,
			"had_track_pt_120_nm1");
		draw_from_trees("had_track_pt",baseline_pre_tk+"num_e_iso_tracks_mT==0&&num_mu_iso_tracks_mT==0&&had_track_pt>0&&had_track_relIso<0.1&&had_track_mT<100","(weightppb*4000)",
			";p_{T}^{tk} [GeV];Events / 5 fb^{-1}", 20, 0, 100,
			"plotLog:plotSig",-1,-1,
			"had_track_pt_120_nm1_log");
	draw_from_trees("had_track_relIso",baseline_pre_tk+"num_e_iso_tracks_mT==0&&num_mu_iso_tracks_mT==0&&had_track_pt>10&&had_track_mT<100","(weightppb*4000)",
			";I_{rel}^{tk};Events / 5 fb^{-1}", 20, 0, 1,
			":plotSig",-1,-1,
			"had_track_relIso_120_nm1");
	draw_from_trees("had_track_relIso",baseline_pre_tk+"num_e_iso_tracks_mT==0&&num_mu_iso_tracks_mT==0&&had_track_pt>10&&had_track_mT<100","(weightppb*4000)",
			";I_{rel}^{tk};Events / 5 fb^{-1}", 20, 0, 1,
			"plotLog:plotSig",-1,-1,
			"had_track_relIso_120_nm1_log");
	draw_from_trees("had_track_mT",baseline_pre_tk+"num_e_iso_tracks_mT==0&&num_mu_iso_tracks_mT==0&&had_track_pt>10&&had_track_relIso<0.1","(weightppb*4000)",
		  ";m_{T} (IsoTk, E_{T}^{miss}) [GeV];Events / 5 fb^{-1}", 10, 0, 200,
		  ":plotSig",100,-1,
			"had_track_mT_120_nm1");
    draw_from_trees("had_track_mT",baseline_pre_tk+"num_e_iso_tracks_mT==0&&num_mu_iso_tracks_mT==0&&had_track_pt>10&&had_track_relIso<0.1","(weightppb*4000)",
		  ";m_{T} (IsoTk, E_{T}^{miss}) [GeV];Events / 5 fb^{-1}", 10, 0, 200,
		  "plotLog:plotSig",100,-1,
		  "had_track_mT_log_120_nm1");
		
      draw_from_trees("had_track_mT",baseline_pre_tk+"had_track_pt>0","(weightppb*4000)",
		  ";m_{T} (IsoTk, E_{T}^{miss}) [GeV];Events / 5 fb^{-1}", 10, 0, 200,
		  ":plotSig",100,-1,
		  "had_track_mT_120");
    draw_from_trees("had_track_mT",baseline_pre_tk+"had_track_pt>0","(weightppb*4000)",
		  ";m_{T} (IsoTk, E_{T}^{miss}) [GeV];Events / 5 fb^{-1}", 10, 0, 200,
		  "plotLog:plotSig",100,-1,
		  "had_track_mT_log_120");
    draw_from_trees("num_had_iso_tracks",baseline_pre_tk,"(weightppb*4000)",
		  ";n_{IsoTk};Events / 5 fb^{-1}", 6, 0, 6,
		  "plotLog:plotSig",-1,-1,
		  "num_had_iso_tracks_pt15_120");
        draw_from_trees("num_had_iso_tracks_mT",baseline_pre_tk,"(weightppb*4000)",
		  ";n_{IsoTk};Events / 5 fb^{-1}", 6, 0, 6,
		  "plotLog:plotSig",-1,-1,
		  "num_had_iso_tracks_mT_120");
	draw_from_trees("had_track_pt",baseline_pre_tk+"had_track_pt>0&&had_track_mT<100","(weightppb*4000)",
			";p_{T}^{tk} [GeV];Events / 5 fb^{-1}", 20, 0, 100,
			":plotSig",-1,-1,
			"had_track_pt_120");
	draw_from_trees("had_track_pt",baseline_pre_tk+"had_track_pt>0&&had_track_mT<100","(weightppb*4000)",
			";p_{T}^{tk} [GeV];Events / 5 fb^{-1}", 20, 0, 100,
			"plotLog:plotSig",-1,-1,
			"had_track_pt_log_120");
	draw_from_trees("had_track_pt",baseline_pre_tk+"had_track_pt>0&&had_track_mT<100&&had_track_relIso<0.1","(weightppb*4000)",
			";p_{T}^{tk} [GeV];Events / 5 fb^{-1}", 20, 0, 100,
			":plotSig",-1,-1,
			"had_track_pt_relIso0p1_120");
	draw_from_trees("had_track_pt",baseline_pre_tk+"had_track_pt>0&&had_track_mT<100&&had_track_relIso<0.1","(weightppb*4000)",
			";p_{T}^{tk} [GeV];Events / 5 fb^{-1}", 20, 0, 100,
			"plotLog:plotSig",-1,-1,
			"had_track_pt_relIso0p1_log_120");
		draw_from_trees("had_track_relIso",baseline_pre_tk+"had_track_pt>0&&had_track_mT<100","(weightppb*4000)",
			";I_{rel}^{tk};Events / 5 fb^{-1}", 20, 0, 1,
			":plotSig",-1,-1,
			"had_track_relIso_120");
	draw_from_trees("had_track_relIso",baseline_pre_tk+"had_track_pt>0&&had_track_mT<100","(weightppb*4000)",
			";I_{rel}^{tk};Events / 5 fb^{-1}", 20, 0, 1,
			"plotLog:plotSig",-1,-1,
			"had_track_relIso_log_120");
	draw_from_trees("had_track_relIso",baseline_pre_tk+"had_track_pt>10&&had_track_mT<100","(weightppb*4000)",
			";I_{rel}^{tk};Events / 5 fb^{-1}", 20, 0, 1,
			":plotSig",-1,-1,
			"had_track_relIso_pt10_120");
	draw_from_trees("had_track_relIso",baseline_pre_tk+"had_track_pt>10&&had_track_mT<100","(weightppb*4000)",
			";I_{rel}^{tk};Events / 5 fb^{-1}", 20, 0, 1,
			"plotLog:plotSig",-1,-1,
			"had_track_relIso_pt10_log_120");
  


  
  // Most sensitive bins
  // T1bbbb (1500, 100)
  draw_from_trees("ht30",baseline+bgeq2+"num_jets_pt30>=4&&mht30>500","(weightppb*4000)",
   		  ";H_{T} [GeV];Events / 5 fb^{-1}", 10, 500, 3500.,
   		  "plotSig:plotLog",1200,-1,
   		  "ht30_mht500_bgeq2_nj1_ht1200_nm1_t1bbbb_1500_100_120");
  draw_from_trees("mht30",baseline+bgeq2+"num_jets_pt30>=4&&ht30>1200","(weightppb*4000)",
   		  ";H_{T}^{miss} [GeV];Events / 5 fb^{-1}", 13, 200, 1500.,
   		  "plotSig:plotLog",500,-1,
   		  "mht30_bgeq2_nj1_ht1200_nm1_t1bbbb_1500_100_120");
  draw_from_trees("num_jets_pt30",baseline+bgeq2+"mht30>500&&ht30>1200","(weightppb*4000)",
		  ";n_{jets} (p_{T} > 30 GeV);Events / 5 fb^{-1}", 11, 4, 15,
		  "plotSig:plotLog",4,-1,
		  "num_jets_pt30_mht500_bgeq2_ht1200_nm1_t1bbbb_1500_100_120");
  draw_from_trees("num_csvm_jets30",baseline+"num_jets_pt30>=4&&mht30>500&&ht30>1200","(weightppb*4000)",
		  ";n_{CSVM} (p_{T} > 30 GeV);Events / 5 fb^{-1}", 6, 0, 6,
		  "plotLog:plotSig",2,-1,
		  "num_csvm_jets30_mht500_nj1_ht1200_nm1_t1bbbb_1500_100_120");

  // T1bbbb (1000, 900)
  draw_from_trees("ht30",baseline+bgeq2+"num_jets_pt30>=4&&mht30>500","(weightppb*4000)",
   		  ";H_{T} [GeV];Events / 5 fb^{-1}", 10, 500, 3500.,
   		  "plotSig:plotLog",500,-1,
   		  "ht30_mht500_bgeq2_nj1_ht500_nm1_t1bbbb_1000_900_120");
  draw_from_trees("mht30",baseline+bgeq2+"num_jets_pt30>=4&&ht30>500","(weightppb*4000)",
   		  ";H_{T}^{miss} [GeV];Events / 5 fb^{-1}", 13, 200, 1500.,
   		  "plotSig:plotLog",500,-1,
   		  "mht30_bgeq2_nj1_ht500_nm1_t1bbbb_1000_900_120");
  draw_from_trees("num_jets_pt30",baseline+bgeq2+"mht30>500&&ht30>500","(weightppb*4000)",
		  ";n_{jets} (p_{T} > 30 GeV);Events / 5 fb^{-1}", 11, 4, 15,
		  "plotSig:plotLog",4,-1,
		  "num_jets_pt30_mht500_bgeq2_ht500_nm1_t1bbbb_1000_900_120");
  draw_from_trees("num_csvm_jets30",baseline+"num_jets_pt30>=4&&mht30>500&&ht30>500","(weightppb*4000)",
		  ";n_{CSVM} (p_{T} > 30 GeV);Events / 5 fb^{-1}", 6, 0, 6,
		  "plotLog:plotSig",2,-1,
		  "num_csvm_jets30_mht500_nj1_ht500_nm1_t1bbbb_1000_900_120");
  
  // T1tttt (1500, 100)
  draw_from_trees("ht30",baseline+bgeq2+"num_jets_pt30>=9&&mht30>500","(weightppb*4000)",
   		  ";H_{T} [GeV];Events / 5 fb^{-1}", 10, 500, 3500.,
   		  "plotSig:",1000,-1,
   		  "ht30_mht500_bgeq2_nj3_nm1_t1tttt_1500_100_120");
  draw_from_trees("mht30",baseline+bgeq2+"ht30>1000&&num_jets_pt30>=9","(weightppb*4000)",
   		  ";H_{T}^{miss} [GeV];Events / 5 fb^{-1}", 13, 200, 1500.,
   		  "plotSig:plotLog",500,-1,
   		  "mht30_ht1000_bgeq2_nj3_nm1_t1tttt_1500_100_120");
  draw_from_trees("num_jets_pt30",baseline+bgeq2+"ht30>1000&&mht30>500","(weightppb*4000)",
		  ";n_{jets} (p_{T} > 30 GeV);Events / 5 fb^{-1}", 11, 4, 15,
		  "plotSig:plotLog",9,-1,
		  "num_jets_pt30_ht1000_mht500_bgeq2_nm1_t1tttt_1500_100_120");
  draw_from_trees("num_csvm_jets30",baseline+"ht30>1000&&num_jets_pt30>=9&&mht30>500","(weightppb*4000)",
		  ";n_{CSVM} (p_{T} > 30 GeV);Events / 5 fb^{-1}", 6, 0, 6,
		  "plotSig",2,-1,
		  "num_csvm_jets30_ht1000_mht500_nj3_nm1_t1tttt_1500_100_120");

  // T1tttt (1200, 800)
  draw_from_trees("ht30",baseline+bgeq2+"num_jets_pt30>=9","(weightppb*4000)",
   		  ";H_{T} [GeV];Events / 5 fb^{-1}", 10, 500, 3500.,
   		  "plotSig:plotLog",500,-1,
   		  "ht30_mht200_bgeq2_nj3_ht500_nm1_t1tttt_1200_800_120");
  draw_from_trees("mht30",baseline+bgeq2+"num_jets_pt30>=9","(weightppb*4000)",
   		  ";H_{T}^{miss} [GeV];Events / 5 fb^{-1}", 13, 200, 1500.,
   		  "plotSig:plotLog",200,-1,
   		  "mht30_bgeq2_nj3_ht500_nm1_t1tttt_1200_800_120");
  draw_from_trees("num_jets_pt30",baseline+bgeq2,"(weightppb*4000)",
		  ";n_{jets} (p_{T} > 30 GeV);Events / 5 fb^{-1}", 11, 4, 15,
		  "plotSig:plotLog",9,-1,
		  "num_jets_pt30_mht200_bgeq2_ht500_nm1_t1tttt_1200_800_120");
  draw_from_trees("num_csvm_jets30",baseline+"num_jets_pt30>=9","(weightppb*4000)",
		  ";n_{CSVM} (p_{T} > 30 GeV);Events / 5 fb^{-1}", 6, 0, 6,
		  "plotLog:plotSig",2,-1,
		  "num_csvm_jets30_mht200_nj3_ht500_nm1_t1tttt_1200_800_120");
  
  // T1qqqq (1400, 100)
  draw_from_trees("ht30",baseline+"mht30>400","(weightppb*4000)",
   		  ";H_{T} [GeV];Events / 5 fb^{-1}", 10, 500, 3500.,
   		  "plotSig:plotLog",1400,-1,
   		  "ht30_mht400_nj1_nm1_t1qqqq_1400_100_120");
  draw_from_trees("mht30",baseline+"ht30>1400","(weightppb*4000)",
   		  ";H_{T}^{miss} [GeV];Events / 5 fb^{-1}", 13, 200, 1500.,
   		  "plotSig:plotLog",400,-1,
   		  "mht30_ht1400_nj1_nm1_t1qqqq_1400_100_120");
  draw_from_trees("num_jets_pt30",baseline+"ht30>1400&&mht30>400","(weightppb*4000)",
		  ";n_{jets} (p_{T} > 30 GeV);Events / 5 fb^{-1}", 11, 4, 15,
		  "plotSig:plotLog",4,-1,
		  "num_jets_pt30_mht400_ht1400_nm1_t1qqqq_1400_100_120");
  draw_from_trees("num_csvm_jets30",baseline+"ht30>1400&&mht30>400","(weightppb*4000)",
		  ";n_{CSVM} (p_{T} > 30 GeV);Events / 5 fb^{-1}", 6, 0, 6,
		  "plotLog:plotSig",-1,-1,
		  "num_csvm_jets30_mht400_ht1400_nj1_nm1_t1qqqq_1400_100_120");
  // T1qqqq (1000, 800)
  draw_from_trees("num_csvm_jets30",baseline+"num_jets_pt30>=6&&mht30>300","(weightppb*4000)",
		  ";n_{CSVM} (p_{T} > 30 GeV);Events / 5 fb^{-1}", 6, 0, 6,
		  "plotLog:plotSig",-1,-1,
		  "num_csvm_jets30_mht300_ht500_6j_nm1_t1qqqq_1000_800_120");
  draw_from_trees("ht30",baseline+"num_jets_pt30>=6&&mht30>300","(weightppb*4000)",
   		  ";H_{T} [GeV];Events / 5 fb^{-1}", 10, 500, 3500.,
   		  "plotSig:plotLog",-1,-1,
   		  "ht30_mht300_6j_nm1_t1qqqq_1000_800_120");
  draw_from_trees("mht30",baseline+"num_jets_pt30>=6","(weightppb*4000)",
   		  ";H_{T}^{miss} [GeV];Events / 5 fb^{-1}", 13, 200, 1500.,
   		  "plotSig:plotLog",300,-1,
   		  "mht30_ht500_6j_nm1_t1qqqq_1000_800_120");
  draw_from_trees("num_jets_pt30",baseline+"mht30>300","(weightppb*4000)",
		  ";n_{jets} (p_{T} > 30 GeV);Events / 5 fb^{-1}", 11, 4, 15,
		  "plotSig:plotLog",6,-1,
		  "num_jets_pt30_mht300_ht500_nm1_t1qqqq_1000_800_120");
 
 


  draw_from_trees("num_taus_POG_phys14_ID",baseline_pre_tk,"(weightppb*4000)",
		  ";n_{#tau} (p_{T} > 20 GeV);Events / 5 fb^{-1}", 6, 0, 6,
		  "plotLog:plotSig",-1,-1,
		  "num_taus_POG_phys14_ID_120");
  draw_from_trees("tau_mT",baseline_pre_tk+"tau_pt>0","(weightppb*4000)",
		  ";m_{T} (#tau, E_{T}^{miss}) [GeV];Events / 5 fb^{-1}", 10, 0, 200,
		  "plotLog:plotSig",-1,-1,
		  "tau_mT_120");



 
  return 0;
  
}

