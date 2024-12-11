#include <string>
#include <iostream>
#include <filesystem>
#include "../lib/tinyxml2.h"

#include "utils.hpp"
#include "state.hpp"
#include "mod.hpp"

namespace wowsmod
{
	OfficialMod::OfficialMod(const std::string& _name) : name(_name) {}

	const bool OfficialMod::loadXMLDocument()
	{
		modXml = std::make_shared<tinyxml2::XMLDocument>();
		if (tinyxml2::XMLError::XML_SUCCESS != modXml->LoadFile((DEFAULT_DIRECTORY + name + "\\mod.xml").c_str()))
		{
			std::cout << name << ": ����mod.xml�ļ�ʧ��\n";
			return false;
		}

		name = reinterpret_cast<Element*>(modXml->RootElement())->PathElement({"AudioModification", "Name"})->GetText();
		std::cout << name << ": �Ѽ���mod.xml�ļ�\n";
		return true;
	}

	void OfficialMod::readModXml()
	{
		auto eventNodeList = reinterpret_cast<Element*>(modXml->RootElement())->PathElementList({"AudioModification", "ExternalEvent"});
		for (auto currentNode: eventNodeList)
		{
			xmlTemplate.push_back
			(
				ExternalEvent
				(
					std::string(currentNode->FirstChildElement("Name")->GetText()),
					std::string(currentNode->PathElement({"Container", "ExternalId"})->GetText()),
					pathCount
				)
			);
			ExternalEvent& currentEvent = xmlTemplate.back();

			auto pathNodeList = currentNode->PathElementList({"Container", "Path"});
			for (auto currentPath: pathNodeList)
			{
				auto stateNodeList = currentPath->PathElementList({"StateList", "State"});
				std::vector<State> stateList;
				Element* crewNameValueElement = nullptr;
				for (auto stateNode: stateNodeList)
				{
					auto name = std::string(stateNode->FirstChildElement()->GetText());
					if (name != "CrewName")
					{
						std::string value = std::string(stateNode->LastChildElement()->GetText());
						stateList.push_back(State(name, value));
					}
					else
					{
						crewNameValueElement = reinterpret_cast<Element*>(stateNode->FirstChildElement("Value"));
					}
				}

				if (crewNameValueElement == nullptr) continue;

				int index;
				if (currentEvent.push(std::move(stateList), index))
				{
					++pathCount;
				}

				std::string crewName = std::string(crewNameValueElement->GetText());
				std::vector<Crew>::iterator found = find_if(crewVector.begin(), crewVector.end(), [&](Crew& c) { return c.is(crewName); });
				Crew* currentCrew = nullptr;
				if (found == crewVector.end())
				{
					crewVector.emplace_back(Crew(name, crewName));
					currentCrew = crewVector.data() + (crewVector.size() - 1);
				}
				else
				{
					currentCrew = &*found;
				}

				auto fileNameNodeList = currentPath->PathElementList({"FilesList", "File", "Name"});
				std::vector<std::string> fileList;
				for (auto fileNameNode: fileNameNodeList)
				{
					fileList.push_back(std::string(fileNameNode->GetText()));
				}

				currentCrew->set(currentEvent.front() + index, std::move(fileList));
			}
		}

		for (Crew& c : crewVector)
		{
			c.validate();
		}
		std::cout << name << ": �Ѷ�ȡmod.xml�ļ�\n";
	}

	std::shared_ptr<tinyxml2::XMLDocument> OfficialMod::createModXml(const Crew& c) const
	{
		auto outXml = std::make_shared<tinyxml2::XMLDocument>();
		
		outXml->LinkEndChild(outXml->NewDeclaration());
		auto root = reinterpret_cast<Element*>(outXml->NewElement("AudioModification.xml"));
		outXml->LinkEndChild(root);
		auto audioModificationNode = root->InsertNewChildElementWithIndent("AudioModification", 1);
		c.appendNameNode(audioModificationNode);
		for (const ExternalEvent& e : xmlTemplate)
		{
			e.appendExternalEventNode(audioModificationNode, c);
		}
		return outXml;
	}

	void OfficialMod::writeModXmlAndCopyVoiceWem() const
	{
		int succeeded = 0;
		for (const Crew& c : crewVector)
		{
			auto outXml = createModXml(c);
			if (!outXml || !c.createDirectory())
			{
				continue;
			}
			if (tinyxml2::XMLError::XML_SUCCESS != outXml->SaveFile((("bin\\banks\\Mods\\" + c.getName() + "\\mod.xml").c_str())))
			{
				std::cout << c.getName() << ": xml�ļ����ʧ��\n";
				continue;
			}
			std::cout << c.getName() << ": �����xml�ļ�\n";

			c.copyVoiceWem();
			++succeeded;
		}
		std::cout << "�ѳɹ����" << succeeded << '/' << crewVector.size() << "����ɫ\n";
	}

	void OfficialMod::print() const
	{
		std::cout
			<< "Mod Xml Template" << std::endl
			<< "  �� Name: " << name << std::endl
			<< "  �� External Event Count: " << xmlTemplate.size() << std::endl
			<< "  �� Path Count: " << pathCount << std::endl;
		for (const ExternalEvent& e : xmlTemplate)
		{
			e.print(&e == &xmlTemplate.back());
		}

		for (const Crew& c : crewVector)
		{
			c.print();
		}
	}

#pragma region Static
	inline std::vector<OfficialMod> OfficialMod::modVector;

	void OfficialMod::findMods()
	{
		if (!std::filesystem::exists(DEFAULT_DIRECTORY))
		{
			std::cout << DEFAULT_DIRECTORY << "������\n";
			return;
		}
		else if (std::filesystem::is_empty(DEFAULT_DIRECTORY))
		{
			std::cout << DEFAULT_DIRECTORY << "Ϊ��\n";
			return;
		}

		for (std::filesystem::directory_iterator i(DEFAULT_DIRECTORY); i != end(i); ++i)
		{
			if (exists(i->path() / "mod.xml"))
			{
				std::string modDirectoryName = i->path().filename().string();
				modVector.push_back(OfficialMod(modDirectoryName));
			}
		}
		std::cout << "�ҵ�" << modVector.size() << "��mod\n";
	}

	void OfficialMod::generateMods()
	{
		findMods();
		for (OfficialMod& m : modVector)
		{
			if (m.loadXMLDocument())
			{
				m.readModXml();
				m.writeModXmlAndCopyVoiceWem();
				//m.print();
			}
		}
		std::cout
			<< "banks�ļ��н�������" << (std::filesystem::current_path() / "bin").string() << "��" << std::endl
			<< "�������ƶ���bin\\�汾���\\res_mods�У�����readme.txt�ԣ�" << std::endl << std::endl;

	}

	void OfficialMod::generateTemplate()
	{
		findMods();
		if (modVector.size() && modVector[0].loadXMLDocument())
		{
			modVector[0].readModXml();
			std::cout << "������" << modVector[0].name << "����mod.xmlģ��";
			auto outXml = modVector[0].createModXml(Crew("", "Template"));
			if (std::filesystem::exists("bin"))
			{
				std::cout << "bin�ļ����Ѵ���\n";
			}
			else if (!std::filesystem::create_directories("bin"))
			{
				std::cout << "����bin�ļ���ʧ��\n";
			}
			else
			{
				std::cout << "����bin�ļ��гɹ�\n";
			}
			if (tinyxml2::XMLError::XML_SUCCESS != outXml->SaveFile("bin\\mod.xml"))
			{
				std::cout << "mod.xmlģ���ļ����ʧ��\n";
			}
			std::cout << "����" << (std::filesystem::current_path() / "bin").string() << "�����mod.xmlģ���ļ�\n";
		}
	}
#pragma endregion
}