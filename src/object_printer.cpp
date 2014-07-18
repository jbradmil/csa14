#include <iostream>
#include <string>
#include <unistd.h>
#include <math.h>
#include "object_printer.hpp"

using std::cout;
using std::endl;


ObjectPrinter::ObjectPrinter(const std::string& in_file_name):
  cfA(in_file_name, false){
}

void ObjectPrinter::PrintObjects(const unsigned int nentries) {

  //Loop over events
  for(unsigned int i(0); i<nentries; ++i){
    GetEntry(i);

    cout << "*****Event " << i << "*****" << endl;
  
    /*  
    cout << "Printing contents of mc_doc..." << endl;
    for(unsigned int imc = 0; imc < mc_doc_id->size(); imc++){
      if (mc_doc_status->at(imc)==3||mc_doc_status->at(imc)==22||mc_doc_status->at(imc)==23) {  // hard scatter
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
      }
    }
    cout << endl;
    
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
      cout << "Printing contents of mc_final..." << endl;
      for(unsigned int imc = 0; imc < mc_final_id->size(); imc++){
      cout<<imc<<": ID "<<(int)mc_final_id->at(imc)<<",   \tMom ID "<<(int)mc_final_mother_id->at(imc)
      <<",   \tpT "<<mc_final_pt->at(imc)
      <<",   \teta "<<mc_final_eta->at(imc)
      <<",   \tphi "<<mc_final_phi->at(imc)
      <<",   \tstatus "<<mc_final_status->at(imc)
      <<", \tN daughters "<<mc_final_numOfDaughters->at(imc)<<endl;
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
    */
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
      /*
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
