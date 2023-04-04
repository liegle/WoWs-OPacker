#include "header.hpp"

namespace wowsmod
{
	external_event::external_event(const ::std::string& _name, const ::std::string& _externalId, int _frontIndex)
		: name(_name), externalId(_externalId), frontIndex(_frontIndex) {}

	const int external_event::front() const { return frontIndex; }

	const bool external_event::push(::std::vector<state>&& stateList, int& index)
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
			pathVector.push_back(::std::move(stateList));
			index = (int)pathVector.size() - 1;
			return true;
		}
		index = i;
		return false;
	}

	void external_event::appendExternalEventNode(const xml outXml, const node audioModificationNode, const crew& c) const
	{
		node externalEventNode = insertNode(outXml, audioModificationNode, "ExternalEvent", 2);

		insertNode(outXml, externalEventNode, "Name", 3)->appendChild(outXml->createTextNode(name.c_str()));
		node containerNode = insertNode(outXml, externalEventNode, "Container", 3);

		insertNode(outXml, containerNode, "Name", 4)->appendChild(outXml->createTextNode("Voice"));
		insertNode(outXml, containerNode, "ExternalId", 4)->appendChild(outXml->createTextNode(externalId.c_str()));

		int i = 0;
		for (const ::std::vector<state>& stateList : pathVector)
		{
			node pathNode = insertNode(outXml, containerNode, "Path", 4);

			node stateListNode = insertNode(outXml, pathNode, "StateList", 5);
			for (const state& s : stateList)
			{
				s.appendStateNode(outXml, stateListNode);
			}
			c.appendFilesListNode(outXml, pathNode, i + frontIndex);
			++i;
		}
	}

	void external_event::print(bool isFinal) const
	{
		::std::string __1 = isFinal ? "  ©¸ " : "  ©À ";
		::std::string __2 = isFinal ? "    " : "  ©¦ ";
		::std::cout
			<< __1 << "ExternalEvent" << ::std::endl
			<< __2 << " ©À Name: " << name << ::std::endl
			<< __2 << " ©À ExternalId: " << externalId << ::std::endl;
		for (const ::std::vector<state>& v : pathVector)
		{
			::std::string __3 = (&v == &pathVector.back()) ? " ©¸ " : " ©À ";
			::std::string __4 = (&v == &pathVector.back()) ? "   " : " ©¦ ";
			::std::cout << __2 << __3 << "StateList\n";
			for (const state& s : v)
			{
				::std::string __5 = (&s == &v.back()) ? " ©¸ " : " ©À ";
				s.print(__2, __4, __5);
			}
		}
	}
}