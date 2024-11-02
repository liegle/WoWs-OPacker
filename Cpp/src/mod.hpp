#pragma once

#include "crew.hpp"
#include "external_event.hpp"

namespace wowsmod
{
	class OfficialMod
	{
	private:
		std::string name;
		std::shared_ptr<tinyxml2::XMLDocument> modXml;

		std::vector<ExternalEvent> xmlTemplate;
		int pathCount = 0;
		std::vector<Crew> crewVector;

		OfficialMod(const std::string&);

		const bool loadXMLDocument();
		void readModXml();
		std::shared_ptr<tinyxml2::XMLDocument> createModXml(const Crew&) const;
		void writeModXmlAndCopyVoiceWem() const;
		void print() const;

		static std::vector<OfficialMod> modVector;
		static void findMods();
	public:
		static void generateMods();
		static void generateTemplate();
	};
}