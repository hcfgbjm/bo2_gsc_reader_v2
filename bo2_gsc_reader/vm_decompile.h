#pragma once

enum ObjectType
{
	OBJECT_LEVEL,
	OBJECT_ANIM,
	OBJECT_SELF,
	OBJECT_CASTED,
};

extern ObjectType CurrentObject;
extern char* CastedObject;
extern char VariableNameBuffer[256];

char* GetStringForCurrentObject();

void jump_expression_decompile(BYTE jumpOP, DWORD jumpTo);

void waittill_call_decompile(char* functionName, BYTE* currentPos);

void call_decompile(char* functionName, bool hasPrecodepos, DWORD _numOfParameters, bool pointerCall, bool methodCall, bool threadCall, bool resultUnused);

void build_operation(int index); // used by vm_stack.cpp

void operator_decompile(OperatorType operatorType);

void CheckIfJumps(BYTE* opcodesPtr);

void WriteRegisterInfo(BYTE* gscBuffer, BYTE* ip);