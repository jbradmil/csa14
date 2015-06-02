#define Expectation_cxx
// The class definition in Expectation.h has been generated automatically
// by the ROOT utility TTree::MakeSelector(). This class is derived
// from the ROOT class TSelector. For more information on the TSelector
// framework see $ROOTSYS/README/README.SELECTOR or the ROOT User Manual.

// The following methods are defined in this file:
//    Begin():        called every time a loop on the tree starts,
//                    a convenient place to create your histograms.
//    SlaveBegin():   called after Begin(), when on PROOF called only on the
//                    slave servers.
//    Process():      called for each event, in this function you decide what
//                    to read and fill your histograms.
//    SlaveTerminate: called at the end of the loop on the tree, when on PROOF
//                    called only on the slave servers.
//    Terminate():    called at the end of the loop on the tree,
//                    a convenient place to draw/fit your histograms.
//
// To use this file, try the following session on your Tree T:
//
// Root > T->Process("Expectation.C")
// Root > T->Process("Expectation.C","some options")
// Root > T->Process("Expectation.C+")
//

#include "Expectation.h"
#include <TH2.h>
#include <TStyle.h>


void Expectation::Begin(TTree * /*tree*/)
{
 
  TString option = GetOption();
  tExpectation_=NULL;

}

void Expectation::SlaveBegin(TTree * /*tree*/)
{
  // The SlaveBegin() function is called after the Begin() function.
  // When running with PROOF SlaveBegin() is called on each slave server.
  // The tree argument is deprecated (on PROOF 0 is passed).

  TString option = GetOption();
  // tree
  tExpectation_ = new TTree("LostLeptonExpectation","a simple Tree with simple variables");
  // 	tExpectation_->SetAutoSave(10000000000);
  //  tExpectation_->SetAutoFlush(1000000);
  tExpectation_->Branch("EvtNum",&event,"EvtNum/i");
  tExpectation_->Branch("HT",&ht30,"HT/F");
  tExpectation_->Branch("MHT",&mht30,"MHT/F");
  tExpectation_->Branch("NJets",&num_jets_pt30,"NJets/s");
  tExpectation_->Branch("BTags",&num_csvm_jets30,"BTags/s");
  tExpectation_->Branch("Bin",&Bin_,"Bin/s");
  tExpectation_->Branch("NVtx",&num_primary_vertices,"NVtx/s");
  tExpectation_->Branch("minDeltaPhiN",&min_delta_phi_met_N,"minDeltaPhiN/F");
  tExpectation_->Branch("Weight", &weightppb, "Weight/F");
  tExpectation_->Branch("MET",&met,"MET/F");
  tExpectation_->Branch("METPhi",&met_phi,"METPhi/F");
  tExpectation_->Branch("MTW",&MTW,"MTW/F");  

  tExpectation_->Branch("selectedIDIsoElectronsNum", &num_veto_els, "selectedIDIsoElectronsNum/s");
  tExpectation_->Branch("selectedIDIsoMuonsNum", &num_veto_mus, "selectedIDIsoMuonsNum/s");

  tExpectation_->Branch("GenElecNum", &num_true_els, "GenElecNum/s");
  tExpectation_->Branch("GenMuNum", &num_true_mus, "GenMuNum/s");

  tExpectation_->Branch("Expectation",&Expectation_,"Expectation/s");
  tExpectation_->Branch("ExpectationDiLep",&ExpectationDiLep_,"ExpectationDiLep/s");

  tExpectation_->Branch("MuDiLepControlSample",&MuDiLepControlSample_,"MuDiLepControlSample/s");
  tExpectation_->Branch("ElecDiLepControlSample",&ElecDiLepControlSample_,"ElecDiLepControlSample/s");

  //  tExpectation_->Branch("muMTW",&muMTW,"muMTW/s");
  tExpectation_->Branch("muAcc",&muAcc,"muAcc/s");
  tExpectation_->Branch("muReco",&muReco,"muReco/s");
  tExpectation_->Branch("muIso",&muIso,"muIso/s");
  tExpectation_->Branch("MuPurity",&MuPurity_,"MuPurity/s");

  //  tExpectation_->Branch("elecMTW",&elecMTW,"elecMTW/s");
  tExpectation_->Branch("elecAcc",&elecAcc,"elecAcc/s");
  tExpectation_->Branch("elecReco",&elecReco,"elecReco/s");
  tExpectation_->Branch("elecIso",&elecIso,"elecIso/s");
  tExpectation_->Branch("ElecPurity",&ElecPurity_,"ElecPurity/s");

  tExpectation_->Branch("mu_pt",&mu_pt);
  tExpectation_->Branch("mu_veto",&mu_veto);
  tExpectation_->Branch("mu_mT",&mu_mT);
  tExpectation_->Branch("mu_tm",&mu_tm);
  tExpectation_->Branch("mu_activity_mu",&mu_activity_mu);
  tExpectation_->Branch("mu_activity_el",&mu_activity_el);
  tExpectation_->Branch("true_mu_pt",&true_mu_pt);
  tExpectation_->Branch("true_mu_activity_mu",&true_mu_activity_mu);
  tExpectation_->Branch("true_mu_activity_el",&true_mu_activity_el);

  tExpectation_->Branch("el_pt",&el_pt);
  tExpectation_->Branch("el_veto",&el_veto);
  tExpectation_->Branch("el_mT",&el_mT);
  tExpectation_->Branch("el_tm",&el_tm);
  tExpectation_->Branch("el_activity_mu",&el_activity_mu);
  tExpectation_->Branch("el_activity_el",&el_activity_el);
  tExpectation_->Branch("true_el_pt",&true_el_pt);
  tExpectation_->Branch("true_el_activity_mu",&true_el_activity_mu);
  tExpectation_->Branch("true_el_activity_el",&true_el_activity_el);

  
  GetOutputList()->Add(tExpectation_);
  SearchBins_ = new SearchBins();
  std::cout<<"Applying filters: "<<applyFilters_<<std::endl;
  
}

Bool_t Expectation::Process(Long64_t entry)
{

  resetValues();
  fChain->GetTree()->GetEntry(entry);
  weightppb*=10000;
  double Weight=weightppb;
  
  double HT=ht30, MHT=mht30;
  int NJets=num_jets_pt30, BTags=num_csvm_jets30;
  double minDeltaPhiN=min_delta_phi_met_N;
  
  if(HT<minHT_ || MHT< minMHT_ || NJets < minNJets_  )
    {
      return kTRUE;
    }
  if(minDeltaPhiN<minDeltaPhiN_) return kTRUE;
  //if(applyFilters_ &&  !FiltersPass() ) return kTRUE;
  if (BTags<2) return kTRUE;
  Bin_ = SearchBins_->GetBinNumber(HT,MHT,NJets,BTags);

  int GenMuNum=num_true_mus, GenElecNum=num_true_els;
  int selectedIDIsoMuonsNum=num_veto_mus, selectedIDIsoElectronsNum=num_veto_els;

  // find leading veto mu and elec
  for (unsigned int imu(0); imu<mu_pt->size(); imu++) {
    if (mu_veto->at(imu)) {
      vetoMu=imu;
      break;
    }
  }
  for (unsigned int iel(0); iel<el_pt->size(); iel++) {
    if (el_veto->at(iel)) {
      vetoElec=iel;
      break;
    }
  }

  // cout << "GenMuNum=" << GenMuNum << ", GenElecNum=" << GenElecNum << endl;
  // cout << "selectedIDIsoMuonsNum=" << num_veto_mus << ", selectedIDIsoElectronsNum=" << num_veto_els << endl;
  // cout << "vetoMu=" << vetoMu << ", vetoElec=" << vetoElec << endl;
  
  if( (GenMuNum==1 && GenElecNum==0)) {
    if ( true_mu_pt->at(0) < minMuPt_ || std::abs(true_mu_eta->at(0)) > maxMuEta_) {
      muAcc=0;
      Expectation_=1;
    } else {
      muAcc=2;
      bool RecoNotMatched=true;
      if (true_mu_reco_ind->at(0)>=0&&true_mu_passID->at(0)) {
	RecoNotMatched=false;
	muReco =2;
	bool IsoNotMatched=true;
	if(true_mu_mini_iso->at(0)<0.2) {
	  // cout << "Found veto mu" << endl;
	  IsoNotMatched=false;
	  muIso=2;
	  Expectation_=2;
	  if (vetoMu>=0) MTW=mu_mT->at(vetoMu);
	  // muMTW=mu_mT->at(vetoMu);
	  MuDiLepControlSample_=2;
	}
	if(IsoNotMatched) {
	  muIso=0;
	  Expectation_=1;
	}
      }
      if(RecoNotMatched) {
	muReco=0;
	Expectation_=1;
      }
    }      
  } else if (GenMuNum==0 && GenElecNum==1) {
    if ( true_el_pt->at(0) < 10. || std::abs(true_el_eta->at(0)) > 2.5) {
      elecAcc=0;
      Expectation_=1;
    } else {
      elecAcc=2;
      bool RecoNotMatched=true;
      if (true_el_reco_ind->at(0)>=0&&true_el_passID->at(0)) {
	RecoNotMatched=false;
	elecReco =2;
	bool IsoNotMatched=true;
	if(true_el_mini_iso->at(0)<0.1) {
	  // cout << "Found veto el" << endl;
	  IsoNotMatched=false;
	  elecIso=2;
	  Expectation_=2;
	  if (vetoElec>=0) MTW=el_mT->at(vetoElec);
	  // elecMTW=el_mT->at(vetoElec);
	  ElecDiLepControlSample_=2;
	}
	if(IsoNotMatched) {
	  elecIso=0;
	  Expectation_=1;
	}
      }
      if(RecoNotMatched) {
	elecReco=0;
	Expectation_=1;
      }
    }  
  } else if (num_true_mus+num_true_els==2) {  // dilepton
    if(num_veto_mus==0 && num_veto_els==0) {
      Expectation_=1;
      ExpectationDiLep_=1;
    } else if(num_veto_mus==1 && num_veto_els==0) {
      // cout << "Found veto mu" << endl;
    MuDiLepControlSample_=0;
      if (vetoMu>=0) MTW=mu_mT->at(vetoMu);
      // muMTW=mu_mT->at(vetoMu);
    } else if(num_veto_mus==0 && num_veto_els==1) {
      // cout << "Found veto el" << endl;
      ElecDiLepControlSample_=0;
      if (vetoElec>=0) MTW=el_mT->at(vetoElec);
      // elecMTW=el_mT->at(vetoElec);
    }
  }

  // new purity studies
  if(selectedIDIsoMuonsNum==1 && selectedIDIsoElectronsNum==0 && (GenMuNum+ GenElecNum)<2) {
      if(GenMuNum<1) MuPurity_=0;
      else MuPurity_=2;
    }
  if(selectedIDIsoMuonsNum==0 && selectedIDIsoElectronsNum==1 && (GenMuNum+ GenElecNum)<2) {
      if(GenElecNum<1) ElecPurity_=0;
      else ElecPurity_=2;
  }

  
  tExpectation_->Fill();
  return kTRUE;
}

void Expectation::SlaveTerminate()
{
  // The SlaveTerminate() function is called after all entries or objects
  // have been processed. When running with PROOF SlaveTerminate() is called
  // on each slave server.

}

void Expectation::Terminate()
{

  GetOutputList()->Print();
  std::cout << "tExpectation_:" << tExpectation_ << '\n';
  tExpectation_ = dynamic_cast<TTree*>(GetOutputList()->FindObject("LostLeptonExpectation"));
  std::cout << "tExpectation_:" << tExpectation_ << '\n';
	
  TFile *outPutFile = new TFile("macros/junews/lostleptons/arne/Expectation/Expectation_data.root","RECREATE"); 
  outPutFile->cd();
  tExpectation_->Write();
  outPutFile->Close();
  
}

void Expectation::resetValues()
{
  Expectation_=0;
  muIso =1;
  muReco =1;
  muAcc =1;
  // muMTW =1;
  muTotal=1;
  elecIso =1;
  elecReco =1;
  elecAcc =1;
  elecTotal=1;
  // elecMTW=1;
  ExpectationDiLep_=0;
  MuDiLepControlSample_=1;
  ElecDiLepControlSample_=1;
  MuPurity_=1;
  ElecPurity_=1;
  MTW=-99.;
  vetoMu=-1;
  vetoElec=-1;
}


SearchBins::SearchBins()
{
  
  // HTmin,HTmax,MHTmin,MHTmax,NJetsmin,NJetsmax,BTagsmin,BTagsmax
  // NJets 4,6 BTags=0
  // fixed ht Njets btags all MHT bins
  // bins_.push_back( Bin(500,800,200,500,4,6,-1,0) );
  // bins_.push_back( Bin(800,1200,200,500,4,6,-1,0) );
  // bins_.push_back( Bin(1200,99999,200,500,4,6,-1,0) );
  
  // bins_.push_back( Bin(500,1200,500,750,4,6,-1,0) );
  // bins_.push_back( Bin(1200,99999,500,750,4,6,-1,0) );
  
  // bins_.push_back( Bin(800,99999,750,9999,4,6,-1,0) );
  
  // // NJewts 7,8 BTags=0
  // bins_.push_back( Bin(500,800,200,500,7,8,-1,0) );
  // bins_.push_back( Bin(800,1200,200,500,7,8,-1,0) );
  // bins_.push_back( Bin(1200,99999,200,500,7,8,-1,0) );
  
  // bins_.push_back( Bin(500,1200,500,750,7,8,-1,0) );
  // bins_.push_back( Bin(1200,99999,500,750,7,8,-1,0) );
  
  // bins_.push_back( Bin(800,99999,750,9999,7,8,-1,0) );
  
  
  // // NJewts 9,9999 BTags=0
  // bins_.push_back( Bin(500,800,200,500,9,9999,-1,0) );
  // bins_.push_back( Bin(800,1200,200,500,9,9999,-1,0) );
  // bins_.push_back( Bin(1200,99999,200,500,9,9999,-1,0) );
  
  // bins_.push_back( Bin(500,1200,500,750,9,9999,-1,0) );
  // bins_.push_back( Bin(1200,99999,500,750,9,9999,-1,0) );
  
  // bins_.push_back( Bin(800,99999,750,9999,9,9999,-1,0) );
  
  
  
  // // NJets 4,6 BTags=1
  // // fixed ht Njets btags all MHT bins
  // bins_.push_back( Bin(500,800,200,500,4,6,1,1) );
  // bins_.push_back( Bin(800,1200,200,500,4,6,1,1) );
  // bins_.push_back( Bin(1200,99999,200,500,4,6,1,1) );
  
  // bins_.push_back( Bin(500,1200,500,750,4,6,1,1) );
  // bins_.push_back( Bin(1200,99999,500,750,4,6,1,1) );
  
  // bins_.push_back( Bin(800,99999,750,9999,4,6,1,1) );
  
  // // NJewts 7,8 BTags=0
  // bins_.push_back( Bin(500,800,200,500,7,8,1,1) );
  // bins_.push_back( Bin(800,1200,200,500,7,8,1,1) );
  // bins_.push_back( Bin(1200,99999,200,500,7,8,1,1) );
  
  // bins_.push_back( Bin(500,1200,500,750,7,8,1,1) );
  // bins_.push_back( Bin(1200,99999,500,750,7,8,1,1) );
  
  // bins_.push_back( Bin(800,99999,750,9999,7,8,1,1) );
  
  
  // // NJewts 9,9999 BTags=1
  // bins_.push_back( Bin(500,800,200,500,9,9999,1,1) );
  // bins_.push_back( Bin(800,1200,200,500,9,9999,1,1) );
  // bins_.push_back( Bin(1200,99999,200,500,9,9999,1,1) );
  
  // bins_.push_back( Bin(500,1200,500,750,9,9999,1,1) );
  // bins_.push_back( Bin(1200,99999,500,750,9,9999,1,1) );
  
  // bins_.push_back( Bin(800,99999,750,9999,9,9999,1,1) );
  
  
  
  // NJets 4,6 BTags=2
  // fixed ht Njets btags all MHT bins
  bins_.push_back( Bin(500,800,200,500,4,6,2,2) );
  bins_.push_back( Bin(800,1200,200,500,4,6,2,2) );
  bins_.push_back( Bin(1200,99999,200,500,4,6,2,2) );
  
  bins_.push_back( Bin(500,1200,500,750,4,6,2,2) );
  bins_.push_back( Bin(1200,99999,500,750,4,6,2,2) );
  
  bins_.push_back( Bin(800,99999,750,9999,4,6,2,2) );
  
  // NJewts 7,8 BTags=2
  bins_.push_back( Bin(500,800,200,500,7,8,2,2) );
  bins_.push_back( Bin(800,1200,200,500,7,8,2,2) );
  bins_.push_back( Bin(1200,99999,200,500,7,8,2,2) );
  
  bins_.push_back( Bin(500,1200,500,750,7,8,2,2) );
  bins_.push_back( Bin(1200,99999,500,750,7,8,2,2) );
  
  bins_.push_back( Bin(800,99999,750,9999,7,8,2,2) );
  
  
  // NJewts 9,9999 BTags=2
  bins_.push_back( Bin(500,800,200,500,9,9999,2,2) );
  bins_.push_back( Bin(800,1200,200,500,9,9999,2,2) );
  bins_.push_back( Bin(1200,99999,200,500,9,9999,2,2) );
  
  bins_.push_back( Bin(500,1200,500,750,9,9999,2,2) );
  bins_.push_back( Bin(1200,99999,500,750,9,9999,2,2) );
  
  bins_.push_back( Bin(800,99999,750,9999,9,9999,2,2) );
  
  
  // NJets 4,6 BTags=>3
  // fixed ht Njets btags all MHT bins
  bins_.push_back( Bin(500,800,200,500,4,6,3,9999) );
  bins_.push_back( Bin(800,1200,200,500,4,6,3,9999) );
  bins_.push_back( Bin(1200,99999,200,500,4,6,3,9999) );
  
  bins_.push_back( Bin(500,1200,500,750,4,6,3,9999) );
  bins_.push_back( Bin(1200,99999,500,750,4,6,3,9999) );
  
  bins_.push_back( Bin(800,99999,750,9999,4,6,3,9999) );
  
  // NJewts 7,8 BTags=>3
  bins_.push_back( Bin(500,800,200,500,7,8,3,9999) );
  bins_.push_back( Bin(800,1200,200,500,7,8,3,9999) );
  bins_.push_back( Bin(1200,99999,200,500,7,8,3,9999) );
  
  bins_.push_back( Bin(500,1200,500,750,7,8,3,9999) );
  bins_.push_back( Bin(1200,99999,500,750,7,8,3,9999) );
  
  bins_.push_back( Bin(800,99999,750,9999,7,8,3,9999) );
  
  
  // NJewts 9,9999 BTags=>3
  bins_.push_back( Bin(500,800,200,500,9,9999,3,9999) );
  bins_.push_back( Bin(800,1200,200,500,9,9999,3,9999) );
  bins_.push_back( Bin(1200,99999,200,500,9,9999,3,9999) );
  
  bins_.push_back( Bin(500,1200,500,750,9,9999,3,9999) );
  bins_.push_back( Bin(1200,99999,500,750,9,9999,3,9999) );
  
  bins_.push_back( Bin(800,99999,750,9999,9,9999,3,9999) );
  std::cout<<"Loaded bins: "<<bins_.size()<<std::endl;
  for(unsigned int i=0; i<bins_.size();i++)
    {
      usedBin_.push_back(0); 
    }
}
void SearchBins::PrintUsed()
{
  for(unsigned int i=0; i< usedBin_.size();i++) std::cout<<"Bin["<<i<<"] has been used: "<<usedBin_[i]<<std::endl; 
}

unsigned int SearchBins::GetBinNumber(double HT, double MHT, int NJets, int BTags)
{
  unsigned int result =999;
  int match =-1;
  for(unsigned int i=0; i<bins_.size();i++)
    {
      // 		std::cout<<"Bin["<<i<<"]: HT["<<bins_[i].HTmin_<<","<<bins_[i].HTmax_<<"] MHT["<<bins_[i].MHTmin_<<","<<bins_[i].MHTmax_<<"] NJets["<<bins_[i].NJetsmin_<<","<<bins_[i].NJetsmax_<<"] BTags["<<bins_[i].BTagsmin_<<","<<bins_[i].BTagsmax_<<"]\n";
      if(HT>bins_[i].HTmin_ && 
	 HT<bins_[i].HTmax_ &&
	 MHT>bins_[i].MHTmin_ && 
	 MHT<bins_[i].MHTmax_ &&
	 NJets+0.1>bins_[i].NJetsmin_ && 
	 NJets-0.1<bins_[i].NJetsmax_ &&
	 BTags+0.1>bins_[i].BTagsmin_ && 
	 BTags-0.1<bins_[i].BTagsmax_
	 )
	{
	  result=i;
	  match++;
	  usedBin_[i]=usedBin_[i]+1;
	}
    }
  if(match==-1 && !DY_)
    {
      // these are the ones with MHT > 750 and HT < 800
      //    std::cout<<"Error event fits in no bin!!! HT: "<<HT<<", MHT: "<<MHT<<", NJets: "<<NJets<<", BTags: "<<BTags<<std::endl;
      result=999;
    }
  if(match>0)
    {
      std::cout<<"Error event fits in more than one bin!!!! HT: "<<HT<<", MHT: "<<MHT<<", NJets: "<<NJets<<", BTags: "<<BTags<<std::endl;
    }
  return result+1; // to not use the 0 bin but start counting at 1
}
