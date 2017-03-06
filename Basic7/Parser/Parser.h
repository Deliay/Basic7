#pragma once

class Parser
{
	/*
		Program	->	Modules		
		Modules	->	Module
		Module	->	Static Function, Class, Static Class
		Blocks	->	Declares, Stmts
		Declare	->	Dim [Id] As [Type] [= InitVal], Dim [Id] As New [Class](Param)
		Declares->	Declares Declare | empty
		Function->	Params, Ret, Block
		Navigate->	Get, Set, Let
		Navigates>	Navigates Navigate
		Property->	Navigates
		Member	->	Function, Declare, Property
		Members	->	Members Member
		Class	->	Constructor, Members
		Type	->	BasicType | Class
		Stmts	->	Stmts Stmt | empty
		Stmt	->	id = bool
				|	If(bool) Then Stmts
				|	If(bool) Then Stmts Else Stmts
				|	While/Until(bool) Stmts Wend
				|	Do Stmts While/Until(bool)
				|	Break, Continue

		id = id[bool] | id
		* bool  -> bool || join | join
		* join  -> join && eqv | eqv
		* eqv   -> eqv == rel | eqv != rel | rel
		* rel   -> expr < expr | expr <= expr | expr >= expr | expr > expr | expr
		* expr  -> expr + term | expr - term | term
		* term  -> term * unary | term / unary | unary
		* unary -> !unary | -unary | factor
		* factor-> (bool) | id | Number | Real | True | false

	*/
};