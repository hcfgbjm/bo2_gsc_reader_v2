#include "stdafx.h"

bool opcode_dec = false;

__inline BYTE* GET_ALIGNED_WORD			(BYTE* x) { return (BYTE*)(((DWORD)x + 1) & 0xFFFFFFFE); }
__inline BYTE* GET_ALIGNED_DWORD		(BYTE* x) { return (BYTE*)(((DWORD)x + 3) & 0xFFFFFFFC); }


char* CastedObject;

enum
{
	OBJECT_LEVEL,
	OBJECT_ANIM,
	OBJECT_SELF,
	OBJECT_CASTED
};

char VariableNameBuffer[256]; // enough?

DWORD CurrentObject;

char* GetStringForCurrentObject()
{
	switch (CurrentObject)
	{
	case OBJECT_LEVEL:
		return "level";
	case OBJECT_ANIM:
		return "anim";
	case OBJECT_SELF:
		return "self";
	case OBJECT_CASTED:
		return CastedObject;
	}

	return 0;
}

int GetFormattedLength(char *format, ...)
{
	va_list ap;

	va_start(ap, format);
	int length = _vscprintf(format, ap);
	va_end(ap);

	return length;
}

char* MallocAndSprintf(char* format, ...)
{
	va_list ap;

	va_start(ap, format);
	int length = _vscprintf(format, ap);
	va_end(ap);

	char* allocatedString = (char*)malloc(length + 1);

	va_start(ap, format);
	_vsnprintf_s(allocatedString, length + 1, length + 1, format, ap);
	va_end(ap);

	return allocatedString;
}

DWORD IfJumpEndStack[256]; // erase this, make something like, wait till result for jump is poped
DWORD* IfJumpEndStackPos = IfJumpEndStack - 4;

void CheckIfJumps(BYTE* opcodesPtr)
{
	if (*IfJumpEndStackPos == (DWORD)opcodesPtr)
	{
		DecTabLevel();
		AddString("}\n", true);
		IfJumpEndStackPos--;
	}
}

void jump_expression_decompile(BYTE jumpOP, DWORD jumpTo)
{
	bool includeNegation; // put a ! before the condition if true, for example, if ( !(IsDefined ( ... )) )

	switch (jumpOP)
	{
	case OP_JumpOnFalse:
	case OP_JumpOnFalseExpr:
		includeNegation = false;
		break;
	case OP_JumpOnTrue:
	case OP_JumpOnTrueExpr:
		includeNegation = true;
		break;
	}

	if (StackGetFlags() & OP_FLAG_BOOL_NOT)
		includeNegation = !includeNegation;

	AddString("if ( ", true); // an if and an opening brace

	if (includeNegation)
		AddString("!(", false);
	AddString((char*)StackGetLastValue(), false);
	if (includeNegation)
		AddString(")", false);

	AddString(" )\n", false); // closing if brace
	
	StackPop();

	IfJumpEndStackPos++;
	*IfJumpEndStackPos = jumpTo;

	AddString("{\n", true);

	IncTabLevel();
}

// if hasPrecodepos is true, it ignores _numOfParameters
void call_decompile(char* functionName, bool hasPrecodepos, DWORD _numOfParameters, bool methodCall, bool threadCall, bool resultUnused)
{
	DWORD numOfParameters = 0;

	if (hasPrecodepos)
	{
		for (DWORD i = 0; StackGetValueType(i) != type_precodepos; i++)
			numOfParameters++;
	}
	else
		numOfParameters = _numOfParameters;

	string _FunctionString;
	char* FunctionString;
	
	if (methodCall)
	{
		_FunctionString.append((char*)StackGetLastValue());
		_FunctionString.append(" ");

		StackPop(); // pop the method object (like self, etc...) from the stack

		if (hasPrecodepos)
			numOfParameters--; // if we don't do this, then the method will be counted as parameter and we don't want that
	}
	
	if (threadCall)
	{
		_FunctionString.append("thread");
		_FunctionString.append(" ");
	}

	if (numOfParameters) // if the function has parameters
	{
		_FunctionString.append(functionName);
		_FunctionString.append("( ");

		for (DWORD i = 0; i < numOfParameters; i++)
		{
			_FunctionString.append((char*)StackGetLastValue());
			if ((i + 1) < numOfParameters) // execute this only if it's not the last iteration
				_FunctionString.append(", ");

			StackPop(); // pop the parameter from the stack
		}

		_FunctionString.append(" )");
	}
	else
	{
		_FunctionString.append(functionName);
		_FunctionString.append("()");
	}

	if (hasPrecodepos) // pop the precodepos
		StackPop();

	// we just push the newly copied allocated decompiled string to the stack
	FunctionString = (char*)malloc(_FunctionString.length() + 1);
	strcpy_s(FunctionString, _FunctionString.length() + 1, _FunctionString.c_str());

	StackPush<char*>(FunctionString, type_decompiled_string);

	// resultUnused is true if there's a OP_DecTop after the function call, which means that it pops the result instantly
	if (resultUnused)
		AddString("%s;\n", true, FunctionString);
}

int GetOperatorPrecedenceLevel(OperatorType operatorType)
{
	switch (operatorType)
	{
	case OP_BOOLNOT:
	case OP_BOOLCOMPLEMENT:
		return 3;

	case OP_MULTIPLY:
	case OP_DIVIDE:
	case OP_MOD:
		return 5;

	case OP_PLUS:
	case OP_MINUS:
		return 6;

	case OP_SHIFT_LEFT:
	case OP_SHIFT_RIGHT:
		return 7;

	case OP_LESS:
	case OP_GREATER:
	case OP_LESS_EQUAL:
	case OP_GREATER_EQUAL:
		return 8;

	case OP_EQUALITY:
	case OP_INEQUALITY:
		return 9;
		
	case OP_BIT_AND:
		return 10;

	case OP_BIT_EX_OR:
		return 11;

	case OP_BIT_OR:
		return 12;

	case OP_LOGICAL_AND:
		return 13;

	case OP_LOGICAL_OR:
		return 14;

	default:
		return 0;
	};
}

char* GetStringForOperator(OperatorType operatorType)
{
	switch (operatorType)
	{
	case OP_BOOLNOT:
		return "!";
	case OP_BOOLCOMPLEMENT:
		return "~";
	case OP_MULTIPLY:
		return " * ";
	case OP_DIVIDE:
		return " / ";
	case OP_MOD:
		return " %% "; // double modulus or AddString will crash
	case OP_PLUS:
		return " + ";
	case OP_MINUS:
		return " - ";
	case OP_SHIFT_LEFT:
		return " << ";
	case OP_SHIFT_RIGHT:
		return " >> ";
	case OP_LESS:
		return " < ";
	case OP_GREATER:
		return " > ";
	case OP_LESS_EQUAL:
		return " <= ";
	case OP_GREATER_EQUAL:
		return " >= ";
	case OP_EQUALITY:
		return " == ";
	case OP_INEQUALITY:
		return " != ";
	case OP_BIT_AND:
		return " & ";
	case OP_BIT_EX_OR:
		return " ^ ";
	case OP_BIT_OR:
		return " | ";
	case OP_LOGICAL_AND:
		return " && ";
	case OP_LOGICAL_OR:
		return " || ";
	default:
		return NULL;
	}
}

// should be only called by StackGetValue!!!

// ok so what we need to fakin do here is, for example, we get this info from OperatorsInfo struct:
// operands:		3 ranklevel 0.5 10
// operators:		OP_PLUS OP_MULTIPLY OP_MULTIPLY
// execution order:	2 1 3
// now without correct execution order, the operation will be this: 3 + ranklevel * 0.5 * 10
// we see that the 2nd operator is executed first, then the 1st, and then the third
// by operator precedence, the first execution (2nd operator) doesn't need parenthesis because * > +
// however, the 1st operation needs parenthesis, otherwise it would do 0.5 * 10, and that's not what we want
// so in the end, what we get is: (3 + ranklevel * 0.5) * 10
void build_operation(int index)
{
	OperatorsInfo* operatorsInfo = StackGetOperatorsInfo(index);

	if (operatorsInfo->numOfOperators < 1)
	{
		cout << "Error: build_operation requested an operation to be built, but it has no operators" << endl;
		cin.get();
		ExitProcess(-1);
	}

	// new operators info which will be used to make the operation build easier
	// basically this one is allowed to have multiple operands in operandList[x] and parenthesis in them
	OperatorsInfo* newOperatorsInfo = (OperatorsInfo*)malloc(sizeof(OperatorsInfo));

	// clone operandsInfo into newOperandsInfo
	newOperatorsInfo->numOfOperands = operatorsInfo->numOfOperands;
	newOperatorsInfo->operandList = (char**)malloc(sizeof(char*) * operatorsInfo->numOfOperands);
	for (DWORD i = 0; i < newOperatorsInfo->numOfOperands; i++)
	{
		newOperatorsInfo->operandList[i] = (char*)malloc(strlen(operatorsInfo->operandList[i]) + 1);
		strcpy_s(newOperatorsInfo->operandList[i], strlen(operatorsInfo->operandList[i]) + 1, operatorsInfo->operandList[i]);
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

	DWORD* processedOperatorIndexes = (DWORD*)malloc(sizeof(DWORD) * newOperatorsInfo->numOfOperators);

	DWORD firstCorrectOperatorIndex = 0; // correct execution index of current processed operator (i)
	DWORD firstOperatorIndex = 0;

	// we do this until only 1 operator is left to be processed
	while (newOperatorsInfo->numOfOperators != 1)
	{
		// reset the processed operator indexes which will be used to calculate the order of execution by operator precedence
		for (DWORD i = 0; i < newOperatorsInfo->numOfOperators; i++)
			processedOperatorIndexes[i] = -1;

		// we loop each operator, get it's execution order by operator precedence, and if it's wrong, then we put the operator's operands inside parenthesis
		firstCorrectOperatorIndex = newOperatorsInfo->operatorExecutionOrder[0];
		firstOperatorIndex = 0;

		// get execution order by operator precedence
		for (DWORD i2 = 0; i2 < newOperatorsInfo->numOfOperators; i2++)
		{
			if (processedOperatorIndexes[i2] == -1)
			{
				firstOperatorIndex = i2;
				break;
			}
		}

		for (DWORD i = 0; i < newOperatorsInfo->numOfOperators; i++)
		{
			// continue if the operator has been processed already or if the other operator is the same as the first operator we're processing
			if (processedOperatorIndexes[i] != -1 || i == firstOperatorIndex)
				continue;

			// the lower the precedence, the more its preferred to be executed first
			if (GetOperatorPrecedenceLevel(newOperatorsInfo->operatorList[i]) < GetOperatorPrecedenceLevel(newOperatorsInfo->operatorList[firstOperatorIndex]))
				firstOperatorIndex = i;
		}

		// convert the operation to a string
		char* newOperation = MallocAndSprintf(
			"%s%s%s",
			newOperatorsInfo->operandList[firstCorrectOperatorIndex],
			GetStringForOperator(newOperatorsInfo->operatorList[firstCorrectOperatorIndex]),
			newOperatorsInfo->operandList[firstCorrectOperatorIndex + 1]
		);

		// free both operands
		free(newOperatorsInfo->operandList[firstCorrectOperatorIndex]);
		free(newOperatorsInfo->operandList[firstCorrectOperatorIndex + 1]);

		// we put the string inside a parenthesis if it's not executed first by operator precedence
		if (firstOperatorIndex != firstCorrectOperatorIndex)
		{
			char* oldOperation = newOperation;
			newOperation = MallocAndSprintf("(%s)", oldOperation);

			free(oldOperation);
		}

		newOperatorsInfo->operandList[firstCorrectOperatorIndex] = newOperation;

		// since there's a space left now between the newly created operand and the next operand (and a space between operators)
		// we must shift to the left everything that is after the new operand
		for (DWORD i = firstCorrectOperatorIndex + 2; i < newOperatorsInfo->numOfOperands; i++)
			newOperatorsInfo->operandList[i - 1] = newOperatorsInfo->operandList[i];
		for (DWORD i = firstCorrectOperatorIndex + 1; i < newOperatorsInfo->numOfOperators; i++)
			newOperatorsInfo->operatorList[i - 1] = newOperatorsInfo->operatorList[i];

		// same goes for the operator execution order
		for (DWORD i = 1; i < newOperatorsInfo->numOfOperators; i++)
		{
			newOperatorsInfo->operatorExecutionOrder[i - 1] = newOperatorsInfo->operatorExecutionOrder[i];

			if (newOperatorsInfo->operatorExecutionOrder[i - 1] > firstCorrectOperatorIndex)
				newOperatorsInfo->operatorExecutionOrder[i - 1]--;
		}

		newOperatorsInfo->numOfOperands--;
		newOperatorsInfo->numOfOperators--;
	}

	/*cout << "operands: " << endl;
	for (DWORD i = 0; i < operatorsInfo->numOfOperands; i++)
		cout << operatorsInfo->operandList[i] << endl;

	cout << "operators: " << endl;
	for (DWORD i = 0; i < operatorsInfo->numOfOperators; i++)
		cout << operatorsInfo->operatorList[i] << endl;

	cout << "execution order: " << endl;
	for (DWORD i = 0; i < operatorsInfo->numOfOperators; i++)
		cout << operatorsInfo->operatorExecutionOrder[i] << endl;

	cout << endl;
	cin.get();*/

	// best part now, we create the operation string now (there's 1 operator left only)
	char* OperationString;
	OperationString = MallocAndSprintf("%s%s%s", newOperatorsInfo->operandList[0], GetStringForOperator(newOperatorsInfo->operatorList[0]), newOperatorsInfo->operandList[1]);

	(stack.currentVar - index)->value = (DWORD)OperationString;

	// free the stuff we've malloc'd
	free(processedOperatorIndexes);
	
	// free the OperatorsInfo members (code from StackPop)
	for (DWORD i = 0; i < newOperatorsInfo->numOfOperands; i++)
		free(newOperatorsInfo->operandList[i]);
	free(newOperatorsInfo->operandList);
	
	free(newOperatorsInfo->operatorList);
	free(newOperatorsInfo->operatorExecutionOrder);
	
	// free the OperatorsInfo struct
	free(newOperatorsInfo);
}

// can test with wait_for_buffer_time_to_pass from _utility.gsc
void operator_decompile(OperatorType operatorType)
{
	if (StackGetValueType(1) != type_decompiled_string && StackGetValueType(1) != type_buildable_operation)
	{
		cout << "Error: first operator isn't of type type_decompiled_string or type_buildable_operation" << endl;
		cin.get();
		ExitProcess(-1);
	}
	else if (StackGetValueType(0) != type_decompiled_string && StackGetValueType(0) != type_buildable_operation)
	{
		cout << "Error: second operator of isn't of type type_decompiled_string or type_buildable_operation" << endl;
		cin.get();
		ExitProcess(-1);
	}
	
	OperatorsInfo* operatorsInfo = (OperatorsInfo*)malloc(sizeof(OperatorsInfo));
	memset(operatorsInfo, 0, sizeof(OperatorsInfo));

	// create a new OperatorsInfo struct and push it into the stack as type_unbuilt_operation if both last values are of type type_decompiled_string
	if (StackGetValueType(1) == type_decompiled_string && StackGetValueType(0) == type_decompiled_string)
	{
		operatorsInfo->numOfOperands = 2;
		operatorsInfo->operandList = (char**)malloc(sizeof(char*) * operatorsInfo->numOfOperands);
		operatorsInfo->operandList[0] = (char*)malloc(strlen((char*)StackGetValue(1)) + 1);
		operatorsInfo->operandList[1] = (char*)malloc(strlen((char*)StackGetValue(0)) + 1);
		strcpy_s(operatorsInfo->operandList[0], strlen((char*)StackGetValue(1)) + 1, (char*)StackGetValue(1));
		strcpy_s(operatorsInfo->operandList[1], strlen((char*)StackGetValue(0)) + 1, (char*)StackGetValue(0));

		operatorsInfo->numOfOperators = 1;
		operatorsInfo->operatorList = (OperatorType*)malloc(sizeof(OperatorType) * operatorsInfo->numOfOperators);
		operatorsInfo->operatorList[0] = operatorType;

		operatorsInfo->operatorExecutionOrder = (DWORD*)malloc(sizeof(DWORD) * operatorsInfo->numOfOperators);
		operatorsInfo->operatorExecutionOrder[0] = 0; // means 1st operation to be executed
	}
	// if one of the 2 last values is of type_unbuilt_operation but the other one is type_decompiled_string
	else if ((StackGetValueType(1) == type_decompiled_string || StackGetValueType(0) == type_decompiled_string) && (StackGetValueType(1) == type_buildable_operation || StackGetValueType(0) == type_buildable_operation))
	{
		OperatorsInfo* oldOperatorsInfo;
		char* operand; // the other operand which is of type type_decompiled_string
		bool isLeftOperand; // true if the operand of type type_decompiled_string is written from the left of the operator

		if (StackGetValueType(1) == type_buildable_operation)
		{
			oldOperatorsInfo = StackGetOperatorsInfo(1);
			operand = (char*)StackGetValue(0);
			isLeftOperand = false;
		}
		else
		{
			oldOperatorsInfo = StackGetOperatorsInfo(0);
			operand = (char*)StackGetValue(1);
			isLeftOperand = true;
		}

		operatorsInfo->numOfOperands = oldOperatorsInfo->numOfOperands + 1;
		operatorsInfo->operandList = (char**)malloc(sizeof(char*) * operatorsInfo->numOfOperands);
		for (DWORD i = 0; i < oldOperatorsInfo->numOfOperands; i++) // copy the operands from the oldOperatorsInfo, leave a space at the start if the type_decompiled_string operand is to the left of the operator
		{
			operatorsInfo->operandList[isLeftOperand ? i + 1 : i] = (char*)malloc(strlen(oldOperatorsInfo->operandList[i]) + 1);
			strcpy_s(operatorsInfo->operandList[isLeftOperand ? i + 1 : i], strlen(oldOperatorsInfo->operandList[i]) + 1, oldOperatorsInfo->operandList[i]);
		}
		// copy the new operand to the left or right
		operatorsInfo->operandList[isLeftOperand ? 0 : operatorsInfo->numOfOperands - 1] = (char*)malloc(strlen(operand) + 1);
		strcpy_s(operatorsInfo->operandList[isLeftOperand ? 0 : operatorsInfo->numOfOperands - 1], strlen(operand) + 1, operand);

		operatorsInfo->numOfOperators = oldOperatorsInfo->numOfOperators + 1;
		operatorsInfo->operatorList = (OperatorType*)malloc(sizeof(OperatorType) * operatorsInfo->numOfOperators);
		// copy the operator types from the oldOperatorsInfo
		memcpy(
			isLeftOperand ? operatorsInfo->operatorList + 1 : operatorsInfo->operatorList,
			oldOperatorsInfo->operatorList,
			sizeof(OperatorType) * oldOperatorsInfo->numOfOperators
			);
		// copy the new operator to the left or right
		operatorsInfo->operatorList[isLeftOperand ? 0 : operatorsInfo->numOfOperators - 1] = operatorType;

		operatorsInfo->operatorExecutionOrder = (DWORD*)malloc(sizeof(DWORD) * operatorsInfo->numOfOperators);

		memcpy(
			isLeftOperand ? operatorsInfo->operatorExecutionOrder + 1 : operatorsInfo->operatorExecutionOrder,
			oldOperatorsInfo->operatorExecutionOrder,
			sizeof(DWORD) * oldOperatorsInfo->numOfOperators
			);
		operatorsInfo->operatorExecutionOrder[isLeftOperand ? 0 : operatorsInfo->numOfOperators - 1] = operatorsInfo->numOfOperators - 1;
	}
	// if both last values are of type type_unbuilt_operation, then we merge them
	else
	{
		// get the left & right operators info (like in "v1 * v2 + v3 * v4" when operatorType is "+", left operator info would represent "v1 * v2" and right "v3 * v4")
		OperatorsInfo* leftOldOperatorsInfo = StackGetOperatorsInfo(1);
		OperatorsInfo* rightOldOperatorsInfo = StackGetOperatorsInfo(0);

		// calculate the number of operands and allocate the list respectively
		operatorsInfo->numOfOperands = leftOldOperatorsInfo->numOfOperands + rightOldOperatorsInfo->numOfOperands;
		operatorsInfo->operandList = (char**)malloc(sizeof(char*) * operatorsInfo->numOfOperands);

		// copy the left & right operands to the operand list (basically combine them, we don't add any operand)
		for (DWORD i = 0; i < leftOldOperatorsInfo->numOfOperands; i++)
		{
			operatorsInfo->operandList[i] = (char*)malloc(strlen(leftOldOperatorsInfo->operandList[i]) + 1);
			strcpy_s(operatorsInfo->operandList[i], strlen(leftOldOperatorsInfo->operandList[i]) + 1, leftOldOperatorsInfo->operandList[i]);
		}
		for (DWORD i = 0; i < rightOldOperatorsInfo->numOfOperands; i++)
		{
			operatorsInfo->operandList[leftOldOperatorsInfo->numOfOperands + i] = (char*)malloc(strlen(rightOldOperatorsInfo->operandList[i]) + 1);
			strcpy_s(operatorsInfo->operandList[rightOldOperatorsInfo->numOfOperands + i], strlen(rightOldOperatorsInfo->operandList[i]) + 1, rightOldOperatorsInfo->operandList[i]);
		}

		// calculate the number of operators and allocate the list respectively
		operatorsInfo->numOfOperators = leftOldOperatorsInfo->numOfOperators + 1 + rightOldOperatorsInfo->numOfOperators; // we insert 1 operator in the middle
		operatorsInfo->operatorList = (OperatorType*)malloc(sizeof(OperatorType) * operatorsInfo->numOfOperators);

		// copy the left & right operators to the operand list and insert operatorType in the middle of them
		memcpy(
			&operatorsInfo->operatorList[0],
			leftOldOperatorsInfo->operatorList,
			sizeof(OperatorType) * leftOldOperatorsInfo->numOfOperators
			);
		operatorsInfo->operatorList[leftOldOperatorsInfo->numOfOperators] = operatorType;
		memcpy(
			&operatorsInfo->operatorList[leftOldOperatorsInfo->numOfOperators + 1],
			rightOldOperatorsInfo->operatorList,
			sizeof(OperatorType) * rightOldOperatorsInfo->numOfOperators
			);

		// set the operator execution order (the operator that we're writing right now will have the latest execution order)
		operatorsInfo->operatorExecutionOrder = (DWORD*)malloc(sizeof(DWORD) * operatorsInfo->numOfOperators);

		memcpy(
			&operatorsInfo->operatorExecutionOrder[0],
			leftOldOperatorsInfo->operatorExecutionOrder,
			sizeof(DWORD) * leftOldOperatorsInfo->numOfOperators
			);
		operatorsInfo->operatorExecutionOrder[leftOldOperatorsInfo->numOfOperators] = operatorsInfo->numOfOperators - 1; // latest execution order is operatorsInfo->numOfOperators - 1
		memcpy(
			&operatorsInfo->operatorExecutionOrder[leftOldOperatorsInfo->numOfOperators + 1],
			rightOldOperatorsInfo->operatorExecutionOrder,
			sizeof(DWORD) * rightOldOperatorsInfo->numOfOperators
			);
	}
	
	StackPop();
	StackPop();
	
	StackPush<int>(NULL, type_buildable_operation);
	StackSetOperatorsInfo(operatorsInfo);
}

void WriteRegisterInfo(BYTE* gscBuffer, BYTE* ip)
{
	AddString(" IP = 0x%X ; SP = 0x%X\n", false, ip - gscBuffer, StackGetRelativePos());
}

// converts, for example, "0.650000" to "0.65"
char* RemoveDecimalTrailingZeros(char* Float)
{
	int len = strlen(Float);

	if (len < 2)
		return Float;

	for (int i = len - 1; i != 0; i--)
	{
		if (Float[i] == '0' && Float[i - 1] != '.')
			Float[i] = 0x00;
		else
			break;
	}

	return Float;
}

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

	char* Float = RemoveDecimalTrailingZeros(MallocAndSprintf("%f", *(float*)(GET_ALIGNED_DWORD(currentPos))));
	
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