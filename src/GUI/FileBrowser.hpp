#pragma once

#include <string>

namespace FileBrowser {
	void Initialize();
	void ShutDown();
	std::string GetFileOpen(const char* filter = "All Files (*.*)\0*.*\0"); // Returns an empty string if dialog is canceled
	std::string GetFileSave(const char* filter = "All Files (*.*)\0*.*\0"); // Returns an empty string if dialog is canceled
	std::string GetFolder(); // Returns an empty string if dialog is canceled
}