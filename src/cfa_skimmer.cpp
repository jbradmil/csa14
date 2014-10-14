#include "cfa_skimmer.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <utility>
#include "TFile.h"
#include "TTree.h"
#include "event_handler.hpp"
#include "event_number.hpp"
#include "pu_constants.hpp"
#include "lumi_reweighting_stand_alone.hpp"
#include "timer.hpp"
#include "in_json_2012.hpp"

CfASkimmer::CfASkimmer(const std::string& in_file_name,
                       const bool is_list,
		       const int Nentries_in):
  EventHandler(in_file_name, is_list, -1, true),
  Nentries(Nentries_in){
}

void CfASkimmer::Skim(const std::string& out_file_name){

  TFile skimFile(out_file_name.c_str(), "RECREATE");
  TDirectory *cfA_dir(skimFile.mkdir("configurableAnalysis", "configurableAnalysis"));
  cfA_dir->cd();
  TTree *skimTreeA(chainA.CloneTree(0));
  chainA.CopyAddresses(skimTreeA);
  TTree *skimTreeB(chainB.CloneTree(0));
  chainB.CopyAddresses(skimTreeB);
  skimFile.cd();

  std::set<EventNumber> eventList;
  unsigned int startCount(0), met200Count(0), numJetsCount(0), numCSVMCount(0), minDeltaPhiCount(0), muonVetoCount(0), electronVetoCount(0);
 
  int n_to_process(Nentries);
  if(n_to_process<0) n_to_process=GetTotalEntries();
  Timer timer(n_to_process);
  timer.Start();
  for(int i(0); i<n_to_process; ++i){
    if((i%10000==0||i==1||i==10||i==100||i==1000) && i!=0){
      timer.PrintRemainingTime();
    }
    timer.Iterate();
    //   cout << "JERR" << endl;
    GetEntry(i);
    std::pair<std::set<EventNumber>::iterator, bool> returnVal(eventList.insert(EventNumber(run, event, lumiblock)));
    if(!returnVal.second) continue;

    ++startCount;
    // MET>200
    if (pfTypeImets_et->at(0)<200.) continue;
    met200Count++;
    // 3 good jets
    if (GetNumGoodJets(50)<3) continue;
    numJetsCount++;
    // 2 b-tags
    if (GetNumCSVMJets(30.)<2) continue;
    numCSVMCount++;
    // minDeltaPhi
    // if (getMinDeltaPhiMETN(3,50,2.4,true,30,2.4,true,true)<4) continue;
    minDeltaPhiCount++;
    // no leptons
    vector<int>  reco_veto_muons, reco_veto_electrons;
    if (cmEnergy>8) {
      reco_veto_muons=GetRecoMuons(true);
      reco_veto_electrons=GetRecoElectrons(true);
    }
    else {
      reco_veto_muons=GetRA2bMuons(true);
      reco_veto_electrons=GetRA2bElectrons(true);
    }
    if (reco_veto_muons.size()>0) continue;
    muonVetoCount++;
    if (reco_veto_electrons.size()>0) continue;
    electronVetoCount++;

    skimTreeA->Fill();
    skimTreeB->Fill();

  }

  TTree cutFlow("cutFlow", "cutFlow");
  cutFlow.Branch("startCount", &startCount);
  cutFlow.Branch("met200Count", &met200Count);
  cutFlow.Branch("numJetsCount", &numJetsCount);
  cutFlow.Branch("numCSVMCount", &numCSVMCount);
  cutFlow.Branch("minDeltaPhiCount", &minDeltaPhiCount);
  cutFlow.Branch("muonVetoCount", &muonVetoCount);
  cutFlow.Branch("electronVetoCount", &electronVetoCount);
  cutFlow.Fill();

  std::cout << "startCount " << startCount << std::endl;
  std::cout << "met200Count " << met200Count << std::endl;
  std::cout << "numCSVMCount " << numCSVMCount << std::endl;
  std::cout << "minDeltaPhiCount " << minDeltaPhiCount << std::endl;
  std::cout << "muonVetoCount " << muonVetoCount << std::endl;
  std::cout << "electronVetoCount " << electronVetoCount << std::endl;

  TTree timeInfo("timeInfo", "timeInfo");
  time_t my_timer_t;
  struct tm *timer_s(0);
  time(&my_timer_t);
  timer_s=localtime(&my_timer_t);
  timeInfo.Branch("year", &timer_s->tm_year);
  timeInfo.Branch("month", &timer_s->tm_mon);
  timeInfo.Branch("day", &timer_s->tm_mday);
  timeInfo.Branch("hour", &timer_s->tm_hour);
  timeInfo.Branch("minute", &timer_s->tm_min);
  timeInfo.Branch("second", &timer_s->tm_sec);
  timeInfo.Fill();

  cfA_dir->cd();
  skimTreeA->Write();
  skimTreeB->Write();
  skimFile.cd();
  cutFlow.Write();
  timeInfo.Write();
  skimFile.Close();
}
