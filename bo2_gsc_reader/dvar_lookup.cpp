#include "stdafx.h"

struct DvarHashEntry
{
	char	m_Name[128];
	int		m_Hash;
};

char *DvarFileMemory;

DvarHashEntry	*DvarEntries;
int				DvarEntryCount;

// File format:
// name<newline>
// name<newline>
// etc...
bool InitDvarTable(const char *File)
{
	FILE *dvarList = nullptr;

	// Open file
	fopen_s(&dvarList, File, "r");

	// Check for file opening errors, if there were any, exit
	if (!dvarList)
		return false;

	// Get the input file size
	fseek(dvarList, 0, SEEK_END);
	long fileSize = ftell(dvarList);
	fseek(dvarList, 0, SEEK_SET);

	// Allocate memory
	DvarFileMemory = (char *)VirtualAlloc(nullptr, fileSize, MEM_COMMIT, PAGE_READWRITE);

	if (!DvarFileMemory)
		return false;

	// Read the file
	fread_s(DvarFileMemory, fileSize, 1, fileSize, dvarList);

	// Close it
	fclose(dvarList);

	return true;
}

void FreeDvarTable()
{
	if (DvarFileMemory)
		VirtualFree(DvarFileMemory, 0, MEM_RELEASE);

	if (DvarEntries)
		VirtualFree(DvarEntries, 0, MEM_RELEASE);

	DvarFileMemory	= nullptr;
	DvarEntries		= nullptr;
	DvarEntryCount	= 0;
}

bool ParseDvarTable()
{
	int lineCount = 0;

	// Count the number of lines first (then allocate N entries)
	for (char *ptr = DvarFileMemory; *ptr != '\0'; ptr++)
	{
		if (*ptr == '\n')
			lineCount++;
	}

	// Check if there are any valid lines
	if (lineCount <= 0)
		return false;

	// Allocate the entry for each
	DvarEntries		= (DvarHashEntry *)VirtualAlloc(nullptr, sizeof(DvarHashEntry) * lineCount, MEM_COMMIT, PAGE_READWRITE);
	DvarEntryCount	= lineCount;

	if (!DvarEntries)
		return false;

	// Now map each dvar
	char *lineBegin = DvarFileMemory;
	char *lineEnd	= DvarFileMemory;

	for (int i = 0; i < DvarEntryCount;)
	{
		// Ignore Window's return lines
		if (*lineEnd == '\r')
			*lineEnd++ = '\0';

		// Scanned the entire line
		if (*lineEnd == '\n')
		{
			*lineEnd++	= '\0';

			// Copy the string
			strcpy_s(DvarEntries[i].m_Name, lineBegin);

			// Calculate hash
			DvarEntries[i].m_Hash = GetDvarHash(lineBegin);

			// Set the new pointer
			lineBegin = lineEnd;

			i++;
		}

		lineEnd++;
	}

	return true;
}

const char *DvarNameForHash(int Hash)
{
	for (int i = 0; i < DvarEntryCount; i++)
	{
		if (DvarEntries[i].m_Hash == Hash)
			return DvarEntries[i].m_Name;
	}

	printf("Unknown hash value 0x%X\n", Hash);
	return nullptr;
}