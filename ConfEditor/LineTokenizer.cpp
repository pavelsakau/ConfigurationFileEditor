#include "LineTokenizer.h"

vector<Token> LineTokenizer::ParseNonCommentLine(const wxString& line, bool& ignoreStatementFound, const bool& getLengthOnly)
{
	vector<Token> tokens;
	bool prevWasWhitespace = false;
	wxString buf("");
	int line_length = line.length();
	int buf_length = 0;
	for (int i = 0; i < line_length; i++) {
		bool isWhitespace = line[i] == ' ';
		if ((isWhitespace && (!prevWasWhitespace) && buf_length > 0) || ((!isWhitespace) && prevWasWhitespace && buf_length > 0)) {
			tokens.push_back(Token(prevWasWhitespace ? Token::TokenType::Whitespace : Token::TokenType::Text, getLengthOnly ? wxEmptyString : buf, buf_length));
			if (!getLengthOnly) {
				buf.clear();
			}
			buf_length = 0;
		}
		if (!getLengthOnly) {
			buf.append(line[i]);
		}
		buf_length++;
		prevWasWhitespace = isWhitespace;
	}
	if (buf_length > 0) {
		tokens.push_back(Token(prevWasWhitespace ? Token::TokenType::Whitespace : Token::TokenType::Text, getLengthOnly ? wxEmptyString : buf, buf_length));
	}
	return tokens;
}

vector<Token> LineTokenizer::ParseSingleLine(const wxString& line, bool& ignoreStatementFound, const bool& getLengthOnly)
{
	vector<Token> tokens;
	if (wxString(IGNORE_TEXT).Cmp(line) == 0) {
		tokens.push_back(Token(Token::TokenType::Ignore, getLengthOnly ? wxEmptyString : line , line.length()));
		ignoreStatementFound = true;
	} else {
		int semicolonPos = line.Find(";");
		if (semicolonPos == wxNOT_FOUND) {
			tokens = ParseNonCommentLine(line, ignoreStatementFound, getLengthOnly);
		} else {
			tokens = ParseNonCommentLine(line.substr(0, (size_t)semicolonPos), ignoreStatementFound, getLengthOnly);
			tokens.push_back(Token(Token::TokenType::Comment, getLengthOnly ? wxEmptyString : line.substr((size_t)semicolonPos, line.length() - semicolonPos), line.length() - semicolonPos));
		}
	}
	if (wxString(wxEmptyString).Cmp(line) == 0 && tokens.size() == 0) {
		tokens.push_back(Token(ignoreStatementFound ? Token::TokenType::Ignore : Token::TokenType::Text, wxString(""), 0));
	}
	return tokens;
}

vector<Token> LineTokenizer::ParseTokens(const vector<wxString>& lines, const bool& getLengthOnly)
{
	vector<Token> tokens;
	bool ignoreStatementFound = false;
	for (wxString line : lines)
	{
		if (ignoreStatementFound) {
			line.Append('\n');
			tokens.push_back(Token(Token::TokenType::Ignore, getLengthOnly ? wxEmptyString : line, line.length()));
		} else {
			vector<Token> line_tokens = LineTokenizer::ParseSingleLine(line, ignoreStatementFound, getLengthOnly);
			tokens.insert(tokens.end(), line_tokens.begin(), line_tokens.end());
			if (!getLengthOnly) {
				tokens.back().AppendNewLineBreak();
			}
			tokens.back().SetLength(tokens.back().GetLength() + 1);
		}
	}

	return tokens;
}