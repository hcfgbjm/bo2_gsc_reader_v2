#pragma once

#define MAX_VM_LOCAL_STACK_VARS		2048 // enoguh?
#define MAX_VM_STACK_VARS			2048

#define OP_FLAG_BOOL_NOT		1

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
	type_decompiled_string // added by myself, it means that the value is a char* and points to the call string like "GetDvarFloat ( #"scr_xpscale" )"
};

struct VariableValue
{
	VariableType type;
	DWORD value;
	DWORD flags; // added by myself to make decompilation easier
};