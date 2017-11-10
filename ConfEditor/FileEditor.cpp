#include "FileEditor.h"

FileEditor::FileEditor(const wxString& title, wxWindow* parent) : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize)
{
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	editor = new wxTextCtrl(this, wxID_ANY, "sample", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_RICH2);
	sizer->Add(editor, 1, wxEXPAND | wxALL);
	this->SetSizer(sizer);
}