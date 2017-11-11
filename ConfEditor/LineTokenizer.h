#ifndef LINE_TOKENIZER_H
#define LINE_TOKENIZER_H

#include "Token.h"
#include <vector>

using namespace std;

#define IGNORE_TEXT "#IGNORE"

class LineTokenizer
{
private:
	static vector<Token> ParseNonCommentLine(const wxString& line, bool& ignoreStatementFound, const bool& getLengthOnly = false);
public:
	static vector<Token> ParseTokens(const vector<wxString>& lines, const bool& getLengthOnly = false);
	static vector<Token> ParseSingleLine(const wxString& line, bool& ignoreStatementFound, const bool& getLengthOnly = false);
};

#endif
