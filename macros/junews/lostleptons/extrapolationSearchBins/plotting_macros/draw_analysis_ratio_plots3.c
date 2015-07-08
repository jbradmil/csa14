
#include "TCanvas.h"
#include "TH1F.h"
#include "TDirectory.h"
#include "TStyle.h"
#include "TLine.h"
#include "TText.h"

#include "histio.C"

   double met_max(1000.) ;


   void do_plots( TH1F* hp ) ;

   void draw_analysis_ratio_plots3( const char* infile = "outputfiles/cdtt_input.root", const char* save_file_base = "plot-dtt-analysis3" ) {

      gDirectory -> Delete( "h*" ) ;

      gStyle -> SetOptStat(0) ;

      loadHist( infile ) ;

      int wx(50), wy(50) ;


      char cname[100] ;
      char ctitle[100] ;

      TCanvas* can ;
      int ci ;

      char savefile[10000] ;




    //--- 3x3 Njet vs HT grid

      sprintf( cname, "can_draw_analysis_ratio_plots" ) ;
      sprintf( ctitle, "lost/found ratio vs W pT" ) ;
      can = new TCanvas( cname, ctitle, 900, 900 ) ;
      can -> Divide( 3, 3 ) ;

      can -> SetWindowPosition( wx, wy ) ; wx += 50 ; wy += 20 ;

      ci = 1 ;

      for ( int hti=1; hti<4; hti++ ) {
         for ( int nji=1; nji<4; nji++ ) {

            can -> cd( ci++ ) ;

            char hname[100] ;

            sprintf( hname, "h_wpt_lostfound_ratio_nj%d_ht%d", nji, hti ) ;
            TH1F* hp = (TH1F*) gDirectory -> FindObject( hname ) ;
            if ( hp == 0x0 ) { printf("\n\n *** Missing hist %s\n\n", hname ) ; return ; }
            hp -> SetMarkerStyle(20) ;
            hp -> SetMarkerSize(0.5) ;

            do_plots( hp ) ;

         } // wpti
      } // hti


      sprintf( savefile, "outputfiles/%s-lostfound-ratio.pdf", save_file_base ) ;
      can -> SaveAs( savefile ) ;





    //--- 3 Njet plots

      sprintf( cname, "can_draw_analysis_ratio_plots_njet" ) ;
      sprintf( ctitle, "lost/found ratio vs W pT, Njet bins" ) ;
      can = new TCanvas( cname, ctitle, 1300, 600 ) ;
      can -> Divide( 3, 1 ) ;

      can -> SetWindowPosition( wx, wy ) ; wx += 50 ; wy += 20 ;

      ci = 1 ;

      for ( int nji=1; nji<4; nji++ ) {

         can -> cd( ci++ ) ;

         for ( int hti=1; hti<4; hti++ ) {

            char hname[100] ;

            sprintf( hname, "h_wpt_lostfound_ratio_nj%d_ht%d", nji, hti ) ;
            TH1F* hp = (TH1F*) gDirectory -> FindObject( hname ) ;
            if ( hp == 0x0 ) { printf("\n\n *** Missing hist %s\n\n", hname ) ; return ; }

            if ( hti==1 ) {
               do_plots( hp ) ;
               hp -> SetMarkerStyle(20) ;
               hp -> SetMarkerSize(0.5) ;
               hp -> Draw("same") ;
            } else {
               if ( hti==2) { hp -> SetLineColor(2) ; hp -> SetMarkerColor(2) ; hp -> SetMarkerStyle(21) ; hp -> SetMarkerSize(0.5) ; }
               if ( hti==3) { hp -> SetLineColor(4) ; hp -> SetMarkerColor(4) ; hp -> SetMarkerStyle(22) ; hp -> SetMarkerSize(0.5) ; }
               hp->Draw("same") ;
            }


         } // hti
      } // nji


      sprintf( savefile, "outputfiles/%s-lostfound-ratio-njet-bins.pdf", save_file_base ) ;
      can -> SaveAs( savefile ) ;




    //--- 3 HT plots

      sprintf( cname, "can_draw_analysis_ratio_plots_ht" ) ;
      sprintf( ctitle, "lost/found ratio vs W pT, HT bins" ) ;
      can = new TCanvas( cname, ctitle, 1300, 600 ) ;
      can -> Divide( 3, 1 ) ;

      can -> SetWindowPosition( wx, wy ) ; wx += 50 ; wy += 20 ;

      ci = 1 ;

      for ( int hti=1; hti<4; hti++ ) {

         can -> cd( ci++ ) ;

         for ( int nji=1; nji<4; nji++ ) {

            char hname[100] ;

            sprintf( hname, "h_wpt_lostfound_ratio_nj%d_ht%d", nji, hti ) ;
            TH1F* hp = (TH1F*) gDirectory -> FindObject( hname ) ;
            if ( hp == 0x0 ) { printf("\n\n *** Missing hist %s\n\n", hname ) ; return ; }

            if ( nji==1 ) {
               hp -> SetMarkerStyle(20) ;
               hp -> SetMarkerSize(0.5) ;
               hp -> SetMarkerColor(1) ;
               hp -> SetLineColor(1) ;
               do_plots( hp ) ;
               hp -> Draw("same") ;
            } else {
               if ( nji==2) { hp -> SetLineColor(2) ; hp -> SetMarkerColor(2) ; hp -> SetMarkerStyle(21) ; hp -> SetMarkerSize(0.5) ; }
               if ( nji==3) { hp -> SetLineColor(4) ; hp -> SetMarkerColor(4) ; hp -> SetMarkerStyle(22) ; hp -> SetMarkerSize(0.5) ; }
               hp->Draw("same") ;
            }


         } // nji
      } // hti


      sprintf( savefile, "outputfiles/%s-lostfound-ratio-ht-bins.pdf", save_file_base ) ;
      can -> SaveAs( savefile ) ;








   } // draw_analysis_ratio_plots3

  //=============================================================================================================

   void do_plots( TH1F* hp ) {

      if ( hp==0x0 ) return ;

      TLine* line = new TLine() ;
      line -> SetLineStyle(3) ;

     // hp_mc -> SetAxisRange( 0., met_max, "X" ) ;
      hp -> Draw( ) ;
      float hmax(4.0) ;
      hp -> SetMaximum( hmax ) ;
      hp -> SetMinimum( 0. ) ;
      line -> DrawLine( 200., 0., 200., hmax ) ;
      line -> DrawLine( 500., 0., 500., hmax ) ;
      line -> DrawLine( 750., 0., 750., hmax ) ;

   } // do_plots

  //=============================================================================================================












