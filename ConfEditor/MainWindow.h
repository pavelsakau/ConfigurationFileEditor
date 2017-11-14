#ifndef MAIN_WINDOW_H_
#define MAIN_WINDOW_H_

#include <wx/wx.h>
#include <wx/frame.h>
#include <wx/splitter.h>
#include <wx/listctrl.h>
#include "FileEditor.h"
#include "Toolbar.h"

class MainWindow : public wxFrame
{
	FileEditor* fileeditor;
	Toolbar* toolbar;
	bool filesChanged;

public:
	MainWindow(wxWindow *parent,
		wxWindowID id,
		const wxString& title,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxDEFAULT_FRAME_STYLE,
		const wxString& name = wxFrameNameStr);

	void SetFileEditor(FileEditor* fileeditor);
	void SetToolbar(Toolbar* toolbar);
	void FileOpen(wxListEvent& event);
	void SetFilesChanged(const bool& flag);
	void SendFileEditorCtrlChar(char ch);

	void OnSave(wxCommandEvent& event);
	void OnReset(wxCommandEvent& event);
	void OnUndo(wxCommandEvent& event);
	void OnRedo(wxCommandEvent& event);
	void OnCut(wxCommandEvent& event);
	void OnCopy(wxCommandEvent& event);
	void OnPaste(wxCommandEvent& event);

	void OnStart(wxCommandEvent& event);
	void OnStop(wxCommandEvent& event);
	void OnRestart(wxCommandEvent& event);

	void ItemDoubleclick(wxListEvent& event);
	void ItemSelected(wxListEvent& event);
	void SavePointLeft(wxStyledTextEvent& event);
	void SavePointReached(wxStyledTextEvent& event);

	void OnClose(wxCloseEvent& event);
};

#endif