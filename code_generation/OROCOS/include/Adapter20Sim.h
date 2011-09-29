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
#include "XVMatrix.h"
namespace common20sim {
//TODO redefine =operators
template<class T>
class Adapter20Sim {
	typedef std::vector<double> flat_matrix;
private:
	T* _port;
	std::string _fullName;
	std::string _shortName;
	std::string _description;
	flat_matrix data;
	XVMatrix* _link;
public:
	Adapter20Sim(std::string name, std::string desc, XVMatrix &link) {
		_fullName = replaceIllegalCharacter(name);
		_shortName = makeShortName(name);
		_description = desc;
		_link = &link;
		_port = NULL;
	}
//	Port20Sim(const Port20Sim &copy)
//	{
//		_name=copy._name;
//		_description=copy._description;
//		_link=copy._link;
//		_port=copy._port;
//	}
	virtual ~Adapter20Sim() {
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
	XVMatrix* getLink()
	{
		return _link;
	}

	flat_matrix & getValue() {
		data.resize(_link->getCArray().count());
		data.assign(_link->getCArray().address(),_link->getCArray().address()+_link->getCArray().count());
		return data;
	}

	void setPort(T &port) {
		_port = &port;
	}
	T& getPort() {
		return *_port;
	}
	void setValue(flat_matrix &rsh) {
		data.assign( rsh.begin(),rsh.end());
		_link->setValues(data);

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

