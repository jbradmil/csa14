#include "TFile.h"
#include "TString.h"
#include <iostream>
#include <TH2.h>
#include <vector>
#include "TCanvas.h"
#include <TLegend.h>
#include <TROOT.h>
#include <TStyle.h>
#include "EffCompare.C"
class Plotting
{
public:
	Plotting()
	{
		std::cout<<"plotting..."<<std::endl;
	}
	void SaveEfficiency(TH2F *input);
	~Plotting(){}
};
void EffMerge()
{
	Plotting *p = new Plotting();
	gStyle->SetPaintTextFormat("5.2f");
	gStyle->SetStatW(0.1);
	gStyle->SetStatH(0.1);
	gStyle->SetStatY(202);
	gStyle->SetTitleYOffset(1.3);
	gROOT->SetBatch(true);
	TFile *ttbarwpjEffInput = new TFile("Efficiencies.root","UPDATE");
	ttbarwpjEffInput->mkdir("TagAndProbe");
	TDirectory *dTagAndProbe = (TDirectory*)ttbarwpjEffInput->Get("TagAndProbe");
	TDirectory *dTTbarWPJEff = (TDirectory*)ttbarwpjEffInput->Get("Efficiencies");
	TFile *MuIsoFile = new TFile("MuIsoMC_TagAndProbe.root","READ");
	TDirectory *dInputTagAndProbeMCIsoMu = (TDirectory*)MuIsoFile->Get("MuIso/MuIsoActivityPt/fit_eff_plots");
	TFile *MuRecoFile = new TFile("MuRecoMC_TagAndProbe.root","READ");
	TDirectory *dInputTagAndProbeMCRecoMu = (TDirectory*)MuRecoFile->Get("MuReco/MuRecoActivityPt/fit_eff_plots");
	TFile *ElecIsoFile = new TFile("ElecIsoMC_TagAndProbe.root","READ");
	TDirectory *dInputTagAndProbeMCIsoElec = (TDirectory*)ElecIsoFile->Get("ElecIso/ElecIsoActivityPt/fit_eff_plots");
	TFile *ElecRecoFile = new TFile("ElecRecoMC_TagAndProbe.root","READ");
	TDirectory *dInputTagAndProbeMCRecoElec = (TDirectory*)ElecRecoFile->Get("ElecReco/ElecRecoActivityPt/fit_eff_plots");
	
	// get TH2F
	
	TCanvas* TMC00 = (TCanvas*)dInputTagAndProbeMCIsoMu->Get("Pt_Activity_PLOT")->Clone();
	TH2F *mc_tap_eff_mu_iso = (TH2F*)TMC00->GetPrimitive("Pt_Activity_PLOT")->Clone();
	
	TMC00 = (TCanvas*)dInputTagAndProbeMCRecoMu->Get("Pt_Activity_PLOT")->Clone();
	TH2F *mc_tap_eff_mu_reco = (TH2F*)TMC00->GetPrimitive("Pt_Activity_PLOT")->Clone();
	
	TMC00 = (TCanvas*)dInputTagAndProbeMCIsoElec->Get("Pt_Activity_PLOT")->Clone();
	TH2F *mc_tap_eff_elec_iso = (TH2F*)TMC00->GetPrimitive("Pt_Activity_PLOT")->Clone();
	
	TMC00 = (TCanvas*)dInputTagAndProbeMCRecoElec->Get("Pt_Activity_PLOT")->Clone();
	TH2F *mc_tap_eff_elec_reco = (TH2F*)TMC00->GetPrimitive("Pt_Activity_PLOT")->Clone();
	TString name1 = "t#bar{t}, W#rightarrowl#nu+jets (truth)";
	TString name2 = "Tag&Probe MC";
	dTagAndProbe->cd();
	mc_tap_eff_mu_iso->SetName("MuIsoTagAndProbeMC");
	mc_tap_eff_mu_iso->SetTitle("Simulation, L=4 fb-1, #sqrt{s}=13 TeV #mu iso Tag & Probe MC; p_{T} [GeV]; Activity");
	mc_tap_eff_mu_iso->SetMarkerSize(2.0);
	mc_tap_eff_mu_iso->UseCurrentStyle();
	TH2FCompare(name1, name2,(TH2F*) dTTbarWPJEff->Get("MuIsoPTActivity"), mc_tap_eff_mu_iso, ttbarwpjEffInput, "MuIsoPTActivity");
	p->SaveEfficiency(mc_tap_eff_mu_iso);
	mc_tap_eff_mu_iso->Write();
	mc_tap_eff_mu_reco->SetName("MuRecoTagAndProbeMC");
	mc_tap_eff_mu_reco->SetTitle("Simulation, L=4 fb-1, #sqrt{s}=13 TeV #mu reco Tag & Probe MC; p_{T} [GeV]; Activity");
	mc_tap_eff_mu_reco->SetMarkerSize(2.0);
	mc_tap_eff_mu_reco->UseCurrentStyle();
	TH2FCompare(name1, name2,(TH2F*) dTTbarWPJEff->Get("MuRecoPTActivity"), mc_tap_eff_mu_reco, ttbarwpjEffInput, "MuRecoPTActivity");
	p->SaveEfficiency(mc_tap_eff_mu_reco);
	mc_tap_eff_mu_reco->Write();
	mc_tap_eff_elec_iso->SetName("ElecIsoTagAndProbeMC");
	mc_tap_eff_elec_iso->SetTitle("Simulation, L=4 fb-1, #sqrt{s}=13 TeV e iso Tag & Probe MC; p_{T} [GeV]; Activity");
	mc_tap_eff_elec_iso->SetMarkerSize(2.0);
	mc_tap_eff_elec_iso->UseCurrentStyle();
	TH2FCompare(name1, name2,(TH2F*) dTTbarWPJEff->Get("ElecIsoPTActivity"), mc_tap_eff_elec_iso, ttbarwpjEffInput, "ElecIsoPTActivity");
	p->SaveEfficiency(mc_tap_eff_elec_iso);
	mc_tap_eff_elec_iso->Write();
	mc_tap_eff_elec_reco->SetName("ElecRecoTagAndProbeMC");
	mc_tap_eff_elec_reco->SetTitle("Simulation, L=4 fb-1, #sqrt{s}=13 TeV e reco Tag & Probe MC; p_{T} [GeV]; Activity");
	mc_tap_eff_elec_reco->SetMarkerSize(2.0);
	mc_tap_eff_elec_reco->UseCurrentStyle();
	TH2FCompare(name1, name2,(TH2F*) dTTbarWPJEff->Get("ElecRecoPTActivity"), mc_tap_eff_elec_reco, ttbarwpjEffInput, "ElecRecoPTActivity");
	p->SaveEfficiency(mc_tap_eff_elec_reco);
	mc_tap_eff_elec_reco->Write();
	
	ttbarwpjEffInput->Close();
	MuIsoFile->Close();
	MuRecoFile->Close();
	ElecIsoFile->Close();
	ElecRecoFile->Close();
	
}

void Plotting::SaveEfficiency(TH2F *input)
{
	gROOT->SetBatch(true);
	const TString th2Name = input->GetName();
	const TString th2Title = input->GetTitle();
	TCanvas *c1 = new TCanvas(th2Name,th2Title,1);
	c1->cd();
	c1->SetLogx();
	c1->SetLogy();
	input->SetMarkerSize(2.0);
	input->UseCurrentStyle();
	input->SetMinimum(0.);
	input->SetMaximum(1.);
	input->GetZaxis()->SetLimits(0.,100.);
	input->Draw("ColZ,Text,E");
	c1->Update();
	
	// c1->SaveAs(s+"MuonAccEff3"+".png");
	c1->SaveAs(th2Name+".pdf");
	delete c1;
	gROOT->SetBatch(false);
	
}
