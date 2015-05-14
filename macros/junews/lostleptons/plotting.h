#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include <THStack.h>
#include "TStyle.h"
#include "TPad.h"
#include "TROOT.h"
#include "TFile.h"
#include "TSystem.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "macros/include/my_style.hpp"

void MakeDTTPDFPlot(TH1D* hLost, TH1D* hFound, TString hname, TString plotdir) {

  set_style(hLost, "ttbar");
  hFound->SetLineColor(2);
  hFound->SetLineWidth(3);
  hFound->SetLineStyle(7);

  TCanvas * c1 = new TCanvas("c1", "c1", 700, 700);
  c1->SetFillStyle(4000);

  hLost->GetYaxis()->SetTitle("Fraction of events");
  hLost->GetXaxis()->SetLabelSize(0.04);
  hLost->GetXaxis()->SetTitleSize(0.04);
  hLost->GetXaxis()->SetTitleOffset(1.3);
  hLost->GetYaxis()->SetLabelSize(0.04);
  hLost->GetYaxis()->SetTitleSize(0.04);
  hLost->GetYaxis()->SetTitleOffset(1.3);
  hLost->Draw("hist");
  hFound->Draw("hist,same");

  TLegend * leg2 = new TLegend(0.2, 0.75, 0.4, 0.92);
  set_style(leg2,0.035);
  leg2->AddEntry(hLost,"Lost","f");
  leg2->AddEntry(hFound,"Found","l");
  leg2->Draw();

  c1->Print(plotdir+"PDF/plots/ll_PDFs_"+hname+".pdf");

  delete c1;
  delete leg2;
  
}

void MakeClosurePlot(TH1D* hPred, TH1D* hTrue, TString hname, TString plotdir, bool logy=true) {

  set_style(hTrue, "data_obs");
  set_style(hPred, "ttbar");
  hPred->SetFillColor(1007);
  
  double SF = hTrue->GetBinContent(1)/hPred->GetBinContent(1);
  hPred->Scale(SF);

   // Setup canvas and pads  
  TCanvas * c1 = new TCanvas("c1", "c1", 700, 700);
  c1->SetFillStyle(4000);
  TPad * pad1 = new TPad("pad1", "top pad" , 0.0, 0.3, 1.0, 1.0);
  pad1->SetBottomMargin(0.0);
  pad1->Draw();
  TPad * pad2 = new TPad("pad2", "bottom pad", 0.0, 0.0, 1.0, 0.3);
  pad2->SetTopMargin(0.0);
  pad2->SetBottomMargin(0.35);
  pad2->Draw();
  pad1->cd();
  pad1->SetLogy(logy);

  TH1D * staterr = (TH1D *) hPred->Clone("staterr");
  staterr->Sumw2();
  //staterr->SetFillColor(kRed);
  staterr->SetFillColor(kGray+3);
  staterr->SetMarkerSize(0);
  staterr->SetFillStyle(3013);

  //cout << "Compute ratio hist..." << endl;
  TH1D * ratio = (TH1D *) hPred->Clone("ratio");
  ratio->Sumw2();
  ratio->SetMarkerSize(0.8);
  ratio->SetMarkerColor(1);
  //ratio->SetMarkerSize(0.5);
  ratio->Divide(hTrue, hPred, 1., 1., "");
  TH1D * ratiostaterr = (TH1D *) hTrue->Clone("ratiostaterr");
  ratiostaterr->Sumw2();
  ratiostaterr->SetStats(0);
  ratiostaterr->SetTitle(hPred->GetTitle());
  ratiostaterr->GetYaxis()->SetTitle("True/Pred.");
  ratiostaterr->SetMaximum(2.2);
  ratiostaterr->SetMinimum(0);
  ratiostaterr->SetMarkerSize(0);
  //ratiostaterr->SetFillColor(kRed);
  ratiostaterr->SetFillColor(kGray+3);
  ratiostaterr->SetFillStyle(3013);
  ratiostaterr->GetXaxis()->SetLabelSize(0.12);
  ratiostaterr->GetXaxis()->SetTitleSize(0.14);
  ratiostaterr->GetXaxis()->SetTitleOffset(1.10);
  ratiostaterr->GetYaxis()->SetLabelSize(0.10);
  ratiostaterr->GetYaxis()->SetTitleSize(0.12);
  ratiostaterr->GetYaxis()->SetTitleOffset(0.6);
  ratiostaterr->GetYaxis()->SetNdivisions(505);
  TLine* ratiounity = new TLine(hPred->GetBinLowEdge(1),1,hPred->GetBinLowEdge(hPred->GetNbinsX()+1),1);
  ratiounity->SetLineStyle(2);
  for (Int_t i = 0; i < hPred->GetNbinsX()+2; i++) {
    ratiostaterr->SetBinContent(i, 1.0);
    if (hPred->GetBinContent(i) > 1e-6) { //< not empty
      double binerror = hPred->GetBinError(i) / hPred->GetBinContent(i);
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
  for (Int_t i = 0; i < hPred->GetNbinsX()+2; i++) {
    if (hPred->GetBinContent(i) > 1e-6) { //< not empty
      double binerror2 = (pow(hPred->GetBinError(i), 2));
      double binerror = sqrt(binerror2);
      ratiosysterr->SetBinError(i, binerror / hPred->GetBinContent(i));
    }
  }

  double max = hTrue->GetMaximum();
  if (hPred->GetMaximum() > max) max = hPred->GetMaximum();

  if (logy) {
    hPred->SetMaximum(max*10);
    hPred->SetMinimum(0.5);
  } else {
    hPred->SetMaximum(max*1.5);
    hPred->SetMinimum(0.);
  }

  hPred->GetYaxis()->SetTitle("Events / bin");
  hPred->Draw("hist");
  hTrue->Draw("e1,same");
  staterr->Draw("e2 same");
  hPred->GetXaxis()->SetTitleSize(0.035);
  hPred->GetXaxis()->SetLabelSize(0.035);
  /* hTrue->GetYaxis()->SetTitleSize(0.035); */
  /* hTrue->GetYaxis()->SetLabelSize(0.035); */

  TLegend * leg2 = new TLegend(0.72, 0.68, 0.94, 0.92);
  set_style(leg2,0.035);
  leg2->AddEntry(hTrue,"True.","elp");
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
  latex->DrawLatex(0.19, 0.84, "#sqrt{s} = 13 TeV, L = 10 fb^{-1}");

  pad2->cd();
  pad2->SetGridy(0);
  ratiostaterr->Draw("e2");
  //ratiosysterr->Draw("e2 same");
  ratiostaterr->Draw("e2 same");
  ratiounity->Draw();
  ratio->Draw("e1 same");
  TLegend * ratioleg = new TLegend(0.72, 0.88, 0.94, 0.96);
  set_style(ratioleg);
  ratioleg->SetTextSize(0.07);
  ratioleg->AddEntry(ratiostaterr, "MC uncert. (stat)", "f");
  ratioleg->Draw();

  pad1->cd();
  gPad->RedrawAxis();
  gPad->Modified();
  gPad->Update();
  pad2->cd();
  gPad->RedrawAxis();
  gPad->Modified();
  gPad->Update();

  c1->cd();
  
  c1->Print(plotdir+"closure/plots/closure_test"+hname+".pdf");

  delete staterr;
  delete ratio;
  delete ratiostaterr;
  delete ratiosysterr;
  delete leg2;
  delete ratioleg;
  delete pad1;
  delete pad2;
  delete c1;
  delete latex;

}

void MakeWPTPlot(TH1D* hPTW_mu, TH1D* hPTW_mu_RLF, TH1D* hPTW_el, TH1D* hPTW_el_RLF, TString plotdir, bool logy=true) {

  TCanvas * c1 = new TCanvas("c1", "c1", 700, 700);
  c1->SetFillStyle(4000);
  hPTW_mu->SetLineWidth(3);
  hPTW_mu->SetLineStyle(7);
  hPTW_mu->SetLineColor(1000);
  hPTW_mu_RLF->SetLineWidth(3);
  hPTW_mu_RLF->SetLineColor(1000);
  hPTW_el->SetLineWidth(3);
  hPTW_el->SetLineStyle(7);
  hPTW_el->SetLineColor(1002);
  hPTW_el_RLF->SetLineWidth(3);
  hPTW_el_RLF->SetLineColor(1002);

  TLegend * leg1 = new TLegend(0.5, 0.75, 0.7, 0.87);
  set_style(leg1,0.035);
  leg1->SetTextColor(1000);
  leg1->SetHeader("Muons");
  leg1->AddEntry(hPTW_mu,"Found","l");
  leg1->AddEntry(hPTW_mu_RLF,"Lost","l");
  leg1->Draw();
  TLegend * leg2 = new TLegend(0.7, 0.75, 0.9, 0.87);
  set_style(leg2,0.035);
  leg2->SetTextColor(1002);
  leg2->SetHeader("Electrons");
  leg2->AddEntry(hPTW_el,"Found","l");
  leg2->AddEntry(hPTW_el_RLF,"Lost","l");
  leg2->Draw();

  double max = hPTW_mu->GetMaximum();
  if (hPTW_mu_RLF->GetMaximum()>max) max = hPTW_mu_RLF->GetMaximum();
  if (hPTW_el->GetMaximum()>max) max = hPTW_el->GetMaximum();
  if (hPTW_el_RLF->GetMaximum()>max) max = hPTW_el_RLF->GetMaximum();

  if (logy) {
    hPTW_mu->SetMaximum(max*10);
    hPTW_mu->SetMinimum(0.5);
  } else {
    hPTW_mu->SetMaximum(max*1.5);
    hPTW_mu->SetMinimum(0.);
  }
  hPTW_mu->Draw("hist");
  hPTW_mu_RLF->Draw("hist,same");
  hPTW_el->Draw("hist,same");
  hPTW_el_RLF->Draw("hist,same");
  c1->SetLogy(logy);

  hPTW_mu->GetXaxis()->SetLabelSize(0.03);
  hPTW_mu->GetXaxis()->SetTitleSize(0.03);
  hPTW_mu->GetXaxis()->SetTitleOffset(1.3);
  hPTW_mu->GetYaxis()->SetLabelSize(0.03);
  hPTW_mu->GetYaxis()->SetTitleSize(0.03);
  hPTW_mu->GetYaxis()->SetTitleOffset(1.3);
  c1->Modified();
  
  leg1->Draw();
  leg2->Draw();

  TLatex * latex = new TLatex();
  latex->SetNDC();
  latex->SetTextAlign(12);
  latex->SetTextFont(62);
  latex->SetTextSize(0.037);
  latex->DrawLatex(0.12, 0.87, "CMS Simulation");
  latex->SetTextSize(0.028);
  latex->DrawLatex(0.12, 0.83, "#sqrt{s} = 13 TeV, L = 10 fb^{-1}");

  c1->Print(plotdir+"closure/plots/reco_WpT.pdf");

  delete leg1;
  delete leg2;
  delete c1;
  delete latex;

}




