#ifndef CONF_EDITOR_APP_H_
#define CONF_EDITOR_APP_H_

#include <wx/wx.h>
#include <wx/frame.h>
#include <memory>
#include "Toolbar.h"
#include "FileList.h"
#include "FileEditor.h"

using namespace std;

class ConfEditorApp : public wxApp
{
private:

	wxFrame* window;
	Toolbar* toolbar;
	FileList* filelist;
	FileEditor* fileeditor;
public:
	bool OnInit();
	int OnExit();
	void OnMainWindowClose(wxCloseEvent& event);

	~ConfEditorApp();
};

#endif