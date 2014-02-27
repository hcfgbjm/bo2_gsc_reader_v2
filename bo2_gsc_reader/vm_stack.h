#pragma once

using namespace std;

struct StackEntry
{
	VariableType type;
	DWORD value;		// pointer to the decompiled string when type is type_decompiled_string
						// NULL when it's type_precodepos

	OperatorsInfo* operatorsInfo;
	DWORD flags; // remove later
};

struct vmStack_s
{
	char* localVars[MAX_VM_LOCAL_STACK_VARS];
	StackEntry vars[MAX_VM_STACK_VARS];
	char** currentLocalVar;
	StackEntry* currentVar;
};

void StackCopy(vmStack_s* from, vmStack_s* to);