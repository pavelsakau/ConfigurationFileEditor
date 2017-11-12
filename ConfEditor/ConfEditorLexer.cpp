// Scintilla source code edit control
/** Lexer for ConfEditor **/

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdarg.h>
#include <assert.h>

#include <string>
#include <vector>
#include <map>
#include <algorithm>

#include "ILexer.h"
#include "Scintilla.h"
#include "SciLexer.h"

#include "WordList.h"
#include "LexAccessor.h"
#include "Accessor.h"
#include "StyleContext.h"
#include "CharacterSet.h"
#include "LexerModule.h"
#include "OptionSet.h"
#include "SparseState.h"

#ifdef SCI_NAMESPACE
using namespace Scintilla;
#endif

// Options used for ConfEditorLexer
struct OptionsConfEditor {
	bool stylingWithinPreprocessor;
	bool identifiersAllowDollars;
	bool trackPreprocessor;
	bool updatePreprocessor;
	bool triplequotedStrings;
	bool hashquotedStrings;
	bool fold;
	bool foldSyntaxBased;
	bool foldComment;
	bool foldCommentMultiline;
	bool foldCommentExplicit;
	std::string foldExplicitStart;
	std::string foldExplicitEnd;
	bool foldExplicitAnywhere;
	bool foldPreprocessor;
	bool foldCompact;
	bool foldAtElse;
	OptionsConfEditor() {
		stylingWithinPreprocessor = false;
		identifiersAllowDollars = true;
		trackPreprocessor = true;
		updatePreprocessor = true;
		triplequotedStrings = false;
		hashquotedStrings = false;
		fold = false;
		foldSyntaxBased = true;
		foldComment = false;
		foldCommentMultiline = true;
		foldCommentExplicit = true;
		foldExplicitStart = "";
		foldExplicitEnd = "";
		foldExplicitAnywhere = false;
		foldPreprocessor = false;
		foldCompact = false;
		foldAtElse = false;
	}
};

static const char *const confEditorWordLists[] = {
	"Primary keywords and identifiers",
	"Secondary keywords and identifiers",
	"Documentation comment keywords",
	"Global classes and typedefs",
	"Preprocessor definitions",
	0,
};

struct OptionSetCPP : public OptionSet<OptionsConfEditor> {
	OptionSetCPP() {
		DefineWordListSets(confEditorWordLists);
	}
};

class ConfEditorLexer : public ILexer {
	bool caseSensitive;
	CharacterSet setWord;
	CharacterSet setNegationOp;
	CharacterSet setArithmethicOp;
	CharacterSet setRelOp;
	CharacterSet setLogicalOp;
	WordList keywords;
	WordList keywords2;
	WordList keywords3;
	WordList keywords4;
	WordList ppDefinitions;
	std::map<std::string, std::string> preprocessorDefinitionsStart;
	OptionsConfEditor options;
	OptionSetCPP osCPP;
	SparseState<std::string> rawStringTerminators;
	enum { activeFlag = 0x40 };
public:
	ConfEditorLexer(bool caseSensitive_) :
		caseSensitive(caseSensitive_),
		setWord(CharacterSet::setAlphaNum, "._", 0x80, true),
		setNegationOp(CharacterSet::setNone, "!"),
		setArithmethicOp(CharacterSet::setNone, "+-/*%"),
		setRelOp(CharacterSet::setNone, "=!<>"),
		setLogicalOp(CharacterSet::setNone, "|&") {
	}
	virtual ~ConfEditorLexer() {
	}
	void SCI_METHOD Release() {
		delete this;
	}
	int SCI_METHOD Version() const {
		return lvOriginal;
	}
	const char * SCI_METHOD PropertyNames() {
		return osCPP.PropertyNames();
	}
	int SCI_METHOD PropertyType(const char *name) {
		return osCPP.PropertyType(name);
	}
	const char * SCI_METHOD DescribeProperty(const char *name) {
		return osCPP.DescribeProperty(name);
	}
	int SCI_METHOD PropertySet(const char *key, const char *val);
	const char * SCI_METHOD DescribeWordListSets() {
		return osCPP.DescribeWordListSets();
	}
	int SCI_METHOD WordListSet(int n, const char *wl);
	void SCI_METHOD Lex(unsigned int startPos, int length, int initStyle, IDocument *pAccess);
	void SCI_METHOD Fold(unsigned int startPos, int length, int initStyle, IDocument *pAccess);

	void * SCI_METHOD PrivateCall(int, void *) {
		return 0;
	}

	static ILexer *LexerFactoryConfEditor() {
		return new ConfEditorLexer(true);
	}
	static ILexer *LexerFactoryConfEditorInsensitive() {
		return new ConfEditorLexer(false);
	}
};

int SCI_METHOD ConfEditorLexer::PropertySet(const char *key, const char *val) {
	return -1;
}

int SCI_METHOD ConfEditorLexer::WordListSet(int n, const char *wl) {
	WordList *wordListN = 0;
	switch (n) {
	case 0:
		wordListN = &keywords;
		break;
	case 1:
		wordListN = &keywords2;
		break;
	case 2:
		wordListN = &keywords3;
		break;
	case 3:
		wordListN = &keywords4;
		break;
	case 4:
		wordListN = &ppDefinitions;
		break;
	}
	return -1;
}

void SCI_METHOD ConfEditorLexer::Lex(unsigned int startPos, int length, int initStyle, IDocument *pAccess) {
	LexAccessor styler(pAccess);

	CharacterSet setOKBeforeRE(CharacterSet::setNone, "([{=,:;!%^&*|?~+-");
	CharacterSet setCouldBePostOp(CharacterSet::setNone, "+-");
	CharacterSet setDoxygen(CharacterSet::setAlpha, "$@\\&<>#{}[]");
	CharacterSet setWordStart(CharacterSet::setAlpha, "_", 0x80, true);

	int chPrevNonWhite = ' ';
	int visibleChars = 0;
	bool lastWordWasUUID = false;
	int styleBeforeDCKeyword = SCE_C_DEFAULT;
	bool continuationLine = false;
	bool isIncludePreprocessor = false;

	StyleContext sc(startPos, length, initStyle, styler, 0x7f);
	//sc.state = SCE_C_DEFAULT;

	for (; sc.More();) {

		if (sc.state == SCE_C_COMMENTDOC) {
			sc.SetState(SCE_C_COMMENTDOC);
		} else if (sc.state == SCE_C_COMMENT && !sc.atLineEnd) {
			sc.SetState(SCE_C_COMMENT);
		} else if (sc.atLineStart && sc.ch != ';' && sc.ch != '#' && sc.ch != ' ') {
			sc.SetState(SCE_C_DEFAULT);
		} else if (sc.ch == ';') {
			sc.SetState(SCE_C_COMMENT);
		} else if (sc.atLineStart && sc.ch == '#' && sc.state != SCE_C_COMMENTDOC) {
			if (sc.Match("#IGNORE\r\n") || sc.Match("#IGNORE\r") || sc.Match("#IGNORE\n")) {
				sc.SetState(SCE_C_COMMENTDOC);
			} else {
				sc.SetState(SCE_C_DEFAULT);
			}
		} else if (sc.ch == ' ' && (!sc.atLineStart && (sc.chPrev == ' ' || sc.chPrev == '\t'))) {
			sc.SetState(SCE_C_WORD2);
		} else {
			sc.SetState(SCE_C_DEFAULT);
		}

		sc.Forward();
	}
	sc.Complete();
}

void SCI_METHOD ConfEditorLexer::Fold(unsigned int startPos, int length, int initStyle, IDocument *pAccess) {
	return;
}

extern LexerModule lmConfEditor(SCLEX_AUTOMATIC, ConfEditorLexer::LexerFactoryConfEditor, "ConfEditor", confEditorWordLists);

