#include "stdafx.h"

vmStack_s stack;

void InitStacks()
{
	memset(&stack, 0x00, sizeof(vmStack_s));

	stack.currentVar = stack.vars - 1;
	stack.currentLocalVar = stack.localVars - 1;
}

// add stack overflow checks

// global stack

void StackPop()
{
	if (stack.currentVar->type == type_decompiled_string)
	{
		free((void*)stack.currentVar->value); // migrate all strings with this type to malloc first
		stack.currentVar->value = 0;
	}

	memset(stack.currentVar, 0x00, sizeof(VariableValue)); // needed?

	stack.currentVar--;
}

DWORD StackGetValue(int index)
{
	return (stack.currentVar - index)->value;
}

VariableType StackGetValueType(int index)
{
	return (stack.currentVar - index)->type;
}

DWORD StackGetLastValue()
{
	return stack.currentVar->value;
}

VariableType StackGetLastValueType()
{
	return stack.currentVar->type;
}

DWORD StackGetRelativePos()
{
	return (stack.currentVar - stack.vars + 1) * sizeof(VariableValue*);
}

DWORD StackGetFlags()
{
	return stack.currentVar->flags;
}

void StackSetFlag(DWORD flag)
{
	stack.currentVar->flags |= flag;
}

// local stack

void StackLocalPush(char* varName)
{
	stack.currentLocalVar++;

	*stack.currentLocalVar = varName;
}

void StackLocalPop()
{
	memset(*stack.currentLocalVar, 0x00, sizeof(char*)); // needed?

	stack.currentLocalVar--;
}

char* StackLocalGetValue(int index)
{
	return *(stack.currentLocalVar - index);
}

char* StackLocalGetLastValue()
{
	return *stack.currentLocalVar;
}