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

	wxString parameters = wxT("");
	//for (int i = 1; i < this->argc - 1; i++) {
	//	parameters.Append(this->argv[i]).Append(" ");
	//}
	//if (this->argc > 1) {
	//	parameters.Append(this->argv[this->argc - 1]);
	//}

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

	wxInitAllImageHandlers();
	wxString title = isAdminMode ? wxT("Configuration editor (Administrator)") : wxT("Configuration editor (View-only)");
	window = new MainWindow(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(1250, 600));
	toolbar = new Toolbar(window->CreateToolBar(wxTB_DEFAULT_STYLE, wxID_ANY, wxT("Toolbar")), wxT("Toolbar"), window);

	wxBoxSizer* splitterSizer = new wxBoxSizer(wxHORIZONTAL);
	wxSplitterWindow* splitter = new wxSplitterWindow(window, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_THIN_SASH);
	splitterSizer->Add(splitter, 1, wxEXPAND | wxALL);

	filelist = new FileList(wxT("File list"), splitter);

	fileeditor = new FileEditor(wxT("File editor"), splitter);
	window->SetFileEditor(fileeditor);
	window->SetToolbar(toolbar);

	splitter->SplitVertically(filelist, fileeditor, window->GetRect().GetWidth()*0.25);

	window->SetSizer(splitterSizer);

	//fileeditor->LoadFile(wxString("Favorites.cfg"));
	filelist->LoadFilesFromDir(cwd);

	window->Connect(wxID_SAVE, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MainWindow::OnSave));
	window->Connect(wxID_RESET, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MainWindow::OnReset));
	window->Connect(wxID_UNDO, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MainWindow::OnUndo));
	window->Connect(wxID_REDO, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MainWindow::OnRedo));
	window->Connect(wxID_CUT, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MainWindow::OnCut));
	window->Connect(wxID_COPY, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MainWindow::OnCopy));
	window->Connect(wxID_PASTE, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MainWindow::OnPaste));

	window->Connect(wxID_ANY, wxEVT_LIST_ITEM_ACTIVATED, wxListEventHandler(MainWindow::ItemDoubleclick));

	window->Connect(wxID_ANY, wxEVT_STC_SAVEPOINTLEFT, wxStyledTextEventHandler(MainWindow::SavePointLeft));
	window->Connect(wxID_ANY, wxEVT_STC_SAVEPOINTREACHED, wxStyledTextEventHandler(MainWindow::SavePointReached));

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