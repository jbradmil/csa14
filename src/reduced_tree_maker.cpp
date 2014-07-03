#include "reduced_tree_maker.hpp"
#include <vector>
#include <string>
#include <set>
#include "timer.hpp"
#include "event_handler.hpp"

using namespace std;

typedef struct 
{
  float pt;
  float eta;
  float phi;
  float minDR;
  float minDq;
} hsLepInfo;

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

  unsigned short num_hs_electrons(0), num_hs_muons(0), num_hs_taus(0), num_hs_nus(0), num_hs_leptons(0);
  int num_lost_electrons(0), num_lost_muons(0), num_lost_taus(0);

  //hsLepInfo hs_electron1 = {-FLT_MAX, -FLT_MAX, -FLT_MAX};
  //hsLepInfo hs_electron2 = {-FLT_MAX, -FLT_MAX, -FLT_MAX};

  float gen_lep1_dr(-FLT_MAX), gen_lep2_dr(-FLT_MAX);



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

  reduced_tree.Branch("num_hs_electrons", &num_hs_electrons);
  reduced_tree.Branch("num_hs_muons", &num_hs_muons);
  reduced_tree.Branch("num_hs_taus", &num_hs_taus);
  reduced_tree.Branch("num_hs_leptons", &num_hs_leptons);
  reduced_tree.Branch("num_hs_nus", &num_hs_nus);

  //reduced_tree.Branch("hs_electron1", &hs_electron1, "pt/F:eta:phi/F");
  //reduced_tree.Branch("hs_electron2", &hs_electron2, "pt/F:eta:phi/F");

  reduced_tree.Branch("num_lost_electrons", &num_lost_electrons);
  reduced_tree.Branch("num_lost_muons", &num_lost_muons);
  reduced_tree.Branch("num_lost_taus", &num_lost_taus);

  reduced_tree.Branch("gen_lep1_dr", &gen_lep1_dr);
  reduced_tree.Branch("gen_lep2_dr", &gen_lep2_dr);

  //cout << "JERR" << endl;
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

    //cout << "JERR1" << endl;

     //num_hs_electrons=GetNGenParticles(11);
     //cout << "JERR2" << endl;

    num_hs_muons=GetNGenParticles(13);
    //num_hs_taus=GetNGenParticles(15);
    num_hs_leptons=num_hs_electrons+num_hs_muons+num_hs_taus;
    //num_hs_nus=GetNGenParticles(12)+GetNGenParticles(14)+GetNGenParticles(16);

    /*
      int el1(-1), el2(-1);
      hs_electron1 = {-FLT_MAX, -FLT_MAX, -FLT_MAX};
      hs_electron2 = {-FLT_MAX, -FLT_MAX, -FLT_MAX};
      if (num_hs_electrons>=1) {
      el1 = GetGenParticleIndex(11);
      hs_electron1.pt=mc_doc_pt->at(el1);
      hs_electron1.eta=mc_doc_eta->at(el1);
      hs_electron1.phi=mc_doc_phi->at(el1);
      if (num_hs_electrons>=2) {
      el2 = GetGenParticleIndex(11, 1);
      hs_electron2.pt=mc_doc_pt->at(el2);
      hs_electron2.eta=mc_doc_eta->at(el2);
      hs_electron2.phi=mc_doc_phi->at(el2);
      }
      }
    */

    num_lost_electrons=num_hs_electrons-num_ra2b_veto_electrons;
    num_lost_muons=num_hs_muons-num_ra2b_veto_muons;
    num_lost_taus=num_hs_taus-num_ra2b_veto_taus;
    //cout << "JERR3" << endl;

    gen_lep1_dr=ReadMuonCacheDR(1);
    gen_lep2_dr=ReadMuonCacheDR(2);

    reduced_tree.Fill(); 
  }

  reduced_tree.Write();
  file.Close();
}
