#ifdef  __APPLE__

#include "FileBrowser.hpp"

#include "Debugging/Log.hpp"

void FileBrowser::Initialize(
	spdlog::warn("Sorry, file explorer not implemented for Mac yet ! ");
}

void FileBrowser::ShutDown(
	spdlog::warn("Sorry, file explorer not implemented for Mac yet ! ");
}

std::string FileBrowser::GetFileOpen(const char* filter) {
	spdlog::warn("Sorry, file explorer not implemented for Mac yet ! ");
	return "";
}

std::string FileBrowser::GetFileSave(const char* filter) {
	spdlog::warn("Sorry, file explorer not implemented for Mac yet ! ");
	return "";
}

std::string FileBrowser::GetFolder(const char* filter) {
	spdlog::warn("Sorry, file explorer not implemented for Mac yet ! ");
	return "";
}

#endif //  __APPLE__
