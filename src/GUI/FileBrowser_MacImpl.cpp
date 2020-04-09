#ifdef  __APPLE__

#include "FileBrowser.hpp"

void FileBrowser::Initialize(
	spdlog::warn("Sorry, file explorer not implemented for Mac yet ! ");
}

void FileBrowser::ShutDown(
	spdlog::warn("Sorry, file explorer not implemented for Mac yet ! ");
}

std::string FileBrowser::GetFileOpen(FileFilter filter) {
	spdlog::warn("Sorry, file explorer not implemented for Mac yet ! ");
	return "";
}

std::string FileBrowser::GetFileSave(FileFilter filter) {
	spdlog::warn("Sorry, file explorer not implemented for Mac yet ! ");
	return "";
}

std::string FileBrowser::GetFolder() {
	spdlog::warn("Sorry, file explorer not implemented for Mac yet ! ");
	return "";
}

#endif //  __APPLE__
