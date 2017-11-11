#ifndef FILE_EDITOR_H_
#define FILE_EDITOR_H_

#include <wx/wx.h>
#include <wx/panel.h>
#include <vector>
#include <wx/textctrl.h>
#include <wx/richtext/richtextctrl.h>
#include <wx/stc/stc.h>

using namespace std;

class FileEditor : public wxPanel
{
private:
	wxTextCtrl* editor;
	//wxStyledTextCtrl* editor;
	//wxRichTextCtrl* editor;

	wxFont DefaultFont;

	wxTextAttr PlainTextAttr;
	wxTextAttr CommentTextAttr;
	wxTextAttr IgnoreTextAttr;
	wxTextAttr WhitespaceTextAttr;

	long cutOrPasteLineNo;

	bool settingStyle;

public:

	enum TextType
	{
		PlainText,
		Comment,
		Ignore,
		Whitespace
	};

	FileEditor(const wxString& title, wxWindow* parent);
	void AppendText(const wxString& text, const TextType& type);

	void AppendText(const wxString& text, const wxTextAttr& style);
	void AppendTextNoStyleSave(const wxString& text, const wxTextAttr& style);

	void SetStyle(const long& begin, const long& end, const TextType& type);

	wxTextAttr TextTypeToTextAttr(const TextType& type);

	//void AppendTextDefaultStyle(const wxString& text);
	void SetTextStyleType(const TextType& type);

	void OnKeyUp(wxKeyEvent &event);
	void OnKeyDown(wxKeyEvent &event);

	void OnTextCut(wxClipboardTextEvent& event);
	void OnTextPaste(wxClipboardTextEvent& event);

	void OnTextUpdate(wxCommandEvent& event);

	long GetNumberOfLines();

	long XYToPosition(const long& column, const long& line);
	bool PositionToXY(const long& pos, long& column, long& line);

	long GetLineLength(const long& lineNo);

	void SaveCutOrPasteLineNo();

	void Freeze();
	void Thaw();

	vector<wxString> GetLinesContent(const long& startLine, const long& endLine);
};

#endif
