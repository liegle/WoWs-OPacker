#include <iostream>
#include <conio.h>

#include "utils.hpp"
#include "mod.hpp"

int main()
{
	std::cout
		<< "OPacker" << std::endl
		<< "�汾��" << wowsmod::VERSION << std::endl
		<< "����BUG˽��BվUID38112033" << std::endl;

	if (::wowsmod::printMenu({ "����mod.xmlģ��", "��������mod" }))
	{
		::wowsmod::OfficialMod::generateMods();
	}
	else
	{
		::wowsmod::OfficialMod::generateTemplate();
	}

	std::cout << "��������˳�\n";
	while (!_kbhit());
}