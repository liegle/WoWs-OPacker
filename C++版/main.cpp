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
vector<string> get_commlist(string);
int create_comm_dirs(string, string);

int main()
{
	CoInitialize(NULL);
	const int version = 2;
	vector<string> ModsVec = start_menu(version);
	if (!ModsVec.empty())
	{
		pack_mods(ModsVec);
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
		for (vector<string>::iterator ModsIt = ModsVec.begin(); ModsIt != ModsVec.end(); ModsIt++)
		{
			vector<string> CommList;
			CommList = get_commlist(*ModsIt);
			if (CommList.empty())
			{
				continue;
			}
			for (vector<string>::iterator CommName = CommList.begin(); CommName != CommList.end(); CommName++)
			{
				if (!create_comm_dirs(*ModsIt, *CommName))
				{
					continue;
				}

				MSXML2::IXMLDOMDocumentPtr ModXml;
				MSXML2::IXMLDOMNodeListPtr NodeList;

				if (!SUCCEEDED(ModXml.CreateInstance(__uuidof(MSXML2::DOMDocument60))))
				{
					cout << *ModsIt + ' ' + *CommName << "：创建xml对象失败\n";
					ModXml.Release();
					continue;
				}
				cout << *ModsIt + ' ' + *CommName << "：已创建xml对象\n";
				ModXml->put_preserveWhiteSpace(VARIANT_FALSE); 
				if (VARIANT_FALSE == ModXml->load((_variant_t)("res_unpack\\banks\\OfficialMods\\" + *ModsIt + "\\mod.xml").c_str()))
				{
					cout << *ModsIt + ' ' + *CommName << "：xml文件载入失败，原因：" << string(ModXml->parseError->Getreason()) << endl;
					ModXml.Release();
					continue;
				}
				cout << *ModsIt + ' ' + *CommName << "：已载入xml文件\n";

				ModXml->selectSingleNode("/AudioModification.xml/AudioModification/Name")->Puttext((_bstr_t)(*ModsIt + ' ' + *CommName).c_str());
				
				NodeList = ModXml->selectNodes("/AudioModification.xml/AudioModification/ExternalEvent/Container/Path");
				for (int i = 0; i < NodeList->Getlength(); i++)
				{
					if (NodeList->Getitem(i)->selectSingleNode("StateList/State[Name='CrewName']/Value")->hasChildNodes())
					{
						if (string(NodeList->Getitem(i)->selectSingleNode("StateList/State[Name='CrewName']/Value")->Gettext()) != *CommName)
						{
							NodeList->Getitem(i)->GetparentNode()->removeChild(NodeList->Getitem(i));
							//i--;
						}
						else
						{
							NodeList->Getitem(i)->selectSingleNode("StateList")
								->removeChild(NodeList->Getitem(i)->selectSingleNode("StateList/State[Name='CrewName']"));
						}
					}
				}
				NodeList.Release();
				NodeList = NULL;
				if (!SUCCEEDED(ModXml->save((_variant_t)(("bin\\banks\\Mods\\" + *ModsIt + ' ' + *CommName + "\\mod.xml").c_str()))))
				{
					cout << *ModsIt + ' ' + *CommName << "：xml文件输出失败，原因：" << string(ModXml->GetparseError()->Getreason()) << endl;
					ModXml.Release();
					continue;
				}
				cout << *ModsIt + ' ' + *CommName << "：已输出xml文件\n";

				NodeList = ModXml->selectNodes("/AudioModification.xml/AudioModification/ExternalEvent/Container/Path/FilesList/File/Name");
				for (int i = 0; i < NodeList->Getlength(); i++)
				{
					copy_file("res_unpack\\banks\\OfficialMods\\" + *ModsIt + '\\' + string(NodeList->Getitem(i)->Gettext()),
						"bin\\banks\\Mods\\" + *ModsIt + ' ' + *CommName + '\\' + string(NodeList->Getitem(i)->Gettext()),
						copy_options::overwrite_existing);
				}
				NodeList.Release();
				NodeList = NULL;
				cout << *ModsIt + ' ' + *CommName << "：已复制wem文件\n";
				ModXml.Release();
				ModXml = NULL;
			}
		}
		cout << "--------------------\n完成\nbanks文件夹将生成于C:\\Games\\World_of_Warships\\bin下\n请自行移动至bin\\版本编号\\res_mods中（放在readme.txt旁）\n\n";
		getchar();
	}
}

vector<string> get_commlist(string ModName)
{
	vector<string> CommList;
	MSXML2::IXMLDOMDocumentPtr ModXml;

	if (!SUCCEEDED(ModXml.CreateInstance(__uuidof(MSXML2::DOMDocument60))))
	{
		cout << ModName << "：创建xml对象失败\n";
		ModXml.Release();
		return vector<string>();
	}
	ModXml->put_preserveWhiteSpace(VARIANT_FALSE);
	cout << ModName << "：已创建xml对象\n";
	if (VARIANT_FALSE == ModXml->load((_variant_t)("res_unpack\\banks\\OfficialMods\\" + ModName + "\\mod.xml").c_str()))
	{
		cout << ModName << "：xml文件载入失败，原因：" << string(ModXml->parseError->Getreason()) << endl;
		ModXml.Release();
		return vector<string>();
	}
	cout << ModName << "：已载入xml文件\n";
	MSXML2::IXMLDOMNodeListPtr NodeList;

	NodeList = ModXml->selectSingleNode("/AudioModification.xml/AudioModification/ExternalEvent/Container")
		->selectNodes("Path/StateList/State[Name='CrewName']/Value");
	for (int i = 0; i < NodeList->Getlength(); i++)
	{
		CommList.push_back((string)NodeList->Getitem(i)->Gettext());
	}
	NodeList.Release();
	NodeList = NULL;
	return CommList;
}

int create_comm_dirs(string ModName, string CommName)
{
	if (exists(path("bin\\banks\\Mods") / (ModName + ' ' + CommName)))
	{
		cout << ModName + ' ' + CommName << "：文件夹已存在\n";
	}
	else if (!create_directories(path("bin\\banks\\Mods") / (ModName + ' ' + CommName)))
	{
		cout << ModName + ' ' + CommName << "：创建文件夹失败\n";
		return 0;
	}
	else
	{
		cout << ModName + ' ' + CommName << "：创建文件夹成功\n";
	}
	return 1;
}
