#define draw_RLF_bins_of_X_cxx
#include "rtree_512.h"
#include <TStyle.h>
#include <TCanvas.h>
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

#include "macros/include/my_style.hpp"

#include <iostream>

using namespace std;

void set_hist_styles(TH1D* Hlow, TH1D* Hmed, TH1D* Hhigh, Width_t width=3) {

  Hlow->SetLineColor(1000);
  Hmed->SetLineColor(1004);
  Hhigh->SetLineColor(1003);
 
  Hlow->SetMarkerColor(1000);
  Hmed->SetMarkerColor(1004);
  Hhigh->SetMarkerColor(1003);

  Hlow->SetMarkerStyle(20);
  Hmed->SetMarkerStyle(20);
  Hhigh->SetMarkerStyle(20);

  Hlow->SetMarkerSize(1);
  Hmed->SetMarkerSize(1);
  Hhigh->SetMarkerSize(1);

  Hlow->SetLineWidth(width);
  Hmed->SetLineWidth(width);
  Hhigh->SetLineWidth(width);
  
}

void rtree_512::draw_RLF_bins_of_X()
{ 

  //  gROOT->SetStyle("CMS");
  gStyle->SetOptStat(0);
  TH1::StatOverflows(kTRUE);

  TH1::SetDefaultSumw2(); //trick to turn on Sumw2 for all histos

  const int nbins(8);
  double bins[nbins+1] = {200, 250, 300, 350, 400, 500, 600, 750, 2000};

  TCanvas * thecanvas= new TCanvas("thecanvas","the canvas",800,600);
  
  TH1D * Hlow_lost   = new TH1D("Hlow_lost","",nbins, bins);
  TH1D * Hlow_found   = new TH1D("Hlow_found","",nbins, bins);
  TH1D * Hlow   = new TH1D("Hlow","",nbins, bins);
  TH1D * Hmed_lost   = new TH1D("Hmed_lost","",nbins, bins);
  TH1D * Hmed_found   = new TH1D("Hmed_found","",nbins, bins);
  TH1D * Hmed   = new TH1D("Hmed","",nbins, bins);
  TH1D * Hhigh_lost   = new TH1D("Hhigh_lost","",nbins, bins);
  TH1D * Hhigh_found   = new TH1D("Hhigh_found","",nbins, bins);
  TH1D * Hhigh   = new TH1D("Hhigh","",nbins, bins);
  
  if (fChain == 0) return;

  Long64_t nentries = fChain->GetEntriesFast();

  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    if (entry==1||entry==10||entry==100||entry==1000||entry%10000==0) {
      printf("\r          ");
      printf("Entry %d/%d\n",entry,nentries);
    }
    // if (Cut(ientry) < 0) continue;

    // Baseline selection:
    if (!(mht30>200&&min_delta_phi_met_N>6&&num_jets_pt30>=4&&ht30>500)) continue;
    // one true e/mu
    if (!(num_true_mus+num_true_els==1 && num_true_had_taus==0)) continue;
    // muons
    if (num_true_mus==1) {    
      if (num_veto_mus==0) { // lost
	for (unsigned int imu(0); imu<true_mu_pt->size(); imu++) {
	  if (true_mu_ptW->at(imu)>200) {
	    if (ht30>500&&ht30<800) Hlow_lost->Fill(true_mu_ptW->at(imu), weightppb*10000);
	    else if (ht30>800&&ht30<1200) Hmed_lost->Fill(true_mu_ptW->at(imu), weightppb*10000);
	    else if (ht30>1200) Hhigh_lost->Fill(true_mu_ptW->at(imu), weightppb*10000);
	  }
	}
      } else if (num_veto_mus==1) { // found
	for (unsigned int imu(0); imu<mu_pt->size(); imu++) {
	  if (mu_ptW->at(imu)>200&&mu_pt->at(imu)>10&&mu_mini_iso_tr02->at(imu)<0.2&&mu_mT->at(imu)<100) {
	    if (ht30>500&&ht30<800) Hlow_found->Fill(mu_ptW->at(imu), weightppb*10000);
	    else if (ht30>800&&ht30<1200) Hmed_found->Fill(mu_ptW->at(imu), weightppb*10000);
	    else if (ht30>1200) Hhigh_found->Fill(mu_ptW->at(imu), weightppb*10000);
	  }
	}
      }
    }
    // electrons
    else if (num_true_els==1) {
      if (num_veto_els==0) { // lost
	for (unsigned int iel(0); iel<true_el_pt->size(); iel++) {
	  if (true_el_ptW->at(iel)>200) {
	    if (ht30>500&&ht30<800) Hlow_lost->Fill(true_el_ptW->at(iel), weightppb*10000);
	    else if (ht30>800&&ht30<1200) Hmed_lost->Fill(true_el_ptW->at(iel), weightppb*10000);
	    else if (ht30>1200) Hhigh_lost->Fill(true_el_ptW->at(iel), weightppb*10000);
	  }
	}
      } else if (num_veto_els==1) { // found
	for (unsigned int iel(0); iel<el_pt->size(); iel++) {
	  if (el_ptW->at(iel)>200&&el_pt->at(iel)>10&&el_mini_iso_tr02->at(iel)<0.2&&el_mT->at(iel)<100) {
	    if (ht30>500&&ht30<800) Hlow_found->Fill(el_ptW->at(iel), weightppb*10000);
	    else if (ht30>800&&ht30<1200) Hmed_found->Fill(el_ptW->at(iel), weightppb*10000);
	    else if (ht30>1200) Hhigh_found->Fill(el_ptW->at(iel), weightppb*10000);
	  }
	}
      }
    }
  }


  gPad->SetRightMargin(0.1);
  gPad->Modified();

  Hlow_found->SetBinContent(nbins, Hlow_found->GetBinContent(nbins)+Hlow_found->GetBinContent(nbins+1));
  Hmed_found->SetBinContent(nbins, Hmed_found->GetBinContent(nbins)+Hmed_found->GetBinContent(nbins+1));
  Hhigh_found->SetBinContent(nbins, Hhigh_found->GetBinContent(nbins)+Hhigh_found->GetBinContent(nbins+1));
  Hlow_lost->SetBinContent(nbins, Hlow_lost->GetBinContent(nbins)+Hlow_lost->GetBinContent(nbins+1));
  Hmed_lost->SetBinContent(nbins, Hmed_lost->GetBinContent(nbins)+Hmed_lost->GetBinContent(nbins+1));
  Hhigh_lost->SetBinContent(nbins, Hhigh_lost->GetBinContent(nbins)+Hhigh_lost->GetBinContent(nbins+1));


  Hlow->Divide(Hlow_lost, Hlow_found, 1, 1, "B");
  Hmed->Divide(Hmed_lost, Hmed_found, 1, 1, "B");
  Hhigh->Divide(Hhigh_lost, Hhigh_found, 1, 1, "B");


  Hlow_found->Print("all");
  Hlow_lost->Print("all");
  Hlow->Print("all");
  Hmed_found->Print("all");
  Hmed_lost->Print("all");
  Hmed->Print("all");

  TLegend * leg =  new TLegend(0.40,0.7,0.8,0.9);
  leg->SetFillColor(0);
  leg->SetBorderSize(0);
  leg->SetLineStyle(0);
  leg->SetFillStyle(0);
  leg->SetTextFont(132);
  leg->SetTextSize(.04);
  char label[100];
  sprintf(label,"500<H_{T}<800");
  leg->AddEntry(Hlow,label);
  sprintf(label,"800<H_{T}<1200");
  leg->AddEntry(Hmed,label);
  sprintf(label,"H_{T}>1200");
  leg->AddEntry(Hhigh,label);

  
  Hmed->SetXTitle("p_{T}^{W} [GeV]");

  TString ytitle="R_{LF}";
  Hhigh->SetYTitle(ytitle);
  Hmed->SetYTitle(ytitle);
  Hlow->SetYTitle(ytitle);
  Hmed->GetYaxis()->SetTitleOffset(1.3);

  double max = Hmed->GetMaximum();
  if (Hhigh->GetMaximum()>max) max = Hhigh->GetMaximum();
  if (Hlow->GetMaximum()>max) max = Hlow->GetMaximum();
  Hmed->SetMaximum(2.5);
  
  TString drawopt="hist e";
  Hmed->Draw(drawopt); drawopt="hist e SAME";
  Hhigh->Draw(drawopt); drawopt="hist e SAME";  
  Hlow->Draw(drawopt); drawopt="hist e SAME";

  //   if (var=="minDeltaPhi") {
  //     Hhigh->SetMinimum(0);
  //     Hhigh->GetXaxis()->SetRangeUser(0,2);
  //   }


  leg->Draw();

  thecanvas->Print("macros/junews/lostleptons/RLF/plots/mu_ptW_slices_of_HT_512.pdf"); 
    
  delete Hlow;
  delete Hmed;
  delete Hhigh;
  delete Hlow_found;
  delete Hmed_found;
  delete Hhigh_found;
  delete Hlow_lost;
  delete Hmed_lost;
  delete Hhigh_lost;
  // delete Hratio;
  delete thecanvas;
}

