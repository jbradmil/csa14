
#include "TMath.h"
#include "TH1D.h"
#include "TChain.h"
#include "TLine.h"
#include "TLegend.h"
#include "TString.h"
#include "TLatex.h"
#include "TCut.h"
#include "TStyle.h"
#include "TPad.h"
#include "TROOT.h"
#include "TCanvas.h"
#include "TEfficiency.h"
#include "TGraphErrors.h"
#include "TH1.h"

// #include "macros/include/cms_style.h"

#include <iostream>

using namespace std;

void bTagEff(/*const float csvVal = 0.898, const TString treestring = "reduced_trees/SMS-T1bbbb_2J_mGl-1500_mLSP-100*v75*.root", const TCut xtraCuts=""*/) 
{ 

  TH1::StatOverflows(true);
  TH1::SetDefaultSumw2(); //trick to turn on Sumw2 for all histos


  gStyle->SetOptStat(0);

  const unsigned int nbinsx(10);
  TH1D* hPassT1bbbb = new TH1D("hPassT1bbbb","",nbinsx,0,400); // 
  TH1D* hAllT1bbbb = new TH1D("hAllT1bbbb","",nbinsx,0,400); //

  TChain * pypuT1bbbb = new TChain("reduced_tree");
  pypuT1bbbb->Add("reduced_trees/SMS-T1bbbb_2J_mGl-1500_mLSP-100*v75*.root");// treestring is passed as an argument
  pypuT1bbbb->Project("hPassT1bbbb","jet1_pt","abs(jet1_partonId)==5&&jet1_csv>0.898");
  pypuT1bbbb->Project("hAllT1bbbb","jet1_pt","abs(jet1_partonId)==5");
  pypuT1bbbb->Project("+hPassT1bbbb","jet2_pt","abs(jet2_partonId)==5&&jet2_csv>0.898");
  pypuT1bbbb->Project("+hAllT1bbbb","jet2_pt","abs(jet2_partonId)==5");
  pypuT1bbbb->Project("+hPassT1bbbb","jet3_pt","abs(jet3_partonId)==5&&jet3_csv>0.898");
  pypuT1bbbb->Project("+hAllT1bbbb","jet3_pt","abs(jet3_partonId)==5");
  pypuT1bbbb->Project("+hPassT1bbbb","jet4_pt","abs(jet4_partonId)==5&&jet4_csv>0.898");
  pypuT1bbbb->Project("+hAllT1bbbb","jet4_pt","abs(jet4_partonId)==5");
  pypuT1bbbb->Project("+hPassT1bbbb","jet5_pt","abs(jet5_partonId)==5&&jet5_csv>0.898");
  pypuT1bbbb->Project("+hAllT1bbbb","jet5_pt","abs(jet5_partonId)==5");
  pypuT1bbbb->Project("+hPassT1bbbb","jet6_pt","abs(jet6_partonId)==5&&jet6_csv>0.898");
  pypuT1bbbb->Project("+hAllT1bbbb","jet6_pt","abs(jet6_partonId)==5");
  hPassT1bbbb->Print("all");
  hAllT1bbbb->Print("all");



  // add overflow
  //  hPassT1bbbb->SetBinContent(nbinsx, hPassT1bbbb->GetBinContent(nbinsx)+hPassT1bbbb->GetBinContent(nbinsx+1));
  // hAllT1bbbb->SetBinContent(nbinsx, hAllT1bbbb->GetBinContent(nbinsx)+hAllT1bbbb->GetBinContent(nbinsx+1));

  TEfficiency* pEffT1bbbb = new TEfficiency(*hPassT1bbbb, *hAllT1bbbb);
  // TGraphAsymmErrors* grT1bbbb = pEffT1bbbb->CreateGraph();

  TH1D* hPassT1tttt = new TH1D("hPassT1tttt","",nbinsx,0,400); // 
  TH1D* hAllT1tttt = new TH1D("hAllT1tttt","",nbinsx,0,400); //

  TChain * pypuT1tttt = new TChain("reduced_tree");
  pypuT1tttt->Add("reduced_trees/SMS-T1tttt_2J_mGl-1500_mLSP-100*v75*.root");// treestring is passed as an argument
  pypuT1tttt->Project("hPassT1tttt","jet1_pt","abs(jet1_partonId)==5&&jet1_csv>0.898");
  pypuT1tttt->Project("hAllT1tttt","jet1_pt","abs(jet1_partonId)==5");
  pypuT1tttt->Project("+hPassT1tttt","jet2_pt","abs(jet2_partonId)==5&&jet2_csv>0.898");
  pypuT1tttt->Project("+hAllT1tttt","jet2_pt","abs(jet2_partonId)==5");
  pypuT1tttt->Project("+hPassT1tttt","jet3_pt","abs(jet3_partonId)==5&&jet3_csv>0.898");
  pypuT1tttt->Project("+hAllT1tttt","jet3_pt","abs(jet3_partonId)==5");
  pypuT1tttt->Project("+hPassT1tttt","jet4_pt","abs(jet4_partonId)==5&&jet4_csv>0.898");
  pypuT1tttt->Project("+hAllT1tttt","jet4_pt","abs(jet4_partonId)==5");
  pypuT1tttt->Project("+hPassT1tttt","jet5_pt","abs(jet5_partonId)==5&&jet5_csv>0.898");
  pypuT1tttt->Project("+hAllT1tttt","jet5_pt","abs(jet5_partonId)==5");
  pypuT1tttt->Project("+hPassT1tttt","jet6_pt","abs(jet6_partonId)==5&&jet6_csv>0.898");
  pypuT1tttt->Project("+hAllT1tttt","jet6_pt","abs(jet6_partonId)==5");
  hPassT1tttt->Print("all");
  hAllT1tttt->Print("all");


 TEfficiency* pEffT1tttt = new TEfficiency(*hPassT1tttt, *hAllT1tttt);

  TH1D* hPassttbar = new TH1D("hPassttbar","",nbinsx,0,400); // 
  TH1D* hAllttbar = new TH1D("hAllttbar","",nbinsx,0,400); //

  TChain * pyputtbar = new TChain("reduced_tree");
  pyputtbar->Add("reduced_trees/TTJets*v75*.root");// treestring is passed as an argument
  pyputtbar->Project("hPassttbar","jet1_pt","abs(jet1_partonId)==5&&jet1_csv>0.898");
  pyputtbar->Project("hAllttbar","jet1_pt","abs(jet1_partonId)==5");
  pyputtbar->Project("+hPassttbar","jet2_pt","abs(jet2_partonId)==5&&jet2_csv>0.898");
  pyputtbar->Project("+hAllttbar","jet2_pt","abs(jet2_partonId)==5");
  pyputtbar->Project("+hPassttbar","jet3_pt","abs(jet3_partonId)==5&&jet3_csv>0.898");
  pyputtbar->Project("+hAllttbar","jet3_pt","abs(jet3_partonId)==5");
  pyputtbar->Project("+hPassttbar","jet4_pt","abs(jet4_partonId)==5&&jet4_csv>0.898");
  pyputtbar->Project("+hAllttbar","jet4_pt","abs(jet4_partonId)==5");
  pyputtbar->Project("+hPassttbar","jet5_pt","abs(jet5_partonId)==5&&jet5_csv>0.898");
  pyputtbar->Project("+hAllttbar","jet5_pt","abs(jet5_partonId)==5");
  pyputtbar->Project("+hPassttbar","jet6_pt","abs(jet6_partonId)==5&&jet6_csv>0.898");
  pyputtbar->Project("+hAllttbar","jet6_pt","abs(jet6_partonId)==5");
  hPassttbar->Print("all");
  hAllttbar->Print("all");
  TEfficiency* pEffttbar = new TEfficiency(*hPassttbar, *hAllttbar);

  TH1D* hPassQCD = new TH1D("hPassQCD","",nbinsx,0,400); // 
  TH1D* hAllQCD = new TH1D("hAllQCD","",nbinsx,0,400); //

  TChain * pypuQCD = new TChain("reduced_tree");
  pypuQCD->Add("reduced_trees/QCD*v75*.root");// treestring is passed as an argument
  pypuQCD->Project("hPassQCD","jet1_pt","abs(jet1_partonId)==5&&jet1_csv>0.898");
  pypuQCD->Project("hAllQCD","jet1_pt","abs(jet1_partonId)==5");
  pypuQCD->Project("+hPassQCD","jet2_pt","abs(jet2_partonId)==5&&jet2_csv>0.898");
  pypuQCD->Project("+hAllQCD","jet2_pt","abs(jet2_partonId)==5");
  pypuQCD->Project("+hPassQCD","jet3_pt","abs(jet3_partonId)==5&&jet3_csv>0.898");
  pypuQCD->Project("+hAllQCD","jet3_pt","abs(jet3_partonId)==5");
  pypuQCD->Project("+hPassQCD","jet4_pt","abs(jet4_partonId)==5&&jet4_csv>0.898");
  pypuQCD->Project("+hAllQCD","jet4_pt","abs(jet4_partonId)==5");
  pypuQCD->Project("+hPassQCD","jet5_pt","abs(jet5_partonId)==5&&jet5_csv>0.898");
  pypuQCD->Project("+hAllQCD","jet5_pt","abs(jet5_partonId)==5");
  pypuQCD->Project("+hPassQCD","jet6_pt","abs(jet6_partonId)==5&&jet6_csv>0.898");
  pypuQCD->Project("+hAllQCD","jet6_pt","abs(jet6_partonId)==5");
  hPassQCD->Print("all");
  hAllQCD->Print("all");
  TEfficiency* pEffQCD = new TEfficiency(*hPassQCD, *hAllQCD);


  TCanvas * thecanvas= new TCanvas("thecanvas","the canvas",800,600);

  pEffT1bbbb->SetLineColor(38);
  pEffT1bbbb->SetMarkerColor(38);
  pEffT1bbbb->SetLineWidth(2);
  pEffT1tttt->SetLineColor(8);
  pEffT1tttt->SetMarkerColor(8);
  pEffT1tttt->SetLineWidth(2);
  pEffttbar->SetLineColor(46);
  pEffttbar->SetMarkerColor(46);
  pEffttbar->SetLineWidth(2);
  pEffQCD->SetLineColor(1);
  pEffQCD->SetMarkerColor(1);
  pEffQCD->SetLineWidth(2);

  pEffT1bbbb->Draw("AP"); 
  pEffT1tttt->Draw("P,same"); 
  pEffttbar->Draw("P,same"); 
  pEffQCD->Draw("P,same"); 

  pEffT1bbbb->SetTitle("CSVT b-Tagger Efficiency;p_{T} [GeV];Efficiency");
  // grT1bbbb->GetYaxis()->SetTitleOffset(1.3);
  thecanvas->Print("macros/twiki/plots/CSVTEff_vs_pt.pdf");
  thecanvas->Print("macros/twiki/plots/CSVTEff_vs_pt.png");


  // delete pypuT1bbbb;
  // delete hPassT1bbbb;
  // delete hFail;
  // delete gr;
  // delete thecanvas;
}
