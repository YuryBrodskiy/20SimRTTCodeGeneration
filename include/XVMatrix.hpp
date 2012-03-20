#pragma once

#include "xxmatrix.h"
#include <vector>
#include <string>

namespace common20sim
{

  enum CEType {UNKNOWN, INPUT, OUTPUT, PARAMETER, INTERNAL};

  typedef struct _XVMatrix
  {
      std::string name;
      std::string description;
      CEType type;

      XXMatrix storage;
      std::vector<double> values;
  } XVMatrix;


}
