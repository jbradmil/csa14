#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <iostream>
#include <iomanip>


void getSkimCutflow(TString files) {
  
  TChain *fChain = new TChain("cutFlow");   //!pointer to the analyzed TTree or TChain
   // Declaration of leaf types
   UInt_t          startCount_;
   UInt_t          met200Count_;
   UInt_t          numJetsCount_;
   UInt_t          numCSVMCount_;
   UInt_t          minDeltaPhiCount_;
   UInt_t          muonVetoCount_;
   UInt_t          electronVetoCount_;

   // List of branches
   TBranch        *b_startCount;   //!
   TBranch        *b_met200Count;   //!
   TBranch        *b_numJetsCount;   //!
   TBranch        *b_numCSVMCount;   //!
   TBranch        *b_minDeltaPhiCount;   //!
   TBranch        *b_muonVetoCount;   //!
   TBranch        *b_electronVetoCount;   //!

   fChain->SetBranchAddress("startCount", &startCount_, &b_startCount);
   fChain->SetBranchAddress("met200Count", &met200Count_, &b_met200Count);
   fChain->SetBranchAddress("numJetsCount", &numJetsCount_, &b_numJetsCount);
   fChain->SetBranchAddress("numCSVMCount", &numCSVMCount_, &b_numCSVMCount);
   fChain->SetBranchAddress("minDeltaPhiCount", &minDeltaPhiCount_, &b_minDeltaPhiCount);
   fChain->SetBranchAddress("muonVetoCount", &muonVetoCount_, &b_muonVetoCount);
   fChain->SetBranchAddress("electronVetoCount", &electronVetoCount_, &b_electronVetoCount);

   fChain->Add(files);

   int startCount(0);
   int met200Count(0);
   int numJetsCount(0);
   int numCSVMCount(0);
   int minDeltaPhiCount(0);
   int muonVetoCount(0);
   int electronVetoCount(0);

  for (unsigned int entry=0; entry<fChain->GetEntries(); entry++) {
    fChain->GetEntry(entry);
    startCount+=startCount_;
    met200Count+=met200Count_;
    numJetsCount+=numJetsCount_;
    numCSVMCount+=numCSVMCount_;
    minDeltaPhiCount+=minDeltaPhiCount_;
    muonVetoCount+=muonVetoCount_;
    electronVetoCount+=electronVetoCount_;
  }

  printf("*****Cutflow*****\n");
  printf("startCount\t\t\t%d\n",startCount);
  printf("met200Count\t\t\t%d\n",met200Count);
  printf("numJetsCount\t\t\t%d\n",numJetsCount);
  printf("numCSVMCount\t\t\t%d\n",numCSVMCount);
  printf("minDeltaPhiCount\t\t%d\n",minDeltaPhiCount);
  printf("muonVetoCount\t\t\t%d\n",muonVetoCount);
  printf("electronVetoCount\t\t%d\n",electronVetoCount);

   return;

}
