#pragma once

#include <vector>
#include <string>

#include "configuration/XVMatrix.hpp"

#include <std_msgs/Float64MultiArray.h>
#include <rtt/RTT.hpp>

namespace common20sim {

	using namespace RTT;

	typedef std_msgs::Float64MultiArray flat_matrix_t;
	typedef std_msgs::Float64MultiArray::_data_type flat_matrix_internal_t;

	/**
	 * @brief Removes illegal characters from 20sim generated names.
	 * This was necessary for interpretation based software, like the TaskBrowser, because they
	 * use things as '.' to access sub-properties.
	 */
	std::string replaceIllegalCharacter(std::string str);
	std::string replaceIllegalCharacter(std::string str, std::string pattern, std::string replacement);
	std::string makeShortName(std::string str);

	template<class T>
	class Adapter20Sim {

	public:
		Adapter20Sim(XVMatrix& mat, T* port) :
			m_port(port), m_matrix(mat), m_xx_data(mat.storage.mat)
		{
			m_full_name = replaceIllegalCharacter(m_matrix.name);
			m_short_name = makeShortName(m_matrix.name);

			// setup/resize m_data
			if(m_matrix.storage.columns != 0 && m_matrix.storage.rows != 0)
			{
			  m_size = m_matrix.storage.columns * m_matrix.storage.rows;
				m_port_data.data.resize(m_size, 0);
			}
			else
			{
				log(Error) << "XXMatrix settings unknown." << endlog();
				throw std::out_of_range("XXMatrix size unknown.");
			}

			assert(m_xx_data != NULL);
			assert(m_size != 0);
		}

		Adapter20Sim(const Adapter20Sim& copy) :
		  m_port(copy.m_port), m_matrix(copy.m_matrix), m_xx_data(copy.m_xx_data)
		{
			m_full_name = copy.m_full_name;
			m_short_name = copy.m_short_name;
			m_port_data = copy.m_port_data;
			m_size = copy.m_size;
		}

    Adapter20Sim& operator=(const Adapter20Sim& ass)
		{
      m_port = ass.m_port;
      m_matrix = ass.m_matrix;
      m_xx_data = ass.m_xx_data;
      m_full_name = ass.m_full_name;
			m_short_name = ass.m_short_name;
			m_port_data = ass.m_port_data;
			m_size = ass.m_size;
			return *this;
		}

		virtual ~Adapter20Sim()
		{
         assert(m_xx_data == m_matrix.storage.mat);
		}

		std::string getFullName()
		{
		  return m_full_name;
		}

		std::string getShortName()
		{
			return m_short_name;
		}

		std::string getDescription()
		{
			return m_matrix.description;
		}

		flat_matrix_t& getPortData()
		{
		  return m_port_data;
		}

		double* getXXData()
		{
		  return m_xx_data;
		}

		std::size_t getSize()
		{
		  return m_size;
		}

		T* getPort()
		{
			return m_port;
		}

		void copyPortToVariable()
		{
		  if(m_port_data.data.size() == m_size)
		    memcpy(m_xx_data, m_port_data.data.data(), m_size * sizeof(double));
		  else
				throw std::out_of_range("Input data size does not match variable data size.");
		}

		void copyVariableToPort()
		{
		  if(m_port_data.data.size() == m_size)
      	memcpy(m_port_data.data.data(), m_xx_data, m_size * sizeof(double));
			else
				throw std::out_of_range("Variable data size does not match port data size.");
		}

	private:
    T* m_port;
    std::string m_full_name;
    std::string m_short_name;

    XVMatrix& m_matrix;

    flat_matrix_t m_port_data; // from/to ports
    XXDouble* m_xx_data; // 20sim internal matrix
    std::size_t m_size;
	};
}
