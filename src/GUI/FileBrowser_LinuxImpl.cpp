#ifdef  __linux__

#include "FileBrowser.hpp"

#include "Debugging/Log.hpp"

std::string FileBrowser::openfilename(const char* filter) {
	spdlog::warn("Sorry, file explorer not implemented for Linux yet ! ");
	return "";
}

std::string FileBrowser::savefilename(const char* filter) {
	spdlog::warn("Sorry, file explorer not implemented for Linux yet ! ");
	return "";
}

#endif //  __linux__
