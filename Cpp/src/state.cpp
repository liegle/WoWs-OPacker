#include <string>
#include <iostream>

#include "utils.hpp"
#include "state.hpp"

namespace wowsmod
{
	State::State(const std::string& _name, const std::string& _value)
		: name(_name), value(_value) {}

	const bool State::operator==(const State& other) const
	{
		return name == other.name && value == other.value;
	}

	void State::appendStateNode(Element* stateListNode) const
	{
		auto stateNode = stateListNode->InsertNewChildElementWithIndent("State", 6);
		stateNode->InsertNewChildElementWithIndent("Name", 7)->InsertNewText(name.c_str());
		stateNode->InsertNewChildElementWithIndent("Value", 7)->InsertNewText(value.c_str());
	}

	void State::print(std::string __2, std::string __4, std::string __5) const
	{
		std::cout << __2 << __4 << __5 << "State    Name: " << name << "    Value: " << value << std::endl;
	}
}