#include "header.hpp"

int main()
{
	::std::cout
		<< "OPacker" << ::std::endl
		<< "版本：" << wowsmod::version << ::std::endl
		<< "反馈BUG私信B站UID38112033" << ::std::endl;

	if (FAILED(CoInitialize(nullptr)))
	{
		::std::cout << "COM初始化失败\n";
	}
	else if (::wowsmod::printMenu({ "生成mod.xml模板", "生成所有mod" }))
	{
		::wowsmod::mod::generateMods();
	}
	else
	{
		::wowsmod::mod::generateTemplate();
	}

	::std::cout << "按任意键退出\n";
	while (!_kbhit());
	CoUninitialize();
}