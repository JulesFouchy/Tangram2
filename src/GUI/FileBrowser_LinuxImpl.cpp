#ifdef  __linux__

#include "FileBrowser.hpp"

void FileBrowser::Initialize(
	spdlog::warn("Sorry, file explorer not implemented for Linux yet ! ");
}

void FileBrowser::ShutDown(
	spdlog::warn("Sorry, file explorer not implemented for Linux yet ! ");
}

std::string FileBrowser::GetFileOpen(FileFilter filter) {
	spdlog::warn("Sorry, file explorer not implemented for Linux yet ! ");
	return "";
}

std::string FileBrowser::GetFileSave(FileFilter filter) {
	spdlog::warn("Sorry, file explorer not implemented for Linux yet ! ");
	return "";
}

std::string FileBrowser::GetFolder() {
	spdlog::warn("Sorry, file explorer not implemented for Linux yet ! ");
	return "";
}

#endif //  __linux__
