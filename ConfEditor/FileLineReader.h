#ifndef FILE_LINE_READER_H_
#define FILE_LINE_READER_H_

#include <wx/wx.h>
#include <vector>

using namespace std;

class FileLineReader
{
public:
	static vector<wxString> ReadFile(wxString filename);
};

#endif
