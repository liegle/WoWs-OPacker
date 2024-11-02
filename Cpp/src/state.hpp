#pragma once

namespace wowsmod
{
	class State
	{
	private:
		std::string name;
		std::string value;

	public:
		State(const std::string&, const std::string&);

		const bool operator==(const State&) const;
		void appendStateNode(Element*) const;
		void print(std::string, std::string, std::string) const;
	};
}