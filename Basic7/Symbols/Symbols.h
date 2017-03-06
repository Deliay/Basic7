#pragma once
#include <sstream>
///程序保留关键字
#define RESWORD(NAME, TAG, WORD) const ReserveWord NAME(TAG, WORD)

///程序保留类型
#define RESTYPE(NAME, WIDTH) const ReserveType NAME(VarType::NAME, Tag::BASICTYPE, WIDTH)
#define RESTYPET(NAME, TAG, WIDTH) const ReserveType NAME(VarType::NAME, TAG, WIDTH)

#ifdef _WIN32
#include <Windows.h>
#undef FALSE
#undef TRUE
#undef EOF
#undef OPTIONAL
#endif // _WIN32

namespace Basic7
{

	namespace Symbols
	{
		///Tag 是程序读入的形式化标签之一
		enum class Tag : int
		{
			PROGRAM_END = 0,
			LINE_END = '\n',
			BitAnd = '&',
			Split = ',',
			BitOr = '|',
			Equal = '=',
			Not = '!',
			Less = '<', Large = '>',
			BitNot = '~',
			Add = '+',
			Sub = '-',
			Multi = '*',
			Div = '/',
			Mod = '%',
			LeftBar = '[', RightBar = ']',
			LeftPar = '(', RightPar = ')',
			LeftBlock = '{', RightBlock = '}',
			TAG_START = 256, TAG_NULL,
			NUMBER,             //Number(Integer)
			REAL,               //Floats
			ARRAY,              //Array
			IDENTITY,           //Identities
			TRUE,               //TRUE Flag
			FALSE,              //FALSE Flag
			IF, ELSE, ELSEIF, THEN,
			DO, WHILE, UNTIL, WEND, BREAK, CONTINUE, LOOP,
			STATIC, CLASS, FUNCTION, SUB, RETURN, OPTIONAL,
			MODULE,
			AND, OR, EQV, NEQV, LESEQ, LARGEEQ,
			MINUS,
			BASICTYPE,			//for basic type
			LITERAL,            //for "" strings
			DIM,                //Dim Keyword
			AS,                 //As Keyword
			END,                //End Keyword
			TAG_END,			//TAG RANGE
		};

		const std::initializer_list<Tag> ExistTags = { Tag::PROGRAM_END,
			Tag::LINE_END,
			Tag::BitAnd,
			Tag::Split,
			Tag::BitOr,
			Tag::Equal,
			Tag::Not,
			Tag::Less,
			Tag::Large,
			Tag::BitNot,
			Tag::Add,
			Tag::Sub,
			Tag::Multi,
			Tag::Div,
			Tag::Mod,
			Tag::LeftBar,
			Tag::LeftPar,
			Tag::LeftBlock,
			Tag::RightBar,
			Tag::RightPar,
			Tag::RightBlock,
		};

		inline static bool CheckTag(const Tag& tag)
		{
			for (auto item : Symbols::ExistTags) if (item == tag) return true;
			if (tag > Tag::TAG_START && tag < Tag::TAG_END) return true;

			return false;
		}

		///Operators 标志运算符(带优先级
		enum class Operators : int
		{
			Group3 = 30,
			Mod,
			Div = Mod,
			Multi = Mod,
			Add,
			Sub = Add,
			SHL,
			SHR = SHL,
			Group4 = 40,
			Less,
			LessEq = Less,
			Large = Less,
			LargeEq = Less,
			LogicEq,
			LogicNEq = LogicEq,
			Group8 = 80,
			Equal,
		};


		enum class VarType : int
		{
			Void,
			Bool8,
			Bool16,
			Bool32,
			Int8,
			Int16,
			Int32,
			Int64,
			UInt8,
			UInt16,
			UInt32,
			UInt64,
			Half,
			Single,
			Double,
			DoubleEx,
			IntPtr,
			UIntPtr,
			Char,
			Byte,
			Array,
			Type,
			Class,
			Interface,
			Function,
			Delegate,
			Module,
			Dynmaic,
			Property,
			Typedef,
			String,
			Const,
		};

#define VARTYPENAME(TAG) Symbols::VarTypeStr[(int)TAG]

		const std::string VarTypeStr[] = {
			"Void",
			"Bool8",
			"Bool16",
			"Bool32",
			"Int8",
			"Int16",
			"Int32",
			"Int64",
			"UInt8",
			"UInt16",
			"UInt32",
			"UInt64",
			"Half",
			"Single",
			"Double",
			"DoubleEx",
			"IntPtr",
			"UIntPtr",
			"Char",
			"Byte",
			"Array",
			"Type",
			"Class",
			"Interface",
			"Function",
			"Delegate",
			"Module",
			"Dynmaic",
			"Property",
			"Typedef",
			"String",
			"Const"
		};

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

			inline static bool IsArray(const Type& t);
		};


		class SymbolTable
		{

		};
	}

	namespace Expr
	{
		using namespace Symbols;
		static int labels = 0;

		///Base element for base Nodes
		class Node
		{
		public:
			Node(int lexline) : line(lexline) {};
			Node() = delete;
			inline int getLine()
			{
				return line;
			}

		private:
			int line = 0;
		};

		class Expr : public Node
		{
		public:
			Expr(Token * tok, Type * p) : Node(tok->Line), op(tok), type(p) {};
			Expr() = delete;
			inline Token * Op() { return op; };
			inline Type * getType() { return type; };
			virtual Expr * gen() { return this; };
			virtual Expr * reduce() { return this; };
			virtual void jumping(int t, int f) { emitjumps(this, t, f); };
			virtual void emitjumps(const Expr * s, int truejump, int falsejump)
			{
				if (truejump != 0 && falsejump != 0)
				{
					//if Expr Goto TrueJump
					//Goto FalseJump
				}
				else if (truejump != 0)
				{
					//if Expr Goto TrueJump
				}
				else if (falsejump != 0)
				{
					//ifFalse Expr Goto FalseJump
				}
			};
			virtual operator std::string() { return *op; };

		private:
			Token * op;
			Symbols::Type * type;
		};

		class Id : public Expr
		{
		public:
			Id(Word * idTok, SymbolTable * t, Type * type, int Offset, bool Static) : 
				Expr(idTok, type), offset_(Offset), static_(Static), id_(idTok), scope_(t) {};
			
			inline int varoffset() { return offset_; }
			inline SymbolTable * varscope() { return scope_; }
			inline bool isStatic() { return static_; }
			inline bool isIniail() { return initial_; }
			inline Word * id() { return id_; }

		private:
			int offset_;
			SymbolTable * scope_;
			bool static_, initial_;
			Word * id_;

		};

	}

	namespace Stmts
	{
		class Stmt : public Expr::Node
		{
		public:
			Stmt(int lexline) : Node(lexline) {};
			Stmt() = delete;
			virtual void gen(int begin, int after) {};
			int after() { return _after; }
			int before() { return _before; }

		private:
			int _after = 0, _before = 0;
		};

		static const Stmt * Null = nullptr;
		static Stmt * Encloseing = nullptr;
		static Stmt * LastFunction = nullptr;

		class Scope : public Stmt
		{

		};
	}
}