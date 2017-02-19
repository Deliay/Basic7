#include "Lexer\LexicalAnalyzer.h"
#include <stdio.h>

using namespace Basic7;
int main()
{
	Lexer::LexerAnalyer lexer = {};

	FILE *file = fopen(R"(F:\Projects\Basic7\Debug\test.vb7)", "r");
	fseek(file, 0, SEEK_END);
	size_t fileSize = ftell(file);
	fseek(file, 0, SEEK_SET);

	void* buffer = malloc(fileSize);
	fileSize = fread(buffer, 1, fileSize, file);

	std::string codes(static_cast<char*>(buffer), fileSize);

	if (lexer.LoadSource(codes))
	{
		for (auto item : lexer.GetResult())
		{
			printf("%d\t%d\n", item->Line, (int)item->TokenTag);
		}
	}
	else
	{
		puts("read fail");
		return EXIT_FAILURE;
	}
	
	return EXIT_SUCCESS;

}