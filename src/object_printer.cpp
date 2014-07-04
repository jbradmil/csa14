#include <iostream>
#include <string>
#include <unistd.h>
#include "object_printer.hpp"
#include "cfa.hpp"

using std::cout;
using std::endl;


ObjectPrinter::ObjectPrinter(const std::string& in_file_name):
  cfA(in_file_name, false){
}

void ObjectPrinter::PrintObjects(const unsigned int nentries) {

  //Loop over events
  for(uint i(0); i<nentries; ++i){
    GetEntry(i);
    
    //if (mc_mus_id->size()!=1) continue;
    if (mc_electrons_id->size()==0) continue;

    cout << "*****Event " << i << "*****" << endl;
    /*
    cout << "Printing mc_mus..." << endl;
    for(unsigned int imc = 0; imc < mc_mus_id->size(); imc++){
      //if(abs(mc_mus_id->at(imc)) == pdtlund::mu_minus)
      cout<<imc<<": ID "<<(int)mc_mus_id->at(imc)<<",   \tMom ID "<<(int)mc_mus_mother_id->at(imc)
	  <<",   \tpT "<<mc_mus_pt->at(imc)
	  <<",   \teta "<<mc_mus_eta->at(imc)
	  <<",   \tphi "<<mc_mus_phi->at(imc)
	  <<", \tGMom ID "<<(int)mc_mus_grandmother_id->at(imc)
	  <<", \tGGMom ID "<<(int)mc_mus_ggrandmother_id->at(imc)
	  <<", \tN daughters "<<mc_mus_numOfDaughters->at(imc)<<endl;
    }
    cout<<endl;
  

    cout << "Printing mus..." << endl;
    for(unsigned int imc = 0; imc < mus_pt->size(); imc++){
      //if(abs(mc_mus_id->at(imc)) == pdtlund::mu_minus)
      cout<<imc<<": ID "<<(int)mus_gen_id->at(imc)<<",   \tMom ID "<<(int)mus_gen_mother_id->at(imc)
	  <<",   \treco/gen pT "<< mus_pt->at(imc) << "/" << mus_gen_pt->at(imc)
	  <<",   \treco/gen eta "<< mus_eta->at(imc) << "/" << mus_gen_eta->at(imc)
	  <<",   \treco/gen phi "<< mus_phi->at(imc) << "/" << mus_gen_phi->at(imc)<<endl;
    }
    cout<<endl;
 
    */
    
    cout << "Printing mc_electrons..." << endl;
    for(unsigned int imc = 0; imc < mc_electrons_id->size(); imc++){
      //if(abs(mc_electrons_id->at(imc)) == pdtlund::mu_minus)
      cout<<imc<<": ID "<<(int)mc_electrons_id->at(imc)<<",   \tMom ID "<<(int)mc_electrons_mother_id->at(imc)
	  <<",   \tpT "<<mc_electrons_pt->at(imc)
	  <<",   \teta "<<mc_electrons_eta->at(imc)
	  <<",   \tphi "<<mc_electrons_phi->at(imc)
	  <<", \tGMom ID "<<(int)mc_electrons_grandmother_id->at(imc)
	  <<", \tGGMom ID "<<(int)mc_electrons_ggrandmother_id->at(imc)
	  <<", \tN daughters "<<mc_electrons_numOfDaughters->at(imc)<<endl;
    }
    cout<<endl;
  

    cout << "Printing els..." << endl;
    for(unsigned int imc = 0; imc < els_pt->size(); imc++){
      //if(abs(mc_els_id->at(imc)) == pdtlund::mu_minus)
      cout<<imc<<": ID "<<(int)els_gen_id->at(imc)<<",   \tMom ID "<<(int)els_gen_mother_id->at(imc)
	  <<",   \treco/gen pT "<< els_pt->at(imc) << "/" << els_gen_pt->at(imc)
	  <<",   \treco/gen eta "<< els_eta->at(imc) << "/" << els_gen_eta->at(imc)
	  <<",   \treco/gen phi "<< els_phi->at(imc) << "/" << els_gen_phi->at(imc)<<endl;
    }
    cout<<endl;
    
    /*
    cout << "Printing mc_taus..." << endl;
    for(unsigned int imc = 0; imc < mc_taus_id->size(); imc++){
      //if(abs(mc_taus_id->at(imc)) == pdtlund::mu_minus)
      cout<<imc<<": ID "<<(int)mc_taus_id->at(imc)<<",   \tMom ID "<<(int)mc_taus_mother_id->at(imc)
	  <<",   \tcharge "<<mc_taus_charge->at(imc)
	  <<",   \tpT "<<mc_taus_pt->at(imc)
	  <<",   \teta "<<mc_taus_eta->at(imc)
	  <<",   \tphi "<<mc_taus_phi->at(imc)
	  <<", \tGMom ID "<<(int)mc_taus_grandmother_id->at(imc)
	  <<", \tGGMom ID "<<(int)mc_taus_ggrandmother_id->at(imc)
	  <<", \tN daughters "<<mc_taus_numOfDaughters->at(imc)<<endl;
    }
    cout<<endl;
  

    cout << "Printing taus..." << endl;
    for(unsigned int imc = 0; imc < taus_pt->size(); imc++){
      //if(abs(mc_taus_id->at(imc)) == pdtlund::mu_minus)
      cout<<imc<<": charge " << taus_charge->at(imc) 
	  <<",   \treco pT "<< taus_pt->at(imc) 
	  <<",   \treco eta "<< taus_eta->at(imc) 
	  <<",   \treco phi "<< taus_phi->at(imc)<<endl;
    }
    cout<<endl;
  
    */
  }
}
