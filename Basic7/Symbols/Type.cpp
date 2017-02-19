#include "Tokens.h"

bool Basic7::Symbols::Type::operator==(const Type & t) const
{
	if (lexeme == t.lexeme && Width == t.Width) return true;
	return false;
}

bool Basic7::Symbols::ReserveType::ExceptType(const Type & t, const std::initializer_list<Type>& list)
{
	for (auto item : list)
	{
		if (item == t) return true;
	}
	return false;
}

bool Basic7::Symbols::ReserveType::IsBoolean(const Type & t)
{
	return ExceptType(t, { Bool8, Bool16, Bool32 });
}

bool Basic7::Symbols::ReserveType::IsNumberic(const Type & t)
{
	return ExceptType(t, {Int8, Int16, Int32, UInt8, UInt16, UInt32, IntPtr, UIntPtr, Char, Byte, Half, Single, Double, DoubleEx});
}

bool Basic7::Symbols::ReserveType::IsInteger(const Type & t)
{
	return ExceptType(t, { Int8, Int16, Int32, UInt8, UInt16, UInt32, IntPtr, UIntPtr, Char, Byte });
}

bool Basic7::Symbols::ReserveType::IsFloat(const Type & t)
{
	return ExceptType(t, { Half, Single, Double, DoubleEx });
}

const Basic7::Symbols::Type & Basic7::Symbols::ReserveType::Max(const Type & t1, const Type & t2)
{
	if (t1 == t2) return t1;

	if (t1.Width == t2.Width)
	{
		return IsFloat(t1) ? t1 : t2;
	}
	else
	{
		return t1.Width > t2.Width ? t1 : t2;
	}
}

bool Basic7::Symbols::ReserveType::CovertableT1T2(const Type & t1, const Type & t2)
{
	if (t1 == t2) return true;
	if (IsNumberic(t1) && IsNumberic(t2) && Max(t1, t2) == t1)
		return true;
	else if (IsBoolean(t1) && IsBoolean(t2))
		return true;

	return false;
}
