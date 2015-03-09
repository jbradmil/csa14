//Skims reduced trees, outputs to Adam/skimmed

#include <string>
#include <iostream>
#include <unistd.h>
#include "TFile.h"
#include "TChain.h"
#include "TCut.h"
#include "remove_duplicate_events.hpp"

using namespace std;

void SkimAdamTree(string inFilename, string outFilename) {
  TChain *inCh = new TChain("tree");
  cout << "Input: " << inFilename << endl;
  inCh->Add((inFilename).c_str()); // give it reduced_trees/SAMPLE_NAME/
  cout << "Output: " << outFilename << endl;

  // remove_duplicate_events(inCh);


  TFile* outfile = new TFile(outFilename.c_str(),"recreate");
  outfile->cd();
  TCut ht("ht30>500"), njets("njets30>=4"), mht30("mht30>200"), mdp("mindphin_metjet>4"), nb("nbm30>=2"), presel(ht+njets+mht30+mdp+nb);
  //TTree *outCh = inCh->CopyTree("(fatpT30_MJ>400||ht30>750)&&met>200&&num_csvm_jets30>1&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0");
  // 13 TeV
  TTree *outCh = inCh->CopyTree(ht+njets+mht30);
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
  size_t pos=baseName.rfind(".root");
  if(pos!=std::string::npos){
    baseName.erase(pos);
  }
  pos=baseName.rfind("/");
  if(pos!=std::string::npos){
    baseName.erase(0,pos+1);
  }
  // 13 TeV
  outFilename="Adam/skimmed/"+baseName+"_skimmed.root";
  SkimAdamTree(inFilename,outFilename);
}
