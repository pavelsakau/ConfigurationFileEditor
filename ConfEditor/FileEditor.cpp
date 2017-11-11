#include "FileEditor.h"
#include "Colorer.h"
#include <vector>
using namespace std;

FileEditor::FileEditor(const wxString& title, wxWindow* parent) : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL)
{
	DefaultFont = wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxString("Courier"));

	PlainTextAttr = wxTextAttr(wxColor(0, 0, 0), *wxWHITE, DefaultFont);
	CommentTextAttr = wxTextAttr(wxColor(0, 155, 0), *wxWHITE, DefaultFont);
	IgnoreTextAttr = wxTextAttr(wxColor(0, 0, 155), *wxWHITE, DefaultFont);
	WhitespaceTextAttr = wxTextAttr(wxNullColour, *wxRED, DefaultFont);

	cutOrPasteLineNo = -1;
	settingStyle = false;

	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	editor = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_RICH2 | wxHSCROLL | wxTE_PROCESS_TAB);
	//editor = new wxStyledTextCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_RICH2 | wxHSCROLL | wxTE_PROCESS_TAB);
	//editor = new wxRichTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_RICH2 | wxHSCROLL | wxTE_PROCESS_TAB);
	editor->SetDoubleBuffered(true);
	editor->SetFont(DefaultFont);
	sizer->Add(editor, 1, wxEXPAND | wxALL);
	this->SetSizer(sizer);

	editor->Connect(wxEVT_KEY_UP, wxKeyEventHandler(FileEditor::OnKeyUp), nullptr, this);
	editor->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(FileEditor::OnKeyDown), nullptr, this);
	editor->Connect(wxEVT_TEXT_CUT, wxClipboardTextEventHandler(FileEditor::OnTextCut), nullptr, this);
	editor->Connect(wxEVT_TEXT_PASTE, wxClipboardTextEventHandler(FileEditor::OnTextPaste), nullptr, this);
	//editor->Connect(wxEVT_TEXT, wxCommandEventHandler(FileEditor::OnTextUpdate), nullptr, this);
}

void FileEditor::OnTextUpdate(wxCommandEvent& event)
{
	if (settingStyle) {
		settingStyle = false;
		event.StopPropagation();
	} else {
		event.Skip();
	}
}

void FileEditor::SaveCutOrPasteLineNo()
{
	long selectionStart, selectionEnd;
	editor->GetSelection(&selectionStart, &selectionEnd);
	long column, line;
	if (selectionStart < selectionEnd) {
		editor->PositionToXY(selectionStart, &column, &line);
	} else {
		long currentInsertionPoint = editor->GetInsertionPoint();
		editor->PositionToXY(currentInsertionPoint, &column, &line);
	}
	cutOrPasteLineNo = line;
}

void FileEditor::OnTextCut(wxClipboardTextEvent& event)
{
	SaveCutOrPasteLineNo();
	event.Skip();
}

void FileEditor::OnTextPaste(wxClipboardTextEvent& event)
{
	SaveCutOrPasteLineNo();
	event.Skip();
}

void FileEditor::OnKeyUp(wxKeyEvent &event)
{
	editor->Freeze();

	long currentInsertionPoint = editor->GetInsertionPoint();
	long column, line;
	editor->PositionToXY(currentInsertionPoint, &column, &line);

	if (event.ControlDown()) {
		if ((event.GetKeyCode() == 'x' || event.GetKeyCode() == 'X') && cutOrPasteLineNo >= 0) {
			Colorer::RecolorLines(*this, cutOrPasteLineNo, cutOrPasteLineNo);
			cutOrPasteLineNo = -1;
		}
		if ((event.GetKeyCode() == 'v' || event.GetKeyCode() == 'V') && cutOrPasteLineNo >= 0) {
			Colorer::RecolorLines(*this, cutOrPasteLineNo, line);
			cutOrPasteLineNo = -1;
		}
	} else if (event.AltDown()) {
	} else if (event.ShiftDown() && event.GetKeyCode() == WXK_TAB) {
	} else if (event.GetKeyCode() == WXK_RETURN || event.GetKeyCode() == WXK_NUMPAD_ENTER) {
		Colorer::RecolorLines(*this, line - 1, line);
	} else if (event.GetKeyCode() == WXK_DELETE || event.GetKeyCode() == WXK_NUMPAD_DELETE) {
		Colorer::RecolorLines(*this, line, line);
	} else if (event.GetKeyCode() == WXK_BACK) {
		Colorer::RecolorLines(*this, line, line);
	} else if ((event.GetKeyCode() >= 32 && event.GetKeyCode() <= 126) || event.GetKeyCode() == WXK_TAB || (event.GetKeyCode() >= WXK_NUMPAD0 && event.GetKeyCode() <= WXK_NUMPAD9) || event.GetKeyCode() == WXK_NUMPAD_SPACE ||
				event.GetKeyCode() == WXK_NUMPAD_TAB || event.GetKeyCode() == WXK_NUMPAD_EQUAL || event.GetKeyCode() == WXK_NUMPAD_MULTIPLY || event.GetKeyCode() == WXK_NUMPAD_ADD || 
				event.GetKeyCode() == WXK_NUMPAD_SEPARATOR || event.GetKeyCode() == WXK_NUMPAD_SUBTRACT || event.GetKeyCode() == WXK_NUMPAD_DECIMAL || event.GetKeyCode() == WXK_NUMPAD_DIVIDE) {
		Colorer::RecolorLines(*this, line, line);
	}

	editor->Thaw();

	event.Skip();
}

void FileEditor::OnKeyDown(wxKeyEvent &event)
{
	//event.ResumePropagation(1);
	event.Skip();
}

void FileEditor::AppendTextNoStyleSave(const wxString& text, const wxTextAttr& style)
{
	//editor->SetDefaultStyle(wxRichTextAttr(style));
	settingStyle = true;
	editor->SetDefaultStyle(style);
	editor->AppendText(text);
}

void FileEditor::AppendText(const wxString& text, const wxTextAttr& style)
{
	//wxTextAttr saveStyle = editor->GetDefaultStyle();
	AppendTextNoStyleSave(text, style);
	//editor->SetDefaultStyle(saveStyle);
}

//void FileEditor::AppendTextDefaultStyle(const wxString& text)
//{
//	editor->AppendText(text);
//}

void FileEditor::SetStyle(const long& begin, const long& end, const TextType& type)
{
	//editor->SetStyleEx(wxRichTextRange(begin, end), wxRichTextAttr(TextTypeToTextAttr(type)), wxRICHTEXT_SETSTYLE_NONE);
	settingStyle = true;
	editor->SetStyle(begin, end, TextTypeToTextAttr(type));
}

wxTextAttr FileEditor::TextTypeToTextAttr(const TextType& type)
{
	switch (type)
	{
	case TextType::PlainText:
		return PlainTextAttr;
	case TextType::Comment:
		return CommentTextAttr;
		break;
	case TextType::Ignore:
		return IgnoreTextAttr;
		break;
	case TextType::Whitespace:
		return WhitespaceTextAttr;
		break;
	};
	return PlainTextAttr;
}

void FileEditor::AppendText(const wxString& text, const TextType& type)
{
	AppendText(text, TextTypeToTextAttr(type));
}

vector<wxString> FileEditor::GetLinesContent(const long& startLine, const long& endLine)
{
	vector<wxString> lines;
	const int editorLinesCount = editor->GetNumberOfLines();
	for (int i = startLine; i <= endLine; i++)
	{
		if (i >= 0 && i < editorLinesCount) {
			lines.push_back(editor->GetLineText(i));
		}
	}
	return lines;
}

void FileEditor::SetTextStyleType(const TextType& type)
{
	switch (type)
	{
	case TextType::PlainText:
		editor->SetDefaultStyle(PlainTextAttr);
		break;
	case TextType::Comment:
		editor->SetDefaultStyle(CommentTextAttr);
		break;
	case TextType::Ignore:
		editor->SetDefaultStyle(IgnoreTextAttr);
		break;
	case TextType::Whitespace:
		editor->SetDefaultStyle(WhitespaceTextAttr);
		break;
	};
}

long FileEditor::GetNumberOfLines()
{
	return editor->GetNumberOfLines();
}

long FileEditor::XYToPosition(const long& column, const long& line)
{
	return editor->XYToPosition(column, line);
}

bool FileEditor::PositionToXY(const long& pos, long& column, long& line)
{
	return editor->PositionToXY(pos, &column, &line);
}


long FileEditor::GetLineLength(const long& lineNo)
{
	return editor->GetLineLength(lineNo);
}

void FileEditor::Freeze()
{
	editor->Freeze();
}

void FileEditor::Thaw()
{
	editor->Thaw();
}
