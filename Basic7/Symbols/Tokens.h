#pragma once
#include <sstream>
#include "Tags.h"
#ifdef _WIN32
#include <Windows.h>
#undef FALSE
#undef TRUE
#undef EOF
#undef OPTIONAL
#endif // _WIN32

///�������ؼ���
#define RESWORD(NAME, TAG, WORD) const ReserveWord NAME(TAG, WORD)

///����������
#define RESTYPE(NAME, WIDTH) const ReserveType NAME(VarType::NAME, Tag::BASICTYPE, WIDTH)
#define RESTYPET(NAME, TAG, WIDTH) const ReserveType NAME(VarType::NAME, TAG, WIDTH)

namespace Basic7
{
	namespace Symbols
	{
		class Token
		{
		public:
			///storage toke type
			const Tag TokenTag;
			///token line, 0 for reserve
			const int Line;
			///name for identity
			//const std::string Name;

			//disable nav for default constructor
			///initial Toekn with spificy tag, line
			Token(const Tag& tag, int line) : TokenTag(tag), Line(line) {};
			//Token(const Tag& tag, const std::string & name, int line) : TokenTag(tag), Line(line), Name(name) {};
			Token() = delete;

			//bool operator== (const Token& tok) { return TokenTag == tok.TokenTag && Line == tok.Line; };

			virtual operator std::string() const
			{
				return std::string((char *)&TokenTag);
			}

			inline long AddRef() volatile
			{
#ifdef _WIN32 
				return InterlockedIncrement(&refcount);
#endif
			}

			inline long Release() volatile
			{
#ifdef _WIN32
				if (auto ret = InterlockedDecrement(&refcount))
#endif
				{
					return ret;
				}
				delete this;
				return 0;
			}


		protected:
			volatile long refcount = 1;
			
		};

		static Token Null = { Tag::TAG_NULL, 0 };

		class Number : public Token
		{
		public:
			const unsigned int Value = 0;

			Number(unsigned int v, int line) : Token(Tag::NUMBER, line), Value(v) {}
			Number() = delete;

			operator std::string() const override
			{
				char buffer[10];
				_ultoa_s(Value, buffer, 10);
				return buffer;
			}

		};

		class Word : public Token
		{
		public:
			std::string lexeme;
			Word(const Tag& tag, const std::string& s, int line) : Token(tag, line), lexeme(s) {}
			Word() = delete;
			operator std::string() const override
			{
				return lexeme;
			}
			
		};

		class ReserveWord : public Word
		{
		public:
			ReserveWord(const Tag& tag, const std::string& s) : Word(tag, s, 0) {}
			ReserveWord() = delete;
		};

		RESWORD(True, Tag::TRUE, "true");	RESWORD(False, Tag::FALSE, "false");
		RESWORD(And, Tag::AND, "&&");		RESWORD(Or, Tag::OR, "||");
		RESWORD(Eqv, Tag::EQV, "==");		RESWORD(NEqv, Tag::NEQV, "!=");
		RESWORD(LessEq, Tag::LESEQ, "<=");	RESWORD(LargeEq, Tag::LARGEEQ, ">=");
		RESWORD(Minus, Tag::MINUS, "minus");

		class Operator : public Word
		{
		public:
			const Operators Level;
			Operator(const Tag& tag, const std::string& s, Operators lvl) : Word(tag, s, 0), Level(lvl) {};
			Operator() = delete;

		};

		class EndOfLine : public Token
		{
		public:
			EndOfLine(int line) : Token(Tag::LINE_END, line) {};
			EndOfLine() = delete;
		};

		class Real : public Token
		{
		public:
			const double Value = 0.0;
			const int Width = 8;

			/// 2 for half, 4 for single, 8 for double
			Real(double val, int line, int width = 8) : Token(Tag::REAL, line), Value(val), Width(width) {};
			Real() = delete;

			operator std::string() const override
			{
				char buffer[32];
				_fcvt_s(buffer, Value, 15, nullptr, nullptr);
				return buffer;
			}
		};

		class Type : public Word
		{
		public:
			const int Width = 1;
			
			Type(const std::string& s, const Tag& tag, int width, int line) : Word(tag, s, line), Width(width) {};
			Type(const VarType& vtype, const Tag& tag, int width, int line) : Word(tag, VARTYPENAME(vtype), line), Width(width) {};
			Type() = delete;

			bool operator== (const Type & t) const;
		};


		class ReserveType : public Type
		{
		public:
			ReserveType(const VarType& type, const Tag& tag, int width) : Type(type, tag, width, 0) {};
			ReserveType() = delete;

			static bool ExceptType(const Type& t, const std::initializer_list<Type>& list);
			static bool IsBoolean(const Type& t);
			static bool IsNumberic(const Type& t);
			static bool IsInteger(const Type& t);
			static bool IsFloat(const Type& t);
			static const Type& Max(const Type& t1, const Type& t2);
			static bool CovertableT1T2(const Type& t1, const Type& t2);

		};

		RESTYPE(Int8, 1);	RESTYPE(Int16, 2);	RESTYPE(Int32, 4);	RESTYPE(Int64, 8);
		RESTYPE(UInt8, 1);	RESTYPE(UInt16, 2); RESTYPE(UInt32, 4); RESTYPE(UInt64, 8);
		RESTYPE(Half, 2);	RESTYPE(Single, 4); RESTYPE(Double, 8); RESTYPE(DoubleEx, 10);
		RESTYPE(IntPtr, 4); RESTYPE(UIntPtr, 4);
		RESTYPE(Bool8, 1);	RESTYPE(Bool16, 2); RESTYPE(Bool32, 4);
		RESTYPE(Char, 2);	RESTYPE(Byte, 1);	RESTYPE(Void, 0);
		RESTYPET(Function, Tag::FUNCTION, 4);

		class Array : public Type
		{
		public:
			const Type ArrType;
			const int Size = 0;

			Array(const Type& type, int size) : Type("[]", Tag::ARRAY, type.Width, 0), Size(size), ArrType(type) {};
			Array() = delete;

			operator std::string() const override
			{
				std::stringstream ss;
				ss << ArrType.lexeme << "[" << Size << "]";
				return ss.str();
			}

			inline static bool IsArray(const Type& t)
			{
				return t.TokenTag == Tag::ARRAY;
			}
		};


	}

}