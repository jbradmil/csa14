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
#include "TCollection.h"
#include "TList.h"
#include "TGraphAsymmErrors.h"
#include "TEfficiency.h"
#include "TFile.h"

// #include "macros/include/cms_style.h"

#include <iostream>

using namespace std;

const unsigned int nSamples(14);

TString qcd_files[nSamples] = 
  {
    "reduced_trees/QCD_Pt-30to50_Tune4C_13TeV_pythia8_Spring14miniaod-castor_PU20bx25_POSTLS170_V5-v1_MINIAODSIM_UCSB2126_v75.list.root",
    "reduced_trees/QCD_Pt-50to80_Tune4C_13TeV_pythia8_Spring14miniaod-castor_PU20bx25_POSTLS170_V5-v1_MINIAODSIM_UCSB2127_v75.list.root",
    "reduced_trees/QCD_Pt-80to120_Tune4C_13TeV_pythia8_Spring14miniaod-castor_PU20bx25_POSTLS170_V5-v1_MINIAODSIM_UCSB2128_v75.list.root",
    "reduced_trees/QCD_Pt-120to170_Tune4C_13TeV_pythia8_Spring14miniaod-castor_PU20bx25_POSTLS170_V5-v1_MINIAODSIM_UCSB2109_v75.list.root",
    "reduced_trees/QCD_Pt-1000to1400_Tune4C_13TeV_pythia8_Spring14miniaod-castor_PU20bx25_POSTLS170_V5-v1_MINIAODSIM_UCSB2118_v75.list.root",
    "reduced_trees/QCD_Pt-170to300_Tune4C_13TeV_pythia8_Spring14miniaod-castor_PU20bx25_POSTLS170_V5-v1_MINIAODSIM_UCSB2098_v75.list.root",
    "reduced_trees/QCD_Pt-300to470_Tune4C_13TeV_pythia8_Spring14miniaod-castor_PU20bx25_POSTLS170_V5-v1_MINIAODSIM_UCSB2135_v75.list.root",
    "reduced_trees/QCD_Pt-470to600_Tune4C_13TeV_pythia8_Spring14miniaod-castor_PU20bx25_POSTLS170_V5-v1_MINIAODSIM_UCSB2137_v75.list.root",
    "reduced_trees/QCD_Pt-600to800_Tune4C_13TeV_pythia8_Spring14miniaod-castor_PU20bx25_POSTLS170_V5-v1_MINIAODSIM_UCSB2138_v75.list.root",
    "reduced_trees/QCD_Pt-800to1000_Tune4C_13TeV_pythia8_Spring14miniaod-castor_PU20bx25_POSTLS170_V5-v1_MINIAODSIM_UCSB2130_v75.list.root",
    "reduced_trees/QCD_Pt-1400to1800_Tune4C_13TeV_pythia8_Spring14miniaod-castor_PU20bx25_POSTLS170_V5-v1_MINIAODSIM_UCSB2101_v75.list.root",
    "reduced_trees/QCD_Pt-1800to2400_Tune4C_13TeV_pythia8_Spring14miniaod-PU20bx25_POSTLS170_V5-v1_MINIAODSIM_UCSB2100_v75.list.root",
    "reduced_trees/QCD_Pt-2400to3200_Tune4C_13TeV_pythia8_Spring14miniaod-PU20bx25_POSTLS170_V5-v1_MINIAODSIM_UCSB2099_v75.list.root",			
    "reduced_trees/QCD_Pt-3200_Tune4C_13TeV_pythia8_Spring14miniaod-PU20bx25_POSTLS170_V5-v1_MINIAODSIM_UCSB2136_v75.list.root"
  };

Double_t weights[nSamples] = 
  { 
    161500000./4848856.,
    22110000./4726495.,
    3000114./4794295.,
    493200./3562052.,
    120300./2793204.,
    7475./2700637.,
    587.1/2907137.,
    167/2853619.,
    28.25/2875729.,
    8.195/2834231.,
    0.7346/2931414.,
    0.102/2953210.,
    0.0064/2939491.,
    0.000163/2953151.
  };

TGraphAsymmErrors* GetCombinedEfficiency(const TString cut = "min_delta_phi_met_N>4.", const TString preselection="")
{ 
  
  TH1::SetDefaultSumw2();

  const int nbins=16;
  const double varbins[nbins+1]={0,20,40,60,80,100,125,150,175,200,250,300,400,500,600,800,1000};


    
  TCut all(preselection);
  TCut ccut(cut);
  TCut pass(all+ccut);


  TFile *fout = new TFile("macros/qcd_control/output.root", "recreate");
  fout->cd();

  cout << "Denom. cuts: " << (string)all << endl;
  cout << "Num. cuts: " << (string)pass << endl;

  TList* pList=new TList();
  vector<TH1D> v_all, v_pass;

  for (unsigned int sample(0); sample<nSamples; sample++) {
      
    TChain * ch = new TChain();
    ch->Add(qcd_files[sample]+"/reduced_tree");


    TH1D* hMETall = new TH1D(Form("hMETall_%d",sample),"hMETall",nbins,varbins);
    TH1D* hMETpass = new TH1D(Form("hMETpass_%d",sample),"hMETpass",nbins,varbins);

    ch->Project(hMETall->GetName(),"met",all);
    ch->Project(hMETpass->GetName(),"met",pass);

    Double_t e_overflow(0.), i_overflow(0.);
    i_overflow=hMETall->IntegralAndError(nbins,nbins+1,e_overflow);
    hMETall->SetBinContent(nbins, i_overflow);
    hMETall->SetBinError(nbins, e_overflow);
    i_overflow=hMETpass->IntegralAndError(nbins,nbins+1,e_overflow);
    hMETpass->SetBinContent(nbins, i_overflow);
    hMETpass->SetBinError(nbins, e_overflow);

    printf("Sample %d: hMETpass/hMETall=%.1f/%.1f\n",sample, hMETpass->Integral(),hMETall->Integral());

      
    delete ch;

    v_all.push_back(*hMETall);
    v_pass.push_back(*hMETpass);

    delete hMETall;
    delete hMETpass;

  }

  for (unsigned int sample(0); sample<nSamples; sample++) {
    cout << "Found hist " << v_all[sample].GetName() << endl;
    v_all[sample].Write();
    v_pass[sample].Write();
    TEfficiency* pEff = new TEfficiency(v_pass[sample],v_all[sample]);
    pList->Add(pEff);
  }

  TGraphAsymmErrors* gr = TEfficiency::Combine(pList,"v,mode",nSamples,weights);
  return gr;
}
  
void mdpRatios() {

  TGraphAsymmErrors* mdpN = GetCombinedEfficiency("min_delta_phi_met_N>4.","ht50>400&&jet2_pt>70&&jet3_pt>50&&num_reco_veto_electrons==0&&num_reco_veto_muons==0&&num_csvm_jets>0");
  TGraphAsymmErrors* mdp = GetCombinedEfficiency("min_delta_phi_met>0.3","ht50>400&&jet2_pt>70&&jet3_pt>50&&num_reco_veto_electrons==0&&num_reco_veto_muons==0&&num_csvm_jets>0");

  mdpN->SetLineColor(46);
  mdp->SetLineColor(38);
  mdpN->SetMarkerColor(46);
  mdp->SetMarkerColor(38);
  mdpN->SetMarkerStyle(21);
  mdp->SetMarkerStyle(22);
  mdpN->SetLineWidth(2);
  mdp->SetLineWidth(2);


  TCanvas * thecanvas= new TCanvas("thecanvas","the canvas",800,600);
  
  mdp->Draw("AP");
  mdpN->Draw("P");

  mdp->SetTitle("Efficiency of #Delta#phi cuts as a function of E_{T}^{miss};E_{T}^{miss} [GeV];Cut efficiency");
  mdp->GetYaxis()->SetLabelSize(0.028);

  cout << "Dump efficiencies for min_delta_phi_met_N: " << endl;
  mdpN->Print();
  cout << "Dump efficiencies for min_delta_phi_met: " << endl;
  mdp->Print();

  TLegend * leg = new TLegend(0.65,0.75,0.8,0.9);
  leg->SetFillColor(0);
  leg->SetBorderSize(0);
  leg->SetLineStyle(0);
  leg->SetFillStyle(0);
  leg->SetTextFont(132);
  leg->SetTextSize(.04);
  leg->AddEntry(mdpN,"#Delta#hat{#phi}_{N}^{min}>4.","lp");
  leg->AddEntry(mdp,"#Delta#phi_{min}>0.3","lp");

  leg->Draw();

  thecanvas->Print("macros/qcd_control/plots/ratio_test_geq1bm_all_samples.pdf");

  //  delete mdpN;
  // delete leg;
  // delete thecanvas;
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
