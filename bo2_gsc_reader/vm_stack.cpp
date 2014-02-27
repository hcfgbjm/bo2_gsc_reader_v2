#include "stdafx.h"

void GSCDecompilerClass::InitStacks()
{
	memset(&this->stack, 0, sizeof(vmStack_s));

	stack.currentVar = stack.vars - 1;
	stack.currentLocalVar = stack.localVars - 1;
}

void GSCDecompilerClass::FreeStacks()
{
	// free variables of the global stack
	while (stack.currentVar != stack.vars - 1)
		StackPop();

	// free variables of the local stack
	while (stack.currentLocalVar != stack.localVars - 1)
		StackLocalPop();
}

// global stack

void GSCDecompilerClass::StackPop()
{
	if (stack.currentVar->type == type_decompiled_string)
	{
		if (stack.currentVar->value)
			free((void*)stack.currentVar->value);
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

	memset(stack.currentVar, 0x00, sizeof(StackEntry)); // clear the stack entry we're poping

	stack.currentVar--;
}

DWORD GSCDecompilerClass::StackGetValue(int index)
{
	// special case for type_buildable_operation (build the operation string if it's not built yet)
	// sure need to build?!?!
	if (StackGetValueType(index) == type_buildable_operation && !((stack.currentVar - index)->value))
		build_operation(index);

	return (stack.currentVar - index)->value;
}

VariableType GSCDecompilerClass::StackGetValueType(int index)
{
	return (stack.currentVar - index)->type;
}

OperatorsInfo* GSCDecompilerClass::StackGetOperatorsInfo(int index)
{
	return (stack.currentVar - index)->operatorsInfo;
}

void GSCDecompilerClass::StackSetOperatorsInfo(OperatorsInfo* operatorsInfo)
{
	stack.currentVar->operatorsInfo = operatorsInfo;
}

DWORD GSCDecompilerClass::StackGetLastValue()
{
	return StackGetValue(0);
}

VariableType GSCDecompilerClass::StackGetLastValueType()
{
	return StackGetValueType(0);
}

DWORD GSCDecompilerClass::StackGetRelativePos()
{
	return (stack.currentVar - stack.vars + 1) * sizeof(StackEntry*);
}

// local stack

void GSCDecompilerClass::StackLocalPush(char* varName)
{
	stack.currentLocalVar++;

	*stack.currentLocalVar = varName;
}

void GSCDecompilerClass::StackLocalPop()
{
	memset(stack.currentLocalVar, 0x00, sizeof(char*));

	stack.currentLocalVar--;
}

char* GSCDecompilerClass::StackLocalGetValue(int index)
{
	return *(stack.currentLocalVar - index);
}

void StackCopy(vmStack_s* from, vmStack_s* to)
{
	to->currentVar = to->vars - 1;
	to->currentLocalVar = to->localVars - 1;

	// copy variables of the global stack
	for (int i = 0; i < (from->currentVar - from->vars + 1); to->currentVar++, i++)
	{
		if (from->vars[i].type == type_decompiled_string && from->vars[i].value)
		{
			to->vars[i].value = (DWORD)malloc(strlen((char*)from->vars[i].value) + 1);
			strcpy_s((char*)to->vars[i].value, strlen((char*)from->vars[i].value) + 1, (char*)from->vars[i].value);

			to->vars[i].type = type_decompiled_string;
		}
		else if (from->vars[i].type == type_buildable_operation)
		{
			// make a function for this? im cloning OperatorsInfo in a lot places now i think -_-
			OperatorsInfo* newOperatorsInfo = (OperatorsInfo*)malloc(sizeof(OperatorsInfo));
			OperatorsInfo* operatorsInfo = from->vars[i].operatorsInfo;

			newOperatorsInfo->numOfOperands = operatorsInfo->numOfOperands;
			newOperatorsInfo->operandList = (char**)malloc(sizeof(char*) * operatorsInfo->numOfOperands);
			for (DWORD i2 = 0; i2 < newOperatorsInfo->numOfOperands; i2++)
			{
				newOperatorsInfo->operandList[i2] = (char*)malloc(strlen(operatorsInfo->operandList[i2]) + 1);
				strcpy_s(newOperatorsInfo->operandList[i2], strlen(operatorsInfo->operandList[i2]) + 1, operatorsInfo->operandList[i2]);
			}

			newOperatorsInfo->numOfOperators = operatorsInfo->numOfOperators;
			newOperatorsInfo->operatorList = (OperatorType*)malloc(sizeof(OperatorType) * operatorsInfo->numOfOperators);
			memcpy(
				newOperatorsInfo->operatorList,
				operatorsInfo->operatorList,
				sizeof(OperatorType) * operatorsInfo->numOfOperators
				);
	
			newOperatorsInfo->operatorExecutionOrder = (DWORD*)malloc(sizeof(DWORD) * operatorsInfo->numOfOperators);
			memcpy(
				newOperatorsInfo->operatorExecutionOrder,
				operatorsInfo->operatorExecutionOrder,
				sizeof(DWORD) * operatorsInfo->numOfOperators
				);

			to->vars[i].operatorsInfo = newOperatorsInfo;

			to->vars[i].type = type_buildable_operation;
		}
		else // other types don't need allocating
		{
			to->vars[i].type = from->vars[i].type;
			to->vars[i].value = from->vars[i].value;
		}
	}

	// copy variables of the local stack
	for (int i = 0; i < (from->currentLocalVar - from->localVars + 1); to->currentLocalVar++, i++)
	{
		to->localVars[i] = from->localVars[i];
	}
}