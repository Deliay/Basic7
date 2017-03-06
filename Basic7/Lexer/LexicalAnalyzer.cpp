#include "LexicalAnalyzer.h"

uint32_t Basic7::Lexer::HashSeq(const char *_First, size_t _Count)
{
	const uint32_t offset = 2166136261U;
	const uint32_t prime = 16777619U;

	uint32_t value = offset;
	for (uint32_t next = 0; next < _Count; ++next)
	{
		value ^= (uint32_t)_First[next];
		value *= prime;
	}
	return (value);
}

Basic7::Lexer::LexerAnalyer::LexerAnalyer()
{
	using namespace Symbols;
	//initial reserve words
	words = {
		RESTOKEN(True),		RESTOKEN(False),
		RESTOKEN(And),		RESTOKEN(Or),
		RESTOKEN(Eqv),		RESTOKEN(NEqv),
		RESTOKEN(LargeEq),	RESTOKEN(LessEq),
		RESTOKEN(Minus),
		RESTOKEN(Bool8),	RESTOKEN(Bool16),	RESTOKEN(Bool32),
		RESTOKEN(Int8),		RESTOKEN(Int16),	RESTOKEN(Int32),	RESTOKEN(Int64),
		RESTOKEN(UInt8),	RESTOKEN(UInt16),	RESTOKEN(UInt32),	RESTOKEN(UInt64),
		RESTOKEN(Half),		RESTOKEN(Single),	RESTOKEN(Double),	RESTOKEN(DoubleEx),
		RESTOKEN(IntPtr),	RESTOKEN(UIntPtr), 
		RESTOKEN(Char),		RESTOKEN(Byte),		RESTOKEN(Void),
		RESTOKEN_WORD(Tag::DIM, "dim"),
		RESTOKEN_WORD(Tag::IF, "if"),				RESTOKEN_WORD(Tag::ELSE, "else"),
		RESTOKEN_WORD(Tag::ELSEIF, "elseif"),		RESTOKEN_WORD(Tag::THEN, "then"),
		RESTOKEN_WORD(Tag::OPTIONAL, "optional"),
		RESTOKEN_WORD(Tag::END, "end"),
		RESTOKEN_WORD(Tag::AS, "as"),
		RESTOKEN_WORD(Tag::DO, "do"),				RESTOKEN_WORD(Tag::WHILE, "while"),
		RESTOKEN_WORD(Tag::WEND, "wend"),			RESTOKEN_WORD(Tag::LOOP, "loop"),
		RESTOKEN_WORD(Tag::UNTIL, "until"),
		RESTOKEN_WORD(Tag::FUNCTION, "function"),	RESTOKEN_WORD(Tag::SUB, "sub"),
		RESTOKEN_WORD(Tag::BREAK, "break"),			RESTOKEN_WORD(Tag::CONTINUE, "continue"),
		RESTOKEN_WORD(Tag::RETURN, "return"),
		RESTOKEN_WORD(Tag::CLASS, "class"),
		RESTOKEN_WORD(Tag::STATIC, "static"),
		RESTOKEN_WORD(Tag::MODULE, "module"),
		RESTOKEN_OP(Equal, "="),
		RESTOKEN_OP(Add, "+"),		RESTOKEN_OP(Sub, "-"),
		RESTOKEN_OP(Multi, "*"),	RESTOKEN_OP(Div, "/"),
		RESTOKEN_OP(Mod, "%"), 
		RESTOKEN_OP(Less, "<"),		RESTOKEN_OP(Large, ">"),
	};

}

Basic7::Lexer::LexerAnalyer::~LexerAnalyer()
{
	for (auto item : result)
	{
		item->Release();
	}
}

bool Basic7::Lexer::LexerAnalyer::LoadSource(const std::string& soruce)
{
	reader.PutSource(soruce);
	Token *curTok = Scan();
	do
	{
		if (!Symbols::CheckTag(curTok->TokenTag))
		{
			curTok->Release();
			return false;
		}
				
		result.push_back(curTok);
		curTok = Scan();
	} while (curTok->TokenTag != Tag::PROGRAM_END);

	curTok->Release();

	return true;
}

Basic7::Symbols::Token* Basic7::Lexer::LexerAnalyer::Scan()
{
	char peek = ' ';
	for (;; READ)
	{
		if (EOF) return new Token(Tag::PROGRAM_END, reader.CurrentLine() - 1);
		if (peek == ' ' || peek == '\t' || peek == '\r' ) continue;
		if (peek == '\n') return new Token(Tag::LINE_END, reader.CurrentLine());
		else break;
	}

	//occur digit
	if (isdigit(peek))
	{
		int v = 0;
		do
		{
			v = 10 * v + (peek - 48); READ;
		} while (isdigit(peek));
		//break while not a digital
				
		//but not occur a dot
		if (peek != '.')
		{
			//reader back a position
			reader.Back();
			//return a NUMBER Token
			return new Symbols::Number(v, CUL);
		}

		//occur a dot, make a real
		double f = v, d = 10;
		for (;; READ)
		{
			//not a digit
			if (!isdigit(peek)) break;
			f = f + (peek - 48) / d; d *= 10;
		}

		//occur a f or '!' flag for float(single) type
		if (peek == 'f' || peek == '!')
		{
			return new Symbols::Real(f, CUL, 4);
		}

		//double
		return new Symbols::Real(f, CUL);
	}

	if (isalpha(peek))
	{
		std::string str = {};
		do
		{
			str.push_back(peek); READ;
		} while (isdigit(peek) || isalpha(peek));

		reader.Back();

		Token* tok = TokenExist(str);
		if (tok)
		{
			tok->AddRef();
			return tok;
		}
		else
		{
			return new Symbols::Word(Tag::IDENTITY, str, CUL);
		}
				
	}

	if (reader.isTerminalSymbol(peek))
	{
		std::string str = {};
		do
		{
			str.push_back(peek); READ;
		} while (reader.isTerminalSymbol(peek));
		reader.Back();

		Token* tok = TokenExist(str);
		if (tok)
		{
			tok->AddRef();
			return tok;
		}
		else
		{
			return new Symbols::Word(Tag::IDENTITY, str, CUL);
		}


	}

	if (peek == '"') 
	{
		std::string str = {};
		READ;
		do
		{
			str.push_back(peek);
			READ;
		} while (peek != '"' && peek != '\n');
		return new Symbols::Word(Tag::LITERAL, str, CUL);
	}

	return new Token((Tag)peek, CUL);

}

Basic7::Symbols::Token* Basic7::Lexer::LexerAnalyer::TokenExist(const std::string& kw)
{
	auto iter = words.find(HashSeq(kw.c_str(), kw.length()));
	if (iter == words.end())
	{
		return nullptr;
	}
	else
	{
		return &iter->second;
	}
}