//Skims reduced trees, outputs to reduced_trees/skimmed

#include <string>
#include <iostream>
#include <unistd.h>
#include "TFile.h"
#include "TChain.h"
#include "TCut.h"
#include "remove_duplicate_events.hpp"

using namespace std;

void SkimReducedTree(string inFilename, string outFilename) {
  TChain *inCh = new TChain("reduced_tree");
  cout << "Input: " << inFilename+"*root" << endl;
  inCh->Add((inFilename+"*root").c_str()); // give it reduced_trees/SAMPLE_NAME/
  cout << "Output: " << outFilename << endl;

  // remove_duplicate_events(inCh);


  TFile* outfile = new TFile(outFilename.c_str(),"recreate");
  outfile->cd();
  TCut ht("ht30>500"), njets("jet4_pt>30"), mht30("mht30>200"), mdp("min_delta_phi_met_N>4"), nb("num_csvm_jets30>=2"), presel(ht+njets+mht30+mdp);
  //TTree *outCh = inCh->CopyTree("(fatpT30_MJ>400||ht30>750)&&met>200&&num_csvm_jets30>1&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0");
  // 13 TeV
  TTree *outCh = inCh->CopyTree(ht+njets+mht30);
  //  TTree *outCh = inCh->CopyTree(ht+njets+"num_csvm_jets30>=2&&min_delta_phi_met_N>6&&lumiblock<400000");
  // 8 TeV  
  //  TTree *outCh = inCh->CopyTree("jet4_pt>40&&passesJSONCut&&passesPVCut&&passes2012METCleaningCut&&met>200&&ht40>500&&num_reco_veto_muons==0&&num_reco_veto_electrons==0");
  cout << "Saved " << outCh->GetEntries() << " events." << endl;
  outCh->Write();
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
  size_t pos=baseName.rfind("/");
  if(pos!=std::string::npos){
    baseName.erase(pos);
  }
  pos=baseName.rfind("/");
  if(pos!=std::string::npos){
    baseName.erase(0,pos+1);
  }
  // 13 TeV
  outFilename="reduced_trees/13TeV/skimmed/"+baseName+"_skimmed.root";
  //outFilename="reduced_trees/13TeV/skimmed/test_dataset/"+baseName+"_skimmed.root";
  //outFilename="reduced_trees/13TeV/skimmed/test_mc/"+baseName+"_skimmed.root";
  // 8 TeV  
  //  outFilename="reduced_trees/8TeV/skimmed/"+baseName+"_skimmed.root";
  SkimReducedTree(inFilename,outFilename);
}
