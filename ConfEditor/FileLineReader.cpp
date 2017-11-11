#include "FileLineReader.h"
#include <fstream>

vector<wxString> FileLineReader::ReadFile(wxString filename)
{
	//TODO error processing here!
	vector<wxString> filelines;

	ifstream file(filename.ToStdString());
	string str; 
	while (getline(file, str))
	{
		filelines.push_back(wxString(str));
	}

	return filelines;
}