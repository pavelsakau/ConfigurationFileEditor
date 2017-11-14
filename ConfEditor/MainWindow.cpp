#include "MainWindow.h"

MainWindow::MainWindow(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
	: wxFrame(parent, id, title, pos, size, style, name)
{
}

void MainWindow::OnClose(wxCloseEvent& event)
{
	wxString currentLabel = this->GetLabel();
	if (currentLabel.EndsWith("*")) {
		int res = wxMessageBox(wxT("Do you want to save changes before exit?"), "Warning", wxYES_NO);
		if (res == wxYES) {
			fileeditor->SaveFile();
		}
	}
	event.Skip();
	event.ShouldPropagate();
}

void MainWindow::SavePointLeft(wxStyledTextEvent& event)
{
	toolbar->EnableTool(wxID_SAVE, true);
	toolbar->EnableTool(wxID_RESET, true);

	wxString currentLabel = this->GetLabel();
	if (!currentLabel.EndsWith("*")) {
		this->SetLabel(currentLabel.Append("*"));
	}
}

void MainWindow::SavePointReached(wxStyledTextEvent& event)
{
	toolbar->EnableTool(wxID_SAVE, false);
	toolbar->EnableTool(wxID_RESET, false);

	wxString currentLabel = this->GetLabel();
	if (currentLabel.EndsWith("*")) {
		this->SetLabel(currentLabel.substr(0, currentLabel.length() - 1));
	}
}

void MainWindow::FileOpen(wxListEvent& event)
{
	wxString currentLabel = this->GetLabel();
	if (currentLabel.EndsWith("*")) {
		int res = wxMessageBox(wxT("Do you want to save changes for '") + event.GetText() + "'?", "Warning", wxYES_NO);
		if (res == wxYES) {
			fileeditor->SaveFile();
		}
	}
	fileeditor->LoadFile(*(wxString *)event.GetData());
}

void MainWindow::ItemSelected(wxListEvent& event)
{
	FileOpen(event);
}

void MainWindow::ItemDoubleclick(wxListEvent& event)
{
	FileOpen(event);
}

void MainWindow::SetFileEditor(FileEditor* fileeditor)
{
	this->fileeditor = fileeditor;
}

void MainWindow::SetToolbar(Toolbar* toolbar)
{
	this->toolbar = toolbar;
}

void MainWindow::OnSave(wxCommandEvent& event) {
	fileeditor->SaveFile();
}

void MainWindow::OnReset(wxCommandEvent& event) {
	fileeditor->ResetFile();
}

void MainWindow::OnUndo(wxCommandEvent& event) {
	fileeditor->Undo();
}

void MainWindow::OnRedo(wxCommandEvent& event) {
	fileeditor->Redo();
}

void MainWindow::OnCut(wxCommandEvent& event) {
	fileeditor->Cut();
}

void MainWindow::OnCopy(wxCommandEvent& event) {
	fileeditor->Copy();
}

void MainWindow::OnPaste(wxCommandEvent& event) {
	fileeditor->Paste();
}