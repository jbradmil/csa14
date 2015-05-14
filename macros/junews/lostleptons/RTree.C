#define RTree_cxx
#include "RTree.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include "macros/junews/lostleptons/plotting.h"
#include "macros/junews/lostleptons/utils.h"

void RTree::MakeMHTPDFs(TString outdir) {
  TFile* outfile = new TFile(outdir+"closure/ll_PDFs.root", "recreate");

  const int nbinsx(8); // ptW
  double binsx[nbinsx+1] = {200, 250, 300, 350, 400, 500, 600, 750, 1000};
  TH1D* hPTW_mu = new TH1D("hPTW_mu", ";Reconstructed p_{T}^{W} [GeV];Events / bin", nbinsx, binsx); 
  TH1D* hPTW_mu_RLF = new TH1D("hPTW_mu_RLF", ";Reconstructed p_{T}^{W} [GeV];Events / bin", nbinsx, binsx); 
  TH1D* hPTW_el = new TH1D("hPTW_el", ";Reconstructed p_{T}^{W} [GeV];Events / bin", nbinsx, binsx); 
  TH1D* hPTW_el_RLF = new TH1D("hPTW_el_RLF", ";Reconstructed p_{T}^{W} [GeV];Events / bin", nbinsx, binsx);
  TH1D* hMHTtrue_ht1nj1= new TH1D("hMHTtrue_ht1nj1", ";H_{T}^{miss} [GeV];Events / bin", nbinsx, binsx); 
  TH1D* hMHTtrue_ht2nj1= new TH1D("hMHTtrue_ht2nj1", ";H_{T}^{miss} [GeV];Events / bin", nbinsx, binsx); 
  TH1D* hMHTtrue_ht3nj1= new TH1D("hMHTtrue_ht3nj1", ";H_{T}^{miss} [GeV];Events / bin", nbinsx, binsx);
  TH1D* hMHTtrue_ht1nj2= new TH1D("hMHTtrue_ht1nj2", ";H_{T}^{miss} [GeV];Events / bin", nbinsx, binsx); 
  TH1D* hMHTtrue_ht2nj2= new TH1D("hMHTtrue_ht2nj2", ";H_{T}^{miss} [GeV];Events / bin", nbinsx, binsx); 
  TH1D* hMHTtrue_ht3nj2= new TH1D("hMHTtrue_ht3nj2", ";H_{T}^{miss} [GeV];Events / bin", nbinsx, binsx);
  TH1D* hMHTtrue_ht1nj3= new TH1D("hMHTtrue_ht1nj3", ";H_{T}^{miss} [GeV];Events / bin", nbinsx, binsx); 
  TH1D* hMHTtrue_ht2nj3= new TH1D("hMHTtrue_ht2nj3", ";H_{T}^{miss} [GeV];Events / bin", nbinsx, binsx); 
  TH1D* hMHTtrue_ht3nj3= new TH1D("hMHTtrue_ht3nj3", ";H_{T}^{miss} [GeV];Events / bin", nbinsx, binsx); 
  TH1D* hMHTpred_ht1nj1= new TH1D("hMHTpred_ht1nj1", ";H_{T}^{miss} [GeV];Events / bin", nbinsx, binsx); 
  TH1D* hMHTpred_ht2nj1= new TH1D("hMHTpred_ht2nj1", ";H_{T}^{miss} [GeV];Events / bin", nbinsx, binsx); 
  TH1D* hMHTpred_ht3nj1= new TH1D("hMHTpred_ht3nj1", ";H_{T}^{miss} [GeV];Events / bin", nbinsx, binsx);
  TH1D* hMHTpred_ht1nj2= new TH1D("hMHTpred_ht1nj2", ";H_{T}^{miss} [GeV];Events / bin", nbinsx, binsx); 
  TH1D* hMHTpred_ht2nj2= new TH1D("hMHTpred_ht2nj2", ";H_{T}^{miss} [GeV];Events / bin", nbinsx, binsx); 
  TH1D* hMHTpred_ht3nj2= new TH1D("hMHTpred_ht3nj2", ";H_{T}^{miss} [GeV];Events / bin", nbinsx, binsx);
  TH1D* hMHTpred_ht1nj3= new TH1D("hMHTpred_ht1nj3", ";H_{T}^{miss} [GeV];Events / bin", nbinsx, binsx); 
  TH1D* hMHTpred_ht2nj3= new TH1D("hMHTpred_ht2nj3", ";H_{T}^{miss} [GeV];Events / bin", nbinsx, binsx); 
  TH1D* hMHTpred_ht3nj3= new TH1D("hMHTpred_ht3nj3", ";H_{T}^{miss} [GeV];Events / bin", nbinsx, binsx);

  gStyle->SetOptStat(0);
  TH1::StatOverflows(kTRUE);
  TH1::SetDefaultSumw2(1);
  TH3::SetDefaultSumw2(1);

  TFile* fRLF = TFile::Open(outdir+"RLF/RLF.root");
  TH3D* hRLF = (TH3D*)fRLF->Get("hRLF");
  TFile* fDTT = TFile::Open(outdir+"PDF/ll_PDFs.root");
  TH1D* hPDFlost_ht1nj1= (TH1D*) fDTT->Get("hPDFlost_ht1nj1");
  TH1D* hPDFlost_ht1nj2= (TH1D*) fDTT->Get("hPDFlost_ht1nj2");
  TH1D* hPDFlost_ht1nj3= (TH1D*) fDTT->Get("hPDFlost_ht1nj3");
  TH1D* hPDFlost_ht2nj1= (TH1D*) fDTT->Get("hPDFlost_ht2nj1");
  TH1D* hPDFlost_ht2nj2= (TH1D*) fDTT->Get("hPDFlost_ht2nj2");
  TH1D* hPDFlost_ht2nj3= (TH1D*) fDTT->Get("hPDFlost_ht2nj3");
  TH1D* hPDFlost_ht3nj1= (TH1D*) fDTT->Get("hPDFlost_ht3nj1");
  TH1D* hPDFlost_ht3nj2= (TH1D*) fDTT->Get("hPDFlost_ht3nj2");
  TH1D* hPDFlost_ht3nj3= (TH1D*) fDTT->Get("hPDFlost_ht3nj3");

  if (fChain == 0) return;
  Long64_t nentries = fChain->GetEntries();
  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    if (jentry==1||jentry==10||jentry==100||jentry==1000||jentry%10000==0) {
      printf("Entry %d/%d\n",jentry,nentries);
    }
    // if (jentry>25) break;
    // treating these as data (no lumi weights)
    // Baseline selection:
    if (!(mht30>200&&min_delta_phi_met_N>6&&num_jets_pt30>=4&&ht30>500&&num_csvm_jets30>=2)) continue;
    // one true e/mu
    if (!(num_true_mus+num_true_els==1 && num_true_had_taus==0)) continue;
    // fill the true mht distirbutions form 0-lepton events
    if (num_veto_mus+num_veto_els==0) { // lost
	if (num_jets_pt30>=4&num_jets_pt30<=6) { 
	  if (ht30>500&&ht30<800) hMHTtrue_ht1nj1->Fill(mht30);
	  else if (ht30>800&&ht30<1200) hMHTtrue_ht2nj1->Fill(mht30);
	  else if (ht30>1200) hMHTtrue_ht3nj1->Fill(mht30);
	}
	else if (num_jets_pt30>=7&num_jets_pt30<=8) { 
	  if (ht30>500&&ht30<800) hMHTtrue_ht1nj2->Fill(mht30);
	  else if (ht30>800&&ht30<1200) hMHTtrue_ht2nj2->Fill(mht30);
	  else if (ht30>1200) hMHTtrue_ht3nj2->Fill(mht30);
	}
	else if (num_jets_pt30>=9) { 
	  if (ht30>500&&ht30<800) hMHTtrue_ht1nj3->Fill(mht30);
	  else if (ht30>800&&ht30<1200) hMHTtrue_ht2nj3->Fill(mht30);
	  else if (ht30>1200) hMHTtrue_ht3nj3->Fill(mht30);
	}
    }
    // now make PDFs from found leptons
    else if (num_true_mus==1&&num_veto_mus==1) { // found
	for (unsigned int imu(0); imu<mu_pt->size(); imu++) {
	  if (mu_ptW->at(imu)>200&&mu_pt->at(imu)>10&&mu_mini_iso_tr02->at(imu)<0.2&&mu_mT->at(imu)<100) {
	    double ptW = mu_ptW->at(imu);
	    double RLF = hRLF->GetBinContent(hRLF->FindBin(ptW, ht30, num_jets_pt30));
	    hPTW_mu->Fill(ptW/*, weightppb*10055*/);
	    hPTW_mu_RLF->Fill(ptW, RLF);
	    if (num_jets_pt30>=4&num_jets_pt30<=6) {
	      if (ht30>500&&ht30<800) FillMHTPDF(hMHTpred_ht1nj1, hPDFlost_ht1nj1, ptW, RLF);
	      else if (ht30>800&&ht30<1200) FillMHTPDF(hMHTpred_ht2nj1, hPDFlost_ht2nj1, ptW, RLF);
	      else if (ht30>1200) FillMHTPDF(hMHTpred_ht3nj1, hPDFlost_ht3nj1, ptW, RLF);
	    }
	    else if (num_jets_pt30>=7&num_jets_pt30<=8) {
	      if (ht30>500&&ht30<800) FillMHTPDF(hMHTpred_ht1nj2, hPDFlost_ht1nj2, ptW, RLF);
	      else if (ht30>800&&ht30<1200) FillMHTPDF(hMHTpred_ht2nj2, hPDFlost_ht2nj2, ptW, RLF);
	      else if (ht30>1200) FillMHTPDF(hMHTpred_ht3nj2, hPDFlost_ht3nj2, ptW, RLF);
	    }
	    else if (num_jets_pt30>=9){
	      if (ht30>500&&ht30<800) FillMHTPDF(hMHTpred_ht1nj3, hPDFlost_ht1nj3, ptW, RLF);
	      else if (ht30>800&&ht30<1200) FillMHTPDF(hMHTpred_ht2nj3, hPDFlost_ht2nj3, ptW);
	      else if (ht30>1200) FillMHTPDF(hMHTpred_ht3nj3, hPDFlost_ht3nj3, ptW, RLF);
	    }
	  }
	}
      }
    // electrons
    else if (num_true_els==1&&num_veto_els==1) { // found
	for (unsigned int iel(0); iel<el_pt->size(); iel++) {
	  if (el_ptW->at(iel)>200&&el_pt->at(iel)>10&&el_mini_iso_tr02->at(iel)<0.1&&el_mT->at(iel)<100) {
	    double ptW = el_ptW->at(iel);
	    double RLF = hRLF->GetBinContent(hRLF->FindBin(ptW, ht30, num_jets_pt30));
	    hPTW_el->Fill(ptW/*, weightppb*10055*/);
	    hPTW_el_RLF->Fill(ptW, RLF);
	    if (num_jets_pt30>=4&num_jets_pt30<=6) {
	      if (ht30>500&&ht30<800) FillMHTPDF(hMHTpred_ht1nj1, hPDFlost_ht1nj1, ptW, RLF);
	      else if (ht30>800&&ht30<1200) FillMHTPDF(hMHTpred_ht2nj1, hPDFlost_ht2nj1, ptW, RLF);
	      else if (ht30>1200) FillMHTPDF(hMHTpred_ht3nj1, hPDFlost_ht3nj1, ptW, RLF);
	    }
	    else if (num_jets_pt30>=7&num_jets_pt30<=8) {
	      if (ht30>500&&ht30<800) FillMHTPDF(hMHTpred_ht1nj2, hPDFlost_ht1nj2, ptW, RLF);
	      else if (ht30>800&&ht30<1200) FillMHTPDF(hMHTpred_ht2nj2, hPDFlost_ht2nj2, ptW, RLF);
	      else if (ht30>1200) FillMHTPDF(hMHTpred_ht3nj2, hPDFlost_ht3nj2, ptW, RLF);
	    }
	    else if (num_jets_pt30>=9){
	      if (ht30>500&&ht30<800) FillMHTPDF(hMHTpred_ht1nj3, hPDFlost_ht1nj3, ptW, RLF);
	      else if (ht30>800&&ht30<1200) FillMHTPDF(hMHTpred_ht2nj3, hPDFlost_ht2nj3, ptW);
	      else if (ht30>1200) FillMHTPDF(hMHTpred_ht3nj3, hPDFlost_ht3nj3, ptW, RLF);
	    }
	  }
	}
      }
  } // end loop over events

  MakeWPTPlot(hPTW_mu, hPTW_mu_RLF, hPTW_el, hPTW_el_RLF, outdir);

  FixOverflow(hMHTpred_ht1nj1);
  FixOverflow(hMHTpred_ht2nj1);
  FixOverflow(hMHTpred_ht3nj1);
  FixOverflow(hMHTpred_ht1nj2);
  FixOverflow(hMHTpred_ht2nj2);
  FixOverflow(hMHTpred_ht3nj2);
  FixOverflow(hMHTpred_ht1nj3);
  FixOverflow(hMHTpred_ht2nj3);
  FixOverflow(hMHTpred_ht3nj3);
  FixOverflow(hMHTtrue_ht1nj1);
  FixOverflow(hMHTtrue_ht2nj1);
  FixOverflow(hMHTtrue_ht3nj1);
  FixOverflow(hMHTtrue_ht1nj2);
  FixOverflow(hMHTtrue_ht2nj2);
  FixOverflow(hMHTtrue_ht3nj2);
  FixOverflow(hMHTtrue_ht1nj3);
  FixOverflow(hMHTtrue_ht2nj3);
  FixOverflow(hMHTtrue_ht3nj3);
  
  TStyle *tdrStyle = new TStyle("tdrStyle","Style for P-TDR");
  setTDRStyle(tdrStyle);
  tdrStyle->cd();
  MakeClosurePlot(hMHTpred_ht1nj1, hMHTtrue_ht1nj1, "ht1nj1", outdir);
  MakeClosurePlot(hMHTpred_ht2nj1, hMHTtrue_ht2nj1, "ht2nj1", outdir);
  MakeClosurePlot(hMHTpred_ht3nj1, hMHTtrue_ht3nj1, "ht3nj1", outdir, false);
  MakeClosurePlot(hMHTpred_ht1nj2, hMHTtrue_ht1nj2, "ht1nj2", outdir);
  MakeClosurePlot(hMHTpred_ht2nj2, hMHTtrue_ht2nj2, "ht2nj2", outdir, false);
  MakeClosurePlot(hMHTpred_ht3nj2, hMHTtrue_ht3nj2, "ht3nj2", outdir, false);
  MakeClosurePlot(hMHTpred_ht1nj3, hMHTtrue_ht1nj3, "ht1nj3", outdir, false);
  MakeClosurePlot(hMHTpred_ht2nj3, hMHTtrue_ht2nj3, "ht2nj3", outdir, false);
  MakeClosurePlot(hMHTpred_ht3nj3, hMHTtrue_ht3nj3, "ht3nj3", outdir, false);
  
}

void RTree::MakeDTTPDFs(TString outdir) {

  TFile* outfile = new TFile(outdir+"PDF/ll_PDFs.root", "recreate");

  gStyle->SetOptStat(0);
  TH1::StatOverflows(kTRUE);

  TH1::SetDefaultSumw2(1);

  TH1D* hPDFlost_ht1nj1 = new TH1D("hPDFlost_ht1nj1",";#frac{1}{2}(1-cos#Delta#theta_{T});% of events", 20, 0, 1);
  TH1D* hPDFfound_ht1nj1 = new TH1D("hPDFfound_ht1nj1",";#frac{1}{2}(1-cos#Delta#theta_{T});% of events", 20, 0, 1);
  TH1D* hPDFlost_ht1nj2 = new TH1D("hPDFlost_ht1nj2",";#frac{1}{2}(1-cos#Delta#theta_{T});% of events", 20, 0, 1);
  TH1D* hPDFfound_ht1nj2 = new TH1D("hPDFfound_ht1nj2",";#frac{1}{2}(1-cos#Delta#theta_{T});% of events", 20, 0, 1);
  TH1D* hPDFlost_ht1nj3 = new TH1D("hPDFlost_ht1nj3",";#frac{1}{2}(1-cos#Delta#theta_{T});% of events", 20, 0, 1);
  TH1D* hPDFfound_ht1nj3 = new TH1D("hPDFfound_ht1nj3",";#frac{1}{2}(1-cos#Delta#theta_{T});% of events", 20, 0, 1);

  TH1D* hPDFlost_ht2nj1 = new TH1D("hPDFlost_ht2nj1",";#frac{1}{2}(1-cos#Delta#theta_{T});% of events", 20, 0, 1);
  TH1D* hPDFfound_ht2nj1 = new TH1D("hPDFfound_ht2nj1",";#frac{1}{2}(1-cos#Delta#theta_{T});% of events", 20, 0, 1);
  TH1D* hPDFlost_ht2nj2 = new TH1D("hPDFlost_ht2nj2",";#frac{1}{2}(1-cos#Delta#theta_{T});% of events", 20, 0, 1);
  TH1D* hPDFfound_ht2nj2 = new TH1D("hPDFfound_ht2nj2",";#frac{1}{2}(1-cos#Delta#theta_{T});% of events", 20, 0, 1);
  TH1D* hPDFlost_ht2nj3 = new TH1D("hPDFlost_ht2nj3",";#frac{1}{2}(1-cos#Delta#theta_{T});% of events", 20, 0, 1);
  TH1D* hPDFfound_ht2nj3 = new TH1D("hPDFfound_ht2nj3",";#frac{1}{2}(1-cos#Delta#theta_{T});% of events", 20, 0, 1);

  TH1D* hPDFlost_ht3nj1 = new TH1D("hPDFlost_ht3nj1",";#frac{1}{2}(1-cos#Delta#theta_{T});% of events", 20, 0, 1);
  TH1D* hPDFfound_ht3nj1 = new TH1D("hPDFfound_ht3nj1",";#frac{1}{2}(1-cos#Delta#theta_{T});% of events", 20, 0, 1);
  TH1D* hPDFlost_ht3nj2 = new TH1D("hPDFlost_ht3nj2",";#frac{1}{2}(1-cos#Delta#theta_{T});% of events", 20, 0, 1);
  TH1D* hPDFfound_ht3nj2 = new TH1D("hPDFfound_ht3nj2",";#frac{1}{2}(1-cos#Delta#theta_{T});% of events", 20, 0, 1);
  TH1D* hPDFlost_ht3nj3 = new TH1D("hPDFlost_ht3nj3",";#frac{1}{2}(1-cos#Delta#theta_{T});% of events", 20, 0, 1);
  TH1D* hPDFfound_ht3nj3 = new TH1D("hPDFfound_ht3nj3",";#frac{1}{2}(1-cos#Delta#theta_{T});% of events", 20, 0, 1);
  
  if (fChain == 0) return;

  Long64_t nentries = fChain->GetEntries();

  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    if (jentry==1||jentry==10||jentry==100||jentry==1000||jentry%10000==0) {
      //printf("\r          ");
      printf("Entry %d/%d\n",jentry,nentries);
    }
    // if (Cut(ientry) < 0) continue;

    // Baseline selection:
    if (!(mht30>200&&min_delta_phi_met_N>6&&num_jets_pt30>=4&&ht30>500)) continue;
    // one true e/mu
    if (!(num_true_mus+num_true_els==1 && num_true_had_taus==0)) continue;

    if (num_true_mus==1) {    
      if (num_veto_mus==0) { // lost
	for (unsigned int imu(0); imu<true_mu_pt->size(); imu++) {
	  if (true_mu_ptW->at(imu)>200) {
	    if (num_jets_pt30>=4&num_jets_pt30<=6) {
	      if (ht30>500&&ht30<800) hPDFlost_ht1nj1->Fill(0.5*(1-cos(true_mu_dTT->at(imu))), 1);
	      else if (ht30>800&&ht30<1200) hPDFlost_ht2nj1->Fill(0.5*(1-cos(true_mu_dTT->at(imu))), 1);
	      else if (ht30>1200) hPDFlost_ht3nj1->Fill(0.5*(1-cos(true_mu_dTT->at(imu))), 1);
	    }
	    else if (num_jets_pt30>=7&num_jets_pt30<=8) {
	      if (ht30>500&&ht30<800) hPDFlost_ht1nj2->Fill(0.5*(1-cos(true_mu_dTT->at(imu))), 1);
	      else if (ht30>800&&ht30<1200) hPDFlost_ht2nj2->Fill(0.5*(1-cos(true_mu_dTT->at(imu))), 1);
	      else if (ht30>1200) hPDFlost_ht3nj2->Fill(0.5*(1-cos(true_mu_dTT->at(imu))), 1);
	    }
	    else if (num_jets_pt30>=9){
	      if (ht30>500&&ht30<800) hPDFlost_ht1nj3->Fill(0.5*(1-cos(true_mu_dTT->at(imu))), 1);
	      else if (ht30>800&&ht30<1200) hPDFlost_ht2nj3->Fill(0.5*(1-cos(true_mu_dTT->at(imu))), 1);
	      else if (ht30>1200) hPDFlost_ht3nj3->Fill(0.5*(1-cos(true_mu_dTT->at(imu))), 1);
	    }
	  }
	}
      } else if (num_veto_mus==1) { // found
	for (unsigned int imu(0); imu<mu_pt->size(); imu++) {
	  if (mu_ptW->at(imu)>200&&mu_pt->at(imu)>10&&mu_mini_iso_tr02->at(imu)<0.2&&mu_mT->at(imu)<100) {
	    if (num_jets_pt30>=4&num_jets_pt30<=6) {
	      if (ht30>500&&ht30<800) hPDFfound_ht1nj1->Fill(0.5*(1-cos(mu_dTT->at(imu))), 1);
	      else if (ht30>800&&ht30<1200) hPDFfound_ht2nj1->Fill(0.5*(1-cos(mu_dTT->at(imu))), 1);
	      else if (ht30>1200) hPDFfound_ht3nj1->Fill(0.5*(1-cos(mu_dTT->at(imu))), 1);
	    }
	    else if (num_jets_pt30>=7&num_jets_pt30<=8) {
	      if (ht30>500&&ht30<800) hPDFfound_ht1nj2->Fill(0.5*(1-cos(mu_dTT->at(imu))), 1);
	      else if (ht30>800&&ht30<1200) hPDFfound_ht2nj2->Fill(0.5*(1-cos(mu_dTT->at(imu))), 1);
	      else if (ht30>1200) hPDFfound_ht3nj2->Fill(0.5*(1-cos(mu_dTT->at(imu))), 1);
	    }
	    else if (num_jets_pt30>=9){
	      if (ht30>500&&ht30<800) hPDFfound_ht1nj3->Fill(0.5*(1-cos(mu_dTT->at(imu))), 1);
	      else if (ht30>800&&ht30<1200) hPDFfound_ht2nj3->Fill(0.5*(1-cos(mu_dTT->at(imu))), 1);
	      else if (ht30>1200) hPDFfound_ht3nj3->Fill(0.5*(1-cos(mu_dTT->at(imu))), 1);
	    }
	  }
	}
      }
    }
    // electrons
    else if (num_true_els==1) {
      if (num_veto_els==0) { // lost
	for (unsigned int iel(0); iel<true_el_pt->size(); iel++) {
	  if (true_el_ptW->at(iel)>200) {
	    if (num_jets_pt30>=4&num_jets_pt30<=6) {
	      if (ht30>500&&ht30<800) hPDFlost_ht1nj1->Fill(0.5*(1-cos(true_el_dTT->at(iel))), 1);
	      else if (ht30>800&&ht30<1200) hPDFlost_ht2nj1->Fill(0.5*(1-cos(true_el_dTT->at(iel))), 1);
	      else if (ht30>1200) hPDFlost_ht3nj1->Fill(0.5*(1-cos(true_el_dTT->at(iel))), 1);
	    }
	    else if (num_jets_pt30>=7&num_jets_pt30<=8) {
	      if (ht30>500&&ht30<800) hPDFlost_ht1nj2->Fill(0.5*(1-cos(true_el_dTT->at(iel))), 1);
	      else if (ht30>800&&ht30<1200) hPDFlost_ht2nj2->Fill(0.5*(1-cos(true_el_dTT->at(iel))), 1);
	      else if (ht30>1200) hPDFlost_ht3nj2->Fill(0.5*(1-cos(true_el_dTT->at(iel))), 1);
	    }
	    else if (num_jets_pt30>=9){
	      if (ht30>500&&ht30<800) hPDFlost_ht1nj3->Fill(0.5*(1-cos(true_el_dTT->at(iel))), 1);
	      else if (ht30>800&&ht30<1200) hPDFlost_ht2nj3->Fill(0.5*(1-cos(true_el_dTT->at(iel))), 1);
	      else if (ht30>1200) hPDFlost_ht3nj3->Fill(0.5*(1-cos(true_el_dTT->at(iel))), 1);
	    }
	  }
	}
      } else if (num_veto_els==1) { // found
	for (unsigned int iel(0); iel<el_pt->size(); iel++) {
	  if (el_ptW->at(iel)>200&&el_pt->at(iel)>10&&el_mini_iso_tr02->at(iel)<0.1&&el_mT->at(iel)<100) {
	    if (num_jets_pt30>=4&num_jets_pt30<=6) {
	      if (ht30>500&&ht30<800) hPDFfound_ht1nj1->Fill(0.5*(1-cos(el_dTT->at(iel))), 1);
	      else if (ht30>800&&ht30<1200) hPDFfound_ht2nj1->Fill(0.5*(1-cos(el_dTT->at(iel))), 1);
	      else if (ht30>1200) hPDFfound_ht3nj1->Fill(0.5*(1-cos(el_dTT->at(iel))), 1);
	    }
	    else if (num_jets_pt30>=7&num_jets_pt30<=8) {
	      if (ht30>500&&ht30<800) hPDFfound_ht1nj2->Fill(0.5*(1-cos(el_dTT->at(iel))), 1);
	      else if (ht30>800&&ht30<1200) hPDFfound_ht2nj2->Fill(0.5*(1-cos(el_dTT->at(iel))), 1);
	      else if (ht30>1200) hPDFfound_ht3nj2->Fill(0.5*(1-cos(el_dTT->at(iel))), 1);
	    }
	    else if (num_jets_pt30>=9){
	      if (ht30>500&&ht30<800) hPDFfound_ht1nj3->Fill(0.5*(1-cos(el_dTT->at(iel))), 1);
	      else if (ht30>800&&ht30<1200) hPDFfound_ht2nj3->Fill(0.5*(1-cos(el_dTT->at(iel))), 1);
	      else if (ht30>1200) hPDFfound_ht3nj3->Fill(0.5*(1-cos(el_dTT->at(iel))), 1);
	    }
	  }
	}
      }
    }

  } // loop over all events

  NormalizeHist(hPDFlost_ht1nj1);
  NormalizeHist(hPDFlost_ht1nj2);
  NormalizeHist(hPDFlost_ht1nj3);
  NormalizeHist(hPDFlost_ht2nj1);
  NormalizeHist(hPDFlost_ht2nj2);
  NormalizeHist(hPDFlost_ht2nj3);
  NormalizeHist(hPDFlost_ht3nj1);
  NormalizeHist(hPDFlost_ht3nj2);
  NormalizeHist(hPDFlost_ht3nj3);
  NormalizeHist(hPDFfound_ht1nj1);
  NormalizeHist(hPDFfound_ht1nj2);
  NormalizeHist(hPDFfound_ht1nj3);
  NormalizeHist(hPDFfound_ht2nj1);
  NormalizeHist(hPDFfound_ht2nj2);
  NormalizeHist(hPDFfound_ht2nj3);
  NormalizeHist(hPDFfound_ht3nj1);
  NormalizeHist(hPDFfound_ht3nj2);
  NormalizeHist(hPDFfound_ht3nj3);

  TStyle *tdrStyle = new TStyle("tdrStyle","Style for P-TDR");
  setTDRStyle(tdrStyle);
  MakeDTTPDFPlot(hPDFlost_ht1nj1, hPDFfound_ht1nj1, "ht1nj1", outdir);
  MakeDTTPDFPlot(hPDFlost_ht1nj2, hPDFfound_ht1nj2, "ht1nj2", outdir);
  MakeDTTPDFPlot(hPDFlost_ht1nj3, hPDFfound_ht1nj3, "ht1nj3", outdir);
  MakeDTTPDFPlot(hPDFlost_ht2nj1, hPDFfound_ht2nj1, "ht2nj1", outdir);
  MakeDTTPDFPlot(hPDFlost_ht2nj2, hPDFfound_ht2nj2, "ht2nj2", outdir);
  MakeDTTPDFPlot(hPDFlost_ht2nj3, hPDFfound_ht2nj3, "ht2nj3", outdir);
  MakeDTTPDFPlot(hPDFlost_ht3nj1, hPDFfound_ht3nj1, "ht3nj1", outdir);
  MakeDTTPDFPlot(hPDFlost_ht3nj2, hPDFfound_ht3nj2, "ht3nj2", outdir);
  MakeDTTPDFPlot(hPDFlost_ht3nj3, hPDFfound_ht3nj3, "ht3nj3", outdir);
  
  outfile->cd();
  hPDFlost_ht1nj1->Write();
  hPDFlost_ht1nj2->Write();
  hPDFlost_ht1nj3->Write();
  hPDFlost_ht2nj1->Write();
  hPDFlost_ht2nj2->Write();
  hPDFlost_ht2nj3->Write();
  hPDFlost_ht3nj1->Write();
  hPDFlost_ht3nj2->Write();
  hPDFlost_ht3nj3->Write();
  hPDFfound_ht1nj1->Write();
  hPDFfound_ht1nj2->Write();
  hPDFfound_ht1nj3->Write();
  hPDFfound_ht2nj1->Write();
  hPDFfound_ht2nj2->Write();
  hPDFfound_ht2nj3->Write();
  hPDFfound_ht3nj1->Write();
  hPDFfound_ht3nj2->Write();
  hPDFfound_ht3nj3->Write();
  
}

void RTree::MakeLFRatioHist(TString outfilename) {

  // This fills the TH3 we use to look up RLF

  gStyle->SetOptStat(0);
  TH1::StatOverflows(kTRUE);

  TH1::SetDefaultSumw2(1); //trick to turn on Sumw2 for all histos
  TH3::SetDefaultSumw2(1);

  TFile outfile(outfilename, "recreate");

  const int nbinsx(8); // ptW
  double binsx[nbinsx+1] = {200, 250, 300, 350, 400, 500, 600, 750, 1000000};
  const int nbinsy(3); // HT
  double binsy[nbinsy+1] = {500., 800., 1200., 1000000};
  const int nbinsz(3); // NJets
  double binsz[nbinsz+1] = {3.5, 6.5, 8.5, 1000000};
  
  TH3D* hlost = new TH3D("hlost","", nbinsx, binsx, nbinsy, binsy, nbinsz, binsz);
  TH3D* hfound = new TH3D("hfound","", nbinsx, binsx, nbinsy, binsy, nbinsz, binsz);
  TH3D* RLF = new TH3D("hRLF",";p_{T}^{W} [GeV];R_{LF}", nbinsx, binsx, nbinsy, binsy, nbinsz, binsz);
  TH3D* hlost_mu = new TH3D("hlost_mu","", nbinsx, binsx, nbinsy, binsy, nbinsz, binsz);
  TH3D* hfound_mu = new TH3D("hfound_mu","", nbinsx, binsx, nbinsy, binsy, nbinsz, binsz);
  TH3D* RLF_mu = new TH3D("hRLF_mu",";p_{T}^{W} [GeV];R_{LF}", nbinsx, binsx, nbinsy, binsy, nbinsz, binsz);
  TH3D* hlost_el = new TH3D("hlost_el","", nbinsx, binsx, nbinsy, binsy, nbinsz, binsz);
  TH3D* hfound_el = new TH3D("hfound_el","", nbinsx, binsx, nbinsy, binsy, nbinsz, binsz);
  TH3D* RLF_el = new TH3D("hRLF_el",";p_{T}^{W} [GeV];R_{LF}", nbinsx, binsx, nbinsy, binsy, nbinsz, binsz);


  if (fChain == 0) return;

  Long64_t nentries = fChain->GetEntries();

  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    if (jentry==1||jentry==10||jentry==100||jentry==1000||jentry%10000==0) {
      //printf("\r          ");
      printf("Entry %d/%d\n",jentry,nentries);
    }
    //if (jentry>1) break;
    // if (Cut(ientry) < 0) continue;

    // Baseline selection:
    if (!(mht30>200&&min_delta_phi_met_N>6&&num_jets_pt30>=4&&ht30>500&&num_csvm_jets30>=2)) continue;
    // one true e/mu
    if (!(num_true_mus+num_true_els==1 && num_true_had_taus==0)) continue;
    // muons
    if (num_true_mus==1) {    
      if (num_veto_mus==0) { // lost
	for (unsigned int imu(0); imu<true_mu_pt->size(); imu++) {
	  if (true_mu_ptW->at(imu)>200) {
	    hlost_mu->Fill(true_mu_ptW->at(imu), ht30, num_jets_pt30, 1);
	  }
	}
      } else if (num_veto_mus==1) { // found
	for (unsigned int imu(0); imu<mu_pt->size(); imu++) {
	  if (mu_ptW->at(imu)>200&&mu_pt->at(imu)>10&&mu_mini_iso_tr02->at(imu)<0.2&&mu_mT->at(imu)<100) {
	    hfound_mu->Fill(mu_ptW->at(imu), ht30, num_jets_pt30, 1);
	  }
	}
      }
    }
    // electrons
    else if (num_true_els==1) {
      if (num_veto_els==0) { // lost
	for (unsigned int iel(0); iel<true_el_pt->size(); iel++) {
	  if (true_el_ptW->at(iel)>200) {
	    hlost_el->Fill(true_el_ptW->at(iel), ht30, num_jets_pt30, 1);
	  }
	}
      } else if (num_veto_els==1) { // found
	for (unsigned int iel(0); iel<el_pt->size(); iel++) {
	  if (el_ptW->at(iel)>200&&el_pt->at(iel)>10&&el_mini_iso_tr02->at(iel)<0.1&&el_mT->at(iel)<100) {
	    hfound_el->Fill(el_ptW->at(iel), ht30, num_jets_pt30, 1);
	  }
	}
      }
    }
  }
  //}

  hlost->Add(hlost_mu, hlost_el, 1., 1.);
  hfound->Add(hfound_mu, hfound_el, 1., 1.);
  RLF_mu->Divide(hlost_mu, hfound_mu, 1., 1., "B");
  RLF_el->Divide(hlost_el, hfound_el, 1., 1., "B");
  RLF->Divide(hlost, hfound, 1., 1., "B");
  
  hlost->Print("all");
  hfound->Print("all");
  RLF->Print("all");
   
  outfile.cd();
  RLF_mu->Write();
  RLF_el->Write();
  RLF->Write();
  //outfile.Close();
  
  delete hlost;
  delete hfound;
  delete RLF;
  delete hlost_mu;
  delete hfound_mu;
  delete RLF_mu;
  delete hlost_el;
  delete hfound_el;
  delete RLF_el;

  
}


