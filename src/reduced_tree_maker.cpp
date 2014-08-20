#include "reduced_tree_maker.hpp"
#include <vector>
#include <string>
#include <set>
#include "timer.hpp"
#include "event_handler.hpp"

using namespace std;

ReducedTreeMaker::ReducedTreeMaker(const std::string& in_file_name,
                                   const bool is_list,
                                   const double weight_in):
  EventHandler(in_file_name, is_list, weight_in, true){
}

void ReducedTreeMaker::MakeReducedTree(const std::string& out_file_name){
  TFile file(out_file_name.c_str(), "recreate");
  std::set<EventNumber> eventList;
  file.cd();

  //  const bool isRealData(sampleName.find("Run2012")!=std::string::npos);
  //  const bool isttbar(sampleName.find("TTJets")!=std::string::npos || sampleName.find("TT_")!=std::string::npos);
  std::vector<float> dataDist(pu::RunsThrough203002, pu::RunsThrough203002+60);
  std::vector<float> MCDist(pu::Summer2012_S10, pu::Summer2012_S10+60);//QQQ this needs to change later for general pileup scenario
  reweight::LumiReWeighting lumiWeights(MCDist, dataDist);

  TTree reduced_tree("reduced_tree","reduced_tree");

  int entry(0);
  //  bool passesPVCut(false), passesMETCleaningCut(false);


  float pu_num_interactions(0.0), pu_true_num_interactions(0.0);
  unsigned short num_primary_vertices(0);

  float eoot_pu, loot_pu, oot_pu;

  float met(0.0), met_phi(0.0), ht(0.0);

  unsigned short num_jets_pt20(0), num_jets_pt50(0), num_jets_pt80(0), num_jets_pt100(0), num_jets_pt150(0);
  unsigned short num_csvt_jets(0), num_csvm_jets(0), num_csvl_jets(0);
  unsigned short num_gen_partons(0), num_gen_partons_pt20(0), num_gen_partons_pt40(0), num_gen_partons_pt70(0), num_gen_partons_pt100(0), num_gen_partons_pt150(0);


  /*
    unsigned short num_ra2b_veto_electrons(0), num_ra2b_veto_muons(0), num_ra2b_veto_taus(0), num_ra2b_veto_leptons(0);
    unsigned short num_ra2b_noiso_electrons(0), num_ra2b_noiso_muons(0), num_ra2b_noiso_taus(0), num_ra2b_noiso_leptons(0);
    unsigned short num_ra2b_loose_electrons(0), num_ra2b_loose_muons(0), num_ra2b_loose_taus(0), num_ra2b_loose_leptons(0);
    unsigned short num_ra2b_medium_electrons(0), num_ra2b_medium_muons(0), num_ra2b_medium_taus(0), num_ra2b_medium_leptons(0);
    unsigned short num_ra2b_tight_electrons(0), num_ra2b_tight_muons(0), num_ra2b_tight_taus(0), num_ra2b_tight_leptons(0);
    unsigned short num_lost_ra2b_electrons(0), num_lost_ra2b_muons(0), num_lost_ra2b_leptons(0);
  */
  // unsigned short num_ra2b_iso_tracks(0);  
  // unsigned short num_iso_tracks(0);
  
 
  float full_weight(0.0), lumi_weight(0.0), top_pt_weight(0.0), top_pt_weight_official(0.0), pu_weight(0.0);

  unsigned short num_gen_electrons(0), num_gen_muons(0), num_iso_gen_electrons(0), num_iso_gen_muons(0), num_gen_leptons(0), num_iso_gen_leptons(0);
  // num_gen_taus(0), num_gen_nus(0);
  unsigned short num_ignored_gen_muons(0), num_ignored_gen_electrons(0);

  // const int nptbins(6);
  // float pt_thresholds[nptbins] = {0., 5., 10., 20., 30., 40};
  // vector<int> num_gen_muons_pt(nptbins,0);

  // num_lost_taus(0);

  int num_reco_muons(0), num_reco_veto_muons(0);
  int num_reco_electrons(0), num_reco_veto_electrons(0);

  int num_lost_electrons(0), num_lost_muons(0), num_lost_leptons(0);


  genMuInfo gen_mu1, gen_mu2, gen_mu3, gen_mu4;
  genElInfo gen_el1, gen_el2, gen_el3, gen_el4;
  //  genTauInfo gen_tau1, gen_tau2;

  int muon_gen_mother_id;
  float muon_gen_pt, muon_gen_eta, muon_gen_phi;
  int muon_reco_match;
  int muon_signal, muon_veto, muon_PFmatched;
  float muon_pt, muon_eta, muon_phi;
  float muon_relIso;
  float muon_NH_Et, muon_CH_pt, muon_ph_Et, muon_PU_pt;
  float muon_NH_Iso, muon_CH_Iso, muon_ph_Iso, muon_PU_Iso;
  float muon_minDR_jet, muon_mT;
  int muon2_gen_mother_id;
  float muon2_gen_pt, muon2_gen_eta, muon2_gen_phi;
  int muon2_reco_match;
  int muon2_signal, muon2_veto, muon2_PFmatched;
  float muon2_pt, muon2_eta, muon2_phi;
  float muon2_relIso;
  float muon2_NH_Et, muon2_CH_pt, muon2_ph_Et, muon2_PU_pt;
  float muon2_NH_Iso, muon2_CH_Iso, muon2_ph_Iso, muon2_PU_Iso;
  float muon2_minDR_jet, muon2_mT;

  int electron_gen_mother_id;
  float electron_gen_pt, electron_gen_eta, electron_gen_phi;
  int electron_reco_match;
  int electron_signal, electron_veto, electron_PFmatched;
  float electron_pt, electron_eta, electron_phi;
  float electron_relIso;
  float electron_NH_Et, electron_CH_pt, electron_ph_Et;
  float electron_NH_Iso, electron_CH_Iso, electron_ph_Iso;
  float electron_minDR_jet, electron_mT;
  int electron2_gen_mother_id;
  float electron2_gen_pt, electron2_gen_eta, electron2_gen_phi;
  int electron2_reco_match;
  int electron2_signal, electron2_veto, electron2_PFmatched;
  float electron2_pt, electron2_eta, electron2_phi;
  float electron2_relIso;
  float electron2_NH_Et, electron2_CH_pt, electron2_ph_Et;
  float electron2_NH_Iso, electron2_CH_Iso, electron2_ph_Iso;
  float electron2_minDR_jet, electron2_mT;

  int lep1(0), lep2(0);

  //  reduced_tree.Branch("passesPVCut",&passesPVCut);
  //  reduced_tree.Branch("passesMETCleaningCut",&passesMETCleaningCut);

  reduced_tree.Branch("full_weight", &full_weight);
  reduced_tree.Branch("lumi_weight", &lumi_weight);
  reduced_tree.Branch("pu_weight", &pu_weight);
  reduced_tree.Branch("top_pt_weight", &top_pt_weight);
  reduced_tree.Branch("top_pt_weight_official", &top_pt_weight_official);
 
  reduced_tree.Branch("run", &run);
  reduced_tree.Branch("event", &event);
  reduced_tree.Branch("lumiblock", &lumiblock);
  reduced_tree.Branch("entry", &entry);

  reduced_tree.Branch("pu_true_num_interactions", &pu_true_num_interactions);
  reduced_tree.Branch("pu_num_interactions", &pu_num_interactions);
  reduced_tree.Branch("num_primary_vertices", &num_primary_vertices);
  reduced_tree.Branch("eoot_pu", &eoot_pu);
  reduced_tree.Branch("loot_pu", &loot_pu);
  reduced_tree.Branch("oot_pu", &oot_pu);

  reduced_tree.Branch("met", &met);
  reduced_tree.Branch("met_phi", &met_phi);
  reduced_tree.Branch("ht", &ht);

  reduced_tree.Branch("num_jets_pt20", &num_jets_pt20); 
  reduced_tree.Branch("num_jets_pt50", &num_jets_pt50); 
  reduced_tree.Branch("num_jets_pt80", &num_jets_pt80); 
  reduced_tree.Branch("num_jets_pt100", &num_jets_pt100); 
  reduced_tree.Branch("num_jets_pt150", &num_jets_pt150); 

  reduced_tree.Branch("num_csvt_jets", &num_csvt_jets); 
  reduced_tree.Branch("num_csvm_jets", &num_csvm_jets); 
  reduced_tree.Branch("num_csvl_jets", &num_csvl_jets); 

  reduced_tree.Branch("num_gen_partons", &num_gen_partons);
  reduced_tree.Branch("num_gen_partons_pt20", &num_gen_partons_pt20);
  reduced_tree.Branch("num_gen_partons_pt40", &num_gen_partons_pt40);
  reduced_tree.Branch("num_gen_partons_pt70", &num_gen_partons_pt70);
  reduced_tree.Branch("num_gen_partons_pt100", &num_gen_partons_pt100);
  reduced_tree.Branch("num_gen_partons_pt150", &num_gen_partons_pt150);

  /* 
     reduced_tree.Branch("num_ra2b_veto_electrons", &num_ra2b_veto_electrons);
     reduced_tree.Branch("num_ra2b_veto_muons", &num_ra2b_veto_muons);
     reduced_tree.Branch("num_ra2b_veto_taus", &num_ra2b_veto_taus);
     reduced_tree.Branch("num_ra2b_veto_leptons", &num_ra2b_veto_leptons);

     reduced_tree.Branch("num_ra2b_noiso_electrons", &num_ra2b_noiso_electrons);
     reduced_tree.Branch("num_ra2b_noiso_muons", &num_ra2b_noiso_muons);
     reduced_tree.Branch("num_ra2b_noiso_taus", &num_ra2b_noiso_taus);
     reduced_tree.Branch("num_ra2b_noiso_leptons", &num_ra2b_noiso_leptons);

     reduced_tree.Branch("num_ra2b_loose_electrons", &num_ra2b_loose_electrons);
     reduced_tree.Branch("num_ra2b_loose_muons", &num_ra2b_loose_muons);
     reduced_tree.Branch("num_ra2b_loose_taus", &num_ra2b_loose_taus);
     reduced_tree.Branch("num_ra2b_loose_leptons", &num_ra2b_loose_leptons);

     reduced_tree.Branch("num_ra2b_medium_electrons", &num_ra2b_medium_electrons);
     reduced_tree.Branch("num_ra2b_medium_muons", &num_ra2b_medium_muons);
     reduced_tree.Branch("num_ra2b_medium_taus", &num_ra2b_medium_taus);
     reduced_tree.Branch("num_ra2b_medium_leptons", &num_ra2b_medium_leptons);

     reduced_tree.Branch("num_ra2b_tight_electrons", &num_ra2b_tight_electrons);
     reduced_tree.Branch("num_ra2b_tight_muons", &num_ra2b_tight_muons);
     reduced_tree.Branch("num_ra2b_tight_taus", &num_ra2b_tight_taus);
     reduced_tree.Branch("num_ra2b_tight_leptons", &num_ra2b_tight_leptons);
  */

  reduced_tree.Branch("num_gen_muons", &num_gen_muons);
  reduced_tree.Branch("num_iso_gen_muons", &num_iso_gen_muons);
  reduced_tree.Branch("num_ignored_gen_muons", &num_ignored_gen_muons);

  string gen_muon_branches("is_iso/I:pt/F:eta/F:phi/F:mother_id/I:minDR/F:minDPt/F:mus_match/I:topPt/F:deltaRWb/F:signal_muon/I:veto_muon/I:reco_pt/F:reco_eta/F:reco_phi/F:reco_dZ/F:reco_d0PV/F:reco_relIso/F:reco_EMIso/F:reco_NHIso/F:reco_CHIso/F:reco_PUIso/F:reco_IsoEn/F:reco_EMEn/F:reco_CHEn/F:reco_NHEn/F:reco_PUEn/F:reco_hasPFMatch/I:reco_minDR/F");
  reduced_tree.Branch("gen_mu1", &gen_mu1, gen_muon_branches.c_str());
  reduced_tree.Branch("gen_mu2", &gen_mu2, gen_muon_branches.c_str());
  reduced_tree.Branch("gen_mu3", &gen_mu3, gen_muon_branches.c_str());
  reduced_tree.Branch("gen_mu4", &gen_mu4, gen_muon_branches.c_str());

  reduced_tree.Branch("muon_gen_mother_id", &muon_gen_mother_id);
  reduced_tree.Branch("muon_gen_pt", &muon_gen_pt);
  reduced_tree.Branch("muon_gen_eta", &muon_gen_eta);
  reduced_tree.Branch("muon_gen_phi", &muon_gen_phi);
  reduced_tree.Branch("muon_reco_match", &muon_reco_match);

  reduced_tree.Branch("muon_signal", &muon_signal);
  reduced_tree.Branch("muon_veto", &muon_veto);
  reduced_tree.Branch("muon_PFmatched", &muon_PFmatched);

  reduced_tree.Branch("muon_pt", &muon_pt);
  reduced_tree.Branch("muon_eta", &muon_eta);
  reduced_tree.Branch("muon_phi", &muon_phi);

  reduced_tree.Branch("muon_relIso", &muon_relIso);
  reduced_tree.Branch("muon_NH_Iso", &muon_NH_Iso);
  reduced_tree.Branch("muon_CH_Iso", &muon_CH_Iso);
  reduced_tree.Branch("muon_ph_Iso", &muon_ph_Iso);
  reduced_tree.Branch("muon_PU_Iso", &muon_PU_Iso);

  reduced_tree.Branch("muon_NH_Et", &muon_NH_Et);
  reduced_tree.Branch("muon_CH_pt", &muon_CH_pt);
  reduced_tree.Branch("muon_ph_Et", &muon_ph_Et);
  reduced_tree.Branch("muon_PU_pt", &muon_PU_pt);

  reduced_tree.Branch("muon_minDR_jet", &muon_minDR_jet);
  reduced_tree.Branch("muon_mT", &muon_mT);

  reduced_tree.Branch("muon2_gen_mother_id", &muon2_gen_mother_id);
  reduced_tree.Branch("muon2_gen_pt", &muon2_gen_pt);
  reduced_tree.Branch("muon2_gen_eta", &muon2_gen_eta);
  reduced_tree.Branch("muon2_gen_phi", &muon2_gen_phi);
  reduced_tree.Branch("muon2_reco_match", &muon2_reco_match);

  reduced_tree.Branch("muon2_signal", &muon2_signal);
  reduced_tree.Branch("muon2_veto", &muon2_veto);
  reduced_tree.Branch("muon2_PFmatched", &muon2_PFmatched);

  reduced_tree.Branch("muon2_pt", &muon2_pt);
  reduced_tree.Branch("muon2_eta", &muon2_eta);
  reduced_tree.Branch("muon2_phi", &muon2_phi);

  reduced_tree.Branch("muon2_relIso", &muon2_relIso);
  reduced_tree.Branch("muon2_NH_Iso", &muon2_NH_Iso);
  reduced_tree.Branch("muon2_CH_Iso", &muon2_CH_Iso);
  reduced_tree.Branch("muon2_ph_Iso", &muon2_ph_Iso);
  reduced_tree.Branch("muon2_PU_Iso", &muon2_PU_Iso);

  reduced_tree.Branch("muon2_NH_Et", &muon2_NH_Et);
  reduced_tree.Branch("muon2_CH_pt", &muon2_CH_pt);
  reduced_tree.Branch("muon2_ph_Et", &muon2_ph_Et);
  reduced_tree.Branch("muon2_PU_pt", &muon2_PU_pt);

  reduced_tree.Branch("muon2_minDR_jet", &muon2_minDR_jet);
  reduced_tree.Branch("muon2_mT", &muon2_mT);

  reduced_tree.Branch("num_reco_veto_muons", &num_reco_veto_muons);
  reduced_tree.Branch("num_reco_muons", &num_reco_muons);

  reduced_tree.Branch("num_gen_electrons", &num_gen_electrons);
  reduced_tree.Branch("num_iso_gen_electrons", &num_iso_gen_electrons);
  reduced_tree.Branch("num_ignored_gen_electrons", &num_ignored_gen_electrons);

  string gen_electron_branches("is_iso/I:pt/F:eta/F:phi/F:mother_id/I:minDR/F:minDPt/F:els_match/I:topPt/F:deltaRWb/F:signal_electron/I:veto_electron/I:reco_pt/F:reco_eta/F:reco_phi/F:reco_dZ/F:reco_d0PV/F:reco_relIso/F:reco_EMIso/F:reco_NHIso/F:reco_CHIso/F:reco_PUIso/F:reco_hasPFMatch/I:reco_minDR/F");
  reduced_tree.Branch("gen_el1", &gen_el1, gen_electron_branches.c_str());
  reduced_tree.Branch("gen_el2", &gen_el2, gen_electron_branches.c_str());
  reduced_tree.Branch("gen_el3", &gen_el3, gen_electron_branches.c_str());
  reduced_tree.Branch("gen_el4", &gen_el4, gen_electron_branches.c_str());

  reduced_tree.Branch("num_reco_veto_electrons", &num_reco_veto_electrons);
  reduced_tree.Branch("num_reco_electrons", &num_reco_electrons);

  reduced_tree.Branch("electron_gen_mother_id", &electron_gen_mother_id);
  reduced_tree.Branch("electron_gen_pt", &electron_gen_pt);
  reduced_tree.Branch("electron_gen_eta", &electron_gen_eta);
  reduced_tree.Branch("electron_gen_phi", &electron_gen_phi);
  reduced_tree.Branch("electron_reco_match", &electron_reco_match);

  reduced_tree.Branch("electron_signal", &electron_signal);
  reduced_tree.Branch("electron_veto", &electron_veto);
  reduced_tree.Branch("electron_PFmatched", &electron_PFmatched);

  reduced_tree.Branch("electron_pt", &electron_pt);
  reduced_tree.Branch("electron_eta", &electron_eta);
  reduced_tree.Branch("electron_phi", &electron_phi);

  reduced_tree.Branch("electron_relIso", &electron_relIso);
  reduced_tree.Branch("electron_NH_Iso", &electron_NH_Iso);
  reduced_tree.Branch("electron_CH_Iso", &electron_CH_Iso);
  reduced_tree.Branch("electron_ph_Iso", &electron_ph_Iso);

  reduced_tree.Branch("electron_NH_Et", &electron_NH_Et);
  reduced_tree.Branch("electron_CH_pt", &electron_CH_pt);
  reduced_tree.Branch("electron_ph_Et", &electron_ph_Et);

  reduced_tree.Branch("electron_minDR_jet", &electron_minDR_jet);
  reduced_tree.Branch("electron_mT", &electron_mT);

  reduced_tree.Branch("electron2_gen_mother_id", &electron2_gen_mother_id);
  reduced_tree.Branch("electron2_gen_pt", &electron2_gen_pt);
  reduced_tree.Branch("electron2_gen_eta", &electron2_gen_eta);
  reduced_tree.Branch("electron2_gen_phi", &electron2_gen_phi);
  reduced_tree.Branch("electron2_reco_match", &electron2_reco_match);

  reduced_tree.Branch("electron2_signal", &electron2_signal);
  reduced_tree.Branch("electron2_veto", &electron2_veto);
  reduced_tree.Branch("electron2_PFmatched", &electron2_PFmatched);

  reduced_tree.Branch("electron2_pt", &electron2_pt);
  reduced_tree.Branch("electron2_eta", &electron2_eta);
  reduced_tree.Branch("electron2_phi", &electron2_phi);

  reduced_tree.Branch("electron2_relIso", &electron2_relIso);
  reduced_tree.Branch("electron2_NH_Iso", &electron2_NH_Iso);
  reduced_tree.Branch("electron2_CH_Iso", &electron2_CH_Iso);
  reduced_tree.Branch("electron2_ph_Iso", &electron2_ph_Iso);

  reduced_tree.Branch("electron2_NH_Et", &electron2_NH_Et);
  reduced_tree.Branch("electron2_CH_pt", &electron2_CH_pt);
  reduced_tree.Branch("electron2_ph_Et", &electron2_ph_Et);

  reduced_tree.Branch("electron2_minDR_jet", &electron2_minDR_jet);
  reduced_tree.Branch("electron2_mT", &electron2_mT);

  //  reduced_tree.Branch("num_gen_taus", &num_gen_taus);
  //  reduced_tree.Branch("gen_tau1", &gen_tau1, "pt/F:eta:phi/F:minDR:minDPt/F:taus_match:loss_code/I");
  //  reduced_tree.Branch("gen_tau2", &gen_tau2, "pt/F:eta:phi/F:minDR:minDPt/F:taus_match:loss_code/I");


  reduced_tree.Branch("num_gen_leptons", &num_gen_leptons);
  reduced_tree.Branch("num_iso_gen_leptons", &num_iso_gen_leptons);
  // reduced_tree.Branch("num_gen_nus", &num_gen_nus);

  reduced_tree.Branch("num_lost_electrons", &num_lost_electrons);
  reduced_tree.Branch("num_lost_muons", &num_lost_muons);
  reduced_tree.Branch("num_lost_leptons", &num_lost_leptons);

  reduced_tree.Branch("lep1", &lep1);
  reduced_tree.Branch("lep2", &lep2);

 
  Timer timer(GetTotalEntries());
  timer.Start();
  for(int i(0); i<GetTotalEntries(); ++i){
    if(i%1000==0 && i!=0){
      timer.PrintRemainingTime();
    }
    timer.Iterate();
    //        cout << "JERR" << endl;
    GetEntry(i);
    entry=i;
    //    cout << "*****Event " << i << "*****" << endl;

    std::pair<std::set<EventNumber>::iterator, bool> returnVal(eventList.insert(EventNumber(run, event, lumiblock)));
    if(!returnVal.second) continue;
    
    // passesMETCleaningCut=PassesMETCleaningCut();
    // passesPVCut=PassesPVCut();
    
    num_primary_vertices=GetNumVertices();

    pu_true_num_interactions=GetTrueNumInteractions();
    pu_num_interactions=GetNumInteractions();
    eoot_pu=GetEarlyOutOfTimePU(1);
    loot_pu=GetLateOutOfTimePU();
    oot_pu=eoot_pu+loot_pu;
    //       cout << "JERR1" << endl;

    met=pfTypeImets_et->at(0);
    met_phi=pfTypeImets_phi->at(0);
    //        cout << "JERR2" << endl;
    ht=GetHT();
    num_jets_pt20=GetNumGoodJets();
    num_jets_pt50=GetNumGoodJets(50);
    num_jets_pt80=GetNumGoodJets(80);
    num_jets_pt100=GetNumGoodJets(100);
    num_jets_pt150=GetNumGoodJets(150);
    //  cout << "num_jets=" << num_jets << endl;
    num_csvt_jets=GetNumCSVTJets();
    num_csvm_jets=GetNumCSVMJets();
    num_csvl_jets=GetNumCSVLJets();

    num_gen_partons=GetNGenPartons();
    num_gen_partons_pt20=GetNGenPartons(20.);
    num_gen_partons_pt40=GetNGenPartons(40.);
    num_gen_partons_pt70=GetNGenPartons(70.);
    num_gen_partons_pt100=GetNGenPartons(100.);
    num_gen_partons_pt150=GetNGenPartons(150.);


    /*
      num_ra2b_veto_electrons=GetNumRA2bElectrons(0);
      num_ra2b_veto_muons=GetNumRA2bMuons(0);
      //   num_ra2b_veto_taus=GetNumRA2bTaus(0);
      num_ra2b_veto_leptons=num_ra2b_veto_electrons+num_ra2b_veto_muons;
      num_ra2b_noiso_electrons=GetNumRA2bElectrons(0,false);
      num_ra2b_noiso_muons=GetNumRA2bMuons(0,false);
      //   num_ra2b_noiso_taus=GetNumRA2bTaus(0,false);
      num_ra2b_noiso_leptons=num_ra2b_noiso_electrons+num_ra2b_noiso_muons;
      num_ra2b_loose_electrons=GetNumRA2bElectrons(1);
      num_ra2b_loose_muons=GetNumRA2bMuons(1);
      //    num_ra2b_loose_taus=GetNumRA2bTaus(1);
      num_ra2b_loose_leptons=num_ra2b_loose_electrons+num_ra2b_loose_muons;
      num_ra2b_medium_electrons=GetNumRA2bElectrons(2);
      num_ra2b_medium_muons=GetNumRA2bMuons(2);
      //    num_ra2b_medium_taus=GetNumRA2bTaus(2);
      num_ra2b_medium_leptons=num_ra2b_medium_electrons+num_ra2b_medium_muons;
      num_ra2b_tight_electrons=GetNumRA2bElectrons(3);
      num_ra2b_tight_muons=GetNumRA2bMuons(3);
      //    num_ra2b_tight_taus=GetNumRA2bTaus(3);
      num_ra2b_tight_leptons=num_ra2b_tight_electrons+num_ra2b_tight_muons;
    */
    // num_ra2b_iso_tracks=NewGetNumIsoTracks();
    

    double this_scale_factor(scaleFactor);
    //   pu_weight=isRealData?1.0:GetPUWeight(lumiWeights);
    lumi_weight=this_scale_factor;
    //   top_pt_weight=isttbar?GetTopPtWeight():1.0;
    //   top_pt_weight_official=isttbar?GetTopPtWeightOfficial():1.0;
    //   full_weight=pu_weight*lumi_weight*top_pt_weight_official;

    if (!genMuonsUpToDate) SetupGenMuons();
    num_gen_muons=genMuonCache.size();
    num_iso_gen_muons=0;
    num_lost_muons=0;
    for (uint imu(0); imu<num_gen_muons; imu++) {
      if (genMuonCache[imu].IsIso()) num_iso_gen_muons++;
      if (!genMuonCache[imu].IsVeto()) num_lost_muons++;
    }
    num_ignored_gen_muons = GetNumIgnoredGenMuons();
    SetMuonValues(1,gen_mu1);
    SetMuonValues(2,gen_mu2);
    SetMuonValues(3,gen_mu3);
    SetMuonValues(4,gen_mu4);

    if (!genElectronsUpToDate) SetupGenElectrons();
    num_gen_electrons=genElectronCache.size();
    num_iso_gen_electrons=0;
    num_lost_electrons=0;
    for (uint iel(0); iel<num_gen_electrons; iel++) {
      if (genElectronCache[iel].IsIso()) num_iso_gen_electrons++;
      if (!genElectronCache[iel].IsVeto()) num_lost_electrons++;
    }
    num_ignored_gen_electrons = GetNumIgnoredGenElectrons();
    SetElectronValues(1,gen_el1);
    SetElectronValues(2,gen_el2);
    SetElectronValues(3,gen_el3);
    SetElectronValues(4,gen_el4);
 

    num_gen_leptons=num_gen_electrons+num_gen_muons;
    num_iso_gen_leptons=num_iso_gen_electrons+num_iso_gen_muons;

    vector<int> reco_muons, reco_veto_muons;
    if (cmEnergy>8) {
      reco_muons = GetRecoMuons(false);
      reco_veto_muons = GetRecoMuons(true);
    }
    else {
     reco_muons = GetRA2bMuons(false);
     reco_veto_muons = GetRA2bMuons(true);
     //    cout << "JERR3" << endl;
    }
    num_reco_muons = reco_muons.size();
    num_reco_veto_muons = reco_veto_muons.size();
    
    vector<int> reco_electrons, reco_veto_electrons;
     if (cmEnergy>8) {
       reco_electrons = GetRecoElectrons(false);
       reco_veto_electrons = GetRecoElectrons(true);
     }
     else {
       reco_electrons = GetRA2bElectrons(false);
       reco_veto_electrons = GetRA2bElectrons(true);
       //        cout << "JERR4" << endl;
     }

    num_reco_electrons = reco_electrons.size();
    num_reco_veto_electrons = reco_veto_electrons.size();
    
    //  num_lost_electrons=num_gen_electrons-num_reco_veto_electrons;
    //  num_lost_muons=num_gen_muons-num_reco_veto_muons;
    num_lost_leptons=num_lost_electrons+num_lost_muons;

    muon_gen_mother_id=-999;
    muon_gen_pt=-999.;
    muon_gen_eta=-999.;
    muon_gen_phi=-999.;
    muon_reco_match=-1;
    muon_signal=-1;
    muon_veto=-1;
    muon_PFmatched=-1;
    muon_pt=-1.;
    muon_eta=-999.;
    muon_phi=-999.;
    muon_relIso=-1.;
    muon_NH_Iso=-1.;
    muon_CH_Iso=-1.;
    muon_ph_Iso=-1.;
    muon_PU_Iso=-1.;  
    muon_NH_Et=-1.;
    muon_CH_pt=-1.;
    muon_ph_Et=-1.;
    muon_PU_pt=-1.;
    muon_minDR_jet=-1;
    muon_mT=-1;
    muon2_gen_mother_id=-999;
    muon2_gen_pt=-999.;
    muon2_gen_eta=-999.;
    muon2_gen_phi=-999.;
    muon2_reco_match=-1;
    muon2_signal=-1;
    muon2_veto=-1;
    muon2_PFmatched=-1;
    muon2_pt=-1.;
    muon2_eta=-999.;
    muon2_phi=-999.;
    muon2_relIso=-1.;
    muon2_NH_Iso=-1.;
    muon2_CH_Iso=-1.;
    muon2_ph_Iso=-1.;
    muon2_PU_Iso=-1.;  
    muon2_NH_Et=-1.;
    muon2_CH_pt=-1.;
    muon2_ph_Et=-1.;
    muon2_PU_pt=-1.;
    muon2_minDR_jet=-1;    
    muon2_mT=-1;
  //  cout << "JERRmu" << endl;

    if (num_gen_muons>=1) {
      muon_gen_mother_id=genMuonCache[0].GetMotherId();
      muon_gen_pt=genMuonCache[0].GetLorentzVector().Pt();
      muon_gen_eta=genMuonCache[0].GetLorentzVector().Eta();
      muon_gen_phi=genMuonCache[0].GetLorentzVector().Phi();
      if(genMuonCache[0].GetMusMatch()>=0) {
	muon_reco_match = genMuonCache[0].GetMusMatch();
	if (cmEnergy>=13) {
	  muon_signal=isRecoMuon(muon_reco_match,1);
	  muon_veto=isRecoMuon(muon_reco_match,0);
	  muon_PFmatched=mus_isPF->at(muon_reco_match);
	  muon_pt=mus_pt->at(muon_reco_match);
	  muon_eta=mus_eta->at(muon_reco_match);
	  muon_phi=mus_phi->at(muon_reco_match);
	  muon_NH_Et=mus_pfIsolationR04_sumNeutralHadronEt->at(muon_reco_match);
	  muon_CH_pt=mus_pfIsolationR04_sumChargedHadronPt->at(muon_reco_match);
	  muon_ph_Et=mus_pfIsolationR04_sumPhotonEt->at(muon_reco_match);
	  muon_PU_pt=mus_pfIsolationR04_sumPUPt->at(muon_reco_match);  
	  muon_relIso=GetMuonRelIso(muon_reco_match); 
	}
	else {
	  muon_signal=isRA2bMuon(muon_reco_match,1);
	  muon_veto=isRA2bMuon(muon_reco_match,0);
	  muon_PFmatched=true;
	  muon_pt=pf_mus_pt->at(muon_reco_match);
	  muon_eta=pf_mus_eta->at(muon_reco_match);
	  muon_phi=pf_mus_phi->at(muon_reco_match);
	  muon_NH_Et=pf_mus_pfIsolationR04_sumNeutralHadronEt->at(muon_reco_match);
	  muon_CH_pt=pf_mus_pfIsolationR04_sumChargedHadronPt->at(muon_reco_match);
	  muon_ph_Et=pf_mus_pfIsolationR04_sumPhotonEt->at(muon_reco_match);
	  muon_PU_pt=pf_mus_pfIsolationR04_sumPUPt->at(muon_reco_match);
	  muon_relIso=GetRA2bMuonRelIso(muon_reco_match);
	}
	muon_NH_Iso=muon_NH_Et/muon_pt;
	muon_CH_Iso=muon_CH_pt/muon_pt;
	muon_ph_Iso=muon_ph_Et/muon_pt;
	muon_PU_Iso=muon_PU_pt/muon_pt;
	muon_minDR_jet=GetMinDRMuonJet(muon_reco_match); // compatible with both old and new samples
	muon_mT=GetMTW(muon_pt,met,muon_phi,met_phi);
	//	cout << "muon_minDR_jet: " << muon_minDR_jet << endl;
      }
    }
    if (num_gen_muons>=2) {
      muon2_gen_mother_id=genMuonCache[1].GetMotherId();
      muon2_gen_pt=genMuonCache[1].GetLorentzVector().Pt();
      muon2_gen_eta=genMuonCache[1].GetLorentzVector().Eta();
      muon2_gen_phi=genMuonCache[1].GetLorentzVector().Phi();
      if(genMuonCache[1].GetMusMatch()>=0) {
	muon2_reco_match = genMuonCache[1].GetMusMatch();
	if (cmEnergy>=13) {
	  muon2_signal=isRecoMuon(muon2_reco_match,1);
	  muon2_veto=isRecoMuon(muon2_reco_match,0);
	  muon2_PFmatched=mus_isPF->at(muon2_reco_match);
	  muon2_pt=mus_pt->at(muon2_reco_match);
	  muon2_eta=mus_eta->at(muon2_reco_match);
	  muon2_phi=mus_phi->at(muon2_reco_match);
	  muon2_NH_Et=mus_pfIsolationR04_sumNeutralHadronEt->at(muon2_reco_match);
	  muon2_CH_pt=mus_pfIsolationR04_sumChargedHadronPt->at(muon2_reco_match);
	  muon2_ph_Et=mus_pfIsolationR04_sumPhotonEt->at(muon2_reco_match);
	  muon2_PU_pt=mus_pfIsolationR04_sumPUPt->at(muon2_reco_match);  
	  muon2_relIso=GetMuonRelIso(muon2_reco_match); 
	}
	else {
	  muon2_signal=isRA2bMuon(muon2_reco_match,1);
	  muon2_veto=isRA2bMuon(muon2_reco_match,0);
	  muon2_PFmatched=true;
	  muon2_pt=pf_mus_pt->at(muon2_reco_match);
	  muon2_eta=pf_mus_eta->at(muon2_reco_match);
	  muon2_phi=pf_mus_phi->at(muon2_reco_match);
	  muon2_NH_Et=pf_mus_pfIsolationR04_sumNeutralHadronEt->at(muon2_reco_match);
	  muon2_CH_pt=pf_mus_pfIsolationR04_sumChargedHadronPt->at(muon2_reco_match);
	  muon2_ph_Et=pf_mus_pfIsolationR04_sumPhotonEt->at(muon2_reco_match);
	  muon2_PU_pt=pf_mus_pfIsolationR04_sumPUPt->at(muon2_reco_match);
	  muon2_relIso=GetRA2bMuonRelIso(muon2_reco_match);
	}
	muon2_NH_Iso=muon2_NH_Et/muon2_pt;
	muon2_CH_Iso=muon2_CH_pt/muon2_pt;
	muon2_ph_Iso=muon2_ph_Et/muon2_pt;
	muon2_PU_Iso=muon2_PU_pt/muon2_pt;
	muon2_minDR_jet=GetMinDRMuonJet(muon2_reco_match); // compatible with both old and new samples
	muon2_mT=GetMTW(muon2_pt,met,muon2_phi,met_phi);
	//	cout << "muon_minDR_jet: " << muon_minDR_jet << endl;
      }
    }

    electron_gen_mother_id=-999;
    electron_gen_pt=-999.;
    electron_gen_eta=-999.;
    electron_gen_phi=-999.;
    electron_reco_match=-1;
    electron_signal=-1;
    electron_veto=-1;
    electron_PFmatched=-1;
    electron_pt=-1.;
    electron_eta=-999.;
    electron_phi=-999.;
    electron_relIso=-1.;
    electron_NH_Iso=-1.;
    electron_CH_Iso=-1.;
    electron_ph_Iso=-1.;
    electron_NH_Et=-1.;
    electron_CH_pt=-1.;
    electron_ph_Et=-1.;
    electron_minDR_jet=-1;
    electron_mT=-1;
    electron2_gen_mother_id=-999;
    electron2_gen_pt=-999.;
    electron2_gen_eta=-999.;
    electron2_gen_phi=-999.;
    electron2_reco_match=-1;
    electron2_signal=-1;
    electron2_veto=-1;
    electron2_PFmatched=-1;
    electron2_pt=-1.;
    electron2_eta=-999.;
    electron2_phi=-999.;
    electron2_relIso=-1.;
    electron2_NH_Iso=-1.;
    electron2_CH_Iso=-1.;
    electron2_ph_Iso=-1.;
    electron2_NH_Et=-1.;
    electron2_CH_pt=-1.;
    electron2_ph_Et=-1.;
    electron2_minDR_jet=-1;    
    electron2_mT=-1;
   //  cout << "JERRel" << endl;

    if (num_gen_electrons>=1) {
      electron_gen_mother_id=genElectronCache[0].GetMotherId();
      electron_gen_pt=genElectronCache[0].GetLorentzVector().Pt();
      electron_gen_eta=genElectronCache[0].GetLorentzVector().Eta();
      electron_gen_phi=genElectronCache[0].GetLorentzVector().Phi();
      if(genElectronCache[0].GetElsMatch()>=0) {
	electron_reco_match = genElectronCache[0].GetElsMatch();
	electron_signal=isRecoElectron(electron_reco_match,1);
	electron_veto=isRecoElectron(electron_reco_match,0);
	electron_PFmatched=hasPFMatch(electron_reco_match, particleId::electron);
	electron_pt=els_pt->at(electron_reco_match);
	electron_eta=els_eta->at(electron_reco_match);
	electron_phi=els_phi->at(electron_reco_match);
	if (cmEnergy<13) { 
	  electron_relIso=GetElectronRelIso(electron_reco_match);
	  electron_NH_Et=els_PFneutralHadronIsoR03->at(electron_reco_match);
	  electron_CH_pt=els_PFchargedHadronIsoR03->at(electron_reco_match);
	  electron_ph_Et=els_PFphotonIsoR03->at(electron_reco_match);
	}
	else {
	  electron_relIso=GetCSAElectronIsolation(electron_reco_match);
	  electron_NH_Et=els_pfIsolationR03_sumNeutralHadronEt->at(electron_reco_match);
	  electron_CH_pt=els_pfIsolationR03_sumChargedHadronPt->at(electron_reco_match);
	  electron_ph_Et=els_pfIsolationR03_sumPhotonEt->at(electron_reco_match);
	}
	electron_NH_Iso=electron_NH_Et/electron_pt;
	electron_CH_Iso=electron_CH_pt/electron_pt;
	electron_ph_Iso=electron_ph_Et/electron_pt;
	electron_minDR_jet=GetMinDRElectronJet(electron_reco_match); // compatible with both old and new samples
	electron_mT=GetMTW(electron_pt,met,electron_phi,met_phi);
      }
    }
    if (num_gen_electrons>=2) {
      electron2_gen_mother_id=genElectronCache[1].GetMotherId();
      electron2_gen_pt=genElectronCache[1].GetLorentzVector().Pt();
      electron2_gen_eta=genElectronCache[1].GetLorentzVector().Eta();
      electron2_gen_phi=genElectronCache[1].GetLorentzVector().Phi();
      if(genElectronCache[1].GetElsMatch()>=0) {
	electron2_reco_match = genElectronCache[1].GetElsMatch();
	electron2_signal=isRecoElectron(electron2_reco_match,1);
	electron2_veto=isRecoElectron(electron2_reco_match,0);
	electron2_PFmatched=hasPFMatch(electron2_reco_match, particleId::electron);
	electron2_pt=els_pt->at(electron2_reco_match);
	electron2_eta=els_eta->at(electron2_reco_match);
	electron2_phi=els_phi->at(electron2_reco_match);
	if (cmEnergy<13) { 
	  electron2_relIso=GetElectronRelIso(electron2_reco_match);
	  electron2_NH_Et=els_PFneutralHadronIsoR03->at(electron2_reco_match);
	  electron2_CH_pt=els_PFchargedHadronIsoR03->at(electron2_reco_match);
	  electron2_ph_Et=els_PFphotonIsoR03->at(electron2_reco_match);
	}
	else {
	  electron2_relIso=GetCSAElectronIsolation(electron2_reco_match);
	  electron2_NH_Et=els_pfIsolationR03_sumNeutralHadronEt->at(electron2_reco_match);
	  electron2_CH_pt=els_pfIsolationR03_sumChargedHadronPt->at(electron2_reco_match);
	  electron2_ph_Et=els_pfIsolationR03_sumPhotonEt->at(electron2_reco_match);
	}
	electron2_NH_Iso=electron2_NH_Et/electron2_pt;
	electron2_CH_Iso=electron2_CH_pt/electron2_pt;
	electron2_ph_Iso=electron2_ph_Et/electron2_pt;
	electron2_minDR_jet=GetMinDRElectronJet(electron2_reco_match); // compatible with both old and new samples
 	electron2_mT=GetMTW(electron2_pt,met,electron2_phi,met_phi);
     }
    }

    if (num_gen_muons>=1||num_gen_electrons>=1) {
      if(muon_gen_pt>electron_gen_pt) { // muon is leading
	lep1=1;
	if (electron_gen_pt>0) lep2=3;
	if (muon2_gen_pt>electron_gen_pt) lep2=2;
      }
      else { // electron is leading
	lep1=3;
	if (muon_gen_pt>0) lep2=1;
	if (electron2_gen_pt>muon_gen_pt) lep2=4;
      }
    }

    reduced_tree.Fill(); 
    //  if (i==20) break;

  }

  reduced_tree.Write();
  file.Close();
}

void ReducedTreeMaker::SetMuonValues(uint index, genMuInfo& gen_mu) {
  //  gen_mu = {0, -FLT_MAX, -FLT_MAX, -FLT_MAX, -999, -1., -1., -1, -1., -1., 
  //	    0, 0, -FLT_MAX, -FLT_MAX, -FLT_MAX, -FLT_MAX, -FLT_MAX, -FLT_MAX, -FLT_MAX, -FLT_MAX, -FLT_MAX, -FLT_MAX, -FLT_MAX, -FLT_MAX, -FLT_MAX, -FLT_MAX, -FLT_MAX, 0, 999.};
  if (index>genMuonCache.size()) return;
  
  gen_mu.is_iso=genMuonCache[index-1].IsIso();
  gen_mu.pt=genMuonCache[index-1].GetLorentzVector().Pt();
  gen_mu.eta=genMuonCache[index-1].GetLorentzVector().Eta();
  gen_mu.phi=genMuonCache[index-1].GetLorentzVector().Phi();
  gen_mu.mother_id=genMuonCache[index-1].GetMotherId();
  gen_mu.minDR=genMuonCache[index-1].GetMinDR().second;
  gen_mu.minDPt=genMuonCache[index-1].GetMinDPt().second;
  gen_mu.mus_match=genMuonCache[index-1].GetMusMatch();
  gen_mu.topPt=genMuonCache[index-1].GetTopPt();
  gen_mu.deltaRWb=genMuonCache[index-1].GetDeltaRWb();
  //  printf("Gen muon params: %.2f/%.2f/%.2f/%.2f/%.2f/%d\n",gen_mu.pt, gen_mu.eta, gen_mu.phi, gen_mu.minDR, gen_mu.minDPt, gen_mu.mus_match);
  if (gen_mu.mus_match>=0) { // set reco properties
    gen_mu.signal_muon=isRecoMuon(gen_mu.mus_match,1);
    gen_mu.veto_muon=isRecoMuon(gen_mu.mus_match,0);
    gen_mu.reco_pt=mus_pt->at(gen_mu.mus_match);
    gen_mu.reco_eta=mus_eta->at(gen_mu.mus_match);
    gen_mu.reco_phi=mus_phi->at(gen_mu.mus_match); 
    gen_mu.reco_dZ=getDZ(mus_tk_vx->at(gen_mu.mus_match), mus_tk_vy->at(gen_mu.mus_match), mus_tk_vz->at(gen_mu.mus_match), mus_tk_px->at(gen_mu.mus_match), 
			 mus_tk_py->at(gen_mu.mus_match), mus_tk_pz->at(gen_mu.mus_match), 0);
    gen_mu.reco_d0PV=mus_tk_d0dum->at(gen_mu.mus_match)-pv_x->at(0)*sin(mus_tk_phi->at(gen_mu.mus_match))+pv_y->at(0)*cos(mus_tk_phi->at(gen_mu.mus_match));
    gen_mu.reco_relIso=GetMuonRelIso(gen_mu.mus_match);
    gen_mu.reco_EMIso=mus_pfIsolationR04_sumPhotonEt->at(gen_mu.mus_match)/gen_mu.reco_pt;
    gen_mu.reco_CHIso=mus_pfIsolationR04_sumChargedHadronPt->at(gen_mu.mus_match)/gen_mu.reco_pt;
    gen_mu.reco_NHIso=mus_pfIsolationR04_sumNeutralHadronEt->at(gen_mu.mus_match)/gen_mu.reco_pt;
    gen_mu.reco_PUIso=mus_pfIsolationR04_sumPUPt->at(gen_mu.mus_match)/gen_mu.reco_pt;
    gen_mu.reco_IsoEn=gen_mu.reco_relIso*gen_mu.reco_pt;
    gen_mu.reco_EMEn=mus_pfIsolationR04_sumPhotonEt->at(gen_mu.mus_match);
    gen_mu.reco_CHEn=mus_pfIsolationR04_sumChargedHadronPt->at(gen_mu.mus_match);
    gen_mu.reco_NHEn=mus_pfIsolationR04_sumNeutralHadronEt->at(gen_mu.mus_match);
    gen_mu.reco_PUEn=mus_pfIsolationR04_sumPUPt->at(gen_mu.mus_match);
    gen_mu.reco_hasPFMatch=hasPFMatch(gen_mu.mus_match, particleId::muon);
    gen_mu.reco_minDR=GetMinDRMuonJet(gen_mu.mus_match);
    // printf("Reco muon params: %d/%d/%.2f\n",gen_mu.signal_muon, gen_mu.veto_muon, gen_mu.reco_pt);
  }
}

void ReducedTreeMaker::SetElectronValues(uint index, genElInfo& gen_el) {
  //  gen_el = {0, -FLT_MAX, -FLT_MAX, -FLT_MAX, -999, -1., -1., -1, -1., -1., 
  //	    0, 0, -FLT_MAX, -FLT_MAX, -FLT_MAX, -FLT_MAX, -FLT_MAX, -FLT_MAX, -FLT_MAX, -FLT_MAX, -FLT_MAX, -FLT_MAX, 0, 999.};
  if (index>genElectronCache.size()) return;
  // cout << "Gen properties..." << endl;
  
  gen_el.is_iso=genElectronCache[index-1].IsIso();
  gen_el.pt=genElectronCache[index-1].GetLorentzVector().Pt();
  gen_el.eta=genElectronCache[index-1].GetLorentzVector().Eta();
  gen_el.phi=genElectronCache[index-1].GetLorentzVector().Phi();
  gen_el.mother_id=genElectronCache[index-1].GetMotherId();
  gen_el.minDR=genElectronCache[index-1].GetMinDR().second;
  gen_el.minDPt=genElectronCache[index-1].GetMinDPt().second;
  gen_el.topPt=genElectronCache[index-1].GetTopPt();
  gen_el.deltaRWb=genElectronCache[index-1].GetDeltaRWb();
  gen_el.els_match=genElectronCache[index-1].GetElsMatch();
  if (gen_el.els_match>=0) { // set reco properties
    gen_el.signal_electron=isRecoElectron(gen_el.els_match,3);
    gen_el.veto_electron=isRecoElectron(gen_el.els_match,0);
    gen_el.reco_pt=els_pt->at(gen_el.els_match);
    gen_el.reco_eta=els_scEta->at(gen_el.els_match);
    gen_el.reco_phi=els_phi->at(gen_el.els_match); 
    gen_el.reco_dZ=getDZ(els_vx->at(gen_el.els_match), els_vy->at(gen_el.els_match), els_vz->at(gen_el.els_match), cos(els_tk_phi->at(gen_el.els_match))*els_tk_pt->at(gen_el.els_match), 
			 sin(els_tk_phi->at(gen_el.els_match))*els_tk_pt->at(gen_el.els_match), els_tk_pz->at(gen_el.els_match), 0);
    gen_el.reco_d0PV=els_d0dum->at(gen_el.els_match)-pv_x->at(0)*sin(els_tk_phi->at(gen_el.els_match))+pv_y->at(0)*cos(els_tk_phi->at(gen_el.els_match));
    //   cout << "Iso properties..." << endl;
    if (cmEnergy<13) {
      gen_el.reco_relIso=GetElectronRelIso(gen_el.els_match);
      gen_el.reco_EMIso=els_PFphotonIsoR03->at(gen_el.els_match)/gen_el.reco_pt;
      gen_el.reco_CHIso=els_PFchargedHadronIsoR03->at(gen_el.els_match)/gen_el.reco_pt;
      gen_el.reco_NHIso=els_PFneutralHadronIsoR03->at(gen_el.els_match)/gen_el.reco_pt;
      //  gen_el.reco_PUIso=els_pfIsolationR03_sumPUPt->at(gen_el.els_match)/gen_el.reco_pt;
    }
    else {
      gen_el.reco_relIso=GetCSAElectronIsolation(gen_el.els_match);
      gen_el.reco_EMIso=els_pfIsolationR03_sumPhotonEt->at(gen_el.els_match)/gen_el.reco_pt;
      gen_el.reco_CHIso=els_pfIsolationR03_sumChargedHadronPt->at(gen_el.els_match)/gen_el.reco_pt;
      gen_el.reco_NHIso=els_pfIsolationR03_sumNeutralHadronEt->at(gen_el.els_match)/gen_el.reco_pt;
      gen_el.reco_PUIso=els_pfIsolationR03_sumPUPt->at(gen_el.els_match)/gen_el.reco_pt;
    }
    gen_el.reco_hasPFMatch=hasPFMatch(gen_el.els_match, particleId::electron);
    gen_el.reco_minDR=GetMinDRElectronJet(gen_el.els_match);
    // if (gen_el.signal_electron) printf("Reco electron params: %d/%d/%.2f\n",gen_el.signal_electron, gen_el.veto_electron, gen_el.reco_pt);
  }
  
}

void ReducedTreeMaker::SetTauValues(uint index, genTauInfo& gen_tau) {
  gen_tau.pt=genTauCache[index-1].GetLorentzVector().Pt();
  gen_tau.eta=genTauCache[index-1].GetLorentzVector().Eta();
  gen_tau.phi=genTauCache[index-1].GetLorentzVector().Phi();
  gen_tau.minDR=genTauCache[index-1].GetMinDR().second;
  gen_tau.minDPt=genTauCache[index-1].GetMinDPt().second;
  gen_tau.taus_match=genTauCache[index-1].GetTausMatch();
  gen_tau.loss_code=genTauCache[index-1].GetLossCode();
}
