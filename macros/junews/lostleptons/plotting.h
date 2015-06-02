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
#include "TPaveText.h"
#include "macros/include/my_style.hpp"
//#include "macros/junews/lostleptons/RatioError.h"


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




