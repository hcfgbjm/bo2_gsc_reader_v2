#include "stdafx.h"

// 0x0
BYTE* OP_End_Decompile(DWORD gscBuffer, BYTE* opcodesPtr, bool functionEnd);

// 0x1
BYTE* OP_Return_Decompile(DWORD gscBuffer, BYTE* opcodesPtr);

// 0x2
BYTE* OP_GetUndefined_Decompile(DWORD gscBuffer, BYTE* opcodesPtr);

// 0x3
BYTE* OP_GetZero_Decompile(DWORD gscBuffer, BYTE* opcodesPtr);

// 0x4
BYTE* OP_GetByte_Decompile(DWORD gscBuffer, BYTE* opcodesPtr);

// 0x5
BYTE* OP_GetNegByte_Decompile(DWORD gscBuffer, BYTE* opcodesPtr);

// 0x6
BYTE* OP_GetUnsignedShort_Decompile(DWORD gscBuffer, BYTE* opcodesPtr);

// 0x7
BYTE* OP_GetNegUnsignedShort_Decompile(DWORD gscBuffer, BYTE* opcodesPtr);

// 0x8
BYTE* OP_GetInteger_Decompile(DWORD gscBuffer, BYTE* opcodesPtr);

// 0x9
BYTE* OP_GetFloat_Decompile(DWORD gscBuffer, BYTE* opcodesPtr);

// 0xA
BYTE* OP_GetString_Decompile(DWORD gscBuffer, BYTE* opcodesPtr);

// 0xB
BYTE* OP_GetIString_Decompile(DWORD gscBuffer, BYTE* opcodesPtr);

// 0xC
BYTE* OP_GetVector_Decompile(DWORD gscBuffer, BYTE* opcodesPtr);

// 0xD
BYTE* OP_GetLevelObject_Decompile(DWORD gscBuffer, BYTE* opcodesPtr);

// 0xE
BYTE* OP_GetAnimObject_Decompile(DWORD gscBuffer, BYTE* opcodesPtr);

// 0xF
BYTE* OP_GetSelf_Decompile(DWORD gscBuffer, BYTE* opcodesPtr);

// 0x10
BYTE* OP_GetLevel_Decompile(DWORD gscBuffer, BYTE* opcodesPtr);

// 0x11
BYTE* OP_GetGame_Decompile(DWORD gscBuffer, BYTE* opcodesPtr);

// 0x12
BYTE* OP_GetAnim_Decompile(DWORD gscBuffer, BYTE* opcodesPtr);

// 0x13
BYTE* OP_GetAnimation_Decompile(DWORD gscBuffer, BYTE* opcodesPtr);

// 0x14
BYTE* OP_GetGameRef_Decompile(DWORD gscBuffer, BYTE* opcodesPtr);

// 0x15
BYTE* OP_GetFunction_Decompile(DWORD gscBuffer, BYTE* opcodesPtr);

// 0x17
BYTE* OP_CreateLocalVariable_Decompile(DWORD gscBuffer, BYTE* opcodesPtr);

// 0x19
BYTE* OP_EvalLocalVariableCached_Decompile(DWORD gscBuffer, BYTE* opcodesPtr);

// 0x1A
BYTE* OP_EvalArray_Decompile(DWORD gscBuffer, BYTE* opcodesPtr);

// 0x1C
BYTE* OP_EvalArrayRef_Decompile(DWORD gscBuffer, BYTE* opcodesPtr);

// 0x1D
BYTE* OP_ClearArray_Decompile(DWORD gscBuffer, BYTE* opcodesPtr);

// 0x1E
BYTE* OP_EmptyArray_Decompile(DWORD gscBuffer, BYTE* opcodesPtr);

// 0x1F
BYTE* OP_GetSelfObject_Decompile(DWORD gscBuffer, BYTE* opcodesPtr);

// 0x20
BYTE* OP_EvalFieldVariable_Decompile(DWORD gscBuffer, BYTE* opcodesPtr);

// 0x21
BYTE* OP_ClearFieldVariable_Decompile(DWORD gscBuffer, BYTE* opcodesPtr);

// 0x22
BYTE* OP_SafeCreateVariableFieldCached_Decompile(DWORD gscBuffer, BYTE* opcodesPtr);

// 0x24
BYTE* OP_SafeSetWaittillVariableFieldCached_Decompile(DWORD gscBuffer, BYTE* opcodesPtr);

// 0x25
BYTE* OP_clearparams_Decompile(DWORD gscBuffer, BYTE* opcodesPtr);

// 0x26
BYTE* OP_checkclearparams_Decompile(DWORD gscBuffer, BYTE* opcodesPtr);

// 0x27
BYTE* OP_EvalLocalVariableRefCached_Decompile(DWORD gscBuffer, BYTE* opcodesPtr);

// 0x28
BYTE* OP_SetVariableField_Decompile(DWORD gscBuffer, BYTE* opcodesPtr);

// 0x2B
BYTE* OP_wait_Decompile(DWORD gscBuffer, BYTE* opcodesPtr);

// 0x2C
BYTE* OP_waittillFrameEnd_Decompile(DWORD gscBuffer, BYTE* opcodesPtr);

// 0x2D
BYTE* OP_PreScriptCall_Decompile(DWORD gscBuffer, BYTE* opcodesPtr);

// 0x2E
BYTE* OP_ScriptFunctionCall_Decompile(DWORD gscBuffer, BYTE* opcodesPtr);

// 0x30
BYTE* OP_ScriptMethodCall_Decompile(DWORD gscBuffer, BYTE* opcodesPtr);

// 0x32
BYTE* OP_ScriptThreadCall_Decompile(DWORD gscBuffer, BYTE* opcodesPtr);

// 0x34
BYTE* OP_ScriptMethodThreadCall_Decompile(DWORD gscBuffer, BYTE* opcodesPtr);

// 0x36
BYTE* OP_DecTop_Decompile(DWORD gscBuffer, BYTE* opcodesPtr);

// 0x37
BYTE* OP_CastFieldObject_Decompile(DWORD gscBuffer, BYTE* opcodesPtr);

// 0x38
BYTE* OP_CastBool_Decompile(DWORD gscBuffer, BYTE* opcodesPtr);

// 0x39
BYTE* OP_BoolNot_Decompile(DWORD gscBuffer, BYTE* opcodesPtr);

// 0x3A
BYTE* OP_BoolComplement_Decompile(DWORD gscBuffer, BYTE* opcodesPtr);

// 0x3B
BYTE* OP_JumpOnFalse_Decompile(DWORD gscBuffer, BYTE* opcodesPtr);

// 0x3C
BYTE* OP_JumpOnTrue_Decompile(DWORD gscBuffer, BYTE* opcodesPtr);

// 0x3D
BYTE* OP_JumpOnFalseExpr_Decompile(DWORD gscBuffer, BYTE* opcodesPtr);

// 0x3E
BYTE* OP_JumpOnTrueExpr_Decompile(DWORD gscBuffer, BYTE* opcodesPtr);

// 0x3F
BYTE* OP_jump_Decompile(DWORD gscBuffer, BYTE* opcodesPtr);

// 0x41
BYTE* OP_inc_Decompile(DWORD gscBuffer, BYTE* opcodesPtr);

// 0x42
BYTE* OP_dec_Decompile(DWORD gscBuffer, BYTE* opcodesPtr);

// 0x43
BYTE* OP_bit_or_Decompile(DWORD gscBuffer, BYTE* opcodesPtr);

// 0x44
BYTE* OP_bit_ex_or_Decompile(DWORD gscBuffer, BYTE* opcodesPtr);

// 0x45
BYTE* OP_bit_and_Decompile(DWORD gscBuffer, BYTE* opcodesPtr);

// 0x46
BYTE* OP_equality_Decompile(DWORD gscBuffer, BYTE* opcodesPtr);

// 0x47
BYTE* OP_inequality_Decompile(DWORD gscBuffer, BYTE* opcodesPtr);

// 0x48
BYTE* OP_less_Decompile(DWORD gscBuffer, BYTE* opcodesPtr);

// 0x49
BYTE* OP_greater_Decompile(DWORD gscBuffer, BYTE* opcodesPtr);

// 0x4A
BYTE* OP_less_equal_Decompile(DWORD gscBuffer, BYTE* opcodesPtr);

// 0x4B
BYTE* OP_greater_equal_Decompile(DWORD gscBuffer, BYTE* opcodesPtr);

// 0x4C
BYTE* OP_shift_left_Decompile(DWORD gscBuffer, BYTE* opcodesPtr);

// 0x4D
BYTE* OP_shift_right_Decompile(DWORD gscBuffer, BYTE* opcodesPtr);

// 0x4E
BYTE* OP_plus_Decompile(DWORD gscBuffer, BYTE* opcodesPtr);

// 0x4F
BYTE* OP_minus_Decompile(DWORD gscBuffer, BYTE* opcodesPtr);

// 0x50
BYTE* OP_multiply_Decompile(DWORD gscBuffer, BYTE* opcodesPtr);

// 0x51
BYTE* OP_divide_Decompile(DWORD gscBuffer, BYTE* opcodesPtr);

// 0x52
BYTE* OP_mod_Decompile(DWORD gscBuffer, BYTE* opcodesPtr);

// 0x53
BYTE* OP_size_Decompile(DWORD gscBuffer, BYTE* opcodesPtr);

// 0x54
BYTE* OP_waittillmatch_Decompile(DWORD gscBuffer, BYTE* opcodesPtr);

// 0x55
BYTE* OP_waittill_Decompile(DWORD gscBuffer, BYTE* opcodesPtr);

// 0x56
BYTE* OP_notify_Decompile(DWORD gscBuffer, BYTE* opcodesPtr);

// 0x57
BYTE* OP_endon_Decompile(DWORD gscBuffer, BYTE* opcodesPtr);

// 0x58
BYTE* OP_voidCodepos_Decompile(DWORD gscBuffer, BYTE* opcodesPtr);

// 0x59
BYTE* OP_switch_Decompile(DWORD gscBuffer, BYTE* opcodesPtr);

// 0x5A
BYTE *OP_endswitch_Decompile(DWORD gscBuffer, BYTE *opcodesPtr);

// 0x5B
BYTE* OP_vector_Decompile(DWORD gscBuffer, BYTE* opcodesPtr);

// 0x5C
BYTE* OP_GetHash_Decompile(DWORD gscBuffer, BYTE* opcodesPtr);

// 0x5F
BYTE* OP_isdefined_Decompile(DWORD gscBuffer, BYTE* opcodesPtr);

// 0x67
BYTE* OP_gettime_Decompile(DWORD gscBuffer, BYTE* opcodesPtr);

// 0x68
BYTE* OP_getdvar_Decompile(DWORD gscBuffer, BYTE* opcodesPtr);

// 0x69
BYTE* OP_getdvarint_Decompile(DWORD gscBuffer, BYTE* opcodesPtr);

// 0x6A
BYTE* OP_getdvarfloat_Decompile(DWORD gscBuffer, BYTE* opcodesPtr);

// 0x6B
BYTE* OP_getdvarvector_Decompile(DWORD gscBuffer, BYTE* opcodesPtr);

// 0x70
BYTE* OP_GetFirstArrayKey_Decompile(DWORD gscBuffer, BYTE* opcodesPtr);

// 0x71
BYTE* OP_GetNextArrayKey_Decompile(DWORD gscBuffer, BYTE* opcodesPtr);

// 0x73
BYTE* OP_GetUndefined2_Decompile(DWORD gscBuffer, BYTE* opcodesPtr);

// 0x7B
BYTE* OP_skipdev_Decompile(DWORD gscBuffer, BYTE* opcodesPtr);