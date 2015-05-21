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
				   const int Nentries_in,
				   const jec_type_t jec_type):
  EventHandler(in_file_name, is_list, weight_in, false, jec_type),
  Nentries(Nentries_in){
  }

void ReducedTreeMaker::MakeReducedTree(const std::string& out_file_name, const bool is_skimmed){

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

  vector<float> jet_pt, jet_eta, jet_phi, jet_csv, jet_energy;
  vector<float> jet_corr_def, jet_corr_new;
  vector<float> jet_l1_corr, jet_l2_corr, jet_l3_corr;
  vector<bool> jet_looseID;
  vector<float> jet_delta_phi_met, jet_delta_phi_mht, jet_delta_T, jet_mT;
  vector<float> jet_mdp_jet, jet_mdR_jet;
  vector<int> jet_parton_id, jet_parton_mother_id;
  // vector<bool> jet_gen_match;
  vector<float> jet_gen_pt, jet_gen_E, jet_gen_emE, jet_gen_hadE, jet_gen_invE;
  vector<bool> jet_blep_tru;
  vector<float> jet_muF, jet_chgEmF, jet_nEmF, jet_chgHadF, jet_nHadF;
  vector<int> jet_mu_mult, jet_nu_mult, jet_nu_id, jet_nu_mom_id;
  vector<float> jet_nu_mht;

  int mmjet, omjet, umjet;
  float wjetmm, wjetom, wjetum;
  
  float jet1_pt(0.0), jet2_pt(0.), jet3_pt(0.), jet4_pt(0.), jet5_pt(0.), jet6_pt(0.);
  // float jet1_eta(0.0), jet2_eta(0.), jet3_eta(0.), jet4_eta(0.), jet5_eta(0.), jet6_eta(0.);
  // float jet1_phi(0.0), jet2_phi(0.), jet3_phi(0.), jet4_phi(0.), jet5_phi(0.), jet6_phi(0.);
  // float jet1_csv(0.0), jet2_csv(0.), jet3_csv(0.), jet4_csv(0.), jet5_csv(0.), jet6_csv(0.);
  // float jet1_partonId(0.0), jet2_partonId(0.), jet3_partonId(0.), jet4_partonId(0.), jet5_partonId(0.), jet6_partonId(0.);
  // float jet1_motherId(0.0), jet2_motherId(0.), jet3_motherId(0.), jet4_motherId(0.), jet5_motherId(0.), jet6_motherId(0.);

  float pu_num_interactions(0.0), pu_true_num_interactions(0.0);
  unsigned short num_primary_vertices(0);

  float eoot_pu, loot_pu, oot_pu;

  float def_met(0.0), def_met_phi(0.0);
  float met(0.0), met_phi(0.0);
  float def_mht(0.0), def_mht_phi(0.0);
  float mht30(0.0), mht30_phi(0.0),  mht50(0.0), mht50_phi(0.0);
  float mht30_raw(0.0), mht30_phi_raw(0.0),  mht50_raw(0.0), mht50_phi_raw(0.0);
  float mht30_central(0.), mht30_central_phi(0.),  mht30_central_raw(0.), mht30_central_raw_phi(0.);
  //float mht30_corr(0.0), mht30_phi_corr(0.0),  mht50_corr(0.0), mht50_phi_corr(0.0);
  float ht20(0.0), ht30(0.0), ht40(0.0), ht50(0.0), ht60(0.0), ht70(0.0), ht80(0.0);
  float ht30_forward(0.);
  
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



  int num_mus(0), num_veto_mus(0);
  int num_low_pt_mus(0), num_high_pt_mus(0);
  int num_els(0), num_veto_els(0);
  int num_low_pt_els(0), num_high_pt_els(0);
  int num_veto_taus_mT(0);
  vector<bool> tau_loose, tau_medium, tau_tight;
  vector<float> tau_pt, tau_dB_iso, tau_chargedIsoPtSum, tau_mT;

  float iso_track_pt, iso_track_dB_iso, iso_track_mT;


  // int num_lost_els(0), num_lost_mus(0), num_lost_leptons(0);



  vector<bool> mu_signal, mu_veto, mu_vid, mu_tm;
  vector<float> mu_pt, mu_eta, mu_phi;
  vector<float> mu_dB_iso_R04, mu_mini_iso_tr02, mu_ch_iso;
  vector<float> mu_mT, mu_ptW, mu_dTT, mu_ptW_MHT, mu_dTT_MHT;
  vector<int> mu_truid, mu_momid;
  

  vector<bool> el_signal, el_veto, el_sid, el_vid, el_tm;
  vector<float> el_pt, el_eta, el_phi;
  vector<float> el_dB_iso_R03, el_mini_iso_tr02, el_ch_iso;
  vector<float> el_mT, el_ptW, el_dTT, el_ptW_MHT, el_dTT_MHT;
  vector<int> el_truid, el_momid;


  float min_delta_phi_met, min_delta_phi_met_loose_jets;
  float min_delta_phi_met_N, min_delta_phi_met_N_loose_jets;
  float min_delta_phi_met_N_old;
  float min_delta_phi_met_2jets, min_delta_phi_met_4jets, min_delta_phi_met_alljets;
  float min_delta_phi_met_N_2jets, min_delta_phi_met_N_4jets, min_delta_phi_met_N_alljets;
  float min_delta_phi_met_N_2012;
  float min_delta_phi_mht, min_delta_phi_mht_N;
  // float deltaPhiN_1, deltaPhiN_2, deltaPhiN_3;
  int num_iso_tracks_pt10, num_iso_tracks_pt10_mT, num_iso_tracks_pt15, num_iso_tracks_pt15_mT;
  // int num_el_tracks, num_mu_tracks, num_had_tracks;
  int num_el_tracks_mT, num_mu_tracks_mT, num_had_tracks_mT;
  //int num_had_tracks_no_iso, num_had_tracks_mT_no_iso;

  vector<bool> el_track_tm;
  vector<int> el_track_truid, el_track_momid;
  vector<float> el_track_pt, el_track_ch_iso, el_track_mT;
  vector<bool> mu_track_tm;
  vector<int> mu_track_truid, mu_track_momid;
  vector<float> mu_track_pt, mu_track_ch_iso, mu_track_mT;
  vector<bool> had_track_from_tau;
  vector<int> had_track_truid;
  vector<float> had_track_pt, had_track_ch_iso, had_track_mT;
  // bool had_track_from_tau;
  // int had_track_truid;
  // float had_track_pt, had_track_ch_iso, had_track_dB_iso, had_track_mini_iso, had_track_mT;
  

  float mT_mu, mT_el;
  float deltaThetaT, W_pT;
  bool SL_control_sample, QCD_control_sample;

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

  double sum_skinny_jet_mass(0.);

  double min_mTWB(0.), min_mTWB_Wmass(0.);
  double mTWB_2nd(0.);
  double min_delta_phi_b_met(0.);

  double doc_met(0.);
  double gluino1_pt(0.), gluino2_pt(0.);

  unsigned short num_true_els(0), num_true_mus(0), num_true_had_taus(0), num_true_lep_taus(0);
  vector<float> true_el_pt, true_el_eta, true_el_min_parton_dR, true_el_mini_iso, true_el_d0, true_el_tk_pt, true_el_tk_ch_iso;
  vector<bool> true_el_reco, true_el_passID, true_el_track;
  vector<float> true_el_ptW, true_el_dTT;
  vector<float> true_el_ptW_MHT, true_el_dTT_MHT;
  vector<float> true_mu_pt, true_mu_eta, true_mu_min_parton_dR, true_mu_mini_iso, true_mu_d0, true_mu_tk_pt, true_mu_tk_ch_iso;
  vector<bool> true_mu_reco, true_mu_passID, true_mu_track;
  vector<float> true_mu_ptW, true_mu_dTT;
  vector<float> true_mu_ptW_MHT, true_mu_dTT_MHT;
  vector<float> true_had_tau_pt, true_had_tau_eta, true_had_tau_min_parton_dR;
  vector<float> true_had_tau_ptW, true_had_tau_dTT;
  vector<float> true_had_tau_ptW_MHT, true_had_tau_dTT_MHT;

  vector<float> genW_pt;
  vector<bool> genW_lep;

  unsigned int num_photons_pt100(0), num_photons_pt100_old(0);
  float ph_mht(0);
  vector<float> ph_pt, ph_eta;
  vector<bool> ph_id;
  vector<float> ph_ch_iso, ph_nh_iso, ph_ph_iso;
  vector<float> ph_ch_iso_cut, ph_nh_iso_cut, ph_ph_iso_cut;

  //  cout << "Defining tree branches" << endl;

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

  reduced_tree.Branch("jet_pt", &jet_pt);
  reduced_tree.Branch("jet_eta", &jet_eta);
  reduced_tree.Branch("jet_phi", &jet_phi);
  reduced_tree.Branch("jet_energy", &jet_energy);
  reduced_tree.Branch("jet_csv", &jet_csv);
  reduced_tree.Branch("jet_corr_def", &jet_corr_def);
  reduced_tree.Branch("jet_corr_new", &jet_corr_new);
  // reduced_tree.Branch("jet_l1_corr", &jet_l1_corr);
  // reduced_tree.Branch("jet_l2_corr", &jet_l2_corr);
  // reduced_tree.Branch("jet_l3_corr", &jet_l3_corr);
  // reduced_tree.Branch("jet_looseID", &jet_looseID);
  reduced_tree.Branch("jet_delta_phi_met", &jet_delta_phi_met);
  reduced_tree.Branch("jet_delta_phi_mht", &jet_delta_phi_mht);
  // reduced_tree.Branch("jet_delta_T", &jet_delta_T);
  reduced_tree.Branch("jet_mT", &jet_mT);
  reduced_tree.Branch("jet_parton_id", &jet_parton_id);
  //  reduced_tree.Branch("jet_parton_mother_id", &jet_parton_mother_id);
  reduced_tree.Branch("jet_mdp_jet", &jet_mdp_jet);
  reduced_tree.Branch("jet_mdR_jet", &jet_mdR_jet);
  // reduced_tree.Branch("jet_gen_match", &jet_gen_match);
  reduced_tree.Branch("jet_gen_pt", &jet_gen_pt);
  // reduced_tree.Branch("jet_gen_E", &jet_gen_E);
  // reduced_tree.Branch("jet_gen_emE", &jet_gen_emE);
  // reduced_tree.Branch("jet_gen_hadE", &jet_gen_hadE);
  // reduced_tree.Branch("jet_gen_invE", &jet_gen_invE);
  // reduced_tree.Branch("jet_blep_tru", &jet_blep_tru);
  // reduced_tree.Branch("jet_muF", &jet_muF);
  // reduced_tree.Branch("jet_mu_mult", &jet_mu_mult);
  // reduced_tree.Branch("jet_chgEmF", &jet_chgEmF);
  // reduced_tree.Branch("jet_nEmF", &jet_nEmF);
  // reduced_tree.Branch("jet_chgHadF", &jet_chgHadF);
  // reduced_tree.Branch("jet_nHadF", &jet_nHadF);
  // reduced_tree.Branch("jet_nu_mult", &jet_nu_mult);
  // reduced_tree.Branch("jet_nu_mht", &jet_nu_mht);
  // reduced_tree.Branch("jet_nu_id", &jet_nu_id);
  // reduced_tree.Branch("jet_nu_mom_id", &jet_nu_mom_id);
  reduced_tree.Branch("mmjet", &mmjet);
  reduced_tree.Branch("wjetmm", &wjetmm);
  reduced_tree.Branch("omjet", &omjet);
  reduced_tree.Branch("wjetom", &wjetom);
  reduced_tree.Branch("umjet", &umjet);
  reduced_tree.Branch("wjetum", &wjetum);
  
  reduced_tree.Branch("jet1_pt", &jet1_pt);
  reduced_tree.Branch("jet2_pt", &jet2_pt);
  reduced_tree.Branch("jet3_pt", &jet3_pt);
  reduced_tree.Branch("jet4_pt", &jet4_pt);
  reduced_tree.Branch("jet5_pt", &jet5_pt);
  reduced_tree.Branch("jet6_pt", &jet6_pt);
 
  reduced_tree.Branch("def_met", &def_met);
  reduced_tree.Branch("def_met_phi", &def_met_phi);
  reduced_tree.Branch("met", &met);
  reduced_tree.Branch("met_phi", &met_phi);
  if (cfAVersion>=78) {
    reduced_tree.Branch("raw_met", &raw_met_et);
    reduced_tree.Branch("raw_met_phi", &raw_met_phi);
  }
  reduced_tree.Branch("def_mht", &def_mht);
  reduced_tree.Branch("def_mht_phi", &def_mht_phi);
  reduced_tree.Branch("mht30", &mht30);
  reduced_tree.Branch("mht30_phi", &mht30_phi);
  reduced_tree.Branch("mht50", &mht50);
  reduced_tree.Branch("mht50_phi", &mht50_phi);
  reduced_tree.Branch("mht30_raw", &mht30_raw);
  reduced_tree.Branch("mht30_phi_raw", &mht30_phi_raw);
  reduced_tree.Branch("mht50_raw", &mht50_raw);
  reduced_tree.Branch("mht50_phi_raw", &mht50_phi_raw);

  reduced_tree.Branch("mht30_central", &mht30_central);
  reduced_tree.Branch("mht30_central_phi", &mht30_central_phi);
  reduced_tree.Branch("mht30_central_raw", &mht30_central_raw);
  reduced_tree.Branch("mht30_central_raw_phi", &mht30_central_raw_phi);
  // reduced_tree.Branch("mht30_corr", &mht30_corr);
  // reduced_tree.Branch("mht30_phi_corr", &mht30_phi_corr);
  // reduced_tree.Branch("mht50_corr", &mht50_corr);
  // reduced_tree.Branch("mht50_phi_corr", &mht50_phi_corr);
  reduced_tree.Branch("mht_over_sqrt_ht30", &mht_over_sqrt_ht30);
  reduced_tree.Branch("mht_over_sqrt_ht50", &mht_over_sqrt_ht50);


  reduced_tree.Branch("ht20", &ht20);
  reduced_tree.Branch("ht30", &ht30);
  reduced_tree.Branch("ht30_forward", &ht30_forward);
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

  reduced_tree.Branch("num_veto_mus", &num_veto_mus);
  reduced_tree.Branch("num_mus", &num_mus);
  reduced_tree.Branch("num_low_pt_mus", &num_low_pt_mus);
  reduced_tree.Branch("num_high_pt_mus", &num_high_pt_mus);
  if (cfAVersion>=73) {
    reduced_tree.Branch("mu_tm", &mu_tm);
    reduced_tree.Branch("mu_truid", &mu_truid);
    reduced_tree.Branch("mu_momid", &mu_momid);
    reduced_tree.Branch("mu_signal", &mu_signal);
    reduced_tree.Branch("mu_veto", &mu_veto);
    reduced_tree.Branch("mu_vid", &mu_vid);
    reduced_tree.Branch("mu_pt", &mu_pt);
    reduced_tree.Branch("mu_eta", &mu_eta);
    reduced_tree.Branch("mu_phi", &mu_phi);
    reduced_tree.Branch("mu_dB_iso_R04", &mu_dB_iso_R04);
    reduced_tree.Branch("mu_mini_iso_tr02", &mu_mini_iso_tr02);
    reduced_tree.Branch("mu_ch_iso", &mu_ch_iso);
    reduced_tree.Branch("mu_mT", &mu_mT);
    reduced_tree.Branch("mu_ptW", &mu_ptW);
    reduced_tree.Branch("mu_dTT", &mu_dTT);
    reduced_tree.Branch("mu_ptW_MHT", &mu_ptW_MHT);
    reduced_tree.Branch("mu_dTT_MHT", &mu_dTT_MHT);
  }

  reduced_tree.Branch("num_veto_els", &num_veto_els);
  reduced_tree.Branch("num_els", &num_els);
  reduced_tree.Branch("num_low_pt_els", &num_low_pt_els);
  reduced_tree.Branch("num_high_pt_els", &num_high_pt_els);
  if (cfAVersion>=73) {
    reduced_tree.Branch("el_tm", &el_tm);
    reduced_tree.Branch("el_truid", &el_truid);
    reduced_tree.Branch("el_momid", &el_momid);
    reduced_tree.Branch("el_signal", &el_signal);
    reduced_tree.Branch("el_veto", &el_veto);
    reduced_tree.Branch("el_sid", &el_sid);
    reduced_tree.Branch("el_vid", &el_vid);
    reduced_tree.Branch("el_pt", &el_pt);
    reduced_tree.Branch("el_eta", &el_eta);
    reduced_tree.Branch("el_phi", &el_phi);
    reduced_tree.Branch("el_dB_iso_R03", &el_dB_iso_R03);
    reduced_tree.Branch("el_mini_iso_tr02", &el_mini_iso_tr02);
    reduced_tree.Branch("el_ch_iso", &el_ch_iso);
    reduced_tree.Branch("el_mT", &el_mT);
    reduced_tree.Branch("el_ptW", &el_ptW);
    reduced_tree.Branch("el_dTT", &el_dTT);
    reduced_tree.Branch("el_ptW_MHT", &el_ptW_MHT);
    reduced_tree.Branch("el_dTT_MHT", &el_dTT_MHT);
  }
  reduced_tree.Branch("min_delta_phi_met_N", &min_delta_phi_met_N);
  reduced_tree.Branch("min_delta_phi_met_N_old", &min_delta_phi_met_N_old);
  reduced_tree.Branch("min_delta_phi_met_N_2012", &min_delta_phi_met_N_2012);
  reduced_tree.Branch("min_delta_phi_met_N_loose_jets", &min_delta_phi_met_N_loose_jets);
  reduced_tree.Branch("min_delta_phi_met", &min_delta_phi_met);
  reduced_tree.Branch("min_delta_phi_met_loose_jets", &min_delta_phi_met_loose_jets);
  reduced_tree.Branch("min_delta_phi_met_2jets", &min_delta_phi_met_2jets);
  reduced_tree.Branch("min_delta_phi_met_4jets", &min_delta_phi_met_4jets);
  reduced_tree.Branch("min_delta_phi_met_alljets", &min_delta_phi_met_alljets);
  reduced_tree.Branch("min_delta_phi_met_N_2jets", &min_delta_phi_met_N_2jets);
  reduced_tree.Branch("min_delta_phi_met_N_4jets", &min_delta_phi_met_N_4jets);
  reduced_tree.Branch("min_delta_phi_met_N_alljets", &min_delta_phi_met_N_alljets);
  reduced_tree.Branch("min_delta_phi_mht_N", &min_delta_phi_mht_N);
  reduced_tree.Branch("min_delta_phi_mht", &min_delta_phi_mht);

  reduced_tree.Branch("mT_mu", &mT_mu);
  reduced_tree.Branch("mT_el", &mT_el);
  reduced_tree.Branch("deltaThetaT", &deltaThetaT);
  reduced_tree.Branch("W_pT", &W_pT);

  reduced_tree.Branch("SL_control_sample", &SL_control_sample);
  reduced_tree.Branch("QCD_control_sample", &QCD_control_sample);


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

  reduced_tree.Branch("sum_skinny_jet_mass", &sum_skinny_jet_mass);

  reduced_tree.Branch("min_mTWB", &min_mTWB);
  reduced_tree.Branch("min_mTWB_Wmass", &min_mTWB_Wmass);
  reduced_tree.Branch("mTWB_2nd", &mTWB_2nd);
  reduced_tree.Branch("min_delta_phi_b_met", &min_delta_phi_b_met);

  reduced_tree.Branch("doc_met", &doc_met);
  reduced_tree.Branch("gluino1_pt", &gluino1_pt);
  reduced_tree.Branch("gluino2_pt", &gluino2_pt);

  reduced_tree.Branch("num_veto_taus_mT", &num_veto_taus_mT);
  reduced_tree.Branch("tau_loose", &tau_loose);
  reduced_tree.Branch("tau_medium", &tau_medium);
  reduced_tree.Branch("tau_tight", &tau_tight);
  reduced_tree.Branch("tau_pt", &tau_pt);
  reduced_tree.Branch("tau_dB_iso", &tau_dB_iso);
  reduced_tree.Branch("tau_chargedIsoPtSum", &tau_chargedIsoPtSum);
  reduced_tree.Branch("tau_mT", &tau_mT);

  reduced_tree.Branch("num_iso_tracks_pt10", &num_iso_tracks_pt10);
  reduced_tree.Branch("num_iso_tracks_pt10_mT", &num_iso_tracks_pt10_mT);
  reduced_tree.Branch("num_iso_tracks_pt15", &num_iso_tracks_pt15);
  reduced_tree.Branch("num_iso_tracks_pt15_mT", &num_iso_tracks_pt15_mT);

  // reduced_tree.Branch("num_el_tracks", &num_el_tracks);
  // reduced_tree.Branch("num_mu_tracks", &num_mu_tracks);
  // reduced_tree.Branch("num_had_tracks", &num_had_tracks);
  reduced_tree.Branch("num_el_tracks_mT", &num_el_tracks_mT);
  reduced_tree.Branch("num_mu_tracks_mT", &num_mu_tracks_mT);
  reduced_tree.Branch("num_had_tracks_mT", &num_had_tracks_mT);
  // reduced_tree.Branch("num_had_tracks_no_iso", &num_had_tracks_no_iso);
  // reduced_tree.Branch("num_had_tracks_mT_no_iso", &num_had_tracks_mT_no_iso);

  reduced_tree.Branch("iso_track_pt", &iso_track_pt);
  reduced_tree.Branch("iso_track_dB_iso", &iso_track_dB_iso);
  reduced_tree.Branch("iso_track_mT", &iso_track_mT);

  reduced_tree.Branch("el_track_tm", &el_track_tm);
  // reduced_tree.Branch("el_track_truid", &el_track_truid);
  // reduced_tree.Branch("el_track_momid", &el_track_momid);
  reduced_tree.Branch("el_track_pt", &el_track_pt);
  reduced_tree.Branch("el_track_ch_iso", &el_track_ch_iso);
  // reduced_tree.Branch("el_track_dB_iso", &el_track_dB_iso);
  //  reduced_tree.Branch("el_track_mini_iso", &el_track_mini_iso);
  reduced_tree.Branch("el_track_mT", &el_track_mT);
  reduced_tree.Branch("mu_track_tm", &mu_track_tm);
  //  reduced_tree.Branch("mu_track_truid", &mu_track_truid);
  //  reduced_tree.Branch("mu_track_momid", &mu_track_momid);
  reduced_tree.Branch("mu_track_pt", &mu_track_pt);
  reduced_tree.Branch("mu_track_ch_iso", &mu_track_ch_iso);
  // reduced_tree.Branch("mu_track_dB_iso", &mu_track_dB_iso);
  //  reduced_tree.Branch("mu_track_mini_iso", &mu_track_mini_iso);
  reduced_tree.Branch("mu_track_mT", &mu_track_mT);
  // reduced_tree.Branch("had_track_from_tau", &had_track_from_tau);
  // reduced_tree.Branch("had_track_truid", &had_track_truid);
  reduced_tree.Branch("had_track_pt", &had_track_pt);
  reduced_tree.Branch("had_track_ch_iso", &had_track_ch_iso);
  // reduced_tree.Branch("had_track_dB_iso", &had_track_dB_iso);
  //  reduced_tree.Branch("had_track_mini_iso", &had_track_mini_iso);
  reduced_tree.Branch("had_track_mT", &had_track_mT);
  //  cout << "True leptons" << endl;
  reduced_tree.Branch("num_true_els", &num_true_els);
  reduced_tree.Branch("true_el_pt", &true_el_pt);
  reduced_tree.Branch("true_el_eta", &true_el_eta);
  reduced_tree.Branch("true_el_min_parton_dR", &true_el_min_parton_dR);
  reduced_tree.Branch("true_el_reco", &true_el_reco);
  reduced_tree.Branch("true_el_passID", &true_el_passID);
  reduced_tree.Branch("true_el_mini_iso", &true_el_mini_iso);
  reduced_tree.Branch("true_el_d0", &true_el_d0);
  reduced_tree.Branch("true_el_ptW", &true_el_ptW);
  reduced_tree.Branch("true_el_dTT", &true_el_dTT);
  reduced_tree.Branch("true_el_ptW_MHT", &true_el_ptW_MHT);
  reduced_tree.Branch("true_el_dTT_MHT", &true_el_dTT_MHT);

  reduced_tree.Branch("num_true_mus", &num_true_mus);
  reduced_tree.Branch("true_mu_pt", &true_mu_pt);
  reduced_tree.Branch("true_mu_eta", &true_mu_eta);
  reduced_tree.Branch("true_mu_min_parton_dR", &true_mu_min_parton_dR);
  reduced_tree.Branch("true_mu_reco", &true_mu_reco);
  reduced_tree.Branch("true_mu_passID", &true_mu_passID);
  reduced_tree.Branch("true_mu_mini_iso", &true_mu_mini_iso);
  reduced_tree.Branch("true_mu_d0", &true_mu_d0);
  reduced_tree.Branch("true_mu_ptW", &true_mu_ptW);
  reduced_tree.Branch("true_mu_dTT", &true_mu_dTT);
  reduced_tree.Branch("true_mu_ptW_MHT", &true_mu_ptW_MHT);
  reduced_tree.Branch("true_mu_dTT_MHT", &true_mu_dTT_MHT);
  
  reduced_tree.Branch("num_true_had_taus", &num_true_had_taus);
  reduced_tree.Branch("true_had_tau_pt", &true_had_tau_pt);
  reduced_tree.Branch("true_had_tau_eta", &true_had_tau_eta);
  reduced_tree.Branch("true_had_tau_min_parton_dR", &true_had_tau_min_parton_dR);
  reduced_tree.Branch("true_had_tau_ptW", &true_had_tau_ptW);
  reduced_tree.Branch("true_had_tau_dTT", &true_had_tau_dTT);
  reduced_tree.Branch("true_had_tau_ptW_MHT", &true_had_tau_ptW_MHT);
  reduced_tree.Branch("true_had_tau_dTT_MHT", &true_had_tau_dTT_MHT);
  
  reduced_tree.Branch("num_true_lep_taus", &num_true_lep_taus);

  reduced_tree.Branch("genW_pt", &genW_pt);
  reduced_tree.Branch("genW_lep", &genW_lep);

  reduced_tree.Branch("num_photons_pt100", &num_photons_pt100);
  reduced_tree.Branch("num_photons_pt100_old", &num_photons_pt100_old);
  reduced_tree.Branch("ph_pt", &ph_pt);
  reduced_tree.Branch("ph_eta", &ph_eta);
  reduced_tree.Branch("ph_id", &ph_id);
  reduced_tree.Branch("ph_ch_iso", &ph_ch_iso);
  reduced_tree.Branch("ph_nh_iso", &ph_nh_iso);
  reduced_tree.Branch("ph_ph_iso", &ph_ph_iso);
  reduced_tree.Branch("ph_ch_iso_cut", &ph_ch_iso_cut);
  reduced_tree.Branch("ph_nh_iso_cut", &ph_nh_iso_cut);
  reduced_tree.Branch("ph_ph_iso_cut", &ph_ph_iso_cut);
  reduced_tree.Branch("ph_mht", &ph_mht);
  // reduced_tree.Branch("ph_mht_old", &ph_mht_old);

  

  cout << "Let's go!" << endl;

  // FILE* fp = fopen(  "SMStttt1500_jack.txt", "wt" ) ;
  

  int n_to_process(Nentries);
  if(n_to_process<0) n_to_process=GetTotalEntries();
  if (is_skimmed) n_to_process = cfA::GetChainB()->GetEntries();
  cout << "to process: " << n_to_process << endl;
  Timer timer(n_to_process);
  timer.Start();
  for(int i(0); i<n_to_process; ++i){
    if((i%10000==0||i==1||i==10||i==100||i==1000) && i!=0){
      timer.PrintRemainingTime();
    }
    timer.Iterate();
    //     cout << "JERR" << endl;
    GetEntry(i);
    entry=i;
   
    // if (i<6066) continue;
    // if (!(event==5467)) continue;
    // cout << "*****Entry " << i << " (event " << event << ")*****" << endl;
    

    std::pair<std::set<EventNumber>::iterator, bool> returnVal(eventList.insert(EventNumber(run, event, lumiblock)));
    if(!returnVal.second) continue;

    // vector<mc_particle> parts = GetMCParticles();
    // vector<size_t> moms = GetMoms(parts);

    type_code=TypeCode();
    //cout << "JERR1" << endl;


    mht30=GetMHT(30.,5.);

    // if (cfAVersion==75&&mht30<200) continue; // careful!!!
    mht30_phi=GetMHTPhi(30.,5.);
    mht50=GetMHT(50.,5.);
    mht50_phi=GetMHTPhi(50.,5.);
    mht30_raw=GetRawMHT(30.,5.);
    mht30_phi_raw=GetRawMHTPhi(30.,5.);
    mht50_raw=GetRawMHT(50.,5.);
    mht50_phi_raw=GetRawMHTPhi(50.,5.);

    mht30_central=GetMHT(30.,2.4);
    mht30_central_phi=GetMHTPhi(30.,2.4);
    mht30_central_raw=GetRawMHT(30.,2.4);
    mht30_central_raw_phi=GetRawMHTPhi(30.,2.4);

    def_met=pfTypeImets_et->at(0);
    def_met_phi=pfTypeImets_phi->at(0);

    met=theMET_;
    met_phi=theMETPhi_;


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

    //cout << "GetJets" << endl;
    highest_csv=GetHighestJetCSV(1);
    second_highest_csv=GetHighestJetCSV(2);
    third_highest_csv=GetHighestJetCSV(3);
    fourth_highest_csv=GetHighestJetCSV(4);
    fifth_highest_csv=GetHighestJetCSV(5);
    sixth_highest_csv=GetHighestJetCSV(6);

    jet1_pt=GetHighestJetPt(1);
    jet2_pt=GetHighestJetPt(2);
    jet3_pt=GetHighestJetPt(3);
    jet4_pt=GetHighestJetPt(4);
    jet5_pt=GetHighestJetPt(5);
    jet6_pt=GetHighestJetPt(6);



    def_mht = GetDEFMHT(30.,5.);
    def_mht_phi = GetDEFMHTPhi(30.,5.);

    //    cout << "GetHT" << endl;
    ht20=GetHT(20.);
    ht30=GetHT(30.);
    ht40=GetHT(40.);
    ht50=GetHT(50.);
    ht60=GetHT(60.);
    ht70=GetHT(70.);
    ht80=GetHT(80.);

    ht30_forward=GetHT(30.,2.4);

    
    if (ht30>0) mht_over_sqrt_ht30=mht30/TMath::Sqrt(ht30);
    else mht_over_sqrt_ht30=-1.;
    if (ht50>0) mht_over_sqrt_ht50=mht50/TMath::Sqrt(ht50);
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
    
    jet_pt.clear(); jet_eta.clear(); jet_phi.clear(); jet_energy.clear();
    jet_csv.clear(); jet_looseID.clear();
    jet_corr_def.clear(); jet_corr_new.clear();
    jet_l1_corr.clear(); jet_l2_corr.clear(); jet_l3_corr.clear();
    jet_delta_phi_met.clear(); jet_delta_phi_mht.clear(); jet_delta_T.clear(); jet_mT.clear();
    jet_parton_id.clear(); jet_parton_mother_id.clear();
    jet_mdp_jet.clear(); jet_mdR_jet.clear();
    // jet_gen_match.clear();
    jet_gen_pt.clear(); jet_gen_emE.clear(); jet_gen_hadE.clear(); jet_gen_invE.clear(); jet_gen_E.clear();
    jet_blep_tru.clear();
    jet_muF.clear(); jet_mu_mult.clear();
    jet_chgEmF.clear(); jet_nEmF.clear(); jet_chgHadF.clear(); jet_nHadF.clear();
    jet_nu_mult.clear(); jet_nu_mht.clear(); jet_nu_id.clear(); jet_nu_mom_id.clear();
    num_truth_matched_b_jets=0;
    num_good_truth_matched_b_jets=0;
    //     cout << "Get jets" << endl;
    // for (uint ijet(0); ijet<jets_AKPF_pt->size(); ijet++) {
    //   if (jets_AKPF_pt->at(ijet)>30&&fabs(jets_AKPF_eta->at(ijet))<2.4) 
    // 	printf("Jet %d: pt/eta/ID = %f/%f/%d\n", ijet, corrJets[ijet].GetPt(theJESType_), corrJets[ijet].GetTLV(theJESType_).Eta(), jetPassLooseID(ijet));
    // }
    if (cfAVersion>=77) {
      std::vector<int> jets = GetJets(true, 30., 2.4);
      // jets[ijet] should now be index in corrJets
      //    cout << "Found " << jets.size() << " jets" << endl;
      for (uint ijet(0); ijet<jets.size(); ijet++) {
	// cout << "Jet " << ijet << "(" << jets[ijet] << ")" << endl;
	jet_pt.push_back(corrJets[jets[ijet]].GetPt(theJESType_));
	jet_eta.push_back(corrJets[jets[ijet]].GetTLV(theJESType_).Eta());
	jet_phi.push_back(corrJets[jets[ijet]].GetTLV(theJESType_).Phi());
	jet_energy.push_back(corrJets[jets[ijet]].GetTLV(theJESType_).E());
	// printf("Jet %d: pt/eta/phi/E = %f/%f/%f/%f\n", jets[ijet], jet_pt[ijet], jet_eta[ijet], jet_phi[ijet], jet_energy[ijet]);
	//      cout << "Get corrections..." << endl;
	jet_corr_def.push_back(corrJets[jets[ijet]].GetCorr(DEF));
	jet_corr_new.push_back(corrJets[jets[ijet]].GetCorr(CORR));
	// jet_l1_corr.push_back(corrJets[jets[ijet]].GetSubCorr(1));
	// jet_l2_corr.push_back(corrJets[jets[ijet]].GetSubCorr(2));
	// jet_l3_corr.push_back(corrJets[jets[ijet]].GetSubCorr(3));
	jet_csv.push_back(corrJets[jets[ijet]].GetBTag());
	jet_looseID.push_back(jetPassLooseID(jets[ijet]));
	jet_delta_phi_met.push_back(Math::GetDeltaPhi(corrJets[jets[ijet]].GetTLV(theJESType_).Phi(),met_phi));
	jet_delta_phi_mht.push_back(Math::GetDeltaPhi(corrJets[jets[ijet]].GetTLV(theJESType_).Phi(),mht30_phi));
	// jet_delta_T.push_back(getDeltaPhiMETN_deltaT(jets[ijet]));
	int mdR_jet(GetClosestRecoJet(jets[ijet], true)), mdp_jet(GetClosestRecoJet(jets[ijet], false));
	if (mdR_jet>=0) jet_mdR_jet.push_back(Math::dR(corrJets[mdR_jet].GetTLV(theJESType_).Eta(), jet_eta[ijet], corrJets[mdR_jet].GetTLV(theJESType_).Phi(), jet_phi[ijet]));
	else jet_mdR_jet.push_back(-999.);
	if (mdp_jet>=0) jet_mdp_jet.push_back(Math::GetDeltaPhi(jet_phi[ijet], corrJets[mdp_jet].GetTLV(theJESType_).Phi()));
	else jet_mdp_jet.push_back(-999.);
	jet_mT.push_back(GetMTW(jet_pt[ijet], met, jet_phi[ijet], met_phi));
	jet_parton_id.push_back(TMath::Nint(jets_AKPF_partonFlavour->at(corrJets[jets[ijet]].GetIndex())));
	// jet_parton_mother_id.push_back(TMath::Nint(jets_AKPF_parton_motherId->at(corrJets[jets[ijet]].GetIndex())));
	// jet_gen_match.push_back(GetClosestGenJet(jets[ijet])>=0);
	//      cout << "Match to gen jet" << endl;
	jet_gen_pt.push_back(GetGenJetPt(jets[ijet]));
	// int gen_match = GetClosestGenJet(jets[ijet]);
	// if (gen_match>=0) {
	// 	jet_gen_E.push_back(mc_jets_energy->at(gen_match));
	// 	if (cfAVersion>=77) {
	// 	  jet_gen_emE.push_back(mc_jets_emEnergy->at(gen_match));
	// 	  jet_gen_hadE.push_back(mc_jets_hadEnergy->at(gen_match));
	// 	  jet_gen_invE.push_back(mc_jets_invisibleEnergy->at(gen_match));
	// 	}
	// } else {
	// 	jet_gen_E.push_back(-999.);
	// 	jet_gen_emE.push_back(-999.);
	// 	jet_gen_hadE.push_back(-999.);
	// 	jet_gen_invE.push_back(-999.);
	// }
	// jet_blep_tru.push_back(isBLepJet(jets[ijet]));
	// double jet_raw_energy = corrJets[jets[ijet]].GetTLV(RAW).E();
	// jet_muF.push_back(jets_AKPF_chgMuE->at(corrJets[jets[ijet]].GetIndex())/jet_raw_energy);
	// jet_mu_mult.push_back(TMath::Nint(jets_AKPF_mu_Mult->at(corrJets[jets[ijet]].GetIndex())));
	// jet_chgEmF.push_back(jets_AKPF_chgEmE->at(corrJets[jets[ijet]].GetIndex())/jet_raw_energy);
	// jet_nEmF.push_back(jets_AKPF_neutralEmE->at(corrJets[jets[ijet]].GetIndex())/jet_raw_energy);
	// jet_chgHadF.push_back(jets_AKPF_chgHadE->at(corrJets[jets[ijet]].GetIndex())/jet_raw_energy);
	// jet_nHadF.push_back(jets_AKPF_neutralHadE->at(corrJets[jets[ijet]].GetIndex())/jet_raw_energy);
	// //      cout << "Look for neutrinos in jet" << endl;
	// vector<int> nus = GetNeutrinosInJet(jets[ijet]);
	// jet_nu_mult.push_back(nus.size());
	// if (nus.size()>0) {
	// 	jet_nu_mht.push_back(GetVectorPtSum(nus));
	// 	int lead_nu = GetMaxPtNu(nus);
	// 	jet_nu_id.push_back(abs(TMath::Nint(mc_final_id->at(lead_nu))));
	// 	int mom_id = abs(TMath::Nint(mc_final_mother_id->at(lead_nu)));
	// 	if (mom_id==11||mom_id==13||mom_id==15) mom_id = abs(TMath::Nint(mc_final_grandmother_id->at(lead_nu)));
	// 	if (mom_id==11||mom_id==13||mom_id==15) mom_id = abs(TMath::Nint(mc_final_ggrandmother_id->at(lead_nu)));
	// 	jet_nu_mom_id.push_back(mom_id);
	// } else {
	// 	jet_nu_mht.push_back(0.);
	// 	jet_nu_id.push_back(0);
	// 	jet_nu_mom_id.push_back(0);
	// }
	if (abs(jet_parton_id[ijet])==5) {
	  num_truth_matched_b_jets++;
	  if (jet_pt[ijet]>30&&abs(jet_eta[ijet])<2.4&&jet_looseID[ijet]) num_good_truth_matched_b_jets++;
	}
      }

      // get worst mis-measured, over-measured, under-measured jets
      //    cout << "Find most mismeasured jets" << endl;
      mmjet=-1;
      omjet=-1;
      umjet=-1;
      wjetmm=0.;
      wjetom=0.;
      wjetum=0.;
      for (uint ijet(0); ijet<jets.size(); ijet++) {
	double jetmm = jet_pt[ijet]-jet_gen_pt[ijet];
	if (abs(jetmm)>=wjetmm){
	  wjetmm=jetmm;
	  mmjet=ijet;
	}
	if (jetmm>=wjetom) {
	  wjetom=jetmm;
	  omjet=ijet;
	}
	if (jetmm<=wjetum) {
	  wjetum=jetmm;
	  umjet=ijet;
	}
      }
    }
    //    cout << "JERR2" << endl;


    sumP30=GetSumP(30.);
    sumP50=GetSumP(50.);
    cent30=GetCentrality(30.);
    cent50=GetCentrality(50.);

    //    cout << "GetNumGoodJets" << endl;
    num_jets_pt30=GetNumGoodJets(30);
    //    if (num_jets_pt30!=num_jets_pt30_old) cout << "Event " << entry << "is funky" << endl;
    num_jets_pt20=GetNumGoodJets(20);
    num_jets_pt40=GetNumGoodJets(40);
    num_jets_pt50=GetNumGoodJets(50);
    num_jets_pt70=GetNumGoodJets(70);
    num_jets_pt100=GetNumGoodJets(100);
    num_jets_pt150=GetNumGoodJets(150);
    //    cout << "num_jets=" << num_jets_pt50 << endl;
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




    // num_true_partons=GetNGenPartons();
    // num_true_partons_pt20=GetNGenPartons(20.);
    // num_true_partons_pt40=GetNGenPartons(40.);
    // num_true_partons_pt70=GetNGenPartons(70.);
    // num_true_partons_pt100=GetNGenPartons(100.);
    // num_true_partons_pt150=GetNGenPartons(150.);




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


    //    cout << "Get leptons" << endl;

    mu_truid.clear(); mu_momid.clear(), mu_tm.clear(); 
    mu_signal.clear(); mu_veto.clear(); mu_vid.clear();
    mu_pt.clear(); mu_eta.clear(); mu_phi.clear();
    mu_dB_iso_R04.clear(); mu_mini_iso_tr02.clear(); mu_ch_iso.clear();
    mu_mT.clear();
    mu_ptW.clear(); mu_dTT.clear(); mu_ptW_MHT.clear(); mu_dTT_MHT.clear();

    vector<int> mus, veto_mus;
    if (cfAVersion>=73&&cfAVersion!=74) {
      mus = GetRecoMuons(0, false, false, true); // save to tree, cut on eta & id
      veto_mus = GetRecoMuons();
    }
    else {
      mus = GetRA2bMuons(false);
      veto_mus = GetRA2bMuons(true);
    }
    num_veto_mus = veto_mus.size();
    num_low_pt_mus=0;
    num_high_pt_mus=0;
    if (cfAVersion>=77) {
      for (uint imu(0); imu<mus.size(); imu++) {
	mu_signal.push_back(isRecoMuon(mus[imu],1));
	mu_veto.push_back(isRecoMuon(mus[imu],0));
	mu_vid.push_back(isRecoMuon(mus[imu],0,false,false));
	mu_pt.push_back(mus_pt->at(mus[imu]));
	mu_eta.push_back(mus_eta->at(mus[imu]));
	mu_phi.push_back(mus_phi->at(mus[imu])); 
	mu_dB_iso_R04.push_back(GetMuonRelIso(mus[imu],0.4));
	if (cfAVersion>=78) mu_mini_iso_tr02.push_back(mus_miniso->at(mus[imu]));
	else mu_mini_iso_tr02.push_back(GetIsolation(mus[imu], 13));
	if (cfAVersion>=77) mu_ch_iso.push_back(GetSimpleChargedTrackIsolation(mus[imu], 13, 0.3)/mu_pt[imu]);
	mu_mT.push_back(GetMTW(mu_pt[imu], met, mu_phi[imu], met_phi));
	if (mu_pt[imu]>20&&mu_vid[imu]&&mu_mini_iso_tr02[imu]<0.2) num_high_pt_mus++;
	if (mu_pt[imu]>5&&mu_pt[imu]<20&&mu_vid[imu]&&mu_ch_iso[imu]<0.2&&mu_mT[imu]<100) num_low_pt_mus++;
	mu_ptW.push_back(GetWpT(mu_pt[imu], mu_phi[imu]));
	mu_dTT.push_back(GetDeltaThetaT(mu_pt[imu], mu_phi[imu]));
	mu_ptW_MHT.push_back(GetWpT(mu_pt[imu], mu_phi[imu], true));
	mu_dTT_MHT.push_back(GetDeltaThetaT(mu_pt[imu], mu_phi[imu], true));
	//	printf("ptW/ht30/njets/RLF = %3.2f/%3.2f/%d/%3.2f\n", mu_ptW[imu], ht30, num_jets_pt30, mu_RLF[imu]);
	// truth-matching
	bool fromW(false);
	int mcID, mcmomID;
	float deltaR;
	mcID = GetTrueMuon(mu_pt[imu], mu_eta[imu], mu_phi[imu], mcmomID, fromW, deltaR);
	mu_truid.push_back(mcID);
	mu_momid.push_back(mcmomID);
	mu_tm.push_back(abs(mcID)==13 && fromW);
      }
    }

    el_truid.clear(); el_momid.clear(), el_tm.clear();
    el_signal.clear(); el_veto.clear(); el_sid.clear(); el_vid.clear();
    el_pt.clear(); el_eta.clear(); el_phi.clear();
    el_dB_iso_R03.clear(); el_mini_iso_tr02.clear(); el_ch_iso.clear();
    el_mT.clear(); el_ptW.clear(); el_dTT.clear(); el_ptW_MHT.clear(); el_dTT_MHT.clear();

    vector<int> els, veto_els;
    if (cfAVersion>=73) {
      els = GetRecoElectrons(0, false, false, true); // save to tree, cut on eta & ID
      //  els_mini = GetRecoElectrons(false, true, true, true);
      veto_els = GetRecoElectrons();
      //  veto_els_mini = GetRecoElectrons(true, true, true, true);
      // els_save = GetRecoElectrons(true, false, false);
    }
    else {
      els = GetRA2bElectrons(false);
      veto_els = GetRA2bElectrons(true);
    }

    num_veto_els = veto_els.size();
    num_low_pt_els=0;
    num_high_pt_els=0;
    
    if (cfAVersion>=77) {
      for (uint iel(0); iel<els.size(); iel++) {
	el_signal.push_back(isRecoElectron(els[iel],1));
	el_veto.push_back(isRecoElectron(els[iel],0));
	el_sid.push_back(isRecoElectron(els[iel],1, false, false));
	el_vid.push_back(isRecoElectron(els[iel],0, false, false));
	el_pt.push_back(els_pt->at(els[iel]));
	el_eta.push_back(els_scEta->at(els[iel]));
	el_phi.push_back(els_phi->at(els[iel]));
	el_dB_iso_R03.push_back(GetCSAElectronIsolation(els[iel]));
	if (cfAVersion>=78) el_mini_iso_tr02.push_back(els_miniso->at(els[iel]));
	else el_mini_iso_tr02.push_back(GetIsolation(els[iel], 11));
	if (cfAVersion>=77) el_ch_iso.push_back(GetSimpleChargedTrackIsolation(els[iel], 11, 0.3)/el_pt[iel]);
	el_mT.push_back(GetMTW(el_pt[iel],met,el_phi[iel],met_phi));
	if (el_pt[iel]>20&&el_vid[iel]&&el_mini_iso_tr02[iel]<0.1) num_high_pt_els++;
	if (el_pt[iel]>5&&el_pt[iel]<20&&el_vid[iel]&&el_ch_iso[iel]<0.2&&el_mT[iel]<100) num_low_pt_els++;
	el_ptW.push_back(GetWpT(el_pt[iel], el_phi[iel]));
	el_dTT.push_back(GetDeltaThetaT(el_pt[iel], el_phi[iel]));
	el_ptW_MHT.push_back(GetWpT(el_pt[iel], el_phi[iel], true));
	el_dTT_MHT.push_back(GetDeltaThetaT(el_pt[iel], el_phi[iel], true));
	// truth-matching
	bool fromW(false);
	int mcID, mcmomID;
	float deltaR;
	mcID = GetTrueElectron(el_pt[iel], el_eta[iel], el_phi[iel], mcmomID, fromW, deltaR);
	el_truid.push_back(mcID);
	el_momid.push_back(mcmomID);
	el_tm.push_back(abs(mcID)==11 && fromW);
      }
    }
   
    //    cout << "minDeltaPhi..." << endl;
    min_delta_phi_met=GetMinDeltaPhiMET(3,30.);
    min_delta_phi_met_loose_jets=GetMinDeltaPhiMET(3,20.,5.);
    min_delta_phi_met_N=getMinDeltaPhiMETN(3,30.,2.4,true,30.,2.4,true,true);
    min_delta_phi_met_N_old=getMinDeltaPhiMETN_Old(3,30.,2.4,true,30.,2.4,true,true);
    // if (min_delta_phi_met_N!=min_delta_phi_met_N_old) cout << "Event " << entry << " is funky" << endl;
    min_delta_phi_met_N_2012=getMinDeltaPhiMETN(3,50.,2.4,true,30.,2.4,true,true);
    min_delta_phi_met_N_loose_jets=getMinDeltaPhiMETN(3,20.,5.,true,20.,5.,false,true);

    min_delta_phi_met_2jets=GetMinDeltaPhiMET(2,30.);
    min_delta_phi_met_4jets=GetMinDeltaPhiMET(4,30.);
    min_delta_phi_met_alljets=GetMinDeltaPhiMET(1000,30.);

    min_delta_phi_met_N_2jets=getMinDeltaPhiMETN(2,30.,2.4,true,30.,2.4,true,true);
    min_delta_phi_met_N_4jets=getMinDeltaPhiMETN(4,30.,2.4,true,30.,2.4,true,true);
    min_delta_phi_met_N_alljets=getMinDeltaPhiMETN(1000,30.,2.4,true,30.,2.4,true,true);

    min_delta_phi_mht=GetMinDeltaPhiMET(3,30.,2.4,true);
    min_delta_phi_mht_N=getMinDeltaPhiMETN(3,30.,2.4,true,30.,2.4,true,true,true);

    
    num_iso_tracks_pt10=0;
    num_iso_tracks_pt10_mT=0;
    num_iso_tracks_pt15=0;
    num_iso_tracks_pt15_mT=0;
    // num_el_tracks=0;
    // num_mu_tracks=0;
    // num_had_tracks=0;
    num_el_tracks_mT=0;
    num_mu_tracks_mT=0;
    num_had_tracks_mT=0;
    // num_had_tracks_no_iso=0;
    // num_had_tracks_mT_no_iso=0;
    iso_track_pt=-1;
    iso_track_dB_iso=-1;
    iso_track_mT=-1;
    el_track_tm.clear();
    el_track_truid.clear();
    el_track_momid.clear();
    el_track_pt.clear();
    el_track_ch_iso.clear();
    //    el_track_mini_iso.clear();
    el_track_mT.clear();
    mu_track_tm.clear();
    mu_track_truid.clear();
    mu_track_momid.clear();   
    mu_track_pt.clear();
    mu_track_ch_iso.clear();
    //    mu_track_mini_iso.clear();
    mu_track_mT.clear();
    had_track_from_tau.clear();
    had_track_truid.clear();
    had_track_pt.clear();
    had_track_ch_iso.clear();
    //  had_track_mini_iso.clear();
    had_track_mT.clear();
    // had_track_from_tau=false;
    // had_track_truid=-999;
    // had_track_pt=-999.;
    // had_track_ch_iso=-999.;
    // had_track_mT=-999.;
    //  cout << "Look for tracks..." << endl;
    vector<std::pair<int, double> > el_tracks, mu_tracks, had_tracks;
    if (cfAVersion==71||cfAVersion==74||(cfAVersion>=76&&cfAVersion<=77)) {
      num_iso_tracks_pt10=GetNumIsoTracks(10, false);
      num_iso_tracks_pt10_mT=GetNumIsoTracks(10, true);
      // store index and relIso of tracks
      vector<pair<int, double> > isotracks = GetIsoTracks(15, false);
      num_iso_tracks_pt15=isotracks.size();
      if (num_iso_tracks_pt15>0) {
	iso_track_pt=isotk_pt->at(isotracks[0].first);
	iso_track_dB_iso=isotracks[0].second;
	//	iso_track_miniIso=GetIsolation(isotracks[0].first,0)/iso_track_pt;
	iso_track_mT=GetMTW(iso_track_pt, met, isotk_phi->at(isotracks[0].first), met_phi);
	for (uint itk(0); itk<isotracks.size(); itk++) {
	  if (GetMTW(isotk_pt->at(isotracks[itk].first), met, isotk_phi->at(isotracks[itk].first), met_phi)<100) num_iso_tracks_pt15_mT++;
	}
      }
    }
    if (cfAVersion>=76) {
      //	cout << "Preparing electron tracks..." << endl;
      NewGetIsoTracks(el_tracks, mu_tracks, had_tracks, true, false); // pass pt>5/10, ID
      for (uint iel(0); iel<el_tracks.size(); iel++) {
	float ch_iso=el_tracks[iel].second;
	if (ch_iso>0.5) continue;
	el_track_pt.push_back(pfcand_pt->at(el_tracks[iel].first));
	// double el_track_eta=pfcand_eta->at(el_tracks[iel].first);
	double el_track_phi=pfcand_phi->at(el_tracks[iel].first);
	el_track_mT.push_back(GetMTW(el_track_pt[iel], met, el_track_phi, met_phi));
	el_track_ch_iso.push_back(ch_iso);
	// count tracks
	if (el_track_pt[iel]>5&&el_track_mT[iel]<100&&el_track_ch_iso[iel]<0.2) num_el_tracks_mT++;
      }
      //	cout << endl << "Preparing muon tracks..." << endl;
      for (uint imu(0); imu<mu_tracks.size(); imu++) {
	float ch_iso=mu_tracks[imu].second;
	if (ch_iso>0.5) continue;
	mu_track_pt.push_back(pfcand_pt->at(mu_tracks[imu].first));
	// double mu_track_eta=pfcand_eta->at(mu_tracks[imu].first);
	double mu_track_phi=pfcand_phi->at(mu_tracks[imu].first);
	mu_track_mT.push_back(GetMTW(mu_track_pt[imu], met, mu_track_phi, met_phi));
	mu_track_ch_iso.push_back(ch_iso);
	// count tracks
	if (mu_track_pt[imu]>5&&mu_track_mT[imu]<100&&mu_track_ch_iso[imu]<0.2) num_mu_tracks_mT++;	
      }
      //	cout << endl << "Preparing hadronic tracks..." << endl;
      for (uint ihad(0); ihad<had_tracks.size(); ihad++) {
	float pt=pfcand_pt->at(had_tracks[ihad].first);
	float ch_iso=had_tracks[ihad].second;
	if (pt<10) continue;
	if (ch_iso>0.5) continue;
	had_track_pt.push_back(pt);
	had_track_ch_iso.push_back(ch_iso);
	double had_track_phi=pfcand_phi->at(had_tracks[ihad].first);
	double mT = GetMTW(pt, met, had_track_phi, met_phi);
	had_track_mT.push_back(mT);
	if (pt>10&&ch_iso<0.1 && mT<100) num_had_tracks_mT++;
      }
    }

 
    doc_met=GetDocMET();
    gluino1_pt=GetGluinoPt(1);
    gluino2_pt=GetGluinoPt(2);

    //    cout << "Cluster fat jets..." << endl;
    
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

    sum_skinny_jet_mass=GetSumSkinnyJetMass();
    

    min_mTWB=GetMinMTWb(30., 0.814, false);
    min_mTWB_Wmass=GetMinMTWb(30., 0.814, true);
    mTWB_2nd=Get2ndMTWb();

    min_delta_phi_b_met=GetMinDeltaPhibMET();

    tau_loose.clear();
    tau_medium.clear();
    tau_tight.clear();
    tau_pt.clear();
    tau_dB_iso.clear();
    tau_chargedIsoPtSum.clear();
    tau_mT.clear();
    //    cout << "JERR3" << endl;
   
    std::vector<int> taus_save = GetTaus(); // loose taus, pt >20, eta< 2.3
    num_veto_taus_mT = 0;
    if (cfAVersion>=77) {
      for (uint itau(0); itau<taus_save.size(); itau++) {
	tau_loose.push_back(taus_byLooseCombinedIsolationDeltaBetaCorr3Hits->at(taus_save[itau]));
	tau_medium.push_back(taus_byMediumCombinedIsolationDeltaBetaCorr3Hits->at(taus_save[itau]));
	tau_tight.push_back(taus_byTightCombinedIsolationDeltaBetaCorr3Hits->at(taus_save[itau]));
	tau_pt.push_back(taus_pt->at(taus_save[itau]));
	tau_dB_iso.push_back(taus_CombinedIsolationDeltaBetaCorrRaw3Hits->at(taus_save[itau]));
	tau_chargedIsoPtSum.push_back(taus_chargedIsoPtSum->at(taus_save[itau]));
	tau_mT.push_back(  GetMTW(taus_pt->at(taus_save[itau]), met, taus_phi->at(taus_save[itau]), met_phi)  );
	if (tau_mT[itau] < 100) num_veto_taus_mT++;
      }
    }
    
    

    //    cout << "Look for true leptons." << endl;
    std::vector<int> true_els;
    std::vector<int> true_mus;
    std::vector<int> true_had_taus;
    std::vector<int> true_lep_taus;
    GetTrueLeptons(true_els, true_mus, true_had_taus, true_lep_taus);
    //    cout << "Found " << true_els.size() << " prompt els." << endl;
    std::vector<int> matched_els = MatchElectrons(true_els);
    //std::vector<int> matched_el_tks = MatchTks(true_els, el_tracks);
    //    cout << "Found " << true_mus.size() << " prompt mus." << endl;
    std::vector<int> matched_mus = MatchMuons(true_mus);
    //std::vector<int> matched_mu_tks = MatchTks(true_mus, mu_tracks);

    num_true_els=true_els.size();
    num_true_mus=true_mus.size();


    true_el_pt.clear();
    true_el_eta.clear();
    true_el_min_parton_dR.clear();
    true_el_reco.clear();
    true_el_passID.clear();
    true_el_mini_iso.clear();
    true_el_d0.clear();
    true_el_ptW.clear();
    true_el_dTT.clear();
    true_el_ptW_MHT.clear();
    true_el_dTT_MHT.clear();
    if (cfAVersion>=77) {
      for (uint iel(0); iel<num_true_els; iel++) {
	true_el_pt.push_back(mc_doc_pt->at(true_els[iel]));
	true_el_eta.push_back(mc_doc_eta->at(true_els[iel]));
	true_el_min_parton_dR.push_back(GetDRToClosestParton(true_els[iel]));
	true_el_ptW.push_back(GetWpT(true_el_pt[iel], mc_doc_phi->at(true_els[iel])));
	true_el_dTT.push_back(GetDeltaThetaT(true_el_pt[iel], mc_doc_phi->at(true_els[iel])));
	true_el_ptW_MHT.push_back(GetWpT(true_el_pt[iel], mc_doc_phi->at(true_els[iel]), true));
	true_el_dTT_MHT.push_back(GetDeltaThetaT(true_el_pt[iel], mc_doc_phi->at(true_els[iel]), true));
	if (matched_els[iel]>=0)	{
	  true_el_reco.push_back(true);
	  true_el_passID.push_back(PassElectronID(matched_els[iel],0));
	  if (cfAVersion>=78) true_el_mini_iso.push_back(els_miniso->at(matched_els[iel]));
	  else true_el_mini_iso.push_back(GetIsolation(matched_els[iel], 11));
	  true_el_d0.push_back(GetElectronD0(matched_els[iel]));
	} else {
	  true_el_reco.push_back(false);
	  true_el_passID.push_back(false);
	  true_el_mini_iso.push_back(-999.);
	  true_el_d0.push_back(-999.);
	}
      }
      true_mu_pt.clear();
      true_mu_eta.clear();
      true_mu_min_parton_dR.clear();
      true_mu_reco.clear();
      true_mu_passID.clear();
      true_mu_mini_iso.clear();
      true_mu_d0.clear();
      true_mu_ptW.clear();
      true_mu_dTT.clear();
      true_mu_ptW_MHT.clear();
      true_mu_dTT_MHT.clear();
      for (uint imu(0); imu<num_true_mus; imu++) {
	true_mu_pt.push_back(mc_doc_pt->at(true_mus[imu]));
	true_mu_eta.push_back(mc_doc_eta->at(true_mus[imu]));
	true_mu_min_parton_dR.push_back(GetDRToClosestParton(true_mus[imu]));
	true_mu_ptW.push_back(GetWpT(true_mu_pt[imu], mc_doc_phi->at(true_mus[imu])));
	true_mu_dTT.push_back(GetDeltaThetaT(true_mu_pt[imu], mc_doc_phi->at(true_mus[imu])));
	true_mu_ptW_MHT.push_back(GetWpT(true_mu_pt[imu], mc_doc_phi->at(true_mus[imu]), true));
	true_mu_dTT_MHT.push_back(GetDeltaThetaT(true_mu_pt[imu], mc_doc_phi->at(true_mus[imu]), true));
	//	cout << "True muon " << imu << ": ";
	if (matched_mus[imu]>=0)	{
	  //	  cout << "matched to reco muon " << matched_mus[imu] << endl;
	  true_mu_reco.push_back(true);
	  true_mu_passID.push_back(PassMuonID(matched_mus[imu]));
	  if (cfAVersion>=78) true_mu_mini_iso.push_back(mus_miniso->at(matched_mus[imu]));
	  else true_mu_mini_iso.push_back(GetIsolation(matched_mus[imu], 13));
	  true_mu_d0.push_back(GetMuonD0(matched_mus[imu]));
	} else {
	  //	  cout << "not matched." << endl;
	  true_mu_reco.push_back(false);
	  true_mu_passID.push_back(false);
	  true_mu_mini_iso.push_back(-999.);
	  true_mu_d0.push_back(-999.);
	}
      }

      num_true_had_taus=true_had_taus.size();
      num_true_lep_taus=true_lep_taus.size();
      true_had_tau_pt.clear();
      true_had_tau_eta.clear();
      true_had_tau_min_parton_dR.clear();
      true_had_tau_ptW.clear();
      true_had_tau_dTT.clear();
      true_had_tau_ptW_MHT.clear();
      true_had_tau_dTT_MHT.clear();
      //if (num_true_had_taus>0) cout << "Found " << num_true_had_taus << " hadronic taus" << endl;
      for (uint itau(0); itau<num_true_had_taus; itau++) {
	true_had_tau_pt.push_back(mc_doc_pt->at(true_had_taus[itau]));
	true_had_tau_eta.push_back(mc_doc_eta->at(true_had_taus[itau]));
	true_had_tau_min_parton_dR.push_back(GetDRToClosestParton(true_had_taus[itau]));
	true_had_tau_ptW.push_back(GetWpT(true_had_tau_pt[itau], mc_doc_phi->at(true_had_taus[itau])));
	true_had_tau_dTT.push_back(GetDeltaThetaT(true_had_tau_pt[itau], mc_doc_phi->at(true_had_taus[itau])));
	true_had_tau_ptW_MHT.push_back(GetWpT(true_had_tau_pt[itau], mc_doc_phi->at(true_had_taus[itau]), true));
	true_had_tau_dTT_MHT.push_back(GetDeltaThetaT(true_had_tau_pt[itau], mc_doc_phi->at(true_had_taus[itau]), true));
      }
      //if (num_true_had_taus>0) cout << "Saved " << true_had_tau_pt.size() << " hadronic taus" << endl;
      genW_pt.clear();
      genW_lep.clear();
      vector<int> Ws = GetGenWs();
      for (uint iW(0); iW<Ws.size(); iW++) {
	genW_pt.push_back(mc_doc_pt->at(Ws[iW]));
	genW_lep.push_back(DecaysToLeptons(Ws[iW]));
      }
    }
    
    //    cout << "JERR4" << endl;
    if (cfAVersion>=78) {
      // num_photons_pt100_old=GetNumPhotons(100,true);
      // num_photons_pt100=GetNumPhotons(100,false);
      num_photons_pt100=0;
      ph_pt.clear();
      ph_eta.clear();
      ph_ch_iso.clear();
      ph_nh_iso.clear();
      ph_ph_iso.clear();
      ph_ch_iso_cut.clear();
      ph_nh_iso_cut.clear();
      ph_ph_iso_cut.clear();
      // ph_id.clear();
      vector<int> photons = GetPhotons(100., true);
      for (uint iph(0); iph<photons.size(); iph++) {
	double pt = photons_pt->at(photons[iph]);
	ph_pt.push_back(pt);
	double eta = photons_eta->at(photons[iph]);
	ph_eta.push_back(eta);
	double ch_iso(GetPhotonIsolation(CH, photons_pf_ch_iso->at(photons[iph]), eta));
	double nh_iso(GetPhotonIsolation(NH, photons_pf_nh_iso->at(photons[iph]), eta));
	double ph_iso(GetPhotonIsolation(PH, photons_pf_ph_iso->at(photons[iph]), eta));
	ph_ch_iso.push_back(ch_iso);
	ph_nh_iso.push_back(nh_iso);
	ph_ph_iso.push_back(ph_iso);
	ph_ch_iso_cut.push_back((fabs(eta)<1.4442) ? 2.56 : 3.12);
	ph_nh_iso_cut.push_back((fabs(eta)<1.4442) ? 3.74 + 0.0025*pt : 17.11 + 0.0118*pt);
	ph_ph_iso_cut.push_back((fabs(eta)<1.4442) ? 2.68 + 0.001*pt : 2.7 + 0.0059*pt);
	// ph_id.push_back(PassPhotonID(photons[iph]));
	if (isGoodPhoton(photons[iph])) num_photons_pt100++; 
      }
      ph_mht=GetPhotonMHT(30,5.,100.,false);
    } // photons--v78 or later
    
    reduced_tree.Fill(); 
    // if (i==10) break;

    // if(num_jets_pt30>=4&&mht30>200&&ht30>500&&num_veto_mus==0&&num_veto_els==0&&min_delta_phi_met_N>4&&num_iso_tracks_pt15_mT==0) {
    //   fprintf(fp,"Run %d : Lumi %d : Evt %d :  NJets %d : HT   %3.0f : MHT %3.0f : Muons %d : ElectronsNum %d : minDelPhi %3.0f : isoTracks %d \n",
    // 	       run, lumiblock, event, num_jets_pt30, ht30, mht30, num_veto_mus, num_veto_els, min_delta_phi_met_N, num_iso_tracks_pt15_mT);
    //   }
    
  }
  reduced_tree.Write();
  file.Close();
}
