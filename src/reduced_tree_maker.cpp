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

  const bool isRealData(sampleName.find("Run2012")!=std::string::npos);
  const bool isttbar(sampleName.find("TTJets")!=std::string::npos || sampleName.find("TT_")!=std::string::npos);
  std::vector<float> dataDist(pu::RunsThrough203002, pu::RunsThrough203002+60);
  std::vector<float> MCDist(pu::Summer2012_S10, pu::Summer2012_S10+60);//QQQ this needs to change later for general pileup scenario
  reweight::LumiReWeighting lumiWeights(MCDist, dataDist);

  TTree reduced_tree("reduced_tree","reduced_tree");

  int entry(0);
  bool passesPVCut(false), passesMETCleaningCut(false);


  float pu_true_num_interactions(0.0);
  unsigned short num_primary_vertices(0);

  float met_sig(0.0), met(0.0);
  unsigned short num_ra2b_iso_tracks(0);
  unsigned short num_ra2b_veto_electrons(0), num_ra2b_veto_muons(0), num_ra2b_veto_taus(0), num_ra2b_veto_leptons(0);
  unsigned short num_ra2b_noiso_electrons(0), num_ra2b_noiso_muons(0), num_ra2b_noiso_taus(0), num_ra2b_noiso_leptons(0);
  unsigned short num_ra2b_loose_electrons(0), num_ra2b_loose_muons(0), num_ra2b_loose_taus(0), num_ra2b_loose_leptons(0);
  unsigned short num_ra2b_medium_electrons(0), num_ra2b_medium_muons(0), num_ra2b_medium_taus(0), num_ra2b_medium_leptons(0);
  unsigned short num_ra2b_tight_electrons(0), num_ra2b_tight_muons(0), num_ra2b_tight_taus(0), num_ra2b_tight_leptons(0);
  unsigned short num_iso_tracks(0);
 
  float full_weight(0.0), lumi_weight(0.0), top_pt_weight(0.0), top_pt_weight_official(0.0), pu_weight(0.0);

  unsigned short num_gen_electrons(0), num_gen_muons(0), num_gen_taus(0), num_gen_nus(0), num_gen_leptons(0);
  unsigned short num_ignored_gen_muons(0);

  const int nptbins(6);
  float pt_thresholds[nptbins] = {0., 5., 10., 20., 30., 40};
  vector<int> num_gen_muons_pt(nptbins,0);

  int num_lost_electrons(0), num_lost_muons(0), num_lost_taus(0);

  int num_ra4_muons(0), num_ra4_veto_muons(0);

  genMuInfo gen_mu1, gen_mu2, gen_mu3, gen_mu4;
  genElInfo gen_el1, gen_el2, gen_el3, gen_el4;
  genTauInfo gen_tau1, gen_tau2;


  reduced_tree.Branch("passesPVCut",&passesPVCut);
  reduced_tree.Branch("passesMETCleaningCut",&passesMETCleaningCut);


  reduced_tree.Branch("pu_true_num_interactions", &pu_true_num_interactions);
  reduced_tree.Branch("num_primary_vertices", &num_primary_vertices);

  reduced_tree.Branch("met_sig", &met_sig);
  reduced_tree.Branch("met", &met);

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

  reduced_tree.Branch("num_iso_tracks", &num_iso_tracks);

  reduced_tree.Branch("full_weight", &full_weight);
  reduced_tree.Branch("lumi_weight", &lumi_weight);
  reduced_tree.Branch("pu_weight", &pu_weight);
  reduced_tree.Branch("top_pt_weight", &top_pt_weight);
  reduced_tree.Branch("top_pt_weight_official", &top_pt_weight_official);
 
  reduced_tree.Branch("run", &run);
  reduced_tree.Branch("event", &event);
  reduced_tree.Branch("lumiblock", &lumiblock);
  reduced_tree.Branch("entry", &entry);




  for (uint pt_bin(0); pt_bin<nptbins; pt_bin++) {
    char iname[100];
    sprintf(iname, "n_gen_muons_pt%d", (int)pt_thresholds[pt_bin]);
    reduced_tree.Branch(iname, &num_gen_muons_pt[pt_bin]);
  }


  reduced_tree.Branch("num_lost_electrons", &num_lost_electrons);
  reduced_tree.Branch("num_lost_muons", &num_lost_muons);
  reduced_tree.Branch("num_lost_taus", &num_lost_taus);

  reduced_tree.Branch("num_ra4_veto_muons", &num_ra4_veto_muons);
  reduced_tree.Branch("num_ra4_muons", &num_ra4_muons);

  reduced_tree.Branch("num_gen_muons", &num_gen_muons);
  reduced_tree.Branch("num_ignored_gen_muons", &num_ignored_gen_muons);
  reduced_tree.Branch("gen_mu1", &gen_mu1, "pt/F:eta:phi/F:minDR:minDPt/F:mus_match:loss_code/I");
  reduced_tree.Branch("gen_mu2", &gen_mu2, "pt/F:eta:phi/F:minDR:minDPt/F:mus_match:loss_code/I");
  reduced_tree.Branch("gen_mu3", &gen_mu3, "pt/F:eta:phi/F:minDR:minDPt/F:mus_match:loss_code/I");
  reduced_tree.Branch("gen_mu4", &gen_mu4, "pt/F:eta:phi/F:minDR:minDPt/F:mus_match:loss_code/I");

  reduced_tree.Branch("num_gen_electrons", &num_gen_electrons);
  reduced_tree.Branch("gen_el1", &gen_el1, "pt/F:eta:phi/F:minDR:minDPt/F:els_match:loss_code/I");
  reduced_tree.Branch("gen_el2", &gen_el2, "pt/F:eta:phi/F:minDR:minDPt/F:els_match:loss_code/I");
  reduced_tree.Branch("gen_el3", &gen_el3, "pt/F:eta:phi/F:minDR:minDPt/F:els_match:loss_code/I");
  reduced_tree.Branch("gen_el4", &gen_el4, "pt/F:eta:phi/F:minDR:minDPt/F:els_match:loss_code/I");

  reduced_tree.Branch("num_gen_taus", &num_gen_taus);
  reduced_tree.Branch("gen_tau1", &gen_tau1, "pt/F:eta:phi/F:minDR:minDPt/F:taus_match:loss_code/I");
  reduced_tree.Branch("gen_tau2", &gen_tau2, "pt/F:eta:phi/F:minDR:minDPt/F:taus_match:loss_code/I");


  reduced_tree.Branch("num_gen_leptons", &num_gen_leptons);
  reduced_tree.Branch("num_gen_nus", &num_gen_nus);

 
  Timer timer(GetTotalEntries());
  timer.Start();
  for(int i(0); i<GetTotalEntries(); ++i){
    if(i%1000==0 && i!=0){
      timer.PrintRemainingTime();
    }
    timer.Iterate();
    // cout << "JERR" << endl;
    GetEntry(i);
    entry=i;
    //if (i>20) break;
    //cout << "*****Event " << i << "*****" << endl;

    std::pair<std::set<EventNumber>::iterator, bool> returnVal(eventList.insert(EventNumber(run, event, lumiblock)));
    if(!returnVal.second) continue;
    
    passesMETCleaningCut=PassesMETCleaningCut();
    passesPVCut=PassesPVCut();

    pu_true_num_interactions=GetNumInteractions();
    num_primary_vertices=GetNumVertices();

    met_sig=pfmets_fullSignif;
    met=pfTypeImets_et->at(0);

    num_ra2b_veto_electrons=GetNumRA2bElectrons(0);
    num_ra2b_veto_muons=GetNumRA2bMuons(0);
    num_ra2b_veto_taus=GetNumRA2bTaus(0);
    num_ra2b_veto_leptons=num_ra2b_veto_electrons+num_ra2b_veto_muons+num_ra2b_veto_taus;
    num_ra2b_noiso_electrons=GetNumRA2bElectrons(0,false);
    num_ra2b_noiso_muons=GetNumRA2bMuons(0,false);
    num_ra2b_noiso_taus=GetNumRA2bTaus(0,false);
    num_ra2b_noiso_leptons=num_ra2b_noiso_electrons+num_ra2b_noiso_muons+num_ra2b_noiso_taus;
    num_ra2b_loose_electrons=GetNumRA2bElectrons(1);
    num_ra2b_loose_muons=GetNumRA2bMuons(1);
    num_ra2b_loose_taus=GetNumRA2bTaus(1);
    num_ra2b_loose_leptons=num_ra2b_loose_electrons+num_ra2b_loose_muons+num_ra2b_loose_taus;
    num_ra2b_medium_electrons=GetNumRA2bElectrons(2);
    num_ra2b_medium_muons=GetNumRA2bMuons(2);
    num_ra2b_medium_taus=GetNumRA2bTaus(2);
    num_ra2b_medium_leptons=num_ra2b_medium_electrons+num_ra2b_medium_muons+num_ra2b_medium_taus;
    num_ra2b_tight_electrons=GetNumRA2bElectrons(3);
    num_ra2b_tight_muons=GetNumRA2bMuons(3);
    num_ra2b_tight_taus=GetNumRA2bTaus(3);
    num_ra2b_tight_leptons=num_ra2b_tight_electrons+num_ra2b_tight_muons+num_ra2b_tight_taus;
    num_ra2b_iso_tracks=NewGetNumIsoTracks();

    double this_scale_factor(scaleFactor);
    pu_weight=isRealData?1.0:GetPUWeight(lumiWeights);
    lumi_weight=this_scale_factor;
    top_pt_weight=isttbar?GetTopPtWeight():1.0;
    top_pt_weight_official=isttbar?GetTopPtWeightOfficial():1.0;
    full_weight=pu_weight*lumi_weight*top_pt_weight_official;


    //num_gen_electrons=GetNGenParticles(11);
    // cout << "SetupGenMuons" << endl;
    if (!genMuonsUpToDate) SetupGenMuons();
    num_gen_muons=genMuonCache.size();
    for (uint pt_bin(0); pt_bin<nptbins; pt_bin++) {
      num_gen_muons_pt[pt_bin] = GetNGenParticles(13, pt_thresholds[pt_bin]);
    }
    //num_gen_taus=GetNGenParticles(15);
    //num_gen_nus=GetNGenParticles(12)+GetNGenParticles(14)+GetNGenParticles(16);

    num_ignored_gen_muons = GetNumIgnoredGenMuons();

    SetMuonValues(1,gen_mu1);
    SetMuonValues(2,gen_mu2);
    SetMuonValues(3,gen_mu3);
    SetMuonValues(4,gen_mu4);

    if (!genElectronsUpToDate) SetupGenElectrons();
    num_gen_electrons=genElectronCache.size();
    SetElectronValues(1,gen_el1);
    SetElectronValues(2,gen_el2);
    SetElectronValues(3,gen_el3);
    SetElectronValues(4,gen_el4);

    // cout << "JERR" << endl;
    if (!genTausUpToDate) SetupGenTaus();
    num_gen_taus=genTauCache.size();
    SetTauValues(1,gen_tau1);
    SetTauValues(2,gen_tau2);


    num_gen_leptons=num_gen_electrons+num_gen_muons+num_gen_taus;

    num_lost_electrons=num_gen_electrons-num_ra2b_veto_electrons;
    num_lost_muons=num_gen_muons-num_ra2b_veto_muons;
    num_lost_taus=num_gen_taus-num_ra2b_veto_taus;

    vector<int> ra4_muons = GetRecoMuons(false, 20);
    vector<int> ra4_veto_muons = GetRecoMuons(true, 15);
    num_ra4_muons = ra4_muons.size();
    num_ra4_veto_muons = ra4_veto_muons.size();
    // cout << "JERR5" << endl;

    reduced_tree.Fill(); 
  }

  reduced_tree.Write();
  file.Close();
}

void ReducedTreeMaker::SetMuonValues(uint index, genMuInfo& gen_mu) {
  if (index>genMuonCache.size()) {
    gen_mu = {-FLT_MAX, -FLT_MAX, -FLT_MAX, -1., -1., -1, -1};
    return;
  }
  gen_mu.pt=genMuonCache[index-1].GetLorentzVector().Pt();
  gen_mu.eta=genMuonCache[index-1].GetLorentzVector().Eta();
  gen_mu.phi=genMuonCache[index-1].GetLorentzVector().Phi();
  gen_mu.minDR=genMuonCache[index-1].GetMinDR().second;
  gen_mu.minDPt=genMuonCache[index-1].GetMinDPt().second;
  gen_mu.mus_match=genMuonCache[index-1].GetMusMatch();
  gen_mu.loss_code=genMuonCache[index-1].GetLossCode();
}

void ReducedTreeMaker::SetElectronValues(uint index, genElInfo& gen_el) {
  if (index>genElectronCache.size()) {
    gen_el = {-FLT_MAX, -FLT_MAX, -FLT_MAX, -1., -1., -1, -1};
    return;
  }
  gen_el.pt=genElectronCache[index-1].GetLorentzVector().Pt();
  gen_el.eta=genElectronCache[index-1].GetLorentzVector().Eta();
  gen_el.phi=genElectronCache[index-1].GetLorentzVector().Phi();
  gen_el.minDR=genElectronCache[index-1].GetMinDR().second;
  gen_el.minDPt=genElectronCache[index-1].GetMinDPt().second;
  gen_el.els_match=genElectronCache[index-1].GetElsMatch();
  gen_el.loss_code=genElectronCache[index-1].GetLossCode();
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
