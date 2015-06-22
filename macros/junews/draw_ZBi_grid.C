
#include "TMath.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TH2Poly.h"
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
#include "TExec.h"
#include <vector>

#include "macros/junews/getBGPie.h"
#include "RooStats/NumberCountingUtils.h"

#include <iostream>

using namespace std;

void MakePlot(const TString treestring = "reduced_trees/13TeV/SMS-T1tttt_2J_mGl-1500_mLSP-100_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2377_v78/*.root",
	      const TString sampleTag="t1tttt", const double lumi=10000., TCut xtra_cuts="num_jets_pt30>=9&&num_csvm_jets30>=3")
{ 

  //  gROOT->SetStyle("CMS");
  //set_palette_style();
   set_deep_sea_palette();
  // TPaletteAxis *palette = new TPaletteAxis(6440,0,6800,2700,htemp);
  // palette->SetLabelColor(1);
  // palette->SetLabelFont(42);
  // palette->SetLabelOffset(0.005);
  // palette->SetLabelSize(0.035);
  // palette->SetTitleOffset(1);
  // palette->SetTitleSize(0.035);
  // Int_t ci;      // for color index setting
  // TColor *color; // for color definition with alpha
  // ci = TColor::GetColor("#00f5f6");
  // palette->SetFillColor(ci);
  // palette->SetFillStyle(1001);

  gStyle->SetOptStat(0);
  gStyle->SetMarkerSize(4);
  gStyle->SetTitleSize(0.045,"x");
  gStyle->SetTitleSize(0.045,"y");
  gStyle->SetPaintTextFormat("5.1f");
  TH1::StatOverflows(kTRUE);
  TH2::StatOverflows(kTRUE);
  TH1::SetDefaultSumw2(); //trick to turn on Sumw2 for all histos
  TH2::SetDefaultSumw2(); //trick to turn on Sumw2 for all histos


  TCut ht("ht30>500"), njets("num_jets_pt30>=4"), mht30("mht30>200"), mu_veto("num_veto_mus==0"), el_veto("num_veto_els==0"), mdp("min_delta_phi_met_N>6");
  TCut isoTk("num_iso_tracks_pt15_mT==0");
  TCut mutkveto("num_mu_tracks_mT==0"), eltkveto("num_el_tracks_mT==0"), hadtkveto("num_had_tracks_mT==0");
  TCut tk3veto(mutkveto+eltkveto+hadtkveto);
  TCut cleaning("passesBadJetFilter&&PBNRcode>0");
  TCut baseline(ht+njets+mht30+mu_veto+el_veto+tk3veto+mdp+cleaning);

  TCut nj1("num_jets_pt30>=4&&num_jets_pt30<=6"), nj2("num_jets_pt30>=7&&num_jets_pt30<=8"), nj3("num_jets_pt30>=9");
  TCut nb0("num_csvm_jets30==0"), nb1("num_csvm_jets30==1"), nb2("num_csvm_jets30==2"), nb3("num_csvm_jets30>=3");
  TCut mht1("mht30>200&&mht30<500"), mht2("mht30>500&&mht30<750"), mht3("mht30>750");
  TCut ht1("ht30>500&&ht30<800"), ht2("ht30>800&&ht30<1200"), ht3("ht30>1200");
  TCut box1(mht1&&ht1), box2(mht1&&ht2), box3(mht1&&ht3), box4(mht2&&(ht1||ht2)), box5(mht2||ht3), box6(mht3&&(ht2||ht3));
  
  TString weight = Form("weightppb*%f",lumi);
  

  TChain * sig = new TChain("reduced_tree");
  sig->Add(treestring);// treestring is passed as an argument
  vector<TH1D> sig_hists;
  sig_hists.push_back(TH1D("hsig1","",2,0,2));
  sig_hists.push_back(TH1D("hsig2","",2,0,2));
  sig_hists.push_back(TH1D("hsig3","",2,0,2));
  sig_hists.push_back(TH1D("hsig4","",2,0,2));
  sig_hists.push_back(TH1D("hsig5","",2,0,2));
  sig_hists.push_back(TH1D("hsig6","",2,0,2));
  TChain * bg = new TChain("reduced_tree");
  bg->Add("reduced_trees/13TeV/skimmed/*.root");
  vector<TH1D> bg_hists;
  bg_hists.push_back(TH1D("hbg1","",2,0,2));
  bg_hists.push_back(TH1D("hbg2","",2,0,2));
  bg_hists.push_back(TH1D("hbg3","",2,0,2));
  bg_hists.push_back(TH1D("hbg4","",2,0,2));
  bg_hists.push_back(TH1D("hbg5","",2,0,2));
  bg_hists.push_back(TH1D("hbg6","",2,0,2));
  
  vector<TCut> boxes;
  boxes.push_back(box1);
  boxes.push_back(box2);
  boxes.push_back(box3);
  boxes.push_back(box4);
  boxes.push_back(box5);
  boxes.push_back(box6);

  vector<TPie> bgPies;

  TH2Poly * Hbg = new TH2Poly("Hbg","BG Yields;H_{T} [GeV];H_{T}^{miss} [GeV]", 500, 1499, 200, 999);
  Hbg->AddBin(500,200,800,500);
  Hbg->AddBin(800,200,1200,500);
  Hbg->AddBin(1200,200,1499,500);
  Hbg->AddBin(500,500,1200,750);
  Hbg->AddBin(1200,500,1499,750);
  Hbg->AddBin(800,750,1499,999);
  TH2Poly * Hcomp = new TH2Poly("Hcomp","BG Composition;H_{T} [GeV];H_{T}^{miss} [GeV]", 500, 1499, 200, 999);
  Hcomp->AddBin(500,200,800,500);
  Hcomp->AddBin(800,200,1200,500);
  Hcomp->AddBin(1200,200,1499,500);
  Hcomp->AddBin(500,500,1200,750);
  Hcomp->AddBin(1200,500,1499,750);
  Hcomp->AddBin(800,750,1499,999);
  TH2Poly * Hsig = new TH2Poly("Hsig","Signal Yields;H_{T} [GeV];H_{T}^{miss} [GeV]", 500, 1499, 200, 999);
  Hsig->AddBin(500,200,800,500);
  Hsig->AddBin(800,200,1200,500);
  Hsig->AddBin(1200,200,1499,500);
  Hsig->AddBin(500,500,1200,750);
  Hsig->AddBin(1200,500,1499,750);
  Hsig->AddBin(800,750,1499,999);
  TH2Poly * HZBi = new TH2Poly("HZBi","Z_{Bi};H_{T} [GeV];H_{T}^{miss} [GeV]", 500, 1499, 200, 999);
  HZBi->AddBin(500,200,800,500);
  HZBi->AddBin(800,200,1200,500);
  HZBi->AddBin(1200,200,1499,500);
  HZBi->AddBin(500,500,1200,750);
  HZBi->AddBin(1200,500,1499,750);
  HZBi->AddBin(800,750,1499,999);
  for (unsigned int box(0); box<6; box++) {
    cout << "Box " << box+1 << "...";
    TString hsname = Form("hsig%d", box+1);
    TString hbname = Form("hbg%d", box+1);
    TString piename = Form("pie%d", box+1);
    TCut selection = (baseline+xtra_cuts+boxes[box])*(weight);
    // cout << selection.GetTitle() << endl;
    sig->Project(hsname, "1", selection);
    double sExp = sig_hists[box].Integral(1,3);
    Hsig->SetBinContent(box+1, sExp);
    bg->Project(hbname, "1", selection);
    double bExp = bg_hists[box].Integral(1,3);
    Hbg->SetBinContent(box+1, bExp);
    HZBi->SetBinContent(box+1, RooStats::NumberCountingUtils::BinomialWithTauExpZ(sExp, bExp, 1.));
    if (box<3) {
      bgPies.push_back(getBGPie(selection, piename));
    }
    else bgPies.push_back(getBGPie(selection, piename, 0.28));
    cout << "DONE!" << endl;
  }
  //hZBi->Print("all");


  

  TCanvas * thecanvas= new TCanvas("thecanvas","the canvas",1800, 1200);
  thecanvas->SetFillStyle(4000);
  thecanvas->Divide(2,2);
  thecanvas->cd(1);
  gPad->SetFillStyle(4000);
  double max = Hbg->GetMaximum()*5;
  if (Hsig->GetMaximum()*1.1>max) max = Hsig->GetMaximum()*1.75;
  Hbg->SetMaximum(max);
  Hbg->SetMinimum(0.1);
  gPad->SetLogz();
  Hbg->Draw("colz,text");
  
  thecanvas->cd(2);
  gPad->SetFillStyle(4000);
  Hcomp->Draw();
  TH1D * httbar = new TH1D("ttbar" , "", 1, 0, 1);
  TH1D * hqcd = new TH1D("qcd" , "", 1, 0, 1);
  TH1D * hznn = new TH1D("znn" , "", 1, 0, 1);
  TH1D * hwjets = new TH1D("wjets" , "", 1, 0, 1);
  TH1D * hother = new TH1D("other" , "", 1, 0, 1);
  TH1D * hsingle_top = new TH1D("single_top" , "", 1, 0, 1);
  set_style(httbar, "ttbar");
  set_style(hqcd, "qcd");
  set_style(hznn, "znn");
  set_style(hwjets, "wjets");
  set_style(hother, "other");
  set_style(hsingle_top, "single_top");
  TLegend * leg1 = new TLegend(0.12, 0.67, 0.23, 0.88);
  set_style(leg1,0.035);
  leg1->AddEntry(httbar, "t#bar{t}", "f");
  leg1->AddEntry(hqcd, "QCD", "f");
  leg1->AddEntry(hznn, "Z+jets", "f");
  TLegend * leg2 = new TLegend(0.23, 0.67, 0.34, 0.88);
  set_style(leg2,0.035);
  leg2->AddEntry(hwjets, "W+jets", "f");
  leg2->AddEntry(hsingle_top, "Single-t", "f");
  leg2->AddEntry(hother, "Other", "f");
  leg1->Draw();
  leg2->Draw();
  vector<TPad*> pads;
  TPad* pad1 = new TPad("pad1","",0.08,0.06,0.36,0.46);
  TPad* pad2 = new TPad("pad2","",0.36,0.06,0.64,0.46);
  TPad* pad3 = new TPad("pad3","",0.64,0.06,0.92,0.46);
  TPad* pad4 = new TPad("pad4","",0.25,0.325,0.53,0.725);
  TPad* pad5 = new TPad("pad5","",0.64,0.325,0.92,0.725);
  TPad* pad6 = new TPad("pad6","",0.47,0.575,0.75,0.975);
  pads.push_back(pad1);
  pads.push_back(pad2);
  pads.push_back(pad3);
  pads.push_back(pad4);
  pads.push_back(pad5);
  pads.push_back(pad6);
  for (unsigned int box(0); box<6; box++) {
    thecanvas->cd(2);
    pads[box]->SetFillStyle(4000);
    pads[box]->Draw();  
    pads[box]->cd();
    bgPies[box].SetLabelFormat("");
    bgPies[box].Draw();
  }
   
  thecanvas->cd(3);
  gPad->SetFillStyle(4000);
  gPad->SetLogz();
  Hsig->SetMaximum(max);
  Hsig->SetMinimum(0.1);
  Hsig->Draw("colz,text");

  thecanvas->cd(4);
  gPad->SetFillStyle(4000);
  HZBi->SetMaximum(3.5);
  HZBi->SetMinimum(0);
  TExec *ex1 = new TExec("ex1","gStyle->SetPalette(56);");
  HZBi->Draw("col");
  ex1->Draw();
  HZBi->Draw("colz,text,same");

  thecanvas->Print("macros/junews/plots/ZBi/"+sampleTag+"_ZBi_bins_616.pdf");

    
  delete sig;
  delete bg;
  delete Hbg;
  delete Hcomp;
  delete Hsig;
  delete HZBi;
  delete thecanvas;
  delete httbar;
  delete hqcd;
  delete hznn;
  delete hwjets;
  delete hsingle_top;
  delete hother;

}


void draw_ZBi_grid() {

 MakePlot("reduced_trees/13TeV/SMS-T2bb_2J_mStop-600_mLSP-580_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2423_v78/*.root", "t2bb_600_580_4to6j_1b", 10000, "num_jets_pt30>=4&&num_jets_pt30<=6&&num_csvm_jets30==1");
  MakePlot("reduced_trees/13TeV/SMS-T2bb_2J_mStop-600_mLSP-580_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2423_v78/*.root", "t2bb_600_580_7to8j_1b", 10000, "num_jets_pt30>=7&&num_jets_pt30<=8&&num_csvm_jets30==1");
  MakePlot("reduced_trees/13TeV/SMS-T2bb_2J_mStop-600_mLSP-580_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2423_v78/*.root", "t2bb_600_580_9j_1b", 10000, "num_jets_pt30>=9&&num_csvm_jets30==1");
  MakePlot("reduced_trees/13TeV/SMS-T2bb_2J_mStop-600_mLSP-580_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2423_v78/*.root", "t2bb_600_580_4to6j_2b", 10000, "num_jets_pt30>=4&&num_jets_pt30<=6&&num_csvm_jets30==2");
  MakePlot("reduced_trees/13TeV/SMS-T2bb_2J_mStop-600_mLSP-580_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2423_v78/*.root", "t2bb_600_580_7to8j_2b", 10000, "num_jets_pt30>=7&&num_jets_pt30<=8&&num_csvm_jets30==2");
  MakePlot("reduced_trees/13TeV/SMS-T2bb_2J_mStop-600_mLSP-580_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2423_v78/*.root", "t2bb_600_580_9j_2b", 10000, "num_jets_pt30>=9&&num_csvm_jets30==2");

  MakePlot("reduced_trees/13TeV/SMS-T2bb_2J_mStop-900_mLSP-100_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2424_v78/*.root", "t2bb_900_100_4to6j_1b", 10000, "num_jets_pt30>=4&&num_jets_pt30<=6&&num_csvm_jets30==1");
  MakePlot("reduced_trees/13TeV/SMS-T2bb_2J_mStop-900_mLSP-100_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2424_v78/*.root", "t2bb_900_100_7to8j_1b", 10000, "num_jets_pt30>=7&&num_jets_pt30<=8&&num_csvm_jets30==1");
  MakePlot("reduced_trees/13TeV/SMS-T2bb_2J_mStop-900_mLSP-100_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2424_v78/*.root", "t2bb_900_100_9j_1b", 10000, "num_jets_pt30>=9&&num_csvm_jets30==1");
  MakePlot("reduced_trees/13TeV/SMS-T2bb_2J_mStop-900_mLSP-100_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2424_v78/*.root", "t2bb_900_100_4to6j_2b", 10000, "num_jets_pt30>=4&&num_jets_pt30<=6&&num_csvm_jets30==2");
  MakePlot("reduced_trees/13TeV/SMS-T2bb_2J_mStop-900_mLSP-100_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2424_v78/*.root", "t2bb_900_100_7to8j_2b", 10000, "num_jets_pt30>=7&&num_jets_pt30<=8&&num_csvm_jets30==2");
  MakePlot("reduced_trees/13TeV/SMS-T2bb_2J_mStop-900_mLSP-100_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2424_v78/*.root", "t2bb_900_100_9j_2b", 10000, "num_jets_pt30>=9&&num_csvm_jets30==2");
  
  MakePlot("reduced_trees/13TeV/SMS-T2qq_2J_mStop-1200_mLSP-100_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2421_v78/*.root", "t2qq_1200_100_4to6j_0b", 10000, "num_jets_pt30>=4&&num_jets_pt30<=6&&num_csvm_jets30==0");
  MakePlot("reduced_trees/13TeV/SMS-T2qq_2J_mStop-1200_mLSP-100_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2421_v78/*.root", "t2qq_1200_100_4to6j_1b", 10000, "num_jets_pt30>=4&&num_jets_pt30<=6&&num_csvm_jets30==1");
  MakePlot("reduced_trees/13TeV/SMS-T2qq_2J_mStop-1200_mLSP-100_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2421_v78/*.root", "t2qq_1200_100_7to8j_0b", 10000, "num_jets_pt30>=7&&num_jets_pt30<=8&&num_csvm_jets30==0");
  MakePlot("reduced_trees/13TeV/SMS-T2qq_2J_mStop-1200_mLSP-100_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2421_v78/*.root", "t2qq_1200_100_7to8j_1b", 10000, "num_jets_pt30>=7&&num_jets_pt30<=8&&num_csvm_jets30==1");

  MakePlot("reduced_trees/13TeV/SMS-T2qq_2J_mStop-600_mLSP-550_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2422_v78/*.root", "t2qq_600_550_4to6j_0b", 10000, "num_jets_pt30>=4&&num_jets_pt30<=6&&num_csvm_jets30==0");
  MakePlot("reduced_trees/13TeV/SMS-T2qq_2J_mStop-600_mLSP-550_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2422_v78/*.root", "t2qq_600_550_4to6j_1b", 10000, "num_jets_pt30>=4&&num_jets_pt30<=6&&num_csvm_jets30==1");
  MakePlot("reduced_trees/13TeV/SMS-T2qq_2J_mStop-600_mLSP-550_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2422_v78/*.root", "t2qq_600_550_7to8j_0b", 10000, "num_jets_pt30>=7&&num_jets_pt30<=8&&num_csvm_jets30==0");
  MakePlot("reduced_trees/13TeV/SMS-T2qq_2J_mStop-600_mLSP-550_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2422_v78/*.root", "t2qq_600_550_7to8j_1b", 10000, "num_jets_pt30>=7&&num_jets_pt30<=8&&num_csvm_jets30==1");
  
 MakePlot("reduced_trees/13TeV/SMS-T2tt_2J_mStop-425_mLSP-325_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2427_v78/*.root", "t2tt_425_325_4to6j_1b", 10000, "num_jets_pt30>=4&&num_jets_pt30<=6&&num_csvm_jets30==1");
  MakePlot("reduced_trees/13TeV/SMS-T2tt_2J_mStop-425_mLSP-325_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2427_v78/*.root", "t2tt_425_325_7to8j_1b", 10000, "num_jets_pt30>=7&&num_jets_pt30<=8&&num_csvm_jets30==1");
  MakePlot("reduced_trees/13TeV/SMS-T2tt_2J_mStop-425_mLSP-325_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2427_v78/*.root", "t2tt_425_325_9j_1b", 10000, "num_jets_pt30>=9&&num_csvm_jets30==1");
  MakePlot("reduced_trees/13TeV/SMS-T2tt_2J_mStop-425_mLSP-325_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2427_v78/*.root", "t2tt_425_325_4to6j_2b", 10000, "num_jets_pt30>=4&&num_jets_pt30<=6&&num_csvm_jets30==2");
  MakePlot("reduced_trees/13TeV/SMS-T2tt_2J_mStop-425_mLSP-325_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2427_v78/*.root", "t2tt_425_325_7to8j_2b", 10000, "num_jets_pt30>=7&&num_jets_pt30<=8&&num_csvm_jets30==2");
  MakePlot("reduced_trees/13TeV/SMS-T2tt_2J_mStop-425_mLSP-325_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2427_v78/*.root", "t2tt_425_325_9j_2b", 10000, "num_jets_pt30>=9&&num_csvm_jets30==2");
  MakePlot("reduced_trees/13TeV/SMS-T2tt_2J_mStop-425_mLSP-325_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2427_v78/*.root", "t2tt_425_325_4to6j_3b", 10000, "num_jets_pt30>=4&&num_jets_pt30<=6&&num_csvm_jets30>=3");
  MakePlot("reduced_trees/13TeV/SMS-T2tt_2J_mStop-425_mLSP-325_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2427_v78/*.root", "t2tt_425_325_7to8j_3b", 10000, "num_jets_pt30>=7&&num_jets_pt30<=8&&num_csvm_jets30>=3");
  MakePlot("reduced_trees/13TeV/SMS-T2tt_2J_mStop-425_mLSP-325_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2427_v78/*.root", "t2tt_425_325_9j_3b", 10000, "num_jets_pt30>=9&&num_csvm_jets30>=3");

  
  MakePlot("reduced_trees/13TeV/SMS-T2tt_2J_mStop-500_mLSP-325_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2426_v78/*.root", "t2tt_500_325_4to6j_1b", 10000, "num_jets_pt30>=4&&num_jets_pt30<=6&&num_csvm_jets30==1");
  MakePlot("reduced_trees/13TeV/SMS-T2tt_2J_mStop-500_mLSP-325_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2426_v78/*.root", "t2tt_500_325_7to8j_1b", 10000, "num_jets_pt30>=7&&num_jets_pt30<=8&&num_csvm_jets30==1");
  MakePlot("reduced_trees/13TeV/SMS-T2tt_2J_mStop-500_mLSP-325_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2426_v78/*.root", "t2tt_500_325_9j_1b", 10000, "num_jets_pt30>=9&&num_csvm_jets30==1");
  MakePlot("reduced_trees/13TeV/SMS-T2tt_2J_mStop-500_mLSP-325_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2426_v78/*.root", "t2tt_500_325_4to6j_2b", 10000, "num_jets_pt30>=4&&num_jets_pt30<=6&&num_csvm_jets30==2");
  MakePlot("reduced_trees/13TeV/SMS-T2tt_2J_mStop-500_mLSP-325_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2426_v78/*.root", "t2tt_500_325_7to8j_2b", 10000, "num_jets_pt30>=7&&num_jets_pt30<=8&&num_csvm_jets30==2");
  MakePlot("reduced_trees/13TeV/SMS-T2tt_2J_mStop-500_mLSP-325_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2426_v78/*.root", "t2tt_500_325_9j_2b", 10000, "num_jets_pt30>=9&&num_csvm_jets30==2");
  MakePlot("reduced_trees/13TeV/SMS-T2tt_2J_mStop-500_mLSP-325_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2426_v78/*.root", "t2tt_500_325_4to6j_3b", 10000, "num_jets_pt30>=4&&num_jets_pt30<=6&&num_csvm_jets30>=3");
  MakePlot("reduced_trees/13TeV/SMS-T2tt_2J_mStop-500_mLSP-325_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2426_v78/*.root", "t2tt_500_325_7to8j_3b", 10000, "num_jets_pt30>=7&&num_jets_pt30<=8&&num_csvm_jets30>=3");
  MakePlot("reduced_trees/13TeV/SMS-T2tt_2J_mStop-500_mLSP-325_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2426_v78/*.root", "t2tt_500_325_9j_3b", 10000, "num_jets_pt30>=9&&num_csvm_jets30>=3");
  
  MakePlot("reduced_trees/13TeV/SMS-T2tt_2J_mStop-650_mLSP-325_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2425_v78/*.root", "t2tt_650_325_4to6j_1b", 10000, "num_jets_pt30>=4&&num_jets_pt30<=6&&num_csvm_jets30==1");
  MakePlot("reduced_trees/13TeV/SMS-T2tt_2J_mStop-650_mLSP-325_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2425_v78/*.root", "t2tt_650_325_7to8j_1b", 10000, "num_jets_pt30>=7&&num_jets_pt30<=8&&num_csvm_jets30==1");
  MakePlot("reduced_trees/13TeV/SMS-T2tt_2J_mStop-650_mLSP-325_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2425_v78/*.root", "t2tt_650_325_9j_1b", 10000, "num_jets_pt30>=9&&num_csvm_jets30==1");
  MakePlot("reduced_trees/13TeV/SMS-T2tt_2J_mStop-650_mLSP-325_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2425_v78/*.root", "t2tt_650_325_4to6j_2b", 10000, "num_jets_pt30>=4&&num_jets_pt30<=6&&num_csvm_jets30==2");
  MakePlot("reduced_trees/13TeV/SMS-T2tt_2J_mStop-650_mLSP-325_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2425_v78/*.root", "t2tt_650_325_7to8j_2b", 10000, "num_jets_pt30>=7&&num_jets_pt30<=8&&num_csvm_jets30==2");
  MakePlot("reduced_trees/13TeV/SMS-T2tt_2J_mStop-650_mLSP-325_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2425_v78/*.root", "t2tt_650_325_9j_2b", 10000, "num_jets_pt30>=9&&num_csvm_jets30==2");
  MakePlot("reduced_trees/13TeV/SMS-T2tt_2J_mStop-650_mLSP-325_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2425_v78/*.root", "t2tt_650_325_4to6j_3b", 10000, "num_jets_pt30>=4&&num_jets_pt30<=6&&num_csvm_jets30>=3");
  MakePlot("reduced_trees/13TeV/SMS-T2tt_2J_mStop-650_mLSP-325_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2425_v78/*.root", "t2tt_650_325_7to8j_3b", 10000, "num_jets_pt30>=7&&num_jets_pt30<=8&&num_csvm_jets30>=3");
  MakePlot("reduced_trees/13TeV/SMS-T2tt_2J_mStop-650_mLSP-325_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2425_v78/*.root", "t2tt_650_325_9j_3b", 10000, "num_jets_pt30>=9&&num_csvm_jets30>=3");
  
    MakePlot("reduced_trees/13TeV/SMS-T2tt_2J_mStop-850_mLSP-100_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2420_v78/*.root", "t2tt_850_100_4to6j_1b", 10000, "num_jets_pt30>=4&&num_jets_pt30<=6&&num_csvm_jets30==1");
  MakePlot("reduced_trees/13TeV/SMS-T2tt_2J_mStop-850_mLSP-100_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2420_v78/*.root", "t2tt_850_100_7to8j_1b", 10000, "num_jets_pt30>=7&&num_jets_pt30<=8&&num_csvm_jets30==1");
  MakePlot("reduced_trees/13TeV/SMS-T2tt_2J_mStop-850_mLSP-100_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2420_v78/*.root", "t2tt_850_100_9j_1b", 10000, "num_jets_pt30>=9&&num_csvm_jets30==1");
  MakePlot("reduced_trees/13TeV/SMS-T2tt_2J_mStop-850_mLSP-100_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2420_v78/*.root", "t2tt_850_100_4to6j_2b", 10000, "num_jets_pt30>=4&&num_jets_pt30<=6&&num_csvm_jets30==2");
  MakePlot("reduced_trees/13TeV/SMS-T2tt_2J_mStop-850_mLSP-100_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2420_v78/*.root", "t2tt_850_100_7to8j_2b", 10000, "num_jets_pt30>=7&&num_jets_pt30<=8&&num_csvm_jets30==2");
  MakePlot("reduced_trees/13TeV/SMS-T2tt_2J_mStop-850_mLSP-100_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2420_v78/*.root", "t2tt_850_100_9j_2b", 10000, "num_jets_pt30>=9&&num_csvm_jets30==2");
  MakePlot("reduced_trees/13TeV/SMS-T2tt_2J_mStop-850_mLSP-100_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2420_v78/*.root", "t2tt_850_100_4to6j_3b", 10000, "num_jets_pt30>=4&&num_jets_pt30<=6&&num_csvm_jets30>=3");
  MakePlot("reduced_trees/13TeV/SMS-T2tt_2J_mStop-850_mLSP-100_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2420_v78/*.root", "t2tt_850_100_7to8j_3b", 10000, "num_jets_pt30>=7&&num_jets_pt30<=8&&num_csvm_jets30>=3");
  MakePlot("reduced_trees/13TeV/SMS-T2tt_2J_mStop-850_mLSP-100_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2420_v78/*.root", "t2tt_850_100_9j_3b", 10000, "num_jets_pt30>=9&&num_csvm_jets30>=3");
    
  MakePlot("reduced_trees/13TeV/SMS-T1tttt_2J_mGl-1500_mLSP-100_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2377_v78/*.root", "t1tttt_1500_100_7to8j_3b", 10000, "num_jets_pt30>=6&&num_jets_pt30<=8&&num_csvm_jets30>=3");
  MakePlot("reduced_trees/13TeV/SMS-T1tttt_2J_mGl-1500_mLSP-100_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2377_v78/*.root", "t1tttt_1500_100_9j_2b", 10000, "num_jets_pt30>=9&&num_csvm_jets30==2");
  MakePlot("reduced_trees/13TeV/SMS-T1tttt_2J_mGl-1500_mLSP-100_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2377_v78/*.root", "t1tttt_1500_100_9j_3b", 10000, "num_jets_pt30>=9&&num_csvm_jets30>=3");
  MakePlot("reduced_trees/13TeV/SMS-T1tttt_2J_mGl-1500_mLSP-100_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2377_v78/*.root", "t1tttt_1500_100_7j_2b", 10000, "num_jets_pt30>=6&&num_csvm_jets30>=2");
  MakePlot("reduced_trees/13TeV/SMS-T1tttt_2J_mGl-1500_mLSP-100_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2377_v78/*.root", "t1tttt_1500_100_7j_3b", 10000, "num_jets_pt30>=6&&num_csvm_jets30>=3");
  MakePlot("reduced_trees/13TeV/SMS-T1tttt_2J_mGl-1500_mLSP-100_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2377_v78/*.root", "t1tttt_1500_100_9j_2b", 10000, "num_jets_pt30>=9&&num_csvm_jets30>=2");
  MakePlot("reduced_trees/13TeV/SMS-T1tttt_2J_mGl-1200_mLSP-800_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2363_v78/*.root", "t1tttt_1200_800_7to8j_2b", 10000, "num_jets_pt30>=6&&num_jets_pt30<=8&&num_csvm_jets30==2");
  MakePlot("reduced_trees/13TeV/SMS-T1tttt_2J_mGl-1200_mLSP-800_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2363_v78/*.root", "t1tttt_1200_800_7to8j_3b", 10000, "num_jets_pt30>=6&&num_jets_pt30<=8&&num_csvm_jets30>=3");
  MakePlot("reduced_trees/13TeV/SMS-T1tttt_2J_mGl-1200_mLSP-800_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2363_v78/*.root", "t1tttt_1200_800_9j_2b", 10000, "num_jets_pt30>=9&&num_csvm_jets30==2");
  MakePlot("reduced_trees/13TeV/SMS-T1tttt_2J_mGl-1200_mLSP-800_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2363_v78/*.root", "t1tttt_1200_800_9j_3b", 10000, "num_jets_pt30>=9&&num_csvm_jets30>=3");
  MakePlot("reduced_trees/13TeV/SMS-T1tttt_2J_mGl-1200_mLSP-800_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2363_v78/*.root", "t1tttt_1200_800_6pj_2pb", 10000, "num_jets_pt30>=6&&num_csvm_jets30>=2");
  MakePlot("reduced_trees/13TeV/SMS-T1tttt_2J_mGl-1200_mLSP-800_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2363_v78/*.root", "t1tttt_1200_800_6pj_3b", 10000, "num_jets_pt30>=6&&num_csvm_jets30>=3");
  MakePlot("reduced_trees/13TeV/SMS-T1tttt_2J_mGl-1200_mLSP-800_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2363_v78/*.root", "t1tttt_1200_800_9j_2pb", 10000, "num_jets_pt30>=9&&num_csvm_jets30>=2");
  MakePlot("reduced_trees/13TeV/SMS-T1bbbb_2J_mGl-1500_mLSP-100_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2364_v78/*.root", "t1bbbb_1500_100_4to6j_2b", 10000, "num_jets_pt30>=4&&num_jets_pt30<=6&&num_csvm_jets30==2");
  MakePlot("reduced_trees/13TeV/SMS-T1bbbb_2J_mGl-1500_mLSP-100_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2364_v78/*.root", "t1bbbb_1500_100_4to6j_3b", 10000, "num_jets_pt30>=4&&num_jets_pt30<=6&&num_csvm_jets30==3");
  MakePlot("reduced_trees/13TeV/SMS-T1bbbb_2J_mGl-1500_mLSP-100_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2364_v78/*.root", "t1bbbb_1500_100_7to8j_2b", 10000, "num_jets_pt30>=7&&num_jets_pt30<=8&&num_csvm_jets30==2");
  MakePlot("reduced_trees/13TeV/SMS-T1bbbb_2J_mGl-1500_mLSP-100_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2364_v78/*.root", "t1bbbb_1500_100_7to8j_3b", 10000, "num_jets_pt30>=7&&num_jets_pt30<=8&&num_csvm_jets30==3");
  MakePlot("reduced_trees/13TeV/SMS-T1bbbb_2J_mGl-1000_mLSP-900_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2365_v78/*.root", "t1bbbb_1000_900_4to6j_2b", 10000, "num_jets_pt30>=4&&num_jets_pt30<=6&&num_csvm_jets30==2");
  MakePlot("reduced_trees/13TeV/SMS-T1bbbb_2J_mGl-1000_mLSP-900_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2365_v78/*.root", "t1bbbb_1000_900_4to6j_3b", 10000, "num_jets_pt30>=4&&num_jets_pt30<=6&&num_csvm_jets30==3");
  MakePlot("reduced_trees/13TeV/SMS-T1bbbb_2J_mGl-1000_mLSP-900_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2365_v78/*.root", "t1bbbb_1000_900_7to8j_2b", 10000, "num_jets_pt30>=7&&num_jets_pt30<=8&&num_csvm_jets30==2");
  MakePlot("reduced_trees/13TeV/SMS-T1bbbb_2J_mGl-1000_mLSP-900_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2365_v78/*.root", "t1bbbb_1000_900_7to8j_3b", 10000, "num_jets_pt30>=7&&num_jets_pt30<=8&&num_csvm_jets30==3");
  MakePlot("reduced_trees/13TeV/SMS-T1qqqq_2J_mGl-1400_mLSP-100_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2366_v78/*.root", "t1qqqq_1400_100_4to6j_0b", 10000, "num_jets_pt30>=4&&num_jets_pt30<=6&&num_csvm_jets30==0");
  MakePlot("reduced_trees/13TeV/SMS-T1qqqq_2J_mGl-1400_mLSP-100_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2366_v78/*.root", "t1qqqq_1400_100_4to6j_1b", 10000, "num_jets_pt30>=4&&num_jets_pt30<=6&&num_csvm_jets30==1");
  MakePlot("reduced_trees/13TeV/SMS-T1qqqq_2J_mGl-1400_mLSP-100_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2366_v78/*.root", "t1qqqq_1400_100_7to8j_0b", 10000, "num_jets_pt30>=7&&num_jets_pt30<=8&&num_csvm_jets30==0");
  MakePlot("reduced_trees/13TeV/SMS-T1qqqq_2J_mGl-1400_mLSP-100_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2366_v78/*.root", "t1qqqq_1400_100_7to8j_1b", 10000, "num_jets_pt30>=7&&num_jets_pt30<=8&&num_csvm_jets30==1");
  MakePlot("reduced_trees/13TeV/SMS-T1qqqq_2J_mGl-1400_mLSP-100_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2366_v78/*.root", "t1qqqq_1400_100_9j_0to1b", 10000, "num_jets_pt30>=9&&num_csvm_jets30<=1");
  MakePlot("reduced_trees/13TeV/SMS-T1qqqq_2J_mGl-1000_mLSP-800_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2367_v78/*.root", "t1qqqq_1000_800_4to6j_0b", 10000, "num_jets_pt30>=4&&num_jets_pt30<=6&&num_csvm_jets30==0");
  MakePlot("reduced_trees/13TeV/SMS-T1qqqq_2J_mGl-1000_mLSP-800_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2367_v78/*.root", "t1qqqq_1000_800_4to6j_1b", 10000, "num_jets_pt30>=4&&num_jets_pt30<=6&&num_csvm_jets30==1");
  MakePlot("reduced_trees/13TeV/SMS-T1qqqq_2J_mGl-1000_mLSP-800_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2367_v78/*.root", "t1qqqq_1000_800_7to8j_0b", 10000, "num_jets_pt30>=7&&num_jets_pt30<=8&&num_csvm_jets30==0");
  MakePlot("reduced_trees/13TeV/SMS-T1qqqq_2J_mGl-1000_mLSP-800_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2367_v78/*.root", "t1qqqq_1000_800_7to8j_1b", 10000, "num_jets_pt30>=7&&num_jets_pt30<=8&&num_csvm_jets30==1");
  MakePlot("reduced_trees/13TeV/SMS-T1qqqq_2J_mGl-1000_mLSP-800_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM_UCSB2367_v78/*.root", "t1qqqq_1000_800_9j_0to1b", 10000, "num_jets_pt30>=9&&num_csvm_jets30<=1");
 


  return;

}
