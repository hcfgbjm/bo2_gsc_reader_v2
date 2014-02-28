#include "stdafx.h"

char* GSCDecompilerClass::GetStringForCurrentObject()
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

	return nullptr;
}

/*void GSCDecompilerClass::jump_expression_decompile(BYTE jumpOP, DWORD jumpTo)
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
	AddString((char*)StackGetValue(0), false);
	if (includeNegation)
		AddString(")", false);

	AddString(" )\n", false); // closing if brace
	
	StackPop();

	IfJumpEndStackPos++;
	*IfJumpEndStackPos = jumpTo;

	AddString("{\n", true);

	IncTabLevel();
}*/

// ip = RELATIVE instruction pointer that points to the call opcode (for example 0x2E, which is OP_ScriptFunctionCall)
// if we find out that the call is external (by searching through externalFunctions from the gsc header), we prepend
// the right gscOfFunction to functionName
char* GSCDecompilerClass::funcname_prepend_gscOfFunction(char* functionName, DWORD ip)
{
	COD9_GSC* gsc = (COD9_GSC*)gscBuffer;

	externalFunction* currentExternalFunction = (externalFunction*)(gscBuffer + gsc->externalFunctions);
	DWORD* references = (DWORD*)(currentExternalFunction + 1);
	for (WORD i = 0; i < gsc->numOfExternalFunctions; i++)
	{
		for (WORD i2 = 0; i2 < currentExternalFunction->numOfReferences; i2++)
		{
			if (references[i2] == ip && currentExternalFunction->gscOfFunction && *(char*)(gscBuffer + currentExternalFunction->gscOfFunction))
			{
				// found the externalFunction struct for this call - we return gscOfFunction::funtionName
				char* result = MallocAndSprintf("%s::%s", gscBuffer + currentExternalFunction->gscOfFunction, functionName);

				return result;
			}
		}

		currentExternalFunction = (externalFunction*)((DWORD)currentExternalFunction + sizeof(DWORD) * currentExternalFunction->numOfReferences);
		currentExternalFunction++;

		references = (DWORD*)(currentExternalFunction + 1);
	}

	// couldn't find externalFunction struct for this call - we return functionName
	char* result = MallocAndSprintf("%s", functionName);

	return result;
}

// special call decompilation for waittill & waittillmatch
void GSCDecompilerClass::waittill_call_decompile(char* functionName, BYTE* currentPos)
{
	string _FunctionString;
	
	// append the method object
	_FunctionString.append((char*)StackGetValue(0));
	_FunctionString.append(" ");
	
	StackPop(); // pop the method object (like self, etc...) from the stack

	// write the function name and write all the parameters
	_FunctionString.append(functionName);
	_FunctionString.append("( ");
	_FunctionString.append((char*)StackGetValue(0));

	StackPop(); // pop the single waittill parameter

	for (BYTE* i = currentPos; *i == OP_SafeSetWaittillVariableFieldCached; i += 2) // += 2 because 1 is opcode size and 1 is the local var index
	{
		_FunctionString.append(", ");
		_FunctionString.append(StackLocalGetValue(*(i + 1)));
	}

	_FunctionString.append(" );\n");

	DecompilerOut((char*)_FunctionString.c_str(), true);
}

// if hasPrecodepos is true, it ignores _numOfParameters
void GSCDecompilerClass::call_decompile(char* functionName, bool hasPrecodepos, DWORD _numOfParameters, bool pointerCall, bool methodCall, bool threadCall, bool resultUnused)
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
		_FunctionString.append((char*)StackGetValue(0));
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
	
	if (pointerCall)
		_FunctionString.append("[[ ");
	_FunctionString.append(functionName);
	if (pointerCall)
		_FunctionString.append(" ]]");

	if (numOfParameters) // if the function has parameters
	{
		_FunctionString.append("( ");

		for (DWORD i = 0; i < numOfParameters; i++)
		{
			_FunctionString.append((char*)StackGetValue(0));
			StackPop();

			if ((i + 1) < numOfParameters) // execute this only if it's not the last iteration
				_FunctionString.append(", ");
		}

		_FunctionString.append(" )");
	}
	else
	{
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
		DecompilerOut("%s;\n", true, FunctionString);
}

int switchtable_sortfunc(const void* a, const void* b)
{
	// no need to align because the structure is 8 bytes itself anyway
	return (*(__int32*)a - *(__int32*)b);
}

BYTE* GSCDecompilerClass::switch_decompile(BYTE* currentPos)
{
	currentPos = GET_ALIGNED_DWORD(*(DWORD*)GET_ALIGNED_DWORD(currentPos) + GET_ALIGNED_DWORD(currentPos) + 4);

	DWORD caseCount = *(DWORD*)currentPos;

	currentPos += 4;

	BYTE* switchEndIP = GET_ALIGNED_DWORD(currentPos) + 8 * caseCount;

	if (!caseCount)
	{
		StackPop();
		return switchEndIP;
	}

	DecompilerOut("switch ( %s )\n", true, StackGetValue(0));
	DecompilerOut("{\n", true);
	IncTabLevel();

	// we generate the switch IPs to a buffer, and sort the copied switch table by IP in ascending order
	BYTE* switchIPTable = (BYTE*)malloc(4 * caseCount);
	BYTE* curSwitchIPTablePos = switchIPTable;

	for (DWORD i = 0; i < caseCount; i++)
		*(DWORD*)(switchIPTable + i * 4) = (DWORD)(currentPos + i * 8 + 8) + *(__int32*)(currentPos + i * 8 + 4) - gscBuffer;

	qsort(switchIPTable, caseCount, 4, switchtable_sortfunc);

	__int32 ipOffset = 0;
	for (DWORD i = 0; i < caseCount; i++)
	{
		// Case name/value
		BYTE* label = currentPos;
		DWORD caseLabelValue = *(DWORD*)label;

		// Case code pointer
		BYTE* ip = curSwitchIPTablePos;
		ipOffset = *(DWORD*)ip;

		currentPos += 8;

		// currentPos + ipOffset here = code ip for this case
		if (!caseLabelValue) // if caseLabelOffset is 0 this is a default case
			DecompilerOut("default:\n", true);
		else
		{
			// int case
			if (caseLabelValue & (1 << 23)) // int cases have the 24th bit set always, and i've proven that checking it is safe unless gsc strings together are > 8MB
				DecompilerOut("case %d:\n", true, caseLabelValue & ~(1 << 23));
			else // string case
				DecompilerOut("case \"%s\":\n", true, (char*)(gscBuffer + caseLabelValue));
		}

		DWORD curCaseIP = *(DWORD*)curSwitchIPTablePos; // relative instruction pointer for current case
		DWORD nextCaseIP = i + 1 == caseCount ? 0 : *(DWORD*)(curSwitchIPTablePos + 4); // relative instruction pointer for next case

		curSwitchIPTablePos += 4;

		// only decompile the code of the case if the current case differs from the next one
		if (!nextCaseIP || curCaseIP != nextCaseIP)
		{
			// find the case's size of code and detect if it has a break at the end
			DWORD caseCodeSize = 0;
			bool caseHasBreak = false;

			// read opcodes until we find OP_jump that jumps at the end of the switch (break) or until we hit the next case's IP (no break)
			for (BYTE* curCasePos = (BYTE*)gscBuffer + curCaseIP; ; caseCodeSize += gsclde(curCasePos), curCasePos += gsclde(curCasePos))
			{
				// if it's a jump, we check if it jumps at the end of the switch, if yes, then we read all the opcodes of the case and there's a break in the end
				// if it's an endswitch, then we can put or not a break, but i prefer to put it
				// if there is a next case and we reach it, we don't put a break
				if (*curCasePos == OP_jump)
				{
					BYTE* jumpsToIP = GET_ALIGNED_WORD(curCasePos + 1) + *(__int16*)(GET_ALIGNED_WORD(curCasePos + 1)) + 2;

					if (jumpsToIP == switchEndIP)
					{
						caseHasBreak = true;
						break;
					}
					else if (jumpsToIP > switchEndIP || jumpsToIP < (BYTE*)(gscBuffer + curCaseIP)) // OP_jump can't jump outside the case...
					{
						IncTabLevel();
						DecompilerOut("// OP_jump that jumps outside the case's boundaries detected\n", true);
						DecTabLevel();

						goto case_loop_continue;
					}
				}
				else if (*curCasePos == OP_endswitch)
				{
					caseHasBreak = true;
					break;
				}
				else if (nextCaseIP && curCasePos == (BYTE*)(gscBuffer + nextCaseIP))
				{
					caseHasBreak = false;
					break;
				}
			}

			if (caseCodeSize)
			{
				IncTabLevel();

				// GSCDecompilerClass must be created in a scope! (we're currently in if (caseCodeSize) scope)
				GSCDecompilerClass gscDecompiler;
				decompiledBuffer.append((char*)(gscDecompiler.decompile(&this->stack, gscBuffer, curCaseIP, caseCodeSize, false, curTabLevel)).c_str());

				if (caseHasBreak)
					DecompilerOut("break;\n", true);

				DecTabLevel();
			}
		}

case_loop_continue: ;
	}

	free(switchIPTable);

	StackPop();

	DecTabLevel();
	DecompilerOut("}\n", true);

	return switchEndIP;
}

int GSCDecompilerClass::GetOperatorPrecedenceLevel(OperatorType operatorType)
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

char* GSCDecompilerClass::GetStringForOperator(OperatorType operatorType)
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
		return " %% "; // double modulus or DecompilerOut will crash
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
void GSCDecompilerClass::build_operation(int index)
{
	OperatorsInfo* operatorsInfo = StackGetOperatorsInfo(index);

	if (operatorsInfo->numOfOperators < 1)
	{
		cout << "Error: build_operation requested an operation to be built, but it has no operators" << endl;
		cin.get();
		ExitProcess(-1);
	}

	#ifdef _DEBUG
	OutputDebugStringA("\n\nbuild_operation struct dump:\n");

	char dbgOutputBuf[1024];
	OutputDebugStringA("- operands:\n");
	for (DWORD i = 0; i < operatorsInfo->numOfOperands; i++)
	{
		sprintf_s(dbgOutputBuf, "%s\n", operatorsInfo->operandList[i]);
		OutputDebugStringA(dbgOutputBuf);
	}

	OutputDebugStringA("- operators:\n");
	for (DWORD i = 0; i < operatorsInfo->numOfOperators; i++)
	{
		sprintf_s(dbgOutputBuf, "%d\n", operatorsInfo->operatorList[i]);
		OutputDebugStringA(dbgOutputBuf);
	}

	OutputDebugStringA("- execution order:\n");
	for (DWORD i = 0; i < operatorsInfo->numOfOperators; i++)
	{
		sprintf_s(dbgOutputBuf, "%d\n", operatorsInfo->operatorExecutionOrder[i]);
		OutputDebugStringA(dbgOutputBuf);
	}

	OutputDebugStringA("Dump finished.\n");
	#endif /* _DEBUG */

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
		// we loop each operator, get it's execution order by operator precedence, and if it's wrong, then we put the operator's operands inside parenthesis
		firstCorrectOperatorIndex = newOperatorsInfo->operatorExecutionOrder[0];
		firstOperatorIndex = 0;

		for (DWORD i = 1; i < newOperatorsInfo->numOfOperators; i++)
		{
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

	// best part now, we create the operation string (there's 1 operator left only)
	char* OperationString;
	OperationString = MallocAndSprintf("%s%s%s", newOperatorsInfo->operandList[0], GetStringForOperator(newOperatorsInfo->operatorList[0]), newOperatorsInfo->operandList[1]);

	// no need to check if value is allocated because StackGetValue does that for us already
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
void GSCDecompilerClass::operator_decompile(OperatorType operatorType)
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
	// if one of the 2 last values is of type_buildable_operation but the other one is type_decompiled_string
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
	// if both last values are of type type_buildable_operation, then we merge them
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
			strcpy_s(operatorsInfo->operandList[leftOldOperatorsInfo->numOfOperands + i], strlen(rightOldOperatorsInfo->operandList[i]) + 1, rightOldOperatorsInfo->operandList[i]);
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

		// we have to manually set the right part of the operation's execution order
		for (DWORD i = 0; i < rightOldOperatorsInfo->numOfOperators; i++)
		{
			operatorsInfo->operatorExecutionOrder[leftOldOperatorsInfo->numOfOperators + 1 + i] =
				rightOldOperatorsInfo->operatorExecutionOrder[i] + leftOldOperatorsInfo->numOfOperators;
		}
	}

	#ifdef _DEBUG
	OutputDebugStringA("\n\noperator_decompile struct dump:\n");

	char dbgOutputBuf[1024];
	OutputDebugStringA("- operands:\n");
	for (DWORD i = 0; i < operatorsInfo->numOfOperands; i++)
	{
		sprintf_s(dbgOutputBuf, "%s\n", operatorsInfo->operandList[i]);
		OutputDebugStringA(dbgOutputBuf);
	}

	OutputDebugStringA("- operators:\n");
	for (DWORD i = 0; i < operatorsInfo->numOfOperators; i++)
	{
		sprintf_s(dbgOutputBuf, "%d\n", operatorsInfo->operatorList[i]);
		OutputDebugStringA(dbgOutputBuf);
	}

	OutputDebugStringA("- execution order:\n");
	for (DWORD i = 0; i < operatorsInfo->numOfOperators; i++)
	{
		sprintf_s(dbgOutputBuf, "%d\n", operatorsInfo->operatorExecutionOrder[i]);
		OutputDebugStringA(dbgOutputBuf);
	}

	OutputDebugStringA("Dump finished.\n");
	#endif /* _DEBUG */
	
	StackPop();
	StackPop();
	
	StackPush<int>(NULL, type_buildable_operation);
	StackSetOperatorsInfo(operatorsInfo);
}

void GSCDecompilerClass::WriteRegisterInfo(BYTE* ip)
{
	DecompilerOut(" IP = 0x%X ; SP = 0x%X\n", false, ip - gscBuffer, StackGetRelativePos());
}