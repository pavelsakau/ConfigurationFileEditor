#include "Toolbar.h"
#include "BitmapHelper.h"
#include "resource.h"

Toolbar::Toolbar(wxToolBar* toolbar, const wxString& title, wxWindow* parent) : wxFrame(parent, wxID_ANY, title, wxDefaultPosition, wxDefaultSize)
{
	this->toolbar = toolbar;

	wxBitmap save(BitmapHelper::GetResourceById(SAVEICON), wxBITMAP_TYPE_ICO_RESOURCE);
	wxBitmap revert(BitmapHelper::GetResourceById(REVERTICON), wxBITMAP_TYPE_ICO_RESOURCE);
	wxBitmap undo(BitmapHelper::GetResourceById(UNDOICON), wxBITMAP_TYPE_ICO_RESOURCE);
	wxBitmap redo(BitmapHelper::GetResourceById(REDOICON), wxBITMAP_TYPE_ICO_RESOURCE);

	wxBitmap cut(BitmapHelper::GetResourceById(CUTICON), wxBITMAP_TYPE_ICO_RESOURCE);
	wxBitmap copy(BitmapHelper::GetResourceById(COPYICON), wxBITMAP_TYPE_ICO_RESOURCE);
	wxBitmap paste(BitmapHelper::GetResourceById(PASTEICON), wxBITMAP_TYPE_ICO_RESOURCE);

	wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);

	this->toolbar->AddTool(wxID_SAVE, wxT("Save"), save, wxT("Save file (Ctrl+S)"));
	this->toolbar->AddTool(wxID_RESET, wxT("Revert"), revert, wxT("Revert file (Ctrl+R)"));
	this->toolbar->AddTool(wxID_UNDO, wxT("Undo"), undo, wxT("Undo changes (Ctrl+Z)"));
	this->toolbar->AddTool(wxID_REDO, wxT("Redo"), redo, wxT("Redo changes (Ctrl+Y)"));
	this->toolbar->AddTool(wxID_CUT, wxT("Cut"), cut, wxT("Cut text (Ctrl+X)"));
	this->toolbar->AddTool(wxID_COPY, wxT("Copy"), copy, wxT("Copy text (Ctrl+C)"));
	this->toolbar->AddTool(wxID_PASTE, wxT("Paste"), paste, wxT("Paste text (Ctrl+V)"));

	this->toolbar->Connect(wxID_SAVE, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(Toolbar::OnToolbarClick));
	this->toolbar->Connect(wxID_RESET, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(Toolbar::OnToolbarClick));
	this->toolbar->Connect(wxID_UNDO, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(Toolbar::OnToolbarClick));
	this->toolbar->Connect(wxID_REDO, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(Toolbar::OnToolbarClick));
	this->toolbar->Connect(wxID_CUT, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(Toolbar::OnToolbarClick));
	this->toolbar->Connect(wxID_COPY, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(Toolbar::OnToolbarClick));
	this->toolbar->Connect(wxID_PASTE, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(Toolbar::OnToolbarClick));

	this->toolbar->EnableTool(wxID_SAVE, false);
	this->toolbar->EnableTool(wxID_RESET, false);
	
	this->toolbar->Realize();

	vbox->Add(this->toolbar, 0, wxEXPAND);

	SetSizer(vbox);
}

void Toolbar::OnQuit(wxCommandEvent& event) {
	Close(true);
}

void Toolbar::EnableTool(int toolid, bool enable) {
	this->toolbar->EnableTool(toolid, enable);
}

void Toolbar::OnToolbarClick(wxCommandEvent& event) {
	event.Skip();
	event.ShouldPropagate();
}
