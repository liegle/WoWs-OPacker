#include "header.hpp"

int main()
{
	::std::cout
		<< "OPacker" << ::std::endl
		<< "�汾��" << wowsmod::version << ::std::endl
		<< "����BUG˽��BվUID38112033" << ::std::endl;

	if (FAILED(CoInitialize(nullptr)))
	{
		::std::cout << "COM��ʼ��ʧ��\n";
	}
	else if (::wowsmod::printMenu({ "����mod.xmlģ��", "��������mod" }))
	{
		::wowsmod::mod::generateMods();
	}
	else
	{
		::wowsmod::mod::generateTemplate();
	}

	::std::cout << "��������˳�\n";
	while (!_kbhit());
	CoUninitialize();
}