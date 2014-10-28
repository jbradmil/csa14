//Skims reduced trees, outputs to reduced_trees/skimmed

#include <string>
#include <iostream>
#include <unistd.h>
#include "TFile.h"
#include "TChain.h"

using namespace std;

void SkimReducedTree(string inFilename, string outFilename) {
  TChain *inCh = new TChain("reduced_tree");
  inCh->Add(inFilename.c_str());
  TFile* outfile = new TFile(outFilename.c_str(),"recreate");
  outfile->cd();
  //TTree *outCh = inCh->CopyTree("(fatpT30_MJ>400||ht30>750)&&met>200&&num_csvm_jets30>1&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0");
  TTree *outCh = inCh->CopyTree("(fatpT30_MJ>400||ht30>750)&&met>200");
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
  size_t pos=baseName.find(".root");
  if(pos!=std::string::npos){
    baseName.erase(pos);
  }
  pos=baseName.rfind("/");
  if(pos!=std::string::npos){
    baseName.erase(0,pos+1);
  }
  outFilename="reduced_trees/skimmed/"+baseName+"_skimmed.root";
  
  SkimReducedTree(inFilename,outFilename);
}
