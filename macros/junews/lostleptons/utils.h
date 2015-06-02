#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include "macros/junews/lostleptons/histio.c"

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

// from Owen
TH1D* make_frac( TH1D* hp_numer, TH1D* hp_denom, const char* hname, const char* htitle ) {

  if ( hp_numer == 0x0 || hp_denom == 0x0 ) return 0x0 ;

  int nbins = hp_numer->GetNbinsX() ;

  printf("\n make_frac: creating %s, %s\n\n", hname, htitle ) ;
  //TH1D* h_fraction = new TH1D( hname, htitle, nbins, hp_numer->GetXaxis()->GetXbins() ) ;
  TH1D* h_fraction = (TH1D*) hp_numer -> Clone( hname ) ;
  h_fraction -> Reset() ;
  h_fraction -> SetTitle( htitle ) ;

  for ( int bi=1; bi<=nbins; bi++ ) {
    double all_val = hp_denom -> GetBinContent( bi ) ;
    double all_err = hp_denom -> GetBinError( bi ) ;
    double lowpt_val = hp_numer -> GetBinContent( bi ) ;
    double lowpt_err = hp_numer -> GetBinError( bi ) ;
    double frac_val = 0. ;
    double frac_err = 0. ;
    if ( all_val > 0 ) {
      frac_val = lowpt_val / all_val ;
      double a_val = lowpt_val ;
      double b_val = all_val - lowpt_val ;
      double a_err = lowpt_err ;
      double b_err = sqrt( all_err*all_err - lowpt_err*lowpt_err ) ;
      frac_err = (1./(all_val*all_val))*sqrt( pow( b_val*a_err, 2. ) + pow( a_val*b_err, 2. ) ) ;
    }
    h_fraction -> SetBinContent( bi, frac_val ) ;
    h_fraction -> SetBinError( bi, frac_err ) ;
  } // bi.

  return h_fraction ;

} // make_frac

  //===================================================================================================================


TH1D* make_ratio( TH1D* hp_numer, TH1D* hp_denom, const char* hname, const char* htitle ) {

  if ( hp_numer == 0x0 || hp_denom == 0x0 ) return 0x0 ;

  int nbins = hp_numer->GetNbinsX() ;

  printf("\n make_ratio: creating %s, %s\n\n", hname, htitle ) ;
  TH1D* h_ratio = (TH1D*) hp_numer -> Clone( hname ) ;
  h_ratio -> Reset() ;
  h_ratio -> SetTitle( htitle ) ;

  for ( int bi=1; bi<=nbins; bi++ ) {
    double found_val = hp_denom -> GetBinContent( bi ) ;
    double found_err = hp_denom -> GetBinError( bi ) ;
    double lost_val = hp_numer -> GetBinContent( bi ) ;
    double lost_err = hp_numer -> GetBinError( bi ) ;
    double ratio_val = 0. ;
    double ratio_err = 0. ;
    if ( found_val > 0. && lost_val > 0. ) {
      ratio_val = lost_val / found_val ;
      ratio_err = ratio_val * sqrt( pow( found_err/found_val, 2 ) + pow( lost_err/lost_val, 2 ) ) ;
    }
    if ( ratio_err>ratio_val ) ratio_err=ratio_val; // Jack--sanity hack
    printf("  bin %2d :  lost = %6.1f +/- %4.1f,  found = %6.1f +/- %4.1f,  ratio = %6.3f +/- %6.3f\n",
	   bi, lost_val, lost_err, found_val, found_err, ratio_val, ratio_err ) ;
    h_ratio -> SetBinContent( bi, ratio_val ) ;
    h_ratio -> SetBinError( bi, ratio_err ) ;
  } // bi.

  return h_ratio ;

} // make_ratio

double pdf_mht_bin_integral( TH1D* hist, int mbi, double ptw, int nmhtbins, double mhtbins[] ) {

   if ( hist == 0x0 ) return -1. ;
   if ( mbi < 1 ) return -1. ;
   if ( mbi >= nmhtbins ) return -1. ;
   if ( ptw <= 0 ) return -1. ;

   double hist_integral = hist -> Integral() ;
   if ( hist_integral <=0 ) return 0. ;

   double ret_val(0.) ;

   double hist_xl = mhtbins[mbi] / ptw ;
   double hist_xh = mhtbins[mbi+1] / ptw ;
   if ( hist_xl > 1. ) return 0. ;
   if ( hist_xl < 1. && hist_xh > 1. ) {
      int hist_binl = hist -> GetXaxis() -> FindBin( hist_xl ) ;
      int hist_binh = hist -> GetNbinsX() ;
      ret_val = hist -> Integral( hist_binl, hist_binh ) / hist_integral ;
   } else if ( hist_xh < 1. ) {
      int hist_binl = hist -> GetXaxis() -> FindBin( hist_xl ) ;
      int hist_binh = hist -> GetXaxis() -> FindBin( hist_xh ) ;
      ret_val = hist -> Integral( hist_binl, hist_binh ) / hist_integral ;
   } else {
      printf(" wtf???\n") ;
   }

   return ret_val ;

} // pdf_mht_bin_integral

//==============================================================================================

void   make_mht_hist_entries( TH1D* h_lostlep_mht, TH1D* hist, double rec_w_pt, double lostfound_ratio, bool verb=false ) {

   if ( h_lostlep_mht == 0x0 ) return;
   if ( hist == 0x0 ) return ;

   if ( verb ) printf("\n\n   make_mht_hist_entries : rec_w_pt = %6.1f\n\n", rec_w_pt ) ;

   if ( verb ) hist -> Print("all") ;
   double hist_integral = hist -> Integral() ;
   if ( hist_integral <= 0 ) return ;

   for ( int bi=1; bi<= hist->GetNbinsX(); bi++ ) {
      double weight = lostfound_ratio * (hist -> GetBinContent( bi )) / hist_integral ;
      double mht_val = rec_w_pt * (hist -> GetXaxis() -> GetBinCenter( bi )) ;
      if (verb) printf( " bin %2d : MHT = %6.1f, weight = %6.3f\n", bi, mht_val, weight ) ;
      h_lostlep_mht -> Fill( mht_val, weight ) ;
   } // bi

   if ( verb ) printf("\n ---------------------------------- \n\n") ;

} // make_mht_hist_entries

//==============================================================================================

void   set_r_hist_labels( TH1D* hp ) {

   if ( hp == 0x0 ) return ;

   TAxis* xaxis = hp -> GetXaxis() ;

   xaxis -> SetBinLabel(  2, "Njet1-Nb0" ) ;
   xaxis -> SetBinLabel(  3, "Njet1-Nb1" ) ;
   xaxis -> SetBinLabel(  4, "Njet1-Nb2" ) ;
   xaxis -> SetBinLabel(  5, "Njet1-Nb3" ) ;

   xaxis -> SetBinLabel(  7, "Njet2-Nb0" ) ;
   xaxis -> SetBinLabel(  8, "Njet2-Nb1" ) ;
   xaxis -> SetBinLabel(  9, "Njet2-Nb2" ) ;
   xaxis -> SetBinLabel( 10, "Njet2-Nb3" ) ;

   xaxis -> SetBinLabel( 12, "Njet3-Nb0" ) ;
   xaxis -> SetBinLabel( 13, "Njet3-Nb1" ) ;
   xaxis -> SetBinLabel( 14, "Njet3-Nb2" ) ;
   xaxis -> SetBinLabel( 15, "Njet3-Nb3" ) ;

   xaxis -> LabelsOption( "v" ) ;

} // set_r_hist_labels

//==============================================================================================

void   set_events_hist_labels( TH1D* hp ) {

   if ( hp == 0x0 ) return ;

   TAxis* xaxis = hp -> GetXaxis() ;

   xaxis -> SetBinLabel(  2, "MHT2_HT1" ) ;
   xaxis -> SetBinLabel(  3, "MHT2_HT2" ) ;
   xaxis -> SetBinLabel(  4, "MHT2_HT3" ) ;

   xaxis -> SetBinLabel(  6, "MHT3_HT2" ) ;
   xaxis -> SetBinLabel(  7, "MHT3_HT3" ) ;

   xaxis -> LabelsOption( "v" ) ;

} // set_events_hist_labels

//==============================================================================================

double pdf_mht_bin_integral( TH1D* hist, int mbi, double ptw ) {

  int nmhtbins = 4 ;  // first bin is not used in analysis.
  double mhtbins[5] = { 0., 200., 500., 750., 20000. } ;  // first bin is not used in analysis.

  
   if ( hist == 0x0 ) return -1. ;
   if ( mbi < 1 ) return -1. ;
   if ( mbi >= nmhtbins ) return -1. ;
   if ( ptw <= 0 ) return -1. ;

   double hist_integral = hist -> Integral() ;
   if ( hist_integral <=0 ) return 0. ;

   double ret_val(0.) ;

   double hist_xl = mhtbins[mbi] / ptw ;
   double hist_xh = mhtbins[mbi+1] / ptw ;
   if ( hist_xl > 1. ) return 0. ;
   if ( hist_xl < 1. && hist_xh > 1. ) {
      int hist_binl = hist -> GetXaxis() -> FindBin( hist_xl ) ;
      int hist_binh = hist -> GetNbinsX() ;
      ret_val = hist -> Integral( hist_binl, hist_binh ) / hist_integral ;
   } else if ( hist_xh < 1. ) {
      int hist_binl = hist -> GetXaxis() -> FindBin( hist_xl ) ;
      int hist_binh = hist -> GetXaxis() -> FindBin( hist_xh ) ;
      ret_val = hist -> Integral( hist_binl, hist_binh ) / hist_integral ;
   } else {
      printf(" wtf???\n") ;
   }

   return ret_val ;

} // pdf_mht_bin_integral

//=====
