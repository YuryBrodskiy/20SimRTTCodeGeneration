/*
 * Port20Sim.h
 *
 *  Created on: Sep 2, 2011
 *      Author: yury
 */

#ifndef PORT20SIM_H_
#define PORT20SIM_H_
#include <boost/algorithm/string.hpp>
#include "vector"
#include "string"
#include "xxmatrix.h"
namespace %MODEL_NAME% {
//TODO redefine =operators
template<class T>
class Port20Sim {
private:
	T* _port;
	std::string _fullName;
	std::string _shortName;
	std::string _description;

	XXMatrix _link;
public:
	Port20Sim(std::string name, std::string desc, XXMatrix link) {
		_fullName = replaceIllegalCharacter(name);
		_shortName = makeShortName(name);
		_description = desc;
		_link = link;
		_port = NULL;
	}
//	Port20Sim(const Port20Sim &copy)
//	{
//		_name=copy._name;
//		_description=copy._description;
//		_link=copy._link;
//		_port=copy._port;
//	}
	virtual ~Port20Sim() {
	}

	std::string getFullName() {
		return _fullName;
	}
	std::string getShortName() {
		return _shortName;
	}
	std::string getDescription() {
		return _description;
	}


	std::vector<std::vector<double> >* getValue() {
		std::vector<std::vector<double> >* arrayCopy;
		arrayCopy=new std::vector<std::vector<double> >;
		XXMatrixToVector(&_link, arrayCopy);
		return arrayCopy;
	}

	void setPort(T &port) {
		_port = &port;
	}
	T& getPort() {
		return *_port;
	}
	void setValue(std::vector<std::vector<double> > &rsh) {

		XXVectorToMatrix(&rsh, &_link);

	}
	Port20Sim & operator =(const std::vector<std::vector<double> > &rsh) {
		std::vector<std::vector<double> >* temp;
		*temp = rsh;
		XXVectorToMatrix(temp, &_link);

		return *this;

	}

	/**
	 * @brief Removes illegal characters from 20sim generated names.
	 * This was necessary for interpretation based software, like the TaskBrowser, because they
	 * use things as '.' to access sub-properties.
	 */
	static std::string replaceIllegalCharacter(std::string str)
	{
		using namespace boost;
		replace_all(str, "\\", "_");
		replace_all(str, "[", "__");
		replace_all(str, "]", "__");
		replace_all(str, ".", "_");
		replace_all(str, ",", "_");
		return str;
	}
private:
	std::string makeShortName(std::string str)
	{
		using namespace boost;
		int pos;
		pos=str.find_last_of("\\");
		if (pos!=std::string::npos)
		{
			std::string temp;
			temp=str.substr(pos+1,temp.length()-pos-1);
			return temp;
		}
		else
		{
		return str;
		}
	}

};

} /* namespace Gain2 */
#endif /* PORT20SIM_H_ */
