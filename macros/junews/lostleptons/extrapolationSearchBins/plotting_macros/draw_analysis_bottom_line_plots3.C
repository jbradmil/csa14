
#include "TCanvas.h"
#include "TH1F.h"
#include "TDirectory.h"
#include "TStyle.h"
#include "TLine.h"
#include "TText.h"

#include "histio.C"

   void do_plots_ratio( TH1F* hp_mc, TH1F* hp_cdtt ) ;
   void do_plots_events( TH1F* hp_mc, TH1F* hp_cdtt, float common_max ) ;
   TH1F* make_ratio( TH1F* hp_n, TH1F* hp_d, const char* hname, const char* htitle ) ;

   void draw_analysis_bottom_line_plots3(
               const char* infile = "outputfiles/analysis-output3-default.root", const char* save_file_base = "plot-dtt-analysis3",
               float common_max_nsb = -1,
               float common_max_njet1 = -1, float common_max_njet2 = -1, float common_max_njet3 = -1
               ) {

      gDirectory -> Delete( "h*" ) ;

      gStyle -> SetOptStat(0) ;

      loadHist( infile ) ;

      int wx(50), wy(50) ;


      char hname[1000] ;
      char savefile[10000] ;

      TH1F* hp_mc ;
      TH1F* hp_cdtt ;

      TCanvas* can ;



     //-----------------------------------------------------------------

      gStyle -> SetPadBottomMargin(0.18) ;
      can = new TCanvas( "can_draw_analysis_bottom_line_plots_ratios", "MHT ratios", 1100, 900 ) ;
      can -> Divide( 3, 2 ) ;
      can -> SetWindowPosition( wx, wy ) ; wx += 50 ; wy += 50 ;



      sprintf( hname, "h_r31_ht2_mc" ) ;
      hp_mc = (TH1F*) gDirectory -> FindObject( hname ) ;
      if ( hp_mc == 0x0 ) { printf("\n\n *** Missing hist %s\n\n", hname ) ; return ; }

      sprintf( hname, "h_r31_ht2_cdtt" ) ;
      hp_cdtt = (TH1F*) gDirectory -> FindObject( hname ) ;
      if ( hp_cdtt == 0x0 ) { printf("\n\n *** Missing hist %s\n\n", hname ) ; return ; }

      can -> cd(2) ;
      do_plots_ratio( hp_mc, hp_cdtt ) ;





      sprintf( hname, "h_r31_ht3_mc" ) ;
      hp_mc = (TH1F*) gDirectory -> FindObject( hname ) ;
      if ( hp_mc == 0x0 ) { printf("\n\n *** Missing hist %s\n\n", hname ) ; return ; }

      sprintf( hname, "h_r31_ht3_cdtt" ) ;
      hp_cdtt = (TH1F*) gDirectory -> FindObject( hname ) ;
      if ( hp_cdtt == 0x0 ) { printf("\n\n *** Missing hist %s\n\n", hname ) ; return ; }

      can -> cd(3) ;
      do_plots_ratio( hp_mc, hp_cdtt ) ;




      sprintf( hname, "h_r21_ht1_mc" ) ;
      hp_mc = (TH1F*) gDirectory -> FindObject( hname ) ;
      if ( hp_mc == 0x0 ) { printf("\n\n *** Missing hist %s\n\n", hname ) ; return ; }

      sprintf( hname, "h_r21_ht1_cdtt" ) ;
      hp_cdtt = (TH1F*) gDirectory -> FindObject( hname ) ;
      if ( hp_cdtt == 0x0 ) { printf("\n\n *** Missing hist %s\n\n", hname ) ; return ; }

      can -> cd(4) ;
      do_plots_ratio( hp_mc, hp_cdtt ) ;




      sprintf( hname, "h_r21_ht2_mc" ) ;
      hp_mc = (TH1F*) gDirectory -> FindObject( hname ) ;
      if ( hp_mc == 0x0 ) { printf("\n\n *** Missing hist %s\n\n", hname ) ; return ; }

      sprintf( hname, "h_r21_ht2_cdtt" ) ;
      hp_cdtt = (TH1F*) gDirectory -> FindObject( hname ) ;
      if ( hp_cdtt == 0x0 ) { printf("\n\n *** Missing hist %s\n\n", hname ) ; return ; }

      can -> cd(5) ;
      do_plots_ratio( hp_mc, hp_cdtt ) ;





      sprintf( hname, "h_r21_ht3_mc" ) ;
      hp_mc = (TH1F*) gDirectory -> FindObject( hname ) ;
      if ( hp_mc == 0x0 ) { printf("\n\n *** Missing hist %s\n\n", hname ) ; return ; }

      sprintf( hname, "h_r21_ht3_cdtt" ) ;
      hp_cdtt = (TH1F*) gDirectory -> FindObject( hname ) ;
      if ( hp_cdtt == 0x0 ) { printf("\n\n *** Missing hist %s\n\n", hname ) ; return ; }


      can -> cd(6) ;
      do_plots_ratio( hp_mc, hp_cdtt ) ;


      sprintf( savefile, "outputfiles/%s-mhtratios.pdf", save_file_base ) ;
      can -> SaveAs( savefile ) ;


     //--------------------------------------------------------------

      gStyle -> SetPadBottomMargin(0.25) ;
      can = new TCanvas( "can_draw_analysis_bottom_line_plots_events", "Event counts", 1100, 900 ) ;
      can -> Divide( 4, 3 ) ;
      can -> SetWindowPosition( wx, wy ) ; wx += 50 ; wy += 50 ;


      int ci(1) ;

      for ( int nji=1; nji<4; nji++ ) {
         for ( int nbi=0; nbi<4; nbi++ ) {

            can -> cd( ci++ ) ;

            sprintf( hname, "h_lostlep_events_mc_nb%d_nj%d", nbi, nji ) ;
            hp_mc = (TH1F*) gDirectory -> FindObject( hname ) ;
            if ( hp_mc == 0x0 ) { printf("\n\n *** Missing hist %s\n\n", hname ) ; return ; }

            sprintf( hname, "h_lostlep_events_cdtt_nb%d_nj%d", nbi, nji ) ;
            hp_cdtt = (TH1F*) gDirectory -> FindObject( hname ) ;
            if ( hp_cdtt == 0x0 ) { printf("\n\n *** Missing hist %s\n\n", hname ) ; return ; }

            float common_max = common_max_njet1 ;
            if ( nji==2 ) common_max = common_max_njet2 ;
            if ( nji==3 ) common_max = common_max_njet3 ;
            do_plots_events( hp_mc, hp_cdtt, common_max ) ;

         } // nbi
      } // nji

      sprintf( savefile, "outputfiles/%s-events.pdf", save_file_base ) ;
      can -> SaveAs( savefile ) ;


     //--------------------------------------------------------------

      gStyle -> SetPadBottomMargin(0.25) ;
      can = new TCanvas( "can_draw_analysis_bottom_line_plots_control_events", "Control sample event counts", 1100, 900 ) ;
      can -> Divide( 4, 3 ) ;
      can -> SetWindowPosition( wx, wy ) ; wx += 50 ; wy += 50 ;


      ci = 1 ;

      for ( int nji=1; nji<4; nji++ ) {
         for ( int nbi=0; nbi<4; nbi++ ) {

            can -> cd( ci++ ) ;

            sprintf( hname, "h_nonzero_integral_count_nb%d_nj%d", nbi, nji ) ;
            TH1F* hp = (TH1F*) gDirectory -> FindObject( hname ) ;
            if ( hp == 0x0 ) { printf("\n\n *** Missing hist %s\n\n", hname ) ; return ; }

            sprintf( hname, "h_lostlep_events_mc_nb%d_nj%d", nbi, nji ) ;
            hp_mc = (TH1F*) gDirectory -> FindObject( hname ) ;
            if ( hp_mc == 0x0 ) { printf("\n\n *** Missing hist %s\n\n", hname ) ; return ; }

            hp -> SetMaximum( 1.4 * (hp ->GetMaximum() ) ) ;
            hp -> GetXaxis() -> SetLabelSize( 0.045 ) ;
            hp -> GetXaxis() -> SetLabelOffset( 0.02 ) ;
            hp -> SetMarkerSize(3.) ;
            hp -> Draw( "hist" ) ;
            hp -> Draw( "text same" ) ;
            hp_mc -> Draw( "same" ) ;

         } // nbi
      } // nji

      sprintf( savefile, "outputfiles/%s-control-sample-events.pdf", save_file_base ) ;
      can -> SaveAs( savefile ) ;


     //--------------------------------------------------------------

      gStyle -> SetPadBottomMargin(0.40) ;
      can = new TCanvas( "can_draw_analysis_bottom_line_plots_control_events_superbins", "Superbin control sample event counts", 800, 700 ) ;
      can -> SetWindowPosition( wx, wy ) ; wx += 50 ; wy += 50 ;


      {
            sprintf( hname, "h_nonzero_integral_count_superbins" ) ;
            TH1F* hp = (TH1F*) gDirectory -> FindObject( hname ) ;
            if ( hp == 0x0 ) { printf("\n\n *** Missing hist %s\n\n", hname ) ; return ; }

            sprintf( hname, "h_lostlep_events_mc_superbins" ) ;
            hp_mc = (TH1F*) gDirectory -> FindObject( hname ) ;
            if ( hp_mc == 0x0 ) { printf("\n\n *** Missing hist %s\n\n", hname ) ; return ; }

            hp -> SetMaximum( 1.4 * (hp ->GetMaximum() ) ) ;
            hp -> GetXaxis() -> SetLabelSize( 0.040 ) ;
            hp -> GetXaxis() -> SetLabelOffset( 0.02 ) ;
            hp -> SetMarkerSize(1.0) ;
            hp_mc -> SetMarkerSize(1.0) ;
            hp_mc -> SetMarkerColor(2) ;
            hp -> Draw( "hist" ) ;
            hp -> Draw( "text same" ) ;
            hp_mc -> Draw( "same" ) ;
            hp_mc -> Draw( "text same" ) ;

      }

      sprintf( savefile, "outputfiles/%s-control-sample-events-superbins.pdf", save_file_base ) ;
      can -> SaveAs( savefile ) ;



     //--------------------------------------------------------------

      gStyle -> SetPadBottomMargin(0.40) ;
      can = new TCanvas( "can_draw_analysis_bottom_line_plots_control_over_bg_events_superbins", "Superbin control / BG event counts", 800, 700 ) ;
      can -> SetWindowPosition( wx, wy ) ; wx += 50 ; wy += 50 ;


      {
            sprintf( hname, "h_nonzero_integral_count_superbins" ) ;
            TH1F* hp = (TH1F*) gDirectory -> FindObject( hname ) ;
            if ( hp == 0x0 ) { printf("\n\n *** Missing hist %s\n\n", hname ) ; return ; }

            sprintf( hname, "h_lostlep_events_mc_superbins" ) ;
            hp_mc = (TH1F*) gDirectory -> FindObject( hname ) ;
            if ( hp_mc == 0x0 ) { printf("\n\n *** Missing hist %s\n\n", hname ) ; return ; }

            char htitle[1000] ;
            sprintf( hname, "h_control_over_bg_ratio_superbins" ) ;
            sprintf( htitle, "control / BG ratio, superbins" ) ;
            TH1F* hp_ratio = make_ratio( hp, hp_mc, hname, htitle ) ;

            hp_ratio -> SetMinimum( 0.0 ) ;
            hp_ratio -> GetXaxis() -> SetLabelSize( 0.040 ) ;
            hp_ratio -> GetXaxis() -> SetLabelOffset( 0.02 ) ;
            hp_ratio -> SetMarkerStyle(20) ;
            hp_ratio -> SetMarkerSize(1.0) ;
            hp_ratio -> SetMarkerColor(4) ;
            hp_ratio -> SetLineColor(4) ;
            hp_ratio -> SetLineWidth(2) ;
            hp_ratio -> Draw( ) ;
            gPad -> SetGridy(1) ;
      }

      sprintf( savefile, "outputfiles/%s-control-sample-over-bg-events-superbins.pdf", save_file_base ) ;
      can -> SaveAs( savefile ) ;



     //--------------------------------------------------------------

      gStyle -> SetPadBottomMargin(0.25) ;
      can = new TCanvas( "can_draw_analysis_bottom_line_plots_bg_over_control_events", "BG / Control sample event ratio", 1100, 900 ) ;
      can -> Divide( 4, 3 ) ;
      can -> SetWindowPosition( wx, wy ) ; wx += 50 ; wy += 50 ;


      ci = 1 ;

      for ( int nji=1; nji<4; nji++ ) {
         for ( int nbi=0; nbi<4; nbi++ ) {

            can -> cd( ci++ ) ;

            sprintf( hname, "h_nonzero_integral_count_nb%d_nj%d", nbi, nji ) ;
            TH1F* hp = (TH1F*) gDirectory -> FindObject( hname ) ;
            if ( hp == 0x0 ) { printf("\n\n *** Missing hist %s\n\n", hname ) ; return ; }

            sprintf( hname, "h_lostlep_events_mc_nb%d_nj%d", nbi, nji ) ;
            hp_mc = (TH1F*) gDirectory -> FindObject( hname ) ;
            if ( hp_mc == 0x0 ) { printf("\n\n *** Missing hist %s\n\n", hname ) ; return ; }

            char htitle[1000] ;
            sprintf( hname, "h_bg_over_control_nb%d_nj%d", nbi, nji ) ;
            sprintf( htitle, "BG / control ratio, Nb%d, Njet%d", nbi, nji ) ;
            TH1F* hp_ratio = make_ratio( hp_mc, hp, hname, htitle ) ;

            hp_ratio -> SetMaximum( 1.4 ) ;
            hp_ratio -> SetMinimum( 0.0 ) ;
            hp_ratio -> GetXaxis() -> SetLabelSize( 0.065 ) ;
            hp_ratio -> GetXaxis() -> SetLabelOffset( 0.02 ) ;
            hp_ratio -> SetMarkerStyle(20) ;
            hp_ratio -> SetMarkerSize(0.7) ;
            hp_ratio -> SetMarkerColor(4) ;
            hp_ratio -> Draw( ) ;
            gPad -> SetGridy(1) ;

         } // nbi
      } // nji

      sprintf( savefile, "outputfiles/%s-bg-over-control-ratio.pdf", save_file_base ) ;
      can -> SaveAs( savefile ) ;



     //--------------------------------------------------------------

      gStyle -> SetPadBottomMargin(0.35) ;
      can = new TCanvas( "can_draw_analysis_bottom_line_plots_superbins_events", "Superbins, BG events", 700, 700 ) ;
      can -> SetWindowPosition( wx, wy ) ; wx += 50 ; wy += 50 ;
      can -> cd() ;

      hp_mc = (TH1F*) gDirectory -> FindObject( "h_lostlep_events_mc_superbins" ) ;
      if ( hp_mc == 0x0 ) { printf("\n\n *** Missing hist h_lostlep_events_mc_superbins\n\n" ) ; return ; }
      hp_cdtt = (TH1F*) gDirectory -> FindObject( "h_lostlep_events_cdtt_superbins" ) ;
      if ( hp_cdtt == 0x0 ) { printf("\n\n *** Missing hist h_lostlep_events_cdtt_superbins\n\n" ) ; return ; }
      hp_cdtt -> GetXaxis() -> SetLabelOffset( 0.02 ) ;
      if ( common_max_nsb > 0 ) hp_cdtt -> SetMaximum( common_max_nsb ) ;
      hp_cdtt -> Draw() ;
      hp_mc -> Draw("samehiste") ;

      sprintf( savefile, "outputfiles/%s-superbins-bg-events.pdf", save_file_base ) ;
      can -> SaveAs( savefile ) ;




     //--------------------------------------------------------------

      gStyle -> SetPadBottomMargin(0.35) ;
      can = new TCanvas( "can_draw_analysis_bottom_line_plots_superbins_err_ratio2", "Superbins, BG error ratio2", 700, 700 ) ;
      can -> SetWindowPosition( wx, wy ) ; wx += 50 ; wy += 50 ;
      can -> cd() ;

      TH1F* h_err_ratio2 = (TH1F*) hp_cdtt -> Clone( "h_err_ratio2" ) ;
      h_err_ratio2 -> Reset() ;
      h_err_ratio2 -> SetTitle( "Ratio of N/cdtterr2, superbins" ) ;
      for ( int bi=1; bi<h_err_ratio2->GetNbinsX(); bi++ ) {
         double ratio2_val = 0. ;
         double n_val = hp_mc -> GetBinContent( bi ) ;
         double cdtt_err = hp_cdtt -> GetBinError( bi ) ;
         if ( cdtt_err > 0 ) {
            ratio2_val = n_val / (cdtt_err*cdtt_err) ;
         }
         h_err_ratio2 -> SetBinContent( bi, ratio2_val ) ;
      } // bi

      h_err_ratio2 -> SetMaximum(6) ;
      h_err_ratio2 -> Draw("hist") ;
      gPad->SetGridy(1) ;

      sprintf( savefile, "outputfiles/%s-superbins-bg-err-ratio2.pdf", save_file_base ) ;
      can -> SaveAs( savefile ) ;


     //--------------------------------------------------------------

      gStyle -> SetPadBottomMargin(0.35) ;
      can = new TCanvas( "can_draw_analysis_bottom_line_plots_superbins_mhtratios1", "Superbins, MHT ratios 1", 700, 700 ) ;
      can -> SetWindowPosition( wx, wy ) ; wx += 50 ; wy += 50 ;
      can -> cd() ;

      hp_mc = (TH1F*) gDirectory -> FindObject( "h_mhtratio_mc_superbins" ) ;
      if ( hp_mc == 0x0 ) { printf("\n\n *** Missing hist h_mhtratio_mc_superbins\n\n" ) ; return ; }
      hp_cdtt = (TH1F*) gDirectory -> FindObject( "h_mhtratio_cdtt_superbins" ) ;
      if ( hp_cdtt == 0x0 ) { printf("\n\n *** Missing hist h_mhtratio_cdtt_superbins\n\n" ) ; return ; }
      hp_cdtt -> GetXaxis() -> SetLabelOffset( 0.03 ) ;
      //hp_cdtt -> SetMaximum( 0.13 ) ;
      hp_cdtt -> SetMaximum( 0.50 ) ;
      hp_cdtt -> SetLineColor(2) ;
      hp_cdtt -> SetMarkerColor(2) ;
      hp_mc -> SetLineColor(4) ;
      hp_mc -> SetMarkerColor(4) ;
      hp_cdtt -> Draw() ;
      hp_mc -> Draw("samehiste") ;
      gPad->SetGridy(1) ;

      sprintf( savefile, "outputfiles/%s-superbins-mhtratios1.pdf", save_file_base ) ;
      can -> SaveAs( savefile ) ;


     //--------------------------------------------------------------

      gStyle -> SetPadBottomMargin(0.35) ;
      can = new TCanvas( "can_draw_analysis_bottom_line_plots_superbins_mhtratios2", "Superbins, MHT ratios 2", 700, 700 ) ;
      can -> SetWindowPosition( wx, wy ) ; wx += 50 ; wy += 50 ;
      can -> cd() ;

      //hp_cdtt -> SetMaximum( 0.016 ) ;
      hp_cdtt -> SetMaximum( 0.17 ) ;
      hp_cdtt -> Draw() ;
      hp_mc -> Draw("samehiste") ;
      gPad->SetGridy(1) ;

      sprintf( savefile, "outputfiles/%s-superbins-mhtratios2.pdf", save_file_base ) ;
      can -> SaveAs( savefile ) ;


     //--------------------------------------------------------------

      gStyle -> SetPadBottomMargin(0.35) ;
      can = new TCanvas( "can_draw_analysis_bottom_line_plots_asuperbins_events", "Analysis superbins, BG events", 700, 700 ) ;
      can -> SetWindowPosition( wx, wy ) ; wx += 50 ; wy += 50 ;
      can -> cd() ;

      hp_mc = (TH1F*) gDirectory -> FindObject( "h_lostlep_events_mc_asuperbins" ) ;
      if ( hp_mc == 0x0 ) { printf("\n\n *** Missing hist h_lostlep_events_mc_asuperbins\n\n" ) ; return ; }
      hp_cdtt = (TH1F*) gDirectory -> FindObject( "h_lostlep_events_cdtt_asuperbins" ) ;
      if ( hp_cdtt == 0x0 ) { printf("\n\n *** Missing hist h_lostlep_events_cdtt_asuperbins\n\n" ) ; return ; }
      hp_cdtt -> GetXaxis() -> SetLabelOffset( 0.02 ) ;
      if ( common_max_nsb > 0 ) hp_cdtt -> SetMaximum( common_max_nsb ) ;

      hp_cdtt -> GetXaxis() -> SetLabelOffset( 0.025 ) ;
      hp_cdtt -> SetLineColor(2) ;
      hp_cdtt -> SetMarkerColor(2) ;
      hp_cdtt -> SetMarkerStyle(20) ;
      hp_cdtt -> SetMarkerSize(0.6) ;
      hp_mc -> SetLineColor(4) ;
      hp_mc -> SetMarkerColor(4) ;
      hp_mc -> SetMarkerStyle(21) ;
      hp_mc -> SetMarkerSize(0.6) ;

      hp_cdtt -> Draw() ;
      hp_mc -> Draw("samehiste") ;
      hp_cdtt -> Draw("same") ;

      sprintf( savefile, "outputfiles/%s-asuperbins-bg-events.pdf", save_file_base ) ;
      can -> SaveAs( savefile ) ;





   } // draw_analysis_bottom_line_plots3

  //=============================================================================================================

   void do_plots_ratio( TH1F* hp_mc, TH1F* hp_cdtt ) {

      if ( hp_mc == 0x0 ) return ;
      if ( hp_cdtt == 0x0 ) return ;

      hp_mc -> SetMaximum( 1.5*(hp_mc->GetMaximum()) ) ;

      hp_mc -> SetMarkerSize( 0.6 ) ;
      hp_cdtt -> SetMarkerSize( 0.6 ) ;

      hp_mc -> GetXaxis() -> SetLabelSize( 0.055 ) ;
      hp_mc -> GetXaxis() -> SetLabelOffset( 0.01 ) ;

      hp_mc -> SetLineColor(4) ;
      hp_mc -> SetMarkerColor(4) ;
      hp_cdtt -> SetLineColor(2) ;
      hp_cdtt -> SetMarkerColor(2) ;

      hp_mc -> Draw() ;
      hp_cdtt -> Draw("same") ;

      gPad -> SetGridy(1) ;


   }

  //=============================================================================================================


   void do_plots_events( TH1F* hp_mc, TH1F* hp_cdtt, float common_max ) {

      if ( hp_mc == 0x0 ) return ;
      if ( hp_cdtt == 0x0 ) return ;

      if ( common_max < 0 ) {
         hp_mc -> SetMaximum( 2.0*(hp_mc->GetMaximum()) ) ;
      } else {
         hp_mc -> SetMaximum( common_max ) ;
      }

      //hp_mc -> SetMarkerSize( 0.6 ) ;
      hp_cdtt -> SetMarkerStyle(20) ;
      hp_cdtt -> SetMarkerColor(4) ;
      hp_cdtt -> SetMarkerSize( 0.6 ) ;

      hp_mc -> GetXaxis() -> SetLabelSize( 0.065 ) ;
      hp_mc -> GetXaxis() -> SetLabelOffset( 0.02 ) ;

      hp_mc -> Draw() ;
      hp_cdtt -> Draw("same") ;
      hp_mc -> Draw( "same e" ) ;
      hp_cdtt -> Draw("same") ;

      //gPad -> SetGridy(1) ;


   }

  //=============================================================================================================

   TH1F* make_ratio( TH1F* hp_n, TH1F* hp_d, const char* hname, const char* htitle ) {

      if ( hp_n == 0x0 ) return 0x0 ;
      if ( hp_d == 0x0 ) return 0x0 ;

      TH1F* hp_return = (TH1F*) hp_d -> Clone( hname ) ;
      hp_return -> Reset() ;
      hp_return -> SetTitle( htitle ) ;

      for ( int bi=1; bi<=hp_d -> GetNbinsX() ; bi++ ) {
         double ratio_val(0.) ;
         double ratio_err(0.) ;
         double numer = hp_n -> GetBinContent( bi ) ;
         double denom = hp_d -> GetBinContent( bi ) ;
         if ( denom > 0 ) {
            ratio_val = numer / denom ;
            if ( numer > 0 ) {
               ratio_err = ratio_val * sqrt( 1./numer + 1./denom ) ;
            }
         }
         hp_return -> SetBinContent( bi, ratio_val ) ;
         hp_return -> SetBinError( bi, ratio_err ) ;
      }

      return hp_return ;

   } // make_ratio

  //=============================================================================================================






