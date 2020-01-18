#ifdef _WIN32

//Big thanks to https://docs.microsoft.com/en-us/windows/win32/learnwin32/example--the-open-dialog-box

#include "FileBrowser.hpp"

#include <windows.h>
#include <Commdlg.h>
#include <shlobj.h>
#include <shobjidl_core.h>
#include <atlstr.h>

#include "Debugging/Log.hpp"
#include <iostream>

void FileBrowser::Initialize() {
	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
	if (FAILED(hr))
		spdlog::error("[FileBrowser::Initialize] failed");
}

void FileBrowser::ShutDown() {
	CoUninitialize();
}

std::string FileBrowser::GetFileOpen(const char* filter) {
	std::string res = "";
	IFileOpenDialog* pFileOpen;
	HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

	if (SUCCEEDED(hr)) {
		hr = pFileOpen->Show(NULL);

		if (SUCCEEDED(hr)) {
			IShellItem* pItem;
			hr = pFileOpen->GetResult(&pItem);
			if (SUCCEEDED(hr))
			{
				PWSTR pszFilePath;
				hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

				if (SUCCEEDED(hr))
				{
					res = CW2A(pszFilePath);
					CoTaskMemFree(pszFilePath);
				}
				pItem->Release();
			}
			else {
				spdlog::error("[FileBrowser::GetFileOpen] failed1");
			}
		}
		pFileOpen->Release();
	}
	else {
		spdlog::error("[FileBrowser::GetFileOpen] failed0");
	}
	return res;
}

std::string FileBrowser::GetFolder()
{
	TCHAR path[MAX_PATH];
	BROWSEINFO bi = { 0 };
	bi.lpszTitle = ("All Folders Automatically Recursed.");
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
	bi.lParam = (LPARAM)"C:/Dev/Tangram2";
	LPITEMIDLIST pidl = SHBrowseForFolder(&bi);

	if (pidl != 0)
	{
		// get the name of the folder and put it in path
		SHGetPathFromIDList(pidl, path);

		//Set the current directory to path
		//SetCurrentDirectory(path);

		// free memory used
		IMalloc* imalloc = 0;
		if (SUCCEEDED(SHGetMalloc(&imalloc)))
		{
			imalloc->Free(pidl);
			imalloc->Release();
		}
		return path;
	}
	return "";
}

std::string FileBrowser::GetFileSave(const char* filter) {
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