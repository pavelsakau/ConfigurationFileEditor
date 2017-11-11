#ifndef FILE_EDITOR_LOADER_H_
#define FILE_EDITOR_LOADER_H_

#include <wx/wx.h>
#include "FileEditor.h"

class FileEditorLoader
{
public:
	static void LoadEditorFromFile(const wxString& filename, FileEditor& fileeditor);
};

#endif
