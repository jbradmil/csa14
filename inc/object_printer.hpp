#ifndef H_OBJECT_PRINTER
#define H_OBJECT_PRINTER

#include <string>
#include <vector>
#include "cfa2014.hpp"

using std::vector;

class ObjectPrinter : public cfA{
public:
  ObjectPrinter(const std::string& in_file_name);
  float GetCSAElectronIsolation(const uint iel) const;
  void PrintObjects(const unsigned int nentries);
};

#endif
