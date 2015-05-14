//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed May 13 11:10:02 2015 by ROOT version 5.34/25
// from TTree reduced_tree/reduced_tree
// found on file: ../../../skims/13TeV/reduced_trees/TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1_MINIAODSIM_UCSB2406_v78.root
//////////////////////////////////////////////////////////

#ifndef RTree_h
#define RTree_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <iostream>

using std::cout;
using std::endl;
using std::vector;

// Header file for the classes stored in the TTree if any.
#include <vector>

// Fixed size dimensions of array or collections stored in the TTree if any.

class RTree {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Float_t         weightppb;
   UInt_t          type_code;
   Short_t         gluino_mass;
   Short_t         lsp_mass;
   Bool_t          passes2012RA2bTrigger;
   Bool_t          passesJSONCut;
   Bool_t          passesPVCut;
   Bool_t          passes2012METCleaningCut;
   Bool_t          passesCSA14METCleaningCut;
   Int_t           trackingfailurefilter_decision;
   Int_t           cschalofilter_decision;
   Int_t           ecallaserfilter_decision;
   Int_t           HBHENoisefilter_decision;
   Int_t           hcallaserfilter_decision;
   Bool_t          passesBadJetFilter;
   Int_t           PBNRcode;
   Float_t         pu_weight;
   Float_t         top_pt_weight;
   Float_t         top_pt_weight_official;
   Float_t         trigger_eff_weight;
   Float_t         btag_weight;
   Float_t         btag_pid_weight;
   Double_t        Prob0;
   Double_t        ProbGEQ1;
   Double_t        Prob1;
   Double_t        ProbGEQ2;
   Double_t        Prob2;
   Double_t        ProbGEQ3;
   Double_t        Prob3;
   Double_t        ProbGEQ4;
   Double_t        Prob0_pt50;
   Double_t        ProbGEQ1_pt50;
   Double_t        Prob1_pt50;
   Double_t        ProbGEQ2_pt50;
   Double_t        Prob2_pt50;
   Double_t        ProbGEQ3_pt50;
   Double_t        Prob3_pt50;
   Double_t        ProbGEQ4_pt50;
   UInt_t          run;
   UInt_t          event;
   UInt_t          lumiblock;
   Int_t           entry;
   Bool_t          pass_HLT_PFHT350_PFMET100;
   Bool_t          pass_HLT_PFHT650;
   Bool_t          pass_HLT_DiCentralPFJet50_PFMET80;
   Bool_t          pass_HLT_DiCentralPFJet30_PFMET80_BTagCSV07;
   Float_t         pu_true_num_interactions;
   Float_t         pu_num_interactions;
   UShort_t        num_primary_vertices;
   Float_t         eoot_pu;
   Float_t         loot_pu;
   Float_t         oot_pu;
   Float_t         highest_csv;
   Float_t         second_highest_csv;
   Float_t         third_highest_csv;
   Float_t         fourth_highest_csv;
   Float_t         fifth_highest_csv;
   Float_t         sixth_highest_csv;
   vector<float>   *jet_pt;
   vector<float>   *jet_eta;
   vector<float>   *jet_phi;
   vector<float>   *jet_energy;
   vector<float>   *jet_csv;
   vector<float>   *jet_corr_def;
   vector<float>   *jet_corr_new;
   vector<float>   *jet_delta_phi_met;
   vector<float>   *jet_delta_phi_mht;
   vector<float>   *jet_mT;
   vector<int>     *jet_parton_id;
   vector<float>   *jet_mdp_jet;
   vector<float>   *jet_mdR_jet;
   vector<float>   *jet_gen_pt;
   Int_t           mmjet;
   Float_t         wjetmm;
   Int_t           omjet;
   Float_t         wjetom;
   Int_t           umjet;
   Float_t         wjetum;
   Float_t         jet1_pt;
   Float_t         jet2_pt;
   Float_t         jet3_pt;
   Float_t         jet4_pt;
   Float_t         jet5_pt;
   Float_t         jet6_pt;
   Float_t         def_met;
   Float_t         def_met_phi;
   Float_t         met;
   Float_t         met_phi;
   Float_t         raw_met;
   Float_t         raw_met_phi;
   Float_t         def_mht;
   Float_t         def_mht_phi;
   Float_t         mht30;
   Float_t         mht30_phi;
   Float_t         mht50;
   Float_t         mht50_phi;
   Float_t         mht30_raw;
   Float_t         mht30_phi_raw;
   Float_t         mht50_raw;
   Float_t         mht50_phi_raw;
   Float_t         mht30_central;
   Float_t         mht30_central_phi;
   Float_t         mht30_central_raw;
   Float_t         mht30_central_raw_phi;
   Float_t         mht_over_sqrt_ht30;
   Float_t         mht_over_sqrt_ht50;
   Float_t         ht20;
   Float_t         ht30;
   Float_t         ht30_forward;
   Float_t         ht40;
   Float_t         ht50;
   Float_t         ht60;
   Float_t         ht70;
   Float_t         ht80;
   Float_t         sumP30;
   Float_t         sumP50;
   Float_t         cent30;
   Float_t         cent50;
   Float_t         m_eff20;
   Float_t         m_eff30;
   Float_t         m_eff40;
   Float_t         m_eff50;
   Float_t         met_over_m_eff20;
   Float_t         met_over_m_eff30;
   Float_t         met_over_m_eff40;
   Float_t         met_over_m_eff50;
   Float_t         met_over_sqrt_ht20;
   Float_t         met_over_sqrt_ht30;
   Float_t         met_over_sqrt_ht40;
   Float_t         met_over_sqrt_ht50;
   Float_t         met_over_sqrt_ht80;
   UShort_t        num_jets_pt20;
   UShort_t        num_jets_pt30;
   UShort_t        num_jets_pt40;
   UShort_t        num_jets_pt50;
   UShort_t        num_jets_pt70;
   UShort_t        num_jets_pt100;
   UShort_t        num_jets_pt150;
   UShort_t        num_csvt_jets;
   UShort_t        num_csvm_jets;
   UShort_t        num_csvl_jets;
   UShort_t        num_csvt_jets20;
   UShort_t        num_csvm_jets20;
   UShort_t        num_csvl_jets20;
   UShort_t        num_csvt_jets30;
   UShort_t        num_csvm_jets30;
   UShort_t        num_csvl_jets30;
   UShort_t        num_csvt_jets40;
   UShort_t        num_csvm_jets40;
   UShort_t        num_csvl_jets40;
   UShort_t        num_truth_matched_b_jets;
   UShort_t        num_good_truth_matched_b_jets;
   Int_t           num_veto_mus;
   Int_t           num_mus;
   vector<bool>    *mu_tm;
   vector<int>     *mu_truid;
   vector<int>     *mu_momid;
   vector<bool>    *mu_signal;
   vector<bool>    *mu_veto;
   vector<bool>    *mu_vid;
   vector<float>   *mu_pt;
   vector<float>   *mu_eta;
   vector<float>   *mu_phi;
   vector<float>   *mu_dB_iso_R04;
   vector<float>   *mu_mini_iso_tr02;
   vector<float>   *mu_mT;
   vector<float>   *mu_ptW;
   vector<float>   *mu_dTT;
   Int_t           num_veto_els;
   Int_t           num_els;
   vector<bool>    *el_tm;
   vector<int>     *el_truid;
   vector<int>     *el_momid;
   vector<bool>    *el_signal;
   vector<bool>    *el_veto;
   vector<bool>    *el_sid;
   vector<bool>    *el_vid;
   vector<float>   *el_pt;
   vector<float>   *el_eta;
   vector<float>   *el_phi;
   vector<float>   *el_dB_iso_R03;
   vector<float>   *el_mini_iso_tr02;
   vector<float>   *el_mT;
   vector<float>   *el_ptW;
   vector<float>   *el_dTT;
   Float_t         min_delta_phi_met_N;
   Float_t         min_delta_phi_met_N_old;
   Float_t         min_delta_phi_met_N_2012;
   Float_t         min_delta_phi_met_N_loose_jets;
   Float_t         min_delta_phi_met;
   Float_t         min_delta_phi_met_loose_jets;
   Float_t         min_delta_phi_met_2jets;
   Float_t         min_delta_phi_met_4jets;
   Float_t         min_delta_phi_met_alljets;
   Float_t         min_delta_phi_met_N_2jets;
   Float_t         min_delta_phi_met_N_4jets;
   Float_t         min_delta_phi_met_N_alljets;
   Float_t         min_delta_phi_mht_N;
   Float_t         min_delta_phi_mht;
   Float_t         mT_mu;
   Float_t         mT_el;
   Float_t         deltaThetaT;
   Float_t         W_pT;
   Bool_t          SL_control_sample;
   Bool_t          QCD_control_sample;
   Double_t        fatpT30_jet1_pt;
   Double_t        fatpT30_jet2_pt;
   Double_t        fatpT30_jet3_pt;
   Double_t        fatpT30_jet4_pt;
   Double_t        fatpT30_jet1_eta;
   Double_t        fatpT30_jet2_eta;
   Double_t        fatpT30_jet3_eta;
   Double_t        fatpT30_jet4_eta;
   Double_t        fatpT30_jet1_phi;
   Double_t        fatpT30_jet2_phi;
   Double_t        fatpT30_jet3_phi;
   Double_t        fatpT30_jet4_phi;
   Double_t        fatpT30_jet1_energy;
   Double_t        fatpT30_jet2_energy;
   Double_t        fatpT30_jet3_energy;
   Double_t        fatpT30_jet4_energy;
   Double_t        fatpT30_jet1_mJ;
   Double_t        fatpT30_jet2_mJ;
   Double_t        fatpT30_jet3_mJ;
   Double_t        fatpT30_jet4_mJ;
   Int_t           fatpT30_jet1_nConst;
   Int_t           fatpT30_jet2_nConst;
   Int_t           fatpT30_jet3_nConst;
   Int_t           fatpT30_jet4_nConst;
   Int_t           num_fatpT30_jets;
   Int_t           num_fatpT30_jets_pt100;
   Int_t           num_fatpT30_jets_pt150;
   Int_t           num_fatpT30_jets_pt200;
   Int_t           num_fatpT30_jets_pt300;
   Double_t        fatpT30_MJ;
   Double_t        fatpT30_MJ_pt100;
   Double_t        fatpT30_MJ_pt150;
   Double_t        fatpT30_MJ_pt200;
   Double_t        fatpT30_MJ_pt300;
   Int_t           num_fatpT30central_jets;
   Int_t           num_fatpT30central_jets_pt100;
   Int_t           num_fatpT30central_jets_pt150;
   Int_t           num_fatpT30central_jets_pt200;
   Int_t           num_fatpT30central_jets_pt300;
   Double_t        fatpT30central_MJ;
   Double_t        fatpT30central_MJ_pt100;
   Double_t        fatpT30central_MJ_pt150;
   Double_t        fatpT30central_MJ_pt200;
   Double_t        fatpT30central_MJ_pt300;
   Double_t        highest_mJ;
   Double_t        scnd_highest_mJ;
   Double_t        thrd_highest_mJ;
   Double_t        frth_highest_mJ;
   Double_t        sum_skinny_jet_mass;
   Double_t        min_mTWB;
   Double_t        min_mTWB_Wmass;
   Double_t        mTWB_2nd;
   Double_t        min_delta_phi_b_met;
   Double_t        doc_met;
   Double_t        gluino1_pt;
   Double_t        gluino2_pt;
   Int_t           num_veto_taus_mT;
   vector<bool>    *tau_loose;
   vector<bool>    *tau_medium;
   vector<bool>    *tau_tight;
   vector<float>   *tau_pt;
   vector<float>   *tau_dB_iso;
   vector<float>   *tau_chargedIsoPtSum;
   vector<float>   *tau_mT;
   Int_t           num_iso_tracks_pt10;
   Int_t           num_iso_tracks_pt10_mT;
   Int_t           num_iso_tracks_pt15;
   Int_t           num_iso_tracks_pt15_mT;
   Int_t           num_el_tracks_mT;
   Int_t           num_mu_tracks_mT;
   Int_t           num_had_tracks_mT;
   Float_t         iso_track_pt;
   Float_t         iso_track_dB_iso;
   Float_t         iso_track_mT;
   vector<bool>    *el_track_tm;
   vector<float>   *el_track_pt;
   vector<float>   *el_track_ch_iso;
   vector<float>   *el_track_mT;
   vector<bool>    *mu_track_tm;
   vector<float>   *mu_track_pt;
   vector<float>   *mu_track_ch_iso;
   vector<float>   *mu_track_mT;
   vector<float>   *had_track_pt;
   vector<float>   *had_track_ch_iso;
   vector<float>   *had_track_mT;
   UShort_t        num_true_els;
   vector<float>   *true_el_pt;
   vector<float>   *true_el_eta;
   vector<float>   *true_el_min_parton_dR;
   vector<bool>    *true_el_reco;
   vector<bool>    *true_el_passID;
   vector<float>   *true_el_mini_iso;
   vector<float>   *true_el_d0;
   vector<float>   *true_el_ptW;
   vector<float>   *true_el_dTT;
   UShort_t        num_true_mus;
   vector<float>   *true_mu_pt;
   vector<float>   *true_mu_eta;
   vector<float>   *true_mu_min_parton_dR;
   vector<bool>    *true_mu_reco;
   vector<bool>    *true_mu_passID;
   vector<float>   *true_mu_mini_iso;
   vector<float>   *true_mu_d0;
   vector<float>   *true_mu_ptW;
   vector<float>   *true_mu_dTT;
   UShort_t        num_true_had_taus;
   vector<float>   *true_had_tau_pt;
   vector<float>   *true_had_tau_eta;
   vector<float>   *true_had_tau_min_parton_dR;
   vector<float>   *true_had_tau_ptW;
   vector<float>   *true_had_tau_dTT;
   UShort_t        num_true_lep_taus;
   UInt_t          num_photons_pt100;
   UInt_t          num_photons_pt100_old;
   vector<float>   *ph_pt;
   vector<float>   *ph_eta;
   vector<bool>    *ph_id;
   vector<float>   *ph_ch_iso;
   vector<float>   *ph_nh_iso;
   vector<float>   *ph_ph_iso;
   vector<float>   *ph_ch_iso_cut;
   vector<float>   *ph_nh_iso_cut;
   vector<float>   *ph_ph_iso_cut;
   Float_t         ph_mht;

   // List of branches
   TBranch        *b_weightppb;   //!
   TBranch        *b_type_code;   //!
   TBranch        *b_gluino_mass;   //!
   TBranch        *b_lsp_mass;   //!
   TBranch        *b_passes2012RA2bTrigger;   //!
   TBranch        *b_passesJSONCut;   //!
   TBranch        *b_passesPVCut;   //!
   TBranch        *b_passes2012METCleaningCut;   //!
   TBranch        *b_passesCSA14METCleaningCut;   //!
   TBranch        *b_trackingfailurefilter_decision;   //!
   TBranch        *b_cschalofilter_decision;   //!
   TBranch        *b_ecallaserfilter_decision;   //!
   TBranch        *b_HBHENoisefilter_decision;   //!
   TBranch        *b_hcallaserfilter_decision;   //!
   TBranch        *b_passesBadJetFilter;   //!
   TBranch        *b_PBNRcode;   //!
   TBranch        *b_pu_weight;   //!
   TBranch        *b_top_pt_weight;   //!
   TBranch        *b_top_pt_weight_official;   //!
   TBranch        *b_trigger_eff_weight;   //!
   TBranch        *b_btag_weight;   //!
   TBranch        *b_btag_pid_weight;   //!
   TBranch        *b_Prob0;   //!
   TBranch        *b_ProbGEQ1;   //!
   TBranch        *b_Prob1;   //!
   TBranch        *b_ProbGEQ2;   //!
   TBranch        *b_Prob2;   //!
   TBranch        *b_ProbGEQ3;   //!
   TBranch        *b_Prob3;   //!
   TBranch        *b_ProbGEQ4;   //!
   TBranch        *b_Prob0_pt50;   //!
   TBranch        *b_ProbGEQ1_pt50;   //!
   TBranch        *b_Prob1_pt50;   //!
   TBranch        *b_ProbGEQ2_pt50;   //!
   TBranch        *b_Prob2_pt50;   //!
   TBranch        *b_ProbGEQ3_pt50;   //!
   TBranch        *b_Prob3_pt50;   //!
   TBranch        *b_ProbGEQ4_pt50;   //!
   TBranch        *b_run;   //!
   TBranch        *b_event;   //!
   TBranch        *b_lumiblock;   //!
   TBranch        *b_entry;   //!
   TBranch        *b_pass_HLT_PFHT350_PFMET100;   //!
   TBranch        *b_pass_HLT_PFHT650;   //!
   TBranch        *b_pass_HLT_DiCentralPFJet50_PFMET80;   //!
   TBranch        *b_pass_HLT_DiCentralPFJet30_PFMET80_BTagCSV07;   //!
   TBranch        *b_pu_true_num_interactions;   //!
   TBranch        *b_pu_num_interactions;   //!
   TBranch        *b_num_primary_vertices;   //!
   TBranch        *b_eoot_pu;   //!
   TBranch        *b_loot_pu;   //!
   TBranch        *b_oot_pu;   //!
   TBranch        *b_highest_csv;   //!
   TBranch        *b_second_highest_csv;   //!
   TBranch        *b_third_highest_csv;   //!
   TBranch        *b_fourth_highest_csv;   //!
   TBranch        *b_fifth_highest_csv;   //!
   TBranch        *b_sixth_highest_csv;   //!
   TBranch        *b_jet_pt;   //!
   TBranch        *b_jet_eta;   //!
   TBranch        *b_jet_phi;   //!
   TBranch        *b_jet_energy;   //!
   TBranch        *b_jet_csv;   //!
   TBranch        *b_jet_corr_def;   //!
   TBranch        *b_jet_corr_new;   //!
   TBranch        *b_jet_delta_phi_met;   //!
   TBranch        *b_jet_delta_phi_mht;   //!
   TBranch        *b_jet_mT;   //!
   TBranch        *b_jet_parton_id;   //!
   TBranch        *b_jet_mdp_jet;   //!
   TBranch        *b_jet_mdR_jet;   //!
   TBranch        *b_jet_gen_pt;   //!
   TBranch        *b_mmjet;   //!
   TBranch        *b_wjetmm;   //!
   TBranch        *b_omjet;   //!
   TBranch        *b_wjetom;   //!
   TBranch        *b_umjet;   //!
   TBranch        *b_wjetum;   //!
   TBranch        *b_jet1_pt;   //!
   TBranch        *b_jet2_pt;   //!
   TBranch        *b_jet3_pt;   //!
   TBranch        *b_jet4_pt;   //!
   TBranch        *b_jet5_pt;   //!
   TBranch        *b_jet6_pt;   //!
   TBranch        *b_def_met;   //!
   TBranch        *b_def_met_phi;   //!
   TBranch        *b_met;   //!
   TBranch        *b_met_phi;   //!
   TBranch        *b_raw_met;   //!
   TBranch        *b_raw_met_phi;   //!
   TBranch        *b_def_mht;   //!
   TBranch        *b_def_mht_phi;   //!
   TBranch        *b_mht30;   //!
   TBranch        *b_mht30_phi;   //!
   TBranch        *b_mht50;   //!
   TBranch        *b_mht50_phi;   //!
   TBranch        *b_mht30_raw;   //!
   TBranch        *b_mht30_phi_raw;   //!
   TBranch        *b_mht50_raw;   //!
   TBranch        *b_mht50_phi_raw;   //!
   TBranch        *b_mht30_central;   //!
   TBranch        *b_mht30_central_phi;   //!
   TBranch        *b_mht30_central_raw;   //!
   TBranch        *b_mht30_central_raw_phi;   //!
   TBranch        *b_mht_over_sqrt_ht30;   //!
   TBranch        *b_mht_over_sqrt_ht50;   //!
   TBranch        *b_ht20;   //!
   TBranch        *b_ht30;   //!
   TBranch        *b_ht30_forward;   //!
   TBranch        *b_ht40;   //!
   TBranch        *b_ht50;   //!
   TBranch        *b_ht60;   //!
   TBranch        *b_ht70;   //!
   TBranch        *b_ht80;   //!
   TBranch        *b_sumP30;   //!
   TBranch        *b_sumP50;   //!
   TBranch        *b_cent30;   //!
   TBranch        *b_cent50;   //!
   TBranch        *b_m_eff20;   //!
   TBranch        *b_m_eff30;   //!
   TBranch        *b_m_eff40;   //!
   TBranch        *b_m_eff50;   //!
   TBranch        *b_met_over_m_eff20;   //!
   TBranch        *b_met_over_m_eff30;   //!
   TBranch        *b_met_over_m_eff40;   //!
   TBranch        *b_met_over_m_eff50;   //!
   TBranch        *b_met_over_sqrt_ht20;   //!
   TBranch        *b_met_over_sqrt_ht30;   //!
   TBranch        *b_met_over_sqrt_ht40;   //!
   TBranch        *b_met_over_sqrt_ht50;   //!
   TBranch        *b_met_over_sqrt_ht80;   //!
   TBranch        *b_num_jets_pt20;   //!
   TBranch        *b_num_jets_pt30;   //!
   TBranch        *b_num_jets_pt40;   //!
   TBranch        *b_num_jets_pt50;   //!
   TBranch        *b_num_jets_pt70;   //!
   TBranch        *b_num_jets_pt100;   //!
   TBranch        *b_num_jets_pt150;   //!
   TBranch        *b_num_csvt_jets;   //!
   TBranch        *b_num_csvm_jets;   //!
   TBranch        *b_num_csvl_jets;   //!
   TBranch        *b_num_csvt_jets20;   //!
   TBranch        *b_num_csvm_jets20;   //!
   TBranch        *b_num_csvl_jets20;   //!
   TBranch        *b_num_csvt_jets30;   //!
   TBranch        *b_num_csvm_jets30;   //!
   TBranch        *b_num_csvl_jets30;   //!
   TBranch        *b_num_csvt_jets40;   //!
   TBranch        *b_num_csvm_jets40;   //!
   TBranch        *b_num_csvl_jets40;   //!
   TBranch        *b_num_truth_matched_b_jets;   //!
   TBranch        *b_num_good_truth_matched_b_jets;   //!
   TBranch        *b_num_veto_mus;   //!
   TBranch        *b_num_mus;   //!
   TBranch        *b_mu_tm;   //!
   TBranch        *b_mu_truid;   //!
   TBranch        *b_mu_momid;   //!
   TBranch        *b_mu_signal;   //!
   TBranch        *b_mu_veto;   //!
   TBranch        *b_mu_vid;   //!
   TBranch        *b_mu_pt;   //!
   TBranch        *b_mu_eta;   //!
   TBranch        *b_mu_phi;   //!
   TBranch        *b_mu_dB_iso_R04;   //!
   TBranch        *b_mu_mini_iso_tr02;   //!
   TBranch        *b_mu_mT;   //!
   TBranch        *b_mu_ptW;   //!
   TBranch        *b_mu_dTT;   //!
   TBranch        *b_num_veto_els;   //!
   TBranch        *b_num_els;   //!
   TBranch        *b_el_tm;   //!
   TBranch        *b_el_truid;   //!
   TBranch        *b_el_momid;   //!
   TBranch        *b_el_signal;   //!
   TBranch        *b_el_veto;   //!
   TBranch        *b_el_sid;   //!
   TBranch        *b_el_vid;   //!
   TBranch        *b_el_pt;   //!
   TBranch        *b_el_eta;   //!
   TBranch        *b_el_phi;   //!
   TBranch        *b_el_dB_iso_R03;   //!
   TBranch        *b_el_mini_iso_tr02;   //!
   TBranch        *b_el_mT;   //!
   TBranch        *b_el_ptW;   //!
   TBranch        *b_el_dTT;   //!
   TBranch        *b_min_delta_phi_met_N;   //!
   TBranch        *b_min_delta_phi_met_N_old;   //!
   TBranch        *b_min_delta_phi_met_N_2012;   //!
   TBranch        *b_min_delta_phi_met_N_loose_jets;   //!
   TBranch        *b_min_delta_phi_met;   //!
   TBranch        *b_min_delta_phi_met_loose_jets;   //!
   TBranch        *b_min_delta_phi_met_2jets;   //!
   TBranch        *b_min_delta_phi_met_4jets;   //!
   TBranch        *b_min_delta_phi_met_alljets;   //!
   TBranch        *b_min_delta_phi_met_N_2jets;   //!
   TBranch        *b_min_delta_phi_met_N_4jets;   //!
   TBranch        *b_min_delta_phi_met_N_alljets;   //!
   TBranch        *b_min_delta_phi_mht_N;   //!
   TBranch        *b_min_delta_phi_mht;   //!
   TBranch        *b_mT_mu;   //!
   TBranch        *b_mT_el;   //!
   TBranch        *b_deltaThetaT;   //!
   TBranch        *b_W_pT;   //!
   TBranch        *b_SL_control_sample;   //!
   TBranch        *b_QCD_control_sample;   //!
   TBranch        *b_fatpT30_jet1_pt;   //!
   TBranch        *b_fatpT30_jet2_pt;   //!
   TBranch        *b_fatpT30_jet3_pt;   //!
   TBranch        *b_fatpT30_jet4_pt;   //!
   TBranch        *b_fatpT30_jet1_eta;   //!
   TBranch        *b_fatpT30_jet2_eta;   //!
   TBranch        *b_fatpT30_jet3_eta;   //!
   TBranch        *b_fatpT30_jet4_eta;   //!
   TBranch        *b_fatpT30_jet1_phi;   //!
   TBranch        *b_fatpT30_jet2_phi;   //!
   TBranch        *b_fatpT30_jet3_phi;   //!
   TBranch        *b_fatpT30_jet4_phi;   //!
   TBranch        *b_fatpT30_jet1_energy;   //!
   TBranch        *b_fatpT30_jet2_energy;   //!
   TBranch        *b_fatpT30_jet3_energy;   //!
   TBranch        *b_fatpT30_jet4_energy;   //!
   TBranch        *b_fatpT30_jet1_mJ;   //!
   TBranch        *b_fatpT30_jet2_mJ;   //!
   TBranch        *b_fatpT30_jet3_mJ;   //!
   TBranch        *b_fatpT30_jet4_mJ;   //!
   TBranch        *b_fatpT30_jet1_nConst;   //!
   TBranch        *b_fatpT30_jet2_nConst;   //!
   TBranch        *b_fatpT30_jet3_nConst;   //!
   TBranch        *b_fatpT30_jet4_nConst;   //!
   TBranch        *b_num_fatpT30_jets;   //!
   TBranch        *b_num_fatpT30_jets_pt100;   //!
   TBranch        *b_num_fatpT30_jets_pt150;   //!
   TBranch        *b_num_fatpT30_jets_pt200;   //!
   TBranch        *b_num_fatpT30_jets_pt300;   //!
   TBranch        *b_fatpT30_MJ;   //!
   TBranch        *b_fatpT30_MJ_pt100;   //!
   TBranch        *b_fatpT30_MJ_pt150;   //!
   TBranch        *b_fatpT30_MJ_pt200;   //!
   TBranch        *b_fatpT30_MJ_pt300;   //!
   TBranch        *b_num_fatpT30central_jets;   //!
   TBranch        *b_num_fatpT30central_jets_pt100;   //!
   TBranch        *b_num_fatpT30central_jets_pt150;   //!
   TBranch        *b_num_fatpT30central_jets_pt200;   //!
   TBranch        *b_num_fatpT30central_jets_pt300;   //!
   TBranch        *b_fatpT30central_MJ;   //!
   TBranch        *b_fatpT30central_MJ_pt100;   //!
   TBranch        *b_fatpT30central_MJ_pt150;   //!
   TBranch        *b_fatpT30central_MJ_pt200;   //!
   TBranch        *b_fatpT30central_MJ_pt300;   //!
   TBranch        *b_highest_mJ;   //!
   TBranch        *b_scnd_highest_mJ;   //!
   TBranch        *b_thrd_highest_mJ;   //!
   TBranch        *b_frth_highest_mJ;   //!
   TBranch        *b_sum_skinny_jet_mass;   //!
   TBranch        *b_min_mTWB;   //!
   TBranch        *b_min_mTWB_Wmass;   //!
   TBranch        *b_mTWB_2nd;   //!
   TBranch        *b_min_delta_phi_b_met;   //!
   TBranch        *b_doc_met;   //!
   TBranch        *b_gluino1_pt;   //!
   TBranch        *b_gluino2_pt;   //!
   TBranch        *b_num_veto_taus_mT;   //!
   TBranch        *b_tau_loose;   //!
   TBranch        *b_tau_medium;   //!
   TBranch        *b_tau_tight;   //!
   TBranch        *b_tau_pt;   //!
   TBranch        *b_tau_dB_iso;   //!
   TBranch        *b_tau_chargedIsoPtSum;   //!
   TBranch        *b_tau_mT;   //!
   TBranch        *b_num_iso_tracks_pt10;   //!
   TBranch        *b_num_iso_tracks_pt10_mT;   //!
   TBranch        *b_num_iso_tracks_pt15;   //!
   TBranch        *b_num_iso_tracks_pt15_mT;   //!
   TBranch        *b_num_el_tracks_mT;   //!
   TBranch        *b_num_mu_tracks_mT;   //!
   TBranch        *b_num_had_tracks_mT;   //!
   TBranch        *b_iso_track_pt;   //!
   TBranch        *b_iso_track_dB_iso;   //!
   TBranch        *b_iso_track_mT;   //!
   TBranch        *b_el_track_tm;   //!
   TBranch        *b_el_track_pt;   //!
   TBranch        *b_el_track_ch_iso;   //!
   TBranch        *b_el_track_mT;   //!
   TBranch        *b_mu_track_tm;   //!
   TBranch        *b_mu_track_pt;   //!
   TBranch        *b_mu_track_ch_iso;   //!
   TBranch        *b_mu_track_mT;   //!
   TBranch        *b_had_track_pt;   //!
   TBranch        *b_had_track_ch_iso;   //!
   TBranch        *b_had_track_mT;   //!
   TBranch        *b_num_true_els;   //!
   TBranch        *b_true_el_pt;   //!
   TBranch        *b_true_el_eta;   //!
   TBranch        *b_true_el_min_parton_dR;   //!
   TBranch        *b_true_el_reco;   //!
   TBranch        *b_true_el_passID;   //!
   TBranch        *b_true_el_mini_iso;   //!
   TBranch        *b_true_el_d0;   //!
   TBranch        *b_true_el_ptW;   //!
   TBranch        *b_true_el_dTT;   //!
   TBranch        *b_num_true_mus;   //!
   TBranch        *b_true_mu_pt;   //!
   TBranch        *b_true_mu_eta;   //!
   TBranch        *b_true_mu_min_parton_dR;   //!
   TBranch        *b_true_mu_reco;   //!
   TBranch        *b_true_mu_passID;   //!
   TBranch        *b_true_mu_mini_iso;   //!
   TBranch        *b_true_mu_d0;   //!
   TBranch        *b_true_mu_ptW;   //!
   TBranch        *b_true_mu_dTT;   //!
   TBranch        *b_num_true_had_taus;   //!
   TBranch        *b_true_had_tau_pt;   //!
   TBranch        *b_true_had_tau_eta;   //!
   TBranch        *b_true_had_tau_min_parton_dR;   //!
   TBranch        *b_true_had_tau_ptW;   //!
   TBranch        *b_true_had_tau_dTT;   //!
   TBranch        *b_num_true_lep_taus;   //!
   TBranch        *b_num_photons_pt100;   //!
   TBranch        *b_num_photons_pt100_old;   //!
   TBranch        *b_ph_pt;   //!
   TBranch        *b_ph_eta;   //!
   TBranch        *b_ph_id;   //!
   TBranch        *b_ph_ch_iso;   //!
   TBranch        *b_ph_nh_iso;   //!
   TBranch        *b_ph_ph_iso;   //!
   TBranch        *b_ph_ch_iso_cut;   //!
   TBranch        *b_ph_nh_iso_cut;   //!
   TBranch        *b_ph_ph_iso_cut;   //!
   TBranch        *b_ph_mht;   //!

   RTree(TTree *tree=0);
   virtual ~RTree();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
   virtual void     MakeLFRatioHist(TString outdir);
   virtual void     MakeDTTPDFs(TString outdir);
   virtual void     MakeMHTPDFs(TString outdir);
};

#endif

#ifdef RTree_cxx
RTree::RTree(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("skims/13TeV/reduced_trees/TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1_MINIAODSIM_UCSB2406_v78.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("skims/13TeV/reduced_trees/TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1_MINIAODSIM_UCSB2406_v78.root");
      }
      f->GetObject("reduced_tree",tree);

   }
   Init(tree);
}

RTree::~RTree()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t RTree::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t RTree::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void RTree::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   jet_pt = 0;
   jet_eta = 0;
   jet_phi = 0;
   jet_energy = 0;
   jet_csv = 0;
   jet_corr_def = 0;
   jet_corr_new = 0;
   jet_delta_phi_met = 0;
   jet_delta_phi_mht = 0;
   jet_mT = 0;
   jet_parton_id = 0;
   jet_mdp_jet = 0;
   jet_mdR_jet = 0;
   jet_gen_pt = 0;
   mu_tm = 0;
   mu_truid = 0;
   mu_momid = 0;
   mu_signal = 0;
   mu_veto = 0;
   mu_vid = 0;
   mu_pt = 0;
   mu_eta = 0;
   mu_phi = 0;
   mu_dB_iso_R04 = 0;
   mu_mini_iso_tr02 = 0;
   mu_mT = 0;
   mu_ptW = 0;
   mu_dTT = 0;
   el_tm = 0;
   el_truid = 0;
   el_momid = 0;
   el_signal = 0;
   el_veto = 0;
   el_sid = 0;
   el_vid = 0;
   el_pt = 0;
   el_eta = 0;
   el_phi = 0;
   el_dB_iso_R03 = 0;
   el_mini_iso_tr02 = 0;
   el_mT = 0;
   el_ptW = 0;
   el_dTT = 0;
   tau_loose = 0;
   tau_medium = 0;
   tau_tight = 0;
   tau_pt = 0;
   tau_dB_iso = 0;
   tau_chargedIsoPtSum = 0;
   tau_mT = 0;
   el_track_tm = 0;
   el_track_pt = 0;
   el_track_ch_iso = 0;
   el_track_mT = 0;
   mu_track_tm = 0;
   mu_track_pt = 0;
   mu_track_ch_iso = 0;
   mu_track_mT = 0;
   had_track_pt = 0;
   had_track_ch_iso = 0;
   had_track_mT = 0;
   true_el_pt = 0;
   true_el_eta = 0;
   true_el_min_parton_dR = 0;
   true_el_reco = 0;
   true_el_passID = 0;
   true_el_mini_iso = 0;
   true_el_d0 = 0;
   true_el_ptW = 0;
   true_el_dTT = 0;
   true_mu_pt = 0;
   true_mu_eta = 0;
   true_mu_min_parton_dR = 0;
   true_mu_reco = 0;
   true_mu_passID = 0;
   true_mu_mini_iso = 0;
   true_mu_d0 = 0;
   true_mu_ptW = 0;
   true_mu_dTT = 0;
   true_had_tau_pt = 0;
   true_had_tau_eta = 0;
   true_had_tau_min_parton_dR = 0;
   true_had_tau_ptW = 0;
   true_had_tau_dTT = 0;
   ph_pt = 0;
   ph_eta = 0;
   ph_id = 0;
   ph_ch_iso = 0;
   ph_nh_iso = 0;
   ph_ph_iso = 0;
   ph_ch_iso_cut = 0;
   ph_nh_iso_cut = 0;
   ph_ph_iso_cut = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("weightppb", &weightppb, &b_weightppb);
   fChain->SetBranchAddress("type_code", &type_code, &b_type_code);
   fChain->SetBranchAddress("gluino_mass", &gluino_mass, &b_gluino_mass);
   fChain->SetBranchAddress("lsp_mass", &lsp_mass, &b_lsp_mass);
   fChain->SetBranchAddress("passes2012RA2bTrigger", &passes2012RA2bTrigger, &b_passes2012RA2bTrigger);
   fChain->SetBranchAddress("passesJSONCut", &passesJSONCut, &b_passesJSONCut);
   fChain->SetBranchAddress("passesPVCut", &passesPVCut, &b_passesPVCut);
   fChain->SetBranchAddress("passes2012METCleaningCut", &passes2012METCleaningCut, &b_passes2012METCleaningCut);
   fChain->SetBranchAddress("passesCSA14METCleaningCut", &passesCSA14METCleaningCut, &b_passesCSA14METCleaningCut);
   fChain->SetBranchAddress("trackingfailurefilter_decision", &trackingfailurefilter_decision, &b_trackingfailurefilter_decision);
   fChain->SetBranchAddress("cschalofilter_decision", &cschalofilter_decision, &b_cschalofilter_decision);
   fChain->SetBranchAddress("ecallaserfilter_decision", &ecallaserfilter_decision, &b_ecallaserfilter_decision);
   fChain->SetBranchAddress("HBHENoisefilter_decision", &HBHENoisefilter_decision, &b_HBHENoisefilter_decision);
   fChain->SetBranchAddress("hcallaserfilter_decision", &hcallaserfilter_decision, &b_hcallaserfilter_decision);
   fChain->SetBranchAddress("passesBadJetFilter", &passesBadJetFilter, &b_passesBadJetFilter);
   fChain->SetBranchAddress("PBNRcode", &PBNRcode, &b_PBNRcode);
   fChain->SetBranchAddress("pu_weight", &pu_weight, &b_pu_weight);
   fChain->SetBranchAddress("top_pt_weight", &top_pt_weight, &b_top_pt_weight);
   fChain->SetBranchAddress("top_pt_weight_official", &top_pt_weight_official, &b_top_pt_weight_official);
   fChain->SetBranchAddress("trigger_eff_weight", &trigger_eff_weight, &b_trigger_eff_weight);
   fChain->SetBranchAddress("btag_weight", &btag_weight, &b_btag_weight);
   fChain->SetBranchAddress("btag_pid_weight", &btag_pid_weight, &b_btag_pid_weight);
   fChain->SetBranchAddress("Prob0", &Prob0, &b_Prob0);
   fChain->SetBranchAddress("ProbGEQ1", &ProbGEQ1, &b_ProbGEQ1);
   fChain->SetBranchAddress("Prob1", &Prob1, &b_Prob1);
   fChain->SetBranchAddress("ProbGEQ2", &ProbGEQ2, &b_ProbGEQ2);
   fChain->SetBranchAddress("Prob2", &Prob2, &b_Prob2);
   fChain->SetBranchAddress("ProbGEQ3", &ProbGEQ3, &b_ProbGEQ3);
   fChain->SetBranchAddress("Prob3", &Prob3, &b_Prob3);
   fChain->SetBranchAddress("ProbGEQ4", &ProbGEQ4, &b_ProbGEQ4);
   fChain->SetBranchAddress("Prob0_pt50", &Prob0_pt50, &b_Prob0_pt50);
   fChain->SetBranchAddress("ProbGEQ1_pt50", &ProbGEQ1_pt50, &b_ProbGEQ1_pt50);
   fChain->SetBranchAddress("Prob1_pt50", &Prob1_pt50, &b_Prob1_pt50);
   fChain->SetBranchAddress("ProbGEQ2_pt50", &ProbGEQ2_pt50, &b_ProbGEQ2_pt50);
   fChain->SetBranchAddress("Prob2_pt50", &Prob2_pt50, &b_Prob2_pt50);
   fChain->SetBranchAddress("ProbGEQ3_pt50", &ProbGEQ3_pt50, &b_ProbGEQ3_pt50);
   fChain->SetBranchAddress("Prob3_pt50", &Prob3_pt50, &b_Prob3_pt50);
   fChain->SetBranchAddress("ProbGEQ4_pt50", &ProbGEQ4_pt50, &b_ProbGEQ4_pt50);
   fChain->SetBranchAddress("run", &run, &b_run);
   fChain->SetBranchAddress("event", &event, &b_event);
   fChain->SetBranchAddress("lumiblock", &lumiblock, &b_lumiblock);
   fChain->SetBranchAddress("entry", &entry, &b_entry);
   fChain->SetBranchAddress("pass_HLT_PFHT350_PFMET100", &pass_HLT_PFHT350_PFMET100, &b_pass_HLT_PFHT350_PFMET100);
   fChain->SetBranchAddress("pass_HLT_PFHT650", &pass_HLT_PFHT650, &b_pass_HLT_PFHT650);
   fChain->SetBranchAddress("pass_HLT_DiCentralPFJet50_PFMET80", &pass_HLT_DiCentralPFJet50_PFMET80, &b_pass_HLT_DiCentralPFJet50_PFMET80);
   fChain->SetBranchAddress("pass_HLT_DiCentralPFJet30_PFMET80_BTagCSV07", &pass_HLT_DiCentralPFJet30_PFMET80_BTagCSV07, &b_pass_HLT_DiCentralPFJet30_PFMET80_BTagCSV07);
   fChain->SetBranchAddress("pu_true_num_interactions", &pu_true_num_interactions, &b_pu_true_num_interactions);
   fChain->SetBranchAddress("pu_num_interactions", &pu_num_interactions, &b_pu_num_interactions);
   fChain->SetBranchAddress("num_primary_vertices", &num_primary_vertices, &b_num_primary_vertices);
   fChain->SetBranchAddress("eoot_pu", &eoot_pu, &b_eoot_pu);
   fChain->SetBranchAddress("loot_pu", &loot_pu, &b_loot_pu);
   fChain->SetBranchAddress("oot_pu", &oot_pu, &b_oot_pu);
   fChain->SetBranchAddress("highest_csv", &highest_csv, &b_highest_csv);
   fChain->SetBranchAddress("second_highest_csv", &second_highest_csv, &b_second_highest_csv);
   fChain->SetBranchAddress("third_highest_csv", &third_highest_csv, &b_third_highest_csv);
   fChain->SetBranchAddress("fourth_highest_csv", &fourth_highest_csv, &b_fourth_highest_csv);
   fChain->SetBranchAddress("fifth_highest_csv", &fifth_highest_csv, &b_fifth_highest_csv);
   fChain->SetBranchAddress("sixth_highest_csv", &sixth_highest_csv, &b_sixth_highest_csv);
   fChain->SetBranchAddress("jet_pt", &jet_pt, &b_jet_pt);
   fChain->SetBranchAddress("jet_eta", &jet_eta, &b_jet_eta);
   fChain->SetBranchAddress("jet_phi", &jet_phi, &b_jet_phi);
   fChain->SetBranchAddress("jet_energy", &jet_energy, &b_jet_energy);
   fChain->SetBranchAddress("jet_csv", &jet_csv, &b_jet_csv);
   fChain->SetBranchAddress("jet_corr_def", &jet_corr_def, &b_jet_corr_def);
   fChain->SetBranchAddress("jet_corr_new", &jet_corr_new, &b_jet_corr_new);
   fChain->SetBranchAddress("jet_delta_phi_met", &jet_delta_phi_met, &b_jet_delta_phi_met);
   fChain->SetBranchAddress("jet_delta_phi_mht", &jet_delta_phi_mht, &b_jet_delta_phi_mht);
   fChain->SetBranchAddress("jet_mT", &jet_mT, &b_jet_mT);
   fChain->SetBranchAddress("jet_parton_id", &jet_parton_id, &b_jet_parton_id);
   fChain->SetBranchAddress("jet_mdp_jet", &jet_mdp_jet, &b_jet_mdp_jet);
   fChain->SetBranchAddress("jet_mdR_jet", &jet_mdR_jet, &b_jet_mdR_jet);
   fChain->SetBranchAddress("jet_gen_pt", &jet_gen_pt, &b_jet_gen_pt);
   fChain->SetBranchAddress("mmjet", &mmjet, &b_mmjet);
   fChain->SetBranchAddress("wjetmm", &wjetmm, &b_wjetmm);
   fChain->SetBranchAddress("omjet", &omjet, &b_omjet);
   fChain->SetBranchAddress("wjetom", &wjetom, &b_wjetom);
   fChain->SetBranchAddress("umjet", &umjet, &b_umjet);
   fChain->SetBranchAddress("wjetum", &wjetum, &b_wjetum);
   fChain->SetBranchAddress("jet1_pt", &jet1_pt, &b_jet1_pt);
   fChain->SetBranchAddress("jet2_pt", &jet2_pt, &b_jet2_pt);
   fChain->SetBranchAddress("jet3_pt", &jet3_pt, &b_jet3_pt);
   fChain->SetBranchAddress("jet4_pt", &jet4_pt, &b_jet4_pt);
   fChain->SetBranchAddress("jet5_pt", &jet5_pt, &b_jet5_pt);
   fChain->SetBranchAddress("jet6_pt", &jet6_pt, &b_jet6_pt);
   fChain->SetBranchAddress("def_met", &def_met, &b_def_met);
   fChain->SetBranchAddress("def_met_phi", &def_met_phi, &b_def_met_phi);
   fChain->SetBranchAddress("met", &met, &b_met);
   fChain->SetBranchAddress("met_phi", &met_phi, &b_met_phi);
   fChain->SetBranchAddress("raw_met", &raw_met, &b_raw_met);
   fChain->SetBranchAddress("raw_met_phi", &raw_met_phi, &b_raw_met_phi);
   fChain->SetBranchAddress("def_mht", &def_mht, &b_def_mht);
   fChain->SetBranchAddress("def_mht_phi", &def_mht_phi, &b_def_mht_phi);
   fChain->SetBranchAddress("mht30", &mht30, &b_mht30);
   fChain->SetBranchAddress("mht30_phi", &mht30_phi, &b_mht30_phi);
   fChain->SetBranchAddress("mht50", &mht50, &b_mht50);
   fChain->SetBranchAddress("mht50_phi", &mht50_phi, &b_mht50_phi);
   fChain->SetBranchAddress("mht30_raw", &mht30_raw, &b_mht30_raw);
   fChain->SetBranchAddress("mht30_phi_raw", &mht30_phi_raw, &b_mht30_phi_raw);
   fChain->SetBranchAddress("mht50_raw", &mht50_raw, &b_mht50_raw);
   fChain->SetBranchAddress("mht50_phi_raw", &mht50_phi_raw, &b_mht50_phi_raw);
   fChain->SetBranchAddress("mht30_central", &mht30_central, &b_mht30_central);
   fChain->SetBranchAddress("mht30_central_phi", &mht30_central_phi, &b_mht30_central_phi);
   fChain->SetBranchAddress("mht30_central_raw", &mht30_central_raw, &b_mht30_central_raw);
   fChain->SetBranchAddress("mht30_central_raw_phi", &mht30_central_raw_phi, &b_mht30_central_raw_phi);
   fChain->SetBranchAddress("mht_over_sqrt_ht30", &mht_over_sqrt_ht30, &b_mht_over_sqrt_ht30);
   fChain->SetBranchAddress("mht_over_sqrt_ht50", &mht_over_sqrt_ht50, &b_mht_over_sqrt_ht50);
   fChain->SetBranchAddress("ht20", &ht20, &b_ht20);
   fChain->SetBranchAddress("ht30", &ht30, &b_ht30);
   fChain->SetBranchAddress("ht30_forward", &ht30_forward, &b_ht30_forward);
   fChain->SetBranchAddress("ht40", &ht40, &b_ht40);
   fChain->SetBranchAddress("ht50", &ht50, &b_ht50);
   fChain->SetBranchAddress("ht60", &ht60, &b_ht60);
   fChain->SetBranchAddress("ht70", &ht70, &b_ht70);
   fChain->SetBranchAddress("ht80", &ht80, &b_ht80);
   fChain->SetBranchAddress("sumP30", &sumP30, &b_sumP30);
   fChain->SetBranchAddress("sumP50", &sumP50, &b_sumP50);
   fChain->SetBranchAddress("cent30", &cent30, &b_cent30);
   fChain->SetBranchAddress("cent50", &cent50, &b_cent50);
   fChain->SetBranchAddress("m_eff20", &m_eff20, &b_m_eff20);
   fChain->SetBranchAddress("m_eff30", &m_eff30, &b_m_eff30);
   fChain->SetBranchAddress("m_eff40", &m_eff40, &b_m_eff40);
   fChain->SetBranchAddress("m_eff50", &m_eff50, &b_m_eff50);
   fChain->SetBranchAddress("met_over_m_eff20", &met_over_m_eff20, &b_met_over_m_eff20);
   fChain->SetBranchAddress("met_over_m_eff30", &met_over_m_eff30, &b_met_over_m_eff30);
   fChain->SetBranchAddress("met_over_m_eff40", &met_over_m_eff40, &b_met_over_m_eff40);
   fChain->SetBranchAddress("met_over_m_eff50", &met_over_m_eff50, &b_met_over_m_eff50);
   fChain->SetBranchAddress("met_over_sqrt_ht20", &met_over_sqrt_ht20, &b_met_over_sqrt_ht20);
   fChain->SetBranchAddress("met_over_sqrt_ht30", &met_over_sqrt_ht30, &b_met_over_sqrt_ht30);
   fChain->SetBranchAddress("met_over_sqrt_ht40", &met_over_sqrt_ht40, &b_met_over_sqrt_ht40);
   fChain->SetBranchAddress("met_over_sqrt_ht50", &met_over_sqrt_ht50, &b_met_over_sqrt_ht50);
   fChain->SetBranchAddress("met_over_sqrt_ht80", &met_over_sqrt_ht80, &b_met_over_sqrt_ht80);
   fChain->SetBranchAddress("num_jets_pt20", &num_jets_pt20, &b_num_jets_pt20);
   fChain->SetBranchAddress("num_jets_pt30", &num_jets_pt30, &b_num_jets_pt30);
   fChain->SetBranchAddress("num_jets_pt40", &num_jets_pt40, &b_num_jets_pt40);
   fChain->SetBranchAddress("num_jets_pt50", &num_jets_pt50, &b_num_jets_pt50);
   fChain->SetBranchAddress("num_jets_pt70", &num_jets_pt70, &b_num_jets_pt70);
   fChain->SetBranchAddress("num_jets_pt100", &num_jets_pt100, &b_num_jets_pt100);
   fChain->SetBranchAddress("num_jets_pt150", &num_jets_pt150, &b_num_jets_pt150);
   fChain->SetBranchAddress("num_csvt_jets", &num_csvt_jets, &b_num_csvt_jets);
   fChain->SetBranchAddress("num_csvm_jets", &num_csvm_jets, &b_num_csvm_jets);
   fChain->SetBranchAddress("num_csvl_jets", &num_csvl_jets, &b_num_csvl_jets);
   fChain->SetBranchAddress("num_csvt_jets20", &num_csvt_jets20, &b_num_csvt_jets20);
   fChain->SetBranchAddress("num_csvm_jets20", &num_csvm_jets20, &b_num_csvm_jets20);
   fChain->SetBranchAddress("num_csvl_jets20", &num_csvl_jets20, &b_num_csvl_jets20);
   fChain->SetBranchAddress("num_csvt_jets30", &num_csvt_jets30, &b_num_csvt_jets30);
   fChain->SetBranchAddress("num_csvm_jets30", &num_csvm_jets30, &b_num_csvm_jets30);
   fChain->SetBranchAddress("num_csvl_jets30", &num_csvl_jets30, &b_num_csvl_jets30);
   fChain->SetBranchAddress("num_csvt_jets40", &num_csvt_jets40, &b_num_csvt_jets40);
   fChain->SetBranchAddress("num_csvm_jets40", &num_csvm_jets40, &b_num_csvm_jets40);
   fChain->SetBranchAddress("num_csvl_jets40", &num_csvl_jets40, &b_num_csvl_jets40);
   fChain->SetBranchAddress("num_truth_matched_b_jets", &num_truth_matched_b_jets, &b_num_truth_matched_b_jets);
   fChain->SetBranchAddress("num_good_truth_matched_b_jets", &num_good_truth_matched_b_jets, &b_num_good_truth_matched_b_jets);
   fChain->SetBranchAddress("num_veto_mus", &num_veto_mus, &b_num_veto_mus);
   fChain->SetBranchAddress("num_mus", &num_mus, &b_num_mus);
   fChain->SetBranchAddress("mu_tm", &mu_tm, &b_mu_tm);
   fChain->SetBranchAddress("mu_truid", &mu_truid, &b_mu_truid);
   fChain->SetBranchAddress("mu_momid", &mu_momid, &b_mu_momid);
   fChain->SetBranchAddress("mu_signal", &mu_signal, &b_mu_signal);
   fChain->SetBranchAddress("mu_veto", &mu_veto, &b_mu_veto);
   fChain->SetBranchAddress("mu_vid", &mu_vid, &b_mu_vid);
   fChain->SetBranchAddress("mu_pt", &mu_pt, &b_mu_pt);
   fChain->SetBranchAddress("mu_eta", &mu_eta, &b_mu_eta);
   fChain->SetBranchAddress("mu_phi", &mu_phi, &b_mu_phi);
   fChain->SetBranchAddress("mu_dB_iso_R04", &mu_dB_iso_R04, &b_mu_dB_iso_R04);
   fChain->SetBranchAddress("mu_mini_iso_tr02", &mu_mini_iso_tr02, &b_mu_mini_iso_tr02);
   fChain->SetBranchAddress("mu_mT", &mu_mT, &b_mu_mT);
   fChain->SetBranchAddress("mu_ptW", &mu_ptW, &b_mu_ptW);
   fChain->SetBranchAddress("mu_dTT", &mu_dTT, &b_mu_dTT);
   fChain->SetBranchAddress("num_veto_els", &num_veto_els, &b_num_veto_els);
   fChain->SetBranchAddress("num_els", &num_els, &b_num_els);
   fChain->SetBranchAddress("el_tm", &el_tm, &b_el_tm);
   fChain->SetBranchAddress("el_truid", &el_truid, &b_el_truid);
   fChain->SetBranchAddress("el_momid", &el_momid, &b_el_momid);
   fChain->SetBranchAddress("el_signal", &el_signal, &b_el_signal);
   fChain->SetBranchAddress("el_veto", &el_veto, &b_el_veto);
   fChain->SetBranchAddress("el_sid", &el_sid, &b_el_sid);
   fChain->SetBranchAddress("el_vid", &el_vid, &b_el_vid);
   fChain->SetBranchAddress("el_pt", &el_pt, &b_el_pt);
   fChain->SetBranchAddress("el_eta", &el_eta, &b_el_eta);
   fChain->SetBranchAddress("el_phi", &el_phi, &b_el_phi);
   fChain->SetBranchAddress("el_dB_iso_R03", &el_dB_iso_R03, &b_el_dB_iso_R03);
   fChain->SetBranchAddress("el_mini_iso_tr02", &el_mini_iso_tr02, &b_el_mini_iso_tr02);
   fChain->SetBranchAddress("el_mT", &el_mT, &b_el_mT);
   fChain->SetBranchAddress("el_ptW", &el_ptW, &b_el_ptW);
   fChain->SetBranchAddress("el_dTT", &el_dTT, &b_el_dTT);
   fChain->SetBranchAddress("min_delta_phi_met_N", &min_delta_phi_met_N, &b_min_delta_phi_met_N);
   fChain->SetBranchAddress("min_delta_phi_met_N_old", &min_delta_phi_met_N_old, &b_min_delta_phi_met_N_old);
   fChain->SetBranchAddress("min_delta_phi_met_N_2012", &min_delta_phi_met_N_2012, &b_min_delta_phi_met_N_2012);
   fChain->SetBranchAddress("min_delta_phi_met_N_loose_jets", &min_delta_phi_met_N_loose_jets, &b_min_delta_phi_met_N_loose_jets);
   fChain->SetBranchAddress("min_delta_phi_met", &min_delta_phi_met, &b_min_delta_phi_met);
   fChain->SetBranchAddress("min_delta_phi_met_loose_jets", &min_delta_phi_met_loose_jets, &b_min_delta_phi_met_loose_jets);
   fChain->SetBranchAddress("min_delta_phi_met_2jets", &min_delta_phi_met_2jets, &b_min_delta_phi_met_2jets);
   fChain->SetBranchAddress("min_delta_phi_met_4jets", &min_delta_phi_met_4jets, &b_min_delta_phi_met_4jets);
   fChain->SetBranchAddress("min_delta_phi_met_alljets", &min_delta_phi_met_alljets, &b_min_delta_phi_met_alljets);
   fChain->SetBranchAddress("min_delta_phi_met_N_2jets", &min_delta_phi_met_N_2jets, &b_min_delta_phi_met_N_2jets);
   fChain->SetBranchAddress("min_delta_phi_met_N_4jets", &min_delta_phi_met_N_4jets, &b_min_delta_phi_met_N_4jets);
   fChain->SetBranchAddress("min_delta_phi_met_N_alljets", &min_delta_phi_met_N_alljets, &b_min_delta_phi_met_N_alljets);
   fChain->SetBranchAddress("min_delta_phi_mht_N", &min_delta_phi_mht_N, &b_min_delta_phi_mht_N);
   fChain->SetBranchAddress("min_delta_phi_mht", &min_delta_phi_mht, &b_min_delta_phi_mht);
   fChain->SetBranchAddress("mT_mu", &mT_mu, &b_mT_mu);
   fChain->SetBranchAddress("mT_el", &mT_el, &b_mT_el);
   fChain->SetBranchAddress("deltaThetaT", &deltaThetaT, &b_deltaThetaT);
   fChain->SetBranchAddress("W_pT", &W_pT, &b_W_pT);
   fChain->SetBranchAddress("SL_control_sample", &SL_control_sample, &b_SL_control_sample);
   fChain->SetBranchAddress("QCD_control_sample", &QCD_control_sample, &b_QCD_control_sample);
   fChain->SetBranchAddress("fatpT30_jet1_pt", &fatpT30_jet1_pt, &b_fatpT30_jet1_pt);
   fChain->SetBranchAddress("fatpT30_jet2_pt", &fatpT30_jet2_pt, &b_fatpT30_jet2_pt);
   fChain->SetBranchAddress("fatpT30_jet3_pt", &fatpT30_jet3_pt, &b_fatpT30_jet3_pt);
   fChain->SetBranchAddress("fatpT30_jet4_pt", &fatpT30_jet4_pt, &b_fatpT30_jet4_pt);
   fChain->SetBranchAddress("fatpT30_jet1_eta", &fatpT30_jet1_eta, &b_fatpT30_jet1_eta);
   fChain->SetBranchAddress("fatpT30_jet2_eta", &fatpT30_jet2_eta, &b_fatpT30_jet2_eta);
   fChain->SetBranchAddress("fatpT30_jet3_eta", &fatpT30_jet3_eta, &b_fatpT30_jet3_eta);
   fChain->SetBranchAddress("fatpT30_jet4_eta", &fatpT30_jet4_eta, &b_fatpT30_jet4_eta);
   fChain->SetBranchAddress("fatpT30_jet1_phi", &fatpT30_jet1_phi, &b_fatpT30_jet1_phi);
   fChain->SetBranchAddress("fatpT30_jet2_phi", &fatpT30_jet2_phi, &b_fatpT30_jet2_phi);
   fChain->SetBranchAddress("fatpT30_jet3_phi", &fatpT30_jet3_phi, &b_fatpT30_jet3_phi);
   fChain->SetBranchAddress("fatpT30_jet4_phi", &fatpT30_jet4_phi, &b_fatpT30_jet4_phi);
   fChain->SetBranchAddress("fatpT30_jet1_energy", &fatpT30_jet1_energy, &b_fatpT30_jet1_energy);
   fChain->SetBranchAddress("fatpT30_jet2_energy", &fatpT30_jet2_energy, &b_fatpT30_jet2_energy);
   fChain->SetBranchAddress("fatpT30_jet3_energy", &fatpT30_jet3_energy, &b_fatpT30_jet3_energy);
   fChain->SetBranchAddress("fatpT30_jet4_energy", &fatpT30_jet4_energy, &b_fatpT30_jet4_energy);
   fChain->SetBranchAddress("fatpT30_jet1_mJ", &fatpT30_jet1_mJ, &b_fatpT30_jet1_mJ);
   fChain->SetBranchAddress("fatpT30_jet2_mJ", &fatpT30_jet2_mJ, &b_fatpT30_jet2_mJ);
   fChain->SetBranchAddress("fatpT30_jet3_mJ", &fatpT30_jet3_mJ, &b_fatpT30_jet3_mJ);
   fChain->SetBranchAddress("fatpT30_jet4_mJ", &fatpT30_jet4_mJ, &b_fatpT30_jet4_mJ);
   fChain->SetBranchAddress("fatpT30_jet1_nConst", &fatpT30_jet1_nConst, &b_fatpT30_jet1_nConst);
   fChain->SetBranchAddress("fatpT30_jet2_nConst", &fatpT30_jet2_nConst, &b_fatpT30_jet2_nConst);
   fChain->SetBranchAddress("fatpT30_jet3_nConst", &fatpT30_jet3_nConst, &b_fatpT30_jet3_nConst);
   fChain->SetBranchAddress("fatpT30_jet4_nConst", &fatpT30_jet4_nConst, &b_fatpT30_jet4_nConst);
   fChain->SetBranchAddress("num_fatpT30_jets", &num_fatpT30_jets, &b_num_fatpT30_jets);
   fChain->SetBranchAddress("num_fatpT30_jets_pt100", &num_fatpT30_jets_pt100, &b_num_fatpT30_jets_pt100);
   fChain->SetBranchAddress("num_fatpT30_jets_pt150", &num_fatpT30_jets_pt150, &b_num_fatpT30_jets_pt150);
   fChain->SetBranchAddress("num_fatpT30_jets_pt200", &num_fatpT30_jets_pt200, &b_num_fatpT30_jets_pt200);
   fChain->SetBranchAddress("num_fatpT30_jets_pt300", &num_fatpT30_jets_pt300, &b_num_fatpT30_jets_pt300);
   fChain->SetBranchAddress("fatpT30_MJ", &fatpT30_MJ, &b_fatpT30_MJ);
   fChain->SetBranchAddress("fatpT30_MJ_pt100", &fatpT30_MJ_pt100, &b_fatpT30_MJ_pt100);
   fChain->SetBranchAddress("fatpT30_MJ_pt150", &fatpT30_MJ_pt150, &b_fatpT30_MJ_pt150);
   fChain->SetBranchAddress("fatpT30_MJ_pt200", &fatpT30_MJ_pt200, &b_fatpT30_MJ_pt200);
   fChain->SetBranchAddress("fatpT30_MJ_pt300", &fatpT30_MJ_pt300, &b_fatpT30_MJ_pt300);
   fChain->SetBranchAddress("num_fatpT30central_jets", &num_fatpT30central_jets, &b_num_fatpT30central_jets);
   fChain->SetBranchAddress("num_fatpT30central_jets_pt100", &num_fatpT30central_jets_pt100, &b_num_fatpT30central_jets_pt100);
   fChain->SetBranchAddress("num_fatpT30central_jets_pt150", &num_fatpT30central_jets_pt150, &b_num_fatpT30central_jets_pt150);
   fChain->SetBranchAddress("num_fatpT30central_jets_pt200", &num_fatpT30central_jets_pt200, &b_num_fatpT30central_jets_pt200);
   fChain->SetBranchAddress("num_fatpT30central_jets_pt300", &num_fatpT30central_jets_pt300, &b_num_fatpT30central_jets_pt300);
   fChain->SetBranchAddress("fatpT30central_MJ", &fatpT30central_MJ, &b_fatpT30central_MJ);
   fChain->SetBranchAddress("fatpT30central_MJ_pt100", &fatpT30central_MJ_pt100, &b_fatpT30central_MJ_pt100);
   fChain->SetBranchAddress("fatpT30central_MJ_pt150", &fatpT30central_MJ_pt150, &b_fatpT30central_MJ_pt150);
   fChain->SetBranchAddress("fatpT30central_MJ_pt200", &fatpT30central_MJ_pt200, &b_fatpT30central_MJ_pt200);
   fChain->SetBranchAddress("fatpT30central_MJ_pt300", &fatpT30central_MJ_pt300, &b_fatpT30central_MJ_pt300);
   fChain->SetBranchAddress("highest_mJ", &highest_mJ, &b_highest_mJ);
   fChain->SetBranchAddress("scnd_highest_mJ", &scnd_highest_mJ, &b_scnd_highest_mJ);
   fChain->SetBranchAddress("thrd_highest_mJ", &thrd_highest_mJ, &b_thrd_highest_mJ);
   fChain->SetBranchAddress("frth_highest_mJ", &frth_highest_mJ, &b_frth_highest_mJ);
   fChain->SetBranchAddress("sum_skinny_jet_mass", &sum_skinny_jet_mass, &b_sum_skinny_jet_mass);
   fChain->SetBranchAddress("min_mTWB", &min_mTWB, &b_min_mTWB);
   fChain->SetBranchAddress("min_mTWB_Wmass", &min_mTWB_Wmass, &b_min_mTWB_Wmass);
   fChain->SetBranchAddress("mTWB_2nd", &mTWB_2nd, &b_mTWB_2nd);
   fChain->SetBranchAddress("min_delta_phi_b_met", &min_delta_phi_b_met, &b_min_delta_phi_b_met);
   fChain->SetBranchAddress("doc_met", &doc_met, &b_doc_met);
   fChain->SetBranchAddress("gluino1_pt", &gluino1_pt, &b_gluino1_pt);
   fChain->SetBranchAddress("gluino2_pt", &gluino2_pt, &b_gluino2_pt);
   fChain->SetBranchAddress("num_veto_taus_mT", &num_veto_taus_mT, &b_num_veto_taus_mT);
   fChain->SetBranchAddress("tau_loose", &tau_loose, &b_tau_loose);
   fChain->SetBranchAddress("tau_medium", &tau_medium, &b_tau_medium);
   fChain->SetBranchAddress("tau_tight", &tau_tight, &b_tau_tight);
   fChain->SetBranchAddress("tau_pt", &tau_pt, &b_tau_pt);
   fChain->SetBranchAddress("tau_dB_iso", &tau_dB_iso, &b_tau_dB_iso);
   fChain->SetBranchAddress("tau_chargedIsoPtSum", &tau_chargedIsoPtSum, &b_tau_chargedIsoPtSum);
   fChain->SetBranchAddress("tau_mT", &tau_mT, &b_tau_mT);
   fChain->SetBranchAddress("num_iso_tracks_pt10", &num_iso_tracks_pt10, &b_num_iso_tracks_pt10);
   fChain->SetBranchAddress("num_iso_tracks_pt10_mT", &num_iso_tracks_pt10_mT, &b_num_iso_tracks_pt10_mT);
   fChain->SetBranchAddress("num_iso_tracks_pt15", &num_iso_tracks_pt15, &b_num_iso_tracks_pt15);
   fChain->SetBranchAddress("num_iso_tracks_pt15_mT", &num_iso_tracks_pt15_mT, &b_num_iso_tracks_pt15_mT);
   fChain->SetBranchAddress("num_el_tracks_mT", &num_el_tracks_mT, &b_num_el_tracks_mT);
   fChain->SetBranchAddress("num_mu_tracks_mT", &num_mu_tracks_mT, &b_num_mu_tracks_mT);
   fChain->SetBranchAddress("num_had_tracks_mT", &num_had_tracks_mT, &b_num_had_tracks_mT);
   fChain->SetBranchAddress("iso_track_pt", &iso_track_pt, &b_iso_track_pt);
   fChain->SetBranchAddress("iso_track_dB_iso", &iso_track_dB_iso, &b_iso_track_dB_iso);
   fChain->SetBranchAddress("iso_track_mT", &iso_track_mT, &b_iso_track_mT);
   fChain->SetBranchAddress("el_track_tm", &el_track_tm, &b_el_track_tm);
   fChain->SetBranchAddress("el_track_pt", &el_track_pt, &b_el_track_pt);
   fChain->SetBranchAddress("el_track_ch_iso", &el_track_ch_iso, &b_el_track_ch_iso);
   fChain->SetBranchAddress("el_track_mT", &el_track_mT, &b_el_track_mT);
   fChain->SetBranchAddress("mu_track_tm", &mu_track_tm, &b_mu_track_tm);
   fChain->SetBranchAddress("mu_track_pt", &mu_track_pt, &b_mu_track_pt);
   fChain->SetBranchAddress("mu_track_ch_iso", &mu_track_ch_iso, &b_mu_track_ch_iso);
   fChain->SetBranchAddress("mu_track_mT", &mu_track_mT, &b_mu_track_mT);
   fChain->SetBranchAddress("had_track_pt", &had_track_pt, &b_had_track_pt);
   fChain->SetBranchAddress("had_track_ch_iso", &had_track_ch_iso, &b_had_track_ch_iso);
   fChain->SetBranchAddress("had_track_mT", &had_track_mT, &b_had_track_mT);
   fChain->SetBranchAddress("num_true_els", &num_true_els, &b_num_true_els);
   fChain->SetBranchAddress("true_el_pt", &true_el_pt, &b_true_el_pt);
   fChain->SetBranchAddress("true_el_eta", &true_el_eta, &b_true_el_eta);
   fChain->SetBranchAddress("true_el_min_parton_dR", &true_el_min_parton_dR, &b_true_el_min_parton_dR);
   fChain->SetBranchAddress("true_el_reco", &true_el_reco, &b_true_el_reco);
   fChain->SetBranchAddress("true_el_passID", &true_el_passID, &b_true_el_passID);
   fChain->SetBranchAddress("true_el_mini_iso", &true_el_mini_iso, &b_true_el_mini_iso);
   fChain->SetBranchAddress("true_el_d0", &true_el_d0, &b_true_el_d0);
   fChain->SetBranchAddress("true_el_ptW", &true_el_ptW, &b_true_el_ptW);
   fChain->SetBranchAddress("true_el_dTT", &true_el_dTT, &b_true_el_dTT);
   fChain->SetBranchAddress("num_true_mus", &num_true_mus, &b_num_true_mus);
   fChain->SetBranchAddress("true_mu_pt", &true_mu_pt, &b_true_mu_pt);
   fChain->SetBranchAddress("true_mu_eta", &true_mu_eta, &b_true_mu_eta);
   fChain->SetBranchAddress("true_mu_min_parton_dR", &true_mu_min_parton_dR, &b_true_mu_min_parton_dR);
   fChain->SetBranchAddress("true_mu_reco", &true_mu_reco, &b_true_mu_reco);
   fChain->SetBranchAddress("true_mu_passID", &true_mu_passID, &b_true_mu_passID);
   fChain->SetBranchAddress("true_mu_mini_iso", &true_mu_mini_iso, &b_true_mu_mini_iso);
   fChain->SetBranchAddress("true_mu_d0", &true_mu_d0, &b_true_mu_d0);
   fChain->SetBranchAddress("true_mu_ptW", &true_mu_ptW, &b_true_mu_ptW);
   fChain->SetBranchAddress("true_mu_dTT", &true_mu_dTT, &b_true_mu_dTT);
   fChain->SetBranchAddress("num_true_had_taus", &num_true_had_taus, &b_num_true_had_taus);
   fChain->SetBranchAddress("true_had_tau_pt", &true_had_tau_pt, &b_true_had_tau_pt);
   fChain->SetBranchAddress("true_had_tau_eta", &true_had_tau_eta, &b_true_had_tau_eta);
   fChain->SetBranchAddress("true_had_tau_min_parton_dR", &true_had_tau_min_parton_dR, &b_true_had_tau_min_parton_dR);
   fChain->SetBranchAddress("true_had_tau_ptW", &true_had_tau_ptW, &b_true_had_tau_ptW);
   fChain->SetBranchAddress("true_had_tau_dTT", &true_had_tau_dTT, &b_true_had_tau_dTT);
   fChain->SetBranchAddress("num_true_lep_taus", &num_true_lep_taus, &b_num_true_lep_taus);
   fChain->SetBranchAddress("num_photons_pt100", &num_photons_pt100, &b_num_photons_pt100);
   fChain->SetBranchAddress("num_photons_pt100_old", &num_photons_pt100_old, &b_num_photons_pt100_old);
   fChain->SetBranchAddress("ph_pt", &ph_pt, &b_ph_pt);
   fChain->SetBranchAddress("ph_eta", &ph_eta, &b_ph_eta);
   fChain->SetBranchAddress("ph_id", &ph_id, &b_ph_id);
   fChain->SetBranchAddress("ph_ch_iso", &ph_ch_iso, &b_ph_ch_iso);
   fChain->SetBranchAddress("ph_nh_iso", &ph_nh_iso, &b_ph_nh_iso);
   fChain->SetBranchAddress("ph_ph_iso", &ph_ph_iso, &b_ph_ph_iso);
   fChain->SetBranchAddress("ph_ch_iso_cut", &ph_ch_iso_cut, &b_ph_ch_iso_cut);
   fChain->SetBranchAddress("ph_nh_iso_cut", &ph_nh_iso_cut, &b_ph_nh_iso_cut);
   fChain->SetBranchAddress("ph_ph_iso_cut", &ph_ph_iso_cut, &b_ph_ph_iso_cut);
   fChain->SetBranchAddress("ph_mht", &ph_mht, &b_ph_mht);
   Notify();
}

Bool_t RTree::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void RTree::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t RTree::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef RTree_cxx
