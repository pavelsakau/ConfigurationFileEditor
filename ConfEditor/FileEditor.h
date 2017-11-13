#ifndef FILE_EDITOR_H_
#define FILE_EDITOR_H_

#include <wx/wx.h>
#include <wx/panel.h>
#include <vector>
#include <wx/textctrl.h>
#include <wx/richtext/richtextctrl.h>
#include <wx/stc/stc.h>

using namespace std;

class FileEditor : public wxPanel
{
private:
	wxWindow* topWindow;

	wxStyledTextCtrl* editor;

	wxFont DefaultFont;

	long cutOrPasteLineNo;

	wxString filename;

	bool isReadOnly;

public:

	FileEditor(const wxString& title, wxWindow* parent, wxWindow* topWindow);

	void OnKeyUp(wxKeyEvent &event);
	void OnKeyDown(wxKeyEvent &event);

	void SetText(const wxString& text);
	void SetReadOnlyMode(bool mode);
	void SetTopWindowTitle(const wxString& filename);

	void Undo();
	void Redo();
	void Cut();
	void Copy();
	void Paste();

	void LoadFile(const wxString& filename);
	void SaveFile();
	void ResetFile();

	void SavePointLeft(wxStyledTextEvent& event);
	void SavePointReached(wxStyledTextEvent& event);
};

#endif
