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
#include "pu_constants.hpp"
#include "lumi_reweighting_stand_alone.hpp"
#include "event_number.hpp"
#include "b_jet.hpp"
#include "fat_jet.hpp"
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


EventHandler::EventHandler(const std::string &fileName, const bool isList, const double scaleFactorIn, const bool fastMode):
  cfA(fileName, isList),
  sortedBJetCache(0),
  bJetsUpToDate(false),
  sortedFatJetCache(0),
  FatJetsUpToDate(false),
  genMuonCache(0),
  genMuonsUpToDate(false),
  genElectronCache(0),
  genElectronsUpToDate(false),
  genTauCache(0),
  genTausUpToDate(false),
  recoMuonCache(0),
  recoMuonsUpToDate(false),
  recoElectronCache(0),
  recoElectronsUpToDate(false),
  recoTauCache(0),
  recoTausUpToDate(false), 
  betaUpToDate(false),
  scaleFactor(scaleFactorIn),
  beta(0),
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
}

void EventHandler::GetEntry(const unsigned int entry){
  cfA::GetEntry(entry);
  betaUpToDate=false;
  bJetsUpToDate=false;
  FatJetsUpToDate=false;
  genMuonsUpToDate=false;
  genElectronsUpToDate=false;
  genTausUpToDate=false;
  recoMuonsUpToDate=false;
  recoElectronsUpToDate=false;
  recoTausUpToDate=false;
}

unsigned EventHandler::TypeCode() const{
  // https://github.com/manuelfs/ra4_code/blob/master/src/event_handler.cpp#L606
  const TString sample_name = sampleName;
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

void EventHandler::GetSortedBJets() const{
  if(!bJetsUpToDate){
    sortedBJetCache.clear();
    for(unsigned int i(0); i<jets_AKPF_pt->size(); ++i){
      if(isGoodJet(i,true,20.)){ //lower pt cut
	double btag_disc(0.);
	if (cfAVersion<77) btag_disc=jets_AKPF_btag_secVertexCombined->at(i);
	else btag_disc=jets_AKPF_btag_inc_secVertexCombined->at(i); 
	sortedBJetCache.push_back(BJet(TLorentzVector(jets_AKPF_px->at(i),jets_AKPF_py->at(i),jets_AKPF_pz->at(i),jets_AKPF_energy->at(i)),btag_disc,i,static_cast<unsigned int>(jets_AKPF_parton_Id->at(i)),static_cast<unsigned int>(jets_AKPF_parton_motherId->at(i))));
      }
    }
    std::sort(sortedBJetCache.begin(),sortedBJetCache.end(), std::greater<BJet>());
    // Key: we're sorting by pt now, not CSV
    bJetsUpToDate=true;
  }
}

void EventHandler::ClusterFatJets() const{
  if(!FatJetsUpToDate){
    sortedFatJetCache.clear();
    //      cout << "ClusterFatJets" << endl;
    vector<PseudoJet> fjets_skinny_30(0);
    vector<PseudoJet> skinny_jets_pt30;
    for(size_t jet = 0; jet<jets_AKPF_pt->size(); ++jet){
      // if(is_nan(jets_AKPF_px->at(jet)) || is_nan(jets_AKPF_py->at(jet))
      // 	  || is_nan(jets_AKPF_pz->at(jet)) || is_nan(jets_AKPF_energy->at(jet))) continue;
      const PseudoJet this_pj(jets_AKPF_px->at(jet), jets_AKPF_py->at(jet),
			      jets_AKPF_pz->at(jet), jets_AKPF_energy->at(jet));
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
      // double px = sortedFatJetCache[index].GetLorentzVector().Px();
      // double py = sortedFatJetCache[index].GetLorentzVector().Py();
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

double EventHandler::GetMHT(const double pt_cut, const double eta_cut) const {
  double px(0.), py(0.);
  for (uint ijet(0); ijet<jets_AKPF_pt->size(); ++ijet){
    if(isGoodJet(ijet,true,pt_cut,eta_cut)){
      px+=jets_AKPF_px->at(ijet);
      py+=jets_AKPF_py->at(ijet);
    }
  }
  return TMath::Sqrt(px*px+py*py);
}

double EventHandler::GetMHTPhi(const double pt_cut, const double eta_cut) const {
  double px(0.), py(0.);
  for (uint ijet(0); ijet<jets_AKPF_pt->size(); ++ijet){
    if(isGoodJet(ijet,true,pt_cut,eta_cut)){
      px+=jets_AKPF_px->at(ijet);
      py+=jets_AKPF_py->at(ijet);
    }
  }
  if (px!=0.) return TMath::ATan(py/px);
  else return TMath::Pi()/2.;
}

double EventHandler::GetHighestJetPt(unsigned int pos) const{
  GetSortedBJets();
  --pos;
  if(pos>=sortedBJetCache.size()){
    return -DBL_MAX;
  }else{
    return sortedBJetCache.at(pos).GetLorentzVector().Pt();
  }
}

int EventHandler::GetJetXIndex(unsigned int pos) const{
  GetSortedBJets();
  --pos;
  if(pos>=sortedBJetCache.size()){
    return -1;
  }else{
    return sortedBJetCache.at(pos).GetIndex();
  }
}

double EventHandler::GetJetXEta(unsigned int pos) const{
  GetSortedBJets();
  --pos;
  if(pos>=sortedBJetCache.size()){
    return -DBL_MAX;
  }else{
    return sortedBJetCache.at(pos).GetLorentzVector().Eta();
  }
}

double EventHandler::GetJetXPhi(unsigned int pos) const{
  GetSortedBJets();
  --pos;
  if(pos>=sortedBJetCache.size()){
    return -DBL_MAX;
  }else{
    return sortedBJetCache.at(pos).GetLorentzVector().Phi();
  }
}

double EventHandler::GetJetXPartonId(unsigned int pos) const{
  GetSortedBJets();
  --pos;
  if(pos>=sortedBJetCache.size()){
    return -DBL_MAX;
  }else{
    return sortedBJetCache.at(pos).GetPartonId();
  }
}

double EventHandler::GetJetXMotherId(unsigned int pos) const{
  GetSortedBJets();
  --pos;
  if(pos>=sortedBJetCache.size()){
    return -DBL_MAX;
  }else{
    return sortedBJetCache.at(pos).GetMotherId();
  }
}

double EventHandler::GetJetXCSV(unsigned int pos) const{
  GetSortedBJets();
  --pos;
  if(pos>=sortedBJetCache.size()){
    return -DBL_MAX;
  }else{
    return sortedBJetCache.at(pos).GetBTag();
  }
}

double EventHandler::GetHighestJetCSV(const unsigned int nth_highest) const{
  std::vector<double> csvs(0);
  for(unsigned int jet(0); jet<jets_AKPF_pt->size(); ++jet){
    if(isGoodJet(jet,true,20.)){
      if (cfAVersion<77) csvs.push_back(jets_AKPF_btag_secVertexCombined->at(jet));
      else csvs.push_back(jets_AKPF_btag_inc_secVertexCombined->at(jet));
    }
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
  if (!recoMuonsUpToDate) {
    if (cfAVersion>=75) recoMuonCache=GetRecoMuons(true);
    else recoMuonCache=GetRA2bMuons(true);
    recoMuonsUpToDate=true;
  }
  for (unsigned int mu(0); mu<recoMuonCache.size(); mu++) {
    int mujet = mus_jet_ind->at(recoMuonCache[mu]);
    if (ijet==mujet) return true;
  }
  // has an isolated (veto) electron
  if (!recoElectronsUpToDate) {
    if (cfAVersion>=75) recoElectronCache=GetRecoElectrons(true);
    else recoElectronCache=GetRA2bElectrons(true);
    recoElectronsUpToDate=true;
  }
  for (unsigned int el(0); el<recoElectronCache.size(); el++) {
    int eljet = els_jet_ind->at(recoElectronCache[el]);
    if (ijet==eljet) return true;
  }
  return false;
}

bool EventHandler::isInMuonCollection(const double eta, const double phi) const{
  if (!recoMuonsUpToDate) {
    if (cfAVersion>=75) recoMuonCache=GetRecoMuons(true);
    else recoMuonCache=GetRA2bMuons(true);
    recoMuonsUpToDate=true;
  }
  // cout << "Found " << recoMuonCache.size() << " muons." << endl;
  for (unsigned int mu(0); mu<recoMuonCache.size(); mu++) {
    double deltaR = Math::GetDeltaR(phi, eta, mus_phi->at(recoMuonCache[mu]), mus_eta->at(recoMuonCache[mu]));
    //  printf("Muon %d, DeltaR %3.2f\n",recoMuonCache[mu], deltaR);
    if (deltaR<0.4) {
      return true;
    }
  }
  return false;
}

bool EventHandler::isInElectronCollection(const double eta, const double phi) const{
  if (!recoElectronsUpToDate) {
    if (cfAVersion>=75) recoElectronCache=GetRecoElectrons(true);
    else recoElectronCache=GetRA2bElectrons(true);
    recoElectronsUpToDate=true;
  }
  for (unsigned int el(0); el<recoElectronCache.size(); el++) {
    double deltaR = Math::GetDeltaR(phi, eta, els_phi->at(recoElectronCache[el]), els_eta->at(recoElectronCache[el]));
    if (deltaR<0.4) {
      return true;
    }
  }
  return false;
}

bool EventHandler::isInTauCollection(const double eta, const double phi) const{
  if (!recoTausUpToDate) {
    recoTauCache=GetRecoTaus();
    recoTausUpToDate=true;
  }
  for (unsigned int tau(0); tau<recoTauCache.size(); tau++) {
    double deltaR = Math::GetDeltaR(phi, eta, taus_phi->at(recoTauCache[tau]), taus_eta->at(recoTauCache[tau]));
    if (deltaR<0.4) {
      return true;
    }
  }
  return false;
}

void EventHandler::GetBeta(const std::string which) const{
  betaUpToDate=true;

  //Clear out the vector before starting a new event!
  beta.clear();

  if (GetcfAVersion()<69){
    beta.resize(jets_AKPF_pt->size(), 0.0);
  }else{
    int totjet = 0;
    int matches = 0;
    for (unsigned int ijet=0; ijet<jets_AKPF_pt->size(); ++ijet) {
      const double pt = jets_AKPF_pt->at(ijet);
      const double eta = fabs(jets_AKPF_eta->at(ijet));
      
      int i = 0;
      totjet++;
      for (std::vector<std::vector<float> >::const_iterator itr = puJet_rejectionBeta->begin(); itr != puJet_rejectionBeta->end(); ++itr, ++i) {
        int j = 0;
        double mypt = 0;
        double myeta = 0;
        double mybeta = 0;
        double result = 0;
        double tmp1 = 0, tmp2 = 0, tmp3 = 0, tmp4 = 0;
        for ( std::vector<float>::const_iterator it = itr->begin(); it != itr->end(); ++it, ++j) {
          
          if ( (j%6)==0 ) mypt = *it;  
          if ( (j%6)==1 ) myeta = fabs(*it); 
          if ( (j%6)==2 ) tmp1 = *it;  
          if ( (j%6)==3 ) tmp2 = *it;  
          if ( (j%6)==4 ) tmp3 = *it;  
          if ( (j%6)==5 ) tmp4 = *it;  
          
          //if ( which == "beta" )                 result = tmp1; 
          //else if ( which == "betaStar" )        result = tmp2; 
          //else if ( which == "betaClassic" )     result = tmp3; 
          //else if ( which == "betaStarClassic" ) result = tmp4; 
          //else result = -5; //Don't assert..
          
          if ( which.compare("beta")==0 )                 result = tmp1; 
          else if ( which.compare("betaStar")==0 )        result = tmp2; 
          else if ( which.compare("betaClassic")==0 )     result = tmp3; 
          else if ( which.compare("betaStarClassic")==0 ) result = tmp4; 
          else result = -5; //Don't assert..
          
        }//vector of info of each jet
        if ( mypt == pt && myeta == eta ) {
          matches++;
          mybeta = result;
          beta.push_back(mybeta);
          break;
        }     
      }//vector of jets
    } //ijet
  }
}

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

// void EventHandler::SetFastJetCollection(const unsigned int pt_cut) const{
//   // pt cut is on the skinny jets we recluster;
//    fastjets_AKPF_R1p2_R0p5_px->clear();
//    fastjets_AKPF_R1p2_R0p5_py->clear();
//    fastjets_AKPF_R1p2_R0p5_pz->clear();
//    fastjets_AKPF_R1p2_R0p5_energy->clear();
//    fastjets_AKPF_R1p2_R0p5_phi->clear();
//    fastjets_AKPF_R1p2_R0p5_eta->clear();
//    fastjets_AKPF_R1p2_R0p5_index->clear();
//    fastjets_AKPF_R1p2_R0p5_nconstituents->clear();
//   switch (pt_cut) {
//   case 10:
//     fastjets_AKPF_R1p2_R0p5_px=fastjets_AKPF_R1p2_R0p5pT10_px;
//     fastjets_AKPF_R1p2_R0p5_py=fastjets_AKPF_R1p2_R0p5pT10_py;
//     fastjets_AKPF_R1p2_R0p5_pz=fastjets_AKPF_R1p2_R0p5pT10_pz;
//     fastjets_AKPF_R1p2_R0p5_energy=fastjets_AKPF_R1p2_R0p5pT10_energy;
//     fastjets_AKPF_R1p2_R0p5_phi=fastjets_AKPF_R1p2_R0p5pT10_phi;
//     fastjets_AKPF_R1p2_R0p5_eta=fastjets_AKPF_R1p2_R0p5pT10_eta;
//     fastjets_AKPF_R1p2_R0p5_index=fastjets_AKPF_R1p2_R0p5pT10_index;
//     fastjets_AKPF_R1p2_R0p5_nconstituents=fastjets_AKPF_R1p2_R0p5pT10_nconstituents;
//   case 15:
//     fastjets_AKPF_R1p2_R0p5_px=fastjets_AKPF_R1p2_R0p5pT15_px;
//     fastjets_AKPF_R1p2_R0p5_py=fastjets_AKPF_R1p2_R0p5pT15_py;
//     fastjets_AKPF_R1p2_R0p5_pz=fastjets_AKPF_R1p2_R0p5pT15_pz;
//     fastjets_AKPF_R1p2_R0p5_energy=fastjets_AKPF_R1p2_R0p5pT15_energy;
//     fastjets_AKPF_R1p2_R0p5_phi=fastjets_AKPF_R1p2_R0p5pT15_phi;
//     fastjets_AKPF_R1p2_R0p5_eta=fastjets_AKPF_R1p2_R0p5pT15_eta;
//     fastjets_AKPF_R1p2_R0p5_index=fastjets_AKPF_R1p2_R0p5pT15_index;
//     fastjets_AKPF_R1p2_R0p5_nconstituents=fastjets_AKPF_R1p2_R0p5pT15_nconstituents;
//   case 20:
//     fastjets_AKPF_R1p2_R0p5_px=fastjets_AKPF_R1p2_R0p5pT20_px;
//     fastjets_AKPF_R1p2_R0p5_py=fastjets_AKPF_R1p2_R0p5pT20_py;
//     fastjets_AKPF_R1p2_R0p5_pz=fastjets_AKPF_R1p2_R0p5pT20_pz;
//     fastjets_AKPF_R1p2_R0p5_energy=fastjets_AKPF_R1p2_R0p5pT20_energy;
//     fastjets_AKPF_R1p2_R0p5_phi=fastjets_AKPF_R1p2_R0p5pT20_phi;
//     fastjets_AKPF_R1p2_R0p5_eta=fastjets_AKPF_R1p2_R0p5pT20_eta;
//     fastjets_AKPF_R1p2_R0p5_index=fastjets_AKPF_R1p2_R0p5pT20_index;
//     fastjets_AKPF_R1p2_R0p5_nconstituents=fastjets_AKPF_R1p2_R0p5pT20_nconstituents;
//   case 25:
//     fastjets_AKPF_R1p2_R0p5_px=fastjets_AKPF_R1p2_R0p5pT25_px;
//     fastjets_AKPF_R1p2_R0p5_py=fastjets_AKPF_R1p2_R0p5pT25_py;
//     fastjets_AKPF_R1p2_R0p5_pz=fastjets_AKPF_R1p2_R0p5pT25_pz;
//     fastjets_AKPF_R1p2_R0p5_energy=fastjets_AKPF_R1p2_R0p5pT25_energy;
//     fastjets_AKPF_R1p2_R0p5_phi=fastjets_AKPF_R1p2_R0p5pT25_phi;
//     fastjets_AKPF_R1p2_R0p5_eta=fastjets_AKPF_R1p2_R0p5pT25_eta;
//     fastjets_AKPF_R1p2_R0p5_index=fastjets_AKPF_R1p2_R0p5pT25_index;
//     fastjets_AKPF_R1p2_R0p5_nconstituents=fastjets_AKPF_R1p2_R0p5pT25_nconstituents;
//   case 30: 
//     fastjets_AKPF_R1p2_R0p5_px=fastjets_AKPF_R1p2_R0p5pT30_px;
//     fastjets_AKPF_R1p2_R0p5_py=fastjets_AKPF_R1p2_R0p5pT30_py;
//     fastjets_AKPF_R1p2_R0p5_pz=fastjets_AKPF_R1p2_R0p5pT30_pz;
//     fastjets_AKPF_R1p2_R0p5_energy=fastjets_AKPF_R1p2_R0p5pT30_energy;
//     fastjets_AKPF_R1p2_R0p5_phi=fastjets_AKPF_R1p2_R0p5pT30_phi;
//     fastjets_AKPF_R1p2_R0p5_eta=fastjets_AKPF_R1p2_R0p5pT30_eta;
//     fastjets_AKPF_R1p2_R0p5_index=fastjets_AKPF_R1p2_R0p5pT30_index;
//     fastjets_AKPF_R1p2_R0p5_nconstituents=fastjets_AKPF_R1p2_R0p5pT30_nconstituents;
//     break;
//   default: // 30 GeV
//     fastjets_AKPF_R1p2_R0p5_px=fastjets_AKPF_R1p2_R0p5pT30_px;
//     fastjets_AKPF_R1p2_R0p5_py=fastjets_AKPF_R1p2_R0p5pT30_py;
//     fastjets_AKPF_R1p2_R0p5_pz=fastjets_AKPF_R1p2_R0p5pT30_pz;
//     fastjets_AKPF_R1p2_R0p5_energy=fastjets_AKPF_R1p2_R0p5pT30_energy;
//     fastjets_AKPF_R1p2_R0p5_phi=fastjets_AKPF_R1p2_R0p5pT30_phi;
//     fastjets_AKPF_R1p2_R0p5_eta=fastjets_AKPF_R1p2_R0p5pT30_eta;
//     fastjets_AKPF_R1p2_R0p5_index=fastjets_AKPF_R1p2_R0p5pT30_index;
//     fastjets_AKPF_R1p2_R0p5_nconstituents=fastjets_AKPF_R1p2_R0p5pT30_nconstituents;
//   }

// }

bool EventHandler::PassesPVCut() const{
  if(beamSpot_x->size()<1 || pv_x->size()<1) return false;
  const double pv_rho(sqrt(pv_x->at(0)*pv_x->at(0) + pv_y->at(0)*pv_y->at(0)));
  if(pv_ndof->at(0)>4 && fabs(pv_z->at(0))<24. && pv_rho<2.0 && pv_isFake->at(0)==0) return true;
  return false;
  return isGoodVertex(0);
}

bool EventHandler::Passes2012METCleaningCut() const{
  for(unsigned int jet(0); jet<jets_AKPF_pt->size(); ++jet){
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
  for(unsigned int jet(0); jet<jets_AKPF_pt->size(); ++jet){
    if(isProblemJet(jet)) return false;
  }
  //if(pfTypeImets_et->at(0)>2.0*pfmets_et->at(0)) return false;
  // if(pfTypeImets_et->at(0)>2.0*mets_AK5_et->at(0)) return false; // updated 11/14 (JB-F)
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
  return jets_AKPF_pt->at(ijet)>50.0
    && fabs(jets_AKPF_eta->at(ijet))>0.9
    && fabs(jets_AKPF_eta->at(ijet))<1.9
				     && jets_AKPF_chg_Mult->at(ijet)-jets_AKPF_neutral_Mult->at(ijet)>=40;
}

bool EventHandler::PassesBadJetFilter() const{
  for(unsigned int i(0); i<jets_AKPF_pt->size(); ++i){
    if(isGoodJet(i,false,30.0,DBL_MAX) && !isGoodJet(i,true,30.0,DBL_MAX)) return false;
  }
  return true;
}

int EventHandler::GetPBNR() const{
  //RA2 particle-based noise rejection                                                                                                                                           

  bool nhBad=false;
  bool phBad=false;
  for (unsigned int it = 0; it<jets_AKPF_pt->size(); it++) {
    //cfA version from Keith                                                                                                                                                     
    double NHF = jets_AKPF_neutralHadE->at(it)/(jets_AKPF_energy->at(it)*jets_AKPF_corrFactorRaw->at(it));
    double PEF = jets_AKPF_photonEnergy->at(it)/(jets_AKPF_energy->at(it)*jets_AKPF_corrFactorRaw->at(it));
    if (NHF > 0.9)  nhBad = true;
    if (PEF > 0.95) phBad = true;
  }

  if (nhBad && phBad) return -3;
  else if (phBad) return -2;
  else if (nhBad) return -1;
  return 1;
}

// isGoodJet(jet, false, 20.0, 5.0, false)
bool EventHandler::isGoodJet(const unsigned int ijet, const bool jetid, const double ptThresh, const double etaThresh/*, const bool doBeta*/) const{
  if(jets_AKPF_pt->at(ijet)<ptThresh || fabs(jets_AKPF_eta->at(ijet))>etaThresh) return false;
  if( jetid && !jetPassLooseID(ijet) ) return false;
  // if(!betaUpToDate) GetBeta();
  //  if(beta.at(ijet)<0.2 && doBeta) return false;
  if (cfAVersion>=75&&cfAVersion<=76) { // overlap removal
    if (jetHasEMu(ijet)) return false;
  }
  return true;
}

bool EventHandler::isCleanJet(const unsigned int ijet, const int pdgId) const{ // remove overlap, based on pfcand hypothesis
  double jet_eta(jets_AKPF_eta->at(ijet)), jet_phi(jets_AKPF_phi->at(ijet));
  for (unsigned int it = 0; it<pfcand_pt->size(); it++) {
    if (static_cast<int>(fabs(pfcand_pdgId->at(it)))!=pdgId) continue;
    double dR = Math::GetDeltaR(jet_phi, jet_eta, pfcand_phi->at(it), pfcand_eta->at(it));
    if (dR<=0.5) return false;
  }
  return true;
}

bool EventHandler::jetPassLooseID(const unsigned int ijet) const{
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

bool EventHandler::isGenMuon(const int index) const{
  if (fabs(mc_mus_id->at(index))!=13) return false;
  //if (mc_mus_pt->at(index)<3.) return false;
  //if (fabs(mc_mus_eta->at(index))>5.) return false;
  if (mc_mus_numOfDaughters->at(index)>0.) return false; // in the case of Brem, remove the inital muon
  return true;
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

bool EventHandler::isIsoGenMuon(const int index) const{
  if (fabs(mc_mus_id->at(index))!=13) return false;
  if (fabs(mc_mus_mother_id->at(index))==24) return true;
  if (fabs(mc_mus_id->at(index))==fabs(mc_mus_mother_id->at(index)) && fabs(mc_mus_grandmother_id->at(index))==24) return true;
  if (fabs(mc_mus_id->at(index))==fabs(mc_mus_mother_id->at(index)) && 
      fabs(mc_mus_mother_id->at(index))==fabs(mc_mus_grandmother_id->at(index)) &&
      fabs(mc_mus_ggrandmother_id->at(index))==24) return true;
  // For now, not counting W->tau->mu events
  return false;
}

std::pair <int, double> EventHandler::GetGenMuonMinDR(const int genLep, const vector<uint> matched) const {
  double gen_pt(genMuonCache[genLep].GetLorentzVector().Pt()), gen_eta(genMuonCache[genLep].GetLorentzVector().Eta()), gen_phi(genMuonCache[genLep].GetLorentzVector().Phi());
  //printf("genMu/pt/eta/phi: %d/%.2f/%.2f/%.2f\n",genMuonCache[genLep].GetMCMusIndex(),genMuonCache[genLep].GetLorentzVector().Pt(),gen_eta,gen_phi);
  double minDeltaR(FLT_MAX);
  //printf("nRecoObjects: %lu\n",reco_eta.size());
  unsigned int nReco(pf_mus_eta->size());
  //cout << "n_mus: " << nReco << endl;
  if (nReco==0) return std::make_pair(-1,-1.);
  int closest(-1);
  for(unsigned int reco(0); reco<nReco; ++reco){
    // printf("recoMu/pt/eta/phi: %d/%.2f/%.2f/%.2f\n",reco,pf_mus_pt->at(reco),pf_mus_eta->at(reco),pf_mus_phi->at(reco));
    //printf("minDR: %d/%.2f\n",closest,minDeltaR);
    //if (pf_mus_pt->at(reco)<3||fabs(pf_mus_eta->at(reco))>5) continue;
    bool already_matched(false);
    for (uint i(0); i<matched.size(); i++) {
      if (reco==matched[i]) {
	already_matched=true;
	break;
      }
    }
    if (already_matched) continue; // avoid double-counting
    const double thisDPt( fabs(gen_pt-pf_mus_pt->at(reco)) );
    if (thisDPt>10.) continue; // wouldn't want this one anyway
    const double thisDeltaR( Math::GetDeltaR(gen_phi, gen_eta, pf_mus_phi->at(reco), pf_mus_eta->at(reco)) );
    if(thisDeltaR<minDeltaR){
      minDeltaR=thisDeltaR;
      closest=reco;
    }
  }
  //printf("minDR: %d/%.2f\n",closest,minDeltaR);
  if (minDeltaR>1.||closest==-1) return std::make_pair(-1,-1.);
  return std::make_pair(closest,minDeltaR);
}

std::pair <int, double> EventHandler::GetGenMuonMinDPt(const int genLep, const vector<uint> matched) const {
  double gen_pt(genMuonCache[genLep].GetLorentzVector().Pt()), gen_eta(genMuonCache[genLep].GetLorentzVector().Eta()), gen_phi(genMuonCache[genLep].GetLorentzVector().Phi());
  double minDPt(FLT_MAX);
  //printf("nRecoObjects: %lu\n",reco_eta.size());
  int closest(-1);
  unsigned int nReco(pf_mus_eta->size());
  if (nReco==0) return std::make_pair(-1,-1.);
  for(unsigned int reco(0); reco<nReco; ++reco){
    //if (pf_mus_pt->at(reco)<3||fabs(pf_mus_eta->at(reco))>5) continue;
    bool already_matched(false);
    for (uint i(0); i<matched.size(); i++) {
      if (reco==matched[i]) {
	already_matched=true;
	break;
      }
    }
    if (already_matched) continue; // avoid double-counting
    const double thisDeltaR( Math::GetDeltaR(gen_phi, gen_eta, pf_mus_phi->at(reco), pf_mus_eta->at(reco)) );
    if (thisDeltaR>1.) continue; // wouldn't want this one anyway
    const double thisDPt( fabs(gen_pt-pf_mus_pt->at(reco)) );
    if(thisDPt<minDPt){
      minDPt=thisDPt;
      closest=reco;
    }
  }
  if (minDPt>10.||closest==-1) return std::make_pair(-1,-1.);
  return std::make_pair(closest,minDPt);
}

// CSA14 version, using mus_

std::pair <int, double> EventHandler::GetGenMuonMinDRCSA14(const int genLep, const vector<uint> matched) const {
  double gen_pt(genMuonCache[genLep].GetLorentzVector().Pt()), gen_eta(genMuonCache[genLep].GetLorentzVector().Eta()), gen_phi(genMuonCache[genLep].GetLorentzVector().Phi());
  //printf("genMu/pt/eta/phi: %d/%.2f/%.2f/%.2f\n",genMuonCache[genLep].GetMCMusIndex(),genMuonCache[genLep].GetLorentzVector().Pt(),gen_eta,gen_phi);
  double minDeltaR(FLT_MAX);
  //printf("nRecoObjects: %lu\n",reco_eta.size());
  unsigned int nReco(mus_eta->size());
  //cout << "n_mus: " << nReco << endl;
  if (nReco==0) return std::make_pair(-1,-1.);
  int closest(-1);
  for(unsigned int reco(0); reco<nReco; ++reco){
    // printf("recoMu/pt/eta/phi: %d/%.2f/%.2f/%.2f\n",reco,mus_pt->at(reco),mus_eta->at(reco),mus_phi->at(reco));
    //printf("minDR: %d/%.2f\n",closest,minDeltaR);
    //if (mus_pt->at(reco)<3||fabs(mus_eta->at(reco))>5) continue;
    bool already_matched(false);
    for (uint i(0); i<matched.size(); i++) {
      if (reco==matched[i]) {
	already_matched=true;
	break;
      }
    }
    if (already_matched) continue; // avoid double-counting
    const double thisDPt( fabs(gen_pt-mus_pt->at(reco)) );
    if (thisDPt>10.) continue; // wouldn't want this one anyway
    const double thisDeltaR( Math::GetDeltaR(gen_phi, gen_eta, mus_phi->at(reco), mus_eta->at(reco)) );
    if(thisDeltaR<minDeltaR){
      minDeltaR=thisDeltaR;
      closest=reco;
    }
  }
  //printf("minDR: %d/%.2f\n",closest,minDeltaR);
  if (minDeltaR>1.||closest==-1) return std::make_pair(-1,-1.);
  return std::make_pair(closest,minDeltaR);
}

std::pair <int, double> EventHandler::GetGenMuonMinDPtCSA14(const int genLep, const vector<uint> matched) const {
  double gen_pt(genMuonCache[genLep].GetLorentzVector().Pt()), gen_eta(genMuonCache[genLep].GetLorentzVector().Eta()), gen_phi(genMuonCache[genLep].GetLorentzVector().Phi());
  double minDPt(FLT_MAX);
  //printf("nRecoObjects: %lu\n",reco_eta.size());
  int closest(-1);
  unsigned int nReco(mus_eta->size());
  if (nReco==0) return std::make_pair(-1,-1.);
  for(unsigned int reco(0); reco<nReco; ++reco){
    //if (mus_pt->at(reco)<3||fabs(mus_eta->at(reco))>5) continue;
    bool already_matched(false);
    for (uint i(0); i<matched.size(); i++) {
      if (reco==matched[i]) {
	already_matched=true;
	break;
      }
    }
    if (already_matched) continue; // avoid double-counting
    const double thisDeltaR( Math::GetDeltaR(gen_phi, gen_eta, mus_phi->at(reco), mus_eta->at(reco)) );
    if (thisDeltaR>1.) continue; // wouldn't want this one anyway
    const double thisDPt( fabs(gen_pt-mus_pt->at(reco)) );
    if(thisDPt<minDPt){
      minDPt=thisDPt;
      closest=reco;
    }
  }
  if (minDPt>10.||closest==-1) return std::make_pair(-1,-1.);
  return std::make_pair(closest,minDPt);
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

float EventHandler::GetMinDRMuonJet(const int mu) const{
  float minDR(999.);
  float mu_eta(mus_eta->at(mu)), mu_phi(mus_phi->at(mu));
  if (cfAVersion<=71||cfAVersion==74) {
    mu_eta=pf_mus_eta->at(mu);
    mu_phi=pf_mus_phi->at(mu);
  }
  float thisDR(999.);
  for (int jet(0); jet<static_cast<int>(jets_AKPF_pt->size()); jet++) {
    //  if (fabs(GetJetGenId(jet))==13) continue;
    //  if (!isCleanJet(jet, 13)) continue; // trying to remove overlap
    if(isGoodJet(jet, true, 0.0, 5.0/*, false*/)) {
      thisDR=Math::GetDeltaR(mu_phi, mu_eta, jets_AKPF_phi->at(jet), jets_AKPF_eta->at(jet));
      //      cout << "jet " << jet << " DR " << thisDR << endl;
    }
    if (thisDR<minDR) minDR=thisDR;
  }
  //  cout << "minDR " << minDR << endl;
  return minDR;
}

void EventHandler::SetupGenMuons() const {
  //  std::cout << "GetGenMuons" << std::endl;
  GetGenMuons();
  //  std::cout << "Calculate dR and dPts" << std::endl;
  vector<uint> mus_matched;
  for(unsigned int genLep(0); genLep<genMuonCache.size(); ++genLep){
    int gen_index(genMuonCache[genLep].GetMCMusIndex());
    genMuonCache[genLep].SetIsIso(isIsoGenMuon(gen_index));
    //  cout << "SetTopPt" << endl;
    genMuonCache[genLep].SetTopPt(GetCorrespondingTopPt(static_cast<int>(mc_mus_charge->at(gen_index))));
    //  cout << "SetDeltaRWb" << endl;
    genMuonCache[genLep].SetDeltaRWb(GetCorrespondingDeltaRlb(gen_index));
    double gen_pt(genMuonCache[genLep].GetLorentzVector().Pt());
    double gen_eta(genMuonCache[genLep].GetLorentzVector().Eta());
    double gen_phi(genMuonCache[genLep].GetLorentzVector().Phi());
    std::pair <int,double> min_dR, min_dPt;
    if (cfAVersion<71||cfAVersion==74) {
      min_dR = GetGenMuonMinDR(genLep, mus_matched);
      min_dPt = GetGenMuonMinDPt(genLep, mus_matched);
    }
    else {
      min_dR = GetGenMuonMinDRCSA14(genLep, mus_matched);
      min_dPt = GetGenMuonMinDPtCSA14(genLep, mus_matched);
    }
    genMuonCache[genLep].SetMinDR(min_dR);
    genMuonCache[genLep].SetMinDPt(min_dPt);
    int dR_ind(genMuonCache[genLep].GetMinDR().first), dPt_ind(genMuonCache[genLep].GetMinDPt().first);
    //  cout << "dR_ind/dPt_ind: " << dR_ind << "/" << dPt_ind << endl;
    if (dR_ind==dPt_ind) genMuonCache[genLep].SetMusMatch(min_dR.first);
    else if (dR_ind<0&&dPt_ind>=0) genMuonCache[genLep].SetMusMatch(min_dPt.first);
    else if (dR_ind>=0&&dPt_ind<0) genMuonCache[genLep].SetMusMatch(min_dR.first);
    else {
      //cout << "Ambiguous case:" << endl;
      //cout << "MinDR: " << dR_ind << ", MinDPt: " << dPt_ind << endl;
      //cout << "JERR" << endl;
      double d1 = sqrt(pow(genMuonCache[genLep].GetMinDR().second/0.05,2)+pow(fabs(gen_pt-mus_pt->at(dR_ind))/0.5,2));
      //cout << "JERR1" << endl;
      double d2 = sqrt(pow(genMuonCache[genLep].GetMinDPt().second/0.5,2)+pow(Math::GetDeltaR(gen_phi, gen_eta, mus_phi->at(dPt_ind), mus_eta->at(dPt_ind))/0.05,2));
      d1<d2 ? genMuonCache[genLep].SetMusMatch(min_dR.first) : genMuonCache[genLep].SetMusMatch(min_dPt.first);
    } 
    if (genMuonCache[genLep].GetMusMatch()>=0) mus_matched.push_back(static_cast<uint>(genMuonCache[genLep].GetMusMatch()));
    int reco_index(genMuonCache[genLep].GetMusMatch());
    genMuonCache[genLep].SetIsVeto(0);
    if (reco_index>=0) {
      if (cfAVersion>=75) genMuonCache[genLep].SetIsVeto(isRecoMuon(reco_index, 0));
      else genMuonCache[genLep].SetIsVeto(isRA2bMuon(reco_index, 0));
    }
    //   printf("gen muon %d: mc/mus = %d/%d--is veto? %d\n", genLep, gen_index, reco_index, genMuonCache[genLep].IsVeto());
    //  cout << "Gen muon " << genMuonCache[genLep].GetMCMusIndex() << " matched to reco muon " << genMuonCache[genLep].GetMusMatch() << endl;
    // Determine if/where/why we lost the muon
    genMuonCache[genLep].SetLossCode(GetGenMuonLossCode(genLep));
  }    
}

int EventHandler::GetGenMuonLossCode(const int genLep) const {
  if (genMuonCache[genLep].GetMusMatch()>=0) return 0; // not lost
  if (mus_eta->size()==0) return 1; // didn't make it to mus
  else return -1;
}

void EventHandler::GetGenMuons() const{
  if (!genMuonsUpToDate) {
    genMuonCache.clear();
    for (unsigned int gen(0); gen<mc_mus_id->size(); gen++) {
      if(isGenMuon(gen)&&isIsoGenMuon(gen)) {
	// if(isGenMuon(gen)&&IsFromB(mc_mus_mother_id->at(gen), mc_mus_grandmother_id->at(gen), mc_mus_ggrandmother_id->at(gen))) {
	genMuonCache.push_back(GenMuon(TLorentzVector(mc_mus_px->at(gen),mc_mus_py->at(gen),mc_mus_pz->at(gen),mc_mus_energy->at(gen)),gen,static_cast<int>(mc_mus_id->at(gen)),static_cast<unsigned int>(mc_mus_mother_id->at(gen))));
      }
    }
    std::sort(genMuonCache.begin(),genMuonCache.end(), std::greater<GenMuon>());
    genMuonsUpToDate=true;
  }
}

int EventHandler::GetNumIgnoredGenMuons() const{
  int ignored(0);
  for (uint gen(0); gen<mc_mus_id->size(); gen++) {
    if(!isGenMuon(gen)||!isIsoGenMuon(gen)) ignored++;
  }
  return ignored;
}

int EventHandler::GetNumIgnoredGenElectrons() const{
  int ignored(0);
  for (uint gen(0); gen<mc_electrons_id->size(); gen++) {
    if(!isGenElectron(gen)||!isIsoGenElectron(gen)) ignored++;
  }
  return ignored;
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

bool EventHandler::isGenElectron(const int index) const{
  if (fabs(mc_electrons_id->at(index))!=11) return false;
  //if (mc_electrons_pt->at(index)<3.) return false;
  //if (fabs(mc_electrons_eta->at(index))>5.) return false;
  if (mc_electrons_numOfDaughters->at(index)>0.) return false; // in the case of Brem, remove the inital electron
  return true;
}

bool EventHandler::isIsoGenElectron(const int index) const{
  if (fabs(mc_electrons_id->at(index))!=11) return false;
  if (fabs(mc_electrons_mother_id->at(index))==24) return true;
  if (fabs(mc_electrons_id->at(index))==fabs(mc_electrons_mother_id->at(index)) && fabs(mc_electrons_grandmother_id->at(index))==24) return true;
  if (fabs(mc_electrons_id->at(index))==fabs(mc_electrons_mother_id->at(index)) && 
      fabs(mc_electrons_mother_id->at(index))==fabs(mc_electrons_grandmother_id->at(index)) &&
      fabs(mc_electrons_ggrandmother_id->at(index))==24) return true;
  // For now, not counting W->tau->e events
  return false;
}

std::pair <int, double> EventHandler::GetGenElectronMinDR(const int genLep, const vector<uint> matched) const {
  double gen_pt(genElectronCache[genLep].GetLorentzVector().Pt()), gen_eta(genElectronCache[genLep].GetLorentzVector().Eta()), gen_phi(genElectronCache[genLep].GetLorentzVector().Phi());
  //printf("genEl/pt/eta/phi: %d/%.2f/%.2f/%.2f\n",genElectronCache[genLep].GetMCElsIndex(),genElectronCache[genLep].GetLorentzVector().Pt(),gen_eta,gen_phi);
  double minDeltaR(FLT_MAX);
  //printf("nRecoObjects: %lu\n",reco_eta.size());
  unsigned int nReco(els_eta->size());
  //cout << "n_els: " << nReco << endl;
  if (nReco==0) return std::make_pair(-1,-1.);
  int closest(-1);
  for(unsigned int reco(0); reco<nReco; ++reco){
    // printf("recoEl/pt/eta/phi: %d/%.2f/%.2f/%.2f\n",reco,els_pt->at(reco),els_eta->at(reco),els_phi->at(reco));
    //printf("minDR: %d/%.2f\n",closest,minDeltaR);
    //if (els_pt->at(reco)<3||fabs(els_eta->at(reco))>5) continue;
    bool already_matched(false);
    for (uint i(0); i<matched.size(); i++) {
      if (reco==matched[i]) {
	already_matched=true;
	break;
      }
    }
    if (already_matched) continue; // avoid double-counting
    const double thisDPt( fabs(gen_pt-els_pt->at(reco)) );
    if (thisDPt>10.) continue; // wouldn't want this one anyway
    const double thisDeltaR( Math::GetDeltaR(gen_phi, gen_eta, els_phi->at(reco), els_eta->at(reco)) );
    if(thisDeltaR<minDeltaR){
      minDeltaR=thisDeltaR;
      closest=reco;
    }
  }
  //printf("minDR: %d/%.2f\n",closest,minDeltaR);
  if (minDeltaR>1.||closest==-1) return std::make_pair(-1,-1.);
  return std::make_pair(closest,minDeltaR);
}

std::pair <int, double> EventHandler::GetGenElectronMinDPt(const int genLep, const vector<uint> matched) const {
  double gen_pt(genElectronCache[genLep].GetLorentzVector().Pt()), gen_eta(genElectronCache[genLep].GetLorentzVector().Eta()), gen_phi(genElectronCache[genLep].GetLorentzVector().Phi());
  double minDPt(FLT_MAX);
  //printf("nRecoObjects: %lu\n",reco_eta.size());
  int closest(-1);
  unsigned int nReco(els_eta->size());
  if (nReco==0) return std::make_pair(-1,-1.);
  for(unsigned int reco(0); reco<nReco; ++reco){
    //if (els_pt->at(reco)<3||fabs(els_eta->at(reco))>5) continue;
    bool already_matched(false);
    for (uint i(0); i<matched.size(); i++) {
      if (reco==matched[i]) {
	already_matched=true;
	break;
      }
    }
    if (already_matched) continue; // avoid double-counting
    const double thisDeltaR( Math::GetDeltaR(gen_phi, gen_eta, els_phi->at(reco), els_eta->at(reco)) );
    if (thisDeltaR>1.) continue; // wouldn't want this one anyway
    const double thisDPt( fabs(gen_pt-els_pt->at(reco)) );
    if(thisDPt<minDPt){
      minDPt=thisDPt;
      closest=reco;
    }
  }
  if (minDPt>10.||closest==-1) return std::make_pair(-1,-1.);
  return std::make_pair(closest,minDPt);
}

float EventHandler::GetMinDRElectronJet(const int el) const{
  float minDR(999.);
  float el_eta(els_scEta->at(el)), el_phi(els_phi->at(el));
  float thisDR(999.);
  for (int jet(0); jet<static_cast<int>(jets_AKPF_pt->size()); jet++) {
    if (fabs(GetJetGenId(jet))==11) continue;
    if(isGoodJet(jet, true, 0.0, 5.0/*, false*/)) {
      thisDR=Math::GetDeltaR(el_phi, el_eta, jets_AKPF_phi->at(jet), jets_AKPF_eta->at(jet));
    }
    if (thisDR<minDR) minDR=thisDR;
  }
  return minDR;
}

int EventHandler::GetJetGenId(const int jet) const{
  float minDR(999.);
  int pdgID(-999);
  float jet_eta(jets_AKPF_eta->at(jet)), jet_phi(jets_AKPF_phi->at(jet));
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

void EventHandler::SetupGenElectrons() const {
  //std::cout << "GetGenElectrons" << std::endl;
  GetGenElectrons();
  //std::cout << "Calculate dR and dPts" << std::endl;
  vector<uint> els_matched;
  for(unsigned int genLep(0); genLep<genElectronCache.size(); ++genLep){
    int gen_index(genElectronCache[genLep].GetMCElsIndex());
    genElectronCache[genLep].SetIsIso(isIsoGenElectron(gen_index));
    genElectronCache[genLep].SetTopPt(GetCorrespondingTopPt(static_cast<int>(mc_electrons_charge->at(gen_index))));
    genElectronCache[genLep].SetTopPt(GetCorrespondingDeltaRWb(static_cast<int>(mc_electrons_charge->at(gen_index))));
    double gen_pt(genElectronCache[genLep].GetLorentzVector().Pt());
    double gen_eta(genElectronCache[genLep].GetLorentzVector().Eta());
    double gen_phi(genElectronCache[genLep].GetLorentzVector().Phi());
    std::pair <int,double> min_dR = GetGenElectronMinDR(genLep, els_matched);
    std::pair <int,double> min_dPt = GetGenElectronMinDPt(genLep, els_matched);
    genElectronCache[genLep].SetMinDR(min_dR);
    genElectronCache[genLep].SetMinDPt(min_dPt);
    int dR_ind(genElectronCache[genLep].GetMinDR().first), dPt_ind(genElectronCache[genLep].GetMinDPt().first);
    if (dR_ind==dPt_ind) genElectronCache[genLep].SetElsMatch(min_dR.first);
    else if (dR_ind<0&&dPt_ind>=0) genElectronCache[genLep].SetElsMatch(min_dPt.first);
    else if (dR_ind>=0&&dPt_ind<0) genElectronCache[genLep].SetElsMatch(min_dR.first);
    else {
      //  cout << "Ambiguous case:" << endl;
      //  cout << "MinDR: " << dR_ind << ", MinDPt: " << dPt_ind << endl;
      //cout << "JERR" << endl;
      double d1 = sqrt(pow(genElectronCache[genLep].GetMinDR().second/0.07,2)+pow(fabs(gen_pt-els_pt->at(dR_ind))/2.5,2));
      if (cfAVersion<=71||cfAVersion==74) d1 = sqrt(pow(genElectronCache[genLep].GetMinDR().second/0.07,2)+pow(fabs(gen_pt-pf_els_pt->at(dR_ind))/2.5,2));
      //cout << "JERR1" << endl;
      double d2 = sqrt(pow(genElectronCache[genLep].GetMinDPt().second/2.5,2)+pow(Math::GetDeltaR(gen_phi, gen_eta, els_phi->at(dPt_ind), els_eta->at(dPt_ind))/0.07,2));
      if (cfAVersion<=71||cfAVersion==74) d2 = sqrt(pow(genElectronCache[genLep].GetMinDPt().second/2.5,2)+pow(Math::GetDeltaR(gen_phi, gen_eta, pf_els_phi->at(dPt_ind), pf_els_eta->at(dPt_ind))/0.07,2));
      d1<d2 ? genElectronCache[genLep].SetElsMatch(min_dR.first) : genElectronCache[genLep].SetElsMatch(min_dPt.first);
    } 
    if (genElectronCache[genLep].GetElsMatch()>=0) els_matched.push_back(static_cast<uint>(genElectronCache[genLep].GetElsMatch()));
    //  cout << "Gen electron " << genElectronCache[genLep].GetMCElsIndex() << " matched to reco electron " << genElectronCache[genLep].GetElsMatch() << endl;
    int reco_index(genElectronCache[genLep].GetElsMatch());
    genElectronCache[genLep].SetIsVeto(0);
    if (reco_index>=0) genElectronCache[genLep].SetIsVeto(isRecoElectron(reco_index, 0));
    // Determine if/where/why we lost the electron
    genElectronCache[genLep].SetLossCode(GetGenElectronLossCode(genLep));
  }    
}

int EventHandler::GetGenElectronLossCode(const int genLep) const {
  if (genElectronCache[genLep].GetElsMatch()>=0) return 0; // not lost
  if (els_eta->size()==0) return 1; // didn't make it to els
  else return -1;
}

void EventHandler::GetGenElectrons() const{
  if (!genElectronsUpToDate) {
    genElectronCache.clear();
    for (unsigned int gen(0); gen<mc_electrons_id->size(); gen++) {
      if(isGenElectron(gen)&&isIsoGenElectron(gen)) {
	//  if(isGenElectron(gen)&&IsFromB(mc_electrons_mother_id->at(gen), mc_electrons_grandmother_id->at(gen), mc_electrons_ggrandmother_id->at(gen))) {
	genElectronCache.push_back(GenElectron(TLorentzVector(mc_electrons_px->at(gen),mc_electrons_py->at(gen),mc_electrons_pz->at(gen),mc_electrons_energy->at(gen)),gen,static_cast<int>(mc_electrons_id->at(gen)),static_cast<unsigned int>(mc_electrons_mother_id->at(gen))));
      }
    }
    std::sort(genElectronCache.begin(),genElectronCache.end(), std::greater<GenElectron>());
    genElectronsUpToDate=true;
  }
}

bool EventHandler::isGenTau(const int index) const{
  if (fabs(mc_taus_id->at(index))!=15) return false;
  return true;
}

std::pair <int, double> EventHandler::GetGenTauMinDR(const int genLep, const vector<uint> matched) const {
  double gen_eta(genTauCache[genLep].GetLorentzVector().Eta()), gen_phi(genTauCache[genLep].GetLorentzVector().Phi());
  //printf("genTau/pt/eta/phi: %d/%.2f/%.2f/%.2f\n",genTauCache[genLep].GetMCTausIndex(),genTauCache[genLep].GetLorentzVector().Pt(),gen_eta,gen_phi);
  double minDeltaR(FLT_MAX);
  //printf("nRecoObjects: %lu\n",reco_eta.size());
  unsigned int nReco(taus_eta->size());
  //cout << "n_taus: " << nReco << endl;
  if (nReco==0) return std::make_pair(-1,-1.);
  int closest(-1);
  for(unsigned int reco(0); reco<nReco; ++reco){
    // printf("recoTau/pt/eta/phi: %d/%.2f/%.2f/%.2f\n",reco,taus_pt->at(reco),taus_eta->at(reco),taus_phi->at(reco));
    //printf("minDR: %d/%.2f\n",closest,minDeltaR);
    //if (taus_pt->at(reco)<3||fabs(taus_eta->at(reco))>5) continue;
    bool already_matched(false);
    for (uint i(0); i<matched.size(); i++) {
      if (reco==matched[i]) {
	already_matched=true;
	break;
      }
    }
    if (already_matched) continue; // avoid double-counting
    const double thisDeltaR( Math::GetDeltaR(gen_phi, gen_eta, taus_phi->at(reco), taus_eta->at(reco)) );
    if(thisDeltaR<minDeltaR){
      minDeltaR=thisDeltaR;
      closest=reco;
    }
  }
  //printf("minDR: %d/%.2f\n",closest,minDeltaR);
  if (minDeltaR>2.||closest==-1) return std::make_pair(-1,-1.);
  return std::make_pair(closest,minDeltaR);
}

std::pair <int, double> EventHandler::GetGenTauMinDPt(const int genLep, const vector<uint> matched) const {
  double gen_pt(genTauCache[genLep].GetLorentzVector().Pt());
  double minDPt(FLT_MAX);
  //printf("nRecoObjects: %lu\n",reco_eta.size());
  int closest(-1);
  unsigned int nReco(taus_eta->size());
  if (nReco==0) return std::make_pair(-1,-1.);
  for(unsigned int reco(0); reco<nReco; ++reco){
    //if (taus_pt->at(reco)<3||fabs(taus_eta->at(reco))>5) continue;
    bool already_matched(false);
    for (uint i(0); i<matched.size(); i++) {
      if (reco==matched[i]) {
	already_matched=true;
	break;
      }
    }
    if (already_matched) continue; // avoid double-counting
    const double thisDPt( fabs(gen_pt-taus_pt->at(reco)) );
    if(thisDPt<minDPt){
      minDPt=thisDPt;
      closest=reco;
    }
  }
  if (minDPt>20.||closest==-1) return std::make_pair(-1,-1.);
  return std::make_pair(closest,minDPt);
}

void EventHandler::SetupGenTaus() const {
  //std::cout << "GetGenTaus" << std::endl;
  GetGenTaus();
  // std::cout << "Calculate dR and dPts" << std::endl;
  vector<uint> taus_matched;
  for(unsigned int genLep(0); genLep<genTauCache.size(); ++genLep){
    double gen_pt(genTauCache[genLep].GetLorentzVector().Pt());
    double gen_eta(genTauCache[genLep].GetLorentzVector().Eta());
    double gen_phi(genTauCache[genLep].GetLorentzVector().Phi());
    std::pair <int,double> min_dR = GetGenTauMinDR(genLep, taus_matched);
    std::pair <int,double> min_dPt = GetGenTauMinDPt(genLep, taus_matched);
    genTauCache[genLep].SetMinDR(min_dR);
    genTauCache[genLep].SetMinDPt(min_dPt);
    int dR_ind(genTauCache[genLep].GetMinDR().first), dPt_ind(genTauCache[genLep].GetMinDPt().first);
    if (dR_ind==dPt_ind) genTauCache[genLep].SetTausMatch(min_dR.first);
    else if (dR_ind<0&&dPt_ind>=0) genTauCache[genLep].SetTausMatch(min_dPt.first);
    else if (dR_ind>=0&&dPt_ind<0) genTauCache[genLep].SetTausMatch(min_dR.first);
    else {
      //    cout << "Ambiguous case:" << endl;
      //    cout << "MinDR: " << dR_ind << ", MinDPt: " << dPt_ind << endl;
      // cout << "JERR" << endl;
      double d1 = sqrt(pow(genTauCache[genLep].GetMinDR().second/0.2,2)+pow(fabs(gen_pt-taus_pt->at(dR_ind))/3.,2));
      // cout << "JERR1" << endl;
      double d2 = sqrt(pow(genTauCache[genLep].GetMinDPt().second/3.,2)+pow(Math::GetDeltaR(gen_phi, gen_eta, taus_phi->at(dPt_ind), taus_eta->at(dPt_ind))/0.2,2));
      d1<d2 ? genTauCache[genLep].SetTausMatch(min_dR.first) : genTauCache[genLep].SetTausMatch(min_dPt.first);
    } 
    if (genTauCache[genLep].GetTausMatch()>=0) taus_matched.push_back(static_cast<uint>(genTauCache[genLep].GetTausMatch()));
    // cout << "Gen tau " << genTauCache[genLep].GetMCTausIndex() << " matched to reco tau " << genTauCache[genLep].GetTausMatch() << " (" << genTauCache[genLep].GetMinDR().second << "/" << genTauCache[genLep].GetMinDPt().second << ")" << endl;
    // Determine if/where/why we lost the tau
    genTauCache[genLep].SetLossCode(GetGenTauLossCode(genLep));
  }    
}

int EventHandler::GetGenTauLossCode(const int genLep) const {
  if (genTauCache[genLep].GetTausMatch()>=0) return 0; // not lost
  if (taus_eta->size()==0) return 1; // didn't make it to taus
  else return -1;
}

void EventHandler::GetGenTaus() const{
  if (!genTausUpToDate) {
    genTauCache.clear();
    for (unsigned int gen(0); gen<mc_taus_id->size(); gen++) {
      if(isGenTau(gen)) {
	genTauCache.push_back(GenTau(TLorentzVector(mc_taus_px->at(gen),mc_taus_py->at(gen),mc_taus_pz->at(gen),mc_taus_energy->at(gen)),gen,static_cast<int>(mc_taus_id->at(gen)),static_cast<unsigned int>(mc_taus_mother_id->at(gen))));
      }
    }
    std::sort(genTauCache.begin(),genTauCache.end(), std::greater<GenTau>());
    genTausUpToDate=true;
  }
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

vector<int> EventHandler::GetRecoMuons(const bool veto, const bool iso2D, const bool mT_cut, const bool orth) const{
  vector<int> muons;
  for(uint index=0; index<mus_pt->size(); index++)
    if(veto){
      if(isRecoMuon(index, 0, iso2D, mT_cut, orth)) muons.push_back(index);
    }	else {
      if(isRecoMuon(index, 1, iso2D, mT_cut, orth)) muons.push_back(index);
    }
  return muons;
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

double EventHandler::getDZ(double vx, double vy, double vz, double px, double py, double pz, int firstGoodVertex){
  return vz - pv_z->at(firstGoodVertex) -((vx-pv_x->at(firstGoodVertex))*px+(vy-pv_y->at(firstGoodVertex))*py)*pz/(px*px+py*py); 
}

// int EventHandler::GetNumRecoMuons

bool EventHandler::isRecoMuon(const uint imu, const uint level, const bool iso2D, const bool mT_cut, const bool orth) const{
  if (imu>mus_pt->size()) return false;
  double pt_thresh(10.0);
  if(level>=1) pt_thresh=20.0;
  double eta_thresh(2.4);
  //  if(level>=1) eta_thresh=2.4;

  if (fabs(mus_eta->at(imu)) >= eta_thresh ) return false;
  if (mus_pt->at(imu) < pt_thresh) return false; 
  if ( !mus_id_GlobalMuonPromptTight->at(imu)) return false;
  // GlobalMuonPromptTight includes: isGlobal, globalTrack()->normalizedChi2() < 10, numberOfValidMuonHits() > 0
  if ( mus_numberOfMatchedStations->at(imu) <= 1 ) return false;
  const double pvx (pv_x->at(0)), pvy(pv_y->at(0));   
  const double d0 = mus_tk_d0dum->at(imu)-pvx*sin(mus_tk_phi->at(imu))+pvy*cos(mus_tk_phi->at(imu));
  const double mus_vz = mus_tk_vz->at(imu);
  const double mus_dz_vtx = fabs(mus_vz-pv_z->at(0));
  if (fabs(d0)>=0.2 || mus_dz_vtx>=0.5) return false;
  if ( !mus_tk_numvalPixelhits->at(imu)) return false;
  if ( mus_tk_LayersWithMeasurement->at(imu) <= 5 ) return false;

  

  // need to make sure both collections have the "slimmed cuts"
  // see https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookMiniAOD

  if (cfAVersion>=75&&!mus_isPF->at(imu)) return false;
  // if (!(mus_pt->at(imu)>5.0 ||
  // 	( mus_pt->at(imu)>3.0 && (mus_isPFMuon->at(imu)||mus_id_TrackerMuonArbitrated->at(imu)||mus_id_AllStandAloneMuons->at(imu)||mus_id_AllGlobalMuons->at(imu)) ) ||
  // 	mus_isPFMuon->at(imu))) return false;
  
  if (iso2D) {
    TLorentzVector nearestJet(GetNearestJet(TLorentzVector(mus_px->at(imu),mus_py->at(imu),mus_pz->at(imu),mus_energy->at(imu)),mus_jet_ind->at(imu)));
    if (nearestJet.Pt()>0)
      {
        TLorentzVector muVec;
        muVec.SetPtEtaPhiM(mus_pt->at(imu),mus_eta->at(imu),mus_phi->at(imu),mus_energy->at(imu));
        double ptRel = TMath::Abs(muVec.Perp(nearestJet.Vect()));
        double minDR = Math::GetDeltaR(mus_phi->at(imu), mus_eta->at(imu), nearestJet.Phi(), nearestJet.Eta());
	if (ptRel>25 || minDR>0.4) return true;
        else return false;
      } else return true;
  } else if (GetMuonRelIso(imu) > 0.2) return false;

  double mT=GetMTW(mus_pt->at(imu), pfTypeImets_et->at(0), mus_phi->at(imu), pfTypeImets_phi->at(0));
  bool orthRA4(mus_pt->at(imu)<20||GetMuonRelIso(imu)>0.12);
  if (mT_cut&&mT>100) {
    if (orth&&!orthRA4) return true;
    else return false;
  }

  return true;
}

double EventHandler::GetMuonRelIso(const unsigned int imu) const{
  double isoNeutral(mus_pfIsolationR04_sumNeutralHadronEt->at(imu) + mus_pfIsolationR04_sumPhotonEt->at(imu) - 0.5*mus_pfIsolationR04_sumPUPt->at(imu));
  if(isoNeutral<0.0) isoNeutral=0.0;
  return (mus_pfIsolationR04_sumChargedHadronPt->at(imu) + isoNeutral) / mus_pt->at(imu);
}

vector<int> EventHandler::GetRecoTaus() const{
  vector<int> taus;
  for(uint index=0; index<taus_pt->size(); index++)
    if(isRA2bTau(index)) taus.push_back(index);
  return taus;
}

vector<int> EventHandler::GetRecoElectrons(const bool veto/*, const bool iso2D, const bool mT_cut, const bool orth*/) const{
  vector<int> electrons;
  for(uint index=0; index<els_pt->size(); index++)
    if(!veto){
      if(isRecoElectron(index,3/*,iso2D,mT_cut,orth*/)) electrons.push_back(index);
    }	else {
      if(isRecoElectron(index,0/*,iso2D,mT_cut,orth*/)) electrons.push_back(index);
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

bool EventHandler::isRecoElectron(const uint iel, const uint level/*, const bool iso2D, const bool mT_cut, const bool orth*/) const{

  if(iel>=els_pt->size()) return false;
  bool barrel;
  if(fabs(els_scEta->at(iel))<=1.479){
    barrel = true;
  }else if(fabs(els_scEta->at(iel))<2.5){
    barrel = false;
  }else{
    return false;
  }

  double pt_cut, deta_cut, dphi_cut, ieta_cut, hovere_cut, d0_cut, dz_cut,
    ooeminusoop_cut, reliso_cut, vprob_cut, misshits_cut;

  if(cfAVersion<75){
    //See https://twiki.cern.ch/twiki/bin/viewauth/CMS/EgammaCutBasedIdentification#Barrel_Cuts_eta_supercluster_1_4
    const bool high_pt = els_pt->at(iel)>20.0;
    switch(level){
    default:
    case 0:
      pt_cut            = 10.;
      deta_cut        = barrel ? 0.007   : 0.01;
      dphi_cut        = barrel ? 0.8     : 0.7;
      ieta_cut        = barrel ? 0.01    : 0.03;
      hovere_cut      = barrel ? 0.15    : DBL_MAX;
      d0_cut          = barrel ? 0.04    : 0.04;
      dz_cut          = barrel ? 0.2     : 0.2;
      ooeminusoop_cut = barrel ? DBL_MAX  : DBL_MAX;
      reliso_cut      = barrel ? 0.15    : 0.15;
      vprob_cut       = barrel ? DBL_MAX  : DBL_MAX;
      misshits_cut    = barrel ? DBL_MAX  : DBL_MAX;
      break;
    case 1:
       pt_cut            = 20.;
     deta_cut        = barrel ? 0.007   : 0.009;
      dphi_cut        = barrel ? 0.15    : 0.10;
      ieta_cut        = barrel ? 0.01    : 0.03;
      hovere_cut      = barrel ? 0.12    : 0.10;
      d0_cut          = barrel ? 0.02    : 0.02;
      dz_cut          = barrel ? 0.2     : 0.2;
      ooeminusoop_cut = barrel ? 0.05    : 0.05;
      reliso_cut      = barrel ? 0.15    : (high_pt ? 0.15 : 0.10);
      vprob_cut       = barrel ? 1.e-6   : 1.e-6;
      misshits_cut    = barrel ? 1       : 1;
      break;
    case 2:
      pt_cut            = 20.;
      deta_cut        = barrel ? 0.004   : 0.007;
      dphi_cut        = barrel ? 0.06    : 0.03;
      ieta_cut        = barrel ? 0.01    : 0.03;
      hovere_cut      = barrel ? 0.12    : 0.1;
      d0_cut          = barrel ? 0.02    : 0.02;
      dz_cut          = barrel ? 0.1     : 0.1;
      ooeminusoop_cut = barrel ? 0.05    : 0.05;
      reliso_cut      = barrel ? 0.15    : (high_pt ? 0.15 : 0.10);
      vprob_cut       = barrel ? 1.e-6   : 1.e-6;
      misshits_cut    = barrel ? 1       : 1;
      break;
    case 3:
      pt_cut            = 20.;
      deta_cut        = barrel ? 0.004   : 0.005;
      dphi_cut        = barrel ? 0.03    : 0.02;
      ieta_cut        = barrel ? 0.01    : 0.03;
      hovere_cut      = barrel ? 0.12    : 0.10;
      d0_cut          = barrel ? 0.02    : 0.02;
      dz_cut          = barrel ? 0.1     : 0.1;
      ooeminusoop_cut = barrel ? 0.05    : 0.05;
      reliso_cut      = barrel ? 0.10    : (high_pt ? 0.10 : 0.07);
      vprob_cut       = barrel ? 1.e-6   : 1.e-6;
      misshits_cut    = barrel ? 1       : 0;
      break;
    }
  }else{
    //See https://twiki.cern.ch/twiki/bin/viewauth/CMS/CutBasedElectronIdentificationRun2#CSA14_selection_conditions_25ns
    switch(level){
    default:
    case 0:
      pt_cut            = 10.;
      deta_cut        = barrel ? 0.02    : 0.0141;
      dphi_cut        = barrel ? 0.2579  : 0.2591;
      ieta_cut        = barrel ? 0.0125  : 0.0371;
      hovere_cut      = barrel ? 0.2564  : 0.1335;
      d0_cut          = barrel ? 0.025   : 0.2232;
      dz_cut          = barrel ? 0.5863  : 0.9513;
      ooeminusoop_cut = barrel ? 0.1508  : 0.1542;
      reliso_cut      = barrel ? 0.3313  : 0.3816;
      vprob_cut       = barrel ? 1.e-6   : 1.e-6;
      misshits_cut    = barrel ? 2       : 3;
      break;
    case 1:
      pt_cut            = 20.;
      deta_cut        = barrel ? 0.0181  : 0.0124;
      dphi_cut        = barrel ? 0.0936  : 0.0624;
      ieta_cut        = barrel ? 0.0123  : 0.035;
      hovere_cut      = barrel ? 0.141   : 0.1115;
      d0_cut          = barrel ? 0.0166  : 0.098;
      dz_cut          = barrel ? 0.54342 : 0.9187;
      ooeminusoop_cut = barrel ? 0.1353  : 0.1443;
      reliso_cut      = barrel ? 0.24    : 0.3529;
      vprob_cut       = barrel ? 1.e-6   : 1.e-6;
      misshits_cut    = barrel ? 1       : 1;
      break;
    case 2:
      pt_cut            = 20.;
      deta_cut        = barrel ? 0.0106  : 0.0108;
      dphi_cut        = barrel ? 0.0323  : 0.0455;
      ieta_cut        = barrel ? 0.0107  : 0.0318;
      hovere_cut      = barrel ? 0.067   : 0.097;
      d0_cut          = barrel ? 0.0131  : 0.0845;
      dz_cut          = barrel ? 0.22310 : 0.7523;
      ooeminusoop_cut = barrel ? 0.1043  : 0.1201;
      reliso_cut      = barrel ? 0.2179  : 0.254;
      vprob_cut       = barrel ? 1.e-6   : 1.e-6;
      misshits_cut    = barrel ? 1       : 1;
      break;
    case 3:
      pt_cut            = 20.;
      deta_cut        = barrel ? 0.0091  : 0.0106;
      dphi_cut        = barrel ? 0.031   : 0.0359;
      ieta_cut        = barrel ? 0.0106  : 0.0305;
      hovere_cut      = barrel ? 0.0532  : 0.0835;
      d0_cut          = barrel ? 0.0126  : 0.0163;
      dz_cut          = barrel ? 0.0116  : 0.5999;
      ooeminusoop_cut = barrel ? 0.0609  : 0.1126;
      reliso_cut      = barrel ? 0.1649  : 0.2075;
      vprob_cut       = barrel ? 1.e-6   : 1.e-6;
      misshits_cut    = barrel ? 1       : 1;
      break;
    }
  }

  const double d0 = els_d0dum->at(iel)
    -pv_x->at(0)*sin(els_tk_phi->at(iel))
    +pv_y->at(0)*cos(els_tk_phi->at(iel));
  const double dz = fabs(els_vz->at(iel)-pv_z->at(0));
  const double sigietaieta = (cfAVersion>=75
                              ? els_full5x5_sigmaIetaIeta->at(iel)
                              : els_sigmaIEtaIEta->at(iel));

  return els_pt->at(iel)>pt_cut
    && deta_cut > fabs(els_dEtaIn->at(iel))
    && dphi_cut > fabs(els_dPhiIn->at(iel))
    && ieta_cut > sigietaieta
    && hovere_cut > els_hadOverEm->at(iel)
    && d0_cut > fabs(d0)
    && dz_cut > fabs(dz)
    && ooeminusoop_cut > fabs((1.0-els_eOverPIn->at(iel))/els_caloEnergy->at(iel))
    && (reliso_cut>GetCSAElectronIsolation(iel))
    && (true || vprob_cut)
    && (els_PATpassConversionVeto->at(iel))
    && (misshits_cut >= els_expectedMissingInnerHits->at(iel));
  
  // JACK'S CODE

  //N.B.: cut does not have the fabs(1/E-1/p) / ooEmooP and conversion rejection cuts from the EGamma POG!!!
  /*
  if (iel>els_pt->size()) return false;
  const double dmax(std::numeric_limits<double>::max());
  double pt_cut(10.0); //Not actually part of the EGamma ID
  double dEtaIn_cut(0.007), phi_cut(0.8), sigmaietaieta_cut(0.01), h_over_e_cut(0.15), d0_cut(0.04), dz_cut(0.2), iso_cut(0.15);

  switch(level){
  case 1:
    pt_cut=20.0;
    if(fabs(els_scEta->at(iel))<=1.479){
      dEtaIn_cut=0.007; phi_cut=0.15; sigmaietaieta_cut=0.01; h_over_e_cut=0.12;
      d0_cut=0.02; dz_cut=0.2; iso_cut=0.15;
    }else if(fabs(els_scEta->at(iel))>1.479&&fabs(els_scEta->at(iel))<2.5){
      dEtaIn_cut=0.009; phi_cut=0.10; sigmaietaieta_cut=0.03; h_over_e_cut=0.1;
      d0_cut=0.02; dz_cut=0.2; iso_cut=(els_pt->at(iel)>20.0?0.15:0.10);
    }
    break;
  case 2:
    pt_cut=20.0;
    if(fabs(els_scEta->at(iel))<=1.479){
      dEtaIn_cut=0.004; phi_cut=0.06; sigmaietaieta_cut=0.01; h_over_e_cut=0.12;
      d0_cut=0.02; dz_cut=0.1; iso_cut=0.15;
    }else if(fabs(els_scEta->at(iel))>1.479&&fabs(els_scEta->at(iel))<2.5){
      dEtaIn_cut=0.007; phi_cut=0.03; sigmaietaieta_cut=0.03; h_over_e_cut=0.1;
      d0_cut=0.02; dz_cut=0.1; iso_cut=(els_pt->at(iel)>20.0?0.15:0.10);
    }
    break;
  case 3:
    pt_cut=20.0;
    if(fabs(els_scEta->at(iel))<=1.479){

      dEtaIn_cut=0.004; phi_cut=0.03; sigmaietaieta_cut=0.01; h_over_e_cut=0.12;
      d0_cut=0.02; dz_cut=0.1; iso_cut=0.10;
      if (cfAVersion>=75) {
	dEtaIn_cut=0.009; iso_cut=0.18;
      }
    }else if(fabs(els_scEta->at(iel))>1.479&&fabs(els_scEta->at(iel))<2.5){
      dEtaIn_cut=0.005; phi_cut=0.02; sigmaietaieta_cut=0.03; h_over_e_cut=0.1;
      d0_cut=0.02; dz_cut=0.1; iso_cut=(els_pt->at(iel)>20.0?0.10:0.07);
      if (cfAVersion>=75) {
	dEtaIn_cut=0.01; sigmaietaieta_cut=0.031;
	h_over_e_cut=0.12; iso_cut=0.16;
      }    
    }
    break;
  case 0: //intentionally falling through to default "veto" case
  default:
    pt_cut=10.0;
    if(fabs(els_scEta->at(iel))<=1.479){ // barrel
      dEtaIn_cut=0.07; phi_cut=0.8; sigmaietaieta_cut=0.01; h_over_e_cut=0.15;
      d0_cut=0.04; dz_cut=0.2; iso_cut=0.15;
      if (cfAVersion>=75) {
	dEtaIn_cut=0.02; phi_cut=0.2579; sigmaietaieta_cut=0.0125; h_over_e_cut=0.2564;
	d0_cut=0.025; dz_cut=0.5863; iso_cut=0.3313;     
      }
    }else if(fabs(els_scEta->at(iel))>1.479&&fabs(els_scEta->at(iel))<2.5){ // endcap
      dEtaIn_cut=0.01; phi_cut=0.7; sigmaietaieta_cut=0.03; h_over_e_cut=dmax;
      d0_cut=0.04; dz_cut=0.2; iso_cut=0.15;
      if (cfAVersion>=75) {
	dEtaIn_cut=0.0141; phi_cut=0.2591; sigmaietaieta_cut=0.0371; h_over_e_cut=0.1335;
	d0_cut=0.2232; dz_cut=0.9513; iso_cut=0.3816; 
      }
    }
    break;
  }
  // if(k>else_pt->size()) return false;
  // if (level>0) {
  if (fabs(els_scEta->at(iel)) >= 2.5 ) return false;
  if (els_pt->at(iel) < pt_cut) return false;
  // }

  if ( fabs(els_dEtaIn->at(iel)) > dEtaIn_cut)  return false;
  if ( fabs(els_dPhiIn->at(iel)) > phi_cut)  return false;
  if ((cfAVersion<=71||cfAVersion==74) && els_sigmaIEtaIEta->at(iel) > sigmaietaieta_cut) return false;
  if (cfAVersion>=75 && els_full5x5_sigmaIetaIeta->at(iel) > sigmaietaieta_cut) return false;
  if (els_hadOverEm->at(iel) > h_over_e_cut) return false;

  const double beamx(beamSpot_x->at(0)), beamy(beamSpot_y->at(0)); 
  const double d0(els_d0dum->at(iel)-beamx*sin(els_tk_phi->at(iel))+beamy*cos(els_tk_phi->at(iel)));
  if ( fabs(d0) >= d0_cut ) return false;
  if ( fabs(els_vz->at(iel) - pv_z->at(0) ) >= dz_cut ) return false;
  if (iso2D) {
    TLorentzVector nearestJet(GetNearestJet(TLorentzVector(els_px->at(iel),els_py->at(iel),els_pz->at(iel),els_energy->at(iel)),els_jet_ind->at(iel)));
    if (nearestJet.Pt()>0)
      {
	TLorentzVector elVec;
	elVec.SetPtEtaPhiM(els_pt->at(iel),els_scEta->at(iel),els_phi->at(iel),els_energy->at(iel));
	double ptRel = TMath::Abs(elVec.Perp(nearestJet.Vect()));
	double minDR = Math::GetDeltaR(els_phi->at(iel), els_eta->at(iel), nearestJet.Phi(), nearestJet.Eta());
	if (ptRel>25 || minDR>0.4) return true;
	else return false;
      } else return true;
  } else{
    float rel_iso(999.);
    if (cfAVersion<=71||cfAVersion==74) rel_iso=GetElectronRelIso(iel);
    else rel_iso=GetCSAElectronIsolation(iel);
    if(rel_iso>=iso_cut) return false;
  }

  double mT=GetMTW(els_pt->at(iel), pfTypeImets_et->at(0), els_phi->at(iel), pfTypeImets_phi->at(0));
  bool orthRA4(els_pt->at(iel)<20||GetCSAElectronIsolation(iel)>0.15);
  if (mT_cut&&mT>100) {
    if (orth&&!orthRA4) return true;
    else return false;
  }

  return true;
  */
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
  for(unsigned int ijet(0); ijet<jets_AKPF_phi->size(); ++ijet){
    if(jets_AKPF_pt->at(ijet)<30.) continue;
    TLV.SetPxPyPzE(jets_AKPF_px->at(ijet), jets_AKPF_py->at(ijet), jets_AKPF_pz->at(ijet), jets_AKPF_energy->at(ijet));
    // printf("jet: px=%3.2f\tpy=%3.2f\tpz=%3.2f\tenergy=%3.2f\n",TLV.Px(),TLV.Py(),TLV.Pz(),TLV.E());
    if (jet_ind==ijet) {
      TLV-=lepton;
      // printf("jet(corr): px=%3.2f\tpy=%3.2f\tpz=%3.2f\tenergy=%3.2f\n",TLV.Px(),TLV.Py(),TLV.Pz(),TLV.E());
    }
    double DR=Math::GetDeltaR(lepton.Phi(), lepton.Eta(), TLV.Phi(), TLV.Eta());
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
  return sortedFatJetCache[index].GetLorentzVector().Pt();
}

double EventHandler::GetFatJetEta(const unsigned int index/*, const unsigned int pt_cut*/) const{
  GetSortedFatJets();
  if (index+1>sortedFatJetCache.size()) return -999;
  //  cout << "GetFatJetEta()" << endl;
  return sortedFatJetCache[index].GetLorentzVector().Eta();
}

double EventHandler::GetFatJetPhi(const unsigned int index/*, const unsigned int pt_cut*/) const{
  GetSortedFatJets();
  if (index+1>sortedFatJetCache.size()) return -999;
  return sortedFatJetCache[index].GetLorentzVector().Phi();
}

double EventHandler::GetFatJetEnergy(const unsigned int index/*, const unsigned int pt_cut*/) const{
  GetSortedFatJets();
  if (index+1>sortedFatJetCache.size()) return -1;
  return sortedFatJetCache[index].GetLorentzVector().E();
}

int EventHandler::GetFatJetnConst(const unsigned int index/*, const unsigned int pt_cut*/) const{
  GetSortedFatJets();
  if (index+1>sortedFatJetCache.size()) return -1;
  return sortedFatJetCache[index].GetNConst();
}

double EventHandler::GetFatJetmJ(const unsigned int index/*, const unsigned int pt_cut*/) const{
  GetSortedFatJets();
  if (index+1>sortedFatJetCache.size()) return -1;
  return sortedFatJetCache[index].GetLorentzVector().M();
}

int EventHandler::GetNFatJets(const double fat_jet_pt_cut, const double fat_jet_eta_cut/* , const unsigned int skinny_jet_pt_cut*/) const{
  GetSortedFatJets();
  uint nJ(0);
  for (uint ifj(0); ifj<sortedFatJetCache.size(); ifj++) {
    if (sortedFatJetCache[ifj].GetLorentzVector().Pt()>fat_jet_pt_cut&&fabs(sortedFatJetCache[ifj].GetLorentzVector().Eta())<fat_jet_eta_cut) nJ++;
  }
  return nJ;
}

double EventHandler::GetMJ(const double fat_jet_pt_cut, const double fat_jet_eta_cut/* , const unsigned int skinny_jet_pt_cut*/) const{
  GetSortedFatJets();
  double MJ(0.);
  for (uint ifj(0); ifj<sortedFatJetCache.size(); ifj++) {
    //  cout << "Fat jet " << ifj << ": mass = " << sortedFatJetCache[ifj].GetLorentzVector().M() << endl;
    if (sortedFatJetCache[ifj].GetLorentzVector().Pt()>fat_jet_pt_cut&&fabs(sortedFatJetCache[ifj].GetLorentzVector().Eta())<fat_jet_eta_cut)
      MJ+=sortedFatJetCache[ifj].GetLorentzVector().M();
  }
  return MJ;
}

double EventHandler::GetHT(const double pt_cut) const{
  double HT(0.0);
  for(unsigned int i(0); i<jets_AKPF_pt->size(); ++i){
    if(isGoodJet(i,true,pt_cut)) HT+=jets_AKPF_pt->at(i);
  }
  return HT;
}

double EventHandler::GetSumP(const double pt_cut) const{
  double px(0.0), py(0.0), pz(0.0), energy(0.0);
  for(unsigned int i(0); i<jets_AKPF_pt->size(); ++i){
    if(isGoodJet(i,true,pt_cut)) {
      px+=jets_AKPF_px->at(i);
      py+=jets_AKPF_py->at(i);
      pz+=jets_AKPF_pz->at(i);
      energy+=jets_AKPF_energy->at(i);
    }
  }
  TLorentzVector TLV(px,py,pz,energy);
  return TLV.M();
}

double EventHandler::GetCentrality(const double pt_cut) const{
  return GetHT(pt_cut)/GetSumP(pt_cut);
}

unsigned int EventHandler::GetNumGoodJets(const double pt) const{
  int numGoodJets(0);
  for(unsigned int i(0); i<jets_AKPF_pt->size(); ++i){
    if(isGoodJet(i,true,pt)) ++numGoodJets;
  }
  return numGoodJets;
}

unsigned int EventHandler::GetNumTruthMatchedBJets(const double pt, const bool good) const{
  int numBJets(0);
  for(unsigned int i(0); i<jets_AKPF_pt->size(); ++i){
    if(good&&!isGoodJet(i,true,pt)) continue;
    if (fabs(static_cast<int>(jets_AKPF_partonFlavour->at(i)))==5) numBJets++;
  }
  return numBJets;
}

unsigned int EventHandler::GetNumIncCSVTJets(const double pt_cut) const{
  int numPassing(0);
  for(unsigned int i(0); i<jets_AKPF_pt->size(); ++i){
    if(isGoodJet(i, true, pt_cut) && jets_AKPF_btag_inc_secVertexCombined->at(i)>ICSVTCut){
      ++numPassing;
    }
  }
  return numPassing;
}

unsigned int EventHandler::GetNumIncCSVMJets(const double pt_cut) const{
  int numPassing(0);
  for(unsigned int i(0); i<jets_AKPF_pt->size(); ++i){
    if(isGoodJet(i, true, pt_cut) && jets_AKPF_btag_inc_secVertexCombined->at(i)>ICSVMCut){
      ++numPassing;
    }
  }
  return numPassing;
}

unsigned int EventHandler::GetNumIncCSVLJets(const double pt_cut) const{
  int numPassing(0);
  for(unsigned int i(0); i<jets_AKPF_pt->size(); ++i){
    if(isGoodJet(i, true, pt_cut) && jets_AKPF_btag_inc_secVertexCombined->at(i)>ICSVLCut){
      ++numPassing;
    }
  }
  return numPassing;
}

unsigned int EventHandler::GetNumCSVTJets(const double pt_cut) const{
  int numPassing(0);
  for(unsigned int i(0); i<jets_AKPF_pt->size(); ++i){
    if(isGoodJet(i, true, pt_cut) && jets_AKPF_btag_secVertexCombined->at(i)>CSVTCut){
      ++numPassing;
    }
  }
  return numPassing;
}

unsigned int EventHandler::GetNumCSVMJets(const double pt_cut) const{
  int numPassing(0);
  for(unsigned int i(0); i<jets_AKPF_pt->size(); ++i){
    if(isGoodJet(i, true, pt_cut) && ((cfAVersion<=76&&jets_AKPF_btag_secVertexCombined->at(i)>CSVMCut)||(cfAVersion>=77&&jets_AKPF_btag_inc_secVertexCombined->at(i)>ICSVMCut))){
      ++numPassing;
    }
  }
  return numPassing;
}

unsigned int EventHandler::GetNumCSVLJets(const double pt_cut) const{
  int numPassing(0);
  for(unsigned int i(0); i<jets_AKPF_pt->size(); ++i){
    if(isGoodJet(i, true, pt_cut) && jets_AKPF_btag_secVertexCombined->at(i)>CSVLCut){
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
  GetSortedBJets();
  double min_mT(DBL_MAX);
  for (uint jet(0); jet<sortedBJetCache.size(); jet++) {
    if (sortedBJetCache[jet].GetLorentzVector().Pt()<pt) continue;
    if (sortedBJetCache[jet].GetBTag()<bTag) continue;
    double mT=GetMTWb(sortedBJetCache[jet].GetLorentzVector().Pt(), pfTypeImets_et->at(0), sortedBJetCache[jet].GetLorentzVector().Phi(), pfTypeImets_phi->at(0), use_W_mass);
    if (mT<min_mT) min_mT=mT;
  }
  if (min_mT==DBL_MAX) return -999.;
  else return min_mT;
}

double EventHandler::GetMinDeltaPhibMET(const double pt, const double bTag) const{
  GetSortedBJets();
  double min_dPhi(DBL_MAX);
  for (uint jet(0); jet<sortedBJetCache.size(); jet++) {
    if (sortedBJetCache[jet].GetLorentzVector().Pt()<pt) continue;
    if (sortedBJetCache[jet].GetBTag()<bTag) continue;
    double dPhi=Math::GetDeltaPhi(sortedBJetCache[jet].GetLorentzVector().Phi(), pfTypeImets_phi->at(0));
    if (dPhi<min_dPhi) min_dPhi=dPhi;
  }
  if (min_dPhi==DBL_MAX) return -999.;
  else return min_dPhi;
}


double EventHandler::Get2ndMTWb(const double pt, const double bTag, const bool use_W_mass) const{
  GetSortedBJets();
  double max_mT(-1.);
  for (uint jet(0); jet<sortedBJetCache.size(); jet++) {
    if (sortedBJetCache[jet].GetLorentzVector().Pt()<pt) continue;
    if (sortedBJetCache[jet].GetBTag()<bTag) continue;
    double mT=GetMTWb(sortedBJetCache[jet].GetLorentzVector().Pt(), pfTypeImets_et->at(0), sortedBJetCache[jet].GetLorentzVector().Phi(), pfTypeImets_phi->at(0), use_W_mass);
    if (mT>max_mT) max_mT=mT;
  }
  // now find 2nd
  double max2_mT(-1.); 
  for (uint jet(0); jet<sortedBJetCache.size(); jet++) {
    if (sortedBJetCache[jet].GetLorentzVector().Pt()<pt) continue;
    if (sortedBJetCache[jet].GetBTag()<bTag) continue;
    double mT=GetMTWb(sortedBJetCache[jet].GetLorentzVector().Pt(), pfTypeImets_et->at(0), sortedBJetCache[jet].GetLorentzVector().Phi(), pfTypeImets_phi->at(0), use_W_mass);
    if (mT==max_mT) continue;
    if (mT>max2_mT) max2_mT=mT;
  }
  return max2_mT;
}

double EventHandler::getDeltaPhiMETN(int goodJetI, float otherpt, float othereta, bool otherid, bool useArcsin) {
  if (goodJetI>static_cast<int>(jets_AKPF_phi->size())||goodJetI<0) return DBL_MAX;
  double deltaT = getDeltaPhiMETN_deltaT(goodJetI, otherpt, othereta, otherid);
  //calculate deltaPhiMETN
  double dp = fabs(Math::GetAbsDeltaPhi(jets_AKPF_phi->at(goodJetI), pfTypeImets_phi->at(0)));
  double dpN;
  if(useArcsin) {
    if( deltaT/pfTypeImets_et->at(0) >= 1.0) dpN = dp / (TMath::Pi()/2.0);
    else dpN = dp / asin(deltaT/pfTypeImets_et->at(0));
  }
  else dpN = dp / atan2(deltaT, pfTypeImets_et->at(0));
  return dpN;
}

double EventHandler::getDeltaPhiMETN_deltaT(unsigned int goodJetI, float otherpt, float othereta, bool otherid) {
  if(goodJetI==999999) return -99;
  //get sum for deltaT
  double sum = 0;
  for (unsigned int i=0; i< jets_AKPF_pt->size(); i++) {
    if(i==goodJetI) continue;
    if(isGoodJet(i, otherid, otherpt, othereta)){
      double jetres = 0.1;
      sum += pow( jetres*(jets_AKPF_px->at(goodJetI)*jets_AKPF_py->at(i) - jets_AKPF_py->at(goodJetI)*jets_AKPF_px->at(i)), 2);
    }//is good jet
  }//i
  double deltaT = sqrt(sum)/jets_AKPF_pt->at(goodJetI);
  return deltaT;
}

double EventHandler::getMinDeltaPhiMETN(unsigned int maxjets, float mainpt, float maineta, bool mainid,
					float otherpt, float othereta, bool otherid, bool useArcsin) 
{
  double mdpN=1E12;
  unsigned int nGoodJets(0);
  for (unsigned int i=0; i<jets_AKPF_pt->size(); i++) {
    if (!isGoodJet(i, mainid, mainpt, maineta)) continue;
    nGoodJets++;
    double dpN = getDeltaPhiMETN(i, otherpt, othereta, otherid, useArcsin);
    //i is for i'th *good* jet, starting at i=0. returns -99 if bad jet--but then i still increases by one
    // Jack --  might have fixed things above...
    if (dpN>=0&&dpN<mdpN) mdpN=dpN;
    if (nGoodJets>=maxjets) break;
  }
  return mdpN;
}

double EventHandler::GetMinDeltaPhiMET(const unsigned int maxjets, const double pt_cut, const double eta_cut) const{
  std::vector<std::pair<double, double> > jets(0);
  for(unsigned int i(0); i<jets_AKPF_phi->size(); ++i){
    if(isGoodJet(i, true, pt_cut, eta_cut)){
      jets.push_back(std::make_pair(jets_AKPF_pt->at(i),jets_AKPF_phi->at(i)));
    }
  }
  std::sort(jets.begin(), jets.end(), std::greater<std::pair<double, double> >());
  double mindp(DBL_MAX);
  for(unsigned int i(0); i<jets.size() && i<maxjets; ++i){
    const double thisdp(fabs((Math::GetAbsDeltaPhi(jets.at(i).second, pfTypeImets_phi->at(0)))));
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
	 ( !mT_cut || GetMTW(isotk_pt->at(itrack),pfTypeImets_et->at(0),isotk_phi->at(itrack),pfTypeImets_phi->at(0))<100 ) &&
	 ( fabs(isotk_eta->at(itrack)) < 2.4 ) //this is more of a sanity check
	 ){
      ++nisotracks;
    }
  }
  return nisotracks;
}

std::vector<std::pair<int,double> > EventHandler::GetIsoTracks(const double ptThresh, const bool mT_cut) const{
  std::vector<std::pair<int,double> > isotracks;
  for ( unsigned int itrack = 0 ; itrack < isotk_pt->size() ; ++itrack) {
    if ( (isotk_pt->at(itrack) >= ptThresh) &&
	 (isotk_iso->at(itrack) /isotk_pt->at(itrack) < 0.1 ) &&
	 ( fabs(isotk_dzpv->at(itrack)) <0.05) &&
	 ( !mT_cut || GetMTW(isotk_pt->at(itrack),pfTypeImets_et->at(0),isotk_phi->at(itrack),pfTypeImets_phi->at(0))<100 ) &&
	 ( fabs(isotk_eta->at(itrack)) < 2.4 ) //this is more of a sanity check
	 ){
      isotracks.push_back(std::make_pair(itrack, isotk_iso->at(itrack) /isotk_pt->at(itrack)));
    }
  }
  return isotracks;
}

double EventHandler::GetPFCandIsolation(const uint indexA) const { // absolute, not relative
  double isoSum(0);
  for (uint other(0); other<pfcand_pdgId->size(); other++) {
    if (other==indexA) continue; // don't count track in it's own isolation sum
    if (static_cast<int>(pfcand_charge->at(other))==0) continue; // only consider charged tracks
    if (fabs(pfcand_dz->at(other))>0.1) continue; // pileup suppression
    double deltaR = Math::GetDeltaR(pfcand_phi->at(indexA), pfcand_eta->at(indexA), pfcand_phi->at(other), pfcand_eta->at(other)); // isolation cone
    if (deltaR>0.3) continue;
    isoSum+=pfcand_pt->at(other);
  }
  return isoSum;
}

bool EventHandler::PassIsoTrackBaseline(const uint itk) const {
  if (static_cast<int>(pfcand_charge->at(itk))==0) return false;
  if (pfcand_pt->at(itk)<5) return false;
  if (fabs(pfcand_eta->at(itk))>2.5) return false;
  if (fabs(pfcand_dz->at(itk))>0.1) return false;
  if (static_cast<int>(pfcand_pdgId->at(itk))==0) return false;
  return true;
}

void EventHandler::NewGetIsoTracks(std::vector<std::pair<int,double> > &eCands, std::vector<std::pair<int,double> > &muCands, std::vector<std::pair<int,double> > &hadCands, bool mT_cut) {
  if (cfAVersion<77) return;
  eCands.clear();
  muCands.clear();
  hadCands.clear();
  // cout << "Found " << pfcand_pt->size() << " PFCands." << endl;
  for (uint itk(0); itk<pfcand_pdgId->size(); itk++) {
    if (!PassIsoTrackBaseline(itk)) continue;
    if (mT_cut && GetMTW(pfcand_pt->at(itk),pfTypeImets_et->at(0),pfcand_phi->at(itk),pfTypeImets_phi->at(0))>100) continue;
    //  int type = static_cast<int>(pfcand_pdgId->at(itk));
    double iso = GetPFCandIsolation(itk);
    double relIso=iso/pfcand_pt->at(itk);
    // note: not cutting here on isolation!
    switch (abs(static_cast<int>(pfcand_pdgId->at(itk)))) {
    case 11:
      eCands.push_back(std::make_pair(itk, relIso));
    case 13:
      muCands.push_back(std::make_pair(itk, relIso));
    case 211:
      hadCands.push_back(std::make_pair(itk, relIso));
    default:
      continue;
    }
  }
  return;
}

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
  return GetMTW(lep_pt,pfTypeImets_et->at(0),lep_phi,pfTypeImets_phi->at(0));
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
  return GetMTW(lep_pt,pfTypeImets_et->at(0),lep_phi,pfTypeImets_phi->at(0));
}

double EventHandler::GetDeltaThetaT(const double lep_pt, const double lep_phi) const {
    
    //Code from an email from Kristen
    TLorentzVector WInLab(0.,0.,0.,0.);
    TLorentzVector LeptonInLab(0.,0.,0.,0.);
    TLorentzVector LeptonInWCM(0.,0.,0.,0.);

    double pfmet_0 = pfTypeImets_et->at(0);
    double pfmet_x = pfmet_0 * cos(pfTypeImets_phi->at(0));
    double pfmet_y = pfmet_0 * sin(pfTypeImets_phi->at(0));
    double lep_px = lep_pt*cos(lep_phi);
    double lep_py = lep_pt*sin(lep_phi);
    
    double mtw = GetMTW(lep_pt,pfTypeImets_et->at(0),lep_phi,pfTypeImets_phi->at(0));
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

double EventHandler::GetWpT(const double lep_pt, const double lep_phi) const {

    double pfmet_0 = pfTypeImets_et->at(0);
    double pfmet_x = pfmet_0 * cos(pfTypeImets_phi->at(0));
    double pfmet_y = pfmet_0 * sin(pfTypeImets_phi->at(0));
    double lep_px = lep_pt*cos(lep_phi);
    double lep_py = lep_pt*sin(lep_phi);
    
    TVector2 W(pfmet_x+lep_px,pfmet_y+lep_py);
    return W.Mod();

}

int EventHandler::GetNumTaus( const bool againstEMu, const bool mT_cut) const {
  // PHYS14 POG ID
  if (cfAVersion<77) return -1;
  int ntaus(0);
  // cout << "Found " << taus_byDecayModeFinding->size() << " taus." << endl;
  for (uint itau(0); itau<taus_byDecayModeFinding->size(); itau++) {
    if (PassPhys14TauID(itau, againstEMu, mT_cut)) ntaus++;
  }
  return ntaus;
}

bool EventHandler::PassPhys14TauID(const int itau, const bool againstEMu, const bool mT_cut) const {
  if (taus_pt->at(itau) < 20.) return false;
  if (fabs(taus_eta->at(itau)) > 2.3) return false;
  if (!taus_byDecayModeFinding->at(itau)) return false;
  if (taus_chargedIsoPtSum->at(itau) > 1.) return false;
  if (againstEMu && (!taus_againstMuonLoose3->at(itau) || !taus_againstElectronLooseMVA5->at(itau))) return false;
  if (mT_cut && GetMTW(taus_pt->at(itau), pfTypeImets_et->at(0), taus_phi->at(itau), pfTypeImets_phi->at(0))>100) return false;
  return true;
}

void EventHandler::GetTrueLeptons(std::vector<int> &true_electrons, std::vector<int> &true_muons, std::vector<int> &true_had_taus) {
  true_electrons.clear();
  true_muons.clear();
  true_had_taus.clear();

  // loop to get all e/mu, tag tau-->e/mu
  //  cout << "Find e/mu" << endl;
  for(unsigned i = 0; i < mc_doc_id->size(); ++i){ 
    const int id = static_cast<int>(floor(fabs(mc_doc_id->at(i))+0.5));
    const int mom = static_cast<int>(floor(mc_doc_mother_id->at(i)));
    const int gmom = static_cast<int>(floor(fabs(mc_doc_grandmother_id->at(i))+0.5));
    const int ggmom = static_cast<int>(floor(fabs(mc_doc_ggrandmother_id->at(i))+0.5));
    // if (abs(mom) == 24 || (abs(mom) == 15 && (gmom == 24 || (gmom == 15 && ggmom == 24)))) PrintGenParticleInfo(i);
    if((id == 11 || id == 13) && (abs(mom) == 24 || (abs(mom) == 15 && (gmom == 24 || (gmom == 15 && ggmom == 24))))){
      if (abs(mom)!=15) { // from W, no prob
	if (id==11) true_electrons.push_back(i);
	else if (id==13) true_muons.push_back(i);
      }
      else { // from tau, check for Brem
	uint nlep(0);
	// cout << " from tau, check for Brem" << endl;
	for(uint j=i; j<mc_doc_id->size(); ++j) {
	  const int momb = static_cast<int>(floor(mc_doc_mother_id->at(j)));
	  const int gmomb = static_cast<int>(floor(fabs(mc_doc_grandmother_id->at(j))+0.5));
	  const int ggmomb = static_cast<int>(floor(fabs(mc_doc_ggrandmother_id->at(j))+0.5));
	  if (abs(momb)!=15||(gmomb!=24&&ggmomb!=24)) continue;
	  const int idb = static_cast<int>(floor(fabs(mc_doc_id->at(j))+0.5));
	  if (idb==id) nlep++;
	  // cout << "nlep=" << nlep << ": ";
	  // PrintGenParticleInfo(j);
	  if (nlep>1) {
	    // cout << "BREM!" << endl;
	    break;
	  }
	  if (momb!=mom) break;
	}
	if (nlep>1) continue;
	if (id==11) true_electrons.push_back(i);
	else if (id==13) true_muons.push_back(i);
      }
    }
  }
  // cout << "Now find taus" << endl;
  // now find taus
  for(unsigned i = 0; i < mc_doc_id->size(); ++i){ 
    const int id = static_cast<int>(floor((mc_doc_id->at(i))));
    const int mom = static_cast<int>(floor(fabs(mc_doc_mother_id->at(i))+0.5));
    if (abs(id)==15 && mom == 24) { // only count highest tau in the chain
      // PrintGenParticleInfo(i);
      true_had_taus.push_back(i);
      uint nmu(0), nel(0);
        // cout << "Check for leptonic decay...id=" << id << endl;
      // do another loop from here to see if the tau decays leptonically
      for(uint j=i; j<mc_doc_id->size(); ++j) {
	const int idb = static_cast<int>(floor((mc_doc_id->at(j))));
	if (abs(idb)!=11&&abs(idb)!=13) continue;
	const int momb = static_cast<int>(floor(mc_doc_mother_id->at(j)));
	if (momb != id) continue;
	const int gmomb = static_cast<int>(floor(fabs(mc_doc_grandmother_id->at(j))+0.5));
	if (gmomb!=24) continue;
	 // cout << "Found one! ";
	 // PrintGenParticleInfo(j);
	if (static_cast<int>(floor(fabs(mc_doc_id->at(j))+0.5))==11) nel++;
	if (static_cast<int>(floor(fabs(mc_doc_id->at(j))+0.5))==13) nmu++;
      }
      if (nel==1||nmu==1) true_had_taus.pop_back();
    }
  }
  // cout << "Found " << true_electrons.size() << " electrons, " << true_muons.size() << " muons, and " << true_had_taus.size() << " taus." << endl;
  return;
  
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
