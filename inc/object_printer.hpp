#ifndef H_OBJECT_PRINTER
#define H_OBJECT_PRINTER

#include <string>
#include <vector>
#include "event_handler.hpp"

using std::vector;

class ObjectPrinter : public EventHandler{
public:
  ObjectPrinter(const std::string& in_file_name, const bool is_list);
  float GetCSAElectronIsolation(const uint iel) const;
  void PrintObjects(const unsigned int nentries);
};

#endif
