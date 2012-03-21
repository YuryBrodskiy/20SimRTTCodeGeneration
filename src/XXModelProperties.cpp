#include "XXModelProperties.hpp"

#include <cstdlib>
#include <rtt/RTT.hpp>
#include <boost/algorithm/string.hpp>
#include <sstream>

/* parameter parsing include */
#include "tinyxml.h"

namespace common20sim
{
  using namespace RTT;

  XXModelProperties::XXModelProperties(Submodel20sim* model) : m_model(model)
  { }

  //@todo Add exceptions for Error's
  void XXModelProperties::load(std::string file) throw(std::invalid_argument)
  {
    TiXmlDocument doc(file);
    if (!doc.LoadFile())
    {
      throw std::invalid_argument("File not found: " + file);
    }

    TiXmlHandle hdoc(&doc);
    TiXmlElement* pElem;
    TiXmlHandle hRoot(0);
    TiXmlNode* tNode(NULL);

    hRoot = TiXmlHandle(hdoc.FirstChildElement().Element());
    pElem = hRoot.FirstChild("modelVariables").FirstChild().Element();

    log(Debug) << "Begin of processing all modelVariables" << endlog();
    try
    {
      for (;pElem != NULL; pElem = pElem->NextSiblingElement())
      {
        XVMatrix m;

        log(Debug) << "Read all relevant fields" << endlog();
        const char * kind = (tNode = pElem->FirstChild("kind")) == NULL ?
            NULL : tNode->ToElement()->GetText();
        m.type = parseCEType(kind);

        const char * name = (tNode = pElem->FirstChild("name")) == NULL ?
            NULL : tNode->ToElement()->GetText();

        if(m.type != INTERNAL && (name == NULL || !std::string(name).compare(""))) 
        {
          throw std::invalid_argument("modelVariable name was NULL or empty.");
        }
        m.name = (name == NULL) ? std::string("") : name; // names may be empty

        const char * description = (tNode = pElem->FirstChild("description")) == NULL ?
            " " : tNode->ToElement()->GetText();
        description = (description == NULL || !std::string(description).compare("")) ? " " : description;
        m.description = description;

        const char * container = (tNode = pElem->FirstChild("storage")) == NULL ?
            NULL : tNode->FirstChild("name")->ToElement()->GetText();
        double * const cont = parseContainer(container);

        const char * strIndex = (tNode = pElem->FirstChild("storage")) == NULL ?
            NULL : tNode->FirstChild("index")->ToElement()->GetText();

        const char * strRows = (tNode = pElem->FirstChild("size")) == NULL ?
            NULL : tNode->FirstChild("rows")->ToElement()->GetText();

        const char * strColumns = (tNode = pElem->FirstChild("size")) == NULL ?
            NULL : tNode->FirstChild("columns")->ToElement()->GetText();

        // Determine storage
        int index, rows, columns;
        if (strIndex == NULL)
        {
          throw std::invalid_argument("XXMatrix storage location invalid.");
        }
        index = atoi(strIndex);
        rows = strRows == NULL ? 1 : atoi(strRows);
        columns = strColumns == NULL ? 1 : atoi(strColumns);

        if(index == -1 || rows == -1 || columns == -1)
        {
          throw std::invalid_argument("XXMatrix storage location invalid(2).");
        }
        m.storage.columns = columns;
        m.storage.rows = rows;
        m.storage.mat = cont + index;

        const char * type = (tNode = pElem->FirstChild("type")) == NULL ?
            NULL : tNode->ToElement()->GetText();

        const char * values = (tNode = pElem->FirstChild("value")) == NULL ?
            NULL : tNode->ToElement()->GetText();
        m.values = parseValues(values, rows, columns);

        m_ports_and_properties.push_back(m);

        log(Debug) << "Name: " << m.name << " Disc: " << m.description <<
            " CEType: " << m.type << " Storage: " << m.storage.mat <<
            " Kind: " << kind << " Type: " << type <<
            " Index: " << index << endlog();
      }
    }
    catch(std::invalid_argument& e)
    {
      std::stringstream s;
      s << "Parse error: " << e.what() << "\nTrace: " << *pElem;
      throw std::invalid_argument(s.str());
    }

    log(Debug) << "End of processing the xml" << endlog();

  }

  std::vector<double> XXModelProperties::parseValues( std::string values_str, unsigned int rows, unsigned int columns)
  {
    using namespace std;
    vector<double> values;

    size_t row_position=0;
    unsigned int crows = 0;

    do
    {
      size_t found = values_str.find_first_of(";", row_position);

      if(found == row_position)
      {
        throw invalid_argument("empty row");
      }

      string row;
      if(found == string::npos)
      {
        if(row_position == values_str.length())
        {
          break;
        }
        row = values_str.substr(row_position);
        row_position = string::npos;
      }
      else
      {
        row = values_str.substr(row_position, found - row_position);
        row_position = found + 1;
      }
      ++crows;

      std::vector<double> tmp(parseRowValues(row, columns));
      values.insert (values.end(),tmp.begin(),tmp.end());
    } while(row_position != string::npos);

    if(crows != rows)
    {
      stringstream s("");
      s << "number of rows(" << crows << ") does not match required amount(" << rows << ")";
      throw invalid_argument(s.str());
    }

    if(values.size() != (rows*columns))
    {
      stringstream s("");
      s << "number of elements(" << values.size() << ") does not match required amount(" << rows*columns << ")";
    	throw invalid_argument(s.str());
    }

    return values;
  }

  std::vector<double> XXModelProperties::parseRowValues(std::string row, unsigned int columns)
  {
	    using namespace std;
	  	vector<double> values;

	  	size_t column_position=0;
	  	size_t found = 0;
	  	unsigned int ccolumns= 0;

		do
		{
			found = row.find_first_of(",", column_position);

			if(found == column_position)
			{
				throw invalid_argument("empty field");
			}

			std::string field;
			if(found == string::npos)
			{
				if(column_position == row.length())
				{
					break;
				}
				field = row.substr(column_position);
				column_position = string::npos;
			}
			else
			{
				field = row.substr(column_position, found - column_position);
				column_position = found + 1;
			}
			++ccolumns;

			double v = strtod(field.c_str(), NULL);
			if(v == HUGE_VAL || v == -HUGE_VAL)
			{
				throw invalid_argument("conversion to double failed");
			}

			values.push_back(v);
		} while(column_position != string::npos);

		if(ccolumns != columns)
		{
		  stringstream s("");
		  s << "number of columns(" << ccolumns << ") does not match required amount(" << columns << ")";
			throw invalid_argument(s.str());
		}

		return values;
  }

  CEType XXModelProperties::parseCEType(std::string type)
  {
    if (boost::iequals(type, "parameter"))
    {
      return PARAMETER;
    }
    else if (boost::iequals(type, "state"))
    {
      return INTERNAL;
    }
    else if (boost::iequals(type, "variable"))
    {
      return INTERNAL;
    }
    else if (boost::iequals(type, "input"))
    {
      return INPUT;
    }
    else if (boost::iequals(type, "output"))
    {
      return OUTPUT;
    }
    else
    {
      log(Error) << "Kind(" << type << ") not recognized." << endlog();
      return INTERNAL;
    }
  }

  double* XXModelProperties::parseContainer(std::string container)
  {
    log(Debug) << " Selecting source of data for this node" << endlog();
    //@todo matrix + index
    if (boost::iequals(container,"%XX_VARIABLE_ARRAY_NAME%"))
    {
      return m_model->%VARPREFIX%%XX_VARIABLE_ARRAY_NAME%;
    }
    else if (boost::iequals(container, "%XX_CONSTANT_ARRAY_NAME%"))
    {
      return m_model->%VARPREFIX%%XX_CONSTANT_ARRAY_NAME%;
    }
    else if (boost::iequals(container, "%XX_PARAMETER_ARRAY_NAME%"))
    {
      return m_model->%VARPREFIX%%XX_PARAMETER_ARRAY_NAME%;
    }
    else if (boost::iequals(container, "%XX_INITIAL_VALUE_ARRAY_NAME%"))
    {
      return m_model->%VARPREFIX%%XX_INITIAL_VALUE_ARRAY_NAME%;
    }
    else if (boost::iequals(container, "%XX_STATE_ARRAY_NAME%"))
    {
      return m_model->%VARPREFIX%%XX_STATE_ARRAY_NAME%;
    }
    else if (boost::iequals(container, "%XX_RATE_ARRAY_NAME%"))
    {
      return m_model->%VARPREFIX%%XX_RATE_ARRAY_NAME%;
    }
    else if (boost::iequals(container, "%XX_MATRIX_ARRAY_NAME%"))
    {
      //Check matrix[index]
//      return m_model->%VARPREFIX%%XX_MATRIX_ARRAY_NAME%[index]->mat;
      throw std::invalid_argument("Unexpected container name: %XX_MATRIX_ARRAY_NAME%");
    }
    else if (boost::iequals(container, "%XX_UNNAMED_ARRAY_NAME%"))
    {
      return m_model->%VARPREFIX%%XX_UNNAMED_ARRAY_NAME%;
    }
    else if (boost::iequals(container, "workarray"))
    {
      return m_model->%VARPREFIX%workarray;
    }
    else
    {
      throw std::invalid_argument("Unexpected container name.");
    }
  }

  std::vector<XVMatrix>& XXModelProperties::getPortsAndProperties()
  {
    return m_ports_and_properties;
  }
}
