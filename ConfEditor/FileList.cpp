#include "FileList.h"
#include <wx/filefn.h>
#include <wx/dir.h>
#include <wx/filename.h>
#include <wx/splitter.h>
#include <wx/settings.h>

FileList::FileList(const wxString& title, wxWindow* parent) : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL)
{
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	item_list = new wxListCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxLC_NO_HEADER);

	wxListItem col0;
	col0.SetId(0);
	col0.SetText(wxT("File name"));
	col0.SetColumn(0);
	col0.SetWidth(wxLIST_AUTOSIZE);
	item_list->DeleteAllColumns();
	item_list->InsertColumn(0, col0);

	sizer->Add(item_list, 1, wxEXPAND | wxALL);

	this->SetSizer(sizer);

	this->Connect(wxEVT_ANY, wxEVT_SIZE, wxSizeEventHandler(FileList::OnResize));
	this->Connect(wxEVT_ANY, wxEVT_LIST_ITEM_ACTIVATED, wxListEventHandler(FileList::ItemDoubleclick));
	this->Connect(wxEVT_ANY, wxEVT_LIST_ITEM_SELECTED, wxListEventHandler(FileList::ItemSelected));
}

void FileList::HideHorizontalScrollbar()
{
	//wxMessageBox(wxString::Format("%d %d %d %d", item_list->HasScrollbar(wxVERTICAL), item_list->HasScrollbar(wxHORIZONTAL), this->HasScrollbar(wxVERTICAL), this->HasScrollbar(wxHORIZONTAL)));
	//item_list->SetScrollbar(wxVERTICAL, 0, 0, 0);
	//item_list->SetScrollbar(wxHORIZONTAL, 0, 0, 0);

	//this->SetScrollbar(wxVERTICAL, 0, 0, 0);
	//this->SetScrollbar(wxHORIZONTAL, 0, 0, 0);
}

void FileList::SetWidthToMatchMaxLen()
{
	wxClientDC dc(this);
	dc.SetFont(item_list->GetFont());

	int maxItemWidth = 0;
	for (int i = 0; i < item_list->GetItemCount(); i++)
	{
		int width = dc.GetTextExtent(item_list->GetItemText(i).Append("buf")).GetWidth();
		if (width > maxItemWidth) {
			maxItemWidth = width;
		}
	}

	((wxSplitterWindow *)this->GetParent())->SetSashPosition(maxItemWidth + wxSystemSettings::GetMetric(wxSYS_VSCROLL_X));
}

void FileList::ItemSelected(wxListEvent& event)
{
	event.Skip();
	event.ShouldPropagate();
}

void FileList::ItemDoubleclick(wxListEvent& event)
{
	event.Skip();
	event.ShouldPropagate();
}

void FileList::SetWidth(int width)
{
	item_list->SetColumnWidth(0, width);
	item_list->Refresh();
}

void FileList::OnResize(wxSizeEvent& event)
{
	SetWidth(event.GetSize().GetWidth() - wxSystemSettings::GetMetric(wxSYS_VSCROLL_X));
	event.Skip();
}

void FileList::LoadFilesFromDir(const wxString& directory)
{
	if (!item_list->DeleteAllItems()) {
	}
	files.Clear();
	size_t filesFound = wxDir::GetAllFiles(directory, &files, wxT("*.cfg"), wxDIR_FILES);
	if (filesFound > 0 && files.size() == filesFound) {
		files.Sort();
		for (size_t i = 0; i < files.size(); i++)
		{
			wxFileName fileName = files[i];

			wxListItem item;
			item.SetId(i);
			item.SetText(fileName.GetFullName());
			item.SetData(&files[i]);

			item_list->InsertItem(item);
		}
	} else {
		// no files found?
	}
}

void FileList::OnQuit(wxCommandEvent& event)
{
	Close(true);
}