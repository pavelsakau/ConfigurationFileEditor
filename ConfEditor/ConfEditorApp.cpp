#include "ConfEditorApp.h"
#include "FileEditorLoader.h"
#include <vector>
#include <wx/listctrl.h>
#include <shlobj.h>
#include <shlwapi.h>
#include <objbase.h>
using namespace std;

bool ConfEditorApp::OnInit()
{
	cwd = ::wxGetCwd();

	isAdminMode = false;

	ChooseAppMode();

	wxInitAllImageHandlers();
	wxString title = isAdminMode ? wxT("Configuration editor (Administrator)") : wxT("Configuration editor (View-only)");
	window = new MainWindow(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(1250, 600));
	//toolbar = new Toolbar(window->CreateToolBar(wxTB_DEFAULT_STYLE, wxID_ANY, wxT("Toolbar")), wxT("Toolbar"), window);
	toolbar = new Toolbar(window, wxT("Toolbar"));

	serviceManager = new ServiceManager();
	serviceManager->LoadServiceName();
	if (serviceManager->OpenManagerAndService()) {
		toolbar->UpdateServerButtons(serviceManager);
		toolbarTimer = new ToolbarTimer(toolbar, serviceManager);
		toolbarTimer->StartTimer();
	}

	wxBoxSizer* splitterSizer = new wxBoxSizer(wxVERTICAL);
	splitterSizer->Add((wxWindow*)toolbar, 0, wxEXPAND | wxALIGN_TOP);
	wxSplitterWindow* splitter = new wxSplitterWindow(window, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_THIN_SASH);
	splitterSizer->Add(splitter, 1, wxEXPAND | wxALL);

	filelist = new FileList(wxT("File list"), splitter);

	fileeditor = new FileEditor(wxT("File editor"), splitter, window);
	fileeditor->SetReadOnlyMode(!isAdminMode);
	window->SetFileEditor(fileeditor);
	window->SetToolbar(toolbar);
	window->SetServiceManager(serviceManager);
	window->SetFilesChanged(false);

	splitter->SplitVertically(filelist, fileeditor, window->GetRect().GetWidth()*0.1);

	window->SetSizer(splitterSizer);

	filelist->LoadFilesFromDir(cwd);
	filelist->SetWidthToMatchMaxLen();

	window->Connect(wxID_SAVE, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MainWindow::OnSave));
	window->Connect(wxID_RESET, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MainWindow::OnReset));
	window->Connect(wxID_UNDO, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MainWindow::OnUndo));
	window->Connect(wxID_REDO, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MainWindow::OnRedo));
	window->Connect(wxID_CUT, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MainWindow::OnCut));
	window->Connect(wxID_COPY, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MainWindow::OnCopy));
	window->Connect(wxID_PASTE, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MainWindow::OnPaste));

	window->Connect(wxID_FILE1, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MainWindow::OnStart));
	window->Connect(wxID_FILE2, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MainWindow::OnStop));
	window->Connect(wxID_FILE3, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MainWindow::OnRestart));

	//window->Connect(wxID_ANY, wxEVT_LIST_ITEM_ACTIVATED, wxListEventHandler(MainWindow::ItemDoubleclick)); remove dblclick event from file open
	window->Connect(wxID_ANY, wxEVT_LIST_ITEM_SELECTED, wxListEventHandler(MainWindow::ItemSelected));

	window->Connect(wxID_ANY, wxEVT_STC_SAVEPOINTLEFT, wxStyledTextEventHandler(MainWindow::SavePointLeft));
	window->Connect(wxID_ANY, wxEVT_STC_SAVEPOINTREACHED, wxStyledTextEventHandler(MainWindow::SavePointReached));
	window->Connect(wxID_ANY, wxEVT_CLOSE_WINDOW, wxCloseEventHandler(MainWindow::OnClose));

	window->Center();
	window->Raise();
	window->Show();

	return true;
}

void ConfEditorApp::ChooseAppMode()
{
	wxString parameters = wxT("");

	if (this->argc == 1) {
		parameters = "AskForAdministrator";
		HINSTANCE result = ShellExecute(NULL, wxT("runas"), this->argv[0], parameters, cwd, SW_NORMAL);
		if (result > (HINSTANCE)32) {
			// User accepted UAC prompt (gave permission).
			// The unprivileged parent should wait for
			// the privileged child to finish.
			this->Exit();
		} else {
			// User rejected UAC prompt.
		}
	} else {
		isAdminMode = true;
	}
}

void ConfEditorApp::OnMainWindowClose(wxCloseEvent& event)
{
}

int ConfEditorApp::OnExit()
{
	return 0;
}

ConfEditorApp::~ConfEditorApp()
{
}

IMPLEMENT_APP(ConfEditorApp);