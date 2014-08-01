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
#include "timer.hpp"
#include "math.hpp"
// #include "in_json_2012.hpp"

const double EventHandler::CSVTCut(0.898);
const double EventHandler::CSVMCut(0.679);
const double EventHandler::CSVLCut(0.244);
// const std::vector<std::vector<int> > VRunLumiPrompt(MakeVRunLumi("Golden"))
// const std::vector<std::vector<int> > VRunLumi24Aug(MakeVRunLumi("24Aug"));
// const std::vector<std::vector<int> > VRunLumi13Jul(MakeVRunLumi("13Jul"));


EventHandler::EventHandler(const std::string &fileName, const bool isList, const double scaleFactorIn, const bool fastMode):
  cfA(fileName, isList),
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
  beta(0){
  if (fastMode&&cmEnergy<=8) { // turn off unnecessary branches
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

bool EventHandler::isInMuonCollection(const double eta, const double phi) const{
  if (!recoMuonsUpToDate) {
    if (cmEnergy>=13) recoMuonCache=GetRecoMuons(true);
    else recoMuonCache=GetRA2bMuons(true);
    recoMuonsUpToDate=true;
  }
  for (unsigned int mu(0); mu<recoMuonCache.size(); mu++) {
    double deltaR = Math::GetDeltaR(phi, eta, mus_phi->at(recoMuonCache[mu]), mus_eta->at(recoMuonCache[mu]));
    if (deltaR<0.4) {
      return true;
    }
  }
  return false;
}

bool EventHandler::isInElectronCollection(const double eta, const double phi) const{
if (!recoElectronsUpToDate) {
    if (cmEnergy>=13) recoElectronCache=GetRecoElectrons(true);
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
      const float pt = jets_AKPF_pt->at(ijet);
      const float eta = fabs(jets_AKPF_eta->at(ijet));
      
      int i = 0;
      totjet++;
      for (std::vector<std::vector<float> >::const_iterator itr = puJet_rejectionBeta->begin(); itr != puJet_rejectionBeta->end(); ++itr, ++i) {
        int j = 0;
        float mypt = 0;
        float myeta = 0;
        float mybeta = 0;
        float result = 0;
        float tmp1 = 0, tmp2 = 0, tmp3 = 0, tmp4 = 0;
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

void EventHandler::GetEntry(const unsigned int entry){
  cfA::GetEntry(entry);
  betaUpToDate=false;
  genMuonsUpToDate=false;
  genElectronsUpToDate=false;
  genTausUpToDate=false;
  recoMuonsUpToDate=false;
  recoElectronsUpToDate=false;
  recoTausUpToDate=false;
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

bool EventHandler::PassesMETCleaningCut() const{
  for(unsigned int jet(0); jet<jets_AKPF_pt->size(); ++jet){
    if(isProblemJet(jet)) return false;
  }
  //if(pfTypeImets_et->at(0)>2.0*pfmets_et->at(0)) return false;
  if(pfTypeImets_et->at(0)>2.0*mets_AK5_et->at(0)) return false; // updated 11/14 (JB-F)
  return cschalofilter_decision
    && (hbhefilter_decision || sampleName.find("TChihh")!=std::string::npos || sampleName.find("HbbHbb")!=std::string::npos)
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
  if (cmEnergy>8) { // overlap removal
    if (isInMuonCollection(jets_AKPF_eta->at(ijet), jets_AKPF_phi->at(ijet))) return false;
    if (isInElectronCollection(jets_AKPF_eta->at(ijet), jets_AKPF_phi->at(ijet))) return false;
    if (isInTauCollection(jets_AKPF_eta->at(ijet), jets_AKPF_phi->at(ijet))) return false;
  }
  return true;
}

bool EventHandler::isCleanJet(const unsigned int ijet, const int pdgId) const{ // remove overlap, based on pfcand hypothesis
  float jet_eta(jets_AKPF_eta->at(ijet)), jet_phi(jets_AKPF_phi->at(ijet));
  for (unsigned int it = 0; it<pfcand_pt->size(); it++) {
    if (static_cast<int>(fabs(pfcand_pdgId->at(it)))!=pdgId) continue;
    float dR = Math::GetDeltaR(jet_phi, jet_eta, pfcand_phi->at(it), pfcand_eta->at(it));
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

bool EventHandler::PassesSpecificTrigger(const std::string trigger) const{ // just check if a specific trigger fired
  for(unsigned int a=0; a<trigger_name->size(); ++a){
    if(trigger_name->at(a).find(trigger)!=std::string::npos
       && trigger_prescalevalue->at(a)==1 && trigger_decision->at(a)==1){
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
  switch(level){
  case 1:
    pt_cut=5.0;
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
  double pt_thresh(5.0);
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

  // if (cmEnergy>8) {
  //   if (isLeptonicTau(k)) return false;
  // }

  return true;
}

// bool EventHandler::isLeptonicTau(const unsigned int k) const{
//   if (!recoElectronsUpToDate) {
//     if (cmEnergy>=13) recoElectronCache=GetRecoElectrons(true);
//     //  else recoElectronCache=GetRA2bElectrons(true);
//     recoElectronsUpToDate=true;
//   }

//   if (!recoMuonsUpToDate) {
//     if (cmEnergy>=13) recoElectronCache=GetRecoMuons(true);
//     //  else recoElectronCache=GetRA2bElectrons(true);
//     recoMuonsUpToDate=true;
//   }

//   int el_ind(taus_el_ind->at(k)), mu_ind(taus_mu_ind->at(k));
//   if (el_ind>=0) {
//     for (unsigned int iel(0); iel<recoElectronCache.size(); iel++) {
//       if (el_ind==recoElectronCache[iel]) return true;
//     }
//   }
//   if (mu_ind>=0) {
//     for (unsigned int imu(0); imu<recoMuonCache.size(); imu++) {
//       if (mu_ind==recoMuonCache[imu]) return true;
//     }
//   }
  
//   return false;
// }

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

int EventHandler::NewGetNumIsoTracks(const double ptThresh) const{
  int nisotracks=0;
  if ( GetcfAVersion() < 71 ) return nisotracks;
  for ( unsigned int itrack = 0 ; itrack < isotk_pt->size() ; ++itrack) {
    if ( (isotk_pt->at(itrack) >= ptThresh) &&
         (isotk_iso->at(itrack) /isotk_pt->at(itrack) < 0.1 ) &&
         ( fabs(isotk_dzpv->at(itrack)) <0.1) && //important to apply this; was not applied at ntuple creation
         ( fabs(isotk_eta->at(itrack)) < 2.4 ) //this is more of a sanity check
         ){
      ++nisotracks;
    }
  }
  return nisotracks;
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
  double npv(-1.0);
  int first_BX_to_check = 0 - BX_before;
  for(unsigned int i(0); i<PU_bunchCrossing->size(); ++i){
    if(PU_bunchCrossing->at(i)<first_BX_to_check) continue;
    if(PU_bunchCrossing->at(i)==0) break;
    npv += PU_NumInteractions->at(i);
  }
  return npv;
}

double EventHandler::GetLateOutOfTimePU() const{
  double npv(-1.0);
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
  return lumiWeights.weight(GetNumInteractions());
}


double EventHandler::GetTopPtWeight() const{
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

std::pair <int, double> EventHandler::GetGenMuonMinDPt(const int genLep, const vector<uint> matched) const {
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

bool EventHandler::IsFromB(const int mom, const int gmom, const int ggmom) const{
  if ( (mom>=500&&mom<600) || (mom>=5000&&mom<6000) || fabs(mom)==5 ) return true;
  if ( (gmom>=500&&mom<600) || (gmom>=5000&&mom<6000) || fabs(gmom)==5 ) return true;
  if ( (ggmom>=500&&mom<600) || (ggmom>=5000&&mom<6000) || fabs(ggmom)==5 ) return true;
  return false;
}

float EventHandler::GetMinDRMuonJet(const int mu) const{
  float minDR(999.);
  float mu_eta(mus_eta->at(mu)), mu_phi(mus_phi->at(mu));
  float thisDR(999.);
  for (int jet(0); jet<static_cast<int>(jets_AKPF_pt->size()); jet++) {
    //  if (fabs(GetJetGenId(jet))==13) continue;
    if (!isCleanJet(jet, 13)) continue; // trying to remove overlap
    if(isGoodJet(jet, true, 0.0, 5.0/*, false*/)) {
      thisDR=Math::GetDeltaR(mu_phi, mu_eta, jets_AKPF_phi->at(jet), jets_AKPF_eta->at(jet));
    }
    if (thisDR<minDR) minDR=thisDR;
  }
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
    genMuonCache[genLep].SetDeltaRWb(GetCorrespondingDeltaRWb(static_cast<int>(mc_mus_charge->at(gen_index))));
    double gen_pt(genMuonCache[genLep].GetLorentzVector().Pt());
    double gen_eta(genMuonCache[genLep].GetLorentzVector().Eta());
    double gen_phi(genMuonCache[genLep].GetLorentzVector().Phi());
    std::pair <int,double> min_dR = GetGenMuonMinDR(genLep, mus_matched);
    std::pair <int,double> min_dPt = GetGenMuonMinDPt(genLep, mus_matched);
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
    if (reco_index>=0) genMuonCache[genLep].SetIsVeto(isRecoMuon(reco_index, 0));
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
      //  if(isGenMuon(gen)&&IsFromB(mc_mus_mother_id->at(gen), mc_mus_grandmother_id->at(gen), mc_mus_ggrandmother_id->at(gen))) {
	genMuonCache.push_back(GenMuon(TLorentzVector(mc_mus_px->at(gen),mc_mus_py->at(gen),mc_mus_pz->at(gen),mc_mus_energy->at(gen)),gen,mc_mus_id->at(gen),static_cast<unsigned int>(mc_mus_mother_id->at(gen))));
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
      //cout << "JERR1" << endl;
      double d2 = sqrt(pow(genElectronCache[genLep].GetMinDPt().second/2.5,2)+pow(Math::GetDeltaR(gen_phi, gen_eta, els_phi->at(dPt_ind), els_eta->at(dPt_ind))/0.07,2));
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
	genElectronCache.push_back(GenElectron(TLorentzVector(mc_electrons_px->at(gen),mc_electrons_py->at(gen),mc_electrons_pz->at(gen),mc_electrons_energy->at(gen)),gen,mc_electrons_id->at(gen),static_cast<unsigned int>(mc_electrons_mother_id->at(gen))));
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
	genTauCache.push_back(GenTau(TLorentzVector(mc_taus_px->at(gen),mc_taus_py->at(gen),mc_taus_pz->at(gen),mc_taus_energy->at(gen)),gen,mc_taus_id->at(gen),static_cast<unsigned int>(mc_taus_mother_id->at(gen))));
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
  bool fromTop(false);
    for(unsigned int imc = 0; imc < mc_doc_id->size(); imc++){
      if (hard_scatter_only && !(mc_doc_status->at(imc)==3||mc_doc_status->at(imc)==22||mc_doc_status->at(imc)==23)) continue;
      if (static_cast<int>(fabs(mc_doc_id->at(imc)))==5) fromTop=true;
      if (!fromTop) continue;
      if (mc_doc_pt->at(imc)<ptCut) continue;
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

vector<int> EventHandler::GetRecoMuons(const bool veto) const{
  vector<int> muons;
  for(uint index=0; index<mus_pt->size(); index++)
    if(veto){
      if(isRecoMuon(index, 0)) muons.push_back(index);
    }	else {
      if(isRecoMuon(index, 1)) muons.push_back(index);
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

bool EventHandler::isRecoMuon(const uint imu, const uint level) const{
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
  const double beamx (beamSpot_x->at(0)), beamy(beamSpot_y->at(0));   
  const double d0 = mus_tk_d0dum->at(imu)-beamx*sin(mus_tk_phi->at(imu))+beamy*cos(mus_tk_phi->at(imu));
  const double mus_vz = mus_tk_vz->at(imu);
  const double mus_dz_vtx = fabs(mus_vz-pv_z->at(0));
  if (fabs(d0)>=0.2 || mus_dz_vtx>=0.5) return false;
  if ( !mus_tk_numvalPixelhits->at(imu)) return false;
  if ( mus_tk_LayersWithMeasurement->at(imu) <= 5 ) return false;

  if (GetMuonRelIso(imu) > 0.2) return false;

  // need to make sure both collections have the "slimmed cuts"
  // see https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookMiniAOD

  if (cmEnergy>8&&!mus_isPF->at(imu)) return false;
  if (!(mus_pt->at(imu)>5.0 ||
	( mus_pt->at(imu)>3.0 && (mus_isPFMuon->at(imu)||mus_id_TrackerMuonArbitrated->at(imu)||mus_id_AllStandAloneMuons->at(imu)||mus_id_AllGlobalMuons->at(imu)) ) ||
	mus_isPFMuon->at(imu))) return false;
  

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

vector<int> EventHandler::GetRecoElectrons(const bool veto) const{
  vector<int> electrons;
  for(uint index=0; index<els_pt->size(); index++)
    if(!veto){
      if(isRecoElectron(index,3)) electrons.push_back(index);
    }	else {
      if(isRecoElectron(index,0)) electrons.push_back(index);
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

bool EventHandler::isRecoElectron(const uint iel, const uint level) const{
//N.B.: cut does not have the fabs(1/E-1/p) and conversion rejection cuts from the EGamma POG!!!
  if (iel>els_pt->size()) return false;
  const double dmax(std::numeric_limits<double>::max());
  double pt_cut(10.0); //Not actually part of the EGamma ID
  double eta_cut(0.007), phi_cut(0.8), sigmaietaieta_cut(0.01), h_over_e_cut(0.15), d0_cut(0.04), dz_cut(0.2), iso_cut(0.15);
  if (cmEnergy>8&&!els_isPF->at(iel)) return false;

  switch(level){
  case 1:
    pt_cut=20.0;
    if(els_isEB->at(iel)){
      eta_cut=0.007; phi_cut=0.15; sigmaietaieta_cut=0.01; h_over_e_cut=0.12;
      d0_cut=0.02; dz_cut=0.2; iso_cut=0.15;
    }else if(els_isEE->at(iel)){
      eta_cut=0.009; phi_cut=0.10; sigmaietaieta_cut=0.03; h_over_e_cut=0.1;
      d0_cut=0.02; dz_cut=0.2; iso_cut=(els_pt->at(iel)>20.0?0.15:0.10);
    }
    break;
  case 2:
    pt_cut=20.0;
    if(els_isEB->at(iel)){
      eta_cut=0.004; phi_cut=0.06; sigmaietaieta_cut=0.01; h_over_e_cut=0.12;
      d0_cut=0.02; dz_cut=0.1; iso_cut=0.15;
    }else if(els_isEE->at(iel)){
      eta_cut=0.007; phi_cut=0.03; sigmaietaieta_cut=0.03; h_over_e_cut=0.1;
      d0_cut=0.02; dz_cut=0.1; iso_cut=(els_pt->at(iel)>20.0?0.15:0.10);
    }
    break;
  case 3:
    pt_cut=20.0;
    if(els_isEB->at(iel)){
      eta_cut=0.004; phi_cut=0.03; sigmaietaieta_cut=0.01; h_over_e_cut=0.12;
      d0_cut=0.02; dz_cut=0.1; iso_cut=0.10;
      if (cmEnergy==13) {
	eta_cut=0.009; iso_cut=0.18;
      }
    }else if(els_isEE->at(iel)){
      eta_cut=0.005; phi_cut=0.02; sigmaietaieta_cut=0.03; h_over_e_cut=0.1;
      d0_cut=0.02; dz_cut=0.1; iso_cut=(els_pt->at(iel)>20.0?0.10:0.07);
      if (cmEnergy==13) {
	eta_cut=0.01; sigmaietaieta_cut=0.031;
	h_over_e_cut=0.12; iso_cut=0.16;
      }    
    }
    break;
  case 0: //intentionally falling through to default "veto" case
  default:
    pt_cut=10.0;
    if(els_isEB->at(iel)){
      eta_cut=0.007; phi_cut=0.8; sigmaietaieta_cut=0.01; h_over_e_cut=0.15;
      d0_cut=0.04; dz_cut=0.2; iso_cut=0.15;
      if (cmEnergy>=13) {
	eta_cut=0.012; iso_cut=0.23;
      }
    }else if(els_isEE->at(iel)){
      eta_cut=0.01; phi_cut=0.7; sigmaietaieta_cut=0.03; h_over_e_cut=dmax;
      d0_cut=0.04; dz_cut=0.2; iso_cut=0.15;
      if (cmEnergy>=13) {
	eta_cut=0.015; sigmaietaieta_cut=0.033; iso_cut=0.25;
      }
    }
    break;
  }
  // if(k>else_pt->size()) return false;
  // if (level>0) {
    if (fabs(els_scEta->at(iel)) >= 2.5 ) return false;
    if (els_pt->at(iel) < pt_cut) return false;
    // }

  if ( fabs(els_dEtaIn->at(iel)) > eta_cut)  return false;
  if ( fabs(els_dPhiIn->at(iel)) > phi_cut)  return false;
  if (cmEnergy<13 && els_sigmaIEtaIEta->at(iel) > sigmaietaieta_cut) return false;
  if (cmEnergy>=13 && els_full5x5_sigmaIetaIeta->at(iel) > sigmaietaieta_cut) return false;
  if (els_hadOverEm->at(iel) > h_over_e_cut) return false;

  const double beamx(beamSpot_x->at(0)), beamy(beamSpot_y->at(0)); 
  const double d0(els_d0dum->at(iel)-beamx*sin(els_tk_phi->at(iel))+beamy*cos(els_tk_phi->at(iel)));
  if ( fabs(d0) >= d0_cut ) return false;
  if ( fabs(els_vz->at(iel) - pv_z->at(0) ) >= dz_cut ) return false;

  float rel_iso(999.);
  if (cmEnergy<13) rel_iso=GetElectronRelIso(iel);
  else rel_iso=GetCSAElectronIsolation(iel);
  if(rel_iso>=iso_cut) return false;

  if (!hasPFMatch(iel, 11)) return false;

  return true;
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
	  && els_n_inner_layer->at(iel) <= 1
	  && fabs(getDZ(els_vx->at(iel), els_vy->at(iel), els_vz->at(iel), cos(els_tk_phi->at(iel))*els_tk_pt->at(iel), 
			sin(els_tk_phi->at(iel))*els_tk_pt->at(iel), els_tk_pz->at(iel), 0)) < 0.1
	  && fabs(1./els_caloEnergy->at(iel) - els_eOverPIn->at(iel)/els_caloEnergy->at(iel)) < 0.05 
	  && hasPFMatch(iel, 11) 
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

bool EventHandler::passedBaseElectronSelection(uint iel, float ElectronPTThreshold){
  if(iel >= els_pt->size()) return false;

  float d0PV = els_d0dum->at(iel)-pv_x->at(0)*sin(els_tk_phi->at(iel))+pv_y->at(0)*cos(els_tk_phi->at(iel));
  
  return (els_pt->at(iel) > ElectronPTThreshold
	  && fabs(els_scEta->at(iel)) < 2.5
	  && !els_hasMatchedConversion->at(iel)
	  && els_n_inner_layer->at(iel) <= 1
	  && fabs(getDZ(els_vx->at(iel), els_vy->at(iel), els_vz->at(iel), cos(els_tk_phi->at(iel))*els_tk_pt->at(iel), 
			sin(els_tk_phi->at(iel))*els_tk_pt->at(iel), els_tk_pz->at(iel), 0)) < 0.1
	  && fabs(1./els_caloEnergy->at(iel) - els_eOverPIn->at(iel)/els_caloEnergy->at(iel)) < 0.05 
	  && hasPFMatch(iel, 11) 
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
/*
bool EventHandler::passedRA4ElectronSelection(uint iel, float ElectronPTThreshold){
  if(iel >= els_pt->size()) return false;
  cout << "Calculating relIso..." << endl;
  double relIso = GetRA4ElectronIsolation(iel);
  cout << "Calculating baseSelection..." << endl;
  return (passedBaseElectronSelection(iel, ElectronPTThreshold) && relIso < 0.15);
}

bool EventHandler::passedRA4ElectronVeto(uint iel, float ElectronPTThreshold){
  if(iel >= els_pt->size()) return false;

  float d0PV = els_d0dum->at(iel)-pv_x->at(0)*sin(els_tk_phi->at(iel))+pv_y->at(0)*cos(els_tk_phi->at(iel));
  double relIso = GetRA4ElectronIsolation(iel);
  
  return (els_pt->at(iel) > ElectronPTThreshold
	  && fabs(els_scEta->at(iel)) < 2.5
	  && relIso < 0.15
	  && fabs(getDZ(els_vx->at(iel), els_vy->at(iel), els_vz->at(iel), cos(els_tk_phi->at(iel))*els_tk_pt->at(iel), 
			sin(els_tk_phi->at(iel))*els_tk_pt->at(iel), els_tk_pz->at(iel), 0)) < 0.2
	  && fabs(d0PV) < 0.04 
	  && ((els_isEB->at(iel) // Endcap selection
	       && fabs(els_dEtaIn->at(iel)) < 0.007
	       && fabs(els_dPhiIn->at(iel)) < 0.8
	       && els_sigmaIEtaIEta->at(iel) < 0.01
	       && els_hadOverEm->at(iel) < 0.15) ||
	      (els_isEE->at(iel)  // Barrel selection
	       && fabs(els_dEtaIn->at(iel)) < 0.01
	       && fabs(els_dPhiIn->at(iel)) < 0.7
	       && els_sigmaIEtaIEta->at(iel) < 0.03))
	  );  
}

float EventHandler::GetEffectiveArea(float SCEta, bool isMC){
  float EffectiveArea;
  
  if(isMC) {
    if (fabs(SCEta) >= 0.0 && fabs(SCEta) < 1.0 ) EffectiveArea = 0.110;
    if (fabs(SCEta) >= 1.0 && fabs(SCEta) < 1.479 ) EffectiveArea = 0.130;
    if (fabs(SCEta) >= 1.479 && fabs(SCEta) < 2.0 ) EffectiveArea = 0.089;
    if (fabs(SCEta) >= 2.0 && fabs(SCEta) < 2.2 ) EffectiveArea = 0.130;
    if (fabs(SCEta) >= 2.2 && fabs(SCEta) < 2.3 ) EffectiveArea = 0.150;
    if (fabs(SCEta) >= 2.3 && fabs(SCEta) < 2.4 ) EffectiveArea = 0.160;
    if (fabs(SCEta) >= 2.4) EffectiveArea = 0.190;
  }
  else {
    //kEleGammaAndNeutralHadronIso03 from 2011 data
    //obtained from http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/EGamma/EGammaAnalysisTools/interface/ElectronEffectiveArea.h?revision=1.3&view=markup
    if (fabs(SCEta) >= 0.0 && fabs(SCEta) < 1.0 ) EffectiveArea = 0.100;
    if (fabs(SCEta) >= 1.0 && fabs(SCEta) < 1.479 ) EffectiveArea = 0.120;
    if (fabs(SCEta) >= 1.479 && fabs(SCEta) < 2.0 ) EffectiveArea = 0.085;
    if (fabs(SCEta) >= 2.0 && fabs(SCEta) < 2.2 ) EffectiveArea = 0.110;
    if (fabs(SCEta) >= 2.2 && fabs(SCEta) < 2.3 ) EffectiveArea = 0.120;
    if (fabs(SCEta) >= 2.3 && fabs(SCEta) < 2.4 ) EffectiveArea = 0.120;
    if (fabs(SCEta) >= 2.4) EffectiveArea = 0.130;
  }
  return EffectiveArea;
}
*/

bool EventHandler::IsMC(){
  std::string sampleName(chainA.GetFile()->GetName());
  return (sampleName.find("Run201") == std::string::npos);
}

double EventHandler::GetHT() const{
  double HT(0.0);
  for(unsigned int i(0); i<jets_AKPF_pt->size(); ++i){
    if(isGoodJet(i)) HT+=jets_AKPF_pt->at(i);
  }
  return HT;
}

int EventHandler::GetNumGoodJets() const{
  int numGoodJets(0);
  for(unsigned int i(0); i<jets_AKPF_pt->size(); ++i){
    if(isGoodJet(i)) ++numGoodJets;
  }
  return numGoodJets;
}

int EventHandler::GetNumCSVTJets() const{
  int numPassing(0);
  for(unsigned int i(0); i<jets_AKPF_pt->size(); ++i){
    if(isGoodJet(i) && jets_AKPF_btag_secVertexCombined->at(i)>CSVTCut){
      ++numPassing;
    }
  }
  return numPassing;
}

int EventHandler::GetNumCSVMJets() const{
  int numPassing(0);
  for(unsigned int i(0); i<jets_AKPF_pt->size(); ++i){
    if(isGoodJet(i) && jets_AKPF_btag_secVertexCombined->at(i)>CSVMCut){
      ++numPassing;
    }
  }
  return numPassing;
}

int EventHandler::GetNumCSVLJets() const{
  int numPassing(0);
  for(unsigned int i(0); i<jets_AKPF_pt->size(); ++i){
    if(isGoodJet(i) && jets_AKPF_btag_secVertexCombined->at(i)>CSVLCut){
      ++numPassing;
    }
  }
  return numPassing;
}
