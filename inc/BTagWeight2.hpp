// -*- C++ -*-
#ifndef BTAGWEIGHT2_H
#define BTAGWEIGHT2_H
#include <vector>
// code from BTV POG
// https://twiki.cern.ch/twiki/pub/CMS/BTagWeight/BTagWeight2.cpp
// https://twiki.cern.ch/twiki/bin/viewauth/CMS/BTagWeight
class BTagWeight
{
 public:
  struct JetInfo {
  JetInfo(double mceff,double datasf) : eff(mceff), sf(datasf) {}
    double eff;
    double sf;
  };
 BTagWeight(int jmin, int jmax) :
  maxTags(jmax), minTags(jmin) {}
  bool filter(int t);
  double weight(std::vector<JetInfo> jets, int tags);
 private:
  int maxTags;
  int minTags;
};
#endif
