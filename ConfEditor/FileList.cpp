#include "FileList.h"

FileList::FileList(const wxString& title, wxWindow* parent) : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL)
{
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	item_list = new wxListCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT); 	// | wxLC_NO_HEADER

	wxListItem col0;
	col0.SetId(0);
	col0.SetText(wxT("File name"));
	col0.SetWidth(350);
	item_list->InsertColumn(0, col0);

	// TODO test code
	const int item_amount = 10;
	for (int n = 0; n < item_amount; n++)
	{
		wxListItem item;
		item.SetId(n);
		item.SetText(wxString::Format(wxT("%i"), n));

		item_list->InsertItem(item);
		item_list->SetItem(n, 0, wxString::Format(wxT("Item number %i"), n));
	}

	sizer->Add(item_list, 1, wxEXPAND | wxALL);
	this->SetSizer(sizer);
	//Connect(wxID_EXIT, wxEVT_CLOSE_WINDOW, wxCommandEventHandler(FileList::OnQuit), this);
}

void FileList::OnQuit(wxCommandEvent& event)
{
	Close(true);
}