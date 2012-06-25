#include "Adapter20Sim.h"

#include <boost/algorithm/string.hpp>

namespace common20sim {

	std::string replaceIllegalCharacter(std::string str)
	{
		str = replaceIllegalCharacter(str, "\\", "_");
		str = replaceIllegalCharacter(str, "[", "__");
		str = replaceIllegalCharacter(str, "]", "__");
		str = replaceIllegalCharacter(str, ".", "_");
		str = replaceIllegalCharacter(str, ",", "_");
		return str;
	}

	std::string replaceIllegalCharacter(std::string str, std::string pattern, std::string replacement)
	{
	  size_t found;
	  found=str.find_first_of(pattern);
    while (found != std::string::npos)
    {
      str.replace(found,1, replacement);
      found=str.find_first_of(pattern,found+replacement.length());
    }
    return str;
	}

	std::string makeShortName(std::string str)
	{
		size_t pos;
		pos=str.find_last_of("\\");
		if (pos != std::string::npos)
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

}

