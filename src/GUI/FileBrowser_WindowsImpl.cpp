#ifdef _WIN32

//Big thanks to https://docs.microsoft.com/en-us/windows/win32/learnwin32/example--the-open-dialog-box
// https://www.codeproject.com/articles/16678/vista-goodies-in-c-using-the-new-vista-file-dialog
// https://docs.microsoft.com/en-us/previous-versions/windows/desktop/legacy/bb776913%28v=vs.85%29

#include "FileBrowser.hpp"

#include <windows.h>
#include <Commdlg.h>
#include <shlobj.h>
#include <shobjidl_core.h>
#include <atlstr.h> // includes CW2A to convert LPCWSTR to std::string

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
	COMDLG_FILTERSPEC rgSpec[] =
	{
		{ L"JPEG", L"*.jpg;*.jpeg" },
		{ L"PDF", L"*.p" },
		{ L"All Files", L"*.*" },
	};
	std::string res = "";
	IFileOpenDialog* pFileOpen;
	HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

	if (SUCCEEDED(hr)) {
		pFileOpen->SetFileTypes(3, rgSpec);
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

std::string FileBrowser::GetFileSave(const char* filter) {
	COMDLG_FILTERSPEC rgSpec[] =
	{
		{ L"JPEG", L"*.jpg;*.jpeg" },
		{ L"PDF", L"*.p" },
		{ L"All Files", L"*.*" },
	};
	std::string res = "";
	IFileSaveDialog* pFileOpen;
	HRESULT hr = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_ALL, IID_IFileSaveDialog, reinterpret_cast<void**>(&pFileOpen));

	if (SUCCEEDED(hr)) {
		pFileOpen->SetFileTypes(3, rgSpec);
		pFileOpen->SetDefaultExtension(L"ifYouSeeThisSometingWentWrong");
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
	spdlog::info("'{}'", res);
	return res;
}



std::string FileBrowser::GetFolder()
{
	std::string res = "";
	IFileOpenDialog* pFileOpen;
	HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

	if (SUCCEEDED(hr)) {
		FILEOPENDIALOGOPTIONS currOptions;
		pFileOpen->GetOptions(&currOptions);
		pFileOpen->SetOptions(FOS_PICKFOLDERS | currOptions);
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

#endif