#pragma once

using namespace std;

#define DECL_DECOMPILE(name) void OP_ ## name ## _Decompile()

class GSCDecompilerClass
{
	// main variables & functions //
	string decompiledBuffer;
	void DecompilerOut(char* format, bool addTabLevel, ...);

	DWORD curTabLevel;
	void IncTabLevel()
	{
		curTabLevel++;
	}

	void DecTabLevel()
	{
		if (curTabLevel)
			curTabLevel--;
	}

	void ResetTabLevel()
	{
		curTabLevel = 0;
	}

public:
	string decompile(vmStack_s* initialStack, DWORD gscBuffer, DWORD start, DWORD size, bool isFunction, DWORD initialTabLevel);

	// structs & variables needed for decompilation //
	bool opcode_dec; // decompile into opcodes or gsc code?

private:
	// current opcode pointer that is being decompiled
	BYTE* opcodesPtr;

	// saved params from this->decompile
	DWORD gscBuffer;
	DWORD decompileStart;
	DWORD decompileSize;
	bool isFunction;

	// the stack (normal stack & local variable stack)
	vmStack_s stack;

	// some vars needed for decompilation
	ObjectType CurrentObject;
	char* CastedObject;
	
	char VariableNameBuffer[512];

	// decompiler helper functions //
	char* GetStringForEscapeSequenceByte(char byte);
	char* GetStringForCurrentObject();

	char* funcname_prepend_gscOfFunction(char* functionName, DWORD ip);
	void waittill_call_decompile(char* functionName, BYTE* currentPos);
	void call_decompile(char* functionName, bool hasPrecodepos, DWORD _numOfParameters, bool pointerCall, bool methodCall, bool threadCall, bool resultUnused);

	BYTE* switch_decompile(BYTE* currentPos);

	int GetOperatorPrecedenceLevel(OperatorType operatorType);
	char* GetStringForOperator(OperatorType operatorType);
	void build_operation(int index);
	void operator_decompile(OperatorType operatorType);
	bool SetVariableField_compound_assignment_decompile();

	BYTE* SetVariableField_foreach_decompile(BYTE* currentPos);

	bool PatternFound(bo2_opcode* pattern, DWORD numOfOpcodesToMatch, BYTE* currentPos);

	void WriteRegisterInfo(BYTE* ip);

	// opcode decompiling functions //
	DECL_DECOMPILE(End);								// 0x00
	DECL_DECOMPILE(Return);								// 0x01
	DECL_DECOMPILE(GetUndefined);						// 0x02
	DECL_DECOMPILE(GetZero);							// 0x03
	DECL_DECOMPILE(GetByte);							// 0x04
	DECL_DECOMPILE(GetNegByte);							// 0x05
	DECL_DECOMPILE(GetUnsignedShort);					// 0x06
	DECL_DECOMPILE(GetNegUnsignedShort);				// 0x07
	DECL_DECOMPILE(GetInteger);							// 0x08
	DECL_DECOMPILE(GetFloat);							// 0x09
	DECL_DECOMPILE(GetString);							// 0x0A
	DECL_DECOMPILE(GetIString);							// 0x0B
	DECL_DECOMPILE(GetVector);							// 0x0C
	DECL_DECOMPILE(GetLevelObject);						// 0x0D
	DECL_DECOMPILE(GetAnimObject);						// 0x0E
	DECL_DECOMPILE(GetSelf);							// 0x0F
	DECL_DECOMPILE(GetLevel);							// 0x10
	DECL_DECOMPILE(GetGame);							// 0x11
	DECL_DECOMPILE(GetAnim);							// 0x12
	DECL_DECOMPILE(GetAnimation);						// 0x13
	DECL_DECOMPILE(GetGameRef);							// 0x14
	DECL_DECOMPILE(GetFunction);						// 0x15
	DECL_DECOMPILE(CreateLocalVariable);				// 0x16
	DECL_DECOMPILE(CreateLocalVariables);				// 0x17
	DECL_DECOMPILE(EvalLocalVariableCached);			// 0x19
	DECL_DECOMPILE(EvalArray);							// 0x1A
	DECL_DECOMPILE(EvalArrayRef);						// 0x1C
	DECL_DECOMPILE(ClearArray);							// 0x1D
	DECL_DECOMPILE(EmptyArray);							// 0x1E
	DECL_DECOMPILE(GetSelfObject);						// 0x1F
	DECL_DECOMPILE(EvalFieldVariable);					// 0x20
	DECL_DECOMPILE(EvalFieldVariableRef);				// 0x21
	DECL_DECOMPILE(ClearFieldVariable);					// 0x22
	DECL_DECOMPILE(SafeSetWaittillVariableFieldCached);	// 0x24
	DECL_DECOMPILE(clearparams);						// 0x25
	DECL_DECOMPILE(checkclearparams);					// 0x26
	DECL_DECOMPILE(EvalLocalVariableRefCached);			// 0x27
	DECL_DECOMPILE(SetVariableField);					// 0x28
	DECL_DECOMPILE(wait);								// 0x2B
	DECL_DECOMPILE(waittillFrameEnd);					// 0x2C
	DECL_DECOMPILE(PreScriptCall);						// 0x2D
	DECL_DECOMPILE(ScriptFunctionCall);					// 0x2E
	DECL_DECOMPILE(ScriptFunctionCallPointer);			// 0x2F
	DECL_DECOMPILE(ScriptMethodCall);					// 0x30
	DECL_DECOMPILE(ScriptMethodCallPointer);			// 0x31
	DECL_DECOMPILE(ScriptThreadCall);					// 0x32
	DECL_DECOMPILE(ScriptThreadCallPointer);			// 0x33
	DECL_DECOMPILE(ScriptMethodThreadCall);				// 0x34
	DECL_DECOMPILE(ScriptMethodThreadCallPointer);		// 0x35
	DECL_DECOMPILE(DecTop);								// 0x36
	DECL_DECOMPILE(CastFieldObject);					// 0x37
	DECL_DECOMPILE(CastBool);							// 0x38
	DECL_DECOMPILE(BoolNot);							// 0x39
	DECL_DECOMPILE(BoolComplement);						// 0x3A
	DECL_DECOMPILE(JumpOnFalse);						// 0x3B
	DECL_DECOMPILE(JumpOnTrue);							// 0x3C
	DECL_DECOMPILE(JumpOnFalseExpr);					// 0x3D
	DECL_DECOMPILE(JumpOnTrueExpr);						// 0x3E
	DECL_DECOMPILE(jump);								// 0x3F
	DECL_DECOMPILE(inc);								// 0x41
	DECL_DECOMPILE(dec);								// 0x42
	DECL_DECOMPILE(bit_or);								// 0x43
	DECL_DECOMPILE(bit_ex_or);							// 0x44
	DECL_DECOMPILE(bit_and);							// 0x45
	DECL_DECOMPILE(equality);							// 0x46
	DECL_DECOMPILE(inequality);							// 0x47
	DECL_DECOMPILE(less);								// 0x48
	DECL_DECOMPILE(greater);							// 0x49
	DECL_DECOMPILE(less_equal);							// 0x4A
	DECL_DECOMPILE(greater_equal);						// 0x4B
	DECL_DECOMPILE(shift_left);							// 0x4C
	DECL_DECOMPILE(shift_right);						// 0x4D
	DECL_DECOMPILE(plus);								// 0x4E
	DECL_DECOMPILE(minus);								// 0x4F
	DECL_DECOMPILE(multiply);							// 0x50
	DECL_DECOMPILE(divide);								// 0x51
	DECL_DECOMPILE(mod);								// 0x52
	DECL_DECOMPILE(size);								// 0x53
	DECL_DECOMPILE(waittillmatch);						// 0x54
	DECL_DECOMPILE(waittill);							// 0x55
	DECL_DECOMPILE(notify);								// 0x56
	DECL_DECOMPILE(endon);								// 0x57
	DECL_DECOMPILE(voidCodepos);						// 0x58
	DECL_DECOMPILE(switch);								// 0x59
	DECL_DECOMPILE(endswitch);							// 0x5A
	DECL_DECOMPILE(vector);								// 0x5B
	DECL_DECOMPILE(GetHash);							// 0x5C
	DECL_DECOMPILE(GetSimpleVector);					// 0x5E
	DECL_DECOMPILE(isdefined);							// 0x5F
	DECL_DECOMPILE(vectorscale);						// 0x60
	DECL_DECOMPILE(anglestoup);							// 0x61
	DECL_DECOMPILE(anglestoright);						// 0x62
	DECL_DECOMPILE(anglestoforward);					// 0x63
	DECL_DECOMPILE(angleclamp180);						// 0x64
	DECL_DECOMPILE(vectorstoangle);						// 0x65
	DECL_DECOMPILE(abs);								// 0x66
	DECL_DECOMPILE(gettime);							// 0x67
	DECL_DECOMPILE(getdvar);							// 0x68
	DECL_DECOMPILE(getdvarint);							// 0x69
	DECL_DECOMPILE(getdvarfloat);						// 0x6A
	DECL_DECOMPILE(getdvarvector);						// 0x6B
	DECL_DECOMPILE(getdvarcolorred);					// 0x6C
	DECL_DECOMPILE(getdvarcolorgreen);					// 0x6D
	DECL_DECOMPILE(getdvarcolorblue);					// 0x6E
	DECL_DECOMPILE(getdvarcoloralpha);					// 0x6F
	DECL_DECOMPILE(GetFirstArrayKey);					// 0x70
	DECL_DECOMPILE(GetNextArrayKey);					// 0x71
	DECL_DECOMPILE(GetUndefined2);						// 0x73
	DECL_DECOMPILE(skipdev);							// 0x7B

	// stack functions //
	void InitStacks();
	void FreeStacks();

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
	void StackPop();
	DWORD StackGetValue(int index);
	VariableType StackGetValueType(int index);
	DWORD StackGetRelativePos();

	OperatorsInfo* StackGetOperatorsInfo(int index);
	void StackSetOperatorsInfo(OperatorsInfo* operatorsInfo);

	// local stack
	void StackLocalPush(char* varName);
	void StackLocalPop();
	char* StackLocalGetValue(int index);

public:
	// class constructor //
	GSCDecompilerClass()
	{
		// reset to 0 every member from this class (can't use memset since it will leave memory leaks because of string decompiledBuffer)
		this->curTabLevel = 0;

		this->opcode_dec = false;

		this->opcodesPtr = 0;

		this->gscBuffer = 0;
		this->decompileStart = 0;
		this->decompileSize = 0;
		this->isFunction = false;

		this->CurrentObject = OBJECT_NONE;
		this->CastedObject = 0;

		memset(this->VariableNameBuffer, 0, sizeof(VariableNameBuffer));
		
		// init the stacks
		this->InitStacks();
	}

	// class destructor //
	~GSCDecompilerClass()
	{
		// free the stacks
		this->FreeStacks();

		// free the current casted object if there's one
		if (this->CastedObject)
			free(this->CastedObject);
	}
};