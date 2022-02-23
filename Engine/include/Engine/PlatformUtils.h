#pragma once

#include <string>

namespace cee
{
	namespace engine
	{
		namespace FileDialogs
		{
			/**
			  * Filter should be a semicolon seperated list.
			  * Returns empty sting if dialog is cancelled.
			  */
			std::string OpenFile(const char* filter = "\0", const char* title = "Open File — Engine");
			std::string SaveFile(const char* filter = "\0", const char* title = "Save File — Engine");
		}
	}
}
