#ifndef H_REDUCED_TREE_MAKER
#define H_REDUCED_TREE_MAKER

#include <string>
#include <vector>
#include "event_handler.hpp"
#include "gen_muon.hpp"

using std::vector;
typedef unsigned int uint;

typedef struct 
{
  float pt;
  float eta;
  float phi;
  float minDR;
  float minDPt;
  int mus_match;
  int loss_code;
} genMuInfo;


class ReducedTreeMaker : public EventHandler{
public:
  ReducedTreeMaker(const std::string& in_file_name,
                   const bool is_list,
                   const double weight_in=1.0);

  void MakeReducedTree(const std::string& out_file_name);
  // void SaveToVector(vector<float> *save, vector<float> *branch, vector<int> selection);
  void SetMuonValues(uint, genMuInfo&);

};

#endif
