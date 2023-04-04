#pragma once

#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
#include <algorithm>

#include <conio.h>

#import <msxml6.dll> no_auto_exclude

namespace wowsmod
{
	using xml = ::MSXML2::IXMLDOMDocumentPtr;
	using node = ::MSXML2::IXMLDOMNodePtr;
	using element = ::MSXML2::IXMLDOMElementPtr;
	using node_list = ::MSXML2::IXMLDOMNodeListPtr;

	const int version = 5;
}

#include "utils.hpp"
#include "state.hpp"
#include "crew.hpp"
#include "external_event.hpp"
#include "mod.hpp"