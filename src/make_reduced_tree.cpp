#include <iostream>
#include <string>
#include <unistd.h>
#include "TSystem.h"
#include "reduced_tree_maker.hpp"
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
  std::string dirName("reduced_trees/");
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
    dirName="reduced_trees/"+energy+"/"+dirName;
    outFilename=dirName+"/"+baseName+".root";
    std::cout << inFilename << "\n" << baseName << "\n" << outFilename << "\n";
  }

  if (gSystem->AccessPathName(dirName.c_str())) gSystem->mkdir(dirName.c_str());

  WeightCalculator w(19399,Nentries);
  ReducedTreeMaker rtm(inFilename, isList, w.GetWeightPerPb(inFilename), Nentries);
  rtm.MakeReducedTree(outFilename);
}
