#include "header.hpp"

namespace wowsmod
{
	mod::mod(const ::std::string& _name) : name(_name) {}

	const bool mod::loadXMLDocument()
	{
		if (FAILED(modXml.CreateInstance(__uuidof(::MSXML2::DOMDocument60))))
		{
			::std::cout << name << ": ����xml����ʧ��\n";
			return false;
		}

		if (VARIANT_FALSE == modXml->load((_variant_t)("res_unpack\\banks\\OfficialMods\\" + name + "\\mod.xml").c_str()))
		{
			::std::cout << name << ": ����mod.xml�ļ�ʧ��\n";
			return false;
		}

		name = (::std::string)modXml->selectSingleNode("/AudioModification.xml/AudioModification/Name")->Gettext();
		::std::cout << name << ": �Ѽ���mod.xml�ļ�\n";
		return true;
	}

	void mod::readModXml()
	{
		node_list eventNodeList = modXml->selectNodes("/AudioModification.xml/AudioModification/ExternalEvent");
		for (int i = 0; i < eventNodeList->Getlength(); ++i)
		{
			node&& currentNode = eventNodeList->Getitem(i);
			xmlTemplate.push_back
			(
				external_event
				(
					(::std::string)currentNode->selectSingleNode("Name")->Gettext(),
					(::std::string)currentNode->selectSingleNode("Container/ExternalId")->Gettext(),
					pathCount
				)
			);
			external_event& currentEvent = xmlTemplate.back();

			node_list pathNodeList = currentNode->selectNodes("Container/Path");
			for (int j = 0; j < pathNodeList->Getlength(); ++j)
			{
				node&& currentPath = pathNodeList->Getitem(j);

				node_list stateNodeList = currentPath->selectNodes("StateList/State");
				::std::vector<state> stateList;
				for (int k = 0; k < stateNodeList->Getlength(); ++k)
				{
					::std::string name = (::std::string)stateNodeList->Getitem(k)->GetfirstChild()->Gettext();
					if (name != "CrewName")
					{
						::std::string value = (::std::string)stateNodeList->Getitem(k)->GetlastChild()->Gettext();
						stateList.push_back(state(name, value));
					}
				}

				int index;
				if (currentEvent.push(::std::move(stateList), index))
				{
					++pathCount;
				}

				auto node = currentPath->selectSingleNode("StateList/State[Name='CrewName']/Value");
				if (node == nullptr) return;
				::std::string crewName = (::std::string)node->Gettext();
				::std::vector<crew>::iterator found = find_if(crewVector.begin(), crewVector.end(), [&](crew& c) { return c.is(crewName); });
				crew& currentCrew =
					(
						(found == crewVector.end()) ?
						(crewVector.push_back(crew(name, crewName)), crewVector.back())
						: *found
						);

				node_list fileNameNodeList = currentPath->selectNodes("FilesList/File/Name");
				::std::vector<::std::string> fileList;
				for (int k = 0; k < fileNameNodeList->Getlength(); ++k)
				{
					fileList.push_back((::std::string)fileNameNodeList->Getitem(k)->Gettext());
				}

				currentCrew.set(currentEvent.front() + index, ::std::move(fileList));
			}
		}

		for (crew& c : crewVector)
		{
			c.validate();
		}
		::std::cout << name << ": �Ѷ�ȡmod.xml�ļ�\n";
	}

	xml mod::createModXml(const crew& c) const
	{
		xml outXml;
		if (FAILED(outXml.CreateInstance(__uuidof(::MSXML2::DOMDocument60))))
		{
			::std::cout << name << ": ����xml����ʧ��\n";
			return nullptr;
		}
		outXml->preserveWhiteSpace = VARIANT_FALSE;
		outXml->appendChild(outXml->createProcessingInstruction("xml", "version=\"1.0\""));
		element root = outXml->createElement("AudioModification.xml");
		outXml->PutRefdocumentElement(root);
		node audioModificationNode = insertNode(outXml, root, "AudioModification", 1);
		c.appendNameNode(outXml, audioModificationNode);
		for (const external_event& e : xmlTemplate)
		{
			e.appendExternalEventNode(outXml, audioModificationNode, c);
		}
		return outXml;
	}

	void mod::writeModXmlAndCopyVoiceWem() const
	{
		int succeeded = 0;
		for (const crew& c : crewVector)
		{
			xml outXml = createModXml(c);
			if (!outXml || !c.createDirectory())
			{
				continue;
			}

			if (FAILED(outXml->save((_variant_t)(("bin\\banks\\Mods\\" + c.getName() + "\\mod.xml").c_str()))))
			{
				::std::cout << c.getName() << ": xml�ļ����ʧ��\n";
				continue;
			}
			::std::cout << c.getName() << ": �����xml�ļ�\n";

			c.copyVoiceWem();
			++succeeded;
		}
		::std::cout << "�ѳɹ����" << succeeded << '/' << crewVector.size() << "����ɫ\n";
	}

	void mod::print() const
	{
		::std::cout
			<< "Mod Xml Template" << ::std::endl
			<< "  �� Name: " << name << ::std::endl
			<< "  �� External Event Count: " << xmlTemplate.size() << ::std::endl
			<< "  �� Path Count: " << pathCount << ::std::endl;
		for (const external_event& e : xmlTemplate)
		{
			e.print(&e == &xmlTemplate.back());
		}

		for (const crew& c : crewVector)
		{
			c.print();
		}
	}

#pragma region Static
	inline ::std::vector<mod> mod::modVector;

	void mod::findMods()
	{
		if (!::std::filesystem::exists("res_unpack\\banks\\OfficialMods\\"))
		{
			::std::cout << "res_unpack\\banks\\OfficialMods\\������\n";
			return;
		}
		else if (::std::filesystem::is_empty("res_unpack\\banks\\OfficialMods\\"))
		{
			::std::cout << "res_unpack\\banks\\OfficialMods\\Ϊ��\n";
			return;
		}

		for (::std::filesystem::directory_iterator i("res_unpack\\banks\\OfficialMods\\"); i != end(i); ++i)
		{
			if (exists(i->path() / "mod.xml"))
			{
				::std::string modDirectoryName = i->path().filename().string();
				modVector.push_back(mod(modDirectoryName));
			}
		}
		::std::cout << "�ҵ�" << modVector.size() << "��mod\n";
	}

	void mod::release()
	{
		modVector.erase(modVector.begin(), modVector.end());
	}

	void mod::generateMods()
	{
		findMods();
		for (mod& m : modVector)
		{
			if (m.loadXMLDocument())
			{
				m.readModXml();
				m.writeModXmlAndCopyVoiceWem();
				//m.print();
			}
		}
		release();
		::std::cout
			<< "banks�ļ��н�������" << (::std::filesystem::current_path() / "bin").string() << "��" << ::std::endl
			<< "�������ƶ���bin\\�汾���\\res_mods�У�����readme.txt�ԣ�" << ::std::endl << ::std::endl;

	}

	void mod::generateTemplate()
	{
		findMods();
		if (modVector.size() && modVector[0].loadXMLDocument())
		{
			modVector[0].readModXml();
			::std::cout << "������" << modVector[0].name << "����mod.xmlģ��";
			xml outXml = modVector[0].createModXml(crew("", "Template"));
			if (!outXml)
			{
				return;
			}
			if (::std::filesystem::exists("bin"))
			{
				::std::cout << "bin�ļ����Ѵ���\n";
			}
			else if (!::std::filesystem::create_directories("bin"))
			{
				::std::cout << "����bin�ļ���ʧ��\n";
			}
			else
			{
				::std::cout << "����bin�ļ��гɹ�\n";
			}
			if (FAILED(outXml->save((_variant_t)"bin\\mod.xml")))
			{
				::std::cout << "mod.xmlģ���ļ����ʧ��\n";
			}
			::std::cout << "����" << (::std::filesystem::current_path() / "bin").string() << "�����mod.xmlģ���ļ�\n";
		}
		release();
	}
#pragma endregion
}