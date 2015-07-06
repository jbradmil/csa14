//Skims arne trees, outputs to TreeMaker/mc/skimmed

#include <string>
#include <iostream>
#include <unistd.h>
#include "TFile.h"
#include "TChain.h"
#include "TCut.h"
#include "remove_duplicate_events.hpp"

using namespace std;

void SkimArneTree(string inFilename, string outFilename) {
  TChain *inCh = new TChain("TreeMaker2/PreSelection");
  cout << "Input: " << inFilename+"*root" << endl;
  inCh->Add((inFilename+"*root").c_str()); // give it TreeMaker/mc/SAMPLE_NAME/
  cout << "Output: " << outFilename << endl;

  // remove_duplicate_events(inCh);


  TFile* outfile = new TFile(outFilename.c_str(),"recreate");
  outfile->cd();
  TCut ht("HT>500"), njets("NJets>=4"), mht("MHT>200"), mdp("DeltaPhi1>0.5&&DeltaPhi2>0.5&&DeltaPhi3>0.3"), nb("BTags>=2"), presel(ht+njets);
  TCut SR(presel+mdp+mht);
  TCut CR(presel+"selectedIDIsoMuonsNum+selectedIDIsoElectronsNum==1");
  //TTree *outCh = inCh->CopyTree("(fatpT30_MJ>400||ht30>750)&&met>200&&num_csvm_jets30>1&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0");
  // 13 TeV
  TTree *outCh = inCh->CopyTree(CR||SR);
  //else if (type=="SL") outCh = inCh->CopyTree(ht+njets+SL+"mht30>125");
  // TTree *outCh = inCh->CopyTree(ht+njets+"num_csvm_jets30>=2&&lumiblock<400000");
  // TTree *outCh = inCh->CopyTree(ht+njets+"num_csvm_jets30>=2");
  // 8 TeV  
  //  TTree *outCh = inCh->CopyTree("jet4_pt>40&&passesJSONCut&&passesPVCut&&passes2012METCleaningCut&&met>200&&ht40>500&&num_reco_veto_muons==0&&num_reco_veto_electrons==0");
  cout << "Saved " << outCh->GetEntries() << " events." << endl;
  outCh->Write();
  outfile->Close();

}

int main(int argc, char *argv[]){
  std::string inFilename("");
  int c(0);
  while((c=getopt(argc, argv, "i:t:"))!=-1){
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
  size_t pos=baseName.rfind("/");
  if(pos!=std::string::npos){
    baseName.erase(pos);
  }
  pos=baseName.rfind("/");
  if(pos!=std::string::npos){
    baseName.erase(0,pos+1);
  }
  // 13 TeV
  outFilename="TreeMaker/mc/skimmed/"+baseName+"_skimmed.root";
  // outFilename="arne_trees/13TeV/skimmed/test_dataset/"+baseName+"_skimmed.root";
  // outFilename="arne_trees/13TeV/skimmed/test_mc/"+baseName+"_skimmed.root";
  // 8 TeV  
  // outFilename="arne_trees/8TeV/skimmed/"+baseName+"_skimmed.root";
  // if (type=="SL") outFilename="arne_trees/8TeV/skimmed/SL/"+baseName+"_skimmed.root";
  SkimArneTree(inFilename,outFilename);
}
