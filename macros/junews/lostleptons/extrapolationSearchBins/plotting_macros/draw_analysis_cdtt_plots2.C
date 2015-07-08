
#include "TCanvas.h"
#include "TH1F.h"
#include "TDirectory.h"
#include "TStyle.h"
#include "TLine.h"
#include "TText.h"

#include "histio.C"

   double met_max(1000.) ;

   void do_plots( TH1F* hp_all, TH1F* hp_lost ) ;

   void draw_analysis_cdtt_plots2( const char* infile = "outputfiles/cdtt_input.root", const char* save_file_base = "plot-dtt-analysis2" ) {

      gDirectory -> Delete( "h*" ) ;

      gStyle -> SetOptStat(0) ;
      gStyle -> SetTitleW(0.90) ;
      gStyle -> SetTitleBorderSize(0) ;
      gStyle -> SetTitleFontSize( 0.055 ) ;

      loadHist( infile ) ;

      int wx(50), wy(50) ;





    //--- Canvas by Wpt, 3x3 Njet vs HT grid ------------------

      for ( int wpti=0; wpti<7; wpti++ ) {

         char cname[100] ;
         char ctitle[100] ;
         sprintf( cname, "draw_analysis_cdtt_plots_wpt%d", wpti ) ;
         sprintf( ctitle, "1/2(1-cos(dtt)) distributions, W pT %d", wpti ) ;
         TCanvas* can = new TCanvas( cname, ctitle, 900, 900 ) ;
         can -> Divide( 3, 3 ) ;

         can -> SetWindowPosition( wx, wy ) ; wx += 50 ; wy += 20 ;

         int ci(1) ;

         for ( int hti=1; hti<4; hti++ ) {
            for ( int nji=1; nji<4; nji++ ) {

               can -> cd( ci++ ) ;

               char hname[100] ;

               sprintf( hname, "h_cdtt_lostlep_nj%d_ht%d_wpt%d", nji, hti, wpti ) ;
               TH1F* hp_lost = (TH1F*) gDirectory -> FindObject( hname ) ;
               if ( hp_lost == 0x0 ) { printf("\n\n *** Missing hist %s\n\n", hname ) ; return ; }

               sprintf( hname, "h_cdtt_all_nj%d_ht%d_wpt%d", nji, hti, wpti ) ;
               TH1F* hp_all = (TH1F*) gDirectory -> FindObject( hname ) ;
               if ( hp_all == 0x0 ) { printf("\n\n *** Missing hist %s\n\n", hname ) ; return ; }


               do_plots( hp_all, hp_lost ) ;

            } // wpti
         } // hti


         char savefile[10000] ;
         sprintf( savefile, "outputfiles/%s-cdtt-wpt%02d.pdf", save_file_base, wpti ) ;
         can -> SaveAs( savefile ) ;

      } // nbi.





    //--- Canvas by Wpt, 3x3 Njet vs HT grid, lost only ------------------

      for ( int wpti=0; wpti<7; wpti++ ) {

         char cname[100] ;
         char ctitle[100] ;
         sprintf( cname, "draw_analysis_cdtt_plots_wpt%d_lost", wpti ) ;
         sprintf( ctitle, "1/2(1-cos(dtt)) lost lepton distributions, W pT %d", wpti ) ;
         TCanvas* can = new TCanvas( cname, ctitle, 900, 900 ) ;
         can -> Divide( 3, 3 ) ;

         can -> SetWindowPosition( wx, wy ) ; wx += 50 ; wy += 20 ;

         int ci(1) ;

         for ( int hti=1; hti<4; hti++ ) {
            for ( int nji=1; nji<4; nji++ ) {

               can -> cd( ci++ ) ;

               char hname[100] ;

               sprintf( hname, "h_cdtt_lostlep_nj%d_ht%d_wpt%d", nji, hti, wpti ) ;
               TH1F* hp_lost = (TH1F*) gDirectory -> FindObject( hname ) ;
               if ( hp_lost == 0x0 ) { printf("\n\n *** Missing hist %s\n\n", hname ) ; return ; }

               char newtitle[100] ;
               sprintf( newtitle, "Njet bin %d, HT bin %d", nji, hti ) ;
               hp_lost -> SetTitle( newtitle ) ;
               hp_lost -> SetTitleSize( 0.045, "x" ) ;
               hp_lost -> SetXTitle( "(1-cos #Delta #theta_{T})/2" ) ;
               hp_lost -> Draw() ;
               hp_lost -> Draw("axis same") ;

            } // wpti
         } // hti


         char savefile[10000] ;
         sprintf( savefile, "outputfiles/%s-cdtt-wpt%02d-lost.pdf", save_file_base, wpti ) ;
         can -> SaveAs( savefile ) ;

      } // wpti.







     //--- Canvas for HT bins --------------------------------------------------------------------

      { // HT bins canvas

         char cname[100] ;
         char ctitle[100] ;
         sprintf( cname, "can_draw_dtt_analysis1_ht_bins" ) ;
         sprintf( ctitle, "MET distributions, HT bins" ) ;
         TCanvas* can = new TCanvas( cname, ctitle, 400, 1100 ) ;
         can -> Divide( 1, 3 ) ;

         can -> SetWindowPosition( wx, wy ) ; wx += 50 ; wy += 20 ;

         int ci(1) ;

         for ( int hti=1; hti<4; hti++ ) {

            can -> cd( ci++ ) ;

            char hname[1000] ;
            char htitle[1000] ;

            sprintf( hname, "h_cdtt_all_nj1_ht1_wpt1" ) ;
            TH1F* hp_toclone = (TH1F*) gDirectory -> FindObject( hname ) ;
            if ( hp_toclone == 0x0 ) { printf("\n\n *** Missing hist %s\n\n", hname ) ; return ; }

            sprintf( hname, "h_cdtt_all_ht%d", hti ) ;
            sprintf( htitle, "1/2(1-cos(dtt)), all, HT%d", hti ) ;
            TH1F* hp_all_sum = (TH1F*) hp_toclone -> Clone( hname ) ;
            hp_all_sum -> Reset() ;
            hp_all_sum -> SetTitle( htitle ) ;

            sprintf( hname, "h_cdtt_lost_ht%d", hti ) ;
            sprintf( htitle, "1/2(1-cos(dtt)), lost lepton, HT%d", hti ) ;
            TH1F* hp_lost_sum = (TH1F*) hp_toclone -> Clone( hname ) ;
            hp_lost_sum -> Reset() ;
            hp_lost_sum -> SetTitle( htitle ) ;

            for ( int nji=1; nji<4; nji++ ) {

                  int wpti = 6 ;

                  sprintf( hname, "h_cdtt_lostlep_nj%d_ht%d_wpt%d", nji, hti, wpti ) ;
                  TH1F* hp_lost = (TH1F*) gDirectory -> FindObject( hname ) ;
                  if ( hp_lost == 0x0 ) { printf("\n\n *** Missing hist %s\n\n", hname ) ; return ; }

                  sprintf( hname, "h_cdtt_all_nj%d_ht%d_wpt%d", nji, hti, wpti ) ;
                  TH1F* hp_all = (TH1F*) gDirectory -> FindObject( hname ) ;
                  if ( hp_all == 0x0 ) { printf("\n\n *** Missing hist %s\n\n", hname ) ; return ; }

                  hp_all_sum -> Add( hp_all ) ;
                  hp_lost_sum   -> Add( hp_lost ) ;

            } // nji

            do_plots( hp_all_sum, hp_lost_sum ) ;

         } // hti

         char savefile[10000] ;
         sprintf( savefile, "outputfiles/%s-cdtt-ht.pdf", save_file_base ) ;
         can -> SaveAs( savefile ) ;

      } // HT bins canvas



     //--- Canvas for Njet bins --------------------------------------------------------------------

      { // Njet bins canvas

         char cname[100] ;
         char ctitle[100] ;
         sprintf( cname, "can_draw_dtt_analysis1_njet_bins" ) ;
         sprintf( ctitle, "MET distributions, Njet bins" ) ;
         TCanvas* can = new TCanvas( cname, ctitle, 400, 1100 ) ;
         can -> Divide( 1, 3 ) ;

         can -> SetWindowPosition( wx, wy ) ; wx += 50 ; wy += 20 ;

         int ci(1) ;

         for ( int nji=1; nji<4; nji++ ) {

            can -> cd( ci++ ) ;

            char hname[1000] ;
            char htitle[1000] ;

            sprintf( hname, "h_cdtt_all_nj1_ht1_wpt1" ) ;
            TH1F* hp_toclone = (TH1F*) gDirectory -> FindObject( hname ) ;
            if ( hp_toclone == 0x0 ) { printf("\n\n *** Missing hist %s\n\n", hname ) ; return ; }

            sprintf( hname, "h_cdtt_all_njet%d", nji ) ;
            sprintf( htitle, "1/2(1-cos(dtt)), all, Njet%d", nji ) ;
            TH1F* hp_all_sum = (TH1F*) hp_toclone -> Clone( hname ) ;
            hp_all_sum -> Reset() ;
            hp_all_sum -> SetTitle( htitle ) ;

            sprintf( hname, "h_cdtt_lost_njet%d", nji ) ;
            sprintf( htitle, "1/2(1-cos(dtt)), lost lepton, Njet%d", nji ) ;
            TH1F* hp_lost_sum = (TH1F*) hp_toclone -> Clone( hname ) ;
            hp_lost_sum -> Reset() ;
            hp_lost_sum -> SetTitle( htitle ) ;

            for ( int hti=1; hti<4; hti++ ) {

                  int wpti = 6 ;

                  sprintf( hname, "h_cdtt_lostlep_nj%d_ht%d_wpt%d", nji, hti, wpti ) ;
                  TH1F* hp_lost = (TH1F*) gDirectory -> FindObject( hname ) ;
                  if ( hp_lost == 0x0 ) { printf("\n\n *** Missing hist %s\n\n", hname ) ; return ; }

                  sprintf( hname, "h_cdtt_all_nj%d_ht%d_wpt%d", nji, hti, wpti ) ;
                  TH1F* hp_all = (TH1F*) gDirectory -> FindObject( hname ) ;
                  if ( hp_all == 0x0 ) { printf("\n\n *** Missing hist %s\n\n", hname ) ; return ; }

                  hp_all_sum -> Add( hp_all ) ;
                  hp_lost_sum   -> Add( hp_lost ) ;

            } // hti

            do_plots( hp_all_sum, hp_lost_sum ) ;

         } // nji

         char savefile[10000] ;
         sprintf( savefile, "outputfiles/%s-cdtt-njet.pdf", save_file_base ) ;
         can -> SaveAs( savefile ) ;

      } // Njet bins canvas







     //--- Canvas for all --------------------------------------------------------------------

      { // sum over all

         char cname[100] ;
         char ctitle[100] ;
         sprintf( cname, "can_draw_dtt_analysis1_all" ) ;
         sprintf( ctitle, "MET distributions, all" ) ;
         TCanvas* can = new TCanvas( cname, ctitle, 700, 600 ) ;

         can -> SetWindowPosition( wx, wy ) ; wx += 50 ; wy += 20 ;

         char hname[1000] ;
         char htitle[1000] ;

         sprintf( hname, "h_cdtt_all_nj1_ht1_wpt1" ) ;
         TH1F* hp_toclone = (TH1F*) gDirectory -> FindObject( hname ) ;
         if ( hp_toclone == 0x0 ) { printf("\n\n *** Missing hist %s\n\n", hname ) ; return ; }

         sprintf( hname, "h_cdtt_all" ) ;
         sprintf( htitle, "1/2(1-cos(dtt)), all" ) ;
         TH1F* hp_all_sum = (TH1F*) hp_toclone -> Clone( hname ) ;
         hp_all_sum -> Reset() ;
         hp_all_sum -> SetTitle( htitle ) ;

         sprintf( hname, "h_cdtt_lost" ) ;
         sprintf( htitle, "1/2(1-cos(dtt)), lost lepton" ) ;
         TH1F* hp_lost_sum = (TH1F*) hp_toclone -> Clone( hname ) ;
         hp_lost_sum -> Reset() ;
         hp_lost_sum -> SetTitle( htitle ) ;

         for ( int nji=1; nji<4; nji++ ) {
            for ( int hti=1; hti<4; hti++ ) {
                  int wpti = 6 ;

                  sprintf( hname, "h_cdtt_lostlep_nj%d_ht%d_wpt%d", nji, hti, wpti ) ;
                  TH1F* hp_lost = (TH1F*) gDirectory -> FindObject( hname ) ;
                  if ( hp_lost == 0x0 ) { printf("\n\n *** Missing hist %s\n\n", hname ) ; return ; }

                  sprintf( hname, "h_cdtt_all_nj%d_ht%d_wpt%d", nji, hti, wpti ) ;
                  TH1F* hp_all = (TH1F*) gDirectory -> FindObject( hname ) ;
                  if ( hp_all == 0x0 ) { printf("\n\n *** Missing hist %s\n\n", hname ) ; return ; }

                  hp_all_sum -> Add( hp_all ) ;
                  hp_lost_sum   -> Add( hp_lost ) ;

            } // hti
         } // nji

         do_plots( hp_all_sum, hp_lost_sum ) ;

         char savefile[10000] ;
         sprintf( savefile, "outputfiles/%s-cdtt-all.pdf", save_file_base ) ;
         can -> SaveAs( savefile ) ;

      } // sum over all











   } // draw_analysis_cdtt_plots2

  //=============================================================================================================

   void do_plots( TH1F* hp_all, TH1F* hp_lost ) {

      if ( hp_all==0x0 ) return ;
      if ( hp_lost==0x0 ) return ;

      hp_lost -> SetFillColor(11) ;

      hp_all -> Draw( "hist" ) ;
      hp_lost -> Draw( "hist same" ) ;
      hp_lost -> Draw( "axis same" ) ;

   } // do_plots

  //=============================================================================================================

