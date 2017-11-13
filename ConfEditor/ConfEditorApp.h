#ifndef CONF_EDITOR_APP_H_
#define CONF_EDITOR_APP_H_

#include <wx/wx.h>
#include <wx/frame.h>
#include <wx/splitter.h>
#include <memory>
#include "Toolbar.h"
#include "FileList.h"
#include "FileEditor.h"
#include "MainWindow.h"

using namespace std;

class ConfEditorApp : public wxApp
{
private:

	MainWindow* window;

	Toolbar* toolbar;
	FileList* filelist;
	FileEditor* fileeditor;

	wxString cwd;
	bool isAdminMode;
public:
	bool OnInit();
	int OnExit();
	void OnMainWindowClose(wxCloseEvent& event);
	void ChooseAppMode();

	~ConfEditorApp();
};

#endif