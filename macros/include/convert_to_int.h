#include <iostream>
#include <vector>
#include <math.h> 
#include "TROOT.h"
#include "TFile.h"
#include "TSystem.h"
#include "TChain.h"
#include "TTree.h"
#include "TCut.h"
#include "THStack.h"
#include "TLine.h"
#include "../include/my_style.hpp"


TChain* ch;


using namespace std;

TString plotdir = "plots/";

void convert_to_int(TH1D* hist) {
  hist->Print("all");
  int nbinsx = hist->GetNbinsX();
  //  double total = hist->Integral(1,nbinsx+1);
  // cout << "total: " << total << endl;
  for (int bin(0); bin<nbinsx; bin++) {
    double pass = hist->Integral(bin+1,nbinsx+1);
    cout << "pass: " << pass << endl;
    hist->SetBinContent(bin+1,pass);
  }
  // hist->Print("all");
  return;
}

void convert_to_int_2D(TH2D* hist) {
  hist->Print("all");
  int nbinsx = hist->GetNbinsX();
  int nbinsy = hist->GetNbinsY();
  //  double total = hist->Integral(1,nbinsx+1);
  // cout << "total: " << total << endl;
  for (int binx(0); binx<nbinsx; binx++) {
    for (int biny(0); biny<nbinsy; biny++) {
      hist->SetBinContent(binx+1,biny+1, hist->Integral(binx+1,1000,biny+1,1000));
    }
  }
  // hist->Print("all");
  return;
}


