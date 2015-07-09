// Originally developed by Owen Long
// Adapted by Jack Bradmiller-Feld

#define FillInputHists_cxx
#include "FillInputHists.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include "TDirectory.h"

#include "histio.C"
#include "physics.h"

//--------------------------------------------------------------------------------------------------------

int nhtbins = 4 ;   // first bin is not used in analysis.
double htbins[5] = { 0., 500., 800., 1200., 20000. } ;  // first bin is not used in analysis.

////////////int nmhtbins = 4 ;  // first bin is not used in analysis.
////////////double mhtbins[5] = { 0., 200., 500., 750., 20000. } ;  // first bin is not used in analysis.
int nmhtbins = 4 ;  // first bin is not used in analysis.
double mhtbins[5] = { 0., 300., 500., 750., 20000. } ;  // first bin is not used in analysis.

int nnbbins = 4 ;
double nbbins[5] = { -0.5, 0.5, 1.5, 2.5, 20. } ;

int nnjetbins = 4 ; // first bin is not used in analysis.
double njetbins[5] = { 0., 3.5, 6.5, 8.5, 20. } ;  // first bin is not used in analysis.

//-------
int nwptbins_for_rlf(14) ;
double wptbins_for_rlf[15] = { 0., 25., 50., 75., 100., 150., 200., 250., 300., 350., 400., 500., 600., 750., 2000.  } ;
//-------
// int nwptbins_for_cdtt(7) ;
// double wptbins_for_cdtt[8] = { 0., 25., 50., 75., 100., 150., 200., 2000.  } ;
//-------

//--------------------------------------------------------------------------------------------------------

TH1F* make_frac( TH1F* hp_numer, TH1F* hp_denom, const char* hname, const char* htitle ) ;
TH1F* make_ratio( TH1F* hp_numer, TH1F* hp_denom, const char* hname, const char* htitle ) ;

double GetWpT(double lep_pt, double lep_phi, double mht, double mht_phi);
double GetCosDTT(double lep_pt, double lep_phi, double mht, double mht_phi);
double GetMTW(double lep_pt, double lep_phi, double mht, double mht_phi);

void FillInputHists::Run(TString outfiletag)
{

  //--- found leptons must pass these cuts (trigger and control sample selection).
  ///////////double mht_cut = 200. ;
  double mht_cut = 125. ;
  double mt_cut = 100. ;
  double lepton_pt_cut = 10. ;
  double trigger_lepton_pt_cut = 20.;



  if (fChain == 0) return;

  gDirectory -> Delete( "h*" ) ;

  TH1F* h_cdtt_all[5][5] ;
  TH1F* h_cdtt_lostlep[5][5] ;
  TH1F* h_cdtt_foundlep[5][5] ;

  TH1F* h_wpt_all[5][5] ;
  TH1F* h_wpt_lostlep[5][5] ;
  TH1F* h_wpt_foundlep[5][5] ;

  int nbins_cdtt(20) ;
  double xl_cdtt(-0.0001) ;
  double xh_cdtt( 1.0001) ;

  for ( int nji=1; nji<nnjetbins; nji++ ) {
    for ( int hti=1; hti<nhtbins; hti++ ) {

      char hname[1000] ;
      char htitle[1000] ;

      sprintf( hname, "h_wpt_all_nj%d_ht%d", nji, hti ) ;
      sprintf( htitle, "W pT, all, Njet%d, HT%d", nji, hti ) ;
      h_wpt_all[nji][hti] = new TH1F( hname, htitle, nwptbins_for_rlf, wptbins_for_rlf ) ;

      sprintf( hname, "h_wpt_lostlep_nj%d_ht%d", nji, hti ) ;
      sprintf( htitle, "W pT, lostlep, Njet%d, HT%d", nji, hti ) ;
      h_wpt_lostlep[nji][hti] = new TH1F( hname, htitle, nwptbins_for_rlf, wptbins_for_rlf ) ;

      sprintf( hname, "h_wpt_foundlep_nj%d_ht%d", nji, hti ) ;
      sprintf( htitle, "W pT, foundlep, Njet%d, HT%d", nji, hti ) ;
      h_wpt_foundlep[nji][hti] = new TH1F( hname, htitle, nwptbins_for_rlf, wptbins_for_rlf ) ;


	sprintf( hname, "h_cdtt_all_nj%d_ht%d", nji, hti ) ;
	sprintf( htitle, "1/2(1-cos(dtt)), all, Njet%d, HT%d", nji, hti ) ;
	h_cdtt_all[nji][hti] = new TH1F( hname, htitle, nbins_cdtt, xl_cdtt, xh_cdtt ) ;

	sprintf( hname, "h_cdtt_lostlep_nj%d_ht%d", nji, hti ) ;
	sprintf( htitle, "1/2(1-cos(dtt)), lost lepton, Njet%d, HT%d", nji, hti ) ;
	h_cdtt_lostlep[nji][hti] = new TH1F( hname, htitle, nbins_cdtt, xl_cdtt, xh_cdtt ) ;

	sprintf( hname, "h_cdtt_foundlep_nj%d_ht%d", nji, hti ) ;
	sprintf( htitle, "1/2(1-cos(dtt)), found lepton, Njet%d, HT%d", nji, hti) ;
	h_cdtt_foundlep[nji][hti] = new TH1F( hname, htitle, nbins_cdtt, xl_cdtt, xh_cdtt ) ;


    } // hti
  } // nji

  Long64_t nentries = fChain->GetEntries();

  //  if ( max_evts > 0 && max_evts < nentries ) nentries = max_evts ;

  printf("\n\n Looping over %llu events.\n\n", nentries ) ;

  Long64_t nbytes = 0, nb = 0;

  for (Long64_t jentry=0; jentry<nentries;jentry++) {

    if ( jentry%(nentries/100) == 0 && jentry>0 ) { printf("  %9llu out of %9llu (%2lld%%)\n", jentry, nentries, ((100*jentry)/nentries)+1 ) ; fflush(stdout) ; }

    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;

    if ( GenMuNum+GenElecNum != 1 ) continue ;
    double WpT(0.), cos_dTT(0.);
    if (GenMuNum==1) {
      WpT=GetWpT(GenMuPt[0], GenMuPhi[0], MHT, MHTPhi);
      cos_dTT=GetCosDTT(GenMuPt[0], GenMuPhi[0], MHT, MHTPhi);
    } else if(GenElecNum==1) {
      WpT=GetWpT(GenElecPt[0], GenElecPhi[0], MHT, MHTPhi);
      cos_dTT=GetCosDTT(GenElecPt[0], GenElecPhi[0], MHT, MHTPhi);
   }
    
    ///////if ( abs(gen_l1_flav) >= 15 ) continue ; // remove all taus

    // if ( abs(gen_l1_flav) != 15 ) continue ; // ONLY look at taus

    // if ( abs(gen_l1_flav) == 15 && abs(gen_trk1_flav)>15 ) continue ; // remove hadronic taus

    ///if ( mht < 200 ) continue ; // try a MHT cut (for the trigger) to see what happens.

    int nji = -1 ;
    int hti = -1 ;
    int wpti = -1 ;

    for ( int i=1; i<nnjetbins; i++ ) { if ( NJets > njetbins[i] && NJets < njetbins[i+1] ) { nji = i ; break ; } }
    if ( nji < 1 ) continue ;

    for ( int i=1; i<nhtbins  ; i++ ) { if ( HT > htbins[i] && HT < htbins[i+1] ) { hti = i ; break ; } }
    if ( hti < 1 ) continue ;

    // for ( int i=0; i<nwptbins_for_cdtt; i++ )  { if ( WpT > wptbins_for_cdtt[i] && WpT < wptbins_for_cdtt[i+1] ) { wpti = i ; break ; } }
    // if ( wpti < 0 ) continue ;

    h_cdtt_all[nji][hti] -> Fill( 0.5*(1.-cos_dTT) ) ;

    h_wpt_all[nji][hti] -> Fill( WpT ) ;

    bool lost((selectedIDIsoElectronsNum + selectedIDIsoMuonsNum) == 0 && DeltaPhi1>0.5 && DeltaPhi2>0.5 && DeltaPhi3>0.3);
    // bool lost((selectedIDIsoElectronsNum + selectedIDIsoMuonsNum) == 0);
    if ( lost ) {

      h_cdtt_lostlep[nji][hti] -> Fill( 0.5*(1.-cos_dTT) ) ;
      h_wpt_lostlep[nji][hti] -> Fill( WpT ) ;

    } else if ( (selectedIDIsoElectronsNum + selectedIDIsoMuonsNum) == 1  ) {

      double mt_rec_mht(0.), rec_lep_pt(0.);
      if (selectedIDIsoElectronsNum==1) {
	mt_rec_mht=GetMTW(selectedIDIsoElectronsPt[0], selectedIDIsoElectronsPhi[0], MHT, MHTPhi);
	rec_lep_pt=selectedIDIsoElectronsPt[0];
      } else if (selectedIDIsoMuonsNum==1) {
	mt_rec_mht=GetMTW(selectedIDIsoMuonsPt[0], selectedIDIsoMuonsPhi[0], MHT, MHTPhi);
	rec_lep_pt=selectedIDIsoMuonsPt[0];
      }

      bool mu_trigger (selectedIDIsoMuonsNum==1 && selectedIDIsoMuonsPt[0] > 55);
      bool el_trigger (selectedIDIsoElectronsNum==1 && selectedIDIsoElectronsPt[0] > 110);
      bool lep_had_trigger (MHT>mht_cut && rec_lep_pt>trigger_lepton_pt_cut);
      if ( (mu_trigger||el_trigger||lep_had_trigger) && mt_rec_mht<mt_cut ) {
	h_cdtt_foundlep[nji][hti] -> Fill( 0.5*(1.-cos_dTT) ) ;
	h_wpt_foundlep[nji][hti] -> Fill( WpT ) ;
      }
    }

  } // jentry


  for ( int nji=1; nji<nnjetbins; nji++ ) {
    for ( int hti=1; hti<nhtbins; hti++ ) {

      char hname[1000] ;
      char htitle[1000] ;

      sprintf( hname, "h_wpt_lostlep_fraction_nj%d_ht%d", nji, hti ) ;
      sprintf( htitle, "lostlep fraction vs W pT, Njet%d, HT%d", nji, hti ) ;
      make_frac( h_wpt_lostlep[nji][hti], h_wpt_all[nji][hti], hname, htitle ) ;

      sprintf( hname, "h_wpt_foundlep_fraction_nj%d_ht%d", nji, hti ) ;
      sprintf( htitle, "foundlep fraction vs W pT, Njet%d, HT%d", nji, hti ) ;
      make_frac( h_wpt_foundlep[nji][hti], h_wpt_all[nji][hti], hname, htitle ) ;

      sprintf( hname, "h_wpt_lostfound_ratio_nj%d_ht%d", nji, hti ) ;
      sprintf( htitle, "lost/found ratio vs W pT, Njet%d, HT%d", nji, hti ) ;
      make_ratio( h_wpt_lostlep[nji][hti], h_wpt_foundlep[nji][hti], hname, htitle ) ;

    } // hti
  } // nji



  char ofile[10000] ;
  sprintf( ofile, "outputfiles/cdtta-input-hists3-%s.root", outfiletag.Data() ) ;
  printf("\n\n Saving histograms in %s\n", ofile ) ;
  saveHist( ofile, "h*" ) ;
  printf("\n\n\n") ;

} // Loop.


  //===================================================================================================================


TH1F* make_frac( TH1F* hp_numer, TH1F* hp_denom, const char* hname, const char* htitle ) {

  if ( hp_numer == 0x0 || hp_denom == 0x0 ) return 0x0 ;

  int nbins = hp_numer->GetNbinsX() ;

  printf("\n make_frac: creating %s, %s\n\n", hname, htitle ) ;
  //TH1F* h_fraction = new TH1F( hname, htitle, nbins, hp_numer->GetXaxis()->GetXbins() ) ;
  TH1F* h_fraction = (TH1F*) hp_numer -> Clone( hname ) ;
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


TH1F* make_ratio( TH1F* hp_numer, TH1F* hp_denom, const char* hname, const char* htitle ) {

  if ( hp_numer == 0x0 || hp_denom == 0x0 ) return 0x0 ;

  int nbins = hp_numer->GetNbinsX() ;

  printf("\n make_ratio: creating %s, %s\n\n", hname, htitle ) ;
  TH1F* h_ratio = (TH1F*) hp_numer -> Clone( hname ) ;
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
    printf("  bin %2d :  lost = %6.1f +/- %4.1f,  found = %6.1f +/- %4.1f,  ratio = %6.3f +/- %6.3f\n",
	   bi, lost_val, lost_err, found_val, found_err, ratio_val, ratio_err ) ;
    h_ratio -> SetBinContent( bi, ratio_val ) ;
    h_ratio -> SetBinError( bi, ratio_err ) ;
  } // bi.

  return h_ratio ;

} // make_ratio


  //===================================================================================================================

