#pragma once

#include "header.hpp"

namespace wowsmod
{
	class mod
	{
	private:
		::std::string name;
		xml modXml;

		::std::vector<external_event> xmlTemplate;
		int pathCount = 0;
		::std::vector<crew> crewVector;

		mod(const ::std::string&);

		const bool loadXMLDocument();
		void readModXml();
		xml createModXml(const crew&) const;
		void writeModXmlAndCopyVoiceWem() const;
		void print() const;

		static ::std::vector<mod> modVector;
		static void findMods();
		static void release();
	public:
		static void generateMods();
		static void generateTemplate();
	};

}