#define FillInputHists_cxx
#include "FillInputHists.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include "macros/junews/lostleptons/plotting.h"
#include "macros/junews/lostleptons/utils.h"

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
double wptbins_for_rlf[15] = { 0., 25., 50., 75., 100., 150., 200., 250., 300., 350., 400., 500., 600., 750., 20000.  } ;
//-------

//--------------------------------------------------------------------------------------------------------

void FillInputHists::Run(TString outdir) {


  //--- found leptons must pass these cuts (trigger and control sample selection).
  ///////////double mht_cut = 200. ;
  double mht_cut = 125. ; // do we want a cut for the dTT shape?
  double mt_cut = 100. ;
  double lepton_pt_cut = 0. ; // no cut for lost-leptons



  if (fChain == 0) return;
  fChain->SetBranchStatus("SelectedPFCandidates*", 0);

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
      sprintf( htitle, "1/2(1-cos(dtt)), found lepton, Njet%d, HT%d", nji, hti ) ;
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

    if ( GenElecNum+GenMuNum != 1 ) continue ;

    bool hadTauEvent(false);
    for (unsigned int itau(0); itau<GenTauNum; itau++) {// remove hadronic tau?
      if (GenTau_GenTauHad[itau]>0) {hadTauEvent=true; break;}
    }
    if (hadTauEvent) continue;

    //if ( abs(gen_l1_flav) != 15 ) continue ; // ONLY look at taus

    //if ( abs(gen_l1_flav) == 15 && abs(gen_trk1_flav)>15 ) continue ; // remove hadronic taus

    if ( MHT < 125 ) continue ; // try a MHT cut (for the trigger) to see what happens.
    if (DeltaPhi1 <0.5 || DeltaPhi2 < 0.5 || DeltaPhi3 < 0.3) continue;// delta phi cut, for consistency
    if (HT < 500 || NJets < 4) continue; // binning baseline

    int nji = -1 ;
    int hti = -1 ;
    int wpti = -1 ;

    for ( int i=1; i<nnjetbins; i++ ) { if ( NJets > njetbins[i] && NJets < njetbins[i+1] ) { nji = i ; break ; } }
    if ( nji < 1 ) continue ;

    for ( int i=1; i<nhtbins  ; i++ ) { if ( HT > htbins[i] && HT < htbins[i+1] ) { hti = i ; break ; } }
    if ( hti < 1 ) continue ;

    double dTT(0.), WpT(0.);

        
    if (GenMuNum==1) {
      dTT = GetLeptonDeltaThetaT( GenMuPt[0], GenMuPhi[0], METPt, METPhi);
      WpT = GetWpT(GenMuPt[0], GenMuPhi[0], METPt, METPhi);
    } else if (GenElecNum==1) {
      dTT = GetLeptonDeltaThetaT( GenElecPt[0], GenElecPhi[0], METPt, METPhi);
      WpT = GetWpT(GenElecPt[0], GenElecPhi[0], METPt, METPhi);
    }
    
    h_cdtt_all[nji][hti] -> Fill( 0.5*(1.-cos(dTT)) ) ;

    h_wpt_all[nji][hti] -> Fill( WpT ) ;

    if ( (selectedIDIsoElectronsNum + selectedIDIsoMuonsNum) == 0 ) {

      h_cdtt_lostlep[nji][hti] -> Fill( 0.5*(1.-cos(dTT)) ) ;
      h_wpt_lostlep[nji][hti] -> Fill( WpT ) ;

    } else if ( (selectedIDIsoElectronsNum + selectedIDIsoMuonsNum) == 1 && MHT>mht_cut) {
      double LpT(0.), MT(0.);
      if (selectedIDIsoElectronsNum==1) {
	LpT=selectedIDIsoElectronsPt[0];
	MT=selectedIDIsoElectrons_MTW[0];
	// sanity check
	dTT = GetLeptonDeltaThetaT( selectedIDIsoElectronsPt[0], selectedIDIsoElectronsPhi[0], METPt, METPhi);
	WpT = GetWpT(selectedIDIsoElectronsPt[0], selectedIDIsoElectronsPhi[0], METPt, METPhi);
     } else if (selectedIDIsoMuonsNum==1) {
	LpT=selectedIDIsoMuonsPt[0];
	MT=selectedIDIsoMuons_MTW[0];
	// sanity check
 	dTT = GetLeptonDeltaThetaT( selectedIDIsoMuonsPt[0], selectedIDIsoMuonsPhi[0], METPt, METPhi);
	WpT = GetWpT(selectedIDIsoMuonsPt[0], selectedIDIsoMuonsPhi[0], METPt, METPhi);
     }
      if (MT<mt_cut && LpT>lepton_pt_cut  ) {
	h_cdtt_foundlep[nji][hti] -> Fill( 0.5*(1.-cos(dTT)) ) ;
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
  sprintf( ofile, "%s/outputfiles/cdtta-input-hists.root", outdir.Data() ) ;
  printf("\n\n Saving histograms in %s\n", ofile ) ;
  saveHist( ofile, "h*" ) ;
  printf("\n\n\n") ;

} // Loop.


//===================================================================================================================
