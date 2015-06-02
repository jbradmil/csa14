// two functions for doing the lost-lepton MHT extrapolation
// developed by Owen Long, adapted by Jack Bradmiller-Feld

#define RTree_cxx
#include "RTree.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include "macros/junews/lostleptons/plotting.h"
#include "macros/junews/lostleptons/utils.h"
#include "macros/junews/lostleptons/RatioError.h"

const int nwptbins_for_rlf(8); // ptW
double wptbins_for_rlf[nwptbins_for_rlf+1] = {200., 250., 300., 350., 400., 500., 600., 750., 2000.} ;
  
int nmhtbins = 4 ;  // first bin is not used in analysis.
double mhtbins[5] = { 0., 200., 500., 750., 20000. } ;  // first bin is not used in analysis.

int nhtbins = 4 ;   // first bin is not used in analysis.
double htbins[5] = { 0., 500., 800., 1200., 20000. } ;  // first bin is not used in analysis.

int nnbbins = 4 ;
double nbbins[5] = { -0.5, 0.5, 1.5, 2.5, 20. } ;

int nnjetbins = 4 ; // first bin is not used in analysis.
double njetbins[5] = { 0., 3.5, 6.5, 8.5, 20. } ;  // first bin is not used in analysis.

int nbins_cdtt(20) ;
double xl_cdtt(-0.0001) ;
double xh_cdtt( 1.0001) ;

void RTree::MakeMHTPDFs(TString outdir, TString input_hist_file, bool verb) {

    //--- found leptons must pass these cuts (trigger and control sample selection).
   /////////////double mht_cut = 200. ;
   double mht_cut = 125. ;
   double mt_cut = 100. ;
   double lepton_pt_cut = 20. ;

   int debug_rsbi_mht2(5) ;

if (fChain == 0) return;

   gDirectory -> Delete( "h*" ) ;

   printf("\n\n Reading input histograms from %s\n\n", input_hist_file.Data() ) ;
   loadHist( input_hist_file ) ;

     
  //---- Define ratio superbins.

  int nrsb_mht2(0) ;
  int nrsb_mht3(0) ;
  int rsuperbin_index_mht2[5][5][5] ; // nb, njet, ht
  int rsuperbin_index_mht3[5][5][5] ; // nb, njet, ht
  char rsuperbin_name_mht2[100][100] ;
  char rsuperbin_name_mht3[100][100] ;

  for ( int nbi=0; nbi<nnbbins; nbi++ ) {
    for ( int nji=1; nji<nnjetbins; nji++ ) {
      for ( int hti=1; hti<nhtbins; hti++ ) {
	rsuperbin_index_mht2[nbi][nji][hti] = -1 ;
	rsuperbin_index_mht3[nbi][nji][hti] = -1 ;
      } // hti
    } // nji
  } // nbi

  //---- Define analysis superbins.

  int nasb_mht2(0) ;
  int nasb_mht3(0) ;
  int asuperbin_index_mht2[5][5][5] ; // nb, njet, ht
  int asuperbin_index_mht3[5][5][5] ; // nb, njet, ht
  char asuperbin_name_mht2[100][100] ;
  char asuperbin_name_mht3[100][100] ;

  for ( int nbi=0; nbi<nnbbins; nbi++ ) {
    for ( int nji=1; nji<nnjetbins; nji++ ) {
      for ( int hti=1; hti<nhtbins; hti++ ) {
	asuperbin_index_mht2[nbi][nji][hti] = -1 ;
	asuperbin_index_mht3[nbi][nji][hti] = -1 ;
      } // hti
    } // nji
  } // nbi

  int asuperbin_rsb_index_mht2[100] ;
  int asuperbin_rsb_index_mht3[100] ;
  for ( int i=0; i<100; i++ ) asuperbin_rsb_index_mht2[i] = -1 ;
  for ( int i=0; i<100; i++ ) asuperbin_rsb_index_mht3[i] = -1 ;

  {
    int nbi, nji, hti ;
    //---- MHT2, HT12

    sprintf( rsuperbin_name_mht2[nrsb_mht2], "MHT2_Nb23_Njet1_HT12" ) ;
    nbi=2; nji=1; hti=1;  rsuperbin_index_mht2[nbi][nji][hti] = nrsb_mht2 ;
    nbi=2; nji=1; hti=2;  rsuperbin_index_mht2[nbi][nji][hti] = nrsb_mht2 ;
    nbi=3; nji=1; hti=1;  rsuperbin_index_mht2[nbi][nji][hti] = nrsb_mht2 ;
    nbi=3; nji=1; hti=2;  rsuperbin_index_mht2[nbi][nji][hti] = nrsb_mht2 ;

    sprintf( asuperbin_name_mht2[nasb_mht2], "MHT2_Nb2_Njet1_HT12" ) ;
    nbi=2; nji=1; hti=1;  asuperbin_index_mht2[nbi][nji][hti] = nasb_mht2 ;
    nbi=2; nji=1; hti=2;  asuperbin_index_mht2[nbi][nji][hti] = nasb_mht2 ;
    asuperbin_rsb_index_mht2[nasb_mht2] = nrsb_mht2 ;
    nasb_mht2 ++ ;
    sprintf( asuperbin_name_mht2[nasb_mht2], "MHT2_Nb3_Njet1_HT12" ) ;
    nbi=3; nji=1; hti=1;  asuperbin_index_mht2[nbi][nji][hti] = nasb_mht2 ;
    nbi=3; nji=1; hti=2;  asuperbin_index_mht2[nbi][nji][hti] = nasb_mht2 ;
    asuperbin_rsb_index_mht2[nasb_mht2] = nrsb_mht2 ;
    //
    nrsb_mht2 ++ ;
    sprintf( rsuperbin_name_mht2[nrsb_mht2], "MHT2_Nb23_Njet23_HT12" ) ;
    nbi=2; nji=2; hti=1;  rsuperbin_index_mht2[nbi][nji][hti] = nrsb_mht2 ;
    nbi=2; nji=3; hti=1;  rsuperbin_index_mht2[nbi][nji][hti] = nrsb_mht2 ;
    nbi=2; nji=2; hti=2;  rsuperbin_index_mht2[nbi][nji][hti] = nrsb_mht2 ;
    nbi=2; nji=3; hti=2;  rsuperbin_index_mht2[nbi][nji][hti] = nrsb_mht2 ;
    nbi=3; nji=2; hti=1;  rsuperbin_index_mht2[nbi][nji][hti] = nrsb_mht2 ;
    nbi=3; nji=3; hti=1;  rsuperbin_index_mht2[nbi][nji][hti] = nrsb_mht2 ;
    nbi=3; nji=2; hti=2;  rsuperbin_index_mht2[nbi][nji][hti] = nrsb_mht2 ;
    nbi=3; nji=3; hti=2;  rsuperbin_index_mht2[nbi][nji][hti] = nrsb_mht2 ;

    nasb_mht2 ++ ;
    sprintf( asuperbin_name_mht2[nasb_mht2], "MHT2_Nb2_Njet2_HT12" ) ;
    nbi=2; nji=2; hti=1;  asuperbin_index_mht2[nbi][nji][hti] = nasb_mht2 ;
    nbi=2; nji=2; hti=2;  asuperbin_index_mht2[nbi][nji][hti] = nasb_mht2 ;
    asuperbin_rsb_index_mht2[nasb_mht2] = nrsb_mht2 ;
    nasb_mht2 ++ ;
    sprintf( asuperbin_name_mht2[nasb_mht2], "MHT2_Nb2_Njet3_HT12" ) ;
    nbi=2; nji=3; hti=1;  asuperbin_index_mht2[nbi][nji][hti] = nasb_mht2 ;
    nbi=2; nji=3; hti=2;  asuperbin_index_mht2[nbi][nji][hti] = nasb_mht2 ;
    asuperbin_rsb_index_mht2[nasb_mht2] = nrsb_mht2 ;
    nasb_mht2 ++ ;
    sprintf( asuperbin_name_mht2[nasb_mht2], "MHT2_Nb3_Njet2_HT12" ) ;
    nbi=3; nji=2; hti=1;  asuperbin_index_mht2[nbi][nji][hti] = nasb_mht2 ;
    nbi=3; nji=2; hti=2;  asuperbin_index_mht2[nbi][nji][hti] = nasb_mht2 ;
    asuperbin_rsb_index_mht2[nasb_mht2] = nrsb_mht2 ;
    nasb_mht2 ++ ;
    sprintf( asuperbin_name_mht2[nasb_mht2], "MHT2_Nb3_Njet3_HT12" ) ;
    nbi=3; nji=3; hti=1;  asuperbin_index_mht2[nbi][nji][hti] = nasb_mht2 ;
    nbi=3; nji=3; hti=2;  asuperbin_index_mht2[nbi][nji][hti] = nasb_mht2 ;
    asuperbin_rsb_index_mht2[nasb_mht2] = nrsb_mht2 ;
 
    //---- MHT2, HT3

    nrsb_mht2 ++ ;
    sprintf( rsuperbin_name_mht2[nrsb_mht2], "MHT2_Nb23_Njet1_HT3" ) ;
    nbi=2; nji=1; hti=3;  rsuperbin_index_mht2[nbi][nji][hti] = nrsb_mht2 ;
    nbi=3; nji=1; hti=3;  rsuperbin_index_mht2[nbi][nji][hti] = nrsb_mht2 ;

    nasb_mht2 ++ ;
    sprintf( asuperbin_name_mht2[nasb_mht2], "MHT2_Nb2_Njet1_HT3" ) ;
    nbi=2; nji=1; hti=3;  asuperbin_index_mht2[nbi][nji][hti] = nasb_mht2 ;
    asuperbin_rsb_index_mht2[nasb_mht2] = nrsb_mht2 ;
    nasb_mht2 ++ ;
    sprintf( asuperbin_name_mht2[nasb_mht2], "MHT2_Nb3_Njet1_HT3" ) ;
    nbi=3; nji=1; hti=3;  asuperbin_index_mht2[nbi][nji][hti] = nasb_mht2 ;
    asuperbin_rsb_index_mht2[nasb_mht2] = nrsb_mht2 ;
    //
    nrsb_mht2 ++ ;
    sprintf( rsuperbin_name_mht2[nrsb_mht2], "MHT2_Nb23_Njet23_HT3" ) ;
    nbi=2; nji=2; hti=3;  rsuperbin_index_mht2[nbi][nji][hti] = nrsb_mht2 ;
    nbi=2; nji=3; hti=3;  rsuperbin_index_mht2[nbi][nji][hti] = nrsb_mht2 ;
    nbi=3; nji=2; hti=3;  rsuperbin_index_mht2[nbi][nji][hti] = nrsb_mht2 ;
    nbi=3; nji=3; hti=3;  rsuperbin_index_mht2[nbi][nji][hti] = nrsb_mht2 ;

    nasb_mht2 ++ ;
    sprintf( asuperbin_name_mht2[nasb_mht2], "MHT2_Nb2_Njet2_HT3" ) ;
    nbi=2; nji=2; hti=3;  asuperbin_index_mht2[nbi][nji][hti] = nasb_mht2 ;
    asuperbin_rsb_index_mht2[nasb_mht2] = nrsb_mht2 ;
    nasb_mht2 ++ ;
    sprintf( asuperbin_name_mht2[nasb_mht2], "MHT2_Nb2_Njet3_HT3" ) ;
    nbi=2; nji=3; hti=3;  asuperbin_index_mht2[nbi][nji][hti] = nasb_mht2 ;
    asuperbin_rsb_index_mht2[nasb_mht2] = nrsb_mht2 ;
    nasb_mht2 ++ ;
    sprintf( asuperbin_name_mht2[nasb_mht2], "MHT2_Nb3_Njet2_HT3" ) ;
    nbi=3; nji=2; hti=3;  asuperbin_index_mht2[nbi][nji][hti] = nasb_mht2 ;
    asuperbin_rsb_index_mht2[nasb_mht2] = nrsb_mht2 ;
    nasb_mht2 ++ ;
    sprintf( asuperbin_name_mht2[nasb_mht2], "MHT2_Nb3_Njet3_HT3" ) ;
    nbi=3; nji=3; hti=3;  asuperbin_index_mht2[nbi][nji][hti] = nasb_mht2 ;
    asuperbin_rsb_index_mht2[nasb_mht2] = nrsb_mht2 ;

    nasb_mht2 ++ ;
    nrsb_mht2 ++ ;
   
    //---- MHT3, HT23
    sprintf( rsuperbin_name_mht3[nrsb_mht3], "MHT3_Nb23_Njet123_HT23" ) ;
    nbi=2; nji=1; hti=2;  rsuperbin_index_mht3[nbi][nji][hti] = nrsb_mht3 ;
    nbi=2; nji=2; hti=2;  rsuperbin_index_mht3[nbi][nji][hti] = nrsb_mht3 ;
    nbi=2; nji=3; hti=2;  rsuperbin_index_mht3[nbi][nji][hti] = nrsb_mht3 ;
    nbi=2; nji=1; hti=3;  rsuperbin_index_mht3[nbi][nji][hti] = nrsb_mht3 ;
    nbi=2; nji=2; hti=3;  rsuperbin_index_mht3[nbi][nji][hti] = nrsb_mht3 ;
    nbi=2; nji=3; hti=3;  rsuperbin_index_mht3[nbi][nji][hti] = nrsb_mht3 ;
    nbi=3; nji=1; hti=2;  rsuperbin_index_mht3[nbi][nji][hti] = nrsb_mht3 ;
    nbi=3; nji=2; hti=2;  rsuperbin_index_mht3[nbi][nji][hti] = nrsb_mht3 ;
    nbi=3; nji=3; hti=2;  rsuperbin_index_mht3[nbi][nji][hti] = nrsb_mht3 ;
    nbi=3; nji=1; hti=3;  rsuperbin_index_mht3[nbi][nji][hti] = nrsb_mht3 ;
    nbi=3; nji=2; hti=3;  rsuperbin_index_mht3[nbi][nji][hti] = nrsb_mht3 ;
    nbi=3; nji=3; hti=3;  rsuperbin_index_mht3[nbi][nji][hti] = nrsb_mht3 ;

    sprintf( asuperbin_name_mht3[nasb_mht3], "MHT3_Nb2_Njet1_HT23" ) ;
    nbi=2; nji=1; hti=2;  asuperbin_index_mht3[nbi][nji][hti] = nasb_mht3 ;
    nbi=2; nji=1; hti=3;  asuperbin_index_mht3[nbi][nji][hti] = nasb_mht3 ;
    asuperbin_rsb_index_mht3[nasb_mht3] = nrsb_mht3 ;
    nasb_mht3 ++ ;
    sprintf( asuperbin_name_mht3[nasb_mht3], "MHT3_Nb2_Njet2_HT23" ) ;
    nbi=2; nji=2; hti=2;  asuperbin_index_mht3[nbi][nji][hti] = nasb_mht3 ;
    nbi=2; nji=2; hti=3;  asuperbin_index_mht3[nbi][nji][hti] = nasb_mht3 ;
    asuperbin_rsb_index_mht3[nasb_mht3] = nrsb_mht3 ;
    nasb_mht3 ++ ;
    sprintf( asuperbin_name_mht3[nasb_mht3], "MHT3_Nb2_Njet3_HT23" ) ;
    nbi=2; nji=3; hti=2;  asuperbin_index_mht3[nbi][nji][hti] = nasb_mht3 ;
    nbi=2; nji=3; hti=3;  asuperbin_index_mht3[nbi][nji][hti] = nasb_mht3 ;
    asuperbin_rsb_index_mht3[nasb_mht3] = nrsb_mht3 ;
    nasb_mht3 ++ ;
    sprintf( asuperbin_name_mht3[nasb_mht3], "MHT3_Nb3_Njet1_HT23" ) ;
    nbi=3; nji=1; hti=2;  asuperbin_index_mht3[nbi][nji][hti] = nasb_mht3 ;
    nbi=3; nji=1; hti=3;  asuperbin_index_mht3[nbi][nji][hti] = nasb_mht3 ;
    asuperbin_rsb_index_mht3[nasb_mht3] = nrsb_mht3 ;
    nasb_mht3 ++ ;
    sprintf( asuperbin_name_mht3[nasb_mht3], "MHT3_Nb3_Njet2_HT23" ) ;
    nbi=3; nji=2; hti=2;  asuperbin_index_mht3[nbi][nji][hti] = nasb_mht3 ;
    nbi=3; nji=2; hti=3;  asuperbin_index_mht3[nbi][nji][hti] = nasb_mht3 ;
    asuperbin_rsb_index_mht3[nasb_mht3] = nrsb_mht3 ;
    nasb_mht3 ++ ;
    sprintf( asuperbin_name_mht3[nasb_mht3], "MHT3_Nb3_Njet3_HT23" ) ;
    nbi=3; nji=3; hti=2;  asuperbin_index_mht3[nbi][nji][hti] = nasb_mht3 ;
    nbi=3; nji=3; hti=3;  asuperbin_index_mht3[nbi][nji][hti] = nasb_mht3 ;
    asuperbin_rsb_index_mht3[nasb_mht3] = nrsb_mht3 ;  
  
    nasb_mht3 ++ ;
    nrsb_mht3 ++ ;
  }
  
  printf("\n\n   Ratio superbins:\n") ;
  for ( int rsbi_mht2=0; rsbi_mht2<nrsb_mht2; rsbi_mht2++ ) {
    printf(" MHT2 rsbi=%2d : %-25s\n", rsbi_mht2, rsuperbin_name_mht2[rsbi_mht2] ) ;
  } // rsbi_mht2
  for ( int rsbi_mht3=0; rsbi_mht3<nrsb_mht3; rsbi_mht3++ ) {
    printf(" MHT3 rsbi=%2d : %-25s\n", rsbi_mht3, rsuperbin_name_mht3[rsbi_mht3] ) ;
  } // rsbi_mht3
  printf("\n\n") ;

  printf("\n\n   Analysis superbins:\n") ;
  for ( int asbi_mht2=0; asbi_mht2<nasb_mht2; asbi_mht2++ ) {
    printf(" MHT2 asbi=%2d : %-25s,  rsbi=%2d, %-25s\n", asbi_mht2, asuperbin_name_mht2[asbi_mht2], asuperbin_rsb_index_mht2[asbi_mht2], rsuperbin_name_mht2[asuperbin_rsb_index_mht2[asbi_mht2]] ) ;
  } // asbi_mht2
  for ( int asbi_mht3=0; asbi_mht3<nasb_mht3; asbi_mht3++ ) {
    printf(" MHT3 asbi=%2d : %-25s,  rsbi=%2d, %-25s\n", asbi_mht3, asuperbin_name_mht3[asbi_mht3], asuperbin_rsb_index_mht3[asbi_mht3], rsuperbin_name_mht3[asuperbin_rsb_index_mht3[asbi_mht3]] ) ;
  } // asbi_mht3
  printf("\n\n") ;

  printf("\n\n Ratio superbin assignments:\n") ;
  for ( int nbi=0; nbi<nnbbins; nbi++ ) {
    for ( int hti=1; hti<nhtbins; hti++ ) {
      for ( int nji=1; nji<nnjetbins; nji++ ) {
	int rsbi_mht2 = rsuperbin_index_mht2[nbi][nji][hti] ;
	char sbname[100] ;
	if ( rsbi_mht2 >= 0 ) { sprintf( sbname, "%s", rsuperbin_name_mht2[rsbi_mht2] ) ; } else { sprintf( sbname, " " ) ; }
	printf("  MHT2   Nb%d, Njet%d, HT%d : %-25s (rsbi=%d)\n", nbi, nji, hti, sbname, rsbi_mht2 ) ;
      } // nji
    } // hti
  } // nbi
  printf("\n") ;
  for ( int nbi=0; nbi<nnbbins; nbi++ ) {
    for ( int hti=1; hti<nhtbins; hti++ ) {
      for ( int nji=1; nji<nnjetbins; nji++ ) {
	int rsbi_mht3 = rsuperbin_index_mht3[nbi][nji][hti] ;
	char sbname[100] ;
	if ( rsbi_mht3 >= 0 ) { sprintf( sbname, "%s", rsuperbin_name_mht3[rsbi_mht3] ) ; } else { sprintf( sbname, " " ) ; }
	printf("  MHT3   Nb%d, Njet%d, HT%d : %-25s (%d)\n", nbi, nji, hti, sbname, rsbi_mht3 ) ;
      } // nji
    } // hti
  } // nbi
  printf("\n\n") ;

  printf("\n\n Analysis superbin assignments:\n") ;
  for ( int nbi=0; nbi<nnbbins; nbi++ ) {
    for ( int hti=1; hti<nhtbins; hti++ ) {
      for ( int nji=1; nji<nnjetbins; nji++ ) {
	int asbi_mht2 = asuperbin_index_mht2[nbi][nji][hti] ;
	char sbname[100] ;
	if ( asbi_mht2 >= 0 ) { sprintf( sbname, "%s", asuperbin_name_mht2[asbi_mht2] ) ; } else { sprintf( sbname, " " ) ; }
	printf("  MHT2   Nb%d, Njet%d, HT%d : %-25s (asbi=%d)\n", nbi, nji, hti, sbname, asbi_mht2 ) ;
      } // nji
    } // hti
  } // nbi
  printf("\n") ;
  for ( int nbi=0; nbi<nnbbins; nbi++ ) {
    for ( int hti=1; hti<nhtbins; hti++ ) {
      for ( int nji=1; nji<nnjetbins; nji++ ) {
	int asbi_mht3 = asuperbin_index_mht3[nbi][nji][hti] ;
	char sbname[100] ;
	if ( asbi_mht3 >= 0 ) { sprintf( sbname, "%s", asuperbin_name_mht3[asbi_mht3] ) ; } else { sprintf( sbname, " " ) ; }
	printf("  MHT3   Nb%d, Njet%d, HT%d : %-25s (%d)\n", nbi, nji, hti, sbname, asbi_mht3 ) ;
      } // nji
    } // hti
  } // nbi
  printf("\n\n") ;

  double mht_bins[4] = {200, 500, 750, 2000};
  
  TH1D* h_lostlep_mht_cdtt_all_3bin = new TH1D( "h_lostlep_mht_cdtt_all_3bin", "lost lepton MHT, from cos(dtt)", 3, mht_bins ) ;


  TH1D* h_foundlep_wpt_mc_all_10bin = new TH1D( "h_foundlep_wpt_mc_all_3bin", "found lepton wpt, from MC", 10, 0., 1000. ) ;
  TH1D* h_foundlep_wpt_mc_10bin[5][5][5] ;

  TH1D* h_lostlep_mht_cdtt_3bin[5][5][5] ;
  
  TH1D* h_lostlep_mht_cdtt_superbins_mht2[300] ;
  TH1D* h_lostlep_mht_cdtt_superbins_mht3[300] ;

  // TH1D* h_lostlep_mht_cdtt_asuperbins_mht2[300] ;
  // TH1D* h_lostlep_mht_cdtt_asuperbins_mht3[300] ;
 
  TH1D* h_rec_w_pt_ht1 = new TH1D( "h_rec_w_pt_ht1", "W pT, HT1", 80, 0., 1000. ) ;
  TH1D* h_rec_w_pt_ht2 = new TH1D( "h_rec_w_pt_ht2", "W pT, HT2", 80, 0., 1000. ) ;
  TH1D* h_rec_w_pt_ht3 = new TH1D( "h_rec_w_pt_ht3", "W pT, HT3", 80, 0., 1000. ) ;



  for ( int nbi=2; nbi<nnbbins; nbi++ ) {
    for ( int nji=1; nji<nnjetbins; nji++ ) {
      for ( int hti=1; hti<nhtbins; hti++ ) {
	char hname[1000] ;
	char htitle[1000] ;

	sprintf( hname, "h_lostlep_mht_cdtt_nb%d_nj%d_ht%d_3bin", nbi, nji, hti ) ;
	sprintf( htitle, "Lost lepton MHT, cdtt, Nb%d, Njet%d, HT%d;H_{T}^{miss} [GeV];Events/bin", nbi, nji, hti ) ;
	h_lostlep_mht_cdtt_3bin[nbi][nji][hti] = new TH1D( hname, htitle, 3, mht_bins ) ;

	sprintf( hname, "h_foundlep_wpt_mc_nb%d_nj%d_ht%d_3bin", nbi, nji, hti ) ;
	sprintf( htitle, "found lepton wpt, MC, Nb%d, Njet%d, HT%d;p_{T}^{W} [GeV];Events/bin", nbi, nji, hti ) ;
	h_foundlep_wpt_mc_10bin[nbi][nji][hti] = new TH1D( hname, htitle, 10, 0., 1000. ) ;
      } // hti
    } // nji
  } // nbi

  for ( int rsbi_mht2=0; rsbi_mht2<nrsb_mht2; rsbi_mht2 ++ ) {
    char hname[1000] ;
    char htitle[1000] ;
    sprintf( hname, "h_lostlep_mht_cdtt_mht2_superbin%02d", rsbi_mht2 ) ;
    sprintf( htitle, "Lost lepton MHT, cdtt, mht2 superbin %d, %s;H_{T}^{miss} [GeV];Events/bin", rsbi_mht2, rsuperbin_name_mht2[rsbi_mht2] ) ;
    h_lostlep_mht_cdtt_superbins_mht2[rsbi_mht2] = new TH1D( hname, htitle, 3, mht_bins ) ;
  } // rsbi_mht2

  for ( int rsbi_mht3=0; rsbi_mht3<nrsb_mht3; rsbi_mht3 ++ ) {
    char hname[1000] ;
    char htitle[1000] ;
    sprintf( hname, "h_lostlep_mht_cdtt_mht3_superbin%02d", rsbi_mht3 ) ;
    sprintf( htitle, "Lost lepton MHT, cdtt, mht3 superbin %d, %s;H_{T}^{miss} [GeV];Events/bin", rsbi_mht3, rsuperbin_name_mht3[rsbi_mht3] ) ;
    h_lostlep_mht_cdtt_superbins_mht3[rsbi_mht3] = new TH1D( hname, htitle, 3, mht_bins ) ;
  } // rsbi_mht3

  
  double rlostfound_err[5][5][8] ; // njet, ht, wpt
  double rlostfound_val[5][5][8] ; // njet, ht, wpt

  //--- data needed for uncertainty calculations

  double integral_sum[5][5][5][5] ; // nb, njet, ht, mht
  double integral_sum_for_rlostfound_err[5][5][5][5][8] ; // nb, njet, ht, mht, wpt
  Long64_t  nonzero_integral_count[5][5][5][5] ; // nb, njet, ht, mht
  Long64_t  mc_count[5][5][5][5] ; // nb, njet, ht, mht
  double r21_err2_sum[5][5][5] ; // nb, njet, ht
  double r31_err2_sum[5][5][5] ; // nb, njet, ht
  double r21_err2_sum_for_rlostfound_err[5][5][5] ; // nb, njet, ht
  double r31_err2_sum_for_rlostfound_err[5][5][5] ; // nb, njet, ht

  double rsb_integral_sum_for_r21[100][5] ; // sbi, mht
  double rsb_integral_sum_for_r31[100][5] ; // sbi, mht
  double rsb_integral_sum_for_rlostfound_err_for_r21[100][5][8][5][5] ; // sbi, mht, wpt, njet, ht
  double rsb_integral_sum_for_rlostfound_err_for_r31[100][5][8][5][5] ; // sbi, mht, wpt, njet, ht
  Long64_t  rsb_nonzero_integral_count_for_r21[100][5] ; // sbi, mht
  Long64_t  rsb_nonzero_integral_count_for_r31[100][5] ; // sbi, mht
  Long64_t  rsb_mc_count_for_r21[100][5] ; // sbi, mht
  Long64_t  rsb_mc_count_for_r31[100][5] ; // sbi, mht
  Long64_t  asb_mc_count_for_r21[100][5] ; // sbi, mht
  Long64_t  asb_mc_count_for_r31[100][5] ; // sbi, mht
  double   rsb_r21_err2_sum[100] ; // sbi
  double   rsb_r31_err2_sum[100] ; // sbi
  double   rsb_r21_err2_sum_for_rlostfound_err[100] ; // sbi
  double   rsb_r31_err2_sum_for_rlostfound_err[100] ; // sbi

  for ( int nbi=2; nbi<nnbbins; nbi++ ) {
    for ( int nji=1; nji<nnjetbins; nji++ ) {
      for ( int hti=1; hti<nhtbins; hti++ ) {
	r21_err2_sum[nbi][nji][hti] = 0. ;
	r31_err2_sum[nbi][nji][hti] = 0. ;
	r21_err2_sum_for_rlostfound_err[nbi][nji][hti] = 0. ;
	r31_err2_sum_for_rlostfound_err[nbi][nji][hti] = 0. ;
	for ( int mbi=1; mbi<nmhtbins; mbi++ ) {
	  integral_sum[nbi][nji][hti][mbi] = 0. ;
	  nonzero_integral_count[nbi][nji][hti][mbi] = 0 ;
	  mc_count[nbi][nji][hti][mbi] = 0 ;
	  for ( int wpti_for_rlf=0; wpti_for_rlf<nwptbins_for_rlf; wpti_for_rlf++ ) {
	    integral_sum_for_rlostfound_err[nbi][nji][hti][mbi][wpti_for_rlf] = 0 ;
	    rlostfound_err[nji][hti][wpti_for_rlf] = 0 ;
	    rlostfound_val[nji][hti][wpti_for_rlf] = 0 ;
	  }
	} // mbi
      } // hti
    } // nji
  } // nbi

  for ( int rsbi_mht2=0; rsbi_mht2<nrsb_mht2; rsbi_mht2++ ) {
    rsb_r21_err2_sum[rsbi_mht2] = 0. ;
    rsb_r21_err2_sum_for_rlostfound_err[rsbi_mht2] = 0. ;
    for ( int mbi=1; mbi<nmhtbins; mbi++ ) {
      rsb_integral_sum_for_r21[rsbi_mht2][mbi] = 0. ;
      rsb_nonzero_integral_count_for_r21[rsbi_mht2][mbi] = 0 ;
      rsb_mc_count_for_r21[rsbi_mht2][mbi] = 0 ;
      for ( int wpti_for_rlf=0; wpti_for_rlf<nwptbins_for_rlf; wpti_for_rlf++ ) {
	for ( int nji=1; nji<nnjetbins; nji++ ) {
	  for ( int hti=1; hti<nhtbins; hti++ ) {
	    rsb_integral_sum_for_rlostfound_err_for_r21[rsbi_mht2][mbi][wpti_for_rlf][nji][hti] = 0. ;
	  } // hti
	} // nji
      } // wpt_for_rlf
    } // mbi
  } // rsbi_mht2

  for ( int asbi_mht2=0; asbi_mht2<nasb_mht2; asbi_mht2++ ) {
    for ( int mbi=1; mbi<nmhtbins; mbi++ ) {
      asb_mc_count_for_r21[asbi_mht2][mbi] = 0 ;
    } // mbi
  } // asbi_mht2


  for ( int rsbi_mht3=0; rsbi_mht3<nrsb_mht3; rsbi_mht3++ ) {
    rsb_r31_err2_sum[rsbi_mht3] = 0. ;
    rsb_r31_err2_sum_for_rlostfound_err[rsbi_mht3] = 0. ;
    for ( int mbi=1; mbi<nmhtbins; mbi++ ) {
      rsb_integral_sum_for_r31[rsbi_mht3][mbi] = 0. ;
      rsb_nonzero_integral_count_for_r31[rsbi_mht3][mbi] = 0 ;
      rsb_mc_count_for_r31[rsbi_mht3][mbi] = 0 ;
      for ( int wpti_for_rlf=0; wpti_for_rlf<nwptbins_for_rlf; wpti_for_rlf++ ) {
	for ( int nji=1; nji<nnjetbins; nji++ ) {
	  for ( int hti=1; hti<nhtbins; hti++ ) {
	    rsb_integral_sum_for_rlostfound_err_for_r31[rsbi_mht3][mbi][wpti_for_rlf][nji][hti] = 0. ;
	  } // hti
	} // nji
      } // wpt_for_rlf
    } // mbi
  } // rsbi_mht3

  for ( int asbi_mht3=0; asbi_mht3<nasb_mht3; asbi_mht3++ ) {
    for ( int mbi=1; mbi<nmhtbins; mbi++ ) {
      asb_mc_count_for_r31[asbi_mht3][mbi] = 0 ;
    } // mbi
  } // asbi_mht3

  Long64_t nentries = fChain->GetEntries();
  Long64_t nbytes = 0, nb = 0;

  //-- begin loop over events in two passes
  for ( int pass=0; pass<2; pass++ ) {
    printf("\n\n Looping over %llu events.  Pass %d\n\n", nentries, pass ) ;
    for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      if (jentry%(nentries/100) == 0 && jentry>0 ) { printf("  %9llu out of %9llu (%2lld%%)\n", jentry, nentries, ((100*jentry)/nentries)+1 ) ; fflush(stdout) ; }

      if ( num_true_mus+num_true_els != 1 ) continue ;
      if ( num_true_had_taus>0 ) continue ; // *********** reject hadronic taus ************

      if (num_csvm_jets30<2) continue;
      // only 2+b events for now

      /////if ( mht < 200 ) continue ; // try a mht cut.

      // find analysis bin
      int hti = -1 ;
      int nbi = -1 ;
      int nji = -1 ;
      int mbi = -1 ;
      int wpti_for_rlf = -1 ;

      for ( int i=1; i<nhtbins  ; i++ ) { if ( ht30 > htbins[i] && ht30 < htbins[i+1] ) { hti = i ; break ; } }
      if ( verb ) printf( " %8llu : HT %6.1f, HT bin %d [%4.0f, %4.0f]\n", jentry, ht30, hti, htbins[hti], htbins[hti+1] ) ;
      if ( hti < 1 ) continue ;

      for ( int i=1; i<nnjetbins; i++ ) { if ( num_jets_pt30 > njetbins[i] && num_jets_pt30 < njetbins[i+1] ) { nji = i ; break ; } }
      if ( verb ) printf( " %8llu : Njet %2d, Njet bin %d [%3.1f, %3.1f]\n", jentry, num_jets_pt30, nji, njetbins[nji], njetbins[nji+1] ) ;
      if ( nji < 1 ) continue ;

      for ( int i=2; i<nnbbins  ; i++ ) { if ( num_csvm_jets30 > nbbins[i] && num_csvm_jets30 < nbbins[i+1] ) { nbi = i ; break ; } }
      if ( nbi < 0 ) continue ;
      if ( verb ) printf( " %8llu : Nb %2d, Nb bin %d [%3.1f, %3.1f]\n", jentry, num_csvm_jets30, nbi, nbbins[nbi], nbbins[nbi+1] ) ;

      for ( int i=0; i<nmhtbins ; i++ ) { if ( mht30 > mhtbins[i] && mht30 < mhtbins[i+1] ) { mbi = i ; break ; } }
      if ( verb ) printf( " %8llu : MHT30 %6.1f, MHT30 bin %d [%4.0f, %4.0f]\n", jentry, mht30, mbi, mhtbins[mbi], mhtbins[mbi+1] ) ;

      // find the true lepton, get info
      double gen_w1_pt_from_mht_and_trk(0.), cosdtt_gentrk_mht(0.);
      if (num_true_mus==1) {
	gen_w1_pt_from_mht_and_trk=true_mu_ptW->at(0);
	cosdtt_gentrk_mht = cos(true_mu_dTT->at(0));
      } else if (num_true_els==1) {
	gen_w1_pt_from_mht_and_trk=true_el_ptW->at(0);
	cosdtt_gentrk_mht = cos(true_el_dTT->at(0));
      }

      if ( (num_veto_mus+num_veto_els) == 0 && pass == 0 ) {
	int rsbi_mht2 = rsuperbin_index_mht2[nbi][nji][hti] ;
	int rsbi_mht3 = rsuperbin_index_mht3[nbi][nji][hti] ;
	int asbi_mht2 = asuperbin_index_mht2[nbi][nji][hti] ;
	int asbi_mht3 = asuperbin_index_mht3[nbi][nji][hti] ;
	if ( mbi >= 1 ) {
	  mc_count[nbi][nji][hti][mbi] += 1 ;
	  if ( rsbi_mht2 >= 0 ) rsb_mc_count_for_r21[rsbi_mht2][mbi] += 1 ;
	  if ( rsbi_mht3 >= 0 ) rsb_mc_count_for_r31[rsbi_mht3][mbi] += 1 ;
	  if ( asbi_mht2 >= 0 ) asb_mc_count_for_r21[asbi_mht2][mbi] += 1 ;
	  if ( asbi_mht3 >= 0 ) asb_mc_count_for_r31[asbi_mht3][mbi] += 1 ;
	}
      }

      double mt_rec_mht(0.), rec_lep_pt(0.), rec_w_pt_mht(0.); 
      if (num_veto_mus+num_veto_els == 1) {
	if (num_veto_mus==1) {
	  for (unsigned int imu(0); imu<mu_pt->size(); imu++) {
	    if (mu_veto->at(imu)&&mu_mT->at(imu)<100) {
	      mt_rec_mht = mu_mT->at(imu);
	      rec_lep_pt = mu_pt->at(imu);
	      rec_w_pt_mht = mu_ptW->at(imu);
	      break;
	    }
	  }
	} else if (num_veto_els==1) {
	  for (unsigned int iel(0); iel<el_pt->size(); iel++) {
	    if (el_veto->at(iel)&&el_mT->at(iel)<100) {
	      mt_rec_mht = el_mT->at(iel);
	      rec_lep_pt = el_pt->at(iel);
	      rec_w_pt_mht = el_ptW->at(iel);
	      break;
	    }
	  }
	}
      }

      if (num_veto_mus+num_veto_els == 1 && mht30>mht_cut && mt_rec_mht<mt_cut && rec_lep_pt>lepton_pt_cut && pass == 0 ) {
	h_foundlep_wpt_mc_all_10bin -> Fill( gen_w1_pt_from_mht_and_trk ) ;
	h_foundlep_wpt_mc_10bin[nbi][nji][hti] -> Fill( gen_w1_pt_from_mht_and_trk ) ;
      }

      if ( mbi < 0 ) continue ; 
      
      if ( verb ) printf( " %8llu : Number of reconstructed leptons : %d\n", jentry, (num_veto_els+num_veto_mus) ) ;
      if ( !((num_veto_els+num_veto_mus) == 1 && mht30>mht_cut && mt_rec_mht<mt_cut && rec_lep_pt>lepton_pt_cut) ) continue ;

      for ( int i=0; i<nwptbins_for_rlf; i++ )  {
	if ( rec_w_pt_mht > wptbins_for_rlf[i] && rec_w_pt_mht < wptbins_for_rlf[i+1] ) { wpti_for_rlf = i ; break ; }
      }

      char hname[100] ;
      sprintf( hname, "h_cdtt_lostlep_nj%d_ht%d", nji, hti ) ;
      TH1D* hist = (TH1D*) gDirectory -> FindObject( hname ) ;
      if ( hist == 0x0 ) { printf("\n\n Missing input histogram: %s\n\n", hname ) ; return ; }
      if ( verb ) printf( " %8llu : 1/2(1-cdtt) hist name : %s\n", jentry, hname ) ;

      sprintf( hname, "h_wpt_lostfound_ratio_nj%d_ht%d", nji, hti ) ;
      TH1D* lostfound_ratio_hist = (TH1D*) gDirectory -> FindObject( hname ) ;
      if ( lostfound_ratio_hist == 0x0 ) { printf( "\n\n Missing input histogram: %s\n\n", hname ) ; return ; }
      double lostfound_ratio(1.) ;
      double lostfound_ratio_err(0.) ;
      int ratio_histbin = lostfound_ratio_hist -> GetXaxis() -> FindBin( rec_w_pt_mht ) ;
      if ( wpti_for_rlf != (ratio_histbin-1) ) { printf("\n\n *** wpt bin mismatch?  rec_w_pt_mht = %6.1f, wpti_for_rlf=%d, ratio_histbin-1=%d\n\n", rec_w_pt_mht, wpti_for_rlf, ratio_histbin-1 ) ; return ; }
      if ( ratio_histbin > 0 ) lostfound_ratio = lostfound_ratio_hist -> GetBinContent( ratio_histbin ) ;
      if ( ratio_histbin > 0 ) lostfound_ratio_err = lostfound_ratio_hist -> GetBinError( ratio_histbin ) ;
      if (verb) printf( " %8llu : lost/found ratio hist %s : bin %d, value %6.3f\n", jentry, hname, ratio_histbin, lostfound_ratio ) ;

      if ( pass == 0 ) {
	make_mht_hist_entries( h_lostlep_mht_cdtt_all_3bin, hist, rec_w_pt_mht, lostfound_ratio ) ;
	make_mht_hist_entries( h_lostlep_mht_cdtt_3bin[nbi][nji][hti], hist, rec_w_pt_mht, lostfound_ratio ) ;
	int rsbi_mht2 = rsuperbin_index_mht2[nbi][nji][hti] ;
	int rsbi_mht3 = rsuperbin_index_mht3[nbi][nji][hti] ;
	if ( rsbi_mht2 >= 0 ) make_mht_hist_entries( h_lostlep_mht_cdtt_superbins_mht2[rsbi_mht2], hist, rec_w_pt_mht, lostfound_ratio ) ;
	if ( rsbi_mht3 >= 0 ) make_mht_hist_entries( h_lostlep_mht_cdtt_superbins_mht3[rsbi_mht3], hist, rec_w_pt_mht, lostfound_ratio ) ;

	if ( hti==1 ) h_rec_w_pt_ht1 -> Fill( rec_w_pt_mht ) ;
	if ( hti==2 ) h_rec_w_pt_ht2 -> Fill( rec_w_pt_mht ) ;
	if ( hti==3 ) h_rec_w_pt_ht3 -> Fill( rec_w_pt_mht ) ;
      }
      
      double i1(0.), i2(0.), i3(0.) ;
      i1 = pdf_mht_bin_integral( hist, 1, rec_w_pt_mht ) ;
      i2 = pdf_mht_bin_integral( hist, 2, rec_w_pt_mht ) ;
      if ( hti > 1 ) i3 = pdf_mht_bin_integral( hist, 3, rec_w_pt_mht ) ;
	
      int rsbi_mht2 = rsuperbin_index_mht2[nbi][nji][hti] ;
      int rsbi_mht3 = rsuperbin_index_mht3[nbi][nji][hti] ;

      if ( pass == 0 ) {
	integral_sum[nbi][nji][hti][1] += i1 * lostfound_ratio ;
	integral_sum[nbi][nji][hti][2] += i2 * lostfound_ratio ;
	integral_sum[nbi][nji][hti][3] += i3 * lostfound_ratio ;

	if ( rsbi_mht2 >= 0 ) {
	  rsb_integral_sum_for_r21[rsbi_mht2][1] += i1 * lostfound_ratio ;
	  rsb_integral_sum_for_r21[rsbi_mht2][2] += i2 * lostfound_ratio ;
	}
	if ( rsbi_mht3 >= 0 ) {
	  rsb_integral_sum_for_r31[rsbi_mht3][1] += i1 * lostfound_ratio ;
	  rsb_integral_sum_for_r31[rsbi_mht3][3] += i3 * lostfound_ratio ;
	}

	if ( wpti_for_rlf>=0 && wpti_for_rlf<nwptbins_for_rlf ) {

	  integral_sum_for_rlostfound_err[nbi][nji][hti][1][wpti_for_rlf] += i1 ;
	  integral_sum_for_rlostfound_err[nbi][nji][hti][2][wpti_for_rlf] += i2 ;
	  integral_sum_for_rlostfound_err[nbi][nji][hti][3][wpti_for_rlf] += i3 ;
	  rlostfound_err[nji][hti][wpti_for_rlf] = lostfound_ratio_err ;
	  rlostfound_val[nji][hti][wpti_for_rlf] = lostfound_ratio ;

	  if ( rsbi_mht2 >= 0 ) {
	    rsb_integral_sum_for_rlostfound_err_for_r21[rsbi_mht2][1][wpti_for_rlf][nji][hti] += i1 ;
	    rsb_integral_sum_for_rlostfound_err_for_r21[rsbi_mht2][2][wpti_for_rlf][nji][hti] += i2 ;
	  }
	  if ( rsbi_mht3 >= 0 ) {
	    rsb_integral_sum_for_rlostfound_err_for_r31[rsbi_mht2][1][wpti_for_rlf][nji][hti] += i1 ;
	    rsb_integral_sum_for_rlostfound_err_for_r31[rsbi_mht2][3][wpti_for_rlf][nji][hti] += i3 ;
	  }
	}

	if ( i1 > 0 ) nonzero_integral_count[nbi][nji][hti][1] += 1 ;
	if ( i2 > 0 ) nonzero_integral_count[nbi][nji][hti][2] += 1 ;
	if ( i3 > 0 ) nonzero_integral_count[nbi][nji][hti][3] += 1 ;

	if ( rsbi_mht2 >= 0 ) {
	  if ( i1 > 0 ) rsb_nonzero_integral_count_for_r21[rsbi_mht2][1] += 1 ;
	  if ( i2 > 0 ) rsb_nonzero_integral_count_for_r21[rsbi_mht2][2] += 1 ;
	}
	if ( rsbi_mht3 >= 0 ) {
	  if ( i1 > 0 ) rsb_nonzero_integral_count_for_r31[rsbi_mht3][1] += 1 ;
	  if ( i3 > 0 ) rsb_nonzero_integral_count_for_r31[rsbi_mht3][3] += 1 ;
	}

	if ( rsbi_mht2 == debug_rsbi_mht2 && i1>0. ) {
	  // printf("  DEBUG1 sbi%02d, %20s : lostfound_ratio = %6.3f +/- %6.3f,  i1=%6.3f, i2=%6.3f\n",
	  //   rsbi_mht2, rsuperbin_name_mht2[rsbi_mht2],
	  //   lostfound_ratio, lostfound_ratio_err,
	  //   i1, i2
	  //   ) ;
	}
      } else { // 2nd pass
	if ( integral_sum[nbi][nji][hti][1] > 0. ) {

	  double s1 = integral_sum[nbi][nji][hti][1] ;
	  double s2 = integral_sum[nbi][nji][hti][2] ;
	  double s3 = integral_sum[nbi][nji][hti][3] ;

	  if ( s1 > 0 ) {
	    r21_err2_sum[nbi][nji][hti] += pow( ( lostfound_ratio * i2/s1 - (lostfound_ratio * s2*i1)/(s1*s1) ), 2. ) ;
	    r31_err2_sum[nbi][nji][hti] += pow( ( lostfound_ratio * i3/s1 - (lostfound_ratio * s3*i1)/(s1*s1) ), 2. ) ;
	  }

	}

	if ( rsbi_mht2 >= 0 ) {
	  if ( rsb_integral_sum_for_r21[rsbi_mht2][1] > 0. ) {
	    double s1 = rsb_integral_sum_for_r21[rsbi_mht2][1] ;
	    double s2 = rsb_integral_sum_for_r21[rsbi_mht2][2] ;
	    if ( s1 > 0 ) {
	      rsb_r21_err2_sum[rsbi_mht2] += pow( ( lostfound_ratio * i2/s1 - (lostfound_ratio * s2*i1)/(s1*s1) ), 2. ) ;
	    }
	    if ( rsbi_mht2 == debug_rsbi_mht2 && i1>0 ) {
	      //      printf("  DEBUG2 sbi%02d, %20s : s1=%9.3f, s2=%9.3f, Rlf = %6.3f+/-%6.3f, rec_w_pt_mht=%7.1f, i1=%6.3f, i2=%6.3f, contrib to r21 err2 sum = %10.8f (sqrt = %8.5f, term1=%8.5f, term2=%8.5f)\n",
	      //        rsbi_mht2, rsuperbin_name_mht2[rsbi_mht2],
	      //        s1, s2,
	      //        lostfound_ratio, lostfound_ratio_err,
	      //        rec_w_pt_mht,
	      //        i1, i2,
	      //        pow( ( lostfound_ratio * i2/s1 - (lostfound_ratio * s2*i1)/(s1*s1) ), 2. ),
	      //        sqrt( pow( ( lostfound_ratio * i2/s1 - (lostfound_ratio * s2*i1)/(s1*s1) ), 2. ) ),
	      //        lostfound_ratio * i2/s1, (lostfound_ratio * s2*i1)/(s1*s1)
	      //        ) ;
	    }
	  }
	}

	if ( rsbi_mht3 >= 0 ) {
	  if ( rsb_integral_sum_for_r31[rsbi_mht3][1] > 0. ) {
	    double s1 = rsb_integral_sum_for_r31[rsbi_mht3][1] ;
	    double s3 = rsb_integral_sum_for_r31[rsbi_mht3][3] ;
	    if ( s1 > 0 ) {
	      rsb_r31_err2_sum[rsbi_mht3] += pow( ( lostfound_ratio * i3/s1 - (lostfound_ratio * s3*i1)/(s1*s1) ), 2. ) ;
	    }
	  }
	}
      } // 2nd pass

      if ( verb ) printf("\n --------------------------------------------------------------------- \n\n") ;

    } // loop over events

    if ( pass > 0 ) {

      for ( int nbi=2; nbi<nnbbins; nbi++ ) {
	for ( int nji=1; nji<nnjetbins; nji++ ) {
	  for ( int hti=1; hti<nhtbins; hti++ ) {

	    double s1 = integral_sum[nbi][nji][hti][1] ;
	    double s2 = integral_sum[nbi][nji][hti][2] ;
	    double s3 = integral_sum[nbi][nji][hti][3] ;

	    for ( int wpti_for_rlf=0; wpti_for_rlf<nwptbins_for_rlf; wpti_for_rlf++ ) {
	      if ( s1 > 0 ) {
		r21_err2_sum_for_rlostfound_err[nbi][nji][hti] += pow( ( integral_sum_for_rlostfound_err[nbi][nji][hti][2][wpti_for_rlf]/s1 - s2 * integral_sum_for_rlostfound_err[nbi][nji][hti][1][wpti_for_rlf] / (s1*s1) ) * rlostfound_err[nji][hti][wpti_for_rlf]  , 2. ) ;
		r31_err2_sum_for_rlostfound_err[nbi][nji][hti] += pow( ( integral_sum_for_rlostfound_err[nbi][nji][hti][3][wpti_for_rlf]/s1 - s3 * integral_sum_for_rlostfound_err[nbi][nji][hti][1][wpti_for_rlf] / (s1*s1) ) * rlostfound_err[nji][hti][wpti_for_rlf]  , 2. ) ;
	      }
	    } // wpti_for_rlf

	  } // hti
	} // nji
      } // nbi

      //-- this crap is to avoid double counting superbins that have more than one Nb bin
      bool rsb_rlf_err_already_done_mht2[100][5][5] ;
      bool rsb_rlf_err_already_done_mht3[100][5][5] ;
      for ( int nji=1; nji<nnjetbins; nji++ ) {
	for ( int hti=1; hti<nhtbins; hti++ ) {
	  for ( int rsbi_mht2=0; rsbi_mht2<nrsb_mht2; rsbi_mht2++ ) {
	    rsb_rlf_err_already_done_mht2[rsbi_mht2][nji][hti] = false ;
	  } // rsbi_mht2
	  for ( int rsbi_mht3=0; rsbi_mht3<nrsb_mht3; rsbi_mht3++ ) {
	    rsb_rlf_err_already_done_mht3[rsbi_mht3][nji][hti] = false ;
	  } // rsbi_mht3
	} // hti
      } // nji


      for ( int nbi=0; nbi<nnbbins; nbi++ ) {
	for ( int nji=1; nji<nnjetbins; nji++ ) {
	  for ( int hti=1; hti<nhtbins; hti++ ) {

	    int rsbi_mht2 = rsuperbin_index_mht2[nbi][nji][hti] ;
	    int rsbi_mht3 = rsuperbin_index_mht3[nbi][nji][hti] ;

	    if ( rsbi_mht2 >= 0 ) {

	      if ( ! rsb_rlf_err_already_done_mht2[rsbi_mht2][nji][hti] ) {

		rsb_rlf_err_already_done_mht2[rsbi_mht2][nji][hti] = true ;

		double s1 = rsb_integral_sum_for_r21[rsbi_mht2][1] ;
		double s2 = rsb_integral_sum_for_r21[rsbi_mht2][2] ;

		for ( int wpti_for_rlf=0; wpti_for_rlf<nwptbins_for_rlf; wpti_for_rlf++ ) {
		  if ( s1 > 0 ) {
		    rsb_r21_err2_sum_for_rlostfound_err[rsbi_mht2] += pow( ( rsb_integral_sum_for_rlostfound_err_for_r21[rsbi_mht2][2][wpti_for_rlf][nji][hti]/s1 - s2 * rsb_integral_sum_for_rlostfound_err_for_r21[rsbi_mht2][1][wpti_for_rlf][nji][hti] / (s1*s1) ) * rlostfound_err[nji][hti][wpti_for_rlf]  , 2. ) ;
		    if ( rsbi_mht2 == debug_rsbi_mht2 && s1>0 ) {
		      printf("  DEBUG3 sbi%02d, %20s, wpti=%2d, nbi=%d, nji=%d, hti=%d : Rlf = %5.3f +/- %5.3f, Isum2=%8.3f, Isum1=%8.3f, contrib to err2 sum= %10.8f (sqrt %8.5f)\n",
			     rsbi_mht2, rsuperbin_name_mht2[rsbi_mht2], wpti_for_rlf, nbi, nji, hti,
			     rlostfound_val[nji][hti][wpti_for_rlf], rlostfound_err[nji][hti][wpti_for_rlf],
			     rsb_integral_sum_for_rlostfound_err_for_r21[rsbi_mht2][2][wpti_for_rlf][nji][hti], rsb_integral_sum_for_rlostfound_err_for_r21[rsbi_mht2][1][wpti_for_rlf][nji][hti],
			     pow( ( rsb_integral_sum_for_rlostfound_err_for_r21[rsbi_mht2][2][wpti_for_rlf][nji][hti]/s1 - s2 * rsb_integral_sum_for_rlostfound_err_for_r21[rsbi_mht2][1][wpti_for_rlf][nji][hti] / (s1*s1) ) * rlostfound_err[nji][hti][wpti_for_rlf]  , 2. ),
			     sqrt(pow( ( rsb_integral_sum_for_rlostfound_err_for_r21[rsbi_mht2][2][wpti_for_rlf][nji][hti]/s1 - s2 * rsb_integral_sum_for_rlostfound_err_for_r21[rsbi_mht2][1][wpti_for_rlf][nji][hti] / (s1*s1) ) * rlostfound_err[nji][hti][wpti_for_rlf]  , 2. ))
			     ) ;
		    }
		  }
		} // wpti_for_rlf
	      }
	    }

	    if ( rsbi_mht3 >= 0 ) {

	      if ( ! rsb_rlf_err_already_done_mht3[rsbi_mht2][nji][hti] ) {

		rsb_rlf_err_already_done_mht3[rsbi_mht2][nji][hti] = true ;

		double s1 = rsb_integral_sum_for_r31[rsbi_mht3][1] ;
		double s3 = rsb_integral_sum_for_r31[rsbi_mht3][3] ;

		for ( int wpti_for_rlf=0; wpti_for_rlf<nwptbins_for_rlf; wpti_for_rlf++ ) {
		  if ( s1 > 0 ) {
		    rsb_r31_err2_sum_for_rlostfound_err[rsbi_mht3] += pow( ( rsb_integral_sum_for_rlostfound_err_for_r31[rsbi_mht3][3][wpti_for_rlf][nji][hti]/s1 - s3 * rsb_integral_sum_for_rlostfound_err_for_r31[rsbi_mht3][1][wpti_for_rlf][nji][hti] / (s1*s1) ) * rlostfound_err[nji][hti][wpti_for_rlf]  , 2. ) ;
		  }
		} // wpti_for_rlf
	      }
	    }

	  } // hti
	} // nji
      } // nbi
    } // 2nd pass shenanigancs

   
    TH1D* h_r21_ht1_cdtt(0x0) ;
    TH1D* h_r21_ht2_cdtt(0x0) ;
    TH1D* h_r21_ht3_cdtt(0x0) ;
    TH1D* h_r31_ht2_cdtt(0x0) ;
    TH1D* h_r31_ht3_cdtt(0x0) ;

    TH1D* h_lostlep_events_cdtt[5][5] ;

    TH1D* h_nonzero_integral_count[5][5] ;

    TH1D* h_lostlep_events_cdtt_superbins(0x0) ;

    TH1D* h_lostlep_events_cdtt_asuperbins(0x0) ;

    TH1D* h_nonzero_integral_count_superbins(0x0) ;

    TH1D* h_mhtratio_cdtt_superbins(0x0) ;

    if ( pass > 0 ) {
      h_r21_ht1_cdtt = new TH1D( "h_r21_ht1_cdtt", "R_MHT2/MHT1, HT1, cdtt values", 16, 0.6, 16.6 ) ; set_r_hist_labels( h_r21_ht1_cdtt ) ;
      h_r21_ht2_cdtt = new TH1D( "h_r21_ht2_cdtt", "R_MHT2/MHT1, HT2, cdtt values", 16, 0.6, 16.6 ) ; set_r_hist_labels( h_r21_ht2_cdtt ) ;
      h_r21_ht3_cdtt = new TH1D( "h_r21_ht3_cdtt", "R_MHT2/MHT1, HT3, cdtt values", 16, 0.6, 16.6 ) ; set_r_hist_labels( h_r21_ht3_cdtt ) ;
      h_r31_ht2_cdtt = new TH1D( "h_r31_ht2_cdtt", "R_MHT3/MHT1, HT2, cdtt values", 16, 0.6, 16.6 ) ; set_r_hist_labels( h_r31_ht2_cdtt ) ;
      h_r31_ht3_cdtt = new TH1D( "h_r31_ht3_cdtt", "R_MHT3/MHT1, HT3, cdtt values", 16, 0.6, 16.6 ) ; set_r_hist_labels( h_r31_ht3_cdtt ) ;

      h_r21_ht1_cdtt -> SetMarkerStyle( 21 ) ; h_r21_ht1_cdtt -> SetMarkerColor( 4 ) ; h_r21_ht1_cdtt -> SetLineColor( 4 ) ;
      h_r21_ht2_cdtt -> SetMarkerStyle( 21 ) ; h_r21_ht2_cdtt -> SetMarkerColor( 4 ) ; h_r21_ht2_cdtt -> SetLineColor( 4 ) ;
      h_r21_ht3_cdtt -> SetMarkerStyle( 21 ) ; h_r21_ht3_cdtt -> SetMarkerColor( 4 ) ; h_r21_ht3_cdtt -> SetLineColor( 4 ) ;
      h_r31_ht2_cdtt -> SetMarkerStyle( 21 ) ; h_r31_ht2_cdtt -> SetMarkerColor( 4 ) ; h_r31_ht2_cdtt -> SetLineColor( 4 ) ;
      h_r31_ht3_cdtt -> SetMarkerStyle( 21 ) ; h_r31_ht3_cdtt -> SetMarkerColor( 4 ) ; h_r31_ht3_cdtt -> SetLineColor( 4 ) ;


      int nhbins_rsb = 4 + nrsb_mht2 + nrsb_mht3 ;
      h_lostlep_events_cdtt_superbins = new TH1D( "h_lostlep_events_cdtt_superbins", "Lost lepton events, cdtt values, ratio superbins", nhbins_rsb, 0.55, nhbins_rsb+0.55 ) ;

      int nhbins_asb = 4 + nasb_mht2 + nasb_mht3 ;
      h_lostlep_events_cdtt_asuperbins = new TH1D( "h_lostlep_events_cdtt_asuperbins", "Lost lepton events, cdtt values, analysis superbins", nhbins_asb, 0.55, nhbins_asb+0.55 ) ;

      h_nonzero_integral_count_superbins   = new TH1D( "h_nonzero_integral_count_superbins", "Events contrib. to numerator, ratio superbins",     nhbins_rsb, 0.55, nhbins_rsb+0.55 ) ;

      h_mhtratio_cdtt_superbins = new TH1D( "h_mhtratio_cdtt_superbins", "MHT ratio, cdtt, ratio superbins", nhbins_rsb, 0.55, nhbins_rsb+0.55 ) ;

      h_mhtratio_cdtt_superbins -> SetMarkerStyle( 21 ) ; h_mhtratio_cdtt_superbins -> SetMarkerColor( 4 ) ; h_mhtratio_cdtt_superbins -> SetLineColor( 4 ) ;

      {
	int hbi(2) ;
	for ( int sbi=0; sbi<nrsb_mht2; sbi++ ) {
	  h_lostlep_events_cdtt_superbins -> GetXaxis() -> SetBinLabel( hbi, rsuperbin_name_mht2[sbi] ) ;
	  h_nonzero_integral_count_superbins -> GetXaxis() -> SetBinLabel( hbi, rsuperbin_name_mht2[sbi] ) ;
	  h_mhtratio_cdtt_superbins -> GetXaxis() -> SetBinLabel( hbi, rsuperbin_name_mht2[sbi] ) ;
	  hbi++ ;
	  if ( sbi==(nrsb_mht2/2-1) ) hbi++ ;
	}
	hbi++ ;
	for ( int sbi=0; sbi<nrsb_mht3; sbi++ ) {
	  h_lostlep_events_cdtt_superbins -> GetXaxis() -> SetBinLabel( hbi, rsuperbin_name_mht3[sbi] ) ;
	  h_nonzero_integral_count_superbins -> GetXaxis() -> SetBinLabel( hbi, rsuperbin_name_mht3[sbi] ) ;
	  h_mhtratio_cdtt_superbins -> GetXaxis() -> SetBinLabel( hbi, rsuperbin_name_mht3[sbi] ) ;
	  hbi++ ;
	}
      }
      {
	int hbi(2) ;
	for ( int sbi=0; sbi<nasb_mht2; sbi++ ) {
	  h_lostlep_events_cdtt_asuperbins -> GetXaxis() -> SetBinLabel( hbi, asuperbin_name_mht2[sbi] ) ;
	  hbi++ ;
	  if ( sbi==(nasb_mht2/2-1) ) hbi++ ;
	}
	hbi++ ;
	for ( int sbi=0; sbi<nasb_mht3; sbi++ ) {
	  h_lostlep_events_cdtt_asuperbins -> GetXaxis() -> SetBinLabel( hbi, asuperbin_name_mht3[sbi] ) ;
	  hbi++ ;
	}
      }

      h_lostlep_events_cdtt_superbins -> SetLineColor( 4 ) ;
      h_lostlep_events_cdtt_superbins -> GetXaxis() -> LabelsOption( "v" ) ;

      h_lostlep_events_cdtt_asuperbins -> SetLineColor( 4 ) ;
      h_lostlep_events_cdtt_asuperbins -> GetXaxis() -> LabelsOption( "v" ) ;


      h_nonzero_integral_count_superbins -> GetXaxis() -> LabelsOption( "v" ) ;
      h_mhtratio_cdtt_superbins -> GetXaxis() -> LabelsOption( "v" ) ;
      h_nonzero_integral_count_superbins -> SetLineColor( 4 ) ;

      for ( int nbi=2; nbi<4; nbi++ ) {
	for ( int nji=1; nji<nnjetbins; nji++ ) {

	  char hname[1000] ;
	  char htitle[1000] ;

	  sprintf( hname, "h_lostlep_events_cdtt_nb%d_nj%d", nbi, nji ) ;
	  sprintf( htitle, "Lost lepton event yield, cdtt, Nb%d, Njet%d", nbi, nji ) ;
	  h_lostlep_events_cdtt[nbi][nji] = new TH1D( hname, htitle, 8, 0.55, 8.55 ) ;
	  set_events_hist_labels( h_lostlep_events_cdtt[nbi][nji] ) ;
	  h_lostlep_events_cdtt[nbi][nji] -> SetLineColor( 4 ) ;

	  sprintf( hname, "h_nonzero_integral_count_nb%d_nj%d", nbi, nji ) ;
	  sprintf( htitle, "Events contrib. to numerator, Nb%d, Njet%d", nbi, nji ) ;
	  h_nonzero_integral_count[nbi][nji] = new TH1D( hname, htitle, 8, 0.55, 8.55 ) ;
	  set_events_hist_labels( h_nonzero_integral_count[nbi][nji] ) ;
	  h_nonzero_integral_count[nbi][nji] -> SetLineColor( 4 ) ;

	} // nji
      } // nbi
    } // 2nd pass only

    for ( int nbi=2; nbi<nnbbins; nbi++ ) {
      for ( int nji=1; nji<nnjetbins; nji++ ) {
	for ( int hti=1; hti<nhtbins; hti++ ) {

	  int rhist_bin = 5*(nji-1) + nbi + 2 ;

	  printf("  Nb%d, Njet%d, HT%d:  MHT1 %9.1f from %9lld non-zero contributions.\n", nbi, nji, hti, integral_sum[nbi][nji][hti][1], nonzero_integral_count[nbi][nji][hti][1] ) ;
	  printf("  Nb%d, Njet%d, HT%d:  MHT2 %9.1f from %9lld non-zero contributions.\n", nbi, nji, hti, integral_sum[nbi][nji][hti][2], nonzero_integral_count[nbi][nji][hti][2] ) ;
	  printf("  Nb%d, Njet%d, HT%d:  MHT3 %9.1f from %9lld non-zero contributions.\n", nbi, nji, hti, integral_sum[nbi][nji][hti][3], nonzero_integral_count[nbi][nji][hti][3] ) ;

	  double r21_val = 0. ;
	  double r21_err = 0. ;
	  double r21_err_stat = 0. ;
	  double r21_err_rlostfound = 0. ;
	  if ( integral_sum[nbi][nji][hti][1] > 0. ) {
	    r21_val = integral_sum[nbi][nji][hti][2] / integral_sum[nbi][nji][hti][1] ;
	    if ( pass > 0 ) {
	      r21_err = sqrt( r21_err2_sum[nbi][nji][hti] + r21_err2_sum_for_rlostfound_err[nbi][nji][hti] ) ;
	      r21_err_stat = sqrt( r21_err2_sum[nbi][nji][hti] ) ;
	      r21_err_rlostfound = sqrt( r21_err2_sum_for_rlostfound_err[nbi][nji][hti] ) ;
	    }
	  }

	  double mc_r21_val = 0. ;
	  double mc_r21_err = 0. ;
	  if ( mc_count[nbi][nji][hti][1] > 0. ) {
	    mc_r21_val = (1.*mc_count[nbi][nji][hti][2])/(1.*mc_count[nbi][nji][hti][1]) ;
	    if ( mc_count[nbi][nji][hti][2] > 0. ) {
	      mc_r21_err = mc_r21_val * sqrt( 1./mc_count[nbi][nji][hti][1] + 1./mc_count[nbi][nji][hti][2] ) ;
	    }
	  }

	  if ( pass == 0 ) {
	    printf("      Nb%d, Njet%d, HT%d : DeltaThetaT R21 = %6.4f,   MC val = %6.3f +/- %6.3f   ( %3lld / %4lld )\n", nbi, nji, hti, r21_val, mc_r21_val, mc_r21_err,
		   mc_count[nbi][nji][hti][2], mc_count[nbi][nji][hti][1] ) ;
	  } else {

	    printf("      Nb%d, Njet%d, HT%d : DeltaThetaT R21 = %6.4f +/- %6.4f,   MC val = %6.3f +/- %6.3f   ( %3lld / %4lld )  (stat = %6.3f, rlostfound = %6.3f)\n",
		   nbi, nji, hti, r21_val, r21_err, mc_r21_val, mc_r21_err,
		   mc_count[nbi][nji][hti][2], mc_count[nbi][nji][hti][1], r21_err_stat, r21_err_rlostfound ) ;

	    if ( hti==1 ) h_r21_ht1_cdtt -> SetBinContent( rhist_bin, r21_val ) ;
	    if ( hti==2 ) h_r21_ht2_cdtt -> SetBinContent( rhist_bin, r21_val ) ;
	    if ( hti==3 ) h_r21_ht3_cdtt -> SetBinContent( rhist_bin, r21_val ) ;
	    if ( hti==1 ) h_r21_ht1_cdtt -> SetBinError( rhist_bin, r21_err ) ;
	    if ( hti==2 ) h_r21_ht2_cdtt -> SetBinError( rhist_bin, r21_err ) ;
	    if ( hti==3 ) h_r21_ht3_cdtt -> SetBinError( rhist_bin, r21_err ) ;

	    if ( hti==1 ) {
	      double cdtt_evts_val(0.), cdtt_evts_err(0.) ;
	      cdtt_evts_val = mc_count[nbi][nji][hti][1] * r21_val ;
	      if ( mc_count[nbi][nji][hti][1] > 0 && r21_val > 0 ) { cdtt_evts_err = cdtt_evts_val * sqrt( 1./mc_count[nbi][nji][hti][1] + pow( r21_err/r21_val, 2. ) ) ; }
	      h_lostlep_events_cdtt[nbi][nji] -> SetBinContent( 2, cdtt_evts_val ) ;
	      h_lostlep_events_cdtt[nbi][nji] -> SetBinError( 2, cdtt_evts_err ) ;
	      printf("          Events: Nb%d, Njet%d, HT%d, MHT2 :  MC = %4lld,  cdtt = %6.2f +/- %6.2f\n",
		     nbi, nji, hti, mc_count[nbi][nji][hti][2], cdtt_evts_val, cdtt_evts_err ) ;
	      h_nonzero_integral_count[nbi][nji] -> SetBinContent( 2, nonzero_integral_count[nbi][nji][hti][2] ) ;
	    }

	    if ( hti==2 ) {
	      double cdtt_evts_val(0.), cdtt_evts_err(0.) ;
	      cdtt_evts_val = mc_count[nbi][nji][hti][1] * r21_val ;
	      if ( mc_count[nbi][nji][hti][1] > 0 && r21_val > 0 ) { cdtt_evts_err = cdtt_evts_val * sqrt( 1./mc_count[nbi][nji][hti][1] + pow( r21_err/r21_val, 2. ) ) ; }
	      h_lostlep_events_cdtt[nbi][nji] -> SetBinContent( 3, cdtt_evts_val ) ;
	      h_lostlep_events_cdtt[nbi][nji] -> SetBinError( 3, cdtt_evts_err ) ;
	      printf("          Events: Nb%d, Njet%d, HT%d, MHT2 :  MC = %4lld,  cdtt = %6.2f +/- %6.2f\n",
		     nbi, nji, hti, mc_count[nbi][nji][hti][2], cdtt_evts_val, cdtt_evts_err ) ;
	      h_nonzero_integral_count[nbi][nji] -> SetBinContent( 3, nonzero_integral_count[nbi][nji][hti][2] ) ;
	    }

	    if ( hti==3 ) {
	      double cdtt_evts_val(0.), cdtt_evts_err(0.) ;
	      cdtt_evts_val = mc_count[nbi][nji][hti][1] * r21_val ;
	      if ( mc_count[nbi][nji][hti][1] > 0 && r21_val > 0 ) { cdtt_evts_err = cdtt_evts_val * sqrt( 1./mc_count[nbi][nji][hti][1] + pow( r21_err/r21_val, 2. ) ) ; }
	      h_lostlep_events_cdtt[nbi][nji] -> SetBinContent( 4, cdtt_evts_val ) ;
	      h_lostlep_events_cdtt[nbi][nji] -> SetBinError( 4, cdtt_evts_err ) ;
	      printf("          Events: Nb%d, Njet%d, HT%d, MHT2 :  MC = %4lld,  cdtt = %6.2f +/- %6.2f\n",
		     nbi, nji, hti, mc_count[nbi][nji][hti][2], cdtt_evts_val, cdtt_evts_err ) ;
	      h_nonzero_integral_count[nbi][nji] -> SetBinContent( 4, nonzero_integral_count[nbi][nji][hti][2] ) ;
	    }
	  }


	  if ( hti > 1 ) {

	    double r31_val = 0. ;
	    double r31_err = 0. ;
	    double r31_err_stat = 0. ;
	    double r31_err_rlostfound = 0. ;
	    if ( integral_sum[nbi][nji][hti][1] > 0. ) {
	      r31_val = integral_sum[nbi][nji][hti][3] / integral_sum[nbi][nji][hti][1] ;
	    }
	    if ( pass > 0 ) {
	      r31_err = sqrt( r31_err2_sum[nbi][nji][hti] + r31_err2_sum_for_rlostfound_err[nbi][nji][hti] ) ;
	      r31_err_stat = sqrt( r31_err2_sum[nbi][nji][hti] ) ;
	      r31_err_rlostfound = sqrt( r31_err2_sum_for_rlostfound_err[nbi][nji][hti] ) ;
	    }

	    double mc_r31_val = 0. ;
	    double mc_r31_err = 0. ;
	    if ( mc_count[nbi][nji][hti][1] > 0. ) {
	      mc_r31_val = (1.*mc_count[nbi][nji][hti][3])/(1.*mc_count[nbi][nji][hti][1]) ;
	      if ( mc_count[nbi][nji][hti][3] > 0. ) {
		mc_r31_err = mc_r31_val * sqrt( 1./mc_count[nbi][nji][hti][1] + 1./mc_count[nbi][nji][hti][3] ) ;
	      }
	    }
	    if ( pass == 0 ) {

	      printf("      Nb%d, Njet%d, HT%d : DeltaThetaT R31 = %6.4f,   MC val = %6.4f +/- %6.4f   ( %3lld / %4lld )\n", nbi, nji, hti, r31_val, mc_r31_val, mc_r31_err,
		     mc_count[nbi][nji][hti][3], mc_count[nbi][nji][hti][1] ) ;

	    } else {

	      printf("      Nb%d, Njet%d, HT%d : DeltaThetaT R31 = %6.4f +/- %6.4f,   MC val = %6.4f +/- %6.4f   ( %3lld / %4lld )  (stat = %6.4f, rlostfound = %6.4f)\n", nbi, nji, hti, r31_val, r31_err, mc_r31_val, mc_r31_err,
		     mc_count[nbi][nji][hti][3], mc_count[nbi][nji][hti][1], r31_err_stat, r31_err_rlostfound ) ;


	      if ( hti==2 ) h_r31_ht2_cdtt -> SetBinContent( rhist_bin, r31_val ) ;
	      if ( hti==3 ) h_r31_ht3_cdtt -> SetBinContent( rhist_bin, r31_val ) ;
	      if ( hti==2 ) h_r31_ht2_cdtt -> SetBinError( rhist_bin, r31_err ) ;
	      if ( hti==3 ) h_r31_ht3_cdtt -> SetBinError( rhist_bin, r31_err ) ;

	      if ( hti==2 ) {
		double cdtt_evts_val(0.), cdtt_evts_err(0.) ;
		cdtt_evts_val = mc_count[nbi][nji][hti][1] * r31_val ;
		if ( mc_count[nbi][nji][hti][1] > 0 && r31_val > 0 ) { cdtt_evts_err = cdtt_evts_val * sqrt( 1./mc_count[nbi][nji][hti][1] + pow( r31_err/r31_val, 2. ) ) ; }
		h_lostlep_events_cdtt[nbi][nji] -> SetBinContent( 6, cdtt_evts_val ) ;
		h_lostlep_events_cdtt[nbi][nji] -> SetBinError( 6, cdtt_evts_err ) ;
		printf("          Events: Nb%d, Njet%d, HT%d, MHT3 :  MC = %4lld,  cdtt = %6.2f +/- %6.2f\n",
		       nbi, nji, hti, mc_count[nbi][nji][hti][3], cdtt_evts_val, cdtt_evts_err ) ;
		h_nonzero_integral_count[nbi][nji] -> SetBinContent( 6, nonzero_integral_count[nbi][nji][hti][3] ) ;
	      }

	      if ( hti==3 ) {
		double cdtt_evts_val(0.), cdtt_evts_err(0.) ;
		cdtt_evts_val = mc_count[nbi][nji][hti][1] * r31_val ;
		if ( mc_count[nbi][nji][hti][1] > 0 && r31_val > 0 ) { cdtt_evts_err = cdtt_evts_val * sqrt( 1./mc_count[nbi][nji][hti][1] + pow( r31_err/r31_val, 2. ) ) ; }
		h_lostlep_events_cdtt[nbi][nji] -> SetBinContent( 7, cdtt_evts_val ) ;
		h_lostlep_events_cdtt[nbi][nji] -> SetBinError( 7, cdtt_evts_err ) ;
		printf("          Events: Nb%d, Njet%d, HT%d, MHT3 :  MC = %4lld,  cdtt = %6.2f +/- %6.2f\n",
		       nbi, nji, hti, mc_count[nbi][nji][hti][3], cdtt_evts_val, cdtt_evts_err ) ;
		h_nonzero_integral_count[nbi][nji] -> SetBinContent( 7, nonzero_integral_count[nbi][nji][hti][3] ) ;
	      }

	    }
	  }

	  printf("\n") ;

	} // hti
      } // nji
    } // nbi

    //-- MHT2 ratio superbins
    double mhtratio_val_rsb_mht2[100] ;
    double mhtratio_err_rsb_mht2[100] ;
    for ( int rsbi_mht2=0; rsbi_mht2<nrsb_mht2; rsbi_mht2++ ) {

      printf("  RSBI %2d  %-25s:  MHT1 %9.1f from %9lld non-zero contributions.\n", rsbi_mht2, rsuperbin_name_mht2[rsbi_mht2],
	     rsb_integral_sum_for_r21[rsbi_mht2][1], rsb_nonzero_integral_count_for_r21[rsbi_mht2][1] ) ;
      printf("  RSBI %2d  %-25s:  MHT2 %9.1f from %9lld non-zero contributions.\n", rsbi_mht2, rsuperbin_name_mht2[rsbi_mht2],
	     rsb_integral_sum_for_r21[rsbi_mht2][2], rsb_nonzero_integral_count_for_r21[rsbi_mht2][2] ) ;

      double r21_val = 0. ;
      double r21_err = 0. ;
      double r21_err_stat = 0. ;
      double r21_err_rlostfound = 0. ;
      if ( rsb_integral_sum_for_r21[rsbi_mht2][1] > 0. ) {
	r21_val = rsb_integral_sum_for_r21[rsbi_mht2][2] / rsb_integral_sum_for_r21[rsbi_mht2][1] ;
	if ( pass > 0 ) {
	  r21_err = sqrt( rsb_r21_err2_sum[rsbi_mht2] + rsb_r21_err2_sum_for_rlostfound_err[rsbi_mht2] ) ;
	  r21_err_stat = sqrt( rsb_r21_err2_sum[rsbi_mht2] ) ;
	  r21_err_rlostfound = sqrt( rsb_r21_err2_sum_for_rlostfound_err[rsbi_mht2] ) ;
	  mhtratio_val_rsb_mht2[rsbi_mht2] = r21_val ;
	  mhtratio_err_rsb_mht2[rsbi_mht2] = r21_err ;
	}
      }

      double mc_r21_val = 0. ;
      double mc_r21_err = 0. ;
      if ( rsb_mc_count_for_r21[rsbi_mht2][1] > 0. ) {
	mc_r21_val = (1.*rsb_mc_count_for_r21[rsbi_mht2][2])/(1.*rsb_mc_count_for_r21[rsbi_mht2][1]) ;
	if ( rsb_mc_count_for_r21[rsbi_mht2][2] > 0. ) {
	  mc_r21_err = mc_r21_val * sqrt( 1./rsb_mc_count_for_r21[rsbi_mht2][1] + 1./rsb_mc_count_for_r21[rsbi_mht2][2] ) ;
	}
      }

      if ( pass == 0 ) {
	printf("      RSBI %2d  %-25s : DeltaThetaT R21 = %6.4f,   MC val = %6.4f +/- %6.4f   ( %3lld / %4lld )\n",
	       rsbi_mht2, rsuperbin_name_mht2[rsbi_mht2], r21_val, mc_r21_val, mc_r21_err, rsb_mc_count_for_r21[rsbi_mht2][2], rsb_mc_count_for_r21[rsbi_mht2][1] ) ;
      } else {

	printf("      RSBI %2d  %-25s : DeltaThetaT R21 = %6.4f +/- %6.4f,   MC val = %6.4f +/- %6.4f   ( %3lld / %4lld )  (stat = %6.4f, rlostfound = %6.4f)\n",
	       rsbi_mht2, rsuperbin_name_mht2[rsbi_mht2], r21_val, r21_err, mc_r21_val, mc_r21_err,
	       rsb_mc_count_for_r21[rsbi_mht2][2], rsb_mc_count_for_r21[rsbi_mht2][1], r21_err_stat, r21_err_rlostfound ) ;

	double cdtt_evts_val(0.), cdtt_evts_err(0.) ;
	cdtt_evts_val = rsb_mc_count_for_r21[rsbi_mht2][1] * r21_val ;
	if ( rsb_mc_count_for_r21[rsbi_mht2][1] > 0 && r21_val > 0 ) { cdtt_evts_err = cdtt_evts_val * sqrt( 1./rsb_mc_count_for_r21[rsbi_mht2][1] + pow( r21_err/r21_val, 2. ) ) ; }
	printf("         RSBI Events: %2d  %-25s :  MC = %4lld,  cdtt = %6.2f +/- %6.2f\n",
	       rsbi_mht2, rsuperbin_name_mht2[rsbi_mht2], rsb_mc_count_for_r21[rsbi_mht2][2], cdtt_evts_val, cdtt_evts_err ) ;

	int hsbi = rsbi_mht2 + 2;
	if ( rsbi_mht2 > (nrsb_mht2/2-1) ) hsbi ++ ;
	h_lostlep_events_cdtt_superbins -> SetBinContent( hsbi, cdtt_evts_val ) ;
	h_lostlep_events_cdtt_superbins -> SetBinError( hsbi, cdtt_evts_err ) ;

	h_mhtratio_cdtt_superbins -> SetBinContent( hsbi, r21_val ) ;
	h_mhtratio_cdtt_superbins -> SetBinError( hsbi, r21_err ) ;

	h_nonzero_integral_count_superbins -> SetBinContent( hsbi, rsb_nonzero_integral_count_for_r21[rsbi_mht2][2] ) ;

      }

      printf("\n") ;

    } // rsbi_mht2

    //-- MHT3 ratio superbins
    double mhtratio_val_rsb_mht3[100] ;
    double mhtratio_err_rsb_mht3[100] ;
    for ( int rsbi_mht3=0; rsbi_mht3<nrsb_mht3; rsbi_mht3++ ) {

      printf("  RSBI %2d  %-25s:  MHT1 %9.1f from %9lld non-zero contributions.\n", rsbi_mht3, rsuperbin_name_mht3[rsbi_mht3],
	     rsb_integral_sum_for_r31[rsbi_mht3][1], rsb_nonzero_integral_count_for_r31[rsbi_mht3][1] ) ;
      printf("  RSBI %2d  %-25s:  MHT3 %9.1f from %9lld non-zero contributions.\n", rsbi_mht3, rsuperbin_name_mht3[rsbi_mht3],
	     rsb_integral_sum_for_r31[rsbi_mht3][3], rsb_nonzero_integral_count_for_r31[rsbi_mht3][3] ) ;

      double r31_val = 0. ;
      double r31_err = 0. ;
      double r31_err_stat = 0. ;
      double r31_err_rlostfound = 0. ;
      if ( rsb_integral_sum_for_r31[rsbi_mht3][1] > 0. ) {
	r31_val = rsb_integral_sum_for_r31[rsbi_mht3][3] / rsb_integral_sum_for_r31[rsbi_mht3][1] ;
	if ( pass > 0 ) {
	  r31_err = sqrt( rsb_r31_err2_sum[rsbi_mht3] + rsb_r31_err2_sum_for_rlostfound_err[rsbi_mht3] ) ;
	  r31_err_stat = sqrt( rsb_r31_err2_sum[rsbi_mht3] ) ;
	  r31_err_rlostfound = sqrt( rsb_r31_err2_sum_for_rlostfound_err[rsbi_mht3] ) ;
	  mhtratio_val_rsb_mht3[rsbi_mht3] = r31_val ;
	  mhtratio_err_rsb_mht3[rsbi_mht3] = r31_err ;
	}
      }

      double mc_r31_val = 0. ;
      double mc_r31_err = 0. ;
      if ( rsb_mc_count_for_r31[rsbi_mht3][1] > 0. ) {
	mc_r31_val = (1.*rsb_mc_count_for_r31[rsbi_mht3][3])/(1.*rsb_mc_count_for_r31[rsbi_mht3][1]) ;
	if ( rsb_mc_count_for_r31[rsbi_mht3][3] > 0. ) {
	  mc_r31_err = mc_r31_val * sqrt( 1./rsb_mc_count_for_r31[rsbi_mht3][1] + 1./rsb_mc_count_for_r31[rsbi_mht3][3] ) ;
	}
      }

      if ( pass == 0 ) {
	printf("      RSBI %2d  %-25s : DeltaThetaT R31 = %6.4f,   MC val = %6.4f +/- %6.4f   ( %3lld / %4lld )\n",
	       rsbi_mht3, rsuperbin_name_mht3[rsbi_mht3], r31_val, mc_r31_val, mc_r31_err, rsb_mc_count_for_r31[rsbi_mht3][3], rsb_mc_count_for_r31[rsbi_mht3][1] ) ;
      } else {

	printf("      RSBI %2d  %-25s : DeltaThetaT R31 = %6.4f +/- %6.4f,   MC val = %6.4f +/- %6.4f   ( %3lld / %4lld )  (stat = %6.4f, rlostfound = %6.4f)\n",
	       rsbi_mht3, rsuperbin_name_mht3[rsbi_mht3], r31_val, r31_err, mc_r31_val, mc_r31_err,
	       rsb_mc_count_for_r31[rsbi_mht3][3], rsb_mc_count_for_r31[rsbi_mht3][1], r31_err_stat, r31_err_rlostfound ) ;

	double cdtt_evts_val(0.), cdtt_evts_err(0.) ;
	cdtt_evts_val = rsb_mc_count_for_r31[rsbi_mht3][1] * r31_val ;
	if ( rsb_mc_count_for_r31[rsbi_mht3][1] > 0 && r31_val > 0 ) { cdtt_evts_err = cdtt_evts_val * sqrt( 1./rsb_mc_count_for_r31[rsbi_mht3][1] + pow( r31_err/r31_val, 2. ) ) ; }
	printf("         RSBI Events: %2d  %-25s :  MC = %4lld,  cdtt = %6.2f +/- %6.2f\n",
	       rsbi_mht3, rsuperbin_name_mht3[rsbi_mht3], rsb_mc_count_for_r31[rsbi_mht3][3], cdtt_evts_val, cdtt_evts_err ) ;

	int hsbi = rsbi_mht3 + nrsb_mht2+4;
	h_lostlep_events_cdtt_superbins -> SetBinContent( hsbi, cdtt_evts_val ) ;
	h_lostlep_events_cdtt_superbins -> SetBinError( hsbi, cdtt_evts_err ) ;

	h_mhtratio_cdtt_superbins -> SetBinContent( hsbi, r31_val ) ;
	h_mhtratio_cdtt_superbins -> SetBinError( hsbi, r31_err ) ;

	h_nonzero_integral_count_superbins -> SetBinContent( hsbi, rsb_nonzero_integral_count_for_r31[rsbi_mht3][3] ) ;

      }

      printf("\n") ;

    } // rsbi_mht3

    //-- MHT2 analysis superbins
    for ( int asbi_mht2=0; asbi_mht2<nasb_mht2; asbi_mht2++ ) {

      double r21_val = mhtratio_val_rsb_mht2[ asuperbin_rsb_index_mht2[asbi_mht2] ] ;
      double r21_err = mhtratio_err_rsb_mht2[ asuperbin_rsb_index_mht2[asbi_mht2] ] ;

      double mc_r21_val = 0. ;
      double mc_r21_err = 0. ;
      if ( asb_mc_count_for_r21[asbi_mht2][1] > 0. ) {
	mc_r21_val = (1.*asb_mc_count_for_r21[asbi_mht2][2])/(1.*asb_mc_count_for_r21[asbi_mht2][1]) ;
	if ( asb_mc_count_for_r21[asbi_mht2][2] > 0. ) {
	  mc_r21_err = mc_r21_val * sqrt( 1./asb_mc_count_for_r21[asbi_mht2][1] + 1./asb_mc_count_for_r21[asbi_mht2][2] ) ;
	}
      }

      if ( pass > 0 ) {

	printf("      ASBI %2d  %-25s : DeltaThetaT R21 = %6.4f +/- %6.4f,   MC val = %6.4f +/- %6.4f   ( %3lld / %4lld )\n",
	       asbi_mht2, asuperbin_name_mht2[asbi_mht2], r21_val, r21_err, mc_r21_val, mc_r21_err,
	       asb_mc_count_for_r21[asbi_mht2][2], asb_mc_count_for_r21[asbi_mht2][1] ) ;

	double cdtt_evts_val(0.), cdtt_evts_err(0.) ;
	cdtt_evts_val = asb_mc_count_for_r21[asbi_mht2][1] * r21_val ;
	if ( asb_mc_count_for_r21[asbi_mht2][1] > 0 && r21_val > 0 ) { cdtt_evts_err = cdtt_evts_val * sqrt( 1./asb_mc_count_for_r21[asbi_mht2][1] + pow( r21_err/r21_val, 2. ) ) ; }
	printf("         ASBI Events: %2d  %-25s :  MC = %4lld +/- %6.2f,  cdtt = %6.2f +/- %6.2f\n",
	       asbi_mht2, asuperbin_name_mht2[asbi_mht2], asb_mc_count_for_r21[asbi_mht2][2], sqrt(asb_mc_count_for_r21[asbi_mht2][2]), cdtt_evts_val, cdtt_evts_err ) ;

	int hsbi = asbi_mht2 + 2;
	if ( asbi_mht2 > (nasb_mht2/2-1) ) hsbi ++ ;
	h_lostlep_events_cdtt_asuperbins -> SetBinContent( hsbi, cdtt_evts_val ) ;
	h_lostlep_events_cdtt_asuperbins -> SetBinError( hsbi, cdtt_evts_err ) ;

	///  h_mhtratio_mc_superbins -> SetBinContent( hsbi, mc_r21_val ) ;
	///  h_mhtratio_mc_superbins -> SetBinError( hsbi, mc_r21_err ) ;

	///  h_mhtratio_cdtt_superbins -> SetBinContent( hsbi, r21_val ) ;
	///  h_mhtratio_cdtt_superbins -> SetBinError( hsbi, r21_err ) ;

      }

      printf("\n") ;

    } // asbi_mht2



    //-- MHT3 analysis superbins
    for ( int asbi_mht3=0; asbi_mht3<nasb_mht3; asbi_mht3++ ) {

      double r31_val = mhtratio_val_rsb_mht3[ asuperbin_rsb_index_mht3[asbi_mht3] ] ;
      double r31_err = mhtratio_err_rsb_mht3[ asuperbin_rsb_index_mht3[asbi_mht3] ] ;


      double mc_r31_val = 0. ;
      double mc_r31_err = 0. ;
      if ( asb_mc_count_for_r31[asbi_mht3][1] > 0. ) {
	mc_r31_val = (1.*asb_mc_count_for_r31[asbi_mht3][3])/(1.*asb_mc_count_for_r31[asbi_mht3][1]) ;
	if ( asb_mc_count_for_r31[asbi_mht3][3] > 0. ) {
	  mc_r31_err = mc_r31_val * sqrt( 1./asb_mc_count_for_r31[asbi_mht3][1] + 1./asb_mc_count_for_r31[asbi_mht3][3] ) ;
	}
      }

      if ( pass > 0 ) {

	printf("      ASBI %2d  %-25s : DeltaThetaT R31 = %6.4f +/- %6.4f,   MC val = %6.4f +/- %6.4f   ( %3lld / %4lld )\n",
	       asbi_mht3, asuperbin_name_mht3[asbi_mht3], r31_val, r31_err, mc_r31_val, mc_r31_err,
	       asb_mc_count_for_r31[asbi_mht3][3], asb_mc_count_for_r31[asbi_mht3][1] ) ;

	double cdtt_evts_val(0.), cdtt_evts_err(0.) ;
	cdtt_evts_val = asb_mc_count_for_r31[asbi_mht3][1] * r31_val ;
	if ( asb_mc_count_for_r31[asbi_mht3][1] > 0 && r31_val > 0 ) { cdtt_evts_err = cdtt_evts_val * sqrt( 1./asb_mc_count_for_r31[asbi_mht3][1] + pow( r31_err/r31_val, 2. ) ) ; }
	printf("         ASBI Events: %2d  %-25s :  MC = %4lld +/- %6.2f,  cdtt = %6.2f +/- %6.2f\n",
	       asbi_mht3, asuperbin_name_mht3[asbi_mht3], asb_mc_count_for_r31[asbi_mht3][3], sqrt(asb_mc_count_for_r31[asbi_mht3][3]), cdtt_evts_val, cdtt_evts_err ) ;

	int hsbi = asbi_mht3 + nasb_mht2+4;
	h_lostlep_events_cdtt_asuperbins -> SetBinContent( hsbi, cdtt_evts_val ) ;
	h_lostlep_events_cdtt_asuperbins -> SetBinError( hsbi, cdtt_evts_err ) ;

	//  h_mhtratio_mc_superbins -> SetBinContent( hsbi, mc_r31_val ) ;
	//  h_mhtratio_mc_superbins -> SetBinError( hsbi, mc_r31_err ) ;

	//  h_mhtratio_cdtt_superbins -> SetBinContent( hsbi, r31_val ) ;
	//  h_mhtratio_cdtt_superbins -> SetBinError( hsbi, r31_err ) ;

      }

      printf("\n") ;

    } // asbi_mht3


    printf("\n\n\n") ;
      
     
  } // pass
   
  saveHist( "macros/junews/lostleptons/529_trigger/outputfiles/analysis-output3.root", "h*" ) ;
   
}

void RTree::PrepareMCComponents(TString outdir) {

  // This fills the TH3 we use to look up RLF

  gStyle->SetOptStat(0);
  TH1::StatOverflows(kTRUE);

  TH1::SetDefaultSumw2(1); //trick to turn on Sumw2 for all histos
  TH3::SetDefaultSumw2(1);


  TH1D* h_wpt_all[5][5] ;
  TH1D* h_wpt_lostlep[5][5] ;
  TH1D* h_wpt_foundlep[5][5] ;
  TH1D* h_cdtt_all[5][5] ;
  TH1D* h_cdtt_lostlep[5][5] ;
  TH1D* h_cdtt_foundlep[5][5] ;


  for ( int nji=1; nji<nnjetbins; nji++ ) { // setup histograms
    for ( int hti=1; hti<nhtbins; hti++ ) {
      char hname[1000] ;
      char htitle[1000] ;

      sprintf( hname, "h_wpt_all_nj%d_ht%d", nji, hti ) ;
      sprintf( htitle, "All leptons Njet%d, HT%d;p_{T}^{W} [GeV];(a.u.)", nji, hti ) ;
      h_wpt_all[nji][hti] = new TH1D( hname, htitle, nwptbins_for_rlf, wptbins_for_rlf ) ;

      sprintf( hname, "h_wpt_lost_nj%d_ht%d", nji, hti ) ;
      sprintf( htitle, "Lost leptons Njet%d, HT%d;p_{T}^{W} [GeV];(a.u.)", nji, hti ) ;
      h_wpt_lostlep[nji][hti] = new TH1D( hname, htitle, nwptbins_for_rlf, wptbins_for_rlf ) ;

      sprintf( hname, "h_wpt_found_nj%d_ht%d", nji, hti ) ;
      sprintf( htitle, "Found leptons Njet%d, HT%d;p_{T}^{W} [GeV];(a.u.)", nji, hti ) ;
      h_wpt_foundlep[nji][hti] = new TH1D( hname, htitle, nwptbins_for_rlf, wptbins_for_rlf ) ;

      sprintf( hname, "h_cdtt_all_nj%d_ht%d", nji, hti ) ;
      sprintf( htitle, "Njet%d, HT%d;#frac{1}{2}(1-cos#Delta#theta_{T})", nji, hti ) ;
      h_cdtt_all[nji][hti] = new TH1D( hname, htitle, nbins_cdtt, xl_cdtt, xh_cdtt ) ;

      sprintf( hname, "h_cdtt_lostlep_nj%d_ht%d", nji, hti ) ;
      h_cdtt_lostlep[nji][hti] = new TH1D( hname, htitle, nbins_cdtt, xl_cdtt, xh_cdtt ) ;

      sprintf( hname, "h_cdtt_foundlep_nj%d_ht%d", nji, hti ) ;
      h_cdtt_foundlep[nji][hti] = new TH1D( hname, htitle, nbins_cdtt, xl_cdtt, xh_cdtt ) ;
    }//hti
  }//nji
  
  //--- found leptons must pass these cuts (trigger and control sample selection).
  ///////////double mht_cut = 200. ;
  double mht_cut = 125. ;
  double mt_cut = 100. ;
  double lepton_pt_cut = 20. ;

  if (fChain == 0) return;

  Long64_t nentries = fChain->GetEntries();

  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    if ( jentry%(nentries/100) == 0 && jentry>0 ) { printf("  %9llu out of %9llu (%2lld%%)\n", jentry, nentries, ((100*jentry)/nentries)+1 ) ; fflush(stdout) ; }
    
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;

    // Baseline selection:
    //if (!(mht30>200&&min_delta_phi_mht_N>6&&num_jets_pt30>=4&&ht30>500)) continue;
    // one true e/mu
    if (!(num_true_mus+num_true_els==1 && num_true_had_taus==0)) continue;

    // find the true lepton, get info
    double gen_w1_pt_from_mht_and_trk(0.), cosdtt_gentrk_mht(0.);
    if (num_true_mus==1) {
      gen_w1_pt_from_mht_and_trk=true_mu_ptW->at(0);
      cosdtt_gentrk_mht = cos(true_mu_dTT->at(0));
    } else if (num_true_els==1) {
      gen_w1_pt_from_mht_and_trk=true_el_ptW->at(0);
      cosdtt_gentrk_mht = cos(true_el_dTT->at(0));
    }
    
    int nji = -1 ;
    int hti = -1 ;

    for ( int i=1; i<nnjetbins; i++ ) { if ( num_jets_pt30 > njetbins[i] && num_jets_pt30 < njetbins[i+1] ) { nji = i ; break ; } }
    if ( nji < 1 ) continue ;

    for ( int i=1; i<nhtbins  ; i++ ) { if ( ht30 > htbins[i] && ht30 < htbins[i+1] ) { hti = i ; break ; } }
    if ( hti < 1 ) continue ;

    h_cdtt_all[nji][hti] -> Fill( 0.5*(1.-cosdtt_gentrk_mht) ) ;
    h_wpt_all[nji][hti] -> Fill( gen_w1_pt_from_mht_and_trk ) ;

    if (num_true_mus==1) {    
      if (num_veto_mus==0) { // lost
	h_wpt_lostlep[nji][hti] -> Fill( true_mu_ptW->at(0) ) ;
	h_cdtt_lostlep[nji][hti] -> Fill( 0.5*(1.-cos(true_mu_dTT->at(0)) ) ) ;
      } else if (num_veto_mus==1) { // found
	for (unsigned int imu(0); imu<mu_pt->size(); imu++) {
	  if (mu_vid->at(imu)&&mu_mini_iso_tr02->at(imu)<0.2&&mu_mT->at(imu)<100) {
	    h_wpt_foundlep[nji][hti] -> Fill( mu_ptW->at(imu) ) ;
	    h_cdtt_foundlep[nji][hti] -> Fill( 0.5*(1.-cos(mu_dTT->at(imu)) ) ) ;
	  }
	}
      }
    }
    // electrons
    else if (num_true_els==1) {
      if (num_veto_els==0) { // lost
	h_wpt_lostlep[nji][hti] -> Fill( true_el_ptW->at(0) ) ;
	h_cdtt_foundlep[nji][hti] -> Fill( 0.5*(1.-cos(true_el_dTT->at(0)) ) ) ;
      } else if (num_veto_els==1) { // found
	for (unsigned int iel(0); iel<el_pt->size(); iel++) {
	  if (el_vid->at(iel)&&el_mini_iso_tr02->at(iel)<0.1&&el_mT->at(iel)<100) {
	    h_wpt_foundlep[nji][hti] -> Fill( el_ptW->at(iel) ) ;
	    h_cdtt_foundlep[nji][hti] -> Fill( 0.5*(1.-cos(el_dTT->at(iel)) ) ) ;
	  }
	}
      }
    }

    
    
    // if (num_veto_mus==0&&num_veto_els==0) { // lost
    //   h_cdtt_lostlep[nji][hti] -> Fill( 0.5*(1.-cosdtt_gentrk_mht) ) ;
    //   h_wpt_lostlep[nji][hti] -> Fill( gen_w1_pt_from_mht_and_trk ) ;
    // } else if (num_veto_els + num_veto_mus == 1) { // found
    //   double mt_rec_mht(0.), rec_lep_pt(0.); 
    //   if (num_veto_mus==1) {
    // 	for (unsigned int imu(0); imu<mu_pt->size(); imu++) {
    // 	  if (mu_veto->at(imu)>10&&mu_mT->at(imu)<100) {
    // 	    mt_rec_mht = mu_mT->at(imu);
    // 	    rec_lep_pt = mu_pt->at(imu);
    // 	    break;
    // 	  }
    // 	}
    //   } else if (num_veto_els==1) {
    // 	for (unsigned int iel(0); iel<el_pt->size(); iel++) {
    // 	  if (el_veto->at(iel)&&el_mT->at(iel)<100) {
    // 	    mt_rec_mht = el_mT->at(iel);
    // 	    rec_lep_pt = el_pt->at(iel);
    // 	    break;
    // 	  }
    // 	}
    //   }
    //   if (mht30>mht_cut && mt_rec_mht<mt_cut && rec_lep_pt>lepton_pt_cut) {
    // 	h_cdtt_foundlep[nji][hti] -> Fill( 0.5*(1.-cosdtt_gentrk_mht) ) ;
    // 	h_wpt_foundlep[nji][hti] -> Fill( gen_w1_pt_from_mht_and_trk ) ;
    //   }
    // }
  } // end loop over events
    
  //}


  for ( int nji=1; nji<nnjetbins; nji++ ) {
    for ( int hti=1; hti<nhtbins; hti++ ) {

      char hname[1000] ;
      char htitle[1000] ;

      sprintf( hname, "h_wpt_lostlep_fraction_nj%d_ht%d", nji, hti ) ;
      sprintf( htitle, "Njet%d, HT%d;p_{T}^{W} [GeV];Lost lepton fraction", nji, hti ) ;
      make_frac( h_wpt_lostlep[nji][hti], h_wpt_all[nji][hti], hname, htitle ) ;

      sprintf( hname, "h_wpt_foundlep_fraction_nj%d_ht%d", nji, hti ) ;
      sprintf( htitle, "Njet%d, HT%d;p_{T}^{W} [GeV];Found lepton fraction", nji, hti ) ;
      make_frac( h_wpt_foundlep[nji][hti], h_wpt_all[nji][hti], hname, htitle ) ;

      sprintf( hname, "h_wpt_lostfound_ratio_nj%d_ht%d", nji, hti ) ;
      sprintf( htitle, "Njet%d, HT%d;p_{T}^{W} [GeV];RLF", nji, hti ) ;
      make_ratio( h_wpt_lostlep[nji][hti], h_wpt_foundlep[nji][hti], hname, htitle ) ;

    } // hti
  } // nji
  
   
  char ofile[10000] ;
  sprintf( ofile, "%sMC/MC-components.root", outdir.Data() ) ;
  printf("\n\n Saving histograms in %s\n", ofile ) ;
  saveHist( ofile, "h*" ) ;
  printf("\n\n\n") ;
  
  
}


