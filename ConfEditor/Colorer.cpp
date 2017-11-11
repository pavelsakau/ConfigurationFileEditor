#include "Colorer.h"
#include "LineTokenizer.h"

void Colorer::RecolorLines(FileEditor& fileeditor, const long& lineStart, const long& lineEnd)
{
	vector<wxString> lines = fileeditor.GetLinesContent(lineStart, lineEnd);
	vector<Token> coloredTokens = LineTokenizer::ParseTokens(lines, true);
	long linesCount = fileeditor.GetNumberOfLines();
	if (lineStart < linesCount) {
		long currentLine = lineStart;
		long positionInLine = 0;
		long globalPosition = fileeditor.XYToPosition(positionInLine, currentLine);
		for (Token token : coloredTokens) {
			if (currentLine >= 0 && currentLine < linesCount) {
				switch (token.GetType())
				{
				case Token::TokenType::Text:
					fileeditor.SetStyle(globalPosition, globalPosition + token.GetLength(), FileEditor::TextType::PlainText);
					break;
				case Token::TokenType::Whitespace:
					if (token.GetLength() > 1) {
						fileeditor.SetStyle(globalPosition, globalPosition + token.GetLength(), FileEditor::TextType::Whitespace);
					} else {
						fileeditor.SetStyle(globalPosition, globalPosition + token.GetLength(), FileEditor::TextType::PlainText);
					}
					break;
				case Token::TokenType::Comment:
					fileeditor.SetStyle(globalPosition, globalPosition + token.GetLength(), FileEditor::TextType::Comment);
					break;
				case Token::TokenType::Ignore:
					fileeditor.SetStyle(globalPosition, globalPosition + token.GetLength(), FileEditor::TextType::Ignore);
					break;
				}
				positionInLine += token.GetLength();
				globalPosition += token.GetLength();
				if (positionInLine >= (long)lines[currentLine - lineStart].length()) {
					currentLine++;
					positionInLine = 0;
				}
			}
		}
	}
}