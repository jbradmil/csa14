#ifndef H_WEIGHTCALCULATOR
#define H_WEIGHTCALCULATOR

#include <string>
#include <map>

// mount?

class WeightCalculator{
public:
  explicit WeightCalculator(const double=19399, const int=-1);

  void SetLuminosity(const double lumiIn=19399);
  double GetLuminosity() const;

  double GetCrossSection(const std::string&) const;
  double GetCrossSection(const std::string&, const int, const int) const;

  int GetTotalEvents(const std::string&) const;
  int GetTotalEvents(const std::string&, const int, const int) const;

  double GetWeight(const std::string&) const;
  double GetWeight(const std::string&, const int, const int) const;
  double GetWeightPerPb(const std::string&) const;

private:
  static std::map<std::string, double> crossSectionTable;
  static std::map<std::string, int> totalEventsTable;
  double lumi;
  int Nentries;

  void SetCrossSections();
  void SetTotalEvents();
};

#endif
