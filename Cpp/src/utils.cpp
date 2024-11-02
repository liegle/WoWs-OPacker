#include <string>
#include <iostream>
#include <conio.h>

#include "utils.hpp"

namespace wowsmod
{
	Element* Element::PathElement(std::vector<const char*> path)
	{
		if (path.size() == 0)
		{
			return this;
		}
		auto name = path[0];
		auto next = std::vector(path.begin() + 1, path.end());
		auto found = FirstChildElement(name);
		auto newThis = reinterpret_cast<Element*>(found);
		return newThis->PathElement(next);
	}

	std::vector<Element*> Element::PathElementList(std::vector<const char*> path)
	{
		std::vector<Element*> v;
		if (path.size() == 0)
		{
			v.push_back(this);
			return v;
		}
		auto name = path[0];
		auto next = std::vector(path.begin() + 1, path.end());
		for (auto child = FirstChildElement(name); child; child = child->NextSiblingElement())
		{
			if (std::string(child->Name()) != std::string(name))
			{
				continue;
			}
			auto got = reinterpret_cast<Element*>(child)->PathElementList(next);
			for (auto ele : got)
			{
				v.push_back(ele);
			}
		}
		return v;
	}

	Element* Element::InsertNewChildElementWithIndent(const std::string& name, const int indentLevel)
	{
		std::string __1 = "\n", __2 = "\n";
		for (int i = 0; i < indentLevel - 1; ++i)
		{
			__1 += '\t';
			__2 += '\t';
		}
		__1 += '\t';
		if (NoChildren())
		{
			InsertNewText(__1.c_str());
		}
		else
		{
			InsertNewText("\t");
		}
		auto child = InsertNewChildElement(name.c_str());
		InsertNewText(__2.c_str());
		return reinterpret_cast<Element*>(child);
	}

	const int printMenu(std::vector<std::string> items)
	{
		int index = 0;
		for (std::string item : items)
		{
			std::cout << index << ". " << item << std::endl;
			++index;
		}

		while (index < 0 || index >= items.size())
		{
			while (!_kbhit());
			index = _getch() - 48;
		}
		return index;
	}
}