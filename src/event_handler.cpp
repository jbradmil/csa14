#include "event_handler.hpp"
#include <cassert>
#include <climits>
#include <cfloat>
#include <ctime>
#include <cmath>
#include <cstdio>
#include <iomanip>
#include <fstream>
#include <vector>
#include <string>
#include <set>
#include <utility>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <functional>
#include <limits>
#include "TChain.h"
#include "TTree.h"
#include "TBranch.h"
#include "TFile.h"
#include "TObjArray.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TCanvas.h"
#include "timer.hpp"
#include "math.hpp"
#include "in_json_2012.hpp"

#include "fastjet/ClusterSequence.hh"
#include "fastjet/PseudoJet.hh"

#include "BTagWeight2.hpp"


const double EventHandler::CSVTCut(0.898);
const double EventHandler::CSVMCut(0.679);
const double EventHandler::CSVLCut(0.244);
const double EventHandler::ICSVTCut(0.941);
const double EventHandler::ICSVMCut(0.814);
const double EventHandler::ICSVLCut(0.423);
const std::vector<std::vector<int> > VRunLumiPrompt(MakeVRunLumi("Golden"));
const std::vector<std::vector<int> > VRunLumi24Aug(MakeVRunLumi("24Aug"));
const std::vector<std::vector<int> > VRunLumi13Jul(MakeVRunLumi("13Jul"));

using namespace fastjet;


EventHandler::EventHandler(const std::string &fileName, const bool isList, const double scaleFactorIn, const bool fastMode, const jec_type_t jec_type_in):
  cfA(fileName, isList),
  theJESType_(jec_type_in),
  corrJets(0),
  JetsUpToDate(false),
  JetConeSize_(0.),
  theMET_(0.),
  theMETPhi_(0.),
  SearchBins_(0),
  sortedFatJetCache(0),
  FatJetsUpToDate(false),
  muons_(0),
  muonsUpToDate_(false),
  electrons_(0),
  electronsUpToDate_(false),
  photons_(0),
  photonsUpToDate_(false),
  muTracks_(0),
  muTracksUpToDate_(false),
  elTracks_(0),
  elTracksUpToDate_(false),
  hadTracks_(0),
  hadTracksUpToDate_(false),
  scaleFactor(scaleFactorIn),
  //betaUpToDate(false),
  //beta(0),
  f_tageff_(0){
  if (fastMode){
    chainB.SetBranchStatus("pfcand*",0);
    if(cfAVersion<=71||cfAVersion==74) { // turn off unnecessary branches
      chainA.SetBranchStatus("triggerobject_*",0);
      chainA.SetBranchStatus("standalone_t*",0);
      chainA.SetBranchStatus("L1trigger_*",0);
      chainA.SetBranchStatus("passprescale*",0);
      chainA.SetBranchStatus("jets_AK5PFclean_*",0);
      chainA.SetBranchStatus("softjetUp_*",0);
      chainA.SetBranchStatus("pdfweights_*",0);
      chainA.SetBranchStatus("photon_*",0);
      chainB.SetBranchStatus("Ntcmets",0);
      chainB.SetBranchStatus("tcmets_*",0);
      chainB.SetBranchStatus("Nphotons",0);
      chainB.SetBranchStatus("photons_*",0);
      chainB.SetBranchStatus("Npf_photons",0);
      chainB.SetBranchStatus("pf_photons_*",0);
      chainB.SetBranchStatus("Njets_AK5PFclean",0);
      chainB.SetBranchStatus("jets_AK5PFclean_*",0);
    }
  }
  // if (cfAVersion>=75) chainB.SetBranchStatus("mc_final*",0);
  LoadJetTagEffMaps();
  // LoadRLFHist();
  LoadJEC();
  LoadSearchBins();
}

// EventHandler::~EventHandler(){
// 	delete f_tageff_;
// 	delete jet_corrector_pfL1FastJetL2L3_;
// }

void EventHandler::GetEntry(const unsigned int entry){
  cfA::GetEntry(entry);
  //  betaUpToDate=false;
  if (cfAVersion>=75) {
    //  cout << "Setup leptons & tracks" << endl;
    muons_ = GetRecoMuons();
    electrons_ = GetRecoElectrons();
    photons_ = GetPhotons();
    GetIsoTracks(muTracks_, elTracks_, hadTracks_, true, false);
  } else{
    muons_ = GetRA2bMuons();
    electrons_ = GetRA2bElectrons();
  }
  JetsUpToDate=false;
  JetConeSize_ = (cfAVersion>=75 ? 0.4 : 0.5);
  //  cout << "GetSortedJets()" << endl;
  GetSortedJets();
  SetCorrectedMET();
  FatJetsUpToDate=false;
  muonsUpToDate_=false;
  electronsUpToDate_=false;

}


// void EventHandler::initEnumNames() {
//   theJESNames_[kJESDEF]="JESDEF";
//   theJESNames_[kJESRAW]="JESRAW";
//   theJESNames_[kJESCORR]="JESCORR";
// }

unsigned EventHandler::TypeCode() const{
  // https://github.com/manuelfs/ra4_code/blob/master/src/event_handler.cpp#L606
  const TString sample_name = sampleName;
  //cout << "Sample name: " << sampleName << endl;
  unsigned sample_code = 0xF;
  if(sample_name.Contains("SMS")){
    sample_code = 0x0;
  }else if(sample_name.Contains("TTJets")
	   || sample_name.Contains("TT_CT10")){
    sample_code = 0x1;
  }else if(sample_name.Contains("WJets")){
    sample_code = 0x2;
  }else if(sample_name.Contains("T_s-channel")
	   || sample_name.Contains("T_tW-channel")
	   || sample_name.Contains("T_t-channel")
	   || sample_name.Contains("Tbar_s-channel")
	   || sample_name.Contains("Tbar_tW-channel")
	   || sample_name.Contains("Tbar_t-channel")){
    sample_code = 0x3;
  }else if(sample_name.Contains("QCD")){
    sample_code = 0x4;
  }else if(sample_name.Contains("DY")){
    sample_code = 0x5;
  }else{
    sample_code = 0xF;
  }

  unsigned num_leps = 0, num_tau_to_lep = 0, num_taus = 0, num_conversions = 0;
  bool counting = false;
  for(unsigned i = 0; i < mc_doc_id->size(); ++i){
    const int id = static_cast<int>(floor(fabs(mc_doc_id->at(i))+0.5));
    const int mom = static_cast<int>(floor(fabs(mc_doc_mother_id->at(i))+0.5));
    const int gmom = static_cast<int>(floor(fabs(mc_doc_grandmother_id->at(i))+0.5));
    const int ggmom = static_cast<int>(floor(fabs(mc_doc_ggrandmother_id->at(i))+0.5));
    if(mom != 15) counting=false;
    if((id == 11 || id == 13) && (mom == 24 || (mom == 15 && (gmom == 24 || (gmom == 15 && ggmom == 24))))){
      ++num_leps;
      if(mom == 15){
	++num_tau_to_lep;
	if(counting){
	  ++num_conversions;
	  counting = false;
	}else{
	  counting = true;
	}
      }
    }else if(id == 15 && mom == 24){
      ++num_taus;
    }
  }
  num_leps -= 2*num_conversions;
  num_tau_to_lep -= 2*num_conversions;
  if(sample_code > 0xF) sample_code = 0xF;
  if(num_leps > 0xF) num_leps = 0xF;
  if(num_tau_to_lep > 0xF) num_tau_to_lep = 0xF;
  if(num_taus > 0xF) num_taus = 0xF;
  return (sample_code << 12) | (num_leps << 8) | (num_tau_to_lep << 4) | num_taus;
}

int EventHandler::GetGluinoMass() const{
  return GetMass("gluino");
}

int EventHandler::GetLSPMass() const{
  return GetMass("bino");
}

int EventHandler::GetMass(const std::string& token) const{
  //  cout << model_params->c_str() << endl;
  std::string::size_type pos(model_params->find(token));
  if(pos==std::string::npos){
    return -1;
  }else{
    pos+=token.size();
    const std::string::size_type end(model_params->find("_",pos));
    if(end==std::string::npos){
      return -1;
    }else{
      return atoi((model_params->substr(pos, end-pos+1)).c_str());
    }
  }
}

double EventHandler::GetDocMET() const{
  double px(0.), py(0.);
  if(sampleName.find("T1bbbb")!=std::string::npos){
    //  cout << "GetDOCMET()" << endl;
    for(unsigned int imc = 0; imc < mc_doc_id->size(); imc++){
      if (mc_doc_status->at(imc)==3||mc_doc_status->at(imc)==22||mc_doc_status->at(imc)==23||mc_doc_id->at(imc)==2212) {  // hard scatter
	if (fabs(static_cast<int>(mc_doc_id->at(imc)))==5) {
	  //	  printf("px: %3.2f, py: %3.2f\n", mc_doc_px->at(imc), mc_doc_py->at(imc));
	  px+=mc_doc_px->at(imc);
	  py+=mc_doc_py->at(imc);
	}
      }
    }
  } else { // SM: just add up neutrinos
    for(unsigned int imc = 0; imc < mc_doc_id->size(); imc++){
      if (mc_doc_status->at(imc)==3||mc_doc_status->at(imc)==22||mc_doc_status->at(imc)==23||mc_doc_id->at(imc)==2212) {  // hard scatter
	if (fabs(static_cast<int>(mc_doc_id->at(imc)))==12||fabs(static_cast<int>(mc_doc_id->at(imc)))==14||fabs(static_cast<int>(mc_doc_id->at(imc)))==16) {
	  px+=mc_doc_px->at(imc);
	  py+=mc_doc_py->at(imc);
	}
      }
    }
  }
  return TMath::Sqrt(px*px+py*py);
}

double EventHandler::GetGluinoPt(int which) const{
  double pt(-1.);
  int count(0);
  for(unsigned int imc = 0; imc < mc_doc_id->size(); imc++){
    if (mc_doc_status->at(imc)==3||mc_doc_status->at(imc)==22||mc_doc_status->at(imc)==23||mc_doc_id->at(imc)==2212) {
      if (fabs(static_cast<int>(mc_doc_id->at(imc)))==1000021) {
	//	printf("px: %3.2f, py: %3.2f, pt: %3.2f\n", mc_doc_px->at(imc), mc_doc_py->at(imc), mc_doc_pt->at(imc));
	pt = mc_doc_pt->at(imc);
	count++;
	if (count==which) break;
      }
    }
  }
  return pt;
}

double EventHandler::GetTriggerEffWeight(const double pt_cut) const {
  if(sampleName.find("Run2012")!=std::string::npos) return 1.;
  if (cfAVersion>=75)  return 1.;

  double trig_eff_weight=1.;
  bool isQCD(sampleName.find("QCD_")!=std::string::npos || sampleName.find("BJets_HT")!=std::string::npos);

  double met=pfTypeImets_et->at(0);
  double ht=GetHT(pt_cut);
  if (isQCD) {
    if (ht<500&&met<150) trig_eff_weight=0.86;
    else if (ht<500&&met>150&&met<250) trig_eff_weight=0.88;
    else if (ht>500&&ht<800&&met<150) trig_eff_weight=0.67;
    else if(ht>500&&ht<800&&met>150&&met<250) trig_eff_weight=1.0;
   else if (ht>800||met>250) trig_eff_weight=1.0;
  } else {
    if (ht<500&&met<150) trig_eff_weight=0.91;
    else if (ht<500&&met>150&&met<250) trig_eff_weight=0.98;
    else if (ht>500&&ht<800&&met<150) trig_eff_weight=0.95;
    else if(ht>500&&ht<800&&met>150&&met<250) trig_eff_weight=0.99;
    else if (ht>800||met>250) trig_eff_weight=1.0;
  }

  return trig_eff_weight;
}

void EventHandler::LoadJEC() {
  jetcorr_filenames_pfL1FastJetL2L3_.clear();
  // files for Phys14 MC -- v4 
  jetcorr_filenames_pfL1FastJetL2L3_.push_back ("JESFiles/PHYS14_V4_MC/PHYS14_V4_MC_L1FastJet_AK4PFchs.txt");
  jetcorr_filenames_pfL1FastJetL2L3_.push_back ("JESFiles/PHYS14_V4_MC/PHYS14_V4_MC_L2Relative_AK4PFchs.txt");
  jetcorr_filenames_pfL1FastJetL2L3_.push_back ("JESFiles/PHYS14_V4_MC/PHYS14_V4_MC_L3Absolute_AK4PFchs.txt");
  jet_corrector_pfL1FastJetL2L3_ = makeJetCorrector(jetcorr_filenames_pfL1FastJetL2L3_);	
}

void EventHandler::LoadSearchBins() {
  SearchBins_ = new SearchBins();
}

// void EventHandler::LoadRLFHist() {
//   if (cfAVersion>=77) {
//     assert(f_RLF_ ==0);
//     TString filename = ("macros/junews/ll_est/ttbar_2b_skim_th3.root");
//     f_RLF_ = new TFile(filename,"READ");
//     if (f_RLF_->IsZombie()) {
//       cout<<"Failed to load RLF histogram "<<sampleName<<endl;
//       delete f_RLF_;
//       f_RLF_=0;
//     }
//     else {
//       TH1D * hRLF = static_cast<TH1D*>(f_RLF_->Get("hRLF_default"));
//       if (hRLF->GetEntries()>0) {
// 	std::cout << "Successfully loaded RLF hists " << filename << endl;
//       }
//       else cout<<"Failed to load RLF histogram "<<sampleName<<endl;
//     }
//   }
// }

// double EventHandler::GetRLF(const double ptW, const double ht, const int njets) const{
//   if (f_RLF_==0) return 0.;
//   std::string sRLF = "h_RLF_default";
//   TH1D * h_RLF = static_cast<TH1D*>(f_RLF_->Get(sRLF.c_str()));
//   h_RLF->Print("all");
//   return h_RLF->GetBinContent(h_RLF->FindBin(ptW,ht,njets));
// }

// double EventHandler::GetRLFError(const double ptW, const double ht, const int njets) const{
//   if (f_RLF_==0) return 0.;
//   std::string sRLF = "h_RLF_default";
//   TH1D * h_RLF = static_cast<TH1D*>(f_RLF_->Get(sRLF.c_str()));
//   return h_RLF->GetBinError(h_RLF->FindBin(ptW,ht,njets));
// }


void EventHandler::LoadJetTagEffMaps() {
  if (cfAVersion<=71) {
    assert(f_tageff_ ==0);
    TString filename = AssembleBTagEffFilename();
    filename.Prepend("btagEffMaps/");
    f_tageff_ = new TFile(filename,"READ");
    if (f_tageff_->IsZombie()) {
      cout<<"Failed to load the b-tag eff map for sample "<<sampleName<<endl;
      //comment this next line to ALLOW JOBS TO RUN WITHOUT BTAG EFF
      //if (!isSampleRealData()) assert(0); //it's just too frustrating to let jobs continue in this state
      delete f_tageff_;
      f_tageff_=0;
    }
    else {
      TH1D * h_btageff = static_cast<TH1D*>(f_tageff_->Get("h_btageff"));
      int nbins=h_btageff->GetNbinsX();
      if (nbins != 17) {
	cout<<" b tag eff map has the wrong number of bins! "<<nbins<<endl;
	assert(0);
      }
      else {
	std::cout << "Successfully loaded b-tag eff maps in " << filename << endl;
      }
    }
  }
}

TString EventHandler::AssembleBTagEffFilename() {
  unsigned found = sampleName.find_last_of("/");
  std::string truncated_name = sampleName.substr(0,found);
  found = truncated_name.find_last_of("/");
  truncated_name = truncated_name.substr(found+1);
  return "histos_btageff_csvm_"+truncated_name+".root";
}

double EventHandler::GetBtagWeight(double pt_cut) const{
  if(sampleName.find("Run2012")!=std::string::npos) return 1.;
  if (cfAVersion>=75)  return 1.;
  vector<BTagWeight::JetInfo> jetinfos;
  int ntagged=0;
  for (unsigned int ijet=0; ijet<jets_AKPF_pt->size(); ++ijet) {
    //only consider good jets (eta, quality, pT)
    if(isGoodJet(ijet, true, pt_cut)){
      //for each jet, what is the flavor and is it tagged (CSVM)
      if (jets_AKPF_btag_secVertexCombined->at(ijet) >=0.679) ntagged++;
      int flavor = static_cast<int>(jets_AKPF_partonFlavour->at(ijet));
      double pt = jets_AKPF_pt->at(ijet);
      double eta = jets_AKPF_eta->at(ijet);
      double SF = GetBtagSF(flavor, pt, eta);
      double effmc = GetBtagEffMC(flavor, pt);
      jetinfos.push_back( BTagWeight::JetInfo(effmc,SF));
    }
  }
  BTagWeight btvweq(ntagged,ntagged);
  return btvweq.weight(jetinfos,ntagged);
}

double EventHandler::GetPIDBtagWeight(double pt_cut) const{
  if(sampleName.find("Run2012")!=std::string::npos) return 1.;
  if (cfAVersion>=75)  return 1.;
  /*
    alternative b-tag SF procedure. Akin to 'PIDweighting' in BaBar.
    Use the b-tag algorithm output and calculate a weight for the _event_ based on the jet parton content and the b-tag SFs
  */
  //careful because 'weight' is a global variable' :-(
  double outputweight=1;
  //loop over jets in the event
  for (unsigned int ijet=0; ijet<jets_AKPF_pt->size(); ++ijet) {
    //only consider good jets (eta, quality, pT)
    if(isGoodJet(ijet, true, pt_cut)){
      //for each jet, what is the flavor and is it tagged
      int flavor = static_cast<int>(jets_AKPF_partonFlavour->at(ijet));
      double pt = jets_AKPF_pt->at(ijet);
      double eta = jets_AKPF_eta->at(ijet);
      bool istagged = (jets_AKPF_btag_secVertexCombined->at(ijet) >=0.679);
      double SF = GetBtagSF(flavor, pt, eta);
      if (istagged) {
	outputweight *= SF;
      }
      else {
	double effmc = GetBtagEffMC(flavor, pt);
	outputweight *= (1.0 - effmc*SF)/(1.0-effmc);
      }
    }
  }
  return outputweight;
}

double EventHandler::GetBtagSF(const int flavor,const double pt,const double jet_eta,const int tagcat) const{
  //this is for CSV L+M+T
  //This is the Moriond13 prescription
  if (tagcat==0) return 1;
  if (tagcat==4) return 0;
  const double eta = fabs(jet_eta);
  if (eta>2.4) return 0; //no b-tag eff here!
  double SF=1;
  if (abs(flavor) == 5 || abs(flavor)==4) { // heavy flavor SFs
    const double x = pt>800? 800 : pt;
    if (tagcat == 1) { //CSVL
      SF = 0.981149*((1.+(-0.000713295*x))/(1.+(-0.000703264*x)));
    }
    else if (tagcat == 2) { //CSVM
      SF = 0.726981*((1.+(0.253238*x))/(1.+(0.188389*x)));
    }
    else if (tagcat == 3) {//CSVT
      SF = 0.869965*((1.+(0.0335062*x))/(1.+(0.0304598*x)));
    }
    else assert(0);
  } //if heavy flavor
  else { //light flavor SFs
    double x = pt;
    if ( pt> 800) x = 800;
    if ( pt> 700 && eta>=1.5 && tagcat==1) x=700; //special maximum pt for high eta, CSVL
    //adapted from https://twiki.cern.ch/twiki/pub/CMS/BtagPOG/SFlightFuncs_Moriond2013.C
    if( tagcat == 1 && eta< 0.5) {
      SF = ((1.04901+(0.00152181*x))+(-3.43568e-06*(x*x)))+(2.17219e-09*(x*(x*x)));
    }
    else if( tagcat == 1 && eta >= 0.5 && eta < 1.0) {
      SF = ((0.991915+(0.00172552*x))+(-3.92652e-06*(x*x)))+(2.56816e-09*(x*(x*x)));
    }
    else if( tagcat == 1 && eta >= 1.0 && eta < 1.5) {
      SF = ((0.962127+(0.00192796*x))+(-4.53385e-06*(x*x)))+(3.0605e-09*(x*(x*x)));
    }
    else if( tagcat == 1 && eta >= 1.5 && eta <= 2.4) {
      SF = ((1.06121+(0.000332747*x))+(-8.81201e-07*(x*x)))+(7.43896e-10*(x*(x*x)));
    }
    else if( tagcat == 2 && eta< 0.8) {
      SF = ((1.06238+(0.00198635*x))+(-4.89082e-06*(x*x)))+(3.29312e-09*(x*(x*x)));
    }
    else if( tagcat == 2 && eta >= 0.8 && eta < 1.6) {
     SF = ((1.08048+(0.00110831*x))+(-2.96189e-06*(x*x)))+(2.16266e-09*(x*(x*x)));
    }
    else if( tagcat == 2 && eta >= 1.6 && eta <= 2.4) {
      SF = ((1.09145+(0.000687171*x))+(-2.45054e-06*(x*x)))+(1.7844e-09*(x*(x*x)));
    }
    else if( tagcat == 3 && eta <= 2.4) {
      SF = ((1.01739+(0.00283619*x))+(-7.93013e-06*(x*x)))+(5.97491e-09*(x*(x*x)));
    }
    else assert(0);
  } //else (light flavor SFs)
  return SF;
}

double EventHandler::GetBtagEffMC(const int flavor, const double pt) const{
  if (f_tageff_==0) return 0;
  std::string sbtageff = "h_btageff";
  std::string sctageff = "h_ctageff";
  std::string sltageff = "h_ltageff";
  TH1D * h_btageff = static_cast<TH1D*>(f_tageff_->Get(sbtageff.c_str()));
  TH1D * h_ctageff = static_cast<TH1D*>(f_tageff_->Get(sctageff.c_str()));
  TH1D * h_ltageff = static_cast<TH1D*>(f_tageff_->Get(sltageff.c_str()));
  TH1D* hh;
  if (abs(flavor)==5) hh=h_btageff;
  else if (abs(flavor)==4) hh=h_ctageff;
  else hh=h_ltageff;
  return hh->GetBinContent( hh->FindBin(pt));
}

// This gives you the weight you apply to the MC when you don't cut on b-tags
void EventHandler::CalculateTagProbs(double &Prob0, double &ProbGEQ1, double &Prob1, double &ProbGEQ2,
				     double &Prob2, double &ProbGEQ3, double &Prob3, double &ProbGEQ4, double pt_cut) {

  //must initialize correctly
  Prob2 = 0;
  Prob1 = 0; ProbGEQ1 = 1; Prob0 = 1; ProbGEQ2 = 0;
  Prob3 = 0; ProbGEQ4 = 0;

  if(sampleName.find("Run2012")!=std::string::npos) { // for data, set all to 1--not really needed
    Prob0=1;
    ProbGEQ1=1;
    Prob1=1;
    ProbGEQ2=1;
    Prob2=1;
    ProbGEQ3=1;
    Prob3=1;
    ProbGEQ4=1;
    return;
  }

  if (f_tageff_ == 0) { //if the b-tag eff file is not there make sure it will be obvious to the user -- fill all with zero
    Prob0=0;
    ProbGEQ1=0;
    Prob1=0;
    ProbGEQ2=0;
    Prob2=0;
    ProbGEQ3=0;
    Prob3=0;
    ProbGEQ4=0;
    return;
  }

  char btageffname[200], ctageffname[200], ltageffname[200];
  std::string sbtageff = "h_btageff";  std::string sctageff = "h_ctageff";  std::string sltageff = "h_ltageff";
  sprintf(btageffname,"%s",sbtageff.c_str());   
  sprintf(ctageffname,"%s",sctageff.c_str());   
  sprintf(ltageffname,"%s",sltageff.c_str());   
  TH1D * h_btageff  = static_cast<TH1D*>(f_tageff_->Get(btageffname));
  TH1D * h_ctageff  = static_cast<TH1D*>(f_tageff_->Get(ctageffname));
  TH1D * h_ltageff  = static_cast<TH1D*>(f_tageff_->Get(ltageffname));

  for (unsigned int ijet=0; ijet<jets_AKPF_pt->size(); ++ijet) {
    double subprob1=0;
    double subprob2=0;

    if(isGoodJet(ijet,true,pt_cut)){ //switch to 40 GeV threshold for MJ note

      double effi = GetJetTagEff(ijet, h_btageff, h_ctageff, h_ltageff);
      //      cout << "jet: " << ijet << ", effi: " << effi << endl;
      Prob0 = Prob0* ( 1 - effi);
      
      double product = 1;
      for (unsigned int kjet=0; kjet<jets_AKPF_pt->size(); ++kjet) {
	if(isGoodJet(kjet,true,pt_cut)){
	  double effk = GetJetTagEff(kjet, h_btageff, h_ctageff, h_ltageff);
	  if(kjet != ijet) product = product*(1-effk);
	  if(kjet > ijet){
	    double subproduct = 1;
	    for (unsigned int jjet=0; jjet<jets_AKPF_pt->size(); ++jjet) {
	      if(jjet != kjet && jjet != ijet){
		if(isGoodJet(jjet,true,pt_cut)){
		  double effj = GetJetTagEff(jjet, h_btageff, h_ctageff, h_ltageff);
		  subproduct = subproduct*(1-effj);
                  if ( jjet > kjet) {
		    double subproduct2 = 1;
		    for (unsigned int ljet=0; ljet<jets_AKPF_pt->size(); ++ljet) {
		      if(ljet != kjet && ljet != ijet && ljet != jjet){
			if(isGoodJet(ljet,true,pt_cut)){
			  double effl = GetJetTagEff(ljet, h_btageff, h_ctageff, h_ltageff);
			  subproduct2 = subproduct2*(1-effl);
			}
                      }
                    } //ljet loop
                    subprob2 += effk*effj*subproduct2;
		  }
		}
	      }
	    }//j loop
	    subprob1 += effk*subproduct;
	  }
	}
      }//k loop
      
      Prob1 += effi*product;
      Prob2 += effi*subprob1;
      Prob3 += effi*subprob2; 

    }
  }

  //  std::cout << "prob0 = " << Prob0 << ", prob1 = " << Prob1 << ", prob2 = " << Prob2 << std::endl;  

  ProbGEQ1 = 1 - Prob0;
  ProbGEQ2 = 1 - Prob1 - Prob0;
  ProbGEQ3 = 1 - Prob2 - Prob1 - Prob0;
  ProbGEQ4 = 1 - Prob3 - Prob2 - Prob1 - Prob0;

  return;

}

//get MC btag efficiency
double EventHandler::GetJetTagEff(unsigned int ijet, TH1D* h_btageff, TH1D* h_ctageff, TH1D* h_ltageff, double pt_cut) {

  double tageff=0;
  const float pt = jets_AKPF_pt->at(ijet);
  const float eta = fabs(jets_AKPF_eta->at(ijet));
  int flavor = static_cast<int>(jets_AKPF_partonFlavour->at(ijet));

  //x is the pt value that will be used to evaluate the SF
  //the max pt depends on flavor, and, for LF jets, eta
  const double cutoff1=800.; const double cutoff2=700.;
  double x;
  //HF or central LF, max is 800
  if ( abs(flavor)==5 || abs(flavor)==4 || eta<1.5) x = pt > cutoff1 ? cutoff1 : pt;
  //high eta LF, max is 700
  else x = pt > cutoff2 ? cutoff2 : pt;

  if( isGoodJet(ijet,true,pt_cut) ) {

    //  if (theBTagEffType_ == kBTagEff05 || theBTagEffType_ == kBTagEffup5 || theBTagEffType_ == kBTagEffdown5) { //new  BTV-11-004 prescription 

    if (abs(flavor) ==4 || abs(flavor)==5) { //heavy flavor
      double errFactor = 1;

      if (pt >cutoff1) { //use twice the errors
	errFactor=2;
      }
      if (abs(flavor) == 4)  errFactor=2; //charm has double the errors   "SFc = SFb with twice the quoted uncertainty"
      //not clear to me what to do for charm with pT> cutoff. errFactor of 2 or 4? must be small effect though

      // Tagger: CSVM within 20 < pt < 800 GeV, abs(eta) < 2.4, x = pt
      double  SFb = 0.726981*((1.+(0.253238*x))/(1.+(0.188389*x)));

      //apply FASTSTIM correction where needed
      SFb *= GetbJetFastsimSF("value",flavor,pt);
       
      // skip syst variations, for now

      // cout<<"jet flavor, pt, SF = "<<abs(flavor)<<" "<<pt<<" "<<SFb<<endl;
      if      (abs(flavor) == 5) tageff = SFb * h_btageff->GetBinContent( h_btageff->FindBin( pt ) );
      else if (abs(flavor) == 4) tageff = SFb * h_ctageff->GetBinContent( h_ctageff->FindBin( pt ) );
      else assert(0);
    } // if heavy flavor
    else { //light flavor [ see https://twiki.cern.ch/twiki/pub/CMS/BtagPOG/SFlightFuncs_Moriond2013.C ]
      //note that these are valid only to a cutoff value, so use 'x' and not 'pt'
      double SF=0;
      double nominalSF=0;
      if ( eta < 0.8 ) {
	nominalSF =  ((1.06238+(0.00198635*x))+(-4.89082e-06*(x*x)))+(3.29312e-09*(x*(x*x))); // SF without + or - variation in mistag rate
	SF = nominalSF;
      }
      else if (eta>=0.8 && eta<1.6) {
	nominalSF = ((1.08048+(0.00110831*x))+(-2.96189e-06*(x*x)))+(2.16266e-09*(x*(x*x)));
	SF = nominalSF;
      }
      else if (eta>=1.6 && eta<=2.4) {
	nominalSF = ((1.09145+(0.000687171*x))+(-2.45054e-06*(x*x)))+(1.7844e-09*(x*(x*x)));
	SF = nominalSF;
      }
      //design question -- what do to for jets at eta>2.4? assert? or return tageff=0?
      //i guess tageff 0 makes the most sense, so leave SF = 0
	
      // double deltaSF = SF - nominalSF; //here is the nominal uncertainty on the fullsim SF

      //5 June 2013 -- new code to apply LF mistags
      //apply FASTSTIM correction where needed
      SF *= GetbJetFastsimSF("value",flavor,pt);

      // now, skip the extra uncertainties for high pT/eta and Fastsim

      tageff = SF * h_ltageff->GetBinContent( h_ltageff->FindBin( pt )); 
      // cout<<"jet flavor, pt, SF = "<<abs(flavor)<<" "<<pt<<" "<<SF<<endl;

    } // if light flavor

      //    } //if BTV-11-004 prescription
    //  else {      //no longer support old prescriptions
    //  assert(0);
    //    }
    
  }


  if (tageff<0) tageff=0;
  if (tageff>1) tageff=1;

  return tageff;
}

double EventHandler::GetbJetFastsimSF(const TString & what, const int flavor, const double pt) const {
  assert( what == "value" || what=="stat" || what=="syst");
  //stat will be an absolute error
  //syst will be a fractional error

  double returnVal = 0;

  //first check if we're in a FASTSIM model
  if(sampleName.find("FSIM")==std::string::npos) return 1.;

  if ( abs(flavor) == 5) {
    //table 2 applies to any FASTSIM sample
    if (what=="value")   returnVal = Get_AN_12_175_Table2_Value(pt);
    else assert(0);
  } //end b-tag
    //new code for other flavors, from Pawandeep
  else if ( abs(flavor) == 4) { //correction for c-tag 
    //table 10 applies to any FASTSIM sample
    if (what=="value")   returnVal = Get_AN_12_175_Table10_Value(pt);
    else assert(0);
  } //end c-tag
  else  { //correction for udsg-tag 
    //table 18 applies to any FASTSIM sample
    if (what=="value") returnVal = Get_AN_12_175_Table18_Value(pt);
    else assert(0);
  } //end udsg-tag   
  
  //cout<<what<<" "<<flavor<<" "<<pt<<"\t"<<returnVal<<endl;
  return returnVal;
}

int EventHandler::GetPtBinIndex(const float pt) const {
  //binning in AN-12-175

  const int n=15; double bins[]={30.,40.,50.,60.,70.,80.,100.,120.,160.,210.,260.,320.,400.,500.,1e9}; //15 values
  //real max is 670, but use last bin value for jets about 670

  if (pt<bins[0] || pt>bins[n-1]) return -1;

  int jj=0; //highest value of jj in loop is 13
  for ( ; jj<n-1; ++jj) if (pt >= bins[jj] && pt<bins[jj+1]) break;

  return jj;  
}

double EventHandler::Get_AN_12_175_Table2_Value(const double pt) const {

  int bin = GetPtBinIndex(pt);

  if (bin<0) return 1;

  double values[]={0.982,0.981,0.992,0.994,0.997,0.9998,1.001,1.000,0.992,0.979,0.947,0.928,0.87,0.84};

  return values[bin];
}

double EventHandler::Get_AN_12_175_Table10_Value(const double pt) const {

  int bin = GetPtBinIndex(pt);
  if (bin<0) return 1;

  double values[]={0.989,0.982,1.009,1.016,1.028,1.022,1.026,1.019,0.99,0.96,0.94,0.92,0.94,1.1};

  return values[bin];
}

double EventHandler::Get_AN_12_175_Table18_Value(const double pt) const {

  int bin = GetPtBinIndex(pt);
  if (bin<0) return 1;

  double values[]={1.32,1.37,1.49,1.48,1.54,1.64,1.62,1.79,1.78,1.93,2.17,2.48,3.0,3.9};

  return values[bin];
}

// JES stuff
double EventHandler::GetUncorrectedJetPt(unsigned int ijet) const {
  return corrJets[ijet].GetPt(RAW);
}


bool EventHandler::PassesJSONCut() const{
  if(sampleName.find("Run2012")!=std::string::npos){
    if(sampleName.find("PromptReco")!=std::string::npos
       &&!inJSON(VRunLumiPrompt, run, lumiblock)) return false;
    if(sampleName.find("24Aug")!=std::string::npos
       && !inJSON(VRunLumi24Aug, run, lumiblock)) return false;
    if(sampleName.find("13Jul")!=std::string::npos
       && !inJSON(VRunLumi13Jul, run, lumiblock)) return false;
    return true;
  }else{
    return true;
  }
}

bool EventHandler::PassesSpecificTrigger(const std::string trigger) const{ // just check if a specific trigger fired
  for(unsigned int a=0; a<trigger_name->size(); ++a){
    if(trigger_name->at(a).find(trigger)!=std::string::npos
       && trigger_prescalevalue->at(a)==1 && trigger_decision->at(a)==1){
      return true;
    }
  }
  return false;
}

bool EventHandler::Passes2012RA2bTrigger() const{
  for(unsigned int a=0; a<trigger_name->size(); ++a){
    if((trigger_name->at(a).find("HLT_PFHT350_PFMET100_v")!=std::string::npos
	|| trigger_name->at(a).find("HLT_PFNoPUHT350_PFMET100_v")!=std::string::npos
	|| trigger_name->at(a).find("HLT_PFHT650_v")!=std::string::npos
	|| trigger_name->at(a).find("HLT_PFNoPUHT650_v")!=std::string::npos
	|| trigger_name->at(a).find("HLT_DiCentralPFJet50_PFMET80_v")!=std::string::npos
	|| trigger_name->at(a).find("HLT_DiCentralPFNoPUJet50_PFMETORPFMETNoMu80_v")!=std::string::npos)
       && trigger_prescalevalue->at(a)==1 && trigger_decision->at(a)==1){
      return true;
    }
  }
  return false;
}

void EventHandler::GetSortedJets() const{
  if(!JetsUpToDate){ 
    //	cout << "GetSortedJets" << endl;
    corrJets.clear();
    for(unsigned int ijet(0); ijet<jets_AKPF_pt->size(); ++ijet){
      // if(isGoodJet(i,true,20.)){ //lower pt cut
      double btag_disc(0.);
      if (cfAVersion<77) btag_disc=jets_AKPF_btag_secVertexCombined->at(ijet);
      else btag_disc=jets_AKPF_btag_inc_secVertexCombined->at(ijet); 
      // do the JEC here -- store it all in the jet vector
      TLorentzVector tlvIn(jets_AKPF_px->at(ijet),jets_AKPF_py->at(ijet),jets_AKPF_pz->at(ijet),jets_AKPF_energy->at(ijet));
      double corrFactorOld = jets_AKPF_corrFactorRaw->at(ijet);
      TLorentzVector tlvRaw = corrFactorOld * tlvIn;
      double corr(0);
      std::vector<float> subCorr(0,0);
      TLorentzVector tlvCorr;
      if (cfAVersion>=78) {
	jet_corrector_pfL1FastJetL2L3_->setRho(fixedGridRhoFastjetAll);
	jet_corrector_pfL1FastJetL2L3_->setJetA(jets_AKPF_area->at(ijet));
	jet_corrector_pfL1FastJetL2L3_->setJetPt (tlvRaw.Pt());
	jet_corrector_pfL1FastJetL2L3_->setJetEta(tlvRaw.Eta());
	corr = jet_corrector_pfL1FastJetL2L3_->getCorrection();
	// have to reload each time
	jet_corrector_pfL1FastJetL2L3_->setRho(fixedGridRhoFastjetAll);
	jet_corrector_pfL1FastJetL2L3_->setJetA(jets_AKPF_area->at(ijet));
	jet_corrector_pfL1FastJetL2L3_->setJetPt (tlvRaw.Pt());
	jet_corrector_pfL1FastJetL2L3_->setJetEta(tlvRaw.Eta());
	subCorr = jet_corrector_pfL1FastJetL2L3_->getSubCorrections();
	if (corr<0) corr=1.; // don't apply correction if it's negative (known issue with V4)
	tlvCorr = corr*tlvRaw;
      } else {
	corr = 1/corrFactorOld;
	tlvCorr = tlvIn; // for pre-v78 samples, just use the default JEC
      }
      corrJets.push_back(Jet(ijet, btag_disc, tlvIn, 1/corrFactorOld, tlvRaw, corr, tlvCorr, subCorr));
      for (unsigned int imu(0); imu<muons_.size(); imu++) {
	if (JetOverlaps(ijet, mus_pt->at(muons_[imu]), mus_eta->at(muons_[imu]), mus_phi->at(muons_[imu]))) corrJets.back().SetisLepton(true);
      }
      for (unsigned int iel(0); iel<electrons_.size(); iel++) {
	if (JetOverlaps(ijet, els_pt->at(electrons_[iel]), els_scEta->at(electrons_[iel]), els_phi->at(electrons_[iel]))) corrJets.back().SetisLepton(true);
      }
      if (cfAVersion>=78) {
	for (unsigned int iph(0); iph<photons_.size(); iph++) {
	  if (JetOverlaps(ijet, photons_pt->at(photons_[iph]), photons_scEta->at(photons_[iph]), photons_phi->at(photons_[iph]))) corrJets.back().SetisPhoton(true);
	}
      }
      //SetupJetOverlaps(corrJets[ijet]);
    }
    std::sort(corrJets.begin(),corrJets.end(), std::greater<Jet>());
    // Key: we're sorting by pt now (corrected if v78+), not CSV
    JetsUpToDate=true;
    // cout << "Started with " << jets_AKPF_pt->size() << " jets, now have " << corrJets.size() << endl;
    // 		for(unsigned int ijet(0); ijet<jets_AKPF_pt->size(); ++ijet){
    // 			printf("Input jet %d : pt=%3.2f, rawE=%3.2f\n", ijet, jets_AKPF_pt->at(ijet), jets_AKPF_energy->at(ijet)*jets_AKPF_corrFactorRaw->at(ijet));
    // 			printf("Saved jet %d : pt=%3.2f, rawE=%3.2f\n", corrJets[ijet].GetIndex(), corrJets[ijet].GetTLV(DEF).Pt(), corrJets[ijet].GetTLV(RAW).E());
    // 			printf("Corrected jet %d : pt=%3.2f, rawE=%3.2f\n", corrJets[ijet].GetIndex(), corrJets[ijet].GetTLV(CORR).Pt(), corrJets[ijet].GetTLV(CORR).E()*corrJets[ijet].GetCorr(CORR));
    // 		}
  }
}

// void EventHandler::SetupJetOverlaps(int ijet) {
//   for (unsigned int imu(0); imu<muons_.size(); imu++) {
//     if (JetOverlaps(ijet, mus_pt->at(muons[imu]), mus_eta->at(muons[imu]), mus_phi->at(muons[imu]))) corrJets[ijet].SetOverlapIndex(MUON, muons_[imu]);
//   }
// }

void EventHandler::SetCorrectedMET() {
 // if (cfAVersion<78) {
    theMET_ =  pfTypeImets_et->at(0);
    theMETPhi_ = pfTypeImets_phi->at(0);
    return;
  // }
 //  else {
 //     double METx = raw_met_et*cos(raw_met_phi);
 //     double METy = raw_met_et*sin(raw_met_phi);
 //     for(unsigned int ijet = 0; ijet < corrJets.size(); ijet++) {
 //       TLorentzVector rawJet = corrJets[ijet].GetTLV(RAW);
 //       TLorentzVector corrJet = corrJets[ijet].GetTLV(theJESType_);
 //       if ( rawJet.Pt() > 10) {
 // 	 METx += rawJet.Px();
 // 	 METx -= corrJet.Px();
 // 	 METy += rawJet.Py();
 // 	 METy -= corrJet.Py();
 //       }
 //     }
 //  double correctedMET = sqrt(METx*METx + METy*METy);
 //  double correctedMETPhi = atan2(METy,METx);
 //  theMET_ = correctedMET;
 //  theMETPhi_ = TVector2::Phi_mpi_pi(correctedMETPhi);
 //  }
 //  return;
}

void EventHandler::ClusterFatJets() const{
  if(!FatJetsUpToDate){
    sortedFatJetCache.clear();
    //      cout << "ClusterFatJets" << endl;
    vector<PseudoJet> fjets_skinny_30(0);
    vector<PseudoJet> skinny_jets_pt30;
    for(size_t jet = 0; jet<corrJets.size(); ++jet){
      // if(is_nan(jets_AKPF_px->at(jet)) || is_nan(jets_AKPF_py->at(jet))
      // 	  || is_nan(jets_AKPF_pz->at(jet)) || is_nan(jets_AKPF_energy->at(jet))) continue;
      TLorentzVector tlv = corrJets[jet].GetTLV(theJESType_);
      const PseudoJet this_pj(tlv.Px(), tlv.Py(), tlv.Pz(), tlv.E());
      if(this_pj.pt()>30.0&&fabs(this_pj.eta())<5.0) skinny_jets_pt30.push_back(this_pj);
    }
    //  cout << "Found " << skinny_jets_pt30.size() << " skinny jets." << endl;
    // Define clustering parameters
    fastjet::Strategy strategy = fastjet::Best;
    fastjet::RecombinationScheme recomb_scheme = fastjet::E_scheme;
    fastjet::JetDefinition jet_def(fastjet::antikt_algorithm, 1.2, recomb_scheme, strategy);
    // run the jet clustering with the above jet definition
    fastjet::ClusterSequence cs_skinny_30(skinny_jets_pt30, jet_def);
    //   cout << "Found " << cs_skinny_30.inclusive_jets().size() << " fat jets." << endl;
    fjets_skinny_30 = sorted_by_pt(cs_skinny_30.inclusive_jets());
    for(size_t jet = 0; jet<fjets_skinny_30.size(); ++jet){ // now add fat jets to cache
      // double px = sortedFatJetCache[index].GetTLV().Px();
      // double py = sortedFatJetCache[index].GetTLV().Py();
      // if (TMath::Sqrt(px*px+py*py)>50.)
	sortedFatJetCache.push_back(FatJet(TLorentzVector(fjets_skinny_30[jet].px(),fjets_skinny_30[jet].py(),fjets_skinny_30[jet].pz(),fjets_skinny_30[jet].E()),fjets_skinny_30[jet].constituents().size(),jet));
    }
  }
  FatJetsUpToDate=true;
}

void EventHandler::GetSortedFatJets() const{
  if(!FatJetsUpToDate){
    sortedFatJetCache.clear();
    if (cfAVersion>=77) {
      //  cout << "Load sortedFatJetCache..." << endl;
      for(unsigned int i(0); i<fjets30_pt->size(); ++i){
	TLorentzVector TLV;
	TLV.SetPtEtaPhiE(fjets30_pt->at(i), fjets30_eta->at(i), fjets30_phi->at(i), fjets30_energy->at(i));
	if (fjets30_pt->at(i)>50) sortedFatJetCache.push_back(FatJet(TLV, 0, i));
      }
      std::sort(sortedFatJetCache.begin(),sortedFatJetCache.end(), std::greater<FatJet>());
      //   cout << "Found " << sortedFatJetCache.size() << " fat jets." << endl;
      // Sorted by mJ
      FatJetsUpToDate=true;
    }
    else ClusterFatJets();
    FatJetsUpToDate=true;
    //  cout << "Found " << sortedFatJetCache.size() << " fat jets." << endl;
  }
}

double EventHandler::GetHighestFatJetmJ(unsigned int pos) const{
  GetSortedFatJets();
  --pos;
  if(pos>=sortedFatJetCache.size()){
    return -DBL_MAX;
  }else{
    return sortedFatJetCache.at(pos).GetmJ();
  }
}

TVector2 EventHandler::GetMHTVec(const double pt_cut, const double eta_cut, const uint exclude) const {
  if(!JetsUpToDate) GetSortedJets();
  double px(0.), py(0.);
  for (uint ijet(0); ijet<corrJets.size(); ++ijet){
    if(isGoodJet(corrJets[ijet].GetIndex(),true,pt_cut,eta_cut)&&ijet!=exclude){
      px+=corrJets[ijet].GetTLV(theJESType_).Px();
      py+=corrJets[ijet].GetTLV(theJESType_).Py();
    }
  }
  TVector2 vec(-1*px, -1*py);
  return vec;
}

double EventHandler::GetMHT(const double pt_cut, const double eta_cut) const {
  TVector2 vec = GetMHTVec(pt_cut, eta_cut);
  return vec.Mod();
}

double EventHandler::GetMHTPhi(const double pt_cut, const double eta_cut) const {
  TVector2 vec = GetMHTVec(pt_cut, eta_cut);
  return TVector2::Phi_mpi_pi(vec.Phi());
}

TVector2 EventHandler::GetRawMHTVec(const double pt_cut, const double eta_cut, const uint exclude) const {
  if(!JetsUpToDate) GetSortedJets();
  double px(0.), py(0.);
  for (uint ijet(0); ijet<corrJets.size(); ++ijet){
    if(isGoodJet(corrJets[ijet].GetIndex(),true,pt_cut,eta_cut,true)&&ijet!=exclude){
      px+=corrJets[ijet].GetTLV(RAW).Px();
      py+=corrJets[ijet].GetTLV(RAW).Py();
    }
  }
  TVector2 vec(-1*px, -1*py);
  return vec;
}

double EventHandler::GetRawMHT(const double pt_cut, const double eta_cut) const {
  TVector2 vec = GetRawMHTVec(pt_cut, eta_cut);
  return vec.Mod();
}

double EventHandler::GetRawMHTPhi(const double pt_cut, const double eta_cut) const {
  TVector2 vec = GetRawMHTVec(pt_cut, eta_cut);
  return TVector2::Phi_mpi_pi(vec.Phi());
}

TVector2 EventHandler::GetDEFMHTVec(const double pt_cut, const double eta_cut, const uint exclude) const {
  if(!JetsUpToDate) GetSortedJets();
  double px(0.), py(0.);
  for (uint ijet(0); ijet<corrJets.size(); ++ijet){
    if(isGoodJet(corrJets[ijet].GetIndex(),true,pt_cut,eta_cut,false,true)&&ijet!=exclude){
      px+=corrJets[ijet].GetTLV(RAW).Px();
      py+=corrJets[ijet].GetTLV(RAW).Py();
    }
  }
  TVector2 vec(-1*px, -1*py);
  return vec;
}

double EventHandler::GetDEFMHT(const double pt_cut, const double eta_cut) const {
  TVector2 vec = GetDEFMHTVec(pt_cut, eta_cut);
  return vec.Mod();
}

double EventHandler::GetDEFMHTPhi(const double pt_cut, const double eta_cut) const {
  TVector2 vec = GetDEFMHTVec(pt_cut, eta_cut);
  return TVector2::Phi_mpi_pi(vec.Phi());
}

double EventHandler::GetHighestJetPt(unsigned int pos) const{
  GetSortedJets();
  --pos;
  if(pos>=corrJets.size()){
    return -DBL_MAX;
  }else{
    return corrJets.at(pos).GetTLV(theJESType_).Pt();
  }
}

int EventHandler::GetJetXIndex(unsigned int pos) const{
  GetSortedJets();
  --pos;
  if(pos>=corrJets.size()){
    return -1;
  }else{
    return corrJets.at(pos).GetIndex();
  }
}

double EventHandler::GetJetXEta(unsigned int pos) const{
  GetSortedJets();
  --pos;
  if(pos>=corrJets.size()){
    return -DBL_MAX;
  }else{
    return corrJets.at(pos).GetTLV(theJESType_).Eta();
  }
}

double EventHandler::GetJetXPhi(unsigned int pos) const{
  GetSortedJets();
  --pos;
  if(pos>=corrJets.size()){
    return -DBL_MAX;
  }else{
    return TVector2::Phi_mpi_pi(corrJets.at(pos).GetTLV(theJESType_).Phi());
  }
}

double EventHandler::GetJetXPartonId(unsigned int pos) const{
  GetSortedJets();
  --pos;
  if(pos>=corrJets.size()){
    return -DBL_MAX;
  }else{
    return jets_AKPF_partonFlavour->at(corrJets[pos].GetIndex());
  }
}

// double EventHandler::GetJetXMotherId(unsigned int pos) const{
//   GetSortedJets();
//   --pos;
//   if(pos>=corrJets.size()){
//     return -DBL_MAX;
//   }else{
//     return corrJets.at(pos).GetIndex();
//   }
// }

double EventHandler::GetJetXCSV(unsigned int pos) const{
  GetSortedJets();
  --pos;
  if(pos>=corrJets.size()){
    return -DBL_MAX;
  }else{
    return corrJets.at(pos).GetBTag();
  }
}

double EventHandler::GetHighestJetCSV(const unsigned int nth_highest) const{
  std::vector<double> csvs(0);
  for(unsigned int jet(0); jet<corrJets.size(); ++jet){
    if(isGoodJet(jet,true,20.)) csvs.push_back(corrJets[jet].GetBTag());
  }
  std::sort(csvs.begin(), csvs.end(), std::greater<double>());
  if(nth_highest<=csvs.size()){
    return csvs.at(nth_highest-1);
  }else{
    return -DBL_MAX;
  }
}

bool EventHandler::jetHasEMu(const int ijet) const{
  // has an isolated (veto) muon
  if (!muonsUpToDate_) {
    if (cfAVersion>=75) muons_=GetRecoMuons(true);
    else muons_=GetRA2bMuons(true);
    muonsUpToDate_=true;
  }
  for (unsigned int mu(0); mu<muons_.size(); mu++) {
    int mujet = mus_jet_ind->at(muons_[mu]);
    if (ijet==mujet) return true;
  }
  // has an isolated (veto) electron
  if (!electronsUpToDate_) {
    if (cfAVersion>=75) electrons_=GetRecoElectrons(true);
    else electrons_=GetRA2bElectrons(true);
    electronsUpToDate_=true;
  }
  for (unsigned int el(0); el<electrons_.size(); el++) {
    int eljet = els_jet_ind->at(electrons_[el]);
    if (ijet==eljet) return true;
  }
  return false;
}

bool EventHandler::isInMuonCollection(const double eta, const double phi) const{
  if (!muonsUpToDate_) {
    if (cfAVersion>=75) muons_=GetRecoMuons(true);
    else muons_=GetRA2bMuons(true);
    muonsUpToDate_=true;
  }
  // cout << "Found " << muons_.size() << " muons." << endl;
  for (unsigned int mu(0); mu<muons_.size(); mu++) {
    double deltaR = Math::GetDeltaR(phi, eta, mus_phi->at(muons_[mu]), mus_eta->at(muons_[mu]));
    //  printf("Muon %d, DeltaR %3.2f\n",muons_[mu], deltaR);
    if (deltaR<0.4) {
      return true;
    }
  }
  return false;
}

bool EventHandler::isInElectronCollection(const double eta, const double phi) const{
  if (!electronsUpToDate_) {
    if (cfAVersion>=75) electrons_=GetRecoElectrons(true);
    else electrons_=GetRA2bElectrons(true);
    electronsUpToDate_=true;
  }
  for (unsigned int el(0); el<electrons_.size(); el++) {
    double deltaR = Math::GetDeltaR(phi, eta, els_phi->at(electrons_[el]), els_eta->at(electrons_[el]));
    if (deltaR<0.4) {
      return true;
    }
  }
  return false;
}

// void EventHandler::GetBeta(const std::string which) const{
//   betaUpToDate=true;

//   //Clear out the vector before starting a new event!
//   beta.clear();

//   if (GetcfAVersion()<69){
//     beta.resize(jets_AKPF_pt->size(), 0.0);
//   }else{
//     int totjet = 0;
//     int matches = 0;
//     for (unsigned int ijet=0; ijet<jets_AKPF_pt->size(); ++ijet) {
//       const double pt = jets_AKPF_pt->at(ijet);
//       const double eta = fabs(jets_AKPF_eta->at(ijet));
      
//       int i = 0;
//       totjet++;
//       for (std::vector<std::vector<float> >::const_iterator itr = puJet_rejectionBeta->begin(); itr != puJet_rejectionBeta->end(); ++itr, ++i) {
//         int j = 0;
//         double mypt = 0;
//         double myeta = 0;
//         double mybeta = 0;
//         double result = 0;
//         double tmp1 = 0, tmp2 = 0, tmp3 = 0, tmp4 = 0;
//         for ( std::vector<float>::const_iterator it = itr->begin(); it != itr->end(); ++it, ++j) {
          
//           if ( (j%6)==0 ) mypt = *it;  
//           if ( (j%6)==1 ) myeta = fabs(*it); 
//           if ( (j%6)==2 ) tmp1 = *it;  
//           if ( (j%6)==3 ) tmp2 = *it;  
//           if ( (j%6)==4 ) tmp3 = *it;  
//           if ( (j%6)==5 ) tmp4 = *it;  
          
//           //if ( which == "beta" )                 result = tmp1; 
//           //else if ( which == "betaStar" )        result = tmp2; 
//           //else if ( which == "betaClassic" )     result = tmp3; 
//           //else if ( which == "betaStarClassic" ) result = tmp4; 
//           //else result = -5; //Don't assert..
          
//           if ( which.compare("beta")==0 )                 result = tmp1; 
//           else if ( which.compare("betaStar")==0 )        result = tmp2; 
//           else if ( which.compare("betaClassic")==0 )     result = tmp3; 
//           else if ( which.compare("betaStarClassic")==0 ) result = tmp4; 
//           else result = -5; //Don't assert..
          
//         }//vector of info of each jet
//         if ( mypt == pt && myeta == eta ) {
//           matches++;
//           mybeta = result;
//           beta.push_back(mybeta);
//           break;
//         }     
//       }//vector of jets
//     } //ijet
//   }
// }

void EventHandler::SetScaleFactor(const double crossSection, const double luminosity, int numEntries){
  const int maxEntriesA(chainA.GetEntries()), maxEntriesB(chainB.GetEntries());
  if(maxEntriesA!=maxEntriesB){
    fprintf(stderr,"Error: Chains have different numbers of entries.\n");
  }
  if(maxEntriesA==0 || maxEntriesB==0){
    fprintf(stderr, "Error: Empty chains.\n");
  }
  if(numEntries<0){
    numEntries=maxEntriesA;
  }
  if(numEntries>0 && luminosity>0.0 && crossSection>0.0){
    scaleFactor=luminosity*crossSection/static_cast<double>(numEntries);
  }else{
    scaleFactor=1.0;
  }
}

int EventHandler::GetcfAVersion() const{
  size_t pos(sampleName.rfind("_v"));
  if(pos!=std::string::npos && pos<(sampleName.size()-2)){
    std::istringstream iss(sampleName.substr(pos+2));
    int version(0);
    iss >> version;
    return version;
  }else{
    return 0;
  }
}


void EventHandler::SetScaleFactor(const double scaleFactorIn){
  scaleFactor=scaleFactorIn;
}


bool EventHandler::PassesPVCut() const{
  if(beamSpot_x->size()<1 || pv_x->size()<1) return false;
  const double pv_rho(sqrt(pv_x->at(0)*pv_x->at(0) + pv_y->at(0)*pv_y->at(0)));
  if(pv_ndof->at(0)>4 && fabs(pv_z->at(0))<24. && pv_rho<2.0 && pv_isFake->at(0)==0) return true;
  return false;
  return isGoodVertex(0);
}

bool EventHandler::Passes2012METCleaningCut() const{
  for(unsigned int jet(0); jet<corrJets.size(); ++jet){
    if(isProblemJet(jet)) return false;
  }
  //if(pfTypeImets_et->at(0)>2.0*pfmets_et->at(0)) return false;
  if(pfTypeImets_et->at(0)>2.0*mets_AK5_et->at(0)) return false; // updated 11/14 (JB-F)
  return cschalofilter_decision
    && (hbhefilter_decision || sampleName.find("SMS")!=std::string::npos || sampleName.find("HbbHbb")!=std::string::npos)
    && hcallaserfilter_decision 
    && ecalTPfilter_decision 
    && trackingfailurefilter_decision 
    && eebadscfilter_decision 
    && (ecallaserfilter_decision || sampleName.find("_v66")!=std::string::npos)
    && greedymuonfilter_decision 
    && inconsistentPFmuonfilter_decision 
    && scrapingVeto_decision
    && PassesBadJetFilter()
    && GetPBNR()>=1;
}

bool EventHandler::PassesCSA14METCleaningCut() const{
  for(unsigned int jet(0); jet<corrJets.size(); ++jet){
    if(isProblemJet(jet)) return false;
  }
  //if(theMET_>2.0*pfmets_et->at(0)) return false;
  // if(theMET_>2.0*mets_AK5_et->at(0)) return false; // updated 11/14 (JB-F)
  return cschalofilter_decision
    && HBHENoisefilter_decision
    && hcallaserfilter_decision 
    && goodVerticesfilter_decision 
    && trackingfailurefilter_decision 
    && eebadscfilter_decision 
    && (ecallaserfilter_decision || sampleName.find("_v66")!=std::string::npos)
    && trkPOG_logErrorTooManyClustersfilter_decision 
    && EcalDeadCellTriggerPrimitivefilter_decision 
    && trkPOG_manystripclus53Xfilter_decision
    && METFiltersfilter_decision
    && trkPOG_toomanystripclus53Xfilter_decision
    && PassesBadJetFilter()
    && GetPBNR()>=1;
}

bool EventHandler::isProblemJet(const unsigned int ijet) const{
  return corrJets[ijet].GetTLV(theJESType_).Pt()>50.0
    && fabs(corrJets[ijet].GetTLV(theJESType_).Eta())>0.9
    && fabs(corrJets[ijet].GetTLV(theJESType_).Eta())<1.9
						      && jets_AKPF_chg_Mult->at(corrJets[ijet].GetIndex())-jets_AKPF_neutral_Mult->at(corrJets[ijet].GetIndex())>=40;
}

bool EventHandler::PassesBadJetFilter() const{
  for(unsigned int i(0); i<corrJets.size(); ++i){
    if(isGoodJet(i,false,30.0,DBL_MAX) && !isGoodJet(i,true,30.0,DBL_MAX)) return false;
  }
  return true;
}

int EventHandler::GetPBNR() const{
  //RA2 particle-based noise rejection                                                                                                                                           

  bool nhBad=false;
  bool phBad=false;
  for (unsigned int it = 0; it<corrJets.size(); it++) {
    //cfA version from Keith                                                                                                                                                     
    double NHF = jets_AKPF_neutralHadE->at(corrJets[it].GetIndex())/corrJets[it].GetTLV(RAW).E();
    double PEF = jets_AKPF_photonEnergy->at(corrJets[it].GetIndex())/corrJets[it].GetTLV(RAW).E();
    // apparently we should cut at pt>30
    if (corrJets[it].GetPt(theJESType_)<30.) continue;
    if (NHF > 0.9)  nhBad = true;
    if (PEF > 0.95) phBad = true;
  }

  if (nhBad && phBad) return -3;
  else if (phBad) return -2;
  else if (nhBad) return -1;
  return 1;
}

bool EventHandler::isGoodJet(const unsigned int ijet, const bool jetid, const double ptThresh, const double etaThresh, const bool use_raw_pt, const bool use_def_pt/*, const bool doBeta*/) const{
	// note: ijet is now the index is the corrJets vector (should be the same for v77 and earlier...)
	TLorentzVector tlv = corrJets[ijet].GetTLV(theJESType_);
	if (use_raw_pt) tlv = corrJets[ijet].GetTLV(RAW);
	else if (use_def_pt) tlv = corrJets[ijet].GetTLV(DEF);
	if(tlv.Pt()<ptThresh || fabs(tlv.Eta())>etaThresh) return false;
	if( jetid && !corrJets[ijet].isLepton() && !jetPassLooseID(ijet)) return false;
	// if(!betaUpToDate) GetBeta();
	//  if(beta.at(ijet)<0.2 && doBeta) return false;
	return true;
}

bool EventHandler::isGoodJet_Old(const unsigned int ijet, const bool jetid, const double ptThresh, const double etaThresh/*, const bool doBeta*/) const{
  if(jets_AKPF_pt->at(ijet)<ptThresh || fabs(jets_AKPF_eta->at(ijet))>etaThresh) return false;
  if( jetid && !jetPassLooseID_Old(ijet) ) return false;
  // if(!betaUpToDate) GetBeta();
  // if(beta.at(ijet)<0.2 && doBeta) return false;
  if (cfAVersion>=75&&cfAVersion<=76) { // overlap removal
    if (jetHasEMu(ijet)) return false;
  }
  return true;
}

bool EventHandler::isCleanJet(const unsigned int ijet, const int pdgId) const{ // remove overlap, based on pfcand hypothesis
  double jet_eta(corrJets[ijet].GetTLV(theJESType_).Eta()), jet_phi(TVector2::Phi_mpi_pi(corrJets[ijet].GetTLV(theJESType_).Phi()));
  for (unsigned int it = 0; it<pfcand_pt->size(); it++) {
    if (static_cast<int>(fabs(pfcand_pdgId->at(it)))!=pdgId) continue;
    double dR = Math::GetDeltaR(jet_phi, jet_eta, pfcand_phi->at(it), pfcand_eta->at(it));
    if (dR<=0.5) return false;
  }
  return true;
}

bool EventHandler::jetPassLooseID(const unsigned int ijet) const{
  //want the uncorrected energy
  const double raw_jet_energy = corrJets[ijet].GetTLV(RAW).E();
  const int numConst = static_cast<int>(jets_AKPF_mu_Mult->at(corrJets[ijet].GetIndex())+jets_AKPF_neutral_Mult->at(corrJets[ijet].GetIndex())+jets_AKPF_chg_Mult->at(corrJets[ijet].GetIndex())); //stealing from Keith
  
  if (raw_jet_energy>0.0) {
    if (jets_AKPF_neutralHadE->at(corrJets[ijet].GetIndex()) /raw_jet_energy <= 0.99
        && jets_AKPF_neutralEmE->at(corrJets[ijet].GetIndex()) / raw_jet_energy <= 0.99
        && numConst >= 2
        && ( fabs(corrJets[ijet].GetTLV(theJESType_).Eta())>=2.4
             || (fabs(corrJets[ijet].GetTLV(theJESType_).Eta())<2.4 && jets_AKPF_chgHadE->at(corrJets[ijet].GetIndex())/raw_jet_energy>0))
        && ( fabs(corrJets[ijet].GetTLV(theJESType_).Eta())>=2.4
             || (fabs(corrJets[ijet].GetTLV(theJESType_).Eta())<2.4 && jets_AKPF_chgEmE->at(corrJets[ijet].GetIndex())/raw_jet_energy<0.99))
        && ( fabs(corrJets[ijet].GetTLV(theJESType_).Eta())>=2.4
             || (fabs(corrJets[ijet].GetTLV(theJESType_).Eta())<2.4 && jets_AKPF_chg_Mult->at(corrJets[ijet].GetIndex())>0))){
      return true;
    }
  }
  return false;
}

bool EventHandler::jetPassLooseID_Old(const unsigned int ijet) const{
	//want the uncorrected energy
	const double jetenergy = jets_AKPF_energy->at(ijet) * jets_AKPF_corrFactorRaw->at(ijet);
	const int numConst = static_cast<int>(jets_AKPF_mu_Mult->at(ijet)+jets_AKPF_neutral_Mult->at(ijet)+jets_AKPF_chg_Mult->at(ijet)); //stealing from Keith
	if (jetenergy>0.0) {
		if (jets_AKPF_neutralHadE->at(ijet) /jetenergy <= 0.99
			&& jets_AKPF_neutralEmE->at(ijet) / jetenergy <= 0.99
				&& numConst >= 2
					&& ( fabs(jets_AKPF_eta->at(ijet))>=2.4
						|| (fabs(jets_AKPF_eta->at(ijet))<2.4 && jets_AKPF_chgHadE->at(ijet)/jetenergy>0))
							&& ( fabs(jets_AKPF_eta->at(ijet))>=2.4
								|| (fabs(jets_AKPF_eta->at(ijet))<2.4 && jets_AKPF_chgEmE->at(ijet)/jetenergy<0.99))
									&& ( fabs(jets_AKPF_eta->at(ijet))>=2.4
		|| (fabs(jets_AKPF_eta->at(ijet))<2.4 && jets_AKPF_chg_Mult->at(ijet)>0))){
			return true;
		}
	}
	return false;
}

bool EventHandler::isRA2bElectron(const unsigned int k,
				  const unsigned short level,
				  const bool use_iso) const{
  //N.B.: cut does not have the fabs(1/E-1/p) and conversion rejection cuts from the EGamma POG!!!
  const double dmax(std::numeric_limits<double>::max());
  double pt_cut(5.0); //Not actually part of the EGamma ID
  double eta_cut(0.007), phi_cut(0.8), sigmaietaieta_cut(0.01), h_over_e_cut(0.15), d0_cut(0.04), dz_cut(0.2), iso_cut(0.15);
  // cout << "Found " << pf_els_pt->size() << " pf_els" << endl; 
  switch(level){
  case 1:
    // cout << "signal electron... " << k << endl;
    pt_cut=20.0;
    if(pf_els_isEB->at(k)){
      eta_cut=0.007; phi_cut=0.15; sigmaietaieta_cut=0.01; h_over_e_cut=0.12;
      d0_cut=0.02; dz_cut=0.2; iso_cut=0.15;
    }else if(pf_els_isEE->at(k)){
      eta_cut=0.009; phi_cut=0.10; sigmaietaieta_cut=0.03; h_over_e_cut=0.1;
      d0_cut=0.02; dz_cut=0.2; iso_cut=(pf_els_pt->at(k)>20.0?0.15:0.10);
    }
    break;
  case 2:
    pt_cut=5.0;
    if(pf_els_isEB->at(k)){
      eta_cut=0.004; phi_cut=0.06; sigmaietaieta_cut=0.01; h_over_e_cut=0.12;
      d0_cut=0.02; dz_cut=0.1; iso_cut=0.15;
    }else if(pf_els_isEE->at(k)){
      eta_cut=0.007; phi_cut=0.03; sigmaietaieta_cut=0.03; h_over_e_cut=0.1;
      d0_cut=0.02; dz_cut=0.1; iso_cut=(pf_els_pt->at(k)>20.0?0.15:0.10);
    }
    break;
  case 3:
    pt_cut=5.0;
    if(pf_els_isEB->at(k)){
      eta_cut=0.004; phi_cut=0.03; sigmaietaieta_cut=0.01; h_over_e_cut=0.12;
      d0_cut=0.02; dz_cut=0.1; iso_cut=0.10;
    }else if(pf_els_isEE->at(k)){
      eta_cut=0.005; phi_cut=0.02; sigmaietaieta_cut=0.03; h_over_e_cut=0.1;
      d0_cut=0.02; dz_cut=0.1; iso_cut=(pf_els_pt->at(k)>20.0?0.10:0.07);
    }
    break;
  case 0: //intentionally falling through to default "veto" case
  default:
    pt_cut=10.0;
    if(pf_els_isEB->at(k)){
      eta_cut=0.007; phi_cut=0.8; sigmaietaieta_cut=0.01; h_over_e_cut=0.15;
      d0_cut=0.04; dz_cut=0.2; iso_cut=0.15;
    }else if(pf_els_isEE->at(k)){
      eta_cut=0.01; phi_cut=0.7; sigmaietaieta_cut=0.03; h_over_e_cut=dmax;
      d0_cut=0.04; dz_cut=0.2; iso_cut=0.15;
    }
    break;
  }
  // if(k>pf_els_pt->size()) return false;
  if (level>0) {
    // cout << "electron kinematics ..." << endl;
    if (fabs(pf_els_scEta->at(k)) >= 2.5 ) return false;
    if (pf_els_pt->at(k) < pt_cut) return false;
  }
  if ( fabs(pf_els_dEtaIn->at(k)) > eta_cut)  return false;
  if ( fabs(pf_els_dPhiIn->at(k)) > phi_cut)  return false;
  if (pf_els_sigmaIEtaIEta->at(k) > sigmaietaieta_cut) return false;
  if (pf_els_hadOverEm->at(k) > h_over_e_cut) return false;

  const double beamx(beamSpot_x->at(0)), beamy(beamSpot_y->at(0)); 
  const double d0(pf_els_d0dum->at(k)-beamx*sin(pf_els_tk_phi->at(k))+beamy*cos(pf_els_tk_phi->at(k)));
  if ( fabs(d0) >= d0_cut ) return false;
  if ( fabs(pf_els_vz->at(k) - pv_z->at(0) ) >= dz_cut ) return false;
  // cout << "electron isolation..." << endl;
  if(GetRA2bElectronRelIso(k)>=iso_cut && use_iso) return false;
  return true;
}

double EventHandler::GetRA2bElectronRelIso(const unsigned int k) const{
  //   cout << "GetRA2bElectronRelIso" << endl;
  const double rho(rho_kt6PFJetsForIsolation2012);
  // get effective area from delR=0.3 2011 data table for neutral+gamma based on supercluster eta pf_els_scEta->at(k)
  double AE(0.10); 
  const double abseta(fabs(pf_els_scEta->at(k)));
  if      ( abseta < 1.0 ) AE = 0.13;
  else if ( abseta >=1.0 && abseta <1.479) AE = 0.14;
  else if ( abseta >=1.479&&abseta <2.0)   AE = 0.07;
  else if ( abseta >=2.0 && abseta <2.2) AE = 0.09;
  else if ( abseta >=2.2 && abseta <2.3) AE = 0.11;
  else if ( abseta >=2.3 && abseta <2.4) AE = 0.11;
  else if ( abseta >=2.4 ) AE = 0.14;

  const double eleIso(pf_els_PFphotonIsoR03->at(k) + pf_els_PFneutralHadronIsoR03->at(k) - rho*AE);
  return ( pf_els_PFchargedHadronIsoR03->at(k) + ( eleIso > 0 ? eleIso : 0.0 ) )/pf_els_pt->at(k);
}

bool EventHandler::isRA2bMuon(const unsigned int k,
			      const unsigned short level,
			      const bool use_iso) const{
  double pt_thresh(10.0);
  if(level>=1) pt_thresh=20.0;
  double eta_thresh(3.0);
  if(level>=1) eta_thresh=2.4;


  if (fabs(pf_mus_eta->at(k)) >= eta_thresh ) return false;
  if (pf_mus_pt->at(k) < pt_thresh) return false; // keep this--not already applied to slimmedMuons collection
  if ( !pf_mus_id_GlobalMuonPromptTight->at(k)) return false;
  // GlobalMuonPromptTight includes: isGlobal, globalTrack()->normalizedChi2() < 10, numberOfValidMuonHits() > 0
  if ( pf_mus_numberOfMatchedStations->at(k) <= 1 ) return false;
  const double beamx (beamSpot_x->at(0)), beamy(beamSpot_y->at(0));   
  const double d0 = pf_mus_tk_d0dum->at(k)-beamx*sin(pf_mus_tk_phi->at(k))+beamy*cos(pf_mus_tk_phi->at(k));
  const double pf_mus_vz = pf_mus_tk_vz->at(k);
  const double pf_mus_dz_vtx = fabs(pf_mus_vz-pv_z->at(0));
  if (fabs(d0)>=0.2 || pf_mus_dz_vtx>=0.5) return false;
  if ( !pf_mus_tk_numvalPixelhits->at(k)) return false;
  if ( pf_mus_tk_LayersWithMeasurement->at(k) <= 5 ) return false;

  if (GetRA2bMuonRelIso(k) > 0.2 && use_iso) return false;
  return true;
}

double EventHandler::GetRA2bMuonRelIso(const unsigned int k) const{
  double isoNeutral(pf_mus_pfIsolationR04_sumNeutralHadronEt->at(k) + pf_mus_pfIsolationR04_sumPhotonEt->at(k) - 0.5*pf_mus_pfIsolationR04_sumPUPt->at(k));
  if(isoNeutral<0.0) isoNeutral=0.0;
  return (pf_mus_pfIsolationR04_sumChargedHadronPt->at(k) + isoNeutral) / pf_mus_pt->at(k);
}

bool EventHandler::isRA2bTau(const unsigned int k,
			     const unsigned short level,
			     const bool require_iso) const{
  double pt_cut(20.0);
  if(level>0) pt_cut=30.0;

  if (taus_pt->at(k)<pt_cut) return false;
  if (fabs(taus_eta->at(k)) > 2.4) return false;
  if (require_iso && (taus_byLooseIsolationDeltaBetaCorr->at(k) <= 0)) return false;


  return true;
}


int EventHandler::GetNumRA2bElectrons(const unsigned short level, const bool use_iso) const{
  int count(0);
  for(unsigned int i(0); i<pf_els_pt->size(); ++i){
    if(isRA2bElectron(i,level, use_iso)) ++count;
  }
  return count;
}

int EventHandler::GetNumRA2bMuons(const unsigned short level, const bool use_iso) const{
  int count(0);
  for(unsigned int i(0); i<pf_mus_pt->size(); ++i){
    if(isRA2bMuon(i,level,use_iso)) ++count;
  }
  return count;
}

int EventHandler::GetNumRA2bTaus(const unsigned short level, const bool use_iso) const{
  int count(0);
  for(unsigned int i(0); i<taus_pt->size(); ++i){
    if(isRA2bTau(i,level,use_iso)) ++count;
  }
  return count;
}


bool EventHandler::isQualityTrack(const unsigned int k) const{
  if (fabs(tracks_eta->at(k))>2.4 ) return false;
  if (!(tracks_highPurity->at(k)>0)) return false;
  return true;  
}

double EventHandler::GetTrueNumInteractions() const{
  double npv(-1.0);
  for(unsigned int i(0); i<PU_bunchCrossing->size(); ++i){
    if(PU_bunchCrossing->at(i)==0){
      npv = PU_TrueNumInteractions->at(i);
    }
  }
  return npv;
}

double EventHandler::GetNumInteractions() const{
  double npv(-1.0);
  for(unsigned int i(0); i<PU_bunchCrossing->size(); ++i){
    if(PU_bunchCrossing->at(i)==0){
      npv = PU_NumInteractions->at(i);
    }
  }
  return npv;
}

double EventHandler::GetEarlyOutOfTimePU(const int BX_before) const{
  double npv(0.);
  int first_BX_to_check = 0 - BX_before;
  for(unsigned int i(0); i<PU_bunchCrossing->size(); ++i){
    if(PU_bunchCrossing->at(i)<first_BX_to_check) continue;
    if(PU_bunchCrossing->at(i)==0) break;
    npv += PU_NumInteractions->at(i);
  }
  return npv;
}

double EventHandler::GetLateOutOfTimePU() const{
  double npv(0.);
  for(unsigned int i(0); i<PU_bunchCrossing->size(); ++i){
    if(PU_bunchCrossing->at(i)==1){
      npv = PU_NumInteractions->at(i);
    }
  }
  return npv;
}

bool EventHandler::isGoodVertex(const unsigned int vertex) const{
  const double pv_rho(sqrt(pv_x->at(vertex)*pv_x->at(vertex) + pv_y->at(vertex)*pv_y->at(vertex)));
  return pv_ndof->at(vertex)>4 && fabs(pv_z->at(vertex))<24. && pv_rho<2.0 && pv_isFake->at(vertex)==0;
}

unsigned short EventHandler::GetNumVertices() const{
  unsigned short num_vertices(0);
  for(unsigned int vertex(0); vertex<pv_x->size(); ++vertex){
    if(isGoodVertex(vertex)) ++num_vertices;
  }
  return num_vertices;
  //return pv_x->size();
}

double EventHandler::GetPUWeight(reweight::LumiReWeighting &lumiWeights) const{
  if (sampleName.find("13TeV")!=std::string::npos) return 1.0;
  return lumiWeights.weight(GetNumInteractions());
}


double EventHandler::GetTopPtWeight() const{
  if (cfAVersion==73||cfAVersion>=75) return 1.0;
  double topPt(-1.0);

  //only for use with ttbar
  //(2 string comparisons for every event is not so good for performance, but oh well)
  if (sampleName.find("TTJets")!=std::string::npos || sampleName.find("TT_")!=std::string::npos) {

    //code from Darren converted to cfA
    for ( unsigned int i=0; i< mc_doc_id->size(); i++ ) {
      // look for the *top* (not antitop) pT
      if ( mc_doc_id->at(i)==6 ) { topPt = mc_doc_pt->at(i); break; }
    }

    if (topPt<0) return 1;
    // note -- we should remove this, now that it is demonstrated that powheg and mc@nlo behave the same as MG
    if ( sampleName.find("madgraph")!=std::string::npos) { //only return weight for madgraph

      const  double p0 = 1.18246e+00;
      const  double p1 = 4.63312e+02;
      const  double p2 = 2.10061e-06;

      double x=topPt;
      if ( x>p1 ) x = p1; //use flat factor above 463 GeV
      double result = p0 + p2 * x * ( x - 2 * p1 );
      return double(result);
    }
  }

  return 1;
}

double EventHandler::GetTopPt() const {
  double topPt(-1);
  for(unsigned int i(0); i<mc_doc_id->size(); ++i){
    // look for the *top* (not antitop) pT
    if(mc_doc_id->at(i)==6) topPt = mc_doc_pt->at(i);
    if(topPt>=0) break; //check to see if we're done
  }
  return topPt;
}

double EventHandler::GetTopPtWeightOfficial() const{ // New 11/07
  if (sampleName.find("13TeV")!=std::string::npos) return 1.0;
  double topweight(-1.0);

  //official recipe from
  // https://twiki.cern.ch/twiki/bin/viewauth/CMS/TopPtReweighting
  //(2 string comparisons for every event is not so good for performance, but oh well)
  if (sampleName.find("TTJets")!=std::string::npos || sampleName.find("TT_")!=std::string::npos) {
    double topPt(-1);
    double topbarPt(-1);
    for(unsigned int i(0); i<mc_doc_id->size(); ++i){
      // look for the *top* (not antitop) pT
      if(mc_doc_id->at(i)==6) topPt = mc_doc_pt->at(i);
      if(mc_doc_id->at(i)==-6) topbarPt = mc_doc_pt->at(i);
      if(topPt>=0 && topbarPt>=0) break; //check to see if we're done
    }

    //SF(x)=exp(a+bx)
    const double a(0.156); //combined 8 TeV values
    const double b(-0.00137);

    //important choice -- I've decided to use the 400 GeV value for values above 400 GeV
    //an alternative would be to just blindly use the formula
    if (topPt >400) topPt=400;
    if (topbarPt >400) topbarPt=400;

    const double SFt(exp(a + b*topPt));
    const double SFtbar(exp(a + b*topbarPt));

    topweight = sqrt( SFt * SFtbar );
  }

  if(topweight<0) topweight=1;
  return topweight;
}


int EventHandler::GetGenParticleIndex(const int pdgId, const int skip) const{
  int count(0);
  int index(-1);
  for (uint j(0); j < mc_doc_id->size(); j++) {
    if (pdgId!=fabs(mc_doc_id->at(j))) continue;
    count++;
    if (count<=skip) continue; // e.g. if you wanted the second object of this type listed in the chain
    index=j;
    break;
  }
  return index;
}

bool EventHandler::isTrueMuon(const double eta, const double phi) const{
  double min_DR(1000.);
  int i_min_DR(-1);
  for (uint imc(0); imc<mc_mus_id->size(); imc++) {
    if (fabs(static_cast<int>(mc_mus_id->at(imc)))!=13) continue;
    if (mc_mus_numOfDaughters->at(imc)>0.) continue; // in the case of Brem, remove the inital muon
    if ((mc_mus_status->at(imc)==3||mc_mus_status->at(imc)==22||mc_mus_status->at(imc)==23)) continue; // remove hard scatter (maybe redundant)
    int i_DR=imc;
    double DR=Math::GetDeltaR(phi, eta, mc_mus_phi->at(imc), mc_mus_eta->at(imc)); 
    if (DR<min_DR) 
      {
	min_DR=DR;
	i_min_DR=i_DR;
      }
  }
  if (i_min_DR<0) return false;
  if (min_DR>0.2) return false;
  if (fabs(static_cast<int>(mc_mus_mother_id->at(i_min_DR)))==24 || fabs(static_cast<int>(mc_mus_grandmother_id->at(i_min_DR)))==24 || fabs(static_cast<int>(mc_mus_ggrandmother_id->at(i_min_DR)))==24) return true;
  else return false;
}

bool EventHandler::isbMuon(const double eta, const double phi) const{
  double min_DR(1000.);
  int i_min_DR(-1);
  for (uint imc(0); imc<mc_mus_id->size(); imc++) {
    if (fabs(static_cast<int>(mc_mus_id->at(imc)))!=13) continue;
    if (mc_mus_numOfDaughters->at(imc)>0.) continue; // in the case of Brem, remove the inital muon
    if ((mc_mus_status->at(imc)==3||mc_mus_status->at(imc)==22||mc_mus_status->at(imc)==23)) continue; // remove hard scatter (maybe redundant)
    int i_DR=imc;
    double DR=Math::GetDeltaR(phi, eta, mc_mus_phi->at(imc), mc_mus_eta->at(imc)); 
    if (DR<min_DR) 
      {
	min_DR=DR;
	i_min_DR=i_DR;
      }
  }
  if (i_min_DR<0) return false;
  if (min_DR>0.2) return false;
  if ( (fabs(mc_mus_mother_id->at(i_min_DR))>500&&fabs(mc_mus_mother_id->at(i_min_DR))<600) || (fabs(mc_mus_mother_id->at(i_min_DR))>5000&&fabs(mc_mus_mother_id->at(i_min_DR))<6000) ) return true;
  if ( (fabs(mc_mus_grandmother_id->at(i_min_DR))>500&&fabs(mc_mus_grandmother_id->at(i_min_DR))<600) || (fabs(mc_mus_grandmother_id->at(i_min_DR))>5000&&fabs(mc_mus_grandmother_id->at(i_min_DR))<6000) ) return true;
  if ( (fabs(mc_mus_ggrandmother_id->at(i_min_DR))>500&&fabs(mc_mus_ggrandmother_id->at(i_min_DR))<600) || (fabs(mc_mus_ggrandmother_id->at(i_min_DR))>5000&&fabs(mc_mus_ggrandmother_id->at(i_min_DR))<6000) ) return true;
  else return false;
}

float EventHandler::GetCorrespondingTopPt(const int charge) const {
  float topPt(-1.);
  for(unsigned int imc = 0; imc < mc_doc_id->size(); imc++){
    if (static_cast<int>(mc_doc_id->at(imc))*charge==6&&(mc_doc_status->at(imc)==3||mc_doc_status->at(imc)==22||mc_doc_status->at(imc)==23)) {
      topPt = mc_doc_pt->at(imc);
    }
  }
  return topPt;
}

float EventHandler::GetCorrespondingDeltaRWb(const int charge) const {
  int the_b(-1), the_W(-1);
  for(unsigned int imc = 0; imc < mc_doc_id->size(); imc++){
    if (static_cast<int>(mc_doc_id->at(imc))*charge==24&&(mc_doc_status->at(imc)==3||mc_doc_status->at(imc)==22||mc_doc_status->at(imc)==23)) {
      the_W=imc;
    }
    if (static_cast<int>(mc_doc_id->at(imc))*charge==-5&&(mc_doc_status->at(imc)==3||mc_doc_status->at(imc)==22||mc_doc_status->at(imc)==23)) {
      the_b=imc;
    }
  }
  if (the_W<0||the_b<0) return -1.;
  return Math::GetDeltaR(mc_doc_phi->at(the_W), mc_doc_eta->at(the_W), mc_doc_phi->at(the_b), mc_doc_eta->at(the_b));
}

float EventHandler::GetCorrespondingDeltaRlb(const int gen_index) const { // actually deltaR(W,mu) now
  int the_b(-1), the_W(-1);
  // cout << "mc_mus index: " << gen_index << endl;
  // cout << "mc_mus size: " << mc_mus_eta->size() << endl;
  double gen_mu_eta(mc_mus_eta->at(gen_index)), gen_mu_phi(mc_mus_phi->at(gen_index));
  int charge(static_cast<int>(mc_mus_charge->at(gen_index)));
  for(unsigned int imc = 0; imc < mc_doc_id->size(); imc++){
    if (static_cast<int>(mc_doc_id->at(imc))*charge==24&&(mc_doc_status->at(imc)==3||mc_doc_status->at(imc)==22||mc_doc_status->at(imc)==23)) {
      the_W=imc;
    }
    if (static_cast<int>(mc_doc_id->at(imc))*charge==-5&&(mc_doc_status->at(imc)==3||mc_doc_status->at(imc)==22||mc_doc_status->at(imc)==23)) {
      the_b=imc;
    }
  }
  //  cout << "the_b: " << the_b << endl;
  if (the_W<0||the_b<0) return -1.;
  return Math::GetDeltaR(gen_mu_phi, gen_mu_eta, mc_doc_phi->at(the_b), mc_doc_eta->at(the_b));
}

bool EventHandler::IsFromB(const int mom, const int gmom, const int ggmom) const{
  if ( (mom>=500&&mom<600) || (mom>=5000&&mom<6000) || fabs(mom)==5 ) return true;
  if ( (gmom>=500&&mom<600) || (gmom>=5000&&mom<6000) || fabs(gmom)==5 ) return true;
  if ( (ggmom>=500&&mom<600) || (ggmom>=5000&&mom<6000) || fabs(ggmom)==5 ) return true;
  return false;
}

bool EventHandler::isTrueElectron(const double eta, const double phi) const{
  double min_DR(1000.);
  int i_min_DR(-1);
  for (uint imc(0); imc<mc_electrons_id->size(); imc++) {
    if (fabs(static_cast<int>(mc_electrons_id->at(imc)))!=11) continue;
    if (mc_electrons_numOfDaughters->at(imc)>0.) continue; // in the case of Brem, remove the inital electron
    if ((mc_electrons_status->at(imc)==3||mc_electrons_status->at(imc)==22||mc_electrons_status->at(imc)==23)) continue; // remove hard scatter (maybe redundant)
    int i_DR=imc;
    double DR=Math::GetDeltaR(phi, eta, mc_electrons_phi->at(imc), mc_electrons_eta->at(imc)); 
    if (DR<min_DR) 
      {
	min_DR=DR;
	i_min_DR=i_DR;
      }
  }
  if (i_min_DR<0) return false;
  if (min_DR>0.2) return false;
  if (fabs(static_cast<int>(mc_electrons_mother_id->at(i_min_DR)))==24 || fabs(static_cast<int>(mc_electrons_grandmother_id->at(i_min_DR)))==24 || fabs(static_cast<int>(mc_electrons_ggrandmother_id->at(i_min_DR)))==24) return true;
  else return false;
}

int EventHandler::GetJetGenId(const int jet) const{
  float minDR(999.);
  int pdgID(-999);
  float jet_eta(corrJets[jet].GetTLV(theJESType_).Eta()), jet_phi(TVector2::Phi_mpi_pi(corrJets[jet].GetTLV(theJESType_).Phi()));
  float thisDR(999.);
  for (int mc(0); mc<static_cast<int>(mc_doc_id->size()); mc++) {
    int status = static_cast<int>(mc_doc_status->at(mc));
    if(status==3||status==22||status==23) {
      thisDR=Math::GetDeltaR(jet_phi, jet_eta, mc_doc_phi->at(mc), mc_doc_eta->at(mc));
      if (thisDR<minDR) {
	minDR=thisDR;
	pdgID=static_cast<int>(mc_doc_id->at(mc));
      }
    }
  }
  return pdgID;
}

int EventHandler::GetNGenPartons(const float ptCut) const{
  int up = GetNGenParticles(2, ptCut);
  int down = GetNGenParticles(1, ptCut);
  int strange = GetNGenParticles(3, ptCut);
  int charm = GetNGenParticles(4, ptCut);
  int bottom = GetNGenParticles(5, ptCut);
  int gluon = GetNGenParticles(21, ptCut);
  return up+down+strange+charm+bottom+gluon;
}

int EventHandler::GetNGenParticles(const int pdgId, const float ptCut, const bool hard_scatter_only) const{
 
  uint count(0);
  for(unsigned int imc = 0; imc < mc_doc_id->size(); imc++){
    if (hard_scatter_only && !(mc_doc_status->at(imc)==3||mc_doc_status->at(imc)==22||mc_doc_status->at(imc)==23)) continue;
    if (mc_doc_pt->at(imc)<ptCut) continue;
    if (mc_doc_id->at(imc)==mc_doc_mother_id->at(imc)) continue;
    if (mc_doc_id->at(imc)==mc_doc_grandmother_id->at(imc)) continue;
    if (mc_doc_id->at(imc)==mc_doc_ggrandmother_id->at(imc)) continue;
    if (static_cast<int>(fabs(mc_doc_id->at(imc)))==pdgId) count++;
  }
  return count;
}

bool EventHandler::hasPFMatch(const int index, const int pdgId) const{
  double deltaRVal = 999.;
  double deltaPT = 999.;
  double leptonEta = 0, leptonPhi = 0, leptonPt = 0;
  if(pdgId == 13) {
    leptonEta = mus_eta->at(index);
    leptonPhi = mus_phi->at(index);
    leptonPt = mus_pt->at(index);
  }  else if(pdgId == 11) {
    leptonEta = els_scEta->at(index);
    leptonPhi = els_phi->at(index);
    leptonPt = els_pt->at(index);
  }
  
  for(unsigned iCand=0; iCand<pfcand_pt->size(); iCand++) {
    if(fabs(pfcand_pdgId->at(iCand))==pdgId) {
      double tempDeltaR = Math::GetDeltaR(leptonPhi, leptonEta, pfcand_phi->at(iCand), pfcand_eta->at(iCand));
      if(tempDeltaR < deltaRVal) {
	deltaRVal = tempDeltaR;
	deltaPT = fabs(leptonPt-pfcand_pt->at(iCand));
      }
    }
  }
  if (pdgId == 11) return (deltaPT<10);
  else return (deltaPT<5);
}

vector<int> EventHandler::GetRecoMuons(const uint level, const bool check_pt, const bool check_iso, bool check_id, const bool use_mini_iso, const double mini_iso_cut) const{
  vector<int> muons;
  for(uint index=0; index<mus_pt->size(); index++) {
    if(isRecoMuon(index, level, check_pt, check_iso, check_id, use_mini_iso, mini_iso_cut)) muons.push_back(index);
  }
  return muons;
}


double EventHandler::getDZ(double vx, double vy, double vz, double px, double py, double pz, int firstGoodVertex){
  return vz - pv_z->at(firstGoodVertex) -((vx-pv_x->at(firstGoodVertex))*px+(vy-pv_y->at(firstGoodVertex))*py)*pz/(px*px+py*py); 
}

bool EventHandler::PassMuonID(const uint imu, const bool tight) const{ // does not include pt or isolation
  if (tight) {
    if (!mus_isPF->at(imu)) return false;
    if (!mus_id_GlobalMuonPromptTight->at(imu)) return false;
    if ( mus_numberOfMatchedStations->at(imu) <= 1 ) return false;
    const double d0 = GetMuonD0(imu);
    const double mus_vz = mus_tk_vz->at(imu);
    const double mus_dz_vtx = fabs(mus_vz-pv_z->at(0));
    if (fabs(d0)>=0.2 || mus_dz_vtx>=0.5) return false;
    if ( mus_tk_numvalPixelhits->at(imu)<1 ) return false;
    if ( mus_tk_LayersWithMeasurement->at(imu) <= 5 ) return false;
    if (cfAVersion>=77&&mus_globalTrack_normalizedChi2->at(imu) > 10) return false;
  } else { // new medium ID? -- https://indico.cern.ch/event/357213/contribution/2/material/slides/0.pdf
    bool goodGlb=false;
    if (mus_isGlobalMuon->at(imu) &&
	mus_globalTrack_normalizedChi2->at(imu) < 3. &&
	mus_trkPositionMatch->at(imu) < 12. &&
	mus_trkKink->at(imu) < 20.) goodGlb=true;
    if (mus_tkHitsFrac->at(imu) < 0.8) return false;
    double segCompCut = (goodGlb) ? 0.303 : 0.451;
    if (mus_segmentCompatibility->at(imu) < segCompCut) return false;
  }

  return true;
}

unsigned int EventHandler::GetNumMediumMuons() const{
  uint nmus(0);
  for (uint imu(0); imu<mus_pt->size(); imu++) {
    if (mus_pt->at(imu)<10.) continue;
    if (fabs(mus_eta->at(imu)) >= 2.4) continue;
    if (!PassMuonID(imu, false)) continue;
    if (mus_miniso->at(imu) > 0.2) continue;
    nmus++;
  }
  return nmus;
}

bool EventHandler::isRecoMuon(const uint imu, const uint level, const bool check_pt, const bool check_iso, const bool check_id, const bool use_mini_iso, const double mini_iso_cut) const{
  if (imu>mus_pt->size()) return false;
  if (fabs(mus_eta->at(imu)) >= 2.4 ) return false;
  if (check_id&&!PassMuonID(imu)) return false; 
  double pt_thresh(10.0);
  if(level>=1) pt_thresh=20.0;
  double relIso_cut=0.2;
  if(level>=1) relIso_cut=0.12;
  if (mus_pt->at(imu)<5.) return false; // always cut here at least
  if (check_pt && mus_pt->at(imu) < pt_thresh) return false;
  if (check_iso) {
    if (use_mini_iso) {
      double mini_iso(999.);
      if (cfAVersion>=78) mini_iso = mus_miniso->at(imu);
      else mini_iso = GetIsolation(imu, 13);
      if (mini_iso>mini_iso_cut) return false;
    } else if (GetMuonRelIso(imu) > relIso_cut) return false;
  }
  return true;
}

double EventHandler::GetMuonD0(const unsigned int imu) const{
  return mus_tk_d0dum->at(imu)-pv_x->at(0)*sin(mus_tk_phi->at(imu))+pv_y->at(0)*cos(mus_tk_phi->at(imu));
}

vector<int> EventHandler::GetRA2bMuons(const bool veto) const{
  vector<int> muons;
  for(uint index=0; index<pf_mus_pt->size(); index++)
    if(veto){
      if(isRA2bMuon(index, 0)) muons.push_back(index);
    }	else {
      if(isRA2bMuon(index, 1)) muons.push_back(index);
    }
  return muons;
}

double EventHandler::GetMuonRelIso(const unsigned int imu, const double R) const{
  double isoNeutral(0.);
  if (R==0.4) {
    isoNeutral=mus_pfIsolationR04_sumNeutralHadronEt->at(imu) + mus_pfIsolationR04_sumPhotonEt->at(imu) - 0.5*mus_pfIsolationR04_sumPUPt->at(imu);
    if(isoNeutral<0.0) isoNeutral=0.0;
    return (mus_pfIsolationR04_sumChargedHadronPt->at(imu) + isoNeutral) / mus_pt->at(imu);
  }
  else if (R==0.3) {
    isoNeutral=mus_pfIsolationR03_sumNeutralHadronEt->at(imu) + mus_pfIsolationR03_sumPhotonEt->at(imu) - 0.5*mus_pfIsolationR03_sumPUPt->at(imu);
    if(isoNeutral<0.0) isoNeutral=0.0;
    return (mus_pfIsolationR03_sumChargedHadronPt->at(imu) + isoNeutral) / mus_pt->at(imu);
  }
  else return -1.;
}

vector<int> EventHandler::GetRecoTaus() const{
  vector<int> taus;
  for(uint index=0; index<taus_pt->size(); index++)
    if(isRA2bTau(index)) taus.push_back(index);
  return taus;
}

vector<int> EventHandler::GetRecoElectrons(const uint level, const bool check_pt, const bool check_iso, const bool check_id, const bool use_mini_iso, const double mini_iso_cut) const{
  vector<int> electrons;
  for(uint index=0; index<els_pt->size(); index++) {
    if(isRecoElectron(index, level, check_pt, check_iso, check_id, use_mini_iso, mini_iso_cut)) electrons.push_back(index);
  }
  return electrons;
}

vector<int> EventHandler::GetRA2bElectrons(const bool veto) const{
  vector<int> electrons;
  for(uint index=0; index<pf_els_pt->size(); index++)
    if(!veto){
      if(isRA2bElectron(index,3)) electrons.push_back(index);
    }	else {
      if(isRA2bElectron(index,0)) electrons.push_back(index);
    }
  return electrons;
}

bool EventHandler::PassElectronID(const uint iel, const uint level) const { // everything but pt, isolation

  bool barrel;
  if(fabs(els_scEta->at(iel))<=1.479){
    barrel = true;
  }else if(fabs(els_scEta->at(iel))<2.5){
    barrel = false;
  }else{
    return false;
  }

  double deta_cut, dphi_cut, ieta_cut, hovere_cut, d0_cut, dz_cut,
    ooeminusoop_cut, vprob_cut, misshits_cut;

  if(cfAVersion<75){
    //See https://twiki.cern.ch/twiki/bin/viewauth/CMS/EgammaCutBasedIdentification#Barrel_Cuts_eta_supercluster_1_4
    switch(level){
    default:
    case 0:
      deta_cut        = barrel ? 0.007   : 0.01;
      dphi_cut        = barrel ? 0.8     : 0.7;
      ieta_cut        = barrel ? 0.01    : 0.03;
      hovere_cut      = barrel ? 0.15    : DBL_MAX;
      d0_cut          = barrel ? 0.04    : 0.04;
      dz_cut          = barrel ? 0.2     : 0.2;
      ooeminusoop_cut = barrel ? DBL_MAX  : DBL_MAX;
      vprob_cut       = barrel ? DBL_MAX  : DBL_MAX;
      misshits_cut    = barrel ? DBL_MAX  : DBL_MAX;
      break;
    case 1:
      deta_cut        = barrel ? 0.007   : 0.009;
      dphi_cut        = barrel ? 0.15    : 0.10;
      ieta_cut        = barrel ? 0.01    : 0.03;
      hovere_cut      = barrel ? 0.12    : 0.10;
      d0_cut          = barrel ? 0.02    : 0.02;
      dz_cut          = barrel ? 0.2     : 0.2;
      ooeminusoop_cut = barrel ? 0.05    : 0.05;
      vprob_cut       = barrel ? 1.e-6   : 1.e-6;
      misshits_cut    = barrel ? 1       : 1;
      break;
    case 2:
      deta_cut        = barrel ? 0.004   : 0.007;
      dphi_cut        = barrel ? 0.06    : 0.03;
      ieta_cut        = barrel ? 0.01    : 0.03;
      hovere_cut      = barrel ? 0.12    : 0.1;
      d0_cut          = barrel ? 0.02    : 0.02;
      dz_cut          = barrel ? 0.1     : 0.1;
      ooeminusoop_cut = barrel ? 0.05    : 0.05;
      vprob_cut       = barrel ? 1.e-6   : 1.e-6;
      misshits_cut    = barrel ? 1       : 1;
      break;
    case 3:
      deta_cut        = barrel ? 0.004   : 0.005;
      dphi_cut        = barrel ? 0.03    : 0.02;
      ieta_cut        = barrel ? 0.01    : 0.03;
      hovere_cut      = barrel ? 0.12    : 0.10;
      d0_cut          = barrel ? 0.02    : 0.02;
      dz_cut          = barrel ? 0.1     : 0.1;
      ooeminusoop_cut = barrel ? 0.05    : 0.05;
      vprob_cut       = barrel ? 1.e-6   : 1.e-6;
      misshits_cut    = barrel ? 1       : 0;
      break;
    }
  }else{
    //See https://twiki.cern.ch/twiki/bin/viewauth/CMS/CutBasedElectronIdentificationRun2#Working_points_for_PHYS14_sample
    switch(level){
    default:
    case 0:
      deta_cut        = barrel ? 0.016315    : 0.010671;
      dphi_cut        = barrel ? 0.252044  : 0.245263;
      ieta_cut        = barrel ? 0.011100  :  0.033987;
      hovere_cut      = barrel ? 0.345843  : 0.134691;
      d0_cut          = barrel ? 0.060279   : 0.273097;
      dz_cut          = barrel ? 0.800538  : 0.885860;
      ooeminusoop_cut = barrel ? 0.248070  : 0.157160;
      vprob_cut       = barrel ? 1.e-6   : 1.e-6; // now just using pass converison veto
      misshits_cut    = barrel ? 2       : 3;
      break;
    case 1:
      deta_cut        = barrel ? 0.0181  : 0.0124;
      dphi_cut        = barrel ? 0.0936  : 0.0624;
      ieta_cut        = barrel ? 0.0123  : 0.035;
      hovere_cut      = barrel ? 0.141   : 0.1115;
      d0_cut          = barrel ? 0.0166  : 0.098;
      dz_cut          = barrel ? 0.54342 : 0.9187;
      ooeminusoop_cut = barrel ? 0.1353  : 0.1443;
      vprob_cut       = barrel ? 1.e-6   : 1.e-6;
      misshits_cut    = barrel ? 1       : 1;
      break;
    case 2:
      deta_cut        = barrel ? 0.0106  : 0.0108;
      dphi_cut        = barrel ? 0.0323  : 0.0455;
      ieta_cut        = barrel ? 0.0107  : 0.0318;
      hovere_cut      = barrel ? 0.067   : 0.097;
      d0_cut          = barrel ? 0.0131  : 0.0845;
      dz_cut          = barrel ? 0.22310 : 0.7523;
      ooeminusoop_cut = barrel ? 0.1043  : 0.1201;
      vprob_cut       = barrel ? 1.e-6   : 1.e-6;
      misshits_cut    = barrel ? 1       : 1;
      break;
    case 3:
      deta_cut        = barrel ? 0.0091  : 0.0106;
      dphi_cut        = barrel ? 0.031   : 0.0359;
      ieta_cut        = barrel ? 0.0106  : 0.0305;
      hovere_cut      = barrel ? 0.0532  : 0.0835;
      d0_cut          = barrel ? 0.0126  : 0.0163;
      dz_cut          = barrel ? 0.0116  : 0.5999;
      ooeminusoop_cut = barrel ? 0.0609  : 0.1126;
      vprob_cut       = barrel ? 1.e-6   : 1.e-6;
      misshits_cut    = barrel ? 1       : 1;
      break;
    }
  }

  const double d0 = GetElectronD0(iel);
  const double dz = fabs(els_vz->at(iel)-pv_z->at(0));
  const double sigietaieta = (cfAVersion>=75
                              ? els_full5x5_sigmaIetaIeta->at(iel)
                              : els_sigmaIEtaIEta->at(iel));

  return deta_cut > fabs(els_dEtaIn->at(iel))
    && dphi_cut > fabs(els_dPhiIn->at(iel))
    && ieta_cut > sigietaieta
    && hovere_cut > els_hadOverEm->at(iel)
    && d0_cut > fabs(d0)
    && dz_cut > fabs(dz)
    && ooeminusoop_cut > fabs((1.0-els_eOverPIn->at(iel))/els_caloEnergy->at(iel))
    // && (true || vprob_cut)
    && (els_PATpassConversionVeto->at(iel))
    && (cfAVersion>=77 && misshits_cut >= els_expectedMissingInnerHits->at(iel));
  
}

bool EventHandler::isRecoElectron(const uint iel, const uint level, const bool check_pt, const bool check_iso, const bool check_id, const bool use_mini_iso, const double mini_iso_cut) const{

  if(iel>=els_pt->size()) return false;
  bool barrel;
  if(fabs(els_scEta->at(iel))<=1.479){
    barrel = true;
  }else if(fabs(els_scEta->at(iel))<2.5){
    barrel = false;
  }else{
    return false;
  }

  if (check_id&&!PassElectronID(iel, level)) return false;

  double pt_cut, reliso_cut;

  if(cfAVersion<75){
    //See https://twiki.cern.ch/twiki/bin/viewauth/CMS/EgammaCutBasedIdentification#Barrel_Cuts_eta_supercluster_1_4
    const bool high_pt = els_pt->at(iel)>20.0;
    switch(level){
    default:
    case 0:
      pt_cut            = 10.;
      reliso_cut      = barrel ? 0.15    : 0.15;
      break;
    case 1:
      pt_cut            = 20.;
      reliso_cut      = barrel ? 0.15    : (high_pt ? 0.15 : 0.10);
      break;
    case 2:
      pt_cut            = 20.;
      reliso_cut      = barrel ? 0.15    : (high_pt ? 0.15 : 0.10);
      break;
    case 3:
      pt_cut            = 20.;
      reliso_cut      = barrel ? 0.10    : (high_pt ? 0.10 : 0.07);
      break;
    }
  }else{
    //See https://twiki.cern.ch/twiki/bin/viewauth/CMS/CutBasedElectronIdentificationRun2#Working_points_for_PHYS14_sample
    switch(level){
    default:
    case 0:
      pt_cut            = 10.;
      reliso_cut      = barrel ? 0.164369   :  0.212604 ;
      break;
    case 1:
      pt_cut            = 20.;
      reliso_cut      = barrel ?  0.120026    : 0.162914 ;
      break;
    case 2:
      pt_cut            = 20.;
      reliso_cut      = barrel ?  0.097213  :  0.116708 ;
      break;
    case 3:
      pt_cut            = 20.;
      reliso_cut      = barrel ? 0.074355  : 0.090185 ;
      break;
    }
  }
  if (els_pt->at(iel)<5.) return false; // always cut here at least
  if (check_pt && els_pt->at(iel)<pt_cut) return false;
  if(check_iso) {  
    if (use_mini_iso) {
      double mini_iso(999.);
      if (cfAVersion>=78) mini_iso = els_miniso->at(iel);
      else mini_iso = GetIsolation(iel, 11);
      if (mini_iso>mini_iso_cut) return false;
    } else {
      if (GetCSAElectronIsolation(iel)>reliso_cut) return false;
    }
  }
  return true;
}

double EventHandler::GetElectronD0(const unsigned int iel) const{
  return els_d0dum->at(iel) - pv_x->at(0)*sin(els_tk_phi->at(iel)) + pv_y->at(0)*cos(els_tk_phi->at(iel));
}

double EventHandler::GetElectronRelIso(const unsigned int k) const{
  const double rho(rho_kt6PFJetsForIsolation2012);
  // get effective area from delR=0.3 2011 data table for neutral+gamma based on supercluster eta els_scEta->at(k)
  double AE(0.10); 
  const double abseta(fabs(els_scEta->at(k)));
  if      ( abseta < 1.0 ) AE = 0.13;
  else if ( abseta >=1.0 && abseta <1.479) AE = 0.14;
  else if ( abseta >=1.479&&abseta <2.0)   AE = 0.07;
  else if ( abseta >=2.0 && abseta <2.2) AE = 0.09;
  else if ( abseta >=2.2 && abseta <2.3) AE = 0.11;
  else if ( abseta >=2.3 && abseta <2.4) AE = 0.11;
  else if ( abseta >=2.4 ) AE = 0.14;

  const double eleIso(els_PFphotonIsoR03->at(k) + els_PFneutralHadronIsoR03->at(k) - rho*AE);
  return ( els_PFchargedHadronIsoR03->at(k) + ( eleIso > 0 ? eleIso : 0.0 ) )/els_pt->at(k);
}

float EventHandler::GetCSAElectronIsolation(const uint iel) const{
  float absiso = els_pfIsolationR03_sumChargedHadronPt->at(iel) + std::max(0.0 , els_pfIsolationR03_sumNeutralHadronEt->at(iel) + els_pfIsolationR03_sumPhotonEt->at(iel) - 0.5 * els_pfIsolationR03_sumPUPt->at(iel) );
  return absiso/els_pt->at(iel);
}

bool EventHandler::passedCSABaseElectronSelection(uint iel, float ElectronPTThreshold){
  if(iel >= els_pt->size()) return false;

  float d0PV = els_d0dum->at(iel)-pv_x->at(0)*sin(els_tk_phi->at(iel))+pv_y->at(0)*cos(els_tk_phi->at(iel));
  
  return (els_pt->at(iel) > ElectronPTThreshold
	  && fabs(els_scEta->at(iel)) < 2.5
	  && !els_hasMatchedConversion->at(iel)
	  && (sampleName.find("RelVal")!=std::string::npos || els_n_inner_layer->at(iel) <= 1)
	  && fabs(getDZ(els_vx->at(iel), els_vy->at(iel), els_vz->at(iel), cos(els_tk_phi->at(iel))*els_tk_pt->at(iel), 
			sin(els_tk_phi->at(iel))*els_tk_pt->at(iel), els_tk_pz->at(iel), 0)) < 0.1
	  && fabs(1./els_caloEnergy->at(iel) - els_eOverPIn->at(iel)/els_caloEnergy->at(iel)) < 0.05 
	  && els_isPF->at(iel) 
	  && fabs(d0PV) < 0.02 
	  && ((fabs(els_scEta->at(iel))<=1.479 // Endcap selection
	       && fabs(els_dEtaIn->at(iel)) < 0.004
	       && fabs(els_dPhiIn->at(iel)) < 0.06
	       && els_sigmaIEtaIEta->at(iel) < 0.01
	       && els_hadOverEm->at(iel) < 0.12 ) ||
	      (fabs(els_scEta->at(iel))>1.479&&fabs(els_scEta->at(iel))<2.5  // Barrel selection
	       && fabs(els_dEtaIn->at(iel)) < 0.007
	       && fabs(els_dPhiIn->at(iel)) < 0.03
	       && els_sigmaIEtaIEta->at(iel) < 0.03
	       && els_hadOverEm->at(iel) < 0.10 ))
	  );
}

bool EventHandler::passedBaseElectronSelection(uint iel, float ElectronPTThreshold){
  if(iel >= els_pt->size()) return false;

  float d0PV = els_d0dum->at(iel)-pv_x->at(0)*sin(els_tk_phi->at(iel))+pv_y->at(0)*cos(els_tk_phi->at(iel));
  
  return (els_pt->at(iel) > ElectronPTThreshold
	  && fabs(els_scEta->at(iel)) < 2.5
	  && !els_hasMatchedConversion->at(iel)
	  && (sampleName.find("RelVal")!=std::string::npos || els_n_inner_layer->at(iel) <= 1)
	  && fabs(getDZ(els_vx->at(iel), els_vy->at(iel), els_vz->at(iel), cos(els_tk_phi->at(iel))*els_tk_pt->at(iel), 
			sin(els_tk_phi->at(iel))*els_tk_pt->at(iel), els_tk_pz->at(iel), 0)) < 0.1
	  && fabs(1./els_caloEnergy->at(iel) - els_eOverPIn->at(iel)/els_caloEnergy->at(iel)) < 0.05 
	  && els_isPF->at(iel) 
	  && fabs(d0PV) < 0.02 
	  && ((els_isEB->at(iel) // Endcap selection
	       && fabs(els_dEtaIn->at(iel)) < 0.004
	       && fabs(els_dPhiIn->at(iel)) < 0.06
	       && els_sigmaIEtaIEta->at(iel) < 0.01
	       && els_hadOverEm->at(iel) < 0.12 ) ||
	      (els_isEE->at(iel)  // Barrel selection
	       && fabs(els_dEtaIn->at(iel)) < 0.007
	       && fabs(els_dPhiIn->at(iel)) < 0.03
	       && els_sigmaIEtaIEta->at(iel) < 0.03
	       && els_hadOverEm->at(iel) < 0.10 ))
	  );
}

TLorentzVector EventHandler::GetNearestJet(const TLorentzVector lepton, const uint jet_ind) const {
  double minDR(DBL_MAX);
  TLorentzVector TLV(0,0,0,0), TLV_out(0,0,0,0);
  // printf("lepton: px=%3.2f\tpy=%3.2f\tpz=%3.2f\tenergy=%3.2f\n",lepton.Px(),lepton.Py(),lepton.Pz(),lepton.E());
  for(unsigned int ijet(0); ijet<corrJets.size(); ++ijet){
    if(corrJets[ijet].GetTLV(theJESType_).Pt()<30.) continue;
    TLV=corrJets[ijet].GetTLV(theJESType_);
    // printf("jet: px=%3.2f\tpy=%3.2f\tpz=%3.2f\tenergy=%3.2f\n",TLV.Px(),TLV.Py(),TLV.Pz(),TLV.E());
    if (jet_ind==ijet) {
      TLV-=lepton;
      // printf("jet(corr): px=%3.2f\tpy=%3.2f\tpz=%3.2f\tenergy=%3.2f\n",TLV.Px(),TLV.Py(),TLV.Pz(),TLV.E());
    }
    double DR=Math::GetDeltaR(TVector2::Phi_mpi_pi(lepton.Phi()), lepton.Eta(), TVector2::Phi_mpi_pi(TLV.Phi()), TLV.Eta());
    //  cout << "DR: " << DR << endl;
    if (DR<minDR) {
      minDR=DR;
      //	cout << "minDR: " << minDR << endl;
      TLV_out=TLV;
    }
  }

  return TLV_out;
}

bool EventHandler::IsMC(){
  return (sampleName.find("Run201") == std::string::npos);
}

double EventHandler::GetFatJetPt(const unsigned int index/*, const unsigned int pt_cut*/) const{
  GetSortedFatJets();
  if (index+1>sortedFatJetCache.size()) return -1;
  //  cout << "GetFatJetPt()" << endl;
  return sortedFatJetCache[index].GetTLV().Pt();
}

double EventHandler::GetFatJetEta(const unsigned int index/*, const unsigned int pt_cut*/) const{
  GetSortedFatJets();
  if (index+1>sortedFatJetCache.size()) return -999;
  //  cout << "GetFatJetEta()" << endl;
  return sortedFatJetCache[index].GetTLV().Eta();
}

double EventHandler::GetFatJetPhi(const unsigned int index/*, const unsigned int pt_cut*/) const{
  GetSortedFatJets();
  if (index+1>sortedFatJetCache.size()) return -999;
  return TVector2::Phi_mpi_pi(sortedFatJetCache[index].GetTLV().Phi());
}

double EventHandler::GetFatJetEnergy(const unsigned int index/*, const unsigned int pt_cut*/) const{
  GetSortedFatJets();
  if (index+1>sortedFatJetCache.size()) return -1;
  return sortedFatJetCache[index].GetTLV().E();
}

int EventHandler::GetFatJetnConst(const unsigned int index/*, const unsigned int pt_cut*/) const{
  GetSortedFatJets();
  if (index+1>sortedFatJetCache.size()) return -1;
  return sortedFatJetCache[index].GetNConst();
}

double EventHandler::GetFatJetmJ(const unsigned int index/*, const unsigned int pt_cut*/) const{
  GetSortedFatJets();
  if (index+1>sortedFatJetCache.size()) return -1;
  return sortedFatJetCache[index].GetTLV().M();
}

int EventHandler::GetNFatJets(const double fat_jet_pt_cut, const double fat_jet_eta_cut/* , const unsigned int skinny_jet_pt_cut*/) const{
  GetSortedFatJets();
  uint nJ(0);
  for (uint ifj(0); ifj<sortedFatJetCache.size(); ifj++) {
    if (sortedFatJetCache[ifj].GetTLV().Pt()>fat_jet_pt_cut&&fabs(sortedFatJetCache[ifj].GetTLV().Eta())<fat_jet_eta_cut) nJ++;
  }
  return nJ;
}

double EventHandler::GetMJ(const double fat_jet_pt_cut, const double fat_jet_eta_cut/* , const unsigned int skinny_jet_pt_cut*/) const{
  GetSortedFatJets();
  double MJ(0.);
  for (uint ifj(0); ifj<sortedFatJetCache.size(); ifj++) {
    //  cout << "Fat jet " << ifj << ": mass = " << sortedFatJetCache[ifj].GetTLV().M() << endl;
    if (sortedFatJetCache[ifj].GetTLV().Pt()>fat_jet_pt_cut&&fabs(sortedFatJetCache[ifj].GetTLV().Eta())<fat_jet_eta_cut)
      MJ+=sortedFatJetCache[ifj].GetTLV().M();
  }
  return MJ;
}

double EventHandler::GetHT(const double pt_cut, const double min_eta) const{
	if (!JetsUpToDate) GetSortedJets();
	double HT(0.0);
	for(unsigned int ijet(0); ijet<corrJets.size(); ++ijet){
	  if (min_eta<-10.) { if(isGoodJet(ijet,true,pt_cut,2.4)) HT+=corrJets[ijet].GetPt(theJESType_); }
	  else if(isGoodJet(ijet,true,pt_cut,5) && corrJets[ijet].GetTLV(theJESType_).Eta()>min_eta) HT+=corrJets[ijet].GetPt(theJESType_);
	}
	return HT;
}

double EventHandler::GetSumP(const double pt_cut) const{
  TLorentzVector TLV_out;
  for(unsigned int i(0); i<corrJets.size(); ++i){
    if(isGoodJet(i,true,pt_cut)) {
      TLV_out+=corrJets[i].GetTLV(theJESType_);
    }
  }
  return TLV_out.P();
}

double EventHandler::GetCentrality(const double pt_cut) const{
  return GetHT(pt_cut)/GetSumP(pt_cut);
}

unsigned int EventHandler::GetNumGoodJets(const double pt_cut) const{
	//cout << "GetNumGoodJets" << endl;
	if(!JetsUpToDate) GetSortedJets();
	uint numGoodJets(0);
	for(unsigned int ijet(0); ijet<corrJets.size(); ++ijet){
		if(isGoodJet(ijet,true,pt_cut,2.4)) ++numGoodJets;
	}
	//cout << "Found " << numGoodJets << " good jets" << endl;
	return numGoodJets;
}

unsigned int EventHandler::GetNumGoodJets_Old(const double pt) const{
	//cout << "GetNumGoodJets_Old" << endl;
	int numGoodJets(0);
	for(unsigned int i(0); i<jets_AKPF_pt->size(); ++i){
		if(isGoodJet_Old(i,true,pt)) ++numGoodJets;
	}
	//cout << "Found " << numGoodJets << " good jets" << endl;
	return numGoodJets;
}

std::vector<int> EventHandler::GetJets(const bool checkID, const double pt, const double eta) const{
  std::vector<int> jets;
  for(unsigned int i(0); i<corrJets.size(); ++i){
    if(isGoodJet(i,checkID,pt,eta)) jets.push_back(i);
  }
  return jets;
}

int EventHandler::GetClosestGenJet(const uint ijet, const double drmax) const {
  if (cfAVersion<75) return -1;
  double rjet_eta(corrJets[ijet].GetTLV(theJESType_).Eta()), rjet_phi(TVector2::Phi_mpi_pi(corrJets[ijet].GetTLV(theJESType_).Phi()));
  int closest_gen_jet = -1;
  double minDR(DBL_MAX);
    for(unsigned int i(0); i<mc_jets_pt->size(); ++i){
      double dR = Math::dR(mc_jets_eta->at(i), rjet_eta, mc_jets_phi->at(i), rjet_phi);
      if (dR<minDR) {
	minDR=dR;
	closest_gen_jet=i;
      }
    }
    if (minDR<drmax) return closest_gen_jet;
    else return -1;
}

int EventHandler::GetClosestRecoJet(const uint ijet, const bool use_dR) const {
  double rjet_eta(corrJets[ijet].GetTLV(theJESType_).Eta()), rjet_phi(TVector2::Phi_mpi_pi(corrJets[ijet].GetTLV(theJESType_).Phi()));
  int closest_reco_jet = -1;
  double minDR(DBL_MAX);
    for(unsigned int i(0); i<corrJets.size(); ++i){
      if (i==ijet) continue;
      double dR(DBL_MAX);
      if (use_dR) dR = Math::dR(corrJets[i].GetTLV(theJESType_).Eta(), rjet_eta, TVector2::Phi_mpi_pi(corrJets[i].GetTLV(theJESType_).Phi()), TVector2::Phi_mpi_pi(rjet_phi));
      else dR = fabs(Math::GetDeltaPhi(TVector2::Phi_mpi_pi(corrJets[i].GetTLV(theJESType_).Phi()), TVector2::Phi_mpi_pi(rjet_phi))); // sort by delta phi instead of delta R
      if (dR<minDR) {
	minDR=dR;
	closest_reco_jet=i;
      }
    }
    return closest_reco_jet;
}

double EventHandler::GetGenJetPt(const uint ijet, const double drmax) const {
  int igjet = GetClosestGenJet(ijet, drmax);
  if (igjet>=0) return mc_jets_pt->at(igjet);
  else return -999.;
}

bool EventHandler::isBLepJet(const uint ijet) const {
  int parton_id = TMath::Nint(jets_AKPF_partonFlavour->at(corrJets[ijet].GetIndex()));
  int parton_mom_id = TMath::Nint(jets_AKPF_parton_motherId->at(corrJets[ijet].GetIndex()));
  // cout << "Jet: " << ijet << ", parton_id: " << parton_id << ", parton_mom_id: " << parton_mom_id << endl;
  if (abs(parton_id)!=5 || abs(parton_mom_id)!=6) return false;
  for (uint imc(0); imc<mc_doc_id->size(); imc++) {
    int status = TMath::Nint(mc_doc_status->at(imc));
    int mcid = TMath::Nint(mc_doc_id->at(imc));
    // int mcmomid = TMath::Nint(mc_doc_mother_id->at(imc));
    // int mcgmomid = TMath::Nint(mc_doc_grandmother_id->at(imc));
    // int mcggmomid = TMath::Nint(mc_doc_ggrandmother_id->at(imc));
    if (!(status==3||status==22||status==23)) continue;
    if (!(abs(mcid)==11||abs(mcid)==13||abs(mcid)==15)) continue;
    // cout<<imc<<": ID "<<mcid<<", \tMom ID "<< mcmomid <<", \tGMom ID "<< mcgmomid <<", \tGGMom ID "<< mcggmomid
    //   <<", \tN mothers "<<mc_doc_numOfMothers->at(imc)
    //   <<", \tN daughters "<<mc_doc_numOfDaughters->at(imc)
    //   <<",   \tpx "<<mc_doc_px->at(imc)
    //   <<",   \tpy "<<mc_doc_py->at(imc)
    //   <<",   \tpz "<<mc_doc_pz->at(imc)
    //   <<", \tstatus "<< TMath::Nint(mc_doc_status->at(imc)) <<endl;
    if (mcid*parton_mom_id < 0) { // because l+ is - and t+ is +
      //  cout << "Matched to blep!." << endl;
      return true;
    }
  }
  return false;
}


unsigned int EventHandler::GetNumTruthMatchedBJets(const double pt, const bool good) const{
  int numBJets(0);
  for(unsigned int i(0); i<corrJets.size(); ++i){
    if(good&&!isGoodJet(i,true,pt)) continue;
    if (abs(TMath::Nint(jets_AKPF_partonFlavour->at(corrJets[i].GetIndex())))==5) numBJets++;
  }
  return numBJets;
}

unsigned int EventHandler::GetNumIncCSVTJets(const double pt_cut) const{
  int numPassing(0);
  for(unsigned int ijet(0); ijet<corrJets.size(); ++ijet){
    if(isGoodJet(ijet, true, pt_cut, 2.4) && corrJets[ijet].GetBTag()>ICSVTCut){
      ++numPassing;
    }
  }
  return numPassing;
}

unsigned int EventHandler::GetNumIncCSVMJets(const double pt_cut) const{
  int numPassing(0);
  for(unsigned int ijet(0); ijet<corrJets.size(); ++ijet){
    if(isGoodJet(ijet, true, pt_cut, 2.4) && corrJets[ijet].GetBTag()>ICSVMCut){
      ++numPassing;
    }
  }
  return numPassing;
}

unsigned int EventHandler::GetNumIncCSVLJets(const double pt_cut) const{
  int numPassing(0);
  for(unsigned int ijet(0); ijet<corrJets.size(); ++ijet){
    if(isGoodJet(ijet, true, pt_cut, 2.4) && corrJets[ijet].GetBTag()>ICSVLCut){
      ++numPassing;
    }
  }
  return numPassing;
}

unsigned int EventHandler::GetNumCSVTJets(const double pt_cut) const{
  int numPassing(0);
  for(unsigned int ijet(0); ijet<corrJets.size(); ++ijet){
    if(isGoodJet(ijet, true, pt_cut, 2.4) && corrJets[ijet].GetBTag()>CSVTCut){
      ++numPassing;
    }
  }
  return numPassing;
}

unsigned int EventHandler::GetNumCSVMJets(const double pt_cut) const{
  int numPassing(0);
  for(unsigned int ijet(0); ijet<corrJets.size(); ++ijet){
    if(isGoodJet(ijet, true, pt_cut, 2.4) && corrJets[ijet].GetBTag()>CSVMCut){
      ++numPassing;
    }
  }
  return numPassing;
}

unsigned int EventHandler::GetNumCSVLJets(const double pt_cut) const{
  int numPassing(0);
  for(unsigned int ijet(0); ijet<corrJets.size(); ++ijet){
    if(isGoodJet(ijet, true, pt_cut, 2.4) && corrJets[ijet].GetBTag()>CSVLCut){
      ++numPassing;
    }
  }
  return numPassing;
}

double EventHandler::GetMTW(const double lep_pt, const double MET, const double lep_phi, const double MET_phi) const{
  return TMath::Sqrt(2*lep_pt*MET*(1-cos(Math::GetDeltaPhi(lep_phi,MET_phi))));
}

double EventHandler::GetMTWb(const double b_pt, const double MET, const double b_phi, const double MET_phi, const bool use_W_mass) const{
  double W_mass=80.385;
  if (use_W_mass) return TMath::Sqrt(W_mass*W_mass+2*b_pt*MET*(TMath::Sqrt(1+(W_mass/MET)*(W_mass/MET))-cos(Math::GetDeltaPhi(b_phi,MET_phi)))); 
  else return TMath::Sqrt(2*b_pt*MET*(1-cos(Math::GetDeltaPhi(b_phi,MET_phi))));
}

double EventHandler::GetMinMTWb(const double pt, const double bTag, const bool use_W_mass) const{
  GetSortedJets();
  double min_mT(DBL_MAX);
  for (uint jet(0); jet<corrJets.size(); jet++) {
    if (corrJets[jet].GetTLV(theJESType_).Pt()<pt) continue;
    if (corrJets[jet].GetBTag()<bTag) continue;
    double mT=GetMTWb(corrJets[jet].GetTLV(theJESType_).Pt(), theMET_, TVector2::Phi_mpi_pi(corrJets[jet].GetTLV(theJESType_).Phi()), TVector2::Phi_mpi_pi(theMETPhi_), use_W_mass);
    if (mT<min_mT) min_mT=mT;
  }
  if (min_mT==DBL_MAX) return -999.;
  else return min_mT;
}

double EventHandler::GetMinDeltaPhibMET(const double pt, const double bTag) const{
  GetSortedJets();
  double min_dPhi(DBL_MAX);
  for (uint jet(0); jet<corrJets.size(); jet++) {
    if (corrJets[jet].GetTLV(theJESType_).Pt()<pt) continue;
    if (corrJets[jet].GetBTag()<bTag) continue;
    double dPhi=Math::GetDeltaPhi(TVector2::Phi_mpi_pi(corrJets[jet].GetTLV(theJESType_).Phi()), theMETPhi_);
    if (dPhi<min_dPhi) min_dPhi=dPhi;
  }
  if (min_dPhi==DBL_MAX) return -999.;
  else return min_dPhi;
}


double EventHandler::Get2ndMTWb(const double pt, const double bTag, const bool use_W_mass) const{
  GetSortedJets();
  double max_mT(-1.);
  for (uint jet(0); jet<corrJets.size(); jet++) {
    if (corrJets[jet].GetTLV(theJESType_).Pt()<pt) continue;
    if (corrJets[jet].GetBTag()<bTag) continue;
    double mT=GetMTWb(corrJets[jet].GetTLV(theJESType_).Pt(), theMET_, TVector2::Phi_mpi_pi(corrJets[jet].GetTLV(theJESType_).Phi()), theMETPhi_, use_W_mass);
    if (mT>max_mT) max_mT=mT;
  }
  // now find 2nd
  double max2_mT(-1.); 
  for (uint jet(0); jet<corrJets.size(); jet++) {
    if (corrJets[jet].GetTLV(theJESType_).Pt()<pt) continue;
    if (corrJets[jet].GetBTag()<bTag) continue;
    double mT=GetMTWb(corrJets[jet].GetTLV(theJESType_).Pt(), theMET_, TVector2::Phi_mpi_pi(corrJets[jet].GetTLV(theJESType_).Phi()), theMETPhi_, use_W_mass);
    if (mT==max_mT) continue;
    if (mT>max2_mT) max2_mT=mT;
  }
  return max2_mT;
}

double EventHandler::getDeltaPhiMETN(int goodJetI, float otherpt, float othereta, bool otherid, bool useArcsin, bool use_mht) {
  // cout << "Compute dpN..." << goodJetI << endl;
  double met(0.), met_phi(0.);
  if (use_mht) {
    TVector2 mht_vec = GetMHTVec(theJESType_);
    met=mht_vec.Mod();
    met_phi=TVector2::Phi_mpi_pi(mht_vec.Phi());
  } else {
    met = theMET_;
    met_phi = theMETPhi_;
  }
  
  if (goodJetI>static_cast<int>(corrJets.size())||goodJetI<0) return DBL_MAX;
  double deltaT = getDeltaPhiMETN_deltaT(goodJetI, otherpt, othereta, otherid);
  //calculate deltaPhiMETN
  double dp = fabs(Math::GetAbsDeltaPhi(corrJets[goodJetI].GetTLV(theJESType_).Phi(), met_phi));
  double dpN;
  if(useArcsin) {
    if( deltaT/met >= 1.0) dpN = dp / (TMath::Pi()/2.0);
    else dpN = dp / asin(deltaT/met);
  }
  else dpN = dp / atan2(deltaT, met);
  //printf("Jet %d: pt=%3.2f, dp=%3.2f, dpN=%3.2f, deltaT=%3.2f\n",goodJetI,corrJets[goodJetI].GetTLV(theJESType_).Pt(),dp,dpN,deltaT);
  return dpN;
}

double EventHandler::getDeltaPhiMETN_deltaT(unsigned int goodJetI, float otherpt, float othereta, bool otherid) {
  // cout << "Compute deltaT..." << endl;
  if(goodJetI==999999) return -99;
  TLorentzVector tlv_I = corrJets[goodJetI].GetTLV(theJESType_);
  //get sum for deltaT
  double sum = 0;
  for (unsigned int i=0; i< corrJets.size(); i++) {
    if(i==goodJetI) continue;
    if(isGoodJet(i, otherid, otherpt, othereta)){
      double jetres = 0.1;
      TLorentzVector tlv_i = corrJets[i].GetTLV(theJESType_);
      // sum += pow( jetres*(jets_AKPF_px->at(goodJetI)*jets_AKPF_py->at(i) - jets_AKPF_py->at(goodJetI)*jets_AKPF_px->at(i)), 2);
      sum += pow( jetres*(tlv_I.Px()*tlv_i.Py() - tlv_I.Py()*tlv_i.Px()), 2);
       // printf("Jet %d, px=%3.2f, py=%3.2f, cont=%3.2f\n",i, tlv_i.Px(), tlv_i.Py(), pow( jetres*(tlv_I.Px()*tlv_i.Py() - tlv_I.Py()*tlv_i.Px()), 2));
    }//is good jet
  }//i
  double deltaT = sqrt(sum)/tlv_I.Pt();
  return deltaT;
}

double EventHandler::getMinDeltaPhiMETN(unsigned int maxjets, float mainpt, float maineta, bool mainid,
					float otherpt, float othereta, bool otherid, bool useArcsin, bool use_mht, double csv) 
{
  if(!JetsUpToDate) GetSortedJets();
  double mdpN=1E12;
  unsigned int nGoodJets(0);
  for (unsigned int i=0; i<corrJets.size(); i++) {
    if (!isGoodJet(i, mainid, mainpt, maineta)) continue;
    if (csv>0&&corrJets[i].GetBTag()<csv) continue; // find min among b-jets (not enabled by default) 
    nGoodJets++;
    double dpN = getDeltaPhiMETN(i, otherpt, othereta, otherid, useArcsin, use_mht);
    //i is for i'th *good* jet, starting at i=0. returns -99 if bad jet--but then i still increases by one
    // Jack --  might have fixed things above...
    if (dpN>=0&&dpN<mdpN) mdpN=dpN;
    if (nGoodJets>=maxjets) break;
  }
  // printf("mdpN=%3.2f\n",mdpN);
  return mdpN;
}

double EventHandler::getDeltaPhiMETN_Old(int goodJetI, float otherpt, float othereta, bool otherid, bool useArcsin) {
  // cout << "Compute dpN..." << goodJetI << endl;
  if (goodJetI>static_cast<int>(jets_AKPF_phi->size())||goodJetI<0) return DBL_MAX;
  double deltaT = getDeltaPhiMETN_deltaT_Old(goodJetI, otherpt, othereta, otherid);
  //calculate deltaPhiMETN
  double dp = fabs(Math::GetAbsDeltaPhi(jets_AKPF_phi->at(goodJetI), theMETPhi_));
  double dpN;
  if(useArcsin) {
    if( deltaT/theMET_ >= 1.0) dpN = dp / (TMath::Pi()/2.0);
    else dpN = dp / asin(deltaT/theMET_);
  }
  else dpN = dp / atan2(deltaT, theMET_);
  // printf("Jet %d: pt=%3.2f, dp=%3.2f, dpN=%3.2f, deltaT=%3.2f\n",goodJetI,jets_AKPF_pt->at(goodJetI),dp,dpN,deltaT);
  return dpN;
}
double EventHandler::getDeltaPhiMETN_deltaT_Old(unsigned int goodJetI, float otherpt, float othereta, bool otherid) {
  // cout << "Compute deltaT..." << endl;
  if(goodJetI==999999) return -99;
  //get sum for deltaT
  double sum = 0;
  for (unsigned int i=0; i< jets_AKPF_pt->size(); i++) {
    if(i==goodJetI) continue;
    if(isGoodJet_Old(i, otherid, otherpt, othereta)){
      double jetres = 0.1;
      sum += pow( jetres*(jets_AKPF_px->at(goodJetI)*jets_AKPF_py->at(i) - jets_AKPF_py->at(goodJetI)*jets_AKPF_px->at(i)), 2);
       // printf("Jet %d, px=%3.2f, py=%3.2f, cont=%3.2f\n",i, jets_AKPF_px->at(i), jets_AKPF_py->at(i), pow( jetres*(jets_AKPF_px->at(goodJetI)*jets_AKPF_py->at(i) - jets_AKPF_py->at(goodJetI)*jets_AKPF_px->at(i)), 2));
    }//is good jet
  }//i
  double deltaT = sqrt(sum)/jets_AKPF_pt->at(goodJetI);
  return deltaT;
}
double EventHandler::getMinDeltaPhiMETN_Old(unsigned int maxjets, float mainpt, float maineta, bool mainid,
					    float otherpt, float othereta, bool otherid, bool useArcsin)
{
  double mdpN=1E12;
  unsigned int nGoodJets(0);
  for (unsigned int i=0; i<jets_AKPF_pt->size(); i++) {
    if (!isGoodJet_Old(i, mainid, mainpt, maineta)) continue;
    nGoodJets++;
    double dpN = getDeltaPhiMETN_Old(i, otherpt, othereta, otherid, useArcsin);
    //i is for i'th *good* jet, starting at i=0. returns -99 if bad jet--but then i still increases by one
    // Jack -- might have fixed things above...
    if (dpN>=0&&dpN<mdpN) mdpN=dpN;
    if (nGoodJets>=maxjets) break;
  }
  // printf("mdpN=%3.2f\n",mdpN);
  return mdpN;
}


double EventHandler::GetMinDeltaPhiMET(const unsigned int maxjets, const double pt_cut, const double eta_cut, const bool use_mht) const{

  double met_phi(0.);
  if (use_mht) {
    TVector2 mht_vec = GetMHTVec(theJESType_);
    met_phi=TVector2::Phi_mpi_pi(mht_vec.Phi());
  } else met_phi = theMETPhi_;

    
  std::vector<std::pair<double, double> > jets(0);
  for(unsigned int i(0); i<corrJets.size(); ++i){
    if(isGoodJet(i, true, pt_cut, eta_cut)){
      jets.push_back(std::make_pair(corrJets[i].GetTLV(theJESType_).Pt(), corrJets[i].GetTLV(theJESType_).Phi()));
    }
  }
  std::sort(jets.begin(), jets.end(), std::greater<std::pair<double, double> >());
  double mindp(DBL_MAX);
  for(unsigned int i(0); i<jets.size() && i<maxjets; ++i){
    const double thisdp(fabs((Math::GetAbsDeltaPhi(jets.at(i).second, met_phi))));
    if(thisdp<mindp){
      mindp=thisdp;
    }
  }
  return mindp;
}

int EventHandler::GetNumIsoTracks(const double ptThresh, const bool mT_cut) const{
  int nisotracks=0;
  for ( unsigned int itrack = 0 ; itrack < isotk_pt->size() ; ++itrack) {
    if ( (isotk_pt->at(itrack) >= ptThresh) &&
	 (isotk_iso->at(itrack) /isotk_pt->at(itrack) < 0.1 ) &&
	 ( fabs(isotk_dzpv->at(itrack)) <0.05) &&
	 ( !mT_cut || GetMTW(isotk_pt->at(itrack),theMET_,isotk_phi->at(itrack),theMETPhi_)<100 ) &&
	 ( fabs(isotk_eta->at(itrack)) < 2.4 ) //this is more of a sanity check
	 ){
      ++nisotracks;
    }
  }
  return nisotracks;
}

// std::vector<std::pair<int,double> > EventHandler::GetIsoTracks(const double ptThresh, const bool mT_cut) const{
//   std::vector<std::pair<int,double> > isotracks;
//   for ( unsigned int itrack = 0 ; itrack < isotk_pt->size() ; ++itrack) {
//     if ( (isotk_pt->at(itrack) >= ptThresh) &&
// 	 (isotk_iso->at(itrack) /isotk_pt->at(itrack) < 0.1 ) &&
// 	 ( fabs(isotk_dzpv->at(itrack)) <0.05) &&
// 	 ( !mT_cut || GetMTW(isotk_pt->at(itrack),theMET_,isotk_phi->at(itrack),theMETPhi_)<100 ) &&
// 	 ( fabs(isotk_eta->at(itrack)) < 2.4 ) //this is more of a sanity check
// 	 ){
//       isotracks.push_back(std::make_pair(itrack, isotk_iso->at(itrack) /isotk_pt->at(itrack)));
//     }
//   }
//   return isotracks;
// }

void EventHandler::GetIsoTracks(vector<IsolatedTrack> &muCands, vector<IsolatedTrack> &eCands,vector<IsolatedTrack> &hadCands, bool checkID, bool mT_cut) {
   if (cfAVersion<77) return;
   eCands.clear();
   muCands.clear();
   hadCands.clear();
   // cout << "Found " << pfcand_pt->size() << " PFCands." << endl;
   for (uint itk(0); itk<pfcand_pdgId->size(); itk++) {
     if (isnan(pfcand_charge->at(itk)) || isnan(pfcand_pt->at(itk))  || isnan(pfcand_dz->at(itk)) || isnan(pfcand_phi->at(itk)) || isnan(pfcand_eta->at(itk)) ) continue;
     if (static_cast<int>(pfcand_charge->at(itk))==0) continue;
     double pt = pfcand_pt->at(itk);
     if (pt<5) continue;
     int id = abs(TMath::Nint(pfcand_pdgId->at(itk)));
     if (id==211&&pt<10) continue;
     double eta = pfcand_eta->at(itk);
     if (fabs(eta)>2.5) continue;
     if (checkID&&!PassIsoTrackBaseline(itk)) continue;
     if (mT_cut && GetMTW(pfcand_pt->at(itk),theMET_,pfcand_phi->at(itk),theMETPhi_)>100) continue;
     double iso = GetPFCandIsolation(itk);
     double relIso=iso/pfcand_pt->at(itk);
     // note: not cutting here on isolation!
     TLorentzVector tlvIn;
     tlvIn.SetPtEtaPhiE(pfcand_pt->at(itk), pfcand_eta->at(itk), pfcand_phi->at(itk), pfcand_energy->at(itk));
     if (id==11) {
       eCands.push_back(IsolatedTrack(itk, id, tlvIn, relIso));
       for (unsigned int iel(0); iel<electrons_.size(); iel++) {
	 if ( TrackOverlaps(itk, els_pt->at(electrons_[iel]), els_scEta->at(electrons_[iel]), els_phi->at(electrons_[iel])) ) eCands.back().SetisLepton(true);
       }
     } else if(id==13) {
       muCands.push_back(IsolatedTrack(itk, id, tlvIn, relIso));
       for (unsigned int imu(0); imu<muons_.size(); imu++) {
	 if ( TrackOverlaps(itk, mus_pt->at(muons_[imu]), mus_eta->at(muons_[imu]), mus_phi->at(muons_[imu])) ) muCands.back().SetisLepton(true);
       }
     } else if(id==211) {
       hadCands.push_back(IsolatedTrack(itk, id, tlvIn, relIso));
     }
   }
   return;
 }


double EventHandler::GetPFCandIsolation(const uint indexA) const { // absolute, not relative -- charged tracks only
  double isoSum(0);
  for (uint other(0); other<pfcand_pdgId->size(); other++) {
    if (isnan(pfcand_charge->at(other)) || isnan(pfcand_pt->at(other))  || isnan(pfcand_fromPV->at(other)) || isnan(pfcand_phi->at(other)) || isnan(pfcand_eta->at(other)) ) continue;
    if (other==indexA) continue; // don't count track in it's own isolation sum
    if (TMath::Nint(pfcand_charge->at(other))==0) continue; // only consider charged tracks
    // if (fabs(pfcand_fromPV->at(other))<=1) continue; // pileup suppression
    if (fabs(pfcand_dz->at(other))>0.1) continue;
    double deltaR = Math::GetDeltaR(pfcand_phi->at(indexA), pfcand_eta->at(indexA), pfcand_phi->at(other), pfcand_eta->at(other)); // isolation cone
    if (deltaR>0.3) continue;
    isoSum+=pfcand_pt->at(other);
  }
  return isoSum;
}

double EventHandler::GetPFCandIsolationDeltaBetaCorr(const uint indexA) const { // absolute, not relative
  double iso_ph(0.), iso_nh(0.), iso_ch(0), iso_pu(0.);
  double ptThresh=0.5;
  double R=0.3;
  for (uint other(0); other<pfcand_pdgId->size(); other++) {
    if (isnan(pfcand_charge->at(other)) || isnan(pfcand_pt->at(other))  || isnan(pfcand_dz->at(other)) || isnan(pfcand_phi->at(other)) || isnan(pfcand_eta->at(other)) ) continue;
    if (other==indexA) continue; // don't count track in it's own isolation sum
    int pdgId = TMath::Nint(pfcand_pdgId->at(other));
      double dr = Math::GetDeltaR(pfcand_phi->at(indexA), pfcand_eta->at(indexA), pfcand_phi->at(other), pfcand_eta->at(other)); // isolation cone
      if (static_cast<int>(pfcand_charge->at(other))==0) { // neutrals
	/////////// PHOTONS ////////////
	if (pfcand_pt->at(other)>ptThresh && pdgId==22) {
	  if (dr<R) iso_ph += pfcand_pt->at(other);
	}
	/////////// NEUTRAL HADRONS ////////////
	else if (pfcand_pt->at(other)>ptThresh && pdgId==130) {
	  if (dr<R) iso_nh += pfcand_pt->at(other);
	}
	////////////////// CHARGED from PV /////////////////////////
      } else if (pfcand_fromPV->at(other)>1){
	if (pdgId==211 &&dr<R) iso_ch += pfcand_pt->at(other);
      }
      ////////////////// CHARGED from PU /////////////////////////
      else if (pfcand_pt->at(other)>ptThresh && dr<R) iso_pu += pfcand_pt->at(other);
  }
  double isolation(0);
  isolation += iso_ph;
  isolation += iso_nh;
  isolation -= iso_pu/2.;
  if(isolation < 0) isolation = 0;
  isolation += iso_ch;
  
  return isolation;
}

bool EventHandler::PassIsoTrackBaseline(const uint itk) const {
  //these already applied
  if (static_cast<int>(pfcand_charge->at(itk))==0) return false;
  // if (pfcand_pt->at(itk)<3) return false;
  if (fabs(pfcand_eta->at(itk))>2.5) return false;
  //if (pfcand_fromPV->at(itk)<=1) return false; // pileup suppression
  if (fabs(pfcand_dz->at(itk))>0.1) return false;
  if (static_cast<int>(pfcand_pdgId->at(itk))==0) return false; // just to be safe
  return true;
}

// void EventHandler::NewGetIsoTracks(std::vector<std::pair<int,double> > &eCands, std::vector<std::pair<int,double> > &muCands, std::vector<std::pair<int,double> > &hadCands, bool checkID, bool mT_cut) {
//   if (cfAVersion<77) return;
//   eCands.clear();
//   muCands.clear();
//   hadCands.clear();
//   // cout << "Found " << pfcand_pt->size() << " PFCands." << endl;
//   for (uint itk(0); itk<pfcand_pdgId->size(); itk++) {
//     if (isnan(pfcand_charge->at(itk)) || isnan(pfcand_pt->at(itk))  || isnan(pfcand_dz->at(itk)) || isnan(pfcand_phi->at(itk)) || isnan(pfcand_eta->at(itk)) ) continue;
//     if (static_cast<int>(pfcand_charge->at(itk))==0) continue;
//     double pt = pfcand_pt->at(itk);
//     if (pt<5) continue;
//     if (abs(TMath::Nint(pfcand_pdgId->at(itk)))==211&&pt<10) continue;
//     double eta = pfcand_eta->at(itk);
//     if (fabs(eta)>2.5) continue;
//     if (checkID&&!PassIsoTrackBaseline(itk)) continue;
//     if (mT_cut && GetMTW(pfcand_pt->at(itk),theMET_,pfcand_phi->at(itk),theMETPhi_)>100) continue;
//     //  int type = static_cast<int>(pfcand_pdgId->at(itk));
//     double iso = GetPFCandIsolation(itk);
//     double relIso=iso/pfcand_pt->at(itk);
//     // note: not cutting here on isolation!
//     switch (abs(TMath::Nint(pfcand_pdgId->at(itk)))) {
//     case 11:
//       eCands.push_back(std::make_pair(itk, relIso));
//       break;
//     case 13:
//       muCands.push_back(std::make_pair(itk, relIso));
//       break;
//     case 211:
//       // if (pfcand_pt->at(itk)>10) printf("pfcand %d: pdgId=%d, pt=%f, ch_rel_iso=%f, mT=%f\n", itk, TMath::Nint(pfcand_pdgId->at(itk)), pfcand_pt->at(itk), relIso, GetMTW(pfcand_pt->at(itk),theMET_,pfcand_phi->at(itk),theMETPhi_));
//       if (pt>10) hadCands.push_back(std::make_pair(itk, relIso));
//       break;
//     default:
//       continue;
//     }
//   }
//   return;
// }

double EventHandler::GetTransverseMassMu() const{
  //Find leading lepton
  vector<int> reco_veto_muons;
  if (cfAVersion>=75) reco_veto_muons=GetRecoMuons(true);
  else reco_veto_muons=GetRA2bMuons(true);
  
  double lep_pt(-1.), lep_phi(-1.);
  if (reco_veto_muons.size()<1) return -999.;
  if (cfAVersion>=75) {
    lep_pt=mus_pt->at(reco_veto_muons[0]);
    lep_phi=mus_phi->at(reco_veto_muons[0]);
  }
  else {
    lep_pt=pf_mus_pt->at(reco_veto_muons[0]);
    lep_phi=pf_mus_phi->at(reco_veto_muons[0]);
  }
  return GetMTW(lep_pt,theMET_,lep_phi,theMETPhi_);
}

double EventHandler::GetTransverseMassEl() const{
  //Find leading lepton
  vector<int> reco_veto_electrons;
  if (cfAVersion>=75) reco_veto_electrons=GetRecoElectrons(true);
  else reco_veto_electrons=GetRA2bElectrons(true);
  double lep_pt(-1.), lep_phi(-1.);
  if (reco_veto_electrons.size()<1) return -999.;
  if (cfAVersion>=75) {
    lep_pt=els_pt->at(reco_veto_electrons[0]);
    lep_phi=els_phi->at(reco_veto_electrons[0]);
  }
  else {
    lep_pt=pf_els_pt->at(reco_veto_electrons[0]);
    lep_phi=pf_els_phi->at(reco_veto_electrons[0]);
  }
  return GetMTW(lep_pt,theMET_,lep_phi,theMETPhi_);
}

double EventHandler::GetDeltaThetaT(const double lep_pt, const double lep_phi, const bool useMHT) const {
    
    //Code from an email from Kristen
    TLorentzVector WInLab(0.,0.,0.,0.);
    TLorentzVector LeptonInLab(0.,0.,0.,0.);
    TLorentzVector LeptonInWCM(0.,0.,0.,0.);

    double pfmet_0 = theMET_;
    double pfmet_x = pfmet_0 * cos(theMETPhi_);
    double pfmet_y = pfmet_0 * sin(theMETPhi_);
    if (useMHT) {
      TVector2 MHT = GetMHTVec();
      pfmet_x = MHT.X();
      pfmet_y = MHT.Y();
    }
    double lep_px = lep_pt*cos(lep_phi);
    double lep_py = lep_pt*sin(lep_phi);
    
    double mtw = GetMTW(lep_pt,theMET_,lep_phi,theMETPhi_);
    double energyw = TMath::Sqrt( mtw*mtw + (lep_px+pfmet_x)*(lep_px+pfmet_x)
				  + (lep_py+pfmet_y)*(lep_py+pfmet_y) );
    
    WInLab.SetXYZT( lep_px+pfmet_x, lep_py+pfmet_y, 0, energyw+0.0001 );
    LeptonInLab.SetXYZT( lep_px, lep_py, 0, lep_pt+0.0001 );

    TVector3 bstToW(WInLab.BoostVector());
    LeptonInLab.Boost(-bstToW);
    LeptonInWCM = LeptonInLab;
    
    double lep_px_wframe = LeptonInWCM.Px();
    double lep_py_wframe = LeptonInWCM.Py();
    
    double w_phi_labframe = TMath::ATan2( (lep_py+pfmet_y),(lep_px+pfmet_x) );
    double lep_phi_wframe = TMath::ATan2( lep_py_wframe,lep_px_wframe );

    return Math::GetDeltaPhi(lep_phi_wframe, w_phi_labframe);

}

double EventHandler::GetWpT(const double lep_pt, const double lep_phi, const bool useMHT) const {

  TVector2 nu;
  if (useMHT) nu = GetMHTVec();
  else nu.SetMagPhi(theMET_, theMETPhi_);
  TVector2 lep(lep_pt*cos(lep_phi), lep_pt*sin(lep_phi));
    
  TVector2 W(nu+lep);
  return W.Mod();

}

std::vector<int> EventHandler::GetTaus( const bool againstEMu, const bool mT_cut) const {
  // PHYS14 POG ID
  std::vector<int> taus;
  if (cfAVersion<77) return taus;
  for (uint itau(0); itau<taus_byDecayModeFinding->size(); itau++) {
    if (!PassPhys14TauID(itau, againstEMu, mT_cut)) continue;
    if (taus_byLooseCombinedIsolationDeltaBetaCorr3Hits->at(itau)<1) continue;
    taus.push_back(itau);
  }
  return taus;
}

bool EventHandler::PassPhys14TauID(const int itau, const bool againstEMu, const bool mT_cut) const {
  if (taus_pt->at(itau) < 20.) return false;
  if (fabs(taus_eta->at(itau)) > 2.3) return false;
  if (!taus_byDecayModeFindingNewDMs->at(itau)) return false;
  //  if (taus_chargedIsoPtSum->at(itau) > 1.) return false;
  if (againstEMu && (!taus_againstMuonLoose3->at(itau) || !taus_againstElectronLooseMVA5->at(itau))) return false;
  if (mT_cut && GetMTW(taus_pt->at(itau), theMET_, taus_phi->at(itau), theMETPhi_)>100) return false;
  return true;
}

std::vector<int> EventHandler::GetGenWs() const {
  std::vector<int> indices;
  for(unsigned int imc = 0; imc < mc_doc_id->size(); imc++){
    if (TMath::Nint(mc_doc_status->at(imc))==52 && abs(TMath::Nint(mc_doc_id->at(imc)))==24) indices.push_back(imc);
  }
  return indices;
}

bool EventHandler::DecaysToLeptons(const uint imc) const {
  int mc_id(TMath::Nint(mc_doc_id->at(imc)));
  for(unsigned int imc2 = imc; imc2 < mc_doc_id->size(); imc2++){
    if (!TMath::Nint(mc_doc_mother_id->at(imc2))==mc_id) continue;
    int mc2_id(TMath::Nint(mc_doc_id->at(imc2)));
    if (!(abs(mc2_id)==11||abs(mc2_id)==13||abs(mc2_id)==15)) continue;
    if (mc2_id*mc_id<0) return true;
  }
  return false;
}

void EventHandler::GetTrueLeptons(std::vector<int> &true_electrons, std::vector<int> &true_muons,
				   std::vector<int> &true_had_taus, std::vector<int> &true_lep_taus) {
  true_electrons.clear();
  true_muons.clear();
  true_had_taus.clear();
  true_lep_taus.clear();
  bool tau_to_3tau(false);
  vector<int> lep_from_tau;
  for(unsigned i = 0; i < mc_doc_id->size(); ++i){
    const int id = static_cast<int>(floor(fabs(mc_doc_id->at(i))+0.5));
    const int mom = static_cast<int>(floor(fabs(mc_doc_mother_id->at(i))+0.5));
    const int gmom = static_cast<int>(floor(fabs(mc_doc_grandmother_id->at(i))+0.5));
    const int ggmom = static_cast<int>(floor(fabs(mc_doc_ggrandmother_id->at(i))+0.5));
    if((id == 11 || id == 13) && (mom == 24 || (mom == 15 && (gmom == 24 || (gmom == 15 && ggmom == 24))))){
      if (mom == 24) { // Lep from W
	if (id==11) true_electrons.push_back(i);
	else if (id==13) true_muons.push_back(i);
      } else if(!tau_to_3tau) { // Lep from tau, check for Brem
	uint nlep(1);
	for(uint j=i+1; j<mc_doc_id->size(); ++j) {
	  const int idb = static_cast<int>(floor(fabs(mc_doc_id->at(j))+0.5));
	  const int momb = static_cast<int>(floor(fabs(mc_doc_mother_id->at(j))+0.5));
	  if(momb==15 && (idb==11 || idb==13)) nlep++;
	  if(momb!=15 || (momb==15&&idb==16) || j==mc_doc_id->size()-1){
	    if(nlep==1){
	      if (id==11) true_electrons.push_back(i);
	      else if (id==13) true_muons.push_back(i);
	      lep_from_tau.push_back(i);
	    }
	    i = j-1; // Moving index to first particle after tau daughters
	    break;
	  }
	} // Loop over tau daughters
      } // if lepton comes from tau
    }
    if(id == 15 && mom == 24){
      true_had_taus.push_back(i);
    }
    // Counting number of tau->tautautau
    if((id == 15) && (mom == 15 && (gmom == 24 || (gmom == 15 && ggmom == 24)))){
      uint nlep(1);
      for(uint j=i+1; j<mc_doc_id->size(); ++j) {
	const int idb = static_cast<int>(floor(fabs(mc_doc_id->at(j))+0.5));
	const int momb = static_cast<int>(floor(fabs(mc_doc_mother_id->at(j))+0.5));
	if(momb==15 && idb==15) nlep++;
	if(momb!=15 || (momb==15&&idb==16) || j==mc_doc_id->size()-1){
	  if(nlep>1) tau_to_3tau = true;
	  i = j-1; // Moving index to first particle after tau daughters
	  break;
	}
      } // Loop over tau daughters
    } // if tau comes from prompt tau
  } // Loop over mc_doc
  // Removing leptonic taus from tau list by finding smallest DeltaR(lep,tau)
  for(unsigned ind = 0; ind < lep_from_tau.size(); ++ind){
    float minDr(9999.), lepEta(mc_doc_eta->at(lep_from_tau[ind])), lepPhi(mc_doc_phi->at(lep_from_tau[ind]));
    int imintau(-1);
    for(unsigned itau=0; itau < true_had_taus.size(); itau++){
      float tauEta(mc_doc_eta->at(true_had_taus[itau])), tauPhi(mc_doc_phi->at(true_had_taus[itau]));
      float tauDr(Math::GetDeltaR(tauPhi, tauEta, lepPhi, lepEta));
      if(tauDr < minDr) {
	minDr = tauDr;
	imintau = itau;
      }
    }
    if(imintau>=0) {
      true_lep_taus.push_back(imintau);
      true_had_taus.erase(true_had_taus.begin()+imintau);
    } else cout<<"Not found a tau match for lepton "<<ind<<endl; // Should not happen
  } // Loop over leptons from taus
  return;
}

double EventHandler::GetDRToClosestParton(const int imc) const {
  double minDR(DBL_MAX);
  double i_eta(mc_doc_eta->at(imc)), i_phi(mc_doc_phi->at(imc));
  for(unsigned jmc = 0; jmc < mc_doc_id->size(); ++jmc){
    const int id = static_cast<int>(floor(fabs(mc_doc_id->at(jmc))+0.5));
    const int status = static_cast<int>(floor(fabs(mc_doc_status->at(jmc))+0.5));
    if (!(id==21||(id>=1&&id<=5))) continue; //partons only
    if (status<22||status>23) continue; // hard scatter only
    double DR = Math::GetDeltaR(i_phi, i_eta, mc_doc_phi->at(jmc), mc_doc_eta->at(jmc));
    if (DR<minDR) {
      minDR=DR;
    }
  }
  if (minDR>1000) return -999.;
  return minDR;
}

std::vector<int> EventHandler::MatchElectrons(const std::vector<int> true_electrons) const{
  std::vector<int> reco_electrons;
  for (uint iel(0); iel<true_electrons.size(); iel++) {
    reco_electrons.push_back(GetClosestRecoElectron(true_electrons[iel]));
  }
  return reco_electrons;
}

int EventHandler::GetClosestRecoElectron(const uint imc) const {
  const int bad_index = -1;
  if(imc >= mc_doc_id->size()) return bad_index;
  TVector3 mc3(mc_doc_pt->at(imc)*cos(mc_doc_phi->at(imc)),
	       mc_doc_pt->at(imc)*sin(mc_doc_phi->at(imc)),
	       mc_doc_pt->at(imc)*sinh(mc_doc_eta->at(imc)));
  // printf("True el: pt/eta/phi = %3.2f/%3.2f/%3.2f\n", mc_doc_pt->at(imc), mc_doc_eta->at(imc), mc_doc_phi->at(imc));
  float best_score = std::numeric_limits<float>::max();
  int best_part = bad_index;
  if (cfAVersion>=75) {
    for(uint iel(0); iel < els_pt->size(); iel++) {
      TVector3 el3(els_pt->at(iel)*cos(els_phi->at(iel)),
		   els_pt->at(iel)*sin(els_phi->at(iel)),
		   els_pt->at(iel)*sinh(els_scEta->at(iel)));
      float this_score = (el3-mc3).Mag2();
      // float deltaR = Math::GetDeltaR(mc_doc_phi->at(imc), mc_doc_eta->at(imc), els_phi->at(iel), els_scEta->at(iel));
      // float deltaPt =  (el3-mc3).Pt();
      if(this_score < best_score){
	best_score = this_score;
	best_part = iel;
	// printf("Reco el: pt/eta/phi = %3.2f/%3.2f/%3.2f\n", els_pt->at(iel), els_scEta->at(iel), els_phi->at(iel));
	// printf("el %d: dPt/dR/score = %3.2f/%3.2f/%3.2f\n", iel, deltaPt, deltaR, this_score);
      }
    }
  } else {
    for(uint iel(0); iel < pf_els_pt->size(); iel++) {
      TVector3 el3(pf_els_pt->at(iel)*cos(pf_els_phi->at(iel)),
		   pf_els_pt->at(iel)*sin(pf_els_phi->at(iel)),
		   pf_els_pt->at(iel)*sinh(pf_els_scEta->at(iel)));
      float this_score = (el3-mc3).Mag2();
      // float deltaR = Math::GetDeltaR(mc_doc_phi->at(imc), mc_doc_eta->at(imc), pf_els_phi->at(iel), pf_els_scEta->at(iel));
      // float deltaPt =  (el3-mc3).Pt();
      if(this_score < best_score){
	best_score = this_score;
	best_part = iel;
	// printf("Reco el: pt/eta/phi = %3.2f/%3.2f/%3.2f\n", pf_els_pt->at(iel), pf_els_scEta->at(iel), pf_els_phi->at(iel));
	// printf("el %d: dPt/dR/score = %3.2f/%3.2f/%3.2f\n", iel, deltaPt, deltaR, this_score);
      }
    }
  }
  if (best_score>200) return -1;
  //  cout << "Matched el: " << best_part << endl;
  return best_part;
}

std::vector<int> EventHandler::MatchMuons(const std::vector<int> true_muons) const{
  std::vector<int> reco_muons;
  for (uint imu(0); imu<true_muons.size(); imu++) {
    reco_muons.push_back(GetClosestRecoMuon(true_muons[imu]));
  }
  //  cout << "Filled reco muon vector " <<  reco_muons.size() << " times." << endl;
  return reco_muons;
}

int EventHandler::GetClosestRecoMuon(const uint imc/*, const double maxDeltaR, const double maxDeltaPT*/) const {
  const int bad_index = -1;
  if(imc >= mc_doc_id->size()) return bad_index;
  TVector3 mc3(mc_doc_pt->at(imc)*cos(mc_doc_phi->at(imc)),
		  mc_doc_pt->at(imc)*sin(mc_doc_phi->at(imc)),
		  mc_doc_pt->at(imc)*sinh(mc_doc_eta->at(imc)));
  //  printf("True mu: pt/eta/phi = %3.2f/%3.2f/%3.2f\n", mc_doc_pt->at(imc), mc_doc_eta->at(imc), mc_doc_phi->at(imc));
  float best_score = std::numeric_limits<float>::max();
  int best_part = bad_index;
  if (cfAVersion>=75) {
    for(uint imu(0); imu < mus_pt->size(); imu++) {
      TVector3 mu3(mus_pt->at(imu)*cos(mus_tk_phi->at(imu)),
		   mus_pt->at(imu)*sin(mus_tk_phi->at(imu)),
		   mus_pt->at(imu)*sinh(mus_eta->at(imu)));
      float this_score = (mu3-mc3).Mag2();
      // float deltaR = Math::GetDeltaR(mc_doc_phi->at(imc), mc_doc_eta->at(imc), mus_tk_phi->at(imu), mus_eta->at(imu));
      // float deltaPt =  (mu3-mc3).Pt();
      if(this_score < best_score){
	best_score = this_score;
	best_part = imu;
	//      printf("Reco mu: pt/eta/phi = %3.2f/%3.2f/%3.2f\n", mus_pt->at(imu), mus_eta->at(imu), mus_tk_phi->at(imu));
	//      printf("mu %d: dPt/dR/score = %3.2f/%3.2f/%3.2f\n", imu, deltaPt, deltaR, this_score);
      }
    }
  } else {
    for(uint imu(0); imu < pf_mus_pt->size(); imu++) {
      TVector3 mu3(pf_mus_pt->at(imu)*cos(pf_mus_tk_phi->at(imu)),
		   pf_mus_pt->at(imu)*sin(pf_mus_tk_phi->at(imu)),
		   pf_mus_pt->at(imu)*sinh(pf_mus_eta->at(imu)));
      float this_score = (mu3-mc3).Mag2();
      // float deltaR = Math::GetDeltaR(mc_doc_phi->at(imc), mc_doc_eta->at(imc), pf_mus_tk_phi->at(imu), pf_mus_eta->at(imu));
      // float deltaPt =  (mu3-mc3).Pt();
      if(this_score < best_score){
	best_score = this_score;
	best_part = imu;
	//      printf("Reco mu: pt/eta/phi = %3.2f/%3.2f/%3.2f\n", pf_mus_pt->at(imu), pf_mus_eta->at(imu), pf_mus_tk_phi->at(imu));
	//      printf("mu %d: dPt/dR/score = %3.2f/%3.2f/%3.2f\n", imu, deltaPt, deltaR, this_score);
      }
    }
  }
  if (best_score>200) return -1;
  //  cout << "Matched mu: " << best_part << endl;
  return best_part;
}

void EventHandler::PrintGenParticleInfo(const int imc) const {
  cout<<imc<<": ID "<<static_cast<int>(mc_doc_id->at(imc))<<", \tMom ID "<<static_cast<int>(mc_doc_mother_id->at(imc)) <<", \tGMom ID "<<static_cast<int>(mc_doc_grandmother_id->at(imc)) <<", \tGGMom ID "<<static_cast<int>(mc_doc_ggrandmother_id->at(imc))
      <<", \tN mothers "<<mc_doc_numOfMothers->at(imc)
      <<", \tN daughters "<<mc_doc_numOfDaughters->at(imc)
      <<",   \tpx "<<mc_doc_px->at(imc)
      <<",   \tpy "<<mc_doc_py->at(imc)
      <<",   \tpz "<<mc_doc_pz->at(imc)
    //   <<",   \tpT "<<mc_doc_pt->at(imc)
    //   <<",   \teta "<<mc_doc_eta->at(imc)
    //  <<",   \tphi "<<mc_doc_phi->at(imc)
      <<", \tstatus "<< static_cast<int>(mc_doc_status->at(imc)) <<endl;
  return;
}

std::vector<int> EventHandler::MatchTks(const std::vector<int> true_leps, const vector<std::pair<int,double> > tracks) const{
  std::vector<int> matched;
  for (uint ilep(0); ilep<true_leps.size(); ilep++) {
    matched.push_back(GetClosestTk(true_leps[ilep], tracks));
  }
  return matched;
}

int EventHandler::GetClosestTk(const uint imc, const vector<std::pair<int,double> > tracks) const {
  const int bad_index = -1;
  if(imc >= mc_doc_id->size()) return bad_index;
  TVector3 mc3(mc_doc_pt->at(imc)*cos(mc_doc_phi->at(imc)),
		  mc_doc_pt->at(imc)*sin(mc_doc_phi->at(imc)),
		  mc_doc_pt->at(imc)*sinh(mc_doc_eta->at(imc)));
  //  printf("True lep: pt/eta/phi = %3.2f/%3.2f/%3.2f\n", mc_doc_pt->at(imc), mc_doc_eta->at(imc), mc_doc_phi->at(imc));
  float best_score = std::numeric_limits<float>::max();
  int best_part = bad_index;
  for(uint itk(0); itk < tracks.size(); itk++) {
    TVector3 tk3(pfcand_pt->at(tracks[itk].first)*cos(pfcand_phi->at(tracks[itk].first)),
		  pfcand_pt->at(tracks[itk].first)*sin(pfcand_phi->at(tracks[itk].first)),
		  pfcand_pt->at(tracks[itk].first)*sinh(pfcand_eta->at(tracks[itk].first)));
    float this_score = (tk3-mc3).Mag2();
    //   float deltaR = Math::GetDeltaR(mc_doc_phi->at(imc), mc_doc_eta->at(imc), pfcand_phi->at(tracks[itk].first), pfcand_eta->at(tracks[itk].first));
    //    float deltaPt =  (tk3-mc3).Pt();
    if(this_score < best_score){
      best_score = this_score;
      best_part = itk;
      //     printf("Reco tk: pt/eta/phi = %3.2f/%3.2f/%3.2f\n", pfcand_pt->at(tracks[itk].first), pfcand_eta->at(tracks[itk].first), pfcand_phi->at(tracks[itk].first));
      //     printf("tk %d: dPt/dR/score = %3.2f/%3.2f/%3.2f\n", itk, deltaPt, deltaR, this_score);
    }
  }
  if (best_score>200) return -1;
  //  cout << "Matched tk: " << best_part << endl;
  return best_part;
}

double EventHandler::GetSimpleChargedTrackIsolation(const int ilep, const int ParticleType, const double rmax) const { // not relative

  double ptThresh(0.5);
  double lep_pt(0.), lep_eta(0.), lep_phi(0.), deadcone_nh(0.), deadcone_ch(0.), deadcone_ph(0.), deadcone_pu(0.);;
  if(ParticleType==11) {
    lep_pt = els_pt->at(ilep);
    lep_eta = els_eta->at(ilep);
    lep_phi = els_phi->at(ilep);
    ptThresh = 0;
    if (fabs(lep_eta)>1.479) {deadcone_ch = 0.015; deadcone_pu = 0.015; deadcone_ph = 0.08;}
  }else if(ParticleType==13){
    lep_pt = mus_pt->at(ilep);
    lep_eta = mus_eta->at(ilep);
    lep_phi = mus_phi->at(ilep);
    deadcone_ch = 0.0001; deadcone_pu = 0.01; deadcone_ph = 0.01;deadcone_nh = 0.01;
  } else{
    lep_pt = pfcand_pt->at(ilep);
    lep_eta = pfcand_eta->at(ilep);
    lep_phi = pfcand_phi->at(ilep);
    deadcone_ch = 0.0001; deadcone_pu = 0.01; deadcone_ph = 0.01;deadcone_nh = 0.01; // Using muon cones
  }

 // find the PF cands that matches the lepton
 double drmin = DBL_MAX;
 uint match_index = 9999999;
 for (unsigned int icand = 0; icand < pfcand_pt->size(); icand++) {
   if(isnan(pfcand_eta->at(icand))
      || isnan(pfcand_phi->at(icand))) continue;
   double dr = Math::dR(pfcand_eta->at(icand), lep_eta, pfcand_phi->at(icand), lep_phi);
   if (dr < drmin){
     drmin = dr;
     match_index = icand;
   }
 }

 double isoSum(0.0);
  for (unsigned int other = 0; other < pfcand_pt->size(); other++) {
   if (other==match_index) continue;
    if (isnan(pfcand_charge->at(other)) || isnan(pfcand_pt->at(other))  || isnan(pfcand_fromPV->at(other)) || isnan(pfcand_phi->at(other)) || isnan(pfcand_eta->at(other)) ) continue;
    if (static_cast<int>(pfcand_charge->at(other))==0) continue; // only consider charged tracks
    if (fabs(pfcand_fromPV->at(other))<=1) continue; // pileup suppression
    double deltaR = Math::GetDeltaR(pfcand_phi->at(match_index), pfcand_eta->at(match_index), pfcand_phi->at(other), pfcand_eta->at(other)); // isolation cone
    if (deltaR>rmax) continue;
    isoSum+=pfcand_pt->at(other);
  }

  return isoSum;
}

double EventHandler::GetIsolation(const int ilep, const int ParticleType, const double rmax, const bool mini, const bool addCH, const bool addPH, const bool addNH, const bool usePFweight) const { // note: not relative isolation!


double ptThresh(0.5);
 double lep_pt(0.), lep_eta(0.), lep_phi(0.), deadcone_nh(0.), deadcone_ch(0.), deadcone_ph(0.), deadcone_pu(0.);;
 if(ParticleType==11) {
   lep_pt = els_pt->at(ilep);
   lep_eta = els_eta->at(ilep);
   lep_phi = els_phi->at(ilep);
   ptThresh = 0;
   if (fabs(lep_eta)>1.479) {deadcone_ch = 0.015; deadcone_pu = 0.015; deadcone_ph = 0.08;}
 }else if(ParticleType==13){
   lep_pt = mus_pt->at(ilep);
   lep_eta = mus_eta->at(ilep);
   lep_phi = mus_phi->at(ilep);
   deadcone_ch = 0.0001; deadcone_pu = 0.01; deadcone_ph = 0.01;deadcone_nh = 0.01;
 } else{
   lep_pt = pfcand_pt->at(ilep);
   lep_eta = pfcand_eta->at(ilep);
   lep_phi = pfcand_phi->at(ilep);
   deadcone_ch = 0.0001; deadcone_pu = 0.01; deadcone_ph = 0.01;deadcone_nh = 0.01; // Using muon cones
 }

 bool need_pfweight = false;
 if(usePFweight) need_pfweight = true;

 double riso_max = std::max(0.4,10./lep_pt);
 // find the PF cands that matches the lepton
 double drmin = DBL_MAX;
 uint match_index = 9999999;
 for (unsigned int icand = 0; icand < pfcand_pt->size(); icand++) {
   if(isnan(pfcand_eta->at(icand))
      || isnan(pfcand_phi->at(icand))) continue;
   double dr = Math::dR(pfcand_eta->at(icand), lep_eta, pfcand_phi->at(icand), lep_phi);
   if (dr < drmin){
     drmin = dr;
     match_index = icand;
   }
 }
 
 // 11, 13, 22 for ele/mu/gamma, 211 for charged hadrons, 130 for neutral hadrons,
 // 1 and 2 for hadronic and em particles in HF
 double iso_nh(0.), iso_ph(0.), iso_ch(0.), iso_pu(0.);
 // get cone size--shrinking if mini, otherwise fixed
 double R(0);
 if (mini) R=std::max(0.05,std::min(rmax, 10./lep_pt));
 else R=rmax;

 for (unsigned int icand = 0; icand < pfcand_pt->size(); icand++) {
   if (icand==match_index) continue;
   uint pdgId = TMath::Nint(pfcand_pdgId->at(icand));
   if (pdgId<7) continue;
   if(isnan(pfcand_pt->at(icand))
      || isnan(pfcand_eta->at(icand))
      || isnan(pfcand_phi->at(icand))) continue;
   double dr = Math::dR(pfcand_eta->at(icand), lep_eta, pfcand_phi->at(icand), lep_phi);
   if (dr > riso_max) continue;
   ////////////////// NEUTRALS /////////////////////////
   if (pfcand_charge->at(icand)==0){
     if (pfcand_pt->at(icand)>ptThresh) {
       double wpv(0.), wpu(0.), wpf(1.);
       for (unsigned int jcand = 0; need_pfweight && jcand < pfcand_pt->size(); jcand++) {
	 if (pfcand_charge->at(icand)!=0 || icand==jcand) continue;
	 double jpt = pfcand_pt->at(jcand);
	 double jdr = Math::dR(pfcand_eta->at(icand), pfcand_eta->at(jcand),
			 pfcand_phi->at(icand), pfcand_phi->at(jcand));
	 if(jdr<=0) continue; // We can either not count it, or count it infinitely...
	 if (pfcand_fromPV->at(icand)>1) wpv += log(jpt/jdr);
	 else wpu += log(jpt/jdr);
       }
       /////////// PHOTONS ////////////
       if (pdgId==22) {
	 if(dr < deadcone_ph) continue;
	   wpf = (usePFweight)?(wpv/(wpv+wpu)):1.;
	   if (dr<R) iso_ph += wpf*pfcand_pt->at(icand);
	 }
	 /////////// NEUTRAL HADRONS ////////////
       else if (pdgId==130) {
	 if(dr < deadcone_nh) continue;
	   wpf = (usePFweight)?(wpv/(wpv+wpu)):1.;
	   if (dr<R) iso_nh += wpf*pfcand_pt->at(icand);
	 }
       }
     ////////////////// CHARGED from PV /////////////////////////
     } else if (pfcand_fromPV->at(icand)>1){
     if (abs(pdgId)==211) {
       if(dr < deadcone_ch) continue;
       if (dr<R) {
	 // if (ParticleType==13) cout << "Adding to cone.." << endl;
	 // if (ParticleType==13) printf("imu %d, ch pfcand %d: pt=%f, deadcone=%f, dr cut=%f, dr=%f\n",ilep, icand, pfcand_pt->at(icand), deadcone_ch, R, dr); 
	 iso_ch += pfcand_pt->at(icand);
       }
     }
     ////////////////// CHARGED from PU /////////////////////////
   } else {
     if (pfcand_pt->at(icand)>ptThresh){
       if(dr < deadcone_pu) continue;
       if (dr<R) iso_pu += pfcand_pt->at(icand);}
     }
   }

 // now add the components
 double isolation(0.);
  if(addPH) isolation += iso_ph;
  if(addNH) isolation += iso_nh;
  if(addPH && addNH && !usePFweight) isolation -= iso_pu/2.;
  if(isolation < 0) isolation = 0;
  if(addCH) isolation += iso_ch;

  // if (ParticleType==13){
  //   printf("Muon %d: pt=%3.3f, mini_iso=%3.3f, iso_ph=%3.3f, iso_nh=%3.3f, iso_ch=%3.3f, iso_pu=%3.3f\n", ilep, mus_pt->at(ilep), isolation, iso_ph, iso_nh, iso_ch, iso_pu); 
  // }
  
  return isolation;
}

// Truth-matching, from Adam, et. al.

int EventHandler::GetTrueMuon(float RecPt, float RecEta, float RecPhi, int &momID, bool &fromW, float &closest_deltaR) const {
  // Jack: now Reco pt, eta, phi, can come from any collection (i.e. pfcand)
  // if(index < 0 || index >= static_cast<int>(mus_eta->size())) return -1;

  closest_deltaR = 9999.; // Old deltaR
  int closest_imc = -1, idLepton = 0;
  // float RecPt = mus_pt->at(index), RecEta = mus_eta->at(index), RecPhi = mus_phi->at(index);
  closest_imc = GetTrueParticle(RecPt, RecEta, RecPhi, closest_deltaR, 13);

  closest_deltaR = 9999.;
  if(closest_imc >= 0){
    idLepton = static_cast<int>(mc_doc_id->at(closest_imc));
    momID = GetMom(mc_doc_id->at(closest_imc), mc_doc_mother_id->at(closest_imc),
                   mc_doc_grandmother_id->at(closest_imc),
                   mc_doc_ggrandmother_id->at(closest_imc),
                   fromW);
    // Finding mindR with respect to partons from top or W or status 23
    for(unsigned imc=0; imc < mc_doc_id->size(); imc++){
      if((abs(TMath::Nint(mc_doc_mother_id->at(imc))) != 24 && abs(TMath::Nint(mc_doc_mother_id->at(imc))) != 6 &&
          mc_doc_status->at(imc) != 23) || abs(TMath::Nint(mc_doc_id->at(imc))) > 5) continue;
      float MCEta = mc_doc_eta->at(imc); float MCPhi = mc_doc_phi->at(imc);
      float deltaR = Math::dR(RecEta,MCEta, RecPhi,MCPhi);
      if(deltaR < closest_deltaR) closest_deltaR = deltaR;
    }
  } else {
    closest_imc = GetTrueParticle(RecPt, RecEta, RecPhi, closest_deltaR, 0);
    if(closest_imc >= 0){
      idLepton = static_cast<int>(mc_doc_id->at(closest_imc));
      momID = GetMom(mc_doc_id->at(closest_imc), mc_doc_mother_id->at(closest_imc),
                     mc_doc_grandmother_id->at(closest_imc),
                     mc_doc_ggrandmother_id->at(closest_imc),
                     fromW);
    } else {
      momID = 0;
      idLepton = 0;
      fromW = false;
    }
  }
  return idLepton;
}

int EventHandler::GetTrueElectron(float RecPt, float RecEta, float RecPhi, int &momID, bool &fromW, float &closest_deltaR) const {
  // Jack: now Reco pt, eta, phi, can come from any collection (i.e. pfcand)
  // if(index < 0 || index >= static_cast<int>(els_eta->size())) return -1;

  closest_deltaR = 9999.;
  int closest_imc = -1, idLepton = 0;
  // float RecPt = els_pt->at(index), RecEta = els_eta->at(index), RecPhi = els_phi->at(index);
  closest_imc = GetTrueParticle(RecPt, RecEta, RecPhi, closest_deltaR, 11);

  closest_deltaR = 9999.;
  if(closest_imc >= 0){
    idLepton = static_cast<int>(mc_doc_id->at(closest_imc));
    momID = GetMom(mc_doc_id->at(closest_imc), mc_doc_mother_id->at(closest_imc),
                   mc_doc_grandmother_id->at(closest_imc),
                   mc_doc_ggrandmother_id->at(closest_imc),
                   fromW);
    // Finding mindR with respect to partons from top or W or status 23
    for(unsigned imc=0; imc < mc_doc_id->size(); imc++){
      if((abs(TMath::Nint(mc_doc_mother_id->at(imc))) != 24 && abs(TMath::Nint(mc_doc_mother_id->at(imc))) != 6 &&
          mc_doc_status->at(imc) != 23) || abs(TMath::Nint(mc_doc_id->at(imc))) > 5) continue;
      float MCEta = mc_doc_eta->at(imc); float MCPhi = mc_doc_phi->at(imc);
      float deltaR = Math::dR(RecEta,MCEta, RecPhi,MCPhi);
      if(deltaR < closest_deltaR) closest_deltaR = deltaR;
    }
  } else {
    closest_imc = GetTrueParticle(RecPt, RecEta, RecPhi, closest_deltaR, 0);
    if(closest_imc >= 0){
      momID = GetMom(mc_doc_id->at(closest_imc), mc_doc_mother_id->at(closest_imc),
                     mc_doc_grandmother_id->at(closest_imc),
                     mc_doc_ggrandmother_id->at(closest_imc),
                     fromW);
      idLepton = static_cast<int>(mc_doc_id->at(closest_imc));
    } else {
      momID = 0;
      idLepton = 0;
      fromW = false;
    }
  }
  return idLepton;
}

int EventHandler::GetTrueParticle(float RecPt, float RecEta, float RecPhi,
                                  float &closest_deltaR, int ID) const {
  const float pT_Threshold(0.3), dR_Threshold(0.1);
  int closest_imc = -1;
  float deltaR = 9999.; closest_deltaR = 9999.;
  float MCEta, MCPhi;
  for(unsigned imc=0; imc < mc_doc_id->size(); imc++){
    if(ID!=0 && abs(TMath::Nint(mc_doc_id->at(imc))) != ID) continue;
    MCEta = mc_doc_eta->at(imc); MCPhi = mc_doc_phi->at(imc);
    deltaR = Math::dR(RecEta,MCEta, RecPhi,MCPhi);
    if(deltaR > dR_Threshold || fabs(mc_doc_pt->at(imc)-RecPt)/RecPt > pT_Threshold) continue;
    if(deltaR < closest_deltaR && deltaR < dR_Threshold) {
      closest_deltaR = deltaR;
      closest_imc = imc;
    }
  }
  return closest_imc;
}

int EventHandler::GetMom(const float id, const float mom, const float gmom,
                         const float ggmom, bool &fromW){
  const int iid = TMath::Nint(id);
  const int imom = TMath::Nint(mom);
  const int igmom = TMath::Nint(gmom);
  const int iggmom = TMath::Nint(ggmom);

  int ret_mom = 0;
  if(imom != iid)       ret_mom = imom;
  else if(igmom != iid) ret_mom = igmom;
  else                  ret_mom = iggmom;

  // There's various leptons that seem to come from W->udsc->l. We won't call them prompt
  fromW = abs(ret_mom)==24 || (abs(ret_mom)==15 && (abs(igmom)==24 || abs(iggmom)==24));

  return ret_mom;
}

size_t EventHandler::MatchCandToStatus1(size_t icand,
                                        const std::vector<mc_particle> &parts) const{
  const size_t bad_index = static_cast<size_t>(-1);
  if(icand >= pfcand_charge->size()) return bad_index;

  if(isnan(pfcand_pt->at(icand))
     || isnan(pfcand_eta->at(icand))
     || isnan(pfcand_phi->at(icand))) return bad_index;

  TVector3 p3cand(pfcand_pt->at(icand)*cos(pfcand_phi->at(icand)),
                  pfcand_pt->at(icand)*sin(pfcand_phi->at(icand)),
                  pfcand_pt->at(icand)*sinh(pfcand_eta->at(icand)));

  float best_score = std::numeric_limits<float>::max();
  size_t best_part = bad_index;
  for(size_t imc = parts.size()-1; imc < parts.size(); --imc){
    const mc_particle &part = parts.at(imc);
    if(part.status_ != 0) continue;
    if(fabs(3.*part.charge_)<0.5) continue;
    float this_score = (part.momentum_.Vect()-p3cand).Mag2();
    if(this_score < best_score){
      best_score = this_score;
      best_part = imc;
    }
  }

  return best_part;
}

std::vector<mc_particle> EventHandler::GetMCParticles() const{
  std::vector<mc_particle> parts;
  for(size_t idoc = 0; idoc < mc_doc_id->size(); ++idoc){
    TLorentzVector v(mc_doc_px->at(idoc),
                     mc_doc_py->at(idoc),
                     mc_doc_pz->at(idoc),
                     mc_doc_energy->at(idoc));

    parts.push_back(mc_particle(v,
                                mc_doc_charge->at(idoc),
                                TMath::Nint(mc_doc_id->at(idoc)),
                                TMath::Nint(mc_doc_mother_id->at(idoc)),
                                TMath::Nint(mc_doc_grandmother_id->at(idoc)),
                                TMath::Nint(mc_doc_ggrandmother_id->at(idoc)),
                                TMath::Nint(mc_doc_status->at(idoc))));
  }
  size_t to_search = parts.size();

  for(size_t ifinal = 0; ifinal < mc_final_id->size(); ++ifinal){
    TLorentzVector v;
    v.SetPtEtaPhiE(mc_final_pt->at(ifinal),
                   mc_final_eta->at(ifinal),
                   mc_final_phi->at(ifinal),
                   mc_final_energy->at(ifinal));
    float charge = mc_final_charge->at(ifinal);
    int id = TMath::Nint(mc_final_id->at(ifinal));
    int mom = TMath::Nint(mc_final_mother_id->at(ifinal));
    int gmom = TMath::Nint(mc_final_grandmother_id->at(ifinal));
    int ggmom = TMath::Nint(mc_final_ggrandmother_id->at(ifinal));

    //Comment out next line to avoid appending particles already in mc_doc
    to_search = 0;
    bool skip = false;
    for(size_t i = 0; !skip && i < to_search; ++i){
      mc_particle &part = parts.at(i);
      if(TMath::Nint(3.*charge) == part.charge_
         && id == part.id_
         && mom == part.mom_
         && gmom == part.gmom_
         && ggmom == part.ggmom_
         && (v-part.momentum_).Vect().Mag()<0.01) skip = true;
    }
    if(!skip){
      parts.push_back(mc_particle(v, charge, id, mom, gmom, ggmom, 0));
    }
  }
  return parts;
}

std::vector<size_t> EventHandler::GetMoms(const std::vector<mc_particle> &parts){
  std::vector<size_t> moms(parts.size());
  for(size_t imc = 0; imc < moms.size(); ++imc){
    moms.at(imc) = GetMom(imc, parts);
  }
  return moms;
}

size_t EventHandler::GetMom(size_t index, const std::vector<mc_particle> &parts){
  const size_t bad_index = static_cast<size_t>(-1);
  if(index >= parts.size()) return bad_index;

  const mc_particle &daughter = parts.at(index);

  size_t low_cousin = index, high_cousin = index;
  bool abort = false;
  for(size_t imc = index - 1 ; !abort && imc < parts.size(); --imc){
    const mc_particle &cousin = parts.at(imc);

    if(cousin.mom_ == daughter.mom_
       && cousin.gmom_ == daughter.gmom_
       && cousin.ggmom_ == daughter.ggmom_){
      low_cousin = imc;
    }else{
      abort = true;
    }
  }

  abort = false;
  for(size_t imc = index + 1 ; !abort && imc < parts.size(); ++imc){
    const mc_particle &cousin = parts.at(imc);

    if(cousin.mom_ == daughter.mom_
       && cousin.gmom_ == daughter.gmom_
       && cousin.ggmom_ == daughter.ggmom_){
      high_cousin = imc;
    }else{
      abort = true;
    }
  }

  float best_score = std::numeric_limits<float>::max();
  size_t best_parent = bad_index;
  //Loop over possible parents
  for(size_t imc = 0; imc < index; ++imc){
    const mc_particle &mom = parts.at(imc);

    if(!(mom.id_ == daughter.mom_
         && mom.mom_ == daughter.gmom_
         && mom.gmom_ == daughter.ggmom_)) continue;

    //Find parent best momentum matched to sum of consecutive potential daughters
    for(size_t low = low_cousin; low <= index; ++low){
      TVector3 diff = mom.momentum_.Vect();
      for(size_t high = low; high <= high_cousin; ++high){
        const mc_particle &sister = parts.at(high);
        diff -= sister.momentum_.Vect();
        if(high < index) continue;
        float this_score = diff.Mag2();
        if(this_score < best_score){
          best_score = this_score;
          best_parent = imc;
        }
      }
    }
  }

  return best_parent;
}

bool EventHandler::FromTau(size_t index,
                           const std::vector<mc_particle> &parts,
                           const std::vector<size_t> &moms){
  if(index >= moms.size()) return false;
  int id = parts.at(index).id_;
  size_t i = moms.at(index);
  while(i < moms.size()){
    if(abs(parts.at(i).id_) == 15 && FromW(i, parts, moms)) return true;
    if(parts.at(i).id_ != id) return false;
    i = moms.at(i);
  }
  return false;
}

bool EventHandler::FromTauLep(size_t index,
                              const std::vector<mc_particle> &parts,
                              const std::vector<size_t> &moms){
  if(index >= moms.size()) return false;
  size_t parent_tau = ParentTau(index, parts, moms);
  for(size_t ilep = parts.size()-1; ilep < parts.size(); --ilep){
    const mc_particle &part = parts.at(ilep);
    if((abs(part.id_) == 11 || abs(part.id_) == 13)
       && IsDescendantOf(ilep, parent_tau, moms)
       && NumDescendants(ilep, parts, moms)==0){
      return true;
    }
  }
  return false;
}

size_t EventHandler::ParentTau(size_t index,
                               const std::vector<mc_particle> &parts,
                               const std::vector<size_t> &moms){
  size_t tau = static_cast<size_t>(-1);
  while(index < moms.size()){
    if(abs(parts.at(index).id_) == 15 && FromW(index, parts, moms)) tau = index;
    index = moms.at(index);
  }
  return tau;
} 

unsigned EventHandler::NumDescendants(size_t index,
                                      const std::vector<mc_particle> &parts,
                                      const std::vector<size_t> &moms,
                                      bool req_chg){
  unsigned num_descendants = 0;
  for(size_t istart = index + 1; istart < moms.size(); ++istart){
    if(parts.at(istart).status_ == 0
       && (!req_chg || parts.at(istart).charge_!=0)
       && IsDescendantOf(istart, index, moms)) ++num_descendants;
  }
  return num_descendants;
}

bool EventHandler::IsDescendantOf(size_t descendant, size_t ancestor,
                                  const std::vector<size_t> &moms){
  if(descendant <= ancestor || descendant >= moms.size()) return false;
  size_t i = moms.at(descendant);
  while(i < moms.size()){
    if(i == ancestor) return true;
    i = moms.at(i);
  }
  return false;
}

bool EventHandler::FromW(size_t index,
                         const vector<mc_particle> &parts,
                         const vector<size_t> &moms){
  if(index >= moms.size()) return false;
  if(IsBrem(index, parts, moms)) return false;

  index = moms.at(index);
  bool found_w = false;
  bool found_bad = false;
  while(index < moms.size()){
    int id = abs(parts.at(index).id_);
    bool bad = (id < 11 || id >16 ) && id != 24;
    if(id == 24){
      if(found_bad){
        return false;
      }else{
        found_w = true;
      }
    }
    if(bad){
      if(found_w){
        found_bad = true;
      }else{
        return false;
      }
    }
    index = moms.at(index);
  }
  return found_w;
}

bool EventHandler::IsBrem(size_t index,
                          const std::vector<mc_particle> &parts,
                          const std::vector<size_t> &moms){
  if(index >= moms.size() || moms.at(index) >= moms.size()) return false;
  const mc_particle &part = parts.at(index);
  int id = part.id_;
  if(abs(id) != 11 && abs(id) != 13 && abs(id) != 15) return false;
  int mom = parts.at(moms.at(index)).id_;
  if(abs(mom) != 11 && abs(mom) != 13 && abs(mom) !=15) return false;

  size_t low, high;
  for(low = index; low < moms.size(); --low){
    if(moms.at(low) != moms.at(index)) break;
  }
  ++low;
  for(high = index; high < moms.size(); ++high){
    if(moms.at(high) != moms.at(index)) break;
  }

  unsigned p = 0, n = 0;
  size_t hp = 0, hn = 0;
  float pp = 0, np = 0;
  for(size_t i = low; i < high; ++i){
    float p3 = parts.at(i).momentum_.Vect().Mag2();
    if(parts.at(i).id_ == abs(id)){
      ++p;
      if(p3>pp){
        pp = p3;
        hp = i;
      }
    }else if(parts.at(i).id_ == -abs(id)){
      ++n;
      if(p3>np){
        np = p;
        hn = i;
      }
    }
  }

  if(((p>n && id > 0 && index == hp) || (n>p && id < 0 && index == hn))){
    return false;
  }else{
    return true;
  }
}

bool EventHandler::TrackIsTrueLepton(const uint itk, const std::vector<int> true_leptons) const {
  if (itk >=pfcand_pt->size()) return false;
  TVector3 tk3(pfcand_pt->at(itk)*cos(pfcand_phi->at(itk)),
		  pfcand_pt->at(itk)*sin(pfcand_phi->at(itk)),
		  pfcand_pt->at(itk)*sinh(pfcand_eta->at(itk)));
  //  printf("Track: pt/eta/phi = %3.2f/%3.2f/%3.2f\n", tk3.Pt(), tk3.Eta(), tk3.Phi());
  float best_score = std::numeric_limits<float>::max();
  for (uint itru(0); itru<true_leptons.size(); itru++) {
    int imc = true_leptons[itru];
    TVector3 mc3(mc_doc_pt->at(imc)*cos(mc_doc_phi->at(imc)),
		 mc_doc_pt->at(imc)*sin(mc_doc_phi->at(imc)),
		 mc_doc_pt->at(imc)*sinh(mc_doc_eta->at(imc)));
    float this_score = (tk3-mc3).Mag2();
    //    float deltaR = Math::GetDeltaR(mc_doc_phi->at(imc), mc_doc_eta->at(imc), tk3.Phi(), tk3.Eta());
    //    float deltaPt =  (tk3-mc3).Pt();
    if(this_score < best_score){
      best_score = this_score;
      //     printf("True lep: pt/eta/phi = %3.2f/%3.2f/%3.2f\n", mc_doc_pt->at(imc), mc_doc_eta->at(imc), mc_doc_phi->at(imc));
      //     printf("True lep %d: dPt/dR/score = %3.2f/%3.2f/%3.2f\n", imc, deltaPt, deltaR, this_score);
    }
  }
  //  cout << "Best score: " << best_score << endl;
  if (best_score<200) return true;
  //  cout << "No match." << endl;
  return false;
}

std::vector<int> EventHandler::GetNeutrinosInJet(const uint ijet) const {
  double jet_eta(corrJets[ijet].GetTLV(theJESType_).Eta()), jet_phi(corrJets[ijet].GetTLV(theJESType_).Eta());
  vector<int> neutrinos;
  for (uint imc(0); imc<mc_final_id->size(); imc++) {
    uint id = abs(TMath::Nint(mc_final_id->at(imc)));
    //  uint momid = abs(TMath::Nint(mc_final_mother_id->at(imc)));
    if (id==12||id==14||id==16) {
      if (Math::dR(jet_eta,mc_final_eta->at(imc), jet_phi,mc_final_phi->at(imc))<0.4) neutrinos.push_back(imc);
    }
  }
  return neutrinos;
}

int EventHandler::GetMaxPtNu(const std::vector<int> nus) const {
  int lead_nu = -1;
  double maxpt = 0.;
  for (uint imc(0); imc<nus.size(); imc++) {
    double pt = mc_final_pt->at(nus[imc]);
    if (pt>maxpt) {
      maxpt=pt;
      lead_nu=nus[imc];
    }
  }
  return lead_nu;
}

double EventHandler::GetVectorPtSum(const std::vector<int> nus) const {
  double px(0.), py(0.);
  for (uint imc(0); imc<nus.size(); imc++) {
    px+=mc_final_pt->at(nus[imc])*cos(mc_final_phi->at(nus[imc]));
    py+=mc_final_pt->at(nus[imc])*sin(mc_final_phi->at(nus[imc]));
  }
  return sqrt(px*px+py*py);
}

double EventHandler::GetSumSkinnyJetMass() const {
  double sum(0.);
 for (unsigned int ijet=0; ijet<corrJets.size(); ++ijet) {
   // must include all of the jets that go into fat jet clustering--only cut is |eta|<5
   if (fabs(corrJets[ijet].GetTLV(theJESType_).Eta())>5.) continue;
   sum+=corrJets[ijet].GetTLV(theJESType_).M();
 }
 return sum;
}

unsigned int EventHandler::GetNumPhotons(const double pt_cut) const {
  if (cfAVersion<78) return 0;
  // cout << "GetNumPhotons" << endl;
  uint nphotons(0);
  for (uint iph(0); iph<photons_pt->size(); iph++) {
    if (isGoodPhoton(iph, pt_cut)) nphotons++;
  }
  return nphotons;
}

double EventHandler::GetPhotonIsolation(const iso_type_t type, const double raw_iso, const double eta) const {
  if (type!=CH&&type!=NH&&type!=PH) {
    cerr << "Error: I don't know which photon isolation to compute." << endl;
    return -1.;
  }
  // cout << "GetPhotonIsolation" << endl;
  double EA_ch(0.), EA_nh(0.), EA_ph(0.);
  // new effective areas: https://twiki.cern.ch/twiki/bin/view/CMS/CutBasedPhotonIdentificationRun2
  // cout << "Using new EAs..." << endl;
  if (fabs(eta)<1.0) {
    EA_ch=0.0234; EA_nh=0.0053; EA_ph=0.078;
  } else if (fabs(eta)>=1.0&&fabs(eta)<1.479) {
    EA_ch=0.0189; EA_nh=0.0103; EA_ph=0.0629;
  }
  else if (fabs(eta)>=1.479&&fabs(eta)<2.0) {
    EA_ch=0.0171; EA_nh=0.0057; EA_ph=0.0264;
  }
  else if (fabs(eta)>=2.0&&fabs(eta)<2.2) {
    EA_ch=0.0129; EA_nh=0.0070; EA_ph=0.0462;
  }
  else if (fabs(eta)>=2.2&&fabs(eta)<2.3) {
    EA_ch=0.0110; EA_nh=0.0152; EA_ph=0.0740;
  }
  else if (fabs(eta)>=2.3&&fabs(eta)<2.4) {
    EA_ch=0.0074; EA_nh=0.0232; EA_ph=0.0924;
  }
  else if (fabs(eta)>=2.4) {
    EA_ch=0.0035; EA_nh=0.1709; EA_ph=0.1484;
  }
  	
  double EA(0.);
  switch(type) {
  case CH : 
    EA=EA_ch;
    break;
  case NH :
    EA=EA_nh;
    break;
  case PH :
    EA=EA_ph;
    break;
  default :
    EA=0.;
  }
  double corr_iso = raw_iso-EA*fixedGridRhoFastjetAll;
  // printf("type=%d, raw_iso=%3.2f, eta=%3.2f, EA=%3.2f, rho=%3.2f, corr_iso=%3.2f\n",type,raw_iso,eta,EA,fixedGridRhoFastjetAll,corr_iso);
  if (corr_iso<0.) return 0.;
  else return corr_iso;
  //return std::max(corr_iso, 0.);
  
}

bool EventHandler::isGoodPhoton(uint iph, const double pt_cut) const{

  double max_iso_ch = 0.;
  double max_iso_nh = 0.;
  double max_iso_ph = 0.;
    
  double pt = photons_pt->at(iph);
  if (pt<pt_cut) return false;
  bool isBarrelPhoton(false), isEndcapPhoton(false);
  double eta=photons_eta->at(iph);
  if(fabs(eta) < 1.4442 ) isBarrelPhoton=true;
  else if(fabs(eta)>1.566 && fabs(eta)<2.5) isEndcapPhoton=true;
  else {
    isBarrelPhoton=false;
    isEndcapPhoton=false;
  }
  if (!isBarrelPhoton && !isEndcapPhoton) return false;
  // printf("Photon %d: pt %3.2f, eta %3.2f\n", iph, pt, eta);
  if (!PassPhotonID(iph)) return false;
  // printf("Passed ID\n");
  max_iso_ch = (isBarrelPhoton) ? 2.67 : 1.79;
  max_iso_nh = (isBarrelPhoton) ? 7.23 + TMath::Exp(0.0028*pt+0.5408) : 8.89+ 0.01725*pt;
  max_iso_ph = (isBarrelPhoton) ? 2.11 + 0.0014*pt : 3.09 + 0.0091*pt;
  double ch_iso = GetPhotonIsolation(CH, photons_pf_ch_iso->at(iph), eta);
  double nh_iso = GetPhotonIsolation(NH, photons_pf_nh_iso->at(iph), eta);
  double ph_iso = GetPhotonIsolation(PH, photons_pf_ph_iso->at(iph), eta);
  // printf("CH Iso %3.2f, NH Iso %3.2f, PH Iso %3.2f\n", ch_iso, nh_iso, ph_iso);
  if (ch_iso>max_iso_ch) return false;
  if (nh_iso>max_iso_nh) return false;
  if (ph_iso>max_iso_ph) return false;
  // printf("Passed isolation\n");
  return true;
}

bool EventHandler::PassPhotonID(uint iph) const{
  bool isBarrelPhoton(false);
  double eta=photons_eta->at(iph);
  if(fabs(eta) < 1.4442 ) isBarrelPhoton=true;
  double sigmaIetaIeta_cut =  isBarrelPhoton ? 0.0176 : 0.0272;
  double hadTowOverEM_cut = (isBarrelPhoton) ? 0.028 : 0.093;

  //  printf("sieie %3.2f, hOverE %3.2f\n", photons_sigmaIetaIeta->at(iph), photons_hadTowOverEM->at(iph));
  if (cfAVersion<78&&photons_sigmaIetaIeta->at(iph)>sigmaIetaIeta_cut) return false;
  if (cfAVersion>=78&&photons_full5x5sigmaIEtaIEta->at(iph)>sigmaIetaIeta_cut) return false;
  if (photons_hadTowOverEM->at(iph)>hadTowOverEM_cut) return false;
  //if (cfAVersion>=78&&!photons_pass_el_veto->at(iph)) return false;
  return true;
}

TVector2 EventHandler::GetPhotonMHTVec(const double jet_pt_cut, const double jet_eta_cut, const double ph_pt_cut) const{
  TVector2 mht_vec = GetMHTVec(jet_pt_cut, jet_eta_cut);
  for (uint iph(0); iph<photons_pt->size(); iph++) {
    if (cfAVersion<78||!isGoodPhoton(iph, ph_pt_cut)) continue;
    TVector2 phVec(photons_px->at(iph),photons_py->at(iph));
    mht_vec+=phVec;
  }
  return mht_vec;
}

double EventHandler::GetPhotonMHT(const double jet_pt_cut, const double jet_eta_cut, const double ph_pt_cut) const {
  TVector2 vec = GetPhotonMHTVec(jet_pt_cut, jet_eta_cut, ph_pt_cut);
  return vec.Mod();
}

double EventHandler::GetPhotonMHTPhi(const double jet_pt_cut, const double jet_eta_cut, const double ph_pt_cut) const {
  TVector2 vec = GetPhotonMHTVec(jet_pt_cut, jet_eta_cut, ph_pt_cut);
  return TVector2::Phi_mpi_pi(vec.Phi());
}

std::vector<int> EventHandler::GetPhotons(double pt_cut, bool checkID) const {
  std::vector<int> photons;
  if (cfAVersion<78) return photons;
  for (uint iph(0); iph<photons_pt->size(); iph++) {
    if (photons_pt->at(iph)<pt_cut) continue;
    bool isBarrelPhoton(false), isEndcapPhoton(false);
    double eta=photons_eta->at(iph);
    if(fabs(eta) < 1.4442 ) isBarrelPhoton=true;
    else if(fabs(eta)>1.566 && fabs(eta)<2.5) isEndcapPhoton=true;
    else {
      isBarrelPhoton=false;
      isEndcapPhoton=false;
    }
    if (!isBarrelPhoton && !isEndcapPhoton) continue;
    if (checkID&&!PassPhotonID(iph)) continue;
    photons.push_back(iph);
  }
  return photons;
}

double EventHandler::GetActivity(const int type, const double eta, const double phi, const double maxDeltaR) const {
  double activity(0.);
  for (unsigned int ijet=0; ijet < corrJets.size() ; ijet++)
	{
	  TLorentzVector TLV(corrJets[ijet].GetTLV(theJESType_));
	  if( Math::dR(eta, TLV.Eta(), phi, TLV.Phi()) > maxDeltaR ) continue;
	  if (type==11) activity+=TLV.Pt() * (jets_AKPF_chgHadE->at(corrJets[ijet].GetIndex()) / TLV.E());
	  else activity+=TLV.Pt() * ( (jets_AKPF_chgEmE->at(corrJets[ijet].GetIndex())+jets_AKPF_chgHadE->at(corrJets[ijet].GetIndex())) / TLV.E());
	}
  return activity;
 }

bool EventHandler::JetOverlaps(int jet, double ipt, double ieta, double iphi) const {
  if(fabs(jets_AKPF_pt->at(jet) - ipt ) / ipt <1. && Math::dR(jets_AKPF_eta->at(jet), ieta, jets_AKPF_phi->at(jet), iphi)<JetConeSize_ ) return true;
  else return false;
}

bool EventHandler::TrackOverlaps(int tk, double ipt, double ieta, double iphi) const {
  if(fabs(pfcand_pt->at(tk) - ipt ) / ipt <1. && Math::dR(pfcand_eta->at(tk), ieta, pfcand_phi->at(tk), iphi)<0.1) return true;
  else return false;
}


