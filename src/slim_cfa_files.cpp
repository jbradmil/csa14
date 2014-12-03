//Skims reduced trees, outputs to reduced_trees/skimmed

#include <string>
#include <iostream>
#include <unistd.h>
#include "TFile.h"
#include "TChain.h"
#include "TDirectory.h"

using namespace std;

void SlimCFAFiles(string inFilename, string outFilename) {
  TChain *inChA = new TChain("configurableAnalysis/eventA");
  TChain *inChB = new TChain("configurableAnalysis/eventB");
  cout << "Input: " << inFilename << endl;
  inChA->Add((inFilename).c_str()); 
  inChB->Add((inFilename).c_str());
  cout << "Output: " << outFilename << endl;

  TFile* outfile = new TFile(outFilename.c_str(),"recreate");
  outfile->mkdir("configurableAnalysis");
  outfile->cd("configurableAnalysis");
  // dir->cd();

  // turn off branches
  inChB->SetBranchStatus("els_status", 0);
  inChB->SetBranchStatus("els_simpleEleId95relIso", 0);
  inChB->SetBranchStatus("els_simpleEleId90relIso", 0);
  inChB->SetBranchStatus("els_simpleEleId85relIso", 0);
  inChB->SetBranchStatus("els_simpleEleId80relIso", 0);
  inChB->SetBranchStatus("els_simpleEleId70relIso", 0);
  inChB->SetBranchStatus("els_simpleEleId60relIso", 0);
  inChB->SetBranchStatus("els_simpleEleId95cIso", 0);
  inChB->SetBranchStatus("els_simpleEleId90cIso", 0);
  inChB->SetBranchStatus("els_simpleEleId85cIso", 0);
  inChB->SetBranchStatus("els_simpleEleId80cIso", 0);
  inChB->SetBranchStatus("els_simpleEleId70cIso", 0);
  inChB->SetBranchStatus("els_simpleEleId60cIso", 0);
  inChB->SetBranchStatus("jets_AK4_status", 0);
  inChB->SetBranchStatus("jets_AK4_gen_Id", 0);
  inChB->SetBranchStatus("jets_AK4_gen_motherID", 0);
  inChB->SetBranchStatus("jets_AK4_gen_threeCharge", 0);
  inChB->SetBranchStatus("jets_AK4_ehf", 0);
  inChB->SetBranchStatus("jets_AK4_emf", 0);
  inChB->SetBranchStatus("jets_AK4_n90Hits", 0);
  inChB->SetBranchStatus("jets_AK4_fHPD", 0);
  inChB->SetBranchStatus("jets_AK4_fRBX", 0);
  inChB->SetBranchStatus("jets_AK4_hitsInN90", 0);
  inChB->SetBranchStatus("jets_AK4_nECALTowers", 0);
  inChB->SetBranchStatus("jets_AK4_nHCALTowers", 0);
  inChB->SetBranchStatus("jets_AK4_fSubDetector1", 0);
  inChB->SetBranchStatus("jets_AK4_fSubDetector2", 0);
  inChB->SetBranchStatus("jets_AK4_fSubDetector3", 0);
  inChB->SetBranchStatus("jets_AK4_fSubDetector4", 0);
  inChB->SetBranchStatus("jets_AK4_area", 0);
  inChB->SetBranchStatus("mus_status", 0);
  inChB->SetBranchStatus("mus_ecalvetoDep", 0);
  inChB->SetBranchStatus("mus_hcalvetoDep", 0);
  inChB->SetBranchStatus("mus_status", 0);
  inChB->SetBranchStatus("mus_status", 0);
  inChB->SetBranchStatus("mus_status", 0);
  inChB->SetBranchStatus("mus_isCaloMuon", 0);
  inChB->SetBranchStatus("mus_isElectron", 0);
  inChB->SetBranchStatus("mus_isConvertedPhoton", 0);
  inChB->SetBranchStatus("mus_isPhoton", 0);
  inChB->SetBranchStatus("mus_picky*", 0);
  inChB->SetBranchStatus("mus_tpfms*", 0);
  inChB->SetBranchStatus("mus_picky*", 0);
  inChB->SetBranchStatus("photons_status", 0);
  inChB->SetBranchStatus("photons_isAlsoElectron", 0);
  inChB->SetBranchStatus("photons_hasPixelSeed", 0);
  inChB->SetBranchStatus("photons_isConverted", 0);
  inChB->SetBranchStatus("photons_isEBEEGap", 0);
  inChB->SetBranchStatus("pv_isFake", 0);
  inChB->SetBranchStatus("pv_tracksSize", 0);
  inChB->SetBranchStatus("taus_status", 0);
  inChB->SetBranchStatus("taus_emf", 0);
  inChB->SetBranchStatus("taus_hcalTotOverPLead", 0);
  inChB->SetBranchStatus("taus_hcalMaxOverPLead", 0);
  inChB->SetBranchStatus("taus_hcal3x3OverPLead", 0);
  inChB->SetBranchStatus("taus_ecalStripSumEOverPLead", 0);
  inChB->SetBranchStatus("taus_elecPreIdOutput", 0);
  inChB->SetBranchStatus("taus_elecPreIdDecision", 0);
  inChB->SetBranchStatus("taus_leadPFChargedHadrCand*", 0);
  inChB->SetBranchStatus("taus_isoPFGammaCandsEtSum", 0);
  inChB->SetBranchStatus("taus_isoPFChargedHadrCandsPtSum", 0);
  inChB->SetBranchStatus("taus_leadingTrackFinding", 0);
  inChB->SetBranchStatus("taus_leadingTrackPtCut", 0);
  inChB->SetBranchStatus("taus_trackIsolation", 0);
  inChB->SetBranchStatus("taus_ecalIsolation", 0);
  inChB->SetBranchStatus("taus_byIsolation", 0);
  inChB->SetBranchStatus("taus_againstElectron", 0);
  inChB->SetBranchStatus("taus_againstMuon", 0);
  inChB->SetBranchStatus("taus_taNC*", 0);
  inChB->SetBranchStatus("taus_byIsoUsingLeadingPi", 0);
  inChB->SetBranchStatus("taus_tkIsoUsingLeadingPi", 0);
  inChB->SetBranchStatus("taus_ecalIsoUsingLeadingPi", 0);
  inChB->SetBranchStatus("taus_trackIsolation", 0);
  inChB->SetBranchStatus("taus_byVLooseIsolation", 0);
  inChB->SetBranchStatus("taus_byLooseIsolation", 0);
  inChB->SetBranchStatus("taus_byMediumIsolation", 0);
  inChB->SetBranchStatus("taus_byTightIsolation", 0);
  inChB->SetBranchStatus("taus_byVLooseIsolationDeltaBetaCorr", 0);
  inChB->SetBranchStatus("taus_byLooseIsolationDeltaBetaCorr", 0);
  inChB->SetBranchStatus("taus_byMediumIsolationDeltaBetaCorr", 0);
  inChB->SetBranchStatus("taus_byTightIsolationDeltaBetaCorr", 0);
  inChB->SetBranchStatus("taus_signalPFChargedHadrCandsSize", 0);
  inChB->SetBranchStatus("taus_muDecision", 0);
  inChB->SetBranchStatus("taus_Nprongs", 0);


  TTree *outChA = inChA->CopyTree("");
  TTree *outChB = inChB->CopyTree("");
  outChA->Write();
  outChB->Write();
  outfile->Close();

}

int main(int argc, char *argv[]){
  std::string inFilename("");
  int c(0);
  while((c=getopt(argc, argv, "i:"))!=-1){
    switch(c){
    case 'i':
      inFilename=optarg;
      break;
    default:
      break;
    }
  }

  std::string outFilename("");
  std::string baseName(inFilename);  
  std::string dirName(inFilename);
  size_t pos=baseName.find(".root");
  if(pos!=std::string::npos){
    baseName.erase(pos);
  }
  pos=baseName.rfind("/");
  if(pos!=std::string::npos){
    baseName.erase(0,pos+1);
  }
  pos=dirName.rfind("/");
  dirName.erase(pos+1);
  // cout << dirName << endl;
  outFilename=dirName+"slimmed/"+baseName+".root";
  
  SlimCFAFiles(inFilename,outFilename);
}
