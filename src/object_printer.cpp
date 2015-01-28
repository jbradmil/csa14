#include <iostream>
#include <string>
#include <unistd.h>
#include <math.h>
#include "object_printer.hpp"
#include "event_handler.hpp"

using std::cout;
using std::endl;


ObjectPrinter::ObjectPrinter(const std::string& in_file_name, const bool is_list):
  EventHandler(in_file_name, is_list, -1., true){
}

float ObjectPrinter::GetCSAElectronIsolation(const uint iel) const{
  float absiso = els_pfIsolationR03_sumChargedHadronPt->at(iel) + std::max(0.0 , els_pfIsolationR03_sumNeutralHadronEt->at(iel) + els_pfIsolationR03_sumPhotonEt->at(iel) - 0.5 * els_pfIsolationR03_sumPUPt->at(iel) );
  return absiso/els_pt->at(iel);
}

void ObjectPrinter::PrintObjects(const unsigned int nentries) {

  //Loop over events
  for(unsigned int i(0); i<nentries; ++i){
    GetEntry(i);

    cout << "*****Event " << event << "*****" << endl;

    
    cout << "Printing contents of mc_doc..." << endl;
    for(unsigned int imc = 0; imc < mc_doc_id->size(); imc++){
      if (mc_doc_status->at(imc)==3||mc_doc_status->at(imc)==22||mc_doc_status->at(imc)==23||mc_doc_id->at(imc)==2212||fabs(mc_doc_id->at(imc))==15||mc_doc_status->at(imc)==1) {  // hard scatter or final state
	uint id = abs(static_cast<int>(mc_doc_id->at(imc)));
	if (id<=16||id==24) { // just leptons, quarks, and W's
	  // if (!(mc_doc_status->at(imc)==3||mc_doc_status->at(imc)==22||mc_doc_status->at(imc)==23||mc_doc_id->at(imc)==2212)) { // not hard scatter
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
	}
      }
    }
    cout << endl;
      
    
    // Isolated tracks
    // int nisotracks=0;
    // for ( unsigned int itrack = 0 ; itrack < isotk_pt->size() ; ++itrack) {
    //   if ( (isotk_pt->at(itrack) >= 15) &&
    // 	   (isotk_iso->at(itrack) /isotk_pt->at(itrack) < 0.1 ) &&
    // 	   ( fabs(isotk_dzpv->at(itrack)) <0.05) &&
    // 	   (GetMTW(isotk_pt->at(itrack),pfTypeImets_et->at(0),isotk_phi->at(itrack),pfTypeImets_phi->at(0))<100 ) &&
    // 	   ( fabs(isotk_eta->at(itrack)) < 2.4 ) //this is more of a sanity check
    // 	   ){
    // 	cout << "isotk/relIso/dzpv/pt/eta/mT: " << itrack << "/" << isotk_iso->at(itrack) /isotk_pt->at(itrack)  << "/" << isotk_dzpv->at(itrack) << "/" << isotk_pt->at(itrack) << "/" << isotk_eta->at(itrack) << "/" << GetMTW(isotk_pt->at(itrack),pfTypeImets_et->at(0),isotk_phi->at(itrack),pfTypeImets_phi->at(0)) << endl; 
    // 	++nisotracks;
    //   }
    // }
    // cout << "Found " << nisotracks << " isolated tracks." << endl << endl;
    
    // Electron ID for Rishi
    //   uint n_veto_els(0);
    // for(unsigned int iel = 0; iel < els_pt->size(); iel++){
    //   cout << "Electron / passVetoId? " << iel << " / " << isRecoElectron(iel, 0) << endl;
    //   if (isRecoElectron(iel, 0)) n_veto_els++;
    // }
    // cout << "Found " << n_veto_els << " electrons." << endl << endl;


  
    // dump of electron id
    // for(uint iel=0; iel<els_pt->size(); iel++) {
    //   cout << "********Electron " << iel << "********" << endl;
    //   double pt_cut(10.0); //Not actually part of the EGamma ID
    //   double dEtaIn_cut(0.007), phi_cut(0.8), sigmaietaieta_cut(0.01), h_over_e_cut(0.15), d0_cut(0.04), dz_cut(0.2), iso_cut(0.15);
    //   if(fabs(els_scEta->at(iel))<=1.479){ // barrel
    // 	dEtaIn_cut=0.07; phi_cut=0.8; sigmaietaieta_cut=0.01; h_over_e_cut=0.15;
    // 	d0_cut=0.04; dz_cut=0.2; iso_cut=0.15;
    // 	if (cmEnergy>=13) {
    // 	  dEtaIn_cut=0.02; phi_cut=0.2579; sigmaietaieta_cut=0.0125; h_over_e_cut=0.2564;
    // 	  d0_cut=0.025; dz_cut=0.5863; iso_cut=0.3313;     
    // 	}
    //   }else if(fabs(els_scEta->at(iel))>1.479&&fabs(els_scEta->at(iel))<2.5){ // endcap
    // 	dEtaIn_cut=0.01; phi_cut=0.7; sigmaietaieta_cut=0.03; h_over_e_cut=std::numeric_limits<double>::max();
    // 	d0_cut=0.04; dz_cut=0.2; iso_cut=0.15;
    // 	if (cmEnergy>=13) {
    // 	  dEtaIn_cut=0.0141; phi_cut=0.2591; sigmaietaieta_cut=0.0371; h_over_e_cut=0.1335;
    // 	  d0_cut=0.2232; dz_cut=0.9513; iso_cut=0.3816; 
    // 	}
    //   }
    //   if (fabs(els_scEta->at(iel)) >= 2.5 ) continue;
    //   cout << "els_scEta: " << els_scEta->at(iel) << endl;
    //   if (els_pt->at(iel) < pt_cut) continue;
    //   cout << "els_pt: " << els_pt->at(iel) << endl;
    //   if ( fabs(els_dEtaIn->at(iel)) > dEtaIn_cut) continue;
    //   cout << "els_dEtaIn: " << els_dEtaIn->at(iel) << endl;
    //   if ( fabs(els_dPhiIn->at(iel)) > phi_cut) continue;
    //   cout << "els_dPhiIn: " << els_dPhiIn->at(iel) << endl;
    //   if (cmEnergy<13 && els_sigmaIEtaIEta->at(iel) > sigmaietaieta_cut) continue;
    //   if (cmEnergy>=13 && els_full5x5_sigmaIetaIeta->at(iel) > sigmaietaieta_cut) continue;
    //   cout << "els_full5x5_sigmaIetaIeta: " << els_full5x5_sigmaIetaIeta->at(iel) << endl;
    //   if (els_hadOverEm->at(iel) > h_over_e_cut) continue;
    //   cout << "els_hadOverEm: " << els_hadOverEm->at(iel) << endl;
    //   const double beamx(beamSpot_x->at(0)), beamy(beamSpot_y->at(0));
    //   const double d0(els_d0dum->at(iel)-beamx*sin(els_tk_phi->at(iel))+beamy*cos(els_tk_phi->at(iel)));
    //   if ( fabs(d0) >= d0_cut ) continue;
    //   cout << "d0: " << fabs(d0) << endl;
    //   if ( fabs(els_vz->at(iel) - pv_z->at(0) ) >= dz_cut ) continue;
    //   cout << "dZ: " << fabs(els_vz->at(iel) - pv_z->at(0) ) << endl;
    //   float rel_iso(999.);
    //   rel_iso=GetCSAElectronIsolation(iel);
    //   if(rel_iso>=iso_cut) continue;
    //   cout << "rel_iso: " << rel_iso << endl;
    // }


      

    

    
    /*
      cout << "Looking for partons..." << endl;
      for(unsigned int imc = 0; imc < mc_doc_id->size(); imc++){
      if ( (fabs(mc_doc_id->at(imc))>0&&fabs(mc_doc_id->at(imc))<6) || fabs(mc_doc_id->at(imc))==21 ) {
      if (mc_doc_status->at(imc)==3||mc_doc_status->at(imc)==22||mc_doc_status->at(imc)==23) {  // hard scatter
      if ((int)mc_doc_ggrandmother_id->at(imc)==0) continue;
      cout<<imc<<": ID "<<(int)mc_doc_id->at(imc)<<",   \tMom ID "<<(int)mc_doc_mother_id->at(imc)
      <<", \tN mothers "<<mc_doc_numOfMothers->at(imc)
      <<", \tN daughters "<<mc_doc_numOfDaughters->at(imc)
      <<",   \tpx "<<mc_doc_px->at(imc)
      <<",   \tpy "<<mc_doc_py->at(imc)
      <<",   \tpz "<<mc_doc_pz->at(imc)
      <<",   \tpT "<<mc_doc_pt->at(imc)
      <<",   \teta "<<mc_doc_eta->at(imc)
      <<",   \tphi "<<mc_doc_phi->at(imc)
      <<",   \tstatus "<<mc_doc_status->at(imc)
      <<", \tGMom ID "<<(int)mc_doc_grandmother_id->at(imc)
      <<", \tGGMom ID "<<(int)mc_doc_ggrandmother_id->at(imc) <<endl;
      } // hard scatter
      }
      }
      cout << endl;
    */
    /*
      cout << "Printing contents of mc_doc..." << endl;
      for(unsigned int imc = 0; imc < mc_doc_id->size(); imc++){
      cout<<imc<<": ID "<<(int)mc_doc_id->at(imc)<<",   \tMom ID "<<(int)mc_doc_mother_id->at(imc)
      <<",   \tpT "<<mc_doc_pt->at(imc)
      <<",   \teta "<<mc_doc_eta->at(imc)
      <<",   \tphi "<<mc_doc_phi->at(imc)
      <<",   \tstatus "<<mc_doc_status->at(imc)
      <<", \tN daughters "<<mc_doc_numOfDaughters->at(imc)<<endl;
      }
      cout << endl;

    
      cout << "Looking for hard scatter top quarks..." << endl;
      for(unsigned int imc = 0; imc < mc_doc_id->size(); imc++){
      if (fabs(mc_doc_id->at(imc))==6 &&
      (mc_doc_status->at(imc)==3||mc_doc_status->at(imc)==22||mc_doc_status->at(imc)==23)) { // only print these tops with hard scatter status
      cout<<imc<<": ID "<<(int)mc_doc_id->at(imc)<<",   \tMom ID "<<(int)mc_doc_mother_id->at(imc)
      <<",   \tpT "<<mc_doc_pt->at(imc)
      <<",   \teta "<<mc_doc_eta->at(imc)
      <<",   \tphi "<<mc_doc_phi->at(imc)
      <<",   \tstatus "<<mc_doc_status->at(imc)
      <<", \tN daughters "<<mc_doc_numOfDaughters->at(imc)<<endl;
      }
      }
      cout << endl;    
    
      cout << "Looking for Ws and bs (hard scatter)..." << endl;
      for(unsigned int imc = 0; imc < mc_doc_id->size(); imc++){
      if ((fabs(mc_doc_id->at(imc))==5||fabs(mc_doc_id->at(imc))==24)&&(mc_doc_status->at(imc)==3||mc_doc_status->at(imc)==22||mc_doc_status->at(imc)==23)) {
      cout<<imc<<": ID "<<(int)mc_doc_id->at(imc)<<",   \tMom ID "<<(int)mc_doc_mother_id->at(imc)
      <<",   \tpT "<<mc_doc_pt->at(imc)
      <<",   \teta "<<mc_doc_eta->at(imc)
      <<",   \tphi "<<mc_doc_phi->at(imc)
      <<",   \tstatus "<<mc_doc_status->at(imc)
      <<", \tN daughters "<<mc_doc_numOfDaughters->at(imc)<<endl;
      }
      }
      cout << endl;   
    
      cout << "Printing mc_mus..." << endl;
      for(unsigned int imc = 0; imc < mc_mus_id->size(); imc++){
      if (fabs((int)mc_mus_mother_id->at(imc))==24||fabs((int)mc_mus_grandmother_id->at(imc))==24||fabs((int)mc_mus_ggrandmother_id->at(imc))==24)
      cout << "Muon from W..." << endl; 
      cout<<imc<<": ID "<<(int)mc_mus_id->at(imc)<<",   \tMom ID "<<(int)mc_mus_mother_id->at(imc)
      <<",   \tpT "<<mc_mus_pt->at(imc)
      <<",   \teta "<<mc_mus_eta->at(imc)
      <<",   \tphi "<<mc_mus_phi->at(imc)
      <<", \tGMom ID "<<(int)mc_mus_grandmother_id->at(imc)
      <<", \tGGMom ID "<<(int)mc_mus_ggrandmother_id->at(imc)
      <<", \tN daughters "<<mc_mus_numOfDaughters->at(imc)<<endl;
      }
      cout<<endl;
      
      cout << "Printing mc_electrons..." << endl;
      for(unsigned int imc = 0; imc < mc_electrons_id->size(); imc++){
      if (fabs((int)mc_electrons_mother_id->at(imc))==24||fabs((int)mc_electrons_grandmother_id->at(imc))==24||fabs((int)mc_electrons_ggrandmother_id->at(imc))==24)
      cout << "Electron from W..." << endl;
      cout<<imc<<": ID "<<(int)mc_electrons_id->at(imc)<<",   \tMom ID "<<(int)mc_electrons_mother_id->at(imc)
      <<",   \tpT "<<mc_electrons_pt->at(imc)
      <<",   \teta "<<mc_electrons_eta->at(imc)
      <<",   \tphi "<<mc_electrons_phi->at(imc)
      <<", \tGMom ID "<<(int)mc_electrons_grandmother_id->at(imc)
      <<", \tGGMom ID "<<(int)mc_electrons_ggrandmother_id->at(imc)
      <<", \tN daughters "<<mc_electrons_numOfDaughters->at(imc)<<endl;
      }
      cout<<endl;
    */
   
  } // end loop over events
}
