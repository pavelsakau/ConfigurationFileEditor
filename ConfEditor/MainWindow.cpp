#include "MainWindow.h"
#include <wx/dialog.h>
#include <thread>
#include <chrono>

MainWindow::MainWindow(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
	: wxFrame(parent, id, title, pos, size, style, name)
{
}

void MainWindow::SetFilesChanged(const bool& flag)
{
	filesChanged = flag;
	toolbar->ShowFilesChangedText(filesChanged);
}

void MainWindow::OnClose(wxCloseEvent& event)
{
	wxString currentLabel = this->GetLabel();
	if (currentLabel.EndsWith("*")) {
		int res = wxMessageBox(wxT("Do you want to save changes before exit?"), "Warning", wxYES_NO);
		if (res == wxYES) {
			OnSave(wxCommandEvent());
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
		wxString currentFilename = fileeditor->GetFilename();
		if (currentFilename.length() > 0) {
			int res = wxMessageBox(wxT("Do you want to save changes for '") + currentFilename + "'?", "Warning", wxYES_NO);
			if (res == wxYES) {
				OnSave(wxCommandEvent());
			}
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

void MainWindow::SetServiceManager(ServiceManager* serviceManager)
{
	this->serviceManager = serviceManager;
}

void MainWindow::SendFileEditorCtrlChar(char ch)
{
	wxKeyEvent ev;
	ev.SetControlDown(true);
	ev.m_keyCode = ch;
	fileeditor->OnKeyDown(ev);
}

void MainWindow::OnSave(wxCommandEvent& event) {
	SetFilesChanged(fileeditor->SaveFile());
}

void MainWindow::OnReset(wxCommandEvent& event) {
	if (wxMessageBox(wxT("Are you sure?"), "Revert", wxYES_NO) == wxYES) {
		fileeditor->ResetFile();
	}
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

void MainWindow::ProgressUpdateThread(wxProgressDialog* dialog)
{
	int progress = 0;
	for (int i = 0; i < 100; i++)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(120)); // it should be 300 for 30s default service startup time, but for user expirience it is better to be lower
		if (progressBarUpdateStop) {
			return;
		}
		dialog->Update(i);
	}
}

void MainWindow::StartServiceThread(wxProgressDialog* startDialog)
{
	if (!serviceManager->startService()) {
		wxMessageBox(wxT("Service startup error/timeout"));
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	startDialog->Close();
}

void MainWindow::StopServiceThread(wxProgressDialog* stopDialog)
{
	if (!serviceManager->StopService()) {
		wxMessageBox(wxT("Service stopping error/timeout"));
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	stopDialog->Close();
}

void MainWindow::OnStartStopProcessing(wxCommandEvent& event, bool setFilesChanged, wxString dialogTitle, wxString dialogText, void (MainWindow::*threadMethod)(wxProgressDialog*))
{
	std::lock_guard<std::mutex> guard(serviceManager->m);
	if (setFilesChanged) {
		SetFilesChanged(false);
	}
	wxProgressDialog* dialog = new wxProgressDialog(dialogTitle, dialogText, 100, this, wxPD_APP_MODAL);
	std::thread t1(threadMethod, this, dialog);
	progressBarUpdateStop = false;
	std::thread t2(&MainWindow::ProgressUpdateThread, this, dialog);
	dialog->CenterOnParent();
	dialog->ShowModal();
	t1.join();
	progressBarUpdateStop = true;
	t2.join();
	dialog->Destroy();
	toolbar->UpdateServerButtons(serviceManager);
}

void MainWindow::OnStart(wxCommandEvent& event) {
	OnStartStopProcessing(event, true, "Service start", "Service is starting now", &MainWindow::StartServiceThread);
}

void MainWindow::OnStop(wxCommandEvent& event) {
	OnStartStopProcessing(event, false, "Service stop", "Service is stopping now", &MainWindow::StopServiceThread);
}

void MainWindow::OnRestart(wxCommandEvent& event) {
	OnStop(event);
	OnStart(event);
}
