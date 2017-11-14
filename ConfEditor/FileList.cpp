#include "FileList.h"
#include <wx/filefn.h>
#include <wx/dir.h>
#include <wx/filename.h>
#include <wx/splitter.h>

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

	((wxSplitterWindow *)this->GetParent())->SetSashPosition(maxItemWidth);
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

void FileList::OnResize(wxSizeEvent& event)
{
	item_list->SetColumnWidth(0, event.GetSize().GetWidth());
	item_list->Refresh();
	event.Skip();
}

void FileList::LoadFilesFromDir(const wxString& directory)
{
	if (!item_list->DeleteAllItems()) {
		// TODO show message and log it
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
			//item_list->SetItem(i, 0, fileName.GetFullName());
		}
		item_list->SetColumnWidth(0, item_list->GetSize().GetWidth());
		item_list->Refresh();
	} else {
		//TODO show message and log it
	}
}

void FileList::OnQuit(wxCommandEvent& event)
{
	Close(true);
}