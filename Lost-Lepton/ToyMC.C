#include <TH1F.h>
#include <iostream>
#include "TRandom3.h"
#include <TFile.h>

void ToyMC()
{
	double meanOfPoissonPromptMuon=40;
	double meanOfPoissonPromptElec=40;
	double muEff=0.9;
	double elecEff=0.7;
	TRandom3 *randomNumber = new TRandom3();

	TH1F *PromptMu = new TH1F ("PromptMu","Prompt #mu",100,0,100);
	TH1F *CSMu = new TH1F("ControlSampleMu","Control-sample #mu",100,0,100);
	TH1F *MuPre = new TH1F("PredictionMu","Prediction #mu",100,0,100);
	TH1F *ExpMu = new TH1F("ExpMu","Expectation #mu",100,0,100);
	TH1F *PromptElec = new TH1F ("PromptElec","Prompt e",100,0,100);
	TH1F *CSElec = new TH1F("ControlSampleElec","Control-sample e",100,0,100);
	TH1F *ElecPre = new TH1F("PredictionElec","Prediction e",100,0,100);
	TH1F *ExpElec = new TH1F("ExpElec","Expectation e",100,0,100);
	int promptMu=0;
	int promptElec=0;
	for(unsigned int i=0; i<100000;i++)
	{
		// dice amount of control sample according to mean poisson
		promptMu= randomNumber->Poisson(meanOfPoissonPromptMuon);
		PromptMu->Fill(promptMu);
		promptElec= randomNumber->Poisson(meanOfPoissonPromptElec);
		PromptElec->Fill(promptElec);
		// according to eff determin size of control sample
		int muControlsample=0, muExpectation=0;
		int elecControlsample=0, elecExpectation=0;
		for(int ii=0; ii<promptMu;ii++)
		{
			if(randomNumber->Rndm()<muEff)
			{
				muControlsample++;
			}
			else
			{
				muExpectation++;
			}
		}
		double MuOnlyWeight = muControlsample * (1-muEff)/muEff;
		MuPre->Fill(MuOnlyWeight);
		CSMu->Fill(muControlsample);
		ExpMu->Fill(muExpectation);
		for(int ii=0; ii<promptElec;ii++)
		{
			if(randomNumber->Rndm()<elecEff)
			{
				elecControlsample++;
			}
			else
			{
				elecExpectation++;
			}
		}
		double ElecOnlyWeight = elecControlsample * (1-elecEff)/elecEff;
		double ElecOnlyError = ElecOnlyWeight * sqrt(elecControlsample);
		ElecPre->Fill(ElecOnlyWeight);
		CSElec->Fill(elecControlsample);
		ExpElec->Fill(elecExpectation);		
	}
	
	
	TFile *outPutFile = new TFile("ToyMC.root","RECREATE"); 
	outPutFile->cd();
	PromptMu->Write();
	MuPre->Write();
	CSMu->Write();
	ExpMu->Write();
	PromptElec->Write();
	CSElec->Write();
	ElecPre->Write();
	ExpElec->Write();
	outPutFile->Close();

}

