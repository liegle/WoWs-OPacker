#include "header.hpp"

namespace wowsmod
{
	crew::crew(const ::std::string& _mod, const ::std::string& _name)
		: mod(_mod), name(_name) {}

	const ::std::string crew::getName() const { return mod + ' ' + name; }

	const bool crew::is(const ::std::string& _name) const { return name == _name; }

	void crew::set(const int index, ::std::vector<::std::string>&& fileList)
	{
		if (fileList.size() == 0)
		{
			fileList.push_back("TISHINA.wem");
		}
		size_t indexPlusOne;
		indexPlusOne = (size_t)index + 1;
		if (pathVector.size() < indexPlusOne)
		{
			pathVector.resize(indexPlusOne, ::std::vector<::std::string>());
		}
		pathVector[index] = ::std::move(fileList);
	}

	void crew::validate()
	{
		fileCount = 0;
		for (::std::vector<::std::string>& v : pathVector)
		{
			if (v.size() == 0)
			{
				v.push_back("TISHINA.wem");
			}
			fileCount += (int)v.size();
		}
	}

	const bool crew::createDirectory() const
	{
		if (::std::filesystem::exists("bin\\banks\\Mods\\" + getName()))
		{
			::std::cout << getName() << ": 文件夹已存在\n";
		}
		else if (!::std::filesystem::create_directories("bin\\banks\\Mods\\" + getName()))
		{
			::std::cout << getName() << ": 创建文件夹失败\n";
			return false;
		}
		else
		{
			::std::cout << getName() << ": 创建文件夹成功\n";
		}
		return true;
	}

	void crew::appendNameNode(const xml outXml, const node audioModificationNode) const
	{
		insertNode(outXml, audioModificationNode, "Name", 2)->appendChild(outXml->createTextNode(name.c_str()));
	}

	void crew::appendFilesListNode(const xml outXml, const node pathNode, const int index) const
	{
		node filesListNode = insertNode(outXml, pathNode, "FilesList", 5);
		if (index >= pathVector.size())
		{
			return;
		}
		for (const ::std::string& file : pathVector[index])
		{
			node fileNode = insertNode(outXml, filesListNode, "File", 6);
			insertNode(outXml, fileNode, "Name", 7)->appendChild(outXml->createTextNode(file.c_str()));
		}
	}

	void crew::copyVoiceWem() const
	{
		int succeeded = 0;

		for (const ::std::vector<::std::string>& fileList : pathVector)
		{
			for (const ::std::string& file : fileList)
			{
				succeeded += ::std::filesystem::copy_file
				(
					"res_unpack\\banks\\OfficialMods\\" + mod + '\\' + file,
					"bin\\banks\\Mods\\" + getName() + '\\' + file,
					::std::filesystem::copy_options::update_existing
				);
			}
		}

		::std::cout << getName() << ": 已复制" << succeeded << "个.wem文件\n";
	}

	void crew::print() const
	{
		::std::cout
			<< "Crew" << ::std::endl
			<< "  ├ Name: " << mod << ' ' << name << ::std::endl
			<< "  ├ Path Count: " << pathVector.size() << ::std::endl
			<< "  ├ File Count: " << fileCount << ::std::endl;
		for (const ::std::vector<::std::string>& v : pathVector)
		{
			::std::string __1 = (&v == &pathVector.back()) ? "  └ " : "  ├ ";
			::std::string __2 = (&v == &pathVector.back()) ? "    " : "  │ ";
			::std::cout << __1 << "FileList\n";
			for (const ::std::string& s : v)
			{
				::std::string __3 = (&s == &v.back()) ? " └ " : " ├ ";
				::std::cout << __2 << __3 << s << ::std::endl;
			}
		}
	}
}