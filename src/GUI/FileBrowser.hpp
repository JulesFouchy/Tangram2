#pragma once

#include <string>

#include <shobjidl_core.h>

class FileBrowser {
public:
	static void Initialize();
	static void ShutDown();
	static std::string GetImageFileOpen(); // Returns an empty string if dialog is canceled
	static std::string GetImageFileSave(); // Returns an empty string if dialog is canceled
	static std::string GetFolder(); // Returns an empty string if dialog is canceled

private:
	static std::string ShowAndGetPath(IFileDialog* dialog); // TODO move this Windows-specific code away
	static void AddOptions(IFileDialog* dialog, FILEOPENDIALOGOPTIONS complementaryOptions); // TODO move this Windows-specific code away
	static void RemoveOptions(IFileDialog* dialog, FILEOPENDIALOGOPTIONS options); // TODO move this Windows-specific code away

	static IFileDialog* m_fileOpenImageDialog;
	static IFileDialog* m_fileSaveImageDialog;
	static IFileDialog* m_folderDialog;

	FileBrowser() = default;
	~FileBrowser() = default;
};