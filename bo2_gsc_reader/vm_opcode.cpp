#include "stdafx.h"

#define Add(format, ...) Scr_AddFuncBuffer(function, format, ##__VA_ARGS__)

#define DATA_ALIGN(alignment) aligned = (((int)opcodesPtr + (alignment - 1)) & ~(alignment - 1))

void InterpretFunction(DWORD gscBuffer, gscFunction *gscFunc)
{
	//write the offset from the compiled GSC file
	AddString("// 0x%X\n", false, gscFunc->start);

	AddString("%s(", false, gscBuffer + gscFunc->name);

	if (gscFunc->numOfParameters) // need to add a space if there are parameters (more close to the original)
	{
		AddString(" ", false);

		if (*(BYTE*)(gscBuffer + gscFunc->start) != OP_CreateLocalVariable)
		{
			AddString("/* Error: Parameter decompilation failed (first opcode isn't OP_CreateLocalVariable) */", false);
			goto end_param_decompilation;
		}
		else if (*(BYTE*)(gscBuffer + gscFunc->start + 1) < gscFunc->numOfParameters)
		{
			AddString("/* Error: Parameter decompilation failed (not enough variables in OP_CreateLocalVariable) */", false);
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

	// init stack
	InitStacks();

	AddString(")\n{\n", false);

	ResetTabLevel();
	IncTabLevel();

	// decompile function opcodes
	InterpretGSCOpCodes(gscBuffer, gscFunc);

	DecTabLevel();

	AddString("} // ", false);
	if (StackGetRelativePos() == 0x0)
		AddString("SP = 0x%X - check OK", false, StackGetRelativePos());
	else
		AddString("SP = 0x%X - check failed (function may have been decompiled incorrectly)", false, StackGetRelativePos());
}

// the goal is to make the gsc decompiling look like the original ones from bo1
void InterpretGSCOpCodes(DWORD gscBuffer, gscFunction* gscFunc)
{
	BYTE* opcodesPtr = (BYTE*)(gscBuffer + gscFunc->start);

	Crc32 crc32;
	Crc32 crc32_2; // only used in OP_End

	BYTE* oldOpcodesPtr = 0;

	while (true)
	{
		oldOpcodesPtr = opcodesPtr;

		CheckIfJumps(opcodesPtr);

		switch (*opcodesPtr)
		{
		case OP_End:
			crc32_2.AddData((BYTE*)(gscBuffer + gscFunc->start), (DWORD)opcodesPtr - gscBuffer - gscFunc->start + 1); // + 1 = size of OP_End
			opcodesPtr = OP_End_Decompile(gscBuffer, opcodesPtr, crc32_2.GetCrc32() == gscFunc->crc32);
			crc32_2.Reset();
			break;
		case OP_Return:
			opcodesPtr = OP_Return_Decompile(gscBuffer, opcodesPtr);
			break;
		case OP_GetUndefined:
			opcodesPtr = OP_GetUndefined_Decompile(gscBuffer, opcodesPtr);
			break;
		case OP_GetZero:
			opcodesPtr = OP_GetZero_Decompile(gscBuffer, opcodesPtr);
			break;
		case OP_GetByte:
			opcodesPtr = OP_GetByte_Decompile(gscBuffer, opcodesPtr);
			break;
		case OP_GetNegByte:
			opcodesPtr = OP_GetNegByte_Decompile(gscBuffer, opcodesPtr);
			break;
		case OP_GetUnsignedShort:
			opcodesPtr = OP_GetUnsignedShort_Decompile(gscBuffer, opcodesPtr);
			break;
		case OP_GetNegUnsignedShort:
			opcodesPtr = OP_GetNegUnsignedShort_Decompile(gscBuffer, opcodesPtr);
			break;
		case OP_GetInteger:
			opcodesPtr = OP_GetInteger_Decompile(gscBuffer, opcodesPtr);
			break;
		case OP_GetFloat:
			opcodesPtr = OP_GetFloat_Decompile(gscBuffer, opcodesPtr);
			break;
		case OP_GetString:
			opcodesPtr = OP_GetString_Decompile(gscBuffer, opcodesPtr);
			break;
		case OP_GetIString:
			opcodesPtr = OP_GetIString_Decompile(gscBuffer, opcodesPtr);
			break;
		case OP_GetVector:
			opcodesPtr = OP_GetVector_Decompile(gscBuffer, opcodesPtr);
			break;
		case OP_GetLevelObject:
			opcodesPtr = OP_GetLevelObject_Decompile(gscBuffer, opcodesPtr);
			break;
		case OP_GetAnimObject:
			opcodesPtr = OP_GetAnimObject_Decompile(gscBuffer, opcodesPtr);
			break;
		case OP_GetSelf:
			opcodesPtr = OP_GetSelf_Decompile(gscBuffer, opcodesPtr);
			break;
		case OP_GetLevel:
			opcodesPtr = OP_GetLevel_Decompile(gscBuffer, opcodesPtr);
			break;
		case OP_GetGame:
			opcodesPtr = OP_GetGame_Decompile(gscBuffer, opcodesPtr);
			break;
		case OP_GetAnim:
			opcodesPtr = OP_GetAnim_Decompile(gscBuffer, opcodesPtr);
			break;
		case OP_GetAnimation:
			opcodesPtr = OP_GetAnimation_Decompile(gscBuffer, opcodesPtr);
			break;
		case OP_GetGameRef:
			opcodesPtr = OP_GetGameRef_Decompile(gscBuffer, opcodesPtr);
			break;
		case OP_GetFunction:
			opcodesPtr = OP_GetFunction_Decompile(gscBuffer, opcodesPtr);
			break;
		case OP_CreateLocalVariable:
			opcodesPtr = OP_CreateLocalVariable_Decompile(gscBuffer, opcodesPtr);
			break;
		case OP_EvalLocalVariableCached:
			opcodesPtr = OP_EvalLocalVariableCached_Decompile(gscBuffer, opcodesPtr);
			break;
		case OP_EvalArray:
			opcodesPtr = OP_EvalArray_Decompile(gscBuffer, opcodesPtr);
			break;
		case OP_EvalArrayRef:
			opcodesPtr = OP_EvalArrayRef_Decompile(gscBuffer, opcodesPtr);
			break;
		case OP_ClearArray:
			opcodesPtr = OP_ClearArray_Decompile(gscBuffer, opcodesPtr);
			break;
		case OP_EmptyArray:
			opcodesPtr = OP_EmptyArray_Decompile(gscBuffer, opcodesPtr);
			break;
		case OP_GetSelfObject:
			opcodesPtr = OP_GetSelfObject_Decompile(gscBuffer, opcodesPtr);
			break;
		case OP_EvalFieldVariable:
			opcodesPtr = OP_EvalFieldVariable_Decompile(gscBuffer, opcodesPtr);
			break;
		case OP_ClearFieldVariable:
			opcodesPtr = OP_ClearFieldVariable_Decompile(gscBuffer, opcodesPtr);
			break;
		case OP_SafeCreateVariableFieldCached:
			opcodesPtr = OP_SafeCreateVariableFieldCached_Decompile(gscBuffer, opcodesPtr);
			break;
		case OP_SafeSetWaittillVariableFieldCached:
			opcodesPtr = OP_SafeSetWaittillVariableFieldCached_Decompile(gscBuffer, opcodesPtr);
			break;
		case OP_clearparams:
			opcodesPtr = OP_clearparams_Decompile(gscBuffer, opcodesPtr);
			break;
		case OP_checkclearparams:
			opcodesPtr = OP_checkclearparams_Decompile(gscBuffer, opcodesPtr);
			break;
		case OP_EvalLocalVariableRefCached:
			opcodesPtr = OP_EvalLocalVariableRefCached_Decompile(gscBuffer, opcodesPtr);
			break;
		case OP_SetVariableField:
			opcodesPtr = OP_SetVariableField_Decompile(gscBuffer, opcodesPtr);
			break;
		case OP_wait:
			opcodesPtr = OP_wait_Decompile(gscBuffer, opcodesPtr);
			break;
		case OP_waittillFrameEnd:
			opcodesPtr = OP_waittillFrameEnd_Decompile(gscBuffer, opcodesPtr);
			break;
		case OP_PreScriptCall:
			opcodesPtr = OP_PreScriptCall_Decompile(gscBuffer, opcodesPtr);
			break;
		case OP_ScriptFunctionCall:
			opcodesPtr = OP_ScriptFunctionCall_Decompile(gscBuffer, opcodesPtr);
			break;
		case OP_ScriptMethodCall:
			opcodesPtr = OP_ScriptMethodCall_Decompile(gscBuffer, opcodesPtr);
			break;
		case OP_ScriptThreadCall:
			opcodesPtr = OP_ScriptThreadCall_Decompile(gscBuffer, opcodesPtr);
			break;
		case OP_ScriptMethodThreadCall:
			opcodesPtr = OP_ScriptMethodThreadCall_Decompile(gscBuffer, opcodesPtr);
			break;
		case OP_DecTop:
			opcodesPtr = OP_DecTop_Decompile(gscBuffer, opcodesPtr);
			break;
		case OP_CastFieldObject:
			opcodesPtr = OP_CastFieldObject_Decompile(gscBuffer, opcodesPtr);
			break;
		case OP_CastBool:
			opcodesPtr = OP_CastBool_Decompile(gscBuffer, opcodesPtr);
			break;
		case OP_BoolNot:
			opcodesPtr = OP_BoolNot_Decompile(gscBuffer, opcodesPtr);
			break;
		case OP_BoolComplement:
			opcodesPtr = OP_BoolComplement_Decompile(gscBuffer, opcodesPtr);
			break;
		case OP_JumpOnFalse:
			opcodesPtr = OP_JumpOnFalse_Decompile(gscBuffer, opcodesPtr);
			break;
		case OP_JumpOnTrue:
			opcodesPtr = OP_JumpOnTrue_Decompile(gscBuffer, opcodesPtr);
			break;
		case OP_JumpOnFalseExpr:
			opcodesPtr = OP_JumpOnFalseExpr_Decompile(gscBuffer, opcodesPtr);
			break;
		case OP_JumpOnTrueExpr:
			opcodesPtr = OP_JumpOnTrueExpr_Decompile(gscBuffer, opcodesPtr);
			break;
		case OP_jump:
			opcodesPtr = OP_jump_Decompile(gscBuffer, opcodesPtr);
			break;
		case OP_inc:
			opcodesPtr = OP_inc_Decompile(gscBuffer, opcodesPtr);
			break;
		case OP_dec:
			opcodesPtr = OP_dec_Decompile(gscBuffer, opcodesPtr);
			break;
		case OP_bit_or:
			opcodesPtr = OP_bit_or_Decompile(gscBuffer, opcodesPtr);
			break;
		case OP_bit_ex_or:
			opcodesPtr = OP_bit_ex_or_Decompile(gscBuffer, opcodesPtr);
			break;
		case OP_bit_and:
			opcodesPtr = OP_bit_and_Decompile(gscBuffer, opcodesPtr);
			break;
		case OP_equality:
			opcodesPtr = OP_equality_Decompile(gscBuffer, opcodesPtr);
			break;
		case OP_inequality:
			opcodesPtr = OP_inequality_Decompile(gscBuffer, opcodesPtr);
			break;
		case OP_less:
			opcodesPtr = OP_less_Decompile(gscBuffer, opcodesPtr);
			break;
		case OP_greater:
			opcodesPtr = OP_greater_Decompile(gscBuffer, opcodesPtr);
			break;
		case OP_less_equal:
			opcodesPtr = OP_less_equal_Decompile(gscBuffer, opcodesPtr);
			break;
		case OP_greater_equal:
			opcodesPtr = OP_greater_equal_Decompile(gscBuffer, opcodesPtr);
			break;
		case OP_shift_left:
			opcodesPtr = OP_shift_left_Decompile(gscBuffer, opcodesPtr);
			break;
		case OP_shift_right:
			opcodesPtr = OP_shift_right_Decompile(gscBuffer, opcodesPtr);
			break;
		case OP_plus:
			opcodesPtr = OP_plus_Decompile(gscBuffer, opcodesPtr);
			break;
		case OP_minus:
			opcodesPtr = OP_minus_Decompile(gscBuffer, opcodesPtr);
			break;
		case OP_multiply:
			opcodesPtr = OP_multiply_Decompile(gscBuffer, opcodesPtr);
			break;
		case OP_divide:
			opcodesPtr = OP_divide_Decompile(gscBuffer, opcodesPtr);
			break;
		case OP_mod:
			opcodesPtr = OP_mod_Decompile(gscBuffer, opcodesPtr);
			break;
		case OP_size:
			opcodesPtr = OP_size_Decompile(gscBuffer, opcodesPtr);
			break;
		case OP_waittillmatch:
			opcodesPtr = OP_waittillmatch_Decompile(gscBuffer, opcodesPtr);
			break;
		case OP_waittill:
			opcodesPtr = OP_waittill_Decompile(gscBuffer, opcodesPtr);
			break;
		case OP_notify:
			opcodesPtr = OP_notify_Decompile(gscBuffer, opcodesPtr);
			break;
		case OP_endon:
			opcodesPtr = OP_endon_Decompile(gscBuffer, opcodesPtr);
			break;
		case OP_voidCodepos:
			opcodesPtr = OP_voidCodepos_Decompile(gscBuffer, opcodesPtr);
			break;
		case OP_vector:
			opcodesPtr = OP_vector_Decompile(gscBuffer, opcodesPtr);
			break;
		case OP_GetHash:
			opcodesPtr = OP_GetHash_Decompile(gscBuffer, opcodesPtr);
			break;
		case OP_isdefined:
			opcodesPtr = OP_isdefined_Decompile(gscBuffer, opcodesPtr);
			break;
		case OP_gettime:
			opcodesPtr = OP_gettime_Decompile(gscBuffer, opcodesPtr);
			break;
		case OP_getdvar:
			opcodesPtr = OP_getdvar_Decompile(gscBuffer, opcodesPtr);
			break;
		case OP_getdvarint:
			opcodesPtr = OP_getdvarint_Decompile(gscBuffer, opcodesPtr);
			break;
		case OP_getdvarfloat:
			opcodesPtr = OP_getdvarfloat_Decompile(gscBuffer, opcodesPtr);
			break;
		case OP_getdvarvector:
			opcodesPtr = OP_getdvarvector_Decompile(gscBuffer, opcodesPtr);
			break;
		case OP_GetFirstArrayKey:
			opcodesPtr = OP_GetFirstArrayKey_Decompile(gscBuffer, opcodesPtr);
			break;
		case OP_GetNextArrayKey:
			opcodesPtr = OP_GetNextArrayKey_Decompile(gscBuffer, opcodesPtr);
			break;
		case OP_GetUndefined2:
			opcodesPtr = OP_GetUndefined2_Decompile(gscBuffer, opcodesPtr);
			break;
		case OP_skipdev:
			opcodesPtr = OP_skipdev_Decompile(gscBuffer, opcodesPtr);
			break;
		default:
			AddString("\t/* Error: unknown opcode (0x%X) */\n", true, *opcodesPtr);
			return;
		}

		crc32.AddData(oldOpcodesPtr, opcodesPtr - oldOpcodesPtr);

		if (crc32.GetCrc32() == gscFunc->crc32) // stop decompiling if its the function's end
			break;
	}
}