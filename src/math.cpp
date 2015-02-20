#include "math.hpp"

double Math::GetDeltaPhi(const double phi1, const double phi2){
  const double dp(phi1-phi2);
  return (dp>0.0)?fmod(dp+pi,2.0*pi)-pi:fmod(dp-pi,2.0*pi)+pi;
}

double Math::GetAbsDeltaPhi(const double phi1, const double phi2){
  return fmod(fabs(phi1-phi2)+pi,2.0*pi)-pi;
}

double Math::GetDeltaR(const double phi1, const double eta1, const double phi2, const double eta2){
  const double dPhi(GetAbsDeltaPhi(phi1,phi2));
  const double dEta(eta1-eta2);
  return std::sqrt(dPhi*dPhi+dEta*dEta);
}

long double Math::AddInQuadrature(long double x, long double y){
  if(fabs(y)>fabs(x)){
    const long double temp = y;
    y=x;
    x=temp;
  }
  if(x==0.) return y;
  const long double rat=y/x;
  return fabs(x)*sqrt(1.0L+rat*rat);
}
long double Math::DeltaPhi(long double phi1, long double phi2){
  long double dphi = fmod(fabs(phi2-phi1), 2.L*pi);
  return dphi>pi ? 2.L*pi-dphi : dphi;
}
float Math::dR(float eta1, float eta2, float phi1, float phi2) {
  return AddInQuadrature(eta1-eta2, DeltaPhi(phi1,phi2));
}
