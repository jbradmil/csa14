#ifndef JETCORRECTOR_H
#define JETCORRECTOR_H
#include <vector>
#include <string>
#include <assert.h>     /* assert */
#include <stdio.h>
#include "TString.h"
#include "JetCorrectorParameters.hpp"
#include "SimpleJetCorrector.hpp"
#include "FactorizedJetCorrector.hpp"

class FactorizedJetCorrector;
FactorizedJetCorrector *makeJetCorrector (std::vector<std::string> &vector_of_file_names);

#endif
