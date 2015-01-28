#include <iostream>
#include <string>
#include <unistd.h>
#include "object_printer.hpp"

using std::cout;
using std::endl;

int main(int argc, char *argv[]){
  std::string inFilename("");
   bool iscfA(false), isList(false);
  int c(0);
  int Nentries(1);
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

  if(iscfA){
    inFilename="/net/cms2/cms2r0/cfA/"+inFilename+"/cfA_"+inFilename+"*.root";
  }

  ObjectPrinter objp(inFilename, isList);
  objp.PrintObjects(Nentries);
}
