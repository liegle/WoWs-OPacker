#pragma once

#include "header.hpp"

namespace wowsmod
{
	class crew
	{
	private:
		::std::string mod;
		::std::string name;

		::std::vector<::std::vector<::std::string>> pathVector;
		int fileCount = 0;

	public:
		crew(const ::std::string&, const ::std::string&);

		const ::std::string getName() const;
		const bool is(const ::std::string&) const;
		void set(const int, ::std::vector<::std::string>&&);
		void validate();
		const bool createDirectory() const;
		void appendNameNode(const xml, const node) const;
		void appendFilesListNode(const xml, const node, const int) const;
		void copyVoiceWem() const;
		void print() const;
	};
}