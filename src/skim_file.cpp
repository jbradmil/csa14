//Skims MC backgrounds using the full path to a local .root file (/path/to/local/file.root) or cfA sample name (TTJets_blahblahblah)

#include <string>
#include <iostream>
#include <unistd.h>
#include "TSystem.h"
#include "cfa_skimmer.hpp"
#include "event_handler.hpp"
#include "weights.hpp"

int main(int argc, char *argv[]){
  std::string inFilename("");
  bool iscfA(false), isList(false);
  int c(0);
  int Nentries(-1);
  while((c=getopt(argc, argv, "i:n:cl"))!=-1){
    switch(c){
    case 'i':
      inFilename=optarg;
      break;
    case 'n':
      Nentries=atoi(optarg);
      break;
    case 'c':
      iscfA=true;
      break;
    case 'l':
      isList=true;
      break;
    default:
      break;
    }
  }

  std::string outFilename("");
  std::string dirName("skims/");
  std::string energy("");
  if (inFilename.find("v71")!=std::string::npos || inFilename.find("v74")!=std::string::npos) energy="8TeV";
  else energy = "13TeV";
  if(iscfA){
    dirName=dirName+energy+"/"+inFilename;
    outFilename=dirName+"/"+inFilename+".root";
    inFilename="/net/cms2/cms2r0/cfA/"+inFilename+"/cfA_"+inFilename+"*.root";
  }else{
    std::string baseName(inFilename);
    size_t pos(baseName.find(".root"));
    if(pos!=std::string::npos)  baseName.erase(pos);
    pos=baseName.find(".list");
    if(pos!=std::string::npos)  baseName.erase(pos);
    pos=baseName.rfind("/");
    if(pos!=std::string::npos){
      if(pos!=baseName.size()-1){
        baseName.erase(0,pos+1);
      }else{
        baseName.append("file_name_ended_with_slash");
      }
    }
    dirName=baseName;
    pos=dirName.find(".part");
    if(pos!=std::string::npos)  dirName.erase(pos);
    dirName="skims/"+energy+"/"+dirName;
    outFilename=dirName+"/"+baseName+".root";
    std::cout << inFilename << "\n" << baseName << "\n" << outFilename << "\n";
  }

  if (gSystem->AccessPathName(dirName.c_str())) {
    std::cout << "Making directory " << dirName << std::endl;
    gSystem->mkdir(dirName.c_str());
  }

  CfASkimmer cs(inFilename, isList, Nentries);
  cs.Skim(outFilename);
}

/*
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
    }*/
