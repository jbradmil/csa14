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

// #include "macros/include/cms_style.h"

#include <iostream>

using namespace std;

void plotXBinsOfY(const TString xvar = "met_over_sqrt_ht30", int nbinsx = 25, double xlow=0., double xhigh=50., const TString yvar = "fatpT30_MJ", TString xAxisTitle="S_{MET}", TString yLegLabel = "M_{J}", const TString treestring = "reduced_trees/13TeV/QCD_HT_1000ToInf_13TeV-madgraph_Phys14DR-PU20bx25_PHYS14_25_V1_ext1-v1_MINIAODSIM_UCSB2297_v77/*.root", const TString plotNote="qcd", const TCut xtraCuts="", bool logy = false) 
{ 

  TH1::StatOverflows(true);
  TH1::SetDefaultSumw2(); //trick to turn on Sumw2 for all histos


 
  float customMax = 1;
  if(logy) customMax = 10;
  //const float customMax = -1;
  const bool doRatio=false; //hack for a specific ratio plot


  //updated to use reducedTrees

  bool addOverflow = true;

  //  gROOT->SetStyle("CMS");
  gStyle->SetOptStat(0);

  // TCut baseline ="ht50>=400&&met>150&&weightppb<1.";
  // TCut lep_veto("num_reco_veto_muons==0&&num_reco_veto_electrons==0");
  // baseline = baseline+lep_veto+xtraCuts;

  TCut baseline ="ht30>500&&num_jets_pt30>=4&&met_over_sqrt_ht30>0";

  TH1D* hBins = new TH1D("hbins","",10,0,10); // bin range shouldn't matter with TH1::StatOverflows(true);
  
  TChain * pypu = new TChain("reduced_tree");
  pypu->Add(treestring);// treestring is passed as an argument
  pypu->Project("hbins",yvar,baseline*"(weightppb)");

  int q2_up = TMath::Nint(hBins->GetMean());
  int q3_up = q2_up + TMath::Nint(hBins->GetRMS());
  int q1_up = q2_up - TMath::Nint(hBins->GetRMS());

  TCut LSB = Form("%s<=%d",yvar.Data(),q1_up);
  TCut MSB = Form("%s>%d && %s<=%d",yvar.Data(),q1_up,yvar.Data(),q2_up);
  TCut SB = Form("%s>%d && %s<=%d",yvar.Data(),q2_up,yvar.Data(),q3_up);
  TCut SIG = Form("%s>=%d",yvar.Data(),q3_up);
  
  TCut cut1=baseline && LSB;
  TCut cut2=baseline && MSB;
  TCut cut3=baseline && SB;
  TCut cut4=baseline && SIG;

  TString selection1 = TString("weightppb*(")+cut1.GetTitle()+")";
  TString selection2 = TString("weightppb*(")+cut2.GetTitle()+")";
  TString selection3 = TString("weightppb*(")+cut3.GetTitle()+")";
  TString selection4 = TString("weightppb*(")+cut4.GetTitle()+")";


  
  

  int height= doRatio ? 800 : 600;
  TCanvas * thecanvas= new TCanvas("thecanvas","the canvas",700,height);
  if (doRatio) {
    thecanvas->Divide(1,2);
    const float padding=0.01; const float ydivide=0.2;
    thecanvas->GetPad(1)->SetPad( padding, ydivide + padding, 1-padding, 1-padding);
    thecanvas->GetPad(2)->SetPad( padding, padding, 1-padding, ydivide-padding);
    thecanvas->cd(1);
  }
  
  TH1D * Hlow   = new TH1D("Hlow",";"+xAxisTitle+";(a.u.)",nbinsx,xlow,xhigh);
  TH1D * Hmed   = new TH1D("Hmed",";"+xAxisTitle+";(a.u.)",nbinsx,xlow,xhigh);
  TH1D * Hmh    = new TH1D("Hmh",";"+xAxisTitle+";(a.u.)",nbinsx,xlow,xhigh);
  TH1D * Hhigh  = new TH1D("Hhigh",";"+xAxisTitle+";(a.u.)",nbinsx,xlow,xhigh);
  TH1D * Hratio = new TH1D("Hratio",";"+xAxisTitle+";(a.u.)",nbinsx,xlow,xhigh);
 
  Hlow->SetLineColor(46);
  Hmed->SetLineColor(38);
  Hmh->SetLineColor(8);
  Hhigh->SetLineColor(kOrange);
 
  Hlow->SetMarkerColor(46);
  Hmed->SetMarkerColor(38);
  Hmh->SetMarkerColor(8);
  Hhigh->SetMarkerColor(kOrange);

  Hlow->SetMarkerStyle(23);
  Hmed->SetMarkerStyle(22);
  Hmh->SetMarkerStyle(21);
  Hhigh->SetMarkerStyle(20);

  Hlow->SetMarkerSize(1);
  Hmed->SetMarkerSize(1);
  Hmh->SetMarkerSize(1);
  Hhigh->SetMarkerSize(1);

  Width_t width=2;
  Hlow->SetLineWidth(width);
  Hmed->SetLineWidth(width);
  Hmh->SetLineWidth(width);
  Hhigh->SetLineWidth(width);

  Hlow->GetYaxis()->SetTitleOffset(1.3);
  Hmed->GetYaxis()->SetTitleOffset(1.3);
  Hmh->GetYaxis()->SetTitleOffset(1.3);
  Hhigh->GetYaxis()->SetTitleOffset(1.3);

  if(logy){
    Hlow->SetMinimum(1E-2);
    Hmed->SetMinimum(1E-2);
    Hmh->SetMinimum(1E-2);
    Hhigh->SetMinimum(1E-2);
  }

  cout << "drawLSB: " << selection1.Data() << endl;
  pypu->Draw(xvar+">>Hlow", selection1);
  cout << "drawMSB: " << selection2.Data() << endl;
  pypu->Draw(xvar+">>Hmed", selection2);
  cout << "drawSB: " << selection3.Data() << endl;
  pypu->Draw(xvar+">>Hmh",  selection3);
  cout << "drawSIG: " << selection4.Data() << endl;
  pypu->Draw(xvar+">>Hhigh",selection4);
  gPad->SetRightMargin(0.1);
  gPad->SetLogy(logy);
  gPad->Modified();

  TLegend * leg = 0;
  if(logy) leg = new TLegend(0.58,0.65,0.68,0.85);
  else {leg = new TLegend(0.58,0.55,0.68,0.85);}
  leg->SetFillColor(0);
  leg->SetBorderSize(0);
  leg->SetLineStyle(0);
  leg->SetFillStyle(0);
  leg->SetTextFont(132);
  leg->SetTextSize(.04);
  if(logy) leg->SetTextSize(0.03);
  char label[100];
  sprintf(label,"%s#leq%d (#mu=%3.2f)",yLegLabel.Data(),q1_up,Hlow->GetMean());
  leg->AddEntry(Hlow,label);
  sprintf(label,"%d<%s#leq%d (#mu=%3.2f)",q1_up,yLegLabel.Data(),q2_up,Hmed->GetMean());
  leg->AddEntry(Hmed,label);
  sprintf(label,"%d<%s#leq%d (#mu=%3.2f)",q2_up,yLegLabel.Data(),q3_up,Hmh->GetMean());
  leg->AddEntry(Hmh,label);
  sprintf(label,"%s>%d (#mu=%3.2f)",yLegLabel.Data(),q3_up,Hhigh->GetMean());
  leg->AddEntry(Hhigh,label);
 


  if (Hlow->Integral()>0) Hlow->Scale( 1.0 / Hlow->Integral());
  if (Hmed->Integral()>0) Hmed->Scale( 1.0 / Hmed->Integral());
  if (Hmh->Integral()>0) Hmh->Scale( 1.0 / Hmh->Integral());
  if (Hhigh->Integral()>0) Hhigh->Scale( 1.0 / Hhigh->Integral());

  if (addOverflow) {
    Hlow->SetBinContent(nbinsx, Hlow->GetBinContent(nbinsx)+Hlow->GetBinContent(nbinsx+1));
    Hmed->SetBinContent(nbinsx, Hmed->GetBinContent(nbinsx)+Hmed->GetBinContent(nbinsx+1));
    Hmh->SetBinContent(nbinsx, Hmh->GetBinContent(nbinsx)+Hmh->GetBinContent(nbinsx+1));
    Hhigh->SetBinContent(nbinsx, Hhigh->GetBinContent(nbinsx)+Hhigh->GetBinContent(nbinsx+1));
  }
  
  
  // Hhigh->SetXTitle("M_{J} [GeV]");
 
  // TString ytitle="(a.u.)";
  // Hhigh->SetYTitle(ytitle);
  // Hmh->SetYTitle(ytitle);
  // Hmed->SetYTitle(ytitle);
  // Hlow->SetYTitle(ytitle);
  // //TString thetitle=btag.GetTitle();
  // TString thetitle="";
  // //  thetitle += " (fail minDeltaPhi)";
  // Hhigh->SetTitle(thetitle);
  // Hmh->SetTitle(thetitle);
  // Hmed->SetTitle(thetitle);
  // Hlow->SetTitle(thetitle);

  double max = TMath::Max(Hhigh->GetMaximum(), Hmh->GetMaximum());
  if (Hmed->GetMaximum()>max) max = Hmed->GetMaximum();
  if (Hlow->GetMaximum()>max) max = Hlow->GetMaximum();
  Hhigh->SetMaximum(1.15*max);

  Hhigh->Draw("hist e SAME"); 
  Hmh->Draw("hist e SAME"); 
  Hmed->Draw("hist e SAME");
  Hlow->Draw("hist e SAME"); 



  leg->Draw();

  thecanvas->Print("macros/Phys14/plots/"+xvar+"_vs_"+yvar+"_"+plotNote+"_slices_119.pdf");


  delete pypu;
  delete Hlow;
  delete Hmed;
  delete Hmh;
  delete Hhigh;
  delete Hratio;
  delete thecanvas;
}

// plotAll() {

//   TCanvas cplots("cplots","plots",900,600);
//   cplots.Divide(1,3);

//   cplots.cd(1);
//   plotSlices("nbjets==1");

//   cplots.cd(2);
//   plotSlices("nbjets>=1");#Delta #hat{#phi

//   cplots.cd(3);
//   plotSlices("nbjets>=2");

//   cplots.SaveAs("compM3jsbvslsb.eps");

// }
