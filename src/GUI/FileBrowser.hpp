#pragma once

#include <string>

#include <shobjidl_core.h>

class FileBrowser {
public:
	static void Initialize();
	static void ShutDown();
	static std::string GetFileOpen(const char* filter = "All Files (*.*)\0*.*\0"); // Returns an empty string if dialog is canceled
	static std::string GetFileSave(const char* filter = "All Files (*.*)\0*.*\0"); // Returns an empty string if dialog is canceled
	static std::string GetFolder(); // Returns an empty string if dialog is canceled
private:
	static std::string GetPathAndFreeDialog(IFileDialog* dialog); // TODO move this Windows-specific code away
	static void AddOptions(IFileDialog* dialog, FILEOPENDIALOGOPTIONS complementaryOptions); // TODO move this Windows-specific code away

	FileBrowser() = default;
	~FileBrowser() = default;
};