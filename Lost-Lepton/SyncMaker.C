#define SyncMaker_cxx
// The class definition in SyncMaker.h has been generated automatically
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
// Root > T->Process("SyncMaker.C")
// Root > T->Process("SyncMaker.C","some options")
// Root > T->Process("SyncMaker.C+")
//

#include "SyncMaker.h"
#include <TH2.h>
#include <TStyle.h>
#include <iostream>


void SyncMaker::Begin(TTree * /*tree*/)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();
	 tOut_=NULL;

}

void SyncMaker::SlaveBegin(TTree * /*tree*/)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();
	 tOut_ = new TTree("LostLeptonExpectation","a simple Tree with simple variables");
	 tOut_->SetAutoSave(10000000000);
	 //  tOut_->SetAutoFlush(1000000);
	 tOut_->Branch("HT",&HT,"HT/F");
	 tOut_->Branch("MHT",&MHT,"MHT/F");
	 tOut_->Branch("NJets",&NJets,"NJets/s");
	 tOut_->Branch("BTags",&BTags,"BTags/s");
	 tOut_->Branch("Leptons",&Leptons,"Leptons/s");
	 tOut_->Branch("isoElectronTracks",&isoElectronTracks,"isoElectronTracks/s");
	 tOut_->Branch("isoMuonTracks",&isoMuonTracks,"isoMuonTracks/s");
	 tOut_->Branch("isoPionTracks",&isoPionTracks,"isoPionTracks/s");
	 tOut_->Branch("NVtx",&NVtx,"NVtx/s");
	 tOut_->Branch("DeltaPhi1",&DeltaPhi1,"DeltaPhi1/F");
	 tOut_->Branch("DeltaPhi2",&DeltaPhi2,"DeltaPhi2/F");
	 tOut_->Branch("DeltaPhi3",&DeltaPhi3,"DeltaPhi3/F");
	 tOut_->Branch("minDeltaPhiN",&minDeltaPhiN,"minDeltaPhiN/F");
	 tOut_->Branch("DeltaPhiN1",&DeltaPhiN1,"DeltaPhiN1/F");
	 tOut_->Branch("DeltaPhiN2",&DeltaPhiN2,"DeltaPhiN2/F");
	 tOut_->Branch("DeltaPhiN3",&DeltaPhiN3,"DeltaPhiN3/F");
	 tOut_->Branch("Weight", &Weight, "Weight/F");
	 tOut_->Branch("MET",&METPt,"MET/F");
	 tOut_->Branch("METPhi",&METPhi,"METPhi/F");
	 tOut_->Branch("selectedIDIsoMuonsNum",&selectedIDIsoMuonsNum,"selectedIDIsoMuonsNum/s");
	 tOut_->Branch("selectedIDIsoElectronsNum",&selectedIDIsoElectronsNum,"selectedIDIsoElectronsNum/s");
	 ResetVariables();
	 std::cout<<"Applying filters: "<<applyFilters_<<std::endl;
	 std::cout<<"************************************\n"
	 <<"*    Row   *     event *     mht30 *\n"
	 <<"************************************\n";

}

Bool_t SyncMaker::Process(Long64_t entry)
{
	fChain->GetTree()->GetEntry(entry);
	// *********************************************************************23 June 2015 sync with jack after fermilab ra2/b workshop********************************
	allB++;
	alldB+=Weight;
	if(selectedIDIsoMuonsNum==0)
	{
	  muvetoB++;
	  muvetodB+=Weight;
	}
	if(selectedIDIsoMuonsNum==0 && selectedIDIsoElectronsNum==0)
	{
	  elecvetoB++;
	  elecvetodB+=Weight;
	}
	if(selectedIDIsoMuonsNum==0 && selectedIDIsoElectronsNum==0 && JetID)
	{
	  jetIDB++;
	  jetIDdB+=Weight;
	}
	if(selectedIDIsoMuonsNum==0 && selectedIDIsoElectronsNum==0 && JetID && NJets>=4)
	{
	  nJetcutB++;
	  nJetcutdB+=Weight;
	}
	if(selectedIDIsoMuonsNum==0 && selectedIDIsoElectronsNum==0 && JetID && NJets>=4 && HT>=500)
	{
	  htcutB++;
	  htcutdB+=Weight;
	}
	if(selectedIDIsoMuonsNum==0 && selectedIDIsoElectronsNum==0 && JetID && NJets>=4 && HT>=500 && MHT>200)
	{
	  mhtcutB++;
	  mhtcutdB+=Weight;
	}
	if(selectedIDIsoMuonsNum==0 && selectedIDIsoElectronsNum==0 && JetID && NJets>=4 && HT>=500 && MHT>200 && IsolatedMuonTracksVetoNum == 0 )
	{
	  isotrakMuvetoB++;
	  isotrakMuvetodB+=Weight;
	}
	if(selectedIDIsoMuonsNum==0 && selectedIDIsoElectronsNum==0 && JetID && NJets>=4 && HT>=500 && MHT>200 && IsolatedMuonTracksVetoNum == 0 && IsolatedElectronTracksVetoNum == 0)
	{
	  isotrakElecvetoB++;
	  isotrakElecvetodB+=Weight;
	}
	if(selectedIDIsoMuonsNum==0 && selectedIDIsoElectronsNum==0 && JetID && NJets>=4 && HT>=500 && MHT>200 && IsolatedMuonTracksVetoNum == 0 && IsolatedElectronTracksVetoNum == 0 && IsolatedPionTracksVetoNum == 0)
	{
	  isotrakPionvetoB++;
	  isotrakPionvetodB+=Weight;
	}
	if(selectedIDIsoMuonsNum==0 && selectedIDIsoElectronsNum==0 && JetID && NJets>=4 && HT>=500 && MHT>200 && IsolatedMuonTracksVetoNum == 0 && IsolatedElectronTracksVetoNum == 0 && IsolatedPionTracksVetoNum == 0 && DeltaPhi1>0.5 && DeltaPhi2>0.5 && DeltaPhi3>0.3)
	{
	  deltaphicutB++;
	  deltaphicutdB+=Weight;
	  if(BTags==0)
	  {
	    btag0B++;
	    btag0dB+=Weight;
	  }
	  if(BTags==1)
	  {
	    btag1B++;
	    btag1dB+=Weight;
	  }
	  if(BTags==2)
	  {
	    btag2B++;
	    btag2dB+=Weight;
	  }
	  if(BTags>=3)
	  {
	    btag3B++;
	    btag3dB+=Weight;
	  }
	}
	// track synchronization
	// muon
	for(unsigned int i=0; i< IsolatedMuonTracksVetoNum;i++)
	{
	  muTrackAll++;
	  if(std::abs(IsolatedMuonTracksVetoEta[i])<2.5)
	  {
	    muTrackEta++;
	    if(IsolatedMuonTracksVetoPt[i]>5)
	    {
	      muTrackPT++;
	      if(IsolatedMuonTracksVeto_MT[i]<100.)
	      {
		muTrackMT++;
		if(i==0) muTrackEvents++;
	      }
	    }
	  }
	}
// elec
	for(unsigned int i=0; i< IsolatedElectronTracksVetoNum;i++)
	{
	  elecTrackAll++;
	  if(std::abs(IsolatedElectronTracksVetoEta[i])<2.5)
	  {
	    elecTrackEta++;
	    if(IsolatedElectronTracksVetoPt[i]>5)
	    {
	      elecTrackPT++;
	      if(IsolatedElectronTracksVeto_MT[i]<100.)
	      {
		elecTrackMT++;
		if(i==0) elecTrackEvents++;
	      }
	    }
	  }
	}
	// pion
	for(unsigned int i=0; i< IsolatedPionTracksVetoNum;i++)
	{
	  pionTrackAll++;
	  if(std::abs(IsolatedPionTracksVetoEta[i])<2.5)
	  {
	    pionTrackEta++;
	    if(IsolatedPionTracksVetoPt[i]>5)
	    {
	      pionTrackPT++;
	      if(IsolatedPionTracksVeto_MT[i]<100)
	      {
		pionTrackMT++;
		if(i==0) pionTrackEvents++;
	      }
	    }
	  }
	}
	// *********************************************************************23 June 2015 sync with jack after fermilab ra2/b workshop end********************************
	
	if(applyFilters_ &&  !FiltersPass() ) return kTRUE;
	all++;
	alld+=Weight;
	if(EvtNum==124734)std::cout<<"NJets: "<<NJets <<" HT: "<<HT<<" MHT: "<<MHT<<"\n";
	if(NJets>=4)
	{
	  nJetcut++;
	  nJetcutd+=Weight;
	  if(HT>500)
	  {
	    htcut++;
	    htcutd+=Weight;
	    if(MHT>200)
	    {
	      
	      mhtcut++;
	      mhtcutd+=Weight;
	      if(selectedIDIsoMuonsNum==0)
	      {
		muveto++;
		muvetod+=Weight;
		if(selectedIDIsoElectronsNum==0)
		{
		  elecveto++;
		  elecvetod+=Weight;
		  if(minDeltaPhiN>6)
		  {
		    deltaphiNcut++;
		    deltaphiNcutd+=Weight;
		    if(isoMuonTracks==0)
		    {
		      isotrakMuveto++;
		      isotrakMuvetod+=Weight;
		      if(isoElectronTracks==0)
		      {
			isotrakElecveto++;
			isotrakElecvetod+=Weight;
			
			if(isoPionTracks==0)
			{
			  isotrakPionveto++;
			  isotrakPionvetod+=Weight;
			  
			  if(BTags==0)
			  {
			    btag0++;
			    btag0d+=Weight;
			  }
			  if(BTags==1)
			  {
			    btag1++;
			    btag1d+=Weight;
			  }
			  if(BTags==2)
			  {
			    btag2++;
			    btag2d+=Weight;
			  }
			  if(BTags>=3)
			  {
			    btag3++;
			    btag3d+=Weight;
			  }
			}
		      
		      }
		     
		    }
		  }
		}
	      }
	    }
	    //else std::cout<<"*     "<<all<<" *     "<<EvtNum<<" * "<<MHT<<" *\n";
	    
	  }
	}
	if(GenMuNum>0)
	{
	  for(unsigned int i=0; i<GenMuNum;i++)
	  {
	    if(GenMuPt[i]>10 && abs(GenMuEta[i])<2.4)
	    {
	      GenMuonPtEtaCut++;
	      GenMuonPtEtaCutd+=Weight;
	    }
	    
	  }
	}
	if(slimmedMuonsNum>0)
	{
	  for(unsigned int i=0; i<slimmedMuonsNum;i++)
	  {
	    if(slimmedMuonsPt[i]>10 && abs(slimmedMuonsEta[i])<2.4)
	    {
	      muonPtEtaCut++;
	      muonPtEtaCutd+=Weight;
// 	      break;
	    }
	  }
	  if(selectedIDMuonsNum>0)
	  {
	    muonID+=selectedIDMuonsNum;
	    muonIDd+=Weight*selectedIDMuonsNum;
	  }
	  if(selectedIDIsoMuonsNum>0)
	  {
	    muonIso+=selectedIDIsoMuonsNum;
	    muonIsod+=Weight*selectedIDIsoMuonsNum;
	  }
	}
	if(slimmedElectronsNum>0)
	{
	  for(unsigned int i=0; i<slimmedElectronsNum;i++)
	  {
	    if(slimmedElectronsPt[i]>10 && abs(slimmedElectronsEta[i])<2.5)
	    {
	      elecPtEtaCut++;
	      elecPtEtaCutd+=Weight;
	      break;
	    }
	  }
	  if(selectedIDElectronsNum>0)
	  {
	    elecID++;
	    elecIDd+=Weight;
	  }
	  if(selectedIDIsoElectronsNum>0)
	  {
	    elecIso++;
	    elecIsod+=Weight;
	  }
	}
	tOut_->Fill();
	
	

   return kTRUE;
}

void SyncMaker::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.

}
bool SyncMaker::FiltersPass()
{
  bool result = true;
  if(!JetID) result=false;
  return result;
}


void SyncMaker::Terminate()
{
	TFile *outPutFile = new TFile("Sync.root","RECREATE"); 
	outPutFile->cd();
	tOut_->Write();
	
	 std::cout<<" *********************************************************************23 June 2015 sync with jack after fermilab ra2/b workshop****************************\n";
        std::cout<<"---------------------Baseline cuts------------------------"<<std::endl;
	 std::cout<<"Sync (unweighted) event count:"<<std::endl;
	std::cout<<"All:                       "<<alldB<<" ("<<allB<<")"<<std::endl;
	std::cout<<"MuVeto:                    "<<muvetodB<<" ("<<muvetoB<<")"<<std::endl;
	std::cout<<"ElecVeto:                  "<<elecvetodB<<" ("<<elecvetoB<<")"<<std::endl;
	std::cout<<"JetID:                     "<<jetIDdB<<" ("<<jetIDB<<")"<<std::endl;
	std::cout<<"NJets>=4:                  "<<nJetcutdB<<" ("<<nJetcutB<<")"<<std::endl;
	std::cout<<"HT>500:                    "<<htcutdB<<" ("<<htcutB<<")"<<std::endl;
	std::cout<<"MHT>200:                   "<<mhtcutdB<<" ("<<mhtcutB<<")"<<std::endl;
	std::cout<<"IsoMuTrackVeto:            "<<isotrakMuvetodB<<" ("<<isotrakMuvetoB<<")"<<std::endl;
	std::cout<<"IsoElecTrackVeto:          "<<isotrakElecvetodB<<" ("<<isotrakElecvetoB<<")"<<std::endl;
	std::cout<<"IsoPionTrackVeto:          "<<isotrakPionvetodB<<" ("<<isotrakPionvetoB<<")"<<std::endl;
	std::cout<<"DeltaPhiCuts:              "<<deltaphicutdB<<" ("<<deltaphicutB<<")"<<std::endl;
	std::cout<<"BTag=0:                    "<<btag0dB<<" ("<<btag0B<<")"<<std::endl;
	std::cout<<"BTag=1:                    "<<btag1dB<<" ("<<btag1B<<")"<<std::endl;
	std::cout<<"BTag=2:                    "<<btag2dB<<" ("<<btag2B<<")"<<std::endl;
	std::cout<<"BTag>=3:                   "<<btag3dB<<" ("<<btag3B<<")"<<std::endl;
	std::cout<<"---------------------Tracks: Muon------------------------"<<std::endl;
	std::cout<<"Sync amount of found muon tracks:"<<std::endl;
	std::cout<<"pdgID 13:                       "<<muTrackAll<<std::endl;
	std::cout<<"Eta<2.5:                        "<<muTrackEta<<std::endl;
	std::cout<<"PT>5:                           "<<muTrackPT<<std::endl;
	std::cout<<"MT<100:                         "<<muTrackMT<<std::endl;
	std::cout<<"Events with at least one track: "<<muTrackEvents<<std::endl;
	std::cout<<"---------------------Tracks: Elec------------------------"<<std::endl;
	std::cout<<"Sync amount of found elec tracks:"<<std::endl;
	std::cout<<"pdgID 13:                       "<<elecTrackAll<<std::endl;
	std::cout<<"Eta<2.5:                        "<<elecTrackEta<<std::endl;
	std::cout<<"PT>5:                           "<<elecTrackPT<<std::endl;
	std::cout<<"MT<100:                         "<<elecTrackMT<<std::endl;
	std::cout<<"Events with at least one track: "<<elecTrackEvents<<std::endl;
	std::cout<<"---------------------Tracks: Pion------------------------"<<std::endl;
	std::cout<<"Sync amount of found pion tracks:"<<std::endl;
	std::cout<<"pdgID 211:                      "<<pionTrackAll<<std::endl;
	std::cout<<"Eta<2.5:                        "<<pionTrackEta<<std::endl;
	std::cout<<"PT>10:                          "<<pionTrackPT<<std::endl;
	std::cout<<"MT<100:                         "<<pionTrackMT<<std::endl;
	std::cout<<"Events with at least one track: "<<pionTrackEvents<<std::endl;
	 std::cout<<" *********************************************************************23 June 2015 sync with jack after fermilab ra2/b workshop end****************************\n";

	std::cout<<"Sync (unweighted) event count:"<<std::endl;
	std::cout<<"All:                   "<<alld<<" ("<<all<<")"<<std::endl;
	std::cout<<"NJets>=4:              "<<nJetcutd<<" ("<<nJetcut<<")"<<std::endl;
	std::cout<<"HT>500:                "<<htcutd<<" ("<<htcut<<")"<<std::endl;
	std::cout<<"MHT>200:               "<<mhtcutd<<" ("<<mhtcut<<")"<<std::endl;
	std::cout<<"IsoMu veto:            "<<muvetod<<" ("<<muveto<<")"<<std::endl;
// 	std::cout<<"IsoMu veto:            "<<muvetod<<" ("<<muveto<<")"<<std::endl;
	std::cout<<"IsoElec veto:          "<<elecvetod<<" ("<<elecveto<<")"<<std::endl;
	std::cout<<"DeltaPhiN>4:           "<<deltaphiNcutd<<" ("<<deltaphiNcut<<")"<<std::endl;
	std::cout<<"isotrakMuveto(mt<100) veto: "<<isotrakMuvetod<<" ("<<isotrakMuveto<<")"<<std::endl;
	std::cout<<"isotrakElecveto(mt<100) veto: "<<isotrakElecvetod<<" ("<<isotrakElecveto<<")"<<std::endl;
	std::cout<<"isotrakPionveto(mt<100) veto: "<<isotrakPionvetod<<" ("<<isotrakPionveto<<")"<<std::endl;
	std::cout<<"btag=0:                "<<btag0d<<" ("<<btag0<<")"<<std::endl;
	std::cout<<"btag=1:                "<<btag1d<<" ("<<btag1<<")"<<std::endl;
	std::cout<<"btag=2:                "<<btag2d<<" ("<<btag2<<")"<<std::endl;
	std::cout<<"btag>=3:               "<<btag3d<<" ("<<btag3<<")"<<std::endl;
	std::cout<<"-------------------------------------------------"<<std::endl;
	std::cout<<"Muon:               "<<std::endl;
	std::cout<<"Gen pt eta cut:            "<<GenMuonPtEtaCutd<<" ("<<GenMuonPtEtaCut<<")"<<std::endl;
	std::cout<<"pt eta cut:            "<<muonPtEtaCutd<<" ("<<muonPtEtaCut<<")"<<std::endl;
	std::cout<<"ID:                    "<<muonIDd<<" ("<<muonID<<")"<<std::endl;
	std::cout<<"miniIso:               "<<muonIsod<<" ("<<muonIso<<")"<<std::endl;
	std::cout<<"Electron:               "<<std::endl;
	std::cout<<"pt eta cut:            "<<elecPtEtaCutd<<" ("<<elecPtEtaCut<<")"<<std::endl;
	std::cout<<"ID:                    "<<elecIDd<<" ("<<elecID<<")"<<std::endl;
	std::cout<<"miniIso:               "<<elecIsod<<" ("<<elecIso<<")"<<std::endl;
}
void SyncMaker::ResetVariables()
{
  	// *********************************************************************23 June 2015 sync with jack after fermilab ra2/b workshop********************************
	allB=0;
	nJetcutB=0;
	htcutB=0;
	mhtcutB=0;
	muvetoB=0;
	elecvetoB=0;
	deltaphicutB=0;
	isotrakMuvetoB=0;
	isotrakElecvetoB=0;
	isotrakPionvetoB=0;
	btag0B=0;
	btag1B=0;
	btag2B=0;
	btag3B=0;
	
	alldB=0.;
	nJetcutdB=0.;
	htcutdB=0.;
	mhtcutdB=0.;
	muvetodB=0.;
	elecvetodB=0.;
	deltaphicutdB=0.;
	isotrakMuvetodB=0;
	isotrakElecvetodB=0;
	isotrakPionvetodB=0;
	btag0dB=0.;
	btag1dB=0.;
	btag2dB=0.;
	btag3dB=0.;
	jetIDB=0;
	
	// isolated track study
	muTrackAll=0;
	muTrackEta=0;
	muTrackPT=0;
	muTrackMT=0;
	elecTrackAll=0;
	elecTrackEta=0;
	elecTrackPT=0;
	elecTrackMT=0;
	pionTrackAll=0;
	pionTrackEta=0;
	pionTrackPT=0;
	pionTrackMT=0;
	
	muTrackEvents=0;
	elecTrackEvents=0;
	pionTrackEvents=0;
  
  	// *********************************************************************23 June 2015 sync with jack after fermilab ra2/b workshop end********************************

	all=0;
	nJetcut=0;
	htcut=0;
	mhtcut=0;
	muveto=0;
	elecveto=0;
	deltaphiNcut=0;
	isotrakMuveto=0;
	isotrakElecveto=0;
	isotrakPionveto=0;
	btag0=0;
	btag1=0;
	btag2=0;
	btag3=0;

	
	alld=0.;
	nJetcutd=0.;
	htcutd=0.;
	mhtcutd=0.;
	muvetod=0.;
	elecvetod=0.;
 	deltaphiNcutd=0.;
	isotrakMuvetod=0;
	isotrakElecvetod=0;
	isotrakPionvetod=0;
	btag0d=0.;
	btag1d=0.;
	btag2d=0.;
	btag3d=0.;
	jetIDdB=0.;
	
	GenMuonPtEtaCut=0;
	GenMuonPtEtaCutd=0.;
	muonPtEtaCut=0;
	muonID=0;
	muonIso=0;
	muonPtEtaCutd=0.;
	muonIDd=0.;
	muonIsod=0.;
	
	elecPtEtaCut=0;
	elecID=0;
	elecIso=0;
	elecPtEtaCutd=0.;
	elecIDd=0.;
	elecIsod=0.;

}
