double GetWpT(double lep_pt, double lep_phi, double mht, double mht_phi) {
  double lep_px = lep_pt * cos(lep_phi);
  double lep_py = lep_pt * sin(lep_phi);
  return sqrt( pow( (lep_px + mht*cos(mht_phi)), 2. ) + pow( (lep_py + mht*sin(mht_phi)), 2. ) ) ;
}

double GetCosDTT(double lep_pt, double lep_phi, double mht, double mht_phi) {

         double px_lab_w = lep_pt * cos( lep_phi ) + mht*cos(mht_phi) ;
         double py_lab_w = lep_pt * sin( lep_phi ) + mht*sin(mht_phi) ;
         double pt_lab_w = sqrt( pow( px_lab_w, 2 ) + pow( py_lab_w, 2 ) ) ;

         double dphi_lab_lepton_mht = lep_phi - mht_phi ;
         if ( dphi_lab_lepton_mht >  3.14159 ) dphi_lab_lepton_mht = dphi_lab_lepton_mht - 2*3.14159 ;
         if ( dphi_lab_lepton_mht < -3.14159 ) dphi_lab_lepton_mht = dphi_lab_lepton_mht + 2*3.14159 ;
         double cosdphi_lab_lepton_mht = cos( dphi_lab_lepton_mht ) ;

	 double mt_w = sqrt( 2*lep_pt*mht*(1.-cosdphi_lab_lepton_mht) ) ;
         double et_lab_w = sqrt( pow( pt_lab_w, 2. ) + pow( mt_w, 2. ) ) ;

	 
         double pt_parallel_lab_lepton = (lep_pt/pt_lab_w)*(lep_pt + mht*cosdphi_lab_lepton_mht ) ;

         return (2*pt_parallel_lab_lepton - pt_lab_w)/et_lab_w ;
  
}

double GetMTW(double lep_pt, double lep_phi, double mht, double mht_phi) {

         double px_lab_w = lep_pt * cos( lep_phi ) + mht*cos(mht_phi) ;
         double py_lab_w = lep_pt * sin( lep_phi ) + mht*sin(mht_phi) ;
         double pt_lab_w = sqrt( pow( px_lab_w, 2 ) + pow( py_lab_w, 2 ) ) ;

         double dphi_lab_lepton_mht = lep_phi - mht_phi ;
         if ( dphi_lab_lepton_mht >  3.14159 ) dphi_lab_lepton_mht = dphi_lab_lepton_mht - 2*3.14159 ;
         if ( dphi_lab_lepton_mht < -3.14159 ) dphi_lab_lepton_mht = dphi_lab_lepton_mht + 2*3.14159 ;
         double cosdphi_lab_lepton_mht = cos( dphi_lab_lepton_mht ) ;

	 return sqrt( 2*lep_pt*mht*(1.-cosdphi_lab_lepton_mht) ) ;
  }
