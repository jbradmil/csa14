#include "reduced_tree_maker.hpp"
#include <vector>
#include <string>
#include <set>
#include "timer.hpp"
#include "event_handler.hpp"

ReducedTreeMaker::ReducedTreeMaker(const std::string& in_file_name,
                                   const bool is_list,
                                   const double weight_in):
  EventHandler(in_file_name, is_list, weight_in, true){
}

void ReducedTreeMaker::MakeReducedTree(const std::string& out_file_name){
  TFile file(out_file_name.c_str(), "recreate");
  std::set<EventNumber> eventList;
  file.cd();

  const bool isRealData(sampleName.find("Run2012")!=std::string::npos);
  const bool isttbar(sampleName.find("TTJets")!=std::string::npos || sampleName.find("TT_")!=std::string::npos);
  std::vector<float> dataDist(pu::RunsThrough203002, pu::RunsThrough203002+60);
  std::vector<float> MCDist(pu::Summer2012_S10, pu::Summer2012_S10+60);//QQQ this needs to change later for general pileup scenario
  reweight::LumiReWeighting lumiWeights(MCDist, dataDist);

  TTree reduced_tree("reduced_tree","reduced_tree");
  bool passesJSONCut(false), passesPVCut(false), passesMETCleaningCut(false),
    passesLeptonVetoCut(false), passesIsoTrackVetoCut(false);

  bool controlSampleOneLepton(false);

  float pu_true_num_interactions(0.0);
  unsigned short num_primary_vertices(0);

  float met_sig(0.0), met(0.0);
  unsigned short num_jets(0), num_b_tagged_jets(0);
 
  unsigned short num_iso_tracks(0);
  unsigned short num_veto_electrons(0), num_veto_muons(0), num_veto_taus(0), num_veto_leptons(0);
  unsigned short num_noiso_electrons(0), num_noiso_muons(0), num_noiso_taus(0), num_noiso_leptons(0);
  unsigned short num_loose_electrons(0), num_loose_muons(0), num_loose_taus(0), num_loose_leptons(0);
  unsigned short num_medium_electrons(0), num_medium_muons(0), num_medium_taus(0), num_medium_leptons(0);
  unsigned short num_tight_electrons(0), num_tight_muons(0), num_tight_taus(0), num_tight_leptons(0);
 
  float ht_jets(0.0), ht_jets_met(0.0), ht_jets_leps(0.0), ht_jets_met_leps(0.0);
  float full_weight(0.0), lumi_weight(0.0), top_pt_weight(0.0), top_pt_weight_official(0.0), pu_weight(0.0), trigger_weight(0.0);

  unsigned short n_gen_electrons(0), n_gen_muons(0), n_gen_taus(0), n_gen_nus(0);
  int n_lost_electrons(0), n_lost_muons(0), n_lost_taus(0);

  reduced_tree.Branch("passesJSONCut", &passesJSONCut);
  reduced_tree.Branch("passesPVCut",&passesPVCut);
  reduced_tree.Branch("passesMETCleaningCut",&passesMETCleaningCut);
  reduced_tree.Branch("passesLeptonVetoCut",&passesLeptonVetoCut);
  reduced_tree.Branch("passesIsoTrackVetoCut",&passesIsoTrackVetoCut);
  
  reduced_tree.Branch("controlSampleOneLepton",&controlSampleOneLepton);

  reduced_tree.Branch("pu_true_num_interactions", &pu_true_num_interactions);
  reduced_tree.Branch("num_primary_vertices", &num_primary_vertices);

  reduced_tree.Branch("met_sig", &met_sig);
  reduced_tree.Branch("met", &met);

  reduced_tree.Branch("num_jets", &num_jets);
  reduced_tree.Branch("num_b_tagged_jets", &num_b_tagged_jets);

  reduced_tree.Branch("num_veto_electrons", &num_veto_electrons);
  reduced_tree.Branch("num_veto_muons", &num_veto_muons);
  reduced_tree.Branch("num_veto_taus", &num_veto_taus);
  reduced_tree.Branch("num_veto_leptons", &num_veto_leptons);

  reduced_tree.Branch("num_noiso_electrons", &num_noiso_electrons);
  reduced_tree.Branch("num_noiso_muons", &num_noiso_muons);
  reduced_tree.Branch("num_noiso_taus", &num_noiso_taus);
  reduced_tree.Branch("num_noiso_leptons", &num_noiso_leptons);

  reduced_tree.Branch("num_loose_electrons", &num_loose_electrons);
  reduced_tree.Branch("num_loose_muons", &num_loose_muons);
  reduced_tree.Branch("num_loose_taus", &num_loose_taus);
  reduced_tree.Branch("num_loose_leptons", &num_loose_leptons);

  reduced_tree.Branch("num_medium_electrons", &num_medium_electrons);
  reduced_tree.Branch("num_medium_muons", &num_medium_muons);
  reduced_tree.Branch("num_medium_taus", &num_medium_taus);
  reduced_tree.Branch("num_medium_leptons", &num_medium_leptons);

  reduced_tree.Branch("num_tight_electrons", &num_tight_electrons);
  reduced_tree.Branch("num_tight_muons", &num_tight_muons);
  reduced_tree.Branch("num_tight_taus", &num_tight_taus);
  reduced_tree.Branch("num_tight_leptons", &num_tight_leptons);

  reduced_tree.Branch("num_iso_tracks", &num_iso_tracks);

  reduced_tree.Branch("ht_jets", &ht_jets);
  reduced_tree.Branch("ht_jets_met", &ht_jets_met);
  reduced_tree.Branch("ht_jets_leps", &ht_jets_leps);
  reduced_tree.Branch("ht_jets_met_leps", &ht_jets_met_leps);

  reduced_tree.Branch("full_weight", &full_weight);
  reduced_tree.Branch("lumi_weight", &lumi_weight);
  reduced_tree.Branch("pu_weight", &pu_weight);
  reduced_tree.Branch("top_pt_weight", &top_pt_weight);
  reduced_tree.Branch("top_pt_weight_official", &top_pt_weight_official);
  reduced_tree.Branch("trigger_weight", &trigger_weight);
 
  reduced_tree.Branch("run", &run);
  reduced_tree.Branch("event", &event);
  reduced_tree.Branch("lumiblock", &lumiblock);

  reduced_tree.Branch("n_gen_electrons", &n_gen_electrons);
  reduced_tree.Branch("n_gen_muons", &n_gen_muons);
  reduced_tree.Branch("n_gen_taus", &n_gen_taus);
  reduced_tree.Branch("n_gen_nus", &n_gen_nus);

  reduced_tree.Branch("n_lost_electrons", &n_lost_electrons);
  reduced_tree.Branch("n_lost_muons", &n_lost_muons);
  reduced_tree.Branch("n_lost_taus", &n_lost_taus);

  Timer timer(GetTotalEntries());
  timer.Start();
  for(int i(0); i<GetTotalEntries(); ++i){
    if(i%1000==0 && i!=0){
      timer.PrintRemainingTime();
    }
    timer.Iterate();
    GetEntry(i);

    std::pair<std::set<EventNumber>::iterator, bool> returnVal(eventList.insert(EventNumber(run, event, lumiblock)));
    if(!returnVal.second) continue;
    
    // Saving our cuts for the reduced tree
    passesJSONCut=PassesJSONCut();
    passesPVCut=PassesPVCut();
    passesMETCleaningCut=PassesMETCleaningCut();
    passesLeptonVetoCut=PassesLeptonVetoCut();
    passesIsoTrackVetoCut=PassesIsoTrackVetoCut();

    pu_true_num_interactions=GetNumInteractions();
    num_primary_vertices=GetNumVertices();

    met_sig=pfmets_fullSignif;
    met=pfTypeImets_et->at(0);

    num_jets=GetNumGoodJets();
    num_b_tagged_jets=GetNumBTaggedJets();

    num_veto_electrons=GetNumElectrons(0);
    num_veto_muons=GetNumMuons(0);
    num_veto_taus=GetNumTaus(0);
    num_veto_leptons=num_veto_electrons+num_veto_muons+num_veto_taus;
    num_noiso_electrons=GetNumElectrons(0,false);
    num_noiso_muons=GetNumMuons(0,false);
    num_noiso_taus=GetNumTaus(0,false);
    num_noiso_leptons=num_noiso_electrons+num_noiso_muons+num_noiso_taus;
    num_loose_electrons=GetNumElectrons(1);
    num_loose_muons=GetNumMuons(1);
    num_loose_taus=GetNumTaus(1);
    num_loose_leptons=num_loose_electrons+num_loose_muons+num_loose_taus;
    num_medium_electrons=GetNumElectrons(2);
    num_medium_muons=GetNumMuons(2);
    num_medium_taus=GetNumTaus(2);
    num_medium_leptons=num_medium_electrons+num_medium_muons+num_medium_taus;
    num_tight_electrons=GetNumElectrons(3);
    num_tight_muons=GetNumMuons(3);
    num_tight_taus=GetNumTaus(3);
    num_tight_leptons=num_tight_electrons+num_tight_muons+num_tight_taus;
    num_iso_tracks=NewGetNumIsoTracks();

    controlSampleOneLepton=PassesSingleLeptonControlCut();

    ht_jets=GetHT(false, false);
    ht_jets_met=GetHT(true, false);
    ht_jets_leps=GetHT(false, true);
    ht_jets_met_leps=GetHT(true, true);

    double this_scale_factor(scaleFactor);
    if(sampleName.find("SMS-TChiHH")!=std::string::npos){
      this_scale_factor=look_up_scale_factor();
    }

    pu_weight=isRealData?1.0:GetPUWeight(lumiWeights);
    lumi_weight=this_scale_factor;
    top_pt_weight=isttbar?GetTopPtWeight():1.0;
    top_pt_weight_official=isttbar?GetTopPtWeightOfficial():1.0;
    trigger_weight=GetSbinWeight();
    full_weight=pu_weight*lumi_weight*top_pt_weight_official*trigger_weight;

    n_gen_electrons=GetNGenParticles(11);
    n_gen_muons=GetNGenParticles(12);
    n_gen_taus=GetNGenParticles(15);
    n_gen_nus=GetNGenParticles(12)+GetNGenParticles(14)+GetNGenParticles(16);

    n_lost_electrons=n_gen_electrons-num_veto_electrons;
    n_lost_muons=n_gen_muons-num_veto_muons;
    n_lost_taus=n_gen_taus-num_veto_taus;

    reduced_tree.Fill(); 
  }

  reduced_tree.Write();
  file.Close();
}
