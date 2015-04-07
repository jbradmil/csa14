#include <iostream>
#include <string>
#include <unistd.h>
#include <math.h>
#include "TMath.h"
#include "object_printer.hpp"
#include "event_handler.hpp"

using std::cout;
using std::endl;


ObjectPrinter::ObjectPrinter(const std::string& in_file_name, const bool is_list):
  EventHandler(in_file_name, is_list, -1., false){
}

void ObjectPrinter::PrintObjects(const unsigned int nentries) {

  // cout << "Jet Corrector Paramters?..." << endl;
 //  JetCorrectorParameters *L1JetParameters = new JetCorrectorParameters("JESFiles/PHYS14_V3_MC/PHYS14_V3_MC_L1FastJet_AK4PFchs.txt");
 //  JetCorrectorParameters *L2JetParameters = new JetCorrectorParameters("JESFiles/PHYS14_V3_MC/PHYS14_V3_MC_L2Relative_AK4PFchs.txt");
 //  JetCorrectorParameters *L3JetParameters = new JetCorrectorParameters("JESFiles/PHYS14_V3_MC/PHYS14_V3_MC_L3Absolute_AK4PFchs.txt");
 //
 //  cout << "Level 1: " << endl;
 //  L1JetParameters->printScreen();
 //  cout << "Level 2: " << endl;
 //  L2JetParameters->printScreen();
 //  cout << "Level 3: " << endl;
 //  L3JetParameters->printScreen();
 //
  //Loop over events
  for(unsigned int i(0); i<nentries; ++i){
    GetEntry(i);

    cout << "*****Event " << event << "*****" << endl;

    // for (uint ijet(0); ijet<jets_AKPF_pt->size(); ijet++) {
    //   bool blep = isBLepJet(ijet);
    //   cout << "blep? " << blep << endl;
    // }
    
    //cout << "Printing contents of mc_doc..." << endl;
  //   for(unsigned int imcf = 0; imcf < mc_doc_id->size(); imcf++){
  //     // if (mc_doc_status->at(imcf)==3||mc_doc_status->at(imcf)==22||mc_doc_status->at(imcf)==23||mc_doc_id->at(imcf)==2212||fabs(mc_doc_id->at(imcf))==15||mc_doc_status->at(imcf)==1) {  // hard scatter or final state
  // 	uint id = abs(TMath::Nint(mc_doc_id->at(imcf)));
  // 	uint mom_id = abs(TMath::Nint(mc_doc_mother_id->at(imcf)));
  // 	uint gmom_id = abs(TMath::Nint(mc_doc_grandmother_id->at(imcf)));
  // 	uint ggmom_id = abs(TMath::Nint(mc_doc_ggrandmother_id->at(imcf)));
  // 	bool b_parton(id==5 || mom_id==5 || gmom_id==5 || ggmom_id==5);
  // 	bool b_meson((id>=500&&id<=599) || (mom_id>=500&&mom_id<=599) || (gmom_id>=500&&gmom_id<=599) || (ggmom_id>=500&&ggmom_id<=599));
  // 	bool b_baryon((id>=5000&&id<=5999) || (mom_id>=5000&&mom_id<=5999) || (gmom_id>=5000&&gmom_id<=5999) || (ggmom_id>=5000&&ggmom_id<=5999));
  // 	if (b_parton || b_meson || b_baryon) { // HF content only
  // 	//	if (id<=16||id==24) { // just leptons, quarks, and W's
  // 	  // if (!(mc_doc_status->at(imcf)==3||mc_doc_status->at(imcf)==22||mc_doc_status->at(imcf)==23||mc_doc_id->at(imcf)==2212)) { // not hard scatter
  // cout<<imcf<<": ID "<<id<<", \tMom ID "<<TMath::Nint(mc_doc_mother_id->at(imcf)) <<", \tGMom ID "<<TMath::Nint(mc_doc_grandmother_id->at(imcf)) <<", \tGGMom ID "<<TMath::Nint(mc_doc_ggrandmother_id->at(imcf))
  //     <<", \tN mothers "<<mc_doc_numOfMothers->at(imcf)
  //     <<", \tN daughters "<<mc_doc_numOfDaughters->at(imcf)
  //     <<",   \tpx "<<mc_doc_px->at(imcf)
  //     <<",   \tpy "<<mc_doc_py->at(imcf)
  //     <<",   \tpz "<<mc_doc_pz->at(imcf)
  //   //   <<",   \tpT "<<mc_doc_pt->at(imcf)
  //   //   <<",   \teta "<<mc_doc_eta->at(imcf)
  //   //  <<",   \tphi "<<mc_doc_phi->at(imcf)
  //     <<", \tstatus "<< TMath::Nint(mc_doc_status->at(imcf)) <<endl;
  // 	}
  // //  }
  //   }
  //   cout << endl;

    // std::vector<int> true_els;
    // std::vector<int> true_mus;
    // std::vector<int> true_had_taus;
    // std::vector<int> tau_descendants;
    // std::vector<int> true_had_tau_kids;
    // std::vector<int> true_lep_taus;
    // GetTrueLeptons(true_els, true_mus, true_had_taus, true_lep_taus);

    // cout << "Hadronic taus... " << endl;
    // for (uint itau(0); itau<true_had_taus.size(); itau++) {
    //   int id = TMath::Nint(mc_doc_id->at(true_had_taus[itau]));
    //   cout<<true_had_taus[itau]<<": ID "<<id<<", \tMom ID "<<TMath::Nint(mc_doc_mother_id->at(true_had_taus[itau])) <<", \tGMom ID "<<TMath::Nint(mc_doc_grandmother_id->at(true_had_taus[itau])) <<", \tGGMom ID "<<TMath::Nint(mc_doc_ggrandmother_id->at(true_had_taus[itau]))
    //   <<", \tN mothers "<<mc_doc_numOfMothers->at(true_had_taus[itau])
    //   <<", \tN daughters "<<mc_doc_numOfDaughters->at(true_had_taus[itau])
    //   <<",   \tpx "<<mc_doc_px->at(true_had_taus[itau])
    //   <<",   \tpy "<<mc_doc_py->at(true_had_taus[itau])
    //   <<",   \tpz "<<mc_doc_pz->at(true_had_taus[itau])
    //   <<", \tstatus "<< TMath::Nint(mc_doc_status->at(true_had_taus[itau])) <<endl;
    //   cout <<  "Look for kids in doc..." << endl;
    //   for(unsigned int imc = itau; imc < mc_doc_id->size(); imc++){
    // 	//if (abs(TMath::Nint(mc_doc_id->at(imc) ))<25) continue;
    // 	if (TMath::Nint(mc_doc_mother_id->at(imc) )==id||TMath::Nint(mc_doc_grandmother_id->at(imc) )==id||TMath::Nint(mc_doc_ggrandmother_id->at(imc) )==id) {
    // 	  cout<<imc<<": ID "<<TMath::Nint(mc_doc_id->at(imc) )<<", \tMom ID "<<TMath::Nint(mc_doc_mother_id->at(imc)) <<", \tGMom ID "<<TMath::Nint(mc_doc_grandmother_id->at(imc)) <<", \tGGMom ID "<<TMath::Nint(mc_doc_ggrandmother_id->at(imc))
    // 	      <<", \tN mothers "<<mc_doc_numOfMothers->at(imc)
    // 	      <<", \tN daughters "<<mc_doc_numOfDaughters->at(imc)
    // 	      <<",   \tpx "<<mc_doc_px->at(imc)
    // 	      <<",   \tpy "<<mc_doc_py->at(imc)
    // 	      <<",   \tpz "<<mc_doc_pz->at(imc)
    // 	      <<", \tstatus "<< TMath::Nint(mc_doc_status->at(imc)) <<endl;
    // 	  tau_descendants.push_back(imc);
    // 	} 
    //   }
    //   cout <<  "Look for kids in final..." << endl;
    //   for(unsigned int imcf = 0; imcf < mc_final_id->size(); imcf++){
    // 	if (abs(TMath::Nint(mc_final_id->at(imcf)) )<25||TMath::Nint(mc_final_id->at(imcf)==111)) continue;
    // 	for (uint ides(0); ides<tau_descendants.size(); ides++) {
    // 	  int idd = TMath::Nint(tau_descendants[ides]);
    // 	  if (TMath::Nint(mc_final_mother_id->at(imcf) )==idd||TMath::Nint(mc_final_grandmother_id->at(imcf) )==idd||TMath::Nint(mc_final_ggrandmother_id->at(imcf) )==idd ||
    // 	      TMath::Nint(mc_final_mother_id->at(imcf) )==id||TMath::Nint(mc_final_grandmother_id->at(imcf) )==id||TMath::Nint(mc_final_ggrandmother_id->at(imcf) )==id) {
    // 	    cout<<imcf<<": ID "<<TMath::Nint(mc_final_id->at(imcf) )<<", \tMom ID "<<TMath::Nint(mc_final_mother_id->at(imcf)) <<", \tGMom ID "<<TMath::Nint(mc_final_grandmother_id->at(imcf)) <<", \tGGMom ID "<<TMath::Nint(mc_final_ggrandmother_id->at(imcf))
    // 		<<", \tN mothers "<<mc_final_numOfMothers->at(imcf)
    // 		<<",   \tpx "<< mc_final_pt->at(imcf)*cos(mc_final_phi->at(imcf))
    // 		<<",   \tpy "<< mc_final_pt->at(imcf)*sin(mc_final_phi->at(imcf)) << endl;
    // 	    true_had_tau_kids.push_back(imcf);
    // 	    break;
    // 	  }
    // 	}
    //   }
    // }

    // look

   //  cout << "Printing contents of mc_doc..." << endl;
  //   for(unsigned int imcf = 0; imcf < mc_doc_id->size(); imcf++){
  //     if (mc_doc_status->at(imcf)==3||mc_doc_status->at(imcf)==22||mc_doc_status->at(imcf)==23||mc_doc_id->at(imcf)==2212||fabs(mc_doc_id->at(imcf))==15||mc_doc_status->at(imcf)==1) {  // hard scatter or final state
  // 	uint id = abs(TMath::Nint(mc_doc_id->at(imcf)));
  // 	if (id<=16||id==24) { // just leptons, quarks, and W's
  // 	  // if (!(mc_doc_status->at(imcf)==3||mc_doc_status->at(imcf)==22||mc_doc_status->at(imcf)==23||mc_doc_id->at(imcf)==2212)) { // not hard scatter
  // cout<<imcf<<": ID "<<TMath::Nint(mc_doc_id->at(imcf))<<", \tMom ID "<<TMath::Nint(mc_doc_mother_id->at(imcf)) <<", \tGMom ID "<<TMath::Nint(mc_doc_grandmother_id->at(imcf)) <<", \tGGMom ID "<<TMath::Nint(mc_doc_ggrandmother_id->at(imcf))
  //     <<", \tN mothers "<<mc_doc_numOfMothers->at(imcf)
  //     <<", \tN daughters "<<mc_doc_numOfDaughters->at(imcf)
  //     <<",   \tpx "<<mc_doc_px->at(imcf)
  //     <<",   \tpy "<<mc_doc_py->at(imcf)
  //     <<",   \tpz "<<mc_doc_pz->at(imcf)
  //   //   <<",   \tpT "<<mc_doc_pt->at(imcf)
  //   //   <<",   \teta "<<mc_doc_eta->at(imcf)
  //   //  <<",   \tphi "<<mc_doc_phi->at(imcf)
  //     <<", \tstatus "<< TMath::Nint(mc_doc_status->at(imcf)) <<endl;
  // 	}
  //     }
  //   }
  //   cout << endl;
    
    // // Electron ID for Rishi
    //   uint n_veto_els(0);
    // for(unsigned int iel = 0; iel < els_pt->size(); iel++){
    //   cout << "Electron / passVetoId? " << iel << " / " << isRecoElectron(iel, 0) << endl;
    //   if (!isRecoElectron(iel, 0)) continue;
    //   n_veto_els++;
    //   printf("\t pt/eta/relIso: %3.2f/%3.2f/%3.2f\n", els_pt->at(iel), els_scEta->at(iel), GetCSAElectronIsolation(iel)); 
    // }
    // cout << "Found " << n_veto_els << " electrons." << endl << endl;
    
    

      
    
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
      for(unsigned int imcf = 0; imcf < mc_doc_id->size(); imcf++){
      if ( (fabs(mc_doc_id->at(imcf))>0&&fabs(mc_doc_id->at(imcf))<6) || fabs(mc_doc_id->at(imcf))==21 ) {
      if (mc_doc_status->at(imcf)==3||mc_doc_status->at(imcf)==22||mc_doc_status->at(imcf)==23) {  // hard scatter
      if ((int)mc_doc_ggrandmother_id->at(imcf)==0) continue;
      cout<<imcf<<": ID "<<(int)mc_doc_id->at(imcf)<<",   \tMom ID "<<(int)mc_doc_mother_id->at(imcf)
      <<", \tN mothers "<<mc_doc_numOfMothers->at(imcf)
      <<", \tN daughters "<<mc_doc_numOfDaughters->at(imcf)
      <<",   \tpx "<<mc_doc_px->at(imcf)
      <<",   \tpy "<<mc_doc_py->at(imcf)
      <<",   \tpz "<<mc_doc_pz->at(imcf)
      <<",   \tpT "<<mc_doc_pt->at(imcf)
      <<",   \teta "<<mc_doc_eta->at(imcf)
      <<",   \tphi "<<mc_doc_phi->at(imcf)
      <<",   \tstatus "<<mc_doc_status->at(imcf)
      <<", \tGMom ID "<<(int)mc_doc_grandmother_id->at(imcf)
      <<", \tGGMom ID "<<(int)mc_doc_ggrandmother_id->at(imcf) <<endl;
      } // hard scatter
      }
      }
      cout << endl;
    */
    /*
      cout << "Printing contents of mc_doc..." << endl;
      for(unsigned int imcf = 0; imcf < mc_doc_id->size(); imcf++){
      cout<<imcf<<": ID "<<(int)mc_doc_id->at(imcf)<<",   \tMom ID "<<(int)mc_doc_mother_id->at(imcf)
      <<",   \tpT "<<mc_doc_pt->at(imcf)
      <<",   \teta "<<mc_doc_eta->at(imcf)
      <<",   \tphi "<<mc_doc_phi->at(imcf)
      <<",   \tstatus "<<mc_doc_status->at(imcf)
      <<", \tN daughters "<<mc_doc_numOfDaughters->at(imcf)<<endl;
      }
      cout << endl;

    
      cout << "Looking for hard scatter top quarks..." << endl;
      for(unsigned int imcf = 0; imcf < mc_doc_id->size(); imcf++){
      if (fabs(mc_doc_id->at(imcf))==6 &&
      (mc_doc_status->at(imcf)==3||mc_doc_status->at(imcf)==22||mc_doc_status->at(imcf)==23)) { // only print these tops with hard scatter status
      cout<<imcf<<": ID "<<(int)mc_doc_id->at(imcf)<<",   \tMom ID "<<(int)mc_doc_mother_id->at(imcf)
      <<",   \tpT "<<mc_doc_pt->at(imcf)
      <<",   \teta "<<mc_doc_eta->at(imcf)
      <<",   \tphi "<<mc_doc_phi->at(imcf)
      <<",   \tstatus "<<mc_doc_status->at(imcf)
      <<", \tN daughters "<<mc_doc_numOfDaughters->at(imcf)<<endl;
      }
      }
      cout << endl;    
    
      cout << "Looking for Ws and bs (hard scatter)..." << endl;
      for(unsigned int imcf = 0; imcf < mc_doc_id->size(); imcf++){
      if ((fabs(mc_doc_id->at(imcf))==5||fabs(mc_doc_id->at(imcf))==24)&&(mc_doc_status->at(imcf)==3||mc_doc_status->at(imcf)==22||mc_doc_status->at(imcf)==23)) {
      cout<<imcf<<": ID "<<(int)mc_doc_id->at(imcf)<<",   \tMom ID "<<(int)mc_doc_mother_id->at(imcf)
      <<",   \tpT "<<mc_doc_pt->at(imcf)
      <<",   \teta "<<mc_doc_eta->at(imcf)
      <<",   \tphi "<<mc_doc_phi->at(imcf)
      <<",   \tstatus "<<mc_doc_status->at(imcf)
      <<", \tN daughters "<<mc_doc_numOfDaughters->at(imcf)<<endl;
      }
      }
      cout << endl;   
    
      cout << "Printing mc_mus..." << endl;
      for(unsigned int imcf = 0; imcf < mc_mus_id->size(); imcf++){
      if (fabs((int)mc_mus_mother_id->at(imcf))==24||fabs((int)mc_mus_grandmother_id->at(imcf))==24||fabs((int)mc_mus_ggrandmother_id->at(imcf))==24)
      cout << "Muon from W..." << endl; 
      cout<<imcf<<": ID "<<(int)mc_mus_id->at(imcf)<<",   \tMom ID "<<(int)mc_mus_mother_id->at(imcf)
      <<",   \tpT "<<mc_mus_pt->at(imcf)
      <<",   \teta "<<mc_mus_eta->at(imcf)
      <<",   \tphi "<<mc_mus_phi->at(imcf)
      <<", \tGMom ID "<<(int)mc_mus_grandmother_id->at(imcf)
      <<", \tGGMom ID "<<(int)mc_mus_ggrandmother_id->at(imcf)
      <<", \tN daughters "<<mc_mus_numOfDaughters->at(imcf)<<endl;
      }
      cout<<endl;
      
      cout << "Printing mc_electrons..." << endl;
      for(unsigned int imcf = 0; imcf < mc_electrons_id->size(); imcf++){
      if (fabs((int)mc_electrons_mother_id->at(imcf))==24||fabs((int)mc_electrons_grandmother_id->at(imcf))==24||fabs((int)mc_electrons_ggrandmother_id->at(imcf))==24)
      cout << "Electron from W..." << endl;
      cout<<imcf<<": ID "<<(int)mc_electrons_id->at(imcf)<<",   \tMom ID "<<(int)mc_electrons_mother_id->at(imcf)
      <<",   \tpT "<<mc_electrons_pt->at(imcf)
      <<",   \teta "<<mc_electrons_eta->at(imcf)
      <<",   \tphi "<<mc_electrons_phi->at(imcf)
      <<", \tGMom ID "<<(int)mc_electrons_grandmother_id->at(imcf)
      <<", \tGGMom ID "<<(int)mc_electrons_ggrandmother_id->at(imcf)
      <<", \tN daughters "<<mc_electrons_numOfDaughters->at(imcf)<<endl;
      }
      cout<<endl;
    */
   
  } // end loop over events
}
