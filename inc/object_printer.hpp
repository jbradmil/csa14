#ifndef H_OBJECT_PRINTER
#define H_OBJECT_PRINTER

#include <string>
#include <vector>
#include "cfa.hpp"

using std::vector;

class ObjectPrinter : public cfA{
public:
  ObjectPrinter(const std::string& in_file_name);
  void PrintObjects(const unsigned int nentries);
};

#endif
