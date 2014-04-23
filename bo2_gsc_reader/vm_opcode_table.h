#pragma once

// cod4 - http://pastebin.com/BShXc6Zr
// or see cod4_opcodes.txt
// check also the structs near it
// credits for finding - master131
// looks like we're going to need to emulate the entire
// virtual machine so we can know what it is doing
// (mainly because of the stack and cache stuff)

// we should be using this enum instead of the defines
// however this enum isn't fully correct yet

// Ref = dont use stack
// else use stack
enum bo2_opcode
{
	OP_End = 0x0,
	OP_Return = 0x1,
	OP_GetUndefined = 0x2, // does the same thing as OP_GetUndefined2
	OP_GetZero = 0x3,
	OP_GetByte = 0x4,
	OP_GetNegByte = 0x5,
	OP_GetUnsignedShort = 0x6,
	OP_GetNegUnsignedShort = 0x7,
	OP_GetInteger = 0x8,
	OP_GetFloat = 0x9,
	OP_GetString = 0xA,
	OP_GetIString = 0xB,
	OP_GetVector = 0xC,
	OP_GetLevelObject = 0xD,
	OP_GetAnimObject = 0xE,
	OP_GetSelf = 0xF,
	OP_GetLevel = 0x10,
	OP_GetGame = 0x11,
	OP_GetAnim = 0x12,
	OP_GetAnimation = 0x13,
	OP_GetGameRef = 0x14,
	OP_GetFunction = 0x15,
	OP_CreateLocalVariable = 0x16,
	OP_CreateLocalVariables = 0x17,
	OP_RemoveLocalVariables = 0x18,
	OP_EvalLocalVariableCached = 0x19,
	OP_EvalArray = 0x1A,
	OP_EvalLocalArrayRefCached = 0x1B, // not handled
	OP_EvalArrayRef = 0x1C,
	OP_ClearArray = 0x1D,
	OP_EmptyArray = 0x1E,
	OP_GetSelfObject = 0x1F,
	OP_EvalFieldVariable = 0x20,
	OP_EvalFieldVariableRef = 0x21,
	OP_ClearFieldVariable = 0x22,
	OP_SafeSetVariableFieldCached = 0x23, // this opcode isn't handled yet (is it even used?)
	OP_SafeSetWaittillVariableFieldCached = 0x24,
	OP_clearparams = 0x25,
	OP_checkclearparams = 0x26,
	OP_EvalLocalVariableRefCached = 0x27,
	OP_SetVariableField = 0x28,
	OP_CallBuiltin = 0x29,
	OP_CallBuiltinMethod = 0x2A,
	OP_wait = 0x2B,
	OP_waittillFrameEnd = 0x2C,
	OP_PreScriptCall = 0x2D, // does the same thing as OP_voidCodepos
	OP_ScriptFunctionCall = 0x2E,
	OP_ScriptFunctionCallPointer = 0x2F,
	OP_ScriptMethodCall = 0x30,
	OP_ScriptMethodCallPointer = 0x31,
	OP_ScriptThreadCall = 0x32,
	OP_ScriptThreadCallPointer = 0x33,
	OP_ScriptMethodThreadCall = 0x34,
	OP_ScriptMethodThreadCallPointer = 0x35,
	OP_DecTop = 0x36,
	OP_CastFieldObject = 0x37,
	OP_CastBool = 0x38,
	OP_BoolNot = 0x39,
	OP_BoolComplement = 0x3A,
	OP_JumpOnFalse = 0x3B,
	OP_JumpOnTrue = 0x3C,
	OP_JumpOnFalseExpr = 0x3D,
	OP_JumpOnTrueExpr = 0x3E,
	/*
	OP_JumpOnFalse		: does stack -= 8
	OP_JumpOnTrue		: does stack -= 8
	OP_JumpOnFalseExpr	: if (*(DWORD*)stack), stack -= 8 (if doesn't jump does stuff to stack)
	OP_JumpOnTrueExpr	: if (!*(DWORD*)stack), stack -= 8 (if doesn't jump does stuff to stack)
	There is no code written by the decompiler if it's an Expr if
	*/
	OP_jump = 0x3F, // does same as OP_skipdev
	OP_jumpback = 0x40, // not handled
	OP_inc = 0x41,
	OP_dec = 0x42,
	OP_bit_or = 0x43,
	OP_bit_ex_or = 0x44,
	OP_bit_and = 0x45,
	OP_equality = 0x46,
	OP_inequality = 0x47,
	OP_less = 0x48,
	OP_greater = 0x49,
	OP_less_equal = 0x4A,
	OP_greater_equal = 0x4B,
	OP_shift_left = 0x4C,
	OP_shift_right = 0x4D,
	OP_plus = 0x4E,
	OP_minus = 0x4F,
	OP_multiply = 0x50,
	OP_divide = 0x51,
	OP_mod = 0x52,
	OP_size = 0x53,
	OP_waittillmatch = 0x54, // does same as OP_waittill
	OP_waittill = 0x55, // does same as OP_waittillmatch
	OP_notify = 0x56,
	OP_endon = 0x57,
	OP_voidCodepos = 0x58, // does the same thing as OP_PreScriptCall
	OP_switch = 0x59,
	OP_endswitch = 0x5A,
	OP_vector = 0x5B,
	OP_GetHash = 0x5C, // doesn't exist in cod4
	OP_realwait = 0x5D,
	OP_GetSimpleVector = 0x5E, // doesn't exist in cod4 (the functions below neither do)
	OP_isdefined = 0x5F,
	OP_vectorscale = 0x60,
	OP_anglestoup = 0x61,
	OP_anglestoright = 0x62,
	OP_anglestoforward = 0x63,
	OP_angleclamp180 = 0x64,
	OP_vectorstoangle = 0x65,
	OP_abs = 0x66,
	OP_gettime = 0x67, // doesn't exist in cod4
	OP_getdvar = 0x68, // returns a string
	OP_getdvarint = 0x69, // returns an int
	OP_getdvarfloat = 0x6A, // returns a float
	OP_getdvarvector = 0x6B, // returns a 3d vector
	OP_getdvarcolorred = 0x6C,
	OP_getdvarcolorgreen = 0x6D,
	OP_getdvarcolorblue = 0x6E,
	OP_getdvarcoloralpha = 0x6F,
	OP_GetFirstArrayKey = 0x70,
	OP_GetNextArrayKey = 0x71,
	OP_weird3 = 0x72, // this one is easy, too lazy to implement
	OP_GetUndefined2 = 0x73, // does the same thing as OP_GetUndefined
	OP_Unknown74 = 0x74, // does OP_DecTop when last var is not of type type_codePos
	OP_NOP = 0x75,
	OP_abort = 0x76,
	OP_object = 0x77,
	OP_thread_object = 0x78, // not sure
	OP_EvalLocalVariable = 0x79,
	OP_EvalLocalVariableRef = 0x7A, // not sure
	OP_skipdev = 0x7B // does same as OP_jump
};