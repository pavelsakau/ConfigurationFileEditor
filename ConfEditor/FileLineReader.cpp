#include "FileLineReader.h"
#include <fstream>
#include <string>

using namespace std;

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

wxString FileLineReader::ReadFileContent(wxString filename)
{
	//TODO error processing here!
	std::ifstream ifs(filename.ToStdString());
	wxString fileContent(string(istreambuf_iterator<char>(ifs), (istreambuf_iterator<char>())));
	ifs.close();
	return fileContent;
}