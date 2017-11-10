#ifndef FILE_EDITOR_H_
#define FILE_EDITOR_H_

#include <wx/wx.h>
#include <wx/panel.h>

class FileEditor : public wxPanel
{
private:
	wxTextCtrl* editor;

public:

	FileEditor(const wxString& title, wxWindow* parent);
};

#endif
