#include <iostream>
#include <string>
#include <unistd.h>
#include "object_printer.hpp"

using std::cout;
using std::endl;

int main(int argc, char *argv[]){
  std::string inFilename("");
  unsigned int nentries(1);
  int c(0);
  while((c=getopt(argc, argv, "i:n:"))!=-1){
    switch(c){
    case 'i':
      inFilename=optarg;
      break;
    case 'n':
      nentries=atoi(optarg);
      break;
    default:
      break;
    }
  }

  ObjectPrinter objp(inFilename);
  objp.PrintObjects(nentries);
}
