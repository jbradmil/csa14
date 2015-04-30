#include "weights.hpp"
#include <map>
#include <iostream>

std::map<std::string, double> WeightCalculator::crossSectionTable;
std::map<std::string, int> WeightCalculator::totalEventsTable;

WeightCalculator::WeightCalculator(const double lumiIn, const int NentriesIn):
  lumi(lumiIn),
  Nentries(NentriesIn)
{
  SetCrossSections();
  SetTotalEvents();
}

void WeightCalculator::SetLuminosity(const double lumiIn){
  lumi=lumiIn;
}

double WeightCalculator::GetLuminosity() const{
  return lumi;
}

double WeightCalculator::GetCrossSection(const std::string &process) const{
  for(std::map<std::string, double>::iterator it(crossSectionTable.begin());
      it!=crossSectionTable.end(); ++it){
    if(process.find(it->first)!=std::string::npos){
      return it->second;
    }
  }
  return -1.0;
}

double WeightCalculator::GetCrossSection(const std::string &process, const int m1,
                                         const int m2) const{
  if(m1>=0 && m2>=0){
    return GetCrossSection(process);
  }else{
    return GetCrossSection(process);
  }
}

int WeightCalculator::GetTotalEvents(const std::string &process) const{
  for(std::map<std::string, int>::iterator it(totalEventsTable.begin());
      it!=totalEventsTable.end(); ++it){
    if(process.find(it->first)!=std::string::npos){
      return it->second;
    }
  }
  return -1;
}

int WeightCalculator::GetTotalEvents(const std::string &process, const int m1,
                                     const int m2) const{
  if(m1>=0 && m2>=0){
    return GetTotalEvents(process);
  }else{
    return GetTotalEvents(process);
  }
}

double WeightCalculator::GetWeight(const std::string &process) const{
  const double xsec(GetCrossSection(process));
  std::cout << "xsec: " << xsec << std::endl;
  int events(Nentries);
  if(events<0) events=GetTotalEvents(process);
  std::cout << "Normalizing to " << events << " events." << std::endl;
  if(events>=0 && xsec>=0.0){
    return lumi*xsec/static_cast<double>(events);
  }else{
    return 1.0;
  }
}

double WeightCalculator::GetWeightPerPb(const std::string &process) const{
  // std::cout << "Process: " << process << std::endl;
  const double xsec(GetCrossSection(process));
  std::cout << "xsec: " << xsec << std::endl;
  int events(Nentries);
  if(events<0) events=GetTotalEvents(process);
  std::cout << "Normalizing to " << events << " events." << std::endl;
  if(events>0 && xsec>=0.0){
    return xsec/static_cast<double>(events);
  }else{
    return 1.0;
  }
}

double WeightCalculator::GetWeight(const std::string &process, const int m1,
                                   const int m2) const{
  if(m1>=0 && m2>=0){
    return GetWeight(process);
  }else{
    return GetWeight(process);
  }
}

void WeightCalculator::SetCrossSections(){
  const double ttbar_xsec(245.8);
  const double ttbar_norm(ttbar_xsec/(13.43+53.4+53.2));
  const double mysterious_k_factor(1.19);
  const double wjets_k_factor(1.23);
  const double zjets_k_factor(1.27);
  crossSectionTable["QCD_HT-250To500_TuneZ2star_8TeV"]=276000.;
  crossSectionTable["QCD_HT-500To1000_TuneZ2star_8TeV"]=8426.;
  crossSectionTable["QCD_HT-1000ToInf_TuneZ2star_8TeV"]=204.;
  crossSectionTable["QCD_Pt-1000to1400_TuneZ2star_8TeV_pythia6"]=0.737844;
  crossSectionTable["QCD_Pt-120to170_TuneZ2star_8TeV_pythia6"]=156293.3;
  crossSectionTable["QCD_Pt-1400to1800_TuneZ2star_8TeV_pythia6"]=0.03352235;
  crossSectionTable["QCD_Pt-170to300_TuneZ2star_8TeV_pythia6"]=34138.15;
  crossSectionTable["QCD_Pt-1800_TuneZ2star_8TeV_pythia6"]=0.001829005;
  crossSectionTable["QCD_Pt-300to470_TuneZ2star_8TeV_pythia6"]=1759.549;
  crossSectionTable["QCD_Pt-470to600_TuneZ2star_8TeV_pythia6"]=113.8791;
  crossSectionTable["QCD_Pt-600to800_TuneZ2star_8TeV_pythia6"]=26.9921;
  crossSectionTable["QCD_Pt-800to1000_TuneZ2star_8TeV_pythia6"]=3.550036;
  crossSectionTable["SMS-TChiZH_ZccbbHbb_2J_mChargino-130to500_mLSP-1to370_TuneZ2star_8TeV-madgraph-tauola"]=0.111*0.561*0.1512;
  crossSectionTable["TTH_Inclusive_M-125_8TeV_pythia6"]=0.1293*0.577;
  crossSectionTable["TTH_HToBB_M-125_8TeV-pythia6"]=0.1293*0.577;
  crossSectionTable["TTJets_FullLeptMGDecays_8TeV-madgraph"]=13.43*ttbar_norm;
  crossSectionTable["TTJets_HadronicMGDecays_8TeV-madgraph"]=53.4*ttbar_norm;
  crossSectionTable["TTJets_SemiLeptMGDecays_8TeV-madgraph"]=53.2*ttbar_norm;
  crossSectionTable["TTJets_WToBC_8TeV-madgraph-tauola"]=ttbar_xsec*0.00084*0.676;
  crossSectionTable["TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola"]=ttbar_xsec;
  crossSectionTable["TTWJets_8TeV-madgraph"]=0.2149;
  crossSectionTable["TTZJets_8TeV-madgraph_v2"]=0.172;
  crossSectionTable["Tbar_t-channel_TuneZ2star_8TeV-powheg"]=30.7;
  crossSectionTable["Tbar_tW-channel-DR_TuneZ2star_8TeV-powheg"]=11.1;
  crossSectionTable["T_t-channel_TuneZ2star_8TeV-powheg"]=56.4;
  crossSectionTable["T_tW-channel-DR_TuneZ2star_8TeV-powheg"]=11.1;
  crossSectionTable["T_s-channel_TuneZ2star_8TeV-powheg-tauola"]=3.79;
  crossSectionTable["Tbar_s-channel_TuneZ2star_8TeV-powheg-tauola"]=1.76;
  crossSectionTable["W2JetsToLNu_TuneZ2Star_8TeV-madgraph"]=1750.0*mysterious_k_factor;
  crossSectionTable["W3JetsToLNu_TuneZ2Star_8TeV-madgraph"]=519.0*mysterious_k_factor;
  crossSectionTable["W4JetsToLNu_TuneZ2Star_8TeV-madgraph"]=214.0*mysterious_k_factor;
  crossSectionTable["WJetsToLNu_HT-250To300_8TeV"]=57.26;
  crossSectionTable["WJetsToLNu_HT-300To400_8TeV"]=45.68;
  crossSectionTable["WJetsToLNu_HT-400ToInf_8TeV"]=30.08;
  crossSectionTable["WH_WToLNu_HToBB_M-125_8TeV-powheg-herwigpp"]=0.7046 *(0.1075+0.1057+0.1125) *0.577;
  crossSectionTable["WW_TuneZ2star_8TeV_pythia6_tauola"]=55.0;
  crossSectionTable["WbbJetsToLNu_Massive_TuneZ2star_8TeV-madgraph-pythia6_tauola"]=211.3;
  crossSectionTable["ZH_ZToBB_HToBB_M-125_8TeV-powheg-herwigpp"]=0.4153*0.1512*0.577;
  crossSectionTable["ZJetsToNuNu_50_HT_100_TuneZ2Star_8TeV_madgraph"]=381.2*mysterious_k_factor;
  crossSectionTable["ZJetsToNuNu_100_HT_200_TuneZ2Star_8TeV_madgraph"]=160.3*mysterious_k_factor;
  crossSectionTable["ZJetsToNuNu_200_HT_400_TuneZ2Star_8TeV_madgraph"]=41.49*mysterious_k_factor;
  crossSectionTable["ZJetsToNuNu_400_HT_inf_TuneZ2Star_8TeV_madgraph"]=5.272*mysterious_k_factor;
  crossSectionTable["WZ_TuneZ2star_8TeV_pythia6_tauola"]=32.3;
  crossSectionTable["ZZ_TuneZ2star_8TeV_pythia6_tauola"]=17.654;
  crossSectionTable["TChihh_400"]=0.03*0.561*0.561;
  crossSectionTable["TChihh_250"]=0.271*0.561*0.561;
  crossSectionTable["TChihh_200"]=0.6975*0.561*0.561;
  crossSectionTable["SMS-HbbHbb_mHiggsino-200_mLSP-1_8TeV-Pythia6Z_jgsmith-SMS-HbbHbb_mHiggsino-200_mLSP-1_8TeV-Pythia6Z-26439e701cfb9736f297615863e915f9_USER_UCSB1807_v69"]=0.608*0.561*0.561;
  crossSectionTable["SMS-HbbHbb_mHiggsino-250_mLSP-1_8TeV-Pythia6Z_jgsmith-SMS-HbbHbb_mHiggsino-250_mLSP-1_8TeV-Pythia6Z-26439e701cfb9736f297615863e915f9_USER_UCSB1808_v69"]=0.244*0.561*0.561;
  crossSectionTable["SMS-HbbHbb_mHiggsino-300_mLSP-1_8TeV-Pythia6Z_jgsmith-SMS-HbbHbb_mHiggsino-300_mLSP-1_8TeV-Pythia6Z-26439e701cfb9736f297615863e915f9_USER_UCSB1810_v69"]=0.111*0.561*0.561;
  crossSectionTable["SMS-HbbHbb_mHiggsino-350_mLSP-1_8TeV-Pythia6Z_jgsmith-SMS-HbbHbb_mHiggsino-350_mLSP-1_8TeV-Pythia6Z-26439e701cfb9736f297615863e915f9_USER_UCSB1811_v69"]=0.0552*0.561*0.561;
  crossSectionTable["SMS-HbbHbb_mHiggsino-400_mLSP-1_8TeV-Pythia6Z_jgsmith-SMS-HbbHbb_mHiggsino-400_mLSP-1_8TeV-Pythia6Z-26439e701cfb9736f297615863e915f9_USER_UCSB1812_v69"]=0.0294*0.561*0.561;
  crossSectionTable["SMS-HbbHbb_mHiggsino-450_mLSP-1_8TeV-Pythia6Z_jgsmith-SMS-HbbHbb_mHiggsino-450_mLSP-1_8TeV-Pythia6Z-26439e701cfb9736f297615863e915f9_USER_UCSB1809_v69"]=0.0163*0.561*0.561;
  crossSectionTable["BJets_HT-250To500_8TeV-madgraph"]=5828.0;
  crossSectionTable["BJets_HT-500To1000_8TeV-madgraph"]=217.6;
  crossSectionTable["BJets_HT-1000ToInf_8TeV-madgraph"]=4.712;
  crossSectionTable["SMS-TChiHH_2b2b_2J_mChargino-200_mLSP-1_TuneZ2star_8TeV-madgraph-tauola"]=0.608*0.561*0.561;
  crossSectionTable["SMS-TChiHH_2b2b_2J_mChargino-250_mLSP-1_TuneZ2star_8TeV-madgraph-tauola"]=0.244*0.561*0.561;
  crossSectionTable["SMS-TChiHH_2b2b_2J_mChargino-300_mLSP-1_TuneZ2star_8TeV-madgraph-tauola"]=0.111*0.561*0.561;
  crossSectionTable["SMS-TChiHH_2b2b_2J_mChargino-350_mLSP-1_TuneZ2star_8TeV-madgraph-tauola"]=0.0552*0.561*0.561;
  crossSectionTable["SMS-TChiHH_2b2b_2J_mChargino-400_mLSP-1_TuneZ2star_8TeV-madgraph-tauola"]=0.0294*0.561*0.561;
  crossSectionTable["SMS-TChiHH_2b2b_2J_mChargino-450_mLSP-1_TuneZ2star_8TeV-madgraph-tauola"]=0.0163*0.561*0.561;
  crossSectionTable["SMS-T1bbbb_2J_mGo-1025_mLSP-50_TuneZ2star_8TeV"]=0.0195439;
  crossSectionTable["SMS-T1tttt_2J_mGo-1025_mLSP-75_TuneZ2star_8TeV"]=0.0195439;
  // CSA14 samples
  crossSectionTable["SMS-T1tttt_2J_mGl-1500_mLSP-100"]=0.0141903;
  crossSectionTable["SMS-T1tttt_2J_mGl-1200_mLSP-800"]=0.0856418;
  crossSectionTable["SMS-T1bbbb_2J_mGl-1500_mLSP-100"]=0.0141903;
  crossSectionTable["SMS-T1bbbb_2J_mGl-1000_mLSP-900"]=0.325388;
  crossSectionTable["SMS-T1qqqq_2J_mGl-1400_mLSP-100"]=0.0252977;
  crossSectionTable["SMS-T1qqqq_2J_mGl-1000_mLSP-800"]=0.325388;  
  crossSectionTable["SMS-T2tt_2J_mStop-425_mLSP-325"]=1.31169;
  crossSectionTable["SMS-T2tt_2J_mStop-500_mLSP-325"]=0.51848;
  crossSectionTable["SMS-T2tt_2J_mStop-650_mLSP-325"]=0.107045;
  crossSectionTable["SMS-T2tt_2J_mStop-850_mLSP-100"]=0.0189612;
  crossSectionTable["SMS-T2bb_2J_mStop-600_mLSP-580"]=0.174599;
  crossSectionTable["SMS-T2bb_2J_mStop-900_mLSP-100"]=0.0128895;
  //  crossSectionTable["TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola-Spring14miniaod-PU20bx25"]=818.8;
  //  crossSectionTable["TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola_Spring14miniaod-PU_S14"]=818.8;
  // https://twiki.cern.ch/twiki/bin/view/Sandbox/FullNNLOcrossSections#Top_cross_section_for_13_TeV
  crossSectionTable["TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola"]=806.1;
  crossSectionTable["WJetsToLNu_HT-100to200_Tune4C_13TeV-madgraph-tauola"]=1817.0*wjets_k_factor;
  crossSectionTable["WJetsToLNu_HT-200to400_Tune4C_13TeV-madgraph-tauola"]=471.6*wjets_k_factor;
  crossSectionTable["WJetsToLNu_HT-400to600_Tune4C_13TeV-madgraph-tauola"]=55.61*wjets_k_factor;
  crossSectionTable["WJetsToLNu_HT-600toInf_Tune4C_13TeV-madgraph-tauola"]=18.81*wjets_k_factor;
  crossSectionTable["QCD_Pt-5to10_Tune4C_13TeV_pythia8"]=80710000000;
  crossSectionTable["QCD_Pt-10to15_Tune4C_13TeV_pythia8"]=7528000000;
  crossSectionTable["QCD_Pt-15to30_Tune4C_13TeV_pythia8"]=2327000000;
  crossSectionTable["QCD_Pt-30to50_Tune4C_13TeV_pythia8"]=161500000;
  crossSectionTable["QCD_Pt-50to80_Tune4C_13TeV_pythia8"]=22110000;
  crossSectionTable["QCD_Pt-80to120_Tune4C_13TeV_pythia8"]=3000114;
  crossSectionTable["QCD_Pt-120to170_Tune4C_13TeV_pythia8"]=493200;
  crossSectionTable["QCD_Pt-170to300_Tune4C_13TeV_pythia8"]=120300;
  crossSectionTable["QCD_Pt-300to470_Tune4C_13TeV_pythia8"]=7475;
  crossSectionTable["QCD_Pt-470to600_Tune4C_13TeV_pythia8"]=587.1;
  crossSectionTable["QCD_Pt-600to800_Tune4C_13TeV_pythia8"]=167;
  crossSectionTable["QCD_Pt-800to1000_Tune4C_13TeV_pythia8"]=28.25;
  crossSectionTable["QCD_Pt-1000to1400_Tune4C_13TeV_pythia8"]=8.195;
  crossSectionTable["QCD_Pt-1400to1800_Tune4C_13TeV_pythia8"]=0.7346;
  crossSectionTable["QCD_Pt-1800to2400_Tune4C_13TeV_pythia8"]=0.102;
  crossSectionTable["QCD_Pt-2400to3200_Tune4C_13TeV_pythia8"]=0.0064;
  crossSectionTable["QCD_Pt-3200_Tune4C_13TeV_pythia8"]=0.000163;
  crossSectionTable["QCD_HT-100To250_13TeV-madgraph"]=28730000.;
  crossSectionTable["QCD_HT_250To500_13TeV-madgraph"]=670500.;
  crossSectionTable["QCD_HT-500To1000_13TeV-madgraph"]=26740.;
  crossSectionTable["QCD_HT_1000ToInf_13TeV-madgraph"]=769.7;
  crossSectionTable["ZJetsToNuNu_HT-100to200_Tune4C_13TeV-madgraph-tauola"]=372.6*zjets_k_factor;
  crossSectionTable["ZJetsToNuNu_HT-200to400_Tune4C_13TeV-madgraph-tauola"]=100.8*zjets_k_factor;
  crossSectionTable["ZJetsToNuNu_HT-400to600_Tune4C_13TeV-madgraph-tauola"]=11.99*zjets_k_factor;
  crossSectionTable["ZJetsToNuNu_HT-600toInf_Tune4C_13TeV-madgraph-tauola"]=4.113*zjets_k_factor;
  crossSectionTable["TToLeptons_s-channel-CSA14_Tune4C_13TeV-aMCatNLO-tauola"]=2.;
  crossSectionTable["TToLeptons_t-channel-CSA14_Tune4C_13TeV-aMCatNLO-tauola"]=103.4;
  crossSectionTable["T_tW-channel-DR_Tune4C_13TeV-CSA14-powheg-tauola"]=35.;
  crossSectionTable["TBarToLeptons_s-channel-CSA14_Tune4C_13TeV-aMCatNLO-tauola"]=1.;
  crossSectionTable["TBarToLeptons_t-channel_Tune4C_CSA14_13TeV-aMCatNLO-tauola"]=61.6;
  crossSectionTable["Tbar_tW-channel-DR_Tune4C_13TeV-CSA14-powheg-tauola"]=35.;
  // Calculated at 13 TeV in
  // https://twiki.cern.ch/twiki/bin/view/LHCPhysics/CERNYellowReportPageAt1314TeV
  // Higgs branching ratios from
  // https://twiki.cern.ch/twiki/bin/view/LHCPhysics/CERNYellowReportPageBR
  crossSectionTable["ZH_HToBB_ZToLL_M-125_13TeV_powheg-herwigpp"]=0.569*0.033658*0.8696;
  crossSectionTable["ZH_HToBB_ZToNuNu_M-125_13TeV_powheg-herwigpp"]=0.569*0.2*0.8696;
  crossSectionTable["WH_HToBB_WToLNu_M-125_13TeV_powheg-herwigpp"]=0.569*0.1086*1.380;
  crossSectionTable["TTbarH_M-125_13TeV_amcatnlo-pythia8-tauola"]=0.5085;
  // crossSectionTable["TTZJets_Tune4C_13TeV-madgraph-tauola"]=2.232;
  // crossSectionTable["TTWJets_Tune4C_13TeV-madgraph-tauola"]=1.152;
  // http://arxiv.org/pdf/1405.0301.pdf
  crossSectionTable["TTZJets_Tune4C_13TeV-madgraph-tauola"]=0.5074;
  crossSectionTable["TTWJets_Tune4C_13TeV-madgraph-tauola"]=0.3404;
  crossSectionTable["DYJetsToLL_M-50_HT-100to200_Tune4C_13TeV"]=194.3;
  crossSectionTable["DYJetsToLL_M-50_HT-200to400_Tune4C_13TeV"]=52.24;
  crossSectionTable["DYJetsToLL_M-50_HT-400to600_Tune4C_13TeV"]=6.546;
  crossSectionTable["DYJetsToLL_M-50_HT-600toInf_Tune4C_13TeV"]=2.179;
  crossSectionTable["Neutrino_Pt-2to20_gun_13TeV-Spring14miniaod"]=78260000000;
  crossSectionTable["RelValTTbar_13TeV-MINIAODSIM-PU25ns_PHYS14_25_V1_Phys14-v1_v73"]=806.1;
  crossSectionTable["GJets_HT-100to200_Tune4C_13TeV"]=1534.;
  crossSectionTable["GJets_HT-200to400_Tune4C_13TeV"]=489.9;
  crossSectionTable["GJets_HT-400to600_Tune4C_13TeV"]=62.05;
  crossSectionTable["GJets_HT-600toInf_Tune4C_13TeV"]=20.87;
}

void WeightCalculator::SetTotalEvents(){
  totalEventsTable["QCD_HT-250To500_TuneZ2star_8TeV"]=27062078;
  totalEventsTable["QCD_HT-500To1000_TuneZ2star_8TeV"]=30599292;
  totalEventsTable["QCD_HT-1000ToInf_TuneZ2star_8TeV"]=13843863;
  totalEventsTable["QCD_Pt-120to170_TuneZ2star_8TeV_pythia6_Summer12_DR53X-PU_S10_START53_V7A-v3_AODSIM_UCSB1654_v67"]=5985732;
  totalEventsTable["QCD_Pt-170to300_TuneZ2star_8TeV_pythia6_v2_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_UCSB1657_v67"]=19970232;
  totalEventsTable["QCD_Pt-300to470_TuneZ2star_8TeV_pythia6_v3_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_UCSB1658_v67"]=19894000;
  totalEventsTable["QCD_Pt-470to600_TuneZ2star_8TeV_pythia6_Summer12_DR53X-PU_S10_START53_V7A-v2_AODSIM_UCSB1659_v67"]=3994848;
  totalEventsTable["QCD_Pt-600to800_TuneZ2star_8TeV_pythia6_Summer12_DR53X-PU_S10_START53_V7A-v2_AODSIM_UCSB1663_v67"]=3996864;
  totalEventsTable["QCD_Pt-800to1000_TuneZ2star_8TeV_pythia6_Summer12_DR53X-PU_S10_START53_V7A-v2_AODSIM_UCSB1660_v67"]=3998563;
  totalEventsTable["QCD_Pt-1000to1400_TuneZ2star_8TeV_pythia6_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_UCSB1664_v67"]=1964088;
  totalEventsTable["QCD_Pt-1400to1800_TuneZ2star_8TeV_pythia6_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_UCSB1665_v67"]=2000062;
  totalEventsTable["QCD_Pt-1800_TuneZ2star_8TeV_pythia6_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_UCSB1666_v67"]=977586;
  totalEventsTable["QCD_Pt-120to170_TuneZ2star_8TeV_pythia6_Summer12_DR53X-PU_S10_START53_V7A-v3_AODSIM_UCSB1513_v66"]=5985732;
  totalEventsTable["QCD_Pt-170to300_TuneZ2star_8TeV_pythia6_v2_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_UCSB1603_v66"]=19970232;
  totalEventsTable["QCD_Pt-300to470_TuneZ2star_8TeV_pythia6_v3_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_UCSB1609_v66"]=19894000;
  totalEventsTable["QCD_Pt-470to600_TuneZ2star_8TeV_pythia6_Summer12_DR53X-PU_S10_START53_V7A-v2_AODSIM_UCSB1659_v67"]=3994848;
  totalEventsTable["QCD_Pt-600to800_TuneZ2star_8TeV_pythia6_Summer12_DR53X-PU_S10_START53_V7A-v2_AODSIM_UCSB1663_v67"]=3996864;
  totalEventsTable["QCD_Pt-800to1000_TuneZ2star_8TeV_pythia6_Summer12_DR53X-PU_S10_START53_V7A-v2_AODSIM_UCSB1559_v66"]=3998563;
  totalEventsTable["QCD_Pt-1000to1400_TuneZ2star_8TeV_pythia6_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_UCSB1664_v67"]=1964088;
  totalEventsTable["QCD_Pt-1400to1800_TuneZ2star_8TeV_pythia6_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_UCSB1665_v67"]=2000062;//
  totalEventsTable["QCD_Pt-1800_TuneZ2star_8TeV_pythia6_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_UCSB1585_v66"]=977586;
  totalEventsTable["TTJets_FullLeptMGDecays_8TeV-madgraph_Summer12_DR53X-PU_S10_START53_V7A-v2_AODSIM_UCSB1596_v66"]=12119013;
  totalEventsTable["TTJets_SemiLeptMGDecays_8TeV-madgraph_Summer12_DR53X-PU_S10_START53_V7A_ext-v1_AODSIM_UCSB1606_v66"]=25413514;
  totalEventsTable["TTJets_HadronicMGDecays_8TeV-madgraph_Summer12_DR53X-PU_S10_START53_V7A_ext-v1_AODSIM_UCSB1613_v66"]=31223821;
  totalEventsTable["TTWJets_8TeV-madgraph_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_UCSB1605_v66"]=196046;
  totalEventsTable["TTZJets_8TeV-madgraph_v2_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_UCSB1604_v66"]=210160;
  totalEventsTable["WbbJetsToLNu_Massive_TuneZ2star_8TeV-madgraph-pythia6_tauola_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_UCSB1677_v67"]=20646001;
  totalEventsTable["ZJetsToNuNu_100_HT_200_TuneZ2Star_8TeV_madgraph_ext_Summer12_DR53X-PU_S10_START53_V7C-v1_AODSIM_UCSB1607_v66"]=5571413+4416646;
  totalEventsTable["ZJetsToNuNu_100_HT_200_TuneZ2Star_8TeV_madgraph_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_UCSB1525_v66"]=5571413+4416646;
  totalEventsTable["ZJetsToNuNu_200_HT_400_TuneZ2Star_8TeV_madgraph_ext_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_UCSB1594_v66"]=4689734+5055885;
  totalEventsTable["ZJetsToNuNu_200_HT_400_TuneZ2Star_8TeV_madgraph_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_UCSB1524_v66"]=4689734+5055885;
  totalEventsTable["ZJetsToNuNu_400_HT_inf_TuneZ2Star_8TeV_madgraph_ext_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_UCSB1602_v66"]=4088782+1006928;
  totalEventsTable["ZJetsToNuNu_400_HT_inf_TuneZ2Star_8TeV_madgraph_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_UCSB1523_v66"]=4088782+1006928;
  totalEventsTable["TTH_Inclusive_M-125_8TeV_pythia6_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM+_UCSB1783_v68"]=1000000;
  totalEventsTable["TChihh_400"]=9999;
  totalEventsTable["TChihh_250"]=9999;
  totalEventsTable["TChihh_200"]=9999;
  totalEventsTable["SMS-HbbHbb_mHiggsino-200_mLSP-1_8TeV-Pythia6Z_jgsmith-SMS-HbbHbb_mHiggsino-200_mLSP-1_8TeV-Pythia6Z-26439e701cfb9736f297615863e915f9_USER_UCSB1807_v69"]=99996;
  totalEventsTable["SMS-HbbHbb_mHiggsino-250_mLSP-1_8TeV-Pythia6Z_jgsmith-SMS-HbbHbb_mHiggsino-250_mLSP-1_8TeV-Pythia6Z-26439e701cfb9736f297615863e915f9_USER_UCSB1808_v69"]=99994;
  totalEventsTable["SMS-HbbHbb_mHiggsino-300_mLSP-1_8TeV-Pythia6Z_jgsmith-SMS-HbbHbb_mHiggsino-300_mLSP-1_8TeV-Pythia6Z-26439e701cfb9736f297615863e915f9_USER_UCSB1810_v69"]=99995;
  totalEventsTable["SMS-HbbHbb_mHiggsino-350_mLSP-1_8TeV-Pythia6Z_jgsmith-SMS-HbbHbb_mHiggsino-350_mLSP-1_8TeV-Pythia6Z-26439e701cfb9736f297615863e915f9_USER_UCSB1811_v69"]=99994;
  totalEventsTable["SMS-HbbHbb_mHiggsino-400_mLSP-1_8TeV-Pythia6Z_jgsmith-SMS-HbbHbb_mHiggsino-400_mLSP-1_8TeV-Pythia6Z-26439e701cfb9736f297615863e915f9_USER_UCSB1812_v69"]=99987;
  totalEventsTable["SMS-HbbHbb_mHiggsino-450_mLSP-1_8TeV-Pythia6Z_jgsmith-SMS-HbbHbb_mHiggsino-450_mLSP-1_8TeV-Pythia6Z-26439e701cfb9736f297615863e915f9_USER_UCSB1809_v69"]=99986;
  totalEventsTable["ZZ_TuneZ2star_8TeV_pythia6_tauola_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_UCSB1876_v71"]=9799908;
  totalEventsTable["ZH_ZToBB_HToBB_M-125_8TeV-powheg-herwigpp_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_UCSB1868_v71"]=996299;
  totalEventsTable["WH_WToLNu_HToBB_M-125_8TeV-powheg-herwigpp_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_UCSB1858_v71"]=1000000;
  totalEventsTable["WW_TuneZ2star_8TeV_pythia6_tauola"]=10000431;
  totalEventsTable["SMS-TChiZH_ZccbbHbb_2J_mChargino-130to500_mLSP-1to370_TuneZ2star_8TeV-madgraph-tauola_Summer12-START53_V19_FSIM-v1_AODSIM_UCSB1873_v71"]=2155;
  totalEventsTable["QCD_Pt-120to170_TuneZ2star_8TeV_pythia6_Summer12_DR53X-PU_S10_START53_V7A-v3_AODSIM"]=5985732;
  totalEventsTable["QCD_Pt-170to300_TuneZ2star_8TeV_pythia6_Summer12_DR53X-PU_S10_START53_V7A-v2_AODSIM"]=5820254;
  totalEventsTable["QCD_Pt-300to470_TuneZ2star_8TeV_pythia6_Summer12_DR53X-PU_S10_START53_V7A-v2_AODSIM"]=5978500;
  totalEventsTable["QCD_Pt-470to600_TuneZ2star_8TeV_pythia6_Summer12_DR53X-PU_S10_START53_V7A-v2_AODSIM"]=3994848;
  totalEventsTable["QCD_Pt-600to800_TuneZ2star_8TeV_pythia6_Summer12_DR53X-PU_S10_START53_V7A-v2_AODSIM"]=3996864;
  totalEventsTable["QCD_Pt-800to1000_TuneZ2star_8TeV_pythia6_Summer12_DR53X-PU_S10_START53_V7A-v2_AODSIM"]=3998563;
  totalEventsTable["QCD_Pt-1000to1400_TuneZ2star_8TeV_pythia6_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM"]=1964088;
  totalEventsTable["QCD_Pt-1400to1800_TuneZ2star_8TeV_pythia6_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM"]=2000062;
  totalEventsTable["QCD_Pt-1800_TuneZ2star_8TeV_pythia6_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM"]=977586;
  totalEventsTable["WbbJetsToLNu_Massive_TuneZ2star_8TeV-madgraph-pythia6_tauola_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_UCSB1859_v71"]=20646001;
  totalEventsTable["TTJets_FullLeptMGDecays_8TeV-madgraph"]=12011428;
  totalEventsTable["TTJets_SemiLeptMGDecays_8TeV-madgraph"]=24953451+30856876+30999167;
  // totalEventsTable["TTJets_SemiLeptMGDecays_8TeV-madgraph"]=24953451;
  totalEventsTable["TTJets_HadronicMGDecays_8TeV-madgraph"]=31223821;
  totalEventsTable["TTJets_WToBC_8TeV-madgraph-tauola_Summer12_DR53X-PU_S10_START53_V19-v1_AODSIM_UCSB1966_v71"]=276156;
  totalEventsTable["TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola_Summer12-START53"]=7299862;
  totalEventsTable["TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola_Summer12_DR53X"]=6923750;
  totalEventsTable["TTWJets_8TeV-madgraph_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM"]=196046;
  totalEventsTable["TTZJets_8TeV-madgraph_v2_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM"]=210160;
  totalEventsTable["ZJetsToNuNu_100_HT_200_TuneZ2Star_8TeV_madgraph_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM"]=4416646;
  totalEventsTable["ZJetsToNuNu_200_HT_400_TuneZ2Star_8TeV_madgraph_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM"]=5055885+4689734;
  totalEventsTable["ZJetsToNuNu_200_HT_400_TuneZ2Star_8TeV_madgraph_ext_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM"]=4689734+5055885;
  totalEventsTable["ZJetsToNuNu_400_HT_inf_TuneZ2Star_8TeV_madgraph_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM"]=1006928+4088782;
  totalEventsTable["ZJetsToNuNu_400_HT_inf_TuneZ2Star_8TeV_madgraph_ext_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM"]=4088782+1006928;
  totalEventsTable["ZH_ZToBB_HToBB_M-125_8TeV-powheg-herwigpp_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_UCSB1868_v71"]=996299;
  totalEventsTable["WH_WToLNu_HToBB_M-125_8TeV-powheg-herwigpp_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_UCSB1858_v71"]=1000000;
  totalEventsTable["WW_TuneZ2star_8TeV_pythia6_tauola"]=10000431;
  totalEventsTable["WZ_TuneZ2star_8TeV_pythia6_tauola_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_UCSB1875_v71"]=10000283;
  totalEventsTable["ZZ_TuneZ2star_8TeV_pythia6_tauola_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_UCSB1876_v71"]=9799908;
  totalEventsTable["TTH_HToBB_M-125_8TeV-pythia6_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM"]=1000008;
  totalEventsTable["W2JetsToLNu_TuneZ2Star_8TeV-madgraph"]=34044921;
  totalEventsTable["W3JetsToLNu_TuneZ2Star_8TeV-madgraph"]=15539503;
  totalEventsTable["W4JetsToLNu_TuneZ2Star_8TeV-madgraph"]=13382803;
  totalEventsTable["WJetsToLNu_HT-250To300_8TeV"]=4626528;
  totalEventsTable["WJetsToLNu_HT-300To400_8TeV"]=1699486+5141023;
  totalEventsTable["WJetsToLNu_HT-400ToInf_8TeV"]=4971847+1647807;
  totalEventsTable["T_s-channel_TuneZ2star_8TeV-powheg-tauola_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM"]=259961;
  totalEventsTable["T_t-channel_TuneZ2star_8TeV-powheg-tauola_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM"]=3758227;
  totalEventsTable["T_tW-channel-DR_TuneZ2star_8TeV-powheg-tauola_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM"]=497658;
  totalEventsTable["Tbar_s-channel_TuneZ2star_8TeV-powheg-tauola_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM"]=139974;
  totalEventsTable["Tbar_t-channel_TuneZ2star_8TeV-powheg-tauola_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM"]=1935072;
  totalEventsTable["Tbar_tW-channel-DR_TuneZ2star_8TeV-powheg-tauola_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM"]=493460;
  totalEventsTable["BJets_HT-250To500_8TeV-madgraph_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_UCSB1893_v71"]=13183812;
  totalEventsTable["BJets_HT-500To1000_8TeV-madgraph_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_UCSB1894_v71"]=6650243;
  totalEventsTable["BJets_HT-1000ToInf_8TeV-madgraph_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_UCSB1895_v71"]=3137949;
  totalEventsTable["SMS-TChiHH_2b2b_2J_mChargino-200_mLSP-1_TuneZ2star_8TeV-madgraph-tauola_Summer12-START53_V19_FSIM-v1_AODSIM_UCSB1872_v71"]=388371;
  totalEventsTable["SMS-TChiHH_2b2b_2J_mChargino-250_mLSP-1_TuneZ2star_8TeV-madgraph-tauola_Summer12-START53_V19_FSIM-v1_AODSIM_UCSB1872_v71"]=151750;
  totalEventsTable["SMS-TChiHH_2b2b_2J_mChargino-300_mLSP-1_TuneZ2star_8TeV-madgraph-tauola_Summer12-START53_V19_FSIM-v1_AODSIM_UCSB1872_v71"]=147484;
  totalEventsTable["SMS-TChiHH_2b2b_2J_mChargino-350_mLSP-1_TuneZ2star_8TeV-madgraph-tauola_Summer12-START53_V19_FSIM-v1_AODSIM_UCSB1871_v71"]=80478;
  totalEventsTable["SMS-TChiHH_2b2b_2J_mChargino-400_mLSP-1_TuneZ2star_8TeV-madgraph-tauola_Summer12-START53_V19_FSIM-v1_AODSIM_UCSB1871_v71"]=79013;
  totalEventsTable["SMS-TChiHH_2b2b_2J_mChargino-450_mLSP-1_TuneZ2star_8TeV-madgraph-tauola_Summer12-START53_V19_FSIM-v1_AODSIM_UCSB1871_v71"]=76402;
  totalEventsTable["SMS-T1bbbb_2J_mGo-1025_mLSP-50_TuneZ2star_8TeV"]=110722;
  totalEventsTable["SMS-T1tttt_2J_mGo-1075_mLSP-75_TuneZ2star_8TeV"]=123483;

// CSA14 samples
  totalEventsTable["SMS-T1tttt_2J_mGl-1500_mLSP-100_Tune4C_13TeV-madgraph-tauola_Spring14miniaod-PU20bx25_POSTLS170_V5-v2_MINIAODSIM"]=105679;
  totalEventsTable["SMS-T1tttt_2J_mGl-1200_mLSP-800_Tune4C_13TeV-madgraph-tauola_Spring14miniaod-PU20bx25"]=100322;
  totalEventsTable["SMS-T1bbbb_2J_mGl-1500_mLSP-100_Tune4C_13TeV-madgraph-tauola_Spring14miniaod-PU20bx25_POSTLS170_V5-v3_MINIAODSIM"]=105964;
  totalEventsTable["SMS-T1bbbb_2J_mGl-1000_mLSP-900_Tune4C_13TeV-madgraph-tauola_Spring14miniaod-PU20bx25_POSTLS170_V5-v2_MINIAODSIM"]=97584;
  totalEventsTable["SMS-T1qqqq_2J_mGl-1400_mLSP-100_Tune4C_13TeV-madgraph-tauola_Spring14miniaod-PU20bx25_POSTLS170_V5-v1_MINIAODSIM"]=103943;
  totalEventsTable["SMS-T1qqqq_2J_mGl-1000_mLSP-800_Tune4C_13TeV-madgraph-tauola_Spring14miniaod-PU20bx25_POSTLS170_V5-v1_MINIAODSIM"]=97513;
  totalEventsTable["SMS-T1tttt_2J_mGl-1200_mLSP-800_Tune4C_13TeV-madgraph-tauola_duanders"]=140854;
  totalEventsTable["SMS-T1tttt_2J_mGl-1500_mLSP-100_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM"]=105679;
  totalEventsTable["SMS-T1tttt_2J_mGl-1200_mLSP-800_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM"]=100322;
  totalEventsTable["SMS-T1bbbb_2J_mGl-1500_mLSP-100_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM"]=105149;
  totalEventsTable["SMS-T1bbbb_2J_mGl-1000_mLSP-900_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM"]=97134;
  totalEventsTable["SMS-T1qqqq_2J_mGl-1400_mLSP-100_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM"]=102891;
  totalEventsTable["SMS-T1qqqq_2J_mGl-1000_mLSP-800_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1_MINIAODSIM"]=96681;
  totalEventsTable["SMS-T2tt_2J_mStop-425_mLSP-325"]=1045470;
  totalEventsTable["SMS-T2tt_2J_mStop-500_mLSP-325"]=112116;
  totalEventsTable["SMS-T2tt_2J_mStop-650_mLSP-325"]=107131;
  totalEventsTable["SMS-T2tt_2J_mStop-850_mLSP-100"]=102839;
  totalEventsTable["SMS-T2bb_2J_mStop-600_mLSP-580"]=107316;
  totalEventsTable["SMS-T2bb_2J_mStop-900_mLSP-100"]=102661;
  totalEventsTable["TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola_Spring14miniaod-PU20bx25"]=25474122;
  totalEventsTable["TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola_Spring14miniaod-PU_S14"]=17184825;
  totalEventsTable["TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1_MINIAODSIM"]=25446993;
  totalEventsTable["WJetsToLNu_HT-100to200_Tune4C_13TeV-madgraph-tauola_Spring14miniaod"]=4087868/*3181078*/;
  totalEventsTable["WJetsToLNu_HT-200to400_Tune4C_13TeV-madgraph-tauola_Spring14miniaod"]=4933933/*3133549*/;
  totalEventsTable["WJetsToLNu_HT-400to600_Tune4C_13TeV-madgraph-tauola_Spring14miniaod"]=4642823/*2507939*/;
  totalEventsTable["WJetsToLNu_HT-600toInf_Tune4C_13TeV-madgraph-tauola_Spring14miniaod"]=4634811/*2399709*/;
  totalEventsTable["WJetsToLNu_HT-100to200_Tune4C_13TeV-madgraph-tauola_Phys14DR"]=5262265/*3181078*/;
  totalEventsTable["WJetsToLNu_HT-200to400_Tune4C_13TeV-madgraph-tauola_Phys14DR"]=4936077/*3133549*/;
  totalEventsTable["WJetsToLNu_HT-400to600_Tune4C_13TeV-madgraph-tauola_Phys14DR"]=4640594/*2507939*/;
  totalEventsTable["WJetsToLNu_HT-600toInf_Tune4C_13TeV-madgraph-tauola_Phys14DR"]=4581841/*2399709*/;
  totalEventsTable["QCD_Pt-5to10_Tune4C_13TeV_pythia8_Spring14miniaod"]=4791146;
  totalEventsTable["QCD_Pt-10to15_Tune4C_13TeV_pythia8_Spring14miniaod"]=4510630;
  totalEventsTable["QCD_Pt-15to30_Tune4C_13TeV_pythia8_Spring14miniaod"]=4655329;
  totalEventsTable["QCD_Pt-30to50_Tune4C_13TeV_pythia8_Spring14miniaod"]=4848856;
  totalEventsTable["QCD_Pt-50to80_Tune4C_13TeV_pythia8_Spring14miniaod"]=4726495;
  totalEventsTable["QCD_Pt-80to120_Tune4C_13TeV_pythia8_Spring14miniaod"]=4794295;
  totalEventsTable["QCD_Pt-120to170_Tune4C_13TeV_pythia8_Spring14miniaod"]=3562052;
  totalEventsTable["QCD_Pt-170to300_Tune4C_13TeV_pythia8_Spring14miniaod"]=2793204;
  totalEventsTable["QCD_Pt-300to470_Tune4C_13TeV_pythia8_Spring14miniaod"]=2700637;
  totalEventsTable["QCD_Pt-470to600_Tune4C_13TeV_pythia8_Spring14miniaod"]=2907137;
  totalEventsTable["QCD_Pt-600to800_Tune4C_13TeV_pythia8_Spring14miniaod"]=2853619;
  totalEventsTable["QCD_Pt-800to1000_Tune4C_13TeV_pythia8_Spring14miniaod"]=2875729;
  totalEventsTable["QCD_Pt-1000to1400_Tune4C_13TeV_pythia8_Spring14miniaod"]=2834231;
  totalEventsTable["QCD_Pt-1400to1800_Tune4C_13TeV_pythia8_Spring14miniaod"]=2931414;
  totalEventsTable["QCD_Pt-1800to2400_Tune4C_13TeV_pythia8_Spring14miniaod"]=2953210;
  totalEventsTable["QCD_Pt-2400to3200_Tune4C_13TeV_pythia8_Spring14miniaod"]=2939491;
  totalEventsTable["QCD_Pt-3200_Tune4C_13TeV_pythia8_Spring14miniaod"]=2953151;
  totalEventsTable["QCD_Pt-300to470_Tune4C_13TeV_pythia8_Phys14DR"]=1986177;
  totalEventsTable["QCD_Pt-470to600_Tune4C_13TeV_pythia8_Phys14DR"]=2001071;
  totalEventsTable["QCD_Pt-600to800_Tune4C_13TeV_pythia8_Phys14DR-PU20bx25_trkalmb_castor_PHYS14_25_V1-v1_MINIAODSIM"]=1997744;
  totalEventsTable["QCD_Pt-800to1000_Tune4C_13TeV_pythia8_Phys14DR"]=1000065;
  totalEventsTable["QCD_Pt-1000to1400_Tune4C_13TeV_pythia8_Phys14DR"]=500550;
  totalEventsTable["QCD_Pt-1400to1800_Tune4C_13TeV_pythia8_Phys14DR"]=199627;
  totalEventsTable["QCD_Pt-1800to2400_Tune4C_13TeV_pythia8_Phys14DR"]=200079;
  totalEventsTable["QCD_Pt-2400to3200_Tune4C_13TeV_pythia8_Phys14DR"]=200453;
  totalEventsTable["QCD_Pt-3200_Tune4C_13TeV_pythia8_Phys14DR"]=200200;
  // totalEventsTable["QCD_Pt-600to800_Tune4C_13TeV_pythia8_rs_Phys14DR-AVE20BX25_tsg_castor_PHYS14_25_V3-v1_rs_MINIAODSIM_v78"]=2857014;
  totalEventsTable["QCD_HT_250To500_13TeV-madgraph_Spring14miniaod-PU20bx25_PLS170_V7AN1-v1_MINIAODSIM_UCSB2353_v77"]=655322;
  totalEventsTable["QCD_HT-500To1000_13TeV-madgraph_Spring14miniaod-PU20bx25_PLS170_V7AN1-v1_MINIAODSIM_UCSB2354_v77"]=842193;
  totalEventsTable["QCD_HT_1000ToInf_13TeV-madgraph_Spring14miniaod-PU20bx25_PLS170_V7AN1-v2_MINIAODSIM_UCSB2356_v77"]=333882;
  totalEventsTable["QCD_HT-100To250_13TeV-madgraph_Phys14DR-PU20bx25_PHYS14_25_V1-v1_MINIAODSIM_UCSB2295_v77"]=4123612;
  totalEventsTable["QCD_HT_250To500_13TeV-madgraph_Phys14DR-PU20bx25_PHYS14_25_V1-v1_MINIAODSIM_UCSB2308_v77"]=663953+2004219;
  totalEventsTable["QCD_HT_250To500_13TeV-madgraph_Phys14DR-PU20bx25_PHYS14_25_V1_ext1-v2_MINIAODSIM_UCSB2309_v77"]=663953+2004219;
  totalEventsTable["QCD_HT-500To1000_13TeV-madgraph_Phys14DR-PU20bx25_PHYS14_25_V1-v1_MINIAODSIM_UCSB2293_v77"]=849033+3214312;
  totalEventsTable["QCD_HT-500To1000_13TeV-madgraph_Phys14DR-PU20bx25_PHYS14_25_V1_ext1-v1_MINIAODSIM_UCSB2294_v77"]=849033+3214312;
  totalEventsTable["QCD_HT_1000ToInf_13TeV-madgraph_Phys14DR-PU20bx25_PHYS14_25_V1-v1_MINIAODSIM_UCSB2296_v77"]=333733+1130720;
  totalEventsTable["QCD_HT_1000ToInf_13TeV-madgraph_Phys14DR-PU20bx25_PHYS14_25_V1_ext1-v1_MINIAODSIM_UCSB2297_v77"]=333733+1130720;
  totalEventsTable["ZJetsToNuNu_HT-100to200_Tune4C_13TeV-madgraph-tauola_Spring14miniaod"]=4950929;
  totalEventsTable["ZJetsToNuNu_HT-200to400_Tune4C_13TeV-madgraph-tauola_Spring14miniaod"]=4548024;
  totalEventsTable["ZJetsToNuNu_HT-400to600_Tune4C_13TeV-madgraph-tauola_Spring14miniaod"]=4453911;
  totalEventsTable["ZJetsToNuNu_HT-600toInf_Tune4C_13TeV-madgraph-tauola_Spring14miniaod"]=4428190;
  totalEventsTable["ZJetsToNuNu_HT-100to200_Tune4C_13TeV-madgraph-tauola_Phys14DR"]=4986424;
  totalEventsTable["ZJetsToNuNu_HT-200to400_Tune4C_13TeV-madgraph-tauola_Phys14DR"]=4546470;
  totalEventsTable["ZJetsToNuNu_HT-400to600_Tune4C_13TeV-madgraph-tauola_Phys14DR"]=4433784;
  totalEventsTable["ZJetsToNuNu_HT-600toInf_Tune4C_13TeV-madgraph-tauola_Phys14DR"]=4463806-49990;
  totalEventsTable["TToLeptons_s-channel-CSA14_Tune4C_13TeV-aMCatNLO-tauola_Spring14miniaod"]=499400;
  totalEventsTable["TToLeptons_t-channel-CSA14_Tune4C_13TeV-aMCatNLO-tauola_Spring14miniaod"]=3929200;
  totalEventsTable["T_tW-channel-DR_Tune4C_13TeV-CSA14-powheg-tauola_Spring14miniaod"]=986100;
  totalEventsTable["TBarToLeptons_s-channel-CSA14_Tune4C_13TeV-aMCatNLO-tauola_Spring14miniaod"]=248800;
  totalEventsTable["TBarToLeptons_t-channel_Tune4C_CSA14_13TeV-aMCatNLO-tauola_Spring14miniaod"]=1866100;
  totalEventsTable["Tbar_tW-channel-DR_Tune4C_13TeV-CSA14-powheg-tauola_Spring14miniaod"]=971800;
  totalEventsTable["TToLeptons_s-channel-CSA14_Tune4C_13TeV-aMCatNLO-tauola_Phys14DR"]=500000;
  totalEventsTable["TToLeptons_t-channel-CSA14_Tune4C_13TeV-aMCatNLO-tauola_Phys14DR"]=3991000;
  totalEventsTable["T_tW-channel-DR_Tune4C_13TeV-CSA14-powheg-tauola_Phys14DR"]=986100;
  totalEventsTable["TBarToLeptons_s-channel-CSA14_Tune4C_13TeV-aMCatNLO-tauola_Phys14DR"]=250000;
  totalEventsTable["TBarToLeptons_t-channel_Tune4C_CSA14_13TeV-aMCatNLO-tauola_Phys14DR"]=1999800;
  totalEventsTable["Tbar_tW-channel-DR_Tune4C_13TeV-CSA14-powheg-tauola_Phys14DR"]=971800;
  totalEventsTable["ZH_HToBB_ZToLL_M-125_13TeV_powheg-herwigpp"]=198566;
  totalEventsTable["ZH_HToBB_ZToNuNu_M-125_13TeV_powheg-herwigpp"]=195648;
  totalEventsTable["WH_HToBB_WToLNu_M-125_13TeV_powheg-herwigpp"]=200000;
  totalEventsTable["TTbarH_M-125_13TeV_amcatnlo-pythia8-tauola_Spring14miniaod-PU20bx25_POSTLS170_V5-v1_MINIAODSIM_UCSB2152_v75"]=199000;
  totalEventsTable["TTbarH_M-125_13TeV_amcatnlo-pythia8-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v2_MINIAODSIM"]=199700;
  totalEventsTable["TTZJets_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1_MINIAODSIM"]=249275;
  totalEventsTable["TTWJets_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1_MINIAODSIM"]=246521;
  totalEventsTable["DYJetsToLL_M-50_HT-100to200_Tune4C_13TeV"]=3633185;
  totalEventsTable["DYJetsToLL_M-50_HT-200to400_Tune4C_13TeV"]=4665825;
  totalEventsTable["DYJetsToLL_M-50_HT-400to600_Tune4C_13TeV"]=4944926;
  totalEventsTable["DYJetsToLL_M-50_HT-600toInf_Tune4C_13TeV"]=4510506;
  totalEventsTable["Neutrino_Pt-2to20_gun_13TeV-Spring14miniaod"]=39058619;
  totalEventsTable["RelValTTbar_13TeV-MINIAODSIM-PU25ns_PHYS14_25_V1_Phys14-v1_v73"]=9000;
  totalEventsTable["GJets_HT-100to200_Tune4C_13TeV"]=4734234;
  totalEventsTable["GJets_HT-200to400_Tune4C_13TeV"]=4533420;
  totalEventsTable["GJets_HT-400to600_Tune4C_13TeV"]=4560801;
  totalEventsTable["GJets_HT-600toInf_Tune4C_13TeV"]=4341179;
}
