#include "FileEditorLoader.h"
#include "FileEditor.h"
#include "FileLineReader.h"
#include "LineTokenizer.h"
#include "Colorer.h"

void FileEditorLoader::LoadEditorFromFile(const wxString& filename, FileEditor& fileeditor)
{
	/*vector<wxString> lines = FileLineReader::ReadFile(filename);
	vector<Token> tokens = LineTokenizer::ParseTokens(lines);
	for (Token token : tokens)
	{
		switch (token.GetType())
		{
		case Token::TokenType::Text:
			fileeditor.AppendText(token.GetText(), FileEditor::TextType::PlainText);
			break;
		case Token::TokenType::Whitespace:
			if (token.GetText().length() > 1) {
				fileeditor.AppendText(token.GetText(), FileEditor::TextType::Whitespace);
			} else {
				fileeditor.AppendText(token.GetText(), FileEditor::TextType::PlainText);
			}
			break;
		case Token::TokenType::Comment:
			fileeditor.AppendText(token.GetText(), FileEditor::TextType::Comment);
			break;
		case Token::TokenType::Ignore:
			fileeditor.AppendText(token.GetText(), FileEditor::TextType::Ignore);
			break;
		}
	}*/
	fileeditor.Freeze();
	vector<wxString> lines = FileLineReader::ReadFile(filename);
	for (wxString line : lines) {
		fileeditor.AppendText(line.Append("\n"), FileEditor::TextType::PlainText);
	}
	Colorer::RecolorLines(fileeditor, 0, fileeditor.GetNumberOfLines() - 1);
	fileeditor.Thaw();
}