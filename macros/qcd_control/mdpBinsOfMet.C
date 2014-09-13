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

//void drawBinsOfMet(const TString var = "minDeltaPhiN", const TString treestring = "/cu2/ra2b/reducedTrees/V00-02-25_fullpf2pat/reducedTree.SSVHPT_PF2PATjets_JES0_JER0_PFMET_METunc0_PUunc0_BTagEff0_HLTEff0.PythiaPUQCD.root")
void mdpBinsOfMet(const TString var = "min_delta_phi_met_N", const TString btag_cut="num_csvm_jets>=1", TString plotTitle="", bool logy = false, const TString treestring = "reduced_trees/QCD_Pt*.root")
{ 
  const TCut btag(btag_cut);
  //const TCut btag = "nbjetsSSVHPT>=2";
  //const TCut btag = "nbjetsSSVHPT==0";
  //const TCut btag = "nbjetsSSV0>=1";

  //const TString var ="minDeltaPhiN";
  //const TString var ="minDeltaPhi";
  //  const TString var ="bestTopMass";
  //  const TString var ="MET/(MET+HT)";
  //  const TString var ="deltaPhiMPTcaloMET";

  const bool drawLSB = true;
  const bool drawMSB = true;
  const bool drawSB = true;
  const bool drawSIG = true;

  float customMax = 1;
  if(logy) customMax = 10;
  //const float customMax = -1;
  const bool doRatio=false; //hack for a specific ratio plot

  const  int nbins=10;
  float min=0;
  float max=0;
  if (var=="bestTopMass") max=800;
  else if (var=="min_delta_phi_met"||var=="min_delta_phi_met_loose_jets") max=TMath::Pi();
  else if (var.Contains("min_delta_phi_met_N")) max=20;
  else if (var.Contains("minDeltaPhiK")) max=20;
  else if (var=="deltaPhiMPTcaloMET")    max=TMath::Pi();
  else max = 0.6; 

  //updated to use reducedTrees

  bool addOverflow = true;
  if (var.Contains("min_delta_phi_met")) addOverflow=false;
  else  if (var=="bestTopMass") addOverflow=false;
  else  if (var=="deltaPhiMPTcaloMET") addOverflow=false;

  //  gROOT->SetStyle("CMS");
  gStyle->SetOptStat(0);

  TCut baseline ="ht40>=400&&jet1_pt>70&&jet2_pt>70&&jet3_pt>50&&weightppb<100"; //no mindp, no MET
  TString dp_cut(var);
  if (var.Contains("min_delta_phi_met_N")) dp_cut+=">4.";
  else if (var.Contains("min_delta_phi_met")) dp_cut+=">0.3";

  TCut minDPcut(dp_cut);
  //TCut minDPcut = "minDeltaPhi > 0.2"; cout<<"warning -- using special minDeltaPhi cut! "<<minDPcut.GetTitle()<<endl;
  if (!var.Contains("min_delta_phi_met")) baseline = baseline&&minDPcut;
  TCut LSB = "met<60";
  TCut MSB = "met>=60 && met<120";
  TCut SB = "met>=120 && met <175";
  TCut SIG = "met>175";

  TCut cut1=baseline && LSB && btag;
  TCut cut2=baseline && MSB && btag;
  TCut cut3=baseline && SB && btag;
  TCut cut4=baseline && SIG && btag;

  TString selection1 = TString("weightppb*(")+cut1.GetTitle()+")";
  TString selection2 = TString("weightppb*(")+cut2.GetTitle()+")";
  TString selection3 = TString("weightppb*(")+cut3.GetTitle()+")";
  TString selection4 = TString("weightppb*(")+cut4.GetTitle()+")";

//   TChain madgraph("reducedTree");
//   madgraph.Add("/cu2/joshmt/V00-03-01_6/reducedTree.Baseline0_PF_JERbias6_pfMEThigh_PFLepRA20e0mu_minDP_MuonCleaning.QCD.root");

  TChain * pypu = new TChain("reduced_tree");
  //pypu->Add("/cu2/ra2b/reducedTrees/V00-02-05_v2/reducedTree.SSVHPT.PythiaPUQCD.root");
  //pypu->Add("/cu2/ra2b/reducedTrees/V00-02-25_fullpf2pat/reducedTree.SSVHPT_PF2PATjets_JES0_JER0_PFMET_METunc0_PUunc0_BTagEff0_HLTEff0.PythiaPUQCD.root");
  pypu->Add(treestring);// treestring is passed as an argument
  
  
//   TChain py("reducedTree");
//   py.Add("/cu2/joshmt/V00-03-01_6/reducedTree.Baseline0_PF_JERbias6_pfMEThigh_PFLepRA20e0mu_minDP_MuonCleaning.PythiaQCD.root");

  TH1::SetDefaultSumw2(); //trick to turn on Sumw2 for all histos

  //constint nbins=6;
  //  const double varbins[]={0.,160.,180.,260.,400.,800.,2000};

  int height= doRatio ? 800 : 600;
  TCanvas * thecanvas= new TCanvas("thecanvas","the canvas",700,height);
  if (doRatio) {
    thecanvas->Divide(1,2);
    const float padding=0.01; const float ydivide=0.2;
    thecanvas->GetPad(1)->SetPad( padding, ydivide + padding, 1-padding, 1-padding);
    thecanvas->GetPad(2)->SetPad( padding, padding, 1-padding, ydivide-padding);
    thecanvas->cd(1);
  }
  
  TH1D * Hlow   = new TH1D("Hlow","",nbins,min,max);
  TH1D * Hmed   = new TH1D("Hmed","",nbins,min,max);
  TH1D * Hmh    = new TH1D("Hmh","",nbins,min,max);
  TH1D * Hhigh  = new TH1D("Hhigh","",nbins,min,max);
  TH1D * Hratio = new TH1D("Hratio","",nbins,min,max);
  /*
  TH1D Hlow("Hlow","",nbins,varbins);
  TH1D Hmed("Hmed","",nbins,varbins);
  TH1D Hmh("Hmh","",nbins,varbins);
  TH1D Hhigh("Hhigh","",nbins,varbins);
  */
  
  Hlow->SetLineColor(kBlue);
  Hmed->SetLineColor(kGreen);
  Hmh->SetLineColor(kOrange-3);
  Hhigh->SetLineColor(kMagenta+2);
 
  Hlow->SetMarkerColor(kBlue);
  Hmed->SetMarkerColor(kGreen);
  Hmh->SetMarkerColor(kOrange-3);
  Hhigh->SetMarkerColor(kMagenta+2);

  Hlow->SetMarkerStyle(20);
  Hmed->SetMarkerStyle(21);
  Hmh->SetMarkerStyle(22);
  Hhigh->SetMarkerStyle(23);

  Hlow->SetMarkerSize(2);
  Hmed->SetMarkerSize(2);
  Hmh->SetMarkerSize(2);
  Hhigh->SetMarkerSize(2);

  float width=2.5;
  Hlow->SetLineWidth(width);
  Hmed->SetLineWidth(width);
  Hmh->SetLineWidth(width);
  Hhigh->SetLineWidth(width);

  if(logy){
    Hlow->SetMinimum(1E-2);
    Hmed->SetMinimum(1E-2);
    Hmh->SetMinimum(1E-2);
    Hhigh->SetMinimum(1E-2);
  }

  cout << "drawLSB" << endl;
  if (drawLSB) pypu->Draw(var+">>Hlow", selection1);
  cout << "drawMSB" << endl;
  if (drawMSB) pypu->Draw(var+">>Hmed", selection2);
  cout << "drawSB" << endl;
  if (drawSB)  pypu->Draw(var+">>Hmh",  selection3);
  cout << "drawSIG" << endl;
  if (drawSIG) pypu->Draw(var+">>Hhigh",selection4);
  gPad->SetRightMargin(0.1);
  gPad->SetLogy(logy);
  gPad->Modified();

  if (addOverflow) {
    Hlow->SetBinContent(nbins, Hlow->GetBinContent(nbins)+Hlow->GetBinContent(nbins+1));
    Hmed->SetBinContent(nbins, Hmed->GetBinContent(nbins)+Hmed->GetBinContent(nbins+1));
    Hmh->SetBinContent(nbins, Hmh->GetBinContent(nbins)+Hmh->GetBinContent(nbins+1));
    Hhigh->SetBinContent(nbins, Hhigh->GetBinContent(nbins)+Hhigh->GetBinContent(nbins+1));
  }
  
  if (Hlow->Integral()>0) Hlow->Scale( 1.0 / Hlow->Integral());
  if (Hmed->Integral()>0) Hmed->Scale( 1.0 / Hmed->Integral());
  if (Hmh->Integral()>0) Hmh->Scale( 1.0 / Hmh->Integral());
  if (Hhigh->Integral()>0) Hhigh->Scale( 1.0 / Hhigh->Integral());
  
  if (var=="min_delta_phi_met"||var=="min_delta_phi_met_loose_jets")  Hhigh->SetXTitle("#Delta #phi_{min} [rad.]");
  else if (var=="bestTopMass")  {
    TString title="best 3-jet mass (GeV)";
    Hhigh->SetXTitle(title);
    Hmh->SetXTitle(title);
    Hmed->SetXTitle(title);
    Hlow->SetXTitle(title);
  }
  else if (var.Contains("min_delta_phi_met_N")) {
    Hhigh->SetXTitle("#Delta #hat{#phi}_{N}^{min}");
  }
  else {
    Hhigh->SetXTitle(var);
  }
  TString ytitle="(a.u.)";
  Hhigh->SetYTitle(ytitle);
  Hmh->SetYTitle(ytitle);
  Hmed->SetYTitle(ytitle);
  Hlow->SetYTitle(ytitle);
  //TString thetitle=btag.GetTitle();
  TString thetitle="";
  //  thetitle += " (fail minDeltaPhi)";
  Hhigh->SetTitle(thetitle);
  Hmh->SetTitle(thetitle);
  Hmed->SetTitle(thetitle);
  Hlow->SetTitle(thetitle);

  TString drawopt="hist e";
  if (drawSIG)  {Hhigh->Draw(drawopt); drawopt="hist e SAME"; if (customMax>0) Hhigh->SetMaximum(customMax);}
  if (drawSB)   {Hmh->Draw(drawopt); drawopt="hist e SAME";   if (customMax>0) Hmh->SetMaximum(customMax);}
  if (drawMSB)  {Hmed->Draw(drawopt); drawopt="hist e SAME";  if (customMax>0) Hmed->SetMaximum(customMax);}
  if (drawLSB)  {Hlow->Draw(drawopt); drawopt="hist e SAME";  if (customMax>0) Hlow->SetMaximum(customMax);}

//   if (var=="minDeltaPhi") {
//     Hhigh->SetMinimum(0);
//     Hhigh->GetXaxis()->SetRangeUser(0,2);
//   }

  TLegend * leg = 0;
  if(logy) leg = new TLegend(0.38,0.65,0.8,0.85);
  else {leg = new TLegend(0.38,0.55,0.8,0.85);}
  leg->SetFillColor(0);
  leg->SetBorderSize(0);
  leg->SetLineStyle(0);
  leg->SetFillStyle(0);
  leg->SetTextFont(132);
  leg->SetTextSize(.04);
  if(logy) leg->SetTextSize(0.03);
  char label[100];
  if (drawLSB)  {
    sprintf(label,"E_{T}^{miss} < 60 GeV ( #mu=%3.2f)",Hlow->GetMean());
    leg->AddEntry(Hlow,label);
  }
  if (drawMSB)  {
    sprintf(label,"60 < E_{T}^{miss} < 120 GeV ( #mu=%3.2f)",Hmed->GetMean());
    leg->AddEntry(Hmed,label);
  } 
  if (drawSB)   {
    sprintf(label,"120 < E_{T}^{miss} < 175 Ge ( #mu=%3.2f)",Hmh->GetMean());
    leg->AddEntry(Hmh,label);
  }
  if (drawSIG)  {
    sprintf(label,"E_{T}^{miss} > 175 GeV ( #mu=%3.2f)",Hhigh->GetMean());
   leg->AddEntry(Hhigh,label);
  }
  leg->Draw();

  // TLatex* text1=0;
  // text1 = new TLatex(3.570061,23.08044,"CMS Simulation");
  // text1->SetNDC();
  // text1->SetTextAlign(13);
  // text1->SetX(0.45);
  // text1->SetY(.9);
  // text1->SetTextFont(132);
  // text1->SetTextSizePixels(24);
  // text1->Draw();

  if (doRatio) {
    Hratio->Divide(Hmh,Hlow);
    Hratio->SetLineWidth(2);
    thecanvas->cd(2);
    Hratio->Draw();
    Hratio->SetMinimum(0);
    Hratio->SetMaximum(3);
    TLine* l1 = new TLine(min,1,max,1);
    l1->SetLineColor(kMagenta);
    l1->SetLineWidth(2);
    l1->Draw();
    cout<<"KS test results = "<<Hmh->KolmogorovTest(Hlow)<<endl;
  }

//   double chi2=0;
//   for (int i=1; i<=nbins; i++) {
//     double denom=Hlow->GetBinError(i)*Hlow->GetBinError(i) + Hmh->GetBinError(i)*Hmh->GetBinError(i);
//     double c2= denom>0 ? pow( Hlow->GetBinContent(i) - Hmh->GetBinContent(i) ,2) / denom : 0;
//     chi2+=c2;
//   }

//   cout<<"Hand chi^2 = "<<chi2<<endl;
//   Hlow->Chi2Test(Hmh,"WW p");

  if (plotTitle=="") {
    if(logy) {thecanvas->Print("macros/qcd_control/plots/METcorrelation_"+var+"_logy.png");thecanvas->Print("macros/qcd_control/plots/METcorrelation_"+var+"_logy.pdf");}
    else {thecanvas->Print("macros/qcd_control/plots/METcorrelation_"+var+".png"); thecanvas->Print("macros/qcd_control/plots/METcorrelation_"+var+".pdf"); }
  }  
  else {
    if(logy) {thecanvas->Print("macros/qcd_control/plots/"+plotTitle+"_logy.png");thecanvas->Print("macros/qcd_control/plots/"+plotTitle+"_logy.pdf");}
    else {thecanvas->Print("macros/qcd_control/plots/"+plotTitle+".png"); thecanvas->Print("macros/qcd_control/plots/"+plotTitle+".pdf"); }
  }

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
//   plotSlices("nbjets>=1");

//   cplots.cd(3);
//   plotSlices("nbjets>=2");

//   cplots.SaveAs("compM3jsbvslsb.eps");

// }
