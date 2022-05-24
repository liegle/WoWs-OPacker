#include <iostream>
#include <stdlib.h>
#include <cstring>
#include <sstream>
#include <filesystem>
#include <atlbase.h>

#import <msxml6.dll>

using namespace std;
using namespace filesystem;

vector<string> start_menu(int);
void pack_mods(vector<string>);

int main()
{
	CoInitialize(NULL);
	const int version = 1;
	vector<string> ModsVec = start_menu(version);
	if (!ModsVec.empty())
	{
		pack_mods(ModsVec);
		cout << "完成\n\n\n";
	}
	CoUninitialize();
}

vector<string> start_menu(int version)
{
	char Choice = 'y';
vector<string> ModsVec;

cout << "OPacker\n版本：" << version << "\n反馈BUG私信B站UID38112033\n--------------------\n开始搜索[Y]退出[其他]\n";
cin >> Choice;
if (Choice == 'y' || Choice == 'Y')
{
	path Folder("res_unpack\\banks\\OfficialMods");

	if (!exists(Folder))
	{
		cout << "目标文件夹不存在\n";
		return ModsVec;
	}
	else if (filesystem::is_empty(Folder))
	{
		cout << "目标文件夹为空\n";
		return ModsVec;
	}
	else
	{
		int ModNum = 0;
		directory_iterator ModsIt(Folder);
		string ModName;

		while (ModsIt != end(ModsIt))
		{
			if (exists(ModsIt->path() / "mod.xml"))
			{
				if (ModNum == 0)
				{
					cout << "--------------------\n";
				}
				ModName = ModsIt->path().filename().string();
				ModsVec.push_back(ModName);
				cout << ModNum << ". " << ModName << endl;
				++ModNum;
			}
			++ModsIt;
		}
		if (ModNum == 0)
		{
			cout << "未找到可用的Officialmod\n";
		}
		else
		{
			cout << "已找到" << ModNum << "个可能可用的Officialmod，全部生成[Y]退出[其他]\n";
		}
		return ModsVec;
	}
}
else
{
	return ModsVec;
}
}

void pack_mods(vector<string> ModsVec)
{
	char Choice = 'y';

	cin >> Choice;
	if (Choice == 'y' || Choice == 'Y')
	{
		MSXML2::IXMLDOMDocumentPtr ModXml;
		if (!SUCCEEDED(ModXml.CreateInstance(__uuidof(MSXML2::DOMDocument60))))
		{
			cout << "创建xml对象失败\n";
			ModXml.Detach();
			return;
		}
		cout << "已创建xml对象\n";
		cout << "--------------------\n";
		cout << "banks文件夹将生成于C:\\Games\\World_of_Warships\\bin下\n请自行移动至bin\\版本编号\\res_mods中（放在readme.txt旁）\n";
		for (vector<string>::iterator ModsIt = ModsVec.begin(); ModsIt != ModsVec.end(); ModsIt++)
		{
			vector<string> Comm;

			ModXml->put_preserveWhiteSpace(VARIANT_FALSE);
			if (VARIANT_FALSE == ModXml->load((_variant_t)("res_unpack\\banks\\OfficialMods\\" + *ModsIt + "\\mod.xml").c_str()))
			{
				cout << "xml文件载入失败，原因：" << string(ModXml->parseError->Getreason()) << endl;
				ModXml.Detach();
				return;
			}
			cout << *ModsIt << "：已载入xml文件\n";

			MSXML2::IXMLDOMNodeListPtr Containers, Names, Paths;
			MSXML2::IXMLDOMNodePtr Container, Path;

			Containers = ModXml->selectNodes("/AudioModification.xml/AudioModification/ExternalEvent/Container");
			Container = Containers->nextNode();
			Names = Container->selectNodes("Path/StateList/State[Name='CrewName']/Value");
			for (int i = 0; i < Names->Getlength(); i++)
			{
				Comm.push_back(string(Names->nextNode()->GetfirstChild()->Gettext()));
			}

			for (vector<string>::iterator CommName = Comm.begin(); CommName != Comm.end(); CommName++)
			{
				if (exists(path("bin\\banks\\Mods") / (*ModsIt + ' ' + *CommName)))
				{
					cout << *ModsIt + ' ' + *CommName << "：文件夹已存在\n";
				}
				else if (!create_directories(path("bin\\banks\\Mods") / (*ModsIt + ' ' + *CommName)))
				{
					cout << *ModsIt + ' ' + *CommName << "：创建文件夹失败\n";
					continue;
				}
				else
				{
					cout << *ModsIt + ' ' + *CommName << "：创建文件夹成功\n";
				}

				ModXml->selectSingleNode("/AudioModification.xml/AudioModification/Name")->GetfirstChild()->Puttext((_bstr_t)(*ModsIt + ' ' + *CommName).c_str());
				Containers->reset();
				for (int i = 0; i < Containers->Getlength(); i++)
				{
					Container = Containers->nextNode();
					Paths = Container->GetchildNodes();
					Paths->reset();
					for (int j = 0; j < Paths->Getlength(); j++)
					{
						Path = Paths->nextNode();
						if (string(Path->GetnodeName()) == "Path" && Path->selectSingleNode("StateList/State[Name='CrewName']/Value")->hasChildNodes())
						{
							if (string(Path->selectSingleNode("StateList/State[Name='CrewName']/Value")->GetfirstChild()->Gettext()) != *CommName)
							{
								Container->removeChild(Path);
								j--;
							}
							else
							{
								Path->selectSingleNode("StateList")->removeChild(Path->selectSingleNode("StateList/State[Name='CrewName']"));
							}
						}
					}
				}
				if (!SUCCEEDED(ModXml->save((_variant_t)(("bin\\banks\\Mods\\" + *ModsIt + ' ' + *CommName + "\\mod.xml").c_str()))))
				{
					cout << "xml文件输出失败，原因：" << string(ModXml->parseError->Getreason()) << endl;
					Containers.Detach();
					Names.Detach();
					Paths.Detach();
					Container.Detach();
					Path.Detach();
					ModXml.Detach();
					return;
				}
				cout << *ModsIt + ' ' + *CommName << "：已输出xml文件\n";
				//复用一下Paths和Path，不然又要重新整两个指针
				Paths = ModXml->selectNodes("/AudioModification.xml/AudioModification/ExternalEvent/Container/Path/FilesList/File/Name");
				Paths->reset();
				for (int i = 0; i < Paths->Getlength(); i++)
				{
					Path = Paths->nextNode()->GetfirstChild();
					copy_file("res_unpack\\banks\\OfficialMods\\" + *ModsIt + '\\' + string(Path->Gettext()),
						"bin\\banks\\Mods\\" + *ModsIt + ' ' + *CommName + '\\' + string(Path->Gettext()),
						copy_options::overwrite_existing);
				}
				cout << *ModsIt + ' ' + *CommName << "：已复制wem文件\n";
				if (VARIANT_FALSE == ModXml->load((_variant_t)("res_unpack\\banks\\OfficialMods\\" + *ModsIt + "\\mod.xml").c_str()))
				{
					cout << "xml文件载入失败，原因：" << string(ModXml->parseError->Getreason()) << endl;
					Containers.Detach();
					Names.Detach();
					Paths.Detach();
					Container.Detach();
					Path.Detach();
					ModXml.Detach();
					return;
				}
				cout << *ModsIt << "：已载入xml文件\n";
				Containers = ModXml->selectNodes("/AudioModification.xml/AudioModification/ExternalEvent/Container");
			}

			Containers.Detach();
			Names.Detach();
			Paths.Detach();
			Container.Detach();
			Path.Detach();
		}
		ModXml.Detach();
	}
}