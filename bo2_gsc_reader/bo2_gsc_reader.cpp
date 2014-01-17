#include "stdafx.h"
#include "bo2_gsc_file.h"

using namespace std;

BYTE gscBuffer[0x00400000]; // 4mb - I guess it's more than enoguh for allocating compiled gsc sripts

ofstream decompiledFile;

DWORD tabLevel;

void AddString(char* format, bool addTabLevel, ...)
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
		for (DWORD i = 0; i < tabLevel; i++)
			decompiledFile << "\t";
	}

	decompiledFile << allocatedString;

	free(allocatedString);
}

void IncTabLevel()
{
	tabLevel++;
}

void DecTabLevel()
{
	if (tabLevel)
		tabLevel--;
}

void ResetTabLevel()
{
	tabLevel = 0;
}

DWORD trimCharArray(char* charArray, DWORD size)
{
	char* trimmedPtr = charArray;
	DWORD trimmedSize = size;
	for (char* i = charArray; i < charArray + size; i++)
	{
		if ((*i == 0x20) || (*i == 0x0A) || (*i == 0x09))
		{
			*i = 0x00;
			trimmedSize--;
		}
	}
	for (DWORD i = 0; i < trimmedSize; i++)
	{
		if (!*trimmedPtr)
			for (; !*trimmedPtr && trimmedPtr < charArray + size; trimmedPtr++);
		*(charArray + i) = *trimmedPtr;
		trimmedPtr++;
	}
	memset(charArray + trimmedSize, 0x00, size - trimmedSize);
	return trimmedSize;
}

void strrep(char* charArray, char charToReplace, char replaceWith, char* charArrayOut)
{
	while(*charArray)
	{
		if(*charArray == charToReplace)
			*charArrayOut = replaceWith;
		else
			*charArrayOut = *charArray;

		charArray++;
		charArrayOut++;
	}

	//null terminator
	*charArrayOut = '\0';
}

void FixSlashes(char *szInput, char *szOutput)
{
	strrep(szInput, '/', '\\', szOutput);
}

bool wcsreplace(wchar_t *szInput, wchar_t *szFind, wchar_t *szReplace)
{
	wchar_t *loc = wcsstr(szInput, szFind);

	if(!loc)
		return false;

	//since strcpy screws up for some reason
	while(*loc && *szReplace)
	{
		*loc = *szReplace;

		loc++;
		szReplace++;
	}

	*loc = '\0';
	return true;
}

void LoadGSC()
{
	COD9_GSC* gsc = (COD9_GSC*)gscBuffer;
	
	externalFunction* currentExternalFunction = (externalFunction*)(gscBuffer + gsc->externalFunctions);
	for (WORD i = 0; i < gsc->numOfExternalFunctions; i++)
	{
		DWORD* references = (DWORD*)(currentExternalFunction + 1);
		for (WORD i2 = 0; i2 < currentExternalFunction->numOfReferences; i2++)
		{
			switch (currentExternalFunction->flag)
			{
			case FLAG_1:
				cout << "OMG FLAG_1" << endl;
				cin.get();
				break;
			case FLAG_2:
				break;
			case FLAG_3:
				break;
			case FLAG_4:
				break;
			case FLAG_5:
				break;
			}
			references++;
		}

		currentExternalFunction = (externalFunction*)((DWORD)currentExternalFunction + sizeof(externalFunction) + sizeof(DWORD) * currentExternalFunction->numOfReferences);
	}
}

// This code is found in Dvar_RegisterNew ("Can't create dvar" string)
// char is supposed to be 1 byte long here
DWORD GetDvarHash(char* dvar)
{
	char* v8 = dvar;

	if (dvar)
	{
		char v26 = *dvar;
		DWORD v25 = 5381;

		if ( *dvar )
		{
			do
			{
				v8++;
				v25 = 33 * v25 + tolower(v26);
				v26 = *v8;
			}
			while ( *v8 );
		}

		return v25;
	}
	else
		return 0;
}

void DecompileGSC()
{
	COD9_GSC* gsc = (COD9_GSC*)gscBuffer;
	char fixedPath[MAX_PATH];

	// write the full path of the gsc as comment in the output
	AddString("// ", false);
	AddString((char*)(gscBuffer + gsc->name), false);
	AddString("\n\n", false);

	/*// dvar hash test decompiling
	AddString("// %s 0x%X\n\n", false, "scr_rankXpCap", GetDvarHash("scr_rankXpCap"));

	// external function test decompiling
	externalFunction* currentExternalFunction = (externalFunction*)(gscBuffer + gsc->externalFunctions);
	for (WORD i = 0; i < gsc->numOfExternalFunctions; i++)
	{
		AddString("// Flag: 0x%X ", false, currentExternalFunction->flag);
		if (*(char*)(gscBuffer + currentExternalFunction->gscOfFunction))
			AddString("%s::", false, gscBuffer + currentExternalFunction->gscOfFunction);
		AddString("%s", false, gscBuffer + currentExternalFunction->name);

		DWORD* references = (DWORD*)(currentExternalFunction + 1);
		for (WORD i2 = 0; i2 < currentExternalFunction->numOfReferences; i2++)
		{
			AddString(" 0x%X", false, *references);
			references++;
		}

		AddString("\n", false);

		currentExternalFunction = (externalFunction*)((DWORD)currentExternalFunction + sizeof(externalFunction) + sizeof(DWORD) * currentExternalFunction->numOfReferences);
	}
	AddString("// 0x%X\n", false, (DWORD)currentExternalFunction - (DWORD)gscBuffer);
	AddString("\n", false);

	// gsc strings test decompiling
	gscString* currentString = (gscString*)(gscBuffer + gsc->gscStrings);
	for (WORD i = 0; i < gsc->numOfStrings; i++)
	{
		AddString("// ", false);
		currentString->type ? AddString("Canonical string", false) : AddString("Non-canonical string", false);
		AddString(" \"%s\"", false, gscBuffer + currentString->string);

		DWORD* references = (DWORD*)(currentString + 1);
		for (WORD i2 = 0; i2 < currentString->numOfReferences; i2++)
		{
			AddString(" 0x%X", false, *references);
			references++;
		}

		AddString("\n", false);

		currentString = (gscString*)((DWORD)currentString + sizeof(gscString) + sizeof(DWORD) * currentString->numOfReferences);
	}
	AddString("\n", false);*/

	// #include decompiling
	includeStruct* currentInclude = (includeStruct*)(gscBuffer + gsc->includeStructs);
	for (BYTE i = 0; i < gsc->numOfIncludes; i++)
	{
		AddString("#include ", false);
		memset(fixedPath, 0x00, MAX_PATH);
		//NOTE @Nukem - is this needed? Generally the files are forward slashes anyways
		//NOTE @kokole - bo1 gsc had \ and the strings in compiled gsc have / so yea
		//however the CoD compiler can parse them either way
		FixSlashes((char*)(gscBuffer + currentInclude->string), fixedPath);
		AddString(fixedPath, false);
		AddString(";\n", false);
		currentInclude++;
	}
	if (gsc->numOfIncludes) // add 2 newlines if there were any includes
		AddString("\n", false);
	if (!gsc->numOfUsinganimtree)
		AddString("\n", false);

	// #using_animtree decompiling - NOT FULLY TESTED, I NEED A GSC WITH MORE THAN 1 #using_animtree
	usinganimtreeStruct* currentUsinganimtree = (usinganimtreeStruct*)(gscBuffer + gsc->usinganimtreeStructs);
	for (BYTE i = 0; i < gsc->numOfUsinganimtree; i++)
	{
		AddString("#using_animtree ( \"", false);
		memset(fixedPath, 0x00, MAX_PATH);
		FixSlashes((char*)(gscBuffer + currentUsinganimtree->name), fixedPath);
		AddString(fixedPath, false);
		AddString("\" );\n", false);
		currentUsinganimtree = (usinganimtreeStruct*)((DWORD)currentUsinganimtree + sizeof(DWORD) * currentUsinganimtree->numOfReferences);
		currentUsinganimtree = (usinganimtreeStruct*)((DWORD)currentUsinganimtree + sizeof(animReference) * currentUsinganimtree->numOfAnimReferences);
	}
	if (gsc->numOfUsinganimtree) // add 2 newlines if there were any using_animtree
		AddString("\n\n", false);

	// function decompiling
	gscFunction* currentFunction = (gscFunction*)(gscBuffer + gsc->gscFunctions);
	for (WORD i = 0; i < gsc->numOfFunctions; i++)
	{
		InterpretFunction((DWORD)gscBuffer, currentFunction); 
		if (i + 1 != gsc->numOfFunctions) // check if it's not the last the loop and add 2 newlines
			AddString("\n\n", false);
		currentFunction++;
	}
}

int wmain(int argc, wchar_t *argv[])
{
	// add error checks

	FILE *gscFile = NULL;
	errno_t _errno_ = 0; 
	LONG fileSize = 0L;

	wchar_t szFileName[MAX_PATH];
	wchar_t szOutFileName[MAX_PATH];

	memset(szFileName, 0, sizeof(szFileName));
	memset(szOutFileName, 0, sizeof(szOutFileName));

	//if a command line parameter was specified
	if(argc > 1)
	{
		wcscpy_s(szFileName, argv[1]);
		wcscpy_s(szOutFileName, argv[1]);
	}
	else
	{
		OPENFILENAMEW of;
		memset(&of, 0, sizeof(OPENFILENAMEW));

		of.lStructSize	= sizeof(of);
		of.hwndOwner	= NULL;
		of.lpstrFilter	= L"GameScript files (*.gsc)\0*.gsc\0";
		of.lpstrFile	= szFileName;
		of.nMaxFile		= ARRAYSIZE(szFileName);
		of.Flags		= OFN_EXPLORER | OFN_FILEMUSTEXIST;

		if(!GetOpenFileNameW(&of))
			return 1;

		//szFileName was already copied with the function call
		wcscpy_s(szOutFileName, szFileName);
	}

	//rewrite the new extension
	if(!wcsreplace(szOutFileName, L".gsc", L".txt"))
		return 1;

	//open file
	_errno_ = _wfopen_s(&gscFile, szFileName, L"rb");

	// check for file opening errors, if there were any, exit
	if (_errno_)
		return 1;

	// get the input filesize
	fseek(gscFile, 0, SEEK_END);
	fileSize = ftell(gscFile);
	fseek(gscFile, 0, SEEK_SET);

	// read the file
	fread_s(gscBuffer, sizeof(gscBuffer), 1, fileSize, gscFile);

	// close the file
	fclose(gscFile);

	// create output file
	decompiledFile.open(szOutFileName);

	// load the gsc (external functions resolving, includes, etc...)
	//LoadGSC();

	// decompile the gsc
	DecompileGSC();

	// close the output file
	decompiledFile.close();

	return 0;
}