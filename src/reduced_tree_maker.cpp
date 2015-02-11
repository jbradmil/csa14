#include "reduced_tree_maker.hpp"
#include <vector>
#include <string>
#include <set>
#include "timer.hpp"
#include "event_handler.hpp"
#include "event_number.hpp"
#include "math.hpp"
#include "TROOT.h"


using namespace std;

ReducedTreeMaker::ReducedTreeMaker(const std::string& in_file_name,
                                   const bool is_list,
                                   const double weight_in,
				   const int Nentries_in):
  EventHandler(in_file_name, is_list, weight_in, false),
  Nentries(Nentries_in){
  }

void ReducedTreeMaker::MakeReducedTree(const std::string& out_file_name){

  gROOT->ProcessLine("#include <vector>");
  
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

  float weightppb=scaleFactor;
  cout << "Nentries/Weight per pb: " << Nentries << "/" << weightppb << endl;

  unsigned type_code(0x0);

  short lsp_mass(0), gluino_mass(0);

  bool passes2012RA2bTrigger(false);

  bool passesJSONCut(false);

  bool pass_HLT_PFHT350_PFMET100(false), pass_HLT_PFHT650(false), 
    pass_HLT_DiCentralPFJet50_PFMET80(false), pass_HLT_DiCentralPFJet30_PFMET80_BTagCSV07(false);

  bool passesPVCut(false), passes2012METCleaningCut(false), passesCSA14METCleaningCut(false);
  bool passesBadJetFilter(false);
  int PBNRcode(0);


  float highest_csv(0.0), second_highest_csv(0.0),
    third_highest_csv(0.0), fourth_highest_csv(0.0), fifth_highest_csv(0.0), sixth_highest_csv(0.);
  float jet1_pt(0.0), jet2_pt(0.), jet3_pt(0.), jet4_pt(0.), jet5_pt(0.), jet6_pt(0.);
  float jet1_eta(0.0), jet2_eta(0.), jet3_eta(0.), jet4_eta(0.), jet5_eta(0.), jet6_eta(0.);
  float jet1_phi(0.0), jet2_phi(0.), jet3_phi(0.), jet4_phi(0.), jet5_phi(0.), jet6_phi(0.);
  float jet1_csv(0.0), jet2_csv(0.), jet3_csv(0.), jet4_csv(0.), jet5_csv(0.), jet6_csv(0.);
  float jet1_partonId(0.0), jet2_partonId(0.), jet3_partonId(0.), jet4_partonId(0.), jet5_partonId(0.), jet6_partonId(0.);
  float jet1_motherId(0.0), jet2_motherId(0.), jet3_motherId(0.), jet4_motherId(0.), jet5_motherId(0.), jet6_motherId(0.);

  float pu_num_interactions(0.0), pu_true_num_interactions(0.0);
  unsigned short num_primary_vertices(0);

  float eoot_pu, loot_pu, oot_pu;

  float met(0.0), met_phi(0.0);
  float mht30(0.0), mht30_phi(0.0),  mht50(0.0), mht50_phi(0.0);
  float ht20(0.0), ht30(0.0), ht40(0.0), ht50(0.0), ht60(0.0), ht70(0.0), ht80(0.0);

  float mht_over_sqrt_ht30(0.), mht_over_sqrt_ht50(0.);

  float sumP30(0.), sumP50(0.);
  float cent30(0.), cent50(0.);

  float m_eff20(0.0),  m_eff30(0.0), m_eff40(0.0), m_eff50(0.0);
  float met_over_m_eff20(0.0), met_over_m_eff30(0.0), met_over_m_eff40(0.0), met_over_m_eff50(0.0);
  float met_over_sqrt_ht20(0.0), met_over_sqrt_ht30(0.0), met_over_sqrt_ht40(0.0), met_over_sqrt_ht50(0.0), met_over_sqrt_ht80(0.0);
  unsigned short num_jets_pt20(0), num_jets_pt30(0), num_jets_pt40(0), num_jets_pt50(0), num_jets_pt70(0), num_jets_pt100(0), num_jets_pt150(0);
  unsigned short num_csvt_jets(0), num_csvm_jets(0), num_csvl_jets(0);
  unsigned short num_csvt_jets20(0), num_csvm_jets20(0), num_csvl_jets20(0);
  unsigned short num_csvt_jets30(0), num_csvm_jets30(0), num_csvl_jets30(0);
  unsigned short num_csvt_jets40(0), num_csvm_jets40(0), num_csvl_jets40(0);

  unsigned short num_truth_matched_b_jets(0), num_good_truth_matched_b_jets(0);

 
  float top_pt_weight(0.0), top_pt_weight_official(0.0), pu_weight(0.0), trigger_eff_weight, btag_weight, btag_pid_weight;
  double Prob0, ProbGEQ1, Prob1, ProbGEQ2, Prob2, ProbGEQ3, Prob3, ProbGEQ4;
  double Prob0_pt50, ProbGEQ1_pt50, Prob1_pt50, ProbGEQ2_pt50, Prob2_pt50, ProbGEQ3_pt50, Prob3_pt50, ProbGEQ4_pt50;



  int num_reco_muons(0), num_reco_veto_muons(0), num_reco_veto_muons_iso2D(0), num_reco_veto_muons_mT100(0), num_reco_veto_muons_mT100_orth(0);
  int num_reco_electrons(0), num_reco_veto_electrons(0), num_reco_veto_electrons_iso2D(0), num_reco_veto_electrons_mT100(0), num_reco_veto_electrons_mT100_orth(0);
  int num_taus_POG_phys14_ID(0), num_taus_POG_phys14_ID_againstEMu(0), num_taus_POG_phys14_ID_mT100(0), num_taus_POG_phys14_ID_againstEMu_mT100(0);
  float tau_pt, tau_chargedIsoPtSum, tau_mT;

  float iso_track_pt, iso_track_relIso, iso_track_mT;
  float e_track_pt, e_track_relIso, e_track_mT;
  float mu_track_pt, mu_track_relIso, mu_track_mT;
  float had_track_pt, had_track_relIso, had_track_mT;
  

  // int num_lost_electrons(0), num_lost_muons(0), num_lost_leptons(0);



  int muon_true, muon_b;
  int muon_signal, muon_veto;
  float muon_pt, muon_eta, muon_phi;
  float muon_mT, muon_dphi_met;
  float muon_relIso;
  float muon_NH_Iso, muon_CH_Iso, muon_ph_Iso, muon_PU_Iso;
  float muon_ptRel, muon_minDR;



  int electron_true;
  int electron_signal, electron_veto;
  float electron_pt, electron_eta, electron_phi;
  float electron_mT, electron_dphi_met;
  float electron_relIso;
  float electron_NH_Iso, electron_CH_Iso, electron_ph_Iso;
  float electron_ptRel, electron_minDR;

  // int lep1, lep2;

  float min_delta_phi_met, min_delta_phi_met_loose_jets;
  float min_delta_phi_met_N, min_delta_phi_met_N_loose_jets;
  float min_delta_phi_met_N_2012;
  float jet1_DeltaPhiMETN, jet2_DeltaPhiMETN, jet3_DeltaPhiMETN;

  // float deltaPhiN_1, deltaPhiN_2, deltaPhiN_3;
  int num_iso_tracks_pt10, num_iso_tracks_pt10_mT, num_iso_tracks_pt15, num_iso_tracks_pt15_mT;
  int num_e_iso_tracks, num_mu_iso_tracks, num_had_iso_tracks;
  int num_e_iso_tracks_mT, num_mu_iso_tracks_mT, num_had_iso_tracks_mT;

  float mT_mu, mT_el;
  float deltaThetaT, W_pT;
  bool SL_control_sample, QCD_control_sample;
  bool ht_bin1, ht_bin2, ht_bin3, ht_bin4;
  bool met_bin1, met_bin2, met_bin3, met_bin4;
  bool nb_bin1, nb_bin2, nb_bin3;

  // double fatpT10_jet1_pt(-9999.), fatpT10_jet2_pt(-9999.), fatpT10_jet3_pt(-9999.), fatpT10_jet4_pt(-9999.);
  // int fatpT10_jet1_nConst(-1), fatpT10_jet2_nConst(-1), fatpT10_jet3_nConst(-1), fatpT10_jet4_nConst(-1);
  // double fatpT10_jet1_mJ(-9999.), fatpT10_jet2_mJ(-9999.), fatpT10_jet3_mJ(-9999.), fatpT10_jet4_mJ(-9999.);
  // int num_fatpT10_jets(-1), num_fatpT10_jets_pt100(-1), num_fatpT10_jets_pt150(-1), num_fatpT10_jets_pt200(-1),  num_fatpT10_jets_pt300(-1);
  // double fatpT10_MJ(-9999.), fatpT10_MJ_pt100(-9999.), fatpT10_MJ_pt150(-9999.), fatpT10_MJ_pt200(-9999.), fatpT10_MJ_pt300(-9999.);

  // double fatpT15_jet1_pt(-9999.), fatpT15_jet2_pt(-9999.), fatpT15_jet3_pt(-9999.), fatpT15_jet4_pt(-9999.);
  // int fatpT15_jet1_nConst(-1), fatpT15_jet2_nConst(-1), fatpT15_jet3_nConst(-1), fatpT15_jet4_nConst(-1);
  // double fatpT15_jet1_mJ(-9999.), fatpT15_jet2_mJ(-9999.), fatpT15_jet3_mJ(-9999.), fatpT15_jet4_mJ(-9999.);
  // int num_fatpT15_jets(-1), num_fatpT15_jets_pt100(-1), num_fatpT15_jets_pt150(-1), num_fatpT15_jets_pt200(-1),  num_fatpT15_jets_pt300(-1);
  // double fatpT15_MJ(-9999.), fatpT15_MJ_pt100(-9999.), fatpT15_MJ_pt150(-9999.), fatpT15_MJ_pt200(-9999.), fatpT15_MJ_pt300(-9999.);

  // double fatpT20_jet1_pt(-9999.), fatpT20_jet2_pt(-9999.), fatpT20_jet3_pt(-9999.), fatpT20_jet4_pt(-9999.);
  // int fatpT20_jet1_nConst(-1), fatpT20_jet2_nConst(-1), fatpT20_jet3_nConst(-1), fatpT20_jet4_nConst(-1);
  // double fatpT20_jet1_mJ(-9999.), fatpT20_jet2_mJ(-9999.), fatpT20_jet3_mJ(-9999.), fatpT20_jet4_mJ(-9999.);
  // int num_fatpT20_jets(-1), num_fatpT20_jets_pt100(-1), num_fatpT20_jets_pt150(-1), num_fatpT20_jets_pt200(-1),  num_fatpT20_jets_pt300(-1);
  // double fatpT20_MJ(-9999.), fatpT20_MJ_pt100(-9999.), fatpT20_MJ_pt150(-9999.), fatpT20_MJ_pt200(-9999.), fatpT20_MJ_pt300(-9999.);

  // double fatpT25_jet1_pt(-9999.), fatpT25_jet2_pt(-9999.), fatpT25_jet3_pt(-9999.), fatpT25_jet4_pt(-9999.);
  // int fatpT25_jet1_nConst(-1), fatpT25_jet2_nConst(-1), fatpT25_jet3_nConst(-1), fatpT25_jet4_nConst(-1);
  // double fatpT25_jet1_mJ(-9999.), fatpT25_jet2_mJ(-9999.), fatpT25_jet3_mJ(-9999.), fatpT25_jet4_mJ(-9999.);
  // int num_fatpT25_jets(-1), num_fatpT25_jets_pt100(-1), num_fatpT25_jets_pt150(-1), num_fatpT25_jets_pt200(-1),  num_fatpT25_jets_pt300(-1);
  // double fatpT25_MJ(-9999.), fatpT25_MJ_pt100(-9999.), fatpT25_MJ_pt150(-9999.), fatpT25_MJ_pt200(-9999.), fatpT25_MJ_pt300(-9999.);

  double fatpT30_jet1_pt(-9999.), fatpT30_jet2_pt(-9999.), fatpT30_jet3_pt(-9999.), fatpT30_jet4_pt(-9999.);
  double fatpT30_jet1_eta(-9999.), fatpT30_jet2_eta(-9999.), fatpT30_jet3_eta(-9999.), fatpT30_jet4_eta(-9999.);
  double fatpT30_jet1_phi(-9999.), fatpT30_jet2_phi(-9999.), fatpT30_jet3_phi(-9999.), fatpT30_jet4_phi(-9999.);
  double fatpT30_jet1_energy(-9999.), fatpT30_jet2_energy(-9999.), fatpT30_jet3_energy(-9999.), fatpT30_jet4_energy(-9999.);
  int fatpT30_jet1_nConst(-1), fatpT30_jet2_nConst(-1), fatpT30_jet3_nConst(-1), fatpT30_jet4_nConst(-1);
  double fatpT30_jet1_mJ(-9999.), fatpT30_jet2_mJ(-9999.), fatpT30_jet3_mJ(-9999.), fatpT30_jet4_mJ(-9999.);
  int num_fatpT30_jets(-1), num_fatpT30_jets_pt100(-1), num_fatpT30_jets_pt150(-1), num_fatpT30_jets_pt200(-1),  num_fatpT30_jets_pt300(-1);
  double fatpT30_MJ(-9999.), fatpT30_MJ_pt100(-9999.), fatpT30_MJ_pt150(-9999.), fatpT30_MJ_pt200(-9999.), fatpT30_MJ_pt300(-9999.);
  int num_fatpT30central_jets(-1), num_fatpT30central_jets_pt100(-1), num_fatpT30central_jets_pt150(-1), num_fatpT30central_jets_pt200(-1),  num_fatpT30central_jets_pt300(-1);
  double fatpT30central_MJ(-9999.), fatpT30central_MJ_pt100(-9999.), fatpT30central_MJ_pt150(-9999.), fatpT30central_MJ_pt200(-9999.), fatpT30central_MJ_pt300(-9999.);

  double highest_mJ(-1.), scnd_highest_mJ(-1.), thrd_highest_mJ(-1.), frth_highest_mJ(-1.);

  double min_mTWB(0.), min_mTWB_Wmass(0.);
  double mTWB_2nd(0.);
  double min_delta_phi_b_met(0.);

  double doc_met(0.);
  double gluino1_pt(0.), gluino2_pt(0.);

  unsigned short num_true_electrons(0), num_true_muons(0), num_true_had_taus(0), num_true_lep_taus(0);
  vector<float> true_electron_pt, true_electron_eta, true_electron_min_parton_dR, true_electron_rel_iso, true_electron_d0;
  vector<bool> true_electron_passID;
  vector<float> true_muon_pt, true_muon_eta, true_muon_min_parton_dR, true_muon_rel_iso, true_muon_d0;
  vector<bool> true_muon_passID;
  vector<float> true_had_tau_pt, true_had_tau_eta, true_had_tau_min_parton_dR;


  reduced_tree.Branch("weightppb", &weightppb);

  reduced_tree.Branch("type_code", &type_code);
  reduced_tree.Branch("gluino_mass", &gluino_mass);
  reduced_tree.Branch("lsp_mass", &lsp_mass);

  reduced_tree.Branch("passes2012RA2bTrigger", &passes2012RA2bTrigger);

  reduced_tree.Branch("passesJSONCut",&passesJSONCut);
  reduced_tree.Branch("passesPVCut",&passesPVCut);
  reduced_tree.Branch("passes2012METCleaningCut",&passes2012METCleaningCut);
  reduced_tree.Branch("passesCSA14METCleaningCut",&passesCSA14METCleaningCut);
  reduced_tree.Branch("trackingfailurefilter_decision",&trackingfailurefilter_decision);
  reduced_tree.Branch("cschalofilter_decision",&cschalofilter_decision);
  reduced_tree.Branch("ecallaserfilter_decision",&ecallaserfilter_decision);
  reduced_tree.Branch("HBHENoisefilter_decision",&HBHENoisefilter_decision);
  reduced_tree.Branch("hcallaserfilter_decision",&hcallaserfilter_decision);

  // if (cfAVersion>=73&&cfAVersion<=75) {
  //   reduced_tree.Branch("goodVerticesfilter_decision",&goodVerticesfilter_decision);
  //   reduced_tree.Branch("trkPOGfilter_decision",&trkPOGfilter_decision);
  //   reduced_tree.Branch("trkPOG_logErrorTooManyClustersfilter_decision",&trkPOG_logErrorTooManyClustersfilter_decision);
  //   reduced_tree.Branch("EcalDeadCellTriggerPrimitivefilter_decision",&EcalDeadCellTriggerPrimitivefilter_decision);
  //   reduced_tree.Branch("trkPOG_manystripclus53Xfilter_decision",&trkPOG_manystripclus53Xfilter_decision);
  //   reduced_tree.Branch("eebadscfilter_decision",&eebadscfilter_decision);
  //   reduced_tree.Branch("trkPOG_toomanystripclus53Xfilter_decision",&trkPOG_toomanystripclus53Xfilter_decision);
  //   // This is probably the most important one, and it contains a sumamry of the others
  //   // https://cmssdt.cern.ch/SDT/lxr/source/RecoMET/METFilters/python/metFilters_cff.py#0045
  //   reduced_tree.Branch("METFiltersfilter_decision",&METFiltersfilter_decision);
  // }

  reduced_tree.Branch("passesBadJetFilter",&passesBadJetFilter);
  reduced_tree.Branch("PBNRcode",&PBNRcode);

  //  reduced_tree.Branch("full_weight", &full_weight);
  // reduced_tree.Branch("lumi_weight", &lumi_weight);
  reduced_tree.Branch("pu_weight", &pu_weight);
  reduced_tree.Branch("top_pt_weight", &top_pt_weight);
  reduced_tree.Branch("top_pt_weight_official", &top_pt_weight_official);
  reduced_tree.Branch("trigger_eff_weight", &trigger_eff_weight);
  reduced_tree.Branch("btag_weight", &btag_weight);
  reduced_tree.Branch("btag_pid_weight", &btag_pid_weight);
  reduced_tree.Branch("Prob0", &Prob0);
  reduced_tree.Branch("ProbGEQ1", &ProbGEQ1);
  reduced_tree.Branch("Prob1", &Prob1);
  reduced_tree.Branch("ProbGEQ2", &ProbGEQ2);
  reduced_tree.Branch("Prob2", &Prob2);
  reduced_tree.Branch("ProbGEQ3", &ProbGEQ3);
  reduced_tree.Branch("Prob3", &Prob3);
  reduced_tree.Branch("ProbGEQ4", &ProbGEQ4);
  reduced_tree.Branch("Prob0_pt50", &Prob0_pt50);
  reduced_tree.Branch("ProbGEQ1_pt50", &ProbGEQ1_pt50);
  reduced_tree.Branch("Prob1_pt50", &Prob1_pt50);
  reduced_tree.Branch("ProbGEQ2_pt50", &ProbGEQ2_pt50);
  reduced_tree.Branch("Prob2_pt50", &Prob2_pt50);
  reduced_tree.Branch("ProbGEQ3_pt50", &ProbGEQ3_pt50);
  reduced_tree.Branch("Prob3_pt50", &Prob3_pt50);
  reduced_tree.Branch("ProbGEQ4_pt50", &ProbGEQ4_pt50);
 
  reduced_tree.Branch("run", &run);
  reduced_tree.Branch("event", &event);
  reduced_tree.Branch("lumiblock", &lumiblock);
  reduced_tree.Branch("entry", &entry);

  reduced_tree.Branch("pass_HLT_PFHT350_PFMET100", &pass_HLT_PFHT350_PFMET100);
  reduced_tree.Branch("pass_HLT_PFHT650", &pass_HLT_PFHT650);
  reduced_tree.Branch("pass_HLT_DiCentralPFJet50_PFMET80", &pass_HLT_DiCentralPFJet50_PFMET80);
  reduced_tree.Branch("pass_HLT_DiCentralPFJet30_PFMET80_BTagCSV07", &pass_HLT_DiCentralPFJet30_PFMET80_BTagCSV07);

  reduced_tree.Branch("pu_true_num_interactions", &pu_true_num_interactions);
  reduced_tree.Branch("pu_num_interactions", &pu_num_interactions);
  reduced_tree.Branch("num_primary_vertices", &num_primary_vertices);
  reduced_tree.Branch("eoot_pu", &eoot_pu);
  reduced_tree.Branch("loot_pu", &loot_pu);
  reduced_tree.Branch("oot_pu", &oot_pu);

  reduced_tree.Branch("highest_csv", &highest_csv);
  reduced_tree.Branch("second_highest_csv", &second_highest_csv);
  reduced_tree.Branch("third_highest_csv", &third_highest_csv);
  reduced_tree.Branch("fourth_highest_csv", &fourth_highest_csv);
  reduced_tree.Branch("fifth_highest_csv", &fifth_highest_csv);
  reduced_tree.Branch("sixth_highest_csv", &sixth_highest_csv);

  reduced_tree.Branch("jet1_csv", &jet1_csv);
  reduced_tree.Branch("jet2_csv", &jet2_csv);
  reduced_tree.Branch("jet3_csv", &jet3_csv);
  reduced_tree.Branch("jet4_csv", &jet4_csv);
  reduced_tree.Branch("jet5_csv", &jet5_csv);
  reduced_tree.Branch("jet6_csv", &jet6_csv);
  reduced_tree.Branch("jet1_pt", &jet1_pt);
  reduced_tree.Branch("jet2_pt", &jet2_pt);
  reduced_tree.Branch("jet3_pt", &jet3_pt);
  reduced_tree.Branch("jet4_pt", &jet4_pt);
  reduced_tree.Branch("jet5_pt", &jet5_pt);
  reduced_tree.Branch("jet6_pt", &jet6_pt);
  reduced_tree.Branch("jet1_eta", &jet1_eta);
  reduced_tree.Branch("jet2_eta", &jet2_eta);
  reduced_tree.Branch("jet3_eta", &jet3_eta);
  reduced_tree.Branch("jet4_eta", &jet4_eta);
  reduced_tree.Branch("jet5_eta", &jet5_eta);
  reduced_tree.Branch("jet6_eta", &jet6_eta);
  reduced_tree.Branch("jet1_phi", &jet1_phi);
  reduced_tree.Branch("jet2_phi", &jet2_phi);
  reduced_tree.Branch("jet3_phi", &jet3_phi);
  reduced_tree.Branch("jet4_phi", &jet4_phi);
  reduced_tree.Branch("jet5_phi", &jet5_phi);
  reduced_tree.Branch("jet6_phi", &jet6_phi);
  reduced_tree.Branch("jet1_partonId", &jet1_partonId);
  reduced_tree.Branch("jet2_partonId", &jet2_partonId);
  reduced_tree.Branch("jet3_partonId", &jet3_partonId);
  reduced_tree.Branch("jet4_partonId", &jet4_partonId);
  reduced_tree.Branch("jet5_partonId", &jet5_partonId);
  reduced_tree.Branch("jet6_partonId", &jet6_partonId);
  reduced_tree.Branch("jet1_motherId", &jet1_motherId);
  reduced_tree.Branch("jet2_motherId", &jet2_motherId);
  reduced_tree.Branch("jet3_motherId", &jet3_motherId);
  reduced_tree.Branch("jet4_motherId", &jet4_motherId);
  reduced_tree.Branch("jet5_motherId", &jet5_motherId);
  reduced_tree.Branch("jet6_motherId", &jet6_motherId);

  reduced_tree.Branch("met", &met);
  reduced_tree.Branch("met_phi", &met_phi);
  reduced_tree.Branch("mht30", &mht30);
  reduced_tree.Branch("mht30_phi", &mht30_phi);
  reduced_tree.Branch("mht50", &mht50);
  reduced_tree.Branch("mht50_phi", &mht50_phi);
  reduced_tree.Branch("mht_over_sqrt_ht30", &mht_over_sqrt_ht30);
  reduced_tree.Branch("mht_over_sqrt_ht50", &mht_over_sqrt_ht50);


  reduced_tree.Branch("ht20", &ht20);
  reduced_tree.Branch("ht30", &ht30);
  reduced_tree.Branch("ht40", &ht40);
  reduced_tree.Branch("ht50", &ht50);
  reduced_tree.Branch("ht60", &ht60);
  reduced_tree.Branch("ht70", &ht70);
  reduced_tree.Branch("ht80", &ht80);
 
  reduced_tree.Branch("sumP30", &sumP30);
  reduced_tree.Branch("sumP50", &sumP50);
  reduced_tree.Branch("cent30", &cent30);
  reduced_tree.Branch("cent50", &cent50);

  reduced_tree.Branch("m_eff20", &m_eff20);
  reduced_tree.Branch("m_eff30", &m_eff30);
  reduced_tree.Branch("m_eff40", &m_eff40);
  reduced_tree.Branch("m_eff50", &m_eff50);
  reduced_tree.Branch("met_over_m_eff20", &met_over_m_eff20);
  reduced_tree.Branch("met_over_m_eff30", &met_over_m_eff30);
  reduced_tree.Branch("met_over_m_eff40", &met_over_m_eff40);
  reduced_tree.Branch("met_over_m_eff50", &met_over_m_eff50);
  reduced_tree.Branch("met_over_sqrt_ht20", &met_over_sqrt_ht20);
  reduced_tree.Branch("met_over_sqrt_ht30", &met_over_sqrt_ht30);
  reduced_tree.Branch("met_over_sqrt_ht40", &met_over_sqrt_ht40);
  reduced_tree.Branch("met_over_sqrt_ht50", &met_over_sqrt_ht50);
  reduced_tree.Branch("met_over_sqrt_ht80", &met_over_sqrt_ht80);

  reduced_tree.Branch("num_jets_pt20", &num_jets_pt20); 
  reduced_tree.Branch("num_jets_pt30", &num_jets_pt30); 
  reduced_tree.Branch("num_jets_pt40", &num_jets_pt40); 
  reduced_tree.Branch("num_jets_pt50", &num_jets_pt50); 
  reduced_tree.Branch("num_jets_pt70", &num_jets_pt70); 
  reduced_tree.Branch("num_jets_pt100", &num_jets_pt100); 
  reduced_tree.Branch("num_jets_pt150", &num_jets_pt150); 

  reduced_tree.Branch("num_csvt_jets", &num_csvt_jets); 
  reduced_tree.Branch("num_csvm_jets", &num_csvm_jets); 
  reduced_tree.Branch("num_csvl_jets", &num_csvl_jets); 
  reduced_tree.Branch("num_csvt_jets20", &num_csvt_jets20); 
  reduced_tree.Branch("num_csvm_jets20", &num_csvm_jets20); 
  reduced_tree.Branch("num_csvl_jets20", &num_csvl_jets20); 
  reduced_tree.Branch("num_csvt_jets30", &num_csvt_jets30); 
  reduced_tree.Branch("num_csvm_jets30", &num_csvm_jets30); 
  reduced_tree.Branch("num_csvl_jets30", &num_csvl_jets30); 
  reduced_tree.Branch("num_csvt_jets40", &num_csvt_jets40); 
  reduced_tree.Branch("num_csvm_jets40", &num_csvm_jets40); 
  reduced_tree.Branch("num_csvl_jets40", &num_csvl_jets40); 

  reduced_tree.Branch("num_truth_matched_b_jets", &num_truth_matched_b_jets);
  reduced_tree.Branch("num_good_truth_matched_b_jets", &num_good_truth_matched_b_jets);

  // reduced_tree.Branch("muon_gen_mother_id", &muon_gen_mother_id);
  // reduced_tree.Branch("muon_gen_pt", &muon_gen_pt);
  // reduced_tree.Branch("muon_gen_eta", &muon_gen_eta);
  // reduced_tree.Branch("muon_gen_phi", &muon_gen_phi);
  // reduced_tree.Branch("muon_reco_match", &muon_reco_match);

  reduced_tree.Branch("muon_true", &muon_true);
  reduced_tree.Branch("muon_b", &muon_b);
  reduced_tree.Branch("muon_signal", &muon_signal);
  reduced_tree.Branch("muon_veto", &muon_veto);

  reduced_tree.Branch("muon_pt", &muon_pt);
  reduced_tree.Branch("muon_eta", &muon_eta);
  reduced_tree.Branch("muon_phi", &muon_phi);

  reduced_tree.Branch("muon_dphi_met", &muon_dphi_met);
  reduced_tree.Branch("muon_mT", &muon_mT);

  reduced_tree.Branch("muon_relIso", &muon_relIso);
  reduced_tree.Branch("muon_NH_Iso", &muon_NH_Iso);
  reduced_tree.Branch("muon_CH_Iso", &muon_CH_Iso);
  reduced_tree.Branch("muon_ph_Iso", &muon_ph_Iso);
  reduced_tree.Branch("muon_PU_Iso", &muon_PU_Iso);

  reduced_tree.Branch("muon_ptRel", &muon_ptRel);
  reduced_tree.Branch("muon_minDR",&muon_minDR);



  // reduced_tree.Branch("muon_NH_Et", &muon_NH_Et);
  // reduced_tree.Branch("muon_CH_pt", &muon_CH_pt);
  // reduced_tree.Branch("muon_ph_Et", &muon_ph_Et);
  // reduced_tree.Branch("muon_PU_pt", &muon_PU_pt);

  // reduced_tree.Branch("muon_minDR_jet", &muon_minDR_jet);
  // 


  reduced_tree.Branch("num_reco_veto_muons", &num_reco_veto_muons);
  reduced_tree.Branch("num_reco_veto_muons_iso2D", &num_reco_veto_muons_iso2D);
  reduced_tree.Branch("num_reco_veto_muons_mT100", &num_reco_veto_muons_mT100);
  reduced_tree.Branch("num_reco_veto_muons_mT100_orth", &num_reco_veto_muons_mT100_orth);
  reduced_tree.Branch("num_reco_muons", &num_reco_muons);

  reduced_tree.Branch("num_reco_veto_electrons", &num_reco_veto_electrons);
  reduced_tree.Branch("num_reco_veto_electrons_iso2D", &num_reco_veto_electrons_iso2D);
  reduced_tree.Branch("num_reco_veto_electrons_mT100", &num_reco_veto_electrons_mT100);
  reduced_tree.Branch("num_reco_veto_electrons_mT100_orth", &num_reco_veto_electrons_mT100_orth);
  reduced_tree.Branch("num_reco_electrons", &num_reco_electrons);

  // reduced_tree.Branch("electron_gen_mother_id", &electron_gen_mother_id);
  // reduced_tree.Branch("electron_gen_pt", &electron_gen_pt);
  // reduced_tree.Branch("electron_gen_eta", &electron_gen_eta);
  // reduced_tree.Branch("electron_gen_phi", &electron_gen_phi);
  // reduced_tree.Branch("electron_reco_match", &electron_reco_match);

  // reduced_tree.Branch("electron_PFmatched", &electron_PFmatched);

  reduced_tree.Branch("electron_true", &electron_true);
  reduced_tree.Branch("electron_signal", &electron_signal);
  reduced_tree.Branch("electron_veto", &electron_veto);

  reduced_tree.Branch("electron_pt", &electron_pt);
  reduced_tree.Branch("electron_eta", &electron_eta);
  reduced_tree.Branch("electron_phi", &electron_phi);

  reduced_tree.Branch("electron_dphi_met", &electron_dphi_met);
  reduced_tree.Branch("electron_mT", &electron_mT);

  reduced_tree.Branch("electron_relIso", &electron_relIso);
  reduced_tree.Branch("electron_NH_Iso", &electron_NH_Iso);
  reduced_tree.Branch("electron_CH_Iso", &electron_CH_Iso);
  reduced_tree.Branch("electron_ph_Iso", &electron_ph_Iso);

  reduced_tree.Branch("electron_ptRel", &electron_ptRel);
  reduced_tree.Branch("electron_minDR",&electron_minDR);

  // reduced_tree.Branch("electron_NH_Et", &electron_NH_Et);
  // reduced_tree.Branch("electron_CH_pt", &electron_CH_pt);
  // reduced_tree.Branch("electron_ph_Et", &electron_ph_Et);

  // reduced_tree.Branch("electron_minDR_jet", &electron_minDR_jet);

  // reduced_tree.Branch("num_lost_electrons", &num_lost_electrons);
  // reduced_tree.Branch("num_lost_muons", &num_lost_muons);
  // reduced_tree.Branch("num_lost_leptons", &num_lost_leptons);

  // reduced_tree.Branch("lep1", &lep1);
  // reduced_tree.Branch("lep2", &lep2);

  reduced_tree.Branch("min_delta_phi_met_N", &min_delta_phi_met_N);
  reduced_tree.Branch("min_delta_phi_met_N_2012", &min_delta_phi_met_N_2012);
  reduced_tree.Branch("min_delta_phi_met_N_loose_jets", &min_delta_phi_met_N_loose_jets);
  reduced_tree.Branch("min_delta_phi_met", &min_delta_phi_met);
  reduced_tree.Branch("min_delta_phi_met_loose_jets", &min_delta_phi_met_loose_jets);
  reduced_tree.Branch("jet1_DeltaPhiMETN", &jet1_DeltaPhiMETN);
  reduced_tree.Branch("jet2_DeltaPhiMETN", &jet2_DeltaPhiMETN);
  reduced_tree.Branch("jet3_DeltaPhiMETN", &jet3_DeltaPhiMETN);
  // reduced_tree.Branch("deltaPhiN_1", &deltaPhiN_1);
  // reduced_tree.Branch("deltaPhiN_2", &deltaPhiN_2);
  // reduced_tree.Branch("deltaPhiN_3", &deltaPhiN_3);


  reduced_tree.Branch("mT_mu", &mT_mu);
  reduced_tree.Branch("mT_el", &mT_el);
  reduced_tree.Branch("deltaThetaT", &deltaThetaT);
  reduced_tree.Branch("W_pT", &W_pT);

  reduced_tree.Branch("SL_control_sample", &SL_control_sample);
  reduced_tree.Branch("QCD_control_sample", &QCD_control_sample);

  reduced_tree.Branch("ht_bin1", &ht_bin1);
  reduced_tree.Branch("ht_bin2", &ht_bin2);
  reduced_tree.Branch("ht_bin3", &ht_bin3);
  reduced_tree.Branch("ht_bin4", &ht_bin4);
  reduced_tree.Branch("met_bin1", &met_bin1);
  reduced_tree.Branch("met_bin2", &met_bin2);
  reduced_tree.Branch("met_bin3", &met_bin3);
  reduced_tree.Branch("met_bin4", &met_bin4);
  reduced_tree.Branch("nb_bin1", &nb_bin1);
  reduced_tree.Branch("nb_bin2", &nb_bin2);
  reduced_tree.Branch("nb_bin3", &nb_bin3);

  // if (cfAVersion>=74) {
  //   reduced_tree.Branch("fatpT10_jet1_pt", &fatpT10_jet1_pt);
  //   reduced_tree.Branch("fatpT10_jet2_pt", &fatpT10_jet2_pt);
  //   reduced_tree.Branch("fatpT10_jet3_pt", &fatpT10_jet3_pt);
  //   reduced_tree.Branch("fatpT10_jet4_pt", &fatpT10_jet4_pt);
  //   reduced_tree.Branch("fatpT10_jet1_mJ", &fatpT10_jet1_mJ);
  //   reduced_tree.Branch("fatpT10_jet2_mJ", &fatpT10_jet2_mJ);
  //   reduced_tree.Branch("fatpT10_jet3_mJ", &fatpT10_jet3_mJ);
  //   reduced_tree.Branch("fatpT10_jet4_mJ", &fatpT10_jet4_mJ);
  //   reduced_tree.Branch("fatpT10_jet1_nConst", &fatpT10_jet1_nConst);
  //   reduced_tree.Branch("fatpT10_jet2_nConst", &fatpT10_jet2_nConst);
  //   reduced_tree.Branch("fatpT10_jet3_nConst", &fatpT10_jet3_nConst);
  //   reduced_tree.Branch("fatpT10_jet4_nConst", &fatpT10_jet4_nConst);
  //   reduced_tree.Branch("num_fatpT10_jets", &num_fatpT10_jets);
  //   reduced_tree.Branch("num_fatpT10_jets_pt100", &num_fatpT10_jets_pt100);
  //   reduced_tree.Branch("num_fatpT10_jets_pt150", &num_fatpT10_jets_pt150);
  //   reduced_tree.Branch("num_fatpT10_jets_pt200", &num_fatpT10_jets_pt200);
  //   reduced_tree.Branch("num_fatpT10_jets_pt300", &num_fatpT10_jets_pt300);
  //   reduced_tree.Branch("fatpT10_MJ", &fatpT10_MJ);
  //   reduced_tree.Branch("fatpT10_MJ_pt100", &fatpT10_MJ_pt100);
  //   reduced_tree.Branch("fatpT10_MJ_pt150", &fatpT10_MJ_pt150);
  //   reduced_tree.Branch("fatpT10_MJ_pt200", &fatpT10_MJ_pt200);
  //   reduced_tree.Branch("fatpT10_MJ_pt300", &fatpT10_MJ_pt300);

  //   reduced_tree.Branch("fatpT15_jet1_pt", &fatpT15_jet1_pt);
  //   reduced_tree.Branch("fatpT15_jet2_pt", &fatpT15_jet2_pt);
  //   reduced_tree.Branch("fatpT15_jet3_pt", &fatpT15_jet3_pt);
  //   reduced_tree.Branch("fatpT15_jet4_pt", &fatpT15_jet4_pt);
  //   reduced_tree.Branch("fatpT15_jet1_mJ", &fatpT15_jet1_mJ);
  //   reduced_tree.Branch("fatpT15_jet2_mJ", &fatpT15_jet2_mJ);
  //   reduced_tree.Branch("fatpT15_jet3_mJ", &fatpT15_jet3_mJ);
  //   reduced_tree.Branch("fatpT15_jet4_mJ", &fatpT15_jet4_mJ);
  //   reduced_tree.Branch("fatpT15_jet1_nConst", &fatpT15_jet1_nConst);
  //   reduced_tree.Branch("fatpT15_jet2_nConst", &fatpT15_jet2_nConst);
  //   reduced_tree.Branch("fatpT15_jet3_nConst", &fatpT15_jet3_nConst);
  //   reduced_tree.Branch("fatpT15_jet4_nConst", &fatpT15_jet4_nConst);
  //   reduced_tree.Branch("num_fatpT15_jets", &num_fatpT15_jets);
  //   reduced_tree.Branch("num_fatpT15_jets_pt100", &num_fatpT15_jets_pt100);
  //   reduced_tree.Branch("num_fatpT15_jets_pt150", &num_fatpT15_jets_pt150);
  //   reduced_tree.Branch("num_fatpT15_jets_pt200", &num_fatpT15_jets_pt200);
  //   reduced_tree.Branch("num_fatpT15_jets_pt300", &num_fatpT15_jets_pt300);
  //   reduced_tree.Branch("fatpT15_MJ", &fatpT15_MJ);
  //   reduced_tree.Branch("fatpT15_MJ_pt100", &fatpT15_MJ_pt100);
  //   reduced_tree.Branch("fatpT15_MJ_pt150", &fatpT15_MJ_pt150);
  //   reduced_tree.Branch("fatpT15_MJ_pt200", &fatpT15_MJ_pt200);
  //   reduced_tree.Branch("fatpT15_MJ_pt300", &fatpT15_MJ_pt300);

  //   reduced_tree.Branch("fatpT20_jet1_pt", &fatpT20_jet1_pt);
  //   reduced_tree.Branch("fatpT20_jet2_pt", &fatpT20_jet2_pt);
  //   reduced_tree.Branch("fatpT20_jet3_pt", &fatpT20_jet3_pt);
  //   reduced_tree.Branch("fatpT20_jet4_pt", &fatpT20_jet4_pt);
  //   reduced_tree.Branch("fatpT20_jet1_mJ", &fatpT20_jet1_mJ);
  //   reduced_tree.Branch("fatpT20_jet2_mJ", &fatpT20_jet2_mJ);
  //   reduced_tree.Branch("fatpT20_jet3_mJ", &fatpT20_jet3_mJ);
  //   reduced_tree.Branch("fatpT20_jet4_mJ", &fatpT20_jet4_mJ);
  //   reduced_tree.Branch("fatpT20_jet1_nConst", &fatpT20_jet1_nConst);
  //   reduced_tree.Branch("fatpT20_jet2_nConst", &fatpT20_jet2_nConst);
  //   reduced_tree.Branch("fatpT20_jet3_nConst", &fatpT20_jet3_nConst);
  //   reduced_tree.Branch("fatpT20_jet4_nConst", &fatpT20_jet4_nConst);
  //   reduced_tree.Branch("num_fatpT20_jets", &num_fatpT20_jets);
  //   reduced_tree.Branch("num_fatpT20_jets_pt100", &num_fatpT20_jets_pt100);
  //   reduced_tree.Branch("num_fatpT20_jets_pt150", &num_fatpT20_jets_pt150);
  //   reduced_tree.Branch("num_fatpT20_jets_pt200", &num_fatpT20_jets_pt200);
  //   reduced_tree.Branch("num_fatpT20_jets_pt300", &num_fatpT20_jets_pt300);
  //   reduced_tree.Branch("fatpT20_MJ", &fatpT20_MJ);
  //   reduced_tree.Branch("fatpT20_MJ_pt100", &fatpT20_MJ_pt100);
  //   reduced_tree.Branch("fatpT20_MJ_pt150", &fatpT20_MJ_pt150);
  //   reduced_tree.Branch("fatpT20_MJ_pt200", &fatpT20_MJ_pt200);
  //   reduced_tree.Branch("fatpT20_MJ_pt300", &fatpT20_MJ_pt300);

  //   reduced_tree.Branch("fatpT25_jet1_pt", &fatpT25_jet1_pt);
  //   reduced_tree.Branch("fatpT25_jet2_pt", &fatpT25_jet2_pt);
  //   reduced_tree.Branch("fatpT25_jet3_pt", &fatpT25_jet3_pt);
  //   reduced_tree.Branch("fatpT25_jet4_pt", &fatpT25_jet4_pt);
  //   reduced_tree.Branch("fatpT25_jet1_mJ", &fatpT25_jet1_mJ);
  //   reduced_tree.Branch("fatpT25_jet2_mJ", &fatpT25_jet2_mJ);
  //   reduced_tree.Branch("fatpT25_jet3_mJ", &fatpT25_jet3_mJ);
  //   reduced_tree.Branch("fatpT25_jet4_mJ", &fatpT25_jet4_mJ);
  //   reduced_tree.Branch("fatpT25_jet1_nConst", &fatpT25_jet1_nConst);
  //   reduced_tree.Branch("fatpT25_jet2_nConst", &fatpT25_jet2_nConst);
  //   reduced_tree.Branch("fatpT25_jet3_nConst", &fatpT25_jet3_nConst);
  //   reduced_tree.Branch("fatpT25_jet4_nConst", &fatpT25_jet4_nConst);
  //   reduced_tree.Branch("num_fatpT25_jets", &num_fatpT25_jets);
  //   reduced_tree.Branch("num_fatpT25_jets_pt100", &num_fatpT25_jets_pt100);
  //   reduced_tree.Branch("num_fatpT25_jets_pt150", &num_fatpT25_jets_pt150);
  //   reduced_tree.Branch("num_fatpT25_jets_pt200", &num_fatpT25_jets_pt200);
  //   reduced_tree.Branch("num_fatpT25_jets_pt300", &num_fatpT25_jets_pt300);
  //   reduced_tree.Branch("fatpT25_MJ", &fatpT25_MJ);
  //   reduced_tree.Branch("fatpT25_MJ_pt100", &fatpT25_MJ_pt100);
  //   reduced_tree.Branch("fatpT25_MJ_pt150", &fatpT25_MJ_pt150);
  //   reduced_tree.Branch("fatpT25_MJ_pt200", &fatpT25_MJ_pt200);
  //   reduced_tree.Branch("fatpT25_MJ_pt300", &fatpT25_MJ_pt300);
  // }

  reduced_tree.Branch("fatpT30_jet1_pt", &fatpT30_jet1_pt);
  reduced_tree.Branch("fatpT30_jet2_pt", &fatpT30_jet2_pt);
  reduced_tree.Branch("fatpT30_jet3_pt", &fatpT30_jet3_pt);
  reduced_tree.Branch("fatpT30_jet4_pt", &fatpT30_jet4_pt);
  reduced_tree.Branch("fatpT30_jet1_eta", &fatpT30_jet1_eta);
  reduced_tree.Branch("fatpT30_jet2_eta", &fatpT30_jet2_eta);
  reduced_tree.Branch("fatpT30_jet3_eta", &fatpT30_jet3_eta);
  reduced_tree.Branch("fatpT30_jet4_eta", &fatpT30_jet4_eta);
  reduced_tree.Branch("fatpT30_jet1_phi", &fatpT30_jet1_phi);
  reduced_tree.Branch("fatpT30_jet2_phi", &fatpT30_jet2_phi);
  reduced_tree.Branch("fatpT30_jet3_phi", &fatpT30_jet3_phi);
  reduced_tree.Branch("fatpT30_jet4_phi", &fatpT30_jet4_phi);
  reduced_tree.Branch("fatpT30_jet1_energy", &fatpT30_jet1_energy);
  reduced_tree.Branch("fatpT30_jet2_energy", &fatpT30_jet2_energy);
  reduced_tree.Branch("fatpT30_jet3_energy", &fatpT30_jet3_energy);
  reduced_tree.Branch("fatpT30_jet4_energy", &fatpT30_jet4_energy);
  reduced_tree.Branch("fatpT30_jet1_mJ", &fatpT30_jet1_mJ);
  reduced_tree.Branch("fatpT30_jet2_mJ", &fatpT30_jet2_mJ);
  reduced_tree.Branch("fatpT30_jet3_mJ", &fatpT30_jet3_mJ);
  reduced_tree.Branch("fatpT30_jet4_mJ", &fatpT30_jet4_mJ);
  reduced_tree.Branch("fatpT30_jet1_nConst", &fatpT30_jet1_nConst);
  reduced_tree.Branch("fatpT30_jet2_nConst", &fatpT30_jet2_nConst);
  reduced_tree.Branch("fatpT30_jet3_nConst", &fatpT30_jet3_nConst);
  reduced_tree.Branch("fatpT30_jet4_nConst", &fatpT30_jet4_nConst);
  reduced_tree.Branch("num_fatpT30_jets", &num_fatpT30_jets);
  reduced_tree.Branch("num_fatpT30_jets_pt100", &num_fatpT30_jets_pt100);
  reduced_tree.Branch("num_fatpT30_jets_pt150", &num_fatpT30_jets_pt150);
  reduced_tree.Branch("num_fatpT30_jets_pt200", &num_fatpT30_jets_pt200);
  reduced_tree.Branch("num_fatpT30_jets_pt300", &num_fatpT30_jets_pt300);
  reduced_tree.Branch("fatpT30_MJ", &fatpT30_MJ);
  reduced_tree.Branch("fatpT30_MJ_pt100", &fatpT30_MJ_pt100);
  reduced_tree.Branch("fatpT30_MJ_pt150", &fatpT30_MJ_pt150);
  reduced_tree.Branch("fatpT30_MJ_pt200", &fatpT30_MJ_pt200);
  reduced_tree.Branch("fatpT30_MJ_pt300", &fatpT30_MJ_pt300);

  reduced_tree.Branch("num_fatpT30central_jets", &num_fatpT30central_jets);
  reduced_tree.Branch("num_fatpT30central_jets_pt100", &num_fatpT30central_jets_pt100);
  reduced_tree.Branch("num_fatpT30central_jets_pt150", &num_fatpT30central_jets_pt150);
  reduced_tree.Branch("num_fatpT30central_jets_pt200", &num_fatpT30central_jets_pt200);
  reduced_tree.Branch("num_fatpT30central_jets_pt300", &num_fatpT30central_jets_pt300);
  reduced_tree.Branch("fatpT30central_MJ", &fatpT30central_MJ);
  reduced_tree.Branch("fatpT30central_MJ_pt100", &fatpT30central_MJ_pt100);
  reduced_tree.Branch("fatpT30central_MJ_pt150", &fatpT30central_MJ_pt150);
  reduced_tree.Branch("fatpT30central_MJ_pt200", &fatpT30central_MJ_pt200);
  reduced_tree.Branch("fatpT30central_MJ_pt300", &fatpT30central_MJ_pt300);
 
  reduced_tree.Branch("highest_mJ", &highest_mJ);
  reduced_tree.Branch("scnd_highest_mJ", &scnd_highest_mJ);
  reduced_tree.Branch("thrd_highest_mJ", &thrd_highest_mJ);
  reduced_tree.Branch("frth_highest_mJ", &frth_highest_mJ);

  reduced_tree.Branch("min_mTWB", &min_mTWB);
  reduced_tree.Branch("min_mTWB_Wmass", &min_mTWB_Wmass);
  reduced_tree.Branch("mTWB_2nd", &mTWB_2nd);
  reduced_tree.Branch("min_delta_phi_b_met", &min_delta_phi_b_met);

  reduced_tree.Branch("doc_met", &doc_met);
  reduced_tree.Branch("gluino1_pt", &gluino1_pt);
  reduced_tree.Branch("gluino2_pt", &gluino2_pt);

  reduced_tree.Branch("num_taus_POG_phys14_ID", &num_taus_POG_phys14_ID);
  reduced_tree.Branch("num_taus_POG_phys14_ID_againstEMu", &num_taus_POG_phys14_ID_againstEMu);
  reduced_tree.Branch("num_taus_POG_phys14_ID_mT100", &num_taus_POG_phys14_ID_mT100);
  reduced_tree.Branch("num_taus_POG_phys14_ID_againstEMu_mT100", &num_taus_POG_phys14_ID_againstEMu_mT100);  
  reduced_tree.Branch("tau_pt", &tau_pt);
  reduced_tree.Branch("tau_chargedIsoPtSum", &tau_chargedIsoPtSum);
  reduced_tree.Branch("tau_mT", &tau_mT);

  reduced_tree.Branch("num_iso_tracks_pt10", &num_iso_tracks_pt10);
  reduced_tree.Branch("num_iso_tracks_pt10_mT", &num_iso_tracks_pt10_mT);
  reduced_tree.Branch("num_iso_tracks_pt15", &num_iso_tracks_pt15);
  reduced_tree.Branch("num_iso_tracks_pt15_mT", &num_iso_tracks_pt15_mT);

  reduced_tree.Branch("num_e_iso_tracks", &num_e_iso_tracks);
  reduced_tree.Branch("num_mu_iso_tracks", &num_mu_iso_tracks);
  reduced_tree.Branch("num_had_iso_tracks", &num_had_iso_tracks);
  reduced_tree.Branch("num_e_iso_tracks_mT", &num_e_iso_tracks_mT);
  reduced_tree.Branch("num_mu_iso_tracks_mT", &num_mu_iso_tracks_mT);
  reduced_tree.Branch("num_had_iso_tracks_mT", &num_had_iso_tracks_mT);
  
  reduced_tree.Branch("iso_track_pt", &iso_track_pt);
  reduced_tree.Branch("iso_track_relIso", &iso_track_relIso);
  reduced_tree.Branch("iso_track_mT", &iso_track_mT);
  reduced_tree.Branch("e_track_pt", &e_track_pt);
  reduced_tree.Branch("e_track_relIso", &e_track_relIso);
  reduced_tree.Branch("e_track_mT", &e_track_mT);
  reduced_tree.Branch("mu_track_pt", &mu_track_pt);
  reduced_tree.Branch("mu_track_relIso", &mu_track_relIso);
  reduced_tree.Branch("mu_track_mT", &mu_track_mT);
  reduced_tree.Branch("had_track_pt", &had_track_pt);
  reduced_tree.Branch("had_track_relIso", &had_track_relIso);
  reduced_tree.Branch("had_track_mT", &had_track_mT);

  reduced_tree.Branch("num_true_electrons", &num_true_electrons);
  reduced_tree.Branch("true_electron_pt", &true_electron_pt);
  reduced_tree.Branch("true_electron_eta", &true_electron_eta);
  reduced_tree.Branch("true_electron_min_parton_dR", &true_electron_min_parton_dR);
  reduced_tree.Branch("true_electron_passID", &true_electron_passID);
  reduced_tree.Branch("true_electron_rel_iso", &true_electron_rel_iso);
  reduced_tree.Branch("true_electron_d0", &true_electron_d0);

  reduced_tree.Branch("num_true_muons", &num_true_muons);
  reduced_tree.Branch("true_muon_pt", &true_muon_pt);
  reduced_tree.Branch("true_muon_eta", &true_muon_eta);
  reduced_tree.Branch("true_muon_min_parton_dR", &true_muon_min_parton_dR);
  reduced_tree.Branch("true_muon_passID", &true_muon_passID);
  reduced_tree.Branch("true_muon_rel_iso", &true_muon_rel_iso);
  reduced_tree.Branch("true_muon_d0", &true_muon_d0);

  reduced_tree.Branch("num_true_had_taus", &num_true_had_taus);
  reduced_tree.Branch("true_had_tau_pt", &true_had_tau_pt);
  reduced_tree.Branch("true_had_tau_eta", &true_had_tau_eta);
  reduced_tree.Branch("true_had_tau_min_parton_dR", &true_had_tau_min_parton_dR);

  reduced_tree.Branch("num_true_lep_taus", &num_true_lep_taus);




  int n_to_process(Nentries);
  if(n_to_process<0) n_to_process=GetTotalEntries();
  Timer timer(n_to_process);
  timer.Start();
  for(int i(0); i<n_to_process; ++i){
    if((i%10000==0||i==1||i==10||i==100||i==1000) && i!=0){
      timer.PrintRemainingTime();
    }
    timer.Iterate();
    // cout << "JERR" << endl;
    GetEntry(i);
    entry=i;
   
    //  if (i<675) continue;
    //  cout << "*****Event " << i << "*****" << endl;

    std::pair<std::set<EventNumber>::iterator, bool> returnVal(eventList.insert(EventNumber(run, event, lumiblock)));
    if(!returnVal.second) continue;


    type_code=TypeCode();


    lsp_mass=GetLSPMass();
    gluino_mass=GetGluinoMass();

    // just xsec/nGen now
    passes2012METCleaningCut=false;
    passesCSA14METCleaningCut=false;
    if (cfAVersion<=71||cfAVersion==74) passes2012METCleaningCut=Passes2012METCleaningCut();
    // else passesCSA14METCleaningCut=PassesCSA14METCleaningCut();
    passesBadJetFilter=PassesBadJetFilter();
    PBNRcode=GetPBNR();

    passes2012RA2bTrigger=Passes2012RA2bTrigger();

    pass_HLT_PFHT350_PFMET100=PassesSpecificTrigger("HLT_PFNoPUHT350_PFMET100_v")||PassesSpecificTrigger("HLT_PFHT350_PFMET100_v");
    pass_HLT_PFHT650=PassesSpecificTrigger("HLT_PFNoPUHT650_v")||PassesSpecificTrigger("HLT_PFHT650_v");
    pass_HLT_DiCentralPFJet50_PFMET80=PassesSpecificTrigger("HLT_DiCentralPFNoPUJet50_PFMETORPFMETNoMu80_v")||PassesSpecificTrigger("HLT_DiCentralPFJet50_PFMET80_v");
    pass_HLT_DiCentralPFJet30_PFMET80_BTagCSV07=PassesSpecificTrigger("HLT_DiCentralPFJet30_PFMET80_BTagCSV07_v");

    passesJSONCut=PassesJSONCut();
    passesPVCut=PassesPVCut();
    num_primary_vertices=GetNumVertices();
    pu_true_num_interactions=GetTrueNumInteractions();
    pu_num_interactions=GetNumInteractions();
    eoot_pu=GetEarlyOutOfTimePU(1);
    loot_pu=GetLateOutOfTimePU();
    oot_pu=eoot_pu+loot_pu;
     // cout << "JERR1" << endl;

    highest_csv=GetHighestJetCSV(1);
    second_highest_csv=GetHighestJetCSV(2);
    third_highest_csv=GetHighestJetCSV(3);
    fourth_highest_csv=GetHighestJetCSV(4);
    fifth_highest_csv=GetHighestJetCSV(5);
    sixth_highest_csv=GetHighestJetCSV(6);
     // cout << "JERR2" << endl;

    jet1_pt=GetHighestJetPt(1);
    jet2_pt=GetHighestJetPt(2);
    jet3_pt=GetHighestJetPt(3);
    jet4_pt=GetHighestJetPt(4);
    jet5_pt=GetHighestJetPt(5);
    jet6_pt=GetHighestJetPt(6);
    jet1_eta=GetJetXEta(1);
    jet2_eta=GetJetXEta(2);
    jet3_eta=GetJetXEta(3);
    jet4_eta=GetJetXEta(4);
    jet5_eta=GetJetXEta(5);
    jet6_eta=GetJetXEta(6);
    jet1_phi=GetJetXPhi(1);
    jet2_phi=GetJetXPhi(2);
    jet3_phi=GetJetXPhi(3);
    jet4_phi=GetJetXPhi(4);
    jet5_phi=GetJetXPhi(5);
    jet6_phi=GetJetXPhi(6);
    jet1_csv=GetJetXCSV(1);
    jet2_csv=GetJetXCSV(2);
    jet3_csv=GetJetXCSV(3);
    jet4_csv=GetJetXCSV(4);
    jet5_csv=GetJetXCSV(5);
    jet6_csv=GetJetXCSV(6);
    jet1_partonId=GetJetXPartonId(1);
    jet2_partonId=GetJetXPartonId(2);
    jet3_partonId=GetJetXPartonId(3);
    jet4_partonId=GetJetXPartonId(4);
    jet5_partonId=GetJetXPartonId(5);
    jet6_partonId=GetJetXPartonId(6);
    jet1_motherId=GetJetXMotherId(1);
    jet2_motherId=GetJetXMotherId(2);
    jet3_motherId=GetJetXMotherId(3);
    jet4_motherId=GetJetXMotherId(4);
    jet5_motherId=GetJetXMotherId(5);
    jet6_motherId=GetJetXMotherId(6);
    met=pfTypeImets_et->at(0);
    met_phi=pfTypeImets_phi->at(0);

    ht20=GetHT(20.);
    ht30=GetHT(30.);
    ht40=GetHT(40.);
    ht50=GetHT(50.);
    ht60=GetHT(60.);
    ht70=GetHT(70.);
    ht80=GetHT(80.);
    sumP30=GetSumP(30.);
    sumP50=GetSumP(50.);
    cent30=GetCentrality(30.);
    cent50=GetCentrality(50.);

    num_jets_pt20=GetNumGoodJets(20);
    num_jets_pt30=GetNumGoodJets(30);
    num_jets_pt40=GetNumGoodJets(40);
    num_jets_pt50=GetNumGoodJets(50);
    num_jets_pt70=GetNumGoodJets(70);
    num_jets_pt100=GetNumGoodJets(100);
    num_jets_pt150=GetNumGoodJets(150);
    // cout << "num_jets=" << num_jets_pt50 << endl;
    if (cfAVersion<77) {
    num_csvt_jets=GetNumCSVTJets();
    num_csvm_jets=GetNumCSVMJets();
    num_csvl_jets=GetNumCSVLJets();
    num_csvt_jets20=GetNumCSVTJets(20.);
    num_csvm_jets20=GetNumCSVMJets(20.);
    num_csvl_jets20=GetNumCSVLJets(20.);
    num_csvt_jets30=GetNumCSVTJets(30.);
    num_csvm_jets30=GetNumCSVMJets(30.);
    num_csvl_jets30=GetNumCSVLJets(30.);
    num_csvt_jets40=GetNumCSVTJets(40.);
    num_csvm_jets40=GetNumCSVMJets(40.);
    num_csvl_jets40=GetNumCSVLJets(40.);
    } else {
    num_csvt_jets=GetNumIncCSVTJets();
    num_csvm_jets=GetNumIncCSVMJets();
    num_csvl_jets=GetNumIncCSVLJets();
    num_csvt_jets20=GetNumIncCSVTJets(20.);
    num_csvm_jets20=GetNumIncCSVMJets(20.);
    num_csvl_jets20=GetNumIncCSVLJets(20.);
    num_csvt_jets30=GetNumIncCSVTJets(30.);
    num_csvm_jets30=GetNumIncCSVMJets(30.);
    num_csvl_jets30=GetNumIncCSVLJets(30.);
    num_csvt_jets40=GetNumIncCSVTJets(40.);
    num_csvm_jets40=GetNumIncCSVMJets(40.);
    num_csvl_jets40=GetNumIncCSVLJets(40.);
    }
    num_truth_matched_b_jets=GetNumTruthMatchedBJets();
    num_good_truth_matched_b_jets=GetNumTruthMatchedBJets(0.,true);

    

    // num_true_partons=GetNGenPartons();
    // num_true_partons_pt20=GetNGenPartons(20.);
    // num_true_partons_pt40=GetNGenPartons(40.);
    // num_true_partons_pt70=GetNGenPartons(70.);
    // num_true_partons_pt100=GetNGenPartons(100.);
    // num_true_partons_pt150=GetNGenPartons(150.);

    mht30=GetMHT(30.);
    mht30_phi=GetMHTPhi(30.);
    mht50=GetMHT(50.);
    mht50_phi=GetMHTPhi(50.);
    if (mht30>0) mht_over_sqrt_ht30=mht30/TMath::Sqrt(ht30);
    else mht_over_sqrt_ht30=-1.;
    if (mht50>0) mht_over_sqrt_ht50=mht50/TMath::Sqrt(ht50);
    else mht_over_sqrt_ht50=-1.;


    m_eff20=met+ht20;
    m_eff30=met+ht30;
    m_eff40=met+ht40;
    m_eff50=met+ht50;
    if (m_eff20>0) met_over_m_eff20=met/m_eff20;
    else met_over_m_eff20=-1.;
    if (m_eff30>0) met_over_m_eff30=met/m_eff30;
    else met_over_m_eff30=-1.;
    if (m_eff40>0) met_over_m_eff40=met/m_eff40;
    else met_over_m_eff40=-1.;
    if (m_eff50>0) met_over_m_eff50=met/m_eff50;
    else met_over_m_eff50=-1.;
    if (ht20>0) met_over_sqrt_ht20=met/TMath::Sqrt(ht20);
    else met_over_sqrt_ht20=-1.;
    if (ht30>0) met_over_sqrt_ht30=met/TMath::Sqrt(ht30);
    else met_over_sqrt_ht30=-1.;
    if (ht40>0) met_over_sqrt_ht40=met/TMath::Sqrt(ht40);
    else met_over_sqrt_ht40=-1.;
    if (ht50>0) met_over_sqrt_ht50=met/TMath::Sqrt(ht50);
    else met_over_sqrt_ht50=-1.;
    if (ht80>0) met_over_sqrt_ht80=met/TMath::Sqrt(ht80);
    else met_over_sqrt_ht80=-1.;


    double this_scale_factor(scaleFactor);
    pu_weight=isRealData?1.0:GetPUWeight(lumiWeights);
    top_pt_weight=isttbar?GetTopPtWeight():1.0;
    top_pt_weight_official=isttbar?GetTopPtWeightOfficial():1.0;
    weightppb=this_scale_factor;
    if (cfAVersion<73) weightppb*=(pu_weight*top_pt_weight_official);

    trigger_eff_weight=GetTriggerEffWeight();
    if (cfAVersion<=71) {
    btag_weight=GetBtagWeight();
    btag_pid_weight=GetPIDBtagWeight();


    CalculateTagProbs(Prob0, ProbGEQ1, Prob1, ProbGEQ2, Prob2, ProbGEQ3, Prob3, ProbGEQ4);
    CalculateTagProbs(Prob0_pt50, ProbGEQ1_pt50, Prob1_pt50, ProbGEQ2_pt50, Prob2_pt50, ProbGEQ3_pt50, Prob3_pt50, ProbGEQ4_pt50, 50.);
    }



    vector<int> reco_muons, reco_veto_muons;
    vector<int> reco_veto_muons_iso2D, reco_veto_muons_mT, reco_veto_muons_mT_orth;
    if (cfAVersion>=73&&cfAVersion!=74) {
      reco_muons = GetRecoMuons(false);
      reco_veto_muons = GetRecoMuons(true);
      reco_veto_muons_iso2D = GetRecoMuons(true,true);
      reco_veto_muons_mT=GetRecoMuons(true,false,true);
      reco_veto_muons_mT_orth=GetRecoMuons(true,false,true,true);
    }
    else {
      reco_muons = GetRA2bMuons(false);
      reco_veto_muons = GetRA2bMuons(true);
    }
    num_reco_muons = reco_muons.size();
    num_reco_veto_muons = reco_veto_muons.size();
    num_reco_veto_muons_iso2D = reco_veto_muons_iso2D.size();    
    num_reco_veto_muons_mT100 = reco_veto_muons_mT.size();
    num_reco_veto_muons_mT100_orth = reco_veto_muons_mT_orth.size();

    vector<int> reco_electrons, reco_veto_electrons;
    vector<int> reco_veto_electrons_iso2D, reco_veto_electrons_mT, reco_veto_electrons_mT_orth;
    if (cfAVersion>=73&&cfAVersion!=74) {
      reco_electrons = GetRecoElectrons(false);
      reco_veto_electrons = GetRecoElectrons(true);
      // reco_veto_electrons_iso2D=GetRecoElectrons(true,true);
      // reco_veto_electrons_mT = GetRecoElectrons(true,false,true);
      // reco_veto_electrons_mT_orth = GetRecoElectrons(true,false,true,true);
    }
    else {
      reco_electrons = GetRA2bElectrons(false);
      reco_veto_electrons = GetRA2bElectrons(true);
    }

    num_reco_electrons = reco_electrons.size();
    num_reco_veto_electrons = reco_veto_electrons.size();
    num_reco_veto_electrons_iso2D = reco_veto_electrons_iso2D.size();
    num_reco_veto_electrons_mT100 = reco_veto_electrons_mT.size();
    num_reco_veto_electrons_mT100_orth = reco_veto_electrons_mT_orth.size();



    // muon_gen_mother_id=-999;
    // muon_gen_pt=-999.;
    // muon_gen_eta=-999.;
    // muon_gen_phi=-999.;
    // muon_reco_match=-1;
    // muon_PFmatched=-1;
    // muon_NH_Et=-1.;
    // muon_CH_pt=-1.;
    // muon_ph_Et=-1.;
    // muon_PU_pt=-1.;
    // muon_minDR_jet=-1;
    // muon_mT=-1;

    uint muon_index(0);
    if (cfAVersion>=73&&cfAVersion!=74) {
      for (uint imu(0); imu<mus_pt->size(); imu++) {
	if (isRecoMuon(imu,0)) {
	  muon_index=imu;
	  break;
	}
      }
    } else {
      for (uint imu(0); imu<pf_mus_pt->size(); imu++) {
	if (isRA2bMuon(imu,0)) {
	  muon_index=imu;
	  break;
	}
      }
    }

    muon_true=-1;
    muon_b=-1;
    muon_signal=-1;
    muon_veto=-1;
    muon_pt=-1.;
    muon_eta=-999.;
    muon_phi=-999.;
    muon_relIso=-1.;
    muon_NH_Iso=-1.;
    muon_CH_Iso=-1.;
    muon_ph_Iso=-1.;
    muon_PU_Iso=-1.;  
    muon_ptRel=-1.;
    muon_minDR=-1.;
    muon_mT=-1.;
    muon_dphi_met=-999.;

    if (cfAVersion>=73&&cfAVersion<=76) {
      if (mus_pt->size()>0) {
	muon_signal=isRecoMuon(muon_index,1);
	muon_veto=isRecoMuon(muon_index,0);
	muon_pt=mus_pt->at(muon_index);
	muon_eta=mus_eta->at(muon_index);
	muon_phi=mus_phi->at(muon_index);
	muon_true=isTrueMuon(muon_eta, muon_phi);
	muon_b=isbMuon(muon_eta, muon_phi);
	muon_NH_Iso=mus_pfIsolationR04_sumNeutralHadronEt->at(muon_index)/muon_pt;
	muon_CH_Iso=mus_pfIsolationR04_sumChargedHadronPt->at(muon_index)/muon_pt;
	muon_ph_Iso=mus_pfIsolationR04_sumPhotonEt->at(muon_index)/muon_pt;
	muon_PU_Iso=mus_pfIsolationR04_sumPUPt->at(muon_index)/muon_pt;  
	muon_relIso=GetMuonRelIso(muon_index); 
        TLorentzVector nearestJet(GetNearestJet(TLorentzVector(mus_px->at(muon_index),mus_py->at(muon_index),
							       mus_pz->at(muon_index),mus_energy->at(muon_index)), mus_jet_ind->at(muon_index)));
	if (nearestJet.Pt()>0)
          {
            TLorentzVector muVec;
            muVec.SetPtEtaPhiM(muon_pt,muon_eta,muon_phi,mus_energy->at(muon_index));
            muon_ptRel = TMath::Abs(muVec.Perp(nearestJet.Vect()));
            muon_minDR = Math::GetDeltaR(muon_phi, muon_eta, nearestJet.Phi(), nearestJet.Eta());
          }
	muon_mT=GetMTW(muon_pt,met,muon_phi,met_phi);
	muon_dphi_met=fabs(Math::GetDeltaPhi(muon_phi,met_phi));
      }
    } else if (cfAVersion<=71&&pf_mus_pt->size()>0) {
      muon_signal=isRA2bMuon(muon_index,1);
      //	cout << "JERR4" << endl;
      muon_veto=isRA2bMuon(muon_index,0);
      muon_pt=pf_mus_pt->at(muon_index);
      muon_eta=pf_mus_eta->at(muon_index);
      muon_phi=pf_mus_phi->at(muon_index);
      muon_true=isTrueMuon(muon_eta, muon_phi);
      muon_b=isbMuon(muon_eta, muon_phi);
      muon_NH_Iso=pf_mus_pfIsolationR04_sumNeutralHadronEt->at(muon_index)/muon_pt;
      muon_CH_Iso=pf_mus_pfIsolationR04_sumChargedHadronPt->at(muon_index)/muon_pt;
      muon_ph_Iso=pf_mus_pfIsolationR04_sumPhotonEt->at(muon_index)/muon_pt;
      muon_PU_Iso=pf_mus_pfIsolationR04_sumPUPt->at(muon_index)/muon_pt;  
      muon_relIso=GetRA2bMuonRelIso(muon_index); 
      muon_mT=GetMTW(muon_pt,met,muon_phi,met_phi);
      muon_dphi_met=fabs(Math::GetDeltaPhi(muon_phi,met_phi));
    }
    



    // if (genMuonCache.size()>=1) {
    //   // cout << "Gen muons..." << endl;
    //   muon_gen_mother_id=genMuonCache[0].GetMotherId();
    //   muon_gen_pt=genMuonCache[0].GetLorentzVector().Pt();
    //   muon_gen_eta=genMuonCache[0].GetLorentzVector().Eta();
    //   muon_gen_phi=genMuonCache[0].GetLorentzVector().Phi();
    //   if(genMuonCache[0].GetMusMatch()>=0) {
    // 	muon_reco_match = genMuonCache[0].GetMusMatch();
    // 	if (cmEnergy>=13) {
    // 	  muon_signal=isRecoMuon(muon_reco_match,1);
    // 	  muon_veto=isRecoMuon(muon_reco_match,0);
    // 	  muon_PFmatched=mus_isPF->at(muon_reco_match);
    // 	  muon_pt=mus_pt->at(muon_reco_match);
    // 	  muon_eta=mus_eta->at(muon_reco_match);
    // 	  muon_phi=mus_phi->at(muon_reco_match);
    // 	  muon_NH_Et=mus_pfIsolationR04_sumNeutralHadronEt->at(muon_reco_match);
    // 	  muon_CH_pt=mus_pfIsolationR04_sumChargedHadronPt->at(muon_reco_match);
    // 	  muon_ph_Et=mus_pfIsolationR04_sumPhotonEt->at(muon_reco_match);
    // 	  muon_PU_pt=mus_pfIsolationR04_sumPUPt->at(muon_reco_match);  
    // 	  muon_relIso=GetMuonRelIso(muon_reco_match); 
    // 	}
    // 	else {
    // 	  muon_signal=isRA2bMuon(muon_reco_match,1);
    // 	  muon_veto=isRA2bMuon(muon_reco_match,0);
    // 	  muon_PFmatched=true;
    // 	  muon_pt=pf_mus_pt->at(muon_reco_match);
    // 	  muon_eta=pf_mus_eta->at(muon_reco_match);
    // 	  muon_phi=pf_mus_phi->at(muon_reco_match);
    // 	  muon_NH_Et=pf_mus_pfIsolationR04_sumNeutralHadronEt->at(muon_reco_match);
    // 	  muon_CH_pt=pf_mus_pfIsolationR04_sumChargedHadronPt->at(muon_reco_match);
    // 	  muon_ph_Et=pf_mus_pfIsolationR04_sumPhotonEt->at(muon_reco_match);
    // 	  muon_PU_pt=pf_mus_pfIsolationR04_sumPUPt->at(muon_reco_match);
    // 	  muon_relIso=GetRA2bMuonRelIso(muon_reco_match);
    // 	}
    // 	muon_NH_Iso=muon_NH_Et/muon_pt;
    // 	muon_CH_Iso=muon_CH_pt/muon_pt;
    // 	muon_ph_Iso=muon_ph_Et/muon_pt;
    // 	muon_PU_Iso=muon_PU_pt/muon_pt;
    // 	// cout << "muon_minDR_jet... " << endl;
    // 	muon_minDR_jet=GetMinDRMuonJet(muon_reco_match); // compatible with both old and new samples
    // 	muon_mT=GetMTW(muon_pt,met,muon_phi,met_phi);
    // 	// cout << "... " << muon_minDR_jet << endl;
    //   }
    // }


    // electron_gen_mother_id=-999;
    // electron_gen_pt=-999.;
    // electron_gen_eta=-999.;
    // electron_gen_phi=-999.;
    // electron_reco_match=-1;
    // electron_signal=-1;
    // electron_veto=-1;
    // electron_PFmatched=-1;
    // electron_NH_Et=-1.;
    // electron_CH_pt=-1.;
    // electron_ph_Et=-1.;
    // electron_minDR_jet=-1;
    // electron_mT=-1;

    uint electron_index(0);
    if (cfAVersion>=73&&cfAVersion!=74) {
      for (uint iel(0); iel<els_pt->size(); iel++) {
	if (isRecoElectron(iel,0)) {
	  electron_index=iel;
	  break;
	}
      }
    } else {
      for (uint iel(0); iel<pf_els_pt->size(); iel++) {
	if (isRA2bElectron(iel,0)) {
	  electron_index=iel;
	  break;
	}
      }
    }

    electron_true=-1;
    electron_signal=-1;
    electron_veto=-1;
    electron_pt=-1.;
    electron_eta=-999.;
    electron_phi=-999.;
    electron_relIso=-1.;
    electron_NH_Iso=-1.;
    electron_CH_Iso=-1.;
    electron_ph_Iso=-1.;
    electron_ptRel=-1.;
    electron_minDR=-1.;
    electron_mT=-1.;
    electron_dphi_met=-999.;

    if (cfAVersion>=73&&cfAVersion<=76) {
      if (els_pt->size()>0) {
	electron_signal=isRecoElectron(electron_index,1);
	electron_veto=isRecoElectron(electron_index,0);
	electron_pt=els_pt->at(electron_index);
	electron_eta=els_scEta->at(electron_index);
	electron_phi=els_phi->at(electron_index);
	electron_true=isTrueElectron(electron_eta, electron_phi);
	electron_NH_Iso=els_pfIsolationR03_sumNeutralHadronEt->at(electron_index);
	electron_CH_Iso=els_pfIsolationR03_sumChargedHadronPt->at(electron_index);
	electron_ph_Iso=els_pfIsolationR03_sumPhotonEt->at(electron_index);
	electron_relIso=GetCSAElectronIsolation(electron_index); 
	TLorentzVector nearestJet(GetNearestJet(TLorentzVector(els_px->at(electron_index),els_py->at(electron_index),
							       els_pz->at(electron_index),els_energy->at(electron_index)), els_jet_ind->at(electron_index)));
	if (nearestJet.Pt()>0)
	  {
	    TLorentzVector elVec;
	    elVec.SetPtEtaPhiM(electron_pt,electron_eta,electron_phi,els_energy->at(electron_index));
	    electron_ptRel = TMath::Abs(elVec.Perp(nearestJet.Vect()));
	    electron_minDR = Math::GetDeltaR(els_phi->at(electron_index), els_eta->at(electron_index), nearestJet.Phi(), nearestJet.Eta());
	  }
	electron_mT=GetMTW(electron_pt,met,electron_phi,met_phi);
	electron_dphi_met=fabs(Math::GetDeltaPhi(electron_phi,met_phi));
      }
    } else if (cfAVersion<=71){
      if (pf_els_pt->size()>0) {
	electron_signal=isRA2bElectron(electron_index,1);
	electron_veto=isRA2bElectron(electron_index,0);
	electron_pt=pf_els_pt->at(electron_index);
	electron_eta=pf_els_scEta->at(electron_index);
	electron_phi=pf_els_phi->at(electron_index);
	electron_true=isTrueElectron(electron_eta, electron_phi);
	electron_NH_Iso=pf_els_PFneutralHadronIsoR03->at(electron_index);
	electron_CH_Iso=pf_els_PFchargedHadronIsoR03->at(electron_index);
	electron_ph_Iso=pf_els_PFphotonIsoR03->at(electron_index);
	electron_relIso=GetElectronRelIso(electron_index); 
	electron_mT=GetMTW(electron_pt,met,electron_phi,met_phi);
	electron_dphi_met=fabs(Math::GetDeltaPhi(electron_phi,met_phi));
      }
    }



    // if (genElectronCache.size()>=1) {
    //   // cout << "Gen electrons..." << endl;
    //   electron_gen_mother_id=genElectronCache[0].GetMotherId();
    //   electron_gen_pt=genElectronCache[0].GetLorentzVector().Pt();
    //   electron_gen_eta=genElectronCache[0].GetLorentzVector().Eta();
    //   electron_gen_phi=genElectronCache[0].GetLorentzVector().Phi();
    //   if(genElectronCache[0].GetElsMatch()>=0) {
    // 	// cout << "Reco electrons..." << endl;
    // 	electron_reco_match = genElectronCache[0].GetElsMatch();
    // 	if (cmEnergy<13) { 
    // 	  // // cout << "electron id..." << endl;
    // 	  // electron_signal=isRA2bElectron(electron_reco_match,1);
    // 	  // electron_veto=isRA2bElectron(electron_reco_match,0);
    // 	  // electron_pt=pf_els_pt->at(electron_reco_match);
    // 	  // electron_eta=pf_els_eta->at(electron_reco_match);
    // 	  // electron_phi=pf_els_phi->at(electron_reco_match);
    // 	  // electron_PFmatched=true;
    // 	  // // cout << "electron isolation..." << endl;
    // 	  // electron_relIso=GetElectronRelIso(electron_reco_match);
    // 	  // electron_NH_Et=els_PFneutralHadronIsoR03->at(electron_reco_match);
    // 	  // electron_CH_pt=els_PFchargedHadronIsoR03->at(electron_reco_match);
    // 	  // electron_ph_Et=els_PFphotonIsoR03->at(electron_reco_match);
    // 	}
    // 	else {
    // 	  electron_signal=isRecoElectron(electron_reco_match,1);
    // 	  electron_veto=isRecoElectron(electron_reco_match,0);
    // 	  electron_pt=els_pt->at(electron_reco_match);
    // 	  electron_eta=els_eta->at(electron_reco_match);
    // 	  electron_phi=els_phi->at(electron_reco_match);
    // 	  electron_PFmatched=els_isPF->at(electron_reco_match);
    // 	  electron_relIso=GetCSAElectronIsolation(electron_reco_match);
    // 	  electron_NH_Et=els_pfIsolationR03_sumNeutralHadronEt->at(electron_reco_match);
    // 	  electron_CH_pt=els_pfIsolationR03_sumChargedHadronPt->at(electron_reco_match);
    // 	  electron_ph_Et=els_pfIsolationR03_sumPhotonEt->at(electron_reco_match);
    // 	}
    // 	electron_NH_Iso=electron_NH_Et/electron_pt;
    // 	electron_CH_Iso=electron_CH_pt/electron_pt;
    // 	electron_ph_Iso=electron_ph_Et/electron_pt;
    // 	// cout << "electron_minDR_jet... " << endl;
    // 	electron_minDR_jet=GetMinDRElectronJet(electron_reco_match); // compatible with both old and new samples
    // 	// cout << "... " << electron_minDR_jet << endl;
    // 	electron_mT=GetMTW(electron_pt,met,electron_phi,met_phi);
    //   }
    // }
  



    //     cout << "minDeltaPhi..." << endl;
    min_delta_phi_met=GetMinDeltaPhiMET(3,30.);
    min_delta_phi_met_loose_jets=GetMinDeltaPhiMET(3,20.,5.);
    min_delta_phi_met_N=getMinDeltaPhiMETN(3,30.,2.4,true,30.,2.4,true,true);
    min_delta_phi_met_N_2012=getMinDeltaPhiMETN(3,50.,2.4,true,30.,2.4,true,true);
    min_delta_phi_met_N_loose_jets=getMinDeltaPhiMETN(3,20.,5.,true,20.,5.,false,true);
    jet1_DeltaPhiMETN = getDeltaPhiMETN(GetJetXIndex(1));
    jet2_DeltaPhiMETN = getDeltaPhiMETN(GetJetXIndex(2));
    jet3_DeltaPhiMETN = getDeltaPhiMETN(GetJetXIndex(3));

    num_iso_tracks_pt10=0;
    num_iso_tracks_pt10_mT=0;
    num_iso_tracks_pt15=0;
    num_iso_tracks_pt15_mT=0;
    num_e_iso_tracks=0;
    num_mu_iso_tracks=0;
    num_had_iso_tracks=0;
    num_e_iso_tracks_mT=0;
    num_mu_iso_tracks_mT=0;
    num_had_iso_tracks_mT=0;
    iso_track_pt=-1;
    iso_track_relIso=-1;
    iso_track_mT=-1;
    e_track_pt=-1;
    e_track_relIso=-1;
    e_track_mT=-1;
    mu_track_pt=-1;
    mu_track_relIso=-1;
    mu_track_mT=-1;
    had_track_pt=-1;
    had_track_relIso=-1;
    had_track_mT=-1;
    vector<std::pair<int, double> > e_tracks, mu_tracks, had_tracks;
    if (cfAVersion==71||cfAVersion==74||cfAVersion>=76) {
      num_iso_tracks_pt10=GetNumIsoTracks(10, false);
      num_iso_tracks_pt10_mT=GetNumIsoTracks(10, true);
      // store index and relIso of tracks
      vector<pair<int, double> > isotracks = GetIsoTracks(15, false);
      num_iso_tracks_pt15=isotracks.size();
      if (num_iso_tracks_pt15>0) {
	iso_track_pt=isotk_pt->at(isotracks[0].first);
	iso_track_relIso=isotracks[0].second;
	iso_track_mT=GetMTW(iso_track_pt, met, isotk_phi->at(isotracks[0].first), met_phi);
	for (uint itk(0); itk<isotracks.size(); itk++) {
	  if (GetMTW(isotk_pt->at(isotracks[itk].first), met, isotk_phi->at(isotracks[itk].first), met_phi)<100) num_iso_tracks_pt15_mT++;
	}
      }
      if (cfAVersion>=76) {
	NewGetIsoTracks(e_tracks, mu_tracks, had_tracks, false);
	if (e_tracks.size()>0) {
	  e_track_pt=pfcand_pt->at(e_tracks[0].first);
	  double e_track_phi=pfcand_phi->at(e_tracks[0].first);
	  e_track_mT=GetMTW(e_track_pt, met, e_track_phi, met_phi);
	  e_track_relIso=e_tracks[0].second;
	}
	if (mu_tracks.size()>0) {
	  mu_track_pt=pfcand_pt->at(mu_tracks[0].first);
	  double mu_track_phi=pfcand_phi->at(mu_tracks[0].first);
	  mu_track_mT=GetMTW(mu_track_pt, met, mu_track_phi, met_phi);
	  mu_track_relIso=mu_tracks[0].second;
	}
	if (had_tracks.size()>0) {
	  had_track_pt=pfcand_pt->at(had_tracks[0].first);
	  double had_track_phi=pfcand_phi->at(had_tracks[0].first);
	  had_track_mT=GetMTW(had_track_pt, met, had_track_phi, met_phi);
	  had_track_relIso=had_tracks[0].second;
	}
	for (uint itk(0); itk<e_tracks.size(); itk++) {
	  double pt=pfcand_pt->at(e_tracks[itk].first);
	  double phi=pfcand_phi->at(e_tracks[itk].first);
	  double mT=GetMTW(pt, met, phi, met_phi);
	  if (e_tracks[itk].second>0.2) continue;
	  num_e_iso_tracks++;
	  if (mT<100)  num_e_iso_tracks_mT++;
	}
	for (uint itk(0); itk<mu_tracks.size(); itk++) {
	  double pt=pfcand_pt->at(mu_tracks[itk].first);
	  double phi=pfcand_phi->at(mu_tracks[itk].first);
	  double mT=GetMTW(pt, met, phi, met_phi);
	  if (mu_tracks[itk].second>0.2) continue;
	  num_mu_iso_tracks++;
	  if (mT<100)  num_mu_iso_tracks_mT++;
	}
	for (uint itk(0); itk<had_tracks.size(); itk++) {
	  double pt=pfcand_pt->at(had_tracks[itk].first);
	  if(pt<10) continue;
	  double phi=pfcand_phi->at(had_tracks[itk].first);
	  double mT=GetMTW(pt, met, phi, met_phi);
	  if (had_tracks[itk].second>0.1) continue;
	  num_had_iso_tracks++;
	  if (mT<100)  num_had_iso_tracks_mT++;
	}
      }
    }

    // cout << "mT..." << endl;
    mT_mu=GetTransverseMassMu();    
    mT_el=GetTransverseMassEl();
    if (num_reco_veto_electrons+num_reco_veto_muons!=1) {
      deltaThetaT=-999.;
      W_pT=-999.;
    } else if (num_reco_veto_electrons==1&&num_reco_veto_muons==0) {
      deltaThetaT=GetDeltaThetaT(electron_pt, electron_phi);
      W_pT=GetWpT(electron_pt, electron_phi);
    } else if (num_reco_veto_muons==1&&num_reco_veto_electrons==0) {
      deltaThetaT=GetDeltaThetaT(muon_pt, muon_phi);
      W_pT=GetWpT(muon_pt, muon_phi);
    }
    SL_control_sample=(num_reco_veto_electrons+num_reco_veto_muons==1)&&(mT_el<100.||mT_mu<100.);
    QCD_control_sample=(min_delta_phi_met_N<4.);

    ht_bin1=(ht50>400&&ht50<500);
    ht_bin2=(ht50>500&&ht50<800);
    ht_bin3=(ht50>800&&ht50<1000);
    ht_bin4=(ht50>1000);

    met_bin1=(met>125&&met<150);
    met_bin2=(met>150&&met<250);
    met_bin3=(met>250&&met<350);
    met_bin4=(met>350);

    nb_bin1=(num_csvm_jets==1);
    nb_bin2=(num_csvm_jets==2);
    nb_bin3=(num_csvm_jets>=3);

    doc_met=GetDocMET();
    gluino1_pt=GetGluinoPt(1);
    gluino2_pt=GetGluinoPt(2);

    // cout << "Fat jets..." << endl;
    // if (cfAVersion>=74&&cfAVersion<=76) {
    //   fatpT10_jet1_pt=GetFatJetPt(0,10);    
    //   fatpT10_jet2_pt=GetFatJetPt(1,10);
    //   fatpT10_jet3_pt=GetFatJetPt(2,10);
    //   fatpT10_jet4_pt=GetFatJetPt(3,10);
    //   fatpT10_jet1_nConst=GetFatJetnConst(0,10);
    //   fatpT10_jet2_nConst=GetFatJetnConst(1,10);
    //   fatpT10_jet3_nConst=GetFatJetnConst(2,10);
    //   fatpT10_jet4_nConst=GetFatJetnConst(3,10);
    //   fatpT10_jet1_mJ=GetFatJetmJ(0,10);
    //   fatpT10_jet2_mJ=GetFatJetmJ(1,10);
    //   fatpT10_jet3_mJ=GetFatJetmJ(2,10);
    //   fatpT10_jet4_mJ=GetFatJetmJ(3,10);
    //   num_fatpT10_jets=GetNFatJets(50.,10);
    //   num_fatpT10_jets_pt100=GetNFatJets(100.,10);
    //   num_fatpT10_jets_pt150=GetNFatJets(150.,10);
    //   num_fatpT10_jets_pt200=GetNFatJets(200.,10);
    //   num_fatpT10_jets_pt300=GetNFatJets(300.,10);
    //   fatpT10_MJ=GetMJ(50.,10);
    //   fatpT10_MJ_pt100=GetMJ(100.,10);
    //   fatpT10_MJ_pt150=GetMJ(150.,10);
    //   fatpT10_MJ_pt200=GetMJ(200.,10);
    //   fatpT10_MJ_pt300=GetMJ(300.,10);

    //   fatpT15_jet1_pt=GetFatJetPt(0,15);    
    //   fatpT15_jet2_pt=GetFatJetPt(1,15);
    //   fatpT15_jet3_pt=GetFatJetPt(2,15);
    //   fatpT15_jet4_pt=GetFatJetPt(3,15);
    //   fatpT15_jet1_nConst=GetFatJetnConst(0,15);
    //   fatpT15_jet2_nConst=GetFatJetnConst(1,15);
    //   fatpT15_jet3_nConst=GetFatJetnConst(2,15);
    //   fatpT15_jet4_nConst=GetFatJetnConst(3,15);
    //   fatpT15_jet1_mJ=GetFatJetmJ(0,15);
    //   fatpT15_jet2_mJ=GetFatJetmJ(1,15);
    //   fatpT15_jet3_mJ=GetFatJetmJ(2,15);
    //   fatpT15_jet4_mJ=GetFatJetmJ(3,15);
    //   num_fatpT15_jets=GetNFatJets(50.,15);
    //   num_fatpT15_jets_pt100=GetNFatJets(100.,15);
    //   num_fatpT15_jets_pt150=GetNFatJets(150.,15);
    //   num_fatpT15_jets_pt200=GetNFatJets(200.,15);
    //   num_fatpT15_jets_pt300=GetNFatJets(300.,15);
    //   fatpT15_MJ=GetMJ(50.,15);
    //   fatpT15_MJ_pt100=GetMJ(100.,15);
    //   fatpT15_MJ_pt150=GetMJ(150.,15);
    //   fatpT15_MJ_pt200=GetMJ(200.,15);
    //   fatpT15_MJ_pt300=GetMJ(300.,15);

    //   fatpT20_jet1_pt=GetFatJetPt(0,20);    
    //   fatpT20_jet2_pt=GetFatJetPt(1,20);
    //   fatpT20_jet3_pt=GetFatJetPt(2,20);
    //   fatpT20_jet4_pt=GetFatJetPt(3,20);
    //   fatpT20_jet1_nConst=GetFatJetnConst(0,20);
    //   fatpT20_jet2_nConst=GetFatJetnConst(1,20);
    //   fatpT20_jet3_nConst=GetFatJetnConst(2,20);
    //   fatpT20_jet4_nConst=GetFatJetnConst(3,20);
    //   fatpT20_jet1_mJ=GetFatJetmJ(0,20);
    //   fatpT20_jet2_mJ=GetFatJetmJ(1,20);
    //   fatpT20_jet3_mJ=GetFatJetmJ(2,20);
    //   fatpT20_jet4_mJ=GetFatJetmJ(3,20);
    //   num_fatpT20_jets=GetNFatJets(50.,20);
    //   num_fatpT20_jets_pt100=GetNFatJets(100.,20);
    //   num_fatpT20_jets_pt150=GetNFatJets(150.,20);
    //   num_fatpT20_jets_pt200=GetNFatJets(200.,20);
    //   num_fatpT20_jets_pt300=GetNFatJets(300.,20);
    //   fatpT20_MJ=GetMJ(50.,20);
    //   fatpT20_MJ_pt100=GetMJ(100.,20);
    //   fatpT20_MJ_pt150=GetMJ(150.,20);
    //   fatpT20_MJ_pt200=GetMJ(200.,20);
    //   fatpT20_MJ_pt300=GetMJ(300.,20);

    //   fatpT25_jet1_pt=GetFatJetPt(0,25);    
    //   fatpT25_jet2_pt=GetFatJetPt(1,25);
    //   fatpT25_jet3_pt=GetFatJetPt(2,25);
    //   fatpT25_jet4_pt=GetFatJetPt(3,25);
    //   fatpT25_jet1_nConst=GetFatJetnConst(0,25);
    //   fatpT25_jet2_nConst=GetFatJetnConst(1,25);
    //   fatpT25_jet3_nConst=GetFatJetnConst(2,25);
    //   fatpT25_jet4_nConst=GetFatJetnConst(3,25);
    //   fatpT25_jet1_mJ=GetFatJetmJ(0,25);
    //   fatpT25_jet2_mJ=GetFatJetmJ(1,25);
    //   fatpT25_jet3_mJ=GetFatJetmJ(2,25);
    //   fatpT25_jet4_mJ=GetFatJetmJ(3,25);
    //   num_fatpT25_jets=GetNFatJets(50.,25);
    //   num_fatpT25_jets_pt100=GetNFatJets(100.,25);
    //   num_fatpT25_jets_pt150=GetNFatJets(150.,25);
    //   num_fatpT25_jets_pt200=GetNFatJets(200.,25);
    //   num_fatpT25_jets_pt300=GetNFatJets(300.,25);
    //   fatpT25_MJ=GetMJ(50.,25);
    //   fatpT25_MJ_pt100=GetMJ(100.,25);
    //   fatpT25_MJ_pt150=GetMJ(150.,25);
    //   fatpT25_MJ_pt200=GetMJ(200.,25);
    //   fatpT25_MJ_pt300=GetMJ(300.,25);
    // }
    fatpT30_jet1_pt=GetFatJetPt(0);    
    fatpT30_jet2_pt=GetFatJetPt(1);
    fatpT30_jet3_pt=GetFatJetPt(2);
    fatpT30_jet4_pt=GetFatJetPt(3);
    fatpT30_jet1_eta=GetFatJetEta(0);    
    fatpT30_jet2_eta=GetFatJetEta(1);
    fatpT30_jet3_eta=GetFatJetEta(2);
    fatpT30_jet4_eta=GetFatJetEta(3);
    fatpT30_jet1_phi=GetFatJetPhi(0);    
    fatpT30_jet2_phi=GetFatJetPhi(1);
    fatpT30_jet3_phi=GetFatJetPhi(2);
    fatpT30_jet4_phi=GetFatJetPhi(3);
    fatpT30_jet1_energy=GetFatJetEnergy(0);    
    fatpT30_jet2_energy=GetFatJetEnergy(1);
    fatpT30_jet3_energy=GetFatJetEnergy(2);
    fatpT30_jet4_energy=GetFatJetEnergy(3);
    //      	cout << "JERR4" << endl;

    fatpT30_jet1_nConst=GetFatJetnConst(0);
    fatpT30_jet2_nConst=GetFatJetnConst(1);
    fatpT30_jet3_nConst=GetFatJetnConst(2);
    fatpT30_jet4_nConst=GetFatJetnConst(3);
    fatpT30_jet1_mJ=GetFatJetmJ(0);
    fatpT30_jet2_mJ=GetFatJetmJ(1);
    fatpT30_jet3_mJ=GetFatJetmJ(2);
    fatpT30_jet4_mJ=GetFatJetmJ(3);
    num_fatpT30_jets=GetNFatJets(50.);
    num_fatpT30_jets_pt100=GetNFatJets(100.);
    num_fatpT30_jets_pt150=GetNFatJets(150.);
    num_fatpT30_jets_pt200=GetNFatJets(200.);
    num_fatpT30_jets_pt300=GetNFatJets(300.);
    fatpT30_MJ=GetMJ(50.);
    fatpT30_MJ_pt100=GetMJ(100.);
    fatpT30_MJ_pt150=GetMJ(150.);
    fatpT30_MJ_pt200=GetMJ(200.);
    fatpT30_MJ_pt300=GetMJ(300.);
    num_fatpT30central_jets=GetNFatJets(50.,1.5);
    num_fatpT30central_jets_pt100=GetNFatJets(100.,1.5);
    num_fatpT30central_jets_pt150=GetNFatJets(150.,1.5);
    num_fatpT30central_jets_pt200=GetNFatJets(200.,1.5);
    num_fatpT30central_jets_pt300=GetNFatJets(300.,1.5);
    fatpT30central_MJ=GetMJ(50.,1.5);
    fatpT30central_MJ_pt100=GetMJ(100.,1.5);
    fatpT30central_MJ_pt150=GetMJ(150.,1.5);
    fatpT30central_MJ_pt200=GetMJ(200.,1.5);
    fatpT30central_MJ_pt300=GetMJ(300.,1.5);

    highest_mJ=GetHighestFatJetmJ(1);
    scnd_highest_mJ=GetHighestFatJetmJ(2);
    thrd_highest_mJ=GetHighestFatJetmJ(3);
    frth_highest_mJ=GetHighestFatJetmJ(4);
    

    min_mTWB=GetMinMTWb(30., 0.814, false);
    min_mTWB_Wmass=GetMinMTWb(30., 0.814, true);
    mTWB_2nd=Get2ndMTWb();

    min_delta_phi_b_met=GetMinDeltaPhibMET();

    num_taus_POG_phys14_ID = GetNumTaus();
    num_taus_POG_phys14_ID_againstEMu = GetNumTaus(true);
    num_taus_POG_phys14_ID_mT100 = GetNumTaus(false, true);
    num_taus_POG_phys14_ID_againstEMu_mT100 = GetNumTaus(true, true);

    tau_pt=-1;
    tau_chargedIsoPtSum=-1;
    tau_mT=-1;
    if (taus_pt->size()>0)
      {
	tau_pt=taus_pt->at(0);
	tau_chargedIsoPtSum=taus_pt->at(0);
	tau_mT=GetMTW(tau_pt,met,taus_phi->at(0),met_phi);
      }

      std::vector<int> true_electrons;
      std::vector<int> true_muons;
      std::vector<int> true_had_taus;
      std::vector<int> true_lep_taus;
      GetTrueLeptons(true_electrons, true_muons, true_had_taus, true_lep_taus);
      //     cout << "Found " << true_electrons.size() << " prompt electrons." << endl;
      std::vector<int> matched_electrons = MatchElectrons(true_electrons);
      //      cout << "Found " << true_muons.size() << " prompt muons." << endl;
      std::vector<int> matched_muons = MatchMuons(true_muons);

      num_true_electrons=true_electrons.size();
      num_true_muons=true_muons.size();
      num_true_had_taus=true_had_taus.size();
      num_true_lep_taus=true_lep_taus.size();

      true_electron_pt.clear();
      true_electron_eta.clear();
      true_electron_min_parton_dR.clear();
      true_electron_passID.clear();
      true_electron_rel_iso.clear();
      true_electron_d0.clear();
      for (uint iel(0); iel<num_true_electrons; iel++) {
	true_electron_pt.push_back(mc_doc_pt->at(true_electrons[iel]));
	true_electron_eta.push_back(mc_doc_eta->at(true_electrons[iel]));
	true_electron_min_parton_dR.push_back(GetDRToClosestParton(true_electrons[iel]));
	if (matched_electrons[iel]>=0)	{
	  true_electron_passID.push_back(isRecoElectron(matched_electrons[iel],0));
	  true_electron_rel_iso.push_back(GetCSAElectronIsolation(matched_electrons[iel]));
	  true_electron_d0.push_back(GetElectronD0(matched_electrons[iel]));
	} else {
	  true_electron_passID.push_back(-999.);
	  true_electron_rel_iso.push_back(-999.);
	  true_electron_d0.push_back(-999.);
	}
      }
      
      true_muon_pt.clear();
      true_muon_eta.clear();
      true_muon_min_parton_dR.clear();
      true_muon_passID.clear();
      true_muon_rel_iso.clear();
      true_muon_d0.clear();
      for (uint imu(0); imu<num_true_muons; imu++) {
	true_muon_pt.push_back(mc_doc_pt->at(true_muons[imu]));
	true_muon_eta.push_back(mc_doc_eta->at(true_muons[imu]));
	true_muon_min_parton_dR.push_back(GetDRToClosestParton(true_muons[imu]));
	//	cout << "True muon " << imu << ": ";
	if (matched_muons[imu]>=0)	{
	  //	  cout << "matched to reco muon " << matched_muons[imu] << endl;
	  true_muon_passID.push_back(isRecoMuon(matched_muons[imu],0));
	  true_muon_rel_iso.push_back(GetMuonRelIso(matched_muons[imu]));
	  true_muon_d0.push_back(GetMuonD0(matched_muons[imu]));
	} else {
	  //	  cout << "not matched." << endl;
	  true_muon_passID.push_back(-999.);
	  true_muon_rel_iso.push_back(-999.);
	  true_muon_d0.push_back(-999.);
	}
      }
      
      true_had_tau_pt.clear();
      true_had_tau_eta.clear();
      true_had_tau_min_parton_dR.clear();
      for (uint itau(0); itau<num_true_had_taus; itau++) {
	true_had_tau_pt.push_back(mc_doc_pt->at(true_had_taus[itau]));
	true_had_tau_eta.push_back(mc_doc_eta->at(true_had_taus[itau]));
	true_had_tau_min_parton_dR.push_back(GetDRToClosestParton(true_had_taus[itau]));
      }
      
    reduced_tree.Fill(); 
    //    if (i==20) break;

  }

  // Tau ID
  
  reduced_tree.Write();
  file.Close();
}
