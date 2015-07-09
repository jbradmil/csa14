#include <iostream>
#include <vector>
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
#include "TCut.h"
#include "TChain.h"
#include "../../../include/my_style.hpp"
#include "classical_errors.h"
#include "texfile_maker.h"


using namespace std;

double int_lumi = 10.;

TCut baseline = "NJets > 3.1 && MHT > 200 && HT > 500 && DeltaPhi1 > 0.5 && DeltaPhi2 > 0.5 && DeltaPhi3 > 0.3";
TCut ZL = "Expectation == 1 && ExpectationReductionIsoTrack == 0";
TCut mht1("MHT>200&&MHT<500"), mht2("MHT>500&&MHT<750"), mht3("MHT>750");
TCut ht1("HT>500&&HT<800"), ht2("HT>800&&HT<1200"), ht3("HT>1200");
TCut box1(mht1&&ht1), box2(mht1&&ht2), box3(mht1&&ht3), box4(mht2&&(ht1||ht2)), box5(mht2&&ht3), box6(mht3&&(ht2||ht3));
TString weight = "Weight*23.75";
TCut singleMu = "selectedIDIsoMuonsNum==1 && selectedIDIsoElectronsNum==0 && MTW <100";
TCut singleEl = "selectedIDIsoMuonsNum==0 && selectedIDIsoElectronsNum==1 && MTW <100";
TString predSF = "totalWeightDiLepIsoTrackReduced*23.75/2.";

double GetPredictionError(double ratio, double rerror, double clint, double clerr, bool verb=false) { // returns fractional uncertainty
  if (verb) printf("Ratio = %3.4f +/- %3.4f\n", ratio, rerror);
  double fratio2 = pow(rerror/ratio, 2);
  double fclassical2 = pow(clerr/clint, 2);
  if (verb) printf("sqrt(%3.4f + %3.4f) = %3.4f\n", fratio2, fclassical2, sqrt(fratio2+fclassical2));
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

void PrintEventsTable(TH1D* hObs, TH1D* hPred, TH1D* hPredE) {

  TString section_name = hPred->GetName();
  section_name.ReplaceAll("hCPred", "");
  fprintf(ofile, "\n\\section{%s}\n\n", section_name.Data());

  fprintf(ofile, "\\begin{figure}[h]\n");
  fprintf(ofile, "\\centering\n");
  fprintf(ofile,  "\\caption{%s}\n", section_name.Data());
  fprintf(ofile,  "\\begin{subfigure}[b]{0.49\\textwidth}\n");
  fprintf(ofile,    "\\includegraphics[width=\\textwidth]{Closure/plots/%s.pdf}\n", section_name.Data());
  fprintf(ofile,    "\\caption{Classical}\n");
  fprintf(ofile,  "\\end{subfigure}\n");
  fprintf(ofile, "\\begin{subfigure}[b]{0.49\\textwidth}\n");
  fprintf(ofile,      "\\includegraphics[width=\\textwidth]{Closure/plots/%s.pdf}\n", (section_name+"_ext").Data());
  fprintf(ofile,   "\\caption{Extrapolation}\n");
  fprintf(ofile, "\\end{subfigure}\n");
  fprintf(ofile, "\\end{figure}\n\n\n");

  fprintf(ofile, "\\begin{table}[h]\n");
  fprintf(ofile, "\\centering\n");
  fprintf(ofile, "\\caption{Event yields for %s}\n", section_name.Data());
  fprintf(ofile, "\\begin{tabular}{|c|c|c|c|}\n");
  fprintf(ofile, "\\hline\n");
  fprintf(ofile, "HT-MHT Bin & $N_{cl}$ & $N_{ext}$ & $N_{obs}$ \\\\\n");
  fprintf(ofile, "\\hline\n");
  for (int i=1; i<=6; i++) {
    if (i>3) {
      double err_red = (hPred->GetBinContent(i)>0) ? (hPred->GetBinError(i)/hPred->GetBinContent(i)) / (hPredE->GetBinError(i)/hPredE->GetBinContent(i)) : err_red = 1 / (hPredE->GetBinError(i)/hPredE->GetBinContent(i));
      if (hPred->GetBinContent(i)>=10)	fprintf(ofile, "Box %d & $%3.1f\\pm%3.1f$ & $%3.1f\\pm%3.1f$ & $%3.1f\\pm%3.1f$  \\\\ \n", i,
						hPred->GetBinContent(i), hPred->GetBinError(i),
						hPredE->GetBinContent(i), hPredE->GetBinError(i),
						hObs->GetBinContent(i), hObs->GetBinError(i));
      else fprintf(ofile, "Box %d & $%3.2f\\pm%3.2f$ & $%3.2f\\pm%3.2f$ & $%3.2f\\pm%3.2f$ \\\\ \n", i,
		   hPred->GetBinContent(i), hPred->GetBinError(i),
		   hPredE->GetBinContent(i), hPredE->GetBinError(i),
		   hObs->GetBinContent(i), hObs->GetBinError(i));
    } else {
      fprintf(ofile, "Box %d & \\multicolumn{2}{c|}{$%3.1f\\pm%3.1f$} & $%3.1f\\pm%3.1f$ \\\\ \n", i, hPred->GetBinContent(i), hPred->GetBinError(i), hObs->GetBinContent(i), hObs->GetBinError(i) );
    }
    if (i==3) fprintf(ofile, "\\hline \n");
  }
  fprintf(ofile, "\\hline\n");
  fprintf(ofile, "\\end{tabular}\n");
  fprintf(ofile, "\\end{table}\n\n\n");
}

void PrintClosureTable(TH1D* hObs, TH1D* hPred, TH1D* hPredE) {
  TString section_name = hPred->GetName();
  section_name.ReplaceAll("hCPred", "");

  fprintf(ofile, "\\begin{table}[h]\n");
  fprintf(ofile, "\\centering\n");
  fprintf(ofile, "\\caption{Closure/uncertainty comparison for %s}\n", section_name.Data());
  fprintf(ofile, "\\begin{tabular}{|c|c|c|c|c|}\n");
  fprintf(ofile, "\\hline\n");
  fprintf(ofile, "HT-MHT Bin & $\\frac{\\delta(N_{cl})}{N_{cl}}$ & $\\frac{\\delta(N_{ext})}{N_{ext}}$ & $\\frac{N_{cl}-N_{obs}}{N_{obs}}$ &  $\\frac{N_{ext}-N_{obs}}{N_{obs}}$ \\\\\n");
  fprintf(ofile, "\\hline\n");
  for (int i=1; i<=6; i++) {
    double ferr_cl = (hPred->GetBinContent(i)>0) ? (hPred->GetBinError(i)/hPred->GetBinContent(i)) : 0;
    double fncl_cl = (hObs->GetBinContent(i)>0) ? (hPred->GetBinContent(i)-hObs->GetBinContent(i))/hObs->GetBinContent(i) : 0;
    if (i>3) {
      double ferr_ext = (hPredE->GetBinContent(i)>0) ? (hPredE->GetBinError(i)/hPredE->GetBinContent(i)) : 0;
      double fncl_ext = (hObs->GetBinContent(i)>0) ? (hPredE->GetBinContent(i)-hObs->GetBinContent(i))/hObs->GetBinContent(i) : 0;
      fprintf(ofile, "Box %d & $%3.2f$ & $%3.2f$ & $%3.2f$ & $%3.2f$ \\\\ \n", i,
	      ferr_cl, ferr_ext,
	      fncl_cl, fncl_ext);
    } else {
      fprintf(ofile, "Box %d & \\multicolumn{2}{c|}{$%3.2f$} & \\multicolumn{2}{c|}{$%3.2f$} \\\\ \n", i, ferr_cl, fncl_cl );
    }
    if (i==3) fprintf(ofile, "\\hline \n");
  }
  fprintf(ofile, "\\hline\n");
  fprintf(ofile, "\\end{tabular}\n");
  fprintf(ofile, "\\end{table}\n\n");
  fprintf(ofile, "\\clearpage\n\n");

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
  //  ratio->Print("all");
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
  
  c1->Print("Closure/plots/"+name+".pdf");

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

TH1D* GetExpecHist(TString expec_file, TString hname, TCut cuts) {
  
  const Int_t nx = 6;
  TString boxes[nx] = {"Box 1","Box 2","Box 3","Box 4", "Box 5", "Box 6"};
  TH1D* hist = new TH1D(hname, "", nx, 0, nx);
  hist->Sumw2();
  for (int i=1;i<=nx;i++) {
    hist->GetXaxis()->SetBinLabel(i,boxes[i-1].Data());
  }
  set_style(hist, "data_obs");
  TChain* ch = new TChain("LostLeptonExpectation");
  ch->Add(expec_file);

  ch->Project(hname, "0.5", (baseline+ZL+cuts+box1)*weight);
  hname.Prepend("+");
  ch->Project(hname, "1.5", (baseline+ZL+cuts+box2)*weight);
  ch->Project(hname, "2.5", (baseline+ZL+cuts+box3)*weight);
  ch->Project(hname, "3.5", (baseline+ZL+cuts+box4)*weight);
  ch->Project(hname, "4.5", (baseline+ZL+cuts+box5)*weight);
  ch->Project(hname, "5.5", (baseline+ZL+cuts+box6)*weight);

  //  hist->Print("all");

  return hist;

}

TH1D* GetCPredHist(TString cpred_file, TString hname, TCut cuts, int fErrInd) {
  
  const Int_t nx = 6;
  TString boxes[nx] = {"Box 1","Box 2","Box 3","Box 4", "Box 5", "Box 6"};
  TH1D* hist = new TH1D(hname, "", nx, 0, nx);
  for (int i=1;i<=nx;i++) {
    hist->GetXaxis()->SetBinLabel(i,boxes[i-1].Data());
  }
  set_style(hist, "pred");
  TChain* ch = new TChain("LostLeptonPrediction");
  ch->Add(cpred_file);

  ch->Project(hname, "0.5", (baseline+singleMu+cuts+box1)*predSF);
  hname.Prepend("+");
  ch->Project(hname, "1.5", (baseline+singleMu+cuts+box2)*predSF);
  ch->Project(hname, "2.5", (baseline+singleMu+cuts+box3)*predSF);
  ch->Project(hname, "3.5", (baseline+singleMu+cuts+box4)*predSF);
  ch->Project(hname, "4.5", (baseline+singleMu+cuts+box5)*predSF);
  ch->Project(hname, "5.5", (baseline+singleMu+cuts+box6)*predSF);
  ch->Project(hname, "0.5", (baseline+singleEl+cuts+box1)*predSF);
  ch->Project(hname, "1.5", (baseline+singleEl+cuts+box2)*predSF);
  ch->Project(hname, "2.5", (baseline+singleEl+cuts+box3)*predSF);
  ch->Project(hname, "3.5", (baseline+singleEl+cuts+box4)*predSF);
  ch->Project(hname, "4.5", (baseline+singleEl+cuts+box5)*predSF);
  ch->Project(hname, "5.5", (baseline+singleEl+cuts+box6)*predSF);

  for (int i = 0; i<6; i++) {
    if (classical_errors[fErrInd+i]<0) hist->SetBinError(i+1, hist->GetBinContent(i+1));
    else hist->SetBinError(i+1, hist->GetBinContent(i+1)*classical_errors[fErrInd+i]);
  }

  //  hist->Print("all");

  return hist;

}

TH1D* GetEPredHist(TString hname, TH1D* hPred, TH1D* hPred300, TH1D* hRatio, int ratio1_ind, int ratio2_ind=-1, int ratio3_ind=-1) {

  if (ratio2_ind<0) ratio2_ind = ratio1_ind;
  if (ratio3_ind<0) ratio3_ind = ratio2_ind;
  double ratio1 = hRatio->GetBinContent(ratio1_ind), ratio2 = hRatio->GetBinContent(ratio2_ind), ratio3 = hRatio->GetBinContent(ratio3_ind);
  double rerror1 = hRatio->GetBinError(ratio1_ind), rerror2 = hRatio->GetBinError(ratio2_ind), rerror3 = hRatio->GetBinError(ratio3_ind);

  //  printf("Ratios:    %3.4f+/-%3.4f    |    %3.4f+/-%3.4f    |    %3.4f+/-%3.4f\n", ratio1, rerror1, ratio2, rerror2, ratio3, rerror3); 
  const Int_t nx = 6;
  TString boxes[nx] = {"Box 1","Box 2","Box 3","Box 4", "Box 5", "Box 6"};
  TH1D* hEPred = new TH1D(hname, "", nx, 0, nx);
  for (int i=1;i<=nx;i++) {
    hEPred->GetXaxis()->SetBinLabel(i,boxes[i-1].Data());
  }
  set_style(hEPred, "pred");
 
  for (int i = 0; i<3; i++) {
      hEPred->SetBinContent(i+1, hPred->GetBinContent(i+1));
      hEPred->SetBinError(i+1, hPred->GetBinError(i+1));
  }
  Double_t cerr1(0.), cerr2(0.), cerr3(0.);
  double cpred1 = hPred300->IntegralAndError(1, 2, cerr1);
  double cpred2 = hPred300->IntegralAndError(3, 3, cerr2);
  double cpred3 = hPred300->IntegralAndError(2, 3, cerr3);
  hEPred->SetBinContent(4, cpred1*ratio1);
  hEPred->SetBinContent(5, cpred2*ratio2);
  hEPred->SetBinContent(6, cpred3*ratio3);
  hEPred->SetBinError(4, GetPredictionError(ratio1, rerror1, cpred1, cerr1)*cpred1*ratio1);
  hEPred->SetBinError(5, GetPredictionError(ratio2, rerror2, cpred2, cerr2)*cpred2*ratio2);
  hEPred->SetBinError(6, GetPredictionError(ratio3, rerror3, cpred3, cerr3, true)*cpred3*ratio3);

  //  hEPred->Print("all");

  return hEPred;

}

void DoClosureTests(TString expec_file="/cms8r0/johnbr/2014/csa14/Lost-Lepton/Expectation.root",
		    TString cpred_file="/cms8r0/johnbr/2014/csa14/Lost-Lepton/Prediction.root",
 		    TString ratiofile="outputfiles/analysis-output3-default.root")
{

  ofile = fopen("results.tex","w");
  PrintFileHeader();

  // TH1D* hExpNB0NJ1 = GetExpecHist(expec_file, "hExpNB0NJ1", "BTags==0&&NJets>=4&&NJets<=6");
  // TH1D* hExpNB0NJ2 = GetExpecHist(expec_file, "hExpNB0NJ2", "BTags==0&&NJets>=7&&NJets<=8");
  // TH1D* hExpNB0NJ3 = GetExpecHist(expec_file, "hExpNB0NJ3", "BTags==0&&NJets>=9");
  // TH1D* hExpNB1NJ1 = GetExpecHist(expec_file, "hExpNB1NJ1", "BTags==1&&NJets>=4&&NJets<=6");
  // TH1D* hExpNB1NJ2 = GetExpecHist(expec_file, "hExpNB1NJ2", "BTags==1&&NJets>=7&&NJets<=8");
  // TH1D* hExpNB1NJ3 = GetExpecHist(expec_file, "hExpNB1NJ3", "BTags==1&&NJets>=9");
  TH1D* hExpNB2NJ1 = GetExpecHist(expec_file, "hExpNB2NJ1", "BTags==2&&NJets>=4&&NJets<=6");
  TH1D* hExpNB2NJ2 = GetExpecHist(expec_file, "hExpNB2NJ2", "BTags==2&&NJets>=7&&NJets<=8");
  TH1D* hExpNB2NJ3 = GetExpecHist(expec_file, "hExpNB2NJ3", "BTags==2&&NJets>=9");
  TH1D* hExpNB3NJ1 = GetExpecHist(expec_file, "hExpNB3NJ1", "BTags>=3&&NJets>=4&&NJets<=6");
  TH1D* hExpNB3NJ2 = GetExpecHist(expec_file, "hExpNB3NJ2", "BTags>=3&&NJets>=7&&NJets<=8");
  TH1D* hExpNB3NJ3 = GetExpecHist(expec_file, "hExpNB3NJ3", "BTags>=3&&NJets>=9");


  // TH1D* hCPredNB0NJ1 = GetCPredHist(cpred_file, "hCPredNB0NJ1", "BTags==0&&NJets>=4&&NJets<=6", 0);
  // TH1D* hCPredNB0NJ2 = GetCPredHist(cpred_file, "hCPredNB0NJ2", "BTags==0&&NJets>=7&&NJets<=8", 24);
  // TH1D* hCPredNB0NJ3 = GetCPredHist(cpred_file, "hCPredNB0NJ3", "BTags==0&&NJets>=9", 48);
  // TH1D* hCPredNB1NJ1 = GetCPredHist(cpred_file, "hCPredNB1NJ1", "BTags==1&&NJets>=4&&NJets<=6", 6);
  // TH1D* hCPredNB1NJ2 = GetCPredHist(cpred_file, "hCPredNB1NJ2", "BTags==1&&NJets>=7&&NJets<=8", 30);
  // TH1D* hCPredNB1NJ3 = GetCPredHist(cpred_file, "hCPredNB1NJ3", "BTags==1&&NJets>=9", 54);
  TH1D* hCPredNB2NJ1 = GetCPredHist(cpred_file, "hCPredNB2NJ1", "BTags==2&&NJets>=4&&NJets<=6", 12);
  TH1D* hCPredNB2NJ2 = GetCPredHist(cpred_file, "hCPredNB2NJ2", "BTags==2&&NJets>=7&&NJets<=8", 36);
  TH1D* hCPredNB2NJ3 = GetCPredHist(cpred_file, "hCPredNB2NJ3", "BTags==2&&NJets>=9", 60);
  TH1D* hCPredNB3NJ1 = GetCPredHist(cpred_file, "hCPredNB3NJ1", "BTags>=3&&NJets>=4&&NJets<=6", 18);
  TH1D* hCPredNB3NJ2 = GetCPredHist(cpred_file, "hCPredNB3NJ2", "BTags>=3&&NJets>=7&&NJets<=8", 42);
  TH1D* hCPredNB3NJ3 = GetCPredHist(cpred_file, "hCPredNB3NJ3", "BTags>=3&&NJets>=9", 66);

  TH1D* hCPredNB2NJ1_MHT300 = GetCPredHist(cpred_file, "hCPredNB2NJ1_MHT300", "MHT>300&&BTags==2&&NJets>=4&&NJets<=6", 12);
  TH1D* hCPredNB2NJ2_MHT300 = GetCPredHist(cpred_file, "hCPredNB2NJ2_MHT300", "MHT>300&&BTags==2&&NJets>=7&&NJets<=8", 36);
  TH1D* hCPredNB2NJ3_MHT300 = GetCPredHist(cpred_file, "hCPredNB2NJ3_MHT300", "MHT>300&&BTags==2&&NJets>=9", 60);
  TH1D* hCPredNB3NJ1_MHT300 = GetCPredHist(cpred_file, "hCPredNB3NJ1_MHT300", "MHT>300&&BTags>=3&&NJets>=4&&NJets<=6", 18);
  TH1D* hCPredNB3NJ2_MHT300 = GetCPredHist(cpred_file, "hCPredNB3NJ2_MHT300", "MHT>300&&BTags>=3&&NJets>=7&&NJets<=8", 42);
  TH1D* hCPredNB3NJ3_MHT300 = GetCPredHist(cpred_file, "hCPredNB3NJ3_MHT300", "MHT>300&&BTags>=3&&NJets>=9", 66);

  TFile* rfile = new TFile(ratiofile.Data(), "read");
  TH1D* hratios = (TH1D*)rfile->Get("h_mhtratio_cdtt_superbins");
  
  // TH1D* hEPredNB0NJ1 = GetEPredHist("hEPredNB0NJ1", hCPredNB0NJ1, hratios, 2, 6, 10);
  // TH1D* hEPredNB0NJ2 = GetEPredHist("hEPredNB0NJ2", hCPredNB0NJ2, hratios, 2, 6, 10);
  // TH1D* hEPredNB0NJ3 = GetEPredHist("hEPredNB0NJ3", hCPredNB0NJ3, hratios, 2, 6, 10);
  // TH1D* hEPredNB1NJ1 = GetEPredHist("hEPredNB1NJ1", hCPredNB1NJ1, hratios, 3, 7, 10);
  // TH1D* hEPredNB1NJ2 = GetEPredHist("hEPredNB1NJ2", hCPredNB1NJ2, hratios, 4, 8, 10);
  // TH1D* hEPredNB1NJ3 = GetEPredHist("hEPredNB1NJ3", hCPredNB1NJ3, hratios, 4, 8, 10);
  TH1D* hEPredNB2NJ1 = GetEPredHist("hEPredNB2NJ1", hCPredNB2NJ1, hCPredNB2NJ1_MHT300, hratios, 3, 7, 10);
  TH1D* hEPredNB2NJ2 = GetEPredHist("hEPredNB2NJ2", hCPredNB2NJ2, hCPredNB2NJ2_MHT300, hratios, 4, 8, 10);
  TH1D* hEPredNB2NJ3 = GetEPredHist("hEPredNB2NJ3", hCPredNB2NJ3, hCPredNB2NJ3_MHT300, hratios, 4, 8, 10);
  TH1D* hEPredNB3NJ1 = GetEPredHist("hEPredNB3NJ1", hCPredNB3NJ1, hCPredNB3NJ1_MHT300, hratios, 3, 7, 10);
  TH1D* hEPredNB3NJ2 = GetEPredHist("hEPredNB3NJ2", hCPredNB3NJ2, hCPredNB3NJ2_MHT300, hratios, 4, 8, 10);
  TH1D* hEPredNB3NJ3 = GetEPredHist("hEPredNB3NJ3", hCPredNB3NJ3, hCPredNB3NJ3_MHT300, hratios, 4, 8, 10);

  // hExpNB0NJ1->Write();
  // hCPredNB0NJ1->Write();
  // hEPredNB0NJ1->Write();
  // hExpNB0NJ2->Write();
  // hCPredNB0NJ2->Write();
  // hEPredNB0NJ2->Write();
  // hExpNB0NJ3->Write();
  // hCPredNB0NJ3->Write();
  // hEPredNB0NJ3->Write();
  // hExpNB1NJ1->Write();
  // hCPredNB1NJ1->Write();
  // hEPredNB1NJ1->Write();
  // hExpNB1NJ2->Write();
  // hCPredNB1NJ2->Write();
  // hEPredNB1NJ2->Write();
  // hExpNB1NJ3->Write();
  // hCPredNB1NJ3->Write();
  // hEPredNB1NJ3->Write();
  // hExpNB2NJ1->Write();
  // hCPredNB2NJ1->Write();
  // hEPredNB2NJ1->Write();
  // hExpNB2NJ2->Write();
  // hCPredNB2NJ2->Write();
  // hEPredNB2NJ2->Write();
  // hExpNB2NJ3->Write();
  // hCPredNB2NJ3->Write();
  // hEPredNB2NJ3->Write();
  // hExpNB3NJ1->Write();
  // hCPredNB3NJ1->Write();
  // hEPredNB3NJ1->Write();
  // hExpNB3NJ2->Write();
  // hCPredNB3NJ2->Write();
  // hEPredNB3NJ2->Write();
  // hExpNB3NJ3->Write();
  // hCPredNB3NJ3->Write();
  // hEPredNB3NJ3->Write();

  TStyle *tdrStyle = new TStyle("tdrStyle","Style for P-TDR");
  setTDRStyle(tdrStyle);
  tdrStyle->cd();

  // MakeClosurePlot(hExpNB0NJ1, hCPredNB0NJ1, "NB0NJ1");
  // MakeClosurePlot(hExpNB0NJ1, hEPredNB0NJ1, "NB0NJ1_ext");
  // PrintEventsTable(hExpNB0NJ1, hCPredNB0NJ1, hEPredNB0NJ1);
  // PrintClosureTable(hExpNB0NJ1, hCPredNB0NJ1, hEPredNB0NJ1);
  // MakeClosurePlot(hExpNB0NJ2, hCPredNB0NJ2, "NB0NJ2");
  // MakeClosurePlot(hExpNB0NJ2, hEPredNB0NJ2, "NB0NJ2_ext");
  // PrintEventsTable(hExpNB0NJ2, hCPredNB0NJ2, hEPredNB0NJ2);
  // PrintClosureTable(hExpNB0NJ2, hCPredNB0NJ2, hEPredNB0NJ2);
  // MakeClosurePlot(hExpNB0NJ3, hCPredNB0NJ3, "NB0NJ3");
  // MakeClosurePlot(hExpNB0NJ3, hEPredNB0NJ3, "NB0NJ3_ext");
  // PrintEventsTable(hExpNB0NJ3, hCPredNB0NJ3, hEPredNB0NJ3);
  // PrintClosureTable(hExpNB0NJ3, hCPredNB0NJ3, hEPredNB0NJ3);
  
  // MakeClosurePlot(hExpNB1NJ1, hCPredNB1NJ1, "NB1NJ1");
  // MakeClosurePlot(hExpNB1NJ1, hEPredNB1NJ1, "NB1NJ1_ext");
  // PrintEventsTable(hExpNB1NJ1, hCPredNB1NJ1, hEPredNB1NJ1);
  // PrintClosureTable(hExpNB1NJ1, hCPredNB1NJ1, hEPredNB1NJ1);
  // MakeClosurePlot(hExpNB1NJ2, hCPredNB1NJ2, "NB1NJ2");
  // MakeClosurePlot(hExpNB1NJ2, hEPredNB1NJ2, "NB1NJ2_ext");
  // PrintEventsTable(hExpNB1NJ2, hCPredNB1NJ2, hEPredNB1NJ2);
  // PrintClosureTable(hExpNB1NJ2, hCPredNB1NJ2, hEPredNB1NJ2);
  // MakeClosurePlot(hExpNB1NJ3, hCPredNB1NJ3, "NB1NJ3");
  // MakeClosurePlot(hExpNB1NJ3, hEPredNB1NJ3, "NB1NJ3_ext");
  // PrintEventsTable(hExpNB1NJ3, hCPredNB1NJ3, hEPredNB1NJ3);
  // PrintClosureTable(hExpNB1NJ3, hCPredNB1NJ3, hEPredNB1NJ3);

  MakeClosurePlot(hExpNB2NJ1, hCPredNB2NJ1, "NB2NJ1");
  MakeClosurePlot(hExpNB2NJ1, hEPredNB2NJ1, "NB2NJ1_ext");
  PrintEventsTable(hExpNB2NJ1, hCPredNB2NJ1, hEPredNB2NJ1);
  PrintClosureTable(hExpNB2NJ1, hCPredNB2NJ1, hEPredNB2NJ1);
  MakeClosurePlot(hExpNB2NJ2, hCPredNB2NJ2, "NB2NJ2");
  MakeClosurePlot(hExpNB2NJ2, hEPredNB2NJ2, "NB2NJ2_ext");
  PrintEventsTable(hExpNB2NJ2, hCPredNB2NJ2, hEPredNB2NJ2);
  PrintClosureTable(hExpNB2NJ2, hCPredNB2NJ2, hEPredNB2NJ2);
  MakeClosurePlot(hExpNB2NJ3, hCPredNB2NJ3, "NB2NJ3");
  MakeClosurePlot(hExpNB2NJ3, hEPredNB2NJ3, "NB2NJ3_ext");
  PrintEventsTable(hExpNB2NJ3, hCPredNB2NJ3, hEPredNB2NJ3);
  PrintClosureTable(hExpNB2NJ3, hCPredNB2NJ3, hEPredNB2NJ3);
  
  MakeClosurePlot(hExpNB3NJ1, hCPredNB3NJ1, "NB3NJ1");
  MakeClosurePlot(hExpNB3NJ1, hEPredNB3NJ1, "NB3NJ1_ext");
  PrintEventsTable(hExpNB3NJ1, hCPredNB3NJ1, hEPredNB3NJ1);
  PrintClosureTable(hExpNB3NJ1, hCPredNB3NJ1, hEPredNB3NJ1);
  MakeClosurePlot(hExpNB3NJ2, hCPredNB3NJ2, "NB3NJ2");
  MakeClosurePlot(hExpNB3NJ2, hEPredNB3NJ2, "NB3NJ2_ext");
  PrintEventsTable(hExpNB3NJ2, hCPredNB3NJ2, hEPredNB3NJ2);
  PrintClosureTable(hExpNB3NJ2, hCPredNB3NJ2, hEPredNB3NJ2);
  MakeClosurePlot(hExpNB3NJ3, hCPredNB3NJ3, "NB3NJ3");
  MakeClosurePlot(hExpNB3NJ3, hEPredNB3NJ3, "NB3NJ3_ext");
  PrintEventsTable(hExpNB3NJ3, hCPredNB3NJ3, hEPredNB3NJ3);
  PrintClosureTable(hExpNB3NJ3, hCPredNB3NJ3, hEPredNB3NJ3);

  rfile->Close();

  PrintFileTrailer();
  fclose(ofile);
  
}
