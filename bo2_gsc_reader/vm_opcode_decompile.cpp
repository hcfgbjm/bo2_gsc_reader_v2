#include "stdafx.h"

bool opcode_dec = false;

// 0x0
BYTE* OP_End_Decompile(DWORD gscBuffer, BYTE* opcodesPtr, bool functionEnd)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	AddString("// OP_End();", true);
	WriteRegisterInfo((BYTE*)gscBuffer, currentPos - 1);
	}

	if (!functionEnd)
		AddString("return;\n", true);

	return currentPos;
}

// 0x1
BYTE* OP_Return_Decompile(DWORD gscBuffer, BYTE* opcodesPtr)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	AddString("// OP_Return();", true);
	WriteRegisterInfo((BYTE*)gscBuffer, currentPos - 1);
	}

	AddString("return %s;\n", true, StackGetLastValue());

	StackPop(); // the game obviously doesn't do it, but we need to do it

	return currentPos;
}

// 0x2
BYTE* OP_GetUndefined_Decompile(DWORD gscBuffer, BYTE* opcodesPtr)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	AddString("// OP_GetUndefined();", true);
	WriteRegisterInfo((BYTE*)gscBuffer, currentPos - 1);
	}
	
	char* Undefined = MallocAndSprintf("%s", "undefined");

	StackPush<char*>(Undefined, type_decompiled_string);

	return currentPos;
}

// 0x3
BYTE* OP_GetZero_Decompile(DWORD gscBuffer, BYTE* opcodesPtr)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	AddString("// OP_GetZero();", true);
	WriteRegisterInfo((BYTE*)gscBuffer, currentPos - 1);
	}
	
	char* Zero = MallocAndSprintf("%d", 0); // malloc because we free the string every time when poping if the type is type_decompiled_string

	StackPush<char*>(Zero, type_decompiled_string);

	return currentPos;
}

// 0x4
BYTE* OP_GetByte_Decompile(DWORD gscBuffer, BYTE* opcodesPtr)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte

	char* Byte = MallocAndSprintf("%d", *(unsigned __int8*)(currentPos));

	if (opcode_dec) {
	AddString("// OP_GetByte( %s );", true, Byte);
	WriteRegisterInfo((BYTE*)gscBuffer, currentPos - 1);
	}

	StackPush<char*>(Byte, type_decompiled_string);

	currentPos += 1;

	return currentPos;
}

// 0x5
BYTE* OP_GetNegByte_Decompile(DWORD gscBuffer, BYTE* opcodesPtr)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte

	char* Byte = MallocAndSprintf("-%d", *(unsigned __int8*)(currentPos)); // right?

	if (opcode_dec) {
	AddString("// OP_GetNegByte( %s );", true, Byte);
	WriteRegisterInfo((BYTE*)gscBuffer, currentPos - 1);
	}

	StackPush<char*>(Byte, type_decompiled_string);

	currentPos += 1;

	return currentPos;
}

// 0x6
BYTE* OP_GetUnsignedShort_Decompile(DWORD gscBuffer, BYTE* opcodesPtr)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte

	char* UnsignedShort = MallocAndSprintf("%d", *(unsigned __int16*)(GET_ALIGNED_WORD(currentPos)));

	if (opcode_dec) {
	AddString("// OP_GetUnsignedShort( %s );", true, UnsignedShort);
	WriteRegisterInfo((BYTE*)gscBuffer, currentPos - 1);
	}

	StackPush<char*>(UnsignedShort, type_decompiled_string);

	currentPos = GET_ALIGNED_WORD(currentPos) + 2;

	return currentPos;
}

// 0x7
BYTE* OP_GetNegUnsignedShort_Decompile(DWORD gscBuffer, BYTE* opcodesPtr)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte

	char* UnsignedShort = MallocAndSprintf("-%d", *(unsigned __int16*)(GET_ALIGNED_WORD(currentPos))); // right?

	if (opcode_dec) {
	AddString("// OP_GetNegUnsignedShort( %s );", true, UnsignedShort);
	WriteRegisterInfo((BYTE*)gscBuffer, currentPos - 1);
	}

	StackPush<char*>(UnsignedShort, type_decompiled_string);

	currentPos = GET_ALIGNED_WORD(currentPos) + 2;

	return currentPos;
}

// 0x8
BYTE* OP_GetInteger_Decompile(DWORD gscBuffer, BYTE* opcodesPtr)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte

	char* Int = MallocAndSprintf("%d", *(signed __int32*)(GET_ALIGNED_DWORD(currentPos)));

	if (opcode_dec) {
	AddString("// OP_GetInteger( %s );", true, Int);
	WriteRegisterInfo((BYTE*)gscBuffer, currentPos - 1);
	}

	StackPush<char*>(Int, type_decompiled_string);

	currentPos = GET_ALIGNED_DWORD(currentPos) + 4;

	return currentPos;
}

// 0x9
BYTE* OP_GetFloat_Decompile(DWORD gscBuffer, BYTE* opcodesPtr)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte

	//char* Float = RemoveDecimalTrailingZeros(MallocAndSprintf("%f", *(float*)(GET_ALIGNED_DWORD(currentPos))));
	char *Float = MallocAndSprintf("%g", *(float*)(GET_ALIGNED_DWORD(currentPos)));

	if (opcode_dec) {
	AddString("// OP_GetFloat( %s );", true, Float);
	WriteRegisterInfo((BYTE*)gscBuffer, currentPos - 1);
	}

	StackPush<char*>(Float, type_decompiled_string);

	currentPos = GET_ALIGNED_DWORD(currentPos) + 4;

	return currentPos;
}

// 0xA
BYTE* OP_GetString_Decompile(DWORD gscBuffer, BYTE* opcodesPtr)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte

	char* String = MallocAndSprintf("\"%s\"", gscBuffer + *(WORD*)(GET_ALIGNED_WORD(currentPos)));
	char* StringTemp = NULL;

	for (size_t i = 0; i < strlen(String); i++)
	{
		if (String[i] == 0x0A) // \n
		{
			String[i] = 0x00;
			StringTemp = MallocAndSprintf("%s%s%s%s", String, "\\", "n", String + i + 1);

			free(String);
			String = (char*)malloc(strlen(StringTemp) + 1);
			strcpy_s(String, strlen(StringTemp) + 1, StringTemp);
			free(StringTemp);
		}
	}
	
	if (opcode_dec) {
	AddString("// OP_GetString( %s );", true, String);
	WriteRegisterInfo((BYTE*)gscBuffer, currentPos - 1);
	}

	StackPush<char*>(String, type_decompiled_string);

	currentPos = GET_ALIGNED_WORD(currentPos) + 2;

	return currentPos;
}

// 0xB
BYTE* OP_GetIString_Decompile(DWORD gscBuffer, BYTE* opcodesPtr)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte

	char* IString = MallocAndSprintf("&\"%s\"", gscBuffer + *(WORD*)(GET_ALIGNED_WORD(currentPos)));
	
	if (opcode_dec) {
	AddString("// OP_GetIString( %s );", true, IString);
	WriteRegisterInfo((BYTE*)gscBuffer, currentPos - 1);
	}

	StackPush<char*>(IString, type_decompiled_string);

	currentPos = GET_ALIGNED_WORD(currentPos) + 2;

	return currentPos;
}

// 0xC
BYTE* OP_GetVector_Decompile(DWORD gscBuffer, BYTE* opcodesPtr)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte

	float* Vector = (float*)GET_ALIGNED_DWORD(currentPos);
	char* VectorString = MallocAndSprintf("{ %f, %f, %f }", *(Vector), *(Vector + 1), *(Vector + 2));
	
	if (opcode_dec) {
	AddString("// OP_GetVector( %s );", true, VectorString);
	WriteRegisterInfo((BYTE*)gscBuffer, currentPos - 1);
	}

	StackPush<char*>(VectorString, type_decompiled_string);

	currentPos = GET_ALIGNED_DWORD(currentPos) + 12;

	return currentPos;
}

// 0xD
BYTE* OP_GetLevelObject_Decompile(DWORD gscBuffer, BYTE* opcodesPtr)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte

	if (opcode_dec) {
	AddString("// OP_GetLevelObject();", true);
	WriteRegisterInfo((BYTE*)gscBuffer, currentPos - 1);
	}

	CurrentObject = OBJECT_LEVEL;

	return currentPos;
}

// 0xE
BYTE* OP_GetAnimObject_Decompile(DWORD gscBuffer, BYTE* opcodesPtr)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	AddString("// OP_GetAnimObject();", true);
	WriteRegisterInfo((BYTE*)gscBuffer, currentPos - 1);
	}

	CurrentObject = OBJECT_ANIM;

	return currentPos;
}

// 0xF
BYTE* OP_GetSelf_Decompile(DWORD gscBuffer, BYTE* opcodesPtr)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	AddString("// OP_GetSelf();", true);
	WriteRegisterInfo((BYTE*)gscBuffer, currentPos - 1);
	}
	
	char* Self = MallocAndSprintf("%s", "self"); // malloc because we free the string every time when poping if the type is type_decompiled_string

	StackPush<char*>(Self, type_decompiled_string);

	return currentPos;
}

// 0x10
BYTE* OP_GetLevel_Decompile(DWORD gscBuffer, BYTE* opcodesPtr)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	AddString("// OP_GetLevel();", true);
	WriteRegisterInfo((BYTE*)gscBuffer, currentPos - 1);
	}
	
	char* Level = MallocAndSprintf("%s", "level");
	
	StackPush<char*>(Level, type_decompiled_string);

	return currentPos;
}

// 0x11
BYTE* OP_GetGame_Decompile(DWORD gscBuffer, BYTE* opcodesPtr) // missing correct stack managing idk, make type_object_string? lulz
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	AddString("// OP_GetGame();", true);
	WriteRegisterInfo((BYTE*)gscBuffer, currentPos - 1);
	}

	char* Game = MallocAndSprintf("%s", "game");
	
	StackPush<char*>(Game, type_decompiled_string);

	return currentPos;
}

// 0x12
BYTE* OP_GetAnim_Decompile(DWORD gscBuffer, BYTE* opcodesPtr)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte

	if (opcode_dec) {
	AddString("// OP_GetAnim();", true);
	WriteRegisterInfo((BYTE*)gscBuffer, currentPos - 1);
	}

	StackPush<char*>("anim", type_object);

	return currentPos;
}

// 0x13
BYTE* OP_GetAnimation_Decompile(DWORD gscBuffer, BYTE* opcodesPtr)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte

	char* Animation = MallocAndSprintf("%s%s", "%", gscBuffer + *(WORD*)(GET_ALIGNED_DWORD(currentPos)));
	
	if (opcode_dec) {
	AddString("// OP_GetAnimation( %s );", true, Animation);
	WriteRegisterInfo((BYTE*)gscBuffer, currentPos - 1);
	}

	StackPush<char*>(Animation, type_decompiled_string);

	currentPos = GET_ALIGNED_DWORD(currentPos) + 4;

	return currentPos;
}

// 0x14
BYTE* OP_GetGameRef_Decompile(DWORD gscBuffer, BYTE* opcodesPtr)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	AddString("// OP_GetGameRef();", true);
	WriteRegisterInfo((BYTE*)gscBuffer, currentPos - 1);
	}

	return currentPos;
}

// 0x15
BYTE* OP_GetFunction_Decompile(DWORD gscBuffer, BYTE* opcodesPtr) // is this a string? need to check...
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	char* Function = MallocAndSprintf("::%s", gscBuffer + *(DWORD*)(GET_ALIGNED_DWORD(currentPos)));
	
	if (opcode_dec) {
	AddString("// OP_GetFunction( \"%s\" );", true, Function);
	WriteRegisterInfo((BYTE*)gscBuffer, currentPos - 1);
	}

	StackPush<char*>(Function, type_decompiled_string);

	currentPos = GET_ALIGNED_DWORD(currentPos) + 4;

	return currentPos;
}

// 0x17
BYTE* OP_CreateLocalVariable_Decompile(DWORD gscBuffer, BYTE* opcodesPtr)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte

	BYTE numOfVariables = *(BYTE*)(currentPos);
	currentPos += 1;

	if (opcode_dec) {
	AddString("// OP_CreateLocalVariable( %d", true, numOfVariables);
	if (numOfVariables)
		AddString(", ", false);
	}

	char* LocalVar = 0;
	
	for (BYTE i = 0; i < numOfVariables; i++)
	{
		LocalVar = (char*)(gscBuffer + *(WORD*)(GET_ALIGNED_WORD(currentPos)));

		StackLocalPush(LocalVar);

		if (opcode_dec) {
		AddString("\"%s\"", false, LocalVar);
		if ((i + 1) < numOfVariables)
			AddString(", ", false);
		}
		
		currentPos = GET_ALIGNED_WORD(currentPos) + 2;
	}

	if (opcode_dec) {
	AddString(" );", false);
	WriteRegisterInfo((BYTE*)gscBuffer, currentPos - 1);
	}

	return currentPos;
}

// 0x19
BYTE* OP_EvalLocalVariableCached_Decompile(DWORD gscBuffer, BYTE* opcodesPtr)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte

	char* VariableName = MallocAndSprintf("%s", StackLocalGetValue(*(BYTE*)(currentPos)));
	
	if (opcode_dec) {
	AddString("// OP_EvalLocalVariableCached( %d ); ( %s )", true, *(BYTE*)(currentPos), VariableName);
	WriteRegisterInfo((BYTE*)gscBuffer, currentPos - 1);
	}

	StackPush<char*>(VariableName, type_decompiled_string);

	currentPos += 1;

	return currentPos;
}

// 0x1A
BYTE* OP_EvalArray_Decompile(DWORD gscBuffer, BYTE* opcodesPtr)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	AddString("// OP_EvalArray();", true);
	WriteRegisterInfo((BYTE*)gscBuffer, currentPos - 1);
	}

	char* Array = MallocAndSprintf("%s[%s]", StackGetValue(0), StackGetValue(1));

	StackPop();
	StackPop();
	StackPush<char*>(Array, type_decompiled_string);

	return currentPos;
}

// 0x1C
BYTE* OP_EvalArrayRef_Decompile(DWORD gscBuffer, BYTE* opcodesPtr)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	AddString("// OP_EvalArrayRef();", true);
	WriteRegisterInfo((BYTE*)gscBuffer, currentPos - 1);
	}

	sprintf_s(VariableNameBuffer, "%s[%s]", VariableNameBuffer, StackGetLastValue());

	StackPop();

	return currentPos;
}

// 0x1D
BYTE* OP_ClearArray_Decompile(DWORD gscBuffer, BYTE* opcodesPtr)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	AddString("// OP_ClearArray();", true);
	WriteRegisterInfo((BYTE*)gscBuffer, currentPos - 1);
	}

	AddString("%s[%s] = undefined; // Correct?\n", true, VariableNameBuffer, StackGetLastValue());

	StackPop();

	return currentPos;
}

// 0x1E
BYTE* OP_EmptyArray_Decompile(DWORD gscBuffer, BYTE* opcodesPtr)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	AddString("// OP_EmptyArray();", true);
	WriteRegisterInfo((BYTE*)gscBuffer, currentPos - 1);
	}

	char* EmptyArray = MallocAndSprintf("%s", "[]");

	StackPush<char*>(EmptyArray, type_decompiled_string);

	return currentPos;
}

// 0x1F
BYTE* OP_GetSelfObject_Decompile(DWORD gscBuffer, BYTE* opcodesPtr)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte

	if (opcode_dec) {
	AddString("// OP_GetSelfObject();", true);
	WriteRegisterInfo((BYTE*)gscBuffer, currentPos - 1);
	}

	CurrentObject = OBJECT_SELF;

	return currentPos;
}

// 0x20
BYTE* OP_EvalFieldVariable_Decompile(DWORD gscBuffer, BYTE* opcodesPtr)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte

	char* FieldVariable = MallocAndSprintf("%s.%s", GetStringForCurrentObject(), gscBuffer + *(WORD*)(GET_ALIGNED_WORD(currentPos)));

	if (opcode_dec) {
	AddString("// OP_EvalFieldVariable( %s );", true, FieldVariable);
	WriteRegisterInfo((BYTE*)gscBuffer, currentPos - 1);
	}

	StackPush<char*>(FieldVariable, type_decompiled_string);

	currentPos = GET_ALIGNED_WORD(currentPos) + 2;

	return currentPos;
}

// 0x21
BYTE* OP_ClearFieldVariable_Decompile(DWORD gscBuffer, BYTE* opcodesPtr)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte

	char* FieldVariable = (char*)(gscBuffer + *(WORD*)(GET_ALIGNED_WORD(currentPos)));
	char* FieldOwner = GetStringForCurrentObject();

	if (opcode_dec) {
	AddString("// OP_ClearFieldVariable( \"%s\" ); (%s.%s)", true, FieldVariable, FieldOwner, FieldVariable);
	WriteRegisterInfo((BYTE*)gscBuffer, currentPos - 1);
	}

	sprintf_s(VariableNameBuffer, "%s.%s", FieldOwner, FieldVariable);

	currentPos = GET_ALIGNED_WORD(currentPos) + 2;

	return currentPos;
}

// 0x22
BYTE* OP_SafeCreateVariableFieldCached_Decompile(DWORD gscBuffer, BYTE* opcodesPtr)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte

	if (opcode_dec) {
	AddString("// OP_SafeCreateVariableFieldCached( \"%s\" );", true, gscBuffer + *(WORD*)(GET_ALIGNED_WORD(currentPos)));
	WriteRegisterInfo((BYTE*)gscBuffer, currentPos - 1);
	}

	currentPos = GET_ALIGNED_WORD(currentPos) + 2;

	return currentPos;
}

// 0x24
BYTE* OP_SafeSetWaittillVariableFieldCached_Decompile(DWORD gscBuffer, BYTE* opcodesPtr)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte

	if (opcode_dec) {
	AddString("// OP_SafeSetWaittillVariableFieldCached( %d );", true, *(BYTE*)(currentPos));
	WriteRegisterInfo((BYTE*)gscBuffer, currentPos - 1);
	}

	currentPos += 1;

	return currentPos;
}

// 0x25
BYTE* OP_clearparams_Decompile(DWORD gscBuffer, BYTE* opcodesPtr)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte

	if (opcode_dec) {
	AddString("// OP_clearparams();", true);
	WriteRegisterInfo((BYTE*)gscBuffer, currentPos - 1);
	}

	return currentPos;
}

// 0x26
BYTE* OP_checkclearparams_Decompile(DWORD gscBuffer, BYTE* opcodesPtr)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte

	if (opcode_dec) {
	AddString("// OP_checkclearparams();", true);
	WriteRegisterInfo((BYTE*)gscBuffer, currentPos - 1);
	}

	return currentPos;
}

// 0x27
BYTE* OP_EvalLocalVariableRefCached_Decompile(DWORD gscBuffer, BYTE* opcodesPtr)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	AddString("// OP_EvalLocalVariableRefCached( %d ); ( %s )", true, *(BYTE*)(currentPos), StackLocalGetValue(*(BYTE*)(currentPos)));
	WriteRegisterInfo((BYTE*)gscBuffer, currentPos - 1);
	}

	sprintf_s(VariableNameBuffer, "%s", StackLocalGetValue(*(BYTE*)(currentPos)));

	currentPos += 1;

	return currentPos;
}

// 0x28
BYTE* OP_SetVariableField_Decompile(DWORD gscBuffer, BYTE* opcodesPtr)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	AddString("// OP_SetVariableField();", true);
	WriteRegisterInfo((BYTE*)gscBuffer, currentPos - 1);
	}

	AddString("%s = %s;\n", true, VariableNameBuffer, StackGetLastValue());

	StackPop();

	return currentPos;
}

// 0x2B
BYTE* OP_wait_Decompile(DWORD gscBuffer, BYTE* opcodesPtr)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	AddString("// OP_wait();", true);
	WriteRegisterInfo((BYTE*)gscBuffer, currentPos - 1);
	}

	return currentPos;
}

// 0x2C
BYTE* OP_waittillFrameEnd_Decompile(DWORD gscBuffer, BYTE* opcodesPtr)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	AddString("// OP_waittillFrameEnd();", true);
	WriteRegisterInfo((BYTE*)gscBuffer, currentPos - 1);
	}

	return currentPos;
}

// 0x2D
BYTE* OP_PreScriptCall_Decompile(DWORD gscBuffer, BYTE* opcodesPtr)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	AddString("// OP_PreScriptCall();", true);
	WriteRegisterInfo((BYTE*)gscBuffer, currentPos - 1);
	}

	StackPush<BYTE*>(currentPos, type_precodepos);

	return currentPos;
}

// 0x2E
BYTE* OP_ScriptFunctionCall_Decompile(DWORD gscBuffer, BYTE* opcodesPtr)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte

	char* Function = (char*)(gscBuffer + *(DWORD*)(GET_ALIGNED_DWORD(currentPos + 1)));
	
	if (opcode_dec) {
	AddString("// OP_ScriptFunctionCall( \"%s\" );", true, Function);
	WriteRegisterInfo((BYTE*)gscBuffer, currentPos - 1);
	}

	currentPos = GET_ALIGNED_DWORD(currentPos + 1) + 4;

	call_decompile(Function, true, NULL, false, false, *(BYTE*)(currentPos) == OP_DecTop);

	return currentPos;
}

// 0x30
BYTE* OP_ScriptMethodCall_Decompile(DWORD gscBuffer, BYTE* opcodesPtr)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte

	char* Method = (char*)(gscBuffer + *(DWORD*)(GET_ALIGNED_DWORD(currentPos + 1)));
	
	if (opcode_dec) {
	AddString("// OP_ScriptMethodCall( %d, \"%s\" );", true, *(BYTE*)currentPos, gscBuffer + *(DWORD*)(GET_ALIGNED_DWORD(currentPos + 1)));
	WriteRegisterInfo((BYTE*)gscBuffer, currentPos - 1);
	}
	
	if (*(BYTE*)currentPos != 0) // delete later
	{
		cout << "OMG LULZ METHODTHREADCALL HAS %d DIFFERENT LOLZ LULZ LILZ" << endl;
		cin.get();
	}

	currentPos = GET_ALIGNED_DWORD(currentPos + 1) + 4;

	call_decompile(Method, true, NULL, true, false, *(BYTE*)(currentPos) == OP_DecTop);

	return currentPos;
}
// 0x32
BYTE* OP_ScriptThreadCall_Decompile(DWORD gscBuffer, BYTE* opcodesPtr)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	AddString("// OP_ScriptThreadCall( %d, \"%s\" );", true, *(BYTE*)currentPos, gscBuffer + *(DWORD*)(GET_ALIGNED_DWORD(currentPos + 1)));
	WriteRegisterInfo((BYTE*)gscBuffer, currentPos - 1);
	}

	currentPos = GET_ALIGNED_DWORD(currentPos + 1) + 4;

	return currentPos;
}


// 0x34
BYTE* OP_ScriptMethodThreadCall_Decompile(DWORD gscBuffer, BYTE* opcodesPtr)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte

	char* MethodThread = (char*)(gscBuffer + *(DWORD*)(GET_ALIGNED_DWORD(currentPos + 1)));
	
	if (opcode_dec) {
	AddString("// OP_ScriptMethodThreadCall( %d, \"%s\" );", true, *(BYTE*)currentPos, MethodThread);
	WriteRegisterInfo((BYTE*)gscBuffer, currentPos - 1);
	}

	if (*(BYTE*)currentPos != 0) // delete later
	{
		cout << "OMG LULZ METHODTHREADCALL HAS %d DIFFERENT LOLZ LULZ LILZ" << endl;
		cin.get();
	}

	currentPos = GET_ALIGNED_DWORD(currentPos + 1) + 4; // show byte...?

	call_decompile(MethodThread, true, NULL, true, true, *(BYTE*)(currentPos) == OP_DecTop);

	return currentPos;
}

// 0x36
BYTE* OP_DecTop_Decompile(DWORD gscBuffer, BYTE* opcodesPtr)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte

	if (opcode_dec) {
	AddString("// OP_DecTop();", true);
	WriteRegisterInfo((BYTE*)gscBuffer, currentPos - 1);
	}

	StackPop();

	return currentPos;
}

// 0x37
BYTE* OP_CastFieldObject_Decompile(DWORD gscBuffer, BYTE* opcodesPtr)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	AddString("// OP_CastFieldObject();", true);
	WriteRegisterInfo((BYTE*)gscBuffer, currentPos - 1);
	}

	CurrentObject = OBJECT_CASTED;

	CastedObject = MallocAndSprintf("%s", StackGetLastValue());

	StackPop();

	return currentPos;
}

// 0x38
BYTE* OP_CastBool_Decompile(DWORD gscBuffer, BYTE* opcodesPtr)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	AddString("// OP_CastBool();", true);
	WriteRegisterInfo((BYTE*)gscBuffer, currentPos - 1);
	}

	return currentPos;
}

// 0x39
BYTE* OP_BoolNot_Decompile(DWORD gscBuffer, BYTE* opcodesPtr)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	AddString("// OP_BoolNot();", true);
	WriteRegisterInfo((BYTE*)gscBuffer, currentPos - 1);
	}

	StackSetFlag(OP_FLAG_BOOL_NOT); // are flags even needed? :P

	char* BoolNot = MallocAndSprintf("!(%s)", StackGetLastValue());

	StackPop();
	StackPush<char*>(BoolNot, type_decompiled_string);

	return currentPos;
}

// 0x3A
BYTE* OP_BoolComplement_Decompile(DWORD gscBuffer, BYTE* opcodesPtr)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	AddString("// OP_BoolComplement();", true);
	WriteRegisterInfo((BYTE*)gscBuffer, currentPos - 1);
	}

	return currentPos;
}

// 0x3B
BYTE* OP_JumpOnFalse_Decompile(DWORD gscBuffer, BYTE* opcodesPtr)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte

	if (opcode_dec) {
	AddString("// OP_JumpOnFalse( 0x%X );", true, *(WORD*)(GET_ALIGNED_WORD(currentPos)));
	WriteRegisterInfo((BYTE*)gscBuffer, currentPos - 1);
	}

	/*if (*(__int16*)(GET_ALIGNED_WORD(currentPos)) < 0)
	{
		cout << "Error: negative conditional jump" << endl;
		cin.get();
		ExitProcess(-1);
	}*/

	DWORD jumpTo = (DWORD)GET_ALIGNED_WORD(currentPos) + *(__int16*)(GET_ALIGNED_WORD(currentPos)) + 2;

	currentPos = GET_ALIGNED_WORD(currentPos) + 2;

	if (*(__int16*)(GET_ALIGNED_WORD(currentPos)) < 0) // exists in _utility.gsc, not handled yet
		return currentPos;

	jump_expression_decompile(*(BYTE*)(opcodesPtr), jumpTo);

	return currentPos;
}

// 0x3C
BYTE* OP_JumpOnTrue_Decompile(DWORD gscBuffer, BYTE* opcodesPtr)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	AddString("// OP_JumpOnTrue( 0x%X );", true, *(WORD*)(GET_ALIGNED_WORD(currentPos)));
	WriteRegisterInfo((BYTE*)gscBuffer, currentPos - 1);
	}
	
	/*if (*(__int16*)(GET_ALIGNED_WORD(currentPos)) < 0)
	{
		cout << "Error: negative conditional jump" << endl;
		cin.get();
		ExitProcess(-1);
	}*/

	DWORD jumpTo = (DWORD)GET_ALIGNED_WORD(currentPos) + *(__int16*)(GET_ALIGNED_WORD(currentPos)) + 2;

	currentPos = GET_ALIGNED_WORD(currentPos) + 2;

	if (*(__int16*)(GET_ALIGNED_WORD(currentPos)) < 0) // exists in _utility.gsc, not handled yet
		return currentPos;

	jump_expression_decompile(*(BYTE*)(opcodesPtr), jumpTo);

	return currentPos;
}

// 0x3D
BYTE* OP_JumpOnFalseExpr_Decompile(DWORD gscBuffer, BYTE* opcodesPtr)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	AddString("// OP_JumpOnFalseExpr( 0x%X );", true, *(WORD*)(GET_ALIGNED_WORD(currentPos)));
	WriteRegisterInfo((BYTE*)gscBuffer, currentPos - 1);
	}
	
	/*if (*(__int16*)(GET_ALIGNED_WORD(currentPos)) < 0)
	{
		cout << "Error: negative conditional jump" << endl;
		cin.get();
		ExitProcess(-1);
	}*/

	DWORD jumpTo = (DWORD)GET_ALIGNED_WORD(currentPos) + *(__int16*)(GET_ALIGNED_WORD(currentPos)) + 2;

	currentPos = GET_ALIGNED_WORD(currentPos) + 2;

	if (*(__int16*)(GET_ALIGNED_WORD(currentPos)) < 0) // exists in _utility.gsc, not handled yet
		return currentPos;

	jump_expression_decompile(*(BYTE*)(opcodesPtr), jumpTo);

	return currentPos;
}

// 0x3E
BYTE* OP_JumpOnTrueExpr_Decompile(DWORD gscBuffer, BYTE* opcodesPtr)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	AddString("// OP_JumpOnTrueExpr( 0x%X );", true, *(WORD*)(GET_ALIGNED_WORD(currentPos)));
	WriteRegisterInfo((BYTE*)gscBuffer, currentPos - 1);
	}
	
	/*if (*(__int16*)(GET_ALIGNED_WORD(currentPos)) < 0)
	{
		cout << "Error: negative conditional jump" << endl;
		cin.get();
		ExitProcess(-1);
	}*/

	DWORD jumpTo = (DWORD)GET_ALIGNED_WORD(currentPos) + *(__int16*)(GET_ALIGNED_WORD(currentPos)) + 2;

	currentPos = GET_ALIGNED_WORD(currentPos) + 2;

	if (*(__int16*)(GET_ALIGNED_WORD(currentPos)) < 0) // exists in _utility.gsc, not handled yet
		return currentPos;

	jump_expression_decompile(*(BYTE*)(opcodesPtr), jumpTo);

	return currentPos;
}

// 0x3F
BYTE* OP_jump_Decompile(DWORD gscBuffer, BYTE* opcodesPtr)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	AddString("// OP_jump( 0x%X ( %d ) ); ( 0x%X )", true, *(WORD*)(GET_ALIGNED_WORD(currentPos)), *(__int16*)(GET_ALIGNED_WORD(currentPos)), (DWORD)GET_ALIGNED_WORD(currentPos) + *(__int16*)(GET_ALIGNED_WORD(currentPos)) + 2 - gscBuffer);
	WriteRegisterInfo((BYTE*)gscBuffer, currentPos - 1);
	}

	currentPos = GET_ALIGNED_WORD(currentPos) + 2;

	return currentPos;
}

// 0x41
BYTE* OP_inc_Decompile(DWORD gscBuffer, BYTE* opcodesPtr)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	AddString("// OP_PreScriptCall();", true);
	WriteRegisterInfo((BYTE*)gscBuffer, currentPos - 1);
	}

	return currentPos;
}

// 0x42
BYTE* OP_dec_Decompile(DWORD gscBuffer, BYTE* opcodesPtr)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	AddString("// OP_PreScriptCall();", true);
	WriteRegisterInfo((BYTE*)gscBuffer, currentPos - 1);
	}

	return currentPos;
}

// 0x43
BYTE* OP_bit_or_Decompile(DWORD gscBuffer, BYTE* opcodesPtr)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	AddString("// OP_bit_or();", true);
	WriteRegisterInfo((BYTE*)gscBuffer, currentPos - 1);
	}

	operator_decompile(OP_BIT_OR);

	return currentPos;
}

// 0x44
BYTE* OP_bit_ex_or_Decompile(DWORD gscBuffer, BYTE* opcodesPtr)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	AddString("// OP_bit_ex_or();", true);
	WriteRegisterInfo((BYTE*)gscBuffer, currentPos - 1);
	}

	operator_decompile(OP_BIT_EX_OR);

	return currentPos;
}

// 0x45
BYTE* OP_bit_and_Decompile(DWORD gscBuffer, BYTE* opcodesPtr)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	AddString("// OP_bit_and();", true);
	WriteRegisterInfo((BYTE*)gscBuffer, currentPos - 1);
	}

	operator_decompile(OP_BIT_AND);

	return currentPos;
}

// 0x46
BYTE* OP_equality_Decompile(DWORD gscBuffer, BYTE* opcodesPtr)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	AddString("// OP_equality();", true);
	WriteRegisterInfo((BYTE*)gscBuffer, currentPos - 1);
	}

	operator_decompile(OP_EQUALITY);

	return currentPos;
}

// 0x47
BYTE* OP_inequality_Decompile(DWORD gscBuffer, BYTE* opcodesPtr)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	AddString("// OP_inequality();", true);
	WriteRegisterInfo((BYTE*)gscBuffer, currentPos - 1);
	}

	operator_decompile(OP_INEQUALITY);

	return currentPos;
}

// 0x48
BYTE* OP_less_Decompile(DWORD gscBuffer, BYTE* opcodesPtr)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	AddString("// OP_less();", true);
	WriteRegisterInfo((BYTE*)gscBuffer, currentPos - 1);
	}

	operator_decompile(OP_LESS);

	return currentPos;
}

// 0x49
BYTE* OP_greater_Decompile(DWORD gscBuffer, BYTE* opcodesPtr)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	AddString("// OP_greater();", true);
	WriteRegisterInfo((BYTE*)gscBuffer, currentPos - 1);
	}

	operator_decompile(OP_GREATER);

	return currentPos;
}

// 0x4A
BYTE* OP_less_equal_Decompile(DWORD gscBuffer, BYTE* opcodesPtr)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	AddString("// OP_less_equal();", true);
	WriteRegisterInfo((BYTE*)gscBuffer, currentPos - 1);
	}

	operator_decompile(OP_LESS_EQUAL);

	return currentPos;
}

// 0x4B
BYTE* OP_greater_equal_Decompile(DWORD gscBuffer, BYTE* opcodesPtr)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	AddString("// OP_greater_equal();", true);
	WriteRegisterInfo((BYTE*)gscBuffer, currentPos - 1);
	}

	operator_decompile(OP_GREATER_EQUAL);

	return currentPos;
}

// 0x4C
BYTE* OP_shift_left_Decompile(DWORD gscBuffer, BYTE* opcodesPtr)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	AddString("// OP_shift_left();", true);
	WriteRegisterInfo((BYTE*)gscBuffer, currentPos - 1);
	}

	operator_decompile(OP_SHIFT_LEFT);

	return currentPos;
}

// 0x4D
BYTE* OP_shift_right_Decompile(DWORD gscBuffer, BYTE* opcodesPtr)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	AddString("// OP_shift_right();", true);
	WriteRegisterInfo((BYTE*)gscBuffer, currentPos - 1);
	}

	operator_decompile(OP_SHIFT_RIGHT);

	return currentPos;
}

// 0x4E
BYTE* OP_plus_Decompile(DWORD gscBuffer, BYTE* opcodesPtr)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	AddString("// OP_plus();", true);
	WriteRegisterInfo((BYTE*)gscBuffer, currentPos - 1);
	}

	operator_decompile(OP_PLUS);

	return currentPos;
}

// 0x4F
BYTE* OP_minus_Decompile(DWORD gscBuffer, BYTE* opcodesPtr)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	AddString("// OP_minus();", true);
	WriteRegisterInfo((BYTE*)gscBuffer, currentPos - 1);
	}

	operator_decompile(OP_MINUS);

	return currentPos;
}

// 0x50
BYTE* OP_multiply_Decompile(DWORD gscBuffer, BYTE* opcodesPtr)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	AddString("// OP_multiply();", true);
	WriteRegisterInfo((BYTE*)gscBuffer, currentPos - 1);
	}

	operator_decompile(OP_MULTIPLY);

	return currentPos;
}

// 0x51
BYTE* OP_divide_Decompile(DWORD gscBuffer, BYTE* opcodesPtr)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	AddString("// OP_divide();", true);
	WriteRegisterInfo((BYTE*)gscBuffer, currentPos - 1);
	}

	operator_decompile(OP_DIVIDE);

	return currentPos;
}

// 0x52
BYTE* OP_mod_Decompile(DWORD gscBuffer, BYTE* opcodesPtr)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	AddString("// OP_mod();", true);
	WriteRegisterInfo((BYTE*)gscBuffer, currentPos - 1);
	}

	operator_decompile(OP_MOD);

	return currentPos;
}

// 0x53
BYTE* OP_size_Decompile(DWORD gscBuffer, BYTE* opcodesPtr)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	AddString("// OP_size();", true);
	WriteRegisterInfo((BYTE*)gscBuffer, currentPos - 1);
	}

	char* SizeAppended = MallocAndSprintf("%s.size", StackGetLastValue());

	StackPop(); // pop so it frees the old decompiled string
	StackPush<char*>(SizeAppended, type_decompiled_string);

	return currentPos;
}

// 0x54
BYTE* OP_waittillmatch_Decompile(DWORD gscBuffer, BYTE* opcodesPtr)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	AddString("// OP_waittillmatch();", true);
	WriteRegisterInfo((BYTE*)gscBuffer, currentPos - 1);
	}

	return currentPos;
}

// 0x55
BYTE* OP_waittill_Decompile(DWORD gscBuffer, BYTE* opcodesPtr)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	AddString("// OP_waittill();", true);
	WriteRegisterInfo((BYTE*)gscBuffer, currentPos - 1);
	}
	
	call_decompile("waittill", false, 1, true, false, true);

	return currentPos;
}

// 0x56
BYTE* OP_notify_Decompile(DWORD gscBuffer, BYTE* opcodesPtr)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	AddString("// OP_notify();", true);
	WriteRegisterInfo((BYTE*)gscBuffer, currentPos - 1);
	}

	return currentPos;
}

// 0x57
BYTE* OP_endon_Decompile(DWORD gscBuffer, BYTE* opcodesPtr)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	AddString("// OP_endon();", true);
	WriteRegisterInfo((BYTE*)gscBuffer, currentPos - 1);
	}

	call_decompile("endon", false, 1, true, false, true);

	return currentPos;
}

// 0x58
BYTE* OP_voidCodepos_Decompile(DWORD gscBuffer, BYTE* opcodesPtr)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	AddString("// OP_voidCodepos();", true);
	WriteRegisterInfo((BYTE*)gscBuffer, currentPos - 1);
	}

	StackPush<BYTE*>(currentPos, type_precodepos);

	return currentPos;
}

BYTE* OP_switch_Decompile(DWORD gscBuffer, BYTE* opcodesPtr)
{
//	(*(DWORD *)GET_ALIGNED_DWORD(opcodesPtr) + GET_ALIGNED_DWORD(opcodesPtr) + 7) & 0xFFFFFFFC;

//	((opcodesPtr + 3) & 0xFFFFFFFC)

//	BYTE* ofs = (*(DWORD *)((opcodesPtr + 3) & 0xFFFFFFFC) + ((opcodesPtr + 3) & 0xFFFFFFFC) + 7) & 0xFFFFFFFC;

	opcodesPtr += 1;

	return opcodesPtr;
}

// 0x5B
BYTE* OP_vector_Decompile(DWORD gscBuffer, BYTE* opcodesPtr)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	AddString("// OP_vector();", true);
	WriteRegisterInfo((BYTE*)gscBuffer, currentPos - 1);
	}

	char* VectorString = MallocAndSprintf("( %s, %s, %s )", StackGetValue(0), StackGetValue(1), StackGetValue(2));

	StackPop();
	StackPop();
	StackPop();

	StackPush<char*>(VectorString, type_decompiled_string);

	return currentPos;
}

// 0x5C
BYTE* OP_GetHash_Decompile(DWORD gscBuffer, BYTE* opcodesPtr)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte

	char* Hash = MallocAndSprintf("#\"0x%X\"", *(DWORD*)GET_ALIGNED_DWORD(currentPos));
	
	if (opcode_dec) {
	AddString("// OP_GetHash( %s );", true, Hash);
	WriteRegisterInfo((BYTE*)gscBuffer, currentPos - 1);
	}

	StackPush<char*>(Hash, type_decompiled_string);

	currentPos = GET_ALIGNED_DWORD(currentPos) + 4;

	return currentPos;
}

// 0x5F
BYTE* OP_isdefined_Decompile(DWORD gscBuffer, BYTE* opcodesPtr)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	AddString("// OP_isdefined();", true);
	WriteRegisterInfo((BYTE*)gscBuffer, currentPos - 1);
	}

	call_decompile("IsDefined", false, 1, false, false, *(BYTE*)(currentPos) == OP_DecTop);

	return currentPos;
}

// 0x67
BYTE* OP_gettime_Decompile(DWORD gscBuffer, BYTE* opcodesPtr)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	AddString("// OP_gettime();", true);
	WriteRegisterInfo((BYTE*)gscBuffer, currentPos - 1);
	}

	call_decompile("GetTime", false, 0, false, false, *(BYTE*)(currentPos) == OP_DecTop);

	return currentPos;
}

// 0x68
BYTE* OP_getdvar_Decompile(DWORD gscBuffer, BYTE* opcodesPtr)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	AddString("// OP_getdvar();", true);
	WriteRegisterInfo((BYTE*)gscBuffer, currentPos - 1);
	}

	call_decompile("GetDvar", false, 1, false, false, *(BYTE*)(currentPos) == OP_DecTop);

	return currentPos;
}

// 0x69
BYTE* OP_getdvarint_Decompile(DWORD gscBuffer, BYTE* opcodesPtr)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	AddString("// OP_getdvarint();", true);
	WriteRegisterInfo((BYTE*)gscBuffer, currentPos - 1);
	}

	call_decompile("GetDvarInt", false, 1, false, false, *(BYTE*)(currentPos) == OP_DecTop);

	return currentPos;
}

// 0x6A
BYTE* OP_getdvarfloat_Decompile(DWORD gscBuffer, BYTE* opcodesPtr) // need to make a function to set current stack pointer (this will set type to type_function_call)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	AddString("// OP_getdvarfloat();", true);
	WriteRegisterInfo((BYTE*)gscBuffer, currentPos - 1);
	}

	call_decompile("GetDvarFloat", false, 1, false, false, *(BYTE*)(currentPos) == OP_DecTop);

	return currentPos;
}

// 0x6B
BYTE* OP_getdvarvector_Decompile(DWORD gscBuffer, BYTE* opcodesPtr)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	AddString("// OP_getdvarvector();", true);
	WriteRegisterInfo((BYTE*)gscBuffer, currentPos - 1);
	}

	return currentPos;
}

// 0x70
BYTE* OP_GetFirstArrayKey_Decompile(DWORD gscBuffer, BYTE* opcodesPtr)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	AddString("// OP_GetFirstArrayKey( %s );", true, StackGetLastValue());
	WriteRegisterInfo((BYTE*)gscBuffer, currentPos - 1);
	}

	// Stack incremented by 1
	//
	// params:
	//	[Array]  Array to look up
	// returns:
	//	[String, int, object] Key
	call_decompile("GetFirstArrayKey", false, 1, false, false, *(BYTE*)(currentPos) == OP_DecTop);

	return currentPos;
}

// 0x71
BYTE* OP_GetNextArrayKey_Decompile(DWORD gscBuffer, BYTE* opcodesPtr)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	AddString("// OP_GetNextArrayKey( %s, %s );", true, StackGetValue(0), StackGetValue(1));
	WriteRegisterInfo((BYTE*)gscBuffer, currentPos - 1);
	}

	// Stack incremented by 1
	//
	// params:
	//	[Array]               Array to look up
	//	[String, int, object] Previous key
	// returns:
	//	[String, int, object] Key
	call_decompile("GetNextArrayKey", false, 2, false, false, *(BYTE*)(currentPos) == OP_DecTop);

	return currentPos;
}

// 0x73
BYTE* OP_GetUndefined2_Decompile(DWORD gscBuffer, BYTE* opcodesPtr)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte

	if (opcode_dec) {
	AddString("// OP_GetUndefined2();", true);
	WriteRegisterInfo((BYTE*)gscBuffer, currentPos - 1);
	}

	return currentPos;
}

// 0x7B
BYTE* OP_skipdev_Decompile(DWORD gscBuffer, BYTE* opcodesPtr)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	AddString("// OP_skipdev( 0x%X ( %d ) );", true, *(WORD*)(GET_ALIGNED_WORD(currentPos)), *(__int16*)(GET_ALIGNED_WORD(currentPos)));
	WriteRegisterInfo((BYTE*)gscBuffer, currentPos - 1);
	}

	currentPos = GET_ALIGNED_WORD(currentPos) + 2;

	return currentPos;
}