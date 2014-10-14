#ifndef H_CFA_SKIMMER
#define H_CFA_SKIMMER

#include <string>
#include "event_handler.hpp"

class CfASkimmer : public EventHandler{
public:
  CfASkimmer(const std::string& in_file_name,
             const bool is_list,
	     const int Nentries_in=-1);

  void Skim(const std::string& out_file_name);

private:
   int Nentries;
};

#endif
