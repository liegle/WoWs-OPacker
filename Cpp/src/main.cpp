#include <iostream>
#include <conio.h>

#include "utils.hpp"
#include "mod.hpp"

int main()
{
	std::cout
		<< "OPacker" << std::endl
		<< "版本：" << wowsmod::VERSION << std::endl
		<< "反馈BUG私信B站UID38112033" << std::endl;

	if (::wowsmod::printMenu({ "生成mod.xml模板", "生成所有mod" }))
	{
		::wowsmod::OfficialMod::generateMods();
	}
	else
	{
		::wowsmod::OfficialMod::generateTemplate();
	}

	std::cout << "按任意键退出\n";
	while (!_kbhit());
}