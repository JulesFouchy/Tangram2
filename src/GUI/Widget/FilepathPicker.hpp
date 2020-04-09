#pragma once

#include "GUI/FileBrowser.hpp"

class GUIwidget_FilepathPicker {
public:
	GUIwidget_FilepathPicker(FileFilter filter);
	~GUIwidget_FilepathPicker() = default;

	void ShowOpenfilename();
	void ShowSavefilename();
	void ShowFolderBrowser();
	inline const std::string& getFilepath() const { return m_filepath; }
	inline void setFilepath(const std::string& filepath) { m_filepath = filepath; }
private:
	std::string m_filepath;
	FileFilter m_fileFilter;
};