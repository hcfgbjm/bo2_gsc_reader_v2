#pragma once

#define MAX_VM_LOCAL_STACK_VARS		2048 // enoguh?
#define MAX_VM_STACK_VARS			2048

// operator types
// ordered in operator precedence (i used the same as in c++)
// Operators are listed top to bottom, in descending precedence.
// http://en.cppreference.com/w/cpp/language/operator_precedence
enum OperatorType
{
	//OP_INC,				// *var*++; // not needed i think?
	//OP_DEC,				// *var*--;

	OP_BOOLNOT,			// !*var*
	OP_BOOLCOMPLEMENT,	// ~*var*
	
	OP_MULTIPLY,		// *var1* * *var2*
	OP_DIVIDE,			// *var1* / *var2*
	OP_MOD,				// *var1* % *var2*

	OP_PLUS,			// *var1* + *var2*
	OP_MINUS,			// *var1* - *var2*
	
	OP_SHIFT_LEFT,		// *var1* << *var2*
	OP_SHIFT_RIGHT,		// *var1* >> *var2*
	
	OP_LESS,			// *var1* < *var2*
	OP_GREATER,			// *var1* > *var2*
	OP_LESS_EQUAL,		// *var1* <= *var2*
	OP_GREATER_EQUAL,	// *var1* >= *var2*
	
	OP_EQUALITY,		// *var1* == *var2*
	OP_INEQUALITY,		// *var1* != *var2*

	OP_BIT_AND,			// *var1* & *var2*

	OP_BIT_EX_OR,		// *var1* ^ *var2*

	OP_BIT_OR,			// *var1* | *var2*

	OP_LOGICAL_AND,		// *var1* && *var2*

	OP_LOGICAL_OR,		// *var1* || *var2*
};

#define	OP_FLAG_BOOL_NOT		1

enum VariableType
{
	type_undefined,
	type_object,
	type_string,
	type_localized_string,
	type_vector,
	type_hash,
	type_float,
	type_int,
	type_codepos,
	type_precodepos,
	type_function,
	type_stack,
	type_animation,
	type_developer_codepos,
	type_decompiled_string, // added by myself, it means that the value is a char* and points to a decompiled string like "GetDvarFloat ( #"scr_xpscale" )"
	type_buildable_operation,
};

// added by myself to make decompilation easier (this is only present if type is type_unbuilt_operation)
// example: decompiled string = "v1 < v2" ; numOfOperands = 2 ; operandList = { "v1", "v2" } ; numOfOperators = 1 ; operatorList = { OP_LESS } ; operatorExecutionOrder = { 1 }
// another example: decompiled string = "v1 - (v2 - v3) ; numOfOperands = 3 ; operandList = { "v1", "v2", "v3" } ; numOfOperators = 2 ; operatorList = { OP_PLUS, OP_MULTIPLY } ; operatorExecutionOrder = { 2, 1 }
struct OperatorsInfo
{
	DWORD numOfOperands;
	char** operandList;

	DWORD numOfOperators;
	OperatorType* operatorList;
	DWORD* operatorExecutionOrder;
};