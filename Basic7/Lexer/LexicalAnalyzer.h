#pragma once
#include <vector>
#include <utility>
#include <map>
#include "..\Symbols\Tokens.h"

#define RESTOKEN(T) {HashSeq(T), T}
#define RESTOKEN_WORD(T, S) {HashSeq(S), ReserveWord(T,S)}
#define RESTOKEN_OP(T, S) {HashSeq(S), Operator(Tag::T, S, Operators::T)}
#define EOF reader.IsEnd()
#define READ peek = reader.Read()
#define CUL reader.CurrentLine()

namespace Basic7
{
	namespace Lexer
	{
		uint32_t HashSeq(const char *_First, size_t _Count);
		inline uint32_t HashSeq(const std::string& str)
		{
			return HashSeq(str.c_str(), str.length());
		}

		//typedef std::pair<std::string, Symbols::Token> TokenPair;
		typedef std::vector<Symbols::Token*> TokenList;
		typedef std::map<uint32_t, Symbols::Token> Dictionary;

		class Reader
		{
		public:
			Reader(const std::string& source) : src(source) {};
			Reader() = default;

			void PutSource(const std::string& source) { src = source; };

			char Read();
			void Back();
			bool IsEnd();

			inline int32_t CurrentLine() { return curLine; };
			inline int32_t CurrentLinePos() { return curLinePos; };
			inline int32_t CurrentPos() { return curPos; };

			inline static bool isTerminalSymbol(char c)
			{
				return std::string("+-*/%=><!&|").find(c) != std::string::npos;
			}

		private:
			std::string src;
			uint32_t curPos = 0;
			uint32_t curLine = 0;
			uint32_t curLinePos = 0;
		};

		using Symbols::Token;
		using Symbols::Tag;

		class LexerAnalyer
		{
		public:
			LexerAnalyer();
			~LexerAnalyer();
			bool LoadSource(const std::string& soruce);
			Token* Scan();
			Token* TokenExist(const std::string& kw);
			inline const TokenList& GetResult() const
			{
				return result;
			}


		private:
			Dictionary words;
			TokenList result;
			Reader reader;
			void reserve(Symbols::Word t) {}
		};
	}
}