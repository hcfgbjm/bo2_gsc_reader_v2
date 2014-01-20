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
		if ((void*)stack.currentVar->value)
			free((void*)stack.currentVar->value); // migrate all strings with this type to malloc first

		stack.currentVar->value = 0;
	}
	else if (stack.currentVar->type == type_buildable_operation)
	{
		// free the built string if there's one
		if (stack.currentVar->value)
			free((void*)stack.currentVar->value);

		if (stack.currentVar->operatorsInfo)
		{
			OperatorsInfo* operatorsInfo = stack.currentVar->operatorsInfo;

			// free the OperatorsInfo members
			for (DWORD i = 0; i < operatorsInfo->numOfOperands; i++)
				free(operatorsInfo->operandList[i]);
			free(operatorsInfo->operandList);

			free(operatorsInfo->operatorList);
			free(operatorsInfo->operatorExecutionOrder);

			// free the OperatorsInfo struct
			free(operatorsInfo);
		}
	}

	memset(stack.currentVar, 0x00, sizeof(StackEntry)); // needed?

	stack.currentVar--;
}

DWORD StackGetValue(int index)
{
	// special case for type_buildable_operation (build the operation string if it's not built yet)
	// sure need to build?!?!
	if (StackGetValueType(index) == type_buildable_operation && !((stack.currentVar - index)->value))
		build_operation(index);

	return (stack.currentVar - index)->value;
}

VariableType StackGetValueType(int index)
{
	return (stack.currentVar - index)->type;
}

OperatorsInfo* StackGetOperatorsInfo(int index)
{
	return (stack.currentVar - index)->operatorsInfo;
}

void StackSetOperatorsInfo(OperatorsInfo* operatorsInfo)
{
	stack.currentVar->operatorsInfo = operatorsInfo;
}

DWORD StackGetLastValue()
{
	return StackGetValue(0);
}

VariableType StackGetLastValueType()
{
	return StackGetValueType(0);
}

DWORD StackGetRelativePos()
{
	return (stack.currentVar - stack.vars + 1) * sizeof(StackEntry*);
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