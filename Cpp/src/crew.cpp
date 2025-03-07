#include <string>
#include <vector>
#include <filesystem>
#include <iostream>

#include "utils.hpp"
#include "crew.hpp"

namespace wowsmod
{
	Crew::Crew(const std::string& _mod, const std::string& _name)
		: mod(_mod), name(_name) {}

	const std::string Crew::getName() const { return mod + ' ' + name; }

	const bool Crew::is(const std::string& _name) const { return name == _name; }

	void Crew::set(const int index, std::vector<std::string>&& fileList)
	{
		if (fileList.size() == 0)
		{
			fileList.push_back("TISHINA.wem");
		}
		size_t indexPlusOne;
		indexPlusOne = (size_t)index + 1;
		if (pathVector.size() < indexPlusOne)
		{
			pathVector.resize(indexPlusOne, std::vector<std::string>());
		}
		pathVector[index] = std::move(fileList);
	}

	void Crew::validate()
	{
		fileCount = 0;
		for (std::vector<std::string>& v : pathVector)
		{
			if (v.size() == 0)
			{
				v.push_back("TISHINA.wem");
			}
			fileCount += (int)v.size();
		}
	}

	const bool Crew::createDirectory() const
	{
		if (std::filesystem::exists("bin\\banks\\Mods\\" + getName()))
		{
			std::cout << getName() << ": 文件夹已存在\n";
		}
		else if (!std::filesystem::create_directories("bin\\banks\\Mods\\" + getName()))
		{
			std::cout << getName() << ": 创建文件夹失败\n";
			return false;
		}
		else
		{
			std::cout << getName() << ": 创建文件夹成功\n";
		}
		return true;
	}

	void Crew::appendNameNode(Element* audioModificationNode) const
	{
		audioModificationNode->InsertNewChildElementWithIndent("Name", 2)->InsertNewText(name.c_str());
	}

	void Crew::appendFilesListNode(Element* pathNode, const int index) const
	{
		auto filesListNode = pathNode->InsertNewChildElementWithIndent("FilesList", 5);
		if (index >= pathVector.size())
		{
			return;
		}
		for (const std::string& file : pathVector[index])
		{
			filesListNode->InsertNewChildElementWithIndent("File", 6)
				->InsertNewChildElementWithIndent("Name", 7)
				->InsertNewText(file.c_str());
		}
	}

	void Crew::copyVoiceWem() const
	{
		int succeeded = 0;

		for (const std::vector<std::string>& fileList : pathVector)
		{
			for (const std::string& file : fileList)
			{
				auto from = "res_unpack\\banks\\OfficialMods\\" + mod + '\\' + file;
				auto to = "bin\\banks\\Mods\\" + getName() + '\\' + file;
				if (!std::filesystem::is_regular_file(from))
				{
					std::cout << getName() << ": 未找到" << from << std::endl;
					continue;
				}
				succeeded += std::filesystem::copy_file(from, to, std::filesystem::copy_options::update_existing);
			}
		}

		std::cout << getName() << ": 已复制" << succeeded << "个.wem文件\n";
	}

	void Crew::print() const
	{
		std::cout
			<< "Crew" << std::endl
			<< "  ├ Name: " << mod << ' ' << name << std::endl
			<< "  ├ Path Count: " << pathVector.size() << std::endl
			<< "  ├ File Count: " << fileCount << std::endl;
		for (const std::vector<std::string>& v : pathVector)
		{
			std::string __1 = (&v == &pathVector.back()) ? "  └ " : "  ├ ";
			std::string __2 = (&v == &pathVector.back()) ? "    " : "  │ ";
			std::cout << __1 << "FileList\n";
			for (const std::string& s : v)
			{
				std::string __3 = (&s == &v.back()) ? " └ " : " ├ ";
				std::cout << __2 << __3 << s << std::endl;
			}
		}
	}
}