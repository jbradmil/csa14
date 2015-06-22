// adapted from A.-R. Draeger
#ifndef H_BIN
#define H_BIN
#include <iostream>
#include <vector>
class Bin
{
public:
  Bin(){}
  Bin(double HTmin, double HTmax, double MHTmin, double MHTmax, int NJetsmin, int NJetsmax, int BTagsmin, int BTagsmax)
  {
    HTmin_=HTmin;
    HTmax_=HTmax;
    MHTmin_=MHTmin;
    MHTmax_=MHTmax;
    NJetsmin_=NJetsmin;
    NJetsmax_=NJetsmax;
    BTagsmin_=BTagsmin;
    BTagsmax_=BTagsmax;
  }
  double HTmin_, HTmax_, MHTmin_, MHTmax_;
  int NJetsmin_, NJetsmax_, BTagsmin_, BTagsmax_;
  ~Bin(){}
private:
};
#endif
