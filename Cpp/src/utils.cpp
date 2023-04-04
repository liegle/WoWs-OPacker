#include "header.hpp"

namespace wowsmod
{
	node insertNode(const xml document, const node parent, const ::std::string& name, const int indentLevel)
	{
		::std::string __1 = "\n", __2 = "\n";
		for (int i = 0; i < indentLevel - 1; ++i)
		{
			__1 += '\t';
			__2 += '\t';
		}
		__1 += '\t';
		if (parent->childNodes->length)
		{
			parent->appendChild(document->createTextNode("\t"));
		}
		else
		{
			parent->appendChild(document->createTextNode(__1.c_str()));
		}
		node&& child = parent->appendChild(document->createElement(name.c_str()));
		parent->appendChild(document->createTextNode(__2.c_str()));
		return child;
	}

	const int printMenu(::std::vector<::std::string> items)
	{
		int index = 0;
		for (::std::string item : items)
		{
			::std::cout << index << ". " << item << ::std::endl;
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