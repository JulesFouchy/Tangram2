#ifdef _WIN32

//Big thanks to https://www.daniweb.com/programming/software-development/threads/159753/opening-a-filedialog

#include "FileBrowser.hpp"

#include <windows.h>
#include <Commdlg.h>

#include "Debugging/Log.hpp"
#include <iostream>

std::string FileBrowser::openfilename(const char* filter) {
	HWND owner = NULL;
	tagOFNA ofn;
	char fileName[MAX_PATH] = "";
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = owner;
	ofn.lpstrFilter = filter;
	ofn.lpstrFile = fileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = "";
	std::string fileNameStr;
	if (GetOpenFileNameA(&ofn))
		fileNameStr = fileName;
	return fileNameStr;
}

std::string FileBrowser::savefilename(const char* filter) {
	HWND owner = NULL;
	OPENFILENAME ofn;
	char fileName[MAX_PATH] = "";
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = owner;
	ofn.lpstrFilter = filter;
	ofn.lpstrFile = fileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = "";
	std::string fileNameStr;
	if (GetSaveFileNameA(&ofn))
		fileNameStr = fileName;
	return fileNameStr;
}

#endif