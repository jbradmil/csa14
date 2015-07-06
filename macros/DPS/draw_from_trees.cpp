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
#include "TPaveText.h"
#include "../include/my_style.hpp"

TChain* ttbar_ch;
TChain* qcd_ch;
TChain* temp_ch;
TChain* znn_ch;
TChain* wjets_ch;
TChain* single_top_ch;
TChain* other_ch;

TChain* ttbar_data;
TChain* qcd_data;
TChain* temp_data;
TChain* znn_data;
TChain* wjets_data;
TChain* single_top_data;
TChain* other_data;


using namespace std;

TString plotdir = "plots/stacked/";

void draw_from_trees(TString var, TCut other_cuts,
		     TString weights, TString title, int nbinsx, 
		     double xlow, double xup,
		     TString options="plotSig:plotLog:plotData",
		     double cut_low=-1, double cut_high=-1,
		     TString plot_title="default")
{

  //  bool plotSig = options.Contains("plotSig") && (!options.Contains("!plotSig"));
  bool plotLog = options.Contains("plotLog") && (!options.Contains("!plotLog"));
  bool plotData = options.Contains("plotData") && (!options.Contains("!plotData"));
  bool entries = options.Contains("entries") && (!options.Contains("!entries"));

  // Book histograms
  TH1D * httbar = new TH1D("ttbar" , title, nbinsx, xlow, xup);
  TH1D * hqcd = new TH1D("qcd" , title, nbinsx, xlow, xup);
  TH1D * hznn = new TH1D("znn" , title, nbinsx, xlow, xup);
  TH1D * hwjets = new TH1D("wjets" , title, nbinsx, xlow, xup);
  TH1D * hother = new TH1D("other" , title, nbinsx, xlow, xup);
  TH1D * hmc_exp = new TH1D("mc_exp" , title, nbinsx, xlow, xup);
  TH1D * hsingle_top = new TH1D("single_top" , title, nbinsx, xlow, xup); 

  TH1D * hdata_obs = new TH1D("data_obs" , title, nbinsx, xlow, xup);

  // Format cuts
  TCut cut(other_cuts);


  //TCut temp_weight("(8.35943e-05*10000)");

  cout << "Filling histograms for " << var.Data() << endl;
  ttbar_ch->Project("ttbar",var,cut*weights);
  qcd_ch->Project("qcd",var,cut*weights);
  znn_ch->Project("znn",var,cut*weights);
  wjets_ch->Project("wjets",var,cut*weights);
  other_ch->Project("other",var,cut*weights);
  single_top_ch->Project("single_top",var,cut*weights);
  // temp_ch->Project("+qcd",var,cut*temp_weight);

  ttbar_data->Project("data_obs",var,cut*weights);
  qcd_data->Project("+data_obs",var,cut*weights);
  znn_data->Project("+data_obs",var,cut*weights);
  wjets_data->Project("+data_obs",var,cut*weights);
  other_data->Project("+data_obs",var,cut*weights);
  single_top_data->Project("+data_obs",var,cut*weights);


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
  if (entries) ytitle = Form("Entries / %.3f", binwidth);
  hmc_exp->GetXaxis()->SetTitle(httbar->GetXaxis()->GetTitle());
  hmc_exp->GetYaxis()->SetTitle(ytitle);
  cout << "... DONE: add all backgrounds to mc_exp." << endl;

  // make data look like data
     for (int bin(0); bin<nbinsx; bin++) {
       hdata_obs->SetBinContent(bin+1, round(hdata_obs->GetBinContent(bin+1)));
       hdata_obs->SetBinError(bin+1, sqrt(hdata_obs->GetBinContent(bin+1)));
     }



  cout << "... DONE: filled histograms." << endl;

 
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
  set_style(hdata_obs, "data_obs");

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
  ratiostaterr->GetXaxis()->SetTitleSize(0.12);
  ratiostaterr->GetXaxis()->SetTitleOffset(1.12);
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
  TLegend * leg1 = new TLegend(0.48, 0.6, 0.72, 0.92);
  set_style(leg1,0.035);
  if (plotData) leg1->AddEntry(hdata_obs, "Data", "pel");
  leg1->AddEntry(httbar, "t#bar{t}", "f");
  leg1->AddEntry(hqcd, "QCD", "f");
  leg1->AddEntry(hznn, "Z+jets", "f");
  TLegend * leg2 = new TLegend(0.72, 0.6, 0.94, 0.92);
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
    //   hmc_exp->SetMaximum(100000);
    hmc_exp->SetMaximum(200*ymax);
    hmc_exp->SetMinimum(0.1);
    if (var.Contains("num_")) hmc_exp->SetMaximum(1000*ymax);
    if (var.Contains("eta")) hmc_exp->SetMaximum(1000*ymax);
    if (var.Contains("dphi")) hmc_exp->SetMaximum(1000*ymax);
  }
  else {
    //  hmc_exp->SetMaximum(600);
    hmc_exp->SetMaximum(2.5*ymax);
    if (plot_title.Contains("baseline")) hmc_exp->SetMaximum(1.3*ymax);
    if (var.Contains("eta")) hmc_exp->SetMaximum(2*ymax);
  }
  // Vertical lines for cuts
  TLine* line_low = new TLine(cut_low,0,cut_low,1.5*ymax);
  TLine* line_high = new TLine(cut_high,0,cut_high,1.5*ymax);
  set_style(line_low);
  set_style(line_high);

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
  hmc_exp->Draw();
  hmc_exp->GetXaxis()->SetLabelSize(0.03);
  hmc_exp->GetXaxis()->SetTitleSize(0.025);
  hmc_exp->GetYaxis()->SetLabelSize(0.04);
  hs->Draw("hist,same");
  //  TString full_title(hmc_exp->GetTitle()+";"+httbar->GetXaxis()->GetTitle()+";"+ytitle);
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
  latex->SetTextSize(0.052);
  latex->DrawLatex(0.19, 0.89, "CMS Preliminary");
  latex->SetTextSize(0.04);
  latex->DrawLatex(0.19, 0.84, "#sqrt{s} = 13 TeV, L = 0.1 fb^{-1}");


  TPaveText * pave = new TPaveText(0.18, 0.86, 0.28, 0.96, "brNDC");
  pave->SetLineColor(0);
  pave->SetFillColor(0);
  pave->SetShadowColor(0);
  pave->SetBorderSize(1);
  cout << "Plot data..." << endl;
  if (plotData) hdata_obs->Draw("e1 same");
  hmc_exp->GetXaxis()->SetLabelSize(0);
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
  ttbar_data = new TChain("reduced_tree");
  qcd_data = new TChain("reduced_tree");
  znn_data = new TChain("reduced_tree");
  wjets_data = new TChain("reduced_tree");
  single_top_data = new TChain("reduced_tree");
  other_data = new TChain("reduced_tree");


  ttbar_ch->Add("/cms8r0/johnbr/2014/csa14/reduced_trees/13TeV/TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1_MINIAODSIM_UCSB2406_v78/*");
  
  qcd_ch->Add("/cms8r0/johnbr/2014/csa14/reduced_trees/13TeV/QCD_Pt-300to470_Tune4C_13TeV_pythia8_Phys14DR-PU20bx25_trkalmb_castor_PHYS14_25_V1-v2_MINIAODSIM_UCSB2376_v78/*");
  qcd_ch->Add("/cms8r0/johnbr/2014/csa14/reduced_trees/13TeV/QCD_Pt-470to600_Tune4C_13TeV_pythia8_Phys14DR-PU20bx25_trkalmb_castor_PHYS14_25_V1-v2_MINIAODSIM_UCSB2375_v78/*");
  //temp_ch->Add("/cms8r0/johnbr/2014/csa14/save/13TeV/QCD_Pt-600to800_Tune4C_13TeV_pythia8_Phys14DR-PU20bx25_trkalmb_castor_PHYS14_25_V1-v1_MINIAODSIM_UCSB2374_v78/*");
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

  ttbar_data->Add("/cms8r0/johnbr/2014/csa14/reduced_trees/13TeV/TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1_MINIAODSIM_UCSB2406_v78/*");
  
  qcd_data->Add("/cms8r0/johnbr/2014/csa14/reduced_trees/13TeV/QCD_Pt-300to470_Tune4C_13TeV_pythia8_Phys14DR-PU20bx25_trkalmb_castor_PHYS14_25_V1-v2_MINIAODSIM_UCSB2376_v78/*");
  qcd_data->Add("/cms8r0/johnbr/2014/csa14/reduced_trees/13TeV/QCD_Pt-470to600_Tune4C_13TeV_pythia8_Phys14DR-PU20bx25_trkalmb_castor_PHYS14_25_V1-v2_MINIAODSIM_UCSB2375_v78/*");
  //temp_data->Add("/cms8r0/johnbr/2014/csa14/save/13TeV/QCD_Pt-600to800_Tune4C_13TeV_pythia8_Phys14DR-PU20bx25_trkalmb_castor_PHYS14_25_V1-v1_MINIAODSIM_UCSB2374_v78/*");
  qcd_data->Add("/cms8r0/johnbr/2014/csa14/reduced_trees/13TeV/QCD_Pt-600to800_Tune4C_13TeV_pythia8_Phys14DR-PU20bx25_trkalmb_castor_PHYS14_25_V1-v1_MINIAODSIM_UCSB2374_v78/*");
  qcd_data->Add("/cms8r0/johnbr/2014/csa14/reduced_trees/13TeV/QCD_Pt-800to1000_Tune4C_13TeV_pythia8_Phys14DR-PU20bx25_trkalmb_castor_PHYS14_25_V1-v2_MINIAODSIM_UCSB2373_v78/*");
  qcd_data->Add("/cms8r0/johnbr/2014/csa14/reduced_trees/13TeV/QCD_Pt-1000to1400_Tune4C_13TeV_pythia8_Phys14DR-PU20bx25_trkalmb_castor_PHYS14_25_V1-v1_MINIAODSIM_UCSB2372_v78/*");
  qcd_data->Add("/cms8r0/johnbr/2014/csa14/reduced_trees/13TeV/QCD_Pt-1400to1800_Tune4C_13TeV_pythia8_Phys14DR-PU20bx25_trkalmb_castor_PHYS14_25_V1-v1_MINIAODSIM_UCSB2371_v78/*");
  qcd_data->Add("/cms8r0/johnbr/2014/csa14/reduced_trees/13TeV/QCD_Pt-1800to2400_Tune4C_13TeV_pythia8_Phys14DR-PU20bx25_trkalmb_PHYS14_25_V1-v2_MINIAODSIM_UCSB2370_v78/*");
  qcd_data->Add("/cms8r0/johnbr/2014/csa14/reduced_trees/13TeV/QCD_Pt-2400to3200_Tune4C_13TeV_pythia8_Phys14DR-PU20bx25_trkalmb_PHYS14_25_V1-v1_MINIAODSIM_UCSB2369_v78/*");
  qcd_data->Add("/cms8r0/johnbr/2014/csa14/reduced_trees/13TeV/QCD_Pt-3200_Tune4C_13TeV_pythia8_Phys14DR-PU20bx25_trkalmb_PHYS14_25_V1-v1_MINIAODSIM_UCSB2368_v78/*");

  znn_data->Add("/cms8r0/johnbr/2014/csa14/reduced_trees/13TeV/ZJetsToNuNu_HT-100to200_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1_MINIAODSIM_UCSB2408_v78/*");
  znn_data->Add("/cms8r0/johnbr/2014/csa14/reduced_trees/13TeV/ZJetsToNuNu_HT-200to400_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1_MINIAODSIM_UCSB2381_v78/*");
  znn_data->Add("/cms8r0/johnbr/2014/csa14/reduced_trees/13TeV/ZJetsToNuNu_HT-400to600_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v2_MINIAODSIM_UCSB2380_v78/*");
  znn_data->Add("/cms8r0/johnbr/2014/csa14/reduced_trees/13TeV/ZJetsToNuNu_HT-600toInf_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1_MINIAODSIM_UCSB2379_v78/*");

  wjets_data->Add("/cms8r0/johnbr/2014/csa14/reduced_trees/13TeV/WJetsToLNu_HT-100to200_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1_MINIAODSIM_UCSB2407_v78/*");
  wjets_data->Add("/cms8r0/johnbr/2014/csa14/reduced_trees/13TeV/WJetsToLNu_HT-200to400_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1_MINIAODSIM_UCSB2405_v78/*");
  wjets_data->Add("/cms8r0/johnbr/2014/csa14/reduced_trees/13TeV/WJetsToLNu_HT-400to600_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1_MINIAODSIM_UCSB2398_v78/*");
  wjets_data->Add("/cms8r0/johnbr/2014/csa14/reduced_trees/13TeV/WJetsToLNu_HT-600toInf_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1_MINIAODSIM_UCSB2397_v78/*");

  single_top_data->Add("/cms8r0/johnbr/2014/csa14/reduced_trees/13TeV/TBarToLeptons_s-channel-CSA14_Tune4C_13TeV-aMCatNLO-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1_MINIAODSIM_UCSB2411_v78/*");
  single_top_data->Add("/cms8r0/johnbr/2014/csa14/reduced_trees/13TeV/TBarToLeptons_t-channel_Tune4C_CSA14_13TeV-aMCatNLO-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1_MINIAODSIM_UCSB2412_v78/*");
  single_top_data->Add("/cms8r0/johnbr/2014/csa14/reduced_trees/13TeV/Tbar_tW-channel-DR_Tune4C_13TeV-CSA14-powheg-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1_MINIAODSIM_UCSB2395_v78/*");
  single_top_data->Add("/cms8r0/johnbr/2014/csa14/reduced_trees/13TeV/TToLeptons_s-channel-CSA14_Tune4C_13TeV-aMCatNLO-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1_MINIAODSIM_UCSB2410_v78/*");
  single_top_data->Add("/cms8r0/johnbr/2014/csa14/reduced_trees/13TeV/TToLeptons_t-channel-CSA14_Tune4C_13TeV-aMCatNLO-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1_MINIAODSIM_UCSB2409_v78/*");
  single_top_data->Add("/cms8r0/johnbr/2014/csa14/reduced_trees/13TeV/T_tW-channel-DR_Tune4C_13TeV-CSA14-powheg-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1_MINIAODSIM_UCSB2393_v78/*");
 
  other_data->Add("/cms8r0/johnbr/2014/csa14/reduced_trees/13TeV/TTWJets_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1_MINIAODSIM_UCSB2390_v78/*");
  other_data->Add("/cms8r0/johnbr/2014/csa14/reduced_trees/13TeV/TTZJets_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1_MINIAODSIM_UCSB2384_v78/*");
  other_data->Add("/cms8r0/johnbr/2014/csa14/reduced_trees/13TeV/WH_HToBB_WToLNu_M-125_13TeV_powheg-herwigpp_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2382_v78/*");
  other_data->Add("/cms8r0/johnbr/2014/csa14/reduced_trees/13TeV/TTbarH_M-125_13TeV_amcatnlo-pythia8-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v2_MINIAODSIM_UCSB2391_v78/*");
  other_data->Add("/cms8r0/johnbr/2014/csa14/reduced_trees/13TeV/ZH_HToBB_ZToNuNu_M-125_13TeV_powheg-herwigpp_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2392_v78/*");
  
 
  // ttbar_ch->Add("/cms8r0/johnbr/2014/csa14/reduced_trees/13TeV/skimmed/TTJets*v78*.root");
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

  // ttbar_data->Add("/cms8r0/johnbr/2014/csa14/reduced_trees/13TeV/skimmed/TTJets*v78*.root");
  // qcd_data->Add("/cms8r0/johnbr/2014/csa14/reduced_trees/13TeV/skimmed/QCD_Pt*v78*.root");
  // znn_data->Add("/cms8r0/johnbr/2014/csa14/reduced_trees/13TeV/skimmed/ZJets*v78*.root");
  // wjets_data->Add("/cms8r0/johnbr/2014/csa14/reduced_trees/13TeV/skimmed/WJets*v78*.root");
  // single_top_data->Add("/cms8r0/johnbr/2014/csa14/reduced_trees/13TeV/skimmed/TTo*v78*.root");
  // single_top_data->Add("/cms8r0/johnbr/2014/csa14/reduced_trees/13TeV/skimmed/TBarTo*v78*.root");
  // single_top_data->Add("/cms8r0/johnbr/2014/csa14/reduced_trees/13TeV/skimmed/*tW*v78*.root");
  // other_data->Add("/cms8r0/johnbr/2014/csa14/reduced_trees/13TeV/skimmed/ZH*v78*.root");
  // other_data->Add("/cms8r0/johnbr/2014/csa14/reduced_trees/13TeV/skimmed/WH*v78*.root");
  // other_data->Add("/cms8r0/johnbr/2014/csa14/reduced_trees/13TeV/skimmed/TTbar*v78*.root");
  // other_data->Add("/cms8r0/johnbr/2014/csa14/reduced_trees/13TeV/skimmed/TTW*v78*.root");
  // other_data->Add("/cms8r0/johnbr/2014/csa14/reduced_trees/13TeV/skimmed/TTZ*v78*.root");


  cout << "Entries in chains..." << endl;
  cout << "ttbar N=" << ttbar_ch->GetEntries() << endl;
  cout << "qcd N=" << qcd_ch->GetEntries() << endl;
  cout << "znn N=" << znn_ch->GetEntries() << endl;
  cout << "wjets N=" << wjets_ch->GetEntries() << endl;
  cout << "single_top N=" << single_top_ch->GetEntries() << endl;
  cout << "other N=" << other_ch->GetEntries() << endl;

  // quick skim
TCut ht("ht30>500"), njets("num_jets_pt30>=4"), mht30("mht30>200"), mu_veto("num_veto_mus==0"), el_veto("num_veto_els==0"), mdp("abs(jet1_dphi)>0.5&&abs(jet2_dphi)>0.5&&abs(jet3_dphi)>0.3");
  TCut isoTk("num_iso_tracks_pt15_mT==0");
  TCut mutkveto("num_mu_tracks_mT==0"), eltkveto("num_el_tracks_mT==0"), hadtkveto("num_had_tracks_mT==0");
  TCut tk3veto(mutkveto+eltkveto+hadtkveto);
  TCut cleaning("passesBadJetFilter&&PBNRcode>0");
  TCut baseline(ht+njets+mht30+mu_veto+el_veto+tk3veto+mdp+cleaning);
  //TCut baseline(ht+njets+mht30+mu_veto+el_veto+mdp+"Expectation==1");

  TCut Smu(ht+njets+"mht30>125&&num_veto_mus==1&&mu_pt>20&&mu_veto&&mu_ptW>200&&mu_mT<100");
  TCut Sel(ht+njets+"mht30>125&&num_veto_els==1&&el_pt>20&&el_veto&&el_ptW>200&&el_mT<100");

  TCut nj1("num_jets_pt30>=4&&num_jets_pt30<=6"), nj2("num_jets_pt30>=7&&num_jets_pt30<=8"), nj3("num_jets_pt30>=9");
  TCut nb0("num_csvm_jets30==0"), nb1("num_csvm_jets30==1"), nb2("num_csvm_jets30==2"), nb3("num_csvm_jets30>=3");
  TCut bgeq0("num_csvm_jets30>=0"), bgeq1("num_csvm_jets30>=1"), bgeq2("num_csvm_jets30>=2"), bgeq3("num_csvm_jets30>=3");

  TCut mht1("mht30>200&&mht30<500"), mht2("mht30>500&&mht30<750"), mht3("mht30>750");
  TCut ht1("ht30>500&&ht30<800"), ht2("ht30>800&&ht30<1200"), ht3("ht30>1200");

  TCut box1(mht1&&ht1), box2(mht1&&ht2), box3(mht1&&ht3), box4(mht2&&(ht1||ht2)), box5(mht2&&ht3), box6(mht3&&(ht2||ht3));

  /*draw_from_trees(TString var, TCut other_cuts,
    TString weights, TString title, int nelbinsx, 
    double xlow, double xup,
    TString options="plotSig:plotLog:plotData",
    double cut_low=-1, double cut_high=-1,
    TString plot_title="default")
  */
  draw_from_trees("0.5*(1-cos(mu_dTT))", Smu+nb0, "weightppb*100",
  		  ";#frac{1}{2}(1-cos#Delta#theta_{T});", 20, 0, 1,
  		  "plotData", -1, -1,
  		  "cdtt_single_mu_events_NB0_702");
  draw_from_trees("0.5*(1-cos(mu_dTT))", Smu+nb1, "weightppb*100",
  		  ";#frac{1}{2}(1-cos#Delta#theta_{T});", 20, 0, 1,
  		  "plotData", -1, -1,
  		  "cdtt_single_mu_events_NB1_702");
  draw_from_trees("0.5*(1-cos(mu_dTT))", Smu+nb2, "weightppb*100",
  		  ";#frac{1}{2}(1-cos#Delta#theta_{T});", 10, 0, 1,
  		  "plotData", -1, -1,
  		  "cdtt_single_mu_events_NB2_702");
  draw_from_trees("0.5*(1-cos(mu_dTT))", Smu+nb3, "weightppb*100",
  		  ";#frac{1}{2}(1-cos#Delta#theta_{T});", 10, 0, 1,
  		  "plotData", -1, -1,
  		  "cdtt_single_mu_events_NB3_702");
  draw_from_trees("0.5*(1-cos(el_dTT))", Sel+nb0, "weightppb*100",
  		  ";#frac{1}{2}(1-cos#Delta#theta_{T});", 20, 0, 1,
  		  "plotData", -1, -1,
  		  "cdtt_single_el_events_NB0_702");
  draw_from_trees("0.5*(1-cos(el_dTT))", Sel+nb1, "weightppb*100",
  		  ";#frac{1}{2}(1-cos#Delta#theta_{T});", 20, 0, 1,
  		  "plotData", -1, -1,
  		  "cdtt_single_el_events_NB1_702");
  draw_from_trees("0.5*(1-cos(el_dTT))", Sel+nb2, "weightppb*100",
  		  ";#frac{1}{2}(1-cos#Delta#theta_{T});", 10, 0, 1,
  		  "plotData", -1, -1,
  		  "cdtt_single_el_events_NB2_702");
  draw_from_trees("0.5*(1-cos(el_dTT))", Sel+nb3, "weightppb*100",
  		  ";#frac{1}{2}(1-cos#Delta#theta_{T});", 10, 0, 1,
  		  "plotData", -1, -1,
  		  "cdtt_single_el_events_NB3_702");



  return 0;
  
}

