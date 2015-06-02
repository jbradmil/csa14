#include <iostream>
#include <TH1.h>
#include <TH2.h>
#include <TStyle.h>
#include <TString.h>
#include <TCanvas.h>
#include <TFile.h>
#include <THStack.h>
#include "TStyle.h"
#include "TPad.h"
#include "TROOT.h"
#include "TSystem.h"
#include "TLegend.h"
#include "TPaveText.h"
#include "macros/include/my_style.hpp"

double int_lumi = 10.1;

double GetPredictionError(double ratio, double rerror, double clint, double clerr) { // returns fractional uncertainty
  double fratio2 = pow(rerror/ratio, 2), fclassical2 = pow(clerr/clint, 2);
  return sqrt(fratio2+fclassical2);
}

void SetPredictionBinContentAndError(int bin, TH1D* hPredE, TH1D* hClass, double ratio, double rerror, int classical_binl, int classical_binh) { 
  double clerr(0), clint(0);
  clint = hClass->IntegralAndError(classical_binl, classical_binh, clerr);
  double prediction = ratio*clint;
  double error = GetPredictionError(ratio, rerror, clint, clerr) * prediction;
  hPredE->SetBinContent(bin, prediction);
  hPredE->SetBinError(bin, error);
}

void PrintClosureTable(TH1D* hObs, TH1D* hPred, TH1D* hPredE) {
  printf("*** Closure test for %s *** \n", hObs->GetName());
  for (int i=1; i<=6; i++) {
    if (i>3) {
      double err_red = (hPred->GetBinContent(i)>0) ? (hPred->GetBinError(i)/hPred->GetBinContent(i)) / (hPredE->GetBinError(i)/hPredE->GetBinContent(i)) : err_red = 10000;
      if (hPred->GetBinContent(i)>=10)	printf("Box %d & $%3.1f\\pm%3.1f$ & $%3.1f\\pm%3.1f$ & $%3.0f$ & %3.1f \\\\ \n", i,
					       hPred->GetBinContent(i), hPred->GetBinError(i),
					       hPredE->GetBinContent(i), hPredE->GetBinError(i),
					       hObs->GetBinContent(i),
					       err_red);
      else printf("Box %d & $%3.2f\\pm%3.2f$ & $%3.2f\\pm%3.2f$ & $%3.0f$ & %3.1f \\\\ \n", i,
		  hPred->GetBinContent(i), hPred->GetBinError(i),
		  hPredE->GetBinContent(i), hPredE->GetBinError(i),
		  hObs->GetBinContent(i),
		  err_red);
    } else {
      printf("Box %d & \\multicolumn{2}{c|}{$%3.1f\\pm%3.1f$} & \\multicolumn{2}{c|}{$%3.0f$} \\\\ \n", i, hPred->GetBinContent(i), hPred->GetBinError(i), hObs->GetBinContent(i) );
    }
    if (i==3) printf("\\hline \n");
  }
  std::cout << std::endl;
}


void MakeClosurePlot(TH1D* hObs, TH1D* hPred, TString name, bool logy=true) {
  set_style(hObs,"data_obs");
  set_style(hPred,"pred");

   // Setup canvas and pads
  TCanvas * c1 = new TCanvas("c1", "c1", 700, 700);
  c1->SetFillStyle(4000);
  TPad * pad1 = new TPad("pad1", "top pad" , 0.0, 0.3, 1.0, 1.0);
  pad1->SetFillStyle(4000);
  pad1->SetBottomMargin(0.0);
  pad1->Draw();
  TPad * pad2 = new TPad("pad2", "bottom pad", 0.0, 0.0, 1.0, 0.3);
  pad2->SetFillStyle(4000);
  pad2->SetTopMargin(0.0);
  pad2->SetBottomMargin(0.35);
  pad2->Draw();
  pad1->cd();
  pad1->SetLogy(logy);

  TH1D * staterr = (TH1D *) hPred->Clone("staterr");
  //  staterr->Sumw2();
  //staterr->SetFillColor(kRed);
  staterr->SetFillColor(kGray+3);
  staterr->SetMarkerSize(0);
  staterr->SetFillStyle(3013);

  //cout << "Compute ratio hist..." << endl;
  TH1D * ratio = (TH1D *) hPred->Clone("ratio");
  //ratio->Sumw2();
  ratio->SetMarkerSize(0.8);
  ratio->SetMarkerColor(1);
  //ratio->SetMarkerSize(0.5);
  ratio->Divide(hObs, hPred, 1., 1., "B");
  ratio->Print("all");
  TH1D * ratiostaterr = (TH1D *) hObs->Clone("ratiostaterr");
  //  ratiostaterr->Sumw2();
  ratiostaterr->SetStats(0);
  ratiostaterr->SetTitle(hPred->GetTitle());
  ratiostaterr->GetYaxis()->SetTitle("Obs/Pred.");
  ratiostaterr->SetMaximum(2.2);
  ratiostaterr->SetMinimum(0);
  ratiostaterr->SetMarkerSize(0);
  //ratiostaterr->SetFillColor(kRed);
  ratiostaterr->SetFillColor(kGray+3);
  ratiostaterr->SetFillStyle(3013);
  ratiostaterr->GetXaxis()->SetLabelSize(0.2);
  ratiostaterr->GetXaxis()->SetLabelOffset(0.03);
  ratiostaterr->GetXaxis()->SetTitleSize(0.14);
  ratiostaterr->GetXaxis()->SetTitleOffset(1.10);
  ratiostaterr->GetYaxis()->SetLabelSize(0.10);
  ratiostaterr->GetYaxis()->SetTitleSize(0.12);
  ratiostaterr->GetYaxis()->SetTitleOffset(0.6);
  ratiostaterr->GetYaxis()->SetNdivisions(505);
  TLine* ratiounity = new TLine(hPred->GetBinLowEdge(1),1,hPred->GetBinLowEdge(hPred->GetNbinsX()+1),1);
  ratiounity->SetLineStyle(2);
  for (Int_t i = 0; i < hPred->GetNbinsX()+2; i++) {
    // if (hObs->GetBinContent(i)>0.) ratio->SetBinError(i, hObs->GetBinError(i)/hObs->GetBinContent(i)); // just the fractional uncertainty on the observation
    ratiostaterr->SetBinContent(i, 1.0);
    if (hPred->GetBinContent(i) > 1e-6) { //< not empty
      double binerror = hPred->GetBinError(i) / hPred->GetBinContent(i);
      ratiostaterr->SetBinError(i, binerror);
    } else {
      ratiostaterr->SetBinError(i, 999.);
    }
  }
  TH1D * ratiosysterr = (TH1D *) ratiostaterr->Clone("ratiosysterr");
  //  ratiosysterr->Sumw2();
  ratiosysterr->SetMarkerSize(0);
  ratiosysterr->SetFillColor(kYellow-4);
  //ratiosysterr->SetFillStyle(3002);
  ratiosysterr->SetFillStyle(1001);
  for (Int_t i = 0; i < hPred->GetNbinsX()+2; i++) {
    if (hPred->GetBinContent(i) > 1e-6) { //< not empty
      double binerror2 = (pow(hPred->GetBinError(i), 2));
      double binerror = sqrt(binerror2);
      ratiosysterr->SetBinError(i, binerror / hPred->GetBinContent(i));
    }
  }


  double max = hObs->GetMaximum();
  if (hPred->GetMaximum() > max) max = hPred->GetMaximum();

  if (logy) {
    hPred->SetMaximum(max*10);
    hPred->SetMinimum(0.09);
  } else {
    hPred->SetMaximum(max*1.5);
    hPred->SetMinimum(0.);
  }

  hPred->GetYaxis()->SetTitle("Events / bin");
  hPred->Draw("hist");
  hObs->Draw("e1,same");
  staterr->Draw("e2 same");
  hPred->GetXaxis()->SetTitleSize(0.035);
  hPred->GetXaxis()->SetLabelSize(0.035);
  /* hObs->GetYaxis()->SetTitleSize(0.035); */
  /* hObs->GetYaxis()->SetLabelSize(0.035); */

  TLegend * leg2 = new TLegend(0.72, 0.68, 0.94, 0.92);
  set_style(leg2,0.035);
  leg2->AddEntry(hObs,"Obs.","elp");
  leg2->AddEntry(hPred,"Pred.","f");
  leg2->AddEntry(staterr, "Syst. uncert.", "f");
  leg2->Draw();
  TLatex * latex = new TLatex();
  latex->SetNDC();
  latex->SetTextAlign(12);
  latex->SetTextFont(62);
  latex->SetTextSize(0.042);
  latex->DrawLatex(0.19, 0.89, "CMS Simulation");
  latex->SetTextSize(0.03);
  TString stamp = Form("#sqrt{s} = 13 TeV, L = %3.1f fb^{-1}", int_lumi);
  latex->DrawLatex(0.19, 0.84, stamp);

  pad2->cd();
  pad2->SetGridy(0);
  ratiostaterr->GetXaxis()->SetRangeUser(0,6);
  ratiostaterr->Draw("e2");
  //ratiosysterr->Draw("e2 same");
  ratiostaterr->Draw("e2 same");
  ratiounity->Draw();
  ratio->Draw("e1 same");

  TPaveText * pave = new TPaveText(0.18, 0.86, 0.4, 0.96, "brNDC");
  pave->SetLineColor(0);
  pave->SetFillColor(kWhite);
  pave->SetShadowColor(0);
  pave->SetBorderSize(1);
  double nchisq = hObs->Chi2Test(hPred, "UWCHI2/NDF, P"); // MC uncert. (stat)
  double p_value = hObs->Chi2Test(hPred, "UW"); // MC uncert. (stat)
  // //double kolprob = hdata_obs->KolmogorovTest(hmc_pred); // MC uncert. (stat)
  TText * text = pave->AddText(Form("#chi_{#nu}^{2}/ndf = %.3f, p = %.3f", nchisq, p_value));
  // //TText * text = pave->AddText(Form("#chi_{#nu}^{2} = %.3f, K_{s} = %.3f", nchisq, kolprob));
  text->SetTextFont(62);
  text->SetTextSize(0.07);
  // text->SetTextSize(0.06);
  pave->Draw();
  
  /* TLegend * ratioleg = new TLegend(0.72, 0.88, 0.94, 0.96); */
  /* set_style(ratioleg); */
  /* ratioleg->SetTextSize(0.07); */
  /* ratioleg->AddEntry(ratiostaterr, "MC uncert. (stat)", "f"); */
  /* ratioleg->Draw(); */

  pad1->cd();
  gPad->RedrawAxis();
  gPad->Modified();
  gPad->Update();
  pad2->cd();
  gPad->RedrawAxis();
  gPad->Modified();
  gPad->Update();

  c1->cd();
  
  c1->Print("macros/junews/lostleptons/529_trigger/closure/plots/"+name+".pdf");

  delete staterr;
  delete ratio;
  delete ratiostaterr;
  delete ratiosysterr;
  delete leg2;
  // delete ratioleg;
  delete pad1;
  delete pad2;
  delete c1;
  delete latex;
  
}

void DoClosureTests(TString outputfile="macros/junews/lostleptons/529_trigger/closure/closure.root",
			   //TString classicalfile="macros/junews/lostleptons/Classical/Classical.root",
			   TString ratiofile="macros/junews/lostleptons/529_trigger/outputfiles/analysis-output3.root") {

  TFile* ofile = new TFile(outputfile.Data(), "recreate");
  // TH1::SetDefaultSumW2();

  //TFile* cfile = new TFile(classicalfile.Data(), "read");


  
  TFile* rfile = new TFile(ratiofile.Data(), "read");
  // get ratios and uncertainties
  vector<double> ratios;
  vector<double> rerrors;
  TH1D* hratios = (TH1D*)rfile->Get("h_mhtratio_cdtt_superbins");
  ratios.push_back(hratios->GetBinContent(2));
  ratios.push_back(hratios->GetBinContent(3));
  ratios.push_back(hratios->GetBinContent(5));
  ratios.push_back(hratios->GetBinContent(6));
  ratios.push_back(hratios->GetBinContent(8));
  rerrors.push_back(hratios->GetBinError(2));
  rerrors.push_back(hratios->GetBinError(3));
  rerrors.push_back(hratios->GetBinError(5));
  rerrors.push_back(hratios->GetBinError(6));
  rerrors.push_back(hratios->GetBinError(8));

  const Int_t nx = 6;
  TString boxes[nx] = {"Box 1","Box 2","Box 3","Box 4", "Box 5", "Box 6"};
  
  TH1D* hObs_NB2_NJ1 = new TH1D("hObs_NB2_NJ1", "", nx, 0, nx);
  TH1D* hObs_NB2_NJ2 = new TH1D("hObs_NB2_NJ2", "", nx, 0, nx);
  TH1D* hObs_NB2_NJ3 = new TH1D("hObs_NB2_NJ3", "", nx, 0, nx);
  TH1D* hObs_NB3_NJ1 = new TH1D("hObs_NB3_NJ1", "", nx, 0, nx);
  TH1D* hObs_NB3_NJ2 = new TH1D("hObs_NB3_NJ2", "", nx, 0, nx);
  TH1D* hObs_NB3_NJ3 = new TH1D("hObs_NB3_NJ3", "", nx, 0, nx);
  TH1D* hPred_NB2_NJ1 = new TH1D("hPred_NB2_NJ1", "", nx, 0, nx);
  TH1D* hPred_NB2_NJ2 = new TH1D("hPred_NB2_NJ2", "", nx, 0, nx);
  TH1D* hPred_NB2_NJ3 = new TH1D("hPred_NB2_NJ3", "", nx, 0, nx);
  TH1D* hPred_NB3_NJ1 = new TH1D("hPred_NB3_NJ1", "", nx, 0, nx);
  TH1D* hPred_NB3_NJ2 = new TH1D("hPred_NB3_NJ2", "", nx, 0, nx);
  TH1D* hPred_NB3_NJ3 = new TH1D("hPred_NB3_NJ3", "", nx, 0, nx);

  TH1D* hPredE_NB2_NJ1 = new TH1D("hPredE_NB2_NJ1", "", nx, 0, nx);
  TH1D* hPredE_NB2_NJ2 = new TH1D("hPredE_NB2_NJ2", "", nx, 0, nx);
  TH1D* hPredE_NB2_NJ3 = new TH1D("hPredE_NB2_NJ3", "", nx, 0, nx);
  TH1D* hPredE_NB3_NJ1 = new TH1D("hPredE_NB3_NJ1", "", nx, 0, nx);
  TH1D* hPredE_NB3_NJ2 = new TH1D("hPredE_NB3_NJ2", "", nx, 0, nx);
  TH1D* hPredE_NB3_NJ3 = new TH1D("hPredE_NB3_NJ3", "", nx, 0, nx);

  for (int i=1;i<=nx;i++) {
    hObs_NB2_NJ1->GetXaxis()->SetBinLabel(i,boxes[i-1].Data());
    hObs_NB2_NJ2->GetXaxis()->SetBinLabel(i,boxes[i-1].Data());
    hObs_NB2_NJ3->GetXaxis()->SetBinLabel(i,boxes[i-1].Data());
    hObs_NB3_NJ1->GetXaxis()->SetBinLabel(i,boxes[i-1].Data());
    hObs_NB3_NJ2->GetXaxis()->SetBinLabel(i,boxes[i-1].Data());
    hObs_NB3_NJ3->GetXaxis()->SetBinLabel(i,boxes[i-1].Data());
    hPred_NB2_NJ1->GetXaxis()->SetBinLabel(i,boxes[i-1].Data());
    hPred_NB2_NJ2->GetXaxis()->SetBinLabel(i,boxes[i-1].Data());
    hPred_NB2_NJ3->GetXaxis()->SetBinLabel(i,boxes[i-1].Data());
    hPred_NB3_NJ1->GetXaxis()->SetBinLabel(i,boxes[i-1].Data());
    hPred_NB3_NJ2->GetXaxis()->SetBinLabel(i,boxes[i-1].Data());
    hPred_NB3_NJ3->GetXaxis()->SetBinLabel(i,boxes[i-1].Data());
    hPredE_NB2_NJ1->GetXaxis()->SetBinLabel(i,boxes[i-1].Data());
    hPredE_NB2_NJ2->GetXaxis()->SetBinLabel(i,boxes[i-1].Data());
    hPredE_NB2_NJ3->GetXaxis()->SetBinLabel(i,boxes[i-1].Data());
    hPredE_NB3_NJ1->GetXaxis()->SetBinLabel(i,boxes[i-1].Data());
    hPredE_NB3_NJ2->GetXaxis()->SetBinLabel(i,boxes[i-1].Data());
    hPredE_NB3_NJ3->GetXaxis()->SetBinLabel(i,boxes[i-1].Data());
  }

  Double_t Obs_NB2_NJ1[nx] = {1231.0, 239.0, 24.0, 15.0, 3., 2.};
  Double_t Obs_NB2_NJ2[nx] = {248., 142., 22., 3., 3., 0.};
  Double_t Obs_NB2_NJ3[nx] = {27., 19., 18., 1., 1., 0.};
  Double_t Obs_NB3_NJ1[nx] = {182., 44., 3., 3., 1., 2};
  Double_t Obs_NB3_NJ2[nx] = {73., 46., 11., 0., 2., 0.};
  Double_t Obs_NB3_NJ3[nx] = {9., 17., 3., 0., 0., 0.};

  Double_t Obs_NB2_NJ1_err[nx];
  Double_t Obs_NB2_NJ2_err[nx];
  Double_t Obs_NB2_NJ3_err[nx];
  Double_t Obs_NB3_NJ1_err[nx];
  Double_t Obs_NB3_NJ2_err[nx];
  Double_t Obs_NB3_NJ3_err[nx];

  for (int i=0;i<nx;i++) {
    Obs_NB2_NJ1_err[i] = sqrt(Obs_NB2_NJ1[i]);
    Obs_NB2_NJ2_err[i] = sqrt(Obs_NB2_NJ2[i]);
    Obs_NB2_NJ3_err[i] = sqrt(Obs_NB2_NJ3[i]);
    Obs_NB3_NJ1_err[i] = sqrt(Obs_NB3_NJ1[i]);
    Obs_NB3_NJ2_err[i] = sqrt(Obs_NB3_NJ2[i]);
    Obs_NB3_NJ3_err[i] = sqrt(Obs_NB3_NJ3[i]);
  }

  Double_t Pred_NB2_NJ1[nx] = {1110.0, 225.1, 16.7, 23.8, 4.3, 1.7};
  Double_t Pred_NB2_NJ2[nx] = {263.6, 135.6, 20.5, 2.9, 3.0, 0.9};
  Double_t Pred_NB2_NJ3[nx] = {22.6, 26.4, 11.9, 0.5, 0.5, 0.0};
  Double_t Pred_NB3_NJ1[nx] = {159.4, 41.2, 4.7, 4.0, 0.7, 1.4};
  Double_t Pred_NB3_NJ2[nx] = {63.4, 38.3, 4.4, 0.0, 1.3, 0.0};
  Double_t Pred_NB3_NJ3[nx] = {8.0, 9.0, 6.7, 0.0, 0.2, 0.0};

  Double_t Pred_NB2_NJ1_err[nx] = {25.8, 11.8, 3.3, 4.2, 1.7, 0.9};
  Double_t Pred_NB2_NJ2_err[nx] = {11.8, 8.7, 3.5, 1.2, 1.2, 0.9};
  Double_t Pred_NB2_NJ3_err[nx] = {3.5, 3.3, 2.5, 0.4, 0.3, 0.0};
  Double_t Pred_NB3_NJ1_err[nx] = {9.7, 5.2, 2.1, 1.9, 0.7, 1.4};
  Double_t Pred_NB3_NJ2_err[nx] = {5.7, 4.7, 1.7, 0.0, 1.0, 0.0};
  Double_t Pred_NB3_NJ3_err[nx] = {2.1, 2.0, 2.1, 0.0, 0.2, 0.0};

  for (int i=1;i<=nx;i++) {
    hObs_NB2_NJ1->SetBinContent(i,Obs_NB2_NJ1[i-1]);
    hObs_NB2_NJ2->SetBinContent(i,Obs_NB2_NJ2[i-1]);
    hObs_NB2_NJ3->SetBinContent(i,Obs_NB2_NJ3[i-1]);
    hObs_NB3_NJ1->SetBinContent(i,Obs_NB3_NJ1[i-1]);
    hObs_NB3_NJ2->SetBinContent(i,Obs_NB3_NJ2[i-1]);
    hObs_NB3_NJ3->SetBinContent(i,Obs_NB3_NJ3[i-1]);
    hObs_NB2_NJ1->SetBinError(i,Obs_NB2_NJ1_err[i-1]);
    hObs_NB2_NJ2->SetBinError(i,Obs_NB2_NJ2_err[i-1]);
    hObs_NB2_NJ3->SetBinError(i,Obs_NB2_NJ3_err[i-1]);
    hObs_NB3_NJ1->SetBinError(i,Obs_NB3_NJ1_err[i-1]);
    hObs_NB3_NJ2->SetBinError(i,Obs_NB3_NJ2_err[i-1]);
    hObs_NB3_NJ3->SetBinError(i,Obs_NB3_NJ3_err[i-1]);
    hPred_NB2_NJ1->SetBinContent(i,Pred_NB2_NJ1[i-1]);
    hPred_NB2_NJ2->SetBinContent(i,Pred_NB2_NJ2[i-1]);
    hPred_NB2_NJ3->SetBinContent(i,Pred_NB2_NJ3[i-1]);
    hPred_NB3_NJ1->SetBinContent(i,Pred_NB3_NJ1[i-1]);
    hPred_NB3_NJ2->SetBinContent(i,Pred_NB3_NJ2[i-1]);
    hPred_NB3_NJ3->SetBinContent(i,Pred_NB3_NJ3[i-1]);
    hPred_NB2_NJ1->SetBinError(i,Pred_NB2_NJ1_err[i-1]);
    hPred_NB2_NJ2->SetBinError(i,Pred_NB2_NJ2_err[i-1]);
    hPred_NB2_NJ3->SetBinError(i,Pred_NB2_NJ3_err[i-1]);
    hPred_NB3_NJ1->SetBinError(i,Pred_NB3_NJ1_err[i-1]);
    hPred_NB3_NJ2->SetBinError(i,Pred_NB3_NJ2_err[i-1]);
    hPred_NB3_NJ3->SetBinError(i,Pred_NB3_NJ3_err[i-1]);
    if (i<=3) { // same prediciton at low mht
      hPredE_NB2_NJ1->SetBinContent(i,Pred_NB2_NJ1[i-1]);
      hPredE_NB2_NJ2->SetBinContent(i,Pred_NB2_NJ2[i-1]);
      hPredE_NB2_NJ3->SetBinContent(i,Pred_NB2_NJ3[i-1]);
      hPredE_NB3_NJ1->SetBinContent(i,Pred_NB3_NJ1[i-1]);
      hPredE_NB3_NJ2->SetBinContent(i,Pred_NB3_NJ2[i-1]);
      hPredE_NB3_NJ3->SetBinContent(i,Pred_NB3_NJ3[i-1]);
      hPredE_NB2_NJ1->SetBinError(i,Pred_NB2_NJ1_err[i-1]);
      hPredE_NB2_NJ2->SetBinError(i,Pred_NB2_NJ2_err[i-1]);
      hPredE_NB2_NJ3->SetBinError(i,Pred_NB2_NJ3_err[i-1]);
      hPredE_NB3_NJ1->SetBinError(i,Pred_NB3_NJ1_err[i-1]);
      hPredE_NB3_NJ2->SetBinError(i,Pred_NB3_NJ2_err[i-1]);
      hPredE_NB3_NJ3->SetBinError(i,Pred_NB3_NJ3_err[i-1]);
    }
  }
  
  SetPredictionBinContentAndError(4, hPredE_NB2_NJ1, hPred_NB2_NJ1, ratios[0], rerrors[0], 1, 2);
  SetPredictionBinContentAndError(4, hPredE_NB2_NJ2, hPred_NB2_NJ2, ratios[1], rerrors[1], 1, 2);
  SetPredictionBinContentAndError(4, hPredE_NB2_NJ3, hPred_NB2_NJ3, ratios[1], rerrors[1], 1, 2);
  SetPredictionBinContentAndError(4, hPredE_NB3_NJ1, hPred_NB3_NJ1, ratios[0], rerrors[0], 1, 2);
  SetPredictionBinContentAndError(4, hPredE_NB3_NJ2, hPred_NB3_NJ2, ratios[1], rerrors[1], 1, 2);
  SetPredictionBinContentAndError(4, hPredE_NB3_NJ3, hPred_NB3_NJ3, ratios[1], rerrors[1], 1, 2);
  SetPredictionBinContentAndError(5, hPredE_NB2_NJ1, hPred_NB2_NJ1, ratios[2], rerrors[2], 3, 3);
  SetPredictionBinContentAndError(5, hPredE_NB2_NJ2, hPred_NB2_NJ2, ratios[3], rerrors[3], 3, 3);
  SetPredictionBinContentAndError(5, hPredE_NB2_NJ3, hPred_NB2_NJ3, ratios[3], rerrors[3], 3, 3);
  SetPredictionBinContentAndError(5, hPredE_NB3_NJ1, hPred_NB3_NJ1, ratios[2], rerrors[2], 3, 3);
  SetPredictionBinContentAndError(5, hPredE_NB3_NJ2, hPred_NB3_NJ2, ratios[3], rerrors[3], 3, 3);
  SetPredictionBinContentAndError(5, hPredE_NB3_NJ3, hPred_NB3_NJ3, ratios[3], rerrors[3], 3, 3);
  SetPredictionBinContentAndError(6, hPredE_NB2_NJ1, hPred_NB2_NJ1, ratios[4], rerrors[4], 2, 3);
  SetPredictionBinContentAndError(6, hPredE_NB2_NJ2, hPred_NB2_NJ2, ratios[4], rerrors[4], 2, 3);
  SetPredictionBinContentAndError(6, hPredE_NB2_NJ3, hPred_NB2_NJ3, ratios[4], rerrors[4], 2, 3);
  SetPredictionBinContentAndError(6, hPredE_NB3_NJ1, hPred_NB3_NJ1, ratios[4], rerrors[4], 2, 3);
  SetPredictionBinContentAndError(6, hPredE_NB3_NJ2, hPred_NB3_NJ2, ratios[4], rerrors[4], 2, 3);
  SetPredictionBinContentAndError(6, hPredE_NB3_NJ3, hPred_NB3_NJ3, ratios[4], rerrors[4], 2, 3);

  set_style(hObs_NB2_NJ1,"data_obs");
  set_style(hObs_NB2_NJ2,"data_obs");
  set_style(hObs_NB2_NJ3,"data_obs");
  set_style(hObs_NB3_NJ1,"data_obs");
  set_style(hObs_NB3_NJ2,"data_obs");
  set_style(hObs_NB3_NJ3,"data_obs");
  set_style(hPred_NB2_NJ1,"pred");
  set_style(hPred_NB2_NJ2,"pred");
  set_style(hPred_NB2_NJ3,"pred");
  set_style(hPred_NB3_NJ1,"pred");
  set_style(hPred_NB3_NJ2,"pred");
  set_style(hPred_NB3_NJ3,"pred");
  set_style(hPredE_NB2_NJ1,"pred");
  set_style(hPredE_NB2_NJ2,"pred");
  set_style(hPredE_NB2_NJ3,"pred");
  set_style(hPredE_NB3_NJ1,"pred");
  set_style(hPredE_NB3_NJ2,"pred");
  set_style(hPredE_NB3_NJ3,"pred");
   
  hObs_NB2_NJ1->Write();
  hObs_NB2_NJ2->Write();
  hObs_NB2_NJ3->Write();
  hObs_NB3_NJ1->Write();
  hObs_NB3_NJ2->Write();
  hObs_NB3_NJ3->Write();
  hPred_NB2_NJ1->Write();
  hPred_NB2_NJ2->Write();
  hPred_NB2_NJ3->Write();
  hPred_NB3_NJ1->Write();
  hPred_NB3_NJ2->Write();
  hPred_NB3_NJ3->Write();
  hPredE_NB2_NJ1->Write();
  hPredE_NB2_NJ2->Write();
  hPredE_NB2_NJ3->Write();
  hPredE_NB3_NJ1->Write();
  hPredE_NB3_NJ2->Write();
  hPredE_NB3_NJ3->Write();

  TStyle *tdrStyle = new TStyle("tdrStyle","Style for P-TDR");
  setTDRStyle(tdrStyle);
  tdrStyle->cd();
  MakeClosurePlot(hObs_NB2_NJ1, hPred_NB2_NJ1, "NB2_NJ1");
  MakeClosurePlot(hObs_NB2_NJ2, hPred_NB2_NJ2, "NB2_NJ2");
  MakeClosurePlot(hObs_NB2_NJ3, hPred_NB2_NJ3, "NB2_NJ3");
  MakeClosurePlot(hObs_NB3_NJ1, hPred_NB3_NJ1, "NB3_NJ1");
  MakeClosurePlot(hObs_NB3_NJ2, hPred_NB3_NJ2, "NB3_NJ2");
  MakeClosurePlot(hObs_NB3_NJ3, hPred_NB3_NJ3, "NB3_NJ3");

  MakeClosurePlot(hObs_NB2_NJ1, hPredE_NB2_NJ1, "NB2_NJ1_ext");
  MakeClosurePlot(hObs_NB2_NJ2, hPredE_NB2_NJ2, "NB2_NJ2_ext");
  MakeClosurePlot(hObs_NB2_NJ3, hPredE_NB2_NJ3, "NB2_NJ3_ext");
  MakeClosurePlot(hObs_NB3_NJ1, hPredE_NB3_NJ1, "NB3_NJ1_ext");
  MakeClosurePlot(hObs_NB3_NJ2, hPredE_NB3_NJ2, "NB3_NJ2_ext");
  MakeClosurePlot(hObs_NB3_NJ3, hPredE_NB3_NJ3, "NB3_NJ3_ext");

  PrintClosureTable(hObs_NB2_NJ1, hPred_NB2_NJ1, hPredE_NB2_NJ1);
  PrintClosureTable(hObs_NB2_NJ2, hPred_NB2_NJ2, hPredE_NB2_NJ2);
  PrintClosureTable(hObs_NB2_NJ3, hPred_NB2_NJ3, hPredE_NB2_NJ3);
  PrintClosureTable(hObs_NB3_NJ1, hPred_NB3_NJ1, hPredE_NB3_NJ1);
  PrintClosureTable(hObs_NB3_NJ2, hPred_NB3_NJ2, hPredE_NB3_NJ2);
  PrintClosureTable(hObs_NB3_NJ3, hPred_NB3_NJ3, hPredE_NB3_NJ3);
  
  ofile->Close();
}
