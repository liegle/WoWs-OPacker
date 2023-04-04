#pragma once

#include "header.hpp"

namespace wowsmod
{
	class external_event
	{
	private:
		::std::string name;
		::std::string externalId;
		int frontIndex;

		::std::vector<::std::vector<state>> pathVector;

	public:
		external_event(const ::std::string&, const ::std::string&, int);

		const int front() const;
		const bool push(::std::vector<state>&&, int&);
		void appendExternalEventNode(const xml, const node, const crew&) const;
		void print(bool) const;
	};
}