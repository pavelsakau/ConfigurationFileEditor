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

	editor->StyleSetBackground(wxSTC_C_WORD2, wxColour(255,182,193)); // multiple spaces

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

void FileEditor::LoadFile(const wxString& filename)
{
	// TODO check for errors!
	this->filename = filename;
	editor->SetReadOnly(false);
	editor->ClearAll();
	editor->LoadFile(filename);
	editor->SetReadOnly(isReadOnly);

	SetTopWindowTitle(filename);
}

void FileEditor::SaveFile()
{
	// TODO check for errors!
	if (filename.length() > 0) {
		wxFileName fn_cfg(filename);
		wxFileName fn_bak(fn_cfg);
		fn_bak.SetExt("bak");
		bool bakFileRenamed = ::wxRenameFile(fn_cfg.GetFullPath(), fn_bak.GetFullPath(), true);
		if (!bakFileRenamed) {
			wxMessageBox("Can't copy '" + fn_cfg.GetFullPath() + "' content to '" + fn_bak.GetFullPath() + "'", "Error", wxOK, this); 
			// TODO write to log
		}
		editor->SaveFile(fn_cfg.GetFullPath());
	}
}

void FileEditor::ResetFile()
{
	// TODO check for errors!
	if (filename.length() > 0) {
		wxFileName fn_cfg(filename);
		wxFileName fn_bak(fn_cfg);
		fn_bak.SetExt("bak");
		if (fn_bak.FileExists()) {
			editor->LoadFile(fn_bak.GetFullPath());
		} else {
			wxMessageBox("Backup file '" + fn_bak.GetFullPath() + "' does not exist.", "Error", wxOK, this); 
			// TODO write to log
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
		if ((event.GetKeyCode() == 's' || event.GetKeyCode() == 'S')) {
			SaveFile();
			return;
		} else if ((event.GetKeyCode() == 'r' || event.GetKeyCode() == 'R')) {
			ResetFile();
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
