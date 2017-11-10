#ifndef TOOLBAR_H_
#define TOOLBAR_H_

#include <wx/wx.h>
#include <wx/frame.h>
#include <wx/toolbar.h>

using namespace std;

class Toolbar : public wxFrame
{
private:
	wxToolBar* toolbar;
public:
	Toolbar(wxToolBar* toolbar, const wxString& title, wxWindow* parent);

	void OnQuit(wxCommandEvent& event);
};

#endif
