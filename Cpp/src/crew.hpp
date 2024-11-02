#pragma once

namespace wowsmod
{
	class Crew
	{
	private:
		std::string mod;
		std::string name;

		std::vector<std::vector<std::string>> pathVector;
		int fileCount = 0;

	public:
		Crew(const std::string&, const std::string&);

		const std::string getName() const;
		const bool is(const std::string&) const;
		void set(const int, std::vector<std::string>&&);
		void validate();
		const bool createDirectory() const;
		void appendNameNode(Element*) const;
		void appendFilesListNode(Element*, const int) const;
		void copyVoiceWem() const;
		void print() const;
	};
}