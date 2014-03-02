#include "stdafx.h"

using namespace std;

#define DEF_DECOMPILE(name) void GSCDecompilerClass::OP_ ## name ## _Decompile()
#define CASE_DECOMPILE(name) case OP_ ## name ## : OP_ ## name ## _Decompile(); break

void GSCDecompilerClass::DecompilerOut(char* format, bool addTabLevel, ...)
{
	va_list ap;
	
	va_start(ap, addTabLevel);
	int length = _vscprintf(format, ap);
	va_end(ap);

	char* allocatedString = (char*)malloc(length + 1);

	va_start(ap, addTabLevel);
	_vsnprintf_s(allocatedString, length + 1, length + 1, format, ap);
	va_end(ap);

	if (addTabLevel)
	{
		for (DWORD i = 0; i < curTabLevel; i++)
			decompiledBuffer.append("\t");
	}

	decompiledBuffer.append(allocatedString);

	free(allocatedString);
}

// initialStack - stack that the decompiler starts with (can be null)
// start - relative pointer which points to a section of the code from the gsc
// isFunction - if start and size are the start and end of a function
// (it must be false if we are decompiling a code block only, not an entire function)
// initialTabLevel - how many tabs there need to be at each new line initially (can increase or decrease depending on the decompiled code)
string GSCDecompilerClass::decompile(vmStack_s* initialStack, DWORD gscBuffer, DWORD start, DWORD size, bool isFunction, DWORD initialTabLevel)
{
	if (initialStack)
		StackCopy(initialStack, &this->stack);

	opcodesPtr = (BYTE*)(gscBuffer + start);

	this->gscBuffer = gscBuffer;
	decompileStart = start;
	decompileSize = size;
	this->isFunction = isFunction;
	curTabLevel = initialTabLevel;

	while (opcodesPtr < (BYTE*)(gscBuffer + start + size))
	{
		switch (*opcodesPtr)
		{
			CASE_DECOMPILE(End);								// 0x00
			CASE_DECOMPILE(Return);								// 0x01
			CASE_DECOMPILE(GetUndefined);						// 0x02
			CASE_DECOMPILE(GetZero);							// 0x03
			CASE_DECOMPILE(GetByte);							// 0x04
			CASE_DECOMPILE(GetNegByte);							// 0x05
			CASE_DECOMPILE(GetUnsignedShort);					// 0x06
			CASE_DECOMPILE(GetNegUnsignedShort);				// 0x07
			CASE_DECOMPILE(GetInteger);							// 0x08
			CASE_DECOMPILE(GetFloat);							// 0x09
			CASE_DECOMPILE(GetString);							// 0x0A
			CASE_DECOMPILE(GetIString);							// 0x0B
			CASE_DECOMPILE(GetVector);							// 0x0C
			CASE_DECOMPILE(GetLevelObject);						// 0x0D
			CASE_DECOMPILE(GetAnimObject);						// 0x0E
			CASE_DECOMPILE(GetSelf);							// 0x0F
			CASE_DECOMPILE(GetLevel);							// 0x10
			CASE_DECOMPILE(GetGame);							// 0x11
			CASE_DECOMPILE(GetAnim);							// 0x12
			CASE_DECOMPILE(GetAnimation);						// 0x13
			CASE_DECOMPILE(GetGameRef);							// 0x14
			CASE_DECOMPILE(GetFunction);						// 0x15
			CASE_DECOMPILE(CreateLocalVariable);				// 0x16
			CASE_DECOMPILE(CreateLocalVariables);				// 0x17
			CASE_DECOMPILE(EvalLocalVariableCached);			// 0x19
			CASE_DECOMPILE(EvalArray);							// 0x1A
			CASE_DECOMPILE(EvalArrayRef);						// 0x1C
			CASE_DECOMPILE(ClearArray);							// 0x1D
			CASE_DECOMPILE(EmptyArray);							// 0x1E
			CASE_DECOMPILE(GetSelfObject);						// 0x1F
			CASE_DECOMPILE(EvalFieldVariable);					// 0x20
			CASE_DECOMPILE(EvalFieldVariableRef);				// 0x21
			CASE_DECOMPILE(ClearFieldVariable);					// 0x22
			CASE_DECOMPILE(SafeSetWaittillVariableFieldCached);	// 0x24
			CASE_DECOMPILE(clearparams);						// 0x25
			CASE_DECOMPILE(checkclearparams);					// 0x26
			CASE_DECOMPILE(EvalLocalVariableRefCached);			// 0x27
			CASE_DECOMPILE(SetVariableField);					// 0x28
			CASE_DECOMPILE(wait);								// 0x2B
			CASE_DECOMPILE(waittillFrameEnd);					// 0x2C
			CASE_DECOMPILE(PreScriptCall);						// 0x2D
			CASE_DECOMPILE(ScriptFunctionCall);					// 0x2E
			CASE_DECOMPILE(ScriptFunctionCallPointer);			// 0x2F
			CASE_DECOMPILE(ScriptMethodCall);					// 0x30
			CASE_DECOMPILE(ScriptMethodCallPointer);			// 0x31
			CASE_DECOMPILE(ScriptThreadCall);					// 0x32
			CASE_DECOMPILE(ScriptThreadCallPointer);			// 0x33
			CASE_DECOMPILE(ScriptMethodThreadCall);				// 0x34
			CASE_DECOMPILE(ScriptMethodThreadCallPointer);		// 0x35
			CASE_DECOMPILE(DecTop);								// 0x36
			CASE_DECOMPILE(CastFieldObject);					// 0x37
			CASE_DECOMPILE(CastBool);							// 0x38
			CASE_DECOMPILE(BoolNot);							// 0x39
			CASE_DECOMPILE(BoolComplement);						// 0x3A
			CASE_DECOMPILE(JumpOnFalse);						// 0x3B
			CASE_DECOMPILE(JumpOnTrue);							// 0x3C
			CASE_DECOMPILE(JumpOnFalseExpr);					// 0x3D
			CASE_DECOMPILE(JumpOnTrueExpr);						// 0x3E
			CASE_DECOMPILE(jump);								// 0x3F
			CASE_DECOMPILE(inc);								// 0x41
			CASE_DECOMPILE(dec);								// 0x42
			CASE_DECOMPILE(bit_or);								// 0x43
			CASE_DECOMPILE(bit_ex_or);							// 0x44
			CASE_DECOMPILE(bit_and);							// 0x45
			CASE_DECOMPILE(equality);							// 0x46
			CASE_DECOMPILE(inequality);							// 0x47
			CASE_DECOMPILE(less);								// 0x48
			CASE_DECOMPILE(greater);							// 0x49
			CASE_DECOMPILE(less_equal);							// 0x4A
			CASE_DECOMPILE(greater_equal);						// 0x4B
			CASE_DECOMPILE(shift_left);							// 0x4C
			CASE_DECOMPILE(shift_right);						// 0x4D
			CASE_DECOMPILE(plus);								// 0x4E
			CASE_DECOMPILE(minus);								// 0x4F
			CASE_DECOMPILE(multiply);							// 0x50
			CASE_DECOMPILE(divide);								// 0x51
			CASE_DECOMPILE(mod);								// 0x52
			CASE_DECOMPILE(size);								// 0x53
			CASE_DECOMPILE(waittillmatch);						// 0x54
			CASE_DECOMPILE(waittill);							// 0x55
			CASE_DECOMPILE(notify);								// 0x56
			CASE_DECOMPILE(endon);								// 0x57
			CASE_DECOMPILE(voidCodepos);						// 0x58
			CASE_DECOMPILE(switch);								// 0x59
			CASE_DECOMPILE(endswitch);							// 0x5A
			CASE_DECOMPILE(vector);								// 0x5B
			CASE_DECOMPILE(GetHash);							// 0x5C
			CASE_DECOMPILE(GetSimpleVector);					// 0x5E
			CASE_DECOMPILE(isdefined);							// 0x5F
			CASE_DECOMPILE(vectorscale);						// 0x60
			CASE_DECOMPILE(anglestoup);							// 0x61
			CASE_DECOMPILE(anglestoright);						// 0x62
			CASE_DECOMPILE(anglestoforward);					// 0x63
			CASE_DECOMPILE(angleclamp180);						// 0x64
			CASE_DECOMPILE(vectorstoangle);						// 0x65
			CASE_DECOMPILE(abs);								// 0x66
			CASE_DECOMPILE(gettime);							// 0x67
			CASE_DECOMPILE(getdvar);							// 0x68
			CASE_DECOMPILE(getdvarint);							// 0x69
			CASE_DECOMPILE(getdvarfloat);						// 0x6A
			CASE_DECOMPILE(getdvarvector);						// 0x6B
			CASE_DECOMPILE(getdvarcolorred);					// 0x6C
			CASE_DECOMPILE(getdvarcolorgreen);					// 0x6D
			CASE_DECOMPILE(getdvarcolorblue);					// 0x6E
			CASE_DECOMPILE(getdvarcoloralpha);					// 0x6F
			CASE_DECOMPILE(GetFirstArrayKey);					// 0x70
			CASE_DECOMPILE(GetNextArrayKey);					// 0x71
			CASE_DECOMPILE(GetUndefined2);						// 0x73
			CASE_DECOMPILE(skipdev);							// 0x7B
		default:
			DecompilerOut("/* Error: unknown opcode (0x%X) */\n", true, *opcodesPtr);
			goto end_opcode_decompilation;
		}
	}
end_opcode_decompilation:

	if (isFunction)
	{
		if (StackGetRelativePos() == 0)
			DecompilerOut("// SP = 0x%X - check OK", false, StackGetRelativePos());
		else
			DecompilerOut("// SP = 0x%X - check failed (function may have been decompiled incorrectly)", false, StackGetRelativePos());
	}

	return decompiledBuffer;
}

// 0x0
DEF_DECOMPILE(End)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	DecompilerOut("// OP_End();", true);
	WriteRegisterInfo(currentPos - 1);
	}

	if (isFunction && !(opcodesPtr == (BYTE*)(gscBuffer + decompileStart + decompileSize - 1)))
		DecompilerOut("return;\n", true);

	opcodesPtr = currentPos;
}

// 0x1
DEF_DECOMPILE(Return)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	DecompilerOut("// OP_Return();", true);
	WriteRegisterInfo(currentPos - 1);
	}

	DecompilerOut("return %s;\n", true, StackGetValue(0));

	StackPop(); // the game obviously doesn't do it, but we need to do it

	opcodesPtr = currentPos;
}

// 0x2
DEF_DECOMPILE(GetUndefined)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	DecompilerOut("// OP_GetUndefined();", true);
	WriteRegisterInfo(currentPos - 1);
	}
	
	char* Undefined = MallocAndSprintf("%s", "undefined");

	StackPush<char*>(Undefined, type_decompiled_string);

	opcodesPtr = currentPos;
}

// 0x3
DEF_DECOMPILE(GetZero)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	DecompilerOut("// OP_GetZero();", true);
	WriteRegisterInfo(currentPos - 1);
	}
	
	char* Zero = MallocAndSprintf("%d", 0); // malloc because we free the string every time when poping if the type is type_decompiled_string

	StackPush<char*>(Zero, type_decompiled_string);

	opcodesPtr = currentPos;
}

// 0x4
DEF_DECOMPILE(GetByte)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte

	char* Byte = MallocAndSprintf("%d", *(unsigned __int8*)(currentPos));

	if (opcode_dec) {
	DecompilerOut("// OP_GetByte( %s );", true, Byte);
	WriteRegisterInfo(currentPos - 1);
	}

	StackPush<char*>(Byte, type_decompiled_string);

	currentPos += 1;

	opcodesPtr = currentPos;
}

// 0x5
DEF_DECOMPILE(GetNegByte)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte

	char* Byte = MallocAndSprintf("-%d", *(unsigned __int8*)(currentPos)); // right?

	if (opcode_dec) {
	DecompilerOut("// OP_GetNegByte( %s );", true, Byte);
	WriteRegisterInfo(currentPos - 1);
	}

	StackPush<char*>(Byte, type_decompiled_string);

	currentPos += 1;

	opcodesPtr = currentPos;
}

// 0x6
DEF_DECOMPILE(GetUnsignedShort)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte

	char* UnsignedShort = MallocAndSprintf("%d", *(unsigned __int16*)(GET_ALIGNED_WORD(currentPos)));

	if (opcode_dec) {
	DecompilerOut("// OP_GetUnsignedShort( %s );", true, UnsignedShort);
	WriteRegisterInfo(currentPos - 1);
	}

	StackPush<char*>(UnsignedShort, type_decompiled_string);

	currentPos = GET_ALIGNED_WORD(currentPos) + 2;

	opcodesPtr = currentPos;
}

// 0x7
DEF_DECOMPILE(GetNegUnsignedShort)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte

	char* UnsignedShort = MallocAndSprintf("-%d", *(unsigned __int16*)(GET_ALIGNED_WORD(currentPos))); // right?

	if (opcode_dec) {
	DecompilerOut("// OP_GetNegUnsignedShort( %s );", true, UnsignedShort);
	WriteRegisterInfo(currentPos - 1);
	}

	StackPush<char*>(UnsignedShort, type_decompiled_string);

	currentPos = GET_ALIGNED_WORD(currentPos) + 2;

	opcodesPtr = currentPos;
}

// 0x8
DEF_DECOMPILE(GetInteger)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte

	char* Int = MallocAndSprintf("%d", *(signed __int32*)(GET_ALIGNED_DWORD(currentPos)));

	if (opcode_dec) {
	DecompilerOut("// OP_GetInteger( %s );", true, Int);
	WriteRegisterInfo(currentPos - 1);
	}

	StackPush<char*>(Int, type_decompiled_string);

	currentPos = GET_ALIGNED_DWORD(currentPos) + 4;

	opcodesPtr = currentPos;
}

// 0x9
DEF_DECOMPILE(GetFloat)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte

	char *Float = MallocAndSprintf("%g", *(float*)(GET_ALIGNED_DWORD(currentPos)));

	if (opcode_dec) {
	DecompilerOut("// OP_GetFloat( %s );", true, Float);
	WriteRegisterInfo(currentPos - 1);
	}

	StackPush<char*>(Float, type_decompiled_string);

	currentPos = GET_ALIGNED_DWORD(currentPos) + 4;

	opcodesPtr = currentPos;
}

// 0xA
DEF_DECOMPILE(GetString)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte

	char* String = MallocAndSprintf("\"%s\"", gscBuffer + *(WORD*)(GET_ALIGNED_WORD(currentPos)));
	char* StringTemp = NULL;

	volatile size_t stringSize = strlen(String);

	for (size_t i = 0; i < stringSize; i++)
	{
		if (String[i] == '\n') // fix for strings that have '\n' in them
		{
			String[i] = 0x00;
			StringTemp = MallocAndSprintf("%s%s%s%s", String, "\\", "n", String + i + 1);

			free(String);
			String = (char*)malloc(strlen(StringTemp) + 1);
			strcpy_s(String, strlen(StringTemp) + 1, StringTemp);
			free(StringTemp);

			i++; // because we've added another character that doesn't need a check
		}
		else if (String[i] == '%') // fix for strings that have modulus in them (so DecompilerOut doesn't crash)
		{
			String[i] = 0x00;
			StringTemp = MallocAndSprintf("%s%s%s", String, "%%", String + i + 1);

			free(String);
			String = (char*)malloc(strlen(StringTemp) + 1);
			strcpy_s(String, strlen(StringTemp) + 1, StringTemp);
			free(StringTemp);

			i++; // because we've added another character that doesn't need a check
		}

		stringSize = strlen(String);
	}
	
	if (opcode_dec) {
	DecompilerOut("// OP_GetString( %s );", true, String);
	WriteRegisterInfo(currentPos - 1);
	}

	StackPush<char*>(String, type_decompiled_string);

	currentPos = GET_ALIGNED_WORD(currentPos) + 2;

	opcodesPtr = currentPos;
}

// 0xB
DEF_DECOMPILE(GetIString)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte

	char* IString = MallocAndSprintf("&\"%s\"", gscBuffer + *(WORD*)(GET_ALIGNED_WORD(currentPos)));
	
	if (opcode_dec) {
	DecompilerOut("// OP_GetIString( %s );", true, IString);
	WriteRegisterInfo(currentPos - 1);
	}

	StackPush<char*>(IString, type_decompiled_string);

	currentPos = GET_ALIGNED_WORD(currentPos) + 2;

	opcodesPtr = currentPos;
}

// 0xC
DEF_DECOMPILE(GetVector)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte

	float* Vector = (float*)GET_ALIGNED_DWORD(currentPos);
	char* VectorString = MallocAndSprintf("( %f, %f, %f )", *(Vector), *(Vector + 1), *(Vector + 2));
	
	if (opcode_dec) {
	DecompilerOut("// OP_GetVector( %s );", true, VectorString);
	WriteRegisterInfo(currentPos - 1);
	}

	StackPush<char*>(VectorString, type_decompiled_string);

	currentPos = GET_ALIGNED_DWORD(currentPos) + 12;

	opcodesPtr = currentPos;
}

// 0xD
DEF_DECOMPILE(GetLevelObject)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte

	if (opcode_dec) {
	DecompilerOut("// OP_GetLevelObject();", true);
	WriteRegisterInfo(currentPos - 1);
	}

	CurrentObject = OBJECT_LEVEL;

	opcodesPtr = currentPos;
}

// 0xE
DEF_DECOMPILE(GetAnimObject)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	DecompilerOut("// OP_GetAnimObject();", true);
	WriteRegisterInfo(currentPos - 1);
	}

	CurrentObject = OBJECT_ANIM;

	opcodesPtr = currentPos;
}

// 0xF
DEF_DECOMPILE(GetSelf)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	DecompilerOut("// OP_GetSelf();", true);
	WriteRegisterInfo(currentPos - 1);
	}
	
	char* Self = MallocAndSprintf("%s", "self");

	StackPush<char*>(Self, type_decompiled_string);

	opcodesPtr = currentPos;
}

// 0x10
DEF_DECOMPILE(GetLevel)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	DecompilerOut("// OP_GetLevel();", true);
	WriteRegisterInfo(currentPos - 1);
	}
	
	char* Level = MallocAndSprintf("%s", "level");
	
	StackPush<char*>(Level, type_decompiled_string);

	opcodesPtr = currentPos;
}

// 0x11
DEF_DECOMPILE(GetGame)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	DecompilerOut("// OP_GetGame();", true);
	WriteRegisterInfo(currentPos - 1);
	}

	char* Game = MallocAndSprintf("%s", "game");
	
	StackPush<char*>(Game, type_decompiled_string);

	opcodesPtr = currentPos;
}

// 0x12
DEF_DECOMPILE(GetAnim)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte

	if (opcode_dec) {
	DecompilerOut("// OP_GetAnim();", true);
	WriteRegisterInfo(currentPos - 1);
	}

	StackPush<char*>("anim", type_object);

	opcodesPtr = currentPos;
}

// 0x13
DEF_DECOMPILE(GetAnimation)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte

	// double modulus fix like in OP_GetString
	char* Animation = MallocAndSprintf("%s%s", "%%", gscBuffer + *(WORD*)(GET_ALIGNED_DWORD(currentPos)));
	
	if (opcode_dec) {
	DecompilerOut("// OP_GetAnimation( %s );", true, Animation);
	WriteRegisterInfo(currentPos - 1);
	}

	StackPush<char*>(Animation, type_decompiled_string);

	currentPos = GET_ALIGNED_DWORD(currentPos) + 4;

	opcodesPtr = currentPos;
}

// 0x14
DEF_DECOMPILE(GetGameRef)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	DecompilerOut("// OP_GetGameRef();", true);
	WriteRegisterInfo(currentPos - 1);
	}

	sprintf_s(VariableNameBuffer, "%s", "game");

	opcodesPtr = currentPos;
}

// 0x15
DEF_DECOMPILE(GetFunction)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	char* Function = MallocAndSprintf("::%s", gscBuffer + *(DWORD*)(GET_ALIGNED_DWORD(currentPos)));
	
	if (opcode_dec) {
	DecompilerOut("// OP_GetFunction( \"%s\" );", true, Function);
	WriteRegisterInfo(currentPos - 1);
	}

	StackPush<char*>(Function, type_decompiled_string);

	currentPos = GET_ALIGNED_DWORD(currentPos) + 4;

	opcodesPtr = currentPos;
}

// 0x16
DEF_DECOMPILE(CreateLocalVariable)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte

	if (opcode_dec) {
		DecompilerOut("// OP_CreateLocalVariable( ); ", true);
	}

	DecompilerOut("// Error: Unknown opcode \"OP_CreateLocalVariable\"", true);

	opcodesPtr = currentPos;
}

// 0x17
DEF_DECOMPILE(CreateLocalVariables)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte

	BYTE numOfVariables = *(BYTE*)(currentPos);
	currentPos += 1;

	if (opcode_dec) {
	DecompilerOut("// OP_CreateLocalVariable( %d", true, numOfVariables);
	if (numOfVariables)
		DecompilerOut(", ", false);
	}

	char* LocalVar = 0;
	
	for (BYTE i = 0; i < numOfVariables; i++)
	{
		LocalVar = (char*)(gscBuffer + *(WORD*)(GET_ALIGNED_WORD(currentPos)));

		StackLocalPush(LocalVar);

		if (opcode_dec) {
		DecompilerOut("\"%s\"", false, LocalVar);
		if ((i + 1) < numOfVariables)
			DecompilerOut(", ", false);
		}
		
		currentPos = GET_ALIGNED_WORD(currentPos) + 2;
	}

	if (opcode_dec) {
	DecompilerOut(" );", false);
	WriteRegisterInfo(currentPos - 1);
	}

	opcodesPtr = currentPos;
}

// 0x19
DEF_DECOMPILE(EvalLocalVariableCached)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte

	char* VariableName = MallocAndSprintf("%s", StackLocalGetValue(*(BYTE*)(currentPos)));
	
	if (opcode_dec) {
	DecompilerOut("// OP_EvalLocalVariableCached( %d ); ( %s )", true, *(BYTE*)(currentPos), VariableName);
	WriteRegisterInfo(currentPos - 1);
	}

	StackPush<char*>(VariableName, type_decompiled_string);

	currentPos += 1;

	opcodesPtr = currentPos;
}

// 0x1A
DEF_DECOMPILE(EvalArray)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	DecompilerOut("// OP_EvalArray();", true);
	WriteRegisterInfo(currentPos - 1);
	}

	char* Array = MallocAndSprintf("%s[%s]", StackGetValue(0), StackGetValue(1));

	StackPop();
	StackPop();
	StackPush<char*>(Array, type_decompiled_string);

	opcodesPtr = currentPos;
}

// 0x1C
DEF_DECOMPILE(EvalArrayRef)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	DecompilerOut("// OP_EvalArrayRef();", true);
	WriteRegisterInfo(currentPos - 1);
	}

	sprintf_s(VariableNameBuffer, "%s[%s]", VariableNameBuffer, StackGetValue(0));

	StackPop();

	opcodesPtr = currentPos;
}

// 0x1D
DEF_DECOMPILE(ClearArray)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	DecompilerOut("// OP_ClearArray();", true);
	WriteRegisterInfo(currentPos - 1);
	}

	DecompilerOut("%s[%s] = undefined;\n", true, VariableNameBuffer, StackGetValue(0));

	StackPop();

	opcodesPtr = currentPos;
}

// 0x1E
DEF_DECOMPILE(EmptyArray)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	DecompilerOut("// OP_EmptyArray();", true);
	WriteRegisterInfo(currentPos - 1);
	}

	char* EmptyArray = MallocAndSprintf("%s", "[]");

	StackPush<char*>(EmptyArray, type_decompiled_string);

	opcodesPtr = currentPos;
}

// 0x1F
DEF_DECOMPILE(GetSelfObject)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte

	if (opcode_dec) {
	DecompilerOut("// OP_GetSelfObject();", true);
	WriteRegisterInfo(currentPos - 1);
	}

	CurrentObject = OBJECT_SELF;

	opcodesPtr = currentPos;
}

// 0x20
DEF_DECOMPILE(EvalFieldVariable)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte

	char* FieldVariable = MallocAndSprintf("%s.%s", GetStringForCurrentObject(), gscBuffer + *(WORD*)(GET_ALIGNED_WORD(currentPos)));

	if (opcode_dec) {
	DecompilerOut("// OP_EvalFieldVariable( %s );", true, FieldVariable);
	WriteRegisterInfo(currentPos - 1);
	}

	StackPush<char*>(FieldVariable, type_decompiled_string);

	currentPos = GET_ALIGNED_WORD(currentPos) + 2;

	opcodesPtr = currentPos;
}

// 0x21
DEF_DECOMPILE(EvalFieldVariableRef)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte

	char* FieldVariable = (char*)(gscBuffer + *(WORD*)(GET_ALIGNED_WORD(currentPos)));
	char* FieldOwner = GetStringForCurrentObject();

	if (opcode_dec) {
	DecompilerOut("// OP_EvalFieldVariableRef( \"%s\" ); (%s.%s)", true, FieldVariable, FieldOwner, FieldVariable);
	WriteRegisterInfo(currentPos - 1);
	}

	sprintf_s(VariableNameBuffer, "%s.%s", FieldOwner, FieldVariable);

	currentPos = GET_ALIGNED_WORD(currentPos) + 2;

	opcodesPtr = currentPos;
}

// 0x22
DEF_DECOMPILE(ClearFieldVariable)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte

	char* FieldVariable = (char*)(gscBuffer + *(WORD*)(GET_ALIGNED_WORD(currentPos)));
	char* FieldOwner = GetStringForCurrentObject();

	if (opcode_dec) {
	DecompilerOut("// OP_ClearFieldVariable( \"%s\" );", true, gscBuffer + *(WORD*)(GET_ALIGNED_WORD(currentPos)));
	WriteRegisterInfo(currentPos - 1);
	}

	DecompilerOut("%s.%s = undefined;\n", true, FieldOwner, FieldVariable);

	currentPos = GET_ALIGNED_WORD(currentPos) + 2;

	opcodesPtr = currentPos;
}

// 0x24
DEF_DECOMPILE(SafeSetWaittillVariableFieldCached)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte

	if (opcode_dec) {
	DecompilerOut("// OP_SafeSetWaittillVariableFieldCached( %d );", true, *(BYTE*)(currentPos));
	WriteRegisterInfo(currentPos - 1);
	}

	currentPos += 1;

	opcodesPtr = currentPos;
}

// 0x25
DEF_DECOMPILE(clearparams)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte

	if (opcode_dec) {
	DecompilerOut("// OP_clearparams();", true);
	WriteRegisterInfo(currentPos - 1);
	}

	opcodesPtr = currentPos;
}

// 0x26
DEF_DECOMPILE(checkclearparams)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte

	if (opcode_dec) {
	DecompilerOut("// OP_checkclearparams();", true);
	WriteRegisterInfo(currentPos - 1);
	}

	opcodesPtr = currentPos;
}

// 0x27
DEF_DECOMPILE(EvalLocalVariableRefCached)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	DecompilerOut("// OP_EvalLocalVariableRefCached( %d ); ( %s )", true, *(BYTE*)(currentPos), StackLocalGetValue(*(BYTE*)(currentPos)));
	WriteRegisterInfo(currentPos - 1);
	}

	sprintf_s(VariableNameBuffer, "%s", StackLocalGetValue(*(BYTE*)(currentPos)));

	currentPos += 1;

	opcodesPtr = currentPos;
}

// 0x28
DEF_DECOMPILE(SetVariableField)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	DecompilerOut("// OP_SetVariableField();", true);
	WriteRegisterInfo(currentPos - 1);
	}

	BYTE* newCurrentPos = SetVariableField_foreach_decompile(currentPos);

	if (newCurrentPos)
		currentPos = newCurrentPos;
	else
	{
		if (!SetVariableField_compound_assignment_decompile())
			DecompilerOut("%s = %s;\n", true, VariableNameBuffer, StackGetValue(0));
	}

	StackPop();

	opcodesPtr = currentPos;
}

// 0x2B
DEF_DECOMPILE(wait)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	DecompilerOut("// OP_wait();", true);
	WriteRegisterInfo(currentPos - 1);
	}

	DecompilerOut("wait %s;\n", true, (char*)StackGetValue(0));

	StackPop();

	opcodesPtr = currentPos;
}

// 0x2C
DEF_DECOMPILE(waittillFrameEnd)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	DecompilerOut("// OP_waittillFrameEnd();", true);
	WriteRegisterInfo(currentPos - 1);
	}

	DecompilerOut("waittillframeend;\n", true);

	opcodesPtr = currentPos;
}

// 0x2D
DEF_DECOMPILE(PreScriptCall)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	DecompilerOut("// OP_PreScriptCall();", true);
	WriteRegisterInfo(currentPos - 1);
	}

	StackPush<BYTE*>(currentPos, type_precodepos);

	opcodesPtr = currentPos;
}

// 0x2E
DEF_DECOMPILE(ScriptFunctionCall)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte

	char* FunctionName = funcname_prepend_gscOfFunction((char*)(gscBuffer + *(DWORD*)(GET_ALIGNED_DWORD(currentPos + 1))), (DWORD)opcodesPtr - gscBuffer);
	
	if (opcode_dec) {
	DecompilerOut("// OP_ScriptFunctionCall( \"%s\" );", true, FunctionName);
	WriteRegisterInfo(currentPos - 1);
	}
	
	if (*(BYTE*)currentPos != 0) // delete later
	{
		cout << "OMG LULZ SCRIPTFUNCTIONCALL HAS %d DIFFERENT LOLZ LULZ LILZ" << endl;
		cin.get();
	}

	currentPos = GET_ALIGNED_DWORD(currentPos + 1) + 4;

	call_decompile(FunctionName, true, NULL, false, false, false, *(BYTE*)(currentPos) == OP_DecTop);

	free(FunctionName);

	opcodesPtr = currentPos;
}

// 0x2F
DEF_DECOMPILE(ScriptFunctionCallPointer)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte

	char* FunctionName = funcname_prepend_gscOfFunction((char*)StackGetValue(0), (DWORD)opcodesPtr - gscBuffer);
	
	if (opcode_dec) {
	DecompilerOut("// OP_ScriptFunctionCallPointer( \"%s\" );", true, FunctionName);
	WriteRegisterInfo(currentPos - 1);
	}

	StackPop(); // pop the function name from the stack

	currentPos += 1;

	call_decompile(FunctionName, true, NULL, true, false, false, *(BYTE*)(currentPos) == OP_DecTop);

	free(FunctionName);

	opcodesPtr = currentPos;
}

// 0x30
DEF_DECOMPILE(ScriptMethodCall)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte

	char* FunctionName = funcname_prepend_gscOfFunction((char*)(gscBuffer + *(DWORD*)(GET_ALIGNED_DWORD(currentPos + 1))), (DWORD)opcodesPtr - gscBuffer);
	
	if (opcode_dec) {
	DecompilerOut("// OP_ScriptMethodCall( %d, \"%s\" );", true, *(BYTE*)currentPos, FunctionName);
	WriteRegisterInfo(currentPos - 1);
	}
	
	if (*(BYTE*)currentPos != 0) // delete later
	{
		cout << "OMG LULZ METHODTHREADCALL HAS %d DIFFERENT LOLZ LULZ LILZ" << endl;
		cin.get();
	}

	currentPos = GET_ALIGNED_DWORD(currentPos + 1) + 4;

	call_decompile(FunctionName, true, NULL, false, true, false, *(BYTE*)(currentPos) == OP_DecTop);

	free(FunctionName);

	opcodesPtr = currentPos;
}

// 0x31
DEF_DECOMPILE(ScriptMethodCallPointer)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte

	char* FunctionName = funcname_prepend_gscOfFunction((char*)StackGetValue(0), (DWORD)opcodesPtr - gscBuffer);
	
	if (opcode_dec) {
	DecompilerOut("// OP_ScriptMethodCallPointer( \"%s\" );", true, FunctionName);
	WriteRegisterInfo(currentPos - 1);
	}

	StackPop(); // pop the function name from the stack

	currentPos += 1;

	call_decompile(FunctionName, true, NULL, true, true, false, *(BYTE*)(currentPos) == OP_DecTop);

	free(FunctionName);

	opcodesPtr = currentPos;
}

// 0x32
DEF_DECOMPILE(ScriptThreadCall)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte

	char* FunctionName = funcname_prepend_gscOfFunction((char*)(gscBuffer + *(DWORD*)(GET_ALIGNED_DWORD(currentPos + 1))), (DWORD)opcodesPtr - gscBuffer);
	
	if (opcode_dec) {
	DecompilerOut("// OP_ScriptThreadCall( %d, \"%s\" );", true, *(BYTE*)currentPos, FunctionName);
	WriteRegisterInfo(currentPos - 1);
	}

	if (*(BYTE*)currentPos != 0) // delete later
	{
		cout << "OMG LULZ SCRIPTTHREADCALL HAS %d DIFFERENT LOLZ LULZ LILZ" << endl;
		cin.get();
	}

	currentPos = GET_ALIGNED_DWORD(currentPos + 1) + 4;

	call_decompile(FunctionName, true, NULL, false, false, true, *(BYTE*)(currentPos) == OP_DecTop);

	free(FunctionName);

	opcodesPtr = currentPos;
}

// 0x33
DEF_DECOMPILE(ScriptThreadCallPointer)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte

	char* FunctionName = funcname_prepend_gscOfFunction((char*)StackGetValue(0), (DWORD)opcodesPtr - gscBuffer);
	
	if (opcode_dec) {
	DecompilerOut("// OP_ScriptThreadCallPointer( \"%s\" );", true, FunctionName);
	WriteRegisterInfo(currentPos - 1);
	}

	StackPop(); // pop the function name from the stack

	currentPos += 1;

	call_decompile(FunctionName, true, NULL, true, false, true, *(BYTE*)(currentPos) == OP_DecTop);

	free(FunctionName);

	opcodesPtr = currentPos;
}

// 0x34
DEF_DECOMPILE(ScriptMethodThreadCall)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte

	char* FunctionName = funcname_prepend_gscOfFunction((char*)(gscBuffer + *(DWORD*)(GET_ALIGNED_DWORD(currentPos + 1))), (DWORD)opcodesPtr - gscBuffer);
	
	if (opcode_dec) {
	DecompilerOut("// OP_ScriptMethodThreadCall( %d, \"%s\" );", true, *(BYTE*)currentPos, FunctionName);
	WriteRegisterInfo(currentPos - 1);
	}

	if (*(BYTE*)currentPos != 0) // delete later
	{
		cout << "OMG LULZ SCRIPTMETHODTHREADCALL HAS %d DIFFERENT LOLZ LULZ LILZ" << endl;
		cin.get();
	}

	currentPos = GET_ALIGNED_DWORD(currentPos + 1) + 4; // show byte...?

	call_decompile(FunctionName, true, NULL, false, true, true, *(BYTE*)(currentPos) == OP_DecTop);

	free(FunctionName);

	opcodesPtr = currentPos;
}

// 0x35
DEF_DECOMPILE(ScriptMethodThreadCallPointer)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte

	char* FunctionName = funcname_prepend_gscOfFunction((char*)StackGetValue(0), (DWORD)opcodesPtr - gscBuffer);
	
	if (opcode_dec) {
	DecompilerOut("// OP_ScriptMethodThreadCallPointer( \"%s\" );", true, FunctionName);
	WriteRegisterInfo(currentPos - 1);
	}

	StackPop(); // pop the function name from the stack

	currentPos += 1;

	call_decompile(FunctionName, true, NULL, true, true, true, *(BYTE*)(currentPos) == OP_DecTop);

	free(FunctionName); // free the function name passed to call_decompile

	opcodesPtr = currentPos;
}

// 0x36
DEF_DECOMPILE(DecTop)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte

	if (opcode_dec) {
	DecompilerOut("// OP_DecTop();", true);
	WriteRegisterInfo(currentPos - 1);
	}

	StackPop();

	opcodesPtr = currentPos;
}

// 0x37
DEF_DECOMPILE(CastFieldObject)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	DecompilerOut("// OP_CastFieldObject();", true);
	WriteRegisterInfo(currentPos - 1);
	}

	CurrentObject = OBJECT_CASTED;

	if (CastedObject)
		free(CastedObject);
	CastedObject = MallocAndSprintf("%s", StackGetValue(0));

	StackPop();

	opcodesPtr = currentPos;
}

// 0x38
DEF_DECOMPILE(CastBool)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	DecompilerOut("// OP_CastBool();", true);
	WriteRegisterInfo(currentPos - 1);
	}

	opcodesPtr = currentPos;
}

// 0x39
DEF_DECOMPILE(BoolNot)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	DecompilerOut("// OP_BoolNot();", true);
	WriteRegisterInfo(currentPos - 1);
	}

	//StackSetFlag(OP_FLAG_BOOL_NOT); // are flags even needed? :P

	char* BoolNot = MallocAndSprintf("!(%s)", StackGetValue(0));

	StackPop();
	StackPush<char*>(BoolNot, type_decompiled_string);

	opcodesPtr = currentPos;
}

// 0x3A
DEF_DECOMPILE(BoolComplement)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	DecompilerOut("// OP_BoolComplement();", true);
	WriteRegisterInfo(currentPos - 1);
	}

	opcodesPtr = currentPos;
}

// 0x3B
DEF_DECOMPILE(JumpOnFalse)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte

	if (opcode_dec) {
	DecompilerOut("// OP_JumpOnFalse( 0x%X ( %d ) ); ( 0x%X )", true, *(WORD*)(GET_ALIGNED_WORD(currentPos)), *(__int16*)(GET_ALIGNED_WORD(currentPos)), (DWORD)GET_ALIGNED_WORD(currentPos) + *(__int16*)(GET_ALIGNED_WORD(currentPos)) + 2 - gscBuffer);
	WriteRegisterInfo(currentPos - 1);
	}

	//if (*(__int16*)(GET_ALIGNED_WORD(currentPos)) < 0)
	//{
		//cout << "Error: negative conditional jump" << endl;
		//cin.get();
		//ExitProcess(-1);
	//}

	DWORD jumpTo = (DWORD)GET_ALIGNED_WORD(currentPos) + *(__int16*)(GET_ALIGNED_WORD(currentPos)) + 2;

	currentPos = GET_ALIGNED_WORD(currentPos) + 2;

	//if (*(__int16*)(GET_ALIGNED_WORD(currentPos)) < 0) // exists in _utility.gsc, not handled yet
		//opcodesPtr = currentPos;

	StackPop();

	//jump_expression_decompile(*(BYTE*)(opcodesPtr), jumpTo);

	opcodesPtr = currentPos;
}

// 0x3C
DEF_DECOMPILE(JumpOnTrue)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	DecompilerOut("// OP_JumpOnTrue( 0x%X ( %d ) ); ( 0x%X )", true, *(WORD*)(GET_ALIGNED_WORD(currentPos)), *(__int16*)(GET_ALIGNED_WORD(currentPos)), (DWORD)GET_ALIGNED_WORD(currentPos) + *(__int16*)(GET_ALIGNED_WORD(currentPos)) + 2 - gscBuffer);
	WriteRegisterInfo(currentPos - 1);
	}
	
	//if (*(__int16*)(GET_ALIGNED_WORD(currentPos)) < 0)
	//{
		//cout << "Error: negative conditional jump" << endl;
		//cin.get();
		//ExitProcess(-1);
	//}

	DWORD jumpTo = (DWORD)GET_ALIGNED_WORD(currentPos) + *(__int16*)(GET_ALIGNED_WORD(currentPos)) + 2;

	currentPos = GET_ALIGNED_WORD(currentPos) + 2;

	//if (*(__int16*)(GET_ALIGNED_WORD(currentPos)) < 0) // exists in _utility.gsc, not handled yet
		//opcodesPtr = currentPos;

	StackPop();

	//jump_expression_decompile(*(BYTE*)(opcodesPtr), jumpTo);

	opcodesPtr = currentPos;
}

// 0x3D
DEF_DECOMPILE(JumpOnFalseExpr)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	DecompilerOut("// OP_JumpOnFalseExpr( 0x%X ( %d ) ); ( 0x%X )", true, *(WORD*)(GET_ALIGNED_WORD(currentPos)), *(__int16*)(GET_ALIGNED_WORD(currentPos)), (DWORD)GET_ALIGNED_WORD(currentPos) + *(__int16*)(GET_ALIGNED_WORD(currentPos)) + 2 - gscBuffer);
	WriteRegisterInfo(currentPos - 1);
	}
	
	//if (*(__int16*)(GET_ALIGNED_WORD(currentPos)) < 0)
	//{
		//cout << "Error: negative conditional jump" << endl;
		//cin.get();
		//ExitProcess(-1);
	//}

	DWORD jumpTo = (DWORD)GET_ALIGNED_WORD(currentPos) + *(__int16*)(GET_ALIGNED_WORD(currentPos)) + 2;

	currentPos = GET_ALIGNED_WORD(currentPos) + 2;

	//if (*(__int16*)(GET_ALIGNED_WORD(currentPos)) < 0) // exists in _utility.gsc, not handled yet
		//opcodesPtr = currentPos;

	StackPop();

	//jump_expression_decompile(*(BYTE*)(opcodesPtr), jumpTo);

	opcodesPtr = currentPos;
}

// 0x3E
DEF_DECOMPILE(JumpOnTrueExpr)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	DecompilerOut("// OP_JumpOnTrueExpr( 0x%X ( %d ) ); ( 0x%X )", true, *(WORD*)(GET_ALIGNED_WORD(currentPos)), *(__int16*)(GET_ALIGNED_WORD(currentPos)), (DWORD)GET_ALIGNED_WORD(currentPos) + *(__int16*)(GET_ALIGNED_WORD(currentPos)) + 2 - gscBuffer);
	WriteRegisterInfo(currentPos - 1);
	}
	
	//if (*(__int16*)(GET_ALIGNED_WORD(currentPos)) < 0)
	//{
		//cout << "Error: negative conditional jump" << endl;
		//cin.get();
		//ExitProcess(-1);
	//}

	DWORD jumpTo = (DWORD)GET_ALIGNED_WORD(currentPos) + *(__int16*)(GET_ALIGNED_WORD(currentPos)) + 2;

	currentPos = GET_ALIGNED_WORD(currentPos) + 2;

	//if (*(__int16*)(GET_ALIGNED_WORD(currentPos)) < 0) // exists in _utility.gsc, not handled yet
		//opcodesPtr = currentPos;

	StackPop();

	//jump_expression_decompile(*(BYTE*)(opcodesPtr), jumpTo);

	opcodesPtr = currentPos;
}

// 0x3F
DEF_DECOMPILE(jump)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	DecompilerOut("// OP_jump( 0x%X ( %d ) ); ( 0x%X )", true, *(WORD*)(GET_ALIGNED_WORD(currentPos)), *(__int16*)(GET_ALIGNED_WORD(currentPos)), (DWORD)GET_ALIGNED_WORD(currentPos) + *(__int16*)(GET_ALIGNED_WORD(currentPos)) + 2 - gscBuffer);
	WriteRegisterInfo(currentPos - 1);
	}

	currentPos = GET_ALIGNED_WORD(currentPos) + 2;

	opcodesPtr = currentPos;
}

// 0x41
DEF_DECOMPILE(inc)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	DecompilerOut("// OP_inc();", true);
	WriteRegisterInfo(currentPos - 1);
	}

	DecompilerOut("%s++;\n", true, VariableNameBuffer);

	opcodesPtr = currentPos;
}

// 0x42
DEF_DECOMPILE(dec)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	DecompilerOut("// OP_dec();", true);
	WriteRegisterInfo(currentPos - 1);
	}

	DecompilerOut("%s--;\n", true, VariableNameBuffer);

	opcodesPtr = currentPos;
}

// 0x43
DEF_DECOMPILE(bit_or)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	DecompilerOut("// OP_bit_or();", true);
	WriteRegisterInfo(currentPos - 1);
	}

	operator_decompile(OP_BIT_OR);

	opcodesPtr = currentPos;
}

// 0x44
DEF_DECOMPILE(bit_ex_or)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	DecompilerOut("// OP_bit_ex_or();", true);
	WriteRegisterInfo(currentPos - 1);
	}

	operator_decompile(OP_BIT_EX_OR);

	opcodesPtr = currentPos;
}

// 0x45
DEF_DECOMPILE(bit_and)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	DecompilerOut("// OP_bit_and();", true);
	WriteRegisterInfo(currentPos - 1);
	}

	operator_decompile(OP_BIT_AND);

	opcodesPtr = currentPos;
}

// 0x46
DEF_DECOMPILE(equality)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	DecompilerOut("// OP_equality();", true);
	WriteRegisterInfo(currentPos - 1);
	}

	operator_decompile(OP_EQUALITY);

	opcodesPtr = currentPos;
}

// 0x47
DEF_DECOMPILE(inequality)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	DecompilerOut("// OP_inequality();", true);
	WriteRegisterInfo(currentPos - 1);
	}

	operator_decompile(OP_INEQUALITY);

	opcodesPtr = currentPos;
}

// 0x48
DEF_DECOMPILE(less)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	DecompilerOut("// OP_less();", true);
	WriteRegisterInfo(currentPos - 1);
	}

	operator_decompile(OP_LESS);

	opcodesPtr = currentPos;
}

// 0x49
DEF_DECOMPILE(greater)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	DecompilerOut("// OP_greater();", true);
	WriteRegisterInfo(currentPos - 1);
	}

	operator_decompile(OP_GREATER);

	opcodesPtr = currentPos;
}

// 0x4A
DEF_DECOMPILE(less_equal)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	DecompilerOut("// OP_less_equal();", true);
	WriteRegisterInfo(currentPos - 1);
	}

	operator_decompile(OP_LESS_EQUAL);

	opcodesPtr = currentPos;
}

// 0x4B
DEF_DECOMPILE(greater_equal)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	DecompilerOut("// OP_greater_equal();", true);
	WriteRegisterInfo(currentPos - 1);
	}

	operator_decompile(OP_GREATER_EQUAL);

	opcodesPtr = currentPos;
}

// 0x4C
DEF_DECOMPILE(shift_left)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	DecompilerOut("// OP_shift_left();", true);
	WriteRegisterInfo(currentPos - 1);
	}

	operator_decompile(OP_SHIFT_LEFT);

	opcodesPtr = currentPos;
}

// 0x4D
DEF_DECOMPILE(shift_right)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	DecompilerOut("// OP_shift_right();", true);
	WriteRegisterInfo(currentPos - 1);
	}

	operator_decompile(OP_SHIFT_RIGHT);

	opcodesPtr = currentPos;
}

// 0x4E
DEF_DECOMPILE(plus)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	DecompilerOut("// OP_plus();", true);
	WriteRegisterInfo(currentPos - 1);
	}

	operator_decompile(OP_PLUS);

	opcodesPtr = currentPos;
}

// 0x4F
DEF_DECOMPILE(minus)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	DecompilerOut("// OP_minus();", true);
	WriteRegisterInfo(currentPos - 1);
	}

	operator_decompile(OP_MINUS);

	opcodesPtr = currentPos;
}

// 0x50
DEF_DECOMPILE(multiply)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	DecompilerOut("// OP_multiply();", true);
	WriteRegisterInfo(currentPos - 1);
	}

	operator_decompile(OP_MULTIPLY);

	opcodesPtr = currentPos;
}

// 0x51
DEF_DECOMPILE(divide)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	DecompilerOut("// OP_divide();", true);
	WriteRegisterInfo(currentPos - 1);
	}

	operator_decompile(OP_DIVIDE);

	opcodesPtr = currentPos;
}

// 0x52
DEF_DECOMPILE(mod)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	DecompilerOut("// OP_mod();", true);
	WriteRegisterInfo(currentPos - 1);
	}

	operator_decompile(OP_MOD);

	opcodesPtr = currentPos;
}

// 0x53
DEF_DECOMPILE(size)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	DecompilerOut("// OP_size();", true);
	WriteRegisterInfo(currentPos - 1);
	}

	char* SizeAppended = MallocAndSprintf("%s.size", StackGetValue(0));

	StackPop(); // pop so it frees the old decompiled string
	StackPush<char*>(SizeAppended, type_decompiled_string);

	opcodesPtr = currentPos;
}

// 0x54
DEF_DECOMPILE(waittillmatch)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	DecompilerOut("// OP_waittillmatch();", true);
	WriteRegisterInfo(currentPos - 1);
	}

	waittill_call_decompile("waittillmatch", currentPos);

	opcodesPtr = currentPos;
}

// 0x55
DEF_DECOMPILE(waittill)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	DecompilerOut("// OP_waittill();", true);
	WriteRegisterInfo(currentPos - 1);
	}
	
	waittill_call_decompile("waittill", currentPos);

	opcodesPtr = currentPos;
}

// 0x56
DEF_DECOMPILE(notify)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	DecompilerOut("// OP_notify();", true);
	WriteRegisterInfo(currentPos - 1);
	}

	call_decompile("notify", true, NULL, false, true, false, true);

	StackPop(); // this function doesn't have OP_DecTop so we must pop manually

	opcodesPtr = currentPos;
}

// 0x57
DEF_DECOMPILE(endon)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	DecompilerOut("// OP_endon();", true);
	WriteRegisterInfo(currentPos - 1);
	}

	call_decompile("endon", false, 1, false, true, false, true);

	StackPop();

	opcodesPtr = currentPos;
}

// 0x58
DEF_DECOMPILE(voidCodepos)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	DecompilerOut("// OP_voidCodepos();", true);
	WriteRegisterInfo(currentPos - 1);
	}

	StackPush<BYTE*>(currentPos, type_precodepos);

	opcodesPtr = currentPos;
}

// 0x59
DEF_DECOMPILE(switch)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
		DecompilerOut("// OP_switch( %s );", true, StackGetValue(0));
		WriteRegisterInfo(currentPos - 1);
	}

	currentPos = switch_decompile(currentPos);

	opcodesPtr = currentPos;
}

// 0x5A
DEF_DECOMPILE(endswitch)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte

	DWORD caseCount = *(DWORD*)GET_ALIGNED_DWORD(currentPos);

	// Skip each case block
	currentPos = GET_ALIGNED_DWORD(GET_ALIGNED_DWORD(currentPos) + 4) + 8 * caseCount;

	opcodesPtr = currentPos;
}

// 0x5B
DEF_DECOMPILE(vector)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	DecompilerOut("// OP_vector();", true);
	WriteRegisterInfo(currentPos - 1);
	}

	char* VectorString = MallocAndSprintf("( %s, %s, %s )", StackGetValue(0), StackGetValue(1), StackGetValue(2));

	StackPop();
	StackPop();
	StackPop();

	StackPush<char*>(VectorString, type_decompiled_string);

	opcodesPtr = currentPos;
}

// 0x5C
DEF_DECOMPILE(GetHash)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte

	DWORD hashValue		= *(DWORD*)GET_ALIGNED_DWORD(currentPos);
	char *hash			= nullptr;
	const char *value	= DvarNameForHash(hashValue);

	if (!value)
		hash = MallocAndSprintf("#\"0x%X\"", hashValue);
	else
		hash = MallocAndSprintf("#\"%s\"", value);
	
	if (opcode_dec) {
	DecompilerOut("// OP_GetHash( %s );", true, hash);
	WriteRegisterInfo(currentPos - 1);
	}

	StackPush<char*>(hash, type_decompiled_string);

	currentPos = GET_ALIGNED_DWORD(currentPos) + 4;

	opcodesPtr = currentPos;
}

// 0x5E
DEF_DECOMPILE(GetSimpleVector)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	DecompilerOut("// OP_GetSimpleVector( 0x%X );", true, *(BYTE*)currentPos);
	WriteRegisterInfo(currentPos - 1);
	}

	BYTE simplevectorflags = *(BYTE*)currentPos;
	float simplevector1 = 0;
	float simplevector2 = 0;
	float simplevector3 = 0;

	if (simplevectorflags & 0x20)
		simplevector1 = 1.0f;
	else
	{
		if (simplevectorflags & 0x10)
			simplevector1 = -1.0f;
		else
			simplevector1 = 0.0f;
	}

	if (simplevectorflags & 0x08)
		simplevector2 = 1.0f;
	else
	{
		if (simplevectorflags & 0x04)
			simplevector2 = -1.0f;
		else
			simplevector2 = 0.0f;
	}

	if (simplevectorflags & 0x02)
		simplevector3 = 1.0f;
	else
	{
		if (simplevectorflags & 0x01)
			simplevector3 = -1.0f;
		else
			simplevector3 = 0.0f;
	}
	
	char* SimpleVector = MallocAndSprintf("( %g, %g, %g )", simplevector1, simplevector2, simplevector3);

	StackPush<char*>(SimpleVector, type_decompiled_string);

	currentPos += 1;

	opcodesPtr = currentPos;
}

// 0x5F
DEF_DECOMPILE(isdefined)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	DecompilerOut("// OP_isdefined();", true);
	WriteRegisterInfo(currentPos - 1);
	}

	call_decompile("IsDefined", false, 1, false, false, false, *(BYTE*)(currentPos) == OP_DecTop);

	opcodesPtr = currentPos;
}

// 0x60
DEF_DECOMPILE(vectorscale)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	DecompilerOut("// OP_vectorscale();", true);
	WriteRegisterInfo(currentPos - 1);
	}

	call_decompile("vector_scale", false, 2, false, false, false, *(BYTE*)(currentPos) == OP_DecTop);

	opcodesPtr = currentPos;
}

// 0x61
DEF_DECOMPILE(anglestoup)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	DecompilerOut("// OP_anglestoup();", true);
	WriteRegisterInfo(currentPos - 1);
	}

	call_decompile("anglestoup", false, 1, false, false, false, *(BYTE*)(currentPos) == OP_DecTop);

	opcodesPtr = currentPos;
}

// 0x62
DEF_DECOMPILE(anglestoright)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	DecompilerOut("// OP_anglestoright();", true);
	WriteRegisterInfo(currentPos - 1);
	}

	call_decompile("anglestoright", false, 1, false, false, false, *(BYTE*)(currentPos) == OP_DecTop);

	opcodesPtr = currentPos;
}

// 0x63
DEF_DECOMPILE(anglestoforward)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	DecompilerOut("// OP_anglestoforward();", true);
	WriteRegisterInfo(currentPos - 1);
	}

	call_decompile("anglestoforward", false, 1, false, false, false, *(BYTE*)(currentPos) == OP_DecTop);

	opcodesPtr = currentPos;
}

// 0x64
DEF_DECOMPILE(angleclamp180)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	DecompilerOut("// OP_angleclamp180();", true);
	WriteRegisterInfo(currentPos - 1);
	}

	call_decompile("angleclamp180", false, 1, false, false, false, *(BYTE*)(currentPos) == OP_DecTop);

	opcodesPtr = currentPos;
}

// 0x65
DEF_DECOMPILE(vectorstoangle)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	DecompilerOut("// OP_vectorstoangle();", true);
	WriteRegisterInfo(currentPos - 1);
	}

	// i think that the devs made a typo, its not vectorstoangle, its vectortoangles
	call_decompile("vectortoangles", false, 1, false, false, false, *(BYTE*)(currentPos) == OP_DecTop);

	opcodesPtr = currentPos;
}

// 0x66
DEF_DECOMPILE(abs)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	DecompilerOut("// OP_abs();", true);
	WriteRegisterInfo(currentPos - 1);
	}

	call_decompile("abs", false, 1, false, false, false, *(BYTE*)(currentPos) == OP_DecTop);

	opcodesPtr = currentPos;
}

// 0x67
DEF_DECOMPILE(gettime)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	DecompilerOut("// OP_gettime();", true);
	WriteRegisterInfo(currentPos - 1);
	}

	call_decompile("GetTime", false, 0, false, false, false, *(BYTE*)(currentPos) == OP_DecTop);

	opcodesPtr = currentPos;
}

// 0x68
DEF_DECOMPILE(getdvar)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	DecompilerOut("// OP_getdvar();", true);
	WriteRegisterInfo(currentPos - 1);
	}

	call_decompile("GetDvar", false, 1, false, false, false, *(BYTE*)(currentPos) == OP_DecTop);

	opcodesPtr = currentPos;
}

// 0x69
DEF_DECOMPILE(getdvarint)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	DecompilerOut("// OP_getdvarint();", true);
	WriteRegisterInfo(currentPos - 1);
	}

	call_decompile("GetDvarInt", false, 1, false, false, false, *(BYTE*)(currentPos) == OP_DecTop);

	opcodesPtr = currentPos;
}

// 0x6A
DEF_DECOMPILE(getdvarfloat) // need to make a function to set current stack pointer (this will set type to type_function_call)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	DecompilerOut("// OP_getdvarfloat();", true);
	WriteRegisterInfo(currentPos - 1);
	}

	call_decompile("GetDvarFloat", false, 1, false, false, false, *(BYTE*)(currentPos) == OP_DecTop);

	opcodesPtr = currentPos;
}

// 0x6B
DEF_DECOMPILE(getdvarvector)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	DecompilerOut("// OP_getdvarvector();", true);
	WriteRegisterInfo(currentPos - 1);
	}

	call_decompile("GetDvarVector", false, 1, false, false, false, *(BYTE*)(currentPos) == OP_DecTop);

	opcodesPtr = currentPos;
}

// 0x6C
DEF_DECOMPILE(getdvarcolorred)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	DecompilerOut("// OP_getdvarcolorred();", true);
	WriteRegisterInfo(currentPos - 1);
	}

	call_decompile("GetDvarColorRed", false, 1, false, false, false, *(BYTE*)(currentPos) == OP_DecTop);

	opcodesPtr = currentPos;
}

// 0x6D
DEF_DECOMPILE(getdvarcolorgreen)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	DecompilerOut("// OP_getdvarcolorgreen();", true);
	WriteRegisterInfo(currentPos - 1);
	}

	call_decompile("GetDvarColorGreen", false, 1, false, false, false, *(BYTE*)(currentPos) == OP_DecTop);

	opcodesPtr = currentPos;
}

// 0x6E
DEF_DECOMPILE(getdvarcolorblue)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	DecompilerOut("// OP_getdvarcolorblue();", true);
	WriteRegisterInfo(currentPos - 1);
	}

	call_decompile("GetDvarColorBlue", false, 1, false, false, false, *(BYTE*)(currentPos) == OP_DecTop);

	opcodesPtr = currentPos;
}

// 0x6F
DEF_DECOMPILE(getdvarcoloralpha)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	DecompilerOut("// OP_getdvarcoloralpha();", true);
	WriteRegisterInfo(currentPos - 1);
	}

	call_decompile("GetDvarColorAlpha", false, 1, false, false, false, *(BYTE*)(currentPos) == OP_DecTop);

	opcodesPtr = currentPos;
}

// 0x70
DEF_DECOMPILE(GetFirstArrayKey)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	DecompilerOut("// OP_GetFirstArrayKey( %s );", true, StackGetValue(0));
	WriteRegisterInfo(currentPos - 1);
	}

	// Stack incremented by 1
	//
	// params:
	//	[Array]  Array to look up
	// returns:
	//	[String, int, object] Key
	call_decompile("GetFirstArrayKey", false, 1, false, false, false, *(BYTE*)(currentPos) == OP_DecTop);

	opcodesPtr = currentPos;
}

// 0x71
DEF_DECOMPILE(GetNextArrayKey)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	DecompilerOut("// OP_GetNextArrayKey( %s, %s );", true, StackGetValue(0), StackGetValue(1));
	WriteRegisterInfo(currentPos - 1);
	}

	// Stack incremented by 1
	//
	// params:
	//	[Array]               Array to look up
	//	[String, int, object] Previous key
	// returns:
	//	[String, int, object] Key
	call_decompile("GetNextArrayKey", false, 2, false, false, false, *(BYTE*)(currentPos) == OP_DecTop);

	opcodesPtr = currentPos;
}

// 0x73
DEF_DECOMPILE(GetUndefined2)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte

	if (opcode_dec) {
	DecompilerOut("// OP_GetUndefined2();", true);
	WriteRegisterInfo(currentPos - 1);
	}

	opcodesPtr = currentPos;
}

// 0x7B
DEF_DECOMPILE(skipdev)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte
	
	if (opcode_dec) {
	DecompilerOut("// OP_skipdev( 0x%X ( %d ) );", true, *(WORD*)(GET_ALIGNED_WORD(currentPos)), *(__int16*)(GET_ALIGNED_WORD(currentPos)));
	WriteRegisterInfo(currentPos - 1);
	}

	if (*(__int16*)(GET_ALIGNED_WORD(currentPos)) < 0)
	{
		cout << "Error: negative jump not expected in OP_skipdev" << endl;
		cin.get();
		ExitProcess(-1);
	}

	DecompilerOut("/#\n", false);
	GSCDecompilerClass gscDecompiler;
	//gscDecompiler.opcode_dec = true;
	decompiledBuffer.append((char*)gscDecompiler.decompile(
		&this->stack,
		gscBuffer,
		(DWORD)GET_ALIGNED_WORD(currentPos) + 2 - gscBuffer,
		*(__int16*)(GET_ALIGNED_WORD(currentPos)),
		false,
		curTabLevel).c_str()
		);
	DecompilerOut("#/\n", false);

	currentPos = GET_ALIGNED_WORD(currentPos) + *(__int16*)(GET_ALIGNED_WORD(currentPos)) + 2;

	opcodesPtr = currentPos;
}