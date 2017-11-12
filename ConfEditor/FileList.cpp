#include "FileList.h"
#include <wx/filefn.h>
#include <wx/dir.h>

FileList::FileList(const wxString& title, wxWindow* parent) : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL)
{
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	item_list = new wxListCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxLC_NO_HEADER);

	wxListItem col0;
	col0.SetId(0);
	col0.SetText(wxT("File name"));
	col0.SetColumn(0);
	col0.SetWidth(wxLIST_AUTOSIZE_USEHEADER);
	item_list->DeleteAllColumns();
	item_list->InsertColumn(0, col0);

	sizer->Add(item_list, 1, wxEXPAND | wxALL);
	this->SetSizer(sizer);

	this->Connect(wxEVT_ANY, wxEVT_SIZE, wxSizeEventHandler(FileList::OnResize));
	this->Connect(wxEVT_ANY, wxEVT_LIST_ITEM_ACTIVATED, wxListEventHandler(FileList::ItemDoubleclick));
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
	wxArrayString files;
	size_t filesFound = wxDir::GetAllFiles(directory, &files, wxT("*.cfg"), wxDIR_FILES);
	if (filesFound > 0 && files.size() == filesFound) {
		files.Sort();
		for (size_t i = 0; i < files.size(); i++)
		{
			wxListItem item;
			item.SetId(i);
			item.SetText(files[i]);

			item_list->InsertItem(item);
			item_list->SetItem(i, 0, files[i]);
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