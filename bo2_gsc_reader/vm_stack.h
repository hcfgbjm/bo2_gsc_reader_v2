#pragma once

using namespace std;

struct vmStack_s
{
	char* localVars[MAX_VM_LOCAL_STACK_VARS];
	StackEntry vars[MAX_VM_STACK_VARS];
	char** currentLocalVar;
	StackEntry* currentVar;
};

extern vmStack_s stack;

void InitStacks();

// global stack

template <class T>
void StackPush(T value, VariableType type)
{
	if (sizeof(value) > sizeof(DWORD))
	{
		cout << "StackPush: value must not be longer than 4 bytes long" << endl;
		cin.get();
		ExitProcess(-1);
	}

	stack.currentVar++;

	*(T*)&stack.currentVar->value = value;
	stack.currentVar->type = type;
	stack.currentVar->flags = 0;
}

template <class T>
void StackSet(T value, VariableType type) // like StackPush, but it sets the value and type at the current position
{
	if (sizeof(value) > sizeof(DWORD))
	{
		cout << "StackSet: value must not be longer than 4 bytes long" << endl;
		cin.get();
		ExitProcess(-1);
	}

	StackPop();
	StackPush<T>(value, type);
}

void StackPop();
DWORD StackGetValue(int index);
VariableType StackGetValueType(int index);
DWORD StackGetLastValue();
VariableType StackGetLastValueType();
DWORD StackGetRelativePos();
DWORD StackGetFlags();
void StackSetFlag(DWORD flag);

OperatorsInfo* StackGetOperatorsInfo(int index);
void StackSetOperatorsInfo(OperatorsInfo* operatorsInfo);

// local stack

void StackLocalPush(char* varName);
void StackLocalPop();
char* StackLocalGetValue(int index);
char* StackLocalGetLastValue();