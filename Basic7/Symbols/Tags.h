#pragma once

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

	}
}