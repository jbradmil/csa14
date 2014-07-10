#ifndef H_REDUCED_TREE_MAKER
#define H_REDUCED_TREE_MAKER

#include <string>
#include <vector>
#include "event_handler.hpp"

using std::vector;
typedef unsigned int uint;

typedef struct 
{
  int is_iso;
  float pt;
  float eta;
  float phi;
  int mother_id;
  float minDR;
  float minDPt;
  int mus_match;
  int signal_muon;
  int veto_muon;
  float reco_pt;
  float reco_eta;
  float reco_phi;
  float reco_dZ;
  float reco_d0PV;
  float reco_relIso;
  int reco_hasPFMatch;
} genMuInfo;

typedef struct 
{
  int is_iso;
  float pt;
  float eta;
  float phi;
  int mother_id;
  float minDR;
  float minDPt;
  int els_match;
  int signal_electron;
  int veto_electron;
  float reco_pt;
  float reco_eta;
  float reco_phi;
  float reco_dZ;
  float reco_d0PV;
  float reco_relIso;
  int reco_hasPFMatch;
} genElInfo;

typedef struct 
{
  float pt;
  float eta;
  float phi;
  float minDR;
  float minDPt;
  int taus_match;
  int loss_code;
} genTauInfo;



class ReducedTreeMaker : public EventHandler{
public:
  ReducedTreeMaker(const std::string& in_file_name,
                   const bool is_list,
                   const double weight_in=1.0);

  void MakeReducedTree(const std::string& out_file_name);
  // void SaveToVector(vector<float> *save, vector<float> *branch, vector<int> selection);
  void SetMuonValues(uint, genMuInfo&);
  void SetTauValues(uint, genTauInfo&);
  void SetElectronValues(uint, genElInfo&);

};

#endif
