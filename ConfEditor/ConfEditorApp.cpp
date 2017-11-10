#include "ConfEditorApp.h"

bool ConfEditorApp::OnInit()
{
	wxInitAllImageHandlers();
	window = new wxFrame(nullptr, wxID_ANY, wxT("Configuration editor"), wxDefaultPosition, wxSize(800, 600));
	toolbar = new Toolbar(window->CreateToolBar(wxTB_DEFAULT_STYLE, wxID_ANY, wxT("Toolbar")), wxT("Toolbar"), window);

	filelist = new FileList(wxT("File list"), window);
	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	sizer->Add(toolbar);

	sizer->Add(filelist, 20, wxEXPAND | wxLEFT);

	fileeditor = new FileEditor(wxT("File editor"), window);
	sizer->Add(fileeditor, 80, wxEXPAND | wxRIGHT);

	window->SetSizer(sizer);
	window->Show();

	//window->Connect(wxEVT_CLOSE_WINDOW, wxCloseEventHandler(ConfEditorApp::OnMainWindowClose), nullptr, this);
	//this->Bind(wxEVT_CLOSE_WINDOW, wxCloseEventHandler(ConfEditorApp::OnMainWindowClose), );

	return true;
}

void ConfEditorApp::OnMainWindowClose(wxCloseEvent& event)
{
	//window->Close(true);
	//this->OnExit();
	//this->ExitMainLoop();
}

int ConfEditorApp::OnExit()
{
	return 0;
	//window->Close(true);
}

ConfEditorApp::~ConfEditorApp()
{
}

IMPLEMENT_APP(ConfEditorApp);