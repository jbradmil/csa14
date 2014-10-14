// originally developed by Josh Thompson for RA2b 2011 analysis

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
#include "TGraphErrors.h"
#include "TH1.h"

// #include "macros/include/cms_style.h"

#include <iostream>

using namespace std;

void MJClosureTest(const TString MJVar = "MJ_pt100", const TString metsigVar = "met_over_sqrt_ht50", const float metsigCutVal = 4., const TString treestring = "save/QCD*v75*.root", const TCut xtraCuts="") 
{ 

  TH1::StatOverflows(true);
  TH1::SetDefaultSumw2(); //trick to turn on Sumw2 for all histos


  gStyle->SetOptStat(0);

  TCut baseline ="ht50>=500&&jet3_pt>50";
  TCut lep_veto("num_reco_veto_muons==0&&num_reco_veto_electrons==0");
  baseline = baseline+lep_veto+xtraCuts;
  TCut metsigCut = Form("%s>=%f",metsigVar.Data(),metsigCutVal);
  TCut metsigSB = Form("%s<1.5",metsigVar.Data());

  const unsigned int nbinsx(10);
  TH1D* hMJpass = new TH1D("hMJpass","",nbinsx,0,1500); // 
  TH1D* hMJfail = new TH1D("hMJfail","",nbinsx,0,1500); //

  TChain * pypu = new TChain("reduced_tree");
  pypu->Add(treestring);// treestring is passed as an argument
  pypu->Project("hMJpass",MJVar,(baseline+metsigCut)*"(weightppb)");
  hMJpass->Print("all");
  pypu->Project("hMJfail",MJVar,(baseline+metsigSB)*"(weightppb)");
  hMJfail->Print("all");

  // add overflow
  hMJpass->SetBinContent(nbinsx, hMJpass->GetBinContent(nbinsx)+hMJpass->GetBinContent(nbinsx+1));
  hMJfail->SetBinContent(nbinsx, hMJfail->GetBinContent(nbinsx)+hMJfail->GetBinContent(nbinsx+1));

  double x[nbinsx], y[nbinsx], xerr[nbinsx], yerr[nbinsx];
  
  for (unsigned int bin(0); bin<nbinsx; bin++) {
    x[bin]=hMJpass->GetBinCenter(bin+1);
    if(hMJfail->GetBinContent(bin+1)>0) y[bin]=hMJpass->GetBinContent(bin+1)/hMJfail->GetBinContent(bin+1);
    xerr[bin]=hMJpass->GetBinWidth(bin+1)/2;
    if(hMJfail->GetBinContent(bin+1)>0) {
      if (hMJpass->GetBinContent(bin+1)>0) yerr[bin]=y[bin]*sqrt( ::pow(hMJpass->GetBinError(bin+1)/hMJpass->GetBinContent(bin+1),2) + ::pow(hMJfail->GetBinError(bin+1)/hMJfail->GetBinContent(bin+1),2) );
      else yerr[bin]=y[bin]*(hMJfail->GetBinError(bin+1)/hMJfail->GetBinContent(bin+1));
    }
  }

  TCanvas * thecanvas= new TCanvas("thecanvas","the canvas",800,600);
  TGraphErrors* gr = new TGraphErrors(nbinsx,x,y,xerr,yerr);
  gr->SetLineColor(46);
  gr->SetMarkerColor(46);
  gr->SetLineWidth(2);
 
  gr->Draw("AP"); 

  gr->SetTitle("M_{J} - E_{T}^{miss}/#sqrt{H_{T}} closure test;M_{J} [GeV];B/A (high/low E_{T}^{miss}/#sqrt{H_{T}})");
  gr->GetYaxis()->SetTitleOffset(1.3);
  thecanvas->Print("macros/MJ/plots/"+metsigVar+"passOverfail_vs_"+MJVar+".pdf");


  delete pypu;
  delete hMJpass;
  delete hMJfail;
  delete gr;
  delete thecanvas;
}
