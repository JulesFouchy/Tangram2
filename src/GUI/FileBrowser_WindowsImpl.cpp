#ifdef _WIN32

//Big thanks to https://docs.microsoft.com/en-us/windows/win32/learnwin32/example--the-open-dialog-box
// https://www.codeproject.com/articles/16678/vista-goodies-in-c-using-the-new-vista-file-dialog
// https://docs.microsoft.com/en-us/previous-versions/windows/desktop/legacy/bb776913%28v=vs.85%29

#include "FileBrowser.hpp"

#include <atlstr.h> // includes CW2A to convert LPCWSTR to std::string

#include "Debugging/Log.hpp"

IFileDialog* FileBrowser::m_fileOpenImageDialog;
IFileDialog* FileBrowser::m_fileSaveImageDialog;
IFileDialog* FileBrowser::m_folderDialog;

void FileBrowser::Initialize() {
	// Init COM
	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
	if (FAILED(hr))
		spdlog::error("[FileBrowser::Initialize] failed");
	// Create dialog instances
	COMDLG_FILTERSPEC imageFilter[] =
	{
		{ L"PNG", L"*.png" },
		{ L"JPEG", L"*.jpg;*.jpeg" },
	};
		// Open Image File
	hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&m_fileOpenImageDialog));
	if (FAILED(hr)) spdlog::error("[FileBrowser::Initialize] failed to initialize FileOpenDialog");
	m_fileOpenImageDialog->SetFileTypes(_countof(imageFilter), imageFilter);
		// Save Image File
	hr = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_ALL, IID_IFileSaveDialog, reinterpret_cast<void**>(&m_fileSaveImageDialog));
	if (FAILED(hr)) spdlog::error("[FileBrowser::Initialize] failed to initialize FileSaveDialog");
	m_fileSaveImageDialog->SetFileTypes(_countof(imageFilter), imageFilter);
	m_fileSaveImageDialog->SetDefaultExtension(L"ifYouSeeThisSometingWentWrong");
		// Folder
	hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&m_folderDialog));
	if (FAILED(hr)) spdlog::error("[FileBrowser::Initialize] failed to initialize FolderDialog");
	AddOptions(m_folderDialog, FOS_PICKFOLDERS);
}

void FileBrowser::ShutDown() {
	m_folderDialog->Release();
	m_fileSaveImageDialog->Release();
	m_fileOpenImageDialog->Release();
	CoUninitialize();
}

std::string FileBrowser::GetImageFileOpen() {
	return ShowAndGetPath(m_fileOpenImageDialog);
}

std::string FileBrowser::GetImageFileSave() {
	return ShowAndGetPath(m_fileSaveImageDialog);
}

std::string FileBrowser::GetFolder() {
	return ShowAndGetPath(m_folderDialog);
}

std::string FileBrowser::ShowAndGetPath(IFileDialog* dialog) {
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

void FileBrowser::AddOptions(IFileDialog* dialog, FILEOPENDIALOGOPTIONS complementaryOptions) {
	FILEOPENDIALOGOPTIONS currOptions;
	dialog->GetOptions(&currOptions);
	dialog->SetOptions(complementaryOptions | currOptions);
}

void FileBrowser::RemoveOptions(IFileDialog* dialog, FILEOPENDIALOGOPTIONS options) {
	FILEOPENDIALOGOPTIONS currOptions;
	dialog->GetOptions(&currOptions);
	dialog->SetOptions((~options) & currOptions);
}

#endif