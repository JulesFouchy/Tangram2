#ifdef _WIN32

//Big thanks to https://docs.microsoft.com/en-us/windows/win32/learnwin32/example--the-open-dialog-box
// https://www.codeproject.com/articles/16678/vista-goodies-in-c-using-the-new-vista-file-dialog
// https://docs.microsoft.com/en-us/previous-versions/windows/desktop/legacy/bb776913%28v=vs.85%29

#include "FileBrowser.hpp"
#include <atlstr.h> // includes CW2A to convert LPCWSTR to std::string
#include "Debugging/Log.hpp"


void FileBrowser::Initialize() {
	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
	if (FAILED(hr))
		spdlog::error("[FileBrowser::Initialize] failed");
}

void FileBrowser::ShutDown() {
	CoUninitialize();
}

static std::string WinFileBrowser_ShowAndGetPath(IFileDialog* dialog);
static void WinFileBrowser_AddOptions(IFileDialog* dialog, FILEOPENDIALOGOPTIONS complementaryOptions);
static void WinFileBrowser_RemoveOptions(IFileDialog* dialog, FILEOPENDIALOGOPTIONS options);

std::string FileBrowser::GetFileOpen(FileFilter filter) {
	std::string res = "";
	// Create Open File dialog
	IFileDialog* dialog;
	HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&dialog));
	if (FAILED(hr)) 
		spdlog::error("[FileBrowser::GetFileOpen] failed");
	else {
	// Add filter
		switch (filter)
		{
		case FileFilter::None:
			break;
		case FileFilter::Image:
			COMDLG_FILTERSPEC imageOpenFilter[] =
			{
				{ L"Image", L"*.png;*.jpg;*.jpeg" },
				{ L"PNG", L"*.png" },
				{ L"JPEG", L"*.jpg;*.jpeg" },
			};
			dialog->SetFileTypes(_countof(imageOpenFilter), imageOpenFilter);
			break;
		}
	// Show browser
		res = WinFileBrowser_ShowAndGetPath(dialog);
	}
	return res;
}

std::string FileBrowser::GetFileSave(FileFilter filter) {
	std::string res = "";
	// Create Save File dialog
	IFileDialog* dialog;
	HRESULT hr = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_ALL, IID_IFileSaveDialog, reinterpret_cast<void**>(&dialog));
	if (FAILED(hr))
		spdlog::error("[FileBrowser::GetFileSave] failed");
	else {
	// Add filter
		switch (filter)
		{
		case FileFilter::None:
			break;
		case FileFilter::Image:
			COMDLG_FILTERSPEC imageSaveFilter[] =
			{
				{ L"PNG", L"*.png" },
				{ L"JPEG", L"*.jpg;*.jpeg" },
			};
			dialog->SetFileTypes(_countof(imageSaveFilter), imageSaveFilter);
			break;
		}
	// Enable auto fill of extension
	dialog->SetDefaultExtension(L"ifYouSeeThisSometingWentWrongInFileBrowserGetFileSave");
	// Show browser
		res = WinFileBrowser_ShowAndGetPath(dialog);
	}
	return res;
}

std::string FileBrowser::GetFolder() {
	std::string res = "";
	// Create Browser dialog
	IFileDialog* dialog;
	HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&dialog));
	if (FAILED(hr))
		spdlog::error("[FileBrowser::GetFolder] failed");
	else {
		WinFileBrowser_AddOptions(dialog, FOS_PICKFOLDERS);
	// Show browser
		res = WinFileBrowser_ShowAndGetPath(dialog);
	}
	return res;
}

static std::string WinFileBrowser_ShowAndGetPath(IFileDialog* dialog) {
	std::string res = "";
	HRESULT hr = dialog->Show(NULL);

	if (SUCCEEDED(hr)) {
		IShellItem* pItem;
		hr = dialog->GetResult(&pItem);
		if (SUCCEEDED(hr))
		{
			PWSTR pszFilePath;
			hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

			if (SUCCEEDED(hr))
			{
				res = CW2A(pszFilePath);
				CoTaskMemFree(pszFilePath);
			}
			else {
				spdlog::error("[FileBrowser::ShowAndGetPath] failed1");
			}
			pItem->Release();
		}
		else {
			spdlog::error("[FileBrowser::ShowAndGetPath] failed0");
		}
	}
	return res;
}

static void WinFileBrowser_AddOptions(IFileDialog* dialog, FILEOPENDIALOGOPTIONS complementaryOptions) {
	FILEOPENDIALOGOPTIONS currOptions;
	dialog->GetOptions(&currOptions);
	dialog->SetOptions(complementaryOptions | currOptions);
}

static void WinFileBrowser_RemoveOptions(IFileDialog* dialog, FILEOPENDIALOGOPTIONS options) {
	FILEOPENDIALOGOPTIONS currOptions;
	dialog->GetOptions(&currOptions);
	dialog->SetOptions((~options) & currOptions);
}

#endif