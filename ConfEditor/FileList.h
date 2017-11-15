#ifndef FILE_LIST_H_
#define FILE_LIST_H_

#include <wx/wx.h>
#include <wx/listctrl.h>

class FileList : public wxPanel
{
private:
	wxListCtrl* item_list;
	wxArrayString files;

public:
	FileList(const wxString& title, wxWindow* parent);
	void LoadFilesFromDir(const wxString& directory);
	void OnQuit(wxCommandEvent& event);
	void OnResize(wxSizeEvent& event);
	void ItemDoubleclick(wxListEvent& event);
	void ItemSelected(wxListEvent& event);
	void SetWidthToMatchMaxLen();
	void SetWidth(int width);
	void HideHorizontalScrollbar();
};

#endif
