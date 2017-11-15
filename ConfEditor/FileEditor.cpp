#include "FileEditor.h"
#include "Colorer.h"
#include <vector>

#include <ILexer.h>
#include <LexerModule.h>
#include <Catalogue.h>
#include "FileLineReader.h"
#include <wx/filename.h>
#include <wx/filefn.h>

using namespace std;

extern LexerModule lmConfEditor;

FileEditor::FileEditor(const wxString& title, wxWindow* parent, wxWindow* topWindow) : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL), topWindow(topWindow)
{
	DefaultFont = wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxString("Courier"));

	cutOrPasteLineNo = -1;
	filename = wxEmptyString;
	isReadOnly = false;

	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	editor = new wxStyledTextCtrl(this, wxID_ANY);

	//editor->StyleClearAll();

	editor->StyleSetFont(wxSTC_STYLE_DEFAULT, DefaultFont);
	editor->SetFont(DefaultFont);

	Catalogue::AddLexerModule(&lmConfEditor);

	editor->SetLexer(lmConfEditor.GetLanguage());

	editor->StyleSetForeground (wxSTC_C_COMMENT, wxColour(143,188,143)); // ;comment
	editor->StyleSetForeground (wxSTC_C_COMMENTDOC, wxColour(0,191,255)); // #IGNORE

	editor->StyleSetBackground(wxSTC_C_WORD2, wxColour(255,240,245)); // multiple spaces

	sizer->Add(editor, 1, wxEXPAND | wxALL);
	this->SetSizer(sizer);

	editor->Connect(wxID_ANY, wxEVT_STC_SAVEPOINTLEFT, wxStyledTextEventHandler(FileEditor::SavePointLeft), nullptr, this);
	editor->Connect(wxID_ANY, wxEVT_STC_SAVEPOINTREACHED, wxStyledTextEventHandler(FileEditor::SavePointReached), nullptr, this);

	editor->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(FileEditor::OnKeyDown), nullptr, this);
	//editor->Connect(wxEVT_KEY_UP, wxKeyEventHandler(FileEditor::OnKeyUp), nullptr, this);
}

void FileEditor::SetReadOnlyMode(bool mode)
{
	isReadOnly = mode;
}

void FileEditor::SavePointLeft(wxStyledTextEvent& event)
{
	event.Skip();
	event.ShouldPropagate();
}

void FileEditor::SavePointReached(wxStyledTextEvent& event)
{
	event.Skip();
	event.ShouldPropagate();
}

void FileEditor::SetTopWindowTitle(const wxString& filename)
{
	wxString buf(topWindow->GetLabel());
	buf = buf.substr(0, buf.Find(')') + 1) + wxT(" ") ;

	wxFileName fname(filename);

	topWindow->SetLabel(buf + fname.GetFullName());
}

wxString FileEditor::GetFilename()
{
	if (filename.length() > 0) {
		wxFileName fn_cfg(filename);
		return fn_cfg.GetFullName();
	} else {
		return filename;
	}
}

void FileEditor::LoadFile(const wxString& filename)
{
	this->filename = filename;
	editor->SetReadOnly(false);
	editor->ClearAll();
	editor->LoadFile(filename);
	editor->SetReadOnly(isReadOnly);

	SetTopWindowTitle(filename);
}

void FileEditor::SetSaveSuccess(const bool& flag)
{
	saveOpSuccess = flag;
}

bool FileEditor::GetSaveSuccess()
{
	return saveOpSuccess;
}

bool FileEditor::SaveFile()
{
	if (filename.length() > 0) {
		wxFileName fn_cfg(filename);
		wxFileName fn_bak(fn_cfg);
		fn_bak.SetExt("bak");
		bool bakFileRenamed = ::wxRenameFile(fn_cfg.GetFullPath(), fn_bak.GetFullPath(), true);
		if (!bakFileRenamed) {
			wxMessageBox("Can't copy '" + fn_cfg.GetFullPath() + "' content to '" + fn_bak.GetFullPath() + "'", "Error", wxOK, this); 
		}
		editor->SaveFile(fn_cfg.GetFullPath());
		return true;
	}
	return false;
}

void FileEditor::ResetFile()
{
	if (filename.length() > 0) {
		wxFileName fn_cfg(filename);
		wxFileName fn_bak(fn_cfg);
		fn_bak.SetExt("bak");
		if (!fn_bak.FileExists() && fn_cfg.FileExists()) {
			editor->LoadFile(fn_cfg.GetFullPath());
		} else if (!fn_bak.FileExists() && !fn_cfg.FileExists()) {
			wxMessageBox("File '" + fn_cfg.GetFullPath() + "' does not exist.", "Error", wxOK, this);
		} else if (fn_bak.FileExists()) {
			bool bakFileRenamed = ::wxRenameFile(fn_bak.GetFullPath(), fn_cfg.GetFullPath(), true);
			if (!bakFileRenamed) {
				wxMessageBox("Can't copy '" + fn_bak.GetFullPath() + "' content to '" + fn_cfg.GetFullPath() + "'", "Error", wxOK, this); 
			}
			editor->LoadFile(fn_cfg.GetFullPath());
		} else {
			wxMessageBox("Backup file '" + fn_bak.GetFullPath() + "' does not exist.", "Error", wxOK, this); 
		}
	}
}

void FileEditor::SetText(const wxString& text)
{
	editor->SetText(text);
}

void FileEditor::OnKeyUp(wxKeyEvent &event)
{
}

void FileEditor::OnKeyDown(wxKeyEvent &event)
{
	if (event.ControlDown()) {
		wxCommandEvent ev;
		ev.SetEventType(wxEVT_COMMAND_TOOL_CLICKED);
		if ((event.GetKeyCode() == 's' || event.GetKeyCode() == 'S')) {
			ev.SetId(wxID_SAVE);
			this->AddPendingEvent((const wxEvent&)ev);
			return;
		} else if ((event.GetKeyCode() == 'r' || event.GetKeyCode() == 'R')) {
			ev.SetId(wxID_RESET);
			this->AddPendingEvent((const wxEvent&)ev);
			return;
		} 
	}
	event.Skip();
}

void FileEditor::Undo()
{
	editor->Undo();
}

void FileEditor::Redo()
{
	editor->Redo();
}

void FileEditor::Cut()
{
	editor->Cut();
}

void FileEditor::Copy()
{
	editor->Copy();
}

void FileEditor::Paste()
{
	editor->Paste();
}
