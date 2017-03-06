#include "LexicalAnalyzer.h"

char Basic7::Lexer::Reader::Read()
{
	if (curPos < src.length())
	{
		char ch = src[curPos++];
		if (ch == '\r' || ch == '\n')
		{
			if (src[curPos] == '\n')
			{
				ch = src[curPos];
				curPos++;
			}
			curLine++;
			curLinePos = 0;
		}
		else
		{
			curLinePos++;
		}

		return ch;
	}
	else return '\0';
}

void Basic7::Lexer::Reader::Back()
{
	curPos--;
}

bool Basic7::Lexer::Reader::IsEnd()
{
	return curPos + 1 >= src.length();
}