//Skims MC backgrounds using the full path to a local .root file (/path/to/local/file.root) or cfA sample name (TTJets_blahblahblah)

#include <string>
#include <iostream>
#include <unistd.h>
#include "cfa_skimmer.hpp"
#include "event_handler.hpp"
#include "weights.hpp"

int main(int argc, char *argv[]){
  std::string inFilename("");
  bool isList(false), isLocal(false);
  int Nentries(-1);
  int c(0);
  while((c=getopt(argc, argv, "i:n:lL"))!=-1){
    switch(c){
    case 'i':
      inFilename=optarg;
      break;
    case 'n':
      Nentries=atoi(optarg);
      break;
    case 'l':
      isList=true;
    case 'L':
      isLocal=true;
      break;
    default:
      break;
    }
  }

  std::string outFilename("");
  if(isLocal){
    std::string baseName(inFilename);
    size_t pos=baseName.find(".root");
    if(pos!=std::string::npos){
      baseName.erase(pos);
    }
    pos=baseName.rfind("/");
    if(pos!=std::string::npos){
      baseName.erase(0,pos+1);
    }
    outFilename="skims/"+baseName+"_skimmed.root";
  }
  else{
    outFilename="skims/"+inFilename+"_skimmed.root";
    inFilename="/net/cms2/cms2r0/cfA/"+inFilename+"/cfA_"+inFilename+"*.root";
  }

  CfASkimmer cs(inFilename, isList, Nentries);
  cs.Skim(outFilename);
}
