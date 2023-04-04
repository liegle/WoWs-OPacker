#pragma once

#include "header.hpp"

namespace wowsmod
{
	class state
	{
	private:
		::std::string name;
		::std::string value;

	public:
		state(const ::std::string&, const ::std::string&);

		const bool operator==(const state&) const;
		void appendStateNode(const xml, const node) const;
		void print(::std::string, ::std::string, ::std::string) const;
	};
}