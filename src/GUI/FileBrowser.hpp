#pragma once

#include <string>

namespace FileBrowser {
	std::string openfilename(const char* filter = "All Files (*.*)\0*.*\0"); // Returns an empty string if dialog is canceled
	std::string savefilename(const char* filter = "All Files (*.*)\0*.*\0"); // Returns an empty string if dialog is canceled
}