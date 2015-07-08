
#include "TCanvas.h"
#include "TH1F.h"
#include "TDirectory.h"
#include "TStyle.h"
#include "TLine.h"
#include "TText.h"

#include "histio.C"

   double mht_max(1000.) ;

   void do_numbers( TH1F* hp_cdtt, TH1F* hp_mc ) ;

   void do_plots( TH1F* hp_cdtt, TH1F* hp_mc ) ;

   void draw_analysis_mht_plots3( const char* infile = "outputfiles/analysis-output3-default.root", bool with_numbers = true, const char* save_file_base = "plot-dtt-analysis3" ) {

      gDirectory -> Delete( "h*" ) ;

      gStyle -> SetOptStat(0) ;

      loadHist( infile ) ;

      int wx(50), wy(50) ;





    //--- Canvas by Nb, 3x3 Njet vs HT grid

      for ( int nbi=0; nbi<=3; nbi++ ) {

         char cname[100] ;
         char ctitle[100] ;
         sprintf( cname, "can_draw_dtt_analysis1_nb%d", nbi ) ;
         sprintf( ctitle, "MHT distributions, Nb%d", nbi ) ;
         TCanvas* can = new TCanvas( cname, ctitle, 900, 900 ) ;
         can -> Divide( 3, 3 ) ;

         can -> SetWindowPosition( wx, wy ) ; wx += 50 ; wy += 20 ;

         int ci(1) ;

         for ( int hti=1; hti<4; hti++ ) {
            for ( int nji=1; nji<4; nji++ ) {

               can -> cd( ci++ ) ;

               char hname[100] ;

               sprintf( hname, "h_lostlep_mht_cdtt_nb%d_nj%d_ht%d_80bin", nbi, nji, hti ) ;
               TH1F* hpll_cdtt = (TH1F*) gDirectory -> FindObject( hname ) ;
               if ( hpll_cdtt == 0x0 ) { printf("\n\n *** Missing hist %s\n\n", hname ) ; return ; }

               sprintf( hname, "h_lostlep_mht_mc_nb%d_nj%d_ht%d_80bin", nbi, nji, hti ) ;
               TH1F* hpll_mc = (TH1F*) gDirectory -> FindObject( hname ) ;
               if ( hpll_mc == 0x0 ) { printf("\n\n *** Missing hist %s\n\n", hname ) ; return ; }

               do_plots( hpll_cdtt, hpll_mc ) ;
               if ( with_numbers) do_numbers( hpll_cdtt, hpll_mc ) ;

            } // nji
         } // hti


         char savefile[10000] ;
         sprintf( savefile, "outputfiles/%s-mht-nb%d.pdf", save_file_base, nbi ) ;
         can -> SaveAs( savefile ) ;

      } // nbi.




    //--- Canvas for sum over Nb, 3x3 Njet vs HT grid


      { // sum over Nb

         char cname[100] ;
         char ctitle[100] ;
         sprintf( cname, "can_draw_dtt_analysis1_nball" ) ;
         sprintf( ctitle, "MHT distributions, all Nb" ) ;
         TCanvas* can = new TCanvas( cname, ctitle, 900, 900 ) ;
         can -> Divide( 3, 3 ) ;

         can -> SetWindowPosition( wx, wy ) ; wx += 50 ; wy += 20 ;

         int ci(1) ;

         for ( int hti=1; hti<4; hti++ ) {
            for ( int nji=1; nji<4; nji++ ) {

               can -> cd( ci++ ) ;

               char hname[1000] ;
               char htitle[1000] ;

               sprintf( hname, "h_lostlep_mht_cdtt_nb1_nj1_ht1_80bin" ) ;
               TH1F* hp_toclone = (TH1F*) gDirectory -> FindObject( hname ) ;
               if ( hp_toclone == 0x0 ) { printf("\n\n *** Missing hist %s\n\n", hname ) ; return ; }

               sprintf( hname, "h_mht_cdtt_nj%d_ht%d", nji, hti ) ;
               sprintf( htitle, "MHT, from cdtt, Njet%d, HT%d", nji, hti ) ;
               TH1F* hp_cdtt = (TH1F*) hp_toclone -> Clone( hname ) ;
               hp_cdtt -> Reset() ;
               hp_cdtt -> SetTitle( htitle ) ;

               sprintf( hname, "h_mht_mc_nj%d_ht%d", nji, hti ) ;
               sprintf( htitle, "MHT, from MC, Njet%d, HT%d", nji, hti ) ;
               TH1F* hp_mc = (TH1F*) hp_toclone -> Clone( hname ) ;
               hp_mc -> Reset() ;
               hp_mc -> SetTitle( htitle ) ;

               for ( int nbi=0; nbi<=3; nbi++ ) {
                  char hname[100] ;

                  sprintf( hname, "h_lostlep_mht_cdtt_nb%d_nj%d_ht%d_80bin", nbi, nji, hti ) ;
                  TH1F* hpll_cdtt = (TH1F*) gDirectory -> FindObject( hname ) ;
                  if ( hpll_cdtt == 0x0 ) { printf("\n\n *** Missing hist %s\n\n", hname ) ; return ; }

                  sprintf( hname, "h_lostlep_mht_mc_nb%d_nj%d_ht%d_80bin", nbi, nji, hti ) ;
                  TH1F* hpll_mc = (TH1F*) gDirectory -> FindObject( hname ) ;
                  if ( hpll_mc == 0x0 ) { printf("\n\n *** Missing hist %s\n\n", hname ) ; return ; }

                  hp_cdtt -> Add( hpll_cdtt ) ;
                  hp_mc   -> Add( hpll_mc ) ;

               } // nbi.

               do_plots( hp_cdtt, hp_mc ) ;
               if ( with_numbers) do_numbers( hp_cdtt, hp_mc ) ;

            } // nji
         } // hti


         char savefile[10000] ;
         sprintf( savefile, "outputfiles/%s-mht-nball.pdf", save_file_base ) ;
         can -> SaveAs( savefile ) ;

      } // sum over Nb






     //--- Canvas for HT bins --------------------------------------------------------------------

      { // HT bins canvas

         char cname[100] ;
         char ctitle[100] ;
         sprintf( cname, "can_draw_dtt_analysis1_ht_bins" ) ;
         sprintf( ctitle, "MHT distributions, HT bins" ) ;
         TCanvas* can = new TCanvas( cname, ctitle, 400, 1100 ) ;
         can -> Divide( 1, 3 ) ;

         can -> SetWindowPosition( wx, wy ) ; wx += 50 ; wy += 20 ;

         int ci(1) ;

         for ( int hti=1; hti<4; hti++ ) {

            can -> cd( ci++ ) ;

            char hname[1000] ;
            char htitle[1000] ;

            sprintf( hname, "h_lostlep_mht_cdtt_nb1_nj1_ht%d_80bin", hti ) ;
            TH1F* hp_toclone = (TH1F*) gDirectory -> FindObject( hname ) ;
            if ( hp_toclone == 0x0 ) { printf("\n\n *** Missing hist %s\n\n", hname ) ; return ; }

            sprintf( hname, "h_mht_cdtt_ht%d", hti ) ;
            sprintf( htitle, "MHT, from cdtt, HT%d", hti ) ;
            TH1F* hp_cdtt = (TH1F*) hp_toclone -> Clone( hname ) ;
            hp_cdtt -> Reset() ;
            hp_cdtt -> SetTitle( htitle ) ;

            sprintf( hname, "h_mht_mc_ht%d", hti ) ;
            sprintf( htitle, "MHT, from MC, HT%d", hti ) ;
            TH1F* hp_mc = (TH1F*) hp_toclone -> Clone( hname ) ;
            hp_mc -> Reset() ;
            hp_mc -> SetTitle( htitle ) ;

            for ( int nji=1; nji<4; nji++ ) {
               for ( int nbi=0; nbi<4; nbi++ ) {

                  sprintf( hname, "h_lostlep_mht_cdtt_nb%d_nj%d_ht%d_80bin", nbi, nji, hti ) ;
                  TH1F* hpll_cdtt = (TH1F*) gDirectory -> FindObject( hname ) ;
                  if ( hpll_cdtt == 0x0 ) { printf("\n\n *** Missing hist %s\n\n", hname ) ; return ; }

                  sprintf( hname, "h_lostlep_mht_mc_nb%d_nj%d_ht%d_80bin", nbi, nji, hti ) ;
                  TH1F* hpll_mc = (TH1F*) gDirectory -> FindObject( hname ) ;
                  if ( hpll_mc == 0x0 ) { printf("\n\n *** Missing hist %s\n\n", hname ) ; return ; }

                  hp_cdtt -> Add( hpll_cdtt ) ;
                  hp_mc   -> Add( hpll_mc ) ;

               } // nbi
            } // nji

            do_plots( hp_cdtt, hp_mc ) ;
            if ( with_numbers) do_numbers( hp_cdtt, hp_mc ) ;

         } // hti

         char savefile[10000] ;
         sprintf( savefile, "outputfiles/%s-mht-ht.pdf", save_file_base ) ;
         can -> SaveAs( savefile ) ;

      } // HT bins canvas



     //--- Canvas for Njet bins --------------------------------------------------------------------

      { // Njet bins canvas

         char cname[100] ;
         char ctitle[100] ;
         sprintf( cname, "can_draw_dtt_analysis1_njet_bins" ) ;
         sprintf( ctitle, "MHT distributions, Njet bins" ) ;
         TCanvas* can = new TCanvas( cname, ctitle, 400, 1100 ) ;
         can -> Divide( 1, 3 ) ;

         can -> SetWindowPosition( wx, wy ) ; wx += 50 ; wy += 20 ;

         int ci(1) ;

         for ( int nji=1; nji<4; nji++ ) {

            can -> cd( ci++ ) ;

            char hname[1000] ;
            char htitle[1000] ;

            sprintf( hname, "h_lostlep_mht_cdtt_nb1_nj1_ht1_80bin" ) ;
            TH1F* hp_toclone = (TH1F*) gDirectory -> FindObject( hname ) ;
            if ( hp_toclone == 0x0 ) { printf("\n\n *** Missing hist %s\n\n", hname ) ; return ; }

            sprintf( hname, "h_mht_cdtt_njet%d", nji ) ;
            sprintf( htitle, "MHT, from cdtt, Njet%d", nji ) ;
            TH1F* hp_cdtt = (TH1F*) hp_toclone -> Clone( hname ) ;
            hp_cdtt -> Reset() ;
            hp_cdtt -> SetTitle( htitle ) ;

            sprintf( hname, "h_mht_mc_njet%d", nji ) ;
            sprintf( htitle, "MHT, from MC, Njet%d", nji ) ;
            TH1F* hp_mc = (TH1F*) hp_toclone -> Clone( hname ) ;
            hp_mc -> Reset() ;
            hp_mc -> SetTitle( htitle ) ;

            for ( int hti=1; hti<4; hti++ ) {
               for ( int nbi=0; nbi<4; nbi++ ) {

                  sprintf( hname, "h_lostlep_mht_cdtt_nb%d_nj%d_ht%d_80bin", nbi, nji, hti ) ;
                  TH1F* hpll_cdtt = (TH1F*) gDirectory -> FindObject( hname ) ;
                  if ( hpll_cdtt == 0x0 ) { printf("\n\n *** Missing hist %s\n\n", hname ) ; return ; }

                  sprintf( hname, "h_lostlep_mht_mc_nb%d_nj%d_ht%d_80bin", nbi, nji, hti ) ;
                  TH1F* hpll_mc = (TH1F*) gDirectory -> FindObject( hname ) ;
                  if ( hpll_mc == 0x0 ) { printf("\n\n *** Missing hist %s\n\n", hname ) ; return ; }

                  hp_cdtt -> Add( hpll_cdtt ) ;
                  hp_mc   -> Add( hpll_mc ) ;

               } // nbi
            } // hti

            do_plots( hp_cdtt, hp_mc ) ;
            if ( with_numbers) do_numbers( hp_cdtt, hp_mc ) ;

         } // hti

         char savefile[10000] ;
         sprintf( savefile, "outputfiles/%s-mht-njet.pdf", save_file_base ) ;
         can -> SaveAs( savefile ) ;

      } // Njet bins canvas


     //--- Canvas for Nb bins --------------------------------------------------------------------

      { // Nb bins canvas

         char cname[100] ;
         char ctitle[100] ;
         sprintf( cname, "can_draw_dtt_analysis1_nb_bins" ) ;
         sprintf( ctitle, "MHT distributions, Nb bins" ) ;
         TCanvas* can = new TCanvas( cname, ctitle, 400, 1100 ) ;
         can -> Divide( 1, 4 ) ;

         can -> SetWindowPosition( wx, wy ) ; wx += 50 ; wy += 20 ;

         int ci(1) ;

         for ( int nbi=0; nbi<4; nbi++ ) {

            can -> cd( ci++ ) ;

            char hname[1000] ;
            char htitle[1000] ;

            sprintf( hname, "h_lostlep_mht_cdtt_nb1_nj1_ht1_80bin" ) ;
            TH1F* hp_toclone = (TH1F*) gDirectory -> FindObject( hname ) ;
            if ( hp_toclone == 0x0 ) { printf("\n\n *** Missing hist %s\n\n", hname ) ; return ; }

            sprintf( hname, "h_mht_cdtt_nb%d", nbi ) ;
            sprintf( htitle, "MHT, from cdtt, Nb%d", nbi ) ;
            TH1F* hp_cdtt = (TH1F*) hp_toclone -> Clone( hname ) ;
            hp_cdtt -> Reset() ;
            hp_cdtt -> SetTitle( htitle ) ;

            sprintf( hname, "h_mht_mc_nb%d", nbi ) ;
            sprintf( htitle, "MHT, from MC, Nb%d", nbi ) ;
            TH1F* hp_mc = (TH1F*) hp_toclone -> Clone( hname ) ;
            hp_mc -> Reset() ;
            hp_mc -> SetTitle( htitle ) ;

            for ( int hti=1; hti<4; hti++ ) {
               for ( int nji=1; nji<4; nji++ ) {

                  sprintf( hname, "h_lostlep_mht_cdtt_nb%d_nj%d_ht%d_80bin", nbi, nji, hti ) ;
                  TH1F* hpll_cdtt = (TH1F*) gDirectory -> FindObject( hname ) ;
                  if ( hpll_cdtt == 0x0 ) { printf("\n\n *** Missing hist %s\n\n", hname ) ; return ; }

                  sprintf( hname, "h_lostlep_mht_mc_nb%d_nj%d_ht%d_80bin", nbi, nji, hti ) ;
                  TH1F* hpll_mc = (TH1F*) gDirectory -> FindObject( hname ) ;
                  if ( hpll_mc == 0x0 ) { printf("\n\n *** Missing hist %s\n\n", hname ) ; return ; }

                  hp_cdtt -> Add( hpll_cdtt ) ;
                  hp_mc   -> Add( hpll_mc ) ;

               } // nji
            } // hti

            do_plots( hp_cdtt, hp_mc ) ;
            if ( with_numbers) do_numbers( hp_cdtt, hp_mc ) ;

         } // hti

         char savefile[10000] ;
         sprintf( savefile, "outputfiles/%s-mht-nb.pdf", save_file_base ) ;
         can -> SaveAs( savefile ) ;

      } // Nb bins canvas





     //--- Canvas for sum over all --------------------------------------------------------------------

      { // sum over all

         char cname[100] ;
         char ctitle[100] ;
         sprintf( cname, "can_draw_dtt_analysis1_all" ) ;
         sprintf( ctitle, "MHT distributions, all" ) ;
         TCanvas* can = new TCanvas( cname, ctitle, 700, 600 ) ;

         can -> SetWindowPosition( wx, wy ) ; wx += 50 ; wy += 20 ;

         char hname[1000] ;
         char htitle[1000] ;

         sprintf( hname, "h_lostlep_mht_cdtt_nb1_nj1_ht1_80bin" ) ;
         TH1F* hp_toclone = (TH1F*) gDirectory -> FindObject( hname ) ;
         if ( hp_toclone == 0x0 ) { printf("\n\n *** Missing hist %s\n\n", hname ) ; return ; }

         sprintf( hname, "h_mht_cdtt_all" ) ;
         sprintf( htitle, "MHT, from cdtt, all" ) ;
         TH1F* hp_cdtt = (TH1F*) hp_toclone -> Clone( hname ) ;
         hp_cdtt -> Reset() ;
         hp_cdtt -> SetTitle( htitle ) ;

         sprintf( hname, "h_mht_mc_all" ) ;
         sprintf( htitle, "MHT, from MC, all" ) ;
         TH1F* hp_mc = (TH1F*) hp_toclone -> Clone( hname ) ;
         hp_mc -> Reset() ;
         hp_mc -> SetTitle( htitle ) ;

         for ( int nbi=0; nbi<4; nbi++ ) {

            for ( int hti=1; hti<4; hti++ ) {
               for ( int nji=1; nji<4; nji++ ) {

                  sprintf( hname, "h_lostlep_mht_cdtt_nb%d_nj%d_ht%d_80bin", nbi, nji, hti ) ;
                  TH1F* hpll_cdtt = (TH1F*) gDirectory -> FindObject( hname ) ;
                  if ( hpll_cdtt == 0x0 ) { printf("\n\n *** Missing hist %s\n\n", hname ) ; return ; }

                  sprintf( hname, "h_lostlep_mht_mc_nb%d_nj%d_ht%d_80bin", nbi, nji, hti ) ;
                  TH1F* hpll_mc = (TH1F*) gDirectory -> FindObject( hname ) ;
                  if ( hpll_mc == 0x0 ) { printf("\n\n *** Missing hist %s\n\n", hname ) ; return ; }

                  hp_cdtt -> Add( hpll_cdtt ) ;
                  hp_mc   -> Add( hpll_mc ) ;

               } // nji
            } // hti

         } // nbi

         do_plots( hp_cdtt, hp_mc ) ;
         if ( with_numbers) do_numbers( hp_cdtt, hp_mc ) ;

         char savefile[10000] ;
         sprintf( savefile, "outputfiles/%s-mht-all.pdf", save_file_base ) ;
         can -> SaveAs( savefile ) ;

      } // sum over all


     //--- Canvas for superbins for MHT2, HT12 --------------------------------------------------------------------

      {

         char cname[100] ;
         char ctitle[100] ;
         sprintf( cname, "can_draw_dtt_analysis1_superbins_mht2_ht12" ) ;
         sprintf( ctitle, "MHT distributions for mht2 superbins, HT12 " ) ;
         TCanvas* can = new TCanvas( cname, ctitle, 1400, 700 ) ;
         can -> Divide(4,2) ;

         can -> SetWindowPosition( wx, wy ) ; wx += 50 ; wy += 20 ;

         char hname[1000] ;
         char htitle[1000] ;

         int ci(1) ;

         for ( int sbi=0; sbi<3; sbi++ ) {

            can -> cd(ci++ ) ;

            sprintf( hname, "h_lostlep_mht_cdtt_mht2_superbin%02d", sbi ) ;
            TH1F* hp_cdtt = (TH1F*) gDirectory -> FindObject( hname ) ;
            if ( hp_cdtt == 0x0 ) { printf("\n\n *** Missing hist %s\n\n", hname ) ; return ; }

            sprintf( hname, "h_lostlep_mht_mc_mht2_superbin%02d", sbi ) ;
            TH1F* hp_mc = (TH1F*) gDirectory -> FindObject( hname ) ;
            if ( hp_mc == 0x0 ) { printf("\n\n *** Missing hist %s\n\n", hname ) ; return ; }

            do_plots( hp_cdtt, hp_mc ) ;
            if ( with_numbers) do_numbers( hp_cdtt, hp_mc ) ;

         } // sbi


         char savefile[10000] ;
         sprintf( savefile, "outputfiles/%s-superbins-mht2-ht12.pdf", save_file_base ) ;
         can -> SaveAs( savefile ) ;

      } // superbins for mht2, ht12


     //--- Canvas for superbins for MHT2, HT3 --------------------------------------------------------------------

      {

         char cname[100] ;
         char ctitle[100] ;
         sprintf( cname, "can_draw_dtt_analysis1_superbins_mht2_ht3" ) ;
         sprintf( ctitle, "MHT distributions for mht2 superbins, HT3 " ) ;
         TCanvas* can = new TCanvas( cname, ctitle, 1400, 700 ) ;
         can -> Divide(4,2) ;

         can -> SetWindowPosition( wx, wy ) ; wx += 50 ; wy += 20 ;

         char hname[1000] ;
         char htitle[1000] ;

         int ci(1) ;

         for ( int sbi=3; sbi<6; sbi++ ) {

            can -> cd(ci++ ) ;

            sprintf( hname, "h_lostlep_mht_cdtt_mht2_superbin%02d", sbi ) ;
            TH1F* hp_cdtt = (TH1F*) gDirectory -> FindObject( hname ) ;
            if ( hp_cdtt == 0x0 ) { printf("\n\n *** Missing hist %s\n\n", hname ) ; return ; }

            sprintf( hname, "h_lostlep_mht_mc_mht2_superbin%02d", sbi ) ;
            TH1F* hp_mc = (TH1F*) gDirectory -> FindObject( hname ) ;
            if ( hp_mc == 0x0 ) { printf("\n\n *** Missing hist %s\n\n", hname ) ; return ; }

            do_plots( hp_cdtt, hp_mc ) ;
            if ( with_numbers) do_numbers( hp_cdtt, hp_mc ) ;

         } // sbi


         char savefile[10000] ;
         sprintf( savefile, "outputfiles/%s-superbins-mht2-ht3.pdf", save_file_base ) ;
         can -> SaveAs( savefile ) ;

      } // superbins for mht2, ht3



     //--- Canvas for superbins for MHT3 --------------------------------------------------------------------

      {

         char cname[100] ;
         char ctitle[100] ;
         sprintf( cname, "can_draw_dtt_analysis1_superbins_mht3" ) ;
         sprintf( ctitle, "MHT distributions for mht3 superbins" ) ;
         TCanvas* can = new TCanvas( cname, ctitle, 600, 900 ) ;
         can -> Divide(1,2) ;

         can -> SetWindowPosition( wx, wy ) ; wx += 50 ; wy += 20 ;

         char hname[1000] ;
         char htitle[1000] ;

         int ci(1) ;

         for ( int sbi=0; sbi<1; sbi++ ) {

            can -> cd(ci++ ) ;

            sprintf( hname, "h_lostlep_mht_cdtt_mht3_superbin%02d", sbi ) ;
            TH1F* hp_cdtt = (TH1F*) gDirectory -> FindObject( hname ) ;
            if ( hp_cdtt == 0x0 ) { printf("\n\n *** Missing hist %s\n\n", hname ) ; return ; }

            sprintf( hname, "h_lostlep_mht_mc_mht3_superbin%02d", sbi ) ;
            TH1F* hp_mc = (TH1F*) gDirectory -> FindObject( hname ) ;
            if ( hp_mc == 0x0 ) { printf("\n\n *** Missing hist %s\n\n", hname ) ; return ; }

            do_plots( hp_cdtt, hp_mc ) ;
            if ( with_numbers) do_numbers( hp_cdtt, hp_mc ) ;

         } // sbi


         char savefile[10000] ;
         sprintf( savefile, "outputfiles/%s-superbins-mht3.pdf", save_file_base ) ;
         can -> SaveAs( savefile ) ;

      } // superbins for mht3


   } // draw_analysis_mht_plots3

  //=============================================================================================================

   void do_plots( TH1F* hp_cdtt, TH1F* hp_mc ) {

      if ( hp_cdtt==0x0 ) return ;
      if ( hp_mc==0x0 ) return ;

      TLine* line = new TLine() ;
      line -> SetLineStyle(3) ;

      hp_cdtt -> SetLineColor(2) ;
      hp_mc -> SetLineColor(4) ;
      hp_mc -> SetAxisRange( 0., mht_max, "X" ) ;
      hp_mc -> SetMinimum(0.1) ;
      hp_mc -> Draw( "hist" ) ;
      hp_cdtt -> Draw( "hist same" ) ;
      gPad->SetLogy(1) ;
      float hmax = 1.5*(hp_mc -> GetMaximum()) ;
      line -> DrawLine( 300., 0.1, 300., hmax ) ;
      line -> DrawLine( 500., 0.1, 500., hmax ) ;
      line -> DrawLine( 750., 0.1, 750., hmax ) ;

   } // do_plots

  //=============================================================================================================

   void do_numbers( TH1F* hp_cdtt, TH1F* hp_mc ) {

      if ( hp_cdtt==0x0 ) return ;
      if ( hp_mc==0x0 ) return ;

      TText* text = new TText() ;
      text -> SetTextAlign( 22 ) ;

      int hb_mht1_low  = hp_cdtt -> GetXaxis() -> FindBin( 300.+1. ) ;
      int hb_mht1_high = hp_cdtt -> GetXaxis() -> FindBin( 500.-1. ) ;

      int hb_mht2_low  = hp_cdtt -> GetXaxis() -> FindBin( 500.+1. ) ;
      int hb_mht2_high = hp_cdtt -> GetXaxis() -> FindBin( 750.-1. ) ;

      int hb_mht3_low  = hp_cdtt -> GetXaxis() -> FindBin( 750.+1. ) ;
      int hb_mht3_high = hp_cdtt -> GetNbinsX() ;

      double bin1_mc = hp_mc -> Integral( hb_mht1_low, hb_mht1_high ) ;
      double bin2_mc = hp_mc -> Integral( hb_mht2_low, hb_mht2_high ) ;
      double bin3_mc = hp_mc -> Integral( hb_mht3_low, hb_mht3_high ) ;

      double bin1_cdtt = hp_cdtt -> Integral( hb_mht1_low, hb_mht1_high ) ;
      double bin2_cdtt = hp_cdtt -> Integral( hb_mht2_low, hb_mht2_high ) ;
      double bin3_cdtt = hp_cdtt -> Integral( hb_mht3_low, hb_mht3_high ) ;

      double hmax = 1.5*(hp_mc -> GetMaximum()) ;

      double plm = gStyle -> GetPadLeftMargin() ;
      double prm = gStyle -> GetPadRightMargin() ;
      double pmiddle = 1 - plm - prm ;


      double r21_mc = 0. ;
      double r31_mc = 0. ;
      if ( bin1_mc > 0. ) {
         r21_mc = bin2_mc / bin1_mc ;
         r31_mc = bin3_mc / bin1_mc ;
      }

      double r21_cdtt = 0. ;
      double r31_cdtt = 0. ;
      if ( bin1_cdtt > 0. ) {
         r21_cdtt = bin2_cdtt / bin1_cdtt ;
         r31_cdtt = bin3_cdtt / bin1_cdtt ;
      }


      char string[100] ;
      double tx ;

      ////tx = plm + 300.*pmiddle/1000. ;
      tx = plm + 400.*pmiddle/1000. ;
      sprintf( string, "%.1f", bin1_mc ) ;
      text -> SetTextColor(4) ;
      text -> DrawTextNDC( tx, 0.85, string ) ;
      sprintf( string, "%.1f", bin1_cdtt ) ;
      text -> SetTextColor(2) ;
      text -> DrawTextNDC( tx, 0.79, string ) ;


      tx = plm + 625.*pmiddle/1000. ;
      sprintf( string, "%.1f", bin2_mc ) ;
      text -> SetTextColor(4) ;
      text -> DrawTextNDC( tx, 0.85, string ) ;
      sprintf( string, "%.1f", bin2_cdtt ) ;
      text -> SetTextColor(2) ;
      text -> DrawTextNDC( tx, 0.79, string ) ;

      sprintf( string, "%.4f", r21_mc ) ;
      text -> SetTextColor(4) ;
      text -> DrawTextNDC( tx, 0.70, string ) ;
      sprintf( string, "%.4f", r21_cdtt ) ;
      text -> SetTextColor(2) ;
      text -> DrawTextNDC( tx, 0.64, string ) ;



      tx = plm + 875.*pmiddle/1000. ;
      sprintf( string, "%.1f", bin3_mc ) ;
      text -> SetTextColor(4) ;
      text -> DrawTextNDC( tx, 0.85, string ) ;
      sprintf( string, "%.1f", bin3_cdtt ) ;
      text -> SetTextColor(2) ;
      text -> DrawTextNDC( tx, 0.79, string ) ;

      sprintf( string, "%.4f", r31_mc ) ;
      text -> SetTextColor(4) ;
      text -> DrawTextNDC( tx, 0.70, string ) ;
      sprintf( string, "%.4f", r31_cdtt ) ;
      text -> SetTextColor(2) ;
      text -> DrawTextNDC( tx, 0.64, string ) ;


   } // do_numbers

  //=============================================================================================================













