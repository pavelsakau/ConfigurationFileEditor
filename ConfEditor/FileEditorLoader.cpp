#include "FileEditorLoader.h"
#include "FileEditor.h"
#include "FileLineReader.h"
#include "LineTokenizer.h"
#include "Colorer.h"

void FileEditorLoader::LoadEditorFromFile(const wxString& filename, FileEditor& fileeditor)
{
	fileeditor.SetText((const wxString&)FileLineReader::ReadFileContent(filename));
}