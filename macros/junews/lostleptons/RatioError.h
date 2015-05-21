#include <vector>
#include <iostream>
#include <iomanip>
#include "TH1.h"

#ifndef H_RATIOERROR
#define H_RATIOERROR


class RatioError{
public:
  explicit RatioError(TH1D* hPred);
  void FillMHTPDF(TH1D* DTTPDF, double ptW, double RLF, double eRLF, bool verbose=false);
  void SetPredictionsAndErrors(bool verbose=false);
  TH1D* GetPrediction() const;
  vector<double> GetRHL() const;
  vector<double> GetRHLError() const;

private:
  bool prepared_;
  TH1D* hPred_;
  int nMHTBins_;
  // TH1D* hIH_;
  vector<TH1D*> vhIHL_;
  vector<int> vNH_;
  // vector<double> vSH_;
  // double IL_;
  double SL_;
  vector<vector<double> > veRLF_;
  vector<double> vRHL_;
  vector<double> verrRHL_;
  // vector<double> binLowEdges_;
};

#endif
