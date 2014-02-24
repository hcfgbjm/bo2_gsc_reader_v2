#pragma once

// FOR BO2 (COD9) COMPILED GSCS ONLY
// AS THIS IN DEVELOPMENT, NAMES MAY BE CHANGED

// when it's finished, make better order of the structs

static BYTE COD9_GSC_IDENTIFIER[8] =
{
	0x80, 0x47, 0x53, 0x43,
	0x0D, 0x0A, 0x00, 0x06
};

/*
	All of this reference stuff needs more investigation, i'm too blind to see how is this useful currently
	See the game code to get an idea on how to use this
*/

typedef struct _includeStruct
{
	DWORD string;
} includeStruct;

typedef struct _animReference
{
	DWORD name;			// 0x00 - Name of the anim (example: veh_car_destroy from mp_vehicles)
	DWORD reference;	// 0x04 - A relative pointer to the code that references this anim
} animReference;

// scrAnimPub

typedef struct _usinganimtreeStruct
{
	WORD name;					// 0x00 - A relative pointer to the name of this animtree
	WORD numOfReferences;		// 0x02 -
	WORD numOfAnimReferences;	// 0x04 - Amount of references of anims contained in the animtree
	WORD null1;					// 0x06 - Not sure if it's always null, will check later
	/*
		NOTE: This is included in this struct, but as we can't create dynamic structs, we just create another struct which will be used dynamically in the code
		DWORD references[numOfReferences];	//	Relative pointers that point to code that references this animtree (the game writes the
												animtree index at scrAnimPub->animTreeIndex to the places where the animtree is referenced, animtree index's
												size is 4 bytes)
		animReference animReferences[numOfAnimReferences];
	*/
} usinganimtreeStruct;

// All possible types for gscString:
/*
	- For NON_CANONICAL_STRING:
		This is a string that can't be changed in any form when the script is compiled, like to upper or lower char,
		for example: "RANK_PLAYER_WAS_PROMOTED_N" from "precachestring( &"RANK_PLAYER_WAS_PROMOTED_N" );"
	
	- For CANONICAL_STRING:
		This is a string that is converted to lower char when the script is compiled, for example "scoreinfo" from "level.scoreinfo"
*/
#define NON_CANONICAL_STRING	0
#define CANONICAL_STRING		1

// This struct is used to get information about a string in the gsc and allocate it properly in the game's memory
typedef struct _gscString
{
	WORD string;			// 0x00 - A relative pointer to the string
	BYTE numOfReferences;	// 0x02 - Amount of references to this string
	BYTE type;				// 0x03 - Can only be 0 or 1
	/*
		NOTE: This is included in this struct, but as we can't create dynamic structs, we just create another struct which will be used dynamically in the code
		DWORD references[numOfReferences];	//	Relative pointers that point to code that references this string (the game writes a
												2 byte WORD to the places where the string is referenced)
	*/
} gscString;

typedef struct _gscFunction
{
	DWORD crc32;			// 0x00 - CRC32 of function bytecode
	DWORD start;			// 0x04 - A relative pointer to the GSC bytecode start of this function
	WORD name;				// 0x08 - A relative pointer to the name of this function
	BYTE numOfParameters;	// 0x0A -
	BYTE flag;				// 0x0B -
	/*
		Possible flag values (not sure if all of them):

		- If flag & 1, function loaded (I think?)
	*/
} gscFunction;

// All possible flags for externalFunction:
/*
	- For all flags:
		The game does it's stuff to find the function, and when it finds it, it writes the correct function type opcode, the
		correct number of parameters and the function's address to the places where the function is referenced (its referenced in code always)
		The offsets at which it writes the opcode and the number of parameters are: reference + 0x00 for opcode and reference + 0x01 for number of parameters
	
	- For FLAG_1:
		Does the same as FLAG_2 but doesn't write the opcode or the number of parameters (it only writes the function's address to the places
		where the function is referenced)

	- For FLAG_2:
		The game tries to find the function in the game's executable, and then in loaded GSCs (skips to loaded GSCs finding if it has gscOfFunction)
		-	If it finds the function in the game's executable, it writes opcode 0x29 (OP_CallBuiltin) to the places where the function is referenced
		-	If it finds the function in a loaded GSC, it writes opcode 0x2E (OP_ScriptFunctionCall) to the places where the function is referenced
		-	See FLAG_2.txt for list of prefefined functions for this flag

	- For FLAG_3:
		The game tries to find the function in loaded GSCs
		-	If it finds the function in a loaded GSC, it writes opcode 0x32 (OP_ScriptThreadCall) to the places where the function is referenced

	- For FLAG_4:
		Does the same as FLAG_2 but it uses another predefined function list and writes opcode 0x2A (OP_CallBuiltinMethod) instead
		-	See FLAG_4.txt for list of prefefined functions for this flag
		
	- For FLAG_5:
		The game tries to find the function in loaded GSCs
		-	If it finds the function in a loaded GSC, it writes opcode 0x34 (OP_ScriptMethodThreadCall) to the places where the function is referenced
*/
#define FLAG_1	1
#define FLAG_2	2
#define FLAG_3	3
#define FLAG_4	4
#define FLAG_5	5

// IMPORTANT: I MUST WRITE AN EXTERNAL RESOLVER SO I CAN READ NUMBER OF PARAMETERS FROM THE CALL
// functions imported by the gsc (not the functions in the gsc)
typedef struct _externalFunction
{
	WORD name;				// 0x00 - A relative pointer to the name of this function
	WORD gscOfFunction;		// 0x02 - GSC of the function, example: maps/mp/gametypes/_hud::fontpulse (gscOfFunction::name) - WARNING: it can be 0x00, and that
							//		  may mean that IT'S NOT ACTUALLY EXTERNAL (so this struct may be renamed to something like gscFunctionResolver, but meh)
	WORD numOfReferences;	// 0x04 -
	BYTE numOfParameters;	// 0x06 - Function parameter count
	BYTE flag;				// 0x07 - 
	/*
		Possible flag values (not sure if all of them):

		- The game makes a switch ((flag & 15) - 1) // this strips the byte in half and then substracts 1 from the second half
		// so if you have 0x12, it will take 2 - 1 = 1
		// the switch only accepts values from 0 to 4 (else you will get an unresolved external error)

		- If flag & 16, only dev call
	*/
	/*
		NOTE: This is included in this struct, but as we can't create dynamic structs, we just create another struct which will be used dynamically in the code
		DWORD references[numOfReferences]; // Relative pointers that point to code that references this external/imported function
	*/
} externalFunction;

// Replaces bytes in a GSC file at load time
typedef struct _gscRelocation
{
	DWORD value;	// 0x00 - Value of bytes to be inserted
	DWORD offset;	// 0x04 - File offset (where to replace the bytes)
} gscRelocation;

// RVA
// means that you need to make *gsc_allocated_ptr* + THIS_COD9_GSC->...
// example: char* gsc_name = (char*)((DWORD)THIS_COD9_GSC + THIS_COD9_GSC->name)
typedef struct _COD9_GSC
{
	BYTE identifier[8];			// 0x00 - It's the same always
	BYTE unknown1[4];			// 0x08 - I think it's some kind of checksum
	DWORD includeStructs;		// 0x0C - A relative pointer to an array of includeStruct structs, amount = numOfIncludes
	DWORD usinganimtreeStructs; // 0x10 - A relative pointer to an array of usinganimtreeStruct structs, amount = numOfUsinganimtree
	DWORD codeSectionStart;		// 0x14 - A relative pointer to where the GSC bytecode starts (gscFunctions)
	DWORD gscStrings;			// 0x18 - A relative pointer to an array of gscString structs, amount = numOfStrings
	DWORD gscFunctions;			// 0x1C - A relative pointer to an array of gscFunction structs, amount = numOfFunctions
	DWORD externalFunctions;	// 0x20 - A relative pointer to an array of externalFunction structs, amount = numOfExternalFunctions
	DWORD gscRelocations;		// 0x24 - A relative pointer to an array of gscRelocation structs, amount = numOfRelocations
	DWORD size;					// 0x28 -
	DWORD codeSectionSize;		// 0x2C - Size of the executable bytecode
	WORD name;					// 0x30 - A relative pointer to the name of this gsc (with full path and extension, null terminated)
	WORD numOfStrings;			// 0x32 - IMPORTANT: Includes everything except #include strings, #using_animtree strings and function name strings
	WORD numOfFunctions;		// 0x34 - Number of functions in this script file
	WORD numOfExternalFunctions;// 0x36 - Number of functions imported
	WORD numOfRelocations;		// 0x38 - Number of patches in the GSC code/file
	BYTE unknown4[2];			// 0x3A - ?
	BYTE numOfIncludes;			// 0x3C - Number of included source files
	BYTE numOfUsinganimtree;	// 0x3D - Number of included animation trees
	BYTE _padding[2];			// 0x3E - Struct padding
} COD9_GSC; // size: 64 bytes