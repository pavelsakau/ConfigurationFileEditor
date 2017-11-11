#ifndef COLORER_H_
#define COLORER_H_

#include "FileEditor.h"
#include "Token.h"
#include <vector>

using namespace std;

class Colorer
{
public:
	static void RecolorLines(FileEditor& fileeditor, const long& lineStart, const long& lineEnd);
};

#endif
