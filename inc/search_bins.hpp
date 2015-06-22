// adapted from A.-R. Draeger
#ifndef H_SEARCHBINS
#define H_SEARCHBINS

#include "bin.hpp"
class SearchBins
{
public:
  SearchBins();
  unsigned int GetBinNumber(double HT, double MHT, int NJets, int BTags);
  void PrintUsed();
  
  
  ~SearchBins(){}
protected:
  std::vector<Bin> bins_;
  std::vector<int> usedBin_;
};
#endif
