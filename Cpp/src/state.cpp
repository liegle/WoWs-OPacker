#include "header.hpp"

namespace wowsmod
{
	state::state(const ::std::string& _name, const ::std::string& _value)
		: name(_name), value(_value) {}

	const bool state::operator==(const state& other) const
	{
		return name == other.name && value == other.value;
	}

	void state::appendStateNode(const xml outXml, const node stateListNode) const
	{
		node stateNode = insertNode(outXml, stateListNode, "State", 6);
		insertNode(outXml, stateNode, "Name", 7)->appendChild(outXml->createTextNode(name.c_str()));
		insertNode(outXml, stateNode, "Value", 7)->appendChild(outXml->createTextNode(value.c_str()));
	}

	void state::print(::std::string __2, ::std::string __4, ::std::string __5) const
	{
		::std::cout << __2 << __4 << __5 << "State    Name: " << name << "    Value: " << value << ::std::endl;
	}
}