#ifndef H_REDUCED_TREE_MAKER
#define H_REDUCED_TREE_MAKER

#include <string>
#include <vector>
#include "event_handler.hpp"

using std::vector;
typedef unsigned int uint;

class ReducedTreeMaker : public EventHandler{
public:
  ReducedTreeMaker(const std::string& in_file_name,
                   const bool is_list,
		   const double weight_in=1.0,
		   const int Nentries_in=-1,
		   const jec_type_t jec_type=CORR);

  void MakeReducedTree(const std::string& out_file_name, const bool is_skimmed);

private:
   int Nentries;
};

#endif
