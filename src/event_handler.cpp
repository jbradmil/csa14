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
#include "cfa.hpp"
#include "event_number.hpp"
#include "b_jet.hpp"
#include "timer.hpp"
#include "math.hpp"
#include "in_json_2012.hpp"
#include "gen_muon.hpp"

const double EventHandler::CSVTCut(0.898);
const double EventHandler::CSVMCut(0.679);
const double EventHandler::CSVLCut(0.244);
const std::vector<std::vector<int> > VRunLumiPrompt(MakeVRunLumi("Golden"));
const std::vector<std::vector<int> > VRunLumi24Aug(MakeVRunLumi("24Aug"));
const std::vector<std::vector<int> > VRunLumi13Jul(MakeVRunLumi("13Jul"));


EventHandler::EventHandler(const std::string &fileName, const bool isList, const double scaleFactorIn, const bool fastMode):
  cfA(fileName, isList),
  genMuonCache(0),
  genMuonsUpToDate(false),
  betaUpToDate(false),
  scaleFactor(scaleFactorIn),
  beta(0){
  if (fastMode) { // turn off unnecessary branches
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
    /*
      chainB.SetBranchStatus("Nmus",0);
      chainB.SetBranchStatus("mus_*",0);
      chainB.SetBranchStatus("Nels",0);
      chainB.SetBranchStatus("els_*",0);
    */
    chainB.SetBranchStatus("Nmets*",0);
    chainB.SetBranchStatus("mets*",0);
    chainB.SetBranchStatus("mets_AK5_et",1);
    chainB.SetBranchStatus("Njets_AK5PFclean",0);
    chainB.SetBranchStatus("jets_AK5PFclean_*",0);
  }
}

void EventHandler::GetBeta(const std::string which) const{
  betaUpToDate=true;

  //Clear out the vector before starting a new event!
  beta.clear();

  if (GetcfAVersion()<69){
    beta.resize(jets_AK5PF_pt->size(), 0.0);
  }else{
    int totjet = 0;
    int matches = 0;
    for (unsigned int ijet=0; ijet<jets_AK5PF_pt->size(); ++ijet) {
      const float pt = jets_AK5PF_pt->at(ijet);
      const float eta = fabs(jets_AK5PF_eta->at(ijet));
      
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
  genMuonsUpToDate=false;
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
  for(unsigned int jet(0); jet<jets_AK5PF_pt->size(); ++jet){
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
  return jets_AK5PF_pt->at(ijet)>50.0
    && fabs(jets_AK5PF_eta->at(ijet))>0.9
    && fabs(jets_AK5PF_eta->at(ijet))<1.9
                                      && jets_AK5PF_chg_Mult->at(ijet)-jets_AK5PF_neutral_Mult->at(ijet)>=40;
}

bool EventHandler::PassesBadJetFilter() const{
  for(unsigned int i(0); i<jets_AK5PF_pt->size(); ++i){
    if(isGoodJet(i,false,30.0,DBL_MAX) && !isGoodJet(i,true,30.0,DBL_MAX)) return false;
  }
  return true;
}

int EventHandler::GetPBNR() const{
  //RA2 particle-based noise rejection                                                                                                                                           

  bool nhBad=false;
  bool phBad=false;
  for (unsigned int it = 0; it<jets_AK5PF_pt->size(); it++) {
    //cfA version from Keith                                                                                                                                                     
    double NHF = jets_AK5PF_neutralHadE->at(it)/(jets_AK5PF_energy->at(it)*jets_AK5PF_corrFactorRaw->at(it));
    double PEF = jets_AK5PF_photonEnergy->at(it)/(jets_AK5PF_energy->at(it)*jets_AK5PF_corrFactorRaw->at(it));
    if (NHF > 0.9)  nhBad = true;
    if (PEF > 0.95) phBad = true;
  }

  if (nhBad && phBad) return -3;
  else if (phBad) return -2;
  else if (nhBad) return -1;
  return 1;
}

bool EventHandler::isGoodJet(const unsigned int ijet, const bool jetid, const double ptThresh, const double etaThresh, const bool doBeta) const{
  if(jets_AK5PF_pt->at(ijet)<ptThresh || fabs(jets_AK5PF_eta->at(ijet))>etaThresh) return false;
  if( jetid && !jetPassLooseID(ijet) ) return false;
  if(GetcfAVersion()<69||sampleName.find("SMS-TChiHH")!=std::string::npos) return true;
  if(!betaUpToDate) GetBeta();
  if(beta.at(ijet)<0.2 && doBeta) return false;
  return true;
}

bool EventHandler::jetPassLooseID(const unsigned int ijet) const{
  //want the uncorrected energy
  const double jetenergy = jets_AK5PF_energy->at(ijet) * jets_AK5PF_corrFactorRaw->at(ijet);
  const int numConst = static_cast<int>(jets_AK5PF_mu_Mult->at(ijet)+jets_AK5PF_neutral_Mult->at(ijet)+jets_AK5PF_chg_Mult->at(ijet)); //stealing from Keith
  
  if (jetenergy>0.0) {
    if (jets_AK5PF_neutralHadE->at(ijet) /jetenergy <= 0.99
        && jets_AK5PF_neutralEmE->at(ijet) / jetenergy <= 0.99
        && numConst >= 2
        && ( fabs(jets_AK5PF_eta->at(ijet))>=2.4
             || (fabs(jets_AK5PF_eta->at(ijet))<2.4 && jets_AK5PF_chgHadE->at(ijet)/jetenergy>0))
        && ( fabs(jets_AK5PF_eta->at(ijet))>=2.4
             || (fabs(jets_AK5PF_eta->at(ijet))<2.4 && jets_AK5PF_chgEmE->at(ijet)/jetenergy<0.99))
        && ( fabs(jets_AK5PF_eta->at(ijet))>=2.4
             || (fabs(jets_AK5PF_eta->at(ijet))<2.4 && jets_AK5PF_chg_Mult->at(ijet)>0))){
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
  double pt_cut(10.0); //Not actually part of the EGamma ID
  double eta_cut(0.007), phi_cut(0.8), sigmaietaieta_cut(0.01), h_over_e_cut(0.15), d0_cut(0.04), dz_cut(0.2), iso_cut(0.15);
  switch(level){
  case 1:
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
    pt_cut=20.0;
    if(pf_els_isEB->at(k)){
      eta_cut=0.004; phi_cut=0.06; sigmaietaieta_cut=0.01; h_over_e_cut=0.12;
      d0_cut=0.02; dz_cut=0.1; iso_cut=0.15;
    }else if(pf_els_isEE->at(k)){
      eta_cut=0.007; phi_cut=0.03; sigmaietaieta_cut=0.03; h_over_e_cut=0.1;
      d0_cut=0.02; dz_cut=0.1; iso_cut=(pf_els_pt->at(k)>20.0?0.15:0.10);
    }
    break;
  case 3:
    pt_cut=20.0;
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
  //if(k>pf_else_pt->size()) return false;
  if (fabs(pf_els_scEta->at(k)) >= 2.5 ) return false;
  if (pf_els_pt->at(k) < pt_cut) return false;

  if ( fabs(pf_els_dEtaIn->at(k)) > eta_cut)  return false;
  if ( fabs(pf_els_dPhiIn->at(k)) > phi_cut)  return false;
  if (pf_els_sigmaIEtaIEta->at(k) > sigmaietaieta_cut) return false;
  if (pf_els_hadOverEm->at(k) > h_over_e_cut) return false;

  const double beamx(beamSpot_x->at(0)), beamy(beamSpot_y->at(0)); 
  const double d0(pf_els_d0dum->at(k)-beamx*sin(pf_els_tk_phi->at(k))+beamy*cos(pf_els_tk_phi->at(k)));
  if ( fabs(d0) >= d0_cut ) return false;
  if ( fabs(pf_els_vz->at(k) - pv_z->at(0) ) >= dz_cut ) return false;

  if(GetElectronRelIso(k)>=iso_cut && use_iso) return false;
  return true;
}

double EventHandler::GetElectronRelIso(const unsigned int k) const{
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

  if (fabs(pf_mus_eta->at(k)) >= 2.4 ) return false;
  if (pf_mus_pt->at(k) < pt_thresh) return false;
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
  
  double isoNeutral(pf_mus_pfIsolationR04_sumNeutralHadronEt->at(k) + pf_mus_pfIsolationR04_sumPhotonEt->at(k) - 0.5*pf_mus_pfIsolationR04_sumPUPt->at(k));
  if(isoNeutral<0.0) isoNeutral=0.0;
  const double pf_mus_rel_iso((pf_mus_pfIsolationR04_sumChargedHadronPt->at(k) + isoNeutral) / pf_mus_pt->at(k));
  if (pf_mus_rel_iso > 0.2 && use_iso) return false;
  return true;
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

double EventHandler::GetNumInteractions() const{
  double npv(-1.0);
  for(unsigned int i(0); i<PU_bunchCrossing->size(); ++i){
    if(PU_bunchCrossing->at(i)==0){
      npv = PU_TrueNumInteractions->at(i);
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

std::pair <int, double> EventHandler::GetGenMuonMinDR(const int genLep, const vector<uint> matched) const {
  double gen_eta(genMuonCache[genLep].GetLorentzVector().Eta()), gen_phi(genMuonCache[genLep].GetLorentzVector().Phi());
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
  double gen_pt(genMuonCache[genLep].GetLorentzVector().Pt());
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
    const double thisDPt( fabs(gen_pt-mus_pt->at(reco)) );
    if(thisDPt<minDPt){
      minDPt=thisDPt;
      closest=reco;
    }
  }
  if (minDPt>10.||closest==-1) return std::make_pair(-1,-1.);
  return std::make_pair(closest,minDPt);
}

void EventHandler::SetupGenMuons() const {
  //std::cout << "GetGenMuons" << std::endl;
  GetGenMuons();
  //std::cout << "Calculate dR and dPts" << std::endl;
  vector<uint> mus_matched;
  for(unsigned int genLep(0); genLep<genMuonCache.size(); ++genLep){
    double gen_pt(genMuonCache[genLep].GetLorentzVector().Pt());
    double gen_eta(genMuonCache[genLep].GetLorentzVector().Eta());
    double gen_phi(genMuonCache[genLep].GetLorentzVector().Phi());
    std::pair <int,double> min_dR = GetGenMuonMinDR(genLep, mus_matched);
    std::pair <int,double> min_dPt = GetGenMuonMinDPt(genLep, mus_matched);
    genMuonCache[genLep].SetMinDR(min_dR);
    genMuonCache[genLep].SetMinDPt(min_dPt);
    int dR_ind(genMuonCache[genLep].GetMinDR().first), dPt_ind(genMuonCache[genLep].GetMinDPt().first);
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
    if (genMuonCache[genLep].GetMusMatch()>=0) mus_matched.push_back((uint)genMuonCache[genLep].GetMusMatch());
    //cout << "Gen muon " << genMuonCache[genLep].GetMCMusIndex() << " matched to reco muon " << genMuonCache[genLep].GetMusMatch() << endl;
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
      if(isGenMuon(gen)) {
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
    if(!isGenMuon(gen)) ignored++;
  }
  return ignored;
}

int EventHandler::GetNGenParticles(const int pdgId, const float pt, const bool check_sign) const{
  if(!genMuonsUpToDate) SetupGenMuons();
  uint count(0);
  for (uint index(0); index < genMuonCache.size(); index++) {
    if (check_sign && (pdgId*genMuonCache[index].GetPDGId()<0)) continue;
    if (genMuonCache[index].GetLorentzVector().Pt()<pt) continue;
    if (pdgId==fabs(genMuonCache[index].GetPDGId())) count++;
  }
  return count;
}

vector<int> EventHandler::GetRecoMuons(bool veto, float MuonPTThreshold, float MuonETAThreshold) {
  vector<int> muons;
  for(uint index=0; index<mus_pt->size(); index++)
    if(veto){
      if(passedRA4MuonVeto(index, MuonPTThreshold, MuonETAThreshold)) muons.push_back(index);
    }	else {
      if(passedRA4MuonSelection(index, MuonPTThreshold, MuonETAThreshold)) muons.push_back(index);
    }
  return muons;
}

bool EventHandler::passedRA4MuonVeto(uint imu, float MuonPTThreshold, float MuonETAThreshold){
  if(imu >= mus_pt->size()) return false;

  float relIso = GetRA4MuonIsolation(imu);
  
  return ((mus_isGlobalMuon->at(imu) >0 || mus_isTrackerMuon->at(imu) >0)
	  && mus_isPFMuon->at(imu) > 0
	  && fabs(getDZ(mus_tk_vx->at(imu), mus_tk_vy->at(imu), mus_tk_vz->at(imu), mus_tk_px->at(imu), 
			mus_tk_py->at(imu), mus_tk_pz->at(imu), 0)) < 0.5 
	  && mus_pt->at(imu) >= MuonPTThreshold
	  && fabs(mus_eta->at(imu)) <= MuonETAThreshold
	  && relIso < 0.2);
}

double EventHandler::getDZ(double vx, double vy, double vz, double px, double py, double pz, int firstGoodVertex){
  return vz - pv_z->at(firstGoodVertex) -((vx-pv_x->at(firstGoodVertex))*px+(vy-pv_y->at(firstGoodVertex))*py)*pz/(px*px+py*py); 
}

bool EventHandler::passedRA4MuonSelection(uint imu, float MuonPTThreshold, float MuonETAThreshold){
  if(imu >= mus_pt->size()) return false;

  float relIso = GetRA4MuonIsolation(imu);  
  return (passedBaseMuonSelection(imu, MuonPTThreshold, MuonETAThreshold) && relIso < 0.12); 
}

bool EventHandler::passedBaseMuonSelection(uint imu, float MuonPTThreshold, float MuonETAThreshold){
  if(imu >= mus_pt->size()) return false;

  float d0PV = mus_tk_d0dum->at(imu)-pv_x->at(0)*sin(mus_tk_phi->at(imu))+pv_y->at(0)*cos(mus_tk_phi->at(imu));
  int pfIdx=-1;
  
  return (mus_isGlobalMuon->at(imu) > 0
	  && mus_isPFMuon->at(imu) > 0
	  && mus_id_GlobalMuonPromptTight->at(imu)> 0 
	  && mus_tk_LayersWithMeasurement->at(imu) > 5
	  && mus_tk_numvalPixelhits->at(imu) > 0
	  && mus_numberOfMatchedStations->at(imu) > 1
	  //&& fabs(mus_dB->at(imu)) < 0.02
	  && fabs(d0PV) < 0.02
	  && fabs(getDZ(mus_tk_vx->at(imu), mus_tk_vy->at(imu), mus_tk_vz->at(imu), mus_tk_px->at(imu), 
			mus_tk_py->at(imu), mus_tk_pz->at(imu), 0)) < 0.5
	  && mus_pt->at(imu) >= MuonPTThreshold
	  && hasPFMatch(imu, particleId::muon, pfIdx)
	  && fabs(mus_eta->at(imu)) <= MuonETAThreshold);
}

bool EventHandler::hasPFMatch(int index, particleId::leptonType type, int &pfIdx){
  double deltaRVal = 999.;
  double deltaPT = 999.;
  double leptonEta = 0, leptonPhi = 0, leptonPt = 0;
  if(type == particleId::muon ) {
    leptonEta = mus_eta->at(index);
    leptonPhi = mus_phi->at(index);
    leptonPt = mus_pt->at(index);
  }  else if(type == particleId::electron) {
    leptonEta = els_scEta->at(index);
    leptonPhi = els_phi->at(index);
    leptonPt = els_pt->at(index);
  }
  
  for(unsigned iCand=0; iCand<pfcand_pt->size(); iCand++) {
    //     cout<<"Repetition "<<iCand<<": particleId "<<pfcand_particleId->at(iCand)
    // 	<<", deltaRVal "<<deltaRVal<<", deltaPT "<<deltaPT
    // 	<<", eta "<<pfcand_eta->at(iCand)<<", phi "<<pfcand_phi->at(iCand)<<endl;
    if(pfcand_particleId->at(iCand)==type) {
      double tempDeltaR = Math::GetDeltaR(leptonPhi, leptonEta, pfcand_phi->at(iCand), pfcand_eta->at(iCand));
      if(tempDeltaR < deltaRVal) {
	deltaRVal = tempDeltaR;
	deltaPT = fabs(leptonPt-pfcand_pt->at(iCand));
	pfIdx=iCand;
      }
    }
  }
 
  //   cout<<"Lepton "<<index<<" => type "<<type
  //       <<", leptonEta "<< leptonEta<<", leptonPhi "<< leptonPhi
  //       <<", leptonPt "<< leptonPt<<", deltaPT "<< deltaPT<<endl;
  if(type == particleId::electron) return (deltaPT<10);
  else return (deltaPT<5);
}

float EventHandler::GetRA4MuonIsolation(uint imu){
  if(imu >= mus_pt->size()) return -999;
  double sumEt = mus_pfIsolationR03_sumNeutralHadronEt->at(imu) + mus_pfIsolationR03_sumPhotonEt->at(imu) 
    - 0.5*mus_pfIsolationR03_sumPUPt->at(imu);
  if(sumEt<0.0) sumEt=0.0;
  return (mus_pfIsolationR03_sumChargedHadronPt->at(imu) + sumEt)/mus_pt->at(imu);
}
