#include <string>
#include <vector>
#include <iostream>

#include "utils.hpp"
#include "crew.hpp"
#include "external_event.hpp"

namespace wowsmod
{
	ExternalEvent::ExternalEvent(const std::string& _name, const std::string& _externalId, int _frontIndex)
		: name(_name), externalId(_externalId), frontIndex(_frontIndex) {}

	const int ExternalEvent::front() const { return frontIndex; }

	const bool ExternalEvent::push(std::vector<State>&& stateList, int& index)
	{
		int i = 0;
		for (; i < pathVector.size(); ++i)
		{
			if (stateList == pathVector[i])
			{
				break;
			}
		}
		if (i == pathVector.size())
		{
			pathVector.push_back(std::move(stateList));
			index = (int)pathVector.size() - 1;
			return true;
		}
		index = i;
		return false;
	}

	void ExternalEvent::appendExternalEventNode(Element* audioModificationNode, const Crew& c) const
	{
		auto externalEventNode = audioModificationNode->InsertNewChildElementWithIndent("ExternalEvent", 2);

		externalEventNode->InsertNewChildElementWithIndent("Name", 3)->InsertNewText(name.c_str());
		auto containerNode = externalEventNode->InsertNewChildElementWithIndent("Container", 3);

		containerNode->InsertNewChildElementWithIndent("Name", 4)->InsertNewText("Voice");
		containerNode->InsertNewChildElementWithIndent("ExternalId", 4)->InsertNewText(externalId.c_str());

		int i = 0;
		for (const std::vector<State>& stateList : pathVector)
		{
			auto pathNode = containerNode->InsertNewChildElementWithIndent("Path", 4);

			auto stateListNode = pathNode->InsertNewChildElementWithIndent("StateList", 5);
			for (const State& s : stateList)
			{
				s.appendStateNode(stateListNode);
			}
			c.appendFilesListNode(pathNode, i + frontIndex);
			++i;
		}
	}

	void ExternalEvent::print(bool isFinal) const
	{
		std::string __1 = isFinal ? "  ©¸ " : "  ©À ";
		std::string __2 = isFinal ? "    " : "  ©¦ ";
		std::cout
			<< __1 << "ExternalEvent" << std::endl
			<< __2 << " ©À Name: " << name << std::endl
			<< __2 << " ©À ExternalId: " << externalId << std::endl;
		for (const std::vector<State>& v : pathVector)
		{
			std::string __3 = (&v == &pathVector.back()) ? " ©¸ " : " ©À ";
			std::string __4 = (&v == &pathVector.back()) ? "   " : " ©¦ ";
			std::cout << __2 << __3 << "StateList\n";
			for (const State& s : v)
			{
				std::string __5 = (&s == &v.back()) ? " ©¸ " : " ©À ";
				s.print(__2, __4, __5);
			}
		}
	}
}