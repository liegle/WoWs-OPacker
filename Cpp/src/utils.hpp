#pragma once

#include <vector>
#include "../lib/tinyxml2.h"

namespace wowsmod
{
	static constexpr int VERSION = 6;
	static constexpr char DEFAULT_DIRECTORY[] = "res_unpack\\banks\\OfficialMods\\";

	class Element : public tinyxml2::XMLElement
	{
	public:
		Element* PathElement(std::vector<const char*>);
		std::vector<Element*> PathElementList(std::vector<const char*>);
		Element* InsertNewChildElementWithIndent(const std::string&, const int);
	};

	const int printMenu(std::vector<std::string>);
}