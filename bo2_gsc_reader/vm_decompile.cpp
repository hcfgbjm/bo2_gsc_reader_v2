#include "stdafx.h"

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

	return nullptr;
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