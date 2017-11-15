#include "Toolbar.h"
#include "BitmapHelper.h"
#include "resource.h"

Toolbar::Toolbar(wxWindow* parent, const wxString& title) : wxPanel(parent, wxID_ANY)
{
	wxBoxSizer *vbox = new wxBoxSizer(wxHORIZONTAL);

	toolbar = new wxToolBar(this, wxID_ANY);

	wxBitmap save(BitmapHelper::GetResourceById(SAVEICON), wxBITMAP_TYPE_ICO_RESOURCE);
	wxBitmap revert(BitmapHelper::GetResourceById(REVERTICON), wxBITMAP_TYPE_ICO_RESOURCE);
	wxBitmap undo(BitmapHelper::GetResourceById(UNDOICON), wxBITMAP_TYPE_ICO_RESOURCE);
	wxBitmap redo(BitmapHelper::GetResourceById(REDOICON), wxBITMAP_TYPE_ICO_RESOURCE);

	wxBitmap cut(BitmapHelper::GetResourceById(CUTICON), wxBITMAP_TYPE_ICO_RESOURCE);
	wxBitmap copy(BitmapHelper::GetResourceById(COPYICON), wxBITMAP_TYPE_ICO_RESOURCE);
	wxBitmap paste(BitmapHelper::GetResourceById(PASTEICON), wxBITMAP_TYPE_ICO_RESOURCE);

	toolbar->AddTool(wxID_SAVE, wxT("Save"), save, wxT("Save file (Ctrl+S)"));
	toolbar->AddTool(wxID_RESET, wxT("Revert"), revert, wxT("Revert file (Ctrl+R)"));
	toolbar->AddTool(wxID_UNDO, wxT("Undo"), undo, wxT("Undo changes (Ctrl+Z)"));
	toolbar->AddTool(wxID_REDO, wxT("Redo"), redo, wxT("Redo changes (Ctrl+Y)"));
	toolbar->AddTool(wxID_CUT, wxT("Cut"), cut, wxT("Cut text (Ctrl+X)"));
	toolbar->AddTool(wxID_COPY, wxT("Copy"), copy, wxT("Copy text (Ctrl+C)"));
	toolbar->AddTool(wxID_PASTE, wxT("Paste"), paste, wxT("Paste text (Ctrl+V)"));

	toolbar->Connect(wxID_SAVE, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(Toolbar::OnToolbarClick));
	toolbar->Connect(wxID_RESET, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(Toolbar::OnToolbarClick));
	toolbar->Connect(wxID_UNDO, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(Toolbar::OnToolbarClick));
	toolbar->Connect(wxID_REDO, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(Toolbar::OnToolbarClick));
	toolbar->Connect(wxID_CUT, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(Toolbar::OnToolbarClick));
	toolbar->Connect(wxID_COPY, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(Toolbar::OnToolbarClick));
	toolbar->Connect(wxID_PASTE, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(Toolbar::OnToolbarClick));

	toolbar->EnableTool(wxID_SAVE, false);
	toolbar->EnableTool(wxID_RESET, false);

	wxStaticText *txt = new wxStaticText(toolbar, wxID_ADD, "", wxDefaultPosition, wxDefaultSize);
	txt->SetForegroundColour(wxColor(*wxRED));
	toolbar->AddControl(txt);

	wxBitmap start(BitmapHelper::GetResourceById(STARTICON), wxBITMAP_TYPE_ICO_RESOURCE);
	wxBitmap stop(BitmapHelper::GetResourceById(STOPICON), wxBITMAP_TYPE_ICO_RESOURCE);
	wxBitmap restart(BitmapHelper::GetResourceById(RESTARTICON), wxBITMAP_TYPE_ICO_RESOURCE);

	toolbar->AddTool(wxID_FILE1, wxT("Start"), start, wxT("Start service"));
	toolbar->AddTool(wxID_FILE2, wxT("Stop"), stop, wxT("Stop service"));
	toolbar->AddTool(wxID_FILE3, wxT("Restart"), restart, wxT("Restart service"));

	toolbar->Connect(wxID_FILE1, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(Toolbar::OnToolbarClick));
	toolbar->Connect(wxID_FILE2, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(Toolbar::OnToolbarClick));
	toolbar->Connect(wxID_FILE3, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(Toolbar::OnToolbarClick));

	toolbar->EnableTool(wxID_FILE1, false);
	toolbar->EnableTool(wxID_FILE2, false);
	toolbar->EnableTool(wxID_FILE3, false);

	toolbar->InsertStretchableSpace(7);
	toolbar->InsertStretchableSpace(9);
	
	toolbar->Realize();
	vbox->Add(toolbar, 1, wxEXPAND | wxALL);

	SetSizer(vbox);
}

bool Toolbar::UpdateServerButtons(ServiceManager* serviceManager)
{
	if (serviceManager->QueuryServiceStatus()) {
		DWORD currentState = serviceManager->GetServiceCurrentState();
		if (currentState == SERVICE_RUNNING) {
			toolbar->EnableTool(wxID_FILE1, false);
			toolbar->EnableTool(wxID_FILE2, true);
			toolbar->EnableTool(wxID_FILE3, true);
		} else {
			toolbar->EnableTool(wxID_FILE1, true);
			toolbar->EnableTool(wxID_FILE2, false);
			toolbar->EnableTool(wxID_FILE3, false);
		}
		return true;
	}
	return false;
}

void Toolbar::ShowFilesChangedText(bool flag)
{
	wxStaticText *txt = (wxStaticText *)toolbar->FindControl(wxID_ADD);
	if (flag) {
		txt->SetLabel("Files changed");
	} else {
		txt->SetLabel("");
	}
}

void Toolbar::EnableTool(int toolid, bool enable) {
	this->toolbar->EnableTool(toolid, enable);
}

void Toolbar::OnToolbarClick(wxCommandEvent& event) {
	event.Skip();
	event.ShouldPropagate();
}
