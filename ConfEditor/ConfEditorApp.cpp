#include "ConfEditorApp.h"
#include "FileEditorLoader.h"
#include <vector>
using namespace std;

bool ConfEditorApp::OnInit()
{
	wxInitAllImageHandlers();
	window = new wxFrame(nullptr, wxID_ANY, wxT("Configuration editor"), wxDefaultPosition, wxSize(1250, 600));
	//window = new wxSplitterWindow(nullptr, wxID_ANY, wxDefaultPosition, wxSize(800, 600), wxSP_3D, wxT("Configuration editor"));
	toolbar = new Toolbar(window->CreateToolBar(wxTB_DEFAULT_STYLE, wxID_ANY, wxT("Toolbar")), wxT("Toolbar"), window);

	wxBoxSizer* splitterSizer = new wxBoxSizer(wxHORIZONTAL);
	wxSplitterWindow* splitter = new wxSplitterWindow(window, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_THIN_SASH);
	splitterSizer->Add(splitter, 1, wxEXPAND | wxALL);

	filelist = new FileList(wxT("File list"), splitter);
	//wxBoxSizer* filelistSizer = new wxBoxSizer(wxHORIZONTAL);
	//sizer->Add(toolbar);
	//filelistSizer->Add(filelist, 20, wxEXPAND | wxALL);

	fileeditor = new FileEditor(wxT("File editor"), splitter);
	//wxBoxSizer* fileeditorSizer = new wxBoxSizer(wxHORIZONTAL);
	//fileeditorSizer->Add(fileeditor, 80, wxEXPAND | wxALL);

	splitter->SplitVertically(filelist, fileeditor, window->GetRect().GetWidth()*0.25);

	window->SetSizer(splitterSizer);

	//fileeditor->HighlightText(2, 4, wxTextAttr(*wxRED));

	//vector<wxString> lines = FileLineReader::ReadFile("DeviceDebug.cfg");
	//vector<wxString> lines = FileLineReader::ReadFile("Favorites.cfg");
	//vector<wxString> lines = FileLineReader::ReadFile("SwitchMonitor.cfg");
	FileEditorLoader::LoadEditorFromFile(wxString("Favorites.cfg"), *fileeditor);

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