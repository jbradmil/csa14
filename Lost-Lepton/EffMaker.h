#ifndef EffMaker_h
#define EffMaker_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include <TH2F.h>
#include <TH1F.h>
#include "TVector2.h"
#include <cmath>
#include "TCanvas.h"
#include "TEfficiency.h"
#include "TGraphAsymmErrors.h"
#include <TLegend.h>


// output control
const bool saveEffToPDF_=true;
const bool saveEffToPNG_=false;
// cuts

const double mtwCut_ = 100;
// binning of efficiencies
// gloabel 1D binning

const int oneDHT_=14;
double OneDHT_[oneDHT_] = {350,500,550,600,650,700,750,800,900,1000,1200,1500,2000,8000};
const int oneDMHT_=12;
double OneDMHT_[oneDMHT_] = {200,250,300,350,450,500,600,700,900,1200,1700,8000};
const int oneDNJets_=6;
double OneDNJets_[oneDNJets_] = {4,5,6,8,11,20};
const int oneDBJets_=5;
double OneDBJets_[oneDBJets_] = {0,1,2,3,10};
const int oneDDeltaR_=12;
double OneDDeltaR_[oneDDeltaR_]={0 ,0.4 ,0.7 ,0.8 ,0.9 ,1.0 ,1.1 ,1.3 ,1.5 ,1.8 ,2.0 , 5.0 };
const int oneDPTRel_=14;
double OneDPTRel_[oneDPTRel_]={0 ,0.1 ,0.2 ,0.4 ,0.5 ,0.6 ,0.7 ,0.8 ,0.9 ,1.0 ,1.1 ,1.5 ,1.8 , 5.0 };

// used for closure
// const int oneDPT_=12;
// double OneDPT_[oneDPT_]={10,15,20,25,30,40,50,60,70,90,110,1900};
// const int oneDActivity_=9;
// double OneDActivity_[oneDActivity_]={0,5,10,20,40,60,80,100,1600};
// used to compare with tag and probe eff.
const int oneDPT_=7;
double OneDPT_[oneDPT_]={10,20,25,30,40,50,200};
const int oneDActivity_=6;
double OneDActivity_[oneDActivity_]={0,20,40,60,150,1600};

// purity
//mu
//1D

//2D
const int mupurityMHT_ = 5;
double muPurityMHT_[mupurityMHT_] ={200,800,900,1200,9600};
const int mupurityNJet_ = 3;
double muPurityNJet_[mupurityNJet_] ={4,5,22};

// elec

//1D


const int elecpurityMHT_ = 5;
double elecPurityMHT_[elecpurityMHT_] ={800,900,1200,9600};
const int elecpurityNJet_ = 5;
double elecPurityNJet_[elecpurityNJet_] ={2,3,4,5,22};
// mu acceptance
// 2D
const int muaccHT_ = 3;
double muAccHT_ [muaccHT_] = {500,1400,2500};
const int muaccMHT_ = 7;
double muAccMHT_ [muaccMHT_] = {200,250,300,500,700,900,2500};
const int muaccBTags_ = 5;
double muAccBTags_[muaccBTags_] = {0,1,2,3,10};
const int muaccNJets_ = 5;
double muAccNJets_ [muaccNJets_] = {4,5,6,9,20};

// elec acceptance
// 2D
const int elecaccHT_ = 3;
double elecAccHT_ [elecaccHT_] = {500,1300,2500};
const int elecaccMHT_ = 7;
double elecAccMHT_ [elecaccMHT_] = {200,250,300,500,700,900,2500};
const int elecaccBTags_ = 5;
double elecAccBTags_[elecaccBTags_] = {0,1,2,3,10};
const int elecaccNJets_ = 6;
double elecAccNJets_ [elecaccNJets_] = {4,5,6,8,11,20};

// mtw
// 2D
const int muMTWPT2D_=9;
double MuMTWPT2D_[muMTWPT2D_]={10,30,40,50,60,70,90,110,1900};
const int muMTWActivity2D_=9;
double MuMTWActivity2D_[muMTWActivity2D_]={0,5,10,20,40,60,80,100,1600};

const int elecMTWPT2D_=9;
double ElecMTWPT2D_[elecMTWPT2D_]={10,30,40,50,60,70,90,110,1900};
const int elecMTWActivity2D_=9;
double ElecMTWActivity2D_[elecMTWActivity2D_]={0,5,10,20,40,60,80,100,1600};

// isotrack reduction
// 1D
const int isotrackreductionHT_ = 3;
double isoTrackReductionHT_ [isotrackreductionHT_] = {500,1300,2500};
const int isotrackreductionMHT_ = 7;
double isoTrackReductionMHT_ [isotrackreductionMHT_] = {200,250,300,500,700,900,2500};
const int isotrackreductionBTags_ = 5;
double isoTrackReductionBTags_[isotrackreductionBTags_] = {0,1,2,3,10};
const int isotrackreductionNJets_ = 6;
double isoTrackReductionNJets_ [isotrackreductionNJets_] = {4,5,6,8,11,20};
const int isotrackreductionPT_ = 8;
double isoTrackReductionPT_ [isotrackreductionPT_] = {5,10,20,25,30,40,50,200};
const int isotrackreductionActivity_ = 6;
double isoTrackReductionActivity_ [isotrackreductionActivity_] = {0,20,40,60,150,1600};

// 2D
const int isotrackreductionBTags2D_ = 3;
double isoTrackReductionBTags2D_[isotrackreductionBTags2D_] = {0,1,10};
const int isotrackreductionNJets2D_ = 5;
double isoTrackReductionNJets2D_ [isotrackreductionNJets2D_] = {4,6,8,10,20};

const int isotrackreductionPT2D_ = 8;
double isoTrackReductionPT2D_[isotrackreductionPT2D_] = {5,10,20,25,30,40,50,200};
const int isotrackreductionActivity2D_ = 6;
double isoTrackReductionActivity2D_ [isotrackreductionActivity2D_] = {0,20,40,60,150,1600};

class Bin
{
public:
	Bin(){}
	Bin(double HTmin, double HTmax, double MHTmin, double MHTmax, int NJetsmin, int NJetsmax, int BTagsmin, int BTagsmax)
	{
		HTmin_=HTmin;
		HTmax_=HTmax;
		MHTmin_=MHTmin;
		MHTmax_=MHTmax;
		NJetsmin_=NJetsmin;
		NJetsmax_=NJetsmax;
		BTagsmin_=BTagsmin;
		BTagsmax_=BTagsmax;
	}
	double HTmin_, HTmax_, MHTmin_, MHTmax_;
	int NJetsmin_, NJetsmax_, BTagsmin_, BTagsmax_;
	~Bin(){}
private:
};
class SearchBins
{
public:
	SearchBins();
	unsigned int GetBinNumber(double HT, double MHT, int NJets, int BTags);
	
	~SearchBins(){}
protected:
	std::vector<Bin> bins_;
	unsigned int binning;
};
class TH1Feff
{
public:
	TH1Feff(){}
	TH1Feff(const char* name, const char* title, Int_t nbinsx, const double* xbins)
	{
		RefTH1F_ = new 	TH1F(name, title, nbinsx, xbins);
		RefTH1F_->Sumw2();
		name_=name;
		title_=title;
	}
	TH1Feff(const char* name, const char* title, unsigned int nBins, double startBin, double endBin)
	{
		RefTH1F_ = new 	TH1F(name, title, nBins, startBin,endBin);
		RefTH1F_->Sumw2();
		name_=name;
		title_=title;
	}
	TH1F* Clone(){return RefTH1F_;}
	void SetName(const char* title){RefTH1F_->SetName(title); }
	void SetTitle(const char* title){RefTH1F_->SetTitle(title);}
	void Fill(Double_t x,Double_t Weight,bool passOrFail);
	TGraphAsymmErrors* GetEfficiency();
	TH1F* GetSanityCheckTH1F(){return RefTH1F_;}
	void saveResults(TDirectory* MainDirectory);
	~TH1Feff(){}
private:
	TH1F* RefTH1F_;
	vector<Double_t> weights_;
	vector<TH1F*> TH1FFail_, TH1FPass_;
	const char* name_;
	const char* title_;
};

class TH2Feff
{
public:
	TH2Feff(){}
	TH2Feff(const char* name, const char* title, Int_t nbinsx, const Double_t* xbins, Int_t nbinsy, const Double_t* ybins)
	{
		RefTH2F_ = new 	TH2F(name, title, nbinsx, xbins,nbinsy,ybins);
		RefTH2F_->Sumw2();
		nbinsx_=nbinsx; xbins_=xbins;
		nbinsy_=nbinsy; ybins_=ybins;
		name_=name;
		title_=title;
	}
	TH2F* Clone(){return RefTH2F_;}
	void SetName(const char* title){RefTH2F_->SetName(title);}
	void Fill(Double_t x, Double_t y, Double_t Weight,bool passOrFail);
	std::vector<TGraphAsymmErrors*> GetEfficiency();
	TH2F* GetSanityCheckTH2F(){return RefTH2F_;}
	void saveResults(TDirectory* MainDirectory);
	~TH2Feff(){}
private:
	TH2F* RefTH2F_;
	vector<Double_t> weights_;
	vector<TH2F*> TH2FFail_, TH2FPass_;
	Int_t nbinsx_, nbinsy_;
	const Double_t* xbins_;
	const Double_t* ybins_;
	const char* name_;
	const char* title_;
};
class Efficiency : public SearchBins
{
public:
	Efficiency(){}
	Efficiency(const char* name, const char* title);

	void Fill(double HT, double MHT, int NJets, int BTags, double Weight, bool passOrFail);

	void saveResults(TDirectory* MainDirectory);
	~Efficiency(){}
private:	
	const char* name_;
	const char* title_;
	TH1Feff* TH1FSearchBins_;
	std::vector<TH1Feff*> TH1FSearchBinsSplit_;
	unsigned int splitAfter_;
	
};
class SearchBinEventCount : public SearchBins
{
public:
	SearchBinEventCount(){}
	SearchBinEventCount(const char*);
	void Fill(double HT, double MHT, int NJets, int BTags, double Weight);
	void saveResults(TDirectory* MainDirectory);
	~SearchBinEventCount(){}
private:
	TH1F* fullTH1F_;
	std::vector<TH1F*> splitTH1F_;
	unsigned int splitAfter_;
	const char* name_;
};

class EffMaker : public TSelector {
public :	
	TH2F* ratioCalculator(TH2F* passTH2, TH2F* failTH2);
	TH1F* ratioCalculator(TH1F* passTH1, TH1F* failTH1);
	TEfficiency* ratioCalculatorTEff(TH2F* passTH2, TH2F* failTH2);
	TEfficiency* ratioCalculatorTEff(TH1F* passTH1, TH1F* failTH1);
	double MTWCalculator(double metPt,double  metPhi,double  lepPt,double  lepPhi);
	void SaveEfficiency(TH2F *input);
	void SaveEfficiency(TH1F *input);
	TTree          *fChain;   //!pointer to the analyzed TTree or TChain
	
	SearchBinEventCount *totalExpectation_;
	
	// TH efficiencies
	//purity
	//mu
	//1D
	
	
	TH1F *MuPurityBTag_, *MuPurityBTagFail_;
	TH1F *MuPurityNJets_, *MuPurityNJetsFail_;
	TH1F *MuPurityHT_, *MuPurityHTFail_;
	TH1F *MuPurityMHT_, *MuPurityMHTFail_;
	TH1F *MuPurityPT_, *MuPurityPTFail_;
	TH1F *MuPurityActivity_, *MuPurityActivityFail_;
	//2D
	TH2F *MuonPurityMHTNJet, *MuonPurityMHTNJetFail;
	
	//elec
	//1D
	TH1F *ElecPurityBTag_, *ElecPurityBTagFail_;
	TH1F *ElecPurityNJets_, *ElecPurityNJetsFail_;
	TH1F *ElecPurityHT_, *ElecPurityHTFail_;
	TH1F *ElecPurityMHT_, *ElecPurityMHTFail_;
	TH1F *ElecPurityPT_, *ElecPurityPTFail_;
	TH1F *ElecPurityActivity_, *ElecPurityActivityFail_;
	//2D
	TH2F *ElecPurityMHTNJet, *ElecPurityMHTNJetFail;
	
	// x-check of purity
	
	TH1F *MuPurityCheckBTag_, *MuPurityCheckBTagFail_;
	TH1F *MuPurityCheckNJets_, *MuPurityCheckNJetsFail_;
	TH1F *MuPurityCheckHT_, *MuPurityCheckHTFail_;
	TH1F *MuPurityCheckMHT_, *MuPurityCheckMHTFail_;
	TH1F *MuPurityCheckPT_, *MuPurityCheckPTFail_;
	TH1F *MuPurityCheckActivity_, *MuPurityCheckActivityFail_;
	//2D
	TH2F *MuonPurityCheckMHTNJet, *MuonPurityCheckMHTNJetFail;
	
	//elec
	//1D
	TH1F *ElecPurityCheckBTag_, *ElecPurityCheckBTagFail_;
	TH1F *ElecPurityCheckNJets_, *ElecPurityCheckNJetsFail_;
	TH1F *ElecPurityCheckHT_, *ElecPurityCheckHTFail_;
	TH1F *ElecPurityCheckMHT_, *ElecPurityCheckMHTFail_;
	TH1F *ElecPurityCheckPT_, *ElecPurityCheckPTFail_;
	TH1F *ElecPurityCheckActivity_, *ElecPurityCheckActivityFail_;
	//2D
	TH2F *ElecPurityCheckMHTNJet, *ElecPurityCheckMHTNJetFail;
	
	
	// Acceptance
	// mu 
	//1D
	TH1F *MuAccBTag_, *MuAccBTagFail_;
	TH1F *MuAccNJets_, *MuAccNJetsFail_;
	TH1F *MuAccHT_, *MuAccHTFail_;
	TH1F *MuAccMHT_, *MuAccMHTFail_;
	TH1F *MuAccPT_, *MuAccPTFail_;
	TH1F *MuAccActivity_, *MuAccActivityFail_;
	// 2D
	TH2F *MuAccHTNJets_, *MuAccHTNJetsFail_; 
	TH2F *MuAccMHTNJets_, *MuAccMHTNJetsFail_;
	TH2F *MuAccBTagNJets_, *MuAccBTagNJetsFail_;
	TH2F *MuAccMHTNJetsB0_, *MuAccMHTNJetsB0Fail_;
	TH2F *MuAccMHTNJetsB1_Inf_, *MuAccMHTNJetsB1_InfFail_;
	
	// elec
	//1D
	TH1F *ElecAccBTag_, *ElecAccBTagFail_;
	TH1F *ElecAccNJets_, *ElecAccNJetsFail_;
	TH1F *ElecAccHT_, *ElecAccHTFail_;
	TH1F *ElecAccMHT_, *ElecAccMHTFail_;
	TH1F *ElecAccPT_, *ElecAccPTFail_;
	TH1F *ElecAccActivity_, *ElecAccActivityFail_;
	
	// 2D
	TH2F *ElecAccHTNJets_, *ElecAccHTNJetsFail_; 
	TH2F *ElecAccMHTNJets_, *ElecAccMHTNJetsFail_;
	TH2F *ElecAccBTagNJets_, *ElecAccBTagNJetsFail_;
	TH2F *ElecAccMHTNJetsB0_, *ElecAccMHTNJetsB0Fail_;
	TH2F *ElecAccMHTNJetsB1_Inf_, *ElecAccMHTNJetsB1_InfFail_;
	
	// Recoeptance
	// mu 
	//1D
	TH1F *MuRecoBTag_, *MuRecoBTagFail_;
	TH1F *MuRecoNJets_, *MuRecoNJetsFail_;
	TH1F *MuRecoHT_, *MuRecoHTFail_;
	TH1F *MuRecoMHT_, *MuRecoMHTFail_;
	TH1F *MuRecoPT_, *MuRecoPTFail_;
	TH1F *MuRecoActivity_, *MuRecoActivityFail_;
	
	
	// elec
	//1D
	TH1F *ElecRecoBTag_, *ElecRecoBTagFail_;
	TH1F *ElecRecoNJets_, *ElecRecoNJetsFail_;
	TH1F *ElecRecoHT_, *ElecRecoHTFail_;
	TH1F *ElecRecoMHT_, *ElecRecoMHTFail_;
	TH1F *ElecRecoPT_, *ElecRecoPTFail_;
	TH1F *ElecRecoActivity_, *ElecRecoActivityFail_;
	
	// Isoeptance
	// mu 
	//1D
	TH1F *MuIsoBTag_, *MuIsoBTagFail_;
	TH1F *MuIsoNJets_, *MuIsoNJetsFail_;
	TH1F *MuIsoHT_, *MuIsoHTFail_;
	TH1F *MuIsoMHT_, *MuIsoMHTFail_;
	TH1F *MuIsoPT_, *MuIsoPTFail_;
	TH1F *MuIsoActivity_, *MuIsoActivityFail_;
	
	// elec
	//1D
	TH1F *ElecIsoBTag_, *ElecIsoBTagFail_;
	TH1F *ElecIsoNJets_, *ElecIsoNJetsFail_;
	TH1F *ElecIsoHT_, *ElecIsoHTFail_;
	TH1F *ElecIsoMHT_, *ElecIsoMHTFail_;
	TH1F *ElecIsoPT_, *ElecIsoPTFail_;
	TH1F *ElecIsoActivity_, *ElecIsoActivityFail_;
	
	// MTWeptance
	// mu 
	//1D
	TH1F *MuMTWBTag_, *MuMTWBTagFail_;
	TH1F *MuMTWNJets_, *MuMTWNJetsFail_;
	TH1F *MuMTWHT_, *MuMTWHTFail_;
	TH1F *MuMTWMHT_, *MuMTWMHTFail_;
	TH1F *MuMTWPT_, *MuMTWPTFail_;
	TH1F *MuMTWActivity_, *MuMTWActivityFail_;
	// 2D
	TH2F *MuMTWPTActivity_, *MuMTWPTActivityFail_;
	
	// elec
	//1D
	TH1F *ElecMTWBTag_, *ElecMTWBTagFail_;
	TH1F *ElecMTWNJets_, *ElecMTWNJetsFail_;
	TH1F *ElecMTWHT_, *ElecMTWHTFail_;
	TH1F *ElecMTWMHT_, *ElecMTWMHTFail_;
	TH1F *ElecMTWPT_, *ElecMTWPTFail_;
	TH1F *ElecMTWActivity_, *ElecMTWActivityFail_;
	// 2D
	TH2F *ElecMTWPTActivity_, *ElecMTWPTActivityFail_;
	
	
	// Di lep control sample
	// mu 
	//1D
	TH1F *MuDiLepBTag_, *MuDiLepBTagFail_;
	TH1F *MuDiLepNJets_, *MuDiLepNJetsFail_;
	TH1F *MuDiLepHT_, *MuDiLepHTFail_;
	TH1F *MuDiLepMHT_, *MuDiLepMHTFail_;
	TH1F *MuDiLepPT_, *MuDiLepPTFail_;
	TH1F *MuDiLepActivity_, *MuDiLepActivityFail_;
	
	
	// elec
	//1D
	TH1F *ElecDiLepBTag_, *ElecDiLepBTagFail_;
	TH1F *ElecDiLepNJets_, *ElecDiLepNJetsFail_;
	TH1F *ElecDiLepHT_, *ElecDiLepHTFail_;
	TH1F *ElecDiLepMHT_, *ElecDiLepMHTFail_;
	TH1F *ElecDiLepPT_, *ElecDiLepPTFail_;
	TH1F *ElecDiLepActivity_, *ElecDiLepActivityFail_;
	
	// di lep mtw cut applied
	// mu 
	//1D
	TH1F *MuDiLepMTWBTag_, *MuDiLepMTWBTagFail_;
	TH1F *MuDiLepMTWNJets_, *MuDiLepMTWNJetsFail_;
	TH1F *MuDiLepMTWHT_, *MuDiLepMTWHTFail_;
	TH1F *MuDiLepMTWMHT_, *MuDiLepMTWMHTFail_;
	TH1F *MuDiLepMTWPT_, *MuDiLepMTWPTFail_;
	TH1F *MuDiLepMTWActivity_, *MuDiLepMTWActivityFail_;
	
	// elec
	//1D
	TH1F *ElecDiLepMTWBTag_, *ElecDiLepMTWBTagFail_;
	TH1F *ElecDiLepMTWNJets_, *ElecDiLepMTWNJetsFail_;
	TH1F *ElecDiLepMTWHT_, *ElecDiLepMTWHTFail_;
	TH1F *ElecDiLepMTWMHT_, *ElecDiLepMTWMHTFail_;
	TH1F *ElecDiLepMTWPT_, *ElecDiLepMTWPTFail_;
	TH1F *ElecDiLepMTWActivity_, *ElecDiLepMTWActivityFail_;
	
	
	// mu 
	//1D
	TH1F *MuDiLepContributionBTag_, *MuDiLepContributionBTagFail_;
	TH1F *MuDiLepContributionNJets_, *MuDiLepContributionNJetsFail_;
	TH1F *MuDiLepContributionHT_, *MuDiLepContributionHTFail_;
	TH1F *MuDiLepContributionMHT_, *MuDiLepContributionMHTFail_;
	
	// elec
	//1D
	TH1F *ElecDiLepContributionBTag_, *ElecDiLepContributionBTagFail_;
	TH1F *ElecDiLepContributionNJets_, *ElecDiLepContributionNJetsFail_;
	TH1F *ElecDiLepContributionHT_, *ElecDiLepContributionHTFail_;
	TH1F *ElecDiLepContributionMHT_, *ElecDiLepContributionMHTFail_;
	
	// di lep mtw cut applied
	// mu 
	//1D
	TH1F *MuDiLepContributionMTWBTag_, *MuDiLepContributionMTWBTagFail_;
	TH1F *MuDiLepContributionMTWNJets_, *MuDiLepContributionMTWNJetsFail_;
	TH1F *MuDiLepContributionMTWHT_, *MuDiLepContributionMTWHTFail_;
	TH1F *MuDiLepContributionMTWMHT_, *MuDiLepContributionMTWMHTFail_;
	
	// elec
	//1D
	TH1F *ElecDiLepContributionMTWBTag_, *ElecDiLepContributionMTWBTagFail_;
	TH1F *ElecDiLepContributionMTWNJets_, *ElecDiLepContributionMTWNJetsFail_;
	TH1F *ElecDiLepContributionMTWHT_, *ElecDiLepContributionMTWHTFail_;
	TH1F *ElecDiLepContributionMTWMHT_, *ElecDiLepContributionMTWMHTFail_;
	
	// single isolated track from mu or electron
	// muon
	TH1F *IsoTrackMuBTag_, *IsoTrackMuBTagFail_;
	TH1F *IsoTrackMuNJets_, *IsoTrackMuNJetsFail_;
	TH1F *IsoTrackMuHT_, *IsoTrackMuHTFail_;
	TH1F *IsoTrackMuMHT_, *IsoTrackMuMHTFail_;
	TH2F *IsoTrackMuPTActivity_, *IsoTrackMuPTActivityFail_;
	
	TH1F *IsoTrackMuMTWBTag_, *IsoTrackMuMTWBTagFail_;
	TH1F *IsoTrackMuMTWNJets_, *IsoTrackMuMTWNJetsFail_;
	TH1F *IsoTrackMuMTWHT_, *IsoTrackMuMTWHTFail_;
	TH1F *IsoTrackMuMTWMHT_, *IsoTrackMuMTWMHTFail_;
	// match to isolated muon
	TH1F *IsoTrackMuMatchedToIsoMuBTag_, *IsoTrackMuMatchedToIsoMuBTagFail_;
	TH1F *IsoTrackMuMatchedToIsoMuNJets_, *IsoTrackMuMatchedToIsoMuNJetsFail_;
	TH1F *IsoTrackMuMatchedToIsoMuHT_, *IsoTrackMuMatchedToIsoMuHTFail_;
	TH1F *IsoTrackMuMatchedToIsoMuMHT_, *IsoTrackMuMatchedToIsoMuMHTFail_;
	
	// elec
	TH1F *IsoTrackElecBTag_, *IsoTrackElecBTagFail_;
	TH1F *IsoTrackElecNJets_, *IsoTrackElecNJetsFail_;
	TH1F *IsoTrackElecHT_, *IsoTrackElecHTFail_;
	TH1F *IsoTrackElecMHT_, *IsoTrackElecMHTFail_;
	
	TH1F *IsoTrackElecMTWBTag_, *IsoTrackElecMTWBTagFail_;
	TH1F *IsoTrackElecMTWNJets_, *IsoTrackElecMTWNJetsFail_;
	TH1F *IsoTrackElecMTWHT_, *IsoTrackElecMTWHTFail_;
	TH1F *IsoTrackElecMTWMHT_, *IsoTrackElecMTWMHTFail_;
	
	TH2F *IsoTrackElecPTActivity_, *IsoTrackElecPTActivityFail_;
	// match to isolated muon
	TH1F *IsoTrackElecMatchedToIsoElecBTag_, *IsoTrackElecMatchedToIsoElecBTagFail_;
	TH1F *IsoTrackElecMatchedToIsoElecNJets_, *IsoTrackElecMatchedToIsoElecNJetsFail_;
	TH1F *IsoTrackElecMatchedToIsoElecHT_, *IsoTrackElecMatchedToIsoElecHTFail_;
	TH1F *IsoTrackElecMatchedToIsoElecMHT_, *IsoTrackElecMatchedToIsoElecMHTFail_;
	
	// isotrack di lep contribution and efficiency
	//mu contribution
	TH1F *IsoTrackMuDiLepContributionBTag_, *IsoTrackMuDiLepContributionBTagFail_;
	TH1F *IsoTrackMuDiLepContributionNJets_, *IsoTrackMuDiLepContributionNJetsFail_;
	TH1F *IsoTrackMuDiLepContributionHT_, *IsoTrackMuDiLepContributionHTFail_;
	TH1F *IsoTrackMuDiLepContributionMHT_, *IsoTrackMuDiLepContributionMHTFail_;
	// efficiency
	TH1F *IsoTrackMuDiLepEffBTag_, *IsoTrackMuDiLepEffBTagFail_;
	TH1F *IsoTrackMuDiLepEffNJets_, *IsoTrackMuDiLepEffNJetsFail_;
	TH1F *IsoTrackMuDiLepEffHT_, *IsoTrackMuDiLepEffHTFail_;
	TH1F *IsoTrackMuDiLepEffMHT_, *IsoTrackMuDiLepEffMHTFail_;
	TH1F *IsoTrackMuDiLepEffPT_, *IsoTrackMuDiLepEffPTFail_;
	TH1F *IsoTrackMuDiLepEffActivity_, *IsoTrackMuDiLepEffActivityFail_;
	
	//elec contribution
	TH1F *IsoTrackElecDiLepContributionBTag_, *IsoTrackElecDiLepContributionBTagFail_;
	TH1F *IsoTrackElecDiLepContributionNJets_, *IsoTrackElecDiLepContributionNJetsFail_;
	TH1F *IsoTrackElecDiLepContributionHT_, *IsoTrackElecDiLepContributionHTFail_;
	TH1F *IsoTrackElecDiLepContributionMHT_, *IsoTrackElecDiLepContributionMHTFail_;
	// efficiency
	TH1F *IsoTrackElecDiLepEffBTag_, *IsoTrackElecDiLepEffBTagFail_;
	TH1F *IsoTrackElecDiLepEffNJets_, *IsoTrackElecDiLepEffNJetsFail_;
	TH1F *IsoTrackElecDiLepEffHT_, *IsoTrackElecDiLepEffHTFail_;
	TH1F *IsoTrackElecDiLepEffMHT_, *IsoTrackElecDiLepEffMHTFail_;
	TH1F *IsoTrackElecDiLepEffPT_, *IsoTrackElecDiLepEffPTFail_;
	TH1F *IsoTrackElecDiLepEffActivity_, *IsoTrackElecDiLepEffActivityFail_;
	
	// 2D efficiencies
	TH2F *MuIsoPTActivity_, *MuIsoPTActivityFail_;
	TH2F *MuRecoPTActivity_, *MuRecoPTActivityFail_;
	
	TH2F *ElecIsoPTActivity_, *ElecIsoPTActivityFail_;
	TH2F *ElecRecoPTActivity_, *ElecRecoPTActivityFail_;
	
	// TEff used efficienies
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	TH1Feff *MuPurityBTagEff_;
	TH1Feff *MuPurityNJetsEff_;
	TH1Feff *MuPurityHTEff_;
	TH1Feff *MuPurityMHTEff_;
	TH1Feff *MuPurityPTEff_;
	TH1Feff *MuPurityActivityEff_;
	//2D
	TH2Feff *MuonPurityMHTNJetEff_;
	
	//elec
	//1D
	TH1Feff *ElecPurityBTagEff_;
	TH1Feff *ElecPurityNJetsEff_;
	TH1Feff *ElecPurityHTEff_;
	TH1Feff *ElecPurityMHTEff_;
	TH1Feff *ElecPurityPTEff_;
	TH1Feff *ElecPurityActivityEff_;
	//2D
	TH2Feff *ElecPurityMHTNJetEff_;
	
	// Acceptance
	// mu 
	//1D
	TH1Feff *MuAccBTagEff_;
	TH1Feff *MuAccNJetsEff_;
	TH1Feff *MuAccHTEff_;
	TH1Feff *MuAccMHTEff_;
	TH1Feff *MuAccPTEff_;
	TH1Feff *MuAccActivityEff_;
	// 2D
	TH2Feff *MuAccHTNJetsEff_; 
	TH2Feff *MuAccMHTNJetsEff_;
	TH2Feff *MuAccBTagNJetsEff_;
	
	// elec
	//1D
	TH1Feff *ElecAccBTagEff_;
	TH1Feff *ElecAccNJetsEff_;
	TH1Feff *ElecAccHTEff_;
	TH1Feff *ElecAccMHTEff_;
	TH1Feff *ElecAccPTEff_;
	TH1Feff *ElecAccActivityEff_;
	
	// 2D
	TH2Feff *ElecAccHTNJetsEff_; 
	TH2Feff *ElecAccMHTNJetsEff_;
	TH2Feff *ElecAccBTagNJetsEff_;
	
	// Recoeptance
	// mu 
	//1D
	TH1Feff *MuRecoBTagEff_;
	TH1Feff *MuRecoNJetsEff_;
	TH1Feff *MuRecoHTEff_;
	TH1Feff *MuRecoMHTEff_;
	TH1Feff *MuRecoPTEff_;
	TH1Feff *MuRecoActivityEff_;
	
	
	// elec
	//1D
	TH1Feff *ElecRecoBTagEff_, *ElecRecoBTagFailEff_;
	TH1Feff *ElecRecoNJetsEff_, *ElecRecoNJetsFailEff_;
	TH1Feff *ElecRecoHTEff_, *ElecRecoHTFailEff_;
	TH1Feff *ElecRecoMHTEff_, *ElecRecoMHTFailEff_;
	TH1Feff *ElecRecoPTEff_, *ElecRecoPTFailEff_;
	TH1Feff *ElecRecoActivityEff_, *ElecRecoActivityFailEff_;
	
	// Isoeptance
	// mu 
	//1D
	TH1Feff *MuIsoBTagEff_, *MuIsoBTagFailEff_;
	TH1Feff *MuIsoNJetsEff_, *MuIsoNJetsFailEff_;
	TH1Feff *MuIsoHTEff_, *MuIsoHTFailEff_;
	TH1Feff *MuIsoMHTEff_, *MuIsoMHTFailEff_;
	TH1Feff *MuIsoPTEff_, *MuIsoPTFailEff_;
	TH1Feff *MuIsoActivityEff_, *MuIsoActivityFailEff_;
	
	// elec
	//1D
	TH1Feff *ElecIsoBTagEff_, *ElecIsoBTagFailEff_;
	TH1Feff *ElecIsoNJetsEff_, *ElecIsoNJetsFailEff_;
	TH1Feff *ElecIsoHTEff_, *ElecIsoHTFailEff_;
	TH1Feff *ElecIsoMHTEff_, *ElecIsoMHTFailEff_;
	TH1Feff *ElecIsoPTEff_, *ElecIsoPTFailEff_;
	TH1Feff *ElecIsoActivityEff_, *ElecIsoActivityFailEff_;
	
	// MTWeptance
	// mu 
	//1D
	TH1Feff *MuMTWBTagEff_, *MuMTWBTagFailEff_;
	TH1Feff *MuMTWNJetsEff_, *MuMTWNJetsFailEff_;
	TH1Feff *MuMTWHTEff_, *MuMTWHTFailEff_;
	TH1Feff *MuMTWMHTEff_, *MuMTWMHTFailEff_;
	TH1Feff *MuMTWPTEff_, *MuMTWPTFailEff_;
	TH1Feff *MuMTWActivityEff_, *MuMTWActivityFailEff_;
	// 2D
	TH2Feff *MuMTWPTActivityEff_, *MuMTWPTActivityFailEff_;
	
	// elec
	//1D
	TH1Feff *ElecMTWBTagEff_, *ElecMTWBTagFailEff_;
	TH1Feff *ElecMTWNJetsEff_, *ElecMTWNJetsFailEff_;
	TH1Feff *ElecMTWHTEff_, *ElecMTWHTFailEff_;
	TH1Feff *ElecMTWMHTEff_, *ElecMTWMHTFailEff_;
	TH1Feff *ElecMTWPTEff_, *ElecMTWPTFailEff_;
	TH1Feff *ElecMTWActivityEff_, *ElecMTWActivityFailEff_;
	// 2D
	TH2Feff *ElecMTWPTActivityEff_, *ElecMTWPTActivityFailEff_;
	
	
	// Di lep control sample
	// mu 
	//1D
	TH1Feff *MuDiLepBTagEff_, *MuDiLepBTagFailEff_;
	TH1Feff *MuDiLepNJetsEff_, *MuDiLepNJetsFailEff_;
	TH1Feff *MuDiLepHTEff_, *MuDiLepHTFailEff_;
	TH1Feff *MuDiLepMHTEff_, *MuDiLepMHTFailEff_;
	TH1Feff *MuDiLepPTEff_, *MuDiLepPTFailEff_;
	TH1Feff *MuDiLepActivityEff_, *MuDiLepActivityFailEff_;
	
	
	// elec
	//1D
	TH1Feff *ElecDiLepBTagEff_, *ElecDiLepBTagFailEff_;
	TH1Feff *ElecDiLepNJetsEff_, *ElecDiLepNJetsFailEff_;
	TH1Feff *ElecDiLepHTEff_, *ElecDiLepHTFailEff_;
	TH1Feff *ElecDiLepMHTEff_, *ElecDiLepMHTFailEff_;
	TH1Feff *ElecDiLepPTEff_, *ElecDiLepPTFailEff_;
	TH1Feff *ElecDiLepActivityEff_, *ElecDiLepActivityFailEff_;
	
	// di lep mtw cut applied
	// mu 
	//1D
	TH1Feff *MuDiLepMTWBTagEff_, *MuDiLepMTWBTagFailEff_;
	TH1Feff *MuDiLepMTWNJetsEff_, *MuDiLepMTWNJetsFailEff_;
	TH1Feff *MuDiLepMTWHTEff_, *MuDiLepMTWHTFailEff_;
	TH1Feff *MuDiLepMTWMHTEff_, *MuDiLepMTWMHTFailEff_;
	TH1Feff *MuDiLepMTWPTEff_, *MuDiLepMTWPTFailEff_;
	TH1Feff *MuDiLepMTWActivityEff_, *MuDiLepMTWActivityFailEff_;
	
	// elec
	//1D
	TH1Feff *ElecDiLepMTWBTagEff_, *ElecDiLepMTWBTagFailEff_;
	TH1Feff *ElecDiLepMTWNJetsEff_, *ElecDiLepMTWNJetsFailEff_;
	TH1Feff *ElecDiLepMTWHTEff_, *ElecDiLepMTWHTFailEff_;
	TH1Feff *ElecDiLepMTWMHTEff_, *ElecDiLepMTWMHTFailEff_;
	TH1Feff *ElecDiLepMTWPTEff_, *ElecDiLepMTWPTFailEff_;
	TH1Feff *ElecDiLepMTWActivityEff_, *ElecDiLepMTWActivityFailEff_;
	
	
	// mu 
	//1D
	TH1Feff *MuDiLepContributionBTagEff_, *MuDiLepContributionBTagFailEff_;
	TH1Feff *MuDiLepContributionNJetsEff_, *MuDiLepContributionNJetsFailEff_;
	TH1Feff *MuDiLepContributionHTEff_, *MuDiLepContributionHTFailEff_;
	TH1Feff *MuDiLepContributionMHTEff_, *MuDiLepContributionMHTFailEff_;
	
	// elec
	//1D
	TH1Feff *ElecDiLepContributionBTagEff_, *ElecDiLepContributionBTagFailEff_;
	TH1Feff *ElecDiLepContributionNJetsEff_, *ElecDiLepContributionNJetsFailEff_;
	TH1Feff *ElecDiLepContributionHTEff_, *ElecDiLepContributionHTFailEff_;
	TH1Feff *ElecDiLepContributionMHTEff_, *ElecDiLepContributionMHTFailEff_;
	
	// di lep mtw cut applied
	// mu 
	//1D
	TH1Feff *MuDiLepContributionMTWBTagEff_, *MuDiLepContributionMTWBTagFailEff_;
	TH1Feff *MuDiLepContributionMTWNJetsEff_, *MuDiLepContributionMTWNJetsFailEff_;
	TH1Feff *MuDiLepContributionMTWHTEff_, *MuDiLepContributionMTWHTFailEff_;
	TH1Feff *MuDiLepContributionMTWMHTEff_, *MuDiLepContributionMTWMHTFailEff_;
	
	// elec
	//1D
	TH1Feff *ElecDiLepContributionMTWBTagEff_, *ElecDiLepContributionMTWBTagFailEff_;
	TH1Feff *ElecDiLepContributionMTWNJetsEff_, *ElecDiLepContributionMTWNJetsFailEff_;
	TH1Feff *ElecDiLepContributionMTWHTEff_, *ElecDiLepContributionMTWHTFailEff_;
	TH1Feff *ElecDiLepContributionMTWMHTEff_, *ElecDiLepContributionMTWMHTFailEff_;
	
	// single isolated track from mu or electron
	// muon
	TH1Feff *IsoTrackMuBTagEff_, *IsoTrackMuBTagFailEff_;
	TH1Feff *IsoTrackMuNJetsEff_, *IsoTrackMuNJetsFailEff_;
	TH1Feff *IsoTrackMuHTEff_, *IsoTrackMuHTFailEff_;
	TH1Feff *IsoTrackMuMHTEff_, *IsoTrackMuMHTFailEff_;
	
	TH1Feff *IsoTrackMuMTWBTagEff_, *IsoTrackMuMTWBTagFailEff_;
	TH1Feff *IsoTrackMuMTWNJetsEff_, *IsoTrackMuMTWNJetsFailEff_;
	TH1Feff *IsoTrackMuMTWHTEff_, *IsoTrackMuMTWHTFailEff_;
	TH1Feff *IsoTrackMuMTWMHTEff_, *IsoTrackMuMTWMHTFailEff_;
	// match to isolated muon
	TH1Feff *IsoTrackMuMatchedToIsoMuBTagEff_, *IsoTrackMuMatchedToIsoMuBTagFailEff_;
	TH1Feff *IsoTrackMuMatchedToIsoMuNJetsEff_, *IsoTrackMuMatchedToIsoMuNJetsFailEff_;
	TH1Feff *IsoTrackMuMatchedToIsoMuHTEff_, *IsoTrackMuMatchedToIsoMuHTFailEff_;
	TH1Feff *IsoTrackMuMatchedToIsoMuMHTEff_, *IsoTrackMuMatchedToIsoMuMHTFailEff_;
	
	// elec
	TH1Feff *IsoTrackElecBTagEff_, *IsoTrackElecBTagFailEff_;
	TH1Feff *IsoTrackElecNJetsEff_, *IsoTrackElecNJetsFailEff_;
	TH1Feff *IsoTrackElecHTEff_, *IsoTrackElecHTFailEff_;
	TH1Feff *IsoTrackElecMHTEff_, *IsoTrackElecMHTFailEff_;
	
	TH1Feff *IsoTrackElecMTWBTagEff_, *IsoTrackElecMTWBTagFailEff_;
	TH1Feff *IsoTrackElecMTWNJetsEff_, *IsoTrackElecMTWNJetsFailEff_;
	TH1Feff *IsoTrackElecMTWHTEff_, *IsoTrackElecMTWHTFailEff_;
	TH1Feff *IsoTrackElecMTWMHTEff_, *IsoTrackElecMTWMHTFailEff_;
	// match to isolated muon
	TH1Feff *IsoTrackElecMatchedToIsoElecBTagEff_, *IsoTrackElecMatchedToIsoElecBTagFailEff_;
	TH1Feff *IsoTrackElecMatchedToIsoElecNJetsEff_, *IsoTrackElecMatchedToIsoElecNJetsFailEff_;
	TH1Feff *IsoTrackElecMatchedToIsoElecHTEff_, *IsoTrackElecMatchedToIsoElecHTFailEff_;
	TH1Feff *IsoTrackElecMatchedToIsoElecMHTEff_, *IsoTrackElecMatchedToIsoElecMHTFailEff_;
	
	// isotrack di lep contribution and efficiency
	//mu contribution
	TH1Feff *IsoTrackMuDiLepContributionBTagEff_, *IsoTrackMuDiLepContributionBTagFailEff_;
	TH1Feff *IsoTrackMuDiLepContributionNJetsEff_, *IsoTrackMuDiLepContributionNJetsFailEff_;
	TH1Feff *IsoTrackMuDiLepContributionHTEff_, *IsoTrackMuDiLepContributionHTFailEff_;
	TH1Feff *IsoTrackMuDiLepContributionMHTEff_, *IsoTrackMuDiLepContributionMHTFailEff_;
	// efficiency
	TH1Feff *IsoTrackMuDiLepEff_BTagEff_, *IsoTrackMuDiLepEff_BTagFailEff_;
	TH1Feff *IsoTrackMuDiLepEff_NJetsEff_, *IsoTrackMuDiLepEff_NJetsFailEff_;
	TH1Feff *IsoTrackMuDiLepEff_HTEff_, *IsoTrackMuDiLepEff_HTFailEff_;
	TH1Feff *IsoTrackMuDiLepEff_MHTEff_, *IsoTrackMuDiLepEff_MHTFailEff_;
	TH1Feff *IsoTrackMuDiLepEff_PTEff_, *IsoTrackMuDiLepEff_PTFailEff_;
	TH1Feff *IsoTrackMuDiLepEff_ActivityEff_, *IsoTrackMuDiLepEff_ActivityFailEff_;
	
	//elec contribution
	TH1Feff *IsoTrackElecDiLepContributionBTagEff_, *IsoTrackElecDiLepContributionBTagFailEff_;
	TH1Feff *IsoTrackElecDiLepContributionNJetsEff_, *IsoTrackElecDiLepContributionNJetsFailEff_;
	TH1Feff *IsoTrackElecDiLepContributionHTEff_, *IsoTrackElecDiLepContributionHTFailEff_;
	TH1Feff *IsoTrackElecDiLepContributionMHTEff_, *IsoTrackElecDiLepContributionMHTFailEff_;
	// efficiency
	TH1Feff *IsoTrackElecDiLepEff_BTagEff_, *IsoTrackElecDiLepEff_BTagFailEff_;
	TH1Feff *IsoTrackElecDiLepEff_NJetsEff_, *IsoTrackElecDiLepEff_NJetsFailEff_;
	TH1Feff *IsoTrackElecDiLepEff_HTEff_, *IsoTrackElecDiLepEff_HTFailEff_;
	TH1Feff *IsoTrackElecDiLepEff_MHTEff_, *IsoTrackElecDiLepEff_MHTFailEff_;
	TH1Feff *IsoTrackElecDiLepEff_PTEff_, *IsoTrackElecDiLepEff_PTFailEff_;
	TH1Feff *IsoTrackElecDiLepEff_ActivityEff_, *IsoTrackElecDiLepEff_ActivityFailEff_;
	
	// 2D efficiencies
	TH2Feff *MuIsoPTActivityEff_, *MuIsoPTActivityFailEff_;
	TH2Feff *MuRecoPTActivityEff_, *MuRecoPTActivityFailEff_;
	
	TH2Feff *ElecIsoPTActivityEff_, *ElecIsoPTActivityFailEff_;
	TH2Feff *ElecRecoPTActivityEff_, *ElecRecoPTActivityFailEff_;
	
	// ExpectationReductionIsoTrack 
	//1D
	TH1Feff *ExpectationReductionIsoTrackBTagEff_, *ExpectationReductionIsoTrackBTagFailEff_;
	TH1Feff *ExpectationReductionIsoTrackNJetsEff_, *ExpectationReductionIsoTrackNJetsFailEff_;
	TH1Feff *ExpectationReductionIsoTrackHTEff_, *ExpectationReductionIsoTrackHTFailEff_;
	TH1Feff *ExpectationReductionIsoTrackMHTEff_, *ExpectationReductionIsoTrackMHTFailEff_;
	
	
	TH1F *ExpectationReductionIsoTrackBTagEff, *ExpectationReductionIsoTrackBTagEffFail;
	TH1F *ExpectationReductionIsoTrackNJetsEff, *ExpectationReductionIsoTrackNJetsEffFail;
	TH1F *ExpectationReductionIsoTrackHTEff, *ExpectationReductionIsoTrackHTEffFail;
	TH1F *ExpectationReductionIsoTrackMHTEff, *ExpectationReductionIsoTrackMHTEffFail;
	TH1F *ExpectationReductionIsoTrackPTEff, *ExpectationReductionIsoTrackPTEffFail;
	TH1F *ExpectationReductionIsoTrackActivityEff, *ExpectationReductionIsoTrackActivityEffFail;
	//2D
	// 2D
	TH2F *IsoTrackReductionHTNJets_, *IsoTrackReductionHTNJetsFail_; 
	TH2F *IsoTrackReductionMHTNJets_, *IsoTrackReductionMHTNJetsFail_;
	TH2F *IsoTrackReductionBTagNJets_, *IsoTrackReductionBTagNJetsFail_;
	TH2F *IsoTrackReductionPTActivity_, *IsoTrackReductionPTActivityFail_;
	
	// ************************************************************************************************************* 22 June 2015 ****************************************************
	
	// mu iso tracks
	TH1F *ExpectationReductionMuIsoTrackBTagEff, *ExpectationReductionMuIsoTrackBTagEffFail;
	TH1F *ExpectationReductionMuIsoTrackNJetsEff, *ExpectationReductionMuIsoTrackNJetsEffFail;
	TH1F *ExpectationReductionMuIsoTrackHTEff, *ExpectationReductionMuIsoTrackHTEffFail;
	TH1F *ExpectationReductionMuIsoTrackMHTEff, *ExpectationReductionMuIsoTrackMHTEffFail;
	TH1F *ExpectationReductionMuIsoTrackPTEff, *ExpectationReductionMuIsoTrackPTEffFail;
	TH1F *ExpectationReductionMuIsoTrackActivityEff, *ExpectationReductionMuIsoTrackActivityEffFail;
	//2D
	// 2D
	TH2F *MuIsoTrackReductionHTNJets_, *MuIsoTrackReductionHTNJetsFail_; 
	TH2F *MuIsoTrackReductionMHTNJets_, *MuIsoTrackReductionMHTNJetsFail_;
	TH2F *MuIsoTrackReductionBTagNJets_, *MuIsoTrackReductionBTagNJetsFail_;
	TH2F *MuIsoTrackReductionPTActivity_, *MuIsoTrackReductionPTActivityFail_;
	
	//elec iso tracks
	TH1F *ExpectationReductionElecIsoTrackBTagEff, *ExpectationReductionElecIsoTrackBTagEffFail;
	TH1F *ExpectationReductionElecIsoTrackNJetsEff, *ExpectationReductionElecIsoTrackNJetsEffFail;
	TH1F *ExpectationReductionElecIsoTrackHTEff, *ExpectationReductionElecIsoTrackHTEffFail;
	TH1F *ExpectationReductionElecIsoTrackMHTEff, *ExpectationReductionElecIsoTrackMHTEffFail;
	TH1F *ExpectationReductionElecIsoTrackPTEff, *ExpectationReductionElecIsoTrackPTEffFail;
	TH1F *ExpectationReductionElecIsoTrackActivityEff, *ExpectationReductionElecIsoTrackActivityEffFail;
	//2D
	// 2D
	TH2F *ElecIsoTrackReductionHTNJets_, *ElecIsoTrackReductionHTNJetsFail_; 
	TH2F *ElecIsoTrackReductionMHTNJets_, *ElecIsoTrackReductionMHTNJetsFail_;
	TH2F *ElecIsoTrackReductionBTagNJets_, *ElecIsoTrackReductionBTagNJetsFail_;
	TH2F *ElecIsoTrackReductionPTActivity_, *ElecIsoTrackReductionPTActivityFail_;
	
	//pion iso tracks
	TH1F *ExpectationReductionPionIsoTrackBTagEff, *ExpectationReductionPionIsoTrackBTagEffFail;
	TH1F *ExpectationReductionPionIsoTrackNJetsEff, *ExpectationReductionPionIsoTrackNJetsEffFail;
	TH1F *ExpectationReductionPionIsoTrackHTEff, *ExpectationReductionPionIsoTrackHTEffFail;
	TH1F *ExpectationReductionPionIsoTrackMHTEff, *ExpectationReductionPionIsoTrackMHTEffFail;
	TH1F *ExpectationReductionPionIsoTrackPTEff, *ExpectationReductionPionIsoTrackPTEffFail;
	TH1F *ExpectationReductionPionIsoTrackActivityEff, *ExpectationReductionPionIsoTrackActivityEffFail;
	//2D
	// 2D
	TH2F *PionIsoTrackReductionHTNJets_, *PionIsoTrackReductionHTNJetsFail_; 
	TH2F *PionIsoTrackReductionMHTNJets_, *PionIsoTrackReductionMHTNJetsFail_;
	TH2F *PionIsoTrackReductionBTagNJets_, *PionIsoTrackReductionBTagNJetsFail_;
	TH2F *PionIsoTrackReductionPTActivity_, *PionIsoTrackReductionPTActivityFail_;
	// ************************************************************************************************************* 22 June 2015 end ****************************************************
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// delta R relPT
	TH1F *MuIsoPTJetRel_, *MuIsoPTJetRelFail_;
	TH1F *MuIsoDeltaRJet_, *MuIsoDeltaRJetFail_;
	TH1F *MuRecoPTJetRel_, *MuRecoPTJetRelFail_;
	TH1F *MuRecoDeltaRJet_, *MuRecoDeltaRJetFail_;
	TH2F *MuIsoDeltaRRelPTJet_, *MuIsoDeltaRRelPTJetFail_;
	TH2F *MuRecoDeltaRRelPTJet_, *MuRecoDeltaRRelPTJetFail_;
	
	TH1F *ElecIsoPTJetRel_, *ElecIsoPTJetRelFail_;
	TH1F *ElecIsoDeltaRJet_, *ElecIsoDeltaRJetFail_;
	TH1F *ElecRecoPTJetRel_, *ElecRecoPTJetRelFail_;
	TH1F *ElecRecoDeltaRJet_, *ElecRecoDeltaRJetFail_;
	TH2F *ElecIsoDeltaRRelPTJet_, *ElecIsoDeltaRRelPTJetFail_;
	TH2F *ElecRecoDeltaRRelPTJet_, *ElecRecoDeltaRRelPTJetFail_;
	
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Search bin efficiencies
	Efficiency *MuAccSearchBinEff_, *MuRecoSearchBinEff_, *MuIsoSearchBinEff_, *MuMTWSearchBinEff_, *MuDiLepContributionMTWAppliedSearchBinEff_, *MuDiLepEffMTWAppliedSearchBinEff_, *MuPuritySearchBinEff_;
	Efficiency *ElecAccSearchBinEff_, *ElecRecoSearchBinEff_, *ElecIsoSearchBinEff_, *ElecMTWSearchBinEff_, *ElecDiLepContributionMTWAppliedSearchBinEff_, *ElecDiLepEffMTWAppliedSearchBinEff_, *ElecPuritySearchBinEff_;
	
	Efficiency *ExpectationReductionIsoTrack_;
	
   // Declaration of leaf types
	// Declaration of leaf types
	UInt_t          EvtNum;
	Float_t         HT;
	Float_t         MHT;
	UShort_t        NJets;
	UShort_t        BTags;
	UShort_t        Leptons;
	UShort_t        isoTracks;
	UChar_t         DY;
	UShort_t        Bin;
	UShort_t        NVtx;
	Float_t         DeltaPhi1;
	Float_t         DeltaPhi2;
	Float_t         DeltaPhi3;
	Float_t         minDeltaPhiN;
	Float_t         DeltaPhiN1;
	Float_t         DeltaPhiN2;
	Float_t         DeltaPhiN3;
	Float_t         Weight;
	Float_t         MET;
	Float_t         METPhi;
	UShort_t        GenMuNum;
	UShort_t        GenMuFromTau[2];   //[GenMuNum]
	Float_t         GenMuDeltaRJet[2];   //[GenMuNum]
	Float_t         GenMuDeltaPTJet[2];   //[GenMuNum]
	Float_t         GenMuPt[2];   //[GenMuNum]
	Float_t         GenMuEta[2];   //[GenMuNum]
	Float_t         GenMuPhi[2];   //[GenMuNum]
	UShort_t        GenMuonIsoTrackMatched[2];   //[GenMuNum]
	Float_t         GenMuonActivity[2];   //[GenMuNum]
	UShort_t        GenElecNum;
	UShort_t        GenElecFromTau[2];   //[GenElecNum]
	Float_t         GenElecDeltaRJet[2];   //[GenElecNum]
	Float_t         GenElecDeltaPTJet[2];   //[GenElecNum]
	Float_t         GenElecPt[2];   //[GenElecNum]
	Float_t         GenElecEta[2];   //[GenElecNum]
	Float_t         GenElecPhi[2];   //[GenElecNum]
	UShort_t        GenElecIsoTrackMatched[2];   //[GenElecNum]
	Float_t         GenElecActivity[2];   //[GenElecNum]
	UShort_t        GenTauNum;
	Float_t         GenTauPt[2];   //[GenTauNum]
	Float_t         GenTauEta[2];   //[GenTauNum]
	Float_t         GenTauPhi[2];   //[GenTauNum]
	UShort_t        GenTauIsoTrackMatched[2];   //[GenTauNum]
	Float_t         GenTauActivity[2];   //[GenTauNum]
	UShort_t        Expectation;
	UShort_t        ExpectationReductionIsoTrack;
	UShort_t        muAcc;
	UShort_t        muReco;
	UShort_t        muIso;
	Float_t         MTW;
	Float_t         IsoTrackMTW;
	UShort_t        elecAcc;
	UShort_t        elecReco;
	UShort_t        elecIso;
	UShort_t        muIsoTrack;
	UShort_t        MuPurity;
	UShort_t        ElecPurity;
	UShort_t        muActivityMethod;
	UShort_t        elecActivityMethod;
	UShort_t        muIsoTrackActivityMethod;
	UShort_t        elecIsoTrackActivityMethod;
	UShort_t        pionIsoTrackActivityMethod;
	UShort_t        FallsVetoLep;
	UShort_t        selectedIDIsoMuonsNum;
	Float_t         selectedIDIsoMuonsPt[3];   //[selectedIDIsoMuonsNum]
	UShort_t        muIsoMatched[3];   //[selectedIDIsoMuonsNum]
	UShort_t        RecoIsoMuonPromtMatched[3];   //[selectedIDIsoMuonsNum]
	Float_t         RecoIsoMuonPromtMatchedDeltaR[3];   //[selectedIDIsoMuonsNum]
	Float_t         RecoIsoMuonPromtMatchedRelPt[3];   //[selectedIDIsoMuonsNum]
	Float_t         selectedIDIsoMuonsEta[3];   //[selectedIDIsoMuonsNum]
	Float_t         selectedIDIsoMuonsPhi[3];   //[selectedIDIsoMuonsNum]
	Float_t         selectedIDIsoMuonsE[3];   //[selectedIDIsoMuonsNum]
	Float_t         selectedIDIsoMuons_MTW[3];   //[selectedIDIsoMuonsNum]
	Float_t         RecoIsoMuonActivity[3];   //[selectedIDIsoMuonsNum]
	UShort_t        selectedIDMuonsNum;
	Float_t         selectedIDMuonsPt[5];   //[selectedIDMuonsNum]
	UShort_t        selectedIDMuonsMatched[5];   //[selectedIDMuonsNum]
	Float_t         selectedIDMuonsEta[5];   //[selectedIDMuonsNum]
	Float_t         selectedIDMuonsPhi[5];   //[selectedIDMuonsNum]
	Float_t         selectedIDMuonsE[5];   //[selectedIDMuonsNum]
	Float_t         selectedIDMuons_MTW[3];   //[selectedIDIsoMuonsNum]
	Float_t         RecoMuonActivity[5];   //[selectedIDMuonsNum]
	UShort_t        RecoIsoMuonIsoTrackMatched[3];   //[selectedIDIsoMuonsNum]
	UShort_t        selectedIDIsoElectronsNum;
	Float_t         selectedIDIsoElectronsPt[3];   //[selectedIDIsoElectronsNum]
	UShort_t        elecIsoMatched[3];   //[selectedIDIsoElectronsNum]
	UShort_t        RecoIsoElecPromtMatched[3];   //[selectedIDIsoElectronsNum]
	Float_t         RecoIsoElecPromtMatchedDeltaR[3];   //[selectedIDIsoElectronsNum]
	Float_t         RecoIsoElecPromtMatchedRelPt[3];   //[selectedIDIsoElectronsNum]
	Float_t         selectedIDIsoElectronsEta[3];   //[selectedIDIsoElectronsNum]
	Float_t         selectedIDIsoElectronsPhi[3];   //[selectedIDIsoElectronsNum]
	Float_t         selectedIDIsoElectronsE[3];   //[selectedIDIsoElectronsNum]
	Float_t         selectedIDIsoElectrons_MTW[3];   //[selectedIDIsoElectronsNum]
	Float_t         RecoIsoElectronActivity[3];   //[selectedIDIsoElectronsNum]
	UShort_t        RecoIsoElecIsoTrackMatched[3];   //[selectedIDIsoElectronsNum]
	UShort_t        selectedIDElectronsNum;
	Float_t         selectedIDElectronsPt[4];   //[selectedIDElectronsNum]
	UShort_t        selectedIDElectronsMatched[4];   //[selectedIDElectronsNum]
	Float_t         selectedIDElectronsEta[4];   //[selectedIDElectronsNum]
	Float_t         selectedIDElectronsPhi[4];   //[selectedIDElectronsNum]
	Float_t         selectedIDElectronsE[4];   //[selectedIDElectronsNum]
	Float_t         selectedIDElectrons_MTW[3];   //[selectedIDIsoElectronsNum]
	Float_t         RecoElectronActivity[4];   //[selectedIDElectronsNum]
	UShort_t        IsolatedElectronTracksVetoNum;
	Float_t         IsolatedElectronTracksVetoPt[3];   //[IsolatedElectronTracksVetoNum]
	Float_t         IsolatedElectronTracksVetoEta[3];   //[IsolatedElectronTracksVetoNum]
	Float_t         IsolatedElectronTracksVetoPhi[3];   //[IsolatedElectronTracksVetoNum]
	Float_t         IsolatedElectronTracksVetoE[3];   //[IsolatedElectronTracksVetoNum]
	Float_t         IsolatedElectronTracksVetoActivity[3];   //[IsolatedElectronTracksVetoNum]
	Float_t         IsolatedElectronTracksVetoMTW[3];   //[IsolatedElectronTracksVetoNum]
	UShort_t        muIsoTrackMatchedToGenElec[3];   //[IsolatedElectronTracksVetoNum]
	UShort_t        elecIsoTrackMatchedToGenElec[3];   //[IsolatedElectronTracksVetoNum]
	UShort_t        pionIsoTrackMatchedToGenElec[3];   //[IsolatedElectronTracksVetoNum]
	UShort_t        IsolatedMuonTracksVetoNum;
	Float_t         IsolatedMuonTracksVetoPt[3];   //[IsolatedMuonTracksVetoNum]
	Float_t         IsolatedMuonTracksVetoEta[3];   //[IsolatedMuonTracksVetoNum]
	Float_t         IsolatedMuonTracksVetoPhi[3];   //[IsolatedMuonTracksVetoNum]
	Float_t         IsolatedMuonTracksVetoE[3];   //[IsolatedMuonTracksVetoNum]
	Float_t         IsolatedMuonTracksVetoActivity[3];   //[IsolatedMuonTracksVetoNum]
	Float_t         IsolatedMuonTracksVetoMTW[3];   //[IsolatedMuonTracksVetoNum]
	UShort_t        muIsoTrackMatchedToGenMu[3];   //[IsolatedMuonTracksVetoNum]
	UShort_t        elecIsoTrackMatchedToGenMu[3];   //[IsolatedMuonTracksVetoNum]
	UShort_t        pionIsoTrackMatchedToGenMu[3];   //[IsolatedMuonTracksVetoNum]
	UShort_t        IsolatedPionTracksVetoNum;
	Float_t         IsolatedPionTracksVetoPt[4];   //[IsolatedPionTracksVetoNum]
	Float_t         IsolatedPionTracksVetoEta[4];   //[IsolatedPionTracksVetoNum]
	Float_t         IsolatedPionTracksVetoPhi[4];   //[IsolatedPionTracksVetoNum]
	Float_t         IsolatedPionTracksVetoE[4];   //[IsolatedPionTracksVetoNum]
	Float_t         IsolatedPionTracksVetoActivity[4];   //[IsolatedPionTracksVetoNum]
	Float_t         IsolatedPionTracksVetoMTW[4];   //[IsolatedPionTracksVetoNum]
	UShort_t        muIsoTrackMatchedToGenSingleProngTau[4];   //[IsolatedPionTracksVetoNum]
	UShort_t        elecIsoTrackMatchedToGenSingleProngTau[4];   //[IsolatedPionTracksVetoNum]
	UShort_t        pionIsoTrackMatchedToGenSingleProngTau[4];   //[IsolatedPionTracksVetoNum]
	UShort_t        JetsNum;
	Float_t         JetsPt[15];   //[JetsNum]
	Float_t         JetsEta[15];   //[JetsNum]
	Float_t         JetsPhi[15];   //[JetsNum]
	Float_t         JetsE[15];   //[JetsNum]
	Float_t         Jets_bDiscriminator[15];   //[JetsNum]
	Float_t         Jets_chargedEmEnergyFraction[15];   //[JetsNum]
	Float_t         Jets_chargedHadronEnergyFraction[15];   //[JetsNum]
	Int_t           Jets_chargedHadronMultiplicity[15];   //[JetsNum]
	Int_t           Jets_electronMultiplicity[15];   //[JetsNum]
	Float_t         Jets_jetArea[15];   //[JetsNum]
	Float_t         Jets_muonEnergyFraction[15];   //[JetsNum]
	Int_t           Jets_muonMultiplicity[15];   //[JetsNum]
	Float_t         Jets_neutralEmEnergyFraction[15];   //[JetsNum]
	Int_t           Jets_neutralHadronMultiplicity[15];   //[JetsNum]
	Float_t         Jets_photonEnergyFraction[15];   //[JetsNum]
	Int_t           Jets_photonMultiplicity[15];   //[JetsNum]
	UShort_t        ExpectationDiLep;
	UShort_t        MuDiLepControlSample;
	UShort_t        ElecDiLepControlSample;
	UShort_t        StandAloneGenMuIsoTrackMatched;
	UShort_t        StandAloneIsoTrackToMuMatched;
	UShort_t        StandAloneGenElecIsoTrackMatched;
	UShort_t        StandAloneIsoTrackToElecMatched;
	UShort_t        StandAloneIsoTrackToRecoMuMatched;
	UShort_t        StandAloneIsoTrackToRecoElecMatched;
	UShort_t        IsoTrackDiLepControlSampleMu;
	UShort_t        IsoTrackDiLepControlSampleElec;
	UShort_t        IsoTrackDiLepMu;
	UShort_t        IsoTrackDiLepElec;
	Float_t         GenMuWPt;
	Float_t         GenMuWPhi;
	Float_t         GenElecWPt;
	Float_t         GenElecWPhi;
	UShort_t        GenBosonNum;
	Float_t         GenBosonPt[2];   //[GenBosonNum]
	Float_t         GenBosonPhi[2];   //[GenBosonNum]
	
	// List of branches
	TBranch        *b_EvtNum;   //!
	TBranch        *b_HT;   //!
	TBranch        *b_MHT;   //!
	TBranch        *b_NJets;   //!
	TBranch        *b_BTags;   //!
	TBranch        *b_Leptons;   //!
	TBranch        *b_isoTracks;   //!
	TBranch        *b_DY;   //!
	TBranch        *b_Bin;   //!
	TBranch        *b_NVtx;   //!
	TBranch        *b_DeltaPhi1;   //!
	TBranch        *b_DeltaPhi2;   //!
	TBranch        *b_DeltaPhi3;   //!
	TBranch        *b_minDeltaPhiN;   //!
	TBranch        *b_DeltaPhiN1;   //!
	TBranch        *b_DeltaPhiN2;   //!
	TBranch        *b_DeltaPhiN3;   //!
	TBranch        *b_Weight;   //!
	TBranch        *b_MET;   //!
	TBranch        *b_METPhi;   //!
	TBranch        *b_GenMuNum;   //!
	TBranch        *b_GenMuFromTau;   //!
	TBranch        *b_GenMuDeltaRJet;   //!
	TBranch        *b_GenMuDeltaPTJet;   //!
	TBranch        *b_GenMuPt;   //!
	TBranch        *b_GenMuEta;   //!
	TBranch        *b_GenMuPhi;   //!
	TBranch        *b_GenMuonIsoTrackMatched;   //!
	TBranch        *b_GenMuonActivity;   //!
	TBranch        *b_GenElecNum;   //!
	TBranch        *b_GenElecFromTau;   //!
	TBranch        *b_GenElecDeltaRJet;   //!
	TBranch        *b_GenElecDeltaPTJet;   //!
	TBranch        *b_GenElecPt;   //!
	TBranch        *b_GenElecEta;   //!
	TBranch        *b_GenElecPhi;   //!
	TBranch        *b_GenElecIsoTrackMatched;   //!
	TBranch        *b_GenElecActivity;   //!
	TBranch        *b_GenTauNum;   //!
	TBranch        *b_GenTauPt;   //!
	TBranch        *b_GenTauEta;   //!
	TBranch        *b_GenTauPhi;   //!
	TBranch        *b_GenTauIsoTrackMatched;   //!
	TBranch        *b_GenTauActivity;   //!
	TBranch        *b_Expectation;   //!
	TBranch        *b_ExpectationReductionIsoTrack;   //!
	TBranch        *b_muAcc;   //!
	TBranch        *b_muReco;   //!
	TBranch        *b_muIso;   //!
	TBranch        *b_MTW;   //!
	TBranch        *b_IsoTrackMTW;   //!
	TBranch        *b_elecAcc;   //!
	TBranch        *b_elecReco;   //!
	TBranch        *b_elecIso;   //!
	TBranch        *b_muIsoTrack;   //!
	TBranch        *b_MuPurity;   //!
	TBranch        *b_ElecPurity;   //!
	TBranch        *b_muActivityMethod;   //!
	TBranch        *b_elecActivityMethod;   //!
	TBranch        *b_muIsoTrackActivityMethod;   //!
	TBranch        *b_elecIsoTrackActivityMethod;   //!
	TBranch        *b_pionIsoTrackActivityMethod;   //!
	TBranch        *b_FallsVetoLep;   //!
	TBranch        *b_selectedIDIsoMuonsNum;   //!
	TBranch        *b_selectedIDIsoMuonsPt;   //!
	TBranch        *b_muIsoMatched;   //!
	TBranch        *b_RecoIsoMuonPromtMatched;   //!
	TBranch        *b_RecoIsoMuonPromtMatchedDeltaR;   //!
	TBranch        *b_RecoIsoMuonPromtMatchedRelPt;   //!
	TBranch        *b_selectedIDIsoMuonsEta;   //!
	TBranch        *b_selectedIDIsoMuonsPhi;   //!
	TBranch        *b_selectedIDIsoMuonsE;   //!
	TBranch        *b_selectedIDIsoMuons_MTW;   //!
	TBranch        *b_RecoIsoMuonActivity;   //!
	TBranch        *b_selectedIDMuonsNum;   //!
	TBranch        *b_selectedIDMuonsPt;   //!
	TBranch        *b_selectedIDMuonsMatched;   //!
	TBranch        *b_selectedIDMuonsEta;   //!
	TBranch        *b_selectedIDMuonsPhi;   //!
	TBranch        *b_selectedIDMuonsE;   //!
	TBranch        *b_selectedIDMuons_MTW;   //!
	TBranch        *b_RecoMuonActivity;   //!
	TBranch        *b_RecoIsoMuonIsoTrackMatched;   //!
	TBranch        *b_selectedIDIsoElectronsNum;   //!
	TBranch        *b_selectedIDIsoElectronsPt;   //!
	TBranch        *b_elecIsoMatched;   //!
	TBranch        *b_RecoIsoElecPromtMatched;   //!
	TBranch        *b_RecoIsoElecPromtMatchedDeltaR;   //!
	TBranch        *b_RecoIsoElecPromtMatchedRelPt;   //!
	TBranch        *b_selectedIDIsoElectronsEta;   //!
	TBranch        *b_selectedIDIsoElectronsPhi;   //!
	TBranch        *b_selectedIDIsoElectronsE;   //!
	TBranch        *b_selectedIDIsoElectrons_MTW;   //!
	TBranch        *b_RecoIsoElectronActivity;   //!
	TBranch        *b_RecoIsoElecIsoTrackMatched;   //!
	TBranch        *b_selectedIDElectronsNum;   //!
	TBranch        *b_selectedIDElectronsPt;   //!
	TBranch        *b_selectedIDElectronsMatched;   //!
	TBranch        *b_selectedIDElectronsEta;   //!
	TBranch        *b_selectedIDElectronsPhi;   //!
	TBranch        *b_selectedIDElectronsE;   //!
	TBranch        *b_selectedIDElectrons_MTW;   //!
	TBranch        *b_RecoElectronActivity;   //!
	TBranch        *b_IsolatedElectronTracksVetoNum;   //!
	TBranch        *b_IsolatedElectronTracksVetoPt;   //!
	TBranch        *b_IsolatedElectronTracksVetoEta;   //!
	TBranch        *b_IsolatedElectronTracksVetoPhi;   //!
	TBranch        *b_IsolatedElectronTracksVetoE;   //!
	TBranch        *b_IsolatedElectronTracksVetoActivity;   //!
	TBranch        *b_IsolatedElectronTracksVetoMTW;   //!
	TBranch        *b_muIsoTrackMatchedToGenElec;   //!
	TBranch        *b_elecIsoTrackMatchedToGenElec;   //!
	TBranch        *b_pionIsoTrackMatchedToGenElec;   //!
	TBranch        *b_IsolatedMuonTracksVetoNum;   //!
	TBranch        *b_IsolatedMuonTracksVetoPt;   //!
	TBranch        *b_IsolatedMuonTracksVetoEta;   //!
	TBranch        *b_IsolatedMuonTracksVetoPhi;   //!
	TBranch        *b_IsolatedMuonTracksVetoE;   //!
	TBranch        *b_IsolatedMuonTracksVetoActivity;   //!
	TBranch        *b_IsolatedMuonTracksVetoMTW;   //!
	TBranch        *b_muIsoTrackMatchedToGenMu;   //!
	TBranch        *b_elecIsoTrackMatchedToGenMu;   //!
	TBranch        *b_pionIsoTrackMatchedToGenMu;   //!
	TBranch        *b_IsolatedPionTracksVetoNum;   //!
	TBranch        *b_IsolatedPionTracksVetoPt;   //!
	TBranch        *b_IsolatedPionTracksVetoEta;   //!
	TBranch        *b_IsolatedPionTracksVetoPhi;   //!
	TBranch        *b_IsolatedPionTracksVetoE;   //!
	TBranch        *b_IsolatedPionTracksVetoActivity;   //!
	TBranch        *b_IsolatedPionTracksVetoMTW;   //!
	TBranch        *b_muIsoTrackMatchedToGenSingleProngTau;   //!
	TBranch        *b_elecIsoTrackMatchedToGenSingleProngTau;   //!
	TBranch        *b_pionIsoTrackMatchedToGenSingleProngTau;   //!
	TBranch        *b_JetsNum;   //!
	TBranch        *b_JetsPt;   //!
	TBranch        *b_JetsEta;   //!
	TBranch        *b_JetsPhi;   //!
	TBranch        *b_JetsE;   //!
	TBranch        *b_Jets_bDiscriminator;   //!
	TBranch        *b_Jets_chargedEmEnergyFraction;   //!
	TBranch        *b_Jets_chargedHadronEnergyFraction;   //!
	TBranch        *b_Jets_chargedHadronMultiplicity;   //!
	TBranch        *b_Jets_electronMultiplicity;   //!
	TBranch        *b_Jets_jetArea;   //!
	TBranch        *b_Jets_muonEnergyFraction;   //!
	TBranch        *b_Jets_muonMultiplicity;   //!
	TBranch        *b_Jets_neutralEmEnergyFraction;   //!
	TBranch        *b_Jets_neutralHadronMultiplicity;   //!
	TBranch        *b_Jets_photonEnergyFraction;   //!
	TBranch        *b_Jets_photonMultiplicity;   //!
	TBranch        *b_ExpectationDiLep;   //!
	TBranch        *b_MuDiLepControlSample;   //!
	TBranch        *b_ElecDiLepControlSample;   //!
	TBranch        *b_StandAloneGenMuIsoTrackMatched;   //!
	TBranch        *b_StandAloneIsoTrackToMuMatched;   //!
	TBranch        *b_StandAloneGenElecIsoTrackMatched;   //!
	TBranch        *b_StandAloneIsoTrackToElecMatched;   //!
	TBranch        *b_StandAloneIsoTrackToRecoMuMatched;   //!
	TBranch        *b_StandAloneIsoTrackToRecoElecMatched;   //!
	TBranch        *b_IsoTrackDiLepControlSampleMu;   //!
	TBranch        *b_IsoTrackDiLepControlSampleElec;   //!
	TBranch        *b_IsoTrackDiLepMu;   //!
	TBranch        *b_IsoTrackDiLepElec;   //!
	TBranch        *b_GenMuWPt;   //!
	TBranch        *b_GenMuWPhi;   //!
	TBranch        *b_GenElecWPt;   //!
	TBranch        *b_GenElecWPhi;   //!
	TBranch        *b_GenBosonNum;   //!
	TBranch        *b_GenBosonPt;   //!
	TBranch        *b_GenBosonPhi;   //!
	
	EffMaker(TTree * /*tree*/ =0) : fChain(0) { }
	virtual ~EffMaker() { }
	virtual Int_t   Version() const { return 2; }
	virtual void    Begin(TTree *tree);
	virtual void    SlaveBegin(TTree *tree);
	virtual void    Init(TTree *tree);
	virtual Bool_t  Notify();
	virtual Bool_t  Process(Long64_t entry);
	virtual Int_t   GetEntry(Long64_t entry, Int_t getall = 0) { return fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0; }
	virtual void    SetOption(const char *option) { fOption = option; }
	virtual void    SetObject(TObject *obj) { fObject = obj; }
	virtual void    SetInputList(TList *input) { fInput = input; }
	virtual TList  *GetOutputList() const { return fOutput; }
	virtual void    SlaveTerminate();
	virtual void    Terminate();
	
	ClassDef(EffMaker,0);
};

#endif

#ifdef EffMaker_cxx
void EffMaker::Init(TTree *tree)
{
	// The Init() function is called when the selector needs to initialize
	// a new tree or chain. Typically here the branch addresses and branch
	// pointers of the tree will be set.
	// It is normally not necessary to make changes to the generated
	// code, but the routine can be extended by the user if needed.
	// Init() will be called many times when running on PROOF
	// (once per file to be processed).
	
	// Set branch addresses and branch pointers
	if (!tree) return;
	fChain = tree;
	fChain->SetMakeClass(1);
	
	fChain->SetBranchAddress("EvtNum", &EvtNum, &b_EvtNum);
	fChain->SetBranchAddress("HT", &HT, &b_HT);
	fChain->SetBranchAddress("MHT", &MHT, &b_MHT);
	fChain->SetBranchAddress("NJets", &NJets, &b_NJets);
	fChain->SetBranchAddress("BTags", &BTags, &b_BTags);
	fChain->SetBranchAddress("Leptons", &Leptons, &b_Leptons);
	fChain->SetBranchAddress("isoTracks", &isoTracks, &b_isoTracks);
	fChain->SetBranchAddress("DY", &DY, &b_DY);
	fChain->SetBranchAddress("Bin", &Bin, &b_Bin);
	fChain->SetBranchAddress("NVtx", &NVtx, &b_NVtx);
	fChain->SetBranchAddress("DeltaPhi1", &DeltaPhi1, &b_DeltaPhi1);
	fChain->SetBranchAddress("DeltaPhi2", &DeltaPhi2, &b_DeltaPhi2);
	fChain->SetBranchAddress("DeltaPhi3", &DeltaPhi3, &b_DeltaPhi3);
	fChain->SetBranchAddress("minDeltaPhiN", &minDeltaPhiN, &b_minDeltaPhiN);
	fChain->SetBranchAddress("DeltaPhiN1", &DeltaPhiN1, &b_DeltaPhiN1);
	fChain->SetBranchAddress("DeltaPhiN2", &DeltaPhiN2, &b_DeltaPhiN2);
	fChain->SetBranchAddress("DeltaPhiN3", &DeltaPhiN3, &b_DeltaPhiN3);
	fChain->SetBranchAddress("Weight", &Weight, &b_Weight);
	fChain->SetBranchAddress("MET", &MET, &b_MET);
	fChain->SetBranchAddress("METPhi", &METPhi, &b_METPhi);
	fChain->SetBranchAddress("GenMuNum", &GenMuNum, &b_GenMuNum);
	fChain->SetBranchAddress("GenMuFromTau", GenMuFromTau, &b_GenMuFromTau);
	fChain->SetBranchAddress("GenMuDeltaRJet", GenMuDeltaRJet, &b_GenMuDeltaRJet);
	fChain->SetBranchAddress("GenMuDeltaPTJet", GenMuDeltaPTJet, &b_GenMuDeltaPTJet);
	fChain->SetBranchAddress("GenMuPt", GenMuPt, &b_GenMuPt);
	fChain->SetBranchAddress("GenMuEta", GenMuEta, &b_GenMuEta);
	fChain->SetBranchAddress("GenMuPhi", GenMuPhi, &b_GenMuPhi);
	fChain->SetBranchAddress("GenMuonIsoTrackMatched", GenMuonIsoTrackMatched, &b_GenMuonIsoTrackMatched);
	fChain->SetBranchAddress("GenMuonActivity", GenMuonActivity, &b_GenMuonActivity);
	fChain->SetBranchAddress("GenElecNum", &GenElecNum, &b_GenElecNum);
	fChain->SetBranchAddress("GenElecFromTau", GenElecFromTau, &b_GenElecFromTau);
	fChain->SetBranchAddress("GenElecDeltaRJet", GenElecDeltaRJet, &b_GenElecDeltaRJet);
	fChain->SetBranchAddress("GenElecDeltaPTJet", GenElecDeltaPTJet, &b_GenElecDeltaPTJet);
	fChain->SetBranchAddress("GenElecPt", GenElecPt, &b_GenElecPt);
	fChain->SetBranchAddress("GenElecEta", GenElecEta, &b_GenElecEta);
	fChain->SetBranchAddress("GenElecPhi", GenElecPhi, &b_GenElecPhi);
	fChain->SetBranchAddress("GenElecIsoTrackMatched", GenElecIsoTrackMatched, &b_GenElecIsoTrackMatched);
	fChain->SetBranchAddress("GenElecActivity", GenElecActivity, &b_GenElecActivity);
	fChain->SetBranchAddress("GenTauNum", &GenTauNum, &b_GenTauNum);
	fChain->SetBranchAddress("GenTauPt", GenTauPt, &b_GenTauPt);
	fChain->SetBranchAddress("GenTauEta", GenTauEta, &b_GenTauEta);
	fChain->SetBranchAddress("GenTauPhi", GenTauPhi, &b_GenTauPhi);
	fChain->SetBranchAddress("GenTauIsoTrackMatched", GenTauIsoTrackMatched, &b_GenTauIsoTrackMatched);
	fChain->SetBranchAddress("GenTauActivity", GenTauActivity, &b_GenTauActivity);
	fChain->SetBranchAddress("Expectation", &Expectation, &b_Expectation);
	fChain->SetBranchAddress("ExpectationReductionIsoTrack", &ExpectationReductionIsoTrack, &b_ExpectationReductionIsoTrack);
	fChain->SetBranchAddress("muAcc", &muAcc, &b_muAcc);
	fChain->SetBranchAddress("muReco", &muReco, &b_muReco);
	fChain->SetBranchAddress("muIso", &muIso, &b_muIso);
	fChain->SetBranchAddress("MTW", &MTW, &b_MTW);
	fChain->SetBranchAddress("IsoTrackMTW", &IsoTrackMTW, &b_IsoTrackMTW);
	fChain->SetBranchAddress("elecAcc", &elecAcc, &b_elecAcc);
	fChain->SetBranchAddress("elecReco", &elecReco, &b_elecReco);
	fChain->SetBranchAddress("elecIso", &elecIso, &b_elecIso);
	fChain->SetBranchAddress("muIsoTrack", &muIsoTrack, &b_muIsoTrack);
	fChain->SetBranchAddress("MuPurity", &MuPurity, &b_MuPurity);
	fChain->SetBranchAddress("ElecPurity", &ElecPurity, &b_ElecPurity);
	fChain->SetBranchAddress("muActivityMethod", &muActivityMethod, &b_muActivityMethod);
	fChain->SetBranchAddress("elecActivityMethod", &elecActivityMethod, &b_elecActivityMethod);
	fChain->SetBranchAddress("muIsoTrackActivityMethod", &muIsoTrackActivityMethod, &b_muIsoTrackActivityMethod);
	fChain->SetBranchAddress("elecIsoTrackActivityMethod", &elecIsoTrackActivityMethod, &b_elecIsoTrackActivityMethod);
	fChain->SetBranchAddress("pionIsoTrackActivityMethod", &pionIsoTrackActivityMethod, &b_pionIsoTrackActivityMethod);
	fChain->SetBranchAddress("FallsVetoLep", &FallsVetoLep, &b_FallsVetoLep);
	fChain->SetBranchAddress("selectedIDIsoMuonsNum", &selectedIDIsoMuonsNum, &b_selectedIDIsoMuonsNum);
	fChain->SetBranchAddress("selectedIDIsoMuonsPt", selectedIDIsoMuonsPt, &b_selectedIDIsoMuonsPt);
	fChain->SetBranchAddress("muIsoMatched", muIsoMatched, &b_muIsoMatched);
	fChain->SetBranchAddress("RecoIsoMuonPromtMatched", RecoIsoMuonPromtMatched, &b_RecoIsoMuonPromtMatched);
	fChain->SetBranchAddress("RecoIsoMuonPromtMatchedDeltaR", RecoIsoMuonPromtMatchedDeltaR, &b_RecoIsoMuonPromtMatchedDeltaR);
	fChain->SetBranchAddress("RecoIsoMuonPromtMatchedRelPt", RecoIsoMuonPromtMatchedRelPt, &b_RecoIsoMuonPromtMatchedRelPt);
	fChain->SetBranchAddress("selectedIDIsoMuonsEta", selectedIDIsoMuonsEta, &b_selectedIDIsoMuonsEta);
	fChain->SetBranchAddress("selectedIDIsoMuonsPhi", selectedIDIsoMuonsPhi, &b_selectedIDIsoMuonsPhi);
	fChain->SetBranchAddress("selectedIDIsoMuonsE", selectedIDIsoMuonsE, &b_selectedIDIsoMuonsE);
	fChain->SetBranchAddress("selectedIDIsoMuons_MTW", selectedIDIsoMuons_MTW, &b_selectedIDIsoMuons_MTW);
	fChain->SetBranchAddress("RecoIsoMuonActivity", RecoIsoMuonActivity, &b_RecoIsoMuonActivity);
	fChain->SetBranchAddress("selectedIDMuonsNum", &selectedIDMuonsNum, &b_selectedIDMuonsNum);
	fChain->SetBranchAddress("selectedIDMuonsPt", selectedIDMuonsPt, &b_selectedIDMuonsPt);
	fChain->SetBranchAddress("selectedIDMuonsMatched", selectedIDMuonsMatched, &b_selectedIDMuonsMatched);
	fChain->SetBranchAddress("selectedIDMuonsEta", selectedIDMuonsEta, &b_selectedIDMuonsEta);
	fChain->SetBranchAddress("selectedIDMuonsPhi", selectedIDMuonsPhi, &b_selectedIDMuonsPhi);
	fChain->SetBranchAddress("selectedIDMuonsE", selectedIDMuonsE, &b_selectedIDMuonsE);
	fChain->SetBranchAddress("selectedIDMuons_MTW", selectedIDMuons_MTW, &b_selectedIDMuons_MTW);
	fChain->SetBranchAddress("RecoMuonActivity", RecoMuonActivity, &b_RecoMuonActivity);
	fChain->SetBranchAddress("RecoIsoMuonIsoTrackMatched", RecoIsoMuonIsoTrackMatched, &b_RecoIsoMuonIsoTrackMatched);
	fChain->SetBranchAddress("selectedIDIsoElectronsNum", &selectedIDIsoElectronsNum, &b_selectedIDIsoElectronsNum);
	fChain->SetBranchAddress("selectedIDIsoElectronsPt", selectedIDIsoElectronsPt, &b_selectedIDIsoElectronsPt);
	fChain->SetBranchAddress("elecIsoMatched", elecIsoMatched, &b_elecIsoMatched);
	fChain->SetBranchAddress("RecoIsoElecPromtMatched", RecoIsoElecPromtMatched, &b_RecoIsoElecPromtMatched);
	fChain->SetBranchAddress("RecoIsoElecPromtMatchedDeltaR", RecoIsoElecPromtMatchedDeltaR, &b_RecoIsoElecPromtMatchedDeltaR);
	fChain->SetBranchAddress("RecoIsoElecPromtMatchedRelPt", RecoIsoElecPromtMatchedRelPt, &b_RecoIsoElecPromtMatchedRelPt);
	fChain->SetBranchAddress("selectedIDIsoElectronsEta", selectedIDIsoElectronsEta, &b_selectedIDIsoElectronsEta);
	fChain->SetBranchAddress("selectedIDIsoElectronsPhi", selectedIDIsoElectronsPhi, &b_selectedIDIsoElectronsPhi);
	fChain->SetBranchAddress("selectedIDIsoElectronsE", selectedIDIsoElectronsE, &b_selectedIDIsoElectronsE);
	fChain->SetBranchAddress("selectedIDIsoElectrons_MTW", selectedIDIsoElectrons_MTW, &b_selectedIDIsoElectrons_MTW);
	fChain->SetBranchAddress("RecoIsoElectronActivity", RecoIsoElectronActivity, &b_RecoIsoElectronActivity);
	fChain->SetBranchAddress("RecoIsoElecIsoTrackMatched", RecoIsoElecIsoTrackMatched, &b_RecoIsoElecIsoTrackMatched);
	fChain->SetBranchAddress("selectedIDElectronsNum", &selectedIDElectronsNum, &b_selectedIDElectronsNum);
	fChain->SetBranchAddress("selectedIDElectronsPt", selectedIDElectronsPt, &b_selectedIDElectronsPt);
	fChain->SetBranchAddress("selectedIDElectronsMatched", selectedIDElectronsMatched, &b_selectedIDElectronsMatched);
	fChain->SetBranchAddress("selectedIDElectronsEta", selectedIDElectronsEta, &b_selectedIDElectronsEta);
	fChain->SetBranchAddress("selectedIDElectronsPhi", selectedIDElectronsPhi, &b_selectedIDElectronsPhi);
	fChain->SetBranchAddress("selectedIDElectronsE", selectedIDElectronsE, &b_selectedIDElectronsE);
	fChain->SetBranchAddress("selectedIDElectrons_MTW", selectedIDElectrons_MTW, &b_selectedIDElectrons_MTW);
	fChain->SetBranchAddress("RecoElectronActivity", RecoElectronActivity, &b_RecoElectronActivity);
	fChain->SetBranchAddress("IsolatedElectronTracksVetoNum", &IsolatedElectronTracksVetoNum, &b_IsolatedElectronTracksVetoNum);
	fChain->SetBranchAddress("IsolatedElectronTracksVetoPt", IsolatedElectronTracksVetoPt, &b_IsolatedElectronTracksVetoPt);
	fChain->SetBranchAddress("IsolatedElectronTracksVetoEta", IsolatedElectronTracksVetoEta, &b_IsolatedElectronTracksVetoEta);
	fChain->SetBranchAddress("IsolatedElectronTracksVetoPhi", IsolatedElectronTracksVetoPhi, &b_IsolatedElectronTracksVetoPhi);
	fChain->SetBranchAddress("IsolatedElectronTracksVetoE", IsolatedElectronTracksVetoE, &b_IsolatedElectronTracksVetoE);
	fChain->SetBranchAddress("IsolatedElectronTracksVetoActivity", IsolatedElectronTracksVetoActivity, &b_IsolatedElectronTracksVetoActivity);
	fChain->SetBranchAddress("IsolatedElectronTracksVetoMTW", IsolatedElectronTracksVetoMTW, &b_IsolatedElectronTracksVetoMTW);
	fChain->SetBranchAddress("muIsoTrackMatchedToGenElec", muIsoTrackMatchedToGenElec, &b_muIsoTrackMatchedToGenElec);
	fChain->SetBranchAddress("elecIsoTrackMatchedToGenElec", elecIsoTrackMatchedToGenElec, &b_elecIsoTrackMatchedToGenElec);
	fChain->SetBranchAddress("pionIsoTrackMatchedToGenElec", pionIsoTrackMatchedToGenElec, &b_pionIsoTrackMatchedToGenElec);
	fChain->SetBranchAddress("IsolatedMuonTracksVetoNum", &IsolatedMuonTracksVetoNum, &b_IsolatedMuonTracksVetoNum);
	fChain->SetBranchAddress("IsolatedMuonTracksVetoPt", IsolatedMuonTracksVetoPt, &b_IsolatedMuonTracksVetoPt);
	fChain->SetBranchAddress("IsolatedMuonTracksVetoEta", IsolatedMuonTracksVetoEta, &b_IsolatedMuonTracksVetoEta);
	fChain->SetBranchAddress("IsolatedMuonTracksVetoPhi", IsolatedMuonTracksVetoPhi, &b_IsolatedMuonTracksVetoPhi);
	fChain->SetBranchAddress("IsolatedMuonTracksVetoE", IsolatedMuonTracksVetoE, &b_IsolatedMuonTracksVetoE);
	fChain->SetBranchAddress("IsolatedMuonTracksVetoActivity", IsolatedMuonTracksVetoActivity, &b_IsolatedMuonTracksVetoActivity);
	fChain->SetBranchAddress("IsolatedMuonTracksVetoMTW", IsolatedMuonTracksVetoMTW, &b_IsolatedMuonTracksVetoMTW);
	fChain->SetBranchAddress("muIsoTrackMatchedToGenMu", muIsoTrackMatchedToGenMu, &b_muIsoTrackMatchedToGenMu);
	fChain->SetBranchAddress("elecIsoTrackMatchedToGenMu", elecIsoTrackMatchedToGenMu, &b_elecIsoTrackMatchedToGenMu);
	fChain->SetBranchAddress("pionIsoTrackMatchedToGenMu", pionIsoTrackMatchedToGenMu, &b_pionIsoTrackMatchedToGenMu);
	fChain->SetBranchAddress("IsolatedPionTracksVetoNum", &IsolatedPionTracksVetoNum, &b_IsolatedPionTracksVetoNum);
	fChain->SetBranchAddress("IsolatedPionTracksVetoPt", IsolatedPionTracksVetoPt, &b_IsolatedPionTracksVetoPt);
	fChain->SetBranchAddress("IsolatedPionTracksVetoEta", IsolatedPionTracksVetoEta, &b_IsolatedPionTracksVetoEta);
	fChain->SetBranchAddress("IsolatedPionTracksVetoPhi", IsolatedPionTracksVetoPhi, &b_IsolatedPionTracksVetoPhi);
	fChain->SetBranchAddress("IsolatedPionTracksVetoE", IsolatedPionTracksVetoE, &b_IsolatedPionTracksVetoE);
	fChain->SetBranchAddress("IsolatedPionTracksVetoActivity", IsolatedPionTracksVetoActivity, &b_IsolatedPionTracksVetoActivity);
	fChain->SetBranchAddress("IsolatedPionTracksVetoMTW", IsolatedPionTracksVetoMTW, &b_IsolatedPionTracksVetoMTW);
	fChain->SetBranchAddress("muIsoTrackMatchedToGenSingleProngTau", muIsoTrackMatchedToGenSingleProngTau, &b_muIsoTrackMatchedToGenSingleProngTau);
	fChain->SetBranchAddress("elecIsoTrackMatchedToGenSingleProngTau", elecIsoTrackMatchedToGenSingleProngTau, &b_elecIsoTrackMatchedToGenSingleProngTau);
	fChain->SetBranchAddress("pionIsoTrackMatchedToGenSingleProngTau", pionIsoTrackMatchedToGenSingleProngTau, &b_pionIsoTrackMatchedToGenSingleProngTau);
	fChain->SetBranchAddress("JetsNum", &JetsNum, &b_JetsNum);
	fChain->SetBranchAddress("JetsPt", JetsPt, &b_JetsPt);
	fChain->SetBranchAddress("JetsEta", JetsEta, &b_JetsEta);
	fChain->SetBranchAddress("JetsPhi", JetsPhi, &b_JetsPhi);
	fChain->SetBranchAddress("JetsE", JetsE, &b_JetsE);
	fChain->SetBranchAddress("Jets_bDiscriminator", Jets_bDiscriminator, &b_Jets_bDiscriminator);
	fChain->SetBranchAddress("Jets_chargedEmEnergyFraction", Jets_chargedEmEnergyFraction, &b_Jets_chargedEmEnergyFraction);
	fChain->SetBranchAddress("Jets_chargedHadronEnergyFraction", Jets_chargedHadronEnergyFraction, &b_Jets_chargedHadronEnergyFraction);
	fChain->SetBranchAddress("Jets_chargedHadronMultiplicity", Jets_chargedHadronMultiplicity, &b_Jets_chargedHadronMultiplicity);
	fChain->SetBranchAddress("Jets_electronMultiplicity", Jets_electronMultiplicity, &b_Jets_electronMultiplicity);
	fChain->SetBranchAddress("Jets_jetArea", Jets_jetArea, &b_Jets_jetArea);
	fChain->SetBranchAddress("Jets_muonEnergyFraction", Jets_muonEnergyFraction, &b_Jets_muonEnergyFraction);
	fChain->SetBranchAddress("Jets_muonMultiplicity", Jets_muonMultiplicity, &b_Jets_muonMultiplicity);
	fChain->SetBranchAddress("Jets_neutralEmEnergyFraction", Jets_neutralEmEnergyFraction, &b_Jets_neutralEmEnergyFraction);
	fChain->SetBranchAddress("Jets_neutralHadronMultiplicity", Jets_neutralHadronMultiplicity, &b_Jets_neutralHadronMultiplicity);
	fChain->SetBranchAddress("Jets_photonEnergyFraction", Jets_photonEnergyFraction, &b_Jets_photonEnergyFraction);
	fChain->SetBranchAddress("Jets_photonMultiplicity", Jets_photonMultiplicity, &b_Jets_photonMultiplicity);
	fChain->SetBranchAddress("ExpectationDiLep", &ExpectationDiLep, &b_ExpectationDiLep);
	fChain->SetBranchAddress("MuDiLepControlSample", &MuDiLepControlSample, &b_MuDiLepControlSample);
	fChain->SetBranchAddress("ElecDiLepControlSample", &ElecDiLepControlSample, &b_ElecDiLepControlSample);
	fChain->SetBranchAddress("StandAloneGenMuIsoTrackMatched", &StandAloneGenMuIsoTrackMatched, &b_StandAloneGenMuIsoTrackMatched);
	fChain->SetBranchAddress("StandAloneIsoTrackToMuMatched", &StandAloneIsoTrackToMuMatched, &b_StandAloneIsoTrackToMuMatched);
	fChain->SetBranchAddress("StandAloneGenElecIsoTrackMatched", &StandAloneGenElecIsoTrackMatched, &b_StandAloneGenElecIsoTrackMatched);
	fChain->SetBranchAddress("StandAloneIsoTrackToElecMatched", &StandAloneIsoTrackToElecMatched, &b_StandAloneIsoTrackToElecMatched);
	fChain->SetBranchAddress("StandAloneIsoTrackToRecoMuMatched", &StandAloneIsoTrackToRecoMuMatched, &b_StandAloneIsoTrackToRecoMuMatched);
	fChain->SetBranchAddress("StandAloneIsoTrackToRecoElecMatched", &StandAloneIsoTrackToRecoElecMatched, &b_StandAloneIsoTrackToRecoElecMatched);
	fChain->SetBranchAddress("IsoTrackDiLepControlSampleMu", &IsoTrackDiLepControlSampleMu, &b_IsoTrackDiLepControlSampleMu);
	fChain->SetBranchAddress("IsoTrackDiLepControlSampleElec", &IsoTrackDiLepControlSampleElec, &b_IsoTrackDiLepControlSampleElec);
	fChain->SetBranchAddress("IsoTrackDiLepMu", &IsoTrackDiLepMu, &b_IsoTrackDiLepMu);
	fChain->SetBranchAddress("IsoTrackDiLepElec", &IsoTrackDiLepElec, &b_IsoTrackDiLepElec);
	fChain->SetBranchAddress("GenMuWPt", &GenMuWPt, &b_GenMuWPt);
	fChain->SetBranchAddress("GenMuWPhi", &GenMuWPhi, &b_GenMuWPhi);
	fChain->SetBranchAddress("GenElecWPt", &GenElecWPt, &b_GenElecWPt);
	fChain->SetBranchAddress("GenElecWPhi", &GenElecWPhi, &b_GenElecWPhi);
	fChain->SetBranchAddress("GenBosonNum", &GenBosonNum, &b_GenBosonNum);
	fChain->SetBranchAddress("GenBosonPt", GenBosonPt, &b_GenBosonPt);
	fChain->SetBranchAddress("GenBosonPhi", GenBosonPhi, &b_GenBosonPhi);
}

Bool_t EffMaker::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

#endif // #ifdef EffMaker_cxx
