#pragma once

#include <vector>
#include <string>
#include <stdexcept>

#include "XVMatrix.hpp"
#include "xxmodel.h"

namespace common20sim
{
  class XXModelProperties
  {
    public:
      XXModelProperties(Submodel20sim* model);

      /**
       * @brief Reads the XML property files.
       */
      void load(std::string file) throw(std::invalid_argument);

      std::vector<XVMatrix>& getPortsAndProperties();

    protected:
      CEType parseCEType(std::string type);
      std::vector<double> parseValues( std::string values_str, unsigned int rows, unsigned int columns);
      std::vector<double> parseRowValues(std::string row, unsigned int columns);
      double* parseContainer(std::string container);

    private:
      std::vector<XVMatrix> m_ports_and_properties;
      Submodel20sim* m_model;
      XXModelProperties();
  };
}
