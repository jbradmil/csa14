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


  float pu_true_num_interactions(0.0);
  unsigned short num_primary_vertices(0);

  float met(0.0), ht(0.0);

  unsigned short num_jets(0), num_csvt_jets(0), num_csvm_jets(0), num_csvl_jets(0);


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
  reduced_tree.Branch("num_primary_vertices", &num_primary_vertices);

  reduced_tree.Branch("met", &met);
  reduced_tree.Branch("ht", &ht);

  reduced_tree.Branch("num_jets", &num_jets); 
  reduced_tree.Branch("num_csvt_jets", &num_csvt_jets); 
  reduced_tree.Branch("num_csvm_jets", &num_csvm_jets); 
  reduced_tree.Branch("num_csvl_jets", &num_csvl_jets); 


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

  string gen_muon_branches("is_iso/I:pt/F:eta/F:phi/F:mother_id/I:minDR/F:minDPt/F:mus_match/I:signal_muon/I:veto_muon/I:reco_pt/F:reco_eta/F:reco_phi/F:reco_dZ/F:reco_d0PV/F:reco_relIso/F:reco_hasPFMatch/I");
  reduced_tree.Branch("gen_mu1", &gen_mu1, gen_muon_branches.c_str());
  reduced_tree.Branch("gen_mu2", &gen_mu2, gen_muon_branches.c_str());
  reduced_tree.Branch("gen_mu3", &gen_mu3, gen_muon_branches.c_str());
  reduced_tree.Branch("gen_mu4", &gen_mu4, gen_muon_branches.c_str());

  reduced_tree.Branch("num_reco_veto_muons", &num_reco_veto_muons);
  reduced_tree.Branch("num_reco_muons", &num_reco_muons);

  reduced_tree.Branch("num_gen_electrons", &num_gen_electrons);
  reduced_tree.Branch("num_iso_gen_electrons", &num_iso_gen_electrons);
  reduced_tree.Branch("num_ignored_gen_electrons", &num_ignored_gen_electrons);

  string gen_electron_branches("is_iso/I:pt/F:eta/F:phi/F:mother_id/I:minDR/F:minDPt/F:els_match/I:signal_electron/I:veto_electron/I:reco_pt/F:reco_eta/F:reco_phi/F:reco_dZ/F:reco_d0PV/F:reco_relIso/F:reco_hasPFMatch/I");
  reduced_tree.Branch("gen_el1", &gen_el1, gen_electron_branches.c_str());
  reduced_tree.Branch("gen_el2", &gen_el2, gen_electron_branches.c_str());
  reduced_tree.Branch("gen_el3", &gen_el3, gen_electron_branches.c_str());
  reduced_tree.Branch("gen_el4", &gen_el4, gen_electron_branches.c_str());

  reduced_tree.Branch("num_reco_veto_electrons", &num_reco_veto_electrons);
  reduced_tree.Branch("num_reco_electrons", &num_reco_electrons);


  //  reduced_tree.Branch("num_gen_taus", &num_gen_taus);
  //  reduced_tree.Branch("gen_tau1", &gen_tau1, "pt/F:eta:phi/F:minDR:minDPt/F:taus_match:loss_code/I");
  //  reduced_tree.Branch("gen_tau2", &gen_tau2, "pt/F:eta:phi/F:minDR:minDPt/F:taus_match:loss_code/I");


  reduced_tree.Branch("num_gen_leptons", &num_gen_leptons);
  reduced_tree.Branch("num_iso_gen_leptons", &num_iso_gen_leptons);
  // reduced_tree.Branch("num_gen_nus", &num_gen_nus);

  reduced_tree.Branch("num_lost_electrons", &num_lost_electrons);
  reduced_tree.Branch("num_lost_muons", &num_lost_muons);
  reduced_tree.Branch("num_lost_leptons", &num_lost_leptons);

 
  Timer timer(GetTotalEntries());
  timer.Start();
  for(int i(0); i<GetTotalEntries(); ++i){
    if(i%1000==0 && i!=0){
      timer.PrintRemainingTime();
    }
    timer.Iterate();
    //   cout << "JERR" << endl;
    GetEntry(i);
    entry=i;
    //  cout << "*****Event " << i << "*****" << endl;

    std::pair<std::set<EventNumber>::iterator, bool> returnVal(eventList.insert(EventNumber(run, event, lumiblock)));
    if(!returnVal.second) continue;
    
    // passesMETCleaningCut=PassesMETCleaningCut();
    // passesPVCut=PassesPVCut();

    if (cmEnergy<=8) pu_true_num_interactions=GetNumInteractions();
    else pu_true_num_interactions=20;
    num_primary_vertices=GetNumVertices();

    met=pfmets_et->at(0);
    ht=GetHT();
    num_jets=GetNumGoodJets();
    num_csvt_jets=GetNumCSVTJets();
    num_csvm_jets=GetNumCSVMJets();
    num_csvl_jets=GetNumCSVLJets();

    //   cout << "JERR" << endl;

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
    //   cout << "JERR1" << endl;

    if (!genElectronsUpToDate) SetupGenElectrons();
    num_gen_electrons=genElectronCache.size();
    num_iso_gen_electrons=0;
    num_lost_electrons=0;
  for (uint iel(0); iel<num_gen_electrons; iel++) {
      if (genElectronCache[iel].IsIso()) num_iso_gen_electrons++;
      if (!genElectronCache[iel].IsVeto()) num_lost_electrons++;
    }
    num_ignored_gen_electrons = GetNumIgnoredGenElectrons();
    //   cout << "JERR2" << endl;
    SetElectronValues(1,gen_el1);
    SetElectronValues(2,gen_el2);
    SetElectronValues(3,gen_el3);
    SetElectronValues(4,gen_el4);
    //   cout << "JERR3" << endl;
 

    num_gen_leptons=num_gen_electrons+num_gen_muons;
    num_iso_gen_leptons=num_iso_gen_electrons+num_iso_gen_muons;

    vector<int> reco_muons = GetRecoMuons(false);
    vector<int> reco_veto_muons = GetRecoMuons(true);
    num_reco_muons = reco_muons.size();
    num_reco_veto_muons = reco_veto_muons.size();
    
    vector<int> reco_electrons, reco_veto_electrons;
    reco_electrons = GetRecoElectrons(false);
    reco_veto_electrons = GetRecoElectrons(true);


    num_reco_electrons = reco_electrons.size();
    num_reco_veto_electrons = reco_veto_electrons.size();
    
    //  num_lost_electrons=num_gen_electrons-num_reco_veto_electrons;
    //  num_lost_muons=num_gen_muons-num_reco_veto_muons;
    num_lost_leptons=num_lost_electrons+num_lost_muons;

    reduced_tree.Fill(); 
    if (i==100000) break;

  }

  reduced_tree.Write();
  file.Close();
}

void ReducedTreeMaker::SetMuonValues(uint index, genMuInfo& gen_mu) {
  gen_mu = {0, -FLT_MAX, -FLT_MAX, -FLT_MAX, -999, -1., -1., -1, 0, 0, -FLT_MAX, -FLT_MAX, -FLT_MAX, -FLT_MAX, -FLT_MAX, -FLT_MAX, 0};
  if (index>genMuonCache.size()) return;
  
  gen_mu.is_iso=genMuonCache[index-1].IsIso();
  gen_mu.pt=genMuonCache[index-1].GetLorentzVector().Pt();
  gen_mu.eta=genMuonCache[index-1].GetLorentzVector().Eta();
  gen_mu.phi=genMuonCache[index-1].GetLorentzVector().Phi();
  gen_mu.mother_id=genMuonCache[index-1].GetMotherId();
  gen_mu.minDR=genMuonCache[index-1].GetMinDR().second;
  gen_mu.minDPt=genMuonCache[index-1].GetMinDPt().second;
  gen_mu.mus_match=genMuonCache[index-1].GetMusMatch();
  // printf("Gen muon params: %.2f/%.2f/%.2f/%.2f/%.2f/%d\n",gen_mu.pt, gen_mu.eta, gen_mu.phi, gen_mu.minDR, gen_mu.minDPt, gen_mu.mus_match);
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
    int pfIdx(-1);
    gen_mu.reco_hasPFMatch=hasPFMatch(gen_mu.mus_match, particleId::muon, pfIdx);
    // if (gen_mu.signal_muon) printf("Reco muon params: %d/%d/%.2f\n",gen_mu.signal_muon, gen_mu.veto_muon, gen_mu.reco_pt);
  }
}

void ReducedTreeMaker::SetElectronValues(uint index, genElInfo& gen_el) {
  gen_el = {0, -FLT_MAX, -FLT_MAX, -FLT_MAX, -999, -1., -1., -1, 0, 0, -FLT_MAX, -FLT_MAX, -FLT_MAX, -FLT_MAX, -FLT_MAX, -FLT_MAX, 0};
  if (index>genElectronCache.size()) return;
  // cout << "Gen properties..." << endl;
  
  gen_el.is_iso=genElectronCache[index-1].IsIso();
  gen_el.pt=genElectronCache[index-1].GetLorentzVector().Pt();
  gen_el.eta=genElectronCache[index-1].GetLorentzVector().Eta();
  gen_el.phi=genElectronCache[index-1].GetLorentzVector().Phi();
  gen_el.mother_id=genElectronCache[index-1].GetMotherId();
  gen_el.minDR=genElectronCache[index-1].GetMinDR().second;
  gen_el.minDPt=genElectronCache[index-1].GetMinDPt().second;
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
   if (cmEnergy<13) gen_el.reco_relIso=GetElectronRelIso(gen_el.els_match);
    else gen_el.reco_relIso=GetCSAElectronIsolation(gen_el.els_match);
    int pfIdx(-1);
    gen_el.reco_hasPFMatch=hasPFMatch(gen_el.els_match, particleId::electron, pfIdx);
    // if (gen_el.signal_electron) printf("Reco electron params: %d/%d/%.2f\n",gen_el.signal_electron, gen_el.veto_electron, gen_el.reco_pt);
  }
  
}

void ReducedTreeMaker::SetTauValues(uint index, genTauInfo& gen_tau) {
  if (index>genTauCache.size()) {
    gen_tau = {-FLT_MAX, -FLT_MAX, -FLT_MAX, -1., -1., -1, -1};
    return;
  }
  gen_tau.pt=genTauCache[index-1].GetLorentzVector().Pt();
  gen_tau.eta=genTauCache[index-1].GetLorentzVector().Eta();
  gen_tau.phi=genTauCache[index-1].GetLorentzVector().Phi();
  gen_tau.minDR=genTauCache[index-1].GetMinDR().second;
  gen_tau.minDPt=genTauCache[index-1].GetMinDPt().second;
  gen_tau.taus_match=genTauCache[index-1].GetTausMatch();
  gen_tau.loss_code=genTauCache[index-1].GetLossCode();
}
