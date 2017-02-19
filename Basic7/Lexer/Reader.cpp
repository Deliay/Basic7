#include "LexicalAnalyzer.h"

namespace Basic7
{
	namespace Lexer
	{
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

		void Reader::Back()
		{
			curPos--;
		}

		bool Reader::IsEnd()
		{
			return curPos + 1 >= src.length();
		}

	}
}