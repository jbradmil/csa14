#include <TH1.h>
#include <TH2.h>
#include <TH3.h>

void FixOverflow(TH1D* h) {
  int nbinsx = h->GetNbinsX();
  Double_t e_overflow(0.), i_overflow(0.);
  i_overflow=h->IntegralAndError(nbinsx,nbinsx+1,e_overflow);
  h->SetBinContent(nbinsx, i_overflow);
  h->SetBinError(nbinsx, e_overflow);
}

void NormalizeHist(TH1D* h, bool fix_overflow=true) {
  if (fix_overflow) FixOverflow(h);
  double sum = h->Integral(1,h->GetNbinsX());
  h->Scale(1/sum);
}


void FillMHTPDF(TH1D* MHTPDF, TH1D* DTTPDF, double ptW, double RLF) {
  int nbinsDTT = DTTPDF->GetNbinsX();
  //  DTTPDF->Print("all");
  for (int bin(0); bin<nbinsDTT; bin++) {
    MHTPDF->Fill(ptW*DTTPDF->GetBinCenter(bin+1), RLF*DTTPDF->GetBinContent(bin+1)); // treating this as data (no lumi weights)
    //    printf("MHTPDF->Fill(%3.3f*%3.3f, %3.3f*%3.3f);\n",ptW, DTTPDF->GetBinCenter(bin+1), RLF, DTTPDF->GetBinContent(bin+1));
  }
  //  MHTPDF->Print("all");
}
