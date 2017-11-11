#ifndef TOKEN_H_
#define TOKEN_H_

#include <wx/wx.h>

class Token
{
public:
	enum TokenType
	{
		Text,
		Comment,
		Whitespace,
		Ignore
	};
private:
	TokenType type;
	wxString text;
	long length;
public:

	Token(TokenType type, wxString text) : type(type), text(text), length(0) {};
	Token(TokenType type, wxString text, long length) : type(type), text(text), length(length) {};
	TokenType GetType() {return type;};
	wxString& GetText() {return text;};
	long GetLength() {return length;};
	void SetText(const wxString& newtext) {text = newtext;};
	void SetType(const TokenType& newtype) {type = newtype;};
	void SetLength(const long& newlength) {length = newlength;};
	void AppendNewLineBreak() {text.Append('\n');};
};

#endif
