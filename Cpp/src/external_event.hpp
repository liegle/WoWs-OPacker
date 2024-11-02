#pragma once

#include "state.hpp"

namespace wowsmod
{
	class ExternalEvent
	{
	private:
		std::string name;
		std::string externalId;
		int frontIndex;

		std::vector<std::vector<State>> pathVector;

	public:
		ExternalEvent(const std::string&, const std::string&, int);

		const int front() const;
		const bool push(std::vector<State>&&, int&);
		void appendExternalEventNode(Element*, const Crew&) const;
		void print(bool) const;
	};
}