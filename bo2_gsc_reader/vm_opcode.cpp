#include "stdafx.h"

#define Add(format, ...) Scr_AddFuncBuffer(function, format, ##__VA_ARGS__)

#define DATA_ALIGN(alignment) aligned = (((int)opcodesPtr + (alignment - 1)) & ~(alignment - 1))

void InterpretFunction(DWORD gscBuffer, gscFunction *gscFunc)
{
	// write the offset of the function
	AddString("// 0x%X\n", false, gscFunc->start);

	AddString("%s(", false, gscBuffer + gscFunc->name);

	if (gscFunc->numOfParameters) // need to add a space if there are parameters (more close to the original)
	{
		AddString(" ", false);

		if (*(BYTE*)(gscBuffer + gscFunc->start) != OP_CreateLocalVariables)
		{
			AddString("/* Error: Parameter decompilation failed (first opcode isn't OP_CreateLocalVariables) */", false);
			goto end_param_decompilation;
		}
		else if (*(BYTE*)(gscBuffer + gscFunc->start + 1) < gscFunc->numOfParameters)
		{
			AddString("/* Error: Parameter decompilation failed (not enough variables in OP_CreateLocalVariables) */", false);
			goto end_param_decompilation;
		}

		for (int i = 0; i < gscFunc->numOfParameters; i++)
		{
			AddString((char*)(gscBuffer + *(WORD*)(gscBuffer + gscFunc->start + 2 + i * 2)), false);

			if ((i + 1) < gscFunc->numOfParameters)
				AddString(",", false);

			AddString(" ", false);
		}
	}
end_param_decompilation:

	AddString(")\n{\n", false);

	// we can "bruteforce" the size of the function by crc32ing byte by byte until we hit the function's crc32 (which means that the function has ended)
	DWORD gscFuncSize = 0;

	Crc32 crc32;
	crc32.Reset();

	for (DWORD i = 0; crc32.GetCrc32() != gscFunc->crc32; gscFuncSize++, i++)
		crc32.AddData((BYTE*)(gscBuffer + gscFunc->start + i), 1);

	GSCDecompilerClass gscDecompiler;

	// decompile function opcodes
	string decompiledCode = gscDecompiler.decompile(NULL, gscBuffer, gscFunc->start, gscFuncSize, true, 1);

	AddString((char*)decompiledCode.c_str(), false);

	AddString("\n}", false);
}

// gsc opcode length disassembler (takes pointer to opcode as parameter, returns size of the entire instruction (opcode counts too))
DWORD gsclde(BYTE* opcodesPtr)
{
	BYTE* currentPos = opcodesPtr;
	currentPos += 1; // opcode size 1 byte

	// variables used inside the switch
	BYTE numOfVariables = 0;
	DWORD caseCount = 0;

	switch (*opcodesPtr)
	{
	case OP_End:
	case OP_Return:
	case OP_GetUndefined:
	case OP_GetZero:
	case OP_GetLevelObject:
	case OP_GetAnimObject:
	case OP_GetSelf:
	case OP_GetLevel:
	case OP_GetGame:
	case OP_GetAnim:
	case OP_GetGameRef: // ADD DECOMPILATION SUPPORT FOR THIS (WTFFFF)
	case OP_RemoveLocalVariables: // ADD SUPPORT FOR THIS TOO
	case OP_EvalArray:
	case OP_EvalArrayRef:
	case OP_ClearArray:
	case OP_EmptyArray:
	case OP_GetSelfObject:
	case OP_clearparams: // ADD SUPPORT FOR THIS TOO
	case OP_checkclearparams:
	case OP_SetVariableField:
	case OP_wait:
	case OP_waittillFrameEnd:
	case OP_PreScriptCall:
	case OP_DecTop:
	case OP_CastFieldObject:
	case OP_CastBool: // ADD SUPPORT FOR THIS TOO? hmm...
	case OP_BoolNot:
	case OP_BoolComplement: // ADD SUPPORT FOR THIS TOO
	case OP_inc:
	case OP_dec:
	case OP_bit_or:
	case OP_bit_ex_or:
	case OP_bit_and:
	case OP_equality:
	case OP_inequality:
	case OP_less:
	case OP_greater:
	case OP_less_equal:
	case OP_greater_equal:
	case OP_shift_left:
	case OP_shift_right:
	case OP_plus:
	case OP_minus:
	case OP_multiply:
	case OP_divide:
	case OP_mod:
	case OP_size:
	case OP_waittillmatch:
	case OP_waittill:
	case OP_notify:
	case OP_endon:
	case OP_voidCodepos:
	case OP_vector:
	case OP_isdefined:
	case OP_vectorscale:
	case OP_anglestoup:
	case OP_anglestoright:
	case OP_anglestoforward:
	case OP_angleclamp180:
	case OP_vectorstoangle:
	case OP_abs:
	case OP_gettime:
	case OP_getdvar:
	case OP_getdvarint:
	case OP_getdvarfloat:
	case OP_getdvarvector:
	case OP_getdvarcolorred:
	case OP_getdvarcolorgreen:
	case OP_getdvarcolorblue:
	case OP_getdvarcoloralpha:
	case OP_GetFirstArrayKey:
	case OP_GetNextArrayKey:
	case OP_GetUndefined2:
	case OP_Unknown74: // ADD SUPPORT FOR THIS TOO
	case OP_NOP: // ADD SUPPORT FOR THIS TOO (LOL)
	case OP_abort: // ADD SUPPORT FOR THIS TOO (in cod4, this decreases g_script_error_level, and seems to do the same in bo2)
		return 1;
	case OP_GetByte:
	case OP_GetNegByte:
	case OP_EvalLocalVariableCached:
	case OP_EvalLocalArrayRefCached: // ADD SUPPORT FOR THIS TOO
	case OP_SafeSetVariableFieldCached: // ADD SUPPORT FOR THIS TOO
	case OP_SafeSetWaittillVariableFieldCached:
	case OP_EvalLocalVariableRefCached:
	case OP_ScriptFunctionCallPointer:
	case OP_ScriptMethodCallPointer:
	case OP_ScriptThreadCallPointer:
	case OP_ScriptMethodThreadCallPointer:
	case OP_GetSimpleVector:
		return 2;
	case OP_GetUnsignedShort:
	case OP_GetNegUnsignedShort:
	case OP_GetString:
	case OP_GetIString:
	case OP_CreateLocalVariable: // ADD SUPPORT FOR THIS TOO
	case OP_EvalFieldVariable:
	case OP_EvalFieldVariableRef:
	case OP_ClearFieldVariable:
	case OP_JumpOnFalse:
	case OP_JumpOnTrue:
	case OP_JumpOnFalseExpr:
	case OP_JumpOnTrueExpr:
	case OP_jump:
	case OP_thread_object: // ADD SUPPORT FOR THIS TOO (pushes a type_object var to the stack like OP_object, but differently)
	case OP_EvalLocalVariable: // ADD SUPPORT FOR THIS TOO
	case OP_EvalLocalVariableRef: // ADD SUPPORT FOR THIS TOO
	case OP_skipdev: // ADD SUPPORT FOR THIS TOO
		currentPos = GET_ALIGNED_WORD(currentPos) + 2;
		return currentPos - opcodesPtr;
	case OP_GetInteger:
	case OP_GetFloat:
	case OP_GetAnimation:
	case OP_GetFunction:
	case OP_switch: // we don't skip the cases' code here
	case OP_GetHash:
	case OP_weird3: // ADD SUPPORT FOR THIS ONE TOO (what it does is the same as OP_GetUndefined, but changes currentPos too)
		currentPos = GET_ALIGNED_DWORD(currentPos) + 4;
		return currentPos - opcodesPtr;
	case OP_GetVector:
		currentPos = GET_ALIGNED_DWORD(currentPos) + 12;
		return currentPos - opcodesPtr;
	case OP_CreateLocalVariables:
		numOfVariables = *(BYTE*)(currentPos);
		currentPos += 1;
		for (BYTE i = 0; i < numOfVariables; i++)
			currentPos = GET_ALIGNED_WORD(currentPos) + 2;
		return currentPos - opcodesPtr;
	//case OP_CallBuiltin: // idk how to add support for these two, i think it's only generated by the external function resolver anyway
	//case OP_CallBuiltinMethod:
	case OP_ScriptFunctionCall:
	case OP_ScriptMethodCall:
	case OP_ScriptThreadCall:
	case OP_ScriptMethodThreadCall:
		currentPos = GET_ALIGNED_DWORD(currentPos + 1) + 4;
		return currentPos - opcodesPtr;
	//case OP_jumpback: // i don't think this opcode is even supported by the VM?
	case OP_endswitch:
		caseCount = *(DWORD*)GET_ALIGNED_DWORD(currentPos);
		currentPos = GET_ALIGNED_DWORD(GET_ALIGNED_DWORD(currentPos) + 4) + 8 * caseCount;
		return currentPos - opcodesPtr;

	// 0x5D not supported

	case OP_object: // ADD SUPPORT FOR THIS TOO (what i know is that it pushes a type_object var to the stack)
		currentPos = GET_ALIGNED_DWORD(GET_ALIGNED_DWORD(currentPos) + 4) + 4;
		return currentPos - opcodesPtr;
	}

	return 0;
}