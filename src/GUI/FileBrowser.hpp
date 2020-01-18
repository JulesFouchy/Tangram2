#pragma once

#include <string>

#include <shobjidl_core.h>

enum class FileFilter {
	None,
	Image
};

class FileBrowser {
public:
	static void Initialize();
	static void ShutDown();
	static std::string GetFileOpen(FileFilter filter = FileFilter::None); // Returns an empty string if dialog is canceled
	static std::string GetFileSave(FileFilter filter = FileFilter::None); // Returns an empty string if dialog is canceled
	static std::string GetFolder(); // Returns an empty string if dialog is canceled

private:
	FileBrowser() = delete;
	~FileBrowser() = delete;
};