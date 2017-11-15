#ifndef TOOLBAR_H_
#define TOOLBAR_H_

#include <wx/wx.h>
#include <wx/frame.h>
#include <wx/toolbar.h>
#include "ServiceManager.h"

using namespace std;

class Toolbar : public wxPanel
{
private:
	wxToolBar* toolbar;
	wxToolBar* rightSideToolbar;
public:
	Toolbar(wxWindow* parent, const wxString& title);

	void OnToolbarClick(wxCommandEvent& event);
	void EnableTool(int toolid, bool enable);
	void ShowFilesChangedText(bool flag);
	bool UpdateServerButtons(ServiceManager* serviceManager);
};

#endif
